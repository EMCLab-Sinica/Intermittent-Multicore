/*******************************************************************************
*
* Name : FIR Filter
* Purpose : Benchmark an FIR filter. The input values for the filter
* is an array of 51 16-bit values. The order of the filter
* 17.
*
*******************************************************************************/
#include <math.h>
#include "senseTemp.h"
#include "SharedDB.h"
#include "FIR.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

#define FIR_LENGTH 17
const float COEFF[FIR_LENGTH] =
{
-0.000091552734, 0.000305175781, 0.004608154297, 0.003356933594, -0.025939941406,
-0.044006347656, 0.063079833984, 0.290313720703, 0.416748046875, 0.290313720703,
0.063079833984, -0.044006347656, -0.025939941406, 0.003356933594, 0.004608154297,
0.000305175781, -0.000091552734};

/* The following array simulates input A/D converted values */
const unsigned int INPUT[] =
{
0x0000, 0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000, 0x0000,
0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00, 0x2000,
0x2400, 0x2000, 0x1C00, 0x1800, 0x1400, 0x1000, 0x0C00, 0x0800,
0x0400, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00,
0x2000, 0x2400, 0x2000, 0x1C00, 0x1800, 0x1400, 0x1000, 0x0C00,
0x0800, 0x0400, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800,
0x1C00, 0x2000, 0x2400, 0x2000, 0x1C00, 0x1800, 0x1400, 0x1000,
0x0C00, 0x0800, 0x0400};


struct DBImage* DB;
void FIR_task(void *ptr)
{
    DB = (struct DBImage *)ptr;
    int i, y; /* Loop counters */
    volatile float OUTPUT[36],sum;

    while(1)
    {
        resetTask(DB,IDFIR);
        for(int k = 0; k < ITERFIR; k++)
            for(y = 0; y < 36; y++)
            {
                sum=0;
                for(i = 0; i < FIR_LENGTH/2; i++)
                {
                    sum = sum+COEFF[i] * ( INPUT[y + 16 - i] + INPUT[y + i] );
                }
                OUTPUT[y] = sum + (INPUT[y + FIR_LENGTH/2] * COEFF[FIR_LENGTH/2] );
            }
        int ret = commit(DB, IDFIR, NULL, 0, 4);
        
        char uartString[50];
        if(ret == SUCCESS)
            sprintf(uartString, "FIR: SUCCESS\r\n");
        else
            sprintf(uartString, "FIR: FAIL\r\n");
        Udprintf(uartString);
    }
    return;
}

/* [] END OF FILE */
