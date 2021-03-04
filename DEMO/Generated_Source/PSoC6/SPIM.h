/***************************************************************************//**
* \file SPIM.h
* \version 2.0
*
*  This file provides constants and parameter values for the SPI component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(SPIM_CY_SCB_SPI_PDL_H)
#define SPIM_CY_SCB_SPI_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_spi.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component specific functions. */
void SPIM_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_spi_status_t SPIM_Init(cy_stc_scb_spi_config_t const *config);
__STATIC_INLINE void SPIM_DeInit(void);
__STATIC_INLINE void SPIM_Enable(void);
__STATIC_INLINE void SPIM_Disable(void);

/* Register callback. */
__STATIC_INLINE void SPIM_RegisterCallback(cy_cb_scb_spi_handle_events_t callback);

/* Bus state. */
__STATIC_INLINE bool SPIM_IsBusBusy(void);

/* Slave select control. */
__STATIC_INLINE void SPIM_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect);
__STATIC_INLINE void SPIM_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity);

/* Low level: read. */
__STATIC_INLINE uint32_t SPIM_Read(void);
__STATIC_INLINE uint32_t SPIM_ReadArray(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t SPIM_GetRxFifoStatus(void);
__STATIC_INLINE void     SPIM_ClearRxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t SPIM_GetNumInRxFifo(void);
__STATIC_INLINE void     SPIM_ClearRxFifo(void);

/* Low level: write. */
__STATIC_INLINE uint32_t SPIM_Write(uint32_t data);
__STATIC_INLINE uint32_t SPIM_WriteArray(void *buffer, uint32_t size);
__STATIC_INLINE void     SPIM_WriteArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t SPIM_GetTxFifoStatus(void);
__STATIC_INLINE void     SPIM_ClearTxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t SPIM_GetNumInTxFifo(void);
__STATIC_INLINE bool     SPIM_IsTxComplete(void);
__STATIC_INLINE void     SPIM_ClearTxFifo(void);

/* Master/slave specific status. */
__STATIC_INLINE uint32_t SPIM_GetSlaveMasterStatus(void);
__STATIC_INLINE void     SPIM_ClearSlaveMasterStatus(uint32_t clearMask);

/* High level: transfer functions. */
__STATIC_INLINE cy_en_scb_spi_status_t SPIM_Transfer(void *txBuffer, void *rxBuffer, uint32_t size);
__STATIC_INLINE void     SPIM_AbortTransfer(void);
__STATIC_INLINE uint32_t SPIM_GetTransferStatus(void);
__STATIC_INLINE uint32_t SPIM_GetNumTransfered(void);

/* Interrupt handler */
__STATIC_INLINE void SPIM_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t SPIM_initVar;
extern cy_stc_scb_spi_config_t const SPIM_config;
extern cy_stc_scb_spi_context_t SPIM_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the hardware */
#define SPIM_HW     ((CySCB_Type *) SPIM_SCB__HW)

/** The slave select line 0 constant which takes into account pin placement */
#define SPIM_SPI_SLAVE_SELECT0    ( (cy_en_scb_spi_slave_select_t) SPIM_SCB__SS0_POSITION)

/** The slave select line 1 constant which takes into account pin placement */
#define SPIM_SPI_SLAVE_SELECT1    ( (cy_en_scb_spi_slave_select_t) SPIM_SCB__SS1_POSITION)

/** The slave select line 2 constant which takes into account pin placement */
#define SPIM_SPI_SLAVE_SELECT2    ( (cy_en_scb_spi_slave_select_t) SPIM_SCB__SS2_POSITION)

/** The slave select line 3 constant which takes into account pin placement */
#define SPIM_SPI_SLAVE_SELECT3    ((cy_en_scb_spi_slave_select_t) SPIM_SCB__SS3_POSITION)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: SPIM_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t SPIM_Init(cy_stc_scb_spi_config_t const *config)
{
    return Cy_SCB_SPI_Init(SPIM_HW, config, &SPIM_context);
}


/*******************************************************************************
* Function Name: SPIM_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_DeInit(void)
{
    Cy_SCB_SPI_DeInit(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_Enable(void)
{
    Cy_SCB_SPI_Enable(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_Disable(void)
{
    Cy_SCB_SPI_Disable(SPIM_HW, &SPIM_context);
}


/*******************************************************************************
* Function Name: SPIM_RegisterCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_RegisterCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_RegisterCallback(cy_cb_scb_spi_handle_events_t callback)
{
    Cy_SCB_SPI_RegisterCallback(SPIM_HW, callback, &SPIM_context);
}


/*******************************************************************************
* Function Name: SPIM_IsBusBusy
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsBusBusy() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool SPIM_IsBusBusy(void)
{
    return Cy_SCB_SPI_IsBusBusy(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_SetActiveSlaveSelect
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelect() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect)
{
    Cy_SCB_SPI_SetActiveSlaveSelect(SPIM_HW, slaveSelect);
}


/*******************************************************************************
* Function Name: SPIM_SetActiveSlaveSelectPolarity
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelectPolarity() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity)
{
    Cy_SCB_SPI_SetActiveSlaveSelectPolarity(SPIM_HW, slaveSelect, polarity);
}


/*******************************************************************************
* Function Name: SPIM_Read
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Read() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_Read(void)
{
    return Cy_SCB_SPI_Read(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_ReadArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ReadArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_ReadArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_ReadArray(SPIM_HW, buffer, size);
}


/*******************************************************************************
* Function Name: SPIM_GetRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_GetRxFifoStatus(void)
{
    return Cy_SCB_SPI_GetRxFifoStatus(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_ClearRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_ClearRxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearRxFifoStatus(SPIM_HW, clearMask);
}


/*******************************************************************************
* Function Name: SPIM_GetNumInRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_GetNumInRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_GetNumInRxFifo(void)
{
    return Cy_SCB_GetNumInRxFifo(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_ClearRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_ClearRxFifo(void)
{
    Cy_SCB_SPI_ClearRxFifo(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_Write
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Write() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_Write(uint32_t data)
{
    return Cy_SCB_SPI_Write(SPIM_HW, data);
}


/*******************************************************************************
* Function Name: SPIM_WriteArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_WriteArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_WriteArray(SPIM_HW, buffer, size);
}


/*******************************************************************************
* Function Name: SPIM_WriteArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_WriteArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_SPI_WriteArrayBlocking(SPIM_HW, buffer, size);
}


/*******************************************************************************
* Function Name: SPIM_GetTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_GetTxFifoStatus(void)
{
    return Cy_SCB_SPI_GetTxFifoStatus(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_ClearTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_ClearTxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearTxFifoStatus(SPIM_HW, clearMask);
}


/*******************************************************************************
* Function Name: SPIM_GetNumInTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumInTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_GetNumInTxFifo(void)
{
    return Cy_SCB_SPI_GetNumInTxFifo(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_IsTxComplete
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsTxComplete() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool SPIM_IsTxComplete(void)
{
    return Cy_SCB_SPI_IsTxComplete(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_ClearTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_ClearTxFifo(void)
{
    Cy_SCB_SPI_ClearTxFifo(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_GetSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_GetSlaveMasterStatus(void)
{
    return Cy_SCB_SPI_GetSlaveMasterStatus(SPIM_HW);
}


/*******************************************************************************
* Function Name: SPIM_ClearSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_ClearSlaveMasterStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearSlaveMasterStatus(SPIM_HW, clearMask);
}


/*******************************************************************************
* Function Name: SPIM_Transfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Transfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t SPIM_Transfer(void *txBuffer, void *rxBuffer, uint32_t size)
{
    return Cy_SCB_SPI_Transfer(SPIM_HW, txBuffer, rxBuffer, size, &SPIM_context);
}

/*******************************************************************************
* Function Name: SPIM_AbortTransfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_AbortTransfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_AbortTransfer(void)
{
    Cy_SCB_SPI_AbortTransfer(SPIM_HW, &SPIM_context);
}


/*******************************************************************************
* Function Name: SPIM_GetTransferStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTransferStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_GetTransferStatus(void)
{
    return Cy_SCB_SPI_GetTransferStatus(SPIM_HW, &SPIM_context);
}


/*******************************************************************************
* Function Name: SPIM_GetNumTransfered
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumTransfered() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIM_GetNumTransfered(void)
{
    return Cy_SCB_SPI_GetNumTransfered(SPIM_HW, &SPIM_context);
}


/*******************************************************************************
* Function Name: SPIM_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIM_Interrupt(void)
{
    Cy_SCB_SPI_Interrupt(SPIM_HW, &SPIM_context);
}


#if defined(__cplusplus)
}
#endif

#endif /* SPIM_CY_SCB_SPI_PDL_H */


/* [] END OF FILE */
