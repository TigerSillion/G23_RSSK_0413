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
#include "r_ctsu_spiir_sample.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SPIIR_SETTLINGS_MAX       (255)
#define SPIIR_SETTLINGS_MIN       (1)
#define SPIIR_CFG_DECIMAL_POINT   (14)

#define SPIIR_COEF_MAX    ((int16_t)(0x4000))
#define SPIIR_COEF_MIN    ((int16_t)(0xC000))

#define SPIIR_RESULT_MAX  ((int32_t)(0x0000FFFF))
#define SPIIR_RESULT_MIN  ((int32_t)(0x00000000))

#define SPIIR_RANGE_CHECK(a,max,min)                    \
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
    E_POS_A = 0,
    E_POS_B,
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
* Function Name: r_ctsu_spiir_initial
* Description  : initializes control data of SinglePoleIIR
******************************************************************************/
fsp_err_t r_ctsu_spiir_initial (spiir_ctrl_t * p_ctrl, spiir_config_t const * const p_cfg)
{
    uint16_t element_id;

    /* check configuration parameter */
    FSP_ERROR_RETURN(SPIIR_SETTLINGS_MAX >= p_cfg->settlings, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(SPIIR_SETTLINGS_MIN <= p_cfg->settlings, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(SPIIR_COEF_MAX > p_cfg->coefficient[E_POS_B], FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(SPIIR_COEF_MIN < p_cfg->coefficient[E_POS_B], FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(SPIIR_COEF_MAX > p_cfg->coefficient[E_POS_A], FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(SPIIR_COEF_MIN < p_cfg->coefficient[E_POS_A], FSP_ERR_INVALID_ARGUMENT);

    /* initialize control data */
    p_ctrl->count = 0;
    for (element_id = 0; element_id < SPIIR_DATA_SIZE; element_id++)
    {
        p_ctrl->spiir_data[element_id] = 0;
    }

    return FSP_SUCCESS;
}

/******************************************************************************
* Function Name: ctsu_spiir_filter
* Description  : calicurate SinglePoleIIR filter
******************************************************************************/
fsp_err_t r_ctsu_spiir_filter (spiir_ctrl_t * p_ctrl, spiir_config_t const * const p_cfg, uint16_t *p_data)
{
    fsp_err_t ret = FSP_SUCCESS;
    uint16_t element_id;

    for (element_id = 0; element_id < SPIIR_DATA_SIZE; element_id++)
    {
        int32_t spiir_sum;
        /* calculate Single Pole IIR filter */
        spiir_sum = (int32_t)((int32_t)p_data[element_id] * (int32_t)p_cfg->coefficient[E_POS_B]);
        spiir_sum += (int32_t)((int32_t)p_ctrl->spiir_data[element_id] * (int32_t)p_cfg->coefficient[E_POS_A]);
        /* saturation filterd data */
        spiir_sum >>= SPIIR_CFG_DECIMAL_POINT;
        SPIIR_RANGE_CHECK(spiir_sum, SPIIR_RESULT_MAX, SPIIR_RESULT_MIN);
        p_data[element_id] = (uint16_t)spiir_sum;
        /* buffering output data */
        p_ctrl->spiir_data[element_id] = p_data[element_id];
    }
    /* check settling wait */
    if ((p_cfg->settlings - 1) > p_ctrl->count)
    {
        p_ctrl->count += 1;
        ret = FSP_ERR_BUFFER_EMPTY;
    }

    return ret;
}
