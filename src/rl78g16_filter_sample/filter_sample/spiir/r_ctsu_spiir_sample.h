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
#ifndef R_CTSU_SPIIR_SAMPLE_H
#define R_CTSU_SPIIR_SAMPLE_H

#include "filter_define_sample.h"

#define SPIIR_COEF_SIZE       (2)
#define SPIIR_DATA_SIZE       (1)

typedef struct spiir_config
{
    uint16_t            settlings;
    int16_t             coefficient[SPIIR_COEF_SIZE];
} spiir_config_t;

typedef struct spiir_ctrl
{
    uint16_t            count;
    uint16_t            spiir_data[SPIIR_DATA_SIZE];
} spiir_ctrl_t;

fsp_err_t r_ctsu_spiir_initial (spiir_ctrl_t * p_ctrl, spiir_config_t const * const p_cfg);
fsp_err_t r_ctsu_spiir_filter (spiir_ctrl_t * p_ctrl, spiir_config_t const * const p_cfg, uint16_t *p_data);

extern const spiir_config_t g_spiir_cfg;
extern spiir_ctrl_t g_ctsu_spiir_control;

#endif /* R_CTSU_SPIIR_SAMPLE_H */
