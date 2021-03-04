/*******************************************************************************
* File Name: Counter.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the Counter
*  component
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter.h"

/** Indicates whether or not the Counter has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  Counter_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the Counter_Start() routine.
*/
uint8_t Counter_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated Counter_Init() function.
*/ 
cy_stc_tcpwm_counter_config_t const Counter_config =
{
        .period = 65535UL,
        .clockPrescaler = 0UL,
        .runMode = 0UL,
        .countDirection = 0UL,
        .compareOrCapture = 2UL,
        .compare0 = 16384UL,
        .compare1 = 16384UL,
        .enableCompareSwap = false,
        .interruptSources = 1UL,
        .captureInputMode = 3UL,
        .captureInput = CY_TCPWM_INPUT_CREATOR,
        .reloadInputMode = 3UL,
        .reloadInput = CY_TCPWM_INPUT_CREATOR,
        .startInputMode = 3UL,
        .startInput = CY_TCPWM_INPUT_CREATOR,
        .stopInputMode = 3UL,
        .stopInput = CY_TCPWM_INPUT_CREATOR,
        .countInputMode = 0UL,
        .countInput = CY_TCPWM_INPUT_CREATOR,
};


/*******************************************************************************
* Function Name: Counter_Start
****************************************************************************//**
*
*  Calls the Counter_Init() when called the first time and enables 
*  the Counter. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref Counter_initVar
*
*******************************************************************************/
void Counter_Start(void)
{
    if (0U == Counter_initVar)
    {
        (void)Cy_TCPWM_Counter_Init(Counter_HW, Counter_CNT_NUM, &Counter_config); 

        Counter_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(Counter_HW, Counter_CNT_MASK);
    
    #if (Counter_INPUT_DISABLED == 7UL)
        Cy_TCPWM_TriggerStart(Counter_HW, Counter_CNT_MASK);
    #endif /* (Counter_INPUT_DISABLED == 7UL) */    
}


/* [] END OF FILE */
