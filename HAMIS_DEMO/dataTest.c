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
#include "dataTest.h"
#include "stdio.h"
#include "string.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

#define OBJID 0
//#define DATA_INTERVAL 2000000
#define DATA_INTERVAL 20000

void dataTest(void * ptr)
{
    long temp,*work;
    char buf[20];
    int objid = OBJID;
    struct DBImage* DB = (struct DBImage *)ptr;
    while(1){
        for(int x = 0; x < DATA_INTERVAL; x++);
    #if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
         memcpy(&temp, read(DB,0,OBJID),4);
        sprintf(buf, "M0 reads %ld\r\n", temp);
        Uprintf(buf);
        work = (long*)getWorking(DB,0,OBJID);
        *work = 1;
        int ret = commit(DB,0,&objid,1,4);
        if(ret == SUCCESS)
            sprintf(buf, "M0 commit SUCCESS\r\n");
        else
            sprintf(buf, "M0 commit FAIL\r\n");
        Uprintf(buf);
        
    #else /* core is Cortex-M4 */
        memcpy(&temp, read(DB,1,OBJID),4);
//        sprintf(buf, "M4 reads %ld, error = %d\r\n", temp,DB->errorNum);
//        Uprintf(buf);
        work = (long*)getWorking(DB,1,OBJID);
        *work = 4;
        int ret = commit(DB,1,&objid,1,4);
        if(ret == SUCCESS)
            sprintf(buf, "M4 commit SUCCESS\r\n");
        else
            sprintf(buf, "M4 commit FAIL\r\n");
        Uprintf(buf);
        resetTask(DB,1);
    #endif
    }
}

/* [] END OF FILE */
