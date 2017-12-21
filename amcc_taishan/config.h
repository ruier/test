/* config.h - amcc_taishan BSP for AMCC 440GX EVB configuration header */

/*
****************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

****************************************************************************
\NOMANUAL
*/

/*
 * Copyright 2007-2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,26mar12,c_t  disable TAH by default (WIND00272618)
01j,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
01i,15apr10,clx  update BSP version for vxWorks 6.9
01h,15jan10,liu  update the BSP revision
01g,16dec08,b_m  add vxbus timer & uic support.
01f,10oct08,syt  updated for new VxBus Intel fei_8255x net card driver
01e,28aug08,x_s  undef INCLUDE_TFFS by default 
01d,20jue08,x_s  updated for new VxBus EMAC driver
01c,04dec07,x_s  added vxbus macro definition
01b,21may07,x_s  updates for BootApp
01a,17apr07,x_s  initial creation, based on Mainstone
*/

/*
This file contains the configuration parameters for the amcc_taishan BSP,
which supports the AMCC 440GX evaluation board.
*/

#ifndef INCconfigh
#define INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */
#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */

/* Base model string. sysModel adds processor details */
#define SYS_MODEL       "AMCC PowerPC 440GX"

#include "configAll.h"

/*
 * Define one or more of the following to specify the revision(s) of the
 * chip you are using.  These selections control inclusion of
 * errata workarounds.  sysModel() will return "Unsupported processor"
 * if the correct selection is not included.
 */

#define PPC440GX_REVA   /* Needed only for PPC440GX Rev A (1.0)    */
#define PPC440GX_REVB   /* Needed only for PPC440GX Rev B (2.0)    */
#define PPC440GX_REVF   /* Needed only for PPC440GX Rev F (2.0)    */

/*
 * Chip specific workarounds.
 *
 * Note: Rev B errata document has not been consulted yet; may be able
 * to remove some workarounds.
 */
 
#if (defined(PPC440GX_REVA) || defined (PPC440GX_REVB))
#define DDR_RESET_ERRATA     /* errata DDR_1 */
#define TRACEB_EBMI_ERRATA   /* errata CHIP_1 */
#endif

 /*
  * Cicada PHY errata.
  * Workaround for Cicada PHY errata when RGMII mode 10Mps 1/2 duplex
  */
  
#if ((EMAC2_PHY_MODE == PHY_MODE_RGMII) || (EMAC3_PHY_MODE == PHY_MODE_RGMII))
   #define CICADA_PHY_RGMII_10MPS_12DUPLEX_ERRATA
#endif

/* Default boot line */

#define DEFAULT_BOOT_LINE \
    "emac(0,0)host:vxWorks h=10.1.1.2 e=10.1.1.10 u=target"

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

#define INCLUDE_VXBUS

#define ETHERNET_MAC_HANDLER
#define MAX_MAC_ADRS 2
#define MAC_ADRS_LEN 6
#define MAX_MAC_DEVS 2 /* two network devices (fcc, scc) */
#define NV_MAC_ADRS_OFFSET NV_ENET_OFFSET_0
#define NV_BOOT_OFFSET              0

#ifdef  INCLUDE_VXBUS
#define INCLUDE_PARAM_SYS
#define INCLUDE_HWMEM_ALLOC
#define INCLUDE_DMA_SYS
#define INCLUDE_PLB_BUS
#define INCLUDE_PCI_BUS
#define INCLUDE_PCI_BUS_AUTOCONF
#define INCLUDE_PPC440GX_PCI 
#define HWMEM_POOL_SIZE 50000
#define INCLUDE_PCI_OLD_CONFIG_ROUTINES 
#define INCLUDE_VXB_CMDLINE
#define INCLUDE_TIMER_SYS
#define DRV_TIMER_PPC440
#define INCLUDE_INTCTLR_LIB
#define INCLUDE_VXB_LEGACY_INTERRUPTS
#define DRV_INTCTLR_PPC
#define DRV_INTCTLR_UIC
#endif /* INCLUDE_VXBUS */
/*
 * Memory configuration.
 *
 * If LOCAL_MEM_AUTOSIZE is defined, SDRAM size is determined at boot
 * time, and overrides the default set in LOCAL_MEM_SIZE.
 */
#define LOCAL_MEM_AUTOSIZE          /* run-time memory sizing     */

#define LOCAL_MEM_SIZE      0x01000000  /* memory default         */
#define LOCAL_MEM_LOCAL_ADRS    0x00000000  /* fixed at zero          */
#define USER_RESERVED_MEM   0       /* see sysMemTop()        */


/*
 * 440GX Machine Check Handler configuration options.
 * See the 440GX User's Manual for more information.
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
 *  recovering from TLB parity errors.  Selecting this option will
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

#define INCLUDE_440X5_DCACHE_RECOVERY
#define INCLUDE_440X5_TLB_RECOVERY
#define INCLUDE_440X5_PARITY_RECOVERY
#define INCLUDE_440X5_TLB_RECOVERY_MAX
#define INCLUDE_440X5_MCH_LOGGER
#define _PPC_440X5_MCH_LOG_RTN  (FUNCPTR)1  /* use logMsg when available */

/*
 * Base address for accessing the exception/interrupt vector space from
 * code executing outside of it, typically zero.  VEC_BASE_ADRS will
 * normally be the same except when using INCLUDE_440X5_TLB_RECOVERY_MAX.
 */
#define VEC_BASE_ALT_ADRS   0x00000000

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
#undef  VEC_BASE_ADRS
#define VEC_BASE_ADRS   0xcfff0000
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */


/*
 * The constants ROM_TEXT_ADRS, ROM_SIZE, RAM_HIGH_ADRS, and
 * RAM_LOW_ADRS are defined in config.h and Makefile.
 *
 * All definitions for these constants must be identical.
 *
 * The boot ROM is selectable with switch 3 on U46 on the board.
 * The jumper selects either 1MB flash (Am29LV040) OR 1MB of 8 bit
 * SRAM.  If switch 3 is in the ON position SRAM is selected.  If switch
 * 3 is in the OFF position the flash is selected.
 *
 * In order to write to the SRAM with a JTAG debugger, write EBC0_B0CR
 * 0xfff18000 and write EBC0_B0AP 0x9b01548.
 *
 * Boot ROM address space.
 */

#define ROM_BASE_ADRS         0xfff00000          /* base address of ROM  */
#define ROM_TEXT_ADRS         ROM_BASE_ADRS       /* with PC & SP         */
#define ROM_WARM_ADRS         (ROM_TEXT_ADRS+0x0004)  /* warm reboot entry    */
#define ROM_SIZE              0x00100000          /* 1MB              */

#define FLASH_BASE_ADRS       0xFC000000
/*
 * Cache options
 *
 * Note that if both I and D MMUs are 'enabled', the chip applies the
 * same cache mode to all accesses at a given virtual address -- there
 * is no way to selectively enable or disable the instruction cache
 * into a state different from the data cache.
 *
 * You can, however, leave the I MMU 'disabled' and D MMU 'enabled'.
 * In this case, you can manage the D MMU cache mode via the
 * virtual memory library, and manage the I MMU cache mode via
 * the sysStaticTlbDesc[] structure (see sysLib.c).
 */

#define INCLUDE_CACHE_SUPPORT

#define USER_D_CACHE_ENABLE
#define USER_I_CACHE_ENABLE
#undef USER_I_CACHE_MODE
#undef USER_D_CACHE_MODE
#define USER_I_CACHE_MODE   (CACHE_WRITETHROUGH)
#define USER_D_CACHE_MODE   (CACHE_COPYBACK)

/*
 * Define this to enable the L2 data arrays as L2 cache.
 *
 * The data arrays in the L2 controller can be set up to be used as
 * either cache OR SRAM.  They are configured initially as SRAM in the
 * boot sequence in romInit.s and if defined here will be set to act as
 * L2 cache at the end of sysHwInit
 */
#define  INCLUDE_L2_CACHE   /* Use SRAM area for L2 Cache */

/*
 * define this if you want the TLB set up so that boot space from
 * 0xf0000000 to 0xffffffff is writeable. This can be useful in
 * writing to the boot SRAM device with a JTAG debugger, or if
 * using the 4MB flash for TFFS.
 */
#define BOOT_SPACE_WRITEABLE


/*
 * Timers (DEC, FIT, WDT) can be driven from an external clock
 * source, or at the same frequency as the CPU.
 */

#undef  TIMER_CLOCK_EXTERNAL           /* define to use external 25 MHz clock */


/*
 * The clock used to drive the UARTs on the chip can be derived from
 * one of two different sources. The 11.0592 MHz external oscillator on
 * the board can be used, or the internal PLB clock can be used with an
 * additional internal UART divisor (see the CPC0_CR0 register). If you
 * want to use the external UART clock oscillator, define UART_CLOCK_EXTERNAL.
 */

#define UART_CLOCK_EXTERNAL   /* define to use external 11.0592 MHz clock */


/*
 * Auxiliary Timer rates. For PPC440, the FIT timer provides a small set
 * of periods divided off the timer clock.
 */

#define AUX_CLK_RATE_MIN     (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_3))
#define AUX_CLK_RATE_MAX     (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_0))
#define FIT_CLK_RATE_MIN     (800000000 / (1 << FIT_DIVIDER_TAP_3))
#define FIT_CLK_RATE_MAX     (800000000 / (1 << FIT_DIVIDER_TAP_0))

/*
 * Watchdog Timer rates. For PPC440, the FIT timer provides a small set
 * of periods divided off the timer clock.
 *
 * NOTE: The minimum timer of sysTimerClockFreq / (1<<33) is not supported.
 */

#define WDT_RATE_MIN         (sysTimerClkFreq / (1 << WDT_DIVIDER_TAP_2))
#define WDT_RATE_MAX         (sysTimerClkFreq / (1 << WDT_DIVIDER_TAP_0))
#define WDT_CLK_RATE_MIN     (800000000 / (1 << WDT_DIVIDER_TAP_2))
#define WDT_CLK_RATE_MAX     (800000000 / (1 << WDT_DIVIDER_TAP_0))

/* Serial port configuration */

#define INCLUDE_SIO_UTILS
#define DRV_SIO_NS16550

/* Optional TrueFFS support */

#undef INCLUDE_TFFS

#ifdef INCLUDE_TFFS
#   define INCLUDE_TFFS_MOUNT
#   define INCLUDE_FS_EVENT_UTIL   /* File system event utility */
#   define INCLUDE_FS_MONITOR      /* File system monitor */
#   define INCLUDE_ERF             /* Event reporting framework */
#   define INCLUDE_DEVICE_MANAGER  /* Device Manager */
#   define INCLUDE_XBD             /* Extended block device */
#   define INCLUDE_XBD_TRANS
#   define INCLUDE_XBD_BLK_DEV     /* XBD-block device wrapper */
#   define INCLUDE_DOSFS_MAIN
#   define INCLUDE_DOSFS_FAT
#   define INCLUDE_DOSFS_DIR_VFAT
#   define INCLUDE_DOSFS_DIR_FIXED
#   define INCLUDE_DOSFS_FMT
#   define INCLUDE_DOSFS_CHKDSK    /* file system integrity checking */
#   define INCLUDE_DOSFS_CACHE 
#   define INCLUDE_SHOW_ROUTINES
#endif /* INCLUDE_TFFS */

/* PCI configuration */

#define INCLUDE_PCI

#ifdef INCLUDE_PCI

/*   32 bit PCI bus memory map.
 *
 *    size      CPU (virt)   PLB (real)       PCI           usage       map by
 * 0x80000000   0x00000000  0x0.00000000  0x00000000   slave             PIM0
 * 0x02000000   0x90000000  0x3.00000000  0x90000000   Mem no prefetch   POM1
 * 0x02000000   0xA0000000  0x3.10000000  0xA0000000   Mem w/ prefetch   POM0
 * 0x00010000   0xD8000000  0x2.08000000  0x00000000   I/O               fixed
 * 0x03800000   0xD8800000  0x2.08800000  0x00800000   "Extra" I/O       fixed
 */

/*
 * The PCI_MSTR_* and PCI_SLV_* defines are used elsewhere to map directly
 * between CPU virtual address space and PCI address space.  Since the
 * CPU <=> PLB mapping is set up in romInit.s and does not change thereafter,
 * PLB address space need not be considered operationally once the bridge has
 * been set up.
 */

/*
 * Define a 2GB inbound memory window that allows PCI masters to access local
 * memory of the chip.  This window will be setup with the PIM0 registers in
 * the bridge.  Prefetching will be allowed.
 */

#    define PCI_SLV_MEM_LOCAL      LOCAL_MEM_LOCAL_ADRS
#    define PCI_SLV_MEM_BUS        0x00000000
#    define PCI_SLV_MEM_SIZE       MAPPED_SDRAM_SIZE       /* 2GB */

#    define PIM0_LOCAL_ADRS_LOW    PCI_SLV_MEM_LOCAL
#    define PIM0_LOCAL_ADRS_HIGH   0x00000000
#    define PIM0_PCI_ADRS_LOW      PCI_SLV_MEM_BUS
#    define PIM0_PCI_ADRS_HIGH     0x00000000
#    define PIM0_PCI_SIZE_ATTRIB   (PIM_SIZE_2GB | PIM_PREFETCH | PIM_ENABLE)

/*
 * Define a 32MB outbound memory window that allows the CPU to access PCI
 * Memory with prefetching.  This window will be setup with the POM0 registers
 * in the bridge.  The guarded bit should not be set in the TLB entries
 * covering this area.  The processor address of this region should reside
 * between PCI_MEMORY_START and PCI_MEMORY_END.
 */

#    define PCI_MSTR_MEM_LOCAL     0xA0000000           /* Processor address */
#    define PCI_MSTR_MEM_BUS       0xA0000000           /* PCI address       */
#    define PCI_MSTR_MEM_SIZE      0x02000000           /* 32MB */

#    define POM0_LOCAL_ADRS_LOW    0x10000000
#    define POM0_LOCAL_ADRS_HIGH   0x00000003
#    define POM0_PCI_ADRS_LOW      PCI_MSTR_MEM_BUS
#    define POM0_PCI_ADRS_HIGH     0x00000000
#    define POM0_PCI_SIZE_ATTRIB   (POM_SIZE_32MB | POM_ENABLE)

/*
 * Define a 32MB outbound memory window that allows the CPU to access PCI
 * Memory without prefetching. This window will be setup with the POM1
 * registers in the bridge.  The guarded bit should be set in the TLB entries
 * covering this area.  The processor address of this region should reside
 * between PCI_MEMORY_START and PCI_MEMORY_END.
 */

#    define PCI_MSTR_MEMIO_LOCAL   0x90000000           /* Processor address */
#    define PCI_MSTR_MEMIO_BUS     0x90000000           /* PCI address       */
#    define PCI_MSTR_MEMIO_SIZE    0x02000000           /* 32MB */

#    define POM1_LOCAL_ADRS_LOW    0x00000000       /* PLB address low */
#    define POM1_LOCAL_ADRS_HIGH   0x00000003       /* PLB address high */
#    define POM1_PCI_ADRS_LOW      PCI_MSTR_MEMIO_BUS
#    define POM1_PCI_ADRS_HIGH     0x00000000
#    define POM1_PCI_SIZE_ATTRIB   (POM_SIZE_32MB | POM_ENABLE)

/*
 * Master window allowing the CPU to access PCI I/O space.
 */

#    define PCI_MSTR_IO_LOCAL      PLB_PCI_IO_REGION_1_START /* Processor Adrs*/
#    define PCI_MSTR_IO_BUS        0x00000000                /* PCI address   */
#    define PCI_MSTR_IO_SIZE       PLB_PCI_IO_REGION_1_SIZE

/*
 * The third PCI master memory window is not used.
 */

#    define POM2_PCI_SIZE_ATTRIB  (POM_UNUSED)

/*
 * The second and third PCI slave windows are not being used.
 */

#    define PIM1_LOCAL_ADRS_LOW    0x00000000
#    define PIM1_LOCAL_ADRS_HIGH   0x00000000
#    define PIM1_PCI_ADRS          0x00000000   /* I/O space, so no low/high */
#    define PIM1_PCI_SIZE_ATTRIB   PIM_UNUSED

#    define PIM2_LOCAL_ADRS_LOW    0x00000000
#    define PIM2_LOCAL_ADRS_HIGH   0x00000000
#    define PIM2_PCI_ADRS_LOW      0x00000000
#    define PIM2_PCI_ADRS_HIGH     0x00000000
#    define PIM2_PCI_SIZE_ATTRIB   PIM_UNUSED


/* PCI Latency Timer value */
#    define PCI_LAT_TIMER          0

#endif /* INCLUDE_PCI */

/*
 * If network support is included, either the native EMAC or the PCI
 * Ethernet should be chosen. Both are END style drivers.
 */

#define INCLUDE_END

#ifdef  INCLUDE_END

# undef INCLUDE_LN97XEND             /* include lnPci driver */
# undef INCLUDE_FEI8255X_VXB_END     /* include fei driver */
#define INCLUDE_EMAC_VXB_END
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY 
#define INCLUDE_VXB_IBM_ZMII 
#define INCLUDE_VXB_IBM_RGMII 
#define INCLUDE_VXB_IBM_MAL
#undef  INCLUDE_VXB_IBM_TAH 

#define INCLUDE_EMAC_RXCHAN_GROUP

#ifdef INCLUDE_EMAC_RXCHAN_GROUP
    #define EMAC_RXCHAN_GROUP 1
#else
    #define EMAC_RXCHAN_GROUP 0
#endif /* INCLUDE_EMAC_RXCHAN_GROUP */

#endif /* INCLUDE_END */

/*
 * WDB communication
 */

#ifdef  INCLUDE_EMAC_VXB_END
# define  INCLUDE_ZMII       /* 440 EMAC requires ZMII support */
#endif  

#if (WDB_COMM_TYPE == WDB_COMM_SERIAL)
# define DRV_SIO_NS16550

# undef  WDB_TTY_BAUD
# define WDB_TTY_BAUD       38400

# undef  WDB_TTY_ECHO
# define WDB_TTY_ECHO       TRUE
#endif  /* WDB_COMM_TYPE == WDB_COMM_SERIAL */

/*
 * Base address for exception vector handlers
 */

#define VEC_BASE    (char *)0

/*
 * MMU support.
 *
 * On PowerPC Book E processors, MMU translation cannot be deactivated.
 * The VxWorks bootrom establishes 18 or 16 TLB entries (see resetEntry
 * in romInit.s). These entries statically map the entire effective
 * address space to physical addresses, allowing the boot process to
 * load a kernel.
 *
 * This process is repeated for the vxWorks kernel through the
 * sysStaticTlbDesc[] array (see sysLib.c). In VxWorks 5.5.1 this
 * includes VM-enabled mappings beyond the reach of vmBaseLib (above the
 * lowest 4GB of physical address space).
 *
 * Finer-grained storage attributes (caching, guarded,
 * execute/read/write) and mapping (4KB chunks) are managed by the vmLib
 * and vmBaseLib API.  These MMU libraries accomplish this through use
 * and reuse of the remaining TLB entries.  In VxWorks 5.5, only devices
 * with 32-bit physical addresses can be mapped (in the case of this chip,
 * DDR SDRAM, onchip SRAM, and I2O device space).  VxWorks 6.0 VM supports
 * 64-bit PHYS_ADDR in PPC so it can map all devices.
 *
 * For MMU_BASIC, setting the D-MMU as enabled and I-MMU as disabled gives
 * the best performance, as this reduces miss-replacement competition for
 * the 440's 64 TLB entries. For MMU_FULL with Real Time Processes
 * (not support in VxWorks 5.5.1), both D-MMU and I-MMU must be enabled.
 *
 * For more details, see sysPhysMemDesc[] in sysLib.c.
 */

#define INCLUDE_MMU_BASIC
#define USER_D_MMU_ENABLE
#undef  USER_I_MMU_ENABLE

#ifdef INCLUDE_MMU_BASIC

#define INCLUDE_AIM_MMU_CONFIG
#ifndef BOOTAPP
#define INCLUDE_MMU_OPTIMIZE 
#endif

#define INCLUDE_AIM_MMU_MEM_POOL_CONFIG

/* 
 * The following parameters are to configure initial memory usage for 
 * page table and region tables and in event of running out the increment 
 * of memory allocation and is specified as a number of MMU pages (4KByte
 * blocks). 
 */
#define AIM_MMU_INIT_PT_NUM 0x40
#define AIM_MMU_INIT_PT_INCR 0x20
#define AIM_MMU_INIT_RT_NUM 0x10
#define AIM_MMU_INIT_RT_INCR 0x10

/* This write protects the page tables */
#define INCLUDE_AIM_MMU_PT_PROTECTION

/* 
 * INCLUDE_LOCK_TEXT_SECTION Requires use of CAM entries which can 
 * possibly be already used in sysStaticTlbDesc. Also the larger the 
 * page size that can be used the less CAM entries required so aligning 
 * RAM_LOW_ADRS on a large page size boundary is usually necessary.
 * Locking the text section should increase performance because no 
 * Instruction TLB misses are taken within the text section.
 */

#ifndef BOOTAPP
#define INCLUDE_LOCK_TEXT_SECTION
#endif

/*
 * INCLUDE_PAGE_SIZE_OPTIMIZATION - This routines calls one time 
 * vmPageOptimize for the entire kernel mapped address space.
 * It will use where applicable large MMU TLB page sizes to map memory.
 * This routine should reduce TLB misses. Any modification of the kernel
 * MMU state will unoptimize the related MMU pages.
 */
       
#ifndef BOOTAPP
#define INCLUDE_PAGE_SIZE_OPTIMIZATION
#endif

#endif /* INCLUDE_MMU_BASIC */

/* Include the other configuration values */

#include "ibmEvb440gx.h"       /* board header file      */
#include "ppc440gx.h"          /* chip header file       */

#ifdef __cplusplus
    }
#endif

#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif
