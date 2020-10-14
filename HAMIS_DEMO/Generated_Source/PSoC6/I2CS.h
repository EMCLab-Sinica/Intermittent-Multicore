/***************************************************************************//**
* \file I2CS.c
* \version 2.0
*
*  This file provides constants and parameter values for the I2C component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(I2CS_CY_SCB_I2C_PDL_H)
#define I2CS_CY_SCB_I2C_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_i2c.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define I2CS_MODE               (0x1U)
#define I2CS_MODE_SLAVE_MASK    (0x1U)
#define I2CS_MODE_MASTER_MASK   (0x2U)

#define I2CS_ENABLE_SLAVE       (0UL != (I2CS_MODE & I2CS_MODE_SLAVE_MASK))
#define I2CS_ENABLE_MASTER      (0UL != (I2CS_MODE & I2CS_MODE_MASTER_MASK))
#define I2CS_MANUAL_SCL_CONTROL (0U)


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component only APIs. */
void I2CS_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_Init(cy_stc_scb_i2c_config_t const *config);
__STATIC_INLINE void I2CS_DeInit (void);
__STATIC_INLINE void I2CS_Enable (void);
__STATIC_INLINE void I2CS_Disable(void);

/* Data rate configuration functions. */
__STATIC_INLINE uint32_t I2CS_SetDataRate(uint32_t dataRateHz, uint32_t scbClockHz);
__STATIC_INLINE uint32_t I2CS_GetDataRate(uint32_t scbClockHz);

/* Register callbacks. */
__STATIC_INLINE void I2CS_RegisterEventCallback(cy_cb_scb_i2c_handle_events_t callback);
#if (I2CS_ENABLE_SLAVE)
__STATIC_INLINE void I2CS_RegisterAddrCallback (cy_cb_scb_i2c_handle_addr_t callback);
#endif /* (I2CS_ENABLE_SLAVE) */

/* Configuration functions. */
#if (I2CS_ENABLE_SLAVE)
__STATIC_INLINE void     I2CS_SlaveSetAddress(uint8_t addr);
__STATIC_INLINE uint32_t I2CS_SlaveGetAddress(void);
__STATIC_INLINE void     I2CS_SlaveSetAddressMask(uint8_t addrMask);
__STATIC_INLINE uint32_t I2CS_SlaveGetAddressMask(void);
#endif /* (I2CS_ENABLE_SLAVE) */

#if (I2CS_ENABLE_MASTER)
__STATIC_INLINE void I2CS_MasterSetLowPhaseDutyCycle (uint32_t clockCycles);
__STATIC_INLINE void I2CS_MasterSetHighPhaseDutyCycle(uint32_t clockCycles);
#endif /* (I2CS_ENABLE_MASTER) */

/* Bus status. */
__STATIC_INLINE bool     I2CS_IsBusBusy(void);

/* Slave functions. */
#if (I2CS_ENABLE_SLAVE)
__STATIC_INLINE uint32_t I2CS_SlaveGetStatus(void);

__STATIC_INLINE void     I2CS_SlaveConfigReadBuf(uint8_t *buffer, uint32_t size);
__STATIC_INLINE void     I2CS_SlaveAbortRead(void);
__STATIC_INLINE uint32_t I2CS_SlaveGetReadTransferCount(void);
__STATIC_INLINE uint32_t I2CS_SlaveClearReadStatus(void);

__STATIC_INLINE void     I2CS_SlaveConfigWriteBuf(uint8_t *buffer, uint32_t size);
__STATIC_INLINE void     I2CS_SlaveAbortWrite(void);
__STATIC_INLINE uint32_t I2CS_SlaveGetWriteTransferCount(void);
__STATIC_INLINE uint32_t I2CS_SlaveClearWriteStatus(void);
#endif /* (I2CS_ENABLE_SLAVE) */

/* Master interrupt processing functions. */
#if (I2CS_ENABLE_MASTER)
__STATIC_INLINE uint32_t I2CS_MasterGetStatus(void);

__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterRead(cy_stc_scb_i2c_master_xfer_config_t *xferConfig);
__STATIC_INLINE void I2CS_MasterAbortRead(void);
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterWrite(cy_stc_scb_i2c_master_xfer_config_t *xferConfig);
__STATIC_INLINE void I2CS_MasterAbortWrite(void);
__STATIC_INLINE uint32_t I2CS_MasterGetTransferCount(void);

/* Master manual processing functions. */
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterSendStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterSendReStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterSendStop(uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterReadByte(cy_en_scb_i2c_command_t ackNack, uint8_t *byte, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterWriteByte(uint8_t byte, uint32_t timeoutMs);
#endif /* (I2CS_ENABLE_MASTER) */

/* Interrupt handler. */
__STATIC_INLINE void I2CS_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t I2CS_initVar;
extern cy_stc_scb_i2c_config_t const I2CS_config;
extern cy_stc_scb_i2c_context_t I2CS_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the SCB instance */
#define I2CS_HW     ((CySCB_Type *) I2CS_SCB__HW)

/** The desired data rate in Hz */
#define I2CS_DATA_RATE_HZ      (100000U)

/** The frequency of the clock used by the Component in Hz */
#define I2CS_CLK_FREQ_HZ       (12500000U)

/** The number of Component clocks used by the master to generate the SCL
* low phase. This number is calculated by GUI based on the selected data rate.
*/
#define I2CS_LOW_PHASE_DUTY_CYCLE   (0U)

/** The number of Component clocks used by the master to generate the SCL
* high phase. This number is calculated by GUI based on the selected data rate.
*/
#define I2CS_HIGH_PHASE_DUTY_CYCLE  (0U)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: I2CS_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_Init(cy_stc_scb_i2c_config_t const *config)
{
    return Cy_SCB_I2C_Init(I2CS_HW, config, &I2CS_context);
}


/*******************************************************************************
*  Function Name: I2CS_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_DeInit(void)
{
    Cy_SCB_I2C_DeInit(I2CS_HW);
}


/*******************************************************************************
* Function Name: I2CS_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_Enable(void)
{
    Cy_SCB_I2C_Enable(I2CS_HW);
}


/*******************************************************************************
* Function Name: I2CS_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_Disable(void)
{
    Cy_SCB_I2C_Disable(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SetDataRate
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SetDataRate() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SetDataRate(uint32_t dataRateHz, uint32_t scbClockHz)
{
    return Cy_SCB_I2C_SetDataRate(I2CS_HW, dataRateHz, scbClockHz);
}


/*******************************************************************************
* Function Name: I2CS_GetDataRate
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_GetDataRate() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_GetDataRate(uint32_t scbClockHz)
{
    return Cy_SCB_I2C_GetDataRate(I2CS_HW, scbClockHz);
}


/*******************************************************************************
* Function Name: I2CS_RegisterEventCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_RegisterEventCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_RegisterEventCallback(cy_cb_scb_i2c_handle_events_t callback)
{
    Cy_SCB_I2C_RegisterEventCallback(I2CS_HW, callback, &I2CS_context);
}


#if (I2CS_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: I2CS_RegisterAddrCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_RegisterAddrCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_RegisterAddrCallback(cy_cb_scb_i2c_handle_addr_t callback)
{
    Cy_SCB_I2C_RegisterAddrCallback(I2CS_HW, callback, &I2CS_context);
}
#endif /* (I2CS_ENABLE_SLAVE) */


/*******************************************************************************
* Function Name: I2CS_IsBusBusy
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_IsBusBusy() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool I2CS_IsBusBusy(void)
{
    return Cy_SCB_I2C_IsBusBusy(I2CS_HW);
}


#if (I2CS_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: I2CS_SlaveSetAddress
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddress() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_SlaveSetAddress(uint8_t addr)
{
    Cy_SCB_I2C_SlaveSetAddress(I2CS_HW, addr);
}


/*******************************************************************************
* Function Name: I2CS_SlaveGetAddress
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddress() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SlaveGetAddress(void)
{
    return Cy_SCB_I2C_SlaveGetAddress(I2CS_HW);
}


/*******************************************************************************
* Function Name: I2CS_SlaveSetAddressMask
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveSetAddressMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_SlaveSetAddressMask(uint8_t addrMask)
{
    Cy_SCB_I2C_SlaveSetAddressMask(I2CS_HW, addrMask);
}


/*******************************************************************************
* Function Name: I2CS_SlaveGetAddressMask
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddressMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SlaveGetAddressMask(void)
{
    return Cy_SCB_I2C_SlaveGetAddressMask(I2CS_HW);
}
#endif /* (I2CS_ENABLE_SLAVE) */

#if (I2CS_ENABLE_MASTER)
/*******************************************************************************
* Function Name: I2CS_MasterSetLowPhaseDutyCycle
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSetLowPhaseDutyCycle() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_MasterSetLowPhaseDutyCycle(uint32_t clockCycles)
{
    Cy_SCB_I2C_MasterSetLowPhaseDutyCycle(I2CS_HW, clockCycles);
}


/*******************************************************************************
* Function Name: I2CS_MasterSetHighPhaseDutyCycle
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSetHighPhaseDutyCycle() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_MasterSetHighPhaseDutyCycle(uint32_t clockCycles)
{
    Cy_SCB_I2C_MasterSetHighPhaseDutyCycle(I2CS_HW, clockCycles);
}
#endif /* (I2CS_ENABLE_MASTER) */


#if (I2CS_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: I2CS_SlaveGetStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SlaveGetStatus(void)
{
    return Cy_SCB_I2C_SlaveGetStatus(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveConfigReadBuf
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveConfigReadBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_SlaveConfigReadBuf(uint8_t *buffer, uint32_t size)
{
    Cy_SCB_I2C_SlaveConfigReadBuf(I2CS_HW, buffer, size, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveAbortRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveAbortRead() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_SlaveAbortRead(void)
{
    Cy_SCB_I2C_SlaveAbortRead(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveGetReadTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetReadTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SlaveGetReadTransferCount(void)
{
    return Cy_SCB_I2C_SlaveGetReadTransferCount(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveClearReadStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveClearReadStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SlaveClearReadStatus(void)
{
    return Cy_SCB_I2C_SlaveClearReadStatus(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveConfigWriteBuf
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveConfigWriteBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_SlaveConfigWriteBuf(uint8_t *buffer, uint32_t size)
{
    Cy_SCB_I2C_SlaveConfigWriteBuf(I2CS_HW, buffer, size, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveAbortWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveAbortWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_SlaveAbortWrite(void)
{
    Cy_SCB_I2C_SlaveAbortWrite(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveGetWriteTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetWriteTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SlaveGetWriteTransferCount(void)
{
    return Cy_SCB_I2C_SlaveGetWriteTransferCount(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_SlaveClearWriteStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveClearWriteStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_SlaveClearWriteStatus(void)
{
    return Cy_SCB_I2C_SlaveClearWriteStatus(I2CS_HW, &I2CS_context);
}
#endif /* (I2CS_ENABLE_SLAVE) */


#if (I2CS_ENABLE_MASTER)
/*******************************************************************************
* Function Name: I2CS_MasterGetStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterGetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_MasterGetStatus(void)
{
    return Cy_SCB_I2C_MasterGetStatus(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterRead() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterRead(cy_stc_scb_i2c_master_xfer_config_t *xferConfig)
{
    return Cy_SCB_I2C_MasterRead(I2CS_HW, xferConfig, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterAbortRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterAbortRead() PDL driver function.
*
******************************************************************************/
__STATIC_INLINE void I2CS_MasterAbortRead(void)
{
    Cy_SCB_I2C_MasterAbortRead(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterWrite(cy_stc_scb_i2c_master_xfer_config_t *xferConfig)
{
    return Cy_SCB_I2C_MasterWrite(I2CS_HW, xferConfig, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterAbortWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterAbortWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_MasterAbortWrite(void)
{
    Cy_SCB_I2C_MasterAbortWrite(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterGetTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterGetTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2CS_MasterGetTransferCount(void)
{
    return Cy_SCB_I2C_MasterGetTransferCount(I2CS_HW, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterSendStart
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterSendStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendStart(I2CS_HW, address, bitRnW, timeoutMs, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterSendReStart
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendReStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterSendReStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendReStart(I2CS_HW, address, bitRnW, timeoutMs, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterSendStop
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendStop() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterSendStop(uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendStop(I2CS_HW, timeoutMs, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterReadByte
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterReadByte() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterReadByte(cy_en_scb_i2c_command_t ackNack, uint8_t *byte, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterReadByte(I2CS_HW, ackNack, byte, timeoutMs, &I2CS_context);
}


/*******************************************************************************
* Function Name: I2CS_MasterWriteByte
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterWriteByte() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t I2CS_MasterWriteByte(uint8_t byte, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterWriteByte(I2CS_HW, byte, timeoutMs, &I2CS_context);
}
#endif /* (I2CS_ENABLE_MASTER) */


/*******************************************************************************
* Function Name: I2CS_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2CS_Interrupt(void)
{
    Cy_SCB_I2C_Interrupt(I2CS_HW, &I2CS_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* I2CS_CY_SCB_I2C_PDL_H */


/* [] END OF FILE */
