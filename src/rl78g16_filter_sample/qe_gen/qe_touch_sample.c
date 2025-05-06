/***********************************************************************
*
* FILE : qe_sample_main.c
* DATE : 2022-02-14
* DESCRIPTION : Main Program for RL78
*
* NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "qe_touch_config.h"
#include "Config_TAU0_0.h"
#include ".\fir\r_ctsu_fir_sample.h"
#include ".\iir\r_ctsu_iir_sample.h"
#include ".\spiir\r_ctsu_spiir_sample.h"
#include ".\median\r_ctsu_median_sample.h"

void R_CTSU_PinSetInit(void);
void qe_touch_main(void);


uint64_t button_status;
#if (TOUCH_CFG_NUM_SLIDERS != 0)
uint16_t slider_position[TOUCH_CFG_NUM_SLIDERS];
#endif
#if (TOUCH_CFG_NUM_WHEELS != 0)
uint16_t wheel_position[TOUCH_CFG_NUM_WHEELS];
#endif



#define TOUCH_SCAN_CYCLE ((20 * 1000)/100) /* 20ms with 100us timer*/
uint16_t touch_cycle_count = 0;
volatile uint8_t touch_cycle_flag = 0;

void qe_touch_main(void)
{
    fsp_err_t err;
    uint16_t filter_buffer[1];

    BSP_ENABLE_INTERRUPT();

    /* Initialize pins (function created by Smart Configurator) */
    R_CTSU_PinSetInit();


    /* Start timer */
    R_Config_TAU0_0_Start();

    /* Open Touch middleware */
    err = RM_TOUCH_Open(g_qe_touch_instance_config01.p_ctrl, g_qe_touch_instance_config01.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = RM_TOUCH_Open(g_qe_touch_instance_config02.p_ctrl, g_qe_touch_instance_config02.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = RM_TOUCH_Open(g_qe_touch_instance_config03.p_ctrl, g_qe_touch_instance_config03.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = RM_TOUCH_Open(g_qe_touch_instance_config04.p_ctrl, g_qe_touch_instance_config04.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = RM_TOUCH_Open(g_qe_touch_instance_config05.p_ctrl, g_qe_touch_instance_config05.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }

    /* Check filter configuration & Initialize filter control data */
    err = r_ctsu_fir_initial(&g_ctsu_fir_control, &g_fir_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = r_ctsu_iir_initial(&g_ctsu_iir_control1, &g_iir_cfg1);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = r_ctsu_iir_initial(&g_ctsu_iir_control2, &g_iir_cfg2);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = r_ctsu_spiir_initial(&g_ctsu_spiir_control, &g_spiir_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = r_ctsu_median_initial(&g_ctsu_median_control);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }



    /* Main loop */
    while (true)
    {
        /* for [CONFIG01] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config01.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        /* FIR moving average filter sample */
        err = R_CTSU_DataGet(g_qe_ctsu_instance_config01.p_ctrl, filter_buffer);
        if (FSP_SUCCESS == err)
        {
            err = r_ctsu_fir_filter(&g_ctsu_fir_control,&g_fir_cfg, filter_buffer);
            if (FSP_SUCCESS == err)
            {
                R_CTSU_DataInsert(g_qe_ctsu_instance_config01.p_ctrl, filter_buffer);
                err = RM_TOUCH_DataGet(g_qe_touch_instance_config01.p_ctrl, &button_status, NULL, NULL);
                if (FSP_SUCCESS == err)
                {
                    /* TODO: Add your own code here. */
                }
            }
        }

        /* for [CONFIG02] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config02.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        /* IIR notch filter sample */
        err = R_CTSU_DataGet(g_qe_ctsu_instance_config02.p_ctrl, filter_buffer);
        if (FSP_SUCCESS == err)
        {
            err = r_ctsu_iir_filter(&g_ctsu_iir_control1, &g_iir_cfg1, filter_buffer);
            if (FSP_SUCCESS == err)
            {
                R_CTSU_DataInsert(g_qe_ctsu_instance_config02.p_ctrl, filter_buffer);
                err = RM_TOUCH_DataGet(g_qe_touch_instance_config02.p_ctrl, &button_status, NULL, NULL);
                if (FSP_SUCCESS == err)
                {
                    /* TODO: Add your own code here. */
                }
            }
        }

        /* for [CONFIG03] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config03.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        /* IIR low pass filter sample */
        err = R_CTSU_DataGet(g_qe_ctsu_instance_config03.p_ctrl, filter_buffer);
        if (FSP_SUCCESS == err)
        {
            err = r_ctsu_iir_filter(&g_ctsu_iir_control2, &g_iir_cfg2, filter_buffer);
            if (FSP_SUCCESS == err)
            {
                R_CTSU_DataInsert(g_qe_ctsu_instance_config03.p_ctrl, filter_buffer);
                err = RM_TOUCH_DataGet(g_qe_touch_instance_config03.p_ctrl, &button_status, NULL, NULL);
                if (FSP_SUCCESS == err)
                {
                    /* TODO: Add your own code here. */
                }
            }
        }

        /* for [CONFIG04] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config04.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        /* Single Pole IIR low pass filter sample */
        err = R_CTSU_DataGet(g_qe_ctsu_instance_config04.p_ctrl, filter_buffer);
        if (FSP_SUCCESS == err)
        {
            err = r_ctsu_spiir_filter(&g_ctsu_spiir_control, &g_spiir_cfg, filter_buffer);
            if (FSP_SUCCESS == err)
            {
                R_CTSU_DataInsert(g_qe_ctsu_instance_config04.p_ctrl, filter_buffer);
                err = RM_TOUCH_DataGet(g_qe_touch_instance_config04.p_ctrl, &button_status, NULL, NULL);
                if (FSP_SUCCESS == err)
                {
                    /* TODO: Add your own code here. */
                }
            }
        }

        /* for [CONFIG05] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config05.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        /* Median filter sample */
        err = R_CTSU_DataGet(g_qe_ctsu_instance_config05.p_ctrl, filter_buffer);
        if (FSP_SUCCESS == err)
        {
            err = r_ctsu_median_filter(&g_ctsu_median_control, filter_buffer);
            if (FSP_SUCCESS == err)
            {
                R_CTSU_DataInsert(g_qe_ctsu_instance_config05.p_ctrl, filter_buffer);
                err = RM_TOUCH_DataGet(g_qe_touch_instance_config05.p_ctrl, &button_status, NULL, NULL);
                if (FSP_SUCCESS == err)
                {
                    /* TODO: Add your own code here. */
                }
            }
        }

        /* FIXME: Since this is a temporary process, so re-create a waiting process yourself. */
        while (0 == touch_cycle_flag) {}
        touch_cycle_flag = 0;
    }
}

void r_timer_callback(void)
{
    /* Cap Touch cycle count */
    touch_cycle_count++;
    if (TOUCH_SCAN_CYCLE <= touch_cycle_count)
    {
        touch_cycle_count = 0;
        touch_cycle_flag = 1;
    }
}
