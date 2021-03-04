/*******************************************************************************
* File Name: LPComp_0.h
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

#if !defined(CY_LPCOMP_LPComp_0_H)
#define CY_LPCOMP_LPComp_0_H

#include "lpcomp/cy_lpcomp.h"
#include "cyfitter.h"

/**
* \addtogroup group_globals
* @{
*/
extern uint8_t LPComp_0_initVar;
extern const cy_stc_lpcomp_config_t LPComp_0_config;
/** @} group_globals */

/**************************************
*        Function Prototypes
**************************************/
/**
* \addtogroup group_general
* @{
*/

void LPComp_0_Start(void);
__STATIC_INLINE void LPComp_0_Stop(void);
void LPComp_0_Init(void);
__STATIC_INLINE void LPComp_0_Enable(void);
__STATIC_INLINE void LPComp_0_SetInterruptTriggerMode(uint32 intType);
__STATIC_INLINE void LPComp_0_UlpReferenceEnable(void);
__STATIC_INLINE void LPComp_0_UlpReferenceDisable(void);
__STATIC_INLINE uint32 LPComp_0_GetCompare(void);
__STATIC_INLINE void LPComp_0_SetPower(uint32 power);
__STATIC_INLINE void LPComp_0_SetHysteresis(uint32 hysteresis);
__STATIC_INLINE void LPComp_0_SetInputs(cy_en_lpcomp_inputs_t inputP, cy_en_lpcomp_inputs_t inputN);
__STATIC_INLINE void LPComp_0_SetOutputMode(uint32 mode);
__STATIC_INLINE void LPComp_0_SetInterruptMask(uint32 interruptMask);
__STATIC_INLINE uint32 LPComp_0_GetInterruptMask(void);
__STATIC_INLINE uint32 LPComp_0_GetInterruptStatusMasked(void);
__STATIC_INLINE uint32 LPComp_0_GetInterruptStatus(void);
__STATIC_INLINE void LPComp_0_ClearInterrupt(uint32 interruptMask);
__STATIC_INLINE void LPComp_0_SetInterrupt(uint32 interruptMask);
/** @} general */

/**************************************
*           API Constants
**************************************/

#define LPComp_0_HW                             (LPComp_0_cy_mxs40_lpcomp__HW)
#define LPComp_0_IDX                            (LPComp_0_cy_mxs40_lpcomp__IDX)                                                                                           

/* Channel */
#define LPComp_0_CHANNEL ((cy_en_lpcomp_channel_t)((LPComp_0_IDX == 0) ? \
                                                        (CY_LPCOMP_CHANNEL_0) : (CY_LPCOMP_CHANNEL_1)))

/* Constants for LPComp_0_Start(), init parameter */
#define LPComp_0_NOT_INITIALIZED                (0x00u)
#define LPComp_0_INITIALIZED                    (0x01u)

/* Constant for LPComp_0_GetInterruptStatus() and 
* LPComp_0_ClearInterrupt(), interruptMask parameter 
*/
#define LPComp_0_INTR                           (uint32_t)((LPComp_0_IDX == 0) ? \
                                                                   (LPCOMP_INTR_COMP0_Msk) : (LPCOMP_INTR_COMP1_Msk))

/* Constant for LPComp_0_SetInterrupt(), interruptMask parameter */
#define LPComp_0_INTR_SET                       (uint32_t)((LPComp_0_IDX == 0) ? \
                                                                   (LPCOMP_INTR_SET_COMP0_Msk) : (LPCOMP_INTR_SET_COMP1_Msk))

/* Constant for LPComp_0_GetInterruptMask() and 
* LPComp_0_SetInterruptMask(), interruptMask parameter 
*/
#define LPComp_0_INTR_MASK                      (uint32_t)((LPComp_0_IDX == 0) ? \
                                                                   (LPCOMP_INTR_MASK_COMP0_MASK_Msk) : (LPCOMP_INTR_MASK_COMP1_MASK_Msk))

/* Constant for LPComp_0_GetInterruptStatusMasked() */ 
#define LPComp_0_INTR_MASKED                    (uint32_t)((LPComp_0_IDX == 0) ? \
                                                                   (LPCOMP_INTR_MASKED_COMP0_MASKED_Msk) : (LPCOMP_INTR_MASKED_COMP1_MASKED_Msk))

/***************************************
*   Initial Parameter Constants
****************************************/
    
#define LPComp_0_INTERRUPT                      (3u)
#define LPComp_0_POWER                          (2u)
#define LPComp_0_HYSTERESIS                     (1u)
#define LPComp_0_OUT_MODE                       (1u)
#define LPComp_0_LOCAL_VREF_INPUT               (0u)

/******************************************************************************
* Functions
*******************************************************************************/



/*******************************************************************************
* Function Name: LPComp_0_Enable
****************************************************************************//**
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call LPComp_0_Enable() because the LPComp_0_Start() API
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
__STATIC_INLINE void LPComp_0_Enable(void)
{
    Cy_LPComp_Enable(LPComp_0_HW, LPComp_0_CHANNEL);
}


/*******************************************************************************
* Function Name: LPComp_0_SetInterruptTriggerMode
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
__STATIC_INLINE void LPComp_0_SetInterruptTriggerMode(uint32 intType)
{
    Cy_LPComp_SetInterruptTriggerMode(LPComp_0_HW, LPComp_0_CHANNEL, (cy_en_lpcomp_int_t)intType);
}


/*******************************************************************************
* Function Name: LPComp_0_Stop
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
__STATIC_INLINE void LPComp_0_Stop(void)
{
    Cy_LPComp_SetInterruptTriggerMode(LPComp_0_HW, LPComp_0_CHANNEL, CY_LPCOMP_INTR_DISABLE);
    Cy_LPComp_SetPower(LPComp_0_HW, LPComp_0_CHANNEL, CY_LPCOMP_MODE_OFF);
}


/*******************************************************************************
* Function Name: LPComp_0_UlpReferenceEnable
****************************************************************************//**
*
* Invokes the Cy_LPComp_UlpReferenceEnable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_UlpReferenceEnable(void)
{
    Cy_LPComp_UlpReferenceEnable(LPComp_0_HW);
}


/*******************************************************************************
* Function Name: LPComp_0_UlpReferenceDisable
****************************************************************************//**
*
* Invokes the Cy_LPComp_UlpReferenceDisable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_UlpReferenceDisable(void)
{
    Cy_LPComp_UlpReferenceDisable(LPComp_0_HW);   
}


/*******************************************************************************
* Function Name: LPComp_0_GetCompare
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetCompare() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_0_GetCompare(void)
{
    return (Cy_LPComp_GetCompare(LPComp_0_HW, ((cy_en_lpcomp_channel_t)LPComp_0_CHANNEL))); 
}


/*******************************************************************************
* Function Name: LPComp_0_SetPower
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetPower() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_SetPower(uint32 power)
{
    Cy_LPComp_SetPower(LPComp_0_HW, LPComp_0_CHANNEL, (cy_en_lpcomp_pwr_t)power);
}


/*******************************************************************************
* Function Name: LPComp_0_SetHysteresis
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetHysteresis() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_SetHysteresis(uint32 hysteresis)
{
    Cy_LPComp_SetHysteresis(LPComp_0_HW, LPComp_0_CHANNEL, (cy_en_lpcomp_hyst_t) hysteresis); 
}


/*******************************************************************************
* Function Name: LPComp_0_SetInputs
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetInputs() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_SetInputs(cy_en_lpcomp_inputs_t inputP, cy_en_lpcomp_inputs_t inputN)
{
    Cy_LPComp_SetInputs(LPComp_0_HW, LPComp_0_CHANNEL, inputP, inputN); 
}


/*******************************************************************************
* Function Name: LPComp_0_SetOutputMode
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetOutputMode() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_SetOutputMode(uint32 mode)
{
    Cy_LPComp_SetOutputMode(LPComp_0_HW, LPComp_0_CHANNEL, (cy_en_lpcomp_out_t)mode);
}


/*******************************************************************************
* Function Name: LPComp_0_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_SetInterruptMask(uint32 interruptMask)
{
    Cy_LPComp_SetInterruptMask(LPComp_0_HW, interruptMask);
}


/*******************************************************************************
* Function Name: LPComp_0_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_0_GetInterruptMask(void)
{
    return (Cy_LPComp_GetInterruptMask(LPComp_0_HW));
}


/*******************************************************************************
* Function Name: LPComp_0_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_0_GetInterruptStatusMasked(void)
{
    return (Cy_LPComp_GetInterruptStatusMasked(LPComp_0_HW));
}


/*******************************************************************************
* Function Name: LPComp_0_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_LPComp_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32 LPComp_0_GetInterruptStatus(void)
{
    return (Cy_LPComp_GetInterruptStatus(LPComp_0_HW));
}


/*******************************************************************************
* Function Name: LPComp_0_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_LPComp_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_ClearInterrupt(uint32 interruptMask)
{
    Cy_LPComp_ClearInterrupt(LPComp_0_HW, interruptMask);
}


/*******************************************************************************
* Function Name: LPComp_0_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_LPComp_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void LPComp_0_SetInterrupt(uint32 interruptMask)
{
    Cy_LPComp_SetInterrupt(LPComp_0_HW, interruptMask); 
}

#endif    /* CY_LPCOMP_LPComp_0_H */

/* [] END OF FILE */
