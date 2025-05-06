/**
 * @file   slider_jump_filter.c
 * @brief  滑条跳变点处理滤波器
 *
 * 专门处理滑条位置中的跳变点（65535值）
 * 使用历史记录、预测和自适应系数来平滑滑条位置
 */

#include "slider_jump_filter.h"
#include <string.h>

/* 常量定义 */
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
static uint16_t predict_position(slider_jump_ctrl_t *p_ctrl);
static uint16_t spiir_filter(slider_jump_ctrl_t *p_ctrl, uint16_t raw);
static void update_history(slider_jump_ctrl_t *p_ctrl, uint16_t pos);
static void update_adaptive_coef(slider_jump_ctrl_t *p_ctrl);

/**
 * @brief 初始化跳变滤波器
 * @param p_ctrl 滤波器控制块
 * @param p_cfg 滤波器配置
 * @return 0: 成功, 非0: 错误码
 */
int slider_jump_init(slider_jump_ctrl_t *p_ctrl, const slider_jump_cfg_t *p_cfg)
{
    if (p_ctrl == NULL || p_cfg == NULL)
    {
        return -1;
    }
    
    /* 检查参数有效性 */
    if (p_cfg->history_size < 1 || p_cfg->history_size > MAX_HISTORY_SIZE)
    {
        return -2;
    }
    
    /* 初始化控制块 */
    p_ctrl->off_count = 0;
    p_ctrl->off_count_limit = (p_cfg->off_count_limit > 0) ? 
                              p_cfg->off_count_limit : DEFAULT_OFF_COUNT;
    p_ctrl->last_pos = 0;
    p_ctrl->valid = false;
    p_ctrl->a_coef = p_cfg->a_coef;
    p_ctrl->b_coef = p_cfg->b_coef;
    p_ctrl->predict_enable = p_cfg->predict_enable;
    p_ctrl->adaptive_enable = p_cfg->adaptive_enable;
    
    p_ctrl->history_size = p_cfg->history_size;
    p_ctrl->history_index = 0;
    p_ctrl->valid_history = 0;
    p_ctrl->last_raw = TOUCH_OFF_VAL;
    p_ctrl->speed = 0;
    p_ctrl->last_time = 0;
    
    /* 清空历史记录 */
    memset(p_ctrl->history, 0, sizeof(p_ctrl->history));
    
    return 0;
}

/**
 * @brief 应用滤波器处理滑条位置
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @param timestamp 当前时间戳 (ms)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_jump_process(slider_jump_ctrl_t *p_ctrl, uint16_t raw, uint32_t timestamp)
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
        
        /* 如果启用了预测且有足够的历史记录，尝试预测当前位置 */
        if (p_ctrl->predict_enable && p_ctrl->off_count == 1 && p_ctrl->valid_history >= 2)
        {
            uint16_t predicted = predict_position(p_ctrl);
            
            /* 使用预测值更新历史记录 */
            update_history(p_ctrl, predicted);
            
            /* 应用 SPIIR 滤波 */
            filtered = spiir_filter(p_ctrl, predicted);
            
            return filtered;
        }
        
        /* 未达释放阈值：保持最后位置 */
        if (p_ctrl->off_count < p_ctrl->off_count_limit)
        {
            return p_ctrl->last_pos;
        }
        
        /* 连续多帧 OFF：释放状态 */
        p_ctrl->valid = false;
        p_ctrl->off_count = 0;
        p_ctrl->valid_history = 0;
        p_ctrl->last_raw = TOUCH_OFF_VAL;
        p_ctrl->speed = 0;
        
        return TOUCH_OFF_VAL;
    }
    
    /* 有触摸：重置 OFF 计数 */
    p_ctrl->off_count = 0;
    
    /* 计算速度 */
    if (p_ctrl->last_raw != TOUCH_OFF_VAL && p_ctrl->last_time != 0)
    {
        int16_t diff = (int16_t)raw - (int16_t)p_ctrl->last_raw;
        uint32_t time_diff = timestamp - p_ctrl->last_time;
        
        /* 防止除零 */
        if (time_diff > 0)
        {
            /* 计算每毫秒的位置变化 */
            p_ctrl->speed = (diff < 0) ? -diff : diff;
        }
    }
    
    p_ctrl->last_raw = raw;
    p_ctrl->last_time = timestamp;
    
    /* 如果启用了自适应系数，根据速度调整系数 */
    if (p_ctrl->adaptive_enable)
    {
        update_adaptive_coef(p_ctrl);
    }
    
    /* 更新历史记录 */
    update_history(p_ctrl, raw);
    
    /* 应用 SPIIR 滤波 */
    filtered = spiir_filter(p_ctrl, raw);
    
    return filtered;
}

/**
 * @brief 重置滤波器状态
 * @param p_ctrl 滤波器控制块
 */
void slider_jump_reset(slider_jump_ctrl_t *p_ctrl)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->off_count = 0;
        p_ctrl->last_pos = 0;
        p_ctrl->valid = false;
        p_ctrl->history_index = 0;
        p_ctrl->valid_history = 0;
        p_ctrl->last_raw = TOUCH_OFF_VAL;
        p_ctrl->speed = 0;
        p_ctrl->last_time = 0;
        
        /* 清空历史记录 */
        memset(p_ctrl->history, 0, sizeof(p_ctrl->history));
    }
}

/**
 * @brief 更新 SPIIR 滤波器系数
 * @param p_ctrl 滤波器控制块
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 */
void slider_jump_update_coef(slider_jump_ctrl_t *p_ctrl, int16_t a_coef, int16_t b_coef)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->a_coef = a_coef;
        p_ctrl->b_coef = b_coef;
    }
}

/**
 * @brief 启用/禁用预测功能
 * @param p_ctrl 滤波器控制块
 * @param enable 是否启用
 */
void slider_jump_set_predict(slider_jump_ctrl_t *p_ctrl, bool enable)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->predict_enable = enable;
    }
}

/**
 * @brief 启用/禁用自适应系数
 * @param p_ctrl 滤波器控制块
 * @param enable 是否启用
 */
void slider_jump_set_adaptive(slider_jump_ctrl_t *p_ctrl, bool enable)
{
    if (p_ctrl != NULL)
    {
        p_ctrl->adaptive_enable = enable;
    }
}

/**
 * @brief SPIIR 滤波实现
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置
 * @return 滤波后的位置
 */
static uint16_t spiir_filter(slider_jump_ctrl_t *p_ctrl, uint16_t raw)
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
 * @brief 更新历史记录
 * @param p_ctrl 滤波器控制块
 * @param pos 当前位置
 */
static void update_history(slider_jump_ctrl_t *p_ctrl, uint16_t pos)
{
    /* 更新环形缓冲区 */
    p_ctrl->history[p_ctrl->history_index] = pos;
    p_ctrl->history_index = (p_ctrl->history_index + 1) % p_ctrl->history_size;
    
    /* 更新有效历史记录数 */
    if (p_ctrl->valid_history < p_ctrl->history_size)
    {
        p_ctrl->valid_history++;
    }
}

/**
 * @brief 预测当前位置
 * @param p_ctrl 滤波器控制块
 * @return 预测的位置
 */
static uint16_t predict_position(slider_jump_ctrl_t *p_ctrl)
{
    int16_t diff = 0;
    uint16_t predicted;
    uint8_t prev_idx, prev_prev_idx;
    
    /* 至少需要两个历史记录才能预测 */
    if (p_ctrl->valid_history < 2)
    {
        return p_ctrl->last_pos;
    }
    
    /* 计算前两个历史记录的索引 */
    prev_idx = (p_ctrl->history_index + p_ctrl->history_size - 1) % p_ctrl->history_size;
    prev_prev_idx = (p_ctrl->history_index + p_ctrl->history_size - 2) % p_ctrl->history_size;
    
    /* 计算变化趋势 */
    diff = (int16_t)p_ctrl->history[prev_idx] - (int16_t)p_ctrl->history[prev_prev_idx];
    
    /* 根据趋势预测下一个位置 */
    if (diff > 0)
    {
        /* 上升趋势 */
        predicted = (p_ctrl->history[prev_idx] + diff <= 100) ? 
                    (p_ctrl->history[prev_idx] + diff) : 100;
    }
    else if (diff < 0)
    {
        /* 下降趋势 */
        predicted = (p_ctrl->history[prev_idx] >= (uint16_t)(-diff)) ? 
                    (p_ctrl->history[prev_idx] + diff) : 0;
    }
    else
    {
        /* 无变化 */
        predicted = p_ctrl->history[prev_idx];
    }
    
    return predicted;
}

/**
 * @brief 根据速度更新自适应系数
 * @param p_ctrl 滤波器控制块
 */
static void update_adaptive_coef(slider_jump_ctrl_t *p_ctrl)
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
