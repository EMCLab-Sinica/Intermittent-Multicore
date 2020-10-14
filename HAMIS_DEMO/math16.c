/*
 * math16.c
 *
 */

#include <config.h>
#include "FreeRTOS.h"
#include "task.h"
#include "project.h"

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
void math16()
{
    int k;
    volatile int swcount;
    swcount = 0;
    while(1){
        for(k = 0; k < ITERMATH16; k++){
            Sresult[0] = 231;
            Sresult[1] = 12;
            Sresult[2] = Sresult[0] + Sresult[1];
            Sresult[1] = Sresult[0] * Sresult[2];
            Fresult[3] = Sresult[1] / Sresult[2];
        }

        swcount++;
    }
}

