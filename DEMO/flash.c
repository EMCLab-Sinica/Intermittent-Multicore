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
#include "flash.h"
#include "rtos.h"

/* Check TRM details to get this number. 512 byte for a row */
#define SIZEOFROW               512u

/* buffered write on flash*/
#define MAXBUFF 20
static void* fromAdd[MAXBUFF];
static void* toAdd[MAXBUFF];
static int rowTotal[MAXBUFF];
static bool rowValid[MAXBUFF];
/* flash busy */
bool flashBusy = false;

/* the address point user flash row */
uint8_t *BACKUPDATA;
/* the address point user sram row */
uint8_t *SRAMSTART;

char buff[256];

/* interrupt service when an operation completed, conduct the next one if there is one
   parameters:
   return: 
*/
void Flash_ISR (void)
{    
    int i;
    volatile cy_en_flashdrv_status_t flashWriteStatus;
    
    /* Check if the generated interrupt occurred from Flash */
    if(0u != (FLASHC_FM_CTL->INTR & FLASHC_FM_CTL_INTR_TIMER_EXPIRED_Msk))
    {
        /* Clear the Flash interrupt */
        FLASHC_FM_CTL->INTR = _VAL2FLD(FLASHC_FM_CTL_INTR_TIMER_EXPIRED, 1u);
        (void) FLASHC_FM_CTL->INTR;

        flashBusy = false;
        for(i = 0; i < MAXBUFF; i++)
        {
            if(rowValid[i])
            {
                flashBusy = true;
                /* incase for some functions in FreeRTOS mess up the interrupts */
                interruptEnable();
                do{
                    flashWriteStatus = Cy_Flash_IsOperationComplete();
                }
                while(flashWriteStatus == CY_FLASH_DRV_OPCODE_BUSY || flashWriteStatus == CY_FLASH_DRV_IPC_BUSY);
                flashWriteStatus = Cy_Flash_StartWrite((uint32_t)toAdd[i], (const uint32_t *)fromAdd[i]); 
                if(flashWriteStatus == CY_FLASH_DRV_IPC_BUSY)
                    return;
                else
//                    Cy_SCB_UART_PutString(UART_HW, "Write error\r\n"); 
                rowValid[i] = false;
                return;
            }
        }     
    }   
}


/* application interface for copying data to flash 
   parameters:
    * from: address of sram
    * startrow: satring row number 
    * size: size in bytes
   return number of quueuing write commands, 1 indicates the given write operation is on-going: 
*/
int copyDatatoFlash(void* from, void* to, int size)
{
    cy_en_flashdrv_status_t flashWriteStatus;
    SRAMSTART = from;
    BACKUPDATA = to;
    
    int iteration = size / SIZEOFROW,ret;
    
    for(int i = 0; i < iteration; i++){
        /* Non-blocking flash write */
        ret = rowtoFlash((void*)SRAMSTART,(void*)BACKUPDATA);
        
        BACKUPDATA += SIZEOFROW;
        SRAMSTART += SIZEOFROW;
    }
    
    return ret;
}

/* copy a piece of VM to NVM by this function 
   parameters:
    * from: address of sram
    * startrow: satring row number 
    * size: size in bytes
   return: successfull status
*/
int rowtoFlash(void* from, void* to)
{
    volatile cy_en_flashdrv_status_t flashWriteStatus, Status;  
    int i;
    uint8_t ramData[CY_FLASH_SIZEOF_ROW];

    /* if flash is busy, join the queue*/
    if(flashBusy)
    {
        for(i = 0; i < MAXBUFF; i++)
        {
            if(!rowValid[i]){
                fromAdd[i] = from;
                toAdd[i] = to;
                rowValid[i] = true;
                break;
            }
        }
    }
    else
    {
        do{
            Status = Cy_Flash_IsOperationComplete();
        }
        while(Status != CY_FLASH_DRV_SUCCESS);
        flashBusy = true;
        /* incase for some functions in FreeRTOS mess up the interrupts */
        interruptEnable();
//        flashWriteStatus = Cy_Flash_StartWrite((uint32_t)to, (const uint32_t *)from);   
        flashWriteStatus = Cy_Flash_WriteRow((uint32_t)to, (const uint32_t *)from);   
        if(flashWriteStatus != CY_FLASH_DRV_SUCCESS )
        {
            flashBusy = false;
            cy_en_ipcdrv_status_t dummy = Cy_IPC_Drv_LockAcquire(Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_CHAN_SYSCALL));
            
//            Cy_IPC_SystemSemaInit();
//            Cy_IPC_SystemPipeInit();
//            Cy_Flash_Init();
//            flashWriteStatus = Cy_Flash_StartWrite((uint32_t)to, (const uint32_t *)from);   
            flashWriteStatus = Cy_Flash_GetExternalStatus();
            return -1;
        }
        //if(flashWriteStatus != CY_FLASH_DRV_OPERATION_STARTED ){
//            Cy_SCB_UART_PutString(UART_HW, "Write error\r\n"); 
        flashBusy = false;
        //}
    } 
    return i;
}

/* initialize the structure for maintianing flash memory in VM
*/
void initFlashStructure()
{
    int i;
    for(i = 0; i < MAXBUFF; i++){
        rowValid[i] = false;   
    }
}

/* enable interrupts required by the flash operation, some FreeRTOS functions,e.g., resumeTasks, 
*  disable them without enabling them back 
*/
void interruptEnable()
{
    /* Initialize and enable the Flash interrupt */
    Cy_SysInt_SetVector(cpuss_interrupt_fm_IRQn, Flash_ISR);
    NVIC_EnableIRQ(cpuss_interrupt_fm_IRQn);
    __enable_irq(); /* Enable global interrupts. */
    /* Clear the Flash interrupt */
    FLASHC_FM_CTL->INTR = _VAL2FLD(FLASHC_FM_CTL_INTR_TIMER_EXPIRED, 1u);
    (void) FLASHC_FM_CTL->INTR;
}