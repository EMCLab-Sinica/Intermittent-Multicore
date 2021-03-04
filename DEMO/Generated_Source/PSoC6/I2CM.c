/***************************************************************************//**
* \file I2CM.c
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

#include "I2CM.h"
#include "sysint/cy_sysint.h"
#include "cyfitter_sysint.h"
#include "cyfitter_sysint_cfg.h"


#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*     Global variables
***************************************/

/** I2CM_initVar indicates whether the I2CM
*  component has been initialized. The variable is initialized to 0
*  and set to 1 the first time I2CM_Start() is called.
*  This allows  the component to restart without reinitialization
*  after the first call to the I2CM_Start() routine.
*
*  If re-initialization of the component is required, then the
*  I2CM_Init() function can be called before the
*  I2CM_Start() or I2CM_Enable() function.
*/
uint8_t I2CM_initVar = 0U;

/** The instance-specific configuration structure.
* The pointer to this structure should be passed to Cy_SCB_I2C_Init function
* to initialize component with GUI selected settings.
*/
cy_stc_scb_i2c_config_t const I2CM_config =
{
    .i2cMode    = CY_SCB_I2C_MASTER,

    .useRxFifo = false,
    .useTxFifo = false,

    .slaveAddress        = 0U,
    .slaveAddressMask    = 0U,
    .acceptAddrInFifo    = false,
    .ackGeneralAddr      = false,

    .enableWakeFromSleep = false
};

/** The instance-specific context structure.
* It is used while the driver operation for internal configuration and
* data keeping for the I2C. The user should not modify anything in this
* structure.
*/
cy_stc_scb_i2c_context_t I2CM_context;


/*******************************************************************************
* Function Name: I2CM_Start
****************************************************************************//**
*
* Invokes I2CM_Init() and I2CM_Enable().
* Also configures interrupt and low and high oversampling phases.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref I2CM_initVar - used to check initial configuration,
* modified  on first function call.
*
*******************************************************************************/
void I2CM_Start(void)
{
    if (0U == I2CM_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_I2C_Init(I2CM_HW, &I2CM_config, &I2CM_context);

    #if (I2CM_ENABLE_MASTER)
        /* Configure desired data rate */
        (void) Cy_SCB_I2C_SetDataRate(I2CM_HW, I2CM_DATA_RATE_HZ, I2CM_CLK_FREQ_HZ);

        #if (I2CM_MANUAL_SCL_CONTROL)
            Cy_SCB_I2C_MasterSetLowPhaseDutyCycle (I2CM_HW, I2CM_LOW_PHASE_DUTY_CYCLE);
            Cy_SCB_I2C_MasterSetHighPhaseDutyCycle(I2CM_HW, I2CM_HIGH_PHASE_DUTY_CYCLE);
        #endif /* (I2CM_MANUAL_SCL_CONTROL) */
    #endif /* (I2CM_ENABLE_MASTER) */

        /* Hook interrupt service routine */
    #if defined(I2CM_SCB_IRQ__INTC_ASSIGNED)
        (void) Cy_SysInt_Init(&I2CM_SCB_IRQ_cfg, &I2CM_Interrupt);
    #endif /* (I2CM_SCB_IRQ__INTC_ASSIGNED) */

        I2CM_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
#if defined(I2CM_SCB_IRQ__INTC_ASSIGNED)
    NVIC_EnableIRQ((IRQn_Type) I2CM_SCB_IRQ_cfg.intrSrc);
#endif /* (I2CM_SCB_IRQ__INTC_ASSIGNED) */

    Cy_SCB_I2C_Enable(I2CM_HW);
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
