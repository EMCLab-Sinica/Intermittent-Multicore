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
#include <math.h>
#include "senseTemp.h"
#include "SharedDB.h"
#include "dft.h"
#include "stdio.h"

 
#define N 16
#define PI2 6.2832

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif


struct DBImage* DB;
void dft_task(void *ptr)
{
    DB = (struct DBImage *)ptr;
    
    // time and frequency domain data arrays
    int n, k;             // indices for time and frequency domains
    float x[N];           // discrete-time signal, x
    float Xre[N], Xim[N]; // DFT of x (real and imaginary parts)
    float P[N];           // power spectrum of x
     
    while(1){
        resetTask(DB,IDDFT);
        // Generate random discrete-time signal x in range (-1,+1)
        for (n=0 ; n<N ; ++n) x[n] = ((2.0 * 1) / 10) - 1.0;
         
        // Calculate DFT of x using brute force
        for (k=0 ; k<N ; ++k)
        {
            // Real part of X[k]
            Xre[k] = 0;
            for (n=0 ; n<N ; ++n) Xre[k] += x[n] * cos(n * k * PI2 / N);
             
            // Imaginary part of X[k]
            Xim[k] = 0;
            for (n=0 ; n<N ; ++n) Xim[k] -= x[n] * sin(n * k * PI2 / N);
             
            // Power at kth frequency bin
            P[k] = Xre[k]*Xre[k] + Xim[k]*Xim[k];
        }
        
        int ret;
        #ifdef USE_PROBABILITY
        if(getRandom(DB,IDDFT) >= WRITE_PRO)
            ret = commit(DB, IDDFT, NULL, 0, 4);
        else
            DB->NVMInVM->progress[IDDFT]++;
        
        GenerateRandom(DB);
        #else
            ret = commit(DB, IDDFT, NULL, 0, 4);   
        #endif
        char uartString[50];
        if(ret == SUCCESS)
            sprintf(uartString, "DFT: SUCCESS\r\n");
        else
            sprintf(uartString, "DFT: FAIL\r\n");
        Udprintf(uartString);
    }
    
}
/* [] END OF FILE */
