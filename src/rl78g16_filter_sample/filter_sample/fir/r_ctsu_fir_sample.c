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
#include "r_ctsu_fir_sample.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define FIR_TAP_SIZE_MIN         (2)
#define FIR_CFG_DECIMAL_POINT    (14)

#define MAX_FIR_COEFFICIENT_SUM     ((int32_t)(0x00008000))
#define MIN_FIR_COEFFICIENT_SUM     ((int32_t)(0xFFFF8000))
#define MAX_FIR_COEFFICIENT         ((int16_t)(0x4000))
#define MIN_FIR_COEFFICIENT         ((int16_t)(0xC000))

#define FIR_RESULT_MAX  ((int32_t)(0x0000FFFF))
#define FIR_RESULT_MIN  ((int32_t)(0x00000000))

#define FIR_RANGE_CHECK(a,max,min)                      \
    {                                                   \
        if ((a) > (max))                                \
        {                                               \
            (a) = (max);                                \
        }                                               \
        else if ((a) < (min))                           \
        {                                               \
            (a) = (min);                                \
        }                                               \
        else                                            \
        {                                               \
            (void) 0;                  /* Do nothing */ \
        }                                               \
    }

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/******************************************************************************
* Function Name: r_ctsu_fir_initial
* Description  : initializes control data of FIR filter
******************************************************************************/
fsp_err_t r_ctsu_fir_initial (fir_ctrl_t * p_ctrl, fir_config_t const * const p_cfg)
{
    uint16_t element_id;
    uint16_t taps_id;
    int32_t coef_sum = 0;

    /* check configuration parameter */
    FSP_ERROR_RETURN(FIR_TAP_SIZE_MIN <= p_cfg->taps, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(FIR_TAP_SIZE_MAX >= p_cfg->taps, FSP_ERR_INVALID_ARGUMENT);
    for (taps_id = 0; taps_id < p_cfg->taps; taps_id++)
    {
        FSP_ERROR_RETURN(MAX_FIR_COEFFICIENT > p_cfg->p_coefficient[taps_id], FSP_ERR_INVALID_ARGUMENT);
        FSP_ERROR_RETURN(MIN_FIR_COEFFICIENT < p_cfg->p_coefficient[taps_id], FSP_ERR_INVALID_ARGUMENT);
        coef_sum += (int32_t)p_cfg->p_coefficient[taps_id];
        FSP_ERROR_RETURN(MAX_FIR_COEFFICIENT_SUM > coef_sum, FSP_ERR_INVALID_ARGUMENT);
        FSP_ERROR_RETURN(MIN_FIR_COEFFICIENT_SUM < coef_sum, FSP_ERR_INVALID_ARGUMENT);
    }
    /* initialize control data */
    p_ctrl->count = 0;
    for (element_id = 0; element_id < FIR_DATA_SIZE; element_id++)
    {
        for (taps_id = 0; taps_id < p_cfg->taps; taps_id++)
        {
            p_ctrl->fir_data[element_id][taps_id] = 0;
        }
    }

    return FSP_SUCCESS;
}

/******************************************************************************
* Function Name: r_ctsu_fir_filter
* Description  : calicurate FIR filter
******************************************************************************/
fsp_err_t r_ctsu_fir_filter (fir_ctrl_t * p_ctrl, fir_config_t const * const p_cfg, uint16_t *p_data)
{
    fsp_err_t ret = FSP_SUCCESS;
    uint16_t element_id;

    for (element_id = 0; element_id < FIR_DATA_SIZE; element_id++)
    {
        uint16_t taps_id;
        int32_t fir_sum;
        /* buffering input data */
        p_ctrl->fir_data[element_id][0] = p_data[element_id];
        /* calculate FIR filter */
        fir_sum = 0;
        for (taps_id = 0; taps_id < p_cfg->taps; taps_id++)
        {
            fir_sum += (int32_t)((int32_t)p_ctrl->fir_data[element_id][taps_id] * (int32_t)p_cfg->p_coefficient[taps_id]);
        }
        /* saturation filterd data */
        fir_sum >>= FIR_CFG_DECIMAL_POINT;
        FIR_RANGE_CHECK(fir_sum, FIR_RESULT_MAX, FIR_RESULT_MIN);
        p_data[element_id] = (uint16_t)fir_sum;
        /* shift data buffer */
        for (taps_id = p_cfg->taps - 1; taps_id > 0; taps_id--)
        {
            p_ctrl->fir_data[element_id][taps_id] = p_ctrl->fir_data[element_id][taps_id - 1];
        }
    }
    /* check settling wait */
    if ((p_cfg->taps - 1) > p_ctrl->count)
    {
        p_ctrl->count += 1;
        ret = FSP_ERR_BUFFER_EMPTY;
    }

    return ret;
}
