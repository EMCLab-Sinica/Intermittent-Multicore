/*
 * matrixmultiplication.h
 *
 */
#include <config.h>
#include "FreeRTOS.h"
#include "task.h"
#include "SharedDB.h"
#include "stdio.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

/*******************************************************************************
*
* Name : Matrix Multiplication
* Purpose : Benchmark multiplying a 3x4 matrix by a 4x5 matrix.
* Matrix contains 16-bit values.
*
*******************************************************************************/
typedef unsigned short UInt16;
const UInt16 m1[3][4] = {
    {0x01, 0x02, 0x03, 0x04},
    {0x05, 0x06, 0x07, 0x08},
    {0x09, 0x0A, 0x0B, 0x0C}
    };
const UInt16 m2[4][5] = {
    {0x01, 0x02, 0x03, 0x04, 0x05},
    {0x06, 0x07, 0x08, 0x09, 0x0A},
    {0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    {0x10, 0x11, 0x12, 0x13, 0x14}
    };

volatile UInt16 Fm1[3][5];
volatile UInt16 Fm2[3][5];
volatile UInt16 Fm3[3][5];
volatile UInt16 Sm1[3][5];
volatile UInt16 Sm2[3][5];
volatile UInt16 Sm3[3][5];


struct DBImage* DB;
void matrixmultiplication(void* ptr)
{
    int m, n, p;

    DB = (struct DBImage *)ptr;
    int k;
    volatile int swcount;
    swcount=0;
    
    char uartString[50];
    while(1){
        /* For our DB */
        resetTask(DB,IDMATRIXMUL);
        for(k = 0; k < ITERMATRIXMUL; k++){
            for(m = 0; m < 3; m++)
            {
                for(p = 0; p < 5; p++)
                {
                    Sm3[m][p] = 0;

                    for(n = 0; n < 4; n++)
                    {
                        Sm3[m][p] += Sm1[m][n] * Sm2[n][p];
                    }
                }
            }
        }
        int ret = commit(DB,IDMATRIXMUL, NULL, 0, 4);
        
        if(ret == SUCCESS)
            sprintf(uartString, "MatrixMUL: SUCCESS\r\n");
        else
            sprintf(uartString, "MatrixMUL: FAIL\r\n");
        Udprintf(uartString);
    }
}

