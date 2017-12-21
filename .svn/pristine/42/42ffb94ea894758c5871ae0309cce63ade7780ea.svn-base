/* config.h - Freescale P1020RDB BSP configuration file */

/*
 * Copyright (c) 2010-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,10jan13,y_y  add vxBus SPI support.
01j,15nov12,x_s  Added back the macro FLASH_SECTOR_SIZE. (WIND00377897)
01i,04may12,d_l  Added INCLUDE_GENERICPHY for dummyMdio.
01h,25apr12,d_l  Added SPI and VxBus I2C driver.
                 Update BSP version to 6.9/3.
01g,10apr12,x_z  Added support for P1020RDB-PC Rev B board with DDR3. 
01f,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01e,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
01d,21jul11,swu  Change _WRS_VX_AMP to INCLUDE_AMP
                 Change DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC.
                 Remove AMP_CORE1 usage.
                 Update BSP version for vxWorks 6.9.1
01c,02sep10,syt  Update BSP version for vxWorks 6.9
01b,12jun10,syt  BSP_REV bump up.
01a,22mar10,syt  derived from version 01b of fsl_p2020_rdb/config.h
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION             "6.9"
#define BSP_REV                 "/4"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif  /* _ASMLANGUAGE */

/* define the component to support P1020RDB-PC Rev B by default */

#define P1020RDB_PC_REVB

#include "p1020rdb.h"

#ifdef INCLUDE_AMP
#   ifndef _ASMLANGUAGE
#       include <vxIpiLib.h>
#   endif
#endif  /* INCLUDE_AMP */

/* Must be the same as RAM_LOW_ADRS for core0, this used for reboot on core1 */

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

#undef  DRV_TIMER_OPENPIC
#undef  INCLUDE_INTCTLR_DYNAMIC_LIB

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE       115200

#ifdef  INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_M85XX
#   define DRV_RESOURCE_M85XXCCSR
#endif  /* INCLUDE_PCI_BUS */

#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_DMA_SYS
#define INCLUDE_MII_BUS
#define INCLUDE_DUMMY_MDIO
#define INCLUDE_TSEC_MDIO
#define INCLUDE_VSC82XXPHY

#ifndef P1020RDB_PC_REVB
#   define INCLUDE_GENERICPHY
#else /* P1020RDB_PC_REVB */
#   define INCLUDE_QCOM_AR8021PHY
#endif /* P1020RDB_PC_REVB */

#ifdef P1020RDB_PC_REVB
#   define INCLUDE_QCOM_AR8021PHY
#endif /* P1020RDB_PC_REVB */

#define MAX_MAC_DEVS            3
#define ENET_DEFAULT0           WR_ENET0
#define ENET_DEFAULT1           WR_ENET1
#define ENET_DEFAULT2           WR_ENET2

#undef  DRV_DMA_FSL
#define INCLUDE_GPIO

#define WDT_RATE_MIN            (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX            (sysTimerClkFreq / (1 << 21))

#ifdef INCLUDE_AMP
#   undef  SM_BASE_ADRS
#   define SM_BASE_ADRS         0x10000000
#endif  /* INCLUDE_AMP */

#define DEFAULT_BOOT_LINE \
    "motetsec(2,0)host:vxWorks h=192.168.0.1 e=192.168.0.3 u=vxworks pw=vxworks f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE   (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE  _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE       (VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY)

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_LIBRARY_MODE)
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE       (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE
#undef  INCLUDE_L2_SRAM

#define INCLUDE_BRANCH_PREDICTION

/* L2 SRAM Mode */

#ifdef INCLUDE_L2_SRAM
#   define L2SRAM_ADDR          0xf2000000
#   define L2SRAM_WINDOW_SIZE   0x00040000
#endif  /* INCLUDE_L2_SRAM */

#if defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        1
#   define L2_SRAM_SIZE         1
#elif defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        2
#   define L2_SRAM_SIZE         0
#else
#   define L2_CACHE_SIZE        0
#   define L2_SRAM_SIZE         2
#endif  /* INCLUDE_L2_CACHE && INCLUDE_L2_SRAM */

/* Support for SPE 64bit registers */

#define INCLUDE_SPE

/* add on-chip drivers */

/* Use openPicTimer as the auxClk */

#ifdef  INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif  /* INCLUDE_AUX_CLK */

/* SPI support */

/* legacy spi support */

#ifdef DRV_SPI_S25FLXX
#   define INCLUDE_FSL_SPI
#endif  /* DRV_SPI_S25FLXX */

/* vxBus SPI support */

#ifdef DRV_SPIFLASH_SP25
#   define INCLUDE_SPI_BUS
#   define DRV_ESPI_FSL
#endif  /* DRV_SPIFLASH_SP25 */

/* VxBus I2C support */

#ifdef DRV_I2CBUS_FSL 
#   define INCLUDE_I2C_BUS
#endif /* DRV_I2CBUS_FSL */

/* NAND Flash support */

#undef  INCLUDE_NAND_FLASH

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS         0xff800000
#define FLASH_SIZE              0x00800000
#define ROM_SIZE                0x00100000

/*
 * bootline storage and MAC storage in flash not supported with images
 * loaded using WRLOAD so undefine the flash utility.
 */

#ifndef INCLUDE_WRLOAD_IMAGE_BUILD
#   define INCLUDE_FLASH
#endif  /* INCLUDE_WRLOAD_IMAGE_BUILD */

#ifdef INCLUDE_FLASH

#   define NV_RAM_FLASH_NAME        "norFlash"
#   define NV_RAM_FLASH_UNIT        0
#   undef  NV_RAM_ADRS
#   define NV_RAM_ADRS              (ROM_BASE_ADRS - NV_RAM_SIZE)
#   define NV_RAM_OFFSET            (NV_RAM_ADRS - FLASH_BASE_ADRS)
#   undef  NV_RAM_SIZE
#   define NV_RAM_SIZE              0x00020000 /* Flash sector size */
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET           0
#   define NV_MAC_ADRS_OFFSET       0x200

#   define FLASH_SECTOR_SIZE        0x00020000 /* Flash sector size */
#else  /* INCLUDE_FLASH */
#   define NV_RAM_SIZE              0
#endif  /* INCLUDE_FLASH */

/* TrueFFS */

#ifdef INCLUDE_TFFS

#   define INCLUDE_TFFS_MOUNT
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
#   define TFFS_PART0_FLASH_SIZE        0x00400000  /* total size */
#   define TFFS_PART0_FLASH_BOOT_SIZE   0           /* reserved for bootrom */

/* TrueFFS partition 1 */

#   ifdef DRV_SPIFLASH_SP25
#       define TFFS_PART1_NAME              "RFA1"
#       define TFFS_PART1_FLASH_NAME        "spiFlash_sp25probe"
#       define TFFS_PART1_FLASH_UNIT        0
#       define TFFS_PART1_FLASH_BASE_ADRS   0
#       define TFFS_PART1_FLASH_OFFSET      0
#       define TFFS_PART1_FLASH_BOOT_SIZE   0
#       define TFFS_PART1_FLASH_SIZE        0x1000000   /* total size - 16MB */
#   endif  /* DRV_SPIFLASH_SP25 */

#endif /* INCLUDE_TFFS */

/* Flash */

#if defined(INCLUDE_FLASH) || defined(INCLUDE_TFFS)
#   define DRV_VXB_NORFLASH
#   define DRV_VXB_NORFLASH_CMDSET_AMD
#endif /* INCLUDE_FLASH || INCLUDE_TFFS */

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif  /* DRV_STORAGE_SDHC */

/* USB Host */

#ifdef INCLUDE_USB2
#   define INCLUDE_USB
#   undef  INCLUDE_NAND_FLASH
#   undef  INCLUDE_TFFS
#endif  /* INCLUDE_USB2 */

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
#   define FSL_USB_DR_DEVICE_IO_BASE      USB1_BASE
#   define FSL_USB_DR_DEVICE_INT_NUM      EPIC_USB1_INT_VEC
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
#   define INCLUDE_DISK_UTIL
#   ifdef  BOOTAPP
#       define INCLUDE_BOOT_FILESYSTEMS
#   endif  /* BOOTAPP */
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

#define ROM_TEXT_ADRS           0xFFF00100
#define ROM_BASE_ADRS           0xFFF00000

#define PHYS_MEM_START          0x00000000

#ifndef P1020RDB_PC_REVB
#   define PHYS_MEM_SIZE        0x20000000
#else /* P1020RDB_PC_REVB */
#   define PHYS_MEM_SIZE        0x40000000
#endif /* P1020RDB_PC_REVB */

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS           LOCAL_MEM_LOCAL_ADRS

#define AMP_CORE1_OFFSET        0x10000000
#define AMP_CORE1_TEXT_ADRS     0xFFF00100

/*
 * LOCAL_MEM_SIZE only can be defined in config.h when command line build.
 * So we add CDF_OVERRIDE for remove this define when prj build.
 */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE       PHYS_MEM_SIZE 
#   define LOCAL_MEM_LOCAL_ADRS 0x00000000
#endif /* CDF_OVERRIDE */

#define USER_RESERVED_MEM       0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
