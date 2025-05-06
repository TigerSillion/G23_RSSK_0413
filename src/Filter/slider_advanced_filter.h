/**
 * @file   slider_advanced_filter.h
 * @brief  滑条高级滤波器 - 结合 SPIIR 和中值滤波
 */

#ifndef SLIDER_ADVANCED_FILTER_H
#define SLIDER_ADVANCED_FILTER_H

#include <stdint.h>
#include <stdbool.h>

/* 滤波器类型 */
typedef enum {
    FILTER_TYPE_SPIIR,       // 单极点 IIR 滤波器
    FILTER_TYPE_MEDIAN,      // 中值滤波器
    FILTER_TYPE_HYBRID       // 混合滤波器 (先中值后 SPIIR)
} filter_type_t;

/* 滤波器配置 */
typedef struct {
    filter_type_t type;      // 滤波器类型
    uint16_t settlings;      // 稳定时间 (采样次数)
    int16_t a_coef;          // SPIIR 系数 a (保留上一次输出的比例)
    int16_t b_coef;          // SPIIR 系数 b (当前输入的比例)
    uint8_t median_size;     // 中值滤波窗口大小 (3, 5, 7, 9)
    uint8_t off_count_limit; // 连续 OFF 帧阈值
    bool adaptive;           // 是否启用自适应系数
} slider_filter_cfg_t;

/* 滤波器控制块 */
typedef struct {
    filter_type_t type;      // 滤波器类型
    uint16_t count;          // 稳定计数
    uint16_t settlings;      // 稳定时间
    int16_t a_coef;          // SPIIR 系数 a
    int16_t b_coef;          // SPIIR 系数 b
    uint16_t last_pos;       // 上次输出位置
    bool valid;              // 是否已初始化
    uint8_t off_count;       // 连续 OFF 帧计数
    uint8_t off_count_limit; // 连续 OFF 帧阈值
    uint8_t median_size;     // 中值滤波窗口大小
    uint8_t median_index;    // 中值滤波缓冲区索引
    uint16_t median_buf[9];  // 中值滤波缓冲区 (最大支持 9 点)
    bool adaptive;           // 是否启用自适应系数
    uint16_t last_raw;       // 上次原始位置
    uint16_t speed;          // 当前速度 (位置变化量)
} slider_filter_ctrl_t;

/**
 * @brief 初始化滑条滤波器
 * @param p_ctrl 滤波器控制块
 * @param p_cfg 滤波器配置
 * @return 0: 成功, 非0: 错误码
 */
int slider_filter_init(slider_filter_ctrl_t *p_ctrl, const slider_filter_cfg_t *p_cfg);

/**
 * @brief 应用滤波器处理滑条位置
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_filter_process(slider_filter_ctrl_t *p_ctrl, uint16_t raw);

/**
 * @brief 更新 SPIIR 滤波器系数
 * @param p_ctrl 滤波器控制块
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 */
void slider_filter_update_coef(slider_filter_ctrl_t *p_ctrl, int16_t a_coef, int16_t b_coef);

/**
 * @brief 获取当前 SPIIR 滤波器系数
 * @param p_ctrl 滤波器控制块
 * @param p_a_coef 系数 a 的指针
 * @param p_b_coef 系数 b 的指针
 */
void slider_filter_get_coef(slider_filter_ctrl_t *p_ctrl, int16_t *p_a_coef, int16_t *p_b_coef);

/**
 * @brief 重置滤波器状态
 * @param p_ctrl 滤波器控制块
 */
void slider_filter_reset(slider_filter_ctrl_t *p_ctrl);

/**
 * @brief 设置滤波器类型
 * @param p_ctrl 滤波器控制块
 * @param type 滤波器类型
 */
void slider_filter_set_type(slider_filter_ctrl_t *p_ctrl, filter_type_t type);

/**
 * @brief 启用/禁用自适应系数
 * @param p_ctrl 滤波器控制块
 * @param enable 是否启用
 */
void slider_filter_set_adaptive(slider_filter_ctrl_t *p_ctrl, bool enable);

#endif /* SLIDER_ADVANCED_FILTER_H */
