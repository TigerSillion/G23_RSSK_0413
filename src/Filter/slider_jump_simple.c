/**
 * @file   slider_jump_simple.c
 * @brief  简化版滑条跳变点处理滤波器
 *
 * 专门处理滑条位置中的跳变点（65535值）
 * 使用历史记录、预测和自适应系数来平滑滑条位置
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* 常量定义 */
#define TOUCH_OFF_VAL       0xFFFFU    /* 无触摸标志 */
#define SPIIR_DECIMAL_POINT 14         /* 定点数小数位数 */
#define OFF_COUNT_LIMIT     5U         /* 连续 OFF 帧阈值 */
#define MAX_HISTORY_SIZE    5          /* 历史记录最大长度 */

/* 默认系数 (a = 0.94, b = 0.06) */
#define DEFAULT_COEF_A      15401      /* 0.94 * 2^14 */
#define DEFAULT_COEF_B      983        /* 0.06 * 2^14 */

/* 全局变量 */
static uint8_t  g_off_count = 0;       /* 连续 OFF 帧计数 */
static uint16_t g_last_pos = 0;        /* 上次输出位置 */
static bool     g_valid = false;       /* 是否已初始化 */
static int16_t  g_a_coef = DEFAULT_COEF_A; /* SPIIR 系数 a */
static int16_t  g_b_coef = DEFAULT_COEF_B; /* SPIIR 系数 b */

/* 历史记录 */
static uint16_t g_history[MAX_HISTORY_SIZE]; /* 历史位置值 */
static uint8_t  g_history_index = 0;   /* 历史记录索引 */
static uint8_t  g_valid_history = 0;   /* 有效历史记录数 */

/* 速度计算 */
static uint16_t g_last_raw = TOUCH_OFF_VAL; /* 上次原始位置 */
static uint16_t g_speed = 0;           /* 当前速度 */

/* 内部函数声明 */
static uint16_t predict_position(void);
static void update_history(uint16_t pos);

/**
 * @brief 简化版跳变点处理滤波器
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_simple_filter(uint16_t raw)
{
    int32_t spiir_sum;
    
    /* 处理无触摸情况 */
    if (raw == TOUCH_OFF_VAL)
    {
        /* 没有触摸：累加 OFF 计数 */
        if (!g_valid)
        {
            /* 尚未初始化：直接返回 OFF */
            return TOUCH_OFF_VAL;
        }
        
        g_off_count++;
        
        /* 如果有足够的历史记录，尝试预测当前位置 */
        if (g_off_count == 1 && g_valid_history >= 2)
        {
            uint16_t predicted = predict_position();
            
            /* 使用预测值更新历史记录 */
            update_history(predicted);
            
            /* 应用 SPIIR 滤波 */
            spiir_sum = (int32_t)((int32_t)g_last_pos * (int32_t)g_a_coef);
            spiir_sum += (int32_t)((int32_t)predicted * (int32_t)g_b_coef);
            
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
            
            g_last_pos = (uint16_t)spiir_sum;
            return g_last_pos;
        }
        
        /* 未达释放阈值：保持最后位置 */
        if (g_off_count < OFF_COUNT_LIMIT)
        {
            return g_last_pos;
        }
        
        /* 连续多帧 OFF：释放状态 */
        g_valid = false;
        g_off_count = 0;
        g_valid_history = 0;
        g_last_raw = TOUCH_OFF_VAL;
        g_speed = 0;
        
        return TOUCH_OFF_VAL;
    }
    
    /* 有触摸：重置 OFF 计数 */
    g_off_count = 0;
    
    /* 计算速度 */
    if (g_last_raw != TOUCH_OFF_VAL)
    {
        int16_t diff = (int16_t)raw - (int16_t)g_last_raw;
        g_speed = (diff < 0) ? -diff : diff;
    }
    
    g_last_raw = raw;
    
    /* 更新历史记录 */
    update_history(raw);
    
    /* 初次采样：直接初始化 */
    if (!g_valid)
    {
        g_last_pos = raw;
        g_valid = true;
        return g_last_pos;
    }
    
    /* SPIIR 滤波: y[n] = a*y[n-1] + b*x[n] */
    spiir_sum = (int32_t)((int32_t)g_last_pos * (int32_t)g_a_coef);
    spiir_sum += (int32_t)((int32_t)raw * (int32_t)g_b_coef);
    
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
    
    g_last_pos = (uint16_t)spiir_sum;
    return g_last_pos;
}

/**
 * @brief 更新历史记录
 * @param pos 当前位置
 */
static void update_history(uint16_t pos)
{
    /* 更新环形缓冲区 */
    g_history[g_history_index] = pos;
    g_history_index = (g_history_index + 1) % MAX_HISTORY_SIZE;
    
    /* 更新有效历史记录数 */
    if (g_valid_history < MAX_HISTORY_SIZE)
    {
        g_valid_history++;
    }
}

/**
 * @brief 预测当前位置
 * @return 预测的位置
 */
static uint16_t predict_position(void)
{
    int16_t diff = 0;
    uint16_t predicted;
    uint8_t prev_idx, prev_prev_idx;
    
    /* 至少需要两个历史记录才能预测 */
    if (g_valid_history < 2)
    {
        return g_last_pos;
    }
    
    /* 计算前两个历史记录的索引 */
    prev_idx = (g_history_index + MAX_HISTORY_SIZE - 1) % MAX_HISTORY_SIZE;
    prev_prev_idx = (g_history_index + MAX_HISTORY_SIZE - 2) % MAX_HISTORY_SIZE;
    
    /* 计算变化趋势 */
    diff = (int16_t)g_history[prev_idx] - (int16_t)g_history[prev_prev_idx];
    
    /* 根据趋势预测下一个位置 */
    if (diff > 0)
    {
        /* 上升趋势 */
        predicted = (g_history[prev_idx] + diff <= 100) ? 
                    (g_history[prev_idx] + diff) : 100;
    }
    else if (diff < 0)
    {
        /* 下降趋势 */
        predicted = (g_history[prev_idx] >= (uint16_t)(-diff)) ? 
                    (g_history[prev_idx] + diff) : 0;
    }
    else
    {
        /* 无变化 */
        predicted = g_history[prev_idx];
    }
    
    return predicted;
}
