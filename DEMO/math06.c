/*
 * math16.c
 *
 */

#include <config.h>
#include "FreeRTOS01.h"
#include "task01.h"
#include "project.h"
#include "stdio.h"
#include "SharedDB.h"
#include "math06.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

/*******************************************************************************
*
* Name : 16-bit Math
* Purpose : Benchmark 16-bit math functions.
*
*******************************************************************************/
typedef unsigned short UInt16;
UInt16 add16(UInt16 a, UInt16 b)
{
    return (a + b);
}
UInt16 mul16(UInt16 a, UInt16 b)
{
    return (a * b);
}
UInt16 div16(UInt16 a, UInt16 b)
{
    return (a / b);
}

volatile UInt16 Fresult[4];
volatile UInt16 Sresult[4];

struct DBImage* DB;
void math06(void* ptr)
{
    DB = (struct DBImage *)ptr;
    int k;
    volatile int swcount;
    swcount = 0;
    char uartString[50];
    while(1){
        /* For our DB */
        resetTask(DB,IDMATH);
        for(k = 0; k < ITERMATH16; k++){
            Sresult[0] = 231;
            Sresult[1] = 12;
            Sresult[2] = Sresult[0] + Sresult[1];
            Sresult[1] = Sresult[0] * Sresult[2];
            Fresult[3] = Sresult[1] / Sresult[2];
        }
        int objid = 0;
        int ret = commit(DB,IDMATH, &objid, 0, 4);
        
        if(ret == SUCCESS)
            sprintf(uartString, "MATH: SUCCESS\r\n");
        else
            sprintf(uartString, "MATH: FAIL\r\n");
        Udprintf(uartString);
    }
    
}

