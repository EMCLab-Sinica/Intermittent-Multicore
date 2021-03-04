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
#include "FreeRTOS01.h"
#include "task01.h"
#include "blinkLED.h"
#include "SharedDB.h"
#include "stdio.h"
#define BLINK_INTERVAL 2000000

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

struct DBImage* DB;
void blink(void *ptr)
{
    long x;
    bool on = true;
    Cy_GPIO_Clr(LED_RED_PORT, LED_RED_NUM);
    
    
    DB = (struct DBImage *)ptr;
    volatile int a[4];
    char uartString[50];
    while(1){
//        for(x = 0; x < BLINK_INTERVAL; x++);
        resetTask(DB,IDMATH);
        for(int k = 0; k < ITERMATH16; k++){
            a[0] = 231;
            a[1] = 12;
            a[2] = a[0] + a[1];
            a[1] = a[0] * a[2];
            a[3] = a[1] / a[2];
        }
        int ret;
        
        #ifdef USE_PROBABILITY
        if(getRandom(DB,IDMATH) >= WRITE_PRO)
            ret = commit(DB, IDMATH, NULL, 0, 4);
        else
            DB->NVMInVM->progress[IDMATH]++;
        #else
            DB->NVMInVM->progress[IDMATH]++;
        #endif
        
        if(ret == SUCCESS)
            sprintf(uartString, "MATH: SUCCESS\r\n");
        else
            sprintf(uartString, "MATH: FAIL\r\n");
        Udprintf(uartString);
        
        if(on){
            Cy_GPIO_Set(LED_RED_PORT, LED_RED_NUM);
            on = false;
        }
        else{
            Cy_GPIO_Clr(LED_RED_PORT, LED_RED_NUM);
            on = true;
        }
        taskYIELD();
    }
}

/* [] END OF FILE */
