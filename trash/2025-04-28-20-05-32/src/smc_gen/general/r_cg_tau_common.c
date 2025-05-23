/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2021, 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : r_cg_tau_common.c
* Version          : 1.0.20
* Device(s)        : R7F100GSNxFB
* Description      : None
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_TAU0_1.h"
#include "r_cg_tau_common.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TAU0_Create
* Description  : This function enables TAU0 input clock supply and initializes TAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Create(void)
{
    TAU0EN = 1U;    /* start TAU0 clock */
    /* Set TAU0 settings */
    R_Config_TAU0_1_Create();
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Create
* Description  : This function enables TAU1 input clock supply and initializes TAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Create(void)
{
    TAU1EN = 1U;    /* start TAU1 clock */
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Set_PowerOn
* Description  : This function starts the clock supply for TAU0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Set_PowerOn(void)
{
    TAU0EN = 1U;    /* start TAU0 clock */
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Set_PowerOff
* Description  : This function stops the clock supply for TAU0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Set_PowerOff(void)
{
    TAU0EN = 0U;    /* stop TAU0 clock */
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Set_Reset
* Description  : This function sets TAU0 module in reset state.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Set_Reset(void)
{
    TAU0RES = 1U;    /* reset TAU0 */
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Release_Reset
* Description  : This function releases TAU0 module from reset state.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Release_Reset(void)
{
    TAU0RES = 0U;    /* release TAU0 */
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Set_PowerOn
* Description  : This function starts the clock supply for TAU1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Set_PowerOn(void)
{
    TAU1EN = 1U;    /* start TAU1 clock */
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Set_PowerOff
* Description  : This function stops the clock supply for TAU1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Set_PowerOff(void)
{
    TAU1EN = 0U;    /* stop TAU1 clock */
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Set_Reset
* Description  : This function sets TAU1 module in reset state.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Set_Reset(void)
{
    TAU1RES = 1U;    /* reset TAU1 */
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Release_Reset
* Description  : This function releases TAU1 module from reset state.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Release_Reset(void)
{
    TAU1RES = 0U;    /* release TAU1 */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
