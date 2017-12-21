/* t4240qds.h - Freescale T4240QDS board header */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02b,17apr13,j_z  add RapidIO memory map definition.
02a,14mar13,h_k  changed DDR_DATA_INIT_VALUE to 0 for a cosmetic purpose.
01z,11mar13,syt  added two DDR related initial value defines and removed
                 DDR_SDRAM_LAWAR_TGTIFx macros.
01y,26feb13,syt  added DIMM slot related macro.
01x,13feb13,syt  decrease the temp stack size used for DDR initialization.
01w,08feb13,syt  added some macros related with L1 Cache.
01v,06feb13,wyt  removed Simics definition.
01u,25jan13,wyt  fixed some macro defines.
01t,22jan13,h_k  cleaned up UBOOT macros.
01s,09jan13,x_s  added back QIXIS register set from fslSocT4.h.
01r,17dec12,x_s  added DDR configuration parameters.
01q,30nov12,x_s  removed SOC related definitions to file fslSocT4.h.
01p,30nov12,syt  added USB PHY related definitions
01o,21nov12,h_k  added SPR631 definition.
01n,09nov12,h_k  added L3_CACHE_REG_BASE_ADDR and L2_CACHE_REG_BASE_ADDR.
                 changed the number of L2 and L3 to 1 only for non-AMP UP
                 without RTP feature enabled.
                 disabled dynamic harvard mode for AMP and SMP to support
                 extra clusters.
                 updated MMU_ROM_ACCESS, MMU_UNCACHED, MMU_CACHED.
01m,08nov12,h_k  added L2_CACHE_NUMBERS and L3_CACHE_NUMBERS.
01l,05nov12,syt  added I2C support.
01k,06nov12,h_k  added TPH10 control and status register macros.
01j,30oct12,h_k  added IFC definitions. updated DDR rgister settings.
01i,19oct12,syt  added DMA related defines.
01h,18oct12,syt  added SDHC support and some LAW register defines.
01g,15oct12,wap  Add support for SGMII ethernet ports, make some additional
                 cleanups
01f,10oct12,syt  added PCI-EX support and code clean up.
01e,02oct12,h_k  changed QIXIS_BASE for UBOOT.
01d,14sep12,h_k  removed TGEC related macros.
01c,12sep12,h_k  added EPIC QMan BMan portal int vec 10-23.
01b,13aug12,wap  Update Qman/Bman for T4240
01a,07may12,h_k  Created from t4240qds.h 01i
*/

/*
 * This file contains I/O addresses and related constants for the
 * FreeScale T4240QDS board.
 */

#ifndef __INCt4240qdsh
#define __INCt4240qdsh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include <hwif/soc/fslSocT4.h>  


/* Core set */

#define CPU_BIT(n)                  (1 << n)
#define MAX_CPUS                    24

/* number of the L2 and L3 caches */

#if defined(_WRS_CONFIG_SMP) || defined(INCLUDE_AMP)
#define L2_CACHE_NUMBERS            3
#define L3_CACHE_NUMBERS            3

/*
 * Disable dynamic harvard mode.
 *
 * Cacheable instruction fetches requested by the processor that miss are
 * requested from CoreNet as coherent.  When the line is allocated it is marked
 * to allow a hit from instruction fetches and data accesses.
 */

#define L2_CACHE_CSR1_VAL           L2CSR1_DYNAMICHARVARD_MSK
#else
#define L2_CACHE_NUMBERS            1

# ifdef	INCLUDE_RTP
# define L3_CACHE_NUMBERS           3
# else
# define L3_CACHE_NUMBERS           1
# endif	/* INCLUDE_RTP */
#endif	/* WRS_CONFIG_SMP || INCLUDE_AMP */

/* CCSBAR used to access L2 and L3 caches */

#define L2_CACHE_REG_BASE_ADDR      CCSBAR
#define L3_CACHE_REG_BASE_ADDR      CCSBAR

#define ROM_INIT_RAM_BASE           0xF0000000  /* Initial L1 address */
#define ROM_INIT_RAM_SIZE           0x00004000
#define ROM_STACK_ADRS              (ROM_INIT_RAM_BASE + ROM_INIT_RAM_SIZE/4)

#define L1_CACHE_SHIFT              6
#define L1_CACHE_LINE_SIZE          (1 << L1_CACHE_SHIFT)

/* Define Clock Speed and source */

#define FREQ_1_MHZ                  1000000
#define FREQ_33_MHZ                 33333334
#define FREQ_50_MHZ                 50000000
#define FREQ_67_MHZ                 66666667
#define FREQ_75_MHZ                 75000000
#define FREQ_83_MHZ                 83333334
#define FREQ_90_MHZ                 90000000
#define FREQ_100_MHZ                100000000
#define FREQ_111_MHZ                111000000
#define FREQ_125_MHZ                125000000
#define FREQ_133_MHZ                133333334
#define FREQ_150_MHZ                150000000
#define FREQ_160_MHZ                160000000
#define FREQ_167_MHZ                166666667
#define FREQ_266_MHZ                266666000
#define FREQ_400_MHZ                400000000
#define FREQ_333_MHZ                333333334
#define FREQ_533_MHZ                533333000
#define FREQ_500_MHZ                500000000
#define FREQ_600_MHZ                600000000
#define FREQ_800_MHZ                800000000
#define OSCILLATOR_FREQ             FREQ_100_MHZ

/* DDR configuration for board */

#define CS0_BNDS_VALUE              (((DDR_SDRAM_LOCAL_ADRS >> 8) & \
                                      0xff0000) | \
                                     ((DDR_SDRAM_LOCAL_END >> 24) & 0xff) | \
                                     (DDR_SDRAM_LOCAL_ADRS_H << 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS_H << 8))
#define CS1_BNDS_VALUE              0x00000000
#define CS2_BNDS_VALUE              0x00000000
#define CS3_BNDS_VALUE              0x00000000
#define CS0_BNDS_2DDR_VALUE         (((DDR_SDRAM_LOCAL_ADRS2 >> 8) & \
                                      0xff0000) | \
                                     ((DDR_SDRAM_LOCAL_END2 >> 24) & 0xff) | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 8))
#define CS1_BNDS_2DDR_VALUE         0x00000000
#define CS2_BNDS_2DDR_VALUE         0x00000000
#define CS3_BNDS_2DDR_VALUE         0x00000000
#define CS0_BNDS_3DDR_VALUE         (((DDR_SDRAM_LOCAL_ADRS3 >> 8) & \
                                      0xff0000) | \
                                     ((DDR_SDRAM_LOCAL_END3 >> 24) & 0xff) |\
                                     (DDR_SDRAM_LOCAL_ADRS3_H << 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS3_H << 8))
#define CS1_BNDS_3DDR_VALUE         0x00000000
#define CS2_BNDS_3DDR_VALUE         0x00000000
#define CS3_BNDS_3DDR_VALUE         0x00000000
#define CS0_CONFIG_VALUE            0x80014302
#define CS1_CONFIG_VALUE            0x00000000
#define CS2_CONFIG_VALUE            CS1_CONFIG_VALUE
#define CS3_CONFIG_VALUE            CS1_CONFIG_VALUE
#define CS0_CONFIG_2DDR_VALUE       CS0_CONFIG_VALUE
#define CS1_CONFIG_2DDR_VALUE       CS1_CONFIG_VALUE
#define CS2_CONFIG_2DDR_VALUE       CS2_CONFIG_VALUE
#define CS3_CONFIG_2DDR_VALUE       CS3_CONFIG_VALUE
#define CS0_CONFIG_3DDR_VALUE       CS0_CONFIG_VALUE
#define CS1_CONFIG_3DDR_VALUE       CS1_CONFIG_VALUE
#define CS2_CONFIG_3DDR_VALUE       CS2_CONFIG_VALUE
#define CS3_CONFIG_3DDR_VALUE       CS3_CONFIG_VALUE
#define TIMING_CFG_3_VALUE          0x00071000
#define TIMING_CFG_0_VALUE          0x50110004
#define TIMING_CFG_1_VALUE          0xbbb58c46
#define TIMING_CFG_2_VALUE          0x0040c8d4

#ifdef INCLUDE_DDR_ECC
#  define DDR_SDRAM_CFG_VALUE1      0x67040000
#  define DDR_SDRAM_CFG_VALUE2      0xe7040000
#else /* INCLUDE_DDR_ECC */
#  define DDR_SDRAM_CFG_VALUE1      0x47040000
#  define DDR_SDRAM_CFG_VALUE2      0xc7040000
#endif /* INCLUDE_DDR_ECC */

#define DDR_SDRAM_CFG_2_VALUE1      0x00401100
#define DDR_SDRAM_CFG_2_VALUE2      0x00401110
#define DDR_SDRAM_MODE_CFG_VALUE    0x00441c70
#define DDR_SDRAM_MODE_CFG_2_VALUE  0x00180000
#define DDR_SDRAM_MD_CNTL_VALUE     0x00000000
#define DDR_SDRAM_INTERVAL_VALUE    0x18600100
#define DDR_DATA_INIT_VALUE         0x00000000
#define DDR_SDRAM_CLK_CTRL_VALUE    0x02800000
#define DDR_SDRAM_CLK_CTRL_VALUE    0x02800000
#define DDR_INIT_ADDRESS            0x0
#define DDR_INIT_EXT_ADDRESS        0x0
#define TIMING_CFG_4_VALUE          0x00000001
#define TIMING_CFG_5_VALUE          0x04401400
#define DDR_ZQ_CNTL_VALUE           0x89080600
#define DDR_WRLVL_CNTL_VALUE        0x8675f608
#define DDR_WRLVL_CNTL_2_VALUE      0x080a0a0c
#define DDR_WRLVL_CNTL_3_VALUE      0x0c0d0e0a
#define DDRx_DDR_SDRAM_MODE_3_VALUE 0x00001c70
#define DDRx_DDR_SDRAM_MODE_4_VALUE 0x00180000
#define DDRx_DDR_SDRAM_MODE_5_VALUE 0x00001c70
#define DDRx_DDR_SDRAM_MODE_6_VALUE 0x00180000
#define DDRx_DDR_SDRAM_MODE_7_VALUE 0x00001c70
#define DDRx_DDR_SDRAM_MODE_8_VALUE 0x00180000

#define DDRCDR_1_VALUE              0x80040000
#define DDRCDR_2_VALUE              0x00000001

/* DDR DIMM slot's I2C address */

#define DIMM_SLOT0_ADDR             0x51
#define DIMM_SLOT1_ADDR             0x52
#define DIMM_SLOT2_ADDR             0x53
#define DIMM_SLOT3_ADDR             0x54
#define DIMM_SLOT4_ADDR             0x55
#define DIMM_SLOT5_ADDR             0x56

/* T4240 DDR controller number */

#define T4_DDR_CTLR_NUM             3

/* QIXIS Registers - see T4DS-revA.pdf $7.3 */

#define QIXIS_BASE                  0xEFDF0000
#define QIXIS_SIZE                  0x00001000

#define QX_ID                       (QIXIS_BASE + 0x00)
#define QX_VER                      (QIXIS_BASE + 0x01)
#define QX_QVER                     (QIXIS_BASE + 0x02)
#define QX_MODEL                    (QIXIS_BASE + 0x03)
#define QX_QTAG                     (QIXIS_BASE + 0x04)
#define QX_SCR                      (QIXIS_BASE + 0x05)
#define QX_AUX                      (QIXIS_BASE + 0x06)

/* SPD register is deprecated on T4240 */

#define QX_SPD                      (QIXIS_BASE + 0x07)
#define QX_STAT_DUT                 (QIXIS_BASE + 0x08)
#define QX_STAT_SYS                 (QIXIS_BASE + 0x09)
#define QX_STAT_ALARM               (QIXIS_BASE + 0x0A)
#define QX_STAT_PRES1               (QIXIS_BASE + 0x0B)
#define QX_STAT_PRES2               (QIXIS_BASE + 0x0C)
#define QX_RCW_CTL                  (QIXIS_BASE + 0x0D)
#define QX_LED                      (QIXIS_BASE + 0x0E)
#define QX_QMODE                    (QIXIS_BASE + 0x0F)
#define QX_RCFG_CTL                 (QIXIS_BASE + 0x10)
#define QX_RCFG_STAT                (QIXIS_BASE + 0x11)
#define QX_DCM_ADDR                 (QIXIS_BASE + 0x12)
#define QX_DCM_DATA                 (QIXIS_BASE + 0x13)
#define QX_DCM_CMD                  (QIXIS_BASE + 0x14)
#define QX_DCM_MSG                  (QIXIS_BASE + 0x15)
#define QX_GDC                      (QIXIS_BASE + 0x16)
#define QX_GDD                      (QIXIS_BASE + 0x17)
#define QX_DCM_MACK                 (QIXIS_BASE + 0x18)
/* offsets 0x19 is reserved */
#define QX_DMA_CR                   (QIXIS_BASE + 0x1A)
#define QX_DMA_SR                   (QIXIS_BASE + 0x1B)
#define QX_WATCH                    (QIXIS_BASE + 0x1F)
#define QX_PWR_CTL1                 (QIXIS_BASE + 0x20)
#define QX_PWR_CTL2                 (QIXIS_BASE + 0x21)
#define QX_PWR_MSTAT                (QIXIS_BASE + 0x24)
#define QX_PWR_STATn(n)             (QIXIS_BASE + 0x25 + (n - 1)*0x1)
#define QX_CLK_SPD1                 (QIXIS_BASE + 0x30)
#define QX_CLK_SPD2                 (QIXIS_BASE + 0x31)
#define QX_CLK_CTL                  (QIXIS_BASE + 0x32)
#define QX_CLK_ID                   (QIXIS_BASE + 0x33)
#define QX_CLK_ADDR(n)              (QIXIS_BASE + 0x34 + (0x2 * n))
#define QX_CLK_DATA(n)              (QIXIS_BASE + 0x35 + (0x2 * n))
#define QX_RST_CTL                  (QIXIS_BASE + 0x40)
#define QX_RST_STAT                 (QIXIS_BASE + 0x41)
#define QX_RST_REASON               (QIXIS_BASE + 0x42)
#define QX_RST_FORCE(n)             (QIXIS_BASE + 0x43 + (n - 1)*0x1)
#define QX_RST_TRACK                (QIXIS_BASE + 0x48)
#define QX_RST_MASK1                (QIXIS_BASE + 0x4B)
#define QX_RST_MASK2                (QIXIS_BASE + 0x4C)
#define QX_RST_MASK3                (QIXIS_BASE + 0x4D)
#define QX_BRDCFG0                  (QIXIS_BASE + 0x50)
#define QX_BRDCFG1                  (QIXIS_BASE + 0x51)
#define QX_BRDCFG2                  (QIXIS_BASE + 0x52)
#define QX_BRDCFG3                  (QIXIS_BASE + 0x53)
#define QX_BRDCFG4                  (QIXIS_BASE + 0x54)
#define QX_BRDCFG5                  (QIXIS_BASE + 0x55)
#define QX_BRDCFG6                  (QIXIS_BASE + 0x56)
#define QX_BRDCFG7                  (QIXIS_BASE + 0x57)
#define QX_BRDCFG8                  (QIXIS_BASE + 0x58)
#define QX_BRDCFG9                  (QIXIS_BASE + 0x59)
#define QX_BRDCFG11                 (QIXIS_BASE + 0x5B)
#define QX_BRDCFG12                 (QIXIS_BASE + 0x5C)

#define QX_SYSCLK_67_MHZ            0
#define QX_SYSCLK_83_MHZ            1
#define QX_SYSCLK_100_MHZ           2
#define QX_SYSCLK_125_MHZ           3
#define QX_SYSCLK_133_MHZ           4
#define QX_SYSCLK_150_MHZ           5
#define QX_SYSCLK_160_MHZ           6
#define QX_SYSCLK_167_MHZ           7

/* Board config register 4 is used to handle EMI multiplexing */

#define QX_BRDCFG4_EMISEL           0xE0
#define QX_EMI(x)                   (((x) << 5) & QX_BRDCFG4_EMISEL)

/* EMI muxing settings */

#define QX_EMI1_RGMII               0
#define QX_EMI1_SLOT1               1
#define QX_EMI1_SLOT2               2
#define QX_EMI1_SLOT3               3
#define QX_EMI1_SLOT4               4
#define QX_EMI1_SLOT5               5
#define QX_EMI1_SLOT7               7

/* Reset force register 3 settings */

#define PCIEX_SLOT_5_RST            0x08
#define PCIEX_SLOT_6_RST            0x04
#define PCIEX_SLOT_7_RST            0x02
#define PCIEX_SLOT_8_RST            0x01

#ifndef _ASMLANGUAGE
typedef struct emimap
    {
    int         fmanNum;
    int         memacNum;
    int         emiPort;
    } EMI_MAP;
#endif

/* Frequency applied to the "RTC" pin */

#define RTC_FREQ                    25000000

#define DEFAULT_SYSCLKFREQ          FREQ_533_MHZ

/* Use to force freq used with DUART/Timers etc */

#undef  FORCE_DEFAULT_FREQ

/* Platform and DDR bus Frequency */

#define RCWSR0                      *((volatile unsigned int *)(CCSBAR + 0xE0100))
#define T4_MEM_PLL_RAT              ((RCWSR0 & 0x003E0000) >> 17)
#define T4_SYS_PLL_RAT              ((RCWSR0 & 0x3E000000) >> 25)

#define PLATFORM_CLK                0   /* Platfrom clock flag */
#define DDRBUS_CLK                  1   /* DDR clock flag */

/* Timers */

#define SYS_CLK_RATE_MIN            1
#define SYS_CLK_RATE_MAX            8000

#ifdef  INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif  /* INCLUDE_AUX_CLK */

#define AUX_CLK_RATE_MIN            1
#define AUX_CLK_RATE_MAX            50000

#define FIT_CLK_RATE_MIN            (DEFAULT_SYSCLKFREQ / (1 << 21))
#define FIT_CLK_RATE_MAX            (DEFAULT_SYSCLKFREQ / (1 << 10))

                                    
#define ETHERNET_MAC_HANDLER        
                                    
#define WR_ENET0                    0x00
#define WR_ENET1                    0xA0
#define WR_ENET2                    0x1E
                                    
#define CUST_ENET3_0                0xA0
#define CUST_ENET3_1                0xA1
#define CUST_ENET3_2                0xA2
#define CUST_ENET3_3                0xA3
#define CUST_ENET3_4                0xA4
#define CUST_ENET3_5                0xA5
#define CUST_ENET3_6                0xA6
#define CUST_ENET3_7                0xA7
#define CUST_ENET3_8                0xA8
#define CUST_ENET3_9                0xA9
#define CUST_ENET3_10               0xAA
#define CUST_ENET3_11               0xAB
#define CUST_ENET3_12               0xAC
#define CUST_ENET3_13               0xAD
#define CUST_ENET3_14               0xAE
#define CUST_ENET3_15               0xAF
#define CUST_ENET4                  0xAA
#define CUST_ENET5                  0xA0

/* One type ethernet controllers on T4240, MEMAC */

#define MEMAC_NUM                   16

/* default mac address */

#define MAX_MAC_ADRS                MEMAC_NUM
#define MAC_ADRS_LEN                6
#define NV_MAC_ADRS_OFFSET          0x200

/* BOARD SPECIFIC VALUES: */

#define  FLASH_OR0_VALUE            0x0064
#define  LCRR_CLOCK_DIV             2
#define  LAWBARL2_VALUE             ((DDR_SDRAM_LOCAL_ADRS2) >> (LAWBAR_ADRS_SHIFT))
#define  LAWBARL3_VALUE             ((DDR_SDRAM_LOCAL_ADRS3) >> (LAWBAR_ADRS_SHIFT))

/* Base Address of buffer manager access window */

#define BMAN_LAW_BASE               0xF4000000

/* Base Address of queue manager access window */

#define QMAN_LAW_BASE               0xF6000000

/*
 * "magic numbers" used to validate the rebootVec.  The definitions
 * are arbitrary, but should not be "common" values like all zeros,
 * all ones, 0xeeeeeeee, 0x55aa55aa, 0xdeadbeef, etc.
 */

#define MP_MAGIC_RBV                0xfeedfeed
#define MP_MAGIC_CACHE_FLUSHED      0xD07ED07E

/* Base address of debug control/status registers */

#define DCSR_LAW_BASE               0xFD000000
#define DCSR_LAW_SIZE               0x400000

#define PCIEX1_LAW_BASE             0x80000000       /* PCI LAW window */
#define PCIEX1_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX1_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX1_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX1_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX1_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX1_MEM_ADRS             (PCIEX1_LAW_BASE)
#define PCIEX1_MEMIO_ADRS           (PCIEX1_MEM_ADRS   + PCIEX1_MEM_SIZE)
#define PCIEX1_IO_ADRS              (PCIEX1_MEMIO_ADRS + PCIEX1_MEMIO_SIZE)
#define PCIEX1_IO32_ADRS            (PCIEX1_IO_ADRS    + PCIEX1_IO_SIZE)

#define PCIEX2_LAW_BASE             0xA0000000       /* PCI LAW window */
#define PCIEX2_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX2_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX2_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX2_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX2_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX2_MEM_ADRS            (PCIEX2_LAW_BASE)
#define PCIEX2_MEMIO_ADRS          (PCIEX2_MEM_ADRS   + PCIEX2_MEM_SIZE)
#define PCIEX2_IO_ADRS             (PCIEX2_MEMIO_ADRS + PCIEX2_MEMIO_SIZE)
#define PCIEX2_IO32_ADRS           (PCIEX2_IO_ADRS    + PCIEX2_IO_SIZE)

#define PCIEX3_LAW_BASE             0xC0000000       /* PCI LAW window */
#define PCIEX3_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX3_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX3_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX3_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX3_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX3_MEM_ADRS             (PCIEX3_LAW_BASE)
#define PCIEX3_MEMIO_ADRS           (PCIEX3_MEM_ADRS   + PCIEX3_MEM_SIZE)
#define PCIEX3_IO_ADRS              (PCIEX3_MEMIO_ADRS + PCIEX3_MEMIO_SIZE)
#define PCIEX3_IO32_ADRS            (PCIEX3_IO_ADRS    + PCIEX3_IO_SIZE)

#define PCIEX4_LAW_BASE             0xD0000000       /* PCI LAW window */
#define PCIEX4_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX4_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX4_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX4_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX4_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX4_MEM_ADRS             (PCIEX4_LAW_BASE)
#define PCIEX4_MEMIO_ADRS           (PCIEX4_MEM_ADRS   + PCIEX4_MEM_SIZE)
#define PCIEX4_IO_ADRS              (PCIEX4_MEMIO_ADRS + PCIEX4_MEMIO_SIZE)
#define PCIEX4_IO32_ADRS            (PCIEX4_IO_ADRS    + PCIEX4_IO_SIZE)

#define PCI_MMU_TLB_SZ              _MMU_TLB_SZ_256M

/* RapidIO memory map address */

#define FSL_RIO_MEM_ADRS            0xc0000000
#define FSL_RIO_MEM_SIZE            0x10000000

#define FSL_RIO1_MEM_ADRS           (FSL_RIO_MEM_ADRS)
#define FSL_RIO1_MEM_SIZE           0x8000000          /* 128M */
#define FSL_RIO1_LAW_SIZE           (LAWAR_SIZE_128MB)

#define FSL_RIO2_MEM_ADRS           (FSL_RIO_MEM_ADRS + FSL_RIO1_MEM_SIZE)
#define FSL_RIO2_MEM_SIZE           0x8000000          /* 128M */
#define FSL_RIO2_LAW_SIZE           (LAWAR_SIZE_128MB)

/* latency timer value, 64 PCI clocks */

#define PCI_LAT_TIMER               0x40

#define NUM_PCIEX1_SLOTS            0x4
#define NUM_PCIEX2_SLOTS            0x4
#define NUM_PCIEX3_SLOTS            0x4
#define NUM_PCIEX4_SLOTS            0x4


#define I2C_BUS1_CHAN0              0x80
#define I2C_BUS1_CHAN1              0x90
#define I2C_BUS1_CHAN2              0xa0
#define I2C_BUS1_CHAN3              0xb0
#define I2C_BUS1_CHAN4              0xc0
#define I2C_BUS1_CHAN5              0xd0
#define I2C_BUS1_CHAN6              0xe0
#define I2C_BUS1_CHAN7              0xf0
#define I2C_BUS1_CHAN_ALL_OFF       0x00

#define I2C_BUS1_CHAN70             0xf8
#define I2C_BUS1_CHAN71             0xf9
#define I2C_BUS1_CHAN72             0xfa
#define I2C_BUS1_CHAN73             0xfb
#define I2C_BUS1_CHAN74             0xfc
#define I2C_BUS1_CHAN75             0xfd
#define I2C_BUS1_CHAN76             0xfe
#define I2C_BUS1_CHAN77             0xff
#define I2C_BUS1_CHAN7_ALL_OFF      0xf0

#define I2C_BUS1_CHAN_MASK          0x0f
#define I2C_BUS1_SW1_MASK           0xf0
#define I2C_BUS1_SW2_MASK           0x0f

/*
 * These defines supports secondary images on a specific boundary.  Only map
 * 256MB to allow loading at any 256MB boundary.  The runtime kernel can later
 * expand the range when sysStaticTlbDesc becomes active in sysHwInit().
 * At the time of this writing the SMP image for the T4QDS BSP limits this to
 * a 256M boundary.  The T4 BSPs worked fine with a 64MB boundary.
 */

#define IMAGE_MASK                  0xF000          /* 0xF000 - 256MB,
                                                       0xFC00 - 64MB, etc. */
#define IMAGE_BOUNDARY              _MMU_TLB_SZ_256M

#define MMU_RAM_ACCESS              MMU_CACHED  /* see romInit.s */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCt4240qdsh */
