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
#include "r_ctsu_iir_sample.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define IIR_SETTLINGS_MAX       (255)
#define IIR_SETTLINGS_MIN       (3)
#define IIR_CFG_DECIMAL_POINT   (11)

#define MAX_IIR_COEFFICIENT     ((int16_t)(0x1000))
#define MIN_IIR_COEFFICIENT     ((int16_t)(0xF000))

#define IIR_RESULT_MAX          ((int32_t)(0x0000FFFF))
#define IIR_RESULT_MIN          ((int32_t)(0x00000000))

#define IIR_RANGE_CHECK(a,max,min)                      \
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

/******************************************************************************
Typedef definitions
******************************************************************************/
enum
{
    E_COEF_B0 = 0,
    E_COEF_B1,
    E_COEF_B2,
    E_COEF_A1,
    E_COEF_A2,
};

enum
{
    E_BUF_B1 = 0,
    E_BUF_B2,
    E_BUF_A1,
    E_BUF_A2,
};

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
* Function Name: r_ctsu_iir_initial
* Description  : initializes control data of IIR filter
******************************************************************************/
fsp_err_t r_ctsu_iir_initial (iir_ctrl_t * p_ctrl, iir_config_t const * const p_cfg)
{
    uint16_t element_id;
    uint16_t taps_id;

    /* check configuration parameter */
    FSP_ERROR_RETURN(IIR_SETTLINGS_MAX >= p_cfg->settlings, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(IIR_SETTLINGS_MIN <= p_cfg->settlings, FSP_ERR_INVALID_ARGUMENT);
    for (taps_id = 0; taps_id < IIR_COEF_SIZE; taps_id++)
    {
        FSP_ERROR_RETURN(MAX_IIR_COEFFICIENT > p_cfg->coefficient[taps_id], FSP_ERR_INVALID_ARGUMENT);
        FSP_ERROR_RETURN(MIN_IIR_COEFFICIENT < p_cfg->coefficient[taps_id], FSP_ERR_INVALID_ARGUMENT);
    }

    /* initialize control data */
    p_ctrl->count = 0;
    for (element_id = 0; element_id < IIR_DATA_SIZE; element_id++)
    {
        for (taps_id = 0; taps_id < IIR_BUFFER_SIZE; taps_id++)
        {
            p_ctrl->iir_data[element_id][taps_id] = 0;
        }
    }

    return FSP_SUCCESS;
}

/******************************************************************************
* Function Name: ctsu_iir_filter
* Description  : calicurate IIR filter
******************************************************************************/
fsp_err_t r_ctsu_iir_filter (iir_ctrl_t * p_ctrl, iir_config_t const * const p_cfg, uint16_t *p_data)
{
    fsp_err_t ret = FSP_SUCCESS;
    uint16_t element_id;

    for (element_id = 0; element_id < IIR_DATA_SIZE; element_id++)
    {
        int32_t iir_sum;

        /* calculate IIR filter */
        iir_sum = (int32_t)((int32_t)p_ctrl->iir_data[element_id][E_BUF_A2] * (int32_t)p_cfg->coefficient[E_COEF_A2]);
        p_ctrl->iir_data[element_id][E_BUF_A2] = p_ctrl->iir_data[element_id][E_BUF_A1];
        iir_sum += (int32_t)((int32_t)p_ctrl->iir_data[element_id][E_BUF_A1] * (int32_t)p_cfg->coefficient[E_COEF_A1]);
        iir_sum += (int32_t)((int32_t)p_ctrl->iir_data[element_id][E_BUF_B2] * (int32_t)p_cfg->coefficient[E_COEF_B2]);
        p_ctrl->iir_data[element_id][E_BUF_B2] = p_ctrl->iir_data[element_id][E_BUF_B1];
        iir_sum += (int32_t)((int32_t)p_ctrl->iir_data[element_id][E_BUF_B1] * (int32_t)p_cfg->coefficient[E_COEF_B1]);
        p_ctrl->iir_data[element_id][E_BUF_B1] = p_data[element_id];
        iir_sum += (int32_t)((int32_t)p_data[element_id] * (int32_t)p_cfg->coefficient[E_COEF_B0]);

        /* saturation filterd data */
        iir_sum >>= IIR_CFG_DECIMAL_POINT;
        IIR_RANGE_CHECK(iir_sum, IIR_RESULT_MAX, IIR_RESULT_MIN);
        p_data[element_id] = (uint16_t)iir_sum;
        /* buffering output data */
        p_ctrl->iir_data[element_id][E_BUF_A1] = p_data[element_id];
    }
    /* check settling wait */
    if ((p_cfg->settlings - 1) > p_ctrl->count)
    {
        p_ctrl->count += 1;
        ret = FSP_ERR_BUFFER_EMPTY;
    }

    return ret;
}
