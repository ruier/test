/* config.h - Freescale P2020RDB BSP configuration file */

/*
 * Copyright (c) 2009-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,15nov11,ers  Moved L1 parity handling to arch lib. (WIND00309426)
01f,15sep11,agf  update BSP_REV to reflect SDHC driver change
01e,10mar11,my_  rename DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC.
01f,23aug11,ers  CQ253685:Fix memory mapping bugs and isolate the cores'
                 memory from each other.  Support wrload memory mapping and 
                 loading.
01e,25jul11,ers  BSP_REV bump up.
01d,02sep10,syt  Update BSP version for vxWorks 6.9
01c,05apr10,syt  BSP_REV bump up.
01b,29jan10,syt  added AMP support and update bsp version.
01a,10oct09,syt  derived from version 01a of fsl_p2020_ds/config.h
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION             "6.9"
#define BSP_REV                 "/3"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif  /* _ASMLANGUAGE */

#include "p2020rdb.h"

#ifdef _WRS_VX_AMP
#   ifndef _ASMLANGUAGE
#       include <vxIpiLib.h>
#   endif
#endif  /* _WRS_VX_AMP */

/* Must be the same as RAM_LOW_ADRS for core0, this used for reboot on core1 */

#define RAM_LOW_ADRS_CPU0       0x00100000

/*
 * Here we need a region of memory that is safe to use
 * We assume that 0x10000 is enough space for initial vxworks stack
 * before it runs the first task.
 * Assumes also that RAM_LOW_ADRS is at least 0x20000.
 */

#define CPU1_FUNC_START_ADR     (RAM_LOW_ADRS_CPU0 - 0x10000)

#define CPU1_STACK_START_ADR    (CPU1_FUNC_START_ADR + 0x04)
#define CPU1_INIT_START_ADR     (CPU1_FUNC_START_ADR + 0x08)
#define CPU1_LOCAL_MEM_ERPN     (CPU1_FUNC_START_ADR + 0x0C)
#define CPU1_LOCAL_MEM_RPN      (CPU1_FUNC_START_ADR + 0x10)


#ifdef  _WRS_VX_AMP
#   define DEVICE_CORE0         0
#   define DEVICE_CORE1         1
#endif  /* _WRS_VX_AMP */

#ifdef _WRS_CONFIG_SMP

/* # of CPUs configured to vxWorks SMP systems */

#   undef  VX_SMP_NUM_CPUS
#   define VX_SMP_NUM_CPUS      2
#   define INCLUDE_SMP_DEMO
#endif /* _WRS_CONFIG_SMP */

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

#ifdef  _WRS_VX_AMP
#   define INCLUDE_VXIPI
#endif  /* _WRS_VX_AMP */

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

#define INCLUDE_END
#define INCLUDE_ETSEC_VXB_END
#define INCLUDE_DMA_SYS
#define INCLUDE_MII_BUS
#define INCLUDE_DUMMY_MDIO
#define INCLUDE_TSEC_MDIO
#define INCLUDE_VSC82XXPHY
#define INCLUDE_GENERICPHY

#define MAX_MAC_DEVS            3
#define ENET_DEFAULT0           WR_ENET0
#define ENET_DEFAULT1           WR_ENET1
#define ENET_DEFAULT2           WR_ENET2

#undef  DRV_DMA_FSL
#undef  INCLUDE_FSL_SPI
#define INCLUDE_GPIO

#define WDT_RATE_MIN            (sysTimerClkFreq / (1 << 29))
#define WDT_RATE_MAX            (sysTimerClkFreq / (1 << 21))

/*
 * The macro _WRS_VX_AMP is defined in Makefile for AMP bsp,
 * so we can't define this macro in UP or SMP bsp.
 */

#ifdef _WRS_VX_AMP
#   define INCLUDE_AMP
#   define TM_ANCHOR_ADRS       0x4400
#   define SM_BASE_ADRS         0
#endif  /* _WRS_VX_AMP */

#ifdef  AMP_CORE1
#   define DEFAULT_BOOT_LINE \
    "motetsec(2,0)host:vxWorks h=192.168.0.1 e=192.168.0.3 u=vxworks pw=vxworks f=0x0"
#   else  /* AMP_CORE1 */
#   define DEFAULT_BOOT_LINE \
    "motetsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0"
#endif  /* AMP_CORE1 */

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE


# define CACHE_LIBRARY_MODE     (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
# define CAM_DRAM_CACHE_MODE    _MMU_TLB_ATTR_M
# define TLB_CACHE_MODE         (VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY)

#define INCLUDE_CACHE_SUPPORT

#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_LIBRARY_MODE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE       (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE
#undef  INCLUDE_L2_SRAM

#define INCLUDE_BRANCH_PREDICTION

/* L2 SRAM Mode */

#ifdef INCLUDE_L2_SRAM
#   define L2SRAM_ADDR          0xf2000000
#   define L2SRAM_WINDOW_SIZE   0x00080000
#endif  /* INCLUDE_L2_SRAM */

#if defined(INCLUDE_L2_CACHE) && defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_256KB
#   define L2_SRAM_SIZE         L2SIZ_256KB
#elif defined(INCLUDE_L2_CACHE) && !defined(INCLUDE_L2_SRAM)
#   define L2_CACHE_SIZE        L2SIZ_512KB
#   define L2_SRAM_SIZE         0
#else
#   define L2_CACHE_SIZE        0
#   define L2_SRAM_SIZE         L2SIZ_512KB
#endif  /* INCLUDE_L2_CACHE && INCLUDE_L2_SRAM */

/* Support for SPE 64bit registers */

#define INCLUDE_SPE

/* add on-chip drivers */

/* Use openPicTimer as the auxClk */

#ifdef  INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif  /* INCLUDE_AUX_CLK */

/* I2C support */

#undef  INCLUDE_I2C

/* NAND Flash support */

#undef  INCLUDE_NAND_FLASH

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_ADRS         0xff000000
#define FLASH_SIZE              0x01000000
#define HALF_FLASH_SIZE         (FLASH_SIZE / 2)
#define ROM_SIZE                0x00100000

/*
 * bootline storage and MAC storage in flash not supported with images
 * loaded using WRLOAD so undefine the flash utility.
 */

#ifndef INCLUDE_WRLOAD_IMAGE_BUILD
#   define INCLUDE_FLASH
#endif  /* INCLUDE_WRLOAD_IMAGE_BUILD */

#ifdef INCLUDE_FLASH

/* board specific flash configuration needed by MTD driver */

#   define S29GL_FLASH_BIT          1           /* 16-bit */
#   define S29GL_FLASH_NUM          1           /* only one chip */
#   define S29GL_FLASH_WIDTH        2           /* 16-bit */
#   define S29GL_SECTOR_SIZE        0x00020000  /* 128K per sector */
#   define S29GL_FLASH_SIZE         (HALF_FLASH_SIZE - ROM_SIZE)
#   define S29GL_FLASH_OP_DELAY

/* NVRam */

#   define FLASH_ADRS               FLASH_BASE_ADRS
#   define FLASH_WIDTH              S29GL_FLASH_WIDTH
#   define FLASH_CHIP_WIDTH         S29GL_FLASH_WIDTH
#   define FLASH_SECTOR_SIZE        S29GL_SECTOR_SIZE
#   define NV_RAM_ADRS              (ROM_BASE_ADRS - FLASH_SECTOR_SIZE)
#   define NV_RAM_SIZE              FLASH_SECTOR_SIZE
#   define NV_MAC_ADRS_OFFSET       0x200
#   define FLASH_SECTOR_MASK        0xfffe0000
#else  /* INCLUDE_FLASH */
#   define NV_RAM_SIZE              0
#endif  /* INCLUDE_FLASH */

/* TrueFFS */

#ifdef INCLUDE_TFFS
#   define INCLUDE_DOSFS
#   define INCLUDE_TL_FTL
#   define INCLUDE_TFFS_MOUNT
#endif  /* INCLUDE_TFFS */

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif  /* DRV_STORAGE_SDHC */

/* USB Host */

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
#   define FSL_USB_DR_DEVICE_INT_NUM      EPIC_USB_INT_VEC
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

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_SHOW
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

#if ((TOOL==e500v2diab) || (TOOL==e500v2gnu))

/* These are 60X specific for now */

#   undef INCLUDE_HW_FP

/* ev2diab is hard float so undefine soft float */

#   undef INCLUDE_SW_FP

#else  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

/*
 * Using software FP support. Athough task based 32 bit HW_FP is allowed
 * no optimised libraries are available. The kernel doesn't execute any hw
 * fp instructions
 */

#   undef  INCLUDE_HW_FP
#   define INCLUDE_SW_FP

#endif  /* (TOOL==e500v2diab) || (TOOL==e500v2gnu) */

#define ROM_TEXT_ADRS           0xFFF00100
#define ROM_BASE_ADRS           0xFFF00000

#define PHYS_MEM_START          0x00000000
#define PHYS_MEM_SIZE           0x40000000

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS           LOCAL_MEM_LOCAL_ADRS

#ifdef _WRS_VX_AMP
#   define LOCAL_MEM_SIZE       0x20000000
#   define AMP_CORE1_OFFSET     0x20000000
#   define AMP_CORE1_TEXT_ADRS  0xFFF00100
#else  /* _WRS_VX_AMP */
#   define LOCAL_MEM_LOCAL_ADRS 0x00000000
#   define LOCAL_MEM_SIZE       0x40000000
#endif  /* _WRS_VX_AMP */

#define USER_RESERVED_MEM       0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
