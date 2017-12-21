/* fsl_rdb8315/config.h - Freescale MPC8315ERDB board configuration header */

/*
 * Copyright (c) 2008-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,19jul12,clx Update BSP_REV
01g,30aug11,wyt Update version for release. 
01f,29aug11,wyt Add macro PCIE_MAX_BUS.
01e,11mar10,x_z	Update BSP_REV.
01d,02jan10,x_z Uprev for vxWorks6.8.
01c,07dec09,x_z Uprev for vxWorks6.7 and code clean.
01b,15dec08,z_l Increased version number. (WIND00146982)
01a,05may08,z_l Created.
*/

/*
DESCRIPTION
This file contains the configuration parameters for the
Freescale MPC8315ERDB board.
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
#define BSP_REV                 "/1"

/* Define for backward compatability */

#define BSP_VER_1_1             0
#define BSP_VER_1_2             1

/* Base model string. sysModel adds processor details */

#define SYS_MODEL               "Freescale MPC8315E"
#define SYS_MODEL_8315          "Freescale MPC8315"
#define SYS_MODEL_8315E         "Freescale MPC8315E"

/* define if U-Boot boots us, undef if VxWorks bootrom boots us */

#undef BOOT_FROM_UBOOT

#include "configAll.h"
#include "fsl_rdb8315.h"        /* include the fsl_rdb8315 params */

#define DEFAULT_BOOT_LINE \
    "motetsec(0,0)host:vxWorks.st h=92.0.0.1 e=92.0.0.2 u=m8315 pw=zzz tn=8315"

/* Define for MMU */

#define INCLUDE_MMU_BASIC

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#endif /* INCLUDE_MMU_BASIC */

#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_D_CACHE_ENABLE
#   define USER_I_CACHE_ENABLE

/*
 * Does nothing about copyback vs writethrough in h/w,
 * must use sysPhysMemDesc
 */

#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE    (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)

#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE    CACHE_COPYBACK
#endif /* INCLUDE_CACHE_SUPPORT */

/* vxBus drivers */

/* common */

#define INCLUDE_VXBUS

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_QUICC
#define INCLUDE_VXBUS_INIT
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_VXBUS_ACTIVATE
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_VXB_CMDLINE
#define HWMEM_POOL_SIZE         50000

/* SIO */

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

/* ethernet */

#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_TSEC_MDIO

/* timer */

#define DRV_TIMER_DEC_PPC
#define DRV_TIMER_QUICC_PPC
#define INCLUDE_TIMER_SYS

/* USB */

#ifdef INCLUDE_EHCI
#   define INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI_BUS
#   define INCLUDE_EHCI_INIT

#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT

#   define INCLUDE_DOSFS
#endif /* INCLUDE_EHCI */

/* PCI/PCIe */

#if defined(DRV_PCIBUS_M83XX) || defined(DRV_PCIBUS_M83XX_EX)

#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF

/* PCI/PCIe Inbound Windows (PCI/PCIe based addresses) */

#   define PCI_MSTR_MEM_BUS             0x00000000
#   define PCI_MSTR_MEM_SIZE            LOCAL_MEM_SIZE

#   define PCI_BRIDGE_PIMMR_BASE_ADRS   0x40000000 /* fixed 1MB space */

/* PCI configuration */

#   ifdef DRV_PCIBUS_M83XX

/* PCI Local Access Windows Configuration */

#       define PCI_LAW_BASE             0x80000000
#       define PCI_LAW_SIZE             0x10000000      /* 256MB */

/* PCI based addresses */

#       define PCI_MEM_ADRS             0x80000000
#       define PCI_MEM_SIZE             0x08000000      /* 128MB */

#       define PCI_MEMIO_ADRS           0x88000000
#       define PCI_MEMIO_SIZE           0x04000000      /* 64MB  */

#       define PCI_IO_ADRS              0x8c000000
#       define PCI_IO_SIZE              0x04000000      /* 64MB  */

/* CPU based addresses */

#       define CPU_PCI_MEM_ADRS         0x80000000
#       define CPU_PCI_MEM_SIZE         PCI_MEM_SIZE

#       define CPU_PCI_MEMIO_ADRS       0x88000000
#       define CPU_PCI_MEMIO_SIZE       PCI_MEMIO_SIZE

#       define CPU_PCI_IO_ADRS          0x8c000000
#       define CPU_PCI_IO_SIZE          PCI_IO_SIZE
#   endif /* DRV_PCIBUS_M83XX */

/* PCIe */

#   ifdef DRV_PCIBUS_M83XX_EX

/* PCI Local Access Windows Configuration */

#       define PCIEX_LAW_BASE           0xA0000000
#       define PCIEX_LAW_SIZE           0x20000000  /* 512MB */

/* PCI based addresses */

#       define PCIEX_MEM_ADRS           0xA0000000
#       define PCIEX_MEM_SIZE           0x04000000  /* 64MB  */

#       define PCIEX_MEMIO_ADRS         0xA4000000
#       define PCIEX_MEMIO_SIZE         0x04000000  /* 64MB  */

#       define PCIEX_IO_ADRS            0xB0000000
#       define PCIEX_IO_SIZE            0x10000000

/* CPU based addresses */

/* 
 * Only bit 6-7 are used to map bus number, so 4 buses are supported at most. The
 * Config Window must be adjusted for more buses. (For example, 0xA0000000 -
 * 0xB0000000 for 16 buses at most.)
 */
#       define PCIEX_CFG_BASE           0xAC000000
#       define PCIEX_MAX_BUS            4
#       define CPU_PCIEX_MEM_ADRS       0xA0000000
#       define CPU_PCIEX_MEM_SIZE       PCIEX_MEM_SIZE

#       define CPU_PCIEX_MEMIO_ADRS     0xA4000000
#       define CPU_PCIEX_MEMIO_SIZE     PCIEX_MEMIO_SIZE

#       define CPU_PCIEX_IO_ADRS        0xB0000000
#       define CPU_PCIEX_IO_SIZE        PCIEX_IO_SIZE
#   endif /* DRV_PCIBUS_M83XX_EX */

#endif /* defined(DRV_PCIBUS_M83XX) || defined(DRV_PCIBUS_M83XX_EX) */

/* TFFS and SATA */

#if defined(INCLUDE_TFFS) || defined(DRV_STORAGE_FSLSATA)
#   define INCLUDE_DOSFS
#endif /* defined(INCLUDE_TFFS) | defined(DRV_STORAGE_FSLSATA) */

/* DOSFS */

#ifdef INCLUDE_DOSFS
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_CHKDSK
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_SHOW
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_HRFS
#   define INCLUDE_FS_MONITOR
#   define INCLUDE_FS_EVENT_UTIL
#   define INCLUDE_ERF
#   define INCLUDE_XBD
#   define INCLUDE_XBD_BLKDEV
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_DEVICE_MANAGER
#   define INCLUDE_XBD_BLK_DEV
#   define INCLUDE_XBD_PART_LIB
#   define INCLUDE_DISK_UTIL
#endif /* INCLUDE_DOSFS */

/* Local Access Windows Configuration */

/* Memory addresses */

/* NOTE this should match the LAWAR SIZE in romInit for the chosen SDRAM */

#define LOCAL_MEM_SIZE                  0x8000000 /* 128MB */
#define LOCAL_MEM_LOCAL_ADRS            0
#define USER_RESERVED_MEM               0

/* LBC CS0 - 8MB 16-bit flash */

#define FLASH_BASE_ADRS                 0xFE000000
#define FLASH_SIZE                      0x800000 /* 8MB */

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h and
 * Makefile.
 * All definitions for these constants must be identical.
 */

/* Boot ROM address space. */

#define ROM_BASE_ADRS                   FLASH_BASE_ADRS
#define UBOOT_SIZE                      0x80000 /* 512KB */
#define ROM_CODE_OFFSET                 0x100

#ifdef BOOT_FROM_UBOOT
#   define ROM_TEXT_ADRS                (ROM_BASE_ADRS + ROM_CODE_OFFSET + \
                                         NV_RAM_SIZE + UBOOT_SIZE)
#else
#   define ROM_TEXT_ADRS                (ROM_BASE_ADRS + ROM_CODE_OFFSET)
#endif

#define ROM_SIZE                        0x100000 /* 1MB */
#define ROM_WARM_ADRS                   (ROM_TEXT_ADRS + 8)

/*
 * NVRAM configuration
 *
 * This FLASH - S29GL064A is compatible with S29GL256N except sector size.
 */

#define FLASH_ADRS                      FLASH_BASE_ADRS
#define FLASH_SECTOR_SIZE               0x10000
#define SYS_FLASH_TYPE                  FLASH_29GL256N
#define FLASH_WIDTH                     2
#define FLASH_CHIP_WIDTH                2

#undef  FLASH_NO_OVERLAY

#define NV_RAM_SIZE                     FLASH_SECTOR_SIZE
#define NV_RAM_ADRS                     (FLASH_BASE_ADRS + \
                                         S29GL_RESERVED_SIZE - NV_RAM_SIZE)
#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET                  0
#define NV_MAC_ADRS_OFFSET              0x200

/* TFFS configuration */

/* boot sector models: 127 64KB sectors and 8 8KB top sectors */

#define S29GL_FLASH_SIZE                (FLASH_SIZE - FLASH_SECTOR_SIZE)
#define S29GL_FLASH_WIDTH               2
#define S29GL_FLASH_BIT                 1
#define S29GL_FLASH_NUM                 1
#define S29GL_SECTOR_SIZE               FLASH_SECTOR_SIZE

/* space to reserve for a boot-image at the start of the medium */

#define S29GL_RESERVED_SIZE             (FLASH_SIZE / 2)

/*
 * S29GL_FLASH_OP_DELAY must be defined for MPC8315RDB board, it is used in
 * sysMtd.c
 */

#define S29GL_FLASH_OP_DELAY

/* serial line */

#undef  NUM_TTY
#define NUM_TTY                         N_SIO_CHANNELS
#undef  CONSOLE_TTY
#define CONSOLE_TTY                     0 /* 0 = UART1, 1 = UART2 */
#undef  CONSOLE_BAUD_RATE

#ifdef BOOT_FROM_UBOOT
#   define CONSOLE_BAUD_RATE            115200
#else
#   define CONSOLE_BAUD_RATE            9600
#endif /* BOOT_FROM_UBOOT */

/* clock rates */

#define SYS_CLK_RATE_MIN                1    /* minimum system clock rate    */
#define SYS_CLK_RATE_MAX                8000 /* maximum system clock rate    */
#define AUX_CLK_RATE_MIN                1    /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX                8000 /* maximum auxiliary clock rate */

/* default mac address */

#define WR_ENET0                        0x00
#define WR_ENET1                        0xA0
#define WR_ENET2                        0x1E

#define CUST_ENET3_0                    0xA0
#define CUST_ENET3_1                    0xA1
#define CUST_ENET4                      0xAA
#define CUST_ENET5                      0xA0

#define ENET_DEFAULT0                   WR_ENET0
#define ENET_DEFAULT1                   WR_ENET1
#define ENET_DEFAULT2                   WR_ENET2

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER

#define MAX_MAC_DEVS                    1 /* one network devices */
#define MAX_MAC_ADRS                    2
#define MAC_ADRS_LEN                    6

/* misc */

#undef  INCLUDE_RTC

/*
 * Hard Reset Configuration Words, these definations are meaningless when we
 * boot from U-Boot.
 */

#define HRCW_LOW_BYTE0                  0x42
#define HRCW_LOW_BYTE1                  0x06
#define HRCW_LOW_BYTE2                  0x00
#define HRCW_LOW_BYTE3                  0x00
#define HRCW_HIGH_BYTE0                 0xa0
#define HRCW_HIGH_BYTE1                 0x60
#define HRCW_HIGH_BYTE2                 0x6c
#define HRCW_HIGH_BYTE3                 0x00

/*
 * If the FORCE_DEFAULT_BOOT_LINE is defined then the DEFAULT_BOOT_LINE
 * parameters are always used regardless of NVRAM values specified at
 * bootrom time. This is usually used to debug WR downloaded images
 * without a bootrom present.
 */

#undef FORCE_DEFAULT_BOOT_LINE

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

