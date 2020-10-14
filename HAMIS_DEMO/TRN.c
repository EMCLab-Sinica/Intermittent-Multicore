///* ========================================
// *
// * Copyright YOUR COMPANY, THE YEAR
// * All Rights Reserved
// * UNPUBLISHED, LICENSED SOFTWARE.
// *
// * CONFIDENTIAL AND PROPRIETARY INFORMATION
// * WHICH IS THE PROPERTY OF your company.
// *
// * ========================================
//*/
//
//#include "project.h"
//#include "crypto/cy_crypto.h"
//#include "crypto/cy_crypto_server.h"
//#include <stdio.h>
//#include "TRN.h"
//#include "SharedDB.h"
//
///* Macros to configure the Crypto block */
///* IPC data channel for the Crypto */
//#define MY_CHAN_CRYPTO         				(uint32_t)(3u)  
///* IPC interrupt structure for the Crypto server */  
//#define MY_INTR_CRYPTO_SRV     				(uint32_t)(1u) 
///* IPC interrupt structure for the Crypto client */   
//#define MY_INTR_CRYPTO_CLI     				(uint32_t)(2u) 
///* CM0+ IPC interrupt mux number the Crypto server */   
//#define MY_INTR_CRYPTO_SRV_MUX 				(IRQn_Type)(2u)
///* CM0+ IPC interrupt mux number the Crypto client */   
//#define MY_INTR_CRYPTO_CLI_MUX 				(IRQn_Type)(3u)
///* CM0+ ERROR interrupt mux number the Crypto server */  
//#define MY_INTR_CRYPTO_ERR_MUX 				(IRQn_Type)(4u)   
//
///* Macros for the polynomial to configure the programmable Galois and Fibonacci
//   ring oscillators */
//#define CRYPTO_TRNG_GARO_POL             (0x42000000)
//#define CRYPTO_TRNG_FIRO_POL             (0x43000000)
//
///* Macro for the maximum value of the random number generated in bits */
//#define MAX_TRND_VAL                     (7u)
//
//#define PASSWORD_LENGTH                  (8u)
//#define LOWER_LIMIT   (0u)
//#define UPPER_LIMIT   (100)
//#define ASCII_RETURN_CARRIAGE            (0x0D)
//
///* Crypto context structure to store and manipulate the global context */
//cy_stc_crypto_context_t cryptoScratch;
//
//cy_stc_crypto_server_context_t cryptoServerContext;
//
///* Crypto context structure to store and manipulate the TRNG operational context */
//cy_stc_crypto_context_trng_t TRNGContext;
//
///* Crypto configuration structure */
//const cy_stc_crypto_config_t cryptoConfig1 =
//    {
//        /* .ipcChannel             */ MY_CHAN_CRYPTO,
//        /* .acquireNotifierChannel */ MY_INTR_CRYPTO_SRV,
//        /* .releaseNotifierChannel */ MY_INTR_CRYPTO_CLI,
//        /* .releaseNotifierConfig */ {
//        #if (CY_CPU_CORTEX_M0P)
//            /* .intrSrc            */ MY_INTR_CRYPTO_CLI_MUX,
//            /* .cm0pSrc            */ cpuss_interrupts_ipc_2_IRQn, /* depends on selected releaseNotifierChannel value */
//        #else
//            /* .intrSrc            */ cpuss_interrupts_ipc_2_IRQn, /* depends on selected releaseNotifierChannel value */
//        #endif
//            /* .intrPriority       */ 2u,
//        },
//        /* .userCompleteCallback   */ NULL,
//        /* .userGetDataHandler     */ NULL,
//        /* .userErrorHandler       */ NULL,
//        /* .acquireNotifierConfig */ {
//        #if (CY_CPU_CORTEX_M0P)
//            /* .intrSrc            */ MY_INTR_CRYPTO_SRV_MUX,      /* to use with DeepSleep mode should be in DeepSleep capable muxer's range */
//            /* .cm0pSrc            */ cpuss_interrupts_ipc_1_IRQn, /* depends on selected acquireNotifierChannel value */
//        #else
//            /* .intrSrc            */ cpuss_interrupts_ipc_1_IRQn, /* depends on selected acquireNotifierChannel value */
//        #endif
//            /* .intrPriority       */ 2u,
//        },
//        /* .cryptoErrorIntrConfig */ {
//        #if (CY_CPU_CORTEX_M0P)
//            /* .intrSrc            */ MY_INTR_CRYPTO_ERR_MUX,
//            /* .cm0pSrc            */ cpuss_interrupt_crypto_IRQn,
//        #else
//            /* .intrSrc            */ cpuss_interrupt_crypto_IRQn,
//        #endif
//            /* .intrPriority       */ 2u,
//        }
//    };
//
///* UART configuration structure */
//cy_stc_scb_uart_context_t uartContext;
//    
//static bool inused;
///*******************************************************************************
//*******************************************************************************/
//void InitCryto(void * ptr)
//{
//    struct DBImage *DB = ptr;
//    
//    DB->VMAdd->TRNconext = &TRNGContext;
//    uint8_t TRNGData[PASSWORD_LENGTH + 1];
//
//	/* Start the Crypto Server */
//	Cy_Crypto_Server_Start(&cryptoConfig1, &cryptoServerContext);
//
//	/*Initialization of Crypto Driver */
//	Cy_Crypto_Init(&cryptoConfig1, &cryptoScratch);
//
//	/* Enable Crypto Hardware */
//	Cy_Crypto_Enable();
//
//	/* Wait for Crypto Block to be available */
//	Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);
//    
//    for(int i = 0; i < NUMTASK ; i++)
//        DB->VMAdd->randomused[i] = true;
//    
//    inused = false;
//    GenerateRandom(DB);
//}
//
///*******************************************************************************
//*******************************************************************************/
//void GenerateRandom(void * ptr)
//{
//    int32_t index;
//    uint32_t temp;
//
//    struct DBImage *DB = ptr;
//    
//    if(inused)
//        return;
//    else
//        inused = true;
//    for(int i = 0; i < NUMTASK ; i++)
//    {
//        if(DB->VMAdd->randomused[i]){
//            /* Generate a random number */
//            Cy_Crypto_Trng_Generate(CRYPTO_TRNG_GARO_POL, CRYPTO_TRNG_FIRO_POL,\
//                                    MAX_TRND_VAL, &temp, DB->VMAdd->TRNconext);
//
//            /* Wait until crypto completes operation */
//            Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);
//
//            /* Check if the generated random number is in the range of alpha-numeric,
//            special characters ASCII codes. If not, convert to that range */
//            if (temp > UPPER_LIMIT)
//                temp = temp % UPPER_LIMIT;
//                    
//            DB->VMAdd->random[i] = temp;
//            DB->VMAdd->randomused[i] = false;
//        }
//    }
//    inused = false;
//}
//
//
//unsigned int getRandom(void * ptr, int taskID)
//{
//    struct DBImage *DB = ptr;
//    DB->VMAdd->randomused[taskID] = true;    
//    return DB->VMAdd->random[taskID];
//}
//
///* [] END OF FILE */
//