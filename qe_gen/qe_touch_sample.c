/***********************************************************************
*
* FILE : qe_touch_sample.c
* DESCRIPTION : Main Program for RL78
*
* NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "qe_touch_config.h"
#include "../Debug_UART/debug_uart.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>  // 为了使用abs函数

/* 全局毫秒计数器的外部声明 */
extern volatile uint32_t g_msCounter;

#define TOUCH_SCAN_INTERVAL_EXAMPLE (5 * 1000)    /* microseconds */

void R_CTSU_PinSetInit(void);
void qe_touch_main(void);
void qe_touch_delay(uint16_t delay_us);

uint8_t Touch_scan_interval_flag = 0;

uint64_t button_status;
#if (TOUCH_CFG_NUM_SLIDERS != 0)
uint16_t slider_position[TOUCH_CFG_NUM_SLIDERS];
#endif
#if (TOUCH_CFG_NUM_WHEELS != 0)
uint16_t wheel_position[TOUCH_CFG_NUM_WHEELS];
#endif

float temp = 0;

/* 在适当位置添加这些变量 */
static uint16_t last_slider_position = 65535; // 上次滑条位置
static uint32_t last_slider_time = 0;         // 上次滑条读取时间
static float last_temp_value = 0.0f;          // 上次温度值
static bool print_enabled = true;             // 打印使能标志
static uint8_t no_touch_count = 0;            // 无触摸计数器
static bool was_touched = false;              // 上次是否有触摸

void qe_touch_main(void)
{
    fsp_err_t err;

    /* Main loop */
    if (Touch_scan_interval_flag == 1)
    {
        /* for [CONFIG01] configuration */
        err = RM_TOUCH_ScanStart (g_qe_touch_instance_config01.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }

        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        err = RM_TOUCH_DataGet (g_qe_touch_instance_config01.p_ctrl, NULL, slider_position, NULL);
        if (FSP_SUCCESS == err)
        {
            uint32_t current_time = g_msCounter;  // 获取当前时间
            
            if(slider_position[0] != 65535)
            {
                // 有触摸时，重置计数并启用打印
                no_touch_count = 0;
                print_enabled = true;
                was_touched = true;
                
                // 计算温度值
                // 滑条位置范围是0-100，需要映射到16-31度，共30个步进(0.5度一个)
                uint8_t temp_step = slider_position[0] * 30 / 100;  // 映射到0-30步
                temp = 16.0f + (float)temp_step * 0.5f;             // 每步0.5度
                
                // 计算位置差异和时间差（如果有上一次触摸的数据）
                int16_t position_diff = 0;
                float temp_diff = 0.0f;
                uint32_t time_diff = 0;
                
                if (last_slider_position != 65535) {
                    position_diff = (int16_t)slider_position[0] - (int16_t)last_slider_position;
                    temp_diff = temp - last_temp_value;
                    time_diff = current_time - last_slider_time;
                }
                
                // 简化输出格式，只打印最基本信息，避免乱码
                Debug_Send("Slider", DEBUG_COLOR_YELLOW, __FILE__, __func__, __LINE__, 
                          "Pos=%u Temp=%.1f ms=%lu\r\n", 
                          slider_position[0], temp, current_time);
                
                // 只有在位置有明显变化时才打印差异信息
                if (last_slider_position != 65535 && abs(position_diff) >= 1) {
                    Debug_Process(); // 确保上一条消息先发送
                    
                    // 打印位置差异和温度差异信息
                    float temp_step_diff = (float)temp_diff / 0.5f; // 转换为步数变化
                    Debug_Send("Slider", DEBUG_COLOR_CYAN, __FILE__, __func__, __LINE__, 
                              "Delta: Pos=%d Step=%.1f ms=%lu\r\n", 
                              position_diff, temp_step_diff, time_diff);
                }
                
                // 保存当前值作为下次比较
                last_slider_position = slider_position[0];
                last_slider_time = current_time;
                last_temp_value = temp;
            }
            else // 无触摸状态
            {
                // 如果上次有触摸，且这次无触摸，打印一次转换状态
                if (was_touched) {
                    Debug_Send("Slider", DEBUG_COLOR_MAGENTA, __FILE__, __func__, __LINE__, 
                              "Released Pos=%u ms=%lu\r\n",
                              last_slider_position, current_time);
                    was_touched = false;
                }
                
                // 控制无触摸时的打印频率
                if (print_enabled) {
                    no_touch_count++;
                    
                    // 每隔10次打印一次无触摸状态
                    if (no_touch_count % 10 == 1) {
                        Debug_Send("Slider", DEBUG_COLOR_CYAN, __FILE__, __func__, __LINE__, 
                                  "NoTouch cnt=%u ms=%lu\r\n", 
                                  no_touch_count, current_time);
                    }
                    
                    // 连续10次无触摸后停止打印
                    if (no_touch_count >= 10) {
                        print_enabled = false;
                        Debug_Send("Slider", DEBUG_COLOR_RED, __FILE__, __func__, __LINE__, 
                                  "Paused ms=%lu\r\n", current_time);
                    }
                }
                
                // 保存上次的无触摸状态
                last_slider_position = 65535;
            }
            
            /* TODO: Add your own code here. */
        }
        
        // 处理调试消息
        Debug_Process();
        
        Touch_scan_interval_flag = 0;

        /* FIXME: Since this is a temporary process, so re-create a waiting process yourself. */
     //   qe_touch_delay (TOUCH_SCAN_INTERVAL_EXAMPLE);
    }
}

void qe_touch_delay(uint16_t delay_us)
{
    uint32_t i;
    uint32_t loops_required;
    uint16_t clock_mhz;

    clock_mhz = (uint16_t) (R_BSP_GetFclkFreqHz () / 1000000);
    if (0 == clock_mhz)
    {
        clock_mhz = 1;
    }

    loops_required = ((uint32_t) delay_us * (uint32_t) clock_mhz);
    loops_required /= 20;
    for (i = 0; i < loops_required; i++)
    {
        BSP_NOP();
    }
}



