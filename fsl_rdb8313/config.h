/* fsl_rdb8313/config.h - Freescale MPC8313ERDB board configuration header */

/*
 * Copyright (c) 2007-2010, 2012-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,10jun13,y_y  add GTM 64 bit timer support. (WIND00392055)
01k,19jul12,clx  Update BSP_REV
01j,15apr10,clx  Update BSP version for vxWorks 6.9
01i,09sep09,s_z  Integrate the USB TCD support by INCLUDE_FSL_TCD(WIND00180575)
01h,18dec08,z_l  Increased rev of BSP. (WIND00139815)
01g,20aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01f,24jul08,l_g  updated BSP_REV to "/6" for vxWorks 6.7.
01e,04jul08,l_g  update for bsp 2.0/5
01d,03mar08,z_l  updated to vxWorks6.6
01c,23jan08,l_g  update for bsp 2.0/2
01b,18sep07,l_g  add support of REVA3 board and nand boot
01a,26jun07,l_g  initial creation, based on ads834x
*/

/*
DESCRIPTION
This file contains the configuration parameters for the
Freescale MPC8313ERDB board.
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
#define BSP_REV            "/2"        /* 0 for first revision */

/* Define for backward compatability */

#define BSP_VER_1_1      1
#define BSP_VER_1_2      1

#define BOOT_FROM_UBOOT
#undef BOOT_FROM_NAND

/* Define Clock Speed and source */

#define FREQ_33_MHZ             33000000
#define FREQ_66_MHZ             66000000
#define FREQ_133_MHZ            133000000
#define FREQ_166_MHZ            166000000
#define FREQ_266_MHZ            266000000

#define REVA3_BOARD  /* support REVA3 and REVA4 board */

/* The RevA3 board uses higher syctem clock frequency */

#ifdef  REVA3_BOARD
#   define SYS_CLK_FREQ         FREQ_166_MHZ
#   define OSCILLATOR_FREQ      FREQ_33_MHZ
#else
#   define SYS_CLK_FREQ         FREQ_133_MHZ
#   define OSCILLATOR_FREQ      FREQ_66_MHZ
#endif
#define TIMESTAMPPERIOD         (0xffffffff / SYS_CLK_FREQ)
#define DEC_CLK_TO_INC          4            /* # bus clks per increment */
#define DEC_CLOCK_FREQ          SYS_CLK_FREQ /* Set to system default */
#define TIMESTAMP_HZ            SYS_CLK_FREQ

#include "configAll.h"
#include "fsl_rdb8313.h"        /* include the fsl_rdb8313 params */

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE    115200

/* Define CPU */

#define MPC8313

#define DEFAULT_BOOT_LINE \
"motetsec(1,0)host:vxWorks.st h=92.0.0.1 e=92.0.0.2 u=m8313 pw=zzz tn=8313"

/* Define for MMU */

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
#   define USER_D_CACHE_MODE    (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE    CACHE_COPYBACK
#endif /* INCLUDE_CACHE_SUPPORT */

/* Number of TTY definition */

#undef    NUM_TTY
#define   NUM_TTY          N_SIO_CHANNELS    /* defined in fsl_rdb8313.h */

/* clock rates */

#define SYS_CLK_RATE_MIN    1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX    8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN    1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX    8000    /* maximum auxiliary clock rate */

#define INCLUDE_VXBUS

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_QUICC
#define INCLUDE_VXBUS_INIT
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_VXBUS_ACTIVATE
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS
#define INCLUDE_ETSEC_VXB_END 
#define HWMEM_POOL_SIZE       50000
#define DRV_TIMER_DEC_PPC
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_QUICC_PPC
#undef  DRV_TIMER64_QUICC_PPC

#if defined(DRV_TIMER_QUICC_PPC) && defined(DRV_TIMER64_QUICC_PPC)
#error "Cannot choose both of 32bit and 64bit timer because they shared the hardware"
#endif	/* DRV_TIMER_QUICC_PPC && DRV_TIMER64_QUICC_PPC */

#define INCLUDE_MII_BUS
#define INCLUDE_MV88E1X11PHY
#define INCLUDE_GENERICPHY

#define INCLUDE_DUMMY_MDIO
#define INCLUDE_TSEC_MDIO

#define INCLUDE_PCI

#ifdef INCLUDE_PCI
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_M83XX
#   undef  INCLUDE_FEI8255X_VXB_END
#endif

#define INCLUDE_VXB_CMDLINE

/* add on-chip drivers */

#define INCLUDE_SECURITY_ENGINE /* only supported if chip has E ie 8313E */

/* Optional timestamp support */

#define INCLUDE_TIMESTAMP
#define INCLUDE_AUX_CLK

/*  Define for RTC  */

#define INCLUDE_RTC

#ifdef  INCLUDE_PCI
#   define INCLUDE_PCI_AUTOCONF

#   define PCI1_LAW_BASE        0x80000000      /* PCI LAW window */
#   define PCI1_LAW_SIZE        0x10000000      /* PCI LAW size   */

#   define PCI2_LAW_BASE        0x90000000      /* PCI2 LAW window */
#   define PCI2_LAW_SIZE        0x10000000      /* PCI2 LAW size   */

/* PCI based addresses */

#   define PCI_MEM_ADRS         0x80000000
#   define PCI_MEM_SIZE         0x08000000      /* 128MB */

/* This should match PCI_MEM_SIZE */

#   define PCI_MEM_SIZE_MASK    PCI_SIZE_128MB
#   define PCI_MEMIO_ADRS       0x88000000
#   define PCI_MEMIO_SIZE       0x04000000      /* 64MB */

/* This should match PCI_MEMIO_SIZE */

#   define PCI_MEMIO_SIZE_MASK  PCI_SIZE_64MB

#   define PCI_IO_ADRS          0x8c000000
#   define PCI_IO_SIZE          0x04000000      /* IO Space is not available */
#   define PCI_IO_SIZE_MASK     PCI_SIZE_64MB

/* CPU based addresses */

#   define CPU_PCI_MEM_ADRS     0x80000000
#   define CPU_PCI_MEM_SIZE     PCI_MEM_SIZE
#   define CPU_PCI_MEMIO_ADRS   0x88000000
#   define CPU_PCI_MEMIO_SIZE   PCI_MEMIO_SIZE
#   define CPU_PCI_IO_ADRS      0x8c000000
#   define CPU_PCI_IO_SIZE      PCI_IO_SIZE

/* CPU from PCI bus */

#   define PCI_MSTR_MEM_BUS             0x00000000
#   define PCI_MSTR_MEM_SIZE            PCI_LOCAL_MEM_SIZE
#   define PCI_BRIDGE_PIMMR_BASE_ADRS   0x40000000

/* CPU Address that is visible from PCI */

#   define PCI_LOCAL_MEM_BUS            LOCAL_MEM_LOCAL_ADRS
#   define PCI_LOCAL_MEM_SIZE           LOCAL_MEM_SIZE

/* This should at least match size of LOCAL_MEM_SIZE */

#   define PCI_LOCAL_MEM_SIZE_MASK PCI_SIZE_256MB

#   ifndef PCI_CFG_TYPE
#       ifdef INCLUDE_PCI_AUTOCONF
#           define PCI_CFG_TYPE PCI_CFG_AUTO
#       else  /* INCLUDE_PCI_AUTOCONF */
#           define PCI_CFG_TYPE PCI_CFG_FORCE
#       endif /* INCLUDE_PCI_AUTOCONF */
#    endif /* PCI_CFG_TYPE */

#endif /* INCLUDE_PCI */

#define ETHERNET_MAC_HANDLER
#define MAX_MAC_DEVS         2          /* two network devices (fcc, scc) */

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

/* Define the support of Flash */

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH
#   define SYS_FLASH_TYPE           FLASH_EN29LV640
#   define FLASH_WIDTH              2
#   define FLASH_CHIP_WIDTH         2
#   define FLASH_SECTOR_SIZE        0x10000     /* sector size of EN29LV64 */
#   define FLASH_ADRS               FLASH_BASE_ADRS
#   undef  FLASH_NO_OVERLAY
#   undef  FLASH_SIZE_WRITEABLE
#   define SYS_FLASH_DELAY

/* Reserved 8K for VSC7385 firmware */

#   define VSC7385_FIRMWARE_ADRS    (FLASH_BASE_ADRS + 0x1f0000)
#   define NV_RAM_SIZE              (FLASH_SECTOR_SIZE - (8 * 1024))

/* Upper 768K for Bootrom and UBOOT */

#   define NV_RAM_ADRS              (VSC7385_FIRMWARE_ADRS + 8 * 1024 )
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET           0
#   define NV_MAC_ADRS_OFFSET       0x200
#else  /* INCLUDE_FLASH */
#   define NV_RAM_SIZE              NONE
#endif /* INCLUDE_FLASH */

#define FLASH_BASE_ADRS             0xFE000000
#define FLASH_SIZE                  0x800000

/* Memory addresses */

#define LOCAL_MEM_SIZE              0x8000000   /* 128 Mbyte memory available */
#define LOCAL_MEM_LOCAL_ADRS        0x00000000  /* Base of RAM */
#define DDR_SDRAM_LOCAL_ADRS        LOCAL_MEM_LOCAL_ADRS

#define USER_RESERVED_MEM           0x00100000  /* user reserved memory size */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
 * in config.h, MakeSkel, Makefile, and Makefile.*
 * All definitions for these constants must be identical.
 */

#define NAND_PAGE_SIZE          512
#define NAND_BLOCK_SIZE         (16 * 1024)
#define NAND_PAGE_PER_BLOCK     32

#ifdef BOOT_FROM_NAND
#   define NAND_BLOCKCOPY_COUNT 32
#   define NAND_BUFFER_BASE     0xfff00000
#   define NAND_LOADER_ADRS     \
              (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - NAND_BLOCK_SIZE)
#   define NAND_BUFFER_SIZE     0x1000
#   define RAM_IMAGE_ADRS       \
              (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - USER_RESERVED_MEM)
#   define ROM_BASE_ADRS        RAM_IMAGE_ADRS  /* base address of ROM */
#   define NAND_BANK            0
#else  /* BOOT_FROM_NAND */
#   define NAND_BANK            1
#   define NAND_BUFFER_BASE     0xE2800000
#   define ROM_BASE_ADRS        0xfe000000      /* base address of ROM */
#endif /* BOOT_FROM_NAND */

#define NAND_CONTROLLER_BASE    0xE2800000
#define NAND_CONTROLLER_SIZE    0x8000

#ifdef BOOT_FROM_UBOOT
#   define UBOOT_SIZE           0x60000
#   define ROM_TEXT_ADRS        (ROM_BASE_ADRS + 0x100 + UBOOT_SIZE)
#else  /* BOOT_FROM_UBOOT */
#   define ROM_TEXT_ADRS        (ROM_BASE_ADRS + 0x100)
#endif /* BOOT_FROM_UBOOT */

#define ROM_SIZE            0x190000            /* ROM space */
#define ROM_WARM_ADRS       (ROM_TEXT_ADRS + 8) /* warm reboot entry */

/* Hard Reset Configuration Words, more information in target.ref */

#ifdef  REVA3_BOARD

/* 166MHz CSB, 333MHz Core */

#   define HRCW_LOW_BYTE0   0x65
#else /* REVA3_BOARD */

/* 133MHz CSB, 266MHz Core */

#   define HRCW_LOW_BYTE0   0x62
#endif /* REVA3_BOARD */
#define HRCW_LOW_BYTE1      0x04

/* Must be cleared*/

#define HRCW_LOW_BYTE2      0x00

/* Must be cleared*/

#define HRCW_LOW_BYTE3      0x00

#ifndef BOOT_FROM_NAND
#   define HRCW_HIGH_BYTE0  0xa0

/* Rom Location Flash 16 bit. Watch dog disabled */

#   define HRCW_HIGH_BYTE1  0x60
#else /* BOOT_FROM_NAND  */
#   define HRCW_HIGH_BYTE0  0xa4

/* Rom Location Flash 16 bit. Watch dog disabled */

#   define HRCW_HIGH_BYTE1  0x14
#endif /* BOOT_FROM_NAND  */

/* GMII */

#define HRCW_HIGH_BYTE2     0x6c    /* Both TSECs use RGMII */

/* Big Endian */

#define HRCW_HIGH_BYTE3     0x00


#undef INCLUDE_FSL_TCD

#ifdef INCLUDE_FSL_TCD
#   ifndef _ASMLANGUAGE
IMPORT void fslUsbDrDeviceHwInit (void);
#   endif
#   define FSL_USB_DR_DEVICE_IO_BASE      M83XX_DR_USB_BASE
#   define FSL_USB_DR_DEVICE_INT_NUM      INUM_USB2_DR
#   define FSL_USB_DR_DEVICE_HW_INIT      fslUsbDrDeviceHwInit 
#   define FSL_USB_DR_DEVICE_BE_REG       FALSE
#   define FSL_USB_DR_DEVICE_BE_DESC      FALSE
#   define FSL_USB_DR_DEVICE_BE_SETUP     TRUE
#   define FSL_USB_DR_DEVICE_SYS_CONF     TRUE
#   define FSL_USB_DR_DEVICE_BUFFER_ES    FALSE
#   define FSL_USB_DR_DEVICE_PHY_MODE     FSL_USB_DR_PHY_UTMI8
#   define INCLUDE_USB_TARG
#   define INCLUDE_MS_EMULATOR
#   define INCLUDE_MS_EMULATOR_INIT
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_MS_EMULATOR
#   define INCLUDE_DOSFS
#   define INCLUDE_XBD_RAMDRV
#endif /* INCLUDE_MS_EMULATOR */

/* Define for FS */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_ERF
#   define INCLUDE_XBD
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_XBD_PART_LIB
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_XBD_BLKDEV
#endif /* INCLUDE_DOSFS */

/*
 * If the FORCE_DEFAULT_BOOT_LINE is defined then the DEFAULT_BOOT_LINE
 * parameters are always used regardless of NVRAM values specified at
 * bootrom time. This is usually used to debug WR downloaded images
 * without a bootrom present.
 */

#undef  FORCE_DEFAULT_BOOT_LINE

/* Stuff to be excluded if FORCE_DEFAULT_BOOT_LINE defined */

#ifdef  FORCE_DEFAULT_BOOT_LINE
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE  NONE
#endif  /* FORCE_DEFAULT_BOOT_LINE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* __INCconfigh */
#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif

