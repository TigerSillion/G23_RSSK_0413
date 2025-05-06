/**
 * @file   slider_spiir_app.c
 * @brief  滑条 SPIIR 滤波器应用示例
 */

#include "slider_spiir_filter.h"
#include "rm_touch_api.h"
#include "Debug_UART/debug_uart.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* 外部声明 */
extern const touch_instance_t g_qe_touch_instance_config01;
extern uint32_t get_system_time_ms(void);  // 假设有这个函数获取系统时间

/* 常量定义 */
#define TOUCH_OFF_VAL 0xFFFFU
#define PRINT_INTERVAL_MS 200  // 打印间隔时间

/* 系数预设 */
typedef struct {
    const char* name;
    int16_t a_coef;
    int16_t b_coef;
    const char* description;
} spiir_preset_t;

/* 不同响应特性的系数预设 */
static const spiir_preset_t g_presets[] = {
    {
        .name = "平滑",
        .a_coef = 15729,  /* a = 0.96 */
        .b_coef = 655,    /* b = 0.04 */
        .description = "高度平滑，适合精确控制"
    },
    {
        .name = "标准",
        .a_coef = 15401,  /* a = 0.94 */
        .b_coef = 983,    /* b = 0.06 */
        .description = "平衡的平滑度和响应性"
    },
    {
        .name = "快速",
        .a_coef = 14746,  /* a = 0.90 */
        .b_coef = 1638,   /* b = 0.10 */
        .description = "较快响应，适合快速操作"
    },
    {
        .name = "极速",
        .a_coef = 13107,  /* a = 0.80 */
        .b_coef = 3277,   /* b = 0.20 */
        .description = "最快响应，但平滑度较低"
    }
};

#define NUM_PRESETS (sizeof(g_presets) / sizeof(g_presets[0]))

/* 全局变量 */
static uint8_t g_current_preset = 1;  // 默认使用"标准"预设
static char g_print_buffer[128];      // 打印缓冲区
static uint32_t g_last_print_time = 0;
static uint16_t g_last_raw_position = TOUCH_OFF_VAL;
static uint16_t g_last_filtered_position = TOUCH_OFF_VAL;
static bool g_debug_output = true;    // 是否输出调试信息

/**
 * @brief 切换到下一个预设
 * @return 当前预设名称
 */
const char* slider_spiir_switch_preset(void)
{
    g_current_preset = (g_current_preset + 1) % NUM_PRESETS;
    
    // 更新滤波器系数
    slider_spiir_update_coef(g_presets[g_current_preset].a_coef, 
                             g_presets[g_current_preset].b_coef);
    
    // 打印当前预设信息
    if (g_debug_output)
    {
        sprintf(g_print_buffer, "切换到预设: %s (a=0.%d, b=0.%d)\n", 
                g_presets[g_current_preset].name,
                (g_presets[g_current_preset].a_coef * 100) >> 14,
                (g_presets[g_current_preset].b_coef * 100) >> 14);
        DEBUG_PRINT(g_print_buffer);
        
        sprintf(g_print_buffer, "描述: %s\n", g_presets[g_current_preset].description);
        DEBUG_PRINT(g_print_buffer);
    }
    
    return g_presets[g_current_preset].name;
}

/**
 * @brief 设置指定的预设
 * @param preset_id 预设ID
 * @return 预设名称
 */
const char* slider_spiir_set_preset(uint8_t preset_id)
{
    if (preset_id >= NUM_PRESETS)
    {
        preset_id = 0;
    }
    
    g_current_preset = preset_id;
    
    // 更新滤波器系数
    slider_spiir_update_coef(g_presets[g_current_preset].a_coef, 
                             g_presets[g_current_preset].b_coef);
    
    // 打印当前预设信息
    if (g_debug_output)
    {
        sprintf(g_print_buffer, "设置预设: %s (a=0.%d, b=0.%d)\n", 
                g_presets[g_current_preset].name,
                (g_presets[g_current_preset].a_coef * 100) >> 14,
                (g_presets[g_current_preset].b_coef * 100) >> 14);
        DEBUG_PRINT(g_print_buffer);
    }
    
    return g_presets[g_current_preset].name;
}

/**
 * @brief 设置自定义系数
 * @param a_value 系数a的值 (0-100 表示 0.0-1.0)
 * @param b_value 系数b的值 (0-100 表示 0.0-1.0)
 * @note a_value + b_value 应等于 100
 */
void slider_spiir_set_custom_coef(uint8_t a_value, uint8_t b_value)
{
    // 确保 a + b = 100
    if (a_value + b_value != 100)
    {
        if (g_debug_output)
        {
            DEBUG_PRINT("错误: a + b 必须等于 100\n");
        }
        return;
    }
    
    // 转换为 Q14 格式
    int16_t a_coef = (int16_t)((a_value * 16384) / 100);
    int16_t b_coef = (int16_t)((b_value * 16384) / 100);
    
    // 更新滤波器系数
    slider_spiir_update_coef(a_coef, b_coef);
    
    // 打印信息
    if (g_debug_output)
    {
        sprintf(g_print_buffer, "设置自定义系数: a=0.%d, b=0.%d\n", a_value, b_value);
        DEBUG_PRINT(g_print_buffer);
    }
}

/**
 * @brief 处理滑条数据
 * @param raw_position 原始滑条位置
 * @return 滤波后的位置
 */
uint16_t slider_spiir_process(uint16_t raw_position)
{
    uint16_t filtered_position;
    
    // 应用 SPIIR 滤波
    filtered_position = slider_spiir_filter(raw_position);
    
    // 打印调试信息
    if (g_debug_output)
    {
        uint32_t current_time = get_system_time_ms();
        
        // 每隔一段时间打印一次位置信息（或者位置变化时）
        if ((current_time - g_last_print_time >= PRINT_INTERVAL_MS) || 
            (raw_position != g_last_raw_position) || 
            (filtered_position != g_last_filtered_position))
        {
            if (raw_position != TOUCH_OFF_VAL)
            {
                sprintf(g_print_buffer, "滑条: 原始=%3u, 滤波=%3u, 预设=%s\n", 
                        raw_position, filtered_position, g_presets[g_current_preset].name);
                DEBUG_PRINT(g_print_buffer);
            }
            else if (g_last_raw_position != TOUCH_OFF_VAL)
            {
                DEBUG_PRINT("滑条: 释放\n");
            }
            
            g_last_print_time = current_time;
            g_last_raw_position = raw_position;
            g_last_filtered_position = filtered_position;
        }
    }
    
    return filtered_position;
}

/**
 * @brief 启用/禁用调试输出
 * @param enable 是否启用
 */
void slider_spiir_debug_output(bool enable)
{
    g_debug_output = enable;
    
    if (enable)
    {
        DEBUG_PRINT("SPIIR滤波器调试输出已启用\n");
    }
}

/**
 * @brief 初始化 SPIIR 滤波器应用
 */
void slider_spiir_app_init(void)
{
    // 初始化 SPIIR 滤波器
    slider_spiir_init(g_presets[g_current_preset].a_coef, 
                      g_presets[g_current_preset].b_coef);
    
    // 打印初始化信息
    if (g_debug_output)
    {
        DEBUG_PRINT("\n===== 滑条 SPIIR 滤波器应用 =====\n");
        sprintf(g_print_buffer, "当前预设: %s (a=0.%d, b=0.%d)\n", 
                g_presets[g_current_preset].name,
                (g_presets[g_current_preset].a_coef * 100) >> 14,
                (g_presets[g_current_preset].b_coef * 100) >> 14);
        DEBUG_PRINT(g_print_buffer);
        DEBUG_PRINT("可通过按键或命令切换预设\n");
    }
}

/**
 * @brief 滑条 SPIIR 应用主处理函数
 * @return 滤波后的滑条位置
 */
uint16_t slider_spiir_app_process(void)
{
    uint64_t button_status;
    uint16_t slider_position;
    uint16_t wheel_position;
    uint16_t filtered_position;
    
    // 读取触摸数据
    RM_TOUCH_DataGet(&g_qe_touch_instance_config01, &button_status, &slider_position, &wheel_position);
    
    // 处理滑条数据
    filtered_position = slider_spiir_process(slider_position);
    
    return filtered_position;
}

/*
使用示例:

// 在初始化代码中
void app_init(void)
{
    // 其他初始化...
    
    // 初始化 SPIIR 滤波器应用
    slider_spiir_app_init();
}

// 在主循环中
void app_main_loop(void)
{
    // 获取滤波后的滑条位置
    uint16_t slider_position = slider_spiir_app_process();
    
    // 使用滤波后的滑条位置进行应用逻辑
    if (slider_position != TOUCH_OFF_VAL)
    {
        // 例如控制LED亮度
        set_led_brightness(slider_position);
    }
    else
    {
        // 无触摸时的处理
        // ...
    }
    
    // 检测按键切换预设（示例：使用第一个按键）
    static uint64_t last_button_status = 0;
    uint64_t button_status;
    RM_TOUCH_GetButtonStatus(&g_qe_touch_instance_config01, &button_status);
    
    if ((button_status & 0x01) && !(last_button_status & 0x01))
    {
        slider_spiir_switch_preset();
    }
    last_button_status = button_status;
}
*/
