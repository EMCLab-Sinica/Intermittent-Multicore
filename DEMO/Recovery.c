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

#include <Recovery.h>
#include "FreeRTOS.h"
#include <stdio.h>
#include "flash.h"
#include "task.h"
#include "string.h"

/* accual space for stroe data in VM */
static char recovery_VM_Data[SIZEOFROW*RECOVERYDATAROW];
/* structure for maintining the space of the recovery handler in VM */
static struct recoveryData* recovery_VM = (struct recoveryData*)recovery_VM_Data;
/* structure for maintining the space of the recovery handler in NVM */
CY_ALIGN(CY_FLASH_SIZEOF_ROW) 
 uint8_t* recovery_NVM_flash = (uint8_t *)CALCULATE_FLASH_ADDRESS(RECOVERY_ROW);; /* The array will be placed in Flash */

/*
 * Task control block.  A task control block (TCB) is allocated for each task,
 * and stores task state information, including a pointer to the task's context
 * (the task's run time environment, including register values)
 */
typedef struct tskTaskControlBlock
{
	volatile StackType_t	*pxTopOfStack;	/*< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

	#if ( portUSING_MPU_WRAPPERS == 1 )
		xMPU_SETTINGS	xMPUSettings;		/*< The MPU settings are defined as part of the port layer.  THIS MUST BE THE SECOND MEMBER OF THE TCB STRUCT. */
	#endif

	ListItem_t			xStateListItem;	/*< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */
	ListItem_t			xEventListItem;		/*< Used to reference a task from an event list. */
	UBaseType_t			uxPriority;			/*< The priority of the task.  0 is the lowest priority. */
	StackType_t			*pxStack;			/*< Points to the start of the stack. */
	char				pcTaskName[ configMAX_TASK_NAME_LEN ];/*< Descriptive name given to the task when created.  Facilitates debugging only. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        
    /* for failure resilient */
    void *              AddressOfNVMFunction;
    UBaseType_t         uxTCBNumber;
    UBaseType_t         uxTaskNumber;
    unsigned long vBegin;
    unsigned long vEnd;
    
	#if ( portSTACK_GROWTH > 0 )
		StackType_t		*pxEndOfStack;		/*< Points to the end of the stack on architectures where the stack grows up from low memory. */
	#endif

	#if ( portCRITICAL_NESTING_IN_TCB == 1 )
		UBaseType_t		uxCriticalNesting;	/*< Holds the critical section nesting depth for ports that do not maintain their own count in the port layer. */
	#endif

	#if ( configUSE_TRACE_FACILITY == 1 )
		UBaseType_t		uxTCBNumber;		/*< Stores a number that increments each time a TCB is created.  It allows debuggers to determine when a task has been deleted and then recreated. */
		UBaseType_t		uxTaskNumber;		/*< Stores a number specifically for use by third party trace code. */
	#endif

	#if ( configUSE_MUTEXES == 1 )
		UBaseType_t		uxBasePriority;		/*< The priority last assigned to the task - used by the priority inheritance mechanism. */
		UBaseType_t		uxMutexesHeld;
	#endif

	#if ( configUSE_APPLICATION_TASK_TAG == 1 )
		TaskHookFunction_t pxTaskTag;
	#endif

	#if( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
		void *pvThreadLocalStoragePointers[ configNUM_THREAD_LOCAL_STORAGE_POINTERS ];
	#endif

	#if( configGENERATE_RUN_TIME_STATS == 1 )
		uint32_t		ulRunTimeCounter;	/*< Stores the amount of time the task has spent in the Running state. */
	#endif

	#if ( configUSE_NEWLIB_REENTRANT == 1 )
		/* Allocate a Newlib reent structure that is specific to this task.
		Note Newlib support has been included by popular demand, but is not
		used by the FreeRTOS maintainers themselves.  FreeRTOS is not
		responsible for resulting newlib operation.  User must be familiar with
		newlib and must provide system-wide implementations of the necessary
		stubs. Be warned that (at the time of writing) the current newlib design
		implements a system-wide malloc() that must be provided with locks. */
		struct	_reent xNewLib_reent;
	#endif

	#if( configUSE_TASK_NOTIFICATIONS == 1 )
		volatile uint32_t ulNotifiedValue;
		volatile uint8_t ucNotifyState;
	#endif

	/* See the comments above the definition of
	tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE. */
	#if( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
		uint8_t	ucStaticallyAllocated; 		/*< Set to pdTRUE if the task is a statically allocated to ensure no attempt is made to free the memory. */
	#endif

	#if( INCLUDE_xTaskAbortDelay == 1 )
		uint8_t ucDelayAborted;
	#endif

} tskTCB;

extern tskTCB * volatile pxCurrentTCB;
extern unsigned char volatile stopTrack;

/*
 * taskRerun(): rerun the current task invoking this function
 * parameters: none
 * return: none
 * note: Memory allocated by the task code is not automatically freed, and should be freed before the task is deleted
 * */
void taskRerun(){
    xTaskCreate( pxCurrentTCB->AddressOfNVMFunction, pxCurrentTCB->pcTaskName, configMINIMAL_STACK_SIZE, NULL, pxCurrentTCB->uxPriority, NULL);
    vTaskDelete(NULL);//delete the current TCB
}

/*
 * regTaskEnd(): mark the assigned task as started
 * parameters: none
 * return: none
 * */
void regTaskStart(void* add, unsigned short pri, unsigned short TCB, void* TCBA, void* parameter, int stopTrack){
    volatile int i;
    i = sizeof(struct recoveryData);
    if(stopTrack)
        return;
    for(i = 0; i < NUMTASK; i++){
        //find a invalid
        if(recovery_VM->unfinished[i] != 1){
            recovery_VM->address[i] = add;
            recovery_VM->priority[i] = pri;
            recovery_VM->TCBNum[i] = TCB;
            recovery_VM->TCBAdd[i] = TCBA;
            recovery_VM->TPara[i] = parameter;
            recovery_VM->schedulerTask[i] = stopTrack;
            recovery_VM->unfinished[i] = 1;//incase failure before this
            break;
        }
    }
}


/*
 * regTaskEnd(): mark the current as ended
 * parameters: none
 * return: none
 * */
void regTaskEnd(){
    int i;
    for(i = 0; i < NUMTASK; i++){
        //find the slot
        if(recovery_VM->unfinished[i] == 1 && recovery_VM->TCBNum[i] == pxCurrentTCB->uxTCBNumber){
            recovery_VM->unfinished[i] = 0;
            return;
        }
    }
    commitRecoveryData();
}

/*
 * prvcheckAdd(): check if the pointer is actually allocated and can be freed
 * parameters: none
 * return: 1 for yes
 * */
int prvcheckAdd(void * pv){
    (void) pv;
    return 1;
}

/*
 * freePreviousTasks(): free all unfinished tasks stacks after power failure, this only is used by default approach
 * parameters: none
 * return: none
 * */
void freePreviousTasks(){
    //no need to recycle all unfinished tasks now since heap is in VM now
    return;
}


/*
 * failureRecovery(): recover all unfinished tasks after power failure
 * parameters: none
 * return: none
 * */
extern volatile unsigned char stopTrack;
void failureRecovery(){
    int i;
    char buff[64];
    
    restoreRecoveryData();
    stopTrack = 1;//do not need to re-record unfinished tasks 
    for(i = 0; i < NUMTASK; i++){
        //find all unfinished tasks
        if(recovery_VM->unfinished[i] == 1){
            //find a unfinished task
            if(!recovery_VM->schedulerTask[i]){
                
                xTaskCreate(recovery_VM->address[i], "Recovery", STACK_DEPTH, recovery_VM->TPara[i], recovery_VM->priority[i], NULL);
//                sprintf(buff, "Recovery Create: %d\r\n", recovery_VM->TCBNum[i]);
//                Cy_SCB_UART_PutString(UART_HW, buff); 
            }
//            sprintf(buff, "Rend: xPortGetFreeHeapSize = %d\r\n", xPortGetFreeHeapSize());
//            Cy_SCB_UART_PutString(UART_HW, buff); 
        }
    }
    stopTrack = 0;//start recording from this point
    /* Start the scheduler. */
    vTaskStartScheduler();
}

/*
 * initData(): initialize the space in VM, P.S. this should be called at the first time of system execution
 * parameters: none
 * return: none
 * */
void initData()
{
    memset(recovery_VM_Data, 0 , SIZEOFROW*RECOVERYDATAROW);
}

/*
 * commitRecoveryData(): commit data requied by the recovery handler to specific flash
 * parameters: none
 * return: none
 * */
void commitRecoveryData(){
    volatile void* dummy = recovery_NVM_flash;
    recovery_VM->firstTime = 1;
    copyDatatoFlash(recovery_VM, recovery_NVM_flash, SIZEOFROW*RECOVERYDATAROW);
}

/*
 * restoreRecoveryData(): resotre lost data requied by the recovery handler to specific flash
 * parameters: none
 * return: none
 * */
void restoreRecoveryData(){
    memcpy(recovery_VM_Data, (void *)recovery_NVM_flash, SIZEOFROW*RECOVERYDATAROW);
}