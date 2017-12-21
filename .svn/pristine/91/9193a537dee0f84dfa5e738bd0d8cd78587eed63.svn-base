/* config.h - Freescale P1010RDB BSP configuration file */ 

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
01e,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01d,19jul12,clx  update BSP_REV
01c,05mar12,y_y  add vxBus I2C support.
01b,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01a,22oct11,fao  derived from version 01b of fsl_p1020_rdb/config.h
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION             "6.9"
#define BSP_REV                 "/1"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif  /* _ASMLANGUAGE */

#include "p1010rdb.h"

/* Common Includes for VXBUS PCI and ETSEC */ 

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS

#define HWMEM_POOL_SIZE         100000

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC

#define INCLUDE_TIMER_SYS
#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_EARLY_VXB_DELAY
#undef  DRV_TIMER_OPENPIC
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

#ifdef DRV_STORAGE_FSLSATA
#   define INCLUDE_DOSFS
#endif /* DRV_STORAGE_FSLSATA */

#ifdef  INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_QORIQ_PCIEX
#endif  /* INCLUDE_PCI_BUS */

#define INCLUDE_END
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_DMA_SYS
#define INCLUDE_MII_BUS
#define INCLUDE_TSEC_MDIO
#define INCLUDE_VSC82XXPHY
#define INCLUDE_GENERICPHY

#define MAX_MAC_DEVS            3
#define ENET_DEFAULT0           WR_ENET0
#define ENET_DEFAULT1           WR_ENET1
#define ENET_DEFAULT2           WR_ENET2

#undef  DRV_DMA_FSL

/* Spansion Spi flash support */
         
#ifdef DRV_SPI_S25FLXX
#   define INCLUDE_FSL_SPI
#endif

#define INCLUDE_GPIO

#define WDT_RATE_MIN            (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX            (sysTimerClkFreq / (1 << 21))

#define DEFAULT_BOOT_LINE \
    "motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE   (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE  _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE       (VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY)

#define INCLUDE_CACHE_SUPPORT

#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_LIBRARY_MODE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE       (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE

#define INCLUDE_BRANCH_PREDICTION

#if defined(INCLUDE_L2_CACHE)
#   define L2_CACHE_SIZE        1
#elif defined(INCLUDE_L2_CACHE)
#   define L2_CACHE_SIZE        2
#else
#   define L2_CACHE_SIZE        0
#endif  /* INCLUDE_L2_CACHE */

/* Support for SPE 64bit registers */

#define INCLUDE_SPE

/* add on-chip drivers */

/* Use openPicTimer as the auxClk */

#ifdef  INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif  /* INCLUDE_AUX_CLK */

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS         0xee000000
#define FLASH_SIZE              0x02000000
#define HALF_FLASH_SIZE         (FLASH_SIZE / 2)
#define ROM_SIZE                0x00100000

/*
 * bootline storage and MAC storage in flash
 */

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH

/* NVRam */

#   define FLASH_ADRS               FLASH_BASE_ADRS
#   define NV_RAM_ADRS              (FLASH_BASE_ADRS)
#   define NV_RAM_SIZE              0x00100000
#   define NV_MAC_ADRS_OFFSET       0x200
#   define FLASH_SECTOR_SIZE        0x20000 
#   define FLASH_SECTOR_MASK        0xfffe0000
#   define FLASH_OVERLAY
#else  /* INCLUDE_FLASH */
#   define NV_RAM_SIZE              0
#endif  /* INCLUDE_FLASH */

/* TrueFFS */

#ifdef INCLUDE_TFFS

/* TFFS_BASE_ADRS is the base address used by TFFS */

#   define TFFS_BASE_ADRS       (FLASH_BASE_ADRS + 0x100000)

/* TFFS_SIZE is the flash size used by TFFS. This definition can be modified */

#   define TFFS_SIZE            (0x800000)
#   define INCLUDE_DOSFS
#   define INCLUDE_TL_FTL
#   define INCLUDE_TFFS_MOUNT
#endif  /* INCLUDE_TFFS */

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#   define INCLUDE_I2C_BUS
#   undef  INCLUDE_USB1
#   undef  INCLUDE_FLASH
#endif  /* DRV_STORAGE_SDHC */

/* USB Host */

#ifdef INCLUDE_USB1
#   define INCLUDE_I2C_BUS
#   define INCLUDE_USB
#   undef DRV_STORAGE_SDHC
#endif  /* INCLUDE_USB1 */

#ifdef INCLUDE_USB2
#   define INCLUDE_USB
#endif  /* INCLUDE_USB2 */

#ifdef INCLUDE_FSL_TCD
#   undef INCLUDE_USB
#endif  /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_USB_MS_CBI
#   define INCLUDE_USB_MS_CBI_INIT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_USB */

/* USB Target */

#ifdef INCLUDE_FSL_TCD
#   define FSL_USB_DR_DEVICE_IO_BASE      USB_BASE
#   define FSL_USB_DR_DEVICE_INT_NUM      EPIC_USB_INT_VEC
#   define FSL_USB_DR_DEVICE_BE_REG       FALSE
#   define FSL_USB_DR_DEVICE_BE_DESC      FALSE
#   define FSL_USB_DR_DEVICE_BE_SETUP     TRUE
#   define FSL_USB_DR_DEVICE_SYS_CONF     TRUE
#   define FSL_USB_DR_DEVICE_BUFFER_ES    FALSE
#   define INCLUDE_USB_TARG
#   define INCLUDE_MS_EMULATOR
#   define INCLUDE_MS_EMULATOR_INIT
#   ifdef INCLUDE_USB1
#      define FSL_USB_DR_DEVICE_PHY_MODE     FSL_USB_DR_PHY_ULPI
#   else
#      define FSL_USB_DR_DEVICE_PHY_MODE     FSL_USB_DR_PHY_UTMI8
#   endif /* INCLUDE_USB1 */
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

/* vxBus I2C support */

#ifdef INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_GENERIC_DEV    /* GPIO */
#   define DRV_I2C_RTC
#   define INCLUDE_TIMER_RTC
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/*
 * The following parameters are to configure initial memory usage for
 * page table and region tables and in event of running out the increment
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks).
 */

#   define AIM_MMU_INIT_PT_NUM  0x40
#   define AIM_MMU_INIT_PT_INCR 0x20
#   define AIM_MMU_INIT_RT_NUM  0x10
#   define AIM_MMU_INIT_RT_INCR 0x10

/* This write protects the page tables */

#   define INCLUDE_AIM_MMU_PT_PROTECTION

/*
 * INCLUDE_LOCK_TEXT_SECTION Requires use of CAM entries which can
 * possibly be already used in sysStaticTlbDesc. Also the larger the
 * page size that can be used the less CAM entries required so aligning
 * RAM_LOW_ADRS on a large page size boundary is usually necessary.
 * Locking the text section should increase performance because no
 * Instruction TLB misses are taken within the text section.
 */

#   define INCLUDE_LOCK_TEXT_SECTION
#   undef INCLUDE_PAGE_SIZE_OPTIMIZATION /* Not available for 85XX */

#endif /* INCLUDE_MMU_BASIC */

#define INCLUDE_CTORS_DTORS

#undef INCLUDE_HW_FP

#if ((TOOL==e500v2diab) || (TOOL==e500v2gnu))

/* e500v2diab is hard float so undefine soft float */

#   undef INCLUDE_SW_FP

#else  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed
 * no optimised libraries are available. The kernel doesn't execute any hw
 * fp instructions
 */

#   define INCLUDE_SW_FP

#endif  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

#define ROM_TEXT_ADRS           0xefe00000
#define ROM_BASE_ADRS           0xefe00000

#define PHYS_MEM_START          0x00000000
#define PHYS_MEM_SIZE           0x40000000

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS           LOCAL_MEM_LOCAL_ADRS

#define LOCAL_MEM_LOCAL_ADRS    0x00000000
#define LOCAL_MEM_SIZE          0x40000000

#define USER_RESERVED_MEM       0x00000000

/*
 * To help for synchronization, we need implement BSP_BOOT_CACHE_SYNC
 * which is used in romStart()
 */

#define BSP_BOOT_CACHE_SYNC        sysBspCacheSync()

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
