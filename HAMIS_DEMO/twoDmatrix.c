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


struct DBImage* DB;
void twoDmatrix(void* ptr)
{
    UInt8 m1[16][4] = {
    {0x12, 0x56, 0x90, 0x34},
    {0x78, 0x12, 0x56, 0x90},
    {0x34, 0x78, 0x12, 0x56},
    {0x90, 0x34, 0x78, 0x12},
    {0x12, 0x56, 0x90, 0x34},
    {0x78, 0x12, 0x56, 0x90},
    {0x34, 0x78, 0x12, 0x56},
    {0x90, 0x34, 0x78, 0x12},
    {0x12, 0x56, 0x90, 0x34},
    {0x78, 0x12, 0x56, 0x90},
    {0x34, 0x78, 0x12, 0x56},
    {0x90, 0x34, 0x78, 0x12},
    {0x12, 0x56, 0x90, 0x34},
    {0x78, 0x12, 0x56, 0x90},
    {0x34, 0x78, 0x12, 0x56},
    {0x90, 0x34, 0x78, 0x12}
    };
    
    DB = (struct DBImage *)ptr;
    int i, j;
    volatile UInt8 m2[16][4], m3[16][4];
    
    char uartString[50];
    while(1){
        resetTask(DB,ID2DMATRIX);
        for(int k = 0; k < ITER2DMATRIX; k++){
            for(i = 0; i < 16; i++)
                {
                for(j=0; j < 4; j++)
                {
                    m2[i][j] = m1[i][j];
                    m3[i][j] = m2[i][j];
                }
            }
        }
        int ret;
        
        #ifdef USE_PROBABILITY
        if(getRandom(DB,ID2DMATRIX) >= WRITE_PRO)
            ret = commit(DB, ID2DMATRIX, NULL, 0, 4);
        else
            DB->NVMInVM->progress[ID2DMATRIX]++;
        GenerateRandom(DB);
        #else
            ret = commit(DB, ID2DMATRIX, NULL, 0, 4);
        #endif
        
        if(ret == SUCCESS)
            sprintf(uartString, "Two-dimension: SUCCESS\r\n");
        else
            sprintf(uartString, "Two-dimension: FAIL\r\n");
        Udprintf(uartString);
    }
    return;
}

/* [] END OF FILE */
