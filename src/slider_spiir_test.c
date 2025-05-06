/**
 * @file   slider_spiir_test.c
 * @brief  滑条 SPIIR 滤波器测试应用
 */

#include "slider_spiir_filter.h"
#include "rm_touch_api.h"
#include "Debug_UART/debug_uart.h"
#include <stdio.h>
#include <string.h>

/* 系数预设 */
typedef struct {
    const char* name;
    int16_t a_coef;
    int16_t b_coef;
} spiir_preset_t;

/* 不同响应特性的系数预设 */
static const spiir_preset_t g_presets[] = {
    { "平滑", 15729, 655 },   /* a = 0.96, b = 0.04 */
    { "标准", 15401, 983 },   /* a = 0.94, b = 0.06 */
    { "快速", 14746, 1638 },  /* a = 0.90, b = 0.10 */
    { "极速", 13107, 3277 }   /* a = 0.80, b = 0.20 */
};

#define NUM_PRESETS (sizeof(g_presets) / sizeof(g_presets[0]))
#define TOUCH_OFF_VAL 0xFFFFU

/* 全局变量 */
static uint8_t g_current_preset = 1;  // 默认使用"标准"预设
static char g_print_buffer[128];      // 打印缓冲区

/**
 * @brief 切换到下一个预设
 */
void switch_to_next_preset(void)
{
    g_current_preset = (g_current_preset + 1) % NUM_PRESETS;
    
    // 更新滤波器系数
    slider_spiir_update_coef(g_presets[g_current_preset].a_coef, 
                             g_presets[g_current_preset].b_coef);
    
    // 打印当前预设信息
    sprintf(g_print_buffer, "切换到预设: %s (a=0.%d, b=0.%d)\n", 
            g_presets[g_current_preset].name,
            (g_presets[g_current_preset].a_coef * 100) >> 14,
            (g_presets[g_current_preset].b_coef * 100) >> 14);
    
    DEBUG_PRINT(g_print_buffer);
}

/**
 * @brief 处理滑条数据
 * @param raw_position 原始滑条位置
 * @return 滤波后的位置
 */
uint16_t process_slider_position(uint16_t raw_position)
{
    static uint32_t last_print_time = 0;
    static uint16_t last_raw = TOUCH_OFF_VAL;
    static uint16_t last_filtered = TOUCH_OFF_VAL;
    uint16_t filtered_position;
    
    // 应用 SPIIR 滤波
    filtered_position = slider_spiir_filter(raw_position);
    
    // 每100ms打印一次位置信息（或者位置变化时）
    uint32_t current_time = get_system_time_ms(); // 假设有这个函数获取系统时间
    if ((current_time - last_print_time >= 100) || 
        (raw_position != last_raw) || 
        (filtered_position != last_filtered))
    {
        if (raw_position != TOUCH_OFF_VAL)
        {
            sprintf(g_print_buffer, "滑条: 原始=%3u, 滤波=%3u, 预设=%s\n", 
                    raw_position, filtered_position, g_presets[g_current_preset].name);
            DEBUG_PRINT(g_print_buffer);
        }
        else if (last_raw != TOUCH_OFF_VAL)
        {
            DEBUG_PRINT("滑条: 释放\n");
        }
        
        last_print_time = current_time;
        last_raw = raw_position;
        last_filtered = filtered_position;
    }
    
    return filtered_position;
}

/**
 * @brief 初始化 SPIIR 滤波器测试
 */
void slider_spiir_test_init(void)
{
    // 初始化 SPIIR 滤波器
    slider_spiir_init(g_presets[g_current_preset].a_coef, 
                      g_presets[g_current_preset].b_coef);
    
    // 打印初始化信息
    DEBUG_PRINT("\n===== 滑条 SPIIR 滤波器测试 =====\n");
    sprintf(g_print_buffer, "当前预设: %s (a=0.%d, b=0.%d)\n", 
            g_presets[g_current_preset].name,
            (g_presets[g_current_preset].a_coef * 100) >> 14,
            (g_presets[g_current_preset].b_coef * 100) >> 14);
    DEBUG_PRINT(g_print_buffer);
    DEBUG_PRINT("按下按键可切换预设\n");
}

/*
使用示例:

// 在初始化代码中
slider_spiir_test_init();

// 在主循环中
void main_loop(void)
{
    // 获取触摸数据
    uint64_t button_status;
    uint16_t slider_position;
    uint16_t wheel_position;
    
    // 读取触摸数据
    RM_TOUCH_DataGet(&g_qe_touch_instance_config01, &button_status, &slider_position, &wheel_position);
    
    // 处理滑条数据
    uint16_t filtered_position = process_slider_position(slider_position);
    
    // 检测按键切换预设（示例：使用第一个按键）
    static uint64_t last_button_status = 0;
    if ((button_status & 0x01) && !(last_button_status & 0x01))
    {
        switch_to_next_preset();
    }
    last_button_status = button_status;
    
    // 使用滤波后的滑条位置进行应用逻辑
    // ...
}
*/
