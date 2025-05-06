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
#ifndef R_CTSU_MEDIAN_SAMPLE_H
#define R_CTSU_MEDIAN_SAMPLE_H

#include "filter_define_sample.h"

#define MEDIAN_SAMPLE_SIZE  (3)
#define MEDIAN_DATA_SIZE    (1)

typedef struct median_ctrl
{
    uint16_t                count;
    uint16_t                index;
    uint16_t                median_data[MEDIAN_DATA_SIZE][MEDIAN_SAMPLE_SIZE];
} median_ctrl_t;

fsp_err_t r_ctsu_median_initial (median_ctrl_t * p_ctrl);
fsp_err_t r_ctsu_median_filter (median_ctrl_t * p_ctrl, uint16_t *p_data);

extern median_ctrl_t g_ctsu_median_control;

#endif /* R_CTSU_MEDIAN_SAMPLE_H */

