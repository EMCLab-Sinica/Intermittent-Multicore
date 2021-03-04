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

#include "stdio.h"

#define MAXCOUNT 65536

/* Initial TWM and TCPWM */
void InitProfiler();
/* Overflow when counting every 65.536 ms */
void OverflowInterrupt();
/* Launch Test */
void GetTime(uint32_t* overflow, uint32_t* counter);
/* return total time */
uint32_t GetTotalTime();
/* Count the difference */
uint32_t CountTime(uint32_t Boverflow, uint32_t Bcounter);




/* [] END OF FILE */
