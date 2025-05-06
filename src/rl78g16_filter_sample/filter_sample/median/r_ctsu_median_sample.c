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
#include "r_ctsu_median_sample.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define MEDIAN_SAMPLE_SIZE_MIN      (3)
#define MEDIAN_SAMPLE_SIZE_MAX      (9)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void ctsu_insert_sort(uint16_t * p_list , uint16_t s);
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
* Function Name: r_ctsu_median_initial
* Description  : initializes control data of MEDIAN filter
******************************************************************************/
fsp_err_t r_ctsu_median_initial (median_ctrl_t * p_ctrl)
{
    uint16_t element_id;
    uint16_t samples_id;

    /* check configuration parameter */
    FSP_ERROR_RETURN(MEDIAN_SAMPLE_SIZE_MIN <= MEDIAN_SAMPLE_SIZE, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(MEDIAN_SAMPLE_SIZE_MAX >= MEDIAN_SAMPLE_SIZE, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(1 == (MEDIAN_SAMPLE_SIZE & 0x01), FSP_ERR_INVALID_ARGUMENT);

    /* initialize control data */
    p_ctrl->count = 0;
    p_ctrl->index = 0;
    for (element_id = 0; element_id < MEDIAN_DATA_SIZE; element_id++)
    {
        for (samples_id = 0; samples_id < MEDIAN_SAMPLE_SIZE; samples_id++)
        {
            p_ctrl->median_data[element_id][samples_id] = 0;
        }
    }

    return FSP_SUCCESS;
}

/******************************************************************************
* Function Name: ctsu_median_filter
* Description  : calicurate MEDIAN filter
******************************************************************************/
fsp_err_t r_ctsu_median_filter (median_ctrl_t * p_ctrl, uint16_t *p_data)
{
    fsp_err_t ret = FSP_SUCCESS;
    uint16_t element_id;
    uint16_t samples_id;
    uint16_t median_work[MEDIAN_SAMPLE_SIZE];
    uint16_t index = p_ctrl->index;

    /* update ring buffer index */
    p_ctrl->index++;
    if (MEDIAN_SAMPLE_SIZE <= p_ctrl->index)
    {
        p_ctrl->index = 0;
    }

    for (element_id = 0; element_id < MEDIAN_DATA_SIZE; element_id++)
    {
        /* buffering input data */
        p_ctrl->median_data[element_id][index] = p_data[element_id];
        /* copy sampling data to working buffer */
        for (samples_id = 0; samples_id < MEDIAN_SAMPLE_SIZE; samples_id++)
        {
            median_work[samples_id] = p_ctrl->median_data[element_id][samples_id];
        }
        /* calculate MEDIAN filter */
        ctsu_insert_sort(median_work, MEDIAN_SAMPLE_SIZE);
        p_data[element_id] = median_work[MEDIAN_SAMPLE_SIZE / 2];
    }
    /* check settling wait */
    if ((MEDIAN_SAMPLE_SIZE - 1) > p_ctrl->count)
    {
        p_ctrl->count += 1;
        ret = FSP_ERR_BUFFER_EMPTY;
    }

    return ret;
}

/******************************************************************************
* Function Name: ctsu_insert_sort
* Description  : insert sort function
******************************************************************************/
static void ctsu_insert_sort(uint16_t * p_list , uint16_t size)
{
    uint16_t sort_idx;
    uint16_t ins_idx;
    uint16_t tmp;

    /* insert sort */
    for (sort_idx = 1; sort_idx < size; sort_idx++)
    {
        if (p_list[sort_idx - 1] > p_list[sort_idx])
        {
            ins_idx = sort_idx;
            tmp = p_list[sort_idx];
            do
            {
                p_list[ins_idx] = p_list[ins_idx - 1];
                ins_idx--;
            } while ((ins_idx > 0) && (p_list[ins_idx - 1] > tmp));
            p_list[ins_idx] = tmp;
        }
    }
}
