/* sysLib.c - amcc440ep PPC440EP system-dependent routines */

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

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

**************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
*****************************************************************************
   COPYRIGHT   Applied Micro Circuits Corporation (AMCC) 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF AMCC
   All rights reserved
   AMCC Proprietary

*****************************************************************************
\NOMANUAL
*/

/*
modification history
--------------------
01k,18sep08,pch  CQ134158: sysMemTop must not use PM_RESERVED_MEM unless
		 INCLUDE_EDR_PM
01j,17sep08,pch  CQ134488: add missing sysBus value
01i,04sep08,l_z  Fix compile warning
01h,02sep08,x_f  Replace sysDcrInLong with dcrInLong,
		 sysDcrOutLong with dcrOutLong
01g,21mar08,wap  Add VxBus support, switch to VxBus EMAC driver
01f,19sep07,kab  apigen cleanup
01e,08aug07,agf  remove duplicate variable definitions
01d,21mar07,rec  WIND00089964 warm start fails (revert to v01b)
01c,20nov06,rec  sysToMonitor disable cache before disabling MMU
01b,28may06,jmt  Modify sysModel to use SYS_MODEL define
01a,10mar06,mmi  written based on bamboo version 01h

*/

/*
DESCRIPTION
This library provides board-specific routines. The chip drivers included
are:

    uicIntr.c - on-chip Universal Interrupt Controller library
    ppc440Timer.c - on-chip watchdog, aux, and system clock drivers
    sysSerial.c - on-chip dual UART support
    sysBusPci.c - on-chip PCI host bus controller support
    sysNet.c - EMAC3 on-chip, FEI & LN97X PCI END network device drivers
    MalLib.c - Memory Access Layer for EMAC3 communications
    ibmEmacEnd.c - Media Independent Interface support for EMAC3
    iicLib.c - on-chip dual IIC bus driver

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <vxWorks.h>
#include <cacheLib.h>
#include <asm.h>
#include <vme.h>
#include <iv.h>
#include <esf.h>
#include <ioLib.h>
#include <sysLib.h>
#include <config.h>
#include <memLib.h>
#include <excLib.h>
#include <logLib.h>
#include <vxLib.h>
#include <string.h>
#include <intLib.h>
#include <taskLib.h>
#include <stdio.h>
#include <arch/ppc/dcr4xxALib.h>
#ifdef INCLUDE_MMU_BASIC
# include <private/vmLibP.h>
# include <arch/ppc/mmu440Lib.h>
#endif /* INCLUDE_MMU_BASIC */

#ifdef INCLUDE_VXBUS
#include <hwif/vxbus/vxBus.h>
IMPORT void    hardWareInterFaceInit();
#include "hwconf.c"
#include <hwif/vxbus/vxbPlbLib.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#endif /* INCLUDE_VXBUS */

/*
 * MMU library requires cache library support.
 */
#ifdef INCLUDE_MMU_BASIC
# ifndef INCLUDE_CACHE_SUPPORT
#  error "You need to include Cache support for MMU support."
# endif
#endif /* INCLUDE_MMU_BASIC */

/*
 * 440 cache library requires I and D cache on/off configurations to be
 * identical.  If one is enabled, the other has to be. If both are,
 * they must both be on or both be off.
 */
#ifdef INCLUDE_CACHE_SUPPORT

# if (defined(USER_I_CACHE_ENABLE) && !defined(USER_D_CACHE_ENABLE))
#  error "USER_I_CACHE_ENABLE requires USER_D_CACHE_ENABLE"
# elif (defined(USER_D_CACHE_ENABLE) && !defined(USER_I_CACHE_ENABLE))
#  error "USER_D_CACHE_ENABLE requires USER_I_CACHE_ENABLE"
# elif (defined(USER_I_CACHE_ENABLE) && defined(USER_D_CACHE_ENABLE))

#  if (USER_I_CACHE_MODE == CACHE_DISABLED && USER_D_CACHE_MODE != CACHE_DISABLED)
#   error "USER_I_CACHE_MODE disabled requires USER_D_CACHE_MODE disabled"
#  elif (USER_D_CACHE_MODE == CACHE_DISABLED && USER_I_CACHE_MODE != CACHE_DISABLED)
#   error "USER_D_CACHE_MODE disabled requires USER_I_CACHE_MODE disabled"
#  endif /* I & D cache on/off modes identical */

# endif /* I, D cache enablement identical */

#endif /* INCLUDE_CACHE_SUPPORT */

/* externals */

IMPORT STATUS  excIntConnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine);
IMPORT void    excIntHandle (void);
IMPORT void    sysUicIntHandler(void);
IMPORT void    sysUicCrtIntHandler(void);
IMPORT UINT32  vxCcr1Get (void);
IMPORT void    vxCcr1Set (UINT32);
IMPORT void mmu440TlbStaticInit
    (
    int          numDescs,	/* number of TLB Entry Descriptors */
    TLB_ENTRY_DESC * pTlbDesc,	/* pointer to array of TLB Entries */
    BOOL         cacheAllow	/* if TRUE, caching will be enabled */
    );

/* globals */

#ifdef INCLUDE_USER_LEDS
void ledBlink (UINT32 count);
#endif

/* We have a PCI bus, but primary is PLB (which has no assigned code). */

int   sysBus = BUS_TYPE_NONE;	/* system bus type */

int   sysCpu    = CPU;    /* system CPU type (PPC440EP) */
char *sysBootLine = BOOT_LINE_ADRS; /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS; /* catastrophic message area */
int   sysTimerClkFreq;      /* Timer clock frequency       */
SYS_INFO systemInfo;      /* PLL and system frequencies      */
int sysProcNum;     /* processor number of this CPU */


/*
 * The following variables specify the cache line size and the number of
 * cache lines for the 440EP. These are used by the architecture cache
 * libraries.
 */
const int  ppc440ICACHE_LINE_NUM = _ICACHE_LINE_NUM_440EP;
	      /* no. of lines in ICACHE */
const int  ppc440DCACHE_LINE_NUM = _DCACHE_LINE_NUM_440EP;
	      /* no. of lines in DCACHE */
const int  ppc440CACHE_ALIGN_SIZE = _CACHE_ALIGN_SIZE;
	      /* cache line size */

/* locals */


volatile AMCC440EP_BCSR_REG * pBcsrStruct = (AMCC440EP_BCSR_REG*)
					    CPLD_VIRT_START;

#ifdef INCLUDE_MMU_BASIC

/*
 * Identify whether the VM library can manage 36-bit PLB physical
 * addresses.
 */

/*
 * Macros to compose either 64- or 32-bit addresses and to properly
 * define the appropriate data type.
 */
#ifdef MMU_ASID_MAX		/* Base 6 MMU library in effect */

  /* 64-bit address macro for vxWorks 6.0 definition of sysPhysMemDesc[] */

# define ADRS_TYPE              VIRT_ADDR
# define PHYS_XBIT_ADDR(h, l)   (((PHYS_ADDR)(h) << 32) + (l))

#else
  /*
   * Disable the code below as amcc440ep does not officially support
   * pre-vxWorks 6.0 MMU Libraries. The code is left in for future reference
   */
#error "amcc440ep does not officially support pre-vxWorks 6.0 MMU libraries at this time"
#endif /* MMU_ASID_MAX */



/*
 * Decode USER_D_CACHE_MODE to determine main memory cache mode.
 * The macro MEM_STATE will be used to specify the cache state for
 * the sysPhysMemDesc table entry that describes the main memory
 * region.
 *
 * It is safe to assume USER_I_CACHE_ENABLE matches USER_D_CACHE_ENABLE;
 * see code above.
 */

#if (USER_D_CACHE_MODE == CACHE_DISABLED || !defined(USER_D_CACHE_ENABLE))
# define MEM_STATE  VM_STATE_CACHEABLE_NOT  /* default: no caching */
#elif (USER_D_CACHE_MODE == CACHE_COPYBACK)
# define MEM_STATE  VM_STATE_CACHEABLE
#elif (USER_D_CACHE_MODE == CACHE_WRITETHROUGH)
# define MEM_STATE  VM_STATE_CACHEABLE_WRITETHROUGH
#endif   /* USER_D_CACHE_MODE */

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE)
 * array used by the MMU to translate addresses with single page (4k)
 * granularity.
 *
 * This BSP supports two major versions of the VxWorks VM library:
 * Tornado 2.2.1 and Base 6.
 *
 * In Tornado 2.2.1 only MMU_BASIC is supported. The VxWorks vmBaseLib
 * support physical addresses less than or equal to 0x0.ffff.ffff,
 * because PHYS_ADDR is defined with 32-bit storage.  Therefore, the only
 * memory devices that the VM library can map are the Local DDR SDRAM,
 * onchip SRAM0 and I2O registers. MMU_BASIC supports a single VM
 * context.
 *
 * In Base 6 TAR, both MMU_BASIC and MMU_FULL is supported. The VxWorks
 * vmBaseLib and vmLib support the full 36-bit PLB address space because
 * PHYS_ADDR is defined with 64-bit storage. Therefore, the VM library
 * can map all devices.  MMU_FULL supports multiple VM contexts, which
 * are associated with RTPs to provide protected address spaces.
 *
 * PTEs are held in a 2-level page table. Each valid VM context has one
 * Level 1 page table which maps the entire 4GB virtual address space.
 * The Level 1 page table points to several Level 2 page tables which
 * each map portions of a contiguous 4MB address space.  Individual page
 * table entries map an address space of size VM_PAGE_SIZE (typically
 * 4KB).
 *
 * With MMU_FULL, level 2 page tables may be shared between contexts
 * where the mappings are identical -- typically the kernel's Level 2
 * table pointers are shared with the RTPs.
 *
 */

/*
 * Calculating size of page table required:
 * =======================================
 *
 * The size of each Level 1 table is 4K and the size of each Level 2
 * page table is 16K.
 *
 * For the following memory map we can calculate the page table size
 * required as follows:
 *
 *  Memory Area         Size      # of Level 2 pages
 *  ===========         ====      ==================
 *  1. Local Memory     128MB     32
 *  2. PCI Memory       64MB      16
 *  3. PCI IO Regn 1    64K       1
 *  4. PCI IO Regn 2    1MB       1
 *  5. PCI CFG          4K        1
 *  6. PCI IACK         4K        0 @
 *  7. PP Bridge        4K        1
 *  8. OPB3 IO Space    4K        0 @
 *  9  OPB4 IO Space    4K        1
 *  10.Flash            1MB       1
 *  @ - included in previous L2 page
 *
 *
 *  Total # of L2 pages =     57
 *  Total Memory Required for page table = 57 * 16 + 4 = 916 K. (Max when PCI and TFFS included)
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {
    {
    (ADRS_TYPE) LOCAL_MEM_LOCAL_ADRS,
    PHYS_XBIT_ADDR(0, LOCAL_MEM_LOCAL_ADRS),
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID             |
    VM_STATE_MASK_WRITABLE          |
    VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID                  |
    VM_STATE_WRITABLE               |
    VM_STATE_CACHEABLE
    },

#ifdef INCLUDE_PCI
  /* PCI Memory 1 (without prefetch) */
    {
    (ADRS_TYPE) PCIM0A_VIRT_START,
    PHYS_XBIT_ADDR(PCIM0A_PHY_START_HIGH, PCIM0A_PHY_START_LOW),
    PCI_MEMORY1_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },
  /* PCI Memory 2 (with prefetch) */
  {
    (ADRS_TYPE) PCIM0B_VIRT_START,
    PHYS_XBIT_ADDR(PCIM0B_PHY_START_HIGH, PCIM0B_PHY_START_LOW),
    PCI_MEMORY2_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT
    },

    /* PCI IO region 1 */
    {
    (ADRS_TYPE) PLB_PCI_IO_REGION_1_START_LOW,
    PHYS_XBIT_ADDR(PLB_PCI_IO_REGION_1_MAP_START_HIGH, PLB_PCI_IO_REGION_1_MAP_START_LOW),
    PLB_PCI_IO_REGION_1_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },

    /* PCI IO region 2 */
    {
    (ADRS_TYPE) PLB_PCI_IO_REGION_2_START_LOW,
    PHYS_XBIT_ADDR(PLB_PCI_IO_REGION_2_MAP_START_HIGH, PLB_PCI_IO_REGION_2_MAP_START_LOW),
    PLB_PCI_IO_REGION_2_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },

    /* PCI configuration address register */
    {
    (ADRS_TYPE) PCI_CFGADDR,
    PHYS_XBIT_ADDR(PCIC0_CFGADDR_PHY_HIGH, PCIC0_CFGADDR_PHY_LOW),
    PCI_CFG_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },

    /* PCI interrupt acknowledge/special cycle register */
    {
    (ADRS_TYPE) PCIC0_IASC0_VIRT_START,
    PHYS_XBIT_ADDR(PCIC0_IASC0_PHY_HIGH ,PCIC0_IASC0_PHY_LOW),
    PCI_INTERRUPT_ACK_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },

    /* PCI Bridge configuration register */
    {
    (ADRS_TYPE) PP_BRIDGE_CR,
    PHYS_XBIT_ADDR(PP_BRIDGE_CR_PHY_HIGH, PP_BRIDGE_CR_PHY_LOW),
    PP_BRIDGE_CR_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },
#endif  /* INCLUDE_PCI */

    /*
     *  OPB1 peripherals.
     *  (GPTs, SCP (SPI), GPIOs, IICs, UARTs, USB 1.1, OPB arb, EMACs)
     */
    {
    (ADRS_TYPE) OCP_VIRT_START,
    PHYS_XBIT_ADDR(OCP_PHY_START_HIGH, OCP_PHY_START_LOW),
    OCP_MAP_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },

    /*
     * OPB0 peripherals.
     */

    /* (USB 2.0) */
    {
    (ADRS_TYPE) OPBA1_VIRT_START,
    PHYS_XBIT_ADDR(OPB0_PHY_START_HIGH, OPB0_PHY_START_LOW),
    INTERN_OPB4_PERIPHERAL_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },

    /* CPLD registers */
    {
    (ADRS_TYPE) CPLD_VIRT_START,
    PHYS_XBIT_ADDR(CPLD_PHY_START_HIGH, CPLD_PHY_START_LOW),
    CPLD_SIZE_ALIGNED,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE     |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT      |
    VM_STATE_GUARDED
    },

    /* Boot Flash */
    {
    (ADRS_TYPE) ROM_BASE_ADRS,
    PHYS_XBIT_ADDR(0, ROM_BASE_ADRS),
    ROM_SIZE,
    VM_STATE_MASK_VALID         |
    VM_STATE_MASK_WRITABLE      |
    VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID              |
    VM_STATE_WRITABLE           |
    VM_STATE_CACHEABLE_NOT
    }

    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/*
 * Static TLB memory mappings
 *
 * The chip provides TLB entry registers which maintain a cache of
 * virtual to physical memory (PLB) address translations. The first few
 * of these entries are set up once at boot to provide a linear mapping
 * of large blocks that emulates a 'real' mode. A few more of the
 * entries are used in Tornado 2.2.1 to work around the limitation that
 * the VM library can only map 32 bit physical address spaces. These are
 * controlled with the sysStaticTlbDesc array, below.
 *
 * The remaining entries are dynamically used and reused in a round
 * robin fashion to implement the VM library's address map. These
 * are controlled by the sysPhysMemDesc array, above.
 *
 * The static TLB memory mappings contain caching attributes that track
 * the default cache type as defined by USER_D_CACHE_MODE.  The ideal
 * caching attribute for any given memory region should be set to no
 * more than the default cache mode (either CACHE_DISABLED,
 * CACHE_WRITETHROUGH, or CACHE_COPYBACK).  Even in cache mode COPYBACK,
 * it is necessary to support some memory regions as WRITETHROUGH and
 * some as cache DISABLED due to specific device or driver requirements.
 *
 * In addition, when cache is DISABLED, some normally WRITETHROUGH
 * regions are not just marked cache inhibited, but also as Guarded, to
 * remain consistent with the memory map provided by the Ebony OpenBIOS.
 *
 *           #   USER_D_CACHE_MODE (default cache mode)
 *  Static Memory Region #
 *  Ideal Caching Mode   #  DISABLED  WRITETHROUGH  COPYBACK
 *  =====================#==========================================
 *  _INHIBIT       #  -I-   -I-   I--
 *  _INHIBIT_AND_G       #  -IG   -IG   IG-
 *  _WRITETHROUGH      #  -I-   W--   W--
 *  _WRITETHROUGH_OR_G   #  -IG   W--   W--
 *  _COPYBACK      #  -I-     W--   ---
 */

#define _INHIBIT          _MMU_TLB_ATTR_I
#define _INHIBIT_AND_G        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
#if (USER_D_CACHE_MODE == CACHE_DISABLED || !defined(USER_D_CACHE_ENABLE))
# define _WRITETHROUGH    _MMU_TLB_ATTR_I
# define _WRITETHROUGH_OR_G _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
# define _COPYBACK    _MMU_TLB_ATTR_I
#elif (USER_D_CACHE_MODE == CACHE_WRITETHROUGH)
# define _WRITETHROUGH    _MMU_TLB_ATTR_W
# define _WRITETHROUGH_OR_G _MMU_TLB_ATTR_W
# define _COPYBACK    _MMU_TLB_ATTR_W
#elif (USER_D_CACHE_MODE == CACHE_COPYBACK)
# define _WRITETHROUGH    _MMU_TLB_ATTR_W
# define _WRITETHROUGH_OR_G _MMU_TLB_ATTR_W
# define _COPYBACK    0
#endif /* USER_D_CACHE_MODE */

/*
 * The TFFS driver requires that the Flash area be set as cache
 * inhibited, so fall back to inhibit instead of writethrough when TFFS
 * is installed.
 */

#ifdef INCLUDE_TFFS
# define _WRITETHROUGH_OR_TFFS  _INHIBIT
#else /* INCLUDE_TFFS */
# define _WRITETHROUGH_OR_TFFS  _WRITETHROUGH
#endif /* INCLUDE_TFFS */

/*
 * When the boot space is configured as writeable (example: when
 * jumpering it to SRAM for JTAG-based debugging), add the writeable
 * bit.
 */
#ifdef BOOT_SPACE_WRITEABLE
# define BOOT_SPACE_WRITE   _MMU_TLB_PERM_W
#else /* BOOT_SPACE_WRITEABLE */
# define BOOT_SPACE_WRITE   0
#endif /* BOOT_SPACE_WRITEABLE */


/*
 * Define static TLB mappings. This array is used by MMU library to
 * initialize static TLB entries for all memory when the mmu is emulating
 * real mode (MSR[IS,DS]=0) and in Tornado 2.2.1 to map physical address
 * space above 4GB when the mmu is 'enabled' (MSR[IS,DS] = 1).
 *
 * Code in exc440x5ALib.s expects TLB#0 to map the Machine Check handler.
 *
 * Note that this table should be closely coordinated with the boot
 * setup in romInit.s.
 */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
  /*
   * effAddr,  realAddrExt,  realAddr, ts | size
   * | ideal attributes | permissions
   */

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
   /*
    * TLB#0 = Machine check handler
    * TLB#1 = RAM 0 CACHED
    */

  /* TLB #0.  Machine Check handler (uncached) */
    {   VEC_BASE_ADRS + 0x4000,
	0x0,
	0x00004000,
	_MMU_TLB_TS_0   |
	_MMU_TLB_SZ_16K |
	_INHIBIT        |
	_MMU_TLB_PERM_X |
	_MMU_TLB_PERM_W
    },


  /*
   * TLB 1.  RAM 0 (cached).  This includes the Machine Check handler
   * when not using the INCLUDE_440X5_TLB_RECOVERY_MAX option.
   */
    {   VEC_BASE_ALT_ADRS,
	DDR_CA_PHY_START_HIGH,
	DDR_CA_PHY_START_LOW,
	_MMU_TLB_TS_0    |
	_MMU_TLB_SZ_256M |
	_COPYBACK        |
	_MMU_TLB_PERM_X  |
	_MMU_TLB_PERM_W
    },


#else  /* INCLUDE_440X5_TLB_RECOVERY_MAX */
   /*
    * TLB#0 = RAM 0
    * TLB#1 = RAM 0 UNCACHED
    */

  /*
   * TLB #0.  RAM 0 (cached).  This includes the Machine Check handler
   * when not using the INCLUDE_440X5_TLB_RECOVERY_MAX option.
   */
    {   VEC_BASE_ALT_ADRS,
	DDR_CA_PHY_START_HIGH,
	DDR_CA_PHY_START_LOW,
	_MMU_TLB_TS_0    |
	_MMU_TLB_SZ_256M |
	_COPYBACK        |
	_MMU_TLB_PERM_X  |
	_MMU_TLB_PERM_W
    },

  /* TLB #1.  RAM 0 (uncached) */
    {   DDR_UA_VIRT_START,
	DDR_UA_PHY_START_HIGH,
	DDR_UA_PHY_START_LOW,
	_MMU_TLB_TS_0    |
	_MMU_TLB_SZ_256M |
	_INHIBIT         |
	_MMU_TLB_PERM_W
    },
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */

  /* TLB #2.  Internal peripherals */
    {   OCP_VIRT_START,
	OCP_PHY_START_HIGH,
	OCP_PHY_START_LOW,
	_MMU_TLB_TS_0    |
	_MMU_TLB_SZ_16K  |
	_INHIBIT_AND_G   |
	_MMU_TLB_PERM_W
#if (SW_MMU_ENABLE == 1)
	| _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX
#endif
    },

  /* TLB #3.  Boot Space. Note autoCalcSpeed() expects TLB #3 */
    {   BS_VIRT_START,
	BS_PHY_START_HIGH,
	BS_PHY_START_LOW,
	_MMU_TLB_TS_0         |
	_MMU_TLB_SZ_256M      |
	_WRITETHROUGH_OR_TFFS |
	_MMU_TLB_PERM_X       |
	BOOT_SPACE_WRITE
    },

  /*
   * TLB #4.  External Peripherals part 1
   * (NVRAM CPLD)
   */
    {   EBC0_VIRT_START,
	EBC0_PHY_START_HIGH,
	EBC0_PHY_START_LOW,
	_MMU_TLB_TS_0    |
	_MMU_TLB_SZ_256M |
	_INHIBIT_AND_G   |
	_MMU_TLB_PERM_W
    },

  /* TLB #5.  PCI Memory1 */
    {   PCIM0A_VIRT_START,
	PCIM0A_PHY_START_HIGH,
	PCIM0A_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_256M    |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #6.  PCI Memory2 */
    {   PCIM0B_VIRT_START,
	PCIM0B_PHY_START_HIGH,
	PCIM0B_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_256M    |
	_INHIBIT            |
	_MMU_TLB_PERM_W
    },

  /* TLB #7.  PCI Memory3 */
    {   PCIM0C_VIRT_START,
	PCIM0C_PHY_START_HIGH,
	PCIM0C_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_256M    |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #8.  PCI Memory4 */
    {   PCIM0D_VIRT_START,
	PCIM0D_PHY_START_HIGH,
	PCIM0D_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_256M    |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #9.  OPB4 Peripherals */
    {   OPBA1_VIRT_START,
	OPBA1_PHY_START_HIGH,
	OPBA1_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_4K      |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #10.  PCI I/O */
    {   PCIIO0_VIRT_START,
	PCIIO0_PHY_START_HIGH,
	PCIIO0_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_64K     |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #11.  PCI EXTRA I/O */
    {   PCIIO1A_VIRT_START,
	PCIIO1A_PHY_START_HIGH,
	PCIIO1A_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_16M     |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #12.  PCI EXTRA I/O */
    {   PCIIO1B_VIRT_START,
	PCIIO1B_PHY_START_HIGH,
	PCIIO1B_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_16M     |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #13.  PCI EXTRA I/O */
    {   PCIIO1C_VIRT_START,
	PCIIO1C_PHY_START_HIGH,
	PCIIO1C_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_16M     |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #14.  PCI EXTRA I/O */
    {   PCIIO1D_VIRT_START,
	PCIIO1D_PHY_START_HIGH,
	PCIIO1D_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_16M     |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #15.  PCI Config Regs */
    {   PCIC0_VIRT_START,
	PCIC0_PHY_START_HIGH,
	PCIC0_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_4K      |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #16.  PCI Interrupt Ack/special cycle */
    {   PCIC0_IASC0_VIRT_START,
	PCIC0_IASC0_PHY_START_HIGH,
	PCIC0_IASC0_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_4K      |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    },

  /* TLB #17.  PCI Local Config Regs */
    {   PCIL0_VIRT_START,
	PCIL0_PHY_START_HIGH,
	PCIL0_PHY_START_LOW,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_4K      |
	_INHIBIT_AND_G      |
	_MMU_TLB_PERM_W
    }

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX

    ,

  /* TLB #18.  vectors other than Machine Check (cached, execute-only) */
    {   VEC_BASE_ADRS,
	0x0,
	0x00000000,
	_MMU_TLB_TS_0       |
	_MMU_TLB_SZ_16K     |
	_COPYBACK           |
	_MMU_TLB_PERM_X
    }

#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */

};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#endif /* INCLUDE_MMU_BASIC */

/*
 * Strapping structure for writing to CPU bootprom, see sysBootpromWrite
 */

typedef struct bootprom_info
{
  UINT32 sdr0_sdstp0;
  UINT32 sdr0_sdstp1;
  UINT32 sdr0_sdstp2;
  UINT32 sdr0_sdstp3;
} bootprom_info_t;


/* forward declarations */

STATUS  sysInfoGet(SYS_INFO * sysInfo, int verbose);
void  sysUsDelay(UINT32 us_delay);

UCHAR sysPciInByte(ULONG address);
void  sysPciOutByte(ULONG address, UCHAR data);
UINT16  sysPciInWord(ULONG address);
void  sysPciOutWord(ULONG address, UINT16 data);
UINT  sysPciInLong(ULONG address);
void  sysPciOutLong(ULONG address, UINT data);
#ifdef SDRAM_ECC_ENABLE
void  sysEccInit(void);
#endif /* SDRAM_ECC_ENABLE */

/* locals */

#ifdef DOC
#define INCLUDE_TIMESTAMP
#endif /* DOC */

/* BSP Drivers */
#include "nvRamToI2c.c"
#include <vme/nullVme.c>
#include "uicIntr.c"      /* UIC interrupt library */
#include "ppc440Timer.c"  /* ppc440Timer driver */
#include "sysCpcr.c"      /* Chip Control Register access functions */

#ifdef INCLUDE_SIO
#   include "sysSerial.c"
#endif /* INCLUDE_SIO */

#ifdef INCLUDE_PCI
#   include <pci/pciConfigLib.c>
#   include <pci/pciIntLib.c>
#if (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD))
#   include <pci/pciConfigShow.c>
#endif /* (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD)) */
#   ifdef INCLUDE_PCI_AUTOCONF
#     include <pci/pciAutoConfigLib.c>
#   endif /* INCLUDE_PCI_AUTOCONF */
#   include "sysBusPci.c"
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_NETWORK
#  include "sysNet.c"
#endif /* INCLUDE_NETWORK */

#include "iicLib.c"                 /* IIC functions                          */

/***************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string "IBM PowerPC 440EP Rev. x".
* Where x is either 1.0, n.n.
* The base model string is defined in config.h using the SYS_MODEL define.
*
* ERRNO
*/

char * sysModel (void)
    {

    if (vxPvrGet() == PVR_440EP_RA)
	return (SYS_MODEL " Rev. 1.0");

    if (vxPvrGet() == PVR_440EP_RB)
	return (SYS_MODEL " Rev. 2.0");

    return ("Unsupported processor");
    }


/***************************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number,
* for example, 1.1/0. BSP_REV is concatenated to BSP_VERSION and
* returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }


#ifdef INCLUDE_USER_LEDS
/*******************************************************************************
*
* ledBlink - Blinks user leds
*
* This routines blinks the two user leds on the board the number of times
* specified in it's argument. This is used for debugging purpose
*
* RETURNS: void
*
* \NOMANUAL
*/

void ledBlink
    (
    UINT32 count
    )
    {
    UINT32   i;

    /* flash the led  */

    for (i=count; i > 0; i--)
	{

	/* for count number of times alternate led to turn on */

	if (pBcsrStruct->usrConfig & BCSR2_LED_0)
	    {
	    pBcsrStruct->usrConfig |= BCSR2_LED_1;
	    pBcsrStruct->usrConfig &= ~BCSR2_LED_0;
	    }
	    else
	    {
	    pBcsrStruct->usrConfig |= BCSR2_LED_0;
	    pBcsrStruct->usrConfig &= ~BCSR2_LED_1;
	    }

	sysUsDelay (50000);
	}
    }
#endif	/* INCLUDE_USER_LEDS */


/***************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the AMCC 440EP amcc440ep
* board. This routine clears and turns off the external interrupts. It
* is called from usrInit() in usrConfig.c.
*
* NOTE: This routine should not be called directly by the user
* application.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysHwInit (void)
    {
#ifdef INCLUDE_PCI
    UINT32   sdr0Pci0;
#endif

    /* Fill in global system information structure */

    sysInfoGet(&systemInfo, FALSE);

    /* Set the 440 Interrupt Vector Prefix Register */

    vxIvprSet (AMCC440EP_IVPR_VAL);

    /* Support for MMU-less RTPs */
#if defined (INCLUDE_MMU_BASIC)
# if (SW_MMU_ENABLE == TRUE)
    mmu440TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], FALSE);
# endif /* SW_MMU_ENABLE is TRUE */
#endif /* INCLUDE_MMU_BASIC */

    /* Interrupt system initialization */

    sysUicIntrInit ();

    /*
     * Initialize the timer clock mode in the CCR1 register. Determine what
     * frequency the timer clock is operating at.
     */
#ifdef TIMER_CLOCK_EXTERNAL
    vxCcr1Set(vxCcr1Get() | _PPC_CCR1_TCS);
    sysTimerClkFreq = EXT_TIMER_CLK_FREQ;
#else  /* Timers run at the processor clock speed */
    vxCcr1Set(vxCcr1Get() & ~_PPC_CCR1_TCS);
    sysTimerClkFreq = systemInfo.freqProcessor;
#endif /* TIMER_CLOCK_EXTERNAL */

    /*
     * Connect the UIC interrupt handler to both regular and critical class
     * interrupts.
     */

    excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_INTR,
		   (VOIDFUNCPTR) &sysUicIntHandler);
    excIntCrtConnect ((VOIDFUNCPTR *) _EXC_OFF_CRTL,
		   (VOIDFUNCPTR) &sysUicCrtIntHandler);

#ifdef INCLUDE_EMAC_NETWORK
    ibmMalReset(MAL0_DCR_BASE);
#endif /* INCLUDE_EMAC_NETWORK */

#ifdef INCLUDE_PCI

    /* Read PCI configuration */
    sdr0Pci0 = sysSdrInLong(SDR0_PCI0);
    sdr0Pci0 |= SDR0_PCI0_PAE_ENABLE;  /* enable onchip arbiter */
    sysSdrOutLong(SDR0_PCI0,sdr0Pci0);

    /* Initialize the PCI configuration library, and the 440EP PCI bridge */

    pciIntLibInit();
    pciConfigLibInit(PCI_MECHANISM_0, (ULONG)ibmPciConfigRead,
		     (ULONG)ibmPciConfigWrite, (ULONG)ibmPciSpecialCycle);
    sysPciHostBridgeInit();

#ifdef INCLUDE_PCI_AUTOCONF
    /*
     * Perform auto-configuration of devices on the PCI bus
     */

    sysPciAutoConfig();
#endif /* INCLUDE_PCI_AUTOCONF */
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_NETWORK
    sysNetHwInit();   /* network interface */
#endif /* INCLUDE_NETWORK */

    /* Init IIC0 */

    iicInit (IIC0_BASE, &systemInfo);

#ifdef INCLUDE_SIO

    /* initialize the serial driver(s) */

    sysSerialHwInit ();
#endif /* INCLUDE_SIO */

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif
    }


/***************************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupts and configures any required
* features not configured by sysHwInit().
*
* RETURNS: N/A
*
* ERRNO
*/

void sysHwInit2 (void)
    {
    static BOOL configured = FALSE;

    if (!configured)
	{
#ifdef  _EXC_OFF_DECR
	/* connect system clock interrupt : excIntConnect uses malloc */

	excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_DECR,
		       (VOIDFUNCPTR) sysClkInt);
#endif  /* _EXC_OFF_DECR */

#ifdef  _EXC_OFF_FIT
	/* connect auxiliary clock interrupt */

	excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_FIT,
		       (VOIDFUNCPTR) sysAuxClkInt);

	/*
	 * Set default aux clock rate. This must be initialized at
	 * runtime, since the minimum rate varies based on system clock
	 * timer strapping.
	 */

	sysAuxClkRateSet(AUX_CLK_RATE_MIN);
#endif  /* _EXC_OFF_FIT */

#ifdef INCLUDE_NETWORK
	sysNetHwInit2 (); /* network interface */
#endif /* INCLUDE_NETWORK */

#ifdef INCLUDE_SIO
	sysSerialHwInit2(); /* connect serial interrupts */
#endif /* INCLUDE_SIO */

#ifdef INCLUDE_VXBUS
	vxbDevInit ();
	taskSpawn("tDevConn", 11, 0, 10000,
		  vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
#endif

	configured = TRUE;
	}
    }


/***************************************************************************
*
* sysMemSize - get the size of configured SDRAM
*
* Get the size of configured SDRAM
*
* RETURNS: The address of the top of physical memory.
*
* ERRNO
*
*/

UINT32 sysMemSize (void)
    {
    UINT32 memSize = 0;
    UINT32 sdramBankCfg[4];
    int    memBank;

    /* Read all four of the SDRAM controller bank control registers */

    dcrOutLong(SDRAM0_CFGADDR, SDRAM0_B0CR);
    sdramBankCfg[0] = dcrInLong(SDRAM0_CFGDATA);

    dcrOutLong(SDRAM0_CFGADDR, SDRAM0_B1CR);
    sdramBankCfg[1] = dcrInLong(SDRAM0_CFGDATA);

    dcrOutLong(SDRAM0_CFGADDR, SDRAM0_B2CR);
    sdramBankCfg[2] = dcrInLong(SDRAM0_CFGDATA);

    dcrOutLong(SDRAM0_CFGADDR, SDRAM0_B3CR);
    sdramBankCfg[3] = dcrInLong(SDRAM0_CFGDATA);

    /*
     * Check each of the SDRAM banks in the controller. If a bank is
     * enabled get the size it is configured to and add to the total.
     */

    for (memBank = 0; memBank < 4; memBank++)
	{
	if (sdramBankCfg[memBank] & SDRAM_BXCR_SDBE)
	    {
	    switch(sdramBankCfg[memBank] & SDRAM_BXCR_SDSZ_MASK)
		{
		case SDRAM_BXCR_SDSZ_8:
		  memSize = memSize + (8 * 1024 * 1024);         /* 8MB */
		  break;
		case SDRAM_BXCR_SDSZ_16:
		  memSize = memSize + (16 * 1024 * 1024);        /* 16MB */
		  break;
		case SDRAM_BXCR_SDSZ_32:
		  memSize = memSize + (32 * 1024 * 1024);        /* 32MB */
		  break;
		 case SDRAM_BXCR_SDSZ_64:
		  memSize = memSize + (64 * 1024 * 1024);        /* 64MB */
		  break;
		case SDRAM_BXCR_SDSZ_128:
		  memSize = memSize + (128 * 1024 * 1024);       /* 128MB */
		  break;
		case SDRAM_BXCR_SDSZ_256:
		  memSize = memSize + (256 * 1024 * 1024);       /* 256MB */
		  break;
		case SDRAM_BXCR_SDSZ_512:
		  memSize = memSize + (512 * 1024 * 1024);       /* 512MB */
		  break;
		}
	    }
	}

    return memSize;
    }

/***************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Normally, the user specifies the amount of physical memory with the
* macro LOCAL_MEM_SIZE in config.h.  BSPs that support run-time
* memory sizing do so only if the macro LOCAL_MEM_AUTOSIZE is defined.
* If not defined, then LOCAL_MEM_SIZE is assumed to be, and must be, the
* true size of physical memory.
*
* NOTE: Do not adjust LOCAL_MEM_SIZE to reserve memory for application
* use.  See sysMemTop() for more information on reserving memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*
* ERRNO
*/

char * sysPhysMemTop (void)
    {
    static char * physTop = NULL;

    if (physTop == NULL)
	{
#ifdef LOCAL_MEM_AUTOSIZE
	/*
	 * LIMIT physTop to 1GB memory. This can be increased if the static
	 * TLB memory maps established in resetEntry (romInit.s) and
	 * sysStaticTlbDesc[] are altered.  The SDRAM controller hardware
	 * has a maximum limit of 2GB.
	 */
	physTop = (char *)min(sysMemSize(), MAPPED_SDRAM_SIZE);

# ifdef INCLUDE_MMU_BASIC
	/*
	 * based on the size of the memory we calculated, update the
	 * sysPhysMemDesc structure, so that we map the right size of RAM.
	 * We assume that the first element in the sysPhysMemDesc structure
	 * is referring to RAM.
	 */
	sysPhysMemDesc[0].len = (UINT32)physTop - LOCAL_MEM_LOCAL_ADRS;
# endif /* INCLUDE_MMU_BASIC */

#else /*LOCAL_MEM_AUTOSIZE*/
	/* Don't do autosizing, size is given */

	physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
#endif /*LOCAL_MEM_AUTOSIZE*/

	}

    return physTop;
    }


/***************************************************************************
*
* sysMemTop - get the address of the top of VxWorks memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by VxWorks.
*
* The user can reserve memory space by defining the macro
* USER_RESERVED_MEM in config.h. This routine returns the address of the
* reserved memory area. The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*
* ERRNO
*/

char * sysMemTop (void)
    {
    static char * memTop = NULL;

    if (memTop == NULL)
	{
	memTop = sysPhysMemTop ()
#ifdef	INCLUDE_EDR_PM
	- PM_RESERVED_MEM
#endif	/* INCLUDE_EDR_PM */
	- USER_RESERVED_MEM;
	}

    return memTop;
    }


/***************************************************************************
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor. Normally, it is
* called only by reboot()--which services ^X--and by bus errors at
* interrupt level. However, in special circumstances, the user may wish
* to introduce a <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*
* ERRNO
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_WARM_ADRS); /* Warm reboot */

    (void)intLock();        /* lock interrupts */

#ifdef INCLUDE_MMU_BASIC
    VM_ENABLE (FALSE);				/* disable MMU */
#endif /* INCLUDE_MMU_BASIC */

    cacheDisable(_DATA_CACHE);
    cacheDisable(_INSTRUCTION_CACHE);

    /* clear pending interrupts */
    dcrOutLong(UIC0_SR, 0xffffffff);
    dcrOutLong(UIC1_SR, 0xffffffff);

    (*pRom) (startType);  /* jump to bootrom */

    return (OK);    /* in case we ever continue from ROM monitor */
    }


/***************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board, always 0 (zero).
*
* SEE ALSO: sysProcNumSet()
*
* ERRNO
*/

int sysProcNumGet (void)
    {
    return (0);
    }


/***************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board. Processor
* numbers should be unique on a single backplane.
*
* NOTE: This routine has no effect, since there is no VMEbus.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet(),
*
* ERRNO
*/

void sysProcNumSet
    (
    int procNum /* processor number (ignored in amcc440ep BSP) */
    )
    {
    sysProcNum = sysProcNumGet();
    }


/***************************************************************************
*
* sysIntLockLevelSet - set the current interrupt lock-out level
*
* This routine sets the current interrupt lock-out level.
*
* NOTE: This routine has no effect, since the facility is currently not
* implemented.
*
* RETURNS: ERROR, always.
*
* ERRNO
*/

int sysIntLockLevelSet
    (
    int newLvl    /* new interrupt level */
    )
    {
    return (ERROR);
    }


/***************************************************************************
*
* sysInByte - read one byte from an I/O address
*
* This routine reads a single byte from a specified I/O address.
*
* RETURNS: The byte from the specified address.
*
* ERRNO
*/

UCHAR sysInByte
    (
    ULONG address   /* I/O address to read the byte from */
    )
    {
    UCHAR data = (*(UCHAR *)address);

    EIEIO_SYNC;

    return data;
    }


/***************************************************************************
*
* sysOutByte - write one byte to an I/O address
*
* This routine writes a single byte to a specified I/O address.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysOutByte
    (
    ULONG address,  /* I/O address to write the byte to */
    UCHAR data    /* byte to write */
    )
    {
    *(UCHAR *)address = data;

    EIEIO_SYNC;
    }


/*************************************************************************
*
* sysUsDelay - delay for at least x microseconds.
*
* Use the timebase register, running at sysTimerClkFreq, to delay at least
* the requested number of microseconds.
*
* RETURNS: N/A.
*
* ERRNO
*
* \NOMANUAL
*
* \INTERNAL
*
* Do not call before sysHwInit determines sysTimerClkFreq.
*/

void sysUsDelay
    (
    UINT32 delay
    )
    {
    UINT32  hi;       /* current timebase upper 32 bits */
    UINT32  lo;       /* current timebase lower 32 bits */
    UINT32  ticks;    /* number of ticks needed for delay */
    FAST UINT32 target_hi;      /* target timebase upper 32 bits */
    FAST UINT32 target_lo;      /* target timebase lower 32 bits */

    /* get start time */
    vxTimeBaseGet(&hi, &lo);

    /* Calculate number of ticks for desired elapsed time. */
    ticks = delay * (ROUND_UP(sysTimerClkFreq, 1000000) / 1000000);

    /* set target hi and lo, and account for rollover */
    target_lo = lo + ticks;     /* may cause rollover */
    target_hi = hi + (target_lo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
	{
	vxTimeBaseGet(&hi, &lo);
	}
    while (hi < target_hi || (hi == target_hi && lo < target_lo));

    return;
    }


/***************************************************************************
*
* checkRate - validate clock rate within range
*
* This routine verifies the given clock is within the desired ranges
* and emits an error if not. It is a helper for sysInfoGet.
*
* RETURNS: OK or ERROR.
*
* ERRNO
*
* \NOMANUAL
*/

LOCAL STATUS checkRate
    (
    char *  rateName,   /* description of rate */
    UINT32  rateActual, /* actual rate value */
    UINT32  rateMin,    /* minimum acceptable rate */
    UINT32  rateMax,    /* maximum acceptable rate */
    UINT32  cpr0Pllc,   /* CPR0_PLLC register contents */
    UINT32  cpr0Plld,   /* CPR0_PLLD register contents */
    UINT32  cpr0Primad, /* CPR0_PRIMAD register contents */
    UINT32  cpr0Primbd, /* CPR0_PRIMBD register contents */

    UINT32  verbose   /* if nonzero, produce output */
    )
    {
    if (rateActual < rateMin || rateActual > rateMax)
	{
	if (verbose)
	    {
	    printf("Invalid %s frequency calculated :  %d MHz \a\n",
		   rateName, rateActual);
	    printf("It must be between %d-%d MHz \a\n", rateMin, rateMax);
	    printf("CPR0_PLLC reg     :  %8.8x\a\n", cpr0Pllc);
	    printf("CPR0_PLLD reg     :  %8.8x\a\n", cpr0Plld);
	    printf("CPR0_PRIMAD reg   :  %8.8x\a\n", cpr0Primad);
	    printf("CPR0_PRIMBD reg   :  %8.8x\a\n", cpr0Primbd);
	    }
	return ERROR;
	}
    return OK;
    }


/***************************************************************************
*
* sysInfoGet - Determines system information (clock frequencies, etc.)
*
* This routine returns information about the current operating
* environment of the amcc440ep (Yosemite) board based on the board's
* switch settings. Optionally, it prints the information out.
*
* RETURNS: OK or ERROR.
*
* ERRNO
*/

STATUS sysInfoGet
    (
    SYS_INFO  * sysInfo,
    int     verbose
    )
    {
    UINT32   cpr0Pllc;
    UINT32   cpr0Plld;
    UINT32   cpr0Primad;
    UINT32   cpr0Primbd;
    UINT32   cpr0Opbd;
    UINT32   cpr0Perd;
    UINT32   cpr0Mald;
    UINT32   cpr0Spcid;
    UINT32   sdr0Pci0;            /* SDR0_PCI0 register value                */
    UINT32   sdr0Pstrp0;          /* SDR0_PSTRP0 register value */
    UINT32   M;                   /* ratio of VCO clock rate to SYSCLK rate  */
    UINT32   memSize;
    char *   bootStrap = "Default strap setting 0 ";
    char *   pllFbk = "System PLL Bypassed";
    char *   arbInternal = "Disabled";
    char *   arbExternal = "Disabled";
    char *   PCIClockMode = "Asynchronous";
    char *   eepromUsr = "not protected";
    char *   eepromBoot = "not protected";
    char *   flashBootSector = "not protected";
    char *   usb20Cable = "Not present";
    char *   usb11Cable = "Not present";
    char *   usb11Speed = "Full Speed";
    char *   eccState = "Unknown state";

    bzero((char *)sysInfo, sizeof(SYS_INFO));

    /*
     * Read Chip PLL, clocking and PCI config related regs
     */

    /* PLL Configuration */
    cpr0Pllc = sysCprInLong(CPR0_PLLC);

    /* PLL Dividers */
    cpr0Plld = sysCprInLong(CPR0_PLLD);

    /* CPU Clock Divider */
    cpr0Primad = sysCprInLong(CPR0_PRIMAD);

    /* PLB clock Divider */
    cpr0Primbd = sysCprInLong(CPR0_PRIMBD);

    /* OPB Clock Divider */
    cpr0Opbd = sysCprInLong(CPR0_OPBD);

    /* EBC Clock Divider */
    cpr0Perd = sysCprInLong(CPR0_PERD);

    /* MAL Clock Divider */
    cpr0Mald = sysCprInLong(CPR0_MALD);

    /* PCI Clock Divider */
    cpr0Spcid = sysCprInLong(CPR0_SPCID);

    /* PCI configuration */
    sdr0Pci0 = sysSdrInLong(SDR0_PCI0);

    /* Strapping configuration */
    sdr0Pstrp0 = sysSdrInLong(SDR0_PSTRP0);

    if (verbose)
	printf("\n");

    /* The system clock is supplied from a Cypress CY2071AF programmable clock
     * generator. The input to the CY2071AF device is 22.1184 MHZ oscillator.
     * The CY2071AF device on board provides the system clock input (SysClk) to
     * the processor at 66 MHZ. It also provides a 11.0592 MHz clock input for
     * the UART baud rate generator.
     */

    sysInfo->freqSys = BSP_SYSTEM_CLK;

    /*
     * Determine FBDV,FWDVA,FWDVB,OPDV,EPDV,PLBDV,CPUDV,EFBDV,FBDV system PLL
     * BYPASS mode
     */

    sysInfo->pllFbkDiv    = CPR0_PLLD_FBDV_DECODE(cpr0Plld);

    /* 9,11,13,15 are Reserved values ! */

    sysInfo->pllFwdDivA   = CPR0_PLLD_FWDVA_DECODE(cpr0Plld);
    sysInfo->pllFwdDivB   = CPR0_PLLD_FWDVB_DECODE(cpr0Plld);
    sysInfo->pllOpbDiv    = CPR0_OPBD_OPBDV0_DECODE(cpr0Opbd);
    sysInfo->pllExtBusDiv = CPR0_PERD_PERDV0_DECODE(cpr0Perd);
    sysInfo->pllPlbDiv    = CPR0_PRIMBD_PRBDV0_DECODE(cpr0Primbd);
    sysInfo->pllCpuDiv    = CPR0_PRIMAD_PRADV0_DECODE(cpr0Primad);
    sysInfo->pllEfbDiv    = CPR0_PLLD_LFBDV_DECODE(cpr0Plld);
    sysInfo->pllPciDiv    = CPR0_SPCID_SPCIDV0_DECODE(cpr0Spcid);
    sysInfo->pllMalDiv    = CPR0_MALD_MALDV0_DECODE(cpr0Mald);

    /*
     * Check pllFwdDiv to see if running in bypass mode where the CPU speed
     * is equal to the 440EP SYS_CLK_FREQ. If not in bypass mode, check VCO
     * to make sure it is within the proper range.
     *    spec:    VCO = SYS_CLOCK x FBKDIV x PLBDIV x FWDDIV
     * Note freqVCO is calculated in Mhz to avoid errors introduced by rounding.
     */

    if ((cpr0Pllc & CPR0_PLLC_ENG_MASK) == CPR0_PLLC_ENG_DISABLE)
	{
	sysInfo->freqProcessor = sysInfo->freqSys /  sysInfo->pllCpuDiv;
	sysInfo->freqPLB = sysInfo->freqSys / sysInfo->pllPlbDiv;
	}
	else
	{
	if ((cpr0Pllc & CPR0_PLLC_SEL_MASK) == CPR0_PLLC_SEL_CPU)
	    {
	    /* Use CPU clock for PLL feedback */
	    M = sysInfo->pllFwdDivA * sysInfo->pllFbkDiv * sysInfo->pllCpuDiv;
	    pllFbk = "CPU clock";
	    }
	else if ((cpr0Pllc & CPR0_PLLC_SEL_MASK) == CPR0_PLLC_SEL_EBC)
	    {
	    /* Use EXT bus clock for PLL feedback */
	    M = sysInfo->pllFwdDivB * sysInfo->pllFbkDiv
		 * sysInfo->pllExtBusDiv * sysInfo->pllPlbDiv;
	    pllFbk = "EBC clock";
	    }
	else
	    {

	    /* PLL output (A or B) feedback */

	    if ((cpr0Pllc & CPR0_PLLC_SRC_MASK) == CPR0_PLLC_SRC_PLLOUTA)
		{
	/* PLL output A is the feedback */
	M = sysInfo->pllFwdDivA * sysInfo->pllFbkDiv * sysInfo->pllEfbDiv;
	pllFbk = "PLL output A clock";
		}
	    else
		{
	/* PLL output B is the feedback */
	M = sysInfo->pllFwdDivB * sysInfo->pllFbkDiv * sysInfo->pllEfbDiv;
	pllFbk = "PLL output B clock";
		}
	    }

	sysInfo->freqVCOMhz = (BSP_SYSTEM_CLK/1000000 * M) + (M>>1);

	if (checkRate("VCO", sysInfo->freqVCOMhz, VCO_MHZ_MIN, VCO_MHZ_MAX,
		      cpr0Pllc,cpr0Plld,cpr0Primad,cpr0Primbd, verbose) != OK)
	    return(ERROR);

	sysInfo->freqProcessor = sysInfo->freqVCOMhz
				 / (sysInfo->pllFwdDivA * sysInfo->pllCpuDiv);
	sysInfo->freqProcessor *= 1000000;
	sysInfo->freqPLB = sysInfo->freqVCOMhz
			   / (sysInfo->pllFwdDivB * sysInfo->pllPlbDiv);
	sysInfo->freqPLB *= 1000000;
	}

    if (checkRate("CPU", sysInfo->freqProcessor, CPU_HZ_MIN, CPU_HZ_MAX,
		  cpr0Pllc,cpr0Plld,cpr0Primad,cpr0Primbd, verbose) != OK)
	return(ERROR);

    if (checkRate("PLB", sysInfo->freqPLB, PLB_HZ_MIN, PLB_HZ_MAX,
		  cpr0Pllc,cpr0Plld,cpr0Primad,cpr0Primbd, verbose) != OK)
	return(ERROR);

    /* determine MAL clock */

    sysInfo->freqMAL = sysInfo->freqPLB / sysInfo->pllMalDiv;

    /*
     * Determine if one of the two AMCC 440ep amcc440ep board serial EEPROMs
     * was used to supply boot strap information to the 440EP.  If an EEPROM
     * wasn't used then the default bootstrap parameters were used to boot.
     */

    switch (sdr0Pstrp0 & SDR0_PSTRP0_PSTP_MASK)
    {
      case SDR0_PSTRP0_PSTP_DSS0: bootStrap = "Default strap setting 0"; break;
      case SDR0_PSTRP0_PSTP_DSS1: bootStrap = "Default strap setting 1"; break;
      case SDR0_PSTRP0_PSTP_DSS2: bootStrap = "Default strap setting 2";  break;
      case SDR0_PSTRP0_PSTP_DSS3: bootStrap = "Default strap setting 3";  break;
      case SDR0_PSTRP0_PSTP_SPROM1: bootStrap = "EEPROM 1 Enabled";  break;
      case SDR0_PSTRP0_PSTP_SPROM0: bootStrap = "EEPROM 0 Enabled";  break;
      default: bootStrap = "Unsupported setting";  break;
    }

    /*
     * Determine if the internal and external PCI arbiters are enabled.
     * It is an error if both arbiters are enabled.
     */

    if ((sdr0Pci0 & SDR0_PCI0_PAE_MASK)==SDR0_PCI0_PAE_ENABLE)
	arbInternal = "Enabled";
    else
	return ERROR;

    /*
     * Determine the PCI clock mode : Asynchronous or Synchronous
     */

    if ((sdr0Pci0 & SDR0_PCI0_PAME_MASK)==SDR0_PCI0_PAME_ENABLE)
	PCIClockMode = "Asynchronous";
    else
	PCIClockMode = "Synchronous";

    /* Determine external PCI clock */

    if (pBcsrStruct->pciSts & BCSR5_PCI_M66EN )
	sysInfo->freqPCIExt =  6666666;
    else
	sysInfo->freqPCIExt =  33333333;

    /* Determine user EEPROM status */

    if (pBcsrStruct->sttmCtrl &  BCSR7_EEPROM_USR_PRT)
	eepromUsr = "  protected ";

    /* Determine boot EEPROM status */

    if (pBcsrStruct->sttmCtrl &  BCSR7_EEPROM_BOOT_PRT)
	eepromBoot = " protected ";

    /* Determine flash status */

    if (pBcsrStruct->sttmCtrl &  BCSR7_FLASH_PRT)
	flashBootSector = " protected ";

    /* Determine USB devices status */

    if (pBcsrStruct->usbCtrl &  BCSR9_USB20_VCC)
	usb20Cable = " present ";

    if (pBcsrStruct->usbCtrl &  BCSR9_USB11_LS)
	usb11Cable = " Low Speed ";

    if (pBcsrStruct->usbCtrl &  BCSR9_USB11_VCC)
	usb11Cable = " present ";

    /* Determine external PCI clock */

    sysInfo->freqPCIInt =  sysInfo->freqPLB / sysInfo->pllPciDiv;

    /*
     * see how much RAM is installed, and whether ECC is enabled
     */

    dcrOutLong(SDRAM0_CFGADDR, SDRAM0_CFG0);
    switch (dcrInLong(SDRAM0_CFGDATA) & SDRAM_CFG0_MCHK_MASK)
	{
	case SDRAM_CFG0_MCHK_NON:
	    eccState = "None";
	    break;
	case SDRAM_CFG0_MCHK_CHK:
	    eccState = "Enabled";
	    break;
	case SDRAM_CFG0_MCHK_GEN:
	    eccState = "Parity generation only";
	    break;
	}

    memSize = (UINT32)(sysPhysMemTop ()) / (1024 * 1024);

    /*
     * Output info.
     */

    if (verbose)
    {
	printf(" %s   PVR = 0x%x\n", sysModel(),vxPvrGet());
	printf(" sysClk frequency  = %d MHz\n", sysInfo->freqSys/1000000);
	printf(" VCO frequency     = %d MHz\n", sysInfo->freqVCOMhz);
	printf(" CPU frequency     = %d MHz\n", sysInfo->freqProcessor/1000000);
	printf(" PLB frequency     = %d MHz\n", sysInfo->freqPLB / 1000000);
	printf(" OPB frequency     = %d MHz\n", sysInfo->freqPLB /
		(sysInfo->pllOpbDiv * 1000000));
	printf(" Ext Bus frequency = %d MHz\n", sysInfo->freqPLB /
		(sysInfo->pllExtBusDiv * 1000000));
	printf(" MAL frequency     = %d MHz\n", sysInfo->freqMAL / 1000000);
	printf(" Int PCI Clock frequency = %d MHz\n",
		sysInfo->freqPCIInt / 1000000);
	printf(" Ext PCI Clock frequency = %d MHz\n",
		sysInfo->freqPCIExt / 1000000);
	printf("\n");
	printf(" PCI clock mode = %s\n", PCIClockMode);
	printf(" Internal PCI Arbiter: %s\n", arbInternal);
	printf(" External PCI Arbiter: %s", arbExternal);
	printf("\n");
	printf(" System PLL feedback:  %s\n", pllFbk);
	printf(" bootstrap Info :  %s\n", bootStrap);
	printf(" DDR SDRAM : %d MBytes ECC : %s\n", memSize,eccState);
	printf ("Boot EEPROM is %s\n", eepromBoot);
	printf ("User EEPROM is %s\n", eepromUsr);
	printf ("Flash Boot sector is %s\n", flashBootSector);
	printf ("USB 2.0 Cable is %s\n", usb20Cable);
	printf ("USB 1.1 Cable is %s set at %s\n", usb11Cable, usb11Speed);
	printf ("Board Revision:  %d \n", pBcsrStruct->boardRevId);
	printf ("CPLD Code version:  %d \n", pBcsrStruct->cpldCodeRev);
	printf("\n");
     }

    return(OK);
    }

#ifdef INCLUDE_EXC_SHOW
# include "440epBusErrOriginShow.c"
#endif /* INCLUDE_EXC_SHOW */

#ifdef INCLUDE_SHOW_ROUTINES
/***************************************************************************
*
* sysInfoShow - Shows system information (clock frequencies, etc.)
*
* This routine displays information about the current operating
* environment of the amcc440ep board.
*
* RETURNS: OK or ERROR.
*
* ERRNO
*/

STATUS sysInfoShow (void)
    {
    SYS_INFO  sysInfo;

    return(sysInfoGet(&sysInfo, 1));
    }
#endif /* INCLUDE_SHOW_ROUTINES */
