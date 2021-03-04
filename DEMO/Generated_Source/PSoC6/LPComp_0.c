/*******************************************************************************
* File Name: LPComp_0.c
* Version 1.10
*
* Description:
*  This file provides the source code to the API for the Low Power Comparator
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LPComp_0.h"

/** LPComp_0_initVar indicates whether the LPComp_0  component
*  has been initialized. The variable is initialized to 0 and set to 1 the first
*  time LPComp_0_Start() is called.
*  This allows the component to restart without reinitialization after the first 
*  call to the LPComp_0_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  LPComp_0_Init() function can be called before the 
*  LPComp_0_Start() or LPComp_0_Enable() function.
*/
uint8_t LPComp_0_initVar = LPComp_0_NOT_INITIALIZED;

/** The instance-specific configuration structure. This should be used in the 
*  associated LPComp_0_Init() function.
*/
const cy_stc_lpcomp_config_t LPComp_0_config =
{
    (cy_en_lpcomp_out_t)LPComp_0_OUT_MODE,
    (cy_en_lpcomp_hyst_t)LPComp_0_HYSTERESIS,
    (cy_en_lpcomp_pwr_t)LPComp_0_POWER,
    (cy_en_lpcomp_int_t)LPComp_0_INTERRUPT
};

/*******************************************************************************
* Function Name: LPComp_0_Start
****************************************************************************//**
*
* Summary:
*  Performs all of the required initialization for the component and enables
*  power to the block. The first time the routine is executed, the component is
*  initialized to the configuration from the customizer. When called to restart
*  the comparator following a LPComp_0_Stop() call, the current
*  component parameter settings are retained.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  LPComp_0_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void LPComp_0_Start(void)
{
    if (LPComp_0_NOT_INITIALIZED == LPComp_0_initVar)
    {
        LPComp_0_Init();
        LPComp_0_initVar = LPComp_0_INITIALIZED;
    }
    LPComp_0_Enable();
}


/*******************************************************************************
* Function Name: LPComp_0_Init
****************************************************************************//**
*
* Summary:
*  Initializes or restores the component according to the customizer settings.
*  It is not necessary to call LPComp_0_Init() because the
*  LPComp_0_Start() API calls this function and is the preferred method
*  to begin component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_0_Init(void)
{
    uint32_t interruptMask;
    
    (void)Cy_LPComp_Init(LPComp_0_HW, LPComp_0_CHANNEL, &LPComp_0_config);

    interruptMask = LPComp_0_GetInterruptMask();
    LPComp_0_SetInterruptMask(interruptMask | LPComp_0_INTR_MASK);
    
    #if (0u != LPComp_0_LOCAL_VREF_INPUT)
        Cy_LPComp_ConnectULPReference(LPComp_0_HW, LPComp_0_CHANNEL);
        Cy_LPComp_UlpReferenceEnable(LPComp_0_HW);
    #endif 
}


/* [] END OF FILE */
