/* config.h - Freescale P1022 BSP configuration file */

/*
 * Copyright (c) 2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,03apr13,d_l  Uprev BSP_REV to "6.9/2"
01d,13nov12,d_l  Uprev BSP_REV.
01c,27sep12,y_f  Add WindML dependence
                 Uprev BSP_REV to "6.9/1"
01c,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01b,23mar12,d_l  Move L1 Parity handling to arch Lib. (WIND00309426)
                 Add VxBus I2C and NorFlash drivers.
                 Uprev BSP_REV to "6.9/0"
01a,17aug10,e_d  derived from fsl_p2020_ds/config.h
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION             "6.9"
#define BSP_REV                 "/2"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif  /* _ASMLANGUAGE */

#include "p1022.h"

#ifdef INCLUDE_AMP
#   ifndef _ASMLANGUAGE
#       include <vxIpiLib.h>
#   endif
#endif /* INCLUDE_AMP */

/*
 * Must be the same as RAM_LOW_ADRS for core0
 * This used for reboot on core1
 */

#define RAM_LOW_ADRS_CPU0       0x00100000

/*
 * Here we need a region of memory that is safe to use
 * We assume that 0x10000 is enough space for initial vxworks stack
 * before it runs the first task.
 * Assumes also that RAM_LOW_ADRS is at least 0x20000.
 */

#define CPU1_FUNC_START_ADR     (RAM_LOW_ADRS_CPU0 - 0x10000)
#define CPU1_STACK_START_ADR    (CPU1_FUNC_START_ADR + 4)
#define CPU1_INIT_START_ADR     (CPU1_STACK_START_ADR + 4)

#ifdef INCLUDE_AMP
#   define AMP_CPU_STATUS_BLOCK (CPU1_FUNC_START_ADR + 0x20)
#   define AMP_CPU0_STATUS      (CPU1_FUNC_START_ADR + 0x20)
#   define AMP_CPU1_STATUS      (CPU1_FUNC_START_ADR + 0x40)

#   define AMP_CPU1_DDRSPD      (CPU1_FUNC_START_ADR + 0x60)
#   define AMP_CPU1_CCBSPD      (CPU1_FUNC_START_ADR + 0x80)

#   define DEVICE_CORE0         0
#   define DEVICE_CORE1         1

#   ifndef _ASMLANGUAGE
typedef enum ampCpuState {
    ampCpuStateUnknown = 0,
    ampCpuStateReboot  = 1,
    ampCpuStateBooting = 2,
    ampCpuStateRunning = 3,
    ampCpuStateError  = -1
} AMP_CPU_STATE;
#   endif  /* _ASMLANGUAGE */
#endif  /* INCLUDE_AMP */

#undef INCLUDE_SPI_BOOT

/* For SPI boot , we reserve 2M DDR for copy bootrom image from SPI flash to DDR */

#ifdef INCLUDE_SPI_BOOT
#   define SPI_BOOT_RESMEM      0x200000
#else
#   define SPI_BOOT_RESMEM      0x0
#endif /* INCLUDE_SPI_BOOT */

#ifdef INCLUDE_SPI_BOOT
#   define ROM_TEXT_ADRS        0x7FE00500
#   define ROM_BASE_ADRS        0x7FE00000
#else
#   define ROM_TEXT_ADRS        0xFFF00100
#   define ROM_BASE_ADRS        0xFFF00000
#endif /* INCLUDE_SPI_BOOT */

#define ROM_SIZE                0x00100000

/*
 * LOCAL_MEM_SIZE only can be defined in config.h when commandline build.
 * So we add CDF_OVERRIDE for rmove this define when prj build.
 */

#ifndef CDF_OVERRIDE
#define LOCAL_MEM_SIZE          0x80000000 - SPI_BOOT_RESMEM
#define LOCAL_MEM_LOCAL_ADRS    0x00000000
#endif /* CDF_OVERRIDE */

/* Common Includes for VXBUS PCI and ETSEC */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_TIMER_EARLY_VXB_DELAY

#define HWMEM_POOL_SIZE         100000

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC

#undef  DRV_TIMER_OPENPIC
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB

#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS

/* Clock rates */

#define SYS_CLK_RATE_MIN        1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        8000    /* maximum system clock rate */

#define AUX_CLK_RATE_MIN        1
#define AUX_CLK_RATE_MAX        50000
#define AUX_CLK_RATE            20000

#define WDT_RATE_MIN            (sysClkFreq / (1 << 29))
#define WDT_RATE_MAX            (sysClkFreq / (1 << 21))

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#ifdef DRV_PCIBUS_M85XX
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define INCLUDE_PCI_BUS
#   define DRV_RESOURCE_M85XXCCSR
#endif  /* INCLUDE_PCI_BUS */

#define INCLUDE_END
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_TSEC_MDIO

#define MAX_MAC_DEVS            2

#undef  DRV_DMA_FSL
#undef  INCLUDE_FSL_SPI
#undef  DRV_STORAGE_SDHC
#undef  DRV_STORAGE_FSLSATA

#if defined(DRV_STORAGE_SDHC) || defined(DRV_STORAGE_FSLSATA)
#   define INCLUDE_DOSFS
#endif /* DRV_STORAGE_SDHC || DRV_STORAGE_FSLSATA */

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

#define AMP_MEM_START           0x00000000
#define AMP_MEM_SIZE            0x80000000

#define DEFAULT_BOOT_LINE \
    "motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.3 u=vxworks pw=vxworks f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE      (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE     _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE          VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_LIBRARY_MODE)
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE       (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE
#define INCLUDE_BRANCH_PREDICTION
#define L2_CACHE_SIZE           L2SIZ_256KB

/* SPE support */

#define INCLUDE_SPE

/* Use openPicTimer as the auxClk */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif /* INCLUDE_AUX_CLK */

/* WindML */

#undef INCLUDE_WINDML

#ifdef INCLUDE_WINDML
#   define INCLUDE_I2C_BUS
#endif  /* INCLUDE_WINDML */

/* 
 * I2C support 
 * ETSEC need reading their MACs via I2C bus.
 */

#ifdef INCLUDE_ETSEC_VXB_END
#   define INCLUDE_I2C_BUS       
#endif /* INCLUDE_ETSEC_VXB_END */

#ifdef INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_GENERIC_DEV
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

/* SPI support */

#undef DRV_SPI_S25FLXX

#ifdef DRV_SPI_S25FLXX
#   define INCLUDE_FSL_SPI
#endif  /* DRV_SPI_S25FLXX */

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS         0xf8000000
#define FLASH_SIZE              0x08000000
#define FLASH_WINDOW_SIZE       0x00800000
#define FLASH_ROM_ADRS          0xFFF00000

/* TrueFFS flash support */

#undef  INCLUDE_TFFS

#ifdef INCLUDE_TFFS

#   define INCLUDE_DOSFS

#   define TFFS_FORMAT_PRINT /* print the tffs format process */

/* TrueFFS partition 0 */

/*
 * define to support boot region lock function if supported by corresponding
 * Flash chip
 *
 * Note: sysTffsBootUnLock() must be called before tffsBootImagePut(), and
 * sysTffsBootLock() should be called to re-lock the boot region after
 * tffsBootImagePut().
 */

#   undef  TFFS_BOOT_LOCK

#   define TFFS_PART0_NAME              "RFA0"
#   define TFFS_PART0_FLASH_NAME        "norFlash"
#   define TFFS_PART0_FLASH_UNIT        0

/* main Flash information */

#   define TFFS_PART0_FLASH_BASE_ADRS   FLASH_BASE_ADRS
#   define TFFS_PART0_FLASH_OFFSET      0           /* offset to Flash */
#   define TFFS_PART0_FLASH_SIZE        0x04000000  /* total size */
#   define TFFS_PART0_FLASH_BOOT_SIZE   0x00000000  /* reserved for bootrom */
#endif /* INCLUDE_TFFS */

/* NvRam settings for VxBus Nor Flash driver */

#define NV_RAM_FLASH_NAME        "norFlash"
#define NV_RAM_FLASH_UNIT        0
#undef  NV_RAM_SIZE
#define NV_RAM_SIZE              (0x00020000) /* Main Flash sector size */
#undef  NV_RAM_ADRS
#define NV_RAM_ADRS              (FLASH_ROM_ADRS - NV_RAM_SIZE)
#define NV_RAM_OFFSET            (NV_RAM_ADRS - FLASH_BASE_ADRS)
#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET           0

#define DRV_VXB_NORFLASH
#define DRV_VXB_NORFLASH_CMDSET_AMD

/* USB EHCI */

#undef INCLUDE_USB

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

/*  Define for FS */

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
#   define INCLUDE_DISK_UTIL
#ifdef BOOTAPP
#   define INCLUDE_BOOT_FILESYSTEMS
#endif 
#endif  /* INCLUDE_DOSFS */

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

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

#undef INCLUDE_HW_FP

#if ((TOOL==e500v2diab) || (TOOL==e500v2gnu))

/* ev2diab is hard float so undefine soft float */

#   undef INCLUDE_SW_FP

#else  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed
 * no optimised libraries are available. The kernel doesn't execute any hw
 * fp instructions
 */

#   define INCLUDE_SW_FP

#endif  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

#define USER_RESERVED_MEM          0x00000000

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS              LOCAL_MEM_LOCAL_ADRS

/* This macro is define when romInit.s set D-Cache Enable */

#define BSP_BOOT_CACHE_SYNC        sysBspCacheSync()

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */

