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

/* 0x10000000 – 0x10100000 User Flash */
//M0: 0x10000000 - 0x10040000 
//Recovery for M0: 0x10040000 - 0x10060000 
//M4: 0x10080000 - 0x100C0000 
//Recovery for M4: 0x10060000 - 0x10080000 
//Shared data: 0x100C000 - 0x10100000
#define LAST_FLASH_ROW  2040u //2047u
#define FIRST_ROW       512//1470u//768 // take a row
#define SHARED0_ROW      1536
#define SHARED1_ROW      1537
#define SIZEOFROW       512u
#define RECOVERY_ROW 0 // take a row for NVM used by the recovery handler
#define MAIN_ROW 1 // NVM used by main
#define PROGRESS_ROW1 2 //saved the progress, whcih takes 1 row
#define PROGRESS_ROW2 3 //saved the progress, whcih takes 1 row
/* Address for a specific row */
#define CALCULATE_FLASH_ADDRESS(rowNum) (CY_FLASH_BASE + ((rowNum+FIRST_ROW) * CY_FLASH_SIZEOF_ROW))
/* Address for the shared data */
#define SHARED0_ADDRESS (CY_FLASH_BASE + (SHARED0_ROW * CY_FLASH_SIZEOF_ROW))
#define SHARED1_ADDRESS (CY_FLASH_BASE + (SHARED1_ROW * CY_FLASH_SIZEOF_ROW))

/* enable intterupts for flash after FreeRTOS disable them, e.g., resumes all tasks  */
void interruptEnable();
/* inintial variables for controlling flash */
void initFlashStructure();
/* interrupt service function */
void Flash_ISR (void);
/* write a row of flash */
int rowtoFlash(void* from, void* to);
/* application interface for copying data to flash */
int copyDatatoFlash(void* from, void* to, int size);

/* [] END OF FILE */
