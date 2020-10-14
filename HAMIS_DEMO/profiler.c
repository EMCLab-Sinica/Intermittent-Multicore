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
#include <stdio.h>
#include <profiler.h>
#include <string.h>
#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "FreeRTOS01.h"
    #include "task01.h"
#else /* core is Cortex-M4 */
    #include "FreeRTOS.h"
    #include "task.h"
#endif

static uint32_t counterOverflow = 0;

/* Overflow when counting every 65.536 ms */
void OverflowInterrupt()
{
    #if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
        ;//DO nothing
    #else /* core is Cortex-M4 */
         /* Get which event triggered the interrupt */
        uint32_t intSource = Cy_TCPWM_GetInterruptStatus(Counter_HW, Counter_TCPWM__CNT_IDX);
        
        /* Clear interrupt */
        Cy_TCPWM_ClearInterrupt(Counter_HW, Counter_TCPWM__CNT_IDX, CY_TCPWM_INT_ON_CC_OR_TC);
        NVIC_ClearPendingIRQ(Counter_Int_cfg.intrSrc);
        
        /* If the interrupt is triggered by an overflow event, then counting how 
        many times counter overflow happened in one second */ 
        if(intSource == CY_TCPWM_INT_ON_TC)
        {
            counterOverflow++;
        }
    #endif
}

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

/* Initial TWM and TCPWM */
void InitProfiler()
{
    #if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
        ;//DO nothing
    #else /* core is Cortex-M4 */
        /* Place your initialization/startup code here (e.g. MyInst_Start()) */
        /* Initialize with config set in component and enable the Counter */
        Cy_TCPWM_Counter_Init(Counter_HW, Counter_CNT_NUM, &Counter_config);
        Cy_TCPWM_Enable_Multiple(Counter_HW, Counter_CNT_MASK);
        
        /* Hookup and enable interrupt */
        Cy_SysInt_Init(&Counter_Int_cfg, OverflowInterrupt);
        NVIC_EnableIRQ((IRQn_Type)Counter_Int_cfg.intrSrc);
        
        /* Start the Counter and PWM */
        Cy_TCPWM_TriggerStart(Counter_HW, Counter_CNT_MASK);
        
        /* Initilaize with config set in componentand and start the PWM for generating 
         one second time window */
        Cy_TCPWM_PWM_Init(PWM_HW, PWM_CNT_NUM, &PWM_config);
        Cy_TCPWM_Enable_Multiple(PWM_HW, PWM_CNT_MASK);
        Cy_TCPWM_TriggerStart(PWM_HW, PWM_CNT_MASK);
    #endif
}

/* Get current time */
void GetTime(uint32_t* overflow, uint32_t* counter)
{
    *overflow = counterOverflow;
    *counter = Counter_CNT_HW->COUNTER;
    return;
}

/* time info. for FreeRTOS */
uint32_t GetTotalTime()
{    
    return (counterOverflow)* MAXCOUNT + Counter_CNT_HW->COUNTER;
}

/* Count time difference */
uint32_t CountTime(uint32_t Boverflow, uint32_t Bcounter)
{
   uint32_t currentcount = Counter_CNT_HW->COUNTER;;
    
   uint32_t ret = (counterOverflow - Boverflow)* MAXCOUNT + currentcount - Bcounter;

    return ret;
//    average += EXETIME[i];
//    counterOverflow = RESET;
//    
//    /* Manage output */
//    if(i != TRIALNUM-1)
//        sprintf(buf, "%lu,", EXETIME[i]);
//    else
//        sprintf(buf, "%lu", EXETIME[i]);
//    strcat(buffer,buf);
//        
//    /* Manage output */
//    strcat(buffer," us\n\r");
//    Cy_SCB_UART_PutString(UART_HW, buffer);
//    sprintf(buffer, "Measurement finished, avaergae time = %lu us\n\r", average/(TRIALNUM*TESTLOOP));
//    Cy_SCB_UART_PutString(UART_HW, buffer);
}

/* [] END OF FILE */
