/* config.h - Freescale QorIQ P2041RDB BSP configuration file */

/*
 * Copyright (c) 2011-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,22feb13,d_l  uprev BSP_REV to "6.9/2"
01e,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01d,09apr12,h_k  updated BSP_REV for 6.9.2 UP4 release. (WIND00341977)
                 removed optional components.
                 removed redundant definitions and undefinitions.
01c,06mar12,y_y  add vxbus I2C support
01b,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01a,31oct11,d_l  adapted from fsl_p4080_ds version 02r
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "6.9"
#define BSP_REV         "/2"
           
#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

#include "p2041rdb.h"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get 
 * overwritten by operating system or application software.
 *
 * Changing them will require reflashing the bootrom. 
 */

#define CPUn_SPACE_BASE_ADRS    0x00100000  /* RAM_LOW_ADRS */
#define CPUn_SPACE_SIZE         0x00010000  /* must be 2^exp */
#define CPUn_SPACE_SIZE_SHIFT   16          /* used by romInit.s */
#define CPUn_SPACE(n)           ((CPUn_SPACE_BASE_ADRS - ( CPUn_SPACE_SIZE * (n) )))
#define CPUn_FUNC_START_ADR(n)  (CPUn_SPACE(n) + 0)
#define CPUn_STACK_START_ADR(n) (CPUn_SPACE(n) + 4)
#define CPUn_INIT_START_ADR(n)  (CPUn_SPACE(n) + 8)
#define CPUn_LOCAL_MEM_ERPN(n)  (CPUn_SPACE(n) + 12)
#define CPUn_LOCAL_MEM_RPN(n)   (CPUn_SPACE(n) + 16)

/*
 * Errata: A-003474 (Internal DDR calibration circuit is not supported) 
 * This workaround is implemented in bootrom (romInit.s).
 */

#define FSL_ERRATA_DDR_A003474

/*
 * Errata: A-003999 (Running Floating Point instructions requires special 
 * initialization), This workaround is implemented in boortom(romInit.s) 
 * and vxWorks image(sysLib.c).
 */

#define FSL_ERRATA_FPU_A003999

/* Common Includes for VXBUS PCI and DTSEC */

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS

#undef  INCLUDE_PCI_BUS

#ifdef INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define DRV_PCIBUS_QORIQ_PCIEX
#endif /* INCLUDE_PCI_BUS */

#define DRV_RESOURCE_QORIQLAW
#define DRV_RESOURCE_QORIQBMAN
#define DRV_RESOURCE_QORIQQMAN
#define DRV_RESOURCE_QORIQFMAN

#define INCLUDE_INTCTLR_LIB
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_EPIC
#define INCLUDE_INTCTLR_DYNAMIC_LIB

#define INCLUDE_TIMER_SYS
#define DRV_TIMER_M85XX
#define INCLUDE_TIMER_EARLY_VXB_DELAY

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#define DRV_VXBEND_DTSEC
#define INCLUDE_DMA_SYS
#define INCLUDE_P2041_FMAN_UCODE
#define DRV_MII_DTSEC_MDIO
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_END

/*
 * add CDF_OVERRIDE for remove the following define when prj build, but used
 * for commandline build.
 */

#ifndef CDF_OVERRIDE
#   define QORIQ_PCIEX1_ENABLE      FALSE
#   define QORIQ_PCIEX2_ENABLE      TRUE
#   define QORIQ_PCIEX3_ENABLE      FALSE
#   define QORIQ_PCIEX1_UNIT_NUM    0
#   define QORIQ_PCIEX2_UNIT_NUM    1
#   define QORIQ_PCIEX3_UNIT_NUM    2
#endif /* CDF_OVERRIDE */

/*
 * The P2041 also has one 10 gigabit ethernet controllers.
 * To use it, one XAUI riser card is needed,
 * which is not included by default with the P2041RDB board.
 * Consequently, the TGEC driver is not on by default either.
 *
 * When it is enabled, the TGEC driver works in conjunction
 * with the DTEPHY driver for link management with miiBus.
 * The DRV_MII_TGEC_MDIO driver is also required for fiber
 * optic media support.
 *
 */
#undef DRV_VXBEND_TGEC

#ifdef DRV_VXBEND_TGEC
#   define DRV_MII_DTEPHY
#   define DRV_MII_TGEC_MDIO
/*
 * To select fiber media mode on one of the TGEC ports, define
 * the macro below. This requires the TGEC_MDIO driver.
 *
 */
#   undef TGEC_FIBER
#endif

#define HWMEM_POOL_SIZE 100000

/* default mac address */

#define ENET_DEFAULT0 WR_ENET0
#define ENET_DEFAULT1 WR_ENET1
#define ENET_DEFAULT2 WR_ENET2

#undef  FORCE_DEFAULT_BOOT_LINE
#define DEFAULT_BOOT_LINE \
"dtsec(0,0)host:vxWorks h=192.168.0.1 e=192.168.0.2 u=vxworks pw=vxworks f=0x0" 

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
#define INCLUDE_L3_CACHE

#define INCLUDE_BRANCH_PREDICTION

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/*
 * The following parameters are to configure initial memory usage for
 * page table and region tables and in event of running out the increment
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks).
 */

#   define AIM_MMU_INIT_PT_NUM      0x40
#   define AIM_MMU_INIT_PT_INCR     0x20
#   define AIM_MMU_INIT_RT_NUM      0x10
#   define AIM_MMU_INIT_RT_INCR     0x10

#endif /* INCLUDE_MMU_BASIC */

/* Serial channel and TTY */

#undef  NUM_TTY
#define NUM_TTY                     4
#undef  CONSOLE_BAUD_RATE 
#define CONSOLE_BAUD_RATE           115200
#undef  CONSOLE_TTY
#define CONSOLE_TTY                 0

/* Clock rates */

#ifdef INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif /* INCLUDE_AUX_CLK */

#define SYS_CLK_RATE_MIN            1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX            8000    /* maximum system clock rate */
#define AUX_CLK_RATE_MIN            1       /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX            8000    /* maximum auxiliary clock rate */
#define FIT_CLK_RATE_MIN            (DEFAULT_SYSCLKFREQ / (1 << 21))
#define FIT_CLK_RATE_MAX            (DEFAULT_SYSCLKFREQ / (1 << 10))

/* I2C support */

#ifdef INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_RTC
#   define INCLUDE_TIMER_RTC
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

/* SPI support */

#undef DRV_SPI_S25FLXX

#ifdef DRV_SPI_S25FLXX
#   define INCLUDE_FSL_SPI
#endif  /* DRV_SPI_S25FLXX */

/* DMA support */

#undef DRV_DMA_FSL

/* USB EHCI */

#undef INCLUDE_USB

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_USB */

/* SATA */

#undef DRV_STORAGE_FSLSATA

#ifdef DRV_STORAGE_FSLSATA
#   define INCLUDE_DOSFS
#endif /* DRV_STORAGE_FSLSATA */

/* eSDHC */

#undef DRV_STORAGE_SDHC

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif  /* DRV_STORAGE_SDHC */

/* TrueFFS flash support */

#undef INCLUDE_TFFS

#ifdef  INCLUDE_TFFS

/* board specific flash configuration needed by MTD driver */

#   define S29GL_FLASH_BIT      1           /* 16-bit */
#   define S29GL_FLASH_NUM      1           /* only one chip */
#   define S29GL_FLASH_WIDTH    2           /* 16-bit */
#   define S29GL_SECTOR_SIZE    0x00020000  /* 128K per sector */
#   define S29GL_FLASH_SIZE     0x00800000  /* only 8MB used for TrueFFS */
#   define S29GL_FLASH_OP_DELAY

#   define INCLUDE_TL_FTL
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_TFFS */

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

#define FLASH_WINDOW_SIZE           0x00800000 

/* LBC CS0 - flash 0 - default for bootrom */ 

#define FLASH_BASE_ADRS             0xff000000
#define FLASH_ADRS_MASK             0xff000000
#define MAIN_FLASH_SIZE             0x01000000

/* LBC CS3 - nvram, cadmus, ATM phy */

#define INCLUDE_LBC_CS3

#ifdef INCLUDE_LBC_CS3
#   define LBC_CS3_LOCAL_ADRS       CPLD_BASE
#   define LBC_CS3_LOCAL_SIZE_MASK  0xfff00000
#   define LBC_CS3_SIZE             CPLD_SIZE
#endif

#define INCLUDE_FLASH

#ifdef INCLUDE_FLASH

/*
 * 29LV640D flash is not supported in flashMem.c directly but is similar
 * in procedures with 29LV160B
 */
#   define S29GLXX
#   define FLASH_29LV640D           FLASH_29LV160B

#   undef  SYS_FLASH_TYPE
#   define SYS_FLASH_TYPE           FLASH_29LV640D
#   undef  FLASH_WIDTH
#   define FLASH_WIDTH              2       
#   undef  FLASH_CHIP_WIDTH
#   define FLASH_CHIP_WIDTH         2       

#   define NV_RAM_ADRS              0
#   define FLASH_SEGMENT_SIZE       0x10000
#   define FLASH_ADRS               0xffef0000

#   define FLASH_SIZE               0x10000
#   define FLASH_SIZE_WRITEABLE     FLASH_SIZE  /* write all of flash */
#   define NV_RAM_SIZE              FLASH_SIZE
#   undef FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET           0

#else /* INCLUDE_FLASH */

#   define NV_RAM_SIZE              0

#endif

/* Memory addresses */

#define PHYS_MEM_START              0x00000000
#define PHYS_MEM_SIZE               0x80000000
#define DEFAULT_LOCAL_MEM_SIZE      PHYS_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS        PHYS_MEM_START

/* DDR_SDRAM_LOCAL_END equals (PHYS_MEM_START + PHYS_MEM_SIZE - 1) */

#define DDR_SDRAM_LOCAL_END         (0x7FFFFFFF)
#define DDR_SDRAM_LAWAR_SIZE        LAWAR_SIZE_2GB

/*
 * Address of QMAN and BMAN private memory.
 * The QMAN driver will allocate 2MB starting from QMAN_MEM_BASE.
 * The BMAN driver will allocate 1MB starting from BMAN_MEM_BASE.
 * This memory is reserved and should not be accessed by software.
 *
 * The QMAN and BMAN are passed the address of their private memory
 * _once_ at hw init. Once passed and initialized, the private memory
 * cannot be relocated. This has the implication that downloadable 
 * kernels must know and respect whatever location the bootrom has 
 * used to place the private memory. For this reason Wind River chose
 * to hard-code the private memory to a fixed location, as this could
 * be most easily supported in a reference BSP which must implement
 * all possible configurations.
 *
 * vxWorks has multiple methods for allocating reserved memory. For
 * this BSP we have chosen to reserve the memory by hard-coding it
 * at a place immediately above LOCAL_MEM_SIZE. Another method would
 * have been to place it below RAM_LOW_ADRS. Users without Wind River's
 * legacy bootrom considerations should feel free to relocate and 
 * reserve the private space however best suits their application.
 */

#define QMAN_BMAN_RESERVED_MEM      0x00300000
#define QMAN_MEM_BASE               (PHYS_MEM_START + PHYS_MEM_SIZE - \
                                     QMAN_BMAN_RESERVED_MEM)
#define BMAN_MEM_BASE               (PHYS_MEM_START + PHYS_MEM_SIZE - \
                                     0x00100000)

/*
 * CDF_OVERRIDE is automatically defined for vxprj builds, ensuring
 * LOCAL_MEM_SIZE will be referenced from the 20bsp.cdf instead.
 */

#ifndef CDF_OVERRIDE
#   define LOCAL_MEM_SIZE           (PHYS_MEM_SIZE - QMAN_BMAN_RESERVED_MEM)
#   define LOCAL_MEM_LOCAL_ADRS     PHYS_MEM_START
#endif /* CDF_OVERRIDE */

#ifdef IPBASIC
/* remove power management for bootrom */
#   undef INCLUDE_CPU_PWR_MGMT
#endif /* IPBASIC */

/*
 * NEW 36-BIT CONFIGURATION MACROS.
 * 
 * LOCAL_MEM_ERPN specifies the upper 4 bits of the physical address of RAM
 * used by this image.  It cannot be set to anything other than 0 for CPU0.
 * It should be configured using the project facility or vxprj.
 *
 * DDR_SDRAM_LOCAL_ADRS_H specifies the upper 4 bits of the physical address
 * of the first bank of RAM.  This bank is what is used by CPU0 and any CPUs
 * configured with a LOCAL_MEM_ERPN of 0.
 * 
 * DDR_SDRAM_LOCAL_ADRS2_H specifies the upper 4 bits of the physical address
 * of the second bank of RAM.  This bank can be used by any CPU configured
 * with a LOCAL_MEM_ERPN equal to DDR_SDRAM_LOCAL_ADRS2_H.
 * 
 */

#ifndef PRJ_BUILD
#   define LOCAL_MEM_ERPN           0
#endif /* PRJ_BUILD */

#define DDR_SDRAM_LOCAL_ADRS_H      0
#define DDR_SDRAM_LOCAL_ADRS2_H     2

#define DDR_SDRAM_LOCAL_ADRS2       DDR_SDRAM_LOCAL_ADRS
#define DDR_SDRAM_LOCAL_END2        DDR_SDRAM_LOCAL_END

#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS               LOCAL_MEM_LOCAL_ADRS

#define ROM_TEXT_ADRS               0xFFF00100
#define ROM_BASE_ADRS               0xFFF00000
#define ROM_SIZE                    0x00100000

/* 
 * Do not clear memory in romStart() since it is not necessary,
 * and slows boot time.
 */

#undef ROMSTART_BOOT_CLEAR

/* user reserved memory, see sysMemTop() */

#define USER_RESERVED_MEM           0x00000000

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
