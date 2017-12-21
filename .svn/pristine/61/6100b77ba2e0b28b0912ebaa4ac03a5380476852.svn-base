/* apm_serengeti/config.h - APM86290 Serengeti board configuration header */

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
01n,07apr13,d_l  uprev BSP_REV to "6.9/2".
01m,29feb12,syt  updated SATA driver component.
01l,06feb12,syt  added SDHC,SATA,I2C,SPI and TFFS support.
01k,04jan12,x_s  added MMU OPTIMIZATION and Text Lock features by default.
01j,20dec11,x_z  added support for QMAN and on-chip MAC and updated BSP
                 revision.
01i,06dec11,x_s  added AMP support.
01h,13sep11,syt  added conditional compilation CDF_OVERRIDE for
                 LOCAL_MEM_LOCAL_ADRS and LOCAL_MEM_SIZE. (WIND00302746)
01g,11sep11,syt  modified Aux and WatchDog input clock frequency defines.
                 (WIND00295330)
01f,07sep11,x_s  added SMP support.
01e,29aug11,x_s  enabled L2 CACHE.
01d,08aug11,syt  modified conditional compilation about DRV_PCIBUS_APM_PCIEX
01c,30jul11,x_z  add PCI Express support and use GEI825xx as default ethernet
                 port.
01b,18jul11,b_m  add usb host support.
01a,16jun11,syt  initial creation.
*/

/*
DESCRIPTION
This file contains the configuration parameters for the
apm_serengeti board.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION     "6.9"       /* VxWorks 6.9 */
#define BSP_REV         "/2"        /* 0 for first revision */

#define SYS_MODEL       SYS_MODEL_86290

#include "configAll.h"
#include "serengeti.h"

#define DEFAULT_BOOT_LINE \
            "apmMac(0,0)host:vxWorks h=192.168.0.100 e=192.168.0.1 \
            u=target pw=vxTarget f=0x0 tn=Serengeti"

/* floating point support */

#undef  INCLUDE_SW_FP
#define INCLUDE_HW_FP

#define INCLUDE_VXBUS
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_PARAM_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_INTCTLR_LIB
#define HWMEM_POOL_SIZE             50000

#define INCLUDE_TIMER_SYS
#define DRV_TIMER_PPC440

#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_MPIC

#define DRV_SIO_NS16550
#define INCLUDE_SIO_UTILS

#define DRV_APM_PPRO_QMAN
#define DRV_VXBEND_APM_PPRO_MAC
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY

#undef  DRV_PCIBUS_APM_PCIEX

#ifdef DRV_PCIBUS_APM_PCIEX
#   define INCLUDE_PCI_BUS
#   define INCLUDE_PCI_BUS_AUTOCONF
#   define INCLUDE_VXB_LEGACY_INTERRUPTS
#endif /* DRV_PCIBUS_APM_PCIEX */

/* SDHC support */

#ifdef DRV_STORAGE_SDHC
#   define  INCLUDE_DOSFS
#endif /* DRV_STORAGE_SDHC */

/* SATA support */

#ifdef INCLUDE_DRV_STORAGE_AHCI
#   define  INCLUDE_DOSFS
#endif /* INCLUDE_DRV_STORAGE_AHCI */

/* TrueFFS */

#ifdef INCLUDE_TFFS
#   define INCLUDE_DOSFS
#   define INCLUDE_TL_FTL
#   define INCLUDE_TFFS_MOUNT
#   define TFFS_ADRS_OFFSET        S29GL_SECTOR_SIZE         
#endif  /* INCLUDE_TFFS */

#undef INCLUDE_USB

#ifdef INCLUDE_USB
#   define INCLUDE_USB_INIT
#   define INCLUDE_OHCI
#   define INCLUDE_OHCI_INIT
#   define INCLUDE_EHCI
#   define INCLUDE_EHCI_INIT
#   define INCLUDE_USB_GEN2_STORAGE
#   define INCLUDE_USB_GEN2_STORAGE_INIT
#   define INCLUDE_DOSFS
#endif  /* INCLUDE_USB */

#undef  NUM_TTY
#define NUM_TTY                     2
#undef  CONSOLE_TTY
#define CONSOLE_TTY                 0       /* 0 = UART1, 1 = UART2 */
#undef  CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE           57600

/* SPI */

#undef  INCLUDE_APM_SPI

/* I2C */

#undef  INCLUDE_APM_I2C

/*
 * If network support is included, either the native FEMAC or the PCIe Ethernet
 * should be chosen. Both are END style drivers.
 */

#define INCLUDE_END

/* default mac address */

#define ENET_DEFAULT0           0x00
#define ENET_DEFAULT1           0x01
#define ENET_DEFAULT2           0x73

/* bootrom 'M' command support */

#define ETHERNET_MAC_HANDLER

#define CUST_ENET3              0x01
#define CUST_ENET4              0xEF
#define CUST_ENET5_0            0x86
#define CUST_ENET5_1            0x87

#define MAX_MAC_ADRS            2
#define MAX_MAC_DEVS            1
#define MAC_ADRS_LEN            6

/* timers */

/*  System Timer */

#define SYS_CLK_RATE_MIN            3       /* minimum system clock rate     */
#define SYS_CLK_RATE_MAX            5000    /* maximum system clock rate     */

/* Auxiliary Timer */

#define AUX_CLK_RATE_MIN            (CPU_CLK_FREQ / (1 << 25))  /* minimum auxiliary clock rate */
#define AUX_CLK_RATE_MAX            (CPU_CLK_FREQ / (1 << 13))  /* maximum auxiliary clock rate */

/* Watchdog Timer */

#define WDT_RATE_MIN                (CPU_CLK_FREQ / (1 << 29))  /* minimum watchdog timer rate */
#define WDT_RATE_MAX                (CPU_CLK_FREQ / (1 << 21))  /* maximum watchdog timer rate */

/* MMU and CACHE */

#define INCLUDE_MMU_BASIC
#define INCLUDE_CACHE_SUPPORT

#ifdef  INCLUDE_MMU_BASIC
#   define USER_I_MMU_ENABLE
#   define USER_D_MMU_ENABLE
#   define INCLUDE_AIM_MMU_CONFIG

#   ifndef  BOOTAPP
#       define  INCLUDE_MMU_OPTIMIZE

    /*
     * INCLUDE_LOCK_TEXT_SECTION Requires use of CAM entries which can
     * possibly be already used in sysStaticTlbDesc. Also the larger the
     * page size that can be used the less CAM entries required so aligning
     * RAM_LOW_ADRS on a large page size boundary is usually necessary.
     * Locking the text section should increase performance because no
     * Instruction TLB misses are taken within the text section.
     */

#       define  INCLUDE_LOCK_TEXT_SECTION

    /*
     * INCLUDE_PAGE_SIZE_OPTIMIZATION - This routines calls one time
     * vmPageOptimize for the entire kernel mapped address space.
     * It will use where applicable large MMU TLB page sizes to map memory.
     * This routine should reduce TLB misses. Any modification of the kernel
     * MMU state will unoptimize the related MMU pages.
     */

#       define  INCLUDE_PAGE_SIZE_OPTIMIZATION
#   endif /* BOOTAPP */

    /* This write protects the page tables */

#   define INCLUDE_AIM_MMU_PT_PROTECTION

#   define  INCLUDE_AIM_MMU_MEM_POOL_CONFIG

    /*
     * The following parameters are to configure initial memory usage for
     * page table and region tables and in event of running out the increment
     * of memory allocation and is specified as a number of MMU pages (4KByte
     * blocks).
     */

#   define  AIM_MMU_INIT_PT_NUM     0x40
#   define  AIM_MMU_INIT_PT_INCR    0x20
#   define  AIM_MMU_INIT_RT_NUM     0x10
#   define  AIM_MMU_INIT_RT_INCR    0x10

#endif /* INCLUDE_MMU_BASIC */

#ifdef  INCLUDE_CACHE_SUPPORT
#   define USER_I_CACHE_ENABLE
#   undef  USER_I_CACHE_MODE
#   define USER_I_CACHE_MODE        CACHE_COPYBACK

#   define USER_D_CACHE_ENABLE
#   undef  USER_D_CACHE_MODE
#   define USER_D_CACHE_MODE        CACHE_COPYBACK

#   define INCLUDE_L2_CACHE
#endif /* INCLUDE_CACHE_SUPPORT */

/* unnecessary for project build */

#ifndef CDF_OVERRIDE
#   define RAM_LOW_ADRS_CPU0        0x00100000
#endif /* CDF_OVERRIDE */

#define CPUn_SPACE(n)               ((RAM_LOW_ADRS_CPU0 - (0x10000 * n)))
#define CPU_SPACE_SIZE_SHIFT        16  /* 0x10000 = 1 << 16*/
#define CPU_FUNC_START_ADR_OFFSET   0
#define CPU_STACK_START_ADR_OFFSET  4
#define CPU_INIT_START_ADR_OFFSET   8
#define CPU_INIT_STATUS_ADR_OFFSET  12

#define CPUn_FUNC_START_ADR(n)      (CPUn_SPACE(n) + CPU_FUNC_START_ADR_OFFSET)
#define CPUn_STACK_START_ADR(n)     (CPUn_SPACE(n) + CPU_STACK_START_ADR_OFFSET)
#define CPUn_INIT_START_ADR(n)      (CPUn_SPACE(n) + CPU_INIT_START_ADR_OFFSET)
#define CPUn_INIT_STATUS_ADR(n)     (CPUn_SPACE(n) + CPU_INIT_STATUS_ADR_OFFSET)

/* Define the support of NorFlash */

#define FLASH_BASE_ADRS             0xfc000000
#define FLASH_SIZE                  0x04000000

#define ROM_BASE_ADRS               0xFFF00000
#define ROM_SIZE                    0x00100000
#define ROM_TEXT_ADRS               ROM_BASE_ADRS
#define ROM_WARM_ADRS               (ROM_TEXT_ADRS + 0x0004)

/* board specific flash configuration needed by MTD driver */

#define S29GL_FLASH_BIT         1           /* 16-bit */
#define S29GL_FLASH_NUM         1           /* only one chip */
#define S29GL_FLASH_WIDTH       2           /* 16-bit */
#define S29GL_SECTOR_SIZE       0x00020000  /* 128K per sector */
#define S29GL_FLASH_OP_DELAY
#define S29GL_FLASH_SIZE        (FLASH_SIZE - ROM_SIZE - S29GL_SECTOR_SIZE)

/* NVRam */

#define FLASH_ADRS              FLASH_BASE_ADRS
#define FLASH_WIDTH             S29GL_FLASH_WIDTH
#define FLASH_CHIP_WIDTH        S29GL_FLASH_WIDTH
#define FLASH_SECTOR_SIZE       S29GL_SECTOR_SIZE
#define NV_RAM_ADRS             (ROM_BASE_ADRS - FLASH_SECTOR_SIZE)
#define NV_RAM_SIZE             FLASH_SECTOR_SIZE
#define NV_MAC_ADRS_OFFSET      0x200
#define FLASH_SECTOR_MASK       0xfffe0000

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
#   define INCLUDE_DOSFS_MAIN
#   ifdef  BOOTAPP
#       define INCLUDE_BOOT_FILESYSTEMS
#   endif  /* BOOTAPP */
#endif /* INCLUDE_DOSFS */

/*
 * HW Debugger Support
 *
 * When CLEAR_DEBUG_REGISTERS is defined, the bootrom and kernel clear
 * the onchip debugging registers.  This is ideal for a delivered
 * application as it ensures that those chip resources are inert.
 *
 * When it is not defined, the bootrom and kernel do not initialize the
 * onchip debug control registers. This is the most appropriate setting
 * when using hardware assisted debugging.
 */

#undef CLEAR_DEBUG_REGISTERS

/*
 * 440EP Machine Check Handler configuration options.
 * See the PPC465 User's Manual for more information.
 *
 * INCLUDE_440X5_DCACHE_RECOVERY
 *
 *  Makes data cache parity errors recoverable.  Selecting this
 *  option will also select INCLUDE_440X5_PARITY_RECOVERY, and
 *  set USER_D_CACHE_MODE to CACHE_WRITETHROUGH.
 *
 * INCLUDE_440X5_TLB_RECOVERY
 *
 *  Makes TLB parity errors recoverable.  Selecting this option will
 *  also select INCLUDE_440X5_PARITY_RECOVERY and INCLUDE_MMU_BASIC.
 *  (If the functionality delivered by the MMU library is not desired, it
 *  is permissible to #undef both USER_D_MMU_ENABLE and USER_I_MMU_ENABLE.)
 *
 * INCLUDE_440X5_PARITY_RECOVERY
 *
 *  Sets the PRE bit in CCR0.  This is required by the 440GX hardware
 *  if either TLB or DCACHE recovery is to be enabled.  Selecting this
 *  option will also select INCLUDE_EXC_HANDLING.
 *
 * INCLUDE_440X5_TLB_RECOVERY_MAX
 *
 *  Dedicates a TLB entry to the Machine Check handler, and a separate
 *  TLB entry to the remaining interrupt/exception vectors, to maximize
 *  recoverability from TLB parity errors.  Selecting this option will
 *  also select INCLUDE_440X5_TLB_RECOVERY.  This option does not change
 *  the physical location of the vectors, however:
 *
 *  * VEC_BASE_ADRS, which defines the (virtual) address at which the
 *    vector code will execute, must be set to a value higher than the
 *    physical memory size and different than VEC_BASE_TASK_ADRS.
 *
 *  * This option increases the number of static TLB entries by two,
 *    correspondingly reducing the number of dynamic entries available.
 *    The required TLB entries must be defined in sysStaticTlbDesc[]
 *    and coordinated with the value of VEC_BASE_ADRS.
 *
 * INCLUDE_440X5_MCH_LOGGER
 *
 *  Causes the Machine Check handler to log recovered events.
 *
 *   _PPC_440X5_MCH_LOG_RTN
 *
 *  Log function to call, or (FUNCPTR)1 to use logMsg().
 *
 *  The log function executes in an environment similar to that of a
 *  watchdog or an interrupt service routine.  It must not attempt to
 *  delay or suspend itself, and it should perform only the minimum
 *  necessary operations before returning.  It may write to a message
 *  queue provided the timeout parameter is specified as NO_WAIT, or
 *  it may give (but not take) a semaphore.
 *
 *  NOTE: The (FUNCPTR)1 convention is needed because of initialization
 *  sequencing.  Setting this parameter directly to logMsg() may result
 *  in logMsg() being called before it has been properly initialized.
 *
 * While there are performance costs associated with setting CCR0[PRE]
 * and with operating the data cache in writethrough mode, the default and
 * WRS-recommended configuration is to enable all 4 recovery options.
 */

#undef  INCLUDE_440X5_DCACHE_RECOVERY
#undef  INCLUDE_440X5_TLB_RECOVERY
#define INCLUDE_440X5_PARITY_RECOVERY
#undef  INCLUDE_440X5_TLB_RECOVERY_MAX
#undef  INCLUDE_440X5_MCH_LOGGER

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
#   undef  VEC_BASE_ADRS
#   define VEC_BASE_ADRS            0xcfff0000
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */

#ifndef CDF_OVERRIDE

/*
 * On APM86290, the SERDES PHY of SATA port 1 is muxed with PCIE0's,
 * to enable PCIE0, parameter SATA_PORT1_ENABLE should be set as
 * FALSE and PCIEX0_PORT0_ENABLE as TRUE, vice versa.
 */

#   ifdef INCLUDE_DRV_STORAGE_AHCI
#       define SATA_PORT1_ENABLE    TRUE
#   endif /* INCLUDE_DRV_STORAGE_AHCI */

#   ifdef DRV_PCIBUS_APM_PCIEX
#       define PCIEX0_PORT0_ENABLE  FALSE
#   endif /* DRV_PCIBUS_APM_PCIEX */

#   define LOCAL_MEM_LOCAL_ADRS_HI  0x00
#   define LOCAL_MEM_LOCAL_ADRS     PHY_MEM_BASE
#   define LOCAL_MEM_SIZE           PHY_MEM_SIZE
#endif /* CDF_OVERRIDE */

/*
 * Base address for accessing the exception/interrupt vector space from
 * code executing outside of it, typically zero.  VEC_BASE_ADRS will
 * normally be the same except when using INCLUDE_440X5_TLB_RECOVERY_MAX.
 */

#define VEC_BASE_ALT_ADRS           LOCAL_MEM_LOCAL_ADRS

#undef VEC_BASE_ADRS
#define VEC_BASE_ADRS               LOCAL_MEM_LOCAL_ADRS

#define USER_RESERVED_MEM           0x00000000

#ifdef __cplusplus
    }
#endif

#endif  /* __INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif
