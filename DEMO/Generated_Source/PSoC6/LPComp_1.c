/*******************************************************************************
* File Name: LPComp_1.c
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

#include "LPComp_1.h"

/** LPComp_1_initVar indicates whether the LPComp_1  component
*  has been initialized. The variable is initialized to 0 and set to 1 the first
*  time LPComp_1_Start() is called.
*  This allows the component to restart without reinitialization after the first 
*  call to the LPComp_1_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  LPComp_1_Init() function can be called before the 
*  LPComp_1_Start() or LPComp_1_Enable() function.
*/
uint8_t LPComp_1_initVar = LPComp_1_NOT_INITIALIZED;

/** The instance-specific configuration structure. This should be used in the 
*  associated LPComp_1_Init() function.
*/
const cy_stc_lpcomp_config_t LPComp_1_config =
{
    (cy_en_lpcomp_out_t)LPComp_1_OUT_MODE,
    (cy_en_lpcomp_hyst_t)LPComp_1_HYSTERESIS,
    (cy_en_lpcomp_pwr_t)LPComp_1_POWER,
    (cy_en_lpcomp_int_t)LPComp_1_INTERRUPT
};

/*******************************************************************************
* Function Name: LPComp_1_Start
****************************************************************************//**
*
* Summary:
*  Performs all of the required initialization for the component and enables
*  power to the block. The first time the routine is executed, the component is
*  initialized to the configuration from the customizer. When called to restart
*  the comparator following a LPComp_1_Stop() call, the current
*  component parameter settings are retained.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  LPComp_1_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void LPComp_1_Start(void)
{
    if (LPComp_1_NOT_INITIALIZED == LPComp_1_initVar)
    {
        LPComp_1_Init();
        LPComp_1_initVar = LPComp_1_INITIALIZED;
    }
    LPComp_1_Enable();
}


/*******************************************************************************
* Function Name: LPComp_1_Init
****************************************************************************//**
*
* Summary:
*  Initializes or restores the component according to the customizer settings.
*  It is not necessary to call LPComp_1_Init() because the
*  LPComp_1_Start() API calls this function and is the preferred method
*  to begin component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LPComp_1_Init(void)
{
    uint32_t interruptMask;
    
    (void)Cy_LPComp_Init(LPComp_1_HW, LPComp_1_CHANNEL, &LPComp_1_config);

    interruptMask = LPComp_1_GetInterruptMask();
    LPComp_1_SetInterruptMask(interruptMask | LPComp_1_INTR_MASK);
    
    #if (0u != LPComp_1_LOCAL_VREF_INPUT)
        Cy_LPComp_ConnectULPReference(LPComp_1_HW, LPComp_1_CHANNEL);
        Cy_LPComp_UlpReferenceEnable(LPComp_1_HW);
    #endif 
}


/* [] END OF FILE */
