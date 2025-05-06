/**
 * @file   slider_jump_simple.h
 * @brief  简化版滑条跳变点处理滤波器
 */

#ifndef SLIDER_JUMP_SIMPLE_H
#define SLIDER_JUMP_SIMPLE_H

#include <stdint.h>

/**
 * @brief 简化版跳变点处理滤波器
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_simple_filter(uint16_t raw);

#endif /* SLIDER_JUMP_SIMPLE_H */
