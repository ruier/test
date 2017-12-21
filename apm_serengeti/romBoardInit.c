/* romBoardInit.c - APM Serengeti board initialization routines */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,14dec11,x_s  correct wrong condition judgement.
01d,06aug11,syt  updated romSdramInit() function with new configuration
                 parameters, corrected PLB5 configuration register
                 initializtion value.
01c,30jul11,x_z  fix compiling warning.
01b,18jul11,b_m  consolidate soc blocks enable routine.
01a,23jun11,syt  initial creation.
*/

/*
DESCRIPTION
This file contains functions to initialize the NOR Flash interface, on-chip
DDR SDRAM controller and clock controll module on the APM Serengeti board.

This is relocatable code. The initial bootrom code is typically loaded
at a different address than it was linked.  The only memory facility
available to these routines is the stack, initialized to the APM86x90's
on-chip SRAM.  For these reasons, all code in this module must be
position-independent, make sparing use only of auto (stack) variables,
and must avoid use of data and BSS memory and static variables.
Finally, compiler output must be inspected to make sure that no jump
tables with absolute branches have been generated; switch statements and
if-else if-... statements are the leading culprits.

INCLUDE FILES:

*/

/* includes */

#include <vxWorks.h>
#include "serengeti.h"
#include <arch/ppc/vxPpcLib.h>
#include <arch/ppc/dcr4xxALib.h>

/* externs */

IMPORT UINT32 romRead32 (UINT32 address);
IMPORT void   romWrite32 (UINT32 address, UINT32 value);
IMPORT UINT32 romDcrInLong (UINT32 dcr);
IMPORT void   romDcrOutLong (UINT32 dcr, UINT32 value);

/*******************************************************************************
*
* romBootDelay - delay for some amount of time at boot stage
*
* This routine delays for some amount of time at boot stage.
*
* RETURNS: N/A
*/

LOCAL void romBootDelay
    (
    volatile UINT32 delayNum
    )
    {
    volatile UINT32 delay;
	for (delay = 0; delay < (delayNum * 100); delay++);
    }

/*******************************************************************************
*
* romApbClockInit - Initialize AHB-APB Bridge clock
*
* This routine fills will initialize AHB-APB Bridge clock
*
* RETURNS: N/A
*
* \NOMANUAL
*/

LOCAL void romApbClockInit (void)
    {
    UINT32 regValue;
    UINT32 clkFreqDiv;
    UINT32 pllOutClkFreq;

    /* Setup AHB ENABLE_INTF APB clock freq ratio*/

    regValue = romRead32 (ENABLE_INTF) & AHB_APB_CLK_RATIO_MASK;
    romWrite32 (ENABLE_INTF, regValue | AHB_APB_CLK_RATIO);

    /* Scale APB clk divisor down from 100MHz */

    do
        {
        regValue = romRead32 (SCU_SOCPLL1);

        if (romRead32 (SCU_SOCPLL1) & BYPASS_MASK)
            pllOutClkFreq = SYS_REFCLK;
        else
            pllOutClkFreq = (SYS_REFCLK * PLL_NF_RATIO(regValue)) /  \
                            (PLL_NR_RATIO(regValue) * PLL_OD_RATIO(regValue));

        if (pllOutClkFreq >= FREQ_100_MHZ)
            {
            clkFreqDiv = APB_DIV_RATIO(romRead32 (SCU_SOCDIV));
            clkFreqDiv++;

            if (clkFreqDiv >= 0x1f)
                break;
            if (clkFreqDiv & 1)
                clkFreqDiv ++;

            regValue = romRead32 (SCU_SOCDIV);
            regValue &= 0xFFE0E0FF;
            regValue |= ((clkFreqDiv / 2) << 16) | (clkFreqDiv << 8);
            romWrite32 (SCU_SOCDIV, regValue);
            }
        else
            break;
        }
    while (regValue >= 100000000);

    return;
    }

/*******************************************************************************
*
* romDdrCtlrEnable - Enable APM86x90 DDR controller
*
* This routine enables the APM86x90 DDR controller before DDR SDRAM.
*
* RETURNS: N/A
*
* \NOMANUAL
*/

LOCAL void romDdrCtlrEnable (void)
    {
    UINT32 value;
    UINT32 count = 0;

    value = romRead32 (SCU_SRST);

    /*
     * Note:
     *
     * It seems that release reset to the DDR controller via
     * SCU_SRST must be done before clock enable via SCU_CLKEN,
     * otherwise the system will hang forever.
     */

    value &= ~(SCU_SRST_MEMC | SCU_SRST_DDR_PHY);
    romWrite32 (SCU_SRST, value);

    value = romRead32 (SCU_CLKEN);
    romWrite32 (SCU_CLKEN, value | SCU_CLKEN_MEMC_AXI |
                SCU_CLKEN_MEMC_PLB | SCU_CLKEN_MEMC_DDR);

    value = romRead32 (SCU_CSR_SRST);
    romWrite32 (SCU_CSR_SRST, value & ~SCU_CSR_SRST_MEMC);

    /* Disable DDR PLL */

    romWrite32 (SCU_SOCPLL2, romRead32 (SCU_SOCPLL2) | PLL_RESET | PLL_DOWN);

    /* Enable DDR PLL */

    value = romRead32 (SCU_SOCPLL2);
    value = (value | PLL_RESET) & ~(PLL_DOWN | PLL_BYPASS);
    romWrite32 (SCU_SOCPLL2, value);

    /* Set DDR clock frequency as 1333.33 MHz */

    romWrite32 (SCU_SOCPLL2, 0x0208009f);
    romWrite32 (SCU_SOCPLLADJ2, 0x00000058);

    value = romRead32 (SCU_PLLDLY);
    romBootDelay (10);

    romWrite32 (SCU_SOCPLL2, romRead32 (SCU_SOCPLL2) & ~PLL_RESET);

    value = romRead32 (SCU_PLLDLY);
    romBootDelay (10);

    if (romRead32 (MEM_RAM_SHUTDOWN))
        romWrite32 (MEM_RAM_SHUTDOWN, 0);

    do
        {
        value = romRead32 (SCU_MRDY);
        count++;
        }
    while ((!(value & MEMC_RDY_MASK)) && (count < MEM_RDY_TIMEOUT_COUNT));

    romWrite32 (SCU_SOC_SMS_CTL, romRead32 (SCU_SOC_SMS_CTL) | 0x80);

    return;
    }

/*******************************************************************************
*
* sysSdramInit - initialize the DDR3 SDRAM
*
* This routine initializes DDR3 SDRAM on board.
*
* RETURNS: N/A
*
* \NOMANUAL
*/

LOCAL void romSdramInit (void)
    {
    UINT32 value;
    UINT32 rddataEn;
    UINT32 address;

    /* Enable APM86x90 DDR controller */

    romWrite32 (DDR_IO_CONTROL, 0x39000000);
    romWrite32 (MEM_RAM_SHUTDOWN, 0);

    /* Soft reset PHY */

    value = romRead32 (DDRC0_00);
    value &= ~DDRC_PHY_RST;
    romWrite32 (DDRC0_00, value);

    romBootDelay (3);

    value &= ~(DDRC_RMODE_MASK | DDRC_OTD_MASK | DDRC_ACTIVE_RANK_MASK);
    value |= (DDRC_HIGH_SPEED_MODE + DDRC_OTD_SETTING + DDRC_ACTIVE_ALL_RANK + DDRC_PHY_RST);
    romWrite32 (DDRC0_00, value);

    romWrite32 (DDRC0_01, 0x44000051);
    romWrite32 (DDRC0_05, 0x20102812);
    romWrite32 (DDRC0_06, 0x0c162ccb);
    romWrite32 (DDRC0_07, 0x6201a4a4);
    romWrite32 (DDRC0_08, 0x0488726a);
    romWrite32 (DDRC0_10, 0x0080400f);
    romWrite32 (DDRC0_11, 0x00000010);
    romWrite32 (DDRC0_12, 0x00000a50);
    romWrite32 (DDRC0_13, 0x00f0fc60);
    romWrite32 (DDRC0_18, 0x00000100);
    romWrite32 (DDRC0_19, 0x00304008);
    romWrite32 (PHY_1, 0x950030ff);
    romWrite32 (PHY_2, 0x45078800);

    rddataEn = romRead32 (DDRC0_19);

    romWrite32 (PHY_CTRL_SLAVE, 0x00000080);

    /* Clear wrlvl_init_ratio of DDR PHY 3 Rank X Slice X registers */

    address = PHY_3_R0_S0;
    for (; address  < (PHY_3_R3_S8 + 4); address += 4)
        {
        value = romRead32 (address) & ~WRLVL_INIT_RATIO_MASK;
        romWrite32 (address, value);
        }

    romWrite32 (PHY_DQ_OFFSET_0, 0x40404040);
    romWrite32 (PHY_DQ_OFFSET_1, 0x40404040);
    romWrite32 (PHY_DQ_OFFSET_2, 0x00000040);

    romWrite32 (PHY_WR_DQS_SLAVE_R0_S0, 0x00000057);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S0, 0x00000057);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S0, 0x00000057);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S0, 0x00000057);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S1, 0x0000005f);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S1, 0x0000005f);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S1, 0x0000005f);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S1, 0x0000005f);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S2, 0x0000006a);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S2, 0x0000006a);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S2, 0x0000006a);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S2, 0x0000006a);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S3, 0x00000079);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S3, 0x00000079);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S3, 0x00000079);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S3, 0x00000079);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S4, 0x000000ae);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S4, 0x000000ae);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S4, 0x000000ae);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S4, 0x000000ae);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S5, 0x000000c4);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S5, 0x000000c4);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S5, 0x000000c4);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S5, 0x000000c4);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S6, 0x000000e2);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S6, 0x000000e2);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S6, 0x000000e2);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S6, 0x000000e2);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S7, 0x000000fa);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S7, 0x000000fa);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S7, 0x000000fa);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S7, 0x000000fa);
    romWrite32 (PHY_WR_DQS_SLAVE_R0_S8, 0x00000098);
    romWrite32 (PHY_WR_DQS_SLAVE_R1_S8, 0x00000098);
    romWrite32 (PHY_WR_DQS_SLAVE_R2_S8, 0x00000098);
    romWrite32 (PHY_WR_DQS_SLAVE_R3_S8, 0x00000098);

    romWrite32 (PHY_WR_DATA_SLAVE_R0_S0, 0x0000009b);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S0, 0x0000009b);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S0, 0x0000009b);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S0, 0x0000009b);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S1, 0x0000009f);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S1, 0x0000009f);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S1, 0x0000009f);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S1, 0x0000009f);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S2, 0x000000aa);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S2, 0x000000aa);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S2, 0x000000aa);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S2, 0x000000aa);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S3, 0x000000b9);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S3, 0x000000b9);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S3, 0x000000b9);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S3, 0x000000b9);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S4, 0x000000ee);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S4, 0x000000ee);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S4, 0x000000ee);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S4, 0x000000ee);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S5, 0x00000104);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S5, 0x00000104);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S5, 0x00000104);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S5, 0x00000104);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S6, 0x00000122);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S6, 0x00000122);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S6, 0x00000122);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S6, 0x00000122);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S7, 0x0000013a);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S7, 0x0000013a);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S7, 0x0000013a);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S7, 0x0000013a);
    romWrite32 (PHY_WR_DATA_SLAVE_R0_S8, 0x000000d3);
    romWrite32 (PHY_WR_DATA_SLAVE_R1_S8, 0x000000d3);
    romWrite32 (PHY_WR_DATA_SLAVE_R2_S8, 0x000000d3);
    romWrite32 (PHY_WR_DATA_SLAVE_R3_S8, 0x000000d3);

    romWrite32 (PHY_3_R0_S0, 0x00007900);
    romWrite32 (PHY_3_R1_S0, 0x02807900);
    romWrite32 (PHY_3_R2_S0, 0x02807900);
    romWrite32 (PHY_3_R3_S0, 0x02807900);
    romWrite32 (PHY_3_R0_S1, 0x00008e00);
    romWrite32 (PHY_3_R1_S1, 0x02808e00);
    romWrite32 (PHY_3_R2_S1, 0x02808e00);
    romWrite32 (PHY_3_R3_S1, 0x02808e00);
    romWrite32 (PHY_3_R0_S2, 0x00009f00);
    romWrite32 (PHY_3_R1_S2, 0x02809f00);
    romWrite32 (PHY_3_R2_S2, 0x02809f00);
    romWrite32 (PHY_3_R3_S2, 0x02809f00);
    romWrite32 (PHY_3_R0_S3, 0x0000bf00);
    romWrite32 (PHY_3_R1_S3, 0x0280bf00);
    romWrite32 (PHY_3_R2_S3, 0x0280bf00);
    romWrite32 (PHY_3_R3_S3, 0x0280bf00);
    romWrite32 (PHY_3_R0_S4, 0x0000e600);
    romWrite32 (PHY_3_R1_S4, 0x0280e600);
    romWrite32 (PHY_3_R2_S4, 0x0280e600);
    romWrite32 (PHY_3_R3_S4, 0x0280e600);
    romWrite32 (PHY_3_R0_S5, 0x0000f900);
    romWrite32 (PHY_3_R1_S5, 0x0280f900);
    romWrite32 (PHY_3_R2_S5, 0x0280f900);
    romWrite32 (PHY_3_R3_S5, 0x0280f900);
    romWrite32 (PHY_3_R0_S6, 0x00011800);
    romWrite32 (PHY_3_R1_S6, 0x02811800);
    romWrite32 (PHY_3_R2_S6, 0x02811800);
    romWrite32 (PHY_3_R3_S6, 0x02811800);
    romWrite32 (PHY_3_R0_S7, 0x00013800);
    romWrite32 (PHY_3_R1_S7, 0x02813800);
    romWrite32 (PHY_3_R2_S7, 0x02813800);
    romWrite32 (PHY_3_R3_S7, 0x02813800);
    romWrite32 (PHY_3_R0_S8, 0x0000d300);
    romWrite32 (PHY_3_R1_S8, 0x0280d300);
    romWrite32 (PHY_3_R2_S8, 0x0280d300);
    romWrite32 (PHY_3_R3_S8, 0x0280d300);

    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S0,  0x00000105);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S0,  0x00000105);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S0,  0x00000105);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S0,  0x00000105);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S1,  0x0000011b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S1,  0x0000011b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S1,  0x0000011b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S1,  0x0000011b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S2,  0x0000013b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S2,  0x0000013b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S2,  0x0000013b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S2,  0x0000013b);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S3,  0x0000014d);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S3,  0x0000014d);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S3,  0x0000014d);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S3,  0x0000014d);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S4,  0x00000187);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S4,  0x00000187);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S4,  0x00000187);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S4,  0x00000187);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S5,  0x00000189);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S5,  0x00000189);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S5,  0x00000189);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S5,  0x00000189);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S6,  0x000001a5);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S6,  0x000001a5);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S6,  0x000001a5);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S6,  0x000001a5);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S7,  0x000001bc);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S7,  0x000001bc);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S7,  0x000001bc);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S7,  0x000001bc);
    romWrite32 (PHY_FIFO_WE_SLAVE_R0_S8,  0x0000016e);
    romWrite32 (PHY_FIFO_WE_SLAVE_R1_S8,  0x0000016e);
    romWrite32 (PHY_FIFO_WE_SLAVE_R2_S8,  0x0000016e);
    romWrite32 (PHY_FIFO_WE_SLAVE_R3_S8,  0x0000016e);

    romWrite32 (PHY_RD_DQS_SLAVE_R0_S0, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S0, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S0, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S0, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S1, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S1, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S1, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S1, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S2, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S2, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S2, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S2, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S3, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S3, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S3, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S3, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S4, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S4, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S4, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S4, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S5, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S5, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S5, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S5, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S6, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S6, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S6, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S6, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S7, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S7, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S7, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S7, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R0_S8, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R1_S8, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R2_S8, 0x00000040);
    romWrite32 (PHY_RD_DQS_SLAVE_R3_S8, 0x00000040);

    value = romRead32 (DDRC0_00);
    romWrite32 (DDRC0_00, value | 0x01);

    romWrite32 (MEM_ECC_BYPASS, 0xffffffff);

    /* Wait for DDR controller initialize done */

    do
        {
        value = romRead32 (DDRC0_DBG);
        romBootDelay (1);
        }
    while ((value & DDRC_MODE_MASK) != 0x01);

    return;
    }

/******************************************************************************
*
* romBoardEarlyInit - APM Serengeti board initialization
*
* This function initialize APM Serengeti board in the boot stage.
* It initializes the NOR Flash and and DDR interface, enables all
* the needed SOC IP blocks and power module.
*
* RETURNS: N/A
*
* ERRNO
*/

void romBoardEarlyInit (void)
    {
    /* Enable the Data Intervention of PLB5 */

    romDcrOutLong (PCICR, (romDcrInLong (PCICR) | DIE_MASK));

    /* Enable IP config register */

    if (!(romDcrInLong (0x184) & (0x01 << 30)))
        romDcrOutLong (0x184, romDcrInLong (0x184) | 0x40000000);

    /* Initialize NOR Flash interface */

    romWrite32 (NFLASH_BOOT_CFG, 0xff7f0000);
    romWrite32 (NFLASH_SRAM_BNKCFG_0, 0x07000000);
    romWrite32 (NFLASH_SRAM_CTRL2_0, 0x10507200);
    romWrite32 (NFLASH_SRAM_CFG_0, 0x7e030000);
    romWrite32 (NFLASH_SRAM_CTRL1_0, 0x40000000);

    romApbClockInit ();

    /* Enable DDR controller */

    romDdrCtlrEnable ();

    /* Initialize DDR memory on board */

    romSdramInit ();

    /* select SYS_REFCLK as serial clock frequency */

    romWrite32 (UART_MODE_SEL, 0x00);

    return;
    }

