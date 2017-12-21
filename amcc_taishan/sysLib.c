/* sysLib.c - amcc_taishan system-dependent routines */

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
 * Copyright (c) 2007, 2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01e,26mar12,c_t  disable TAH by default. (WIND00272618)
01h,15jan10,liu  define INCLUDE_EMAC_VXB_END to avoid build error. (WIND00190329)
01g,16dec08,b_m  add vxbus timer & uic support.
01f,15oct08,x_z  fix missing "sysBus" api error for BSPVTS(WIND00135289)
01e,04sep08,l_z  Fix compile warning
01d,02sep08,x_f  Remove sysDcr.h include, replace sysDcrInLong with dcrInLong, 
                 sysDcrOutLong with dcrOutLong
01c,20jue08,x_s  updated for new VxBus EMAC driver.
01b,28nov07,x_s  updated for supporting vxBus driver:SIO, EMAC, IbmMal & PCIX. 
01a,17apr07,x_s  initial creation, based on Mainstone
*/

/*
DESCRIPTION
This library provides board-specific routines. The chip drivers included
are:

    sysBusPci.c - on-chip PCI-X host bus controller support
    sysScsi.c - SCSI support
    sysNet.c - EMAC4 on-chip, FEI & LN97X PCI END network device drivers
    phyLib.c - Media Independent Interface support for EMAC4
    l2Lib.c - on-chip Level 2 writethru cache support
    iicLib.c - on-chip dual IIC bus driver

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>

#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>

#include <cacheLib.h>
#include <asm.h>
#include <vme.h>
#include <iv.h>
#include <esf.h>
#include <ioLib.h>
#include "sysLib.h"
#include "config.h"
#include <memLib.h>
#include <excLib.h>
#include <logLib.h>
#include <vxLib.h>
#include <string.h>
#include <intLib.h>
#include <taskLib.h>
#include <stdio.h>
#include <arch/ppc/dcr4xxALib.h>
#if ( defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL) )
# include <private/vmLibP.h>
# include <arch/ppc/mmu440Lib.h>
#endif /* INCLUDE_MMU_BASIC || INCLUDE_MMU_FULL */
#include <time.h>


/*
 * MMU library requires cache library support.
 */
#if ( defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL) )
# ifndef INCLUDE_CACHE_SUPPORT
#  error "You need to include Cache support for MMU support."
# endif
#endif /* INCLUDE_MMU_BASIC || INCLUDE_MMU_FULL */

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
IMPORT UINT32  vxCcr1Get (void);
IMPORT void    vxCcr1Set (UINT32);
IMPORT UINT32  mulDiv64(UINT32, UINT32, UINT32);
IMPORT UINT32  autoCalcSpeed(void);
IMPORT STATUS  ibmPciConfigRead(int bus, int device, int function,
                int offset, int width, void *pResult);
IMPORT STATUS  ibmPciConfigWrite(int bus, int device, int function,
                 int offset, int width, UINT data);
IMPORT STATUS  ibmPciSpecialCycle(int bus, UINT32 data);
IMPORT STATUS  sysPciHostBridgeInit(void);
IMPORT void mmu440TlbStaticInit
    (
    int          numDescs,	/* number of TLB Entry Descriptors */
    TLB_ENTRY_DESC * pTlbDesc,	/* pointer to array of TLB Entries */
    BOOL         cacheAllow	/* if TRUE, caching will be enabled */
    );

#ifdef  INCLUDE_VXBUS
IMPORT void    hardWareInterFaceInit();
IMPORT device_method_t * pSysPlbMethods;

/*
 * Methods which we want to add to the PLB device.
 */

#endif

/* globals */

int   sysBus      = BUS;            /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;            /* system CPU type (PPC440) */
char *sysBootLine = BOOT_LINE_ADRS; /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;   /* catastrophic message area */
int   sysTimerClkFreq;              /* Timer clock frequency         */
SYS_INFO systemInfo;                /* PLL and system frequencies        */
int sysProcNum;                     /* processor number of this CPU */

/*
 * The following variables specify the cache line size and the number of
 * cache lines for the chip. These are used by the architecture cache
 * libraries.
 */
const int  ppc440ICACHE_LINE_NUM = _ICACHE_LINE_NUM_440GX;
                        /* no. of lines in ICACHE */
const int  ppc440DCACHE_LINE_NUM = _DCACHE_LINE_NUM_440GX;
                        /* no. of lines in DCACHE */
const int  ppc440CACHE_ALIGN_SIZE = _CACHE_ALIGN_SIZE;
                        /* cache line size */

#if (defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL))

#define VEC_BASE_SIZE       0x4000  /* vector space size, set by libarch */

/*
 * Identify whether the VM library can manage 36-bit PLB physical
 * addresses.
 */

#ifdef MMU_ASID_MAX     /* Base 6 MMU library in effect */
  /* macro to compose 64-bit PHYS_ADDRs */
# define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))
#endif

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
 * Calculating size of page table required:
 * =======================================
 *
 * The size of each Level 1 table is 4K and the size of each Level 2
 * page table is 16K.
 *
 * For the following memory map we can calculate the page table size
 * required as follows:
 *
 *  Memory Area     Size    # of Level 2 pages
 *  ===========     ====    ==================
 *  1. Local Memory     128MB    32
 *  2. SRAM0 Memory     256KB    1
 *  3. I2O Registers    64KB     1
 *
 *  Total # of L2 pages =        34
 *  Total Memory Required for page table = 34 * 16K + 1 * 4K = 548 K.
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID  | VM_STATE_WRITABLE  | MEM_STATE
    },

    {
    (VIRT_ADDR) SRAM0_BASE_ADDR,
    (PHYS_ADDR) SRAM0_BASE_PLB,
    SRAM0_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID  | VM_STATE_WRITABLE  | MEM_STATE
    },

#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
    /*
     * Exceptions and interrupts which reenable MMU in the enter routine
     * need the vector page mapped when they return to the vector stub
     * to call the handler. Likewise for the handler when it returns to
     * call the exit routine.
     */
    {
    (VIRT_ADDR) VEC_BASE_ADRS,
    (PHYS_ADDR) VEC_BASE_ALT_ADRS,
    VEC_BASE_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID  | VM_STATE_WRITABLE_NOT  | MEM_STATE
    },
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */

    {
    (VIRT_ADDR) I2O_BASE_ADRS,
    (PHYS_ADDR) I2O_BASE_PLB,
    I2O_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE
        | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID  | VM_STATE_WRITABLE  | VM_STATE_CACHEABLE_NOT
        | VM_STATE_GUARDED
    }

#ifdef PHYS_64BIT_ADDR
    ,

#ifdef EBC0_MAPPED
    /* Peripherals -- internal - EBCO Memory Banks */
    {
    (VIRT_ADDR)EBC0_BASE_ADRS,
    PHYS_64BIT_ADDR(EBC0_PHYS_ADRS_HIGH, EBC0_PHYS_ADRS_LOW),
    EBC0_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | MEM_STATE
    },
#endif /* 0 */

    /* Peripherals -- 440 onchip
     * UART, IIC, OPBA, GPIO, ZMII, RGMII, EMAC, GPT, TAH
     */
    {
    (VIRT_ADDR)PERIPH_BASE_ADDR,
    PHYS_64BIT_ADDR(PERIPH_BASE_PLB_UA, PERIPH_BASE_PLB),
    PERIPH_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },
    /* Peripherals -- flash (bootrom) */
    {
    (VIRT_ADDR)CFG_FLASH_BASE,
    PHYS_64BIT_ADDR(BOOT_BASE_PLB_UA, CFG_FLASH_BASE),
    FLASH_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
#ifdef INCLUDE_TFFS
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
#else
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
#endif
    },
    

#ifdef INCLUDE_PCI
    /* PCI memory non-prefetchable */
    {
    (VIRT_ADDR)PCI_MSTR_MEMIO_LOCAL,
    PHYS_64BIT_ADDR(POM1_LOCAL_ADRS_HIGH, POM1_LOCAL_ADRS_LOW),
    PCI_MSTR_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },

    /* PCI memory prefetchable */
    {
    (VIRT_ADDR)PCI_MSTR_MEM_LOCAL,
    PHYS_64BIT_ADDR(POM0_LOCAL_ADRS_HIGH, POM0_LOCAL_ADRS_LOW),
    PCI_MSTR_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED_NOT
    },

    /* PCI IO */
    {
    (VIRT_ADDR)PCI_MSTR_IO_LOCAL,
    PHYS_64BIT_ADDR(PCIX0_BASE_PLB_UA, PCI_IO_ADDR),
    PCI_MSTR_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },

    /* PCI extra IO - not mapped */

    /* PCI configuration cycle */
    {
    (VIRT_ADDR)PCIX0_CFG_PAIR_ADRS,
    PHYS_64BIT_ADDR(PCIX0_BASE_PLB_UA, PCI_EXTRA_IO_ADDR ),
    PCIX0_CFG_PAIR_END - PCIX0_CFG_PAIR_ADRS + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },

    /* PCI bridge config registers */
    {
    (VIRT_ADDR)PCIX0_CONFG_BASE,
    PHYS_64BIT_ADDR(PCIX0_BASE_PLB_UA, PCI_BRIDGE_REG),
    PCIX0_CFG_END - PCIX0_CONFG_BASE + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },

    /* PCI special cycle space */
    {
    (VIRT_ADDR)PCIX0_SPECIAL_CYCLE,
    PHYS_64BIT_ADDR(PCIX0_BASE_PLB_UA, PCI_SPECIAL_CYCLE),
    PCIX0_SPECIAL_CYCLE_END - PCIX0_SPECIAL_CYCLE + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },
#endif /* INCLUDE_PCI */

    /* Peripherals -- Board -- FPGA */
    {
    (VIRT_ADDR)CFG_EBC1_FPGA_BASE,
    PHYS_64BIT_ADDR(BOOT_BASE_PLB_UA, TAISHAN_FPGA_ADDR_PH),
    TAISHAN_FPGA_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },
    /* Peripherals -- Board -- LCM */
    {
    (VIRT_ADDR)CFG_EBC2_LCM_BASE,
    PHYS_64BIT_ADDR(BOOT_BASE_PLB_UA, TAISHAN_LCM_ADDR_PH),
    TAISHAN_LCM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },
    /* Peripherals -- Board -- CONNECTOR */
    {
    (VIRT_ADDR)CFG_EBC3_CONN_BASE,
    PHYS_64BIT_ADDR(BOOT_BASE_PLB_UA, TAISHAN_CON_ADDR_PH),
    TAISHAN_CON_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
    },

#endif /* PHYS_64BIT_ADDR */

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
 *               #   USER_D_CACHE_MODE (default cache mode)
 *  Static Memory Region #
 *  Ideal Caching Mode   #  DISABLED    WRITETHROUGH    COPYBACK
 *  =====================#==========================================
 *  _INHIBIT         #  -I-     -I-     I--
 *  _INHIBIT_AND_G       #  -IG     -IG     IG-
 *  _WRITETHROUGH        #  -I-     W--     W--
 *  _WRITETHROUGH_OR_G   #  -IG     W--     W--
 *  _COPYBACK        #  -I-     W--     ---
 */

#define _INHIBIT            _MMU_TLB_ATTR_I
#define _INHIBIT_AND_G          _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
#if (USER_D_CACHE_MODE == CACHE_DISABLED || !defined(USER_D_CACHE_ENABLE))
# define _WRITETHROUGH      _MMU_TLB_ATTR_I
# define _WRITETHROUGH_OR_G _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
# define _COPYBACK      _MMU_TLB_ATTR_I
#elif (USER_D_CACHE_MODE == CACHE_WRITETHROUGH)
# define _WRITETHROUGH      _MMU_TLB_ATTR_W
# define _WRITETHROUGH_OR_G _MMU_TLB_ATTR_W
# define _COPYBACK      _MMU_TLB_ATTR_W
#elif (USER_D_CACHE_MODE == CACHE_COPYBACK)
# define _WRITETHROUGH      _MMU_TLB_ATTR_W
# define _WRITETHROUGH_OR_G _MMU_TLB_ATTR_W
# define _COPYBACK      0
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
  /* TLB #0.  Machine Check handler (uncached) */
  { VEC_BASE_ADRS + 0x4000, 0x0, 0x00004000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_16K
    | _INHIBIT              | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
#if (SW_MMU_ENABLE == 1)
    | _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX 
#endif
  },
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */
  /*
   * TLB #0 or 1.  RAM 0 (cached).  This includes the Machine Check handler
   * when not using the INCLUDE_440X5_TLB_RECOVERY_MAX option.
   */
  { VEC_BASE_ALT_ADRS, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
#if (SW_MMU_ENABLE == 1)
    | _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX 
#endif
  },
#ifndef INCLUDE_440X5_TLB_RECOVERY_MAX
  /* TLB #1 unless INCLUDE_440X5_TLB_RECOVERY_MAX.  RAM 0 (uncached) */
  { 0x80000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _INHIBIT              | _MMU_TLB_PERM_W
  },
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */
  /* TLB #2.  Peripherals */
  { 0xe0000000, 0x1, 0x40000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _INHIBIT_AND_G        | _MMU_TLB_PERM_W
  },
  /* TLB #3.  Boot Space. Note autoCalcSpeed() expects TLB #3 */
  { 0xf0000000, 0x1, 0xf0000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _WRITETHROUGH_OR_TFFS | _MMU_TLB_PERM_X | BOOT_SPACE_WRITE
  },
  /* TLB #4.  PCI Memory */
  { 0x90000000, 0x3, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _INHIBIT_AND_G        | _MMU_TLB_PERM_W
  },
  /* TLB #5.  PCI Memory */
  { 0xa0000000, 0x3, 0x10000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _INHIBIT              | _MMU_TLB_PERM_W
  },
  /* TLB #6.  I2O */
  { 0xb0000000, 0x0, 0xf0000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _INHIBIT_AND_G        | _MMU_TLB_PERM_W
  },
  /* TLB #7.  Int SRAM */
  { 0xc0000000, 0x0, 0x80000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  },
  /* TLB #8.  PCI */
  { 0xd0000000, 0x2, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _INHIBIT_AND_G        | _MMU_TLB_PERM_W
  },
  /* TLB #9.  RAM 1 (cached) */
  { 0x10000000, 0x0, 0x10000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  },
  /* TLB #10.  RAM 2 (cached) */
  { 0x20000000, 0x0, 0x20000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  },
  /* TLB #11.  RAM 3 (cached) */
  { 0x30000000, 0x0, 0x30000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  },
  /* TLB #12.  RAM 4 (cached) */
  { 0x40000000, 0x0, 0x40000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  },
  /* TLB #13.  RAM 5 (cached) */
  { 0x50000000, 0x0, 0x50000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  },
  /* TLB #14.  RAM 6 (cached) */
  { 0x60000000, 0x0, 0x60000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  },
  /* TLB #15.  RAM 7 (cached) */
  { 0x70000000, 0x0, 0x70000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _COPYBACK             | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
  }
#ifdef  INCLUDE_440X5_TLB_RECOVERY_MAX
  ,
  /* TLB #16.  RAM 0 (uncached) in INCLUDE_440X5_TLB_RECOVERY_MAX config. */
  { 0x80000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M
    | _INHIBIT              | _MMU_TLB_PERM_W
  },
  /* TLB #17.  vectors other than Machine Check (cached, execute-only) */
  { VEC_BASE_ADRS, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_16K
    | _COPYBACK             | _MMU_TLB_PERM_X
  }
#endif  /* INCLUDE_440X5_TLB_RECOVERY_MAX */

#ifndef PHYS_64BIT_ADDR
  /*
   * The below are used for accesses when the mmu is 'enabled'.
   * Add 2 to the stated TLB index numbers when using the
   * INCLUDE_440X5_TLB_RECOVERY_MAX configuration.
   */

  ,

  /* TLB #16.  Peripherals */
  { 0xe0000000, 0x1, 0x40000000, _MMU_TLB_TS_1 | _MMU_TLB_SZ_256M
    | _INHIBIT_AND_G        | _MMU_TLB_PERM_W
  },
  /* TLB #17.  Boot Space */
  { 0xf0000000, 0x1, 0xf0000000, _MMU_TLB_TS_1 | _MMU_TLB_SZ_256M
    | _WRITETHROUGH_OR_TFFS | _MMU_TLB_PERM_X | BOOT_SPACE_WRITE
  },
  /* TLB #18.  PCI Memory */
  { 0x90000000, 0x3, 0x00000000, _MMU_TLB_TS_1 | _MMU_TLB_SZ_256M
    | _INHIBIT_AND_G        | _MMU_TLB_PERM_W
  },
  /* TLB #19.  PCI Memory */
  { 0xa0000000, 0x3, 0x10000000, _MMU_TLB_TS_1 | _MMU_TLB_SZ_256M
    | _INHIBIT              | _MMU_TLB_PERM_W
  },
  /* TLB #20.  PCI */
  { 0xd0000000, 0x2, 0x00000000, _MMU_TLB_TS_1 | _MMU_TLB_SZ_256M
    | _INHIBIT_AND_G        | _MMU_TLB_PERM_W
  }
#endif /* PHYS_64BIT_ADDR */
};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#endif /* defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL) */

/* sysToMonitor looks for the Copyright string to find the warm boot address */

const char copyrightString[] = { 'C', 'o', 'p', 'y' };
const int  copyrightLen = 4;

/* Strapping structure for writing to CPU bootprom, see sysBootpromWrite */

typedef struct bootprom_info
{
  UINT32 sdr0_sdstp0;
  UINT32 sdr0_sdstp1;
  UINT32 sdr0_sdstp2;
  UINT32 sdr0_sdstp3;
} bootprom_info_t;

/* forward declarations */

STATUS  sysInfoGet(SYS_INFO * sysInfo);

UCHAR   sysPciInByte(ULONG address);
void    sysPciOutByte(ULONG address, UCHAR data);
UINT16  sysPciInWord(ULONG address);
void    sysPciOutWord(ULONG address, UINT16 data);
UINT    sysPciInLong(ULONG address);
void    sysPciOutLong(ULONG address, UINT data);
#if	defined(INCLUDE_END) && defined(INCLUDE_EMAC_VXB_END)
STATUS setEnetGroup(int groupNum, int phyModeE2, int phyModeE3);
#endif	

void   sysUsDelay(UINT32 delay);

#ifdef DOC
#define INCLUDE_TIMESTAMP
#endif /* DOC */

/* BSP Drivers */

#include "vme/nullVme.c"

#ifdef INCLUDE_PCI
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include "sysBusPci.c"
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_SCSI
#   include "drv/scsi/ncr810.h"
#endif /* INCLUDE_SCSI */

#ifdef INCLUDE_END
#   include "sysNet.c"
#endif /* INCLUDE_END */

#include "sysCpcr.c"                /* Chip Control Register access functions */
#include "l2Lib.c"                  /* L2 Controller configuration  functions */
#include "iicLib.c"                 /* IIC functions                          */

#ifdef INCLUDE_VXBUS
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>
#include "hwconf.c"

#ifdef INCLUDE_SIO_UTILS
IMPORT void    sysSerialConnectAll(void);
#endif /* INCLUDE_SIO_UTILS */
IMPORT void    hardWareInterFaceInit();

#endif /* INCLUDE_VXBUS */
#include "nvRamToFlash.c"
#include "flashMem.c"
#include "lcdLib.c"


/***************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string "IBM PowerPC 440GX Rev. x".
* Where x is either 1.0, 1.1, or 2.0.
*
* ERRNO
*/

char * sysModel (void)
    {
    switch (vxPvrGet())
        {
#ifdef  PPC440GX_REVA
        case PVR_440GX_RA:
            return(SYS_MODEL " Rev. 1.0");
#endif  /* PPC440GX_REVA */
#ifdef  PPC440GX_REVB
        case PVR_440GX_RB:
            return(SYS_MODEL " Rev. 2.0");
#endif  /* PPC440GX_REVA */
#ifdef PPC440GX_REVF
        case PVR_440GX_RF:
            return(SYS_MODEL " Rev. F");
#endif
        default:
            return("Unsupported processor");
        }
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

void ledOn(int value)
	{
	if (value == 1)
	    *(volatile unsigned int*)(CFG_PERIPHERAL_BASE+0x700) &= ~(0x00100000);
	else if (value == 2)
	    *(volatile unsigned int*)(CFG_PERIPHERAL_BASE+0x700) &= ~(0x00200000);		
	else 
		*(volatile unsigned int*)(CFG_PERIPHERAL_BASE+0x700) &= ~(0x00300000);		
	}

/***************************************************************************
*
* ledOff -  close the led 
*
* This routine close the led.
*
* RETURNS: N/A
*
* ERRNO
*/
void ledOff()
	{
    *(volatile unsigned int*)(CFG_PERIPHERAL_BASE+0x700) |= 0x00300000;
	}

/***************************************************************************
*
* ledInit - initialize the led
*
* This routine initializes the led.
*
* RETURNS: N/A
*
* ERRNO
*/
void ledInit()
	{
	
	dcrOutLong(SDR0_PFC0,dcrInLong(SDR0_PFC0) | 0x00300000);
	dcrOutLong(SDR0_PFC1,dcrInLong(SDR0_PFC1) | 0x00100000);
	
	*(volatile unsigned int*)(CFG_PERIPHERAL_BASE+0x718) &= ~(0x00300000);
	*(volatile unsigned int*)(CFG_PERIPHERAL_BASE+0x704) |= 0x00300000;
    *(volatile unsigned int*)(CFG_PERIPHERAL_BASE+0x700) |= 0x00300000;
	}	

/***************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the IBM 440GX evaluation
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
    int   value;
#endif
#ifndef INCLUDE_VXB_IBM_TAH
    UINT32 mfr;
#endif
    /* set up SRAM0 as memory and (optionally) arrange for L2 cache */
    l2SramInit();
#if (defined(INCLUDE_L2_CACHE) && defined(USER_D_CACHE_ENABLE) && \
     USER_D_CACHE_MODE != CACHE_DISABLED)
    l2CacheInit();
#endif /* INCLUDE_L2_CACHE... */
    /* Fill in global system information structure */

    sysInfoGet(&systemInfo);

    /* Support for MMU-less RTPs */
#if defined (INCLUDE_MMU_BASIC)
#if (SW_MMU_ENABLE == TRUE)
    mmu440TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], FALSE);
#endif /* SW_MMU_ENABLE is TRUE */
#endif /* INCLUDE_MMU_BASIC */

    /*
     * Initialize the timer clock mode in the CCR1 register. Determine what
     * frequency the timer clock is operating at.
     */

#ifdef TIMER_CLOCK_EXTERNAL
    vxCcr1Set(vxCcr1Get() | _PPC_CCR1_TCS);
#else  /* Timers run at the processor clock speed */
    vxCcr1Set(vxCcr1Get() & ~_PPC_CCR1_TCS);
#endif /* TIMER_CLOCK_EXTERNAL */

    sysTimerClkFreq = systemInfo.freqTMR;


    lcdInit();

#ifdef INCLUDE_PCI
    /* Initialize the PCIX Control Register */
    value = (SDR0_XCR_PHCE_EN  | SDR0_XCR_PISE_EN | 
             SDR0_XCR_PCWE_DIS | SDR0_XCR_PPIM_0  |
             SDR0_XCR_PR64E_EN | SDR0_XCR_PDM_MP);  

    value |= SDR0_XCR_PAE_EN;    /* enable onchip arbiter */


    sysSdrOutLong(SDR0_XCR, value);
    
#endif /* INCLUDE_PCI */

#ifndef INCLUDE_VXB_IBM_TAH

   /*
    * If TAH was enabled in 440GX, when the EMAC meet FIFO Overrun event, 
    * TAH will lock up and make MAL lock up. To workaround this, we disable
    * TAH by default, unless the vendor find a better way. 
    */

   mfr = sysSdrInLong(SDR0_MFR);
   mfr |= SDR0_MFR_TAH0_DIS | SDR0_MFR_TAH1_DIS;
   sysSdrOutLong(SDR0_MFR,mfr);
#endif /* INCLUDE_VXB_IBM_TAH */

#if (defined(INCLUDE_END) && defined(INCLUDE_EMAC_VXB_END))
   setEnetGroup (ENET_GROUP, EMAC2_PHY_MODE, EMAC3_PHY_MODE);
#endif

#ifdef  INCLUDE_VXBUS
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
#ifdef INCLUDE_VXBUS
        vxbDevInit();

        sysSdrOutLong(SDR0_UART0, (sysSdrInLong(SDR0_UART0) | SDR0_UARTX_UXEC_EXT));
        sysSdrOutLong(SDR0_UART1, (sysSdrInLong(SDR0_UART1) | SDR0_UARTX_UXEC_EXT));

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif
#endif /* INCLUDE_VXBUS */
        configured = TRUE;
        }
#ifdef INCLUDE_VXBUS
    taskSpawn("tDevConn", 11, 0, 10000,
              vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
#endif /* INCLUDE_VXBUS */
    }

/***************************************************************************
*
* sysMemSize - get the size of configured SDRAM 
*
* This routine retrieves the size of configured SDRAM.
*
* RETURNS: The address of the top of physical memory.
*
* ERRNO
*/

STATUS sysInfoGet
	(
	SYS_INFO    * sysInfo
	)
	{
	unsigned long strp0;
	unsigned long strp1;
	unsigned long temp;
	unsigned long temp1;
	unsigned long lfdiv;
	unsigned long m;
	unsigned long prbdv0;
 
/*
 * Macros for accessing the sdr controller registers
 */

#define PLLSYS0_FWD_DIV_A_MASK	0x00000f00	/* Fwd Div A */
#define PLLSYS0_FWD_DIV_B_MASK	0x000000e0	/* Fwd Div B */
#define PLLSYS0_FB_DIV_MASK	0x0001f000	/* Feedback divisor */
#define PLLSYS0_OPB_DIV_MASK	0x00000003	/* OPB Divisor */
#define PLLSYS1_PERCLK_DIV_MASK 0x03000000	/* Peripheral Clk Divisor */
#define PLLSYS0_SEL_MASK	0x38000000	/* 0 = PLL, 1 = CPU, 5 = PerClk */
#define PLLSYS1_LF_DIV_MASK	0xfc000000	/* PLL Local Feedback Divisor */
#define PLLSYS1_LF_DIV_MASK	0xfc000000	/* PLL Local Feedback Divisor */
#define PLLSYS0_SRC_MASK	0x40000000	/* 0 = PLL A, 1 = PLL B */
#define CONFIG_SYS_CLK_FREQ	33333333    /* external freq to pll	*/

 

    dcrOutLong(SDR0_CFGADDR, SDR0_SDSTP0);
    strp0 = dcrInLong(SDR0_CFGDATA);

    dcrOutLong(SDR0_CFGADDR, SDR0_SDSTP1);
    strp1 = dcrInLong(SDR0_CFGDATA);
	/* Extract configured divisors */

	temp = ((strp0 & PLLSYS0_FWD_DIV_A_MASK) >> 8);
	sysInfo->pllFwdDivA = temp ? temp : 16 ;
	temp = ((strp0 & PLLSYS0_FWD_DIV_B_MASK) >> 5);
	sysInfo->pllFwdDivB = temp ? temp: 8 ;
	temp = (strp0 & PLLSYS0_FB_DIV_MASK) >> 12;
	sysInfo->pllFbkDiv = temp ? temp : 32;
	temp = (strp0 & PLLSYS0_OPB_DIV_MASK);
	sysInfo->pllOpbDiv = temp ? temp : 4;
	temp = (strp1 & PLLSYS1_PERCLK_DIV_MASK) >> 24;
	sysInfo->pllExtBusDiv = temp ? temp : 4;
	prbdv0 = (strp0 >> 2) & 0x7;

	/* Calculate 'M' based on feedback source */
	temp = (strp0 & PLLSYS0_SEL_MASK) >> 27;
	temp1 = (strp1 & PLLSYS1_LF_DIV_MASK) >> 26;
	lfdiv = temp1 ? temp1 : 64;
	if (temp == 0) 
		{ /* PLL output */
		/* Figure which pll to use */
		temp = (strp0 & PLLSYS0_SRC_MASK) >> 30;
		if (!temp)
			m = sysInfo->pllFbkDiv * lfdiv * sysInfo->pllFwdDivA;
		else
			m = sysInfo->pllFbkDiv * lfdiv * sysInfo->pllFwdDivB;
		}
	else if (temp == 1) /* CPU output */
		m = sysInfo->pllFbkDiv * sysInfo->pllFwdDivA;
	else /* PerClk */
		m = sysInfo->pllExtBusDiv * sysInfo->pllOpbDiv * sysInfo->pllFwdDivB;

	/* Now calculate the individual clocks */
	sysInfo->freqVCO = (m * CONFIG_SYS_CLK_FREQ) + (m>>1);
	sysInfo->freqProcessor = sysInfo->freqVCO/sysInfo->pllFwdDivA;
	sysInfo->freqPLB = sysInfo->freqVCO/sysInfo->pllFwdDivB/prbdv0;
	sysInfo->freqOPB = sysInfo->freqPLB/sysInfo->pllOpbDiv;
	sysInfo->freqEBC = sysInfo->freqOPB/sysInfo->pllExtBusDiv;
	sysInfo->freqTMR = sysInfo->freqProcessor;
	
	return OK;
	}


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
* ERRNO
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    static char * physTop = NULL;

    if (physTop == NULL)
        {
#ifdef LOCAL_MEM_AUTOSIZE

        /*
         * LIMIT physTop to 2GB memory. This can be increased if the static
         * TLB memory maps established in resetEntry (romInit.s) and
         * sysStaticTlbDesc[] are altered.  The SDRAM controller hardware
         * has a maximum limit of 2GB.  Also, see sdramAutoConfig()
         * (romSdramInit.c)
         */
        physTop = (char *)min(sysMemSize(), MAPPED_SDRAM_SIZE);

#if (defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL))

        /*
         * based on the size of the memory we calculated, update the
         * sysPhysMemDesc structure, so that we map the right size of RAM.
         * We assume that the first element in the sysPhysMemDesc structure
         * is referring to RAM.
         */
        sysPhysMemDesc[0].len = (UINT32)physTop - LOCAL_MEM_LOCAL_ADRS;
#endif /* defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL) */


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
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM

        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif
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
    UINT32 * romLow  = (UINT32 *)(ROM_BASE_ADRS);
    UINT32 * romHigh = (UINT32 *)(ROM_BASE_ADRS + (RAM_LOW_ADRS / 2));
    FUNCPTR pRom;

    (void)intLock();                /* lock interrupts */
#if (defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL))
    VM_ENABLE (FALSE);              /* disable MMU */
#endif /* defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL) */

    cacheDisable(_DATA_CACHE);
    cacheDisable(_INSTRUCTION_CACHE);

    /* clear pending interrupts */
    dcrOutLong(UIC0_SR, 0xffffffff);
    dcrOutLong(UIC1_SR, 0xffffffff);
    dcrOutLong(UIC2_SR, 0xffffffff);
    dcrOutLong(UICB_SR, 0xffffffff);

    /*
     * This board can potentially be equipped with either a 512KB bootrom
     * occupying one of the 29LV040 sockets, or a 1MB bootrom/image occupying
     * both.  Look for the "Copyright ..." notice at both possible locations.
     * If found at the higher address, or only at the lower address, use
     * the corresponding warm boot entry point.  If found at neither,
     * use the hard reset vector instead.
     */
    if (strncmp((char *)(romHigh + 2), copyrightString, copyrightLen) == 0)
        pRom = (FUNCPTR)&romHigh[1];
    else
        if (strncmp((char *)(romLow + 2), copyrightString, copyrightLen) == 0)
        pRom = (FUNCPTR)&romLow[1];
    else
        pRom = (FUNCPTR)0xfffffffc;

    (*pRom) (startType);    /* jump to bootrom */

    return (OK);        /* in case we ever continue from ROM monitor */
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
* ERRNO
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return (0);
    }

/***************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine is supposed to set the processor number for the CPU board.
* Processor numbers should be unique on a single backplane.
*
* Since there is no VMEbus, the processor number is always 0.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysProcNumGet(),
*/

void sysProcNumSet
    (
    int procNum /* processor number (ignored in this BSP) */
    )
    {
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
* RETURNS: ERROR, always
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
    ULONG   address,    /* I/O address to write the byte to */
    UCHAR   data        /* byte to write */
    )
    {
    *(UCHAR *)address = data;

    EIEIO_SYNC;
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
    ULONG	address 	/* I/O address to read the byte from */
    )
    {
    UCHAR data = (*(UCHAR *)address);

    EIEIO_SYNC;

    return data;
    }

/***************************************************************************
*
* checkRate - validate clock rate within range
*
* This routine verifies the given clock is within the desired ranges
* and emits an error if not. It is a helper for sysInfoGet.
*
* RETURNS: OK or ERROR
*
* ERRNO
*
* \NOMANUAL
*/

STATUS checkRate
    (
    char *  rateName,   /* description of rate */
    UINT32  rateActual, /* actual rate value */
    UINT32  rateMin,    /* minimum acceptable rate */
    UINT32  rateMax,    /* maximum acceptable rate */
    UINT32  plld,       /* CPR0_PLLD register contents */
    UINT32  verbose     /* if nonzero, produce output */
    )
    {
    if (rateActual < rateMin || rateActual > rateMax)
        {
        if (verbose)
            {
            printf("Invalid %s frequency calculated :  %d MHz \a\n",
                   rateName, rateActual);
            printf("It must be between %d-%d MHz \a\n", rateMin, rateMax);
            printf("CPR0_PLLD reg          :  %8.8x\a\n", plld);
            }
        return ERROR;
        }

    return OK;
    }

/*************************************************************************
*
* sysUsDelay - delay for at least x microseconds.
*
* Use the timebase register, running at sysTimerClkFreq, to delay at least
* the requested number of microseconds.
*
* RETURNS: N/A
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
    UINT32  hi;         /* current timebase upper 32 bits */
    UINT32  lo;         /* current timebase lower 32 bits */
    UINT32  ticks;      /* number of ticks needed for delay */
    FAST UINT32 target_hi;      /* target timebase upper 32 bits */
    FAST UINT32 target_lo;      /* target timebase lower 32 bits */

    /* get start time */
    vxTimeBaseGet(&hi, &lo);

    /* Calculate number of ticks for desired elapsed time. */
    ticks = delay * (ROUND_UP(sysTimerClkFreq, 1000000) / 1000000);

    /* set target hi and lo, and account for rollover */
    target_lo = lo + ticks;         /* may cause rollover */
    target_hi = hi + (target_lo < lo ? 1 : 0);  /* account for rollover */

    /* read timebase until target reached */

    do
        {
        vxTimeBaseGet(&hi, &lo);
        }
    while (hi < target_hi || (hi == target_hi && lo < target_lo));

    return;
    }


#if	defined(INCLUDE_END) && defined(INCLUDE_EMAC_VXB_END)
/***************************************************************************
*
* setEnetGroup - perform initial high level initialization associated with a  
*                specific Ethernet Group
*
* This routine performs checking to ensure a valid Ethernet Group and PHY mode
* configuration has been defined, then does some initial global configuration
* associated with the Ethernet Group for the chip/board.
*
* The remaining PHY and PHY Bridge initialization is performed on a per EMAC
* basis via calls from the EMAC driver to the PHY set interface routine in 
* phyLib.
*
* Several configuration combinations are provided for the four on-chip
* ethernets
* 
*  Group0: MII
*  Group1: RMII0,RMII1
*  Group2: SMII0,SMII1,SMII2,SMII3
*  Group3: RMII0,RGMII2|RTBI2,DISABLE3
*  Group4: SMII0,SMII1,RGMII2|RTBI2,RGMII3|RTBI3 
*  Group4: SMII0,SMII1,GMII2/TBI2,DISABLE3 
*  Group4: SMII0,SMII1,DISABLE2, GMII3/TBI3,DISABLE3 
*  Group5: SMII0,SMII1,SMII2,RGMII3|RTBI3
*  Group6: SMII0,SMII1,RGMII2|RTBI2,DISABLE3
*
* RETURNS: OK, or ERROR 
*
* ERRNO
*/

STATUS setEnetGroup
   (
   int groupNum, 
   int phyModeE2, 
   int phyModeE3
   )
   {
   /* 
    * Check for valid Group number and PHY mode combinations.
    */

   if ((groupNum < 0) || (groupNum > 6))
      {
      return(ERROR);   /* invalid groupNum */
      }

   if ((groupNum == 0) && 
       ((phyModeE2 != PHY_MODE_DISABLED) || (phyModeE3 != PHY_MODE_DISABLED)))
      {
      return(ERROR);   /* invalid group 0 configuration */
      }

   if ((groupNum == 1) && 
       ((phyModeE2 != PHY_MODE_DISABLED) || (phyModeE3 != PHY_MODE_DISABLED)))
      {
      return(ERROR);   /* invalid group 1 configuration */
      }

   if ((groupNum == 2) && 
       ((phyModeE2 != PHY_MODE_SMII) || (phyModeE3 != PHY_MODE_SMII))) 
      {
      return(ERROR);   /* invalid group 2 configuration */
      }

   if ((groupNum == 3) && 
       (((phyModeE2 != PHY_MODE_RGMII) && (phyModeE2 != PHY_MODE_RTBI)) || 
         (phyModeE3 != PHY_MODE_DISABLED)))
      {
      return(ERROR);   /* invalid group 3 configuration */
      }

   if ((groupNum == 4) && ( ((phyModeE2 != PHY_MODE_RGMII) && 
                             (phyModeE2 != PHY_MODE_RTBI)) ||
                            ((phyModeE3 != PHY_MODE_RGMII) && 
                             (phyModeE3 != PHY_MODE_RTBI)) ) && 
                          ( ((phyModeE2 != PHY_MODE_GMII)  && 
                             (phyModeE2 != PHY_MODE_TBI)) || 
                             (phyModeE3 != PHY_MODE_DISABLED)) )
      {
      return(ERROR);   /* invalid group 4 configuration */ 
      }

   if ((groupNum == 5) && 
      ((phyModeE2 != PHY_MODE_DISABLED) || 
      ((phyModeE3 != PHY_MODE_RGMII) && (phyModeE3 != PHY_MODE_RTBI))))
      {
      return(ERROR);   /* invalid group 5 configuration */
      }

   if ((groupNum == 6) && ( ((phyModeE2 != PHY_MODE_RGMII) && 
                             (phyModeE2 != PHY_MODE_RTBI)) || 
                             (phyModeE3 != PHY_MODE_DISABLED)) )
      {
      return(ERROR);   /* invalid group 6 configuration */
      }
   /*
    * Program the Ethernet Group in the chip control register
    */

   sysSdrOutLong(SDR0_PFC1, (sysSdrInLong(SDR0_PFC1) & ~SDR0_PFC1_EPS) |
                 SDR0_PFC1_EPS_ENCODE(groupNum));
#ifdef notdef  
   /*
    * Clear out the RGMII and ZMII registers                  
    */

   sysPlbOutLong(RGMII0_FER, 0);
   sysPlbOutLong(RGMII0_SSR, 0);
   sysPlbOutLong(ZMII0_FER, 0);
   sysPlbOutLong(ZMII0_SSR, 0);
   sysPlbOutLong(ZMII0_SMIISR, 0);
#endif

   /*
    * If this is Ethernet Group 4 or 5, disable Trace on the muxed Gbit pins
    */

   if ((groupNum == 4) || (groupNum == 5))
      {
      sysSdrOutLong(SDR0_PFC0, (sysSdrInLong(SDR0_PFC0) & ~SDR0_PFC0_TRE));
      }

   return(OK);

   }
#endif	

#ifdef INCLUDE_EXC_SHOW
# include "440gxBusErrOriginShow.c"
#endif /* INCLUDE_EXC_SHOW */

#ifdef INCLUDE_SHOW_ROUTINES

/***************************************************************************
*
* sysInfoShow - Shows system information based on board switch settings.
*
* This routine displays information (clock frequencies, etc.) about 
* the current operating environment of the board.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS sysInfoShow
    (
    SYS_INFO *  sysInfo
    )
    {
    UINT32      cpr0_plld;
    UINT32      arbEnabledNum = 0;
    char *      pllFbk = "System PLL Bypassed";
    char *      timerClk = "External";
    char *      arbInternal = "Disabled";
    char *      arbExternal = "Disabled";
    char *      eccState = "Unknown state";

    /* default to systemInfo if nothing given */

    if (sysInfo == NULL)
        {
        sysInfo = &systemInfo;
        }

    printf("\n");
    if (sysInfo->flags & SI_PLL_CPU_CLOCK)
        pllFbk = "CPU clock";
    else if (sysInfo->flags & SI_PLL_EBC_CLOCK)
        pllFbk = "EBC clock";
    else if (sysInfo->flags & SI_PLL_LOCAL_A)
        pllFbk = "PLL Local (PLLOUTA)";
    else if (sysInfo->flags & SI_PLL_LOCAL_B)
        pllFbk = "PLL Local (PLLOUTB)";

    cpr0_plld    = sysCprInLong(CPR0_PLLD);
    if (checkRate("VCO", sysInfo->freqVCO / 1000000, VCO_MHZ_MIN, VCO_MHZ_MAX,
                  cpr0_plld, 1) != OK)
        {
        return ERROR;
        }
    if (checkRate("CPU", sysInfo->freqProcessor, CPU_HZ_MIN, CPU_HZ_MAX,
                  cpr0_plld, 1) != OK)
        {
        return ERROR;
        }
    if (checkRate("PLB", sysInfo->freqPLB, PLB_HZ_MIN, PLB_HZ_MAX,
                  cpr0_plld, 1) != OK)
        {
        return ERROR;
        }

#ifdef TIMER_CLOCK_EXTERNAL
    timerClk = "External";
#else  /* Timers run at the processor clock speed */
    timerClk = "Internal";
#endif /* TIMER_CLOCK_EXTERNAL */

    if (sysInfo->flags & SI_ARB_INTERNAL)
        {
        arbInternal = "Enabled";
        arbEnabledNum++;
        }
    if (sysInfo->flags & SI_ARB_EXTERNAL)
        {
        arbExternal = "Enabled";
        arbEnabledNum++;
        }

    /*
     * See whether ECC is enabled
     */
    dcrOutLong(SDRAM0_CFGADDR, SDRAM0_CFG0);
    switch (dcrInLong(SDRAM0_CFGDATA) & SDRAM_CFG0_MCHK_MASK)
        {
        case SDRAM_CFG0_MCHK_NON:
            eccState = "None";
            break;
        case SDRAM_CFG0_MCHK_CHK:
            break;
        case SDRAM_CFG0_MCHK_GEN:
            eccState = "Parity generation only";
            break;
        }

    /*
     * Output info.
     */

    printf(" %s   PVR = 0x%x\n", sysModel(), vxPvrGet());
    printf(" Config'd SDRAM Mem  = %d MB\n", (sysMemSize() / (1024 * 1024)));
    printf(" SysClk frequency    = %d MHz\n", sysInfo->freqSys / 1000000);
    printf(" VCO frequency       = %d MHz\n", sysInfo->freqVCO / 1000000);
    printf(" CPU frequency       = %d MHz\n", sysInfo->freqProcessor / 1000000);
    printf(" PLB frequency       = %d MHz\n", sysInfo->freqPLB   / 1000000);
    printf(" OPB frequency       = %d MHz\n", sysInfo->freqOPB   / 1000000);
    printf(" Ext Bus frequency   = %d MHz\n", sysInfo->freqEBC   / 1000000);
    printf(" MAL frequency       = %d MHz\n", sysInfo->freqMAL   / 1000000);
    printf(" Timer Clk frequency = %d MHz\n", sysInfo->freqEBC   / 1000000);
#ifdef INCLUDE_PCI
    printf(" PCI frequency       = %d MHz\n", sysInfo->freqPCI   / 1000000);
    printf("\n");
    printf(" Internal PCI Arbiter: %s\n", arbInternal);
    printf(" External PCI Arbiter: %s", arbExternal);
    if (arbEnabledNum > 1)
        printf("   ERROR: Only one PCI arbiter can be enabled\n");
    else
        printf("\n");
#else
    printf("\n");
#endif
    printf(" System PLL feedback:  %s\n", pllFbk);
#if	defined(INCLUDE_END) && defined(INCLUDE_EMAC_VXB_END)
    printf(" EMAC Configuration :  \n");
    printf("   ENET Group       :  %d\n", ENET_GROUP);
#endif	
    printf(" DDR SDRAM ECC      :  %s\n", eccState);
    printf(" Timer Clock        :  %s\n", timerClk);
    printf("\n");

    return OK;
    }
#endif /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_SIO_UTILS
#   define BSP_SERIAL_CHAN_GET  bspSerialChanGet
#else /* INCLUDE_SIO_UTILS */
#   define BSP_SERIAL_CHAN_GET  sysSerialChanGet
#endif /* INCLUDE_SIO_UTILS */

/*******************************************************************************
*
* BSP_SERIAL_CHAN_GET - get the serial channel
*
* This routine is called by the vxbus sio driver.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

SIO_CHAN *BSP_SERIAL_CHAN_GET
    (
    int channel
    )
    {
    return ((SIO_CHAN *)ERROR);
    }
/***************************************************************************
*
* sysOpbSpeedGet
*
* \NOMANUAL
*/

int sysOpbSpeedGet (void)
    {
    return systemInfo.freqOPB/1000000;
    }

/*******************************************************************************
*
* sysClkFreqGet - get the system clock.
*
* This routine gets the system clock freq.
*
* RETURNS: system clock
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet (void)
    {
    return sysTimerClkFreq;
    }
