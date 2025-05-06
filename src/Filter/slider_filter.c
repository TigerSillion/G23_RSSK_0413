/**
 * @file   slider_filter.c
 * @brief  简单平滑滤波：指数平滑 + 连续 3 帧无触摸才释放
 *
 * 原始位置 raw: 0-100 或 TOUCH_OFF_VAL；
 * 输出位置 0-100，或 TOUCH_OFF_VAL 表示 OFF
 */

#include <stdint.h>
#include <stdbool.h>

#define TOUCH_OFF_VAL    0xFFFFU    /* 无触摸标志 */
#define SMOOTH_ALPHA_NUM   1        /* α = 1/2 */
#define SMOOTH_ALPHA_DEN   2
#define OFF_COUNT_LIMIT    3U       /* 连续 OFF 帧阈值 */

static uint16_t g_last = 0;        /* 上次输出位置 */
static bool     g_valid = false;   /* 是否已初始化 */
static uint8_t  g_off_count = 0;   /* 连续 OFF 帧计数 */

/**
 * @brief 简单指数平滑滤波
 * @param raw  原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @return     平滑后位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_simple_filter(uint16_t raw)
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
        g_last  = raw;
        g_valid = true;
        return g_last;
    }

    /* 指数平滑： new = last*(1-α) + raw*α */
    g_last = (uint16_t)((g_last * (SMOOTH_ALPHA_DEN - SMOOTH_ALPHA_NUM) + raw * SMOOTH_ALPHA_NUM) / SMOOTH_ALPHA_DEN);
    return g_last;
}

/*
示例使用：
#include "slider_filter.h"
uint16_t filtered = slider_simple_filter(slider_raw);
*/
