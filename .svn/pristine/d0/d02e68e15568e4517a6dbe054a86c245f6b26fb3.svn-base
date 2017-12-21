/* sysDdrConf.c - Freescale T4240QDS DDR controller configuration */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,11mar13,syt  added the initial value tables for DDR initialization.
01c,26feb13,syt  rename file name to sysDdrConf.c and cleaning up.
01b,17feb13,syt  enabled the DDR controller 2 and 3.
01a,10feb13,syt  created.
*/

/*
DESCRIPTION
Configuration file for DDR controller

INCLUDE FILES:
*/

#include <vxWorks.h>
#include <h/spd/spdLib.h>
#include <h/spd/fslDdrCtlr.h>
#include "config.h"

/* forward declarations */

LOCAL UINT32 fslT4OcsFreqGet (void);
LOCAL UINT32 fslT4DdrFreqGet (void);

/* defines */

/* macro to compose 64-bit PHYS_ADDRs */

#define PHYS_64BIT_ADDR(h, l)       (((UINT64)h << 32) + l)

#ifdef INCLUDE_DDR_ECC
#   define  DDR_ECC_CFG             DDR_ECC_ENABLE
#else /* INCLUDE_DDR_ECC */
#   define  DDR_ECC_CFG             DDR_ECC_DISABLE
#endif /* INCLUDE_DDR_ECC */

/*
 * If not define DDR_FIX_PARAMS_CFG, in the early boot stage, boot loader
 * will probe all the DRAM DIMM in the DIMM sockets on target;
 * get DRAM device's parameters from the EEPROM on the valid DRAM DIMMs;
 * compute initial values for the DDR controller register;
 * initialize the DDR controller with the initial values.
 *
 * If define the macro DDR_FIX_PARAMS_CFG as DDR_FIX_PARAMS_ENABLE, boot loader
 * will use the BSP provided register initial values to initialize the DDR
 * controller.
 *
 * Changing macro DDR_FIX_PARAMS_CFG definition will require recreating and
 * reflashing the VxWorks boot loader.
 */

#ifndef DDR_FIX_PARAMS_CFG
#   define  DDR_FIX_PARAMS_CFG      DDR_FIX_PARAMS_DISABLE
#endif /* DRV_SPD_FSL */

/* globals */

/* Platform Clock Ratio Tables */

const UINT32 t4PlatRatioTable[32] = {
    0,  0,  0,  0,  0,  5,  6,  7,  8,  9, 10, 11, 12,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

/* DDR Bus Clock Ratio Tables */

const UINT32 t4DdrRatioTable[32] = {
    0,  1,  0,  0,  0,  5,  6,  0,  8,  9, 10,  0, 12, 13,  0,  0,
   16,  0, 18, 19, 20,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0
};

/* DDR controller group configuration table */

const UINT32   ddr0FixRegParams [] = {
    CS0_BNDS_VALUE, CS0_CONFIG_VALUE, 0x00, /* Chip Slelect register bank0 */
    CS1_BNDS_VALUE, CS1_CONFIG_VALUE, 0x00, /* Chip Slelect register bank1 */
    CS2_BNDS_VALUE, CS2_CONFIG_VALUE, 0x00, /* Chip Slelect register bank2 */
    CS3_BNDS_VALUE, CS3_CONFIG_VALUE, 0x00, /* Chip Slelect register bank3 */
    TIMING_CFG_3_VALUE,             /* DDR SDRAM timing configuration 3 */
    TIMING_CFG_0_VALUE,             /* DDR SDRAM timing configuration 0 */
    TIMING_CFG_1_VALUE,             /* DDR SDRAM timing configuration 1 */
    TIMING_CFG_2_VALUE,             /* DDR SDRAM timing configuration 2 */
    DDR_SDRAM_CFG_VALUE1,           /* DDR SDRAM control configuration */
    DDR_SDRAM_CFG_2_VALUE1,         /* DDR SDRAM control configuration 2 */
    DDR_SDRAM_MODE_CFG_VALUE,       /* DDR SDRAM mode configuration */
    DDR_SDRAM_MODE_CFG_2_VALUE,     /* DDR SDRAM mode configuration 2 */
    DDRx_DDR_SDRAM_MODE_3_VALUE,    /* DDR SDRAM mode configuration 3 */
    DDRx_DDR_SDRAM_MODE_4_VALUE,    /* DDR SDRAM mode configuration 4 */
    DDRx_DDR_SDRAM_MODE_5_VALUE,    /* DDR SDRAM mode configuration 5 */
    DDRx_DDR_SDRAM_MODE_6_VALUE,    /* DDR SDRAM mode configuration 6 */
    DDRx_DDR_SDRAM_MODE_7_VALUE,    /* DDR SDRAM mode configuration 7 */
    DDRx_DDR_SDRAM_MODE_8_VALUE,    /* DDR SDRAM mode configuration 8 */
    DDR_SDRAM_MD_CNTL_VALUE,        /* DDR SDRAM mode control */
    DDR_SDRAM_INTERVAL_VALUE,       /* DDR SDRAM interval configuration */
    DDR_DATA_INIT_VALUE,            /* DDR SDRAM data initialization */
    DDR_SDRAM_CLK_CTRL_VALUE,       /* DDR SDRAM clock control */
    DDR_INIT_ADDRESS,               /* DDR training initialization address */
    DDR_INIT_EXT_ADDRESS,           /* DDR training initialization extended address */
    TIMING_CFG_4_VALUE,             /* DDR SDRAM timing configuration 4 */
    TIMING_CFG_5_VALUE,             /* DDR SDRAM timing configuration 5 */
    DDR_ZQ_CNTL_VALUE,              /* DDR ZQ calibration control */
    DDR_WRLVL_CNTL_VALUE,           /* DDR write leveling control */
    DDR_WRLVL_CNTL_2_VALUE,         /* DDR write leveling control 2 */
    DDR_WRLVL_CNTL_3_VALUE,         /* DDR write leveling control 3 */
    0x00,                           /* DDR Self Refresh Counter register */
    0x00,                           /* DDR Register Control Words 1 */
    0x00,                           /* DDR Register Control Words 2 */
    0x00,                           /* DDR Enhanced Optimization Register */
    DDRCDR_1_VALUE,                 /* DDR Control Driver Register 1 */
    DDRCDR_2_VALUE,                 /* DDR Control Driver Register 2 */
    0x00,                           /* Memory error disable */
    0x00                            /* Memory error interrupt enable */
};

const UINT32   ddr1FixRegParams [] = {
    CS0_BNDS_2DDR_VALUE, CS0_CONFIG_2DDR_VALUE, 0x00, /* Chip Slelect register bank0 */
    CS1_BNDS_2DDR_VALUE, CS1_CONFIG_2DDR_VALUE, 0x00, /* Chip Slelect register bank1 */
    CS2_BNDS_2DDR_VALUE, CS2_CONFIG_2DDR_VALUE, 0x00, /* Chip Slelect register bank2 */
    CS3_BNDS_2DDR_VALUE, CS3_CONFIG_2DDR_VALUE, 0x00, /* Chip Slelect register bank3 */
    TIMING_CFG_3_VALUE,             /* DDR SDRAM timing configuration 3 */
    TIMING_CFG_0_VALUE,             /* DDR SDRAM timing configuration 0 */
    TIMING_CFG_1_VALUE,             /* DDR SDRAM timing configuration 1 */
    TIMING_CFG_2_VALUE,             /* DDR SDRAM timing configuration 2 */
    DDR_SDRAM_CFG_VALUE1,           /* DDR SDRAM control configuration */
    DDR_SDRAM_CFG_2_VALUE1,         /* DDR SDRAM control configuration 2 */
    DDR_SDRAM_MODE_CFG_VALUE,       /* DDR SDRAM mode configuration */
    DDR_SDRAM_MODE_CFG_2_VALUE,     /* DDR SDRAM mode configuration 2 */
    DDRx_DDR_SDRAM_MODE_3_VALUE,    /* DDR SDRAM mode configuration 3 */
    DDRx_DDR_SDRAM_MODE_4_VALUE,    /* DDR SDRAM mode configuration 4 */
    DDRx_DDR_SDRAM_MODE_5_VALUE,    /* DDR SDRAM mode configuration 5 */
    DDRx_DDR_SDRAM_MODE_6_VALUE,    /* DDR SDRAM mode configuration 6 */
    DDRx_DDR_SDRAM_MODE_7_VALUE,    /* DDR SDRAM mode configuration 7 */
    DDRx_DDR_SDRAM_MODE_8_VALUE,    /* DDR SDRAM mode configuration 8 */
    DDR_SDRAM_MD_CNTL_VALUE,        /* DDR SDRAM mode control */
    DDR_SDRAM_INTERVAL_VALUE,       /* DDR SDRAM interval configuration */
    DDR_DATA_INIT_VALUE,            /* DDR SDRAM data initialization */
    DDR_SDRAM_CLK_CTRL_VALUE,       /* DDR SDRAM clock control */
    DDR_INIT_ADDRESS,               /* DDR training initialization address */
    DDR_INIT_EXT_ADDRESS,           /* DDR training initialization extended address */
    TIMING_CFG_4_VALUE,             /* DDR SDRAM timing configuration 4 */
    TIMING_CFG_5_VALUE,             /* DDR SDRAM timing configuration 5 */
    DDR_ZQ_CNTL_VALUE,              /* DDR ZQ calibration control */
    DDR_WRLVL_CNTL_VALUE,           /* DDR write leveling control */
    DDR_WRLVL_CNTL_2_VALUE,         /* DDR write leveling control 2 */
    DDR_WRLVL_CNTL_3_VALUE,         /* DDR write leveling control 3 */
    0x00,                           /* DDR Self Refresh Counter register */
    0x00,                           /* DDR Register Control Words 1 */
    0x00,                           /* DDR Register Control Words 2 */
    0x00,                           /* DDR Enhanced Optimization Register */
    DDRCDR_1_VALUE,                 /* DDR Control Driver Register 1 */
    DDRCDR_2_VALUE,                 /* DDR Control Driver Register 2 */
    0x00,                           /* Memory error disable */
    0x00                            /* Memory error interrupt enable */
};

const UINT32   ddr2FixRegParams [] = {
    CS0_BNDS_3DDR_VALUE, CS0_CONFIG_3DDR_VALUE, 0x00, /* Chip Slelect register bank0 */
    CS1_BNDS_3DDR_VALUE, CS1_CONFIG_3DDR_VALUE, 0x00, /* Chip Slelect register bank1 */
    CS2_BNDS_3DDR_VALUE, CS2_CONFIG_3DDR_VALUE, 0x00, /* Chip Slelect register bank2 */
    CS3_BNDS_3DDR_VALUE, CS3_CONFIG_3DDR_VALUE, 0x00, /* Chip Slelect register bank3 */
    TIMING_CFG_3_VALUE,             /* DDR SDRAM timing configuration 3 */
    TIMING_CFG_0_VALUE,             /* DDR SDRAM timing configuration 0 */
    TIMING_CFG_1_VALUE,             /* DDR SDRAM timing configuration 1 */
    TIMING_CFG_2_VALUE,             /* DDR SDRAM timing configuration 2 */
    DDR_SDRAM_CFG_VALUE1,           /* DDR SDRAM control configuration */
    DDR_SDRAM_CFG_2_VALUE1,         /* DDR SDRAM control configuration 2 */
    DDR_SDRAM_MODE_CFG_VALUE,       /* DDR SDRAM mode configuration */
    DDR_SDRAM_MODE_CFG_2_VALUE,     /* DDR SDRAM mode configuration 2 */
    DDRx_DDR_SDRAM_MODE_3_VALUE,    /* DDR SDRAM mode configuration 3 */
    DDRx_DDR_SDRAM_MODE_4_VALUE,    /* DDR SDRAM mode configuration 4 */
    DDRx_DDR_SDRAM_MODE_5_VALUE,    /* DDR SDRAM mode configuration 5 */
    DDRx_DDR_SDRAM_MODE_6_VALUE,    /* DDR SDRAM mode configuration 6 */
    DDRx_DDR_SDRAM_MODE_7_VALUE,    /* DDR SDRAM mode configuration 7 */
    DDRx_DDR_SDRAM_MODE_8_VALUE,    /* DDR SDRAM mode configuration 8 */
    DDR_SDRAM_MD_CNTL_VALUE,        /* DDR SDRAM mode control */
    DDR_SDRAM_INTERVAL_VALUE,       /* DDR SDRAM interval configuration */
    DDR_DATA_INIT_VALUE,            /* DDR SDRAM data initialization */
    DDR_SDRAM_CLK_CTRL_VALUE,       /* DDR SDRAM clock control */
    DDR_INIT_ADDRESS,               /* DDR training initialization address */
    DDR_INIT_EXT_ADDRESS,           /* DDR training initialization extended address */
    TIMING_CFG_4_VALUE,             /* DDR SDRAM timing configuration 4 */
    TIMING_CFG_5_VALUE,             /* DDR SDRAM timing configuration 5 */
    DDR_ZQ_CNTL_VALUE,              /* DDR ZQ calibration control */
    DDR_WRLVL_CNTL_VALUE,           /* DDR write leveling control */
    DDR_WRLVL_CNTL_2_VALUE,         /* DDR write leveling control 2 */
    DDR_WRLVL_CNTL_3_VALUE,         /* DDR write leveling control 3 */
    0x00,                           /* DDR Self Refresh Counter register */
    0x00,                           /* DDR Register Control Words 1 */
    0x00,                           /* DDR Register Control Words 2 */
    0x00,                           /* DDR Enhanced Optimization Register */
    DDRCDR_1_VALUE,                 /* DDR Control Driver Register 1 */
    DDRCDR_2_VALUE,                 /* DDR Control Driver Register 2 */
    0x00,                           /* Memory error disable */
    0x00                            /* Memory error interrupt enable */
};

/*
 * The T4240 has three DDR3 controllers, supporting DDR3 and DDR3LP devices;
 * the T4240QDS supports this with two industry-standard JEDEC DDR3 DIMM
 * connectors (240 pin) per controller. For each DDR controller, the DIMM socket
 * that is furthest from the processor (DIMM #1) is the only one that supports
 * quad-rank devices, which use all four chip-selects for that controller;
 * in that case, DIMM #2 must remain empty. If single- or dual-rank devices
 * are used, only they can both DIMM slots be populated.
 *
 * The following table is the currently configurations of the DIMM sockets.
 *
 * DDR controller  DIMM socket#  I2C Bus#  I2C EEPROM ADDR  CS combination
 * --------------  ------------  --------  ---------------  --------------
 * DDR #1          DIMM #1       I2C #0    0x51             CS0_CS1_CS2_CS3
 * DDR #1          DIMM #2       I2C #0    0x52             CS2_CS3
 * DDR #2          DIMM #1       I2C #0    0x53             CS0_CS1_CS2_CS3
 * DDR #2          DIMM #2       I2C #0    0x54             CS2_CS3
 * DDR #3          DIMM #1       I2C #0    0x55             CS0_CS1_CS2_CS3
 * DDR #3          DIMM #2       I2C #0    0x56             CS2_CS3
 *
 */

/* DIMM socket configurations of DDR controller 1 */

const DIMM_SLOT_CONF    ddrCtlr0SlotsHwconf[] = {

/*
 *    I2C map base      I2C EEPROM ADDR     CS combination
 *    ------------      ---------------     --------------
 */

    { T4_I2C1_BASE,     DIMM_SLOT0_ADDR,    CS0_CS1_CS2_CS3 },
    { T4_I2C1_BASE,     DIMM_SLOT1_ADDR,    CS2_CS3         },
};

/* DIMM socket configurations of DDR controller 2 */

const DIMM_SLOT_CONF    ddrCtlr1SlotsHwconf[] = {

/*
 *    I2C map base      I2C EEPROM ADDR     CS combination
 *    ------------      ---------------     --------------
 */

    { T4_I2C1_BASE,     DIMM_SLOT2_ADDR,    CS0_CS1_CS2_CS3 },
    { T4_I2C1_BASE,     DIMM_SLOT3_ADDR,    CS2_CS3         },
};

/* DIMM socket configurations of DDR controller 3 */

const DIMM_SLOT_CONF    ddrCtlr2SlotsHwconf[] = {

/*
 *    I2C map base      I2C EEPROM ADDR     CS combination
 *    ------------      ---------------     --------------
 */

    { T4_I2C1_BASE,     DIMM_SLOT4_ADDR,    CS0_CS1_CS2_CS3 },
    { T4_I2C1_BASE,     DIMM_SLOT5_ADDR,    CS2_CS3         },
};

const DDR_HWCONF  sysDdrHwconf [] = {

    /* DDR controller 0 hardware configuration */

    {
    (UINT64)T4_DDR1_BASE,                   /* DDR1 controller base */
    (UINT64)T4_LAWBARH0,                    /* Local Access Window base */
    (UINT64)DDR_SDRAM_LOCAL_ADRS,           /* Physical map base address specified by user */
    (UINT64)PHYS_MEM_SIZE,                  /* Physical map size specified by user */
    (DIMM_SLOT_CONF *)ddrCtlr0SlotsHwconf,  /* DDR DIMM slot configurations */
    2,                                      /* DIMM slot number on DDR1 controller */
    (void *)fslT4DdrFreqGet,                /* DDR bus frequency get function */
    (void *)fslT4OcsFreqGet,                /* I2C bus frequency get function */
    (FSL_DDR_REG *)ddr0FixRegParams,        /* Entry of the fixed parameter table */
    (
    DDR_ECC_CFG | DDR_FIX_PARAMS_CFG        /* Eanble or disable ECC and fix parameter mode */
#ifdef FSL_ERRATA_DDR_A004390
    | T4_ERRATUM_DDR4390                    /* Errata DDR A-004390 */
#endif /* FSL_ERRATA_DDR_A004390 */
#ifdef FSL_ERRATA_DDR_A004934
    | T4_ERRATUM_DDR4934                    /* Errata DDR A-004934 */
#endif /* FSL_ERRATA_DDR_A004934 */
    )
    },

    /* DDR controller 1 hardware configuration */

    {
    (UINT64)T4_DDR2_BASE,                   /* DDR2 controller base */
    (UINT64)T4_LAWBARH0,                    /* Local Access Window base */
    (UINT64)(PHYS_64BIT_ADDR(DDR_SDRAM_LOCAL_ADRS2_H, DDR_SDRAM_LOCAL_ADRS2)),
                                            /* Physical map base address specified by user */
    (UINT64)PHYS_MEM_SIZE,                  /* Physical map size specified by user */
    (DIMM_SLOT_CONF *)ddrCtlr1SlotsHwconf,  /* DDR DIMM slot configurations */
    2,                                      /* DIMM slot number on DDR1 controller */
    (void *)fslT4DdrFreqGet,                /* DDR bus frequency get function */
    (void *)fslT4OcsFreqGet,                /* I2C bus frequency get function */
    (FSL_DDR_REG *)ddr1FixRegParams,        /* Entry of the fixed parameter table */
    (
    DDR_ECC_CFG | DDR_FIX_PARAMS_CFG        /* Eanble or disable ECC and fix parameter mode */
#ifdef FSL_ERRATA_DDR_A004390
    | T4_ERRATUM_DDR4390                    /* Errata DDR A-004390 */
#endif /* FSL_ERRATA_DDR_A004390 */
#ifdef FSL_ERRATA_DDR_A004934
    | T4_ERRATUM_DDR4934                    /* Errata DDR A-004934 */
#endif /* FSL_ERRATA_DDR_A004934 */
    )
    },

    /* DDR controller 2 hardware configuration */

    {
    (UINT64)T4_DDR3_BASE,                   /* DDR3 controller base */
    (UINT64)T4_LAWBARH0,                    /* Local Access Window base */
    (UINT64)(PHYS_64BIT_ADDR(DDR_SDRAM_LOCAL_ADRS3_H, DDR_SDRAM_LOCAL_ADRS3)),
                                            /* Physical map base address specified by user */
    (UINT64)PHYS_MEM_SIZE,                  /* Physical map size specified by user */
    (DIMM_SLOT_CONF *)ddrCtlr2SlotsHwconf,  /* DDR DIMM slot configurations */
    2,                                      /* DIMM slot number on DDR1 controller */
    (void *)fslT4DdrFreqGet,                /* DDR bus frequency get function */
    (void *)fslT4OcsFreqGet,                /* I2C bus frequency get function */
    (FSL_DDR_REG *)ddr2FixRegParams,        /* Entry of the fixed parameter table */
    (
    DDR_ECC_CFG | DDR_FIX_PARAMS_CFG        /* Eanble or disable ECC and fix parameter mode */
#ifdef FSL_ERRATA_DDR_A004390
    | T4_ERRATUM_DDR4390                    /* Errata DDR A-004390 */
#endif /* FSL_ERRATA_DDR_A004390 */
#ifdef FSL_ERRATA_DDR_A004934
    | T4_ERRATUM_DDR4934                    /* Errata DDR A-004934 */
#endif /* FSL_ERRATA_DDR_A004934 */
    )
    }
};

/* T4240QDS memory configuration table */

const SYS_MEM_HWCONF    sysMemHwConf[] = {
    {
    T4_DDR_CTLR_NUM,                        /* 3 DDR controllers on T4240 */
    (DDR_HWCONF *)sysDdrHwconf,             /* Entry of the DDR controllers configutation table */
    }
};

/*******************************************************************************
*
* fslT4FreqGet - get the clock freq of the platform or DDR
*
* This function calculates the frame CCB and DDR clock frequency and
* returns it.
*
* RETURNS: CCB or DDR clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 fslT4FreqGet
    (
    UINT32  freqFlag
    )
    {
    UINT32  oscFreq;
    UINT32  clkFreq;

    switch (*((volatile unsigned char *)QX_CLK_SPD1) & 0x7)
        {
        case QX_SYSCLK_67_MHZ:
            oscFreq = FREQ_67_MHZ;
            break;
        case QX_SYSCLK_83_MHZ:
            oscFreq = FREQ_83_MHZ;
            break;
        case QX_SYSCLK_100_MHZ:
            oscFreq = FREQ_100_MHZ;
            break;
        case QX_SYSCLK_125_MHZ:
            oscFreq = FREQ_125_MHZ;
            break;
        case QX_SYSCLK_133_MHZ:
            oscFreq = FREQ_133_MHZ;
            break;
        case QX_SYSCLK_150_MHZ:
            oscFreq = FREQ_150_MHZ;
            break;
        case QX_SYSCLK_160_MHZ:
            oscFreq = FREQ_160_MHZ;
            break;
        case QX_SYSCLK_167_MHZ:
            oscFreq = FREQ_167_MHZ;
            break;
        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    switch (freqFlag)
        {
        case PLATFORM_CLK:
            clkFreq = oscFreq * t4PlatRatioTable[T4_SYS_PLL_RAT];
            break;
        case DDRBUS_CLK:
            clkFreq = oscFreq * t4DdrRatioTable[T4_MEM_PLL_RAT];
            break;
        default:
            clkFreq = 0;
        }

    return clkFreq;
    }

/*******************************************************************************
*
* fslT4OcsFreqGet - get the clock freq of the platform
*
* This function calculates the CCB clock frequency and returns it.
*
* RETURNS: CCB clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 fslT4OcsFreqGet (void)
    {
    return (fslT4FreqGet (PLATFORM_CLK));
    }

/*******************************************************************************
*
* fslT4OcsFreqGet - get the clock freq of the DDR bus
*
* This function calculates the DDR clock frequency and returns it.
*
* RETURNS: CCB clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 fslT4DdrFreqGet (void)
    {
    return (fslT4FreqGet (DDRBUS_CLK));
    }

