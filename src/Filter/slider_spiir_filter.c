/**
 * @file   slider_spiir_filter.c
 * @brief  滑条 SPIIR (Single Pole Infinite Impulse Response) 滤波器实现
 *
 * 原始位置 raw: 0-100 或 TOUCH_OFF_VAL；
 * 输出位置 0-100，或 TOUCH_OFF_VAL 表示 OFF
 * 
 * SPIIR 滤波器公式: y[n] = a*y[n-1] + b*x[n]
 * 其中 a + b = 1.0，使用定点数表示 (Q14 格式)
 */

#include "slider_spiir_filter.h"
#include <stdbool.h>

/* 常量定义 */
#define TOUCH_OFF_VAL       0xFFFFU    /* 无触摸标志 */
#define SPIIR_DECIMAL_POINT 14         /* 定点数小数位数 */
#define OFF_COUNT_LIMIT     3U         /* 连续 OFF 帧阈值 */

/* 默认系数 (a = 0.94, b = 0.06) */
#define DEFAULT_COEF_A      15401      /* 0.94 * 2^14 */
#define DEFAULT_COEF_B      983        /* 0.06 * 2^14 */

/* 全局变量 */
static uint16_t g_last = 0;            /* 上次输出位置 */
static bool     g_valid = false;       /* 是否已初始化 */
static uint8_t  g_off_count = 0;       /* 连续 OFF 帧计数 */
static int16_t  g_coef_a = DEFAULT_COEF_A;  /* 系数 a */
static int16_t  g_coef_b = DEFAULT_COEF_B;  /* 系数 b */

/**
 * @brief 初始化 SPIIR 滤波器
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 */
void slider_spiir_init(int16_t a_coef, int16_t b_coef)
{
    g_coef_a = a_coef;
    g_coef_b = b_coef;
    g_last = 0;
    g_valid = false;
    g_off_count = 0;
}

/**
 * @brief 应用 SPIIR 滤波器处理滑条位置
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_spiir_filter(uint16_t raw)
{
    if (raw == TOUCH_OFF_VAL)
    {
        /* 没有触摸：累加 OFF 计数 */
        if (!g_valid)
        {
            /* 尚未初始化：直接返回 OFF */
            return TOUCH_OFF_VAL;
        }
        g_off_count++;
        if (g_off_count < OFF_COUNT_LIMIT)
        {
            /* 未达释放阈值：保持最后位置 */
            return g_last;
        }
        /* 连续多帧 OFF：释放状态 */
        g_valid = false;
        g_off_count = 0;
        return TOUCH_OFF_VAL;
    }

    /* 有触摸：重置 OFF 计数 */
    g_off_count = 0;

    /* 初次采样：直接初始化 */
    if (!g_valid)
    {
        g_last = raw;
        g_valid = true;
        return g_last;
    }

    /* SPIIR 滤波: y[n] = a*y[n-1] + b*x[n] */
    int32_t spiir_sum = (int32_t)((int32_t)g_last * (int32_t)g_coef_a);
    spiir_sum += (int32_t)((int32_t)raw * (int32_t)g_coef_b);
    
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
    
    g_last = (uint16_t)spiir_sum;
    return g_last;
}

/**
 * @brief 更新 SPIIR 滤波器系数
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 */
void slider_spiir_update_coef(int16_t a_coef, int16_t b_coef)
{
    g_coef_a = a_coef;
    g_coef_b = b_coef;
}

/**
 * @brief 获取当前 SPIIR 滤波器系数
 * @param p_a_coef 系数 a 的指针
 * @param p_b_coef 系数 b 的指针
 */
void slider_spiir_get_coef(int16_t *p_a_coef, int16_t *p_b_coef)
{
    if (p_a_coef != NULL)
    {
        *p_a_coef = g_coef_a;
    }
    
    if (p_b_coef != NULL)
    {
        *p_b_coef = g_coef_b;
    }
}
