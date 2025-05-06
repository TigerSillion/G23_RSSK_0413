/**
 * @file   slider_jump_app.h
 * @brief  滑条跳变点处理应用接口
 */

#ifndef SLIDER_JUMP_APP_H
#define SLIDER_JUMP_APP_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 初始化跳变滤波器应用
 */
void slider_jump_app_init(void);

/**
 * @brief 滑条跳变滤波器应用主处理函数
 * @return 滤波后的滑条位置
 */
uint16_t slider_jump_app_process(void);

/**
 * @brief 切换到下一个预设
 * @return 当前预设名称
 */
const char* slider_jump_switch_preset(void);

/**
 * @brief 设置指定的预设
 * @param preset_id 预设ID
 * @return 预设名称
 */
const char* slider_jump_set_preset(uint8_t preset_id);

/**
 * @brief 设置自定义 SPIIR 系数
 * @param a_value 系数a的值 (0-100 表示 0.0-1.0)
 * @param b_value 系数b的值 (0-100 表示 0.0-1.0)
 * @note a_value + b_value 应等于 100
 */
void slider_jump_set_custom_coef(uint8_t a_value, uint8_t b_value);

/**
 * @brief 处理滑条数据
 * @param raw_position 原始滑条位置
 * @return 滤波后的位置
 */
uint16_t slider_jump_process(uint16_t raw_position);

/**
 * @brief 启用/禁用调试输出
 * @param enable 是否启用
 */
void slider_jump_debug_output(bool enable);

/**
 * @brief 启用/禁用预测功能
 * @param enable 是否启用
 */
void slider_jump_enable_predict(bool enable);

/**
 * @brief 启用/禁用自适应系数
 * @param enable 是否启用
 */
void slider_jump_enable_adaptive(bool enable);

/**
 * @brief 设置连续OFF帧阈值
 * @param count 阈值 (1-10)
 */
void slider_jump_set_off_count(uint8_t count);

#endif /* SLIDER_JUMP_APP_H */
