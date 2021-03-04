/******************************************************************************
* File Name: touch.c
*
* Version: 1.1
*
* Description: This file contains the functions used to initialize and read touch
*              information from the CapSense sensors 
*
* Related Document: N/A
*
* Hardware Dependency: CY8CKIT-062-WiFi-BT PSoC 6 WiFi-BT Pioneer Kit
*
******************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation.
******************************************************************************
* This software, including source code, documentation and related materials
* ("Software") is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress source code and derivative works for the sole purpose of creating
* custom software in support of licensee product, such licensee product to be
* used only in conjunction with Cypress's integrated circuit as specified in the
* applicable agreement. Any reproduction, modification, translation, compilation,
* or representation of this Software except as specified above is prohibited
* without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes to the Software without notice.
* Cypress does not assume any liability arising out of the application or use
* of Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use as critical components in any products
* where a malfunction or failure may reasonably be expected to result in
* significant injury or death ("ACTIVE Risk Product"). By including Cypress's
* product in a ACTIVE Risk Product, the manufacturer of such system or application
* assumes all risk of such use and in doing so indemnifies Cypress against all
* liability. Use of this Software may be limited by and subject to the applicable
* Cypress software license agreement.
*****************************************************************************/
/*****************************************************************************
* This file contains the functions used to initialize and read touch
* information from the CapSense sensors 
*******************************************************************************/

/* Header file includes */
#include <project.h>
#include "touch.h"
#include "rtos.h"
#include "SharedDB.h"
#include "stdio.h"
#include "FreeRTOS01.h"
#include "task01.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

/*******************************************************************************
* Function Name: TouchTask(void)
********************************************************************************
*
* Summary:
*   Scans the CapSense widgets, performs analysis and updates the touch 
*   information
*
* Parameters:
*  arg: Required argument for task function. 
*
* Side Effects :
*  None
*
*******************************************************************************/
struct DBImage* DB;
void TouchTask(void *arg)
{
    
     DB = (struct DBImage *)arg;
     resetTask(DB,IDTOUCH);   
    
     /* Start touch */
     EZI2C_1_Start(); /* Start EZI2C Component */
     /*
     * Set up communication and initialize data buffer to CapSense data structure
     * to use Tuner application
     */
     EZI2C_1_SetBuffer1((uint8_t *)&CapSense_dsRam,
     sizeof(CapSense_dsRam),
        sizeof(CapSense_dsRam));
     CapSense_Start(); /* Initialize Component */
     CapSense_ScanAllWidgets(); /* Scan all widgets */
     for(;;)
     {
         /* Do this only when a scan is done */
         if(CapSense_NOT_BUSY == CapSense_IsBusy())
         {
            CapSense_ProcessAllWidgets(); /* Process all widgets */
            CapSense_RunTuner(); /* To sync with Tuner application */
            
            bool touch;
            char uartString[50];
            if (CapSense_IsAnyWidgetActive()) /* Scan result verification */
            {
                //Touch
                touch = true;
                sprintf(uartString, "Touch: ");
            }
            else
            {
                touch = false;
                sprintf(uartString, "No touch: ");
            }
            long* work = getWorking(DB, IDTOUCH, OBJ_TOUCH);
            *work = (long)touch;
            
            int ret;
            int objid = OBJ_TOUCH;
            #ifdef USE_PROBABILITY
            if(getRandom(DB,IDTOUCH) >= WRITE_PRO)
                ret = commit(DB, IDTOUCH, NULL, 0, 4);
            else
                DB->NVMInVM->progress[IDTOUCH]++;
            #else
                ret = commit(DB, IDTOUCH, NULL, 0, 4);
            #endif
            
            Udprintf(uartString);
            
            /* Transmit results over UART. */
            if(ret == SUCCESS)
                sprintf(uartString, "SUCCESS\r\n");
            else
                sprintf(uartString, "FAIL\r\n");
                      
            Udprintf(uartString);
            
            vTaskDelay(1);
            CapSense_ScanAllWidgets(); /* Start next scan */
            taskYIELD();
        }
    }
}

/* [] END OF FILE */
