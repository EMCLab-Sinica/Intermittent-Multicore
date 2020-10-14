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

#include "SharedDB.h"
#include "string.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "FreeRTOS01.h"
    #include "task01.h"
#else /* core is Cortex-M4 */
    #include "FreeRTOS.h"
    #include "task.h"
#endif


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

#define min(x,y)         (x < y?x:y)
#define max(x,y)         (x > y?x:y)

/*
*  Reset the taskID's info. of validation
*/
void resetTask(struct DBImage* DB, int taskID)
{
    for(int j = 0; j < NUMOBJ; j++)
    {
        DB->VMAdd->RObj[taskID][j] = false;
        DB->VMAdd->RAWObj[taskID][j] = false;
        DB->VMAdd->RAWEnd[taskID] = 0xFFFFFFFFFFFFFFFF;
        DB->VMAdd->TBegin[taskID] = 0;
        DB->VMAdd->TEnd[taskID] = 0xFFFFFFFFFFFFFFFF;
    }
}

/*
*  Reset all tasks' info. of validation
*/
void resetAllTasks(struct DBImage* DB)
{
    for(int i = 0; i < NUMTASK; i++)
    {
        for(int j = 0; j < NUMOBJ; j++)
        {
            DB->VMAdd->RObj[i][j] = false;
            DB->VMAdd->RAWEnd[i] = 0xFFFFFFFFFFFFFFFF;
            DB->VMAdd->TBegin[i] = 0;
            DB->VMAdd->TEnd[i] = 0xFFFFFFFFFFFFFFFF;
        }
    }
}

/*
*  Return the address of requested obj's consistent version in VM 
*/
void* read(struct DBImage* DB, int taskID, int objID)
{
    DB->VMAdd->TBegin[taskID] = DB->NVMInVM->objConsistent[objID].begin + 1;
    DB->VMAdd->RObj[taskID][objID] = true;
    return DB->NVMInVM->objConsistent[objID].buffer;
}

/*
*  Return the working version's address for a task writing an object
*/
void* getWorking(struct DBImage* DB, int taskID, int objID)
{
    return DB->VMAdd->objWorking[taskID][objID].buffer;
}


/*
*  Commit a task's the modification to consistent versions, the task need to be verified before the commit
*/
int commit(struct DBImage* DB, int taskID, int* objID, int numObj,int size)
{
    int ret = FAIL; 
    bool toNVM = false;
    const unsigned long commitTime = DB->NVMInVM->timeCounter;//current time
    if(numObj == 0){//Only commit progress
        DB->NVMInVM->progress[taskID]++;      
        #if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
            DB->delayed = 0;
            toNVM = true;
        #else /* core is Cortex-M4 */
            #ifdef DELAYCOMMIT
            /* check number of delayed commits */
            if(DB->delayed > MAX_DELAY){
                toNVM = true;
                DB->delayed = 0;
            }
            else
                DB->delayed++;  
            #else
                toNVM = true;
            #endif
        #endif
        
        if(toNVM){
            #ifdef LOGBLOCKING
//            vTaskRunningTime();
            uint32_t begin = pxCurrentTCB->ulRunTimeCounter;
            #endif
            while(CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Set(NVM_SEMA, false)){
                #ifdef ADAPTIVE
                    taskYIELD();;//handle if not acquired, currently we use a while loop
                #else
                    ;
                #endif
            }
            #ifdef LOGBLOCKING
//            vTaskRunningTime();
            uint32_t end = pxCurrentTCB->ulRunTimeCounter;
                #if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
                    DB->NVMInVM->blockM0 += end - begin;
                #else /* core is Cortex-M4 */
                    DB->NVMInVM->blockM4 += end - begin;
                #endif
            #endif
            commitToNVM(DB);
            while (CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Clear(NVM_SEMA, false)){
                ;   
            }  
        }
        
        ret = SUCCESS;
        return ret;
    }
    
    /* validation: phase 1 */ 
    //Set begin/end
    DB->VMAdd->TBegin[taskID] = max(DB->VMAdd->TBegin[taskID], 0);
    DB->VMAdd->TEnd[taskID] = commitTime;
    
    //For reads
    for(int j = 0; j < NUMOBJ; j++)
    {
        if(DB->VMAdd->RAWObj[taskID][j])//task i has read the j obj committed by current task
            DB->VMAdd->TEnd[taskID] = min(DB->VMAdd->TEnd[taskID], DB->VMAdd->RAWEnd[taskID]);
    }
    
    //For writes
     for(int j = 0; j < numObj; j++)
    {
        DB->VMAdd->TBegin[taskID] = max(DB->VMAdd->TBegin[taskID], DB->NVMInVM->objConsistent[objID[j]].begin+1);
    }
    
    /* Phase 2: Check with the semaphore locked */
    //acquire lock for NVM and the second phase
    while(CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Set(VALID_SEMA, false)){
        #ifdef ADAPTIVE
            taskYIELD();//handle if not acquired, currently we use a while loop
        #else
            ;
        #endif
    }
      
    //run through records in the locked area
    for(int i = 0; i < NUMTASK; i++)
    {
        //someone commits within this task's phase 1
        if(DB->VMAdd->p2Records[i].valid && DB->VMAdd->p2Records[i].commitTime > commitTime){
            //if the object is read by this task
            if(DB->VMAdd->RObj[taskID][DB->VMAdd->p2Records[i].objCommit]){
                DB->VMAdd->TEnd[taskID] = min(DB->VMAdd->TEnd[taskID], DB->VMAdd->p2Records[i].begin-1);
            }
            //if the object is written by this task
            if(DB->VMAdd->p2Records[i].objCommit == objID[0])//Currently support single obj write
            {
                DB->VMAdd->TBegin[taskID] = max(DB->VMAdd->TBegin[taskID], DB->VMAdd->p2Records[i].begin+1);
            }
        }
    }
    
    //validation pass
    if(DB->VMAdd->TBegin[taskID] <=  DB->VMAdd->TEnd[taskID]){
        DB->NVMInVM->progress[taskID]++;
        for(int j = 0; j < numObj; j++){
            DB->NVMInVM->objConsistent[objID[j]].begin = DB->VMAdd->TBegin[taskID];
            DB->NVMInVM->objConsistent[objID[j]].end = DB->VMAdd->TEnd[taskID];
            memcpy(DB->NVMInVM->objConsistent[objID[j]].buffer,DB->VMAdd->objWorking[taskID][objID[j]].buffer,size);  
        }
        /* Free delayed commits: M0 */
        
        #if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
            DB->delayed = 0;
            toNVM = true;
        #else /* core is Cortex-M4 */
            #ifdef DELAYCOMMIT
            /* check number of delayed commits */
            if(DB->delayed > MAX_DELAY){
                toNVM = true;
                DB->delayed = 0;
            }
            else
                DB->delayed++;  
            #else
                toNVM = true;
            #endif
        #endif
    
        if(toNVM){
            #ifdef LOGBLOCKING
//            vTaskRunningTime();
            uint32_t begin = pxCurrentTCB->ulRunTimeCounter;
            #endif
            while(CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Set(NVM_SEMA, false)){
                #ifdef ADAPTIVE
                    taskYIELD();;//handle if not acquired, currently we use a while loop
                #else
                    ;
                #endif
            }
            #ifdef LOGBLOCKING
//            vTaskRunningTime();
            uint32_t end = pxCurrentTCB->ulRunTimeCounter;
                #if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
                    DB->NVMInVM->blockM0 += end - begin;
                #else /* core is Cortex-M4 */
                    DB->NVMInVM->blockM4 += end - begin;
                #endif
            #endif
            commitToNVM(DB);
            while (CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Clear(NVM_SEMA, false)){
                ;   
            }  
        }

        ret = SUCCESS;
    }
    else//validation fails: delete and rerun 
    {
        ret = FAIL;
    }
    
    /* Unlock the semaphore, phase 2 validation fails */
    while (CY_IPC_SEMA_SUCCESS != Cy_IPC_Sema_Clear(VALID_SEMA, false));  
    

    
    //for other's read
    if(ret != FAIL)
        for(int i = 0; i < NUMTASK; i++)
        {
            for(int j = 0; j < numObj; j++)
            {
                if(DB->VMAdd->RObj[i][j])//task i has read the j obj committed by current task
                {
                    DB->VMAdd->RAWObj[i][j] = true;
                    DB->VMAdd->RAWEnd[i] = min(DB->VMAdd->TBegin[taskID] - 1, DB->VMAdd->RAWEnd[i]);
                }
            }
        }
        
    return ret;
}

/*
*  Commit all modifications to the VM's consistent versions to NVM
*/
void commitToNVM(struct DBImage* DB)
{
    volatile cy_en_flashdrv_status_t flashWriteStatus, Status;  
    
    DB->NVMInVM->versionNum++;
    do{
        Status = Cy_Flash_IsOperationComplete();
    }
    while(Status != CY_FLASH_DRV_SUCCESS);
    
    DB->NVMInVM->valid = 1;
    if(DB->validNVM){
        flashWriteStatus = Cy_Flash_WriteRow((uint32_t)DB->NVMAdd0, (const uint32_t *)DB->NVMInVM);   
        DB->validNVM = false;
    }
    else{
        flashWriteStatus = Cy_Flash_WriteRow((uint32_t)DB->NVMAdd1, (const uint32_t *)DB->NVMInVM);   
        DB->validNVM = true;
    }
        
    
    if(flashWriteStatus != CY_FLASH_DRV_SUCCESS )
    {
        DB->errorNum++;
        cy_en_ipcdrv_status_t dummy = Cy_IPC_Drv_LockAcquire(Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_CHAN_SYSCALL));

        flashWriteStatus = Cy_Flash_GetExternalStatus();
    }
}

/*
*  Increase the time for validation
*/
void increaseTime(struct DBImage* DB)
{
    DB->NVMInVM->timeCounter++;
}

/*
*  Get the time for validation
*/
unsigned long getCurrentTime(struct DBImage* DB)
{
    return DB->NVMInVM->timeCounter;
}


/* [] END OF FILE */
