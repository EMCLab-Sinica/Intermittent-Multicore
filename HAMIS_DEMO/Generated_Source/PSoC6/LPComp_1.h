/*******************************************************************************
* File Name: LPComp_1.h
* Version 1.10
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Low Power Comparator component.
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

#if !defined(CY_LPCOMP_LPComp_1_H)
#define CY_LPCOMP_LPComp_1_H

#include "lpcomp/cy_lpcomp.h"
#include "cyfitter.h"

/**
* \addtogroup group_globals
* @{
*/
extern uint8_t LPComp_1_initVar;
extern const cy_stc_lpcomp_config_t LPComp_1_config;
/** @} group_globals */

/**************************************
*        Function Prototypes
**************************************/
/**
* \addtogroup group_general
* @{
*/

void LPComp_1_Start(void);
__STATIC_INLINE void LPComp_1_Stop(void);
void LPComp_1_Init(void);
__STATIC_INLINE void LPComp_1_Enable(void);
__STATIC_INLINE void LPComp_1_SetInterruptTriggerMode(uint32 intType);
__STATIC_INLINE void LPComp_1_UlpReferenceEnable(void);
__STATIC_INLINE void LPComp_1_UlpReferenceDisable(void);
__STATIC_INLINE uint32 LPComp_1_GetCompare(void);
__STATIC_INLINE void LPComp_1_SetPower(uint32 power);
__STATIC_INLINE void LPComp_1_SetHysteresis(uint32 hysteresis);
__STATIC_INLINE void LPComp_1_SetInputs(cy_en_lpcomp_inputs_t inputP, cy_en_lpcomp_inputs_t inputN);
__STATIC_INLINE void LPComp_1_SetOutputMode(uint32 mode);
__STATIC_INLINE void LPComp_1_SetInterruptMask(uint32 interruptMask);
__STATIC_INLINE uint32 LPComp_1_GetInterruptMask(void);
__STATIC_INLINE uint32 LPComp_1_GetInterruptStatusMasked(void);
__STATIC_INLINE uint32 LPComp_1_GetInterruptStatus(void);
__STATIC_INLINE void LPComp_1_ClearInterrupt(uint32 interruptMask);
__STATIC_INLINE void LPComp_1_SetInterrupt(uint32 interruptMask);
/** @} general */

/**************************************
*           API Constants
**************************************/

#define LPComp_1_HW                             (LPComp_1_cy_mxs40_lpcomp__HW)
#define LPComp_1_IDX                            (LPComp_1_cy_mxs40_lpcomp__IDX)                                                                                           

/* Channel */
#define LPComp_1_CHANNEL ((cy_en_lpcomp_channel_t)((LPComp_1_IDX == 0) ? \
                                                        (CY_LPCOMP_CHANNEL_0) : (CY_LPCOMP_CHANNEL_1)))

/* Constants for LPComp_1_Start(), init parameter */
#define LPComp_1_NOT_INITIALIZED                (0x00u)
#define LPComp_1_INITIALIZED                    (0x01u)

/* Constant for LPComp_1_GetInterruptStatus() and 
* LPComp_1_ClearInterrupt(), interruptMask parameter 
*/
#define LPComp_1_INTR                           (uint32_t)((LPComp_1_IDX == 0) ? \
                                                                   (LPCOMP_INTR_COMP0_Msk) : (LPCOMP_INTR_COMP1_Msk))

/* Constant for LPComp_1_SetInterrupt(), interruptMask parameter */
#define LPComp_1_INTR_SET                       (uint32_t)((LPComp_1_IDX == 0) ? \
                                                                   (LPCOMP_INTR_SET_COMP0_Msk) : (LPCOMP_INTR_SET_COMP1_Msk))

/* Constant for LPComp_1_GetInterruptMask() and 
* LPComp_1_SetInterruptMask(), interruptMask parameter 
*/
#define LPComp_1_INTR_MASK                      (uint32_t)((LPComp_1_IDX == 0) ? \
                                                                   (LPCOMP_INTR_MASK_COMP0_MASK_Msk) : (LPCOMP_INTR_MASK_COMP1_MASK_Msk))

/* Constant for LPComp_1_GetInterruptStatusMasked() */ 
#define LPComp_1_INTR_MASKED                    (uint32_t)((LPComp_1_IDX == 0) ? \
                                                                   (LPCOMP_INTR_MASKED_COMP0_MASKED_Msk) : (LPCOMP_INTR_MASKED_COMP1_MASKED_Msk))

/***************************************
*   Initial Parameter Constants
****************************************/
    
#define LPComp_1_INTERRUPT                      (3u)
#define LPComp_1_POWER                          (2u)
#define LPComp_1_HYSTERESIS                     (1u)
#define LPComp_1_OUT_MODE                       (1u)
#define LPComp_1_LOCAL_VREF_INPUT               (0u)

/******************************************************************************
* Functions
*******************************************************************************/



/*******************************************************************************
* Function Name: LPComp_1_Enable
****************************************************************************//**
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call LPComp_1_Enable() because the LPComp_1_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_Enable(void)
{
    Cy_LPComp_Enable(LPComp_1_HW, LPComp_1_CHANNEL);
}


/*******************************************************************************
* Function Name: LPComp_1_SetInterruptTriggerMode
****************************************************************************//**
*
* Summary:
*  Sets the interrupt edge detect mode. This also controls the value provided
*  on the output.
*
* Parameters:
*  uint32 intType: Enumerated edge detect mode value:
*  CY_LPCOMP_INTR_DISABLE - Disable
*  CY_LPCOMP_INTR_RISING  - Rising edge detect
*  CY_LPCOMP_INTR_FALLING - Falling edge detect
*  CY_LPCOMP_INTR_BOTH    - Detect both edges
*
* Return:
*  None
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_SetInterruptTriggerMode(uint32 intType)
{
    Cy_LPComp_SetInterruptTriggerMode(LPComp_1_HW, LPComp_1_CHANNEL, (cy_en_lpcomp_int_t)intType);
}


/*******************************************************************************
* Function Name: LPComp_1_Stop
****************************************************************************//**
*
* Summary:
*  Turns off the LP Comparator.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  The function doesn't change component Speed settings.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_Stop(void)
{
    Cy_LPComp_SetInterruptTriggerMode(LPComp_1_HW, LPComp_1_CHANNEL, CY_LPCOMP_INTR_DISABLE);
    Cy_LPComp_SetPower(LPComp_1_HW, LPComp_1_CHANNEL, CY_LPCOMP_MODE_OFF);
}


/*******************************************************************************
* Function Name: LPComp_1_UlpReferenceEnable
****************************************************************************//**
*
* Invokes the Cy_LPComp_UlpReferenceEnable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_UlpReferenceEnable(void)
{
    Cy_LPComp_UlpReferenceEnable(LPComp_1_HW);
}


/*******************************************************************************
* Function Name: LPComp_1_UlpReferenceDisable
****************************************************************************//**
*
* Invokes the Cy_LPComp_UlpReferenceDisable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_UlpReferenceDisable(void)
{
    Cy_LPComp_UlpReferenceDisable(LPComp_1_HW);   
}


/*******************************************************************************
* Function Name: LPComp_1_GetCompare
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetCompare() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_1_GetCompare(void)
{
    return (Cy_LPComp_GetCompare(LPComp_1_HW, ((cy_en_lpcomp_channel_t)LPComp_1_CHANNEL))); 
}


/*******************************************************************************
* Function Name: LPComp_1_SetPower
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetPower() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_SetPower(uint32 power)
{
    Cy_LPComp_SetPower(LPComp_1_HW, LPComp_1_CHANNEL, (cy_en_lpcomp_pwr_t)power);
}


/*******************************************************************************
* Function Name: LPComp_1_SetHysteresis
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetHysteresis() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_SetHysteresis(uint32 hysteresis)
{
    Cy_LPComp_SetHysteresis(LPComp_1_HW, LPComp_1_CHANNEL, (cy_en_lpcomp_hyst_t) hysteresis); 
}


/*******************************************************************************
* Function Name: LPComp_1_SetInputs
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetInputs() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_SetInputs(cy_en_lpcomp_inputs_t inputP, cy_en_lpcomp_inputs_t inputN)
{
    Cy_LPComp_SetInputs(LPComp_1_HW, LPComp_1_CHANNEL, inputP, inputN); 
}


/*******************************************************************************
* Function Name: LPComp_1_SetOutputMode
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetOutputMode() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_SetOutputMode(uint32 mode)
{
    Cy_LPComp_SetOutputMode(LPComp_1_HW, LPComp_1_CHANNEL, (cy_en_lpcomp_out_t)mode);
}


/*******************************************************************************
* Function Name: LPComp_1_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_SetInterruptMask(uint32 interruptMask)
{
    Cy_LPComp_SetInterruptMask(LPComp_1_HW, interruptMask);
}


/*******************************************************************************
* Function Name: LPComp_1_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_1_GetInterruptMask(void)
{
    return (Cy_LPComp_GetInterruptMask(LPComp_1_HW));
}


/*******************************************************************************
* Function Name: LPComp_1_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_1_GetInterruptStatusMasked(void)
{
    return (Cy_LPComp_GetInterruptStatusMasked(LPComp_1_HW));
}


/*******************************************************************************
* Function Name: LPComp_1_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_1_GetInterruptStatus(void)
{
    return (Cy_LPComp_GetInterruptStatus(LPComp_1_HW));
}


/*******************************************************************************
* Function Name: LPComp_1_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_LPComp_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_ClearInterrupt(uint32 interruptMask)
{
    Cy_LPComp_ClearInterrupt(LPComp_1_HW, interruptMask);
}


/*******************************************************************************
* Function Name: LPComp_1_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_1_SetInterrupt(uint32 interruptMask)
{
    Cy_LPComp_SetInterrupt(LPComp_1_HW, interruptMask); 
}

#endif    /* CY_LPCOMP_LPComp_1_H */

/* [] END OF FILE */
