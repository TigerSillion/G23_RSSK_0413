/**
 * @file   slider_spiir_filter.h
 * @brief  滑条 SPIIR (Single Pole Infinite Impulse Response) 滤波器
 */

#ifndef SLIDER_SPIIR_FILTER_H
#define SLIDER_SPIIR_FILTER_H

#include <stdint.h>

/**
 * @brief 初始化 SPIIR 滤波器
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 * @note  系数 a 和 b 应满足 a + b = 1.0，这里使用定点数表示
 *        例如: a = 0.9, b = 0.1 时，a_coef = 14746, b_coef = 1638
 */
void slider_spiir_init(int16_t a_coef, int16_t b_coef);

/**
 * @brief 应用 SPIIR 滤波器处理滑条位置
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_spiir_filter(uint16_t raw);

/**
 * @brief 更新 SPIIR 滤波器系数
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 */
void slider_spiir_update_coef(int16_t a_coef, int16_t b_coef);

/**
 * @brief 获取当前 SPIIR 滤波器系数
 * @param p_a_coef 系数 a 的指针
 * @param p_b_coef 系数 b 的指针
 */
void slider_spiir_get_coef(int16_t *p_a_coef, int16_t *p_b_coef);

#endif /* SLIDER_SPIIR_FILTER_H */
