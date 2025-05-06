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
 IIR Notch Filter
***********************************************************************************************************************/
const iir_config_t g_iir_cfg1 = 
{
    .settlings = 3,
    .coefficient = 
    {
        /* coefficient b */
        2034,               /* b0 : 0.9931640625    */
        -1257,              /* b1 : -0.61376953125  */
        2034,               /* b2 : 0.9931640625    */
        /* coefficient a */
        1257,               /* a1 : 0.61376953125   */
        -2021,              /* a2 : -0.98681640625  */
    }
};

iir_ctrl_t g_ctsu_iir_control1;

/***********************************************************************************************************************
 IIR Low Pass Filter
***********************************************************************************************************************/
const iir_config_t g_iir_cfg2 = 
{
    .settlings = 48,
    .coefficient = 
    {
        /* coefficient b */
        27,                 /* b0 : 0.01318359375   */
        54,                 /* b1 : 0.0263671875    */
        27,                 /* b2 : 0.01318359375   */
        /* coefficient a */
        3373,               /* a1 : 1.64697265625   */
        -1435,              /* a2 : -0.70068359375  */
    }
};

iir_ctrl_t g_ctsu_iir_control2;
