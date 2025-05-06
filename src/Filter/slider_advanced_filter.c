/**
 * @file   slider_advanced_filter.c
 * @brief  滑条高级滤波器 - 结合 SPIIR 和中值滤波
 *
 * 原始位置 raw: 0-100 或 TOUCH_OFF_VAL；
 * 输出位置 0-100，或 TOUCH_OFF_VAL 表示 OFF
 * 
 * SPIIR 滤波器公式: y[n] = a*y[n-1] + b*x[n]
 * 其中 a + b = 1.0，使用定点数表示 (Q14 格式)
 */

#include "slider_advanced_filter.h"
#include <string.h>

/* 常量定义 */
#define TOUCH_OFF_VAL       0xFFFFU    /* 无触摸标志 */
#define SPIIR_DECIMAL_POINT 14         /* 定点数小数位数 */
#define DEFAULT_OFF_COUNT   3U         /* 默认连续 OFF 帧阈值 */

/* 默认系数 (a = 0.94, b = 0.06) */
#define DEFAULT_COEF_A      15401      /* 0.94 * 2^14 */
#define DEFAULT_COEF_B      983        /* 0.06 * 2^14 */

/* 自适应系数相关 */
#define SPEED_THRESHOLD_LOW  2         /* 低速阈值 */
#define SPEED_THRESHOLD_HIGH 10        /* 高速阈值 */
#define COEF_A_SLOW         15729      /* 慢速时的 a 系数 (0.96) */
#define COEF_B_SLOW         655        /* 慢速时的 b 系数 (0.04) */
#define COEF_A_FAST         14090      /* 快速时的 a 系数 (0.86) */
#define COEF_B_FAST         2294       /* 快速时的 b 系数 (0.14) */

/* 内部函数声明 */
static void median_sort(uint16_t *data, uint8_t size);
static uint16_t median_filter(slider_filter_ctrl_t *p_ctrl, uint16_t raw);
static uint16_t spiir_filter(slider_filter_ctrl_t *p_ctrl, uint16_t raw);
static void update_adaptive_coef(slider_filter_ctrl_t *p_ctrl, uint16_t raw);

/**
 * @brief 初始化滑条滤波器
 * @param p_ctrl 滤波器控制块
 * @param p_cfg 滤波器配置
 * @return 0: 成功, 非0: 错误码
 */
int slider_filter_init(slider_filter_ctrl_t *p_ctrl, const slider_filter_cfg_t *p_cfg)
{
    if (p_ctrl == NULL || p_cfg == NULL)
    {
        return -1;
    }
    
    /* 检查参数有效性 */
    if (p_cfg->median_size != 0 && 
        p_cfg->median_size != 3 && 
        p_cfg->median_size != 5 && 
        p_cfg->median_size != 7 && 
        p_cfg->median_size != 9)
    {
        return -2;
    }
    
    /* 初始化控制块 */
    p_ctrl->type = p_cfg->type;
    p_ctrl->count = 0;
    p_ctrl->settlings = p_cfg->settlings;
    p_ctrl->a_coef = p_cfg->a_coef;
    p_ctrl->b_coef = p_cfg->b_coef;
    p_ctrl->last_pos = 0;
    p_ctrl->valid = false;
    p_ctrl->off_count = 0;
    p_ctrl->off_count_limit = (p_cfg->off_count_limit > 0) ? 
                              p_cfg->off_count_limit : DEFAULT_OFF_COUNT;
    p_ctrl->median_size = p_cfg->median_size;
    p_ctrl->median_index = 0;
    p_ctrl->adaptive = p_cfg->adaptive;
    p_ctrl->last_raw = TOUCH_OFF_VAL;
    p_ctrl->speed = 0;
    
    /* 清空中值滤波缓冲区 */
    memset(p_ctrl->median_buf, 0, sizeof(p_ctrl->median_buf));
    
    return 0;
}

/**
 * @brief 应用滤波器处理滑条位置
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_filter_process(slider_filter_ctrl_t *p_ctrl, uint16_t raw)
{
    uint16_t filtered;
    
    /* 检查参数有效性 */
    if (p_ctrl == NULL)
    {
        return raw;
    }
    
    /* 处理无触摸情况 */
    if (raw == TOUCH_OFF_VAL)
    {
        /* 没有触摸：累加 OFF 计数 */
        if (!p_ctrl->valid)
        {
            /* 尚未初始化：直接返回 OFF */
            return TOUCH_OFF_VAL;
        }
        p_ctrl->off_count++;
        if (p_ctrl->off_count < p_ctrl->off_count_limit)
        {
            /* 未达释放阈值：保持最后位置 */
            return p_ctrl->last_pos;
        }
        /* 连续多帧 OFF：释放状态 */
        p_ctrl->valid = false;
        p_ctrl->off_count = 0;
        p_ctrl->last_raw = TOUCH_OFF_VAL;
        p_ctrl->speed = 0;
        return TOUCH_OFF_VAL;
    }
    
    /* 有触摸：重置 OFF 计数 */
    p_ctrl->off_count = 0;
    
    /* 计算速度 */
    if (p_ctrl->last_raw != TOUCH_OFF_VAL)
    {
        int16_t diff = (int16_t)raw - (int16_t)p_ctrl->last_raw;
        p_ctrl->speed = (diff < 0) ? -diff : diff;
    }
    p_ctrl->last_raw = raw;
    
    /* 如果启用了自适应系数，根据速度调整系数 */
    if (p_ctrl->adaptive)
    {
        update_adaptive_coef(p_ctrl, raw);
    }
    
    /* 根据滤波器类型处理 */
    switch (p_ctrl->type)
    {
        case FILTER_TYPE_MEDIAN:
            filtered = median_filter(p_ctrl, raw);
            break;
            
        case FILTER_TYPE_HYBRID:
            /* 先应用中值滤波，再应用 SPIIR */
            filtered = median_filter(p_ctrl, raw);
            filtered = spiir_filter(p_ctrl, filtered);
            break;
            
        case FILTER_TYPE_SPIIR:
        default:
            filtered = spiir_filter(p_ctrl, raw);
            break;
    }
    
    /* 检查稳定时间 */
    if (p_ctrl->settlings > 0 && p_ctrl->count < p_ctrl->settlings)
    {
        p_ctrl->count++;
    }
    
    return filtered;
}

/**
 * @brief 更新 SPIIR 滤波器系数
 * @param p_ctrl 滤波器控制块
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 */
void slider_filter_update_coef(slider_filter_ctrl_t *p_ctrl, int16_t a_coef, int16_t b_coef)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->a_coef = a_coef;
        p_ctrl->b_coef = b_coef;
    }
}

/**
 * @brief 获取当前 SPIIR 滤波器系数
 * @param p_ctrl 滤波器控制块
 * @param p_a_coef 系数 a 的指针
 * @param p_b_coef 系数 b 的指针
 */
void slider_filter_get_coef(slider_filter_ctrl_t *p_ctrl, int16_t *p_a_coef, int16_t *p_b_coef)
{
    if (p_ctrl != NULL)
    {
        if (p_a_coef != NULL)
        {
            *p_a_coef = p_ctrl->a_coef;
        }
        
        if (p_b_coef != NULL)
        {
            *p_b_coef = p_ctrl->b_coef;
        }
    }
}

/**
 * @brief 重置滤波器状态
 * @param p_ctrl 滤波器控制块
 */
void slider_filter_reset(slider_filter_ctrl_t *p_ctrl)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->count = 0;
        p_ctrl->last_pos = 0;
        p_ctrl->valid = false;
        p_ctrl->off_count = 0;
        p_ctrl->median_index = 0;
        p_ctrl->last_raw = TOUCH_OFF_VAL;
        p_ctrl->speed = 0;
        
        /* 清空中值滤波缓冲区 */
        memset(p_ctrl->median_buf, 0, sizeof(p_ctrl->median_buf));
    }
}

/**
 * @brief 设置滤波器类型
 * @param p_ctrl 滤波器控制块
 * @param type 滤波器类型
 */
void slider_filter_set_type(slider_filter_ctrl_t *p_ctrl, filter_type_t type)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->type = type;
        slider_filter_reset(p_ctrl);
    }
}

/**
 * @brief 启用/禁用自适应系数
 * @param p_ctrl 滤波器控制块
 * @param enable 是否启用
 */
void slider_filter_set_adaptive(slider_filter_ctrl_t *p_ctrl, bool enable)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->adaptive = enable;
    }
}

/**
 * @brief 中值滤波实现
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置
 * @return 滤波后的位置
 */
static uint16_t median_filter(slider_filter_ctrl_t *p_ctrl, uint16_t raw)
{
    uint16_t median_work[9];
    uint8_t size;
    
    /* 如果未配置中值滤波或窗口大小无效，直接返回原始值 */
    if (p_ctrl->median_size < 3)
    {
        return raw;
    }
    
    size = p_ctrl->median_size;
    
    /* 更新环形缓冲区 */
    p_ctrl->median_buf[p_ctrl->median_index] = raw;
    p_ctrl->median_index = (p_ctrl->median_index + 1) % size;
    
    /* 复制数据到工作缓冲区 */
    memcpy(median_work, p_ctrl->median_buf, size * sizeof(uint16_t));
    
    /* 排序 */
    median_sort(median_work, size);
    
    /* 返回中值 */
    return median_work[size / 2];
}

/**
 * @brief SPIIR 滤波实现
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置
 * @return 滤波后的位置
 */
static uint16_t spiir_filter(slider_filter_ctrl_t *p_ctrl, uint16_t raw)
{
    int32_t spiir_sum;
    
    /* 初次采样：直接初始化 */
    if (!p_ctrl->valid)
    {
        p_ctrl->last_pos = raw;
        p_ctrl->valid = true;
        return p_ctrl->last_pos;
    }
    
    /* SPIIR 滤波: y[n] = a*y[n-1] + b*x[n] */
    spiir_sum = (int32_t)((int32_t)p_ctrl->last_pos * (int32_t)p_ctrl->a_coef);
    spiir_sum += (int32_t)((int32_t)raw * (int32_t)p_ctrl->b_coef);
    
    /* 定点数转换 */
    spiir_sum >>= SPIIR_DECIMAL_POINT;
    
    /* 范围限制 */
    if (spiir_sum > 100)
    {
        spiir_sum = 100;
    }
    else if (spiir_sum < 0)
    {
        spiir_sum = 0;
    }
    
    p_ctrl->last_pos = (uint16_t)spiir_sum;
    return p_ctrl->last_pos;
}

/**
 * @brief 插入排序算法
 * @param data 待排序数据
 * @param size 数据大小
 */
static void median_sort(uint16_t *data, uint8_t size)
{
    uint8_t i, j;
    uint16_t temp;
    
    for (i = 1; i < size; i++)
    {
        if (data[i-1] > data[i])
        {
            temp = data[i];
            j = i;
            do
            {
                data[j] = data[j-1];
                j--;
            } while (j > 0 && data[j-1] > temp);
            data[j] = temp;
        }
    }
}

/**
 * @brief 根据速度更新自适应系数
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置
 */
static void update_adaptive_coef(slider_filter_ctrl_t *p_ctrl, uint16_t raw)
{
    /* 根据速度调整系数 */
    if (p_ctrl->speed <= SPEED_THRESHOLD_LOW)
    {
        /* 低速：高平滑度 */
        p_ctrl->a_coef = COEF_A_SLOW;
        p_ctrl->b_coef = COEF_B_SLOW;
    }
    else if (p_ctrl->speed >= SPEED_THRESHOLD_HIGH)
    {
        /* 高速：高响应度 */
        p_ctrl->a_coef = COEF_A_FAST;
        p_ctrl->b_coef = COEF_B_FAST;
    }
    else
    {
        /* 中速：线性插值 */
        int16_t range = SPEED_THRESHOLD_HIGH - SPEED_THRESHOLD_LOW;
        int16_t pos = p_ctrl->speed - SPEED_THRESHOLD_LOW;
        int16_t ratio = (pos * 100) / range;
        
        p_ctrl->a_coef = COEF_A_SLOW - ((COEF_A_SLOW - COEF_A_FAST) * ratio) / 100;
        p_ctrl->b_coef = COEF_B_SLOW + ((COEF_B_FAST - COEF_B_SLOW) * ratio) / 100;
    }
}
