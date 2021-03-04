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

#include "SharedDB.h"
#include "dataTest.h"
#include "stdio.h"
#include "string.h"
#include "transmit2PC.h"
#include "FreeRTOS.h"
#include "task.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif
    
#define TIMEINTERVAL 250//ms
struct DBImage* DB;
void transmit(void * ptr)
{
    /* For our DB */
    DB = (struct DBImage *)ptr;
    
    uint32_t last = 0, current;
    while (1){
        current = GetTotalTime();//us
        if(current - last > TIMEINTERVAL * 1000)
            last = current;
        else 
            continue;
       
        resetTask(DB,IDTRANSMIT);
        char uartString[50];
        sprintf(uartString, "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,D:,%ld,%ld\r\n",DB->NVMInVM->progress[0],DB->NVMInVM->progress[1],DB->NVMInVM->progress[2],DB->NVMInVM->progress[3],DB->NVMInVM->progress[4],DB->NVMInVM->progress[5],DB->NVMInVM->progress[6],DB->NVMInVM->progress[7],DB->NVMInVM->progress[8],DB->NVMInVM->progress[9],DB->NVMInVM->blockM0,DB->NVMInVM->blockM4);
//        sprintf(uartString, "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,D:,%ld,%ld\r\n",DB->resume_progress[0],DB->resume_progress[1],DB->resume_progress[2],DB->resume_progress[3],DB->resume_progress[4],DB->resume_progress[5],DB->resume_progress[6],DB->resume_progress[7],DB->resume_progress[8],DB->resume_progress[9],DB->NVMInVM->blockM0,DB->NVMInVM->blockM4);
        Uprintf(uartString);   
        sprintf(uartString, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",DB->VMAdd->random[0],DB->VMAdd->random[1],DB->VMAdd->random[2],DB->VMAdd->random[3],DB->VMAdd->random[4],DB->VMAdd->random[5],DB->VMAdd->random[6],DB->VMAdd->random[7],DB->VMAdd->random[8],DB->VMAdd->random[9]);
        Udprintf(uartString);        
        
//        int ret;
//        if(getRandom(DB,IDTRANSMIT) >= WRITE_PRO)
//            ret = commit(DB, IDTRANSMIT, NULL, 0, 4);
//        else
          commit(DB, IDTRANSMIT, NULL, 0, 4);
//            
//        GenerateRandom(DB);
//        
//        if(ret == SUCCESS)
//            sprintf(uartString, "success\r\n");
//        else
//            sprintf(uartString, "fail\r\n");
        Udprintf(uartString);
        
    }    
}