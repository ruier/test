/* fsl_mpc830x_mitx/config.h - Freescale MPC830x mitx configuration header */

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
01a,30jun11,e_d  derived from version 01b of fsl_mpc8308_rdb/config.h
*/

/*
DESCRIPTION
This file contains the configuration parameters for the
Freescale MPC830x mitx board.
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

#define BSP_VERSION             "6.9"
#define BSP_REV                 "/0"

/* if soc version is MPC8309, please define this macro */

#undef MPC8309_BOARD

/* if boot from nand flash, please define this macro */

#undef BOOT_FROM_NAND

#include "configAll.h"
#include "mpc830xmitx.h"

/* vxBus drivers */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_QUICC
#define HWMEM_POOL_SIZE         50000

#define INCLUDE_TIMER_SYS
#define DRV_TIMER_DEC_PPC

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#define INCLUDE_END
#define INCLUDE_DMA_SYS
#define INCLUDE_MII_BUS
#define INCLUDE_DUMMY_MDIO
#define DRV_INTCTLR_QE
#define INCLUDE_QUICC_ENGINE_UTILS
#define INCLUDE_UCC_VXB_END
#define INCLUDE_UCC_MDIO
#define INCLUDE_GENERICPHY

#ifdef  DRV_PCIBUS_M83XX
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#endif  /* DRV_PCIBUS_M83XX_EX */

#define DRV_DMA_M512X

#define DEFAULT_BOOT_LINE   \
    "motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE    CACHE_COPYBACK

#   define USER_D_CACHE_ENABLE
#   undef  USER_D_CACHE_MODE

/* SNOOP is not supported when the DMAC driver is enabled */

#   ifdef  DRV_DMA_M512X
#       define USER_D_CACHE_MODE    CACHE_COPYBACK
#   else /* DRV_DMA_M512X */
#       define USER_D_CACHE_MODE    (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#   endif  /* DRV_DMA_M512X */

#endif /* INCLUDE_CACHE_SUPPORT */

#undef DRV_RESOURCE_FSLGPIO

/*  Define for I2C  */

#define INCLUDE_I2C

/* Define the support of NorFlash */

#define FLASH_BASE_ADRS             0xFE000000
#define FLASH_SIZE                  0x800000

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH
#   define SYS_FLASH_TYPE           FLASH_EN29LV640
#   define FLASH_WIDTH              2
#   define FLASH_CHIP_WIDTH         2
#   define FLASH_SECTOR_SIZE        0x10000     /* sector size of MX29LV640 */
#   define FLASH_ADRS               FLASH_BASE_ADRS
#   undef  FLASH_NO_OVERLAY
#   undef  FLASH_SIZE_WRITEABLE
#   define SYS_FLASH_DELAY

/* Upper 1M Bytes for bootrom and NV_RAM */

#   define NV_RAM_ADRS              (FLASH_BASE_ADRS + 0x100000)
#   define NV_RAM_SIZE              FLASH_SECTOR_SIZE
#   define NV_MAC_ADRS_OFFSET       0x200

#else  /* INCLUDE_FLASH */
#   define NV_RAM_SIZE              NONE
#endif /* INCLUDE_FLASH */

/* NAND Flash */

#undef  INCLUDE_NAND_FLASH

/* Serial line */

#undef  NUM_TTY
#define NUM_TTY                     2
#undef  CONSOLE_TTY
#ifdef MPC8309_BOARD
#   define CONSOLE_TTY              0 /* 0 = UART1, 1 = UART2 */
#else /* MPC8309_BOARD */
#   define CONSOLE_TTY              1 /* 0 = UART1, 1 = UART2 */
#endif /* MPC8309_BOARD */
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE           9600

/* TrueFFS */

#ifdef INCLUDE_TFFS
#   define INCLUDE_DOSFS
#   define INCLUDE_TL_FTL
#   define INCLUDE_TFFS_MOUNT
#   define TFFS_ADRS_OFFSET         0x200000
#   define S29GL_FLASH_BIT          1
#   define S29GL_FLASH_NUM          1
#   define S29GL_FLASH_WIDTH        2
#   define S29GL_SECTOR_SIZE        0x00010000
#   define S29GL_FLASH_SIZE         FLASH_SIZE - S29GL_SECTOR_SIZE
#endif  /* INCLUDE_TFFS */

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif  /* DRV_STORAGE_SDHC */

/* USB Host */

#ifdef INCLUDE_EHCI
#   define INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI_BUS
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_HCD_BUS
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_DOSFS
#endif /* INCLUDE_EHCI */

/* USB Target */

#ifdef INCLUDE_FSL_TCD
#   ifndef _ASMLANGUAGE
IMPORT void fslUsbDrDeviceHwInit (void);
#   endif
#   define FSL_USB_DR_DEVICE_IO_BASE      USB_DR_BASE
#   define FSL_USB_DR_DEVICE_INT_NUM      INUM_USB_DR
#   define FSL_USB_DR_DEVICE_HW_INIT      fslUsbDrDeviceHwInit
#   define FSL_USB_DR_DEVICE_BE_REG       FALSE
#   define FSL_USB_DR_DEVICE_BE_DESC      FALSE
#   define FSL_USB_DR_DEVICE_BE_SETUP     TRUE
#   define FSL_USB_DR_DEVICE_SYS_CONF     TRUE
#   define FSL_USB_DR_DEVICE_BUFFER_ES    FALSE
#   define FSL_USB_DR_DEVICE_PHY_MODE     FSL_USB_DR_PHY_ULPI
#   define INCLUDE_USB_TARG
#   define INCLUDE_MS_EMULATOR
#   define INCLUDE_MS_EMULATOR_INIT
#endif  /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_MS_EMULATOR
#   define INCLUDE_DOSFS
#   define INCLUDE_XBD_RAMDRV
#endif /* INCLUDE_MS_EMULATOR */

/* Define for FS */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_ERF
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_FS_EVENT_UTIL
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_XBD
#   define INCLUDE_XBD_BLK_DEV
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_MAIN
#endif  /* INCLUDE_DOSFS */

#ifdef BOOT_FROM_NAND
#   define NAND_BUFFER_BASE        0xfff00000

/*
 * Reserve memory for nand boot. By default, this
 * value is set 1M bytes. If image is more than
 * this size. Should increase this value. Note also
 * need increase ROM_SIZE.
 */

#   define BOOT_FROM_NAND_MEMRES   0x100000
#else /* BOOT_FROM_NAND */
#   define NAND_BUFFER_BASE        0xe0600000
#   define BOOT_FROM_NAND_MEMRES   0x0
#endif /* BOOT_FROM_NAND */

/* Hard Reset Configuration Words */

#ifndef MPC8309_BOARD
#   define HRCW_LOW_BYTE0          0x44
#   define HRCW_LOW_BYTE1          0x04
#   define HRCW_LOW_BYTE2          0x00
#   define HRCW_LOW_BYTE3          0x08
#   ifndef BOOT_FROM_NAND
#      define HRCW_HIGH_BYTE0      0x00
#      define HRCW_HIGH_BYTE1      0x60
#   else /* BOOT_FROM_NAND */
#      define HRCW_HIGH_BYTE0      0x04
#      define HRCW_HIGH_BYTE1      0x54
#   endif /* BOOT_FROM_NAND */
#   define HRCW_HIGH_BYTE2         0x00
#   define HRCW_HIGH_BYTE3         0x00
#else /* MPC8309_BOARD */
#   define HRCW_LOW_BYTE0          0x44
#   define HRCW_LOW_BYTE1          0x05
#   define HRCW_LOW_BYTE2          0x00
#   define HRCW_LOW_BYTE3          0x08
#   ifndef BOOT_FROM_NAND
#      define HRCW_HIGH_BYTE0      0xa0
#      define HRCW_HIGH_BYTE1      0x60
#   else /* BOOT_FROM_NAND */
#      define HRCW_HIGH_BYTE0      0xa4
#      define HRCW_HIGH_BYTE1      0x54
#   endif /* BOOT_FROM_NAND */
#   define HRCW_HIGH_BYTE2         0x00
#   define HRCW_HIGH_BYTE3         0x00
#endif /* MPC8309_BOARD */

/* Memory addresses */

#ifndef MPC8309_BOARD
#   define M83XX_SICHL_INIDATA     0x2A815657
#   define M83XX_SICHR_INIDATA     0x00050475
#   define LOCAL_MEM_SIZE          0x08000000 - BOOT_FROM_NAND_MEMRES
#   define NAND_BOOT_RESADR        0x08000000 - BOOT_FROM_NAND_MEMRES
#else /* MPC8309_BOARD */
#   define M83XX_SICHL_INIDATA     0x00280000
#   define M83XX_SICHR_INIDATA     0x00000000
#   define LOCAL_MEM_SIZE          0x10000000 - BOOT_FROM_NAND_MEMRES
#   define NAND_BOOT_RESADR        0x10000000 - BOOT_FROM_NAND_MEMRES
#endif /* MPC8309_BOARD */

#define LOCAL_MEM_LOCAL_ADRS       0x00000000  /* Base of RAM */
#define DDR_SDRAM_LOCAL_ADRS       LOCAL_MEM_LOCAL_ADRS

#define USER_RESERVED_MEM          0x00000000  /* user reserved memory size */

/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
 * in config.h and Makefile.
 * All definitions for these constants must be identical. If ROM image is
 * more 1M size, should increase the ROM_SIZE to right size.
 */

#ifndef BOOT_FROM_NAND
#   define ROM_BASE_ADRS               0xfe000000
#   define ROM_SIZE                    0x100000
#   define ROM_WARM_ADRS               (ROM_TEXT_ADRS + 8)
#   define ROM_TEXT_ADRS               (ROM_BASE_ADRS + 0x100)
#else /* BOOT_FROM_NAND */
#   define ROM_BASE_ADRS               LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
#   define ROM_SIZE                    BOOT_FROM_NAND_MEMRES
#   define ROM_TEXT_ADRS               (ROM_BASE_ADRS + 0x100)
#   define ROM_WARM_ADRS               (ROM_TEXT_ADRS + 8)
#endif /* BOOT_FROM_NAND */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INCconfigh */
#if defined(PRJ_BUILD)
#   include "prjParams.h"
#endif  /* PRJ_BUILD */
