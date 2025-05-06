/**
 * @file   slider_spiir_app.h
 * @brief  滑条 SPIIR 滤波器应用接口
 */

#ifndef SLIDER_SPIIR_APP_H
#define SLIDER_SPIIR_APP_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 初始化 SPIIR 滤波器应用
 */
void slider_spiir_app_init(void);

/**
 * @brief 滑条 SPIIR 应用主处理函数
 * @return 滤波后的滑条位置
 */
uint16_t slider_spiir_app_process(void);

/**
 * @brief 切换到下一个预设
 * @return 当前预设名称
 */
const char* slider_spiir_switch_preset(void);

/**
 * @brief 设置指定的预设
 * @param preset_id 预设ID
 * @return 预设名称
 */
const char* slider_spiir_set_preset(uint8_t preset_id);

/**
 * @brief 设置自定义系数
 * @param a_value 系数a的值 (0-100 表示 0.0-1.0)
 * @param b_value 系数b的值 (0-100 表示 0.0-1.0)
 * @note a_value + b_value 应等于 100
 */
void slider_spiir_set_custom_coef(uint8_t a_value, uint8_t b_value);

/**
 * @brief 处理滑条数据
 * @param raw_position 原始滑条位置
 * @return 滤波后的位置
 */
uint16_t slider_spiir_process(uint16_t raw_position);

/**
 * @brief 启用/禁用调试输出
 * @param enable 是否启用
 */
void slider_spiir_debug_output(bool enable);

#endif /* SLIDER_SPIIR_APP_H */
