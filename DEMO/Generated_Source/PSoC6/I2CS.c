/***************************************************************************//**
* \file I2CS.c
* \version 2.0
*
*  This file provides the source code to the API for the I2C Component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2CS.h"
#include "sysint/cy_sysint.h"
#include "cyfitter_sysint.h"
#include "cyfitter_sysint_cfg.h"


#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*     Global variables
***************************************/

/** I2CS_initVar indicates whether the I2CS
*  component has been initialized. The variable is initialized to 0
*  and set to 1 the first time I2CS_Start() is called.
*  This allows  the component to restart without reinitialization
*  after the first call to the I2CS_Start() routine.
*
*  If re-initialization of the component is required, then the
*  I2CS_Init() function can be called before the
*  I2CS_Start() or I2CS_Enable() function.
*/
uint8_t I2CS_initVar = 0U;

/** The instance-specific configuration structure.
* The pointer to this structure should be passed to Cy_SCB_I2C_Init function
* to initialize component with GUI selected settings.
*/
cy_stc_scb_i2c_config_t const I2CS_config =
{
    .i2cMode    = CY_SCB_I2C_SLAVE,

    .useRxFifo = false,
    .useTxFifo = false,

    .slaveAddress        = 0x8U,
    .slaveAddressMask    = 0xFEU,
    .acceptAddrInFifo    = false,
    .ackGeneralAddr      = false,

    .enableWakeFromSleep = false
};

/** The instance-specific context structure.
* It is used while the driver operation for internal configuration and
* data keeping for the I2C. The user should not modify anything in this
* structure.
*/
cy_stc_scb_i2c_context_t I2CS_context;


/*******************************************************************************
* Function Name: I2CS_Start
****************************************************************************//**
*
* Invokes I2CS_Init() and I2CS_Enable().
* Also configures interrupt and low and high oversampling phases.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref I2CS_initVar - used to check initial configuration,
* modified  on first function call.
*
*******************************************************************************/
void I2CS_Start(void)
{
    if (0U == I2CS_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_I2C_Init(I2CS_HW, &I2CS_config, &I2CS_context);

    #if (I2CS_ENABLE_MASTER)
        /* Configure desired data rate */
        (void) Cy_SCB_I2C_SetDataRate(I2CS_HW, I2CS_DATA_RATE_HZ, I2CS_CLK_FREQ_HZ);

        #if (I2CS_MANUAL_SCL_CONTROL)
            Cy_SCB_I2C_MasterSetLowPhaseDutyCycle (I2CS_HW, I2CS_LOW_PHASE_DUTY_CYCLE);
            Cy_SCB_I2C_MasterSetHighPhaseDutyCycle(I2CS_HW, I2CS_HIGH_PHASE_DUTY_CYCLE);
        #endif /* (I2CS_MANUAL_SCL_CONTROL) */
    #endif /* (I2CS_ENABLE_MASTER) */

        /* Hook interrupt service routine */
    #if defined(I2CS_SCB_IRQ__INTC_ASSIGNED)
        (void) Cy_SysInt_Init(&I2CS_SCB_IRQ_cfg, &I2CS_Interrupt);
    #endif /* (I2CS_SCB_IRQ__INTC_ASSIGNED) */

        I2CS_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
#if defined(I2CS_SCB_IRQ__INTC_ASSIGNED)
    NVIC_EnableIRQ((IRQn_Type) I2CS_SCB_IRQ_cfg.intrSrc);
#endif /* (I2CS_SCB_IRQ__INTC_ASSIGNED) */

    Cy_SCB_I2C_Enable(I2CS_HW);
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
