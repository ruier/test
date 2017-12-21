/* serengeti.h - APM86x90 Serengeti board header */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,06feb12,syt  added I2C device macros.
01c,07sep11,x_s  added SMP support.
01b,06sep11,syt  added macro CPU_CLK_FERQ to fix bspvts test fail.
                 (WIND00295330)
01a,19jun11,syt  initial creation.
*/

/*
 * This file contains I/O addresses and related constants for the
 * APM Serengeti board.
*/

#ifndef __INCserengetih
#define __INCserengetih

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include    "apm86x90.h"

/* APM86290 Clock frequency defines */

#define FREQ_25_MHZ                 25000000
#define FREQ_100_MHZ                100000000
#define FREQ_1200_MHZ               1200000000
#define FREQ_1333_MHZ               1333333333

#define SYS_REFCLK                  FREQ_25_MHZ
#define CPU_CLK_FREQ                FREQ_1200_MHZ
#define SYS_DDR_FREQ                FREQ_1333_MHZ

/* SYS_CLK, AUX_CLK and WDT rates */

#define FIT_DIVIDER_TAP_3           25
#define FIT_DIVIDER_TAP_2           21
#define FIT_DIVIDER_TAP_1           17
#define FIT_DIVIDER_TAP_0           13

#define WDT_DIVIDER_TAP_3           33
#define WDT_DIVIDER_TAP_2           29
#define WDT_DIVIDER_TAP_1           25
#define WDT_DIVIDER_TAP_0           21

/* APM86290 Serengeti board map */

/* Nor Flash  */

#define FLASH_PART0_BASE            FLASH_BASE_ADRS
#define FLASH_PART1_BASE            (FLASH_PART0_BASE + 0x1000000)
#define FLASH_PART2_BASE            (FLASH_PART1_BASE + 0x1000000)
#define FLASH_PART3_BASE            (FLASH_PART2_BASE + 0x1000000)
#define FLASH_BASE_HI               0xf

/* DDR SDRAM */

#define PHY_MEM_BASE                0x00000000
#define PHY_MEM_BASE_HI             0x0
#define PHY_MEM_SIZE                0x40000000

#define LOCAL_MEM_START_ADRS        PHY_MEM_BASE
#define LOCAL_MEM_END_ADRS          (LOCAL_MEM_SIZE - 1)

/* I2C EEprom device */

#define AT24C02_DEV_ADDR            0x51
#define AT24C256_DEV_ADDR           0x52

#define AT24C02_PAGE_SIZE           8
#define AT24C02_DEV_SIZE            256

#define AT24C256_PAGE_SIZE          64
#define AT24C256_DEV_SIZE           32768

/* core number */

#define SERENGETI_MAX_CPUS          2

/* magic number for cpu loop of MP */

#define MP_MAGIC_RBV                0xfeedfeed

#ifndef _ASMLANGUAGE

typedef enum mpCpuState {
    mpCpuStateUnknown = 0,
    mpCpuStateReboot  = 1,
    mpCpuStateSleep   = 2,
    mpCpuStateBooting = 3,
    mpCpuStateReady   = 4,
    mpCpuStateRunning = 5,
    mpCpuStateError   = 6
} MP_CPU_STATE;

#else

#define SERENGETI_MP_CPU_STATUS_UNKNOWN   0
#define SERENGETI_MP_CPU_STATUS_REBOOT    1
#define SERENGETI_MP_CPU_STATUS_SLEEP     2
#define SERENGETI_MP_CPU_STATUS_BOOTING   3
#define SERENGETI_MP_CPU_STATUS_READY     4
#define SERENGETI_MP_CPU_STATUS_RUNNING   5
#define SERENGETI_MP_CPU_STATUS_ERROR     6

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCserengetih */

