/*
 * matrixmultiplication.h
 *
 */
#include <config.h>
#include "FreeRTOS01.h"
#include "task01.h"
#include "SharedDB.h"
#include "stdio.h"
#include "matrixmultiplication01.h"

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

volatile UInt16 Sm1[3][5];
volatile UInt16 Sm2[3][5];
volatile UInt16 Sm3[3][5];



struct DBImage* DB;
void matrixmultiplication01(void* ptr)
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

