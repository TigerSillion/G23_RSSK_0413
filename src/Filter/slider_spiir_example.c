/**
 * @file   slider_spiir_example.c
 * @brief  滑条 SPIIR 滤波器使用示例
 */

#include "slider_spiir_filter.h"
#include <stdio.h>  // 仅用于示例中的打印

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
static uint8_t current_preset = 1;  // 默认使用"标准"预设

/**
 * @brief 示例：使用不同预设处理滑条数据
 * @param raw_position 原始滑条位置
 */
void slider_spiir_example_process(uint16_t raw_position)
{
    uint16_t filtered_position;

    // 使用当前预设的系数
    slider_spiir_update_coef(g_presets[current_preset].a_coef,
                             g_presets[current_preset].b_coef);

    // 应用滤波
    filtered_position = slider_spiir_filter(raw_position);

    // 打印结果（实际应用中可能不需要）
    printf("预设: %s, 原始位置: %u, 滤波后位置: %u\n",
           g_presets[current_preset].name,
           raw_position,
           filtered_position);
}

/**
 * @brief 示例：切换到下一个预设
 * @return 当前使用的预设名称
 */
const char* slider_spiir_example_next_preset(void)
{
    current_preset = (current_preset + 1) % NUM_PRESETS;

    // 更新滤波器系数
    slider_spiir_update_coef(g_presets[current_preset].a_coef,
                             g_presets[current_preset].b_coef);

    return g_presets[current_preset].name;
}

/**
 * @brief 示例：获取当前预设信息
 * @return 当前预设的描述
 */
const char* slider_spiir_example_get_preset_info(void)
{
    return g_presets[current_preset].description;
}

/**
 * @brief 示例：初始化 SPIIR 滤波器
 */
void slider_spiir_example_init(void)
{
    // 使用"标准"预设初始化
    uint8_t preset_id = 1;
    slider_spiir_init(g_presets[preset_id].a_coef, g_presets[preset_id].b_coef);
}

/*
示例使用:

// 初始化
slider_spiir_example_init();

// 在主循环中处理滑条数据
uint16_t raw_position = get_slider_position(); // 获取原始滑条位置
slider_spiir_example_process(raw_position);

// 切换预设（例如通过按钮触发）
if (button_pressed) {
    const char* preset_name = slider_spiir_example_next_preset();
    printf("切换到预设: %s\n", preset_name);
    printf("描述: %s\n", slider_spiir_example_get_preset_info());
}
*/
