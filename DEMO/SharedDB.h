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
#include "config.h"
#include "stdint.h"
#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "FreeRTOS01.h"
    #include "task01.h"
#else /* core is Cortex-M4 */
    #include "FreeRTOS.h"
    #include "task.h"
#endif
#include "TRN.h"

#define SHAREDDB
/* Semaphore number to be used in DB */
#define VALID_SEMA    1u
#define NVM_SEMA    2u


#define SUCCESS 1
#define FAIL 0
#define MAX_DELAY 10


/* Number of data object and maximum of their size */

struct commitRecord{//for the two phase commit
    uint64_t begin;
    uint64_t end;
    uint64_t commitTime;
    int objCommit;
    bool valid;
};

struct working{//working space of data for tasks
    void* address;
    int loc;//1 stands for SRAM, 0 stands for　NVM
    int id;//-1 for create
};


//a data copy's structure
struct data
{
    char buffer[MAXSIZE];
    uint64_t begin;
    uint64_t end;
    unsigned int size;
};

//Structure maintains consistent versions in VM
struct sharedVM
{
    struct data objWorking[NUMTASK][NUMOBJ];
    // used for validation: Task t with Task's TCB = WSRTCB[i], SRBegin[NUMTASK] = min(writer's begin), WSRValid[NUMTASK] = 1
    uint64_t TBegin[NUMTASK]; //The "begin time of each task"
    uint64_t RAWEnd[NUMTASK]; //Each obj read by a task is written
    bool RObj[NUMTASK][NUMOBJ]; //Each obj read by a task is written
    bool RAWObj[NUMTASK][NUMOBJ]; //Each obj read by a task is written
    uint64_t TEnd[NUMTASK]; //The "end time of each task"
    //second phase commit
    struct commitRecord p2Records[NUMTASK];
    void * TRNconext;
    int random[NUMTASK];
    bool randomused[NUMTASK];
};

//Structure maintains consistetnt versions
struct sharedNVM
{
    struct data objConsistent[NUMOBJ];    
    unsigned long progress[NUMTASK];
    int versionNum;
    int valid;
    unsigned long timeCounter;
    unsigned long blockM0;
    unsigned long blockM4;
};

//Structure maintaining whole DB versions
struct DBImage
{
    struct sharedNVM* NVMAdd0;
    struct sharedNVM* NVMAdd1;
    bool validNVM;// false: 0, true: 1
    struct sharedNVM* NVMInVM;// used for consistent in VM and can be used to copy consistent data to NVM 
    struct sharedVM* VMAdd;  
    unsigned long resume_progress[NUMTASK];
    unsigned long resume_blockM0;
    unsigned long resume_blockM4;
    int errorNum;
    int delayed;
    uint32_t CxtSwtM0;
    uint32_t CxtSwtM4;
};

void resetTask(struct DBImage* DB, int taskID);
void resetAllTasks(struct DBImage* DB);
unsigned long getCurrentTime(struct DBImage* DB);

void* read(struct DBImage* DB, int taskID, int objID);
void* getWorking(struct DBImage* DB, int taskID, int objID);
int commit(struct DBImage* DB, int taskID, int* objID, int numObj,int size);
void commitToNVM(struct DBImage* DB);


/* [] END OF FILE */
