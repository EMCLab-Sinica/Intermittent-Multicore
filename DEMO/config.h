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
#define DATACONSISTENCY 1
#define OUR //read from NVM/VM, write to VM

//#ifndef UARTDEBUG
//#define UARTDEBUG
//#endif
#define DELAYCOMMIT
#define ADAPTIVE
#define LOGBLOCKING

#define NUMTASK 10
#define NUMOBJ 10
#define MAXSIZE 4

//Objects
#define OBJ_TEMP 2
#define OBJ_TOUCH 3
#ifdef USE_PROBABILITY
#define WRITE_PRO 0
#endif

//Used for TI applications
#define ITERFIR 1
#define ITERMATH16 100
#define ITERMATH32 300
#define ITER2DMATRIX 2
#define ITERMATRIXMUL 10
#define ITERFLOATING 10

#define IDMATH 0
#define IDTEMP IDMATH+1
#define IDTOUCH IDTEMP+1
#define IDMATRIXMUL IDTOUCH+1
#define IDTRANSMIT IDMATRIXMUL+1
#define IDFFT IDTRANSMIT+1//
#define IDDFT IDTRANSMIT+1
#define IDSHA IDDFT+1
#define IDFIR IDSHA+1//
#define ID2DMATRIX IDSHA+1
#define IDFLAOTING ID2DMATRIX+1


//#define IDCAPCALIBRATE 2
//#define IDTEMPCALIBRATE 3
//#define IDUART 4
////#define IDFIR 5
//#define IDMATH16 6
//#define IDMATH32 7
//#define ID2DMATRIX 8
//#define IDMATRIXMUL 9
