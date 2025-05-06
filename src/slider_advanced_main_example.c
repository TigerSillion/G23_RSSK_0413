/**
 * @file   slider_advanced_main_example.c
 * @brief  滑条高级滤波器主程序示例
 */

#include "slider_advanced_app.h"
#include "rm_touch_api.h"
#include "Debug_UART/debug_uart.h"
#include <stdio.h>

/* 外部声明 */
extern const touch_instance_t g_qe_touch_instance_config01;

/* 全局变量 */
static uint32_t g_ms_counter = 0;

/* 获取系统时间 (ms) */
uint32_t get_system_time_ms(void)
{
    return g_ms_counter;
}

/* 定时器中断处理函数 */
void timer_interrupt_handler(void)
{
    g_ms_counter++;
}

/**
 * @brief 主函数
 */
int main(void)
{
    fsp_err_t err;
    
    /* 初始化系统 */
    EI();
    R_Config_ITL000_ITL001_ITL012_ITL013_Start();
    R_Config_TAU0_1_Start();
    R_Config_UARTA1_Start();
    
    /* 初始化触摸引脚 */
    R_CTSU_PinSetInit();
    
    /* 初始化调试串口 */
    DebugConfig_t cfg = 
    {
        .tsFormat        = DEBUG_TS_MS,
        .minIntervalMs   = 0,
        .maxProcessTimeUs= 1000,
        .bufferSize      = 128,
        .queueDepth      = 128,
        .enableCRC       = false,
        .enableColor     = true
    };
    DEBUG_Init(&cfg);
    
    /* 打开触摸中间件 */
    err = RM_TOUCH_Open(g_qe_touch_instance_config01.p_ctrl, g_qe_touch_instance_config01.p_cfg);
    if (FSP_SUCCESS != err)
    {
        DEBUG_PRINT("触摸初始化失败\n");
        while (true) {}
    }
    
    /* 初始化高级滤波器应用 */
    slider_advanced_app_init();
    
    DEBUG_PRINT("系统初始化完成\n");
    DEBUG_PRINT("滑条高级滤波器示例\n");
    DEBUG_PRINT("可通过触摸滑条测试不同滤波器的效果\n");
    
    /* 主循环 */
    while (1)
    {
        /* 启动触摸扫描 */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config01.p_ctrl);
        
        /* 等待扫描完成 */
        while (FSP_SUCCESS != RM_TOUCH_DataGet(g_qe_touch_instance_config01.p_ctrl, NULL, NULL, NULL))
        {
            /* 等待触摸扫描完成 */
        }
        
        /* 处理滑条数据 */
        uint16_t slider_position = slider_advanced_app_process();
        
        /* 使用滤波后的滑条位置进行应用逻辑 */
        /* 这里可以添加您的应用代码，例如控制LED亮度等 */
        
        /* 检测按键切换预设 */
        static uint64_t last_button_status = 0;
        uint64_t button_status;
        RM_TOUCH_DataGet(g_qe_touch_instance_config01.p_ctrl, &button_status, NULL, NULL);
        
        if ((button_status & 0x01) && !(last_button_status & 0x01))
        {
            slider_advanced_switch_preset();
        }
        last_button_status = button_status;
        
        /* 延时一段时间 */
        R_BSP_SoftwareDelay(10, BSP_DELAY_MILLISECS);
    }
    
    return 0;
}
