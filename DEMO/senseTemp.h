/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#include <project.h>
#include <stdio.h>
#include "sar/cy_sar.h"
#include "sysanalog/cy_sysanalog.h"
#include "sysclk/cy_sysclk.h"


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void SAR_Interrupt(void);
void MCWDT_Interrupt(void);
int32_t DieTemp_CountsTo_Celsius(int16_t adcCounts);
int senseTemp(void *ptr);
