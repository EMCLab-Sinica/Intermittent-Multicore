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

#include "project.h"
#include "UARTM0.h"
#include "config.h"

/* Semaphore number to be used in this example */
#define SEMA_NUM    3u

void Uprintf(char* str)
{
    while(1){
        if (CY_IPC_SEMA_SUCCESS == Cy_IPC_Sema_Set(SEMA_NUM, false))
        {
            /* Send a message over the UART */
            Cy_SCB_UART_PutString(UART_HW, str);
            
            /* Unlock the semaphore */
            while (CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Clear(SEMA_NUM, false));  
            
            /* Wait a bit to avoid multiple messages to be sent */
            Cy_SysLib_Delay(1);
            return;
        }
        else
        {
            /* Semaphore is locked already, try again later */
        }        
    }     
}

void UARTInit()
{
    /* Initialize the UART block */
    Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    
    /* Enable the interrupt handler for UART */
    Cy_SysInt_Init(&UART_SCB_IRQ_cfg, &UART_Interrupt);

    /* Enable the UART block */
    Cy_SCB_UART_Enable(UART_HW);
    
    /* Send welcome message to UART */
    Cy_SCB_UART_PutString(UART_HW, "\r\n---- Uart Initialized ----\r\n");
    
}

void Udprintf(char * str)
{
#ifdef UARTDEBUG
    while(1){
        if (CY_IPC_SEMA_SUCCESS == Cy_IPC_Sema_Set(SEMA_NUM, false))
        {
            /* Send a message over the UART */
            Cy_SCB_UART_PutString(UART_HW, str);
            
            /* Unlock the semaphore */
            while (CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Clear(SEMA_NUM, false));  
            
            /* Wait a bit to avoid multiple messages to be sent */
            Cy_SysLib_Delay(1);
            return;
        }
        else
        {
            /* Semaphore is locked already, try again later */
        }        
    }
#endif
}

/* [] END OF FILE */
