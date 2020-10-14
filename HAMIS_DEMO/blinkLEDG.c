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
#include "blinkLEDG.h"
#define BLINK_INTERVAL 3000000

void blink()
{
    long x;
    bool on = true;
    Cy_GPIO_Clr(LED_GREEN_PORT, LED_GREEN_NUM);
    
    while(1){
        for(x = 0; x < BLINK_INTERVAL; x++);
               
        if(on){
            Cy_GPIO_Set(LED_GREEN_PORT, LED_GREEN_NUM);
            on = false;
        }
        else{
            Cy_GPIO_Clr(LED_GREEN_PORT, LED_GREEN_NUM);
            on = true;
        }
    }
}

/* [] END OF FILE */
