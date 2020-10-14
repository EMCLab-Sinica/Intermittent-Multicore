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

#include "UARTM4.h"
#include "UartOutM4.h"
#define BLINK_INTERVAL 2000000
               
void UartTask()
{
    long x;
    
    while(1){
        for(x = 0; x < BLINK_INTERVAL; x++);
        Uprintf("Print from M4\r\n");   
    }
}