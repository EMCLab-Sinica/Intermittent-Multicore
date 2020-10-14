/***************************************************************************//**
* \file CapSense_SensingCSX_LL.h
* \version 2.0
*
* \brief
*   This file provides the headers of APIs specific to CSX sensing implementation.
*
* \see CapSense v2.0 Datasheet
*
*//*****************************************************************************
* Copyright (2016-2017), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#if !defined(CapSense_SENSING_CSX_LL_H)
#define CapSense_SENSING_CSX_LL_H

#include "syslib/cy_syslib.h"
#include "CapSense_Structure.h"
#include "CapSense_Configuration.h"

#if (CapSense_CSX_EN == 1u)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
* \cond SECTION_CYSENSE_LOW_LEVEL
* \addtogroup group_cysense_low_level
* \{
*/

void CapSense_CSXSetupWidget(uint32 widgetId);
void CapSense_CSXSetupWidgetExt(uint32 widgetId, uint32 sensorId);
void CapSense_CSXScan(void);
void CapSense_CSXScanExt(void);

#if (CapSense_CSX_IDAC_AUTOCAL_EN == CapSense_ENABLE)
    void CapSense_CSXCalibrateWidget(uint32 widgetId, uint16 target);
#endif /* (CapSense_CSX_IDAC_AUTOCAL_EN == CapSense_ENABLE) */

void CapSense_CSXConnectTx(CapSense_FLASH_IO_STRUCT const *txPtr);
void CapSense_CSXConnectRx(CapSense_FLASH_IO_STRUCT const *rxPtr);
void CapSense_CSXDisconnectTx(CapSense_FLASH_IO_STRUCT const *txPtr);
void CapSense_CSXDisconnectRx(CapSense_FLASH_IO_STRUCT const *rxPtr);

/** \}
* \endcond */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
* \cond SECTION_CYSENSE_INTERNAL
* \addtogroup group_cysense_internal
* \{
*/

void CapSense_CSXInitialize(void);
void CapSense_CSXElectrodeCheck(void);
uint32 CapSense_SsCSXGetTxClkDivider(uint32 widgetId);

/** \}
* \endcond */

/* The interrupt handler of the CSX sensing module */
void CapSense_CSXScanISR(void);

/*******************************************************************************
* Global variables exposed by the module
*******************************************************************************/

/* The local variable used to store a scan period */
extern volatile uint16 CapSense_csxScanPeriod;

/* The local variable used to return raw count from post scan ISR */
extern volatile uint16 CapSense_csxRawCount;

/* The pointer to the function used to register a callback in ISR */
extern void (* CapSense_CSXPostScanApiPtr)(void);


/***************************************
* API Constants
***************************************/
#define CapSense_CSX_DEADBAND_CYCLES_NUMBER             (2u)

/* The definition of the time interval is long enough for charging a 100-nF capacitor */
#define CapSense_CSX_MAX_CHARGE_TIME_US                 (100u)
#define CapSense_CSX_AVG_CYCLES_PER_LOOP                (5u)
#define CapSense_CSX_PRECHARGE_WATCHDOG_CYCLES_NUM      (((CapSense_CPU_CLOCK_FREQ_MHZ) * \
                                                                 (CapSense_CSX_MAX_CHARGE_TIME_US)) / \
                                                                 (CapSense_CSX_AVG_CYCLES_PER_LOOP))

/*******************************************************************************
* Register Configuration
*******************************************************************************/

/*
 * +--------+---------------+--------------------------------------------+-------------------------------------------+
 * |        |               | BLOCK_OFF_AFTER_SCAN=TRUE                  | BLOCK_OFF_AFTER_SCAN=FALSE                |
 * +  BITS  +   FIELD       +--------------------------------------------+-------------------------------------------+
 * |        |               |              MODE                          |              MODE                         |
 * |--------|---------------|--------------------------------------------|-------------------------------------------|
 * |  0     | IREF_SEL      | Select Iref according to IrefSel parameter | Select Iref according to IrefSel parameter|
 * |  8:4   | FILTER_DELAY  | 0x02(2-SampleClk cycles)                   | 0x02(2-SampleClk cycles)                  |
 * |  11:10 | SHIELD_DELAY  | 0x00(Delay Line is OFF)                    | 0x00(Delay Line is OFF)                   |
 * |  12    | SENSE_EN      | 0x00(Disabled. All switches are open)      | 0x01(Enabled. Switches can be closed)     |
 * |  17    | FULL_WAVE     | 0x00(Half Wave Mode)                       | 0x00(Half Wave Mode)                      |
 * |  18    | MUTUAL_CAP    | 0x01(Mutual-cap mode)                      | 0x01(Mutual-cap mode)                     |
 * |  19    | CSX_DUAL_CNT  | 0x01(Use two counters)                     | 0x01(Use two counters)                    |
 * |  24    | DSI_COUNT_SEL | 0x00(CSD-Result on the DSI_COUNT bus)      | 0x00(CSD-Result on DSI_COUNT bus)         |
 * |  25    | DSI_SAMPLE_EN | 0x00(DSI_SAMPLE_IN is disabled)            | 0x00(DSI_SAMPLE_IN is disabled)           |
 * |  26    | SAMPLE_SYNC   | 0x00(Double sync is disabled)              | 0x00(Double sync is disabled)             |
 * |  27    | DSI_SENSE_EN  | 0x00(DSI_SENSE_IN is disabled)             | 0x00(DSI_SENSE_IN is disabled)            |
 * |  30    | LP_MODE       | 0x00(High power mode)                      | 0x00(High power mode)                     |
 * |  31    | ENABLE        | 0x00(Analog part is OFF)                   | 0x01(Analog part is ON)                   |
 * +--------+---------------+--------------------------------------------+-------------------------------------------+*/
#if(CapSense_DISABLE != CapSense_BLOCK_OFF_AFTER_SCAN_EN)
    #define CapSense_CONFIG_DEFAULT_EN_CFG              (0x00000000uL)
#else
    #define CapSense_CONFIG_DEFAULT_EN_CFG              (0x80001000uL)
#endif /* (CapSense_DISABLE != CapSense_BLOCK_OFF_AFTER_SCAN_EN) */

#if(CapSense_IREF_SRSS != CapSense_CSD_IREF_SOURCE)
    #define CapSense_CONFIG_DEFAULT_IREF_CFG            (0x00000001uL)
#else
    #define CapSense_CONFIG_DEFAULT_IREF_CFG            (0x00000000uL)
#endif /* (CapSense_IREF_SRSS != CapSense_CSD_IREF_SOURCE) */

#if(CapSense_DISABLE != CapSense_LP_MODE_EN)
    #define CapSense_CONFIG_DEFAULT_LP_MODE_CFG         (0x40000000uL)
#else
    #define CapSense_CONFIG_DEFAULT_LP_MODE_CFG         (0x00000000uL)
#endif /* (CapSense_DISABLE != CapSense_LP_MODE_EN) */

#define CapSense_CONFIG_DEFAULT_FILTER_DELAY_CFG        (0x00000020uL)
#define CapSense_CONFIG_DEFAULT_MUTUAL_CFG              (0x00040000uL)
#define CapSense_CONFIG_DEFAULT_DUAL_CNT_CFG            (0x00080000uL)


#define CapSense_DEFAULT_CSD_CONFIG_CFG                 (CapSense_CONFIG_DEFAULT_IREF_CFG |\
                                                                 CapSense_CONFIG_DEFAULT_FILTER_DELAY_CFG |\
                                                                 CapSense_CONFIG_DEFAULT_MUTUAL_CFG |\
                                                                 CapSense_CONFIG_DEFAULT_DUAL_CNT_CFG |\
                                                                 CapSense_CONFIG_DEFAULT_LP_MODE_CFG |\
                                                                 CapSense_CONFIG_DEFAULT_EN_CFG)


/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 1      | SAMPLE        | 0x01(Clear CSD End Of Scan interrupt)                                                  |
 * | 2      | INIT          | 0x01(Clear CSD End Of Coarse Init interrupt)                                           |
 * | 8      | ADC_RES       | 0x01(Clear ADC End Of Scan interrupt)                                                  |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_INTR_CFG                   (CapSense_CSD_INTR_ALL_MSK)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 1      | SAMPLE        | 0x00(CSD End Of Scan interrupt is cleared)                                             |
 * | 2      | INIT          | 0x00(CSD End Of Coarse Init interrupt is cleared)                                      |
 * | 8      | ADC_RES       | 0x00(ADC End Of Scan interrupt is cleared)                                             |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_INTR_SET_CFG               (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 1      | SAMPLE        | 0x01(CSD End Of Scan interrupt is masked)                                              |
 * | 2      | INIT          | 0x00(CSD End Of Coarse Init interrupt is cleared)                                       |
 * | 8      | ADC_RES       | 0x00(ADC End Of Scan interrupt is cleared)                                              |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_INTR_MASK_CFG              (CapSense_CSD_INTR_MASK_SAMPLE_MSK)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * |  0     |  HSCMP_EN     | 0x00(High Speed comparator is disabled)                                                |
 * |  4     |  HSCMP_INVERT | 0x00(High Speed comparator output is not inverted)                                     |
 * |  31    |  AZ_EN        | 0x00(Auto-Zero is disabled)                                                            |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_HSCMP_CFG                  (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * |  1:0   | PWR_MODE      | 0x00(Amux buffer is disabled)                                                          |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_AMBUF_CFG                  (0x00000000uL)

/* +--------+---------------+------------------------------------------+---------------------------------------------+
 * |        |               |           VDDA >= 2V                     |          VDDA <2V                           |
 * +  BITS  +   FIELD       +------------------------------------------+---------------------------------------------+
 * |        |               |              MODE                        |             MODE                            |
 * |--------|---------------|------------------------------------------|---------------------------------------------|
 * |  0     | REFGEN_EN     | 0x01(Reference generator is enabled)     | 0x01(Reference generator is enabled)        |
 * |  4     | BYPASS        | 0x00(BYPASS mode switch is open)         | 0x00(BYPASS mode switch is open)            |
 * |  5     | VDDA_EN       | 0x00(VDDA mode switch is open)           | 0x00(VDDA mode switch is open)              |
 * |  6     | RES_EN        | 0x01(Resistor string is enabled)         | 0x01(Resistor string is enabled)            |
 * |  12:08 | GAIN          | 0x1F(Gain = 31)                          | 0x00(Gain = 21)                             |
 * |  20:16 | VREFLO_SEL    | 0x00(Open all VREFLO switches)           | 0x00(Open all VREFLO switches)              |
 * |  23    | VREFHI_SEL    | 0x00(Open VREF_LO_INT switch)            | 0x00(Open VREF_LO_INT switch)               |
 * +--------+---------------+------------------------------------------+---------------------------------------------+*/
#if(CapSense_VREF_SRSS != CapSense_CSD_VREF_SOURCE)
    #define CapSense_REFGEN_DEFAULT_GAIN_CFG            (0x00001F00uL)
#else
    #define CapSense_REFGEN_DEFAULT_GAIN_CFG            (0x00001500uL)
#endif /* (CapSense_VREF_SRSS != CapSense_CSD_VREF_SOURCE) */

#define  CapSense_REFGEN_DEFAULT_EN_CFG                 (0x00000001uL)
#define  CapSense_REFGEN_DEFAULT_RES_EN_CFG             (0x00000040uL)

#define CapSense_DEFAULT_CSD_REFGEN_CFG                 (CapSense_REFGEN_DEFAULT_EN_CFG |\
                                                                 CapSense_REFGEN_DEFAULT_GAIN_CFG |\
                                                                 CapSense_REFGEN_DEFAULT_RES_EN_CFG)


/* +--------+---------------+------------------------------------------+---------------------------------------------+
 * |        |               |        AUTO_ZERO_EN=TRUE                 |       AUTO_ZERO_EN=FALSE                    |
 * +  BITS  +   FIELD       +------------------------------------------+---------------------------------------------+
 * |        |               |              MODE                        |             MODE                            |
 * |--------|---------------|------------------------------------------|---------------------------------------------|
 * |  0     | CSDCMP_EN     | 0x00(CSD comparator is disabled)         | 0x00(CSD comparator is disabled)            |
 * |  5:04  | POLARITY_SEL  | 0x00(IDAC_A polarity is used)            | 0x00(IDAC_A polarity is used)               |
 * |  9:08  | CMP_PHASE     | 0x03(Comparator activated on both phases)| 0x03(Comparator activated in both phases)   |
 * |  28    | CMP_MODE      | 0x00(CSD mode)                           | 0x00(CSD mode)                              |
 * |  29    | FEEDBACK_MODE | 0x00(Feedback from sampling flip-flop)   | 0x00(Feedback from sampling flip-flop)      |
 * |  31    | AZ_EN         | 0x01(Auto-Zero functionality is enabled) | 0x00(Auto-Zero functionality is disabled)   |
 * +--------+---------------+------------------------------------------+---------------------------------------------+*/
#if(0u != CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_DEFAULT_CSD_CSDCMP_CFG             (0x80000300uL)
#else
    #define CapSense_DEFAULT_CSD_CSDCMP_CFG             (0x00000300uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN) */

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 6:0    | VAL           | 0x00(Set IDAC value to "0")                                                            |
 * | 7      | POL_DYN       | 0x01(Set dynamic IDAC polarity)                                                        |
 * | 9:8    | POLARITY      | 0x02(IDAC polarity follows CSD_SENSE signal)                                           |
 * | 11:10  | BAL_MODE      | 0x03(IDAC is enabled in both phases and disabled by CSDCOMP at the end of balancing)   |
 * | 17:16  | LEG1_MODE     | 0x03(Configure LEG1 to CSD dynamic mode)                                               |
 * | 19:18  | LEG2_MODE     | 0x00(LEG2 is disabled)                                                                 |
 * | 21     | DSI_CTRL_EN   | 0x00(IDAC DSI control is disabled)                                                     |
 * | 23:22  | RANGE         | Set range according to Csx0IdacGainV2 parameter value                                  |
 * | 24     | LEG1_EN       | 0x01(Output for LEG1 is enabled)                                                       |
 * | 25     | LEG2_EN       | 0x00(Output for LEG2 is disabled)                                                      |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_IDACA_CFG                  ((0x01030E80uL) | \
                                                                 ((CapSense_CSX_IDAC_GAIN << \
                                                                 CSD_IDACA_RANGE_Pos) & \
                                                                 CapSense_CSD_IDACA_RANGE_MSK))

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 6:0    | VAL           | 0x00(Set IDAC value to "0")                                                            |
 * | 7      | POL_DYN       | 0x00(Set to "0" since IDACB is not used)                                               |
 * | 9:8    | POLARITY      | 0x00(Set to "0" since IDACB is not used)                                               |
 * | 11:10  | BAL_MODE      | 0x00(Set to "0" since IDACB is not used)                                               |
 * | 17:16  | LEG1_MODE     | 0x00(Set to "0" since IDACB is not used)                                               |
 * | 19:18  | LEG2_MODE     | 0x00(Set to "0" since IDACB is not used)                                               |
 * | 21     | DSI_CTRL_EN   | 0x00(Set to "0" since IDACB is not used)                                               |
 * | 23:22  | RANGE         | 0x00(Set to "0" since IDACB is not used)                                               |
 * | 24     | LEG1_EN       | 0x00(Output for LEG2 is disabled)                                                      |
 * | 25     | LEG2_EN       | 0x00(Output for LEG2 is disabled)                                                      |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_IDACB_CFG                  (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * +--------+---------------+----------------------------------------------------------------------------------------+
 * | 1:0    | RES_HCAV      | 0x00(Configure HCAV according to CsxInitShieldSwitchRes parameter)                     |
 * | 3:2    | RES_HCAG      | 0x00(Configure HCAG according to CsxInitShieldSwitchRes parameter)                     |
 * | 5:4    | RES_HCBV      | 0x00(Configure HCBV according to CsxInitShieldSwitchRes parameter)                     |
 * | 7:6    | RES_HCBG      | 0x00(Configure HCBG according to CsxInitShieldSwitchRes parameter)                     |
 * | 17:16  | RES_F1PM      | 0x00(Configure F1PM according to CsxInitSwitchRes parameter)                           |
 * | 19:18  | RES_F2PT      | 0x00(Configure 2PT according to CsxInitSwitchRes parameter)                            |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_RES_CFG                 ((CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCAV_Pos) |\
                                                                 (CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCAG_Pos) |\
                                                                 (CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCBV_Pos) |\
                                                                 (CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCBG_Pos) |\
                                                                 (CapSense_CSX_INIT_SWITCH_RES << CSD_SW_RES_RES_F1PM_Pos) |\
                                                                 (CapSense_CSX_INIT_SWITCH_RES << CSD_SW_RES_RES_F2PT_Pos))

/* +--------+---------------+--------------------------------------------+-------------------------------------------+
 * |        |               |      TX_CLK_SOURCE=PRSAUTO                 |     TX_CLK_SOURCE!=PRSAUTO                |
 * +  BITS  +   FIELD       +--------------------------------------------+-------------------------------------------+
 * |        |               |              MODE                          |             MODE                          |
 * |--------|---------------|--------------------------------------------|-------------------------------------------|
 * |  11:00 | SENSE_DIV     | Set to CSX_TX_CLK_DIVIDER value            | Set to CSX_TX_CLK_DIVIDER value           |
 * |  18:16 | LFSR_SIZE     | 0x00(Will be reconfigured in Run Time)     | 0x00(Set to "0" since not used)           |
 * |  23:20 | LFSR_SCALE    | 0x00(Will be reconfigured in Run Time)     | Set to CSX_TX_CLK_SOURCE value            |
 * |  24    | LFSR_CLEAR    | 0x00(Will be reconfigured in Run Time)     | 0x00(Will be reconfigured in Run Time)    |
 * |  25    | SEL_LFSR_MSB  | 0x00(Not used for CSX scanning method)     | 0x00(Not used for CSX scanning method)    |
 * |  27:26 | LFSR_BITS     | 0x0C(Use 5 bits. Range=[-16:15])           | 0x0C(Use 5 bits. Range=[-16:15])          |
 * +--------+---------------+-----------------------------+----------------------------------------------------------+*/
#if(CapSense_CSX_TX_CLK_SOURCE != CapSense_CLK_SOURCE_PRSAUTO)
    #define CapSense_DEFAULT_CSD_SENSE_PERIOD_CFG       (CapSense_CSD_SENSE_PERIOD_LFSR_BITS_MSK | \
                                                                 (uint32)((uint32)CapSense_CSX_TX_CLK_SOURCE << \
                                                                 CapSense_CSD_SENSE_PERIOD_LFSR_SIZE_POS) | \
                                                                 ((uint32)CapSense_CSX_TX_CLK_DIVIDER & \
                                                                 (uint32)CapSense_CSD_SENSE_PERIOD_SENSE_DIV_MSK))
#else
    #define CapSense_DEFAULT_CSD_SENSE_PERIOD_CFG       (CapSense_CSD_SENSE_PERIOD_LFSR_BITS_MSK | \
                                                                 (CapSense_CSX_TX_CLK_DIVIDER & \
                                                                 CapSense_CSD_SENSE_PERIOD_SENSE_DIV_MSK))
#endif /* (CapSense_CSX_TX_CLK_SOURCE != CapSense_CLK_SOURCE_PRSAUTO) */

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 11:0   | SENSE_WIDTH   | 0x0000(Set to "0" SenseClk cycles)                                                     |
 * | 16     | SENSE_POL     | 0x00(Start with low phase)                                                             |
 * | 18     | OVERLAP_PHI1  | 0x00(Non overlap for Phi1)                                                             |
 * | 19     | OVERLAP_PHI2  | 0x00(Non overlap for Phi2)                                                             |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SENSE_DUTY_CFG             (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 0      | SW_HMPM       | 0x00(HMPM switch is statically open)                                                   |
 * | 4      | SW_HMPT       | 0x00(HMPT switch is statically open)                                                   |
 * | 8      | SW_HMPS       | 0x00(HMPS switch is statically open)                                                   |
 * | 12     | SW_HMMA       | 0x00(HMMA switch is statically open)                                                   |
 * | 16     | SW_HMMB       | 0x00(HMMB switch is statically open)                                                   |
 * | 20     | SW_HMCA       | 0x00(HMCA switch is statically open)                                                   |
 * | 24     | SW_HMCB       | 0x00(HMCB switch is statically open)                                                   |
 * | 28     | SW_HMRH       | 0x00(HMRH switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_HS_P_SEL_CFG            (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 16     | SW_HCCC       | 0x00(HCCC switch is statically open)                                                   |
 * | 20     | SW_HCCD       | 0x00(HCCD switch is statically open)                                                   |
 * | 26:24  | SW_HCRH       | 0x00(HCRH switch is statically open)                                                   |
 * | 30:28  | SW_HCRL       | 0x00(HCRL switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_HS_N_SEL_CFG            (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 2:0    | SW_HCAV       | 0x00(HCAV switch is statically open)                                                   |
 * | 6:4    | SW_HCAG       | 0x00(HCAG switch is statically open)                                                   |
 * | 10:8   | SW_HCBV       | 0x00(HCBV switch is statically open)                                                   |
 * | 14:12  | SW_HCBG       | 0x00(HCBG switch is statically open)                                                   |
 * | 16     | SW_HCCV       | 0x00(HCCV switch is statically open)                                                   |
 * | 20     | SW_HCCG       | 0x00(HCCG switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_SHIELD_SEL_CFG          (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * |  4     | SW_IRBY       | 0x00(IRBY switch is statically open)                                                   |
 * |  8     | SW_IRLB       | 0x00(IRLB switch is statically open)                                                   |
 * |  12    | SW_ICA        | 0x00(ICA  switch is statically open)                                                   |
 * |  18:16 | SW_ICB        | 0x00(ICB  switch is statically open)                                                   |
 * |  20    | SW_IRLI       | 0x00(IRLI switch is statically open)                                                   |
 * |  24    | SW_IRH        | 0x00(IRH  switch is statically open)                                                   |
 * |  28    | SW_IRL        | 0x00(IRL  switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_AMUXBUF_SEL_CFG         (0x00000000uL)


/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 12     | SW_BYA        | 0x01(BYA switch is statically open)                                                    |
 * | 16     | SW_BYB        | 0x00(BYB switch is statically open)                                                    |
 * | 20     | SW_CBCC       | 0x00(CBCC switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_BYP_SEL_CFG             (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 2:0    | SW_SFPM       | 0x03(SFPM switch is controlled by Phi2 of SenseClk signal)                             |
 * | 6:4    | SW_SFPT       | 0x02(SFPM switch is controlled by Phi1 of the SenseClk signal)                         |
 * | 10:8   | SW_SFPS       | 0x00(SFPS switch is statically open)                                                   |
 * | 12     | SW_SFMA       | 0x00(SFMA switch is statically open)                                                   |
 * | 16     | SW_SFMB       | 0x00(SFMB switch is statically open)                                                   |
 * | 20     | SW_SFCA       | 0x00(SFCA switch is statically open)                                                   |
 * | 24     | SW_SFCB       | 0x00(SFCB switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_CMP_P_SEL_CFG           (0x00000023uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * |  26:24 | SW_SCRH       |  0x01(SFPS switch is statically closed)                                                |
 * |  30:28 | SW_SCRL       |  0x00(SFPS switch is statically open)                                                  |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_CMP_N_SEL_CFG           (0x01000000uL)

/* +--------+---------------+--------------------------------------------+-------------------------------------------+
 * |        |               |             VREF_SRC=SRSS                  |              VREF_SRC=PASS                |
 * +  BITS  +   FIELD       +--------------------------------------------+-------------------------------------------+
 * |        |               |              MODE                          |              MODE                         |
 * |--------|---------------|--------------------------------------------|-------------------------------------------|
 * |  0     | SW_IAIB       | 0x00(IAIB switch is statically open)       | 0x00(IAIB switch is statically open)      |
 * |  4     | SW_IBCB       | 0x00(IBCB switch is statically open)       | 0x00(IBCB switch is statically open)      |
 * |  16    | SW_SGMB       | 0x00(SGMB switch is statically open)       | 0x00(SGMB switch is statically open)      |
 * |  20    | SW_SGRP       | 0x00(SGRP switch is statically open)       | 0x01(SGRP switch is statically closed)    |
 * |  24    | SW_SGRE       | 0x00(SGRE switch is statically open)       | 0x00(SGRE switch is statically open)      |
 * |  28    | SW_SGR        | 0x01(SGR switch is statically closed)      | 0x00(SGR switch is statically open)       |
 * +--------+---------------+--------------------------------------------+-------------------------------------------+*/
#if(CapSense_VREF_SRSS != CapSense_CSD_VREF_SOURCE)
    #define CapSense_DEFAULT_CSD_SW_REFGEN_SEL_CFG      (0x00100000uL)
#else
    #define CapSense_DEFAULT_CSD_SW_REFGEN_SEL_CFG      (0x10000000uL)
#endif /* (CapSense_VREF_SRSS != CapSense_CSD_VREF_SOURCE) */

#define CapSense_DEFAULT_CSD_SW_REFGEN_SEL_MSK          (0x10100000uL)
/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 0      | SW_F1PM       | 0x00(F1PM switch is statically closed)                                                 |
 * | 10:8   | SW_F1MA       | 0x00(F1MA switch is controlled by Phi2 of SenseClk signal)                             |
 * | 18:16  | SW_F1CA       | 0x00(F1CA switch is controlled by Phi2 of SenseClk signal)                             |
 * | 20     | SW_C1CC       | 0x00(C1CC switch is statically open)                                                   |
 * | 24     | SW_C1CD       | 0x00(C1CD switch is statically open)                                                   |
 * | 28     | SW_C1F1       | 0x00(C1F1 switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_FW_MOD_SEL_CFG          (0x00030301uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 4      | SW_F2PT       | 0x00(F2PT switch is statically closed)                                                 |
 * | 10:8   | SW_F2MA       | 0x00(F2MA switch is controlled by Phi1 of the SenseClk signal)                         |
 * | 14:12  | SW_F2CA       | 0x00(F2CA switch is controlled by Phi1 of the SenseClk signal)                         |
 * | 18:16  | SW_F2CB       | 0x00(F2CB switch is statically open)                                                   |
 * | 20     | SW_C2CC       | 0x00(C2CC switch is statically open)                                                   |
 * | 24     | SW_C2CD       | 0x00(C2CD switch is statically open)                                                   |
 * | 28     | SW_C2F2       | 0x00(C2F2 switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_FW_TANK_SEL_CFG         (0x00002210uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 3:0    | TX_OUT        | 0x00(The TX_OUT signal is controlled by SenseClk signal)                               |
 * | 7:4    | TX_OUT_EN     | 0x00(The TX_OUT_EN signal is set to the static "1")                                    |
 * | 15:12  | TX_AMUXB_EN   | 0x00(The TX_AMUXB_EN signal is set to the static "0")                                  |
 * | 19:16  | TX_N_OUT      | 0x00(The TX_N_OUT signal is set to the static "0")                                     |
 * | 23:20  | TX_N_OUT_EN   | 0x00(The TX_N_OUT_EN signal is set to the static "0")                                  |
 * | 27:24  | TX_N_AMUXA_EN | 0x00(The TX_N_AMUXA_EN signal is set to the static "0")                                |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_IO_SEL_CFG              (0x0000001FuL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 2:0    | DSI_CSH_TANK  | 0x00(DSI_CSH_TANK switch is statically open)                                           |
 * | 6:4    | DSI_CMOD      | 0x00(DSI_CMOD switch is statically open)                                               |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_DSI_SEL_CFG             (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 7:0    | AZ_TIME       | Set to AUTO_ZERO_TIME value                                                            |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SEQ_TIME_CFG               (CapSense_CSX_AUTO_ZERO_TIME & \
                                                                 CapSense_CSD_SEQ_INIT_CNT_CONV_CNT_MSK)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 15:0   | CONV_CNT      | 0x0014(Set coarse initialization time to 4 SenseClk periods)                           |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SEQ_INIT_CNT_CFG           (CapSense_CSX_FINE_INIT_TIME)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 15:0   | CONV_CNT      | 0x0000(Sets the number of conversions per sample to "0")                               |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SEQ_NORM_CNT_CFG           (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 7:0    | ADC_TIME      | 0x00(Set to "0" since ADC is not used)                                                 |
 * | 17:16  | ADC_MODE      | 0b00(Set to "0" since ADC is not used)                                                 |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_DEFAULT_CSD_ADC_CTL_CFG                (0x00000000uL)

/* +--------+---------------+--------------------------------------------+-------------------------------------------+
 * |        |               |      AUTO_ZERO_EN=TRUE                     |     AUTO_ZERO_EN=TRUE                     |
 * +  BITS  +   FIELD       +--------------------------------------------+-------------------------------------------+
 * |        |               |              MODE                          |             MODE                          |
 * |--------|---------------|--------------------------------------------|-------------------------------------------|
 * |  0     | START         | 0x00(CSD sequencer is stopped)             | 0x00(CSD sequencer is stopped)            |
 * |  1     | SEQ_MODE      | 0x00(Regular scan mode is used)            | 0x00(Regular scan mode is used)           |
 * |  3     | ABORT         | 0x01(Reset CSD sequencer to IDLE state)    | 0x01(Reset CSD sequencer to IDLE state)   |
 * |  4     | DSI_START_EN  | 0x00(Disable control by DSI)               | 0x00(Disable control by DSI)              |
 * |  8     | AZ0_SKIP      | 0x00(Do not skip AZ0 state)                | 0x01(Skip AZ0 state)                      |
 * |  9     | AZ1_SKIP      | 0x00(Do not skip AZ1 state)                | 0x01(Skip AZ1 state)                      |
 * +--------+---------------+--------------------------------------------+-------------------------------------------+*/
#if(CapSense_ENABLE == CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_DEFAULT_CSD_SEQ_START_CFG          (0x00000008uL)
#else
    #define CapSense_DEFAULT_CSD_SEQ_START_CFG          (0x00000308uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN) */

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * |  0     | IREF_SEL      | 0x00(Select SRSS Iref)                                                                 |
 * |  8:4   | FILTER_DELAY  | 0x00(Will be reconfigured in Run Time)                                                 |
 * |  11:10 | SHIELD_DELAY  | 0x00(Delay Line is OFF)                                                                |
 * |  12    | SENSE_EN      | 0x01(Enabled. Switches can be closed)                                                  |
 * |  17    | FULL_WAVE     | 0x00(Half Wave Mode)                                                                   |
 * |  18    | MUTUAL_CAP    | 0x01(Mutual-cap mode)                                                                  |
 * |  19    | CSX_DUAL_CNT  | 0x01(Use two counters)                                                                 |
 * |  24    | DSI_COUNT_SEL | 0x00(CSD-Result on DSI_COUNT bus)                                                      |
 * |  25    | DSI_SAMPLE_EN | 0x00(DSI_SAMPLE_IN is disabled)                                                        |
 * |  26    | SAMPLE_SYNC   | 0x00(Double sync is disabled)                                                          |
 * |  27    | DSI_SENSE_EN  | 0x00(DSI_SENSE_IN is disabled)                                                         |
 * |  30    | LP_MODE       | 0x00(High power mode)                                                                  |
 * |  31    | ENABLE        | 0x01(Analog part is ON)                                                                |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/

#if(CapSense_IREF_SRSS != CapSense_CSD_IREF_SOURCE)
    #define CapSense_CONFIG_PRESCAN_IREF_CFG            (0x00000001uL)
#else
    #define CapSense_CONFIG_PRESCAN_IREF_CFG            (0x00000000uL)
#endif /* (CapSense_IREF_SRSS != CapSense_CSD_IREF_SOURCE) */

#if(CapSense_DISABLE != CapSense_LP_MODE_EN)
    #define CapSense_CONFIG_PRESCAN_LP_MODE_CFG         (0x40000000uL)
#else
    #define CapSense_CONFIG_PRESCAN_LP_MODE_CFG         (0x00000000uL)
#endif /* (CapSense_DISABLE != CapSense_LP_MODE_EN) */

#define CapSense_CONFIG_PRESCAN_EN_CFG                  (0x80000000uL)
#define CapSense_CONFIG_PRESCAN_SENSE_EN_CFG            (0x00001000uL)
#define CapSense_CONFIG_PRESCAN_FILTER_DELAY_CFG        (0x00000000uL)
#define CapSense_CONFIG_PRESCAN_MUTUAL_CFG              (0x00040000uL)
#define CapSense_CONFIG_PRESCAN_DUAL_CNT_CFG            (0x00080000uL)


#define CapSense_PRESCAN_CSD_CONFIG_CFG                 (CapSense_CONFIG_PRESCAN_IREF_CFG |\
                                                                 CapSense_CONFIG_PRESCAN_FILTER_DELAY_CFG |\
                                                                 CapSense_CONFIG_PRESCAN_SENSE_EN_CFG |\
                                                                 CapSense_CONFIG_PRESCAN_MUTUAL_CFG |\
                                                                 CapSense_CONFIG_PRESCAN_DUAL_CNT_CFG |\
                                                                 CapSense_CONFIG_PRESCAN_LP_MODE_CFG |\
                                                                 CapSense_CONFIG_PRESCAN_EN_CFG)

/* +--------+---------------+--------------------------------------------+-------------------------------------------+
 * |        |               |    AUTO_ZERO_EN=TRUE                       |    AUTO_ZERO_EN=FALSE                     |
 * +  BITS  +   FIELD       +--------------------------------------------+-------------------------------------------+
 * |        |               |              MODE                          |             MODE                          |
 * |--------|---------------|--------------------------------------------|-------------------------------------------|
 * |  0     | CSDCMP_EN     | 0x01(CSD comparator is Enabled)            | 0x01(CSD comparator is Enabled)           |
 * |  5:4   | POLARITY_SEL  | 0x00(IDAC_A polarity is used)              | 0x00(IDAC_A polarity is used)             |
 * |  9:8   | CMP_PHASE     | 0x03(Comparator activated in both phases)  | 0x03(Comparator activated in both phases) |
 * |  28    | CMP_MODE      | 0x00(CSD mode)                             | 0x00(CSD mode)                            |
 * |  29    | FEEDBACK_MODE | 0x00(Feedback from sampling flip-flop)     | 0x00(Feedback from sampling flip-flop)    |
 * |  31    | AZ_EN         | 0x01(Auto-Zero functionality is enabled)   | 0x00(Auto-Zero functionality is disabled) |
 * +--------+---------------+--------------------------------------------+-------------------------------------------+*/
#if(CapSense_ENABLE == CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_PRESCAN_CSD_CSDCMP_CFG             (0x80000301uL)
#else
    #define CapSense_PRESCAN_CSD_CSDCMP_CFG             (0x00000301uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN) */

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * +--------+---------------+----------------------------------------------------------------------------------------+
 * | 1:0    | RES_HCAV      | 0x00(Configure HCAV according to CsxInitShieldSwitchRes parameter)                     |
 * | 3:2    | RES_HCAG      | 0x00(Configure HCAG according to CsxInitShieldSwitchRes parameter)                     |
 * | 5:4    | RES_HCBV      | 0x00(Configure HCBV according to CsxInitShieldSwitchRes parameter)                     |
 * | 7:6    | RES_HCBG      | 0x00(Configure HCBG according to CsxInitShieldSwitchRes parameter)                     |
 * | 17:16  | RES_F1PM      | 0x00(Configure F1PM according to CsxInitSwitchRes parameter)                           |
 * | 19:18  | RES_F2PT      | 0x00(Configure 2PT according to CsxInitSwitchRes parameter)                            |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_RES_CFG               ((CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCAV_Pos) |\
                                                                 (CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCAG_Pos) |\
                                                                 (CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCBV_Pos) |\
                                                                 (CapSense_CSX_INIT_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCBG_Pos) |\
                                                                 (CapSense_CSX_INIT_SWITCH_RES << CSD_SW_RES_RES_F1PM_Pos) |\
                                                                 (CapSense_CSX_INIT_SWITCH_RES << CSD_SW_RES_RES_F2PT_Pos))

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * |  0     |  HSCMP_EN     | 0x01(High Speed comparator is Enabled)                                                 |
 * |  4     |  HSCMP_INVERT | 0x00(High Speed comparator output is not inverted)                                     |
 * |  31    |  AZ_EN        | 0x00(Auto-Zero is Disabled)                                                            |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_HSCMP_CFG                (0x00000001uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 0      | SW_HMPM       | 0x01(HMPM switch is statically closed)                                                 |
 * | 4      | SW_HMPT       | 0x01(HMPT switch is statically closed)                                                 |
 * | 8      | SW_HMPS       | 0x00(HMPS switch is statically open)                                                   |
 * | 12     | SW_HMMA       | 0x00(HMMA switch is statically open)                                                   |
 * | 16     | SW_HMMB       | 0x00(HMMB switch is statically open)                                                   |
 * | 20     | SW_HMCA       | 0x00(HMCA switch is statically open)                                                   |
 * | 24     | SW_HMCB       | 0x00(HMCB switch is statically open)                                                   |
 * | 28     | SW_HMRH       | 0x00(HMRH switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_HS_P_SEL_CFG          (0x00000011uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 16     | SW_HCCC       | 0x00(HCCC switch is statically open)                                                   |
 * | 20     | SW_HCCD       | 0x00(HCCD switch is statically open)                                                   |
 * | 26:24  | SW_HCRH       | 0x01(HCRH switch is statically closed)                                                 |
 * | 30:28  | SW_HCRL       | 0x00(HCRL switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_HS_N_SEL_CFG      (0x01000000uL)


/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 2:0    | DSI_CSH_TANK  | 0x00(DSI_CSH_TANK switch is statically open)                                           |
 * | 6:4    | DSI_CMOD      | 0x00(DSI_CMOD switch is statically open)                                               |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_DSI_SEL_CFG           (0x00000000uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 2:0    | SW_HCAV       | 0x06(HCAV switch is controlled by HSCMP output)                                        |
 * | 6:4    | SW_HCAG       | 0x00(HCAG switch is statically open)                                                   |
 * | 10:8   | SW_HCBV       | 0x00(HCBV switch is statically open)                                                   |
 * | 14:12  | SW_HCBG       | 0x00(HCBG switch is statically open)                                                   |
 * | 16     | SW_HCCV       | 0x00(HCCV switch is statically open)                                                   |
 * | 20     | SW_HCCG       | 0x00(HCCG switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_SHIELD_SEL_CFG        (0x00000006uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 0      | SW_F1PM       | 0x00(F1PM switch is statically closed)                                                 |
 * | 10:8   | SW_F1MA       | 0x00(F1MA switch is statically closed)                                                 |
 * | 18:16  | SW_F1CA       | 0x00(F1CA switch is statically closed)                                                 |
 * | 20     | SW_C1CC       | 0x00(C1CC switch is statically open)                                                   |
 * | 24     | SW_C1CD       | 0x00(C1CD switch is statically open)                                                   |
 * | 28     | SW_C1F1       | 0x00(C1F1 switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_FW_MOD_SEL_CFG        (0x00010101uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 4      | SW_F2PT       | 0x00(F2PT switch is statically closed)                                                 |
 * | 10:8   | SW_F2MA       | 0x00(F2MA switch is statically closed)                                                 |
 * | 14:12  | SW_F2CA       | 0x00(F2CA switch is statically olosed)                                                 |
 * | 18:16  | SW_F2CB       | 0x00(F2CB switch is statically open)                                                   |
 * | 20     | SW_C2CC       | 0x00(C2CC switch is statically open)                                                   |
 * | 24     | SW_C2CD       | 0x00(C2CD switch is statically open)                                                   |
 * | 28     | SW_C2F2       | 0x00(C2F2 switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_FW_TANK_SEL_CFG       (0x00001110uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 0      | START         | 0x01(STARTED)                                                                          |
 * | 1      | SEQ_MODE      | 0x01(INIT_COARSE)                                                                      |
 * | 3      | ABORT         | 0x00(RESET_TO_IDLE_STATE)                                                              |
 * | 4      | DSI_START_EN  | 0x00(DISABLE_CONTROL_BY_DSI)                                                           |
 * | 8      | AZ0_SKIP      | 0x01(SKIP_AZ0)                                                                         |
 * | 9      | AZ1_SKIP      | 0x01(SKIP_AZ1)                                                                         |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SEQ_START_CFG            (0x00000303uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * +--------+---------------+----------------------------------------------------------------------------------------+
 * | 1:0    | RES_HCAV      | 0x00(Configure HCAV according to CsxScanShieldSwitchRes parameter)                     |
 * | 3:2    | RES_HCAG      | 0x00(Configure HCAG according to CsxScanShieldSwitchRes parameter)                     |
 * | 5:4    | RES_HCBV      | 0x00(Configure HCBV according to CsxScanShieldSwitchRes parameter)                     |
 * | 7:6    | RES_HCBG      | 0x00(Configure HCBG according to CsxScanShieldSwitchRes parameter)                     |
 * | 17:16  | RES_F1PM      | 0x00(Configure F1PM according to CsxScanSwitchRes parameter)                           |
 * | 19:18  | RES_F2PT      | 0x00(Configure 2PT according to CsxScanSwitchRes parameter)                            |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_SCAN_CSD_SW_RES_CFG                    ((CapSense_CSX_SCAN_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCAV_Pos) |\
                                                                 (CapSense_CSX_SCAN_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCAG_Pos) |\
                                                                 (CapSense_CSX_SCAN_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCBV_Pos) |\
                                                                 (CapSense_CSX_SCAN_SHIELD_SWITCH_RES << CSD_SW_RES_RES_HCBG_Pos) |\
                                                                 (CapSense_CSX_SCAN_SWITCH_RES << CSD_SW_RES_RES_F1PM_Pos) |\
                                                                 (CapSense_CSX_SCAN_SWITCH_RES << CSD_SW_RES_RES_F2PT_Pos))

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 0      | SW_F1PM       | 0x01(F1PM switch is statically closed)                                                 |
 * | 10:8   | SW_F1MA       | 0x03(F1MA switch is controlled by Phi2 of the SenseClk signal)                         |
 * | 18:16  | SW_F1CA       | 0x03(F1CA switch is controlled by Phi2 of the SenseClk signal)                         |
 * | 20     | SW_C1CC       | 0x00(C1CC switch is statically open)                                                   |
 * | 24     | SW_C1CD       | 0x00(C1CD switch is statically open)                                                   |
 * | 28     | SW_C1F1       | 0x00(C1F1 switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_SCAN_CSD_SW_FW_MOD_SEL_CFG             (0x00030301uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 4      | SW_F2PT       | 0x01(F2PT switch is statically closed)                                                 |
 * | 10:8   | SW_F2MA       | 0x02(F2MA switch is controlled by Phi1 of the SenseClk signal)                         |
 * | 14:12  | SW_F2CA       | 0x02(F2CA switch is controlled by Phi1 of the SenseClk signal)                         |
 * | 18:16  | SW_F2CB       | 0x00(F2CB switch is statically open)                                                   |
 * | 20     | SW_C2CC       | 0x00(C2CC switch is statically open)                                                   |
 * | 24     | SW_C2CD       | 0x00(C2CD switch is statically open)                                                   |
 * | 28     | SW_C2F2       | 0x00(C2F2 switch is statically open)                                                   |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_SCAN_CSD_SW_FW_TANK_SEL_CFG            (0x00002210uL)

/* +--------+---------------+----------------------------------------------------------------------------------------+
 * |  BITS  |   FIELD       |              MODE                                                                      |
 * |--------|---------------|----------------------------------------------------------------------------------------|
 * | 3:0    | TX_OUT        | 0x00(The TX_OUT signal is controlled by SenseClk signal)                               |
 * | 7:4    | TX_OUT_EN     | 0x00(The TX_OUT_EN signal is set to the static "1")                                    |
 * | 15:12  | TX_AMUXB_EN   | 0x00(The TX_AMUXB_EN signal is set to the static "0")                                  |
 * | 19:16  | TX_N_OUT      | 0x00(The TX_N_OUT signal is set to the static "0")                                     |
 * | 23:20  | TX_N_OUT_EN   | 0x00(The TX_N_OUT_EN signal is set to the static "0")                                  |
 * | 27:24  | TX_N_AMUXA_EN | 0x00(The TX_N_AMUXA_EN signal is set to the static "0")                                |
 * +--------+---------------+----------------------------------------------------------------------------------------+*/
#define CapSense_SCAN_CSD_SW_IO_SEL_CFG                 (0x0000001FuL)

/* +--------+---------------+--------------------------------------------+-------------------------------------------+
 * |        |               |      AUTO_ZERO_EN=TRUE                     |     AUTO_ZERO_EN=TRUE                     |
 * +  BITS  +   FIELD       +--------------------------------------------+-------------------------------------------+
 * |        |               |              MODE                          |             MODE                          |
 * |--------|---------------|--------------------------------------------|-------------------------------------------|
 * |  0     | START         | 0x01(CSD sequencer is stopped)             | 0x01(CSD sequencer is stopped)            |
 * |  1     | SEQ_MODE      | 0x00(Regular CSD scan mode is used)        | 0x00(Regular CSD scan mode is used)       |
 * |  3     | ABORT         | 0x00(Do not reset CSD sequencer)           | 0x00(Do not reset CSD sequencer)          |
 * |  4     | DSI_START_EN  | 0x00(Disable control by DSI)               | 0x00(Disable control by DSI)              |
 * |  8     | AZ0_SKIP      | 0x00(Do not skip AZ0 state)                | 0x01(Skip AZ0 state)                      |
 * |  9     | AZ1_SKIP      | 0x00(Do not skip AZ1 state)                | 0x01(Skip AZ1 state)                      |
 * +--------+---------------+--------------------------------------------+-------------------------------------------+*/
#if(0u != CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_SCAN_CSD_SEQ_START_CFG             (0x00000001uL)
#else
    #define CapSense_SCAN_CSD_SEQ_START_CFG             (0x00000301uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN) */

#endif  /* End of CapSense_CSX_EN */
#endif  /* End of CapSense_SENSING_CSD_LL_H */


/* [] END OF FILE */
