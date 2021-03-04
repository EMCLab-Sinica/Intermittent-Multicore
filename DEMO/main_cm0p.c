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
#include "rtos01.h"
#include "Tester01.h"
#include "Recovery01.h"
#include "UARTM0.h"
#include "Common.h"
#include "config.h"
#include "flash01.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "task.h"
#include "TRN.h"

/* declar our DB need, "the address of NVM (i.e., consistent versions) should be a fixed addrress"*/
static struct DBImage DB;
static struct sharedVM SharedVM;
static char NVMSnapInVM[SIZEOFROW];

/* transmit the addresses for reference DB to another processors */
static uint32_t myArray[4]; /* for Cy_IPC_Sema_Init() */
IPC_STRUCT_Type *myIpcHandle; /* handle for the IPC channel being used */

/* used for recovery */
extern uint8_t* recovery_NVM_flash;
struct recoveryData* recovery_NVM;

int sharedVar;

/* Set up the shared memory */
void sharedMem();

/* RTOS Queue Variables */
QueueHandle_t EventsQueue;

int main(void)
{
    /* Enable global interrupts. */    
    __enable_irq(); 
      
    /* Init Uart */
    UARTInit();
    
    /* Set up the shared memory */
    sharedMem();    
 
    /* Recover system/start from scretch*/
    recovery_NVM = (struct recoveryData*)recovery_NVM_flash;
    if(recovery_NVM->firstTime != 1){
        /* Create tasks */
//        xTaskCreate(dataTest1, "dataTest", STACK_DEPTH, &DB, 1, NULL);
        xTaskCreate(oneDmatrix, "oneDmatrix", STACK_DEPTH, (void*)&DB, 1, NULL);
//        xTaskCreate(blink, "blinkAndMath", STACK_DEPTH, (void*)&DB, 1, NULL);
//        xTaskCreate(UartTask, "Uart", STACK_DEPTH, NULL, 1, NULL);
        xTaskCreate(TouchTask, "Touch Task", STACK_DEPTH, (void*)&DB, 1, NULL);
        xTaskCreate(senseTemp, "temperature",STACK_DEPTH, (void*)&DB, 1, NULL);
        xTaskCreate(math06, "Math", STACK_DEPTH, (void*)&DB, 1, NULL);

        commitRecoveryData();
        //need to consider recovery after this point
        vTaskStartScheduler();    
    }
    else{
        failureRecovery();
    }

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

void sharedMem()
{   
     /* For sharing the DB with another core */
    DB.NVMAdd0 = (struct sharedNVM*)SHARED0_ADDRESS;
    DB.NVMAdd1 = (struct sharedNVM*)SHARED1_ADDRESS;
    DB.NVMInVM = (struct sharedNVM*)NVMSnapInVM;
    DB.delayed = 0;
    DB.VMAdd = &SharedVM;
    
    char buff[50];
    if(DB.NVMAdd0->versionNum > DB.NVMAdd1->versionNum || (DB.NVMAdd0->valid == 1 && DB.NVMAdd1->valid != 1) ){
        DB.validNVM = false;
        memcpy(DB.NVMInVM,DB.NVMAdd0,sizeof(struct sharedNVM));
        
        sprintf(buff, "false: versionNum, %d,@@@ %d\r\n",DB.NVMAdd0->versionNum,DB.NVMAdd1->versionNum);
    }
    else
    {
        DB.validNVM = true;
        memcpy(DB.NVMInVM,DB.NVMAdd1,sizeof(struct sharedNVM));
        
        sprintf(buff, "true: versionNum, %d,@@@ %d\r\n",DB.NVMAdd0->versionNum,DB.NVMAdd1->versionNum);
    }
    
    Uprintf(buff);
   
    /* For timer of DB */
    setDBPtr(&DB);
    
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR);
    
    /* Initialize the IPC semaphore subsystem. This must done by this CPU, with definition of
    semaphore array address, before the other CPU starts using the semaphore system. */
    Cy_IPC_Sema_Init(CY_IPC_CHAN_SEMA, sizeof(myArray) * 8ul/*bits per byte*/, myArray);
    /* Notify the CM4 with the shared memory address. There is no interrupt associated with this notification.
       Wait for channel to be available, then acquire the lock for it, in a single atomic operation;
       and send the address. */
    myIpcHandle = Cy_IPC_Drv_GetIpcBaseAddress(MY_IPC_CHANNEL);
    while(Cy_IPC_Drv_SendMsgPtr(myIpcHandle, CY_IPC_NO_NOTIFICATION, &DB) != CY_IPC_DRV_SUCCESS);
    /* Wait for release, which indicates other CPU has read the pointer value. */
    while (Cy_IPC_Drv_IsLockAcquired(myIpcHandle));
    
    /* initialize some DB structure */
    for(int i = 0; i < NUMOBJ; i++)
    {
        //incase the object is not committed before
        if(DB.NVMInVM->objConsistent[i].begin >= DB.NVMInVM->objConsistent[i].end)
        {
            DB.NVMInVM->objConsistent[i].end = DB.NVMInVM->objConsistent[i].begin + 1;
        }
    }
    
    sprintf(buff, "Transmit DB shared is at %p\r\n",&DB);
    Uprintf(buff);
}

/* [] END OF FILE */
