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

/* [] END OF FILE */

#include <config.h>
#include <project.h>

#define RECOVERYDATAROW 1

#define STACK_DEPTH                         (400u)
void taskRerun();

/* form a structure to easily alloacte the location of flash row */
struct recoveryData{
    unsigned short unfinished[NUMTASK];// 1: running, others for invalid
    void* address[NUMTASK];// Function address of tasks
    unsigned short priority[NUMTASK];
    unsigned short TCBNum[NUMTASK];
    void* TPara[NUMTASK];
    void* TCBAdd[NUMTASK];// TCB address of tasks
    int schedulerTask[NUMTASK];// if it is schduler's task, we don't need to recreate it because the shceduler does
    int firstTime;
};

void taskRerun();
void regTaskStart(void* add, unsigned short pri, unsigned short TCB, void* TCBA, void* parameter, int stopTrack);
void regTaskEnd();
void failureRecovery();
void freePreviousTasks();
/* function for flash-based platform*/
void initData();
void commitRecoveryData();
void restoreRecoveryData();