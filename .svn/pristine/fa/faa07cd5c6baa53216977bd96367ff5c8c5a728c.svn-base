/* config.h - Freescale P1021MDS BSP configuration file */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01f,19jul12,clx  update BSP_REV
01e,05mar12,y_y  add vxBus I2C support.
01d,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
                 Update BSP_REV for vxWorks 6.9.2.1
01c,14may11,y_y  modified I2C RTC macro.(WIND00275906)
01b,07apr11,y_y  modified auxiliary clock rate.(WIND00266083) 
01a,17dec10,y_y  derived from fsl_p1020_rdb/config.h
*/

#ifndef	__INCconfigh
#define	__INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "6.9"
#define BSP_REV         "/2"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif  /* _ASMLANGUAGE */

#include "p1021mds.h"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software.
 * Changing them will require reflashing the bootrom.
 * Here we need a region of memory that is safe to use
 * We assume that 0x10000 is enough space for initial vxworks stack
 * before it runs the first task.
 * Assumes also that RAM_LOW_ADRS is at least 0x20000.
 */

#define CPU1_FUNC_START_ADR     (0x00100000 - 0x10000)
#define CPU1_STACK_START_ADR    (CPU1_FUNC_START_ADR + 4)
#define CPU1_INIT_START_ADR     (CPU1_STACK_START_ADR + 4)
#define AMP_CPU_STATUS_BLOCK    (CPU1_FUNC_START_ADR + 0x20)
#define AMP_CPU0_STATUS         (CPU1_FUNC_START_ADR + 0x20)
#define AMP_CPU1_STATUS         (CPU1_FUNC_START_ADR + 0x40)
#define DEVICE_CORE0            0
#define DEVICE_CORE1            1

#ifndef _ASMLANGUAGE
typedef enum ampCpuState {
    ampCpuStateUnknown = 0,
    ampCpuStateReboot  = 1,
    ampCpuStateBooting = 2,
    ampCpuStateRunning = 3,
    ampCpuStateError  = -1
} AMP_CPU_STATE;
#endif  /* _ASMLANGUAGE */

/* Common Includes for VXBUS */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_VXBUS_SHOW
#define HWMEM_POOL_SIZE         100000

/* Interrupt controller */

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC

/* Timer */

#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_SYS
#define INCLUDE_TIMER_EARLY_VXB_DELAY
#undef  INCLUDE_TIMESTAMP

/* SIO */
/* QE UART is mutually exclusive with DUART */

#define  DRV_SIO_NS16550
#undef   DRV_SIO_UCC

#ifdef DRV_SIO_UCC
#   define INCLUDE_ISR_DEFER
#endif /* DRV_SIO_UCC */

#define INCLUDE_SIO_UTILS

/* Serial Line Settings */

#undef  NUM_TTY

#ifdef DRV_SIO_UCC
#   define NUM_TTY                  1   /* 1 UCC SIO     */
#endif /* DRV_SIO_UCC */

#ifdef DRV_SIO_NS16550
#   define NUM_TTY                  2   /* 2 NS16550 SIO */
#endif /* DRV_SIO_NS16550 */

#undef  CONSOLE_TTY
#define CONSOLE_TTY                 0
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE           115200

/* INCLUDE_PCI_BUS */

#ifdef  INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_M85XX
#   define DRV_RESOURCE_M85XXCCSR
#   undef INCLUDE_INTCTLR_DYNAMIC_LIB
#endif /* INCLUDE_PCI_BUS */

/* ETSEC */

#define INCLUDE_END
#define INCLUDE_ETSEC_VXB_END
#ifdef  INCLUDE_ETSEC_VXB_END
#   define INCLUDE_MII_BUS
#   define INCLUDE_TSEC_MDIO
#   define INCLUDE_MV88E1X11PHY
#endif /* INCLUDE_END */

/* QE_UCC */

#define DRV_INTCTLR_QE
#ifdef  DRV_INTCTLR_QE	
#   define INCLUDE_QUICC_ENGINE_UTILS
#   define INCLUDE_UCC_VXB_END
#   define INCLUDE_UCC_MDIO
#   define INCLUDE_GENERICPHY
#endif /* DRV_INTCTLR_QE */

/* 
 * We use SPI flash save MAC and BOOTLINE information,
 * so define INCLUDE_FSL_SPI as default 
 */

#define  INCLUDE_FSL_SPI
#ifdef INCLUDE_FSL_SPI
#   define NV_RAM_SIZE             0x1000 
#else
#   define NV_RAM_SIZE             0 
#endif

#define DEFAULT_BOOT_LINE \
    "motetsec(0,0)host:vxWorks.st h=128.224.163.155 e=128.224.162.108:fffffe00 g=128.224.162.1 u=target pw=vxTarget f=0x0 tn=P1021-MDS-PB"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE      (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE     _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE          VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY

#define INCLUDE_CACHE_SUPPORT 
#define USER_D_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       (CACHE_LIBRARY_MODE)
#define USER_I_CACHE_ENABLE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE       (CACHE_LIBRARY_MODE)

#define INCLUDE_L2_CACHE
#define INCLUDE_BRANCH_PREDICTION
#define L2_CACHE_SIZE           L2SIZ_256KB

/* SPE support */

#define INCLUDE_SPE
#undef  INCLUDE_SECURITY_ENGINE

/* Use openPicTimer as the auxClk */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif  /* INCLUDE_AUX_CLK */

/* I2C support */

#ifdef INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_RTC
#   define INCLUDE_TIMER_RTC
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

/* DMA support */

#undef  DRV_DMA_FSL

/* Clock rates */

#define	SYS_CLK_RATE_MIN	1 	    /* minimum system clock rate    */
#define	SYS_CLK_RATE_MAX	8000	/* maximum system clock rate    */
#define	AUX_CLK_RATE_MIN	1	    /* minimum auxiliary clock rate */
#define	AUX_CLK_RATE_MAX	50000	/* maximum auxiliary clock rate */

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

#undef INCLUDE_CTORS_DTORS
 
/* Memory addresses */
/* 
 * We use 0x400000 as default, if you change make sure Makefile will be 
 * changed at the same time, more comments can reference the bootInit_nand.c
 */
 
#define USER_RESERVED_MEM      0x400000
#define RAM_IMAGE_ADRS        (0x20000000 - USER_RESERVED_MEM)
#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS          LOCAL_MEM_LOCAL_ADRS
#define ROM_BASE_ADRS          RAM_IMAGE_ADRS     /* base address of ROM */
#define ROM_TEXT_ADRS         (ROM_BASE_ADRS + 0x100)
#define ROM_WARM_ADRS         (ROM_TEXT_ADRS + 8) /* warm reboot entry   */
#define ROM_SIZE               0x2000000

/*
 * LOCAL_MEM_SIZE only can be defined in config.h when commandline build.
 * So we add CDF_OVERRIDE for rmove this define when prj build.
 * For commandline build, we set MV88E1X11PHY_MODE as follow, for prj build
 * we realize it in 20bsp.cdf
 */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE          0x20000000
#   define LOCAL_MEM_LOCAL_ADRS    0x00000000
#   define MV88E1X11PHY0_MODE      MV88E_MODE_RGMII
#   define MV88E1X11PHY1_MODE      MV88E_MODE_SGMII_NOCLK
#   define CONFIG_ETSEC3_IN_SGMII  FALSE
#       if (CONFIG_ETSEC3_IN_SGMII == TRUE)  /* Need to set SW8.6 to ON */
#           define MV88E1X11PHY2_MODE      MV88E_MODE_SGMII_NOCLK
#       else
#           define MV88E1X11PHY2_MODE      MV88E_MODE_RGMII 
#       endif /* CONFIG_ETSEC3_IN_SGMII == TRUE */
#endif /* CDF_OVERRIDE */

/*
 * To help for synchronization, we need implement BSP_BOOT_CACHE_SYNC
 * which is used in romStart()
 */
 
#define BSP_BOOT_CACHE_SYNC        sysBspCacheSync()

/* 
 * Undefine to not clear memory when booting since it has been done
 * by DDR_DATA_INIT register
 */

#undef ROMSTART_BOOT_CLEAR

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER
#define FSL_ENET0           0x00
#define FSL_ENET1           0xA0
#define FSL_ENET2           0x1E
#define CUST_ENET3          0xAA
#define CUST_ENET4          0xA0
#define CUST_ENET5_0        0xA0  
#define CUST_ENET5_1        0xA1  
#define CUST_ENET5_2        0xA2
#define CUST_ENET5_3        0xA3
#define CUST_ENET5_4        0xA4
#define NV_MAC_ADRS_OFFSET  0x100
#define MAX_MAC_ADRS        5
#define MAX_MAC_DEVS        5
#define MAC_ADRS_LEN        6

/* default mac address */

#define ENET_DEFAULT0       FSL_ENET0
#define ENET_DEFAULT1       FSL_ENET1
#define ENET_DEFAULT2       FSL_ENET2
#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */
#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */

