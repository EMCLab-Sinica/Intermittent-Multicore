/*******************************************************************************
* File Name: Counter.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the Counter
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Counter_CY_TCPWM_COUNTER_PDL_H)
#define Counter_CY_TCPWM_COUNTER_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_counter.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  Counter_initVar;
extern cy_stc_tcpwm_counter_config_t const Counter_config;
/** @} group_globals */


/***************************************
*  Conditional Compilation Parameters
***************************************/

#define Counter_INIT_COMPARE_OR_CAPTURE    (2UL)


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void Counter_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t Counter_Init(cy_stc_tcpwm_counter_config_t const *config);
__STATIC_INLINE void Counter_DeInit(void);
__STATIC_INLINE void Counter_Enable(void);
__STATIC_INLINE void Counter_Disable(void);
__STATIC_INLINE uint32_t Counter_GetStatus(void);

#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Counter_INIT_COMPARE_OR_CAPTURE)
    __STATIC_INLINE uint32_t Counter_GetCapture(void);
    __STATIC_INLINE uint32_t Counter_GetCaptureBuf(void);
#else
    __STATIC_INLINE void Counter_SetCompare0(uint32_t compare0);
    __STATIC_INLINE uint32_t Counter_GetCompare0(void);
    __STATIC_INLINE void Counter_SetCompare1(uint32_t compare1);
    __STATIC_INLINE uint32_t Counter_GetCompare1(void);
    __STATIC_INLINE void Counter_EnableCompareSwap(bool enable);
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Counter_INIT_COMPARE_OR_CAPTURE) */

__STATIC_INLINE void Counter_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t Counter_GetCounter(void);
__STATIC_INLINE void Counter_SetPeriod(uint32_t period);
__STATIC_INLINE uint32_t Counter_GetPeriod(void);
__STATIC_INLINE void Counter_TriggerStart(void);
__STATIC_INLINE void Counter_TriggerReload(void);
__STATIC_INLINE void Counter_TriggerStop(void);
__STATIC_INLINE void Counter_TriggerCapture(void);
__STATIC_INLINE uint32_t Counter_GetInterruptStatus(void);
__STATIC_INLINE void Counter_ClearInterrupt(uint32_t source);
__STATIC_INLINE void Counter_SetInterrupt(uint32_t source);
__STATIC_INLINE void Counter_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t Counter_GetInterruptMask(void);
__STATIC_INLINE uint32_t Counter_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define Counter_HW                 (Counter_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define Counter_CNT_HW             (Counter_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define Counter_CNT_NUM            (Counter_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define Counter_CNT_MASK           (1UL << Counter_CNT_NUM)
/** @} group_macros */

#define Counter_INPUT_MODE_MASK    (0x3U)
#define Counter_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: Counter_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t Counter_Init(cy_stc_tcpwm_counter_config_t const *config)
{
    return(Cy_TCPWM_Counter_Init(Counter_HW, Counter_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: Counter_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_DeInit(void)                   
{
    Cy_TCPWM_Counter_DeInit(Counter_HW, Counter_CNT_NUM, &Counter_config);
}

/*******************************************************************************
* Function Name: Counter_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(Counter_HW, Counter_CNT_MASK);
}


/*******************************************************************************
* Function Name: Counter_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(Counter_HW, Counter_CNT_MASK);
}


/*******************************************************************************
* Function Name: Counter_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Counter_GetStatus(void)                
{
    return(Cy_TCPWM_Counter_GetStatus(Counter_HW, Counter_CNT_NUM));
}


#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Counter_INIT_COMPARE_OR_CAPTURE)
    /*******************************************************************************
    * Function Name: Counter_GetCapture
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCapture() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Counter_GetCapture(void)               
    {
        return(Cy_TCPWM_Counter_GetCapture(Counter_HW, Counter_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Counter_GetCaptureBuf
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCaptureBuf() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Counter_GetCaptureBuf(void)            
    {
        return(Cy_TCPWM_Counter_GetCaptureBuf(Counter_HW, Counter_CNT_NUM));
    }
#else
    /*******************************************************************************
    * Function Name: Counter_SetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Counter_SetCompare0(uint32_t compare0)      
    {
        Cy_TCPWM_Counter_SetCompare0(Counter_HW, Counter_CNT_NUM, compare0);
    }


    /*******************************************************************************
    * Function Name: Counter_GetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Counter_GetCompare0(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare0(Counter_HW, Counter_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Counter_SetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Counter_SetCompare1(uint32_t compare1)      
    {
        Cy_TCPWM_Counter_SetCompare1(Counter_HW, Counter_CNT_NUM, compare1);
    }


    /*******************************************************************************
    * Function Name: Counter_GetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Counter_GetCompare1(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare1(Counter_HW, Counter_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Counter_EnableCompareSwap
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_EnableCompareSwap() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Counter_EnableCompareSwap(bool enable)  
    {
        Cy_TCPWM_Counter_EnableCompareSwap(Counter_HW, Counter_CNT_NUM, enable);
    }
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Counter_INIT_COMPARE_OR_CAPTURE) */


/*******************************************************************************
* Function Name: Counter_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_SetCounter(uint32_t count)          
{
    Cy_TCPWM_Counter_SetCounter(Counter_HW, Counter_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: Counter_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Counter_GetCounter(void)               
{
    return(Cy_TCPWM_Counter_GetCounter(Counter_HW, Counter_CNT_NUM));
}


/*******************************************************************************
* Function Name: Counter_SetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_SetPeriod(uint32_t period)          
{
    Cy_TCPWM_Counter_SetPeriod(Counter_HW, Counter_CNT_NUM, period);
}


/*******************************************************************************
* Function Name: Counter_GetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Counter_GetPeriod(void)                
{
    return(Cy_TCPWM_Counter_GetPeriod(Counter_HW, Counter_CNT_NUM));
}


/*******************************************************************************
* Function Name: Counter_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(Counter_HW, Counter_CNT_MASK);
}


/*******************************************************************************
* Function Name: Counter_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(Counter_HW, Counter_CNT_MASK);
}


/*******************************************************************************
* Function Name: Counter_TriggerStop
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_TriggerStop(void)
{
    Cy_TCPWM_TriggerStopOrKill(Counter_HW, Counter_CNT_MASK);
}


/*******************************************************************************
* Function Name: Counter_TriggerCapture
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_TriggerCapture(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(Counter_HW, Counter_CNT_MASK);
}


/*******************************************************************************
* Function Name: Counter_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Counter_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(Counter_HW, Counter_CNT_NUM));
}


/*******************************************************************************
* Function Name: Counter_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(Counter_HW, Counter_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Counter_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(Counter_HW, Counter_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Counter_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Counter_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(Counter_HW, Counter_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: Counter_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Counter_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(Counter_HW, Counter_CNT_NUM));
}


/*******************************************************************************
* Function Name: Counter_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Counter_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(Counter_HW, Counter_CNT_NUM));
}

#endif /* Counter_CY_TCPWM_COUNTER_PDL_H */


/* [] END OF FILE */
