/* config.h - IBM Walnut/Sycamore 405GP/405GPr configuration header */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBMS OR ITS LICENSORS
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

********************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 1999-2004, 2006-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02o,19jul12,clx  update BSP_REV
02n,02feb12,x_s  Remove the component INCLUDE_AIM_MMU_SHOW. (WIND00334904)
02m,11jan11,pch  CQ250219: move defn of MAC_ADRS_LEN from sysNet.c to config.h
02l,04jan11,pch  CQ248235: add MAC set/get support
		 (from wrSbc405gp vn 01r,15dec06,rec)
02k,15apr10,clx  Update BSP version for vxWorks 6.9
02j,20apr09,wap  Switch to VxBus EMAC driver
02i,12feb09,syt  update BSP version
02h,17oct08,pch  CQ135132: fix RAM_HIGH_ADRS definitions
02g,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
02f,21apr08,fao  add vxbus support
02e,23mar07,pmr  fix bootapp support
02d,09nov06,rec  vxWorks 6.5 update
02c,24apr06,jmt  Add SYS_MODEL definition for this BSP
02b,04oct04,dtr  SPR 99491. AIM MMU configuration and text/data alignment.
02a,03sep04,mdo  Documentation fixes for apigen
01z,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
01y,24jun04,mdo  SPR #98438
01x,08jun04,mdo  Change MMU configuration - dependency is elsewhere.
01w,10dec03,jtp  Support MMU_FULL
01v,20sep02,pch  SPR 82335: fix warm boot offset
01u,20aug02,pch  405GP / 405GPr interoperability
01t,18jun02,mcg  add support for 405GPr
01s,12jun02,pch  SPR 74987: C++
01r,02may02,pch  SPR 73648: common build for both Rev D and Rev E processors
01q,11jan02,pch  add INCLUDE_AUX_CLK to support sharing of ppc405Timer.c
		 with wrSbc405gp
01p,30nov01,kab  Update for T2.2:
		 Removed support for Rev B and Rev C parts
		 BSP_REV 7
01o,20aug01,kab  Change ibmEmac to emac - max 4 letter boot dev in DHCP, other
01n,27jul01,kab  Remove 403_ON_405; bump BSP_REV
01m,10may01,mcg  Support for Emac driver data cache changes
01l,18dec00,s_m  undefined PPC405GP_REVE
01k,06dec00,s_m  added define for Rev E
01j,05dec00,s_m  uses external clock by default as this allows variable baud
		 rates
01i,29nov00,s_m  updates from ibm
01h,20nov00,s_m  changed BSP_REV to 5
01g,14nov00,s_m  don't include IMMU enable by default
01f,28nov00,mcg  fixed comments regarding ROM/SRAM switch 6 on bank U79
01e,24aug00,mcg  added PCI master window for VGA cards
01d,09may00,mcg  added information for PCI autoconfig
01c,28mar00,mcg  update for revision 2 of the Walnut BSP
01b,02feb00,mcg  update for revision 1 of the Walnut BSP
01a,08sep99,mcg  created from evb403 config.h version 01v.
*/

/*
DESCRIPTION
This file contains the configuration parameters for the
IBM Walnut/Sycamore 405GP/405GPr CPU evaluation boards.
*/

#ifndef __INCconfigh
#define __INCconfigh

#ifdef __cplusplus
    extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */

#define BSP_VERSION        "6.9"       /* vxWorks 6.9 */
#define BSP_REV            "/1"        /* 0 for first revision */

/* Base model string. sysModelGet adds processor details */

#define SYS_MODEL       "IBM PowerPC"

#include <vsbConfig.h>
#include "configAll.h"

/*
 * Define one or more of the following to specify the revision(s) of
 * the PPC405GP or PPC405GPr chip for which support is to be included.
 * Undefine the others.  sysModel() will return "Unknown processor" if
 * the correct selection is not included.
 *
 * Errata workarounds for rev B or C of the 405GP can be generated, but
 * these selections are no longer considered "supported" and should not
 * be combined with those for newer processors.
 */

#undef  PPC405GP_REVB            /* Only for PPC405GP Rev B */

#undef  PPC405GP_REVC            /* Only for PPC405GP Rev C */

#define PPC405GP_REVD_OR_E       /* PPC405GP Rev D or newer */

#define PPC405GPR_REVB           /* PPC405GPr Rev B (1.1)   */

#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC) || \
    defined (PPC405GP_REVD_OR_E)
#   define PPC405GP
#endif /* PPC405GP_REVB */
#if defined (PPC405GPR_REVB)
#   define PPC405GPR
#endif /* PPC405GPR_REVB */

/* Default boot line */

#define DEFAULT_BOOT_LINE \
    "emac(0,0)host:vxWorks h=10.1.1.2 e=10.1.1.10 u=target"

/*
 * Memory configuration.
 * SDRAM size is determined at boot time if LOCAL_MEM_AUTOSIZE is defined
 */

#define LOCAL_MEM_AUTOSIZE                      /* run-time memory sizing   */
#define LOCAL_MEM_SIZE        0x01000000        /* 16MB memory default      */
#define LOCAL_MEM_LOCAL_ADRS  0x00000000        /* fixed at zero            */
#define USER_RESERVED_MEM     0                 /* see sysMemTop()          */

/*
 * Define SDRAM_ECC_ENABLE to enable ECC if an ECC SDRAM DIMM is detected
 * during SDRAM auto-configuration.  If not defined, ECC will not be enabled
 * if an ECC SDRAM DIMM is detected, and the DIMM will be used as a normal DIMM.
 */

#undef  SDRAM_ECC_ENABLE

/*
 * The constants ROM_TEXT_ADRS and ROM_SIZE are defined both in config.h and
 * in Makefile.  Both definitions for these constants must be identical.
 * Also, the definition of ROM_WARM_ADRS must agree with code in romInit.s.
 *
 * For reasons having to do with bootapp Project builds, RAM_HIGH_ADRS
 * is no longer defined in config.h; however its former value is now used
 * -- near the end of config.h -- as the default setting of RAM_DST_ADRS.
 *
 * The boot ROM is selectable with switch 6 on U79 on the board.
 * The jumper selects either a 512KB flash (Am29F040) OR 512KB of 8 bit SRAM.
 * If switch 6 is in the ON position the flash is selected.  If switch 6 is
 * in the OFF position the SRAM is selected. In order to write to the SRAM
 * with a JTAG debugger, be sure to first set EBC0_B0CR[BU] to '11'b because
 * after reset Bank 0 of the External Bus Controller defaults to "read-only".
 *
 * Boot ROM address space.
 */

#define ROM_BASE_ADRS   0xfff80000               /* base address of ROM  */
#define ROM_TEXT_ADRS   (ROM_BASE_ADRS + 0x0100) /* with PC & SP         */
#define ROM_WARM_ADRS   (ROM_TEXT_ADRS + 0x0004) /* warm reboot entry    */
#define ROM_SIZE        0x0007f000               /* 512KB                */

/*
 * VxBus driver support. Note that the pool size may need to
 * be increased if additional interfaces are enabled.
 * also INCLUDE_VXBUS must always be defined for this BSP
 */

#define INCLUDE_VXBUS

#ifdef INCLUDE_VXBUS
#   define INCLUDE_HWMEM_ALLOC
#   ifdef _WRS_CONFIG_SMP
#       define HWMEM_POOL_SIZE 200000
#   else  /* _WRS_CONFIG_SMP */
#       define HWMEM_POOL_SIZE 50000
#   endif /* _WRS_CONFIG_SMP */
#   define INCLUDE_PLB_BUS
#   define INCLUDE_PARAM_SYS
#   define INCLUDE_VXB_CMDLINE
#   define INCLUDE_SIO_UTILS
#   define DRV_SIO_NS16550       /* serial driver */
#endif  /* INCLUDE_VXBUS */

/* Cache options */

#define INCLUDE_CACHE_SUPPORT

#define USER_D_CACHE_ENABLE
#define USER_I_CACHE_ENABLE
#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE    (CACHE_COPYBACK)


/*
 * 405 timers (PIT, FIT, WDT) can be driven from an external clock source,
 * or at the same frequency as the CPU.
 */

#define TIMER_CLOCK_EXTERNAL

/*
 * The clock used to drive the UARTs on the 405GP can be derived from
 * one of two different sources.  The 11.0592 MHz external oscillator
 * can be used, or the CPU clock can be used with an additional internal UART
 * divisor (see the CPC0_CR0 register).  If you do not want to use the external
 * UART clock oscillator, undef UART_CLOCK_EXTERNAL
 * NOTE: Revs B, C and D of the 405GP have an errata which prevents the
 * internal divisor to work at different rates. So if you want to use a
 * different console BAUD rate, it is recommended that you use the external
 * clock.
 */

#define  UART_CLOCK_EXTERNAL   /* define to use external 11.0592 MHz clock */

/* Optional timestamp support */

#define INCLUDE_AUX_CLK
#undef  INCLUDE_TIMESTAMP

/* Auxiliary Timer rates */

#ifdef TIMER_CLOCK_EXTERNAL
#   define AUX_CLK_RATE_MIN     (EXT_TIMER_CLK_FREQ / (1 << 21) )
#   define AUX_CLK_RATE_MAX     (EXT_TIMER_CLK_FREQ / (1 << 9)  )
#   define AUX_CLK_RATE_DEFAULT (EXT_TIMER_CLK_FREQ / (1 << 17) )
#endif /* TIMER_CLOCK_EXTERNAL */

/* Watchdog Timer rates */

#define WDT_RATE_MIN      1     /* minimum watchdog timer rate */
#define WDT_RATE_MAX      5000  /* maximum watchdog timer rate */

/* Real Time Clock configuration */

#define INCLUDE_RTC

/* Remove unused network drivers */

#undef  INCLUDE_EI
#undef  INCLUDE_EX
#undef  INCLUDE_ENP
#undef  INCLUDE_LN
#undef  INCLUDE_SM_NET
#undef  INCLUDE_SM_SEQ_ADDR


/* #define for software floating point support */

#undef INCLUDE_SW_FP


/* PCI configuration */

#define INCLUDE_PCI

#ifdef INCLUDE_PCI

#   define INCLUDE_PCI_AUTOCONF


/*   PCI bus memory map                                       controlled by
 *
 *   0x00000000 - 0x1FFFFFFF    Reserved for VGA                 (PMM2)
 *   0x20000000 - 0x3FFFFFFF    405GP slave window               (PTM1)
 *   0x40000000 - 0x5FFFFFFF    PCI Mem with prefetch            (PMM0)
 *   0x60000000 - 0x7FFFFFFF    PCI Mem w/o  prefetch            (PMM1)
 *   0x80000000 - 0xFFFFFFFF    unused                             -
 *
 */

/*
 * Define a 2GB slave window that allows PCI masters to access local memory
 * of the 405GP.  This window will be setup with the PTM1 registers in the
 * bridge.
 */

#   define PCI_SLV_MEM_LOCAL      LOCAL_MEM_LOCAL_ADRS
#   define PCI_SLV_MEM_BUS        LOCAL_MEM_LOCAL_ADRS
#   define PCI_SLV_MEM_SIZE       0x80000000           /* 2GB */

#   define PTM1_LOCAL_ADRS        PCI_SLV_MEM_LOCAL
#   define PTM1_SIZE_ATTRIB       PTM_SIZE_2GB | PTM_ENABLE

/*
 * Define a 32MB master window that allows the CPU to access PCI Memory
 * (with prefetch).  This window will be setup with the PMM0 registers in the
 * bridge.
 */

#   define PCI_MSTR_MEM_LOCAL     PCI_MEMORY_START
#   define PCI_MSTR_MEM_BUS       PCI_MEMORY_START
#   define PCI_MSTR_MEM_SIZE      0x02000000           /* 32MB */

#   define PMM0_LOCAL_ADRS        PCI_MSTR_MEM_LOCAL
#   define PMM0_PCI_LOW_ADRS      PCI_MSTR_MEM_BUS
#   define PMM0_PCI_HIGH_ADRS     0x00000000
#   define PMM0_PCI_MASK_ATTRIB   (PMM_MASK_32MB | PMM_PREFETCH | PMM_ENABLE)

/*
 * Define a 32KB master window that allows the CPU to access PCI Memory
 * (non-prefetch). This window will be setup with the PMM1 registers in
 * the bridge.
 */

#   define PCI_MSTR_MEMIO_LOCAL   (PCI_MEMORY_START + PCI_MSTR_MEM_SIZE)
#   define PCI_MSTR_MEMIO_BUS     (PCI_MSTR_MEM_BUS + PCI_MSTR_MEM_SIZE)
#   define PCI_MSTR_MEMIO_SIZE    0x02000000           /* 32MB */

#   define PMM1_LOCAL_ADRS        PCI_MSTR_MEMIO_LOCAL
#   define PMM1_PCI_LOW_ADRS      PCI_MSTR_MEMIO_BUS
#   define PMM1_PCI_HIGH_ADRS     0x00000000
#   define PMM1_PCI_MASK_ATTRIB   (PMM_MASK_32MB | PMM_ENABLE)

/* Master window that allows CPU to access PCI I/O space. */

#   define PCI_MSTR_IO_LOCAL      PLB_PCI_IO_REGION_1_START
#   define PCI_MSTR_IO_BUS        0x00000000
#   define PCI_MSTR_IO_SIZE       PLB_PCI_IO_REGION_1_SIZE

/*
 * The third PCI master memory window can be used to address VGA type devices
 * at PCI address 0x00000000.
 */

#   define PMM2_LOCAL_ADRS       (PCI_MSTR_MEMIO_LOCAL + PCI_MSTR_MEMIO_SIZE)
#   define PMM2_PCI_LOW_ADRS     0x00000000
#   define PMM2_PCI_HIGH_ADRS    0x00000000
#   define PMM2_PCI_MASK_ATTRIB  (PMM_MASK_32MB | PMM_ENABLE)

/* The second PCI slave window in the in the bridge are not being used. */

#   define PTM2_LOCAL_ADRS       PTM_UNUSED
#   define PTM2_SIZE_ATTRIB      PTM_UNUSED

/* PCI Latency Timer value */

#   define PCI_LAT_TIMER          0

#endif /* INCLUDE_PCI */

/*
 * If network support is included, either the native EMAC or the PCI Ethernet
 * should be chosen. Both are END style drivers.
 */

#define INCLUDE_END
#define INCLUDE_EMAC_VXB_END           /* 405GP native EMAC network support */
#define INCLUDE_VXB_IBM_MAL
#define INCLUDE_MII_BUS
#define INCLUDE_GENERICPHY
#undef  INCLUDE_LN97XEND              /* PCI network support               */

/* WDB communication is via an Enhanced Network Driver */

#ifdef  INCLUDE_END
#   undef  WDB_COMM_TYPE
#   define WDB_COMM_TYPE        WDB_COMM_END

#define MAX_MAC_ADRS	1	/* number of MAC addresses to save in EEPROM */
#define MAX_MAC_DEVS	1	/* one network device with MAC address - emac */
#define	MAC_ADRS_LEN	6

# if defined(BOOTAPP) || defined(INCLUDE_BOOT_APP)
#define ETHERNET_MAC_HANDLER    /* bootrom 'M' command sets MAC address */
#define INCLUDE_BOOT_ETH_MAC_HANDLER
# endif	/* BOOTAPP */
#endif  /* INCLUDE_END */

#define VEC_BASE    (char *)0

/*
 * MMU support.
 * By default only DMMU is enabled, and the IMMU is disabled for performance
 * reasons. This enable setting page level cacheability control for data
 * loads and stores, whereas the instruction cacheability is controlled by
 * ICCR. This reduces contention for the TLB entries by the IMMU and also
 * improves performance due to lesser TLB misses. If you want to enable
 * the IMMU, change the #undef below.
 */

/* MMU configuration */

#define INCLUDE_MMU_BASIC
#define USER_D_MMU_ENABLE
#undef  USER_I_MMU_ENABLE

#ifdef INCLUDE_MMU_BASIC

#   define INCLUDE_AIM_MMU_CONFIG

#   ifndef BOOTAPP
#       define INCLUDE_MMU_OPTIMIZE
#   endif

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

#   ifndef BOOTAPP
#       define INCLUDE_LOCK_TEXT_SECTION
#   endif

#   define INCLUDE_PAGE_SIZE_OPTIMIZATION

#endif /* INCLUDE_MMU_BASIC */

/* Include the other configuration values */

#include "walnut.h"            /* board header file */
#include "ppc405GP.h"          /* chip header file  */

#ifdef __cplusplus
    }
#endif

#endif  /* __INCconfigh */
#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

#ifndef RAM_DST_ADRS         /* default uncompress dest. */
# ifndef RAM_HIGH_ADRS
#  define RAM_DST_ADRS       0x00D00000        /* RAM address for ROM boot   */
# else
#  define RAM_DST_ADRS       RAM_HIGH_ADRS
# endif  /* RAM_HIGH_ADRS */
#endif  /* RAM_DST_ADRS */
