/**
 * @file   slider_jump_filter.h
 * @brief  滑条跳变点处理滤波器
 */

#ifndef SLIDER_JUMP_FILTER_H
#define SLIDER_JUMP_FILTER_H

#include <stdint.h>
#include <stdbool.h>

/* 常量定义 */
#define TOUCH_OFF_VAL       0xFFFFU    /* 无触摸标志 */
#define MAX_HISTORY_SIZE    5          /* 历史记录最大长度 */

/* 滤波器配置 */
typedef struct {
    uint8_t off_count_limit;   /* 连续 OFF 帧阈值 */
    uint8_t history_size;      /* 历史记录长度 (1-5) */
    int16_t a_coef;            /* SPIIR 系数 a (保留上一次输出的比例) */
    int16_t b_coef;            /* SPIIR 系数 b (当前输入的比例) */
    bool predict_enable;       /* 是否启用预测 */
    bool adaptive_enable;      /* 是否启用自适应系数 */
} slider_jump_cfg_t;

/* 滤波器控制块 */
typedef struct {
    uint8_t off_count;         /* 连续 OFF 帧计数 */
    uint8_t off_count_limit;   /* 连续 OFF 帧阈值 */
    uint16_t last_pos;         /* 上次输出位置 */
    bool valid;                /* 是否已初始化 */
    int16_t a_coef;            /* SPIIR 系数 a */
    int16_t b_coef;            /* SPIIR 系数 b */
    bool predict_enable;       /* 是否启用预测 */
    bool adaptive_enable;      /* 是否启用自适应系数 */
    
    /* 历史记录 */
    uint8_t history_size;      /* 历史记录长度 */
    uint16_t history[MAX_HISTORY_SIZE]; /* 历史位置值 */
    uint8_t history_index;     /* 历史记录索引 */
    uint8_t valid_history;     /* 有效历史记录数 */
    
    /* 速度计算 */
    uint16_t last_raw;         /* 上次原始位置 */
    uint16_t speed;            /* 当前速度 */
    uint32_t last_time;        /* 上次时间戳 */
} slider_jump_ctrl_t;

/**
 * @brief 初始化跳变滤波器
 * @param p_ctrl 滤波器控制块
 * @param p_cfg 滤波器配置
 * @return 0: 成功, 非0: 错误码
 */
int slider_jump_init(slider_jump_ctrl_t *p_ctrl, const slider_jump_cfg_t *p_cfg);

/**
 * @brief 应用滤波器处理滑条位置
 * @param p_ctrl 滤波器控制块
 * @param raw 原始滑条位置 (0-100 或 TOUCH_OFF_VAL)
 * @param timestamp 当前时间戳 (ms)
 * @return 滤波后的位置 (0-100)，或 TOUCH_OFF_VAL 表示 OFF
 */
uint16_t slider_jump_process(slider_jump_ctrl_t *p_ctrl, uint16_t raw, uint32_t timestamp);

/**
 * @brief 重置滤波器状态
 * @param p_ctrl 滤波器控制块
 */
void slider_jump_reset(slider_jump_ctrl_t *p_ctrl);

/**
 * @brief 更新 SPIIR 滤波器系数
 * @param p_ctrl 滤波器控制块
 * @param a_coef 系数 a (保留上一次输出的比例)
 * @param b_coef 系数 b (当前输入的比例)
 */
void slider_jump_update_coef(slider_jump_ctrl_t *p_ctrl, int16_t a_coef, int16_t b_coef);

/**
 * @brief 启用/禁用预测功能
 * @param p_ctrl 滤波器控制块
 * @param enable 是否启用
 */
void slider_jump_set_predict(slider_jump_ctrl_t *p_ctrl, bool enable);

/**
 * @brief 启用/禁用自适应系数
 * @param p_ctrl 滤波器控制块
 * @param enable 是否启用
 */
void slider_jump_set_adaptive(slider_jump_ctrl_t *p_ctrl, bool enable);

#endif /* SLIDER_JUMP_FILTER_H */
