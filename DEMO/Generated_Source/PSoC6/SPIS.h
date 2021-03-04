/***************************************************************************//**
* \file SPIS.h
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

#if !defined(SPIS_CY_SCB_SPI_PDL_H)
#define SPIS_CY_SCB_SPI_PDL_H

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
void SPIS_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_spi_status_t SPIS_Init(cy_stc_scb_spi_config_t const *config);
__STATIC_INLINE void SPIS_DeInit(void);
__STATIC_INLINE void SPIS_Enable(void);
__STATIC_INLINE void SPIS_Disable(void);

/* Register callback. */
__STATIC_INLINE void SPIS_RegisterCallback(cy_cb_scb_spi_handle_events_t callback);

/* Bus state. */
__STATIC_INLINE bool SPIS_IsBusBusy(void);

/* Slave select control. */
__STATIC_INLINE void SPIS_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect);
__STATIC_INLINE void SPIS_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity);

/* Low level: read. */
__STATIC_INLINE uint32_t SPIS_Read(void);
__STATIC_INLINE uint32_t SPIS_ReadArray(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t SPIS_GetRxFifoStatus(void);
__STATIC_INLINE void     SPIS_ClearRxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t SPIS_GetNumInRxFifo(void);
__STATIC_INLINE void     SPIS_ClearRxFifo(void);

/* Low level: write. */
__STATIC_INLINE uint32_t SPIS_Write(uint32_t data);
__STATIC_INLINE uint32_t SPIS_WriteArray(void *buffer, uint32_t size);
__STATIC_INLINE void     SPIS_WriteArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t SPIS_GetTxFifoStatus(void);
__STATIC_INLINE void     SPIS_ClearTxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t SPIS_GetNumInTxFifo(void);
__STATIC_INLINE bool     SPIS_IsTxComplete(void);
__STATIC_INLINE void     SPIS_ClearTxFifo(void);

/* Master/slave specific status. */
__STATIC_INLINE uint32_t SPIS_GetSlaveMasterStatus(void);
__STATIC_INLINE void     SPIS_ClearSlaveMasterStatus(uint32_t clearMask);

/* High level: transfer functions. */
__STATIC_INLINE cy_en_scb_spi_status_t SPIS_Transfer(void *txBuffer, void *rxBuffer, uint32_t size);
__STATIC_INLINE void     SPIS_AbortTransfer(void);
__STATIC_INLINE uint32_t SPIS_GetTransferStatus(void);
__STATIC_INLINE uint32_t SPIS_GetNumTransfered(void);

/* Interrupt handler */
__STATIC_INLINE void SPIS_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t SPIS_initVar;
extern cy_stc_scb_spi_config_t const SPIS_config;
extern cy_stc_scb_spi_context_t SPIS_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the hardware */
#define SPIS_HW     ((CySCB_Type *) SPIS_SCB__HW)

/** The slave select line 0 constant which takes into account pin placement */
#define SPIS_SPI_SLAVE_SELECT0    ( (cy_en_scb_spi_slave_select_t) SPIS_SCB__SS0_POSITION)

/** The slave select line 1 constant which takes into account pin placement */
#define SPIS_SPI_SLAVE_SELECT1    ( (cy_en_scb_spi_slave_select_t) SPIS_SCB__SS1_POSITION)

/** The slave select line 2 constant which takes into account pin placement */
#define SPIS_SPI_SLAVE_SELECT2    ( (cy_en_scb_spi_slave_select_t) SPIS_SCB__SS2_POSITION)

/** The slave select line 3 constant which takes into account pin placement */
#define SPIS_SPI_SLAVE_SELECT3    ((cy_en_scb_spi_slave_select_t) SPIS_SCB__SS3_POSITION)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: SPIS_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t SPIS_Init(cy_stc_scb_spi_config_t const *config)
{
    return Cy_SCB_SPI_Init(SPIS_HW, config, &SPIS_context);
}


/*******************************************************************************
* Function Name: SPIS_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_DeInit(void)
{
    Cy_SCB_SPI_DeInit(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_Enable(void)
{
    Cy_SCB_SPI_Enable(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_Disable(void)
{
    Cy_SCB_SPI_Disable(SPIS_HW, &SPIS_context);
}


/*******************************************************************************
* Function Name: SPIS_RegisterCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_RegisterCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_RegisterCallback(cy_cb_scb_spi_handle_events_t callback)
{
    Cy_SCB_SPI_RegisterCallback(SPIS_HW, callback, &SPIS_context);
}


/*******************************************************************************
* Function Name: SPIS_IsBusBusy
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsBusBusy() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool SPIS_IsBusBusy(void)
{
    return Cy_SCB_SPI_IsBusBusy(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_SetActiveSlaveSelect
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelect() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect)
{
    Cy_SCB_SPI_SetActiveSlaveSelect(SPIS_HW, slaveSelect);
}


/*******************************************************************************
* Function Name: SPIS_SetActiveSlaveSelectPolarity
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelectPolarity() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity)
{
    Cy_SCB_SPI_SetActiveSlaveSelectPolarity(SPIS_HW, slaveSelect, polarity);
}


/*******************************************************************************
* Function Name: SPIS_Read
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Read() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_Read(void)
{
    return Cy_SCB_SPI_Read(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_ReadArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ReadArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_ReadArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_ReadArray(SPIS_HW, buffer, size);
}


/*******************************************************************************
* Function Name: SPIS_GetRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_GetRxFifoStatus(void)
{
    return Cy_SCB_SPI_GetRxFifoStatus(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_ClearRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_ClearRxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearRxFifoStatus(SPIS_HW, clearMask);
}


/*******************************************************************************
* Function Name: SPIS_GetNumInRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_GetNumInRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_GetNumInRxFifo(void)
{
    return Cy_SCB_GetNumInRxFifo(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_ClearRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_ClearRxFifo(void)
{
    Cy_SCB_SPI_ClearRxFifo(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_Write
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Write() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_Write(uint32_t data)
{
    return Cy_SCB_SPI_Write(SPIS_HW, data);
}


/*******************************************************************************
* Function Name: SPIS_WriteArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_WriteArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_WriteArray(SPIS_HW, buffer, size);
}


/*******************************************************************************
* Function Name: SPIS_WriteArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_WriteArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_SPI_WriteArrayBlocking(SPIS_HW, buffer, size);
}


/*******************************************************************************
* Function Name: SPIS_GetTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_GetTxFifoStatus(void)
{
    return Cy_SCB_SPI_GetTxFifoStatus(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_ClearTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_ClearTxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearTxFifoStatus(SPIS_HW, clearMask);
}


/*******************************************************************************
* Function Name: SPIS_GetNumInTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumInTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_GetNumInTxFifo(void)
{
    return Cy_SCB_SPI_GetNumInTxFifo(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_IsTxComplete
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsTxComplete() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool SPIS_IsTxComplete(void)
{
    return Cy_SCB_SPI_IsTxComplete(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_ClearTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_ClearTxFifo(void)
{
    Cy_SCB_SPI_ClearTxFifo(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_GetSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_GetSlaveMasterStatus(void)
{
    return Cy_SCB_SPI_GetSlaveMasterStatus(SPIS_HW);
}


/*******************************************************************************
* Function Name: SPIS_ClearSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_ClearSlaveMasterStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearSlaveMasterStatus(SPIS_HW, clearMask);
}


/*******************************************************************************
* Function Name: SPIS_Transfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Transfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t SPIS_Transfer(void *txBuffer, void *rxBuffer, uint32_t size)
{
    return Cy_SCB_SPI_Transfer(SPIS_HW, txBuffer, rxBuffer, size, &SPIS_context);
}

/*******************************************************************************
* Function Name: SPIS_AbortTransfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_AbortTransfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_AbortTransfer(void)
{
    Cy_SCB_SPI_AbortTransfer(SPIS_HW, &SPIS_context);
}


/*******************************************************************************
* Function Name: SPIS_GetTransferStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTransferStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_GetTransferStatus(void)
{
    return Cy_SCB_SPI_GetTransferStatus(SPIS_HW, &SPIS_context);
}


/*******************************************************************************
* Function Name: SPIS_GetNumTransfered
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumTransfered() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t SPIS_GetNumTransfered(void)
{
    return Cy_SCB_SPI_GetNumTransfered(SPIS_HW, &SPIS_context);
}


/*******************************************************************************
* Function Name: SPIS_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void SPIS_Interrupt(void)
{
    Cy_SCB_SPI_Interrupt(SPIS_HW, &SPIS_context);
}


#if defined(__cplusplus)
}
#endif

#endif /* SPIS_CY_SCB_SPI_PDL_H */


/* [] END OF FILE */
