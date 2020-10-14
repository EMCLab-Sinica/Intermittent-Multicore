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

#include "twoDmatrix.h"
#include "stdio.h"
#include "SharedDB.h"
typedef unsigned char UInt8;

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

float add(float a, float b)
{
return (a + b);
}
float mul(float a, float b)
{
return (a * b);
}
float div(float a, float b)
{
return (a / b);
}

struct DBImage* DB;
void floating_task(void* ptr)
{
     int m, n, p;

    DB = (struct DBImage *)ptr;
    int k;
    volatile int swcount;
    swcount=0;
    
    char uartString[50];
    while(1){
        /* For our DB */
        volatile float result[4];
        resetTask(DB,IDFLAOTING);
        for(k = 0; k < ITERFLOATING; k++){
            result[0] = 54.567;
            result[1] = 14346.67;
            result[2] = add(result[0], result[1]);
            result[1] = mul(result[0], result[2]);
            result[3] = div(result[1], result[2]);
        }
        
        int ret;
        
        #ifdef USE_PROBABILITY
        if(getRandom(DB,IDFLAOTING) >= WRITE_PRO)
            ret = commit(DB, IDFLAOTING, NULL, 0, 4);
        else
            DB->NVMInVM->progress[IDFLAOTING]++;
        
        GenerateRandom(DB);
        #else
            ret = commit(DB, IDFLAOTING, NULL, 0, 4);
        #endif
        if(ret == SUCCESS)
            sprintf(uartString, "Floating: SUCCESS\r\n");
        else
            sprintf(uartString, "Floating: FAIL\r\n");
        Udprintf(uartString);
    }
}

/* [] END OF FILE */
