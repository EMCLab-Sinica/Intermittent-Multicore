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
#include "rtos.h"
#include "Tester.h"
#include "Recovery.h"
#include "Common.h"
#include "stdio.h"
#include "UARTM4.h"
#include <string.h>
#include "SharedDB.h"
#include "task.h"

/* used for recovery */
extern uint8_t* recovery_NVM_flash;
struct recoveryData* recovery_NVM;

/* used for receiving the FB info */
IPC_STRUCT_Type *myIpcHandle;
static struct DBImage* DB;

/* Set up the shared memory */
void sharedMem();

int main(void)
{
    /* Enable global interrupts. */    
    __enable_irq(); 

    /* Set up the shared memory */
    sharedMem();
            
    
#ifdef USE_PROBABILITY
    /* For random var */
    InitCryto(DB);  
#endif
    
//    uint32_t random = GenerateRandom();
    recovery_NVM = (struct recoveryData*)recovery_NVM_flash;
    if(recovery_NVM->firstTime != 1){
        /* Create tasks */      
        xTaskCreate(sha_task, "sha", STACK_DEPTH, DB, 1, NULL);
        xTaskCreate(dft_task, "dft", STACK_DEPTH, DB, 1, NULL);
        xTaskCreate(transmit, "Transmit", STACK_DEPTH, DB, 1, NULL);
//        xTaskCreate(FIR_task, "FIR", STACK_DEPTH, DB, 1, NULL);
        xTaskCreate(twoDmatrix, "2Dmatrix", STACK_DEPTH, DB, 1, NULL);
        xTaskCreate(floating_task, "Floating", STACK_DEPTH, DB, 1, NULL);
//        xTaskCreate(dataTest, "dataTest", STACK_DEPTH, DB, 1, NULL);  
//        xTaskCreate(blink, "Blink 2", STACK_DEPTH, NULL, 1, NULL);
//        xTaskCreate(UartTask, "Uart", STACK_DEPTH, NULL, 1, NULL);
//        xTaskCreate(math16, "Math", STACK_DEPTH, NULL, 1, NULL);
        commitRecoveryData();
        vTaskStartScheduler();
    }
    else{
        failureRecovery();
    }

    for(;;)
    {
        /* Place your application code here. */
    }
}

void sharedMem()
{
    /* Read the shared memory address, from the other CPU. Wait for success, which indicates that
       (1) the sending CPU acquired the lock, and (2) this CPU read the pointer. */
    myIpcHandle = Cy_IPC_Drv_GetIpcBaseAddress(MY_IPC_CHANNEL);
    while (Cy_IPC_Drv_ReadMsgPtr(myIpcHandle, (void *)&DB) != CY_IPC_DRV_SUCCESS);
    /* Release the lock. This indicates to the other CPU that the read is 
       successfully complete. */
    Cy_IPC_Drv_LockRelease(myIpcHandle, CY_IPC_NO_NOTIFICATION);
    /* Initialize the IPC semaphore subsystem. The other CPU already defined the semaphore
       array address, so this CPU just initializes the IPC channel number. */
    Cy_IPC_Sema_Init(CY_IPC_CHAN_SEMA, (uint32_t)NULL, (uint32_t *)NULL);
        
    for(int i = 0; i < NUMTASK; i++){
        DB->resume_progress[i] = DB->NVMInVM->progress[i];
        
    }
    char uartString[100];
    sprintf(uartString, "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,D:,%ld,%ld\r\n",DB->resume_progress[0],DB->resume_progress[1],DB->resume_progress[2],DB->resume_progress[3],DB->resume_progress[4],DB->resume_progress[5],DB->resume_progress[6],DB->resume_progress[7],DB->resume_progress[8],DB->resume_progress[9],DB->NVMInVM->blockM0,DB->NVMInVM->blockM4);
    Uprintf(uartString);   
    /* For timer of DB */
    setDBPtr(&DB);
    
    char buff[50];
    sprintf(buff, "Receive DB shared is at %p\r\n",DB);
    Uprintf(buff);  
}

/* [] END OF FILE */
