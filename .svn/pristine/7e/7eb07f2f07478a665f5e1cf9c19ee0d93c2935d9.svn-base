/* fsl_rdb8323/config.h - Freescale RDB8323 board configuration header */

/*
 * Copyright (c) 2006-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,15oct10,kab  Rename BSP from rdb8323
01o,15apr10,clx  Update BSP version for vxWorks 6.9
01n,15oct09,l_z  Remove optional timestamp support definition. (WIND00183633)
01m,15sep09,l_z  Remove unused INCLUDE_PPC_FPU. (WIND00153695)
01l,09jul09,e_d  remove INCLUDE_I2C by default and update BSP_REV number.
01k,26jun09,e_d  change timer and interrupt legacy driver to vxBus driver
01j,18dec08,z_l  Increased rev of BSP.
01i,11nov08,d_l  correct USB and GEI macros. (WIND00142426)
                 code cleanup and update BSP_REV number.
01h,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01g,17mar08,wap  Switch from hEnd qeFcc driver to VxBus UCC driver, add ICplus
                 IC175C PHY driver
01f,20sep07,pmr  remove non-default component includes
01e,21aug07,mmi  remove legacy power mangement define
01d,12jul07,vik  added macro INCLUDE_VXB_CMDLINE
01c,23mar07,pmr  fix bootApp support
01b,01feb07,pmr  default configuration
01a,19dec06,pmr  adapted from mds8323/config.h
*/

/*
This file contains the configuration parameters for the
Motorola MPC8323 RDB board.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * BSP version/revision identification, should be placed
 * before #include "configAll.h"
 */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/0"        /* 0 for first revision */
#define BSP_NAME           fsl_rdb8323

/* Define Clock Speed and source  */

#define FREQ_33_MHZ             33000000
#define FREQ_40_MHZ             40000000
#define FREQ_66_MHZ             66000000
#define FREQ_100_MHZ            100000000
#define FREQ_133_MHZ            133000000
#define FREQ_266_MHZ            266000000
#define FREQ_333_MHZ            333000000

/*
 * This define must be set to the value of the resonant oscillator
 * inserted in position U16 or the PCI freq of the MPC8323RDB board.
 * Choose from above list. BSP assumes standalone board ie not in
 * PCI chassis
 */

#define OSCILLATOR_FREQ         FREQ_66_MHZ
#define SYS_CLK_FREQ            FREQ_133_MHZ
#define DEC_CLK_TO_INC          4            /* # bus clks per increment*/
#define DEC_CLOCK_FREQ          SYS_CLK_FREQ /* Set to system default */
#define TIMESTAMP_HZ            SYS_CLK_FREQ
#define FORCE_DEFAULT_CLK
#define TPR                     0x2000
#define LSRT_VALUE              0x3F

#include "configAll.h"

#undef  INCLUDE_HW_FP
#define INCLUDE_SW_FP
#define INCLUDE_FLOATING_POINT
#define INCLUDE_FORMATTED_IO

#define DEFAULT_BOOT_LINE \
"qefcc(0,0)host:target/config/fsl_rdb8323/vxWorks h=92.0.0.1 e=92.0.0.2 u=vxworks pw=vxworks tn=rdb8323"

#define INCLUDE_MMU_BASIC

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_D_CACHE_ENABLE

/*
 * Does nothing about copyback vs writethrough in h/w,
 * must use sysPhysMemDesc
 */

#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE  CACHE_COPYBACK | CACHE_SNOOP_ENABLE
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE  CACHE_COPYBACK
#endif /* INCLUDE_CACHE_SUPPORT */

/* Number of TTY definition */

#undef  NUM_TTY
#define NUM_TTY     N_SIO_CHANNELS

#define INCLUDE_VXBUS

#ifdef INCLUDE_VXBUS
#   define INCLUDE_INTCTLR_LIB
#   define DRV_INTCTLR_PPC
#   define DRV_INTCTLR_QE
#   define DRV_INTCTLR_QUICC
#   define INCLUDE_HWMEM_ALLOC
#   define INCLUDE_PARAM_SYS
#   define INCLUDE_PLB_BUS
#   define INCLUDE_QUICC_ENGINE_UTILS
#   define INCLUDE_VXB_CMDLINE
#   define INCLUDE_UCC_VXB_END
#   define INCLUDE_MII_BUS
#   define INCLUDE_UCC_MDIO
#   define INCLUDE_GENERICPHY
#   define INCLUDE_ICP175CPHY
#   define DRV_SIO_NS16550
#   define INCLUDE_SIO_UTILS
#   define HWMEM_POOL_SIZE 50000
#   define DRV_TIMER_DEC_PPC
#   ifdef INCLUDE_AUX_CLK
#       define DRV_TIMER_QUICC_PPC
#   endif
#   define INCLUDE_TIMER_SYS
#   define INCLUDE_DMA_SYS
#endif /* INCLUDE_VXBUS */

/* clock rates */

#define SYS_CLK_RATE_MIN    1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX    8000    /* maximum auxiliary clock rate */

/* add on-chip drivers */

#undef INCLUDE_SECURITY_ENGINE /* only supported if chip has E ie 8323E */

/* optional USB support */

#undef INCLUDE_USB

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_OHCI
#   define INCLUDE_OHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_FS_EVENT_UTIL    /* File system event utility */
#   define INCLUDE_FS_MONITOR       /* File system monitor */
#   define INCLUDE_ERF              /* Event reporting framework */
#   define INCLUDE_DEVICE_MANAGER   /* Device Manager */
#   define INCLUDE_XBD              /* Extended block device */
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_XBD_BLK_DEV      /* XBD-block device wrapper */
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_CHKDSK     /* file system integrity checking */
#   define INCLUDE_DOSFS_CACHE
#endif /* INCLUDE_USB */

#ifdef  INCLUDE_PCI_BUS

#define INCLUDE_PCI_BUS_AUTOCONF
#define DRV_PCIBUS_M83XX

/*
 * CPU Addr                                    PCI Addr
 * PCI_LOCAL_MEM_BUS   ------------------------- PCI_MSTR_MEM_BUS
 *                     -                       -
 *                     -                       -
 * PCI_LOCAL_MEM_BUS + ------------------------- PCI_MSTR_MEM_BUS +
 * PCI_LOCAL_MEM_SIZE  ----IMMR                - PCI_MSTR_MEM_SIZE
 *                     -                       -
 *                     -                       -----PIMMR
 *                     -                       -
 *                     -                       -
 * CPU_PCI_MEM_ADRS    ------------------------- PCI_MEM_ADRS
 *                     -                       -
 *                     -                       -
 * CPU_PCI_MEMIO_ADRS  ------------------------- PCI_MEMIO_ADRS
 *                     -                       -
 *                     -                       -
 * CPU_PCI_IO_ADRS     ------------------------- PCI_IO_ADRS
 *                     -                       -
 *                     -                       -
 * CPU_PCI_IO_ADRS +   ------------------------- PCI_IO_ADRS +
 * CPU_PCI_IO_SIZE     -                       - PCI_IO_SIZE
 *                     -                       -
 *                     -                       -
 *                     -                       -
 *                     -------------------------- 4GBytes
 */

/* for custom sysPciAutoConfig.c */

#define PCI1_LAW_BASE       0x80000000      /* PCI LAW window */
#define PCI1_LAW_SIZE       0x10000000      /* PCI LAW size   */

/* PCI based addresses */

#define PCI_MEM_ADRS        0x80000000
#define PCI_MEM_SIZE        0x08000000      /*16MB */
#define PCI_MEM_SIZE_MASK   PCI_SIZE_128MB  /* This should match PCI_MEM_SIZE */
#define PCI_MEMIO_ADRS      0x88000000
#define PCI_MEMIO_SIZE      0x04000000      /*16MB */
#define PCI_MEMIO_SIZE_MASK PCI_SIZE_64MB   /* This should match PCI_MEMIO_SIZE */

#define PCI_IO_ADRS         0x8c000000
#define PCI_IO_SIZE         0x04000000      /* IO Space is not available */
#define PCI_IO_SIZE_MASK    PCI_SIZE_64MB

/* CPU based addresses */

#define CPU_PCI_MEM_ADRS    0x80000000
#define CPU_PCI_MEM_SIZE    PCI_MEM_SIZE
#define CPU_PCI_MEMIO_ADRS  0x88000000
#define CPU_PCI_MEMIO_SIZE  PCI_MEMIO_SIZE
#define CPU_PCI_IO_ADRS     0x8c000000
#define CPU_PCI_IO_SIZE     PCI_IO_SIZE

/* CPU from PCI bus */

#define PCI_MSTR_MEM_BUS    0x00000000
#define PCI_MSTR_MEM_SIZE   PCI_LOCAL_MEM_SIZE
#define PCI_BRIDGE_PIMMR_BASE_ADRS  0x40000000

/* CPU Address that is visible from PCI */

#define PCI_LOCAL_MEM_BUS       LOCAL_MEM_LOCAL_ADRS
#define PCI_LOCAL_MEM_SIZE      LOCAL_MEM_SIZE

/* This should at least match size of LOCAL_MEM_SIZE */

#define PCI_LOCAL_MEM_SIZE_MASK PCI_SIZE_256MB

#endif /* INCLUDE_PCI */

/* add necessary drivers */

#ifdef INCLUDE_UCC_VXB_END
#   define ETHERNET_MAC_HANDLER
#   ifndef INCLUDE_END
#       define INCLUDE_END      /* only END-style driver for FCC */
#   endif /* INCLUDE_END */
#endif /* INCLUDE_UCC_VXB_END */

#define MAX_MAC_DEVS    2       /* two network devices (fcc, scc) */

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

/*
 * Both FCC MAC address and bootline is stored in top of flash
 * If you want to move bootimage here must move elsewhere in flash
 * Requires 1 segment
 */

#define INCLUDE_FLASH

#define FLASH_BASE_ADRS         0xfe000000

#ifdef INCLUDE_FLASH
#   define SYS_FLASH_TYPE        FLASH_28F128J3A    /* flash device type */
#   define FLASH_WIDTH           2
#   define FLASH_CHIP_WIDTH      1
#   define FLASH_WIDTH_SPECIAL_2
#   define FLASH_ADRS            (FLASH_BASE_ADRS + 0xfe0000)
#   define FLASH_SIZE            0x20000
#   define NV_RAM_SIZE           FLASH_SIZE
#   define SYS_FLASH_WRITE
#   undef  FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET        0
#endif /* INCLUDE_FLASH */

#undef INCLUDE_I2C

/* Memory addresses */

#define LOCAL_MEM_SIZE          0x4000000      /* 64 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000     /* Base of RAM */
#undef  INCLUDE_LBC_SDRAM
#define INCLUDE_DDR_SDRAM
#define INCLUDE_SECONDARY_DRAM

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_SIZE2             0x4000000   /* 64 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS2       0x08000000  /* Base of RAM */
#define LBC_SDRAM_LOCAL_SIZE_MASK   0xfc000000
#define LBC_SDRAM_LOCAL_ADRS        LOCAL_MEM_LOCAL_ADRS2
#define LBC_SDRAM_LOCAL_SIZE        LOCAL_MEM_SIZE2
#define DDR_SDRAM_LOCAL_ADRS        LOCAL_MEM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_SIZE        LOCAL_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS_END    (DDR_SDRAM_LOCAL_ADRS + DDR_SDRAM_LOCAL_SIZE)

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
 * in config.h, MakeSkel, Makefile, and Makefile.
 * All definitions for these constants must be identical.
 */

#define ROM_BASE_ADRS           FLASH_BASE_ADRS     /* base address of ROM */
#define ROM_TEXT_ADRS           (ROM_BASE_ADRS + 0x100)
#define ROM_SIZE                0x100000            /* ROM space */
#define ROM_WARM_ADRS           (ROM_TEXT_ADRS+8)   /* warm reboot entry */

#define USER_RESERVED_MEM       0x00000000     /* user reserved memory size */

/* Hard Reset Configuration Words */

/*
 * See MPC8323E hardware specification for supported clock freq
 * More information in target.ref
 */

/* spmf 1:2 ie 2*66Mhz == 133Mhz CSB*/

#define HRCW_LOW_BYTE0  0x42

/* corepll ratio 1:2.5 333Mhz  */

#define HRCW_LOW_BYTE1  0x05
#define HRCW_LOW_BYTE2  0x00
#define HRCW_LOW_BYTE3  0x83    /* QuiccEngine 133 * 3 == 400MHz */

#define HRCW_HIGH_BYTE0 0xA0    /* PCI HOST/ARB/CLK_OUT */
#define HRCW_HIGH_BYTE1 0x60
#define HRCW_HIGH_BYTE2 0x00
#define HRCW_HIGH_BYTE3 0x00

#include "rdb8323.h"            /* include the rdb8323 params */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCconfigh */
#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif
