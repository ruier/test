/* config.h - Freescale QorIQ P5020 DS BSP configuration file */

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
01v,27jun13,j_z  uprev BSP_REV to "6.9/7"
01u,22feb13,d_l  uprev BSP_REV to "6.9/6"
01t,28dec12,j_z  change RapidIO smEnd parameter.
01s,20oct12,j_z  add RapidIO support.
01r,21sep12,l_z  Replace INCLUDE_TIMER_STUB with INCLUDE_TIMER_EARLY_VXB_DELAY.
                 (WIND00377568)
01q,28feb12,y_y  add INCLUDE_BOOT_FILESYSTEMS macro to support SD/SATA as boot device.
01p,21feb12,y_y  fix compiler warning on DDR_SDRAM_LOCAL_END macro.
01o,09apr12,h_k  updated BSP_REV for 6.9.2 UP4 release. (WIND00341977)
                 removed optional components.
                 removed redundant definitions and undefinitions.
01n,06mar12,y_y  add vxbus I2C support
01m,29feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01l,28feb12,agf  update BSP_REV for UP3
01k,07feb12,x_s  Added macro definitions for two workarounds. (WIND00331187)
01j,06feb12,y_y  add INCLUDE_BOOT_FILESYSTEMS macro for SD/SATA boot support.
01i,06feb12,agf  inc BSP_REV for UP2
01h,11nov11,ers  WIND00297094: cache fix for vmPageLock/Unlock.
01g,08nov11,y_y  add usb target support.
01f,31oct11,y_y  modified tgec and pcie macros.
01e,24oct11,y_y  added TFFS support and code clean.
01d,27sep11,ers  Replaced DYNAMIC_WRLOAD with generic wrload. Updated BSP 
                 version (WIND00253685).
01c,24aug11,y_y  added I2C device support and misc code clean up.
01b,24may11,y_y  added SPI device support.
01a,24feb11,syt  adapted from fsl_p4080_ds version 02n
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define BSP_VERSION     "20160623-1533"
#define BSP_REV         "/liudan"

#include <vsbConfig.h>
#include <configAll.h>

#ifndef _ASMLANGUAGE
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

#include "p5020ds.h"

/*
 * The following addresses are used for multicore support. They represent
 * reserved memory and should be located where they will not get
 * overwritten by operating system or application software.
 *
 * Changing them will require reflashing the bootrom.
 */

#define CPUn_SPACE_BASE_ADRS    0x00100000 - 0x00010000 /* RAM_LOW_ADRS - 64KB of stack */
#define CPUn_SPACE_SIZE         0x00000400              /* must be 2^n */
#define CPUn_SPACE_SIZE_SHIFT   10                      /* used by romInit.s */
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

/*****************************************************************************/
#define SYS_OR1_BASE_ADRS        0x90000000
#define SYS_OR2_BASE_ADRS        0x91000000
#define SYS_OR4_BASE_ADRS        0x92000000 
/*****************************************************************************/

#define FSL_ERRATA_DDR_A003474

/*
 * Errata: A-003999 (Running Floating Point instructions requires special 
 * initialization), This workaround is implemented in boortom(romInit.s) 
 * and vxWorks image(sysLib.c).
 */

#define FSL_ERRATA_FPU_A003999

/* Common Includes for VXBUS PCI and DTSEC */

#define INCLUDE_VXBUS
#define INCLUDE_VXBUS_SHOW
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS

#define INCLUDE_PLB_BUS

#define INCLUDE_PCI_BUS

#define INCLUDE_WDB_ALWAYS_ENABLED

#ifdef INCLUDE_PCI_BUS
#define INCLUDE_PCI_BUS_AUTOCONF
#undef  DRV_PCIBUS_QORIQ_PCIEX
#define INCLUDE_PCI_OLD_CONFIG_ROUTINES
#define INCLUDE_PCI_BUS_SHOW
#endif /* INCLUDE_PCI_BUS */

#define INCLUDE_SHELL           /* interactive c-expression interpreter */

#ifdef  INCLUDE_SHELL
#define INCLUDE_SHELL_VI_MODE   /* vi editing mode for the shell */
#define INCLUDE_SHELL_EMACS_MODE /* emacs editing mode for the shell */
#define INCLUDE_SHELL_INTERP_C  /* C interpreter */
#define INCLUDE_SHELL_INTERP_CMD /* shell command interpreter */
#endif


#define FOLDER_TELNET

#ifdef FOLDER_TELNET
#define INCLUDE_IPTELNETS
#endif

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
#define INCLUDE_P5020_FMAN_UCODE
#define DRV_MII_DTSEC_MDIO
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#define INCLUDE_END

/*
 * add CDF_OVERRIDE for remove the following define when prj build, but used
 * for commandline build.
 */

#ifndef CDF_OVERRIDE
#   define QORIQ_PCIEX1_ENABLE      TRUE
#   define QORIQ_PCIEX2_ENABLE      FALSE
#   define QORIQ_PCIEX3_ENABLE      FALSE
#   define QORIQ_PCIEX4_ENABLE      FALSE
#   define QORIQ_PCIEX1_UNIT_NUM    0
#   define QORIQ_PCIEX2_UNIT_NUM    1
#   define QORIQ_PCIEX3_UNIT_NUM    2
#   define QORIQ_PCIEX4_UNIT_NUM    3
#   define DTSEC3_IN_RGMII          TRUE
#endif /* CDF_OVERRIDE */

/*
 * The P5020 also has one 10 gigabit ethernet controllers.
 * To use them, one XAUI riser cards is needed,
 * which are not included by default with the P5020DS board.
 * Consequently, the TGEC driver is not on by default either.
 *
 * When it is enabled, the TGEC driver works in conjunction
 * with the DTEPHY driver for link management with miiBus.
 * If fiber media is used, TGEC_FIBER should be defined.
 */
 

#ifdef DRV_VXBEND_TGEC
#   define DRV_MII_DTEPHY
#   undef  TGEC_FIBER
#   define DRV_MII_TGEC_MDIO
#endif  /* DRV_VXBEND_TGEC */

#define ENET_DEFAULT0       WR_ENET0
#define ENET_DEFAULT1       WR_ENET1
#define ENET_DEFAULT2       WR_ENET2

#define HWMEM_POOL_SIZE     200000

/*
 * This is required for SMP mode. When tNet0 is scheduled, we don't
 * have any way of predicting which core it will be running on when
 * it wakes up. But the frame manager is programmed to enqueue frames
 * to a portal belonging to a specific CPU (when a frame queue is
 * created, it's typically targeted at a particular CPU's portal).
 * We need to avoid the case where tNet0 and some other task both
 * try to dequeue from the same portal while running on different
 * cores in an SMP configuration, since then there would be
 * overlapping accesses to the same portal. To do this, we must
 * enforce the rule that if a task wants to dequeue a frame
 * descriptor from a given portal, it has to be running on the
 * CPU corresponding to that portal. To do that, we have to make
 * sure tNet0 always runs on the CPU specified as the target of
 * the DTSEC driver's frame queues, which in an UP and SMP
 * configuration defaults to 0.
 */

#ifdef _WRS_CONFIG_SMP
#   define NET_DAEMONS_CPU_AFFINITY     TRUE
#endif /* WRS_VX_SMP */

#define DEFAULT_BOOT_LINE \
"dtsec(0,0)host:vxWorks h=192.168.4.252 e=192.168.4.240:0XFFFFFF00 u=ssd pw=ssd f=0x0"

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define USER_I_MMU_ENABLE
#define USER_D_MMU_ENABLE

#define CACHE_LIBRARY_MODE          (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE         _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE              (VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY)

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE           CACHE_LIBRARY_MODE
#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE           CACHE_LIBRARY_MODE

#define INCLUDE_L2_CACHE
#define INCLUDE_L3_CACHE
#define L3_WRITE_THRU_MODE  0   /* 1 => write thru, 0 => write back */

#define INCLUDE_BRANCH_PREDICTION


#ifdef INCLUDE_MMU_BASIC
#   define INCLUDE_AIM_MMU_CONFIG
#   define INCLUDE_AIM_MMU_MEM_POOL_CONFIG
#endif /* INCLUDE_MMU_BASIC */

/* Serial channel and TTY */

#undef  NUM_TTY
#define NUM_TTY                     4
#undef  CONSOLE_TTY
#define CONSOLE_TTY                 0
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE           9600


/*ip ping */
#define INCLUDE_END2
#define DRV_VXBEND2_TEI8259X

#define INCLUDE_MV88E1X11PHY

#ifndef COMPILE_BOOTROM
#define INCLUDE_SPY
#endif



/* Clock rates */

#ifdef  INCLUDE_AUX_CLK
#   define DRV_TIMER_OPENPIC
#endif  /* INCLUDE_AUX_CLK */

#define SYS_CLK_RATE_MIN            1
#define SYS_CLK_RATE_MAX            8000
#define AUX_CLK_RATE_MIN            1
#define AUX_CLK_RATE_MAX            50000

/* vxBus I2C support */

#ifdef INCLUDE_I2C_BUS
#   define DRV_I2CBUS_FSL
#   define DRV_I2C_RTC
#   define INCLUDE_TIMER_RTC
#   define DRV_I2C_EEPROM
#   define INCLUDE_EEPROMDRV
#endif /* INCLUDE_I2C_BUS */

/* SPI support */

#ifdef DRV_SPI_S25FLXX
#   define INCLUDE_FSL_SPI
#endif  /* DRV_SPI_S25FLXX */

/* DMA support */

#undef DRV_DMA_FSL

/* USB EHCI */

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_MS_BULKONLY
#   define INCLUDE_USB_MS_BULKONLY_INIT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_USB */

/* USB Target */

#ifdef INCLUDE_FSL_TCD
#   ifndef _ASMLANGUAGE
IMPORT void fslUsbDrDeviceHwInit (void);
#   endif
#   define FSL_USB_DR_DEVICE_IO_BASE      USB2_BASE
#   define FSL_USB_DR_DEVICE_INT_NUM      EPIC_USB2_INT_VEC
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
#endif  /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_MS_EMULATOR
#   define INCLUDE_DOSFS
#   define INCLUDE_XBD_RAMDRV
#endif /* INCLUDE_MS_EMULATOR */

/* SATA */
#ifdef DRV_STORAGE_FSLSATA
#   define INCLUDE_DOSFS
#endif /* DRV_STORAGE_FSLSATA */

/* eSDHC */

#ifdef DRV_STORAGE_SDHC
#   define INCLUDE_DOSFS
#endif  /* DRV_STORAGE_SDHC */

/* TrueFFS flash support */
#if 0
#define INCLUDE_STARTUP_SCRIPT
#endif
#define INCLUDE_TFFS
#ifdef  INCLUDE_TFFS
/* board specific flash configuration needed by MTD driver */
#define S29GL_FLASH_BIT      1           /* 16-bit */
#define S29GL_FLASH_NUM      1           /* only one chip */
#define S29GL_FLASH_WIDTH    2           /* 16-bit */
#define S29GL_SECTOR_SIZE    0x00020000 /* 128K per sector */
#define S29GL_FLASH_SIZE     0x01000000  /* only 8MB used for TrueFFS */
#define S29GL_FLASH_OP_DELAY

#define INCLUDE_TL_FTL
#define INCLUDE_TFFS_MOUNT
#define INCLUDE_DOSFS
#endif  /* INCLUDE_TFFS */

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
#ifndef COMPILE_BOOTROM
#   define INCLUDE_STARTUP_SCRIPT
#endif
#   ifdef  BOOTAPP
#      define INCLUDE_BOOT_FILESYSTEMS
#   endif  /* BOOTAPP */
#endif  /* INCLUDE_DOSFS */

#define FLASH_WINDOW_SIZE           0x00800000

/* Used for LBC */

#define  M85XX_LAWBAR2H(base)        (CAST(VUINT32 *)((base) + 0xc40))
#define  M85XX_LAWBAR2L(base)        (CAST(VUINT32 *)((base) + 0xc44))
#define  M85XX_LAWAR2(base)          (CAST(VUINT32 *)((base) + 0xc48))

/* Used for RAPIDIO */

#define  M85XX_LAWBAR5H(base)        (CAST(VUINT32 *)((base) + 0xc70))
#define  M85XX_LAWBAR5L(base)        (CAST(VUINT32 *)((base) + 0xc74))
#define  M85XX_LAWAR5(base)          (CAST(VUINT32 *)((base) + 0xc78))


/* Used for RAPIDIO */
#define  M85XX_LAWBAR4H(base)        (CAST(VUINT32 *)((base) + 0xc60))
#define  M85XX_LAWBAR4L(base)        (CAST(VUINT32 *)((base) + 0xc64))
#define  M85XX_LAWAR4(base)          (CAST(VUINT32 *)((base) + 0xc68))



/* LBC CS3 - for PIXIS interface (romInit.s) */
#define INCLUDE_LBC_CS3

/* LBC CS0 - flash 0 - default for bootrom */
#define LAWAR0_BASE_ADRS            0XF0000000
#define FLASH_BASE_ADRS             0xF0000000
#define FLASH_BASE_ADRS1            0xF8000000
#define FLASH_ADRS_MASK             0xf0000000
#define MAIN_FLASH_SIZE             0x10000000

#define TFFS_FLASH_SIZE             0x04000000
#define TFFS_FLASH_BASE_ADDR        0xf8000000

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

#define FLASH_BOOT_OFFSET              0xffe00000
#define FSL_BOOT_ADDR                  0xfff00000
#define FSL_BLK_SIZE                   0x20000
#define FLASH_PROG_PRINT               1
#define FLASH_ERASE_PRINT              2
#define FSL_MAX_BOOT_LEN               (1*1024*1024)
#define FSL_MAX_RCW_LEN                80
#define FLASH_SECTOR_BYTE              0x20000

#   define NV_RAM_ADRS              0
#   define FLASH_SEGMENT_SIZE       0x10000
#   define FLASH_ADRS               0xffef0000
#   define NV_IP_ADRS_OFFSET        0x400

#   define FLASH_SIZE               0x10000
#   define FLASH_SIZE_WRITEABLE     FLASH_SIZE  /* write all of flash */
#   define NV_RAM_SIZE              FLASH_SIZE
#   undef  FLASH_NO_OVERLAY
#   undef  NV_BOOT_OFFSET
#   define NV_BOOT_OFFSET           0
#else /* INCLUDE_FLASH */
#   define NV_RAM_SIZE              0
#endif /* INCLUDE_FLASH */

/* Memory addresses */

#define PHYS_MEM_START              0x00000000
#define USER_RESERVED_MEM           0x00000000
#define DDR_SDRAM_LOCAL_END         0x7fffffff
#define PHYS_MEM_SIZE               0x80000000
#define DEFAULT_LOCAL_MEM_SIZE      PHYS_MEM_SIZE
#define DDR_SDRAM_LOCAL_ADRS        PHYS_MEM_START
#define DDR_SDRAM_LAWAR_SIZE        LAWAR_SIZE_2GB

/* user reserved memory, see sysMemTop() */

/*
 * Address of QMAN and BMAN private memory.
 * The QMAN driver will allocate 2MB starting from QMAN_MEM_BASE.
 * The BMAN driver will allocate 1MB starting from BMAN_MEM_BASE.
 * Changing the MEM_BASE's will require reflashing the bootrom.
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
 * at a place immediately before the end of physical memory. Another
 * method would have been to place it below RAM_LOW_ADRS. Users without
 * Wind River's legacy bootrom considerations should feel free to
 * relocate and reserve the private space however best suits their
 * application.
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

#undef INCLUDE_CPU_PWR_MGMT
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

/*
* Do not clear memory in romStart() since it is not necessary on physical
* target, and slows boot time. But when runs on Simics module, not clear
* memory will lead image loading failed, it could be caused by DDR device
* not be simulated perfectly on Simics module. So macro ROMSTART_BOOT_CLEAR
* should be defined on Simics P5020DS module.
*/

#undef  ROMSTART_BOOT_CLEAR   /* see romInit.s */

#define ROM_TEXT_ADRS               0xFFF00100
#define ROM_BASE_ADRS               0xFFF00000
#define ROM_SIZE                    0x00100000

/* parameters for SRIO share memory net */

#define SM_OFF_BOARD            FALSE
#define SM_TAS_TYPE             SM_TAS_HARD
#undef SM_BASE_ADRS
#ifndef _ASMLANGUAGE
    IMPORT char * rioHostAdrs;
#   undef SM_BASE_ADRS
#   define SM_BASE_ADRS         ((sysProcNumGet() == 0) ? \
                                ((char*) (LOCAL_MEM_LOCAL_ADRS)) : \
                                ((char*) (rioHostAdrs)))
#endif /* _ASMLANGUAGE */
#define SM_INT_TYPE             SM_INT_USER_1 /* or SM_INT_NONE Polling mode */
#define SM_MEM_ADRS             0x00010000    /* Start of actual SM region */
#define SM_MEM_SIZE             0x00070000
#define SM_OBJ_MEM_ADRS         (SM_MEM_ADRS + SM_MEM_SIZE) /* SM Objects pool */
#define SM_OBJ_MEM_SIZE         0x00040000
#ifndef _ASMLANGUAGE
#   undef SM_INT_ARG1
#   define SM_INT_ARG1          (SM_MEM_ADRS - 8) /* smEnd arg 1 Rman pDev */
#endif /* _ASMLANGUAGE */
#define SM_INT_ARG2             (SM_MEM_ADRS - 12)/* smEnd arg 2 Device Id */
#define SM_INT_ARG3             0                 /* smEnd arg 3 */
#define SM_SRIO_DESTID          0x15                 /* remote target device Id */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
    #include "prjParams.h"
#endif /* PRJ_BUILD */
