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
#ifndef R_CTSU_FIR_SAMPLE_H
#define R_CTSU_FIR_SAMPLE_H

#include "filter_define_sample.h"

#define FIR_TAP_SIZE_MAX        (9)
#define FIR_DATA_SIZE           (1)

typedef struct fir_config
{
    uint16_t            taps;
    int16_t     const   *p_coefficient;
} fir_config_t;

typedef struct fir_ctrl
{
    uint16_t            count;
    uint16_t            fir_data[FIR_DATA_SIZE][FIR_TAP_SIZE_MAX];
} fir_ctrl_t;

fsp_err_t r_ctsu_fir_initial (fir_ctrl_t * p_ctrl, fir_config_t const * const p_cfg);
fsp_err_t r_ctsu_fir_filter (fir_ctrl_t * p_ctrl, fir_config_t const * const p_cfg, uint16_t *p_data);

extern const fir_config_t g_fir_cfg;
extern fir_ctrl_t g_ctsu_fir_control;

#endif /* R_CTSU_FIR_SAMPLE_H */
