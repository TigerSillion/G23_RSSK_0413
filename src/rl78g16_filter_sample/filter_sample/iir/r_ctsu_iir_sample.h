/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
* THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
************************************************************************************************************************/
#ifndef R_CTSU_IIR_SAMPLE_H
#define R_CTSU_IIR_SAMPLE_H

#include "filter_define_sample.h"

#define IIR_COEF_SIZE         (5)
#define IIR_BUFFER_SIZE       (4)
#define IIR_DATA_SIZE         (1)

typedef struct iir_config
{
    uint16_t                settlings;
    int16_t                 coefficient[IIR_COEF_SIZE];
} iir_config_t;

typedef struct iir_ctrl
{
    uint16_t                count;
    uint16_t                iir_data[IIR_DATA_SIZE][IIR_BUFFER_SIZE];
} iir_ctrl_t;

fsp_err_t r_ctsu_iir_initial (iir_ctrl_t * p_ctrl, iir_config_t const * const p_cfg);
fsp_err_t r_ctsu_iir_filter (iir_ctrl_t * p_ctrl, iir_config_t const * const p_cfg, uint16_t *p_data);

extern const iir_config_t g_iir_cfg1;
extern iir_ctrl_t g_ctsu_iir_control1;
extern const iir_config_t g_iir_cfg2;
extern iir_ctrl_t g_ctsu_iir_control2;

#endif /* R_CTSU_IIR_SAMPLE_H */
