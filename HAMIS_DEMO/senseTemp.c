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

#include "senseTemp.h"
#include "SharedDB.h"
#include "FreeRTOS01.h"
#include "task01.h"

#if (CY_CPU_CORTEX_M0P)  /* core is Cortex-M0+ */
    #include "UARTM0.h"
#else /* core is Cortex-M4 */
    #include "UARTM4.h"
#endif

/*******************************************************************************
* Constants for configuring the SAR ADC clock and interrupt priority.
*******************************************************************************/
/* The ADC clock is an integer divider of the PeriClk.
*
* The maximum supported clock frequency for the SAR is 18 MHz.
* With a PeriClk of 50 MHz, the minimum target clock divider is 3, 
* SAR Clock = 16.67 MHz 
*/
#define SAR_TARGET_CLK_DIVIDER      (3u)
/* Set lowest priority for the SAR interrupt. */
#define SAR_IRQ_PRIORITY            (7u)

/*******************************************************************************
* Constants for configuring Config0 of the SAR ADC
*******************************************************************************/
/* Enable channels 0 and 1. */
#define CONFIG0_CHAN_EN             (3u)

/* Channel 0 is a differential channel.
* Aperture time is set by Sample Time 0.
* Positive terminal is connected to Pin 0 of the SARMUX dedicated port.
* Negative terminal is connected to Pin 1 of the SARMUX dedicated port (input differential pair).
* Averaging is disabled. */
#define CONFIG0_CHAN0_CONFIG        (CY_SAR_CHAN_DIFFERENTIAL_PAIRED \
                                    | CY_SAR_CHAN_SAMPLE_TIME_0 \
                                    | CY_SAR_POS_PORT_ADDR_SARMUX \
                                    | CY_SAR_CHAN_POS_PIN_ADDR_0 \
                                    | CY_SAR_CHAN_AVG_DISABLE)
                            
/* Channel 1 is a differential channel.
* Aperture time is set by Sample Time 1.
* Positive terminal is connected to Pin 2 of the SARMUX dedicated port.
* Negative terminal is connected to Pin 3 of the SARMUX dedicated port (input differential pair).
* Averaging is disabled. */
#define CONFIG0_CHAN1_CONFIG        (CY_SAR_CHAN_DIFFERENTIAL_PAIRED \
                                    | CY_SAR_CHAN_SAMPLE_TIME_1 \
                                    | CY_SAR_POS_PORT_ADDR_SARMUX \
                                    | CY_SAR_CHAN_POS_PIN_ADDR_2 \
                                    | CY_SAR_CHAN_AVG_DISABLE)

/* Set differential channels to use signed format. */
#define CONFIG0_SAMPLE_CTRL         (CY_SAR_DIFFERENTIAL_SIGNED)

/* Channels 2 through 15 are unconfigured. */
#define CONFIG0_CHAN_CONFIG         {(uint32_t)CONFIG0_CHAN0_CONFIG \
                                    , (uint32_t)CONFIG0_CHAN1_CONFIG \
                                    , 0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL}
                                
/* Enable the End of Scan interrupt only. */
#define CONFIG0_INTR_MASK           (CY_SAR_INTR_EOS_MASK)
                                
/* Use the internal 1.2 V bandgap reference for the SAR reference source.
* Enable the bypass capacitor connection. */
#define CONFIG0_VREF_MV_VALUE       (1200uL)
#define CONFIG0_CTRL                (CY_SAR_VREF_SEL_BGR \
                                    | CY_SAR_BYPASS_CAP_ENABLE)
                            
/* Set the aperture times to target a 50 ksps scan rate.
* Recall that the SAR clock is 16.67 MHz.
* Sample Time 0 is set to 302 clock cycles.
* Sample Time 1 is set to 4 clock cycles. */
#define CONFIG0_SAMPLE_TIME01       ((302 << CY_SAR_SAMPLE_TIME0_SHIFT) \
                                    | (4 << CY_SAR_SAMPLE_TIME1_SHIFT))

/* Set the unused sample times 2 and 3 to be 4 clock cycles.
* Note that these two sample times are not used by any channels and only shown for reference */
#define CONFIG0_SAMPLE_TIME23       ((4 << CY_SAR_SAMPLE_TIME2_SHIFT) \
                                    | (4 << CY_SAR_SAMPLE_TIME3_SHIFT))

/* Set the initial state of switches.
* Close the switch between Pin 0 of the SARMUX to the positive terminal of the SAR (used by channel 0).
* Close the switch between Pin 1 of the SARMUX to the negative terminal of the SAR (used by channel 0).
* Close the switch between Pin 2 of the SARMUX to the positive terminal of the SAR (used by channel 1).
* Close the switch between Pin 3 of the SARMUX to the negative terminal of the SAR (used by channel 1). */
#define CONFIG0_MUX_SWITCH0         (CY_SAR_MUX_FW_P0_VPLUS \
                                    | CY_SAR_MUX_FW_P1_VMINUS \
                                    | CY_SAR_MUX_FW_P2_VPLUS \
                                    | CY_SAR_MUX_FW_P3_VMINUS)

/* Enable sequencer control of the SARMUX Pins 0 to 3 used by channel 0 and channel 1. */
#define CONFIG0_MUX_SWITCH_SQ_CTRL  (CY_SAR_MUX_SQ_CTRL_P0 \
                                    | CY_SAR_MUX_SQ_CTRL_P1 \
                                    | CY_SAR_MUX_SQ_CTRL_P2 \
                                    | CY_SAR_MUX_SQ_CTRL_P3)

/* Define the initialization structure for Config1. */
const cy_stc_sar_config_t config0 =
{
    .ctrl               = (uint32_t)CONFIG0_CTRL,
    .sampleCtrl         = (uint32_t)CONFIG0_SAMPLE_CTRL,
    .sampleTime01       = CONFIG0_SAMPLE_TIME01,
    .sampleTime23       = CONFIG0_SAMPLE_TIME23,
    .rangeThres         = CY_SAR_DEINIT,
    .rangeCond          = CY_SAR_RANGE_COND_BELOW,
    .chanEn             = CONFIG0_CHAN_EN,
    .chanConfig         = CONFIG0_CHAN_CONFIG,
    .intrMask           = CONFIG0_INTR_MASK,         
    .satIntrMask        = CY_SAR_DEINIT,                /* Disable the saturation interrupt. */
    .rangeIntrMask      = CY_SAR_DEINIT,                /* Disable the range interrupt. */
    .muxSwitch          = CONFIG0_MUX_SWITCH0,
    .muxSwitchSqCtrl    = CONFIG0_MUX_SWITCH_SQ_CTRL,
    .configRouting      = true,
    .vrefMvValue        = CONFIG0_VREF_MV_VALUE,
};


/*******************************************************************************
* Constants for configuring Config1 of the Scan_ADC
*******************************************************************************/
/* Enable only channel 0. */
#define CONFIG1_CHAN_EN             (1u)

/* Channel 0 is single ended.
* Aperture time is set by Sample Time 0.
* The DieTemp sensor is connected on the SARMUX_VIRT port at PIN_ADDR 0.
* The sensor is enabled once connection to the SAR ADC is made.
* Averaging is enabled. */
#define CONFIG1_CHAN0_CONFIG        (CY_SAR_CHAN_SINGLE_ENDED \
                                    | CY_SAR_CHAN_SAMPLE_TIME_0 \
                                    | CY_SAR_POS_PORT_ADDR_SARMUX_VIRT \
                                    | CY_SAR_CHAN_POS_PIN_ADDR_0 \
                                    | CY_SAR_CHAN_AVG_ENABLE)
                            
/* Single ended channels are signed.
* Averaging mode is set to sequential fixed with 32 samples of averaging. */
#define CONFIG1_SAMPLE_CTRL         (CY_SAR_SINGLE_ENDED_SIGNED \
                                    | CY_SAR_AVG_CNT_32 \
                                    | CY_SAR_AVG_MODE_SEQUENTIAL_FIXED)

/* Channels 1 through 15 are unconfigured. */
#define CONFIG1_CHAN_CONFIG         {(uint32_t)CONFIG1_CHAN0_CONFIG \
                                    , 0uL, 0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL,0uL}
                                
/* Enable the End of Scan interrupt only. */
#define CONFIG1_INTR_MASK           (CY_SAR_INTR_EOS_MASK)
                                
/* Use the internal 1.2 V bandgap reference for the SAR reference source.
* Enable the bypass capacitor connection. 
* Connect the negative terminal for single ended channels to VSSA. */
#define CONFIG1_VREF_MV_VALUE       (1200uL)
#define CONFIG1_CTRL                (CY_SAR_VREF_SEL_BGR \
                                    | CY_SAR_BYPASS_CAP_ENABLE \
                                    | CY_SAR_NEG_SEL_VSSA_KELVIN)

/* Config1 will operate in single shot mode.
* Set the sample time to meet the DieTemp settling time requirement of 1 us.
* With a 16.67 MHz SAR clock, 17 cycles (or a value of 18 in the register)
* gives an aperture time of 1.02 us. */
#define CONFIG1_SAMPLE_TIME01       ((18 << CY_SAR_SAMPLE_TIME0_SHIFT) \
                                    | (4 << CY_SAR_SAMPLE_TIME1_SHIFT))

/* Set the sample times for 2 and 3 to be 4 clock cycles.
* Note that these two sample times are not used by any channels and only shown for reference. */
#define CONFIG1_SAMPLE_TIME23       ((4 << CY_SAR_SAMPLE_TIME2_SHIFT) \
                                    | (4 << CY_SAR_SAMPLE_TIME3_SHIFT))

/* Set the initial state of switches.
* Close the switch between the DieTemp sensor and the positive terminal of the SAR (TEMP_VPLUS).
* Close the switch between VSSA and the negative terminal of the SAR (VSSA_VMINUS). */
#define CONFIG1_MUX_SWITCH0         (CY_SAR_MUX_FW_VSSA_VMINUS \
                                    | CY_SAR_MUX_FW_TEMP_VPLUS)

/* Enable sequencer control for the VSSA and TEMP switches.
* While unnecessary in this design because there is only one channel in Config1, 
* the code is provided for reference for designs with multiple channels. */
#define CONFIG1_MUX_SWITCH_SQ_CTRL  (CY_SAR_MUX_SQ_CTRL_VSSA \
                                    | CY_SAR_MUX_SQ_CTRL_TEMP)

/* Define the initialization structure for Config1. */
const cy_stc_sar_config_t config1 =
{
    .ctrl               = (uint32_t)CONFIG1_CTRL,
    .sampleCtrl         = (uint32_t)CONFIG1_SAMPLE_CTRL,
    .sampleTime01       = CONFIG1_SAMPLE_TIME01,
    .sampleTime23       = CONFIG1_SAMPLE_TIME23,
    .rangeThres         = CY_SAR_DEINIT,
    .rangeCond          = CY_SAR_RANGE_COND_BELOW,                
    .chanEn             = CONFIG1_CHAN_EN,
    .chanConfig         = CONFIG1_CHAN_CONFIG,
    .intrMask           = CONFIG1_INTR_MASK,         
    .satIntrMask        = CY_SAR_DEINIT,                /* Disable the saturation interrupt. */
    .rangeIntrMask      = CY_SAR_DEINIT,                /* Disable the range interrupt. */
    .muxSwitch          = CONFIG1_MUX_SWITCH0,
    .muxSwitchSqCtrl    = CONFIG1_MUX_SWITCH_SQ_CTRL,
    .configRouting      = true,
    .vrefMvValue        = CONFIG1_VREF_MV_VALUE,
};


/* Pointer to the active configuration structure, initialized to Config0. *
*  Firmware uses this pointer to switch configuration of the SAR */
const cy_stc_sar_config_t *activeConfig = &config0;

/* Configuration structure for the SAR interrupt. */
const cy_stc_sysint_t SAR_IRQ_cfg = {
    .intrSrc        	= pass_interrupt_sar_IRQn,
    .intrPriority   	= SAR_IRQ_PRIORITY
};

struct DBImage* DB;
/*******************************************************************************
* Function Name: senseTemp
********************************************************************************
*
* Summary:
*  The main function performs the following actions:
*   1. Initializes UART and SAR ADC
*   2. Starts SAR ADC
*   3. Transmit 2 ADC channel values and DieTemp each second over UART
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
int senseTemp(void *ptr)
{
    /* For our DB */
    DB = (struct DBImage *)ptr;
    resetTask(DB,IDTEMP);
    
    /* Enable analog reference block needed by the SAR. */
    Cy_SysAnalog_Init(&Cy_SysAnalog_Fast_Local);
    Cy_SysAnalog_Enable();
        
    /* Configure the clock for the SAR for a 16.67 MHz clock frequency. */
	Cy_SysClk_PeriphAssignDivider(PCLK_PASS_CLOCK_SAR, CY_SYSCLK_DIV_8_BIT, 1u);
	Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 1u, SAR_TARGET_CLK_DIVIDER - 1u);
	Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 1u);   

    /* Switch to Config1 to scan the DieTemp sensor. */
    activeConfig = &config1;
    Cy_SAR_Init(SAR, activeConfig);
    Cy_SAR_Enable(SAR);
    /* Initiate a single conversion. */
    Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_SINGLE_SHOT);
    
    
    cy_en_sar_status_t sar_status;
    uint32_t intrStatus = 0u;
    /* An array to store the channel results, two channels for Config0 and 1 channel for Config1 */
    static int16_t chanResults[3];
    /* String to transmit over UART. */
    char uartString[50];
    while (1)
    {
        volatile uint32 a = 0;//Cy_MCWDT_GetCount(MCWDT_HW, CY_MCWDT_CTR0);
        volatile uint32 b = a;
        sar_status = Cy_SAR_IsEndConversion(SAR,CY_SAR_RETURN_STATUS);
        
        if(sar_status == CY_SAR_SUCCESS)
        {
             /* Get the ADC result for the DieTemp sensor in Config1. 
            * Convert the ADC results to degrees Celsius. */
            chanResults[2] = (int16_t) DieTemp_CountsTo_Celsius(Cy_SAR_GetResult16(SAR, 0));
            
            long* work = getWorking(DB, IDTEMP, OBJ_TEMP);
            *work = (long)chanResults[2];
            
            int ret;
            int objid = OBJ_TEMP;
            #ifdef USE_PROBABILITY
            if(getRandom(DB,IDTEMP) >= WRITE_PRO)
                ret = commit(DB, IDTEMP, NULL, 0, 4);
            else
                DB->NVMInVM->progress[IDTEMP]++;
            #else
                ret = commit(DB, IDTEMP, NULL, 0, 4);
            #endif
                        
            /* Transmit results over UART. */
            if(ret == SUCCESS)
                sprintf(uartString, "DieTemp = %hd degC: SUCCESS\r\n",chanResults[2]);
            else
                sprintf(uartString, "DieTemp = %hd degC: FAIL\r\n",chanResults[2]);
                      
            Udprintf(uartString);
            
            activeConfig = &config1;
            Cy_SAR_Init(SAR, activeConfig);
            Cy_SAR_Enable(SAR);
            /* Initiate a single conversion. */
            Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_SINGLE_SHOT);
            
            taskYIELD();
        }
    }

    vTaskDelete( NULL );
}

/*******************************************************************************
* Function Name: DieTemp_CountsTo_Celsius
********************************************************************************
*
*  Summary:
*  Function to convert ADC counts to degrees Celsius. For details on operation 
*  please see the Die Temperature (DieTemp) Component datasheet.
*    
*  Parameters:
*  int16_t adcCounts - ADC counts for DieTemp scan from ADC.
*
*  Return:
*  int32_t - Temperature in whole degrees Celsius.
*
**********************************************************************************/
/*******************************************************************************
* Constants used to convert ADC counts to degrees Celsius
* for the DieTemp sensor.
*******************************************************************************/
#define DieTemp_SAR_TEMP_OFFSET_SHIFT      (10u)
#define DieTemp_SAR_TEMP_OFFSET_MULT       (0x400)
#define DieTemp_SAR_TEMP_OFFSET_DIVIDER    (0x10000)
#define DieTemp_SAR_TEMP_SHIFT             (16u)
#define DieTemp_SAR_TEMP_DIVIDER           (0x10000)
#define DieTemp_SCALE_ADJUSTMENT_DIVIDER   (16u)
#define DieTemp_HALF_OF_ONE                ((int32)1u << (DieTemp_SAR_TEMP_SHIFT - 1u))

/* (effectively 0.5 << 4u) 0.5 in Q28.4 format */
#define DieTemp_SCALE_ADJUSTMENT           (8)
/* 15 in Q16.16 format */
#define DieTemp_DUAL_SLOPE_CORRECTION      (0xF0000)
/* 100 in Q16.16 format */
#define DieTemp_HIGH_TEMPERATURE           (0x640000)
/* 40 in Q16.16 format */
#define DieTemp_LOW_TEMPERATURE            (0x280000)

int32_t DieTemp_CountsTo_Celsius(int16_t adcCounts)
{
    int32_t tempCelsius;
    int32_t tInitial;
    int32_t tAdjust;
    int32_t offsetReg;
    int32_t multReg;

    offsetReg = (int16_t)SFLASH->SAR_TEMP_OFFSET;
    multReg   = (int16_t)SFLASH->SAR_TEMP_MULTIPLIER;
    
    /* Calculate tInitial in Q16.16 */
    tInitial = (adcCounts * multReg) + (offsetReg * DieTemp_SAR_TEMP_OFFSET_MULT);

    if(tInitial >= DieTemp_DUAL_SLOPE_CORRECTION)
    {
        /* Shift (100 - tInitial) by 4 bits to prevent scale-adjustment from overflowing. */
        /* Then divide by the integer bits of (100 - cutoff) to end up with a Q16.16 tAdjust */
        tAdjust = (DieTemp_SCALE_ADJUSTMENT * (((int32)DieTemp_HIGH_TEMPERATURE - tInitial)
            / (int32)DieTemp_SCALE_ADJUSTMENT_DIVIDER)) /
            (((int32)DieTemp_HIGH_TEMPERATURE - (int32)DieTemp_DUAL_SLOPE_CORRECTION) /
            DieTemp_SAR_TEMP_DIVIDER);
    }
    else
    {
        /* Shift (40 + tInitial) by 4 bits to prevent scale-adjustment from overflowing. */
        /* Then divide by the integer bits of (40 + cutoff) to end up with a Q16.16 tAdjust */
        tAdjust = ((int32)DieTemp_SCALE_ADJUSTMENT * (((int32)DieTemp_LOW_TEMPERATURE + tInitial)
           / (int32)DieTemp_SCALE_ADJUSTMENT_DIVIDER)) /
            (((int32)DieTemp_LOW_TEMPERATURE + (int32)DieTemp_DUAL_SLOPE_CORRECTION) /
            (int32)DieTemp_SAR_TEMP_DIVIDER);
    }

    /* Add tInitial + tAdjust + 0.5 to round to nearest int. Shift off frac bits, and return. */
    tempCelsius = tInitial + tAdjust + DieTemp_HALF_OF_ONE;

    return (tempCelsius / DieTemp_SAR_TEMP_OFFSET_DIVIDER);
}


/* [] END OF FILE */
