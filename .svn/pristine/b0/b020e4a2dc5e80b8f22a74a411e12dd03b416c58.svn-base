/* sysLib.c - Freescale ads8572 (Whitefin) board system-dependent library */

/*
 * Copyright (c) 2007-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02f,08nov12,l_z  Fix the AMP device filter. (WIND00387440)
02e,11sep11,ers  Rolled back sysPhysMemDesc RAM settings. (WIND00253685)
02d,11jan11,cfm  Removed routines witch have conflicts with the ones in
                 vxbSmSupport.c if component INCLUDE_VXBUS_SM_SUPPORT is
                 included. (WIND00236270)
02c,27dec10,syt  updated LOCAL_MEM entry of sysPhysMemDesc.(WIND00247805)
02b,21dec10,swu  Modify CCSBAR mmu table size.(WIND00247711)
02a,01oct10,jpb  Removed import of _dbgDsmInstRtn.
01z,01jul10,liu  change vxIpiEmit to vxIpiPhysEmit. (WIND00212106)
01y,15mar10,rec  change INCLUDE_CPU_PWR_MGMT to INCLUDE_CPU_LIGHT_PWR_MGR
01x,09Jul09,rgo  Fix slow running romResident image,part1(wind00136185) 
01w,20may09,rgo  Fix compile warnings for sysMsDelay(wind138614)
01v,21nov08,pmr  WIND00143521: fix address map
01u,05nov08,pmr  WIND00140224: reset core1 while stopping it, shutdown ints
01t,03nov08,mdo  apigen documentation fixes
01s,30oct08,x_z  use new local FLASH driver. (WIND00121156)
01r,23oct08,kab  Update sysAmpCpuPrep to new sig per design mod
01q,16oct08,kab  Add sysAmpCpuPrep() - wrload/multios support
01p,04sep08,dtr  Change name of function startcore to sysAmpCpuEnable.
01o,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01o,27aug08,dtr  Add in sysBspFilter for AMP device allocation.
01n,15aug08,dtr  Update startcore for new reboot approach.
01m,18jun08,dtr  Change FORCE_DEFAULT_BOOTLINE
                 to be part of the INCLUDE_WRLOAD_IMAGE_BUILD component.
01l,30apr08,dtr  Add FORCE_DEFAULT_BOOTLINE feature.
01k,23apr08,dtr  Remove hard code IPI enable - WIND00121614
01j,18mar08,dtr  Match address space allocation to linux.
01i,29jan08,dtr  Fix clock calculations.
01h,17jan08,dtr  Fix compiler warnings.
01g,11jan08,dtr  Enable branch prediction.
01f,19dec07,dtr  Initialize decrementer for delay routines pre driver.
01e,29nov07,dtr  Remove fixed size of static TLB table.
01d,27nov07,dtr  Add CPU1_INIT_START_ADR init.
01c,14nov07,dtr  Fix GNU compilation in SMP.
01b,12oct07,dtr  Remove second PIXIS reference in sysPhysMemDesc for RTP
                 support fix. Use PIC for reboot not PIXIS.
01a,07aug07,dtr  Created from ads8544/sysLib.c/01g
*/

/*
DESCRIPTION
This library provides board-specific routines for ads8572 (Whitefin)

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vme.h>
#include <memLib.h>
#include <cacheLib.h>
#include <sysLib.h>
#include "config.h"
#include <string.h>
#include <intLib.h>
#include <logLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <arch/ppc/mmuE500Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>
#include <miiLib.h>

#ifdef _WRS_CONFIG_SMP
#include <private/cpcLibP.h>
#endif

#ifdef INCLUDE_PCI_BUS
    #include <drv/pci/pciConfigLib.h>
    #include <drv/pci/pciIntLib.h>
    #include "mot85xxPci.h"
#endif /* INCLUDE_PCI_BUS */

/* globals */
#ifndef AMP_LINUX
TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash + PIXIS */

    /* needed be first entry here */
    { 0xf0000000, 0x0, 0xf0000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
      _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
      _MMU_TLB_ATTR_G
    },

    /*
     * LOCAL MEMORY needs to be the second entry here  -
     * one TLB would be 256MB so use 1G to get the required 512MB
     */

    { 0x00000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1G |
      _MMU_TLB_PERM_W | _MMU_TLB_PERM_X |
      CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT| _MMU_TLB_ATTR_M
    },

    { CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    }

    /* All these are not protected */
    ,
    { CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_1 | _MMU_TLB_SZ_1M |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

#ifdef INCLUDE_L2_SRAM
    ,
    { L2SRAM_ADDR, 0x0, L2SRAM_ADDR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
      _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
      _MMU_TLB_ATTR_G
    }

#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_PCI_BUS
    ,
    { PCIEX1_MEM_ADRS, 0x0, PCIEX1_MEM_ADRS, _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

    ,
    { PCIEX2_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS, _MMU_TLB_TS_0 |
      PCI_MMU_TLB_SZ | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
    ,

    { PCIEX3_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS, _MMU_TLB_TS_0 |
      PCI_MMU_TLB_SZ | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

#endif  /* INCLUDE_PCI_BUS */

};
#else /* AMP_LINUX */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* Overwrite boot page */
    /* needed be first entry here */
    { 0xe0000000, 0x0, 0xe0000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
      _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
      _MMU_TLB_ATTR_G
    },

    { CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },

    { 0x00000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1G |
      _MMU_TLB_PERM_W | _MMU_TLB_PERM_X |
      CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT| _MMU_TLB_ATTR_M
    },

    { PIXIS_BASE, 0x0, PIXIS_BASE, _MMU_TLB_TS_0 | _MMU_TLB_SZ_64K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },

    /* All these are not protected */
    { CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_1 | _MMU_TLB_SZ_1M |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },

    { 0xe0000000, 0x0, 0xe0000000, _MMU_TLB_TS_1 | _MMU_TLB_SZ_256M |
      _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
      _MMU_TLB_ATTR_G
    }

#ifdef INCLUDE_L2_SRAM
    ,
    { L2SRAM_ADDR, 0x0, L2SRAM_ADDR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
      _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I |
      _MMU_TLB_ATTR_G
    }

#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_PCI_BUS
    ,
    { PCIEX1_MEM_ADRS, 0x0, PCIEX1_MEM_ADRS, _MMU_TLB_TS_0 | PCI_MMU_TLB_SZ |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

    ,
    { PCIEX2_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS, _MMU_TLB_TS_0 |
      PCI_MMU_TLB_SZ | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
    ,

    { PCIEX3_MEM_ADRS, 0x0, PCIEX2_MEM_ADRS, _MMU_TLB_TS_0 |
      PCI_MMU_TLB_SZ | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

#endif  /* INCLUDE_PCI_BUS */

};

#endif /* AMP_LINUX */

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#ifdef MMU_ASID_MAX     /* Base 6 MMU library in effect */
  /* macro to compose 64-bit PHYS_ADDRs */
# define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))
#endif

/*
* sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
* used by the MMU to translate addresses with single page (4k) granularity.
* PTE memory space should not, in general, overlap BAT memory space but
* may be allowed if only Data or Instruction access is mapped via BAT.
*
* Address translations for local RAM, memory mapped PCI bus, the Board Control and
* Status registers, the MPC8260 Internal Memory Map, and local FLASH RAM are set here.
*
* PTEs are held in a Page Table.  Page Table sizes are
* integer powers of two based on amount of memory to be mapped and a
* minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
* for 32-bit PowerPCs are:
*
* Total mapped memory		Page Table size
* -------------------		---------------
*        8 Meg			     64 K
*       16 Meg			    128 K
*       32 Meg			    256 K
*       64 Meg			    512 K
*      128 Meg			      1 Meg
*	.				.
*	.				.
*	.				.
*
* [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
*
*/
PHYS_MEM_DESC sysPhysMemDesc [] =
{
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_LOCAL_ADRS+LOCAL_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    },

    /* 
     * Mapping for alternate uAMP image. The primary CPU requires this mapping 
     * to load the secondary. The secondary (indicated by 
     * INCLUDE_WRLOAD_IMAGE_BUILD) requires this for a combination of graceful 
     * NULL pointer de-ref's, shared memory, and sometimes device driver 
     * resource sharing.  The math requires the physical memory be divided into 
     * two equal partitions one half the size of physical memory. End users 
     * should feel free to modify this as they see fit.
     */

    {
#ifdef INCLUDE_WRLOAD_IMAGE_BUILD
        (VIRT_ADDR) PHYS_MEM_START,
        (PHYS_ADDR) PHYS_MEM_START,
#else
        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
        (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE,
#endif /* INCLUDE_WRLOAD_IMAGE_BUILD */
        PHYS_MEM_SIZE - LOCAL_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE | VM_STATE_MEM_COHERENCY
    }

#ifdef INCLUDE_L2_SRAM
    ,
    {
        (VIRT_ADDR) L2SRAM_ADDR,
        (PHYS_ADDR) L2SRAM_ADDR,
        L2SRAM_WINDOW_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    }
#endif

#ifdef INCLUDE_LBC_CS3
    ,
    {
        (VIRT_ADDR) LBC_CS3_LOCAL_ADRS,
        (PHYS_ADDR) LBC_CS3_LOCAL_ADRS,
        LBC_CS3_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
#endif /* INCLUDE_LBC_CS3 */

#ifdef INCLUDE_PCI_BUS
    ,
    {
        (VIRT_ADDR) PCIEX1_MEM_ADRS,
        (PHYS_ADDR) PCIEX1_MEM_ADRS,
        PCIEX1_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCIEX1_MEMIO_ADRS,
        (PHYS_ADDR) PCIEX1_MEMIO_ADRS,
        PCIEX1_MEMIO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCIEX1_IO_ADRS,
        (PHYS_ADDR) PCIEX1_IO_ADRS,
        PCIEX1_IO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }

    ,
    {
        (VIRT_ADDR) PCIEX2_MEM_ADRS,
        (PHYS_ADDR) PCIEX2_MEM_ADRS,
        PCIEX2_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCIEX2_MEMIO_ADRS,
        (PHYS_ADDR) PCIEX2_MEMIO_ADRS,
        PCIEX2_MEMIO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCIEX2_IO_ADRS,
        (PHYS_ADDR) PCIEX2_IO_ADRS,
        PCIEX2_IO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }

    ,
    {
        (VIRT_ADDR) PCIEX3_MEM_ADRS,
        (PHYS_ADDR) PCIEX3_MEM_ADRS,
        PCIEX3_MEM_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCIEX3_MEMIO_ADRS,
        (PHYS_ADDR) PCIEX3_MEMIO_ADRS,
        PCIEX3_MEMIO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
        (VIRT_ADDR) PCIEX3_IO_ADRS,
        (PHYS_ADDR) PCIEX3_IO_ADRS,
        PCIEX3_IO_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }


#endif /* INCLUDE_PCI_BUS */
    ,
    {
        (VIRT_ADDR) FLASH_BASE_ADRS,
        (PHYS_ADDR) FLASH_BASE_ADRS,
        MAIN_FLASH_SIZE,
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
        VM_STATE_MASK_GUARDED,
        VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
        VM_STATE_GUARDED
    }

};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

/* Clock Ratio Tables */
#define MAX_VALUE_DDR_RATIO 16
UINT32 ddrRatioTable[MAX_VALUE_DDR_RATIO] = { 0,0,0,3,4,0xffffffff,6,0,8,0,10,0,12,0,14,0};

#define MAX_VALUE_PLAT_RATIO 32

UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 4, 0},
    { 5, 0},
    { 6, 0},
    { 0, 0},
    { 8, 0},
    { 0, 0},
    { 10, 0},
    { 0, 0},
    { 12, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0}
};

#define MAX_VALUE_E500_RATIO 10
UINT32 e500RatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
    { 0, 0},
    { 0, 0},
    { 1, 0},
    { 3, 1},
    { 2, 0},
    { 5, 1},
    { 3, 0},
    { 7, 1},
    { 0, 0},
    { 0, 0}
};

int   sysBus      = BUS_TYPE_NONE;                /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (PPC8260) */
char *sysBootLine = BOOT_LINE_ADRS; /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;   /* catastrophic message area */
int   sysProcNum;           /* processor number of this CPU */
BOOL  sysVmeEnable = FALSE;     /* by default no VME */
UINT32  coreFreq,core1Freq,ddrFreq,systemFreq;

void sysMsDelay(UINT);
void sysUsDelay(int);

#ifdef _WRS_CONFIG_SMP
#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;	/* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;	/* lower 32 bit value of timebase */
#endif	/* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
#endif /* _WRS_CONFIG_SMP */

IMPORT void (*_vxb_msDelayRtn)(UINT);
IMPORT void (*_vxb_usDelayRtn)(int);

IMPORT void     mmuE500TlbDynamicInvalidate();
IMPORT void     mmuE500TlbStaticInvalidate();
IMPORT void mmuE500TlbStaticInit (int numDescs,
                                  TLB_ENTRY_DESC *pTlbDesc,
                                  BOOL cacheAllow);
IMPORT BOOL     mmuPpcIEnabled;
IMPORT BOOL     mmuPpcDEnabled;
IMPORT void     sysIvprSet(UINT32);

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void     vxEnableBP();
IMPORT void     vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

/* forward declarations */

void   sysUsDelay (int);

#ifdef INCLUDE_L1_IPARITY_HDLR_INBSP
    #define _EXC_OFF_L1_PARITY 0x1500
IMPORT void jumpIParity();
IMPORT void sysIvor1Set(UINT32);
UINT32 instrParityCount = 0;
#endif  /* INCLUDE_L1_IPARITY_HDLR_INBSP */

IMPORT void vxSdaInit (void);

/* 8260 Reset Configuration Table (From page 9-2 in Rev0 of 8260 book) */
#define END_OF_TABLE 0

UINT32 sysClkFreqGet(void);
UINT32 ppcE500ICACHE_LINE_NUM = (128 * 12);
UINT32 ppcE500DCACHE_LINE_NUM = (128 * 12);

UINT32 ppcE500CACHE_ALIGN_SIZE = 32;


#ifdef	_WRS_CONFIG_SMP
LOCAL void sysCpu1ShutdownLoop (void);
#if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))
STATUS sysSmpTimeBaseSync (void);
#endif	/* DRV_TIMER_DEC_PPC && INCLUDE_SYS_TIMESTAMP */
#endif	/* _WRS_CONFIG_SMP */

#ifdef INCLUDE_I2C
#include "sysMotI2c.c"
#include "sysMpc85xxI2c.c"
#endif

#ifdef INCLUDE_FLASH
    #include "flashMem.c"
    #include <mem/nvRamToFlash.c>
#else
    #include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */


#ifdef INCLUDE_L1_IPARITY_HDLR
    #include "sysL1ICacheParity.c"
#endif

UINT32 inFullVxWorksImage=FALSE;

#define EXT_VEC_IRQ0            56
#define EXT_NUM_IRQ0            EXT_VEC_IRQ0
#define EXT_MAX_IRQS            200

STATUS  sysIntEnablePIC     (int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable      (int intNum);
STATUS  sysCascadeIntDisable     (int intNum);
void    flashTest(VUINT16 *address,VUINT16 *buffer,VINT32 length);

UINT32   baudRateGenClk;
#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif
#ifdef _WRS_CONFIG_SMP
LOCAL	UINT32 physCpuAvailableGet (void);
#endif

#include "sysL2Cache.c"


#ifdef INCLUDE_TFFS
    #include "am29lv64xdMtd.c"
#endif /* INCLUDE_TFFS */

#if DEBUGGING
#include <private/dbgLibP.h>	/* _dbgDsmInstRtn */
#endif

#ifdef	_WRS_CONFIG_SMP
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheArchDisableFromMmu (CACHE_TYPE cache);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
IMPORT STATUS vxbIntToCpuRoute (unsigned int cpu);
#endif	/* _WRS_CONFIG_SMP */


#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */

#define WB_MAX_IRQS 256


/* defines */

#define ZERO    0

#define SYS_MODEL_8572E   "Freescale MPC8572E - Security Engine"
#define SYS_MODEL_8572    "Freescale MPC8572"
#define SYS_MODEL "Freescale MPC8572"

#define SYS_MODEL_E500    "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN "Freescale Unknown processor"


#define DELTA(a,b)                 (abs((int)a - (int)b))
#define HID0_MCP 0x80000000
#define HID1_ABE 0x00001000
#define HID1_ASTME 0x00002000
#define HID1_RXFE  0x00020000


#ifdef INCLUDE_VXBUS
#include "hwconf.c"
#endif
IMPORT device_method_t * pSysPlbMethods;

LOCAL BOOL sysCpu1Check (void);
METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);
#ifdef _WRS_VX_AMP
METHOD_DECL(sysBspDevFilter);
STATUS sysDeviceFilter(VXB_DEVICE_ID pDev);
void sysAmpCpuStatusSet(int status,int cpu);
int sysAmpCpuStatusGet(int cpu);
#endif

LOCAL struct vxbDeviceMethod ads8572PlbMethods[] =
    {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysCpu1Check),
#ifdef _WRS_VX_AMP
	/*
         * Only used for AMP filtering per core for now so not built for
	 * anything else
	 */
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
#endif
    { 0, 0 }
    };
IMPORT void sysCpu1Start (void);
IMPORT void sysCpu1Stop (void);

#ifdef INCLUDE_NETWORK
#include "sysNet.c"
#endif

#ifdef INCLUDE_SM_COMMON
#include "sysSmEnd.c"
#endif


#ifdef INCLUDE_L2_SRAM
LOCAL void sysL2SramEnable(BOOL both);
#endif /* INCLUDE_L2_SRAM */

#ifdef INCLUDE_SPE
    #include <speLib.h>
IMPORT int       (* _func_speProbeRtn) () ;
#endif /* INCLUDE_SPE */

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL void sysL1CacheQuery();
#endif

UINT32 sysTimerClkFreq;

IMPORT  void    sysL1Csr1Set(UINT32);
IMPORT  UINT    sysTimeBaseLGet(void);

IMPORT BOOL e500_spe_exc_enable;
IMPORT BOOL e500_spe_exc_mask;

LOCAL char * physTop = NULL;
LOCAL char * memTop = NULL;

#ifdef IEEE754_FIX
LOCAL SPE_CONTEXT *speCtx;
#endif

#ifdef _WRS_VX_AMP
typedef struct {
	char* deviceName;
	int   unit;
	int   cpu;
	}AMP_CPU_TABLE;

/*
 * Table includes only devices we want to filter
 * if no action required for a device per CPU then it's not in the table
 */
AMP_CPU_TABLE ampCpuTable[] = {
    { XTSEC_NAME, 0, 0 },
    { XTSEC_NAME, 1, 0 },
    { XTSEC_NAME, 2, 1 },
    { XTSEC_NAME, 3, 1 },
    { "ns16550", 0, 0},
    { "ns16550", 1, 0 },
    { "m85xxCCSR", 0, 0 },
    { "openPicTimer", 0,  0 },
    { "openPicTimer", 1,  0 },
    { "m85xxPci", 0, 0 },
    { "m85xxPci", 1, 0 },
    { "m85xxPci", 2, 0 }
	};

/*****************************************************************************
*
* sysDeviceFilter - prevent device from being announced
*
* Called from vxbus to prevent device from being announced and therefore used
* by vxWorks. The default is to return OK.
*
* RETURNS: OK or ERROR
*
* \NOMANUAL
*/

STATUS sysDeviceFilter
    (
    VXB_DEVICE_ID pDev
    )
    {

    /*
     * Compare devices name and unit number with those you want to remove
     * at run-time. Used here to selectively use devices on one cpu vs another.
     */
	int deviceIndex;
	BOOL deviceMatch;

	deviceMatch=FALSE;

	if (pDev->pName == NULL)
		return OK;

	for (deviceIndex=0;deviceIndex<NELEMENTS(ampCpuTable);deviceIndex++)
		{
		if ( (strcmp(pDev->pName,ampCpuTable[deviceIndex].deviceName)==OK) &&
			 (pDev->unitNumber==ampCpuTable[deviceIndex].unit) )
			{
			deviceMatch=TRUE;
			break;
			}
		}

	/* return OK if no match device - assume pass through */
	if (deviceMatch == FALSE)
		return(OK);

	/* if device match then we need to decide whether to filter */

	if(ampCpuTable[deviceIndex].cpu!=(sysCpu1Check()))
		return(ERROR);

	return(OK);
    }

#endif /* _WRS_VX_AMP */

/*****************************************************************************
*
* startCPU1 - Starts CPU1 executing code
*
* RETURNS: NONE
*
* \NOMANUAL
*/

void startCPU1 (void)
    {
    sysCpu1Start ();
    }

#ifndef INCLUDE_VXBUS_SM_SUPPORT

/*****************************************************************************
*
* sysBusTas - Test and Set
*
* RETURNS: STATUS
*
* \NOMANUAL
*/

STATUS sysBusTas
    (
    char *adrs
    )
    {
	return (vxTas(adrs));
    }

#endif /* INCLUDE_VXBUS_SM_SUPPORT */

/*****************************************************************************
*
* sysBusClear - Clears a reservation
*
* RETURNS: NONE
*
* \NOMANUAL
*/

void sysBusClear
    (
    char *adrs
    )
    {
    *adrs=0;
    WRS_ASM("isync;sync;eieio");
    }

#ifndef INCLUDE_VXBUS_SM_SUPPORT

/*****************************************************************************
*
* sysBusIntGen - Interrupts adjacent CPU
*
* RETURNS: STATUS
*
* \NOMANUAL
*/

STATUS sysBusIntGen
    (
    int level,          /* interrupt level to generate   */
    int vector          /* interrupt vector for interrupt*/
    )
    {
#if defined(_WRS_VX_AMP)
    vxIpiPhysEmit (0, (1 << !CPU1CHECK));
#endif /* _WRS_VX_AMP */
    return (OK);
    }

/*******************************************************************************
*
* sysMailboxConnect - connect a routine to the mailbox interrupt
*
* This routine specifies the interrupt service routine to be called at each
* mailbox interrupt.
*
* RETURNS: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), sysMailboxEnable()
*/

STATUS sysMailboxConnect
    (
    FUNCPTR routine,    /* routine called at each mailbox interrupt */
    int arg             /* argument with which to call routine      */
    )
    {

    return (OK);
    }

/*****************************************************************************
*
* sysMailboxEnable - enable the mailbox interrupt
*
* This routine enables the mailbox interrupt.
*
* RETURNS: OK, if mailbox interrupt becomes enabled, else ERROR.
*
* SEE ALSO: sysMailboxConnect(), sysMailboxDisable()
*/

STATUS sysMailboxEnable
    (
    char *mailboxAdrs           /* address of mailbox (ignored) */
    )
    {

    /* enable the mailbox 0 interrupt */

    return (OK);
    }

#endif /* INCLUDE_VXBUS_SM_SUPPORT */

/****************************************************************************
*
* sysMailboxDisable - disable the mailbox interrupt
*
* This routine disables the mailbox interrupt.
*
* RETURNS: OK, if the mailbox interrupt gets disabled, else ERROR
*
* SEE ALSO: sysMailboxConnect(), sysMailboxEnable()
*/

STATUS sysMailboxDisable
    (
    char *mailboxAdrs           /* address of mailbox (ignored) */
    )
    {

    /* disable the mailbox interrupt */

    return (OK);
    }

#ifndef INCLUDE_VXBUS_SM_SUPPORT

/*******************************************************************************
*
* sysBusIntAck - Acknowledge bus interrupt
*
* This routine acknowledges bus interrupts
*
* RETURNS: 0
*
* \NOMANUAL
*/

int sysBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge */
    )
    {
    return (0);
    }

#endif /* INCLUDE_VXBUS_SM_SUPPORT */

#if     defined (INCLUDE_SPE)

/*******************************************************************************
*
* sysSpeProbe - Check if the CPU has SPE unit
*
* This routine returns OK it the CPU has an SPE unit in it.
* Presently it assumes available.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

int  sysSpeProbe (void)
    {
    ULONG regVal;
    int speUnitPresent = OK;

    /* The CPU type is indicated in the Processor Version Register (PVR) */

    regVal = 0;

    switch (regVal)
        {
        case 0:
        default:
            speUnitPresent = OK;
            break;
        }      /* switch  */

    return(speUnitPresent);
    }
#endif  /* INCLUDE_SPE */


/****************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*
* ERRNO: N/A
*/

char * sysModel (void)
    {
    UINT32 device;
    char* retChar = NULL;

    device = *M85XX_SVR(CCSBAR);

    switch(device & 0xffff0000)
	{
	case 0x80E80000:
	    retChar = SYS_MODEL_8572E;
	    break;
	case 0x80E00000:
	    retChar = SYS_MODEL_8572;
	    break;
	default:
	    retChar = SYS_MODEL_E500;
	    break;
	}


    device = *M85XX_PVR(CCSBAR);

    if ((device & 0xfff00000) != 0x80200000)
        retChar =SYS_MODEL_UNKNOWN;


    return(retChar);

    }

/******************************************************************************
*
* sysBspRev - return the BSP version with the revision eg 1.0/<x>
*
* This function returns a pointer to a BSP version with the revision.
* for eg. 1.0/<x>. BSP_REV defined in config.h is concatenated to
* BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO: N/A
*/

char * sysBspRev (void)
    {
    return(BSP_VERSION BSP_REV);
    }
/******************************************************************************
*
* sysClkFreqGet - returns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet (void)
    {
    UINT32  sysClkFreq,oscFreq;
    UINT32 e500Ratio,e5001Ratio,platRatio,ddrRatio;
    UINT8 osc,ddr;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);
    ddrRatio = M85XX_PORPLLSR_DDR_RATIO(CCSBAR);

#ifdef FORCE_DEFAULT_FREQ
    return(DEFAULT_SYSCLKFREQ);
#endif

    if ((platRatio>=MAX_VALUE_PLAT_RATIO)||(platRatioTable[platRatio][0]==0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */
    osc = *PIXIS_OSC(PIXIS_BASE) & 0x7;

    switch (osc)
	{
	case PIXIS_OSC_33_MHZ:
	    oscFreq = FREQ_33_MHZ;
	    break;
	case PIXIS_OSC_40_MHZ:
	    oscFreq = FREQ_40_MHZ;
	    break;
	case PIXIS_OSC_50_MHZ:
	    oscFreq = FREQ_50_MHZ;
	    break;
	case PIXIS_OSC_66_MHZ:
	    oscFreq = FREQ_66_MHZ;
	    break;
	case PIXIS_OSC_83_MHZ:
	    oscFreq = FREQ_83_MHZ;
	    break;
	case PIXIS_OSC_100_MHZ:
	    oscFreq = FREQ_100_MHZ;
	    break;
	case PIXIS_OSC_133_MHZ:
	    oscFreq = FREQ_133_MHZ;
	    break;
	default:
	    oscFreq = OSCILLATOR_FREQ;
	    break;
	}

    systemFreq = oscFreq;

    ddr = (*PIXIS_OSC(PIXIS_BASE) >> 3) & 0x7;

    switch (ddr)
	{
	case PIXIS_DDR_33_MHZ:
	    ddrFreq = FREQ_33_MHZ;
	    break;
	case PIXIS_DDR_40_MHZ:
	    ddrFreq = FREQ_40_MHZ;
	    break;
	case PIXIS_DDR_50_MHZ:
	    ddrFreq = FREQ_50_MHZ;
	    break;
	case PIXIS_DDR_66_MHZ:
	    ddrFreq = FREQ_66_MHZ;
	    break;
	case PIXIS_DDR_83_MHZ:
	    ddrFreq = FREQ_83_MHZ;
	    break;
	case PIXIS_DDR_100_MHZ:
	    ddrFreq = FREQ_100_MHZ;
	    break;
	case PIXIS_DDR_133_MHZ:
	    ddrFreq = FREQ_133_MHZ;
	    break;
	default:
	    ddrFreq = OSCILLATOR_FREQ;
	    break;
	}

    ddrFreq = ddrFreq * ddrRatioTable[ddrRatio];

    sysClkFreq = ((UINT32)(oscFreq * platRatioTable[platRatio][0]))>>((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    e5001Ratio = M85XX_PORPLLSR_E500_1_RATIO(CCSBAR);

    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0]))>>((UINT32)e500RatioTable[e500Ratio][1]);
    core1Freq = ((UINT32)(sysClkFreq * e500RatioTable[e5001Ratio][0]))>>((UINT32)e500RatioTable[e5001Ratio][1]);

    return(sysClkFreq);
    }

/*******************************************************************************
*
* sysPicClkFreqGet - return the OpenPIC timer count frequency
*
* Given the selected picClockDivisor value, this routine calculates
* and returns the resulting OpenPIC timer count frequency.
*
* RETURNS: the OpenPIC timer count frequency
*/

UINT32 sysPicClkFreqGet
    (
    UINT32  picClockDivisor	/* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
	return sysClkFreqGet() / picClockDivisor;
    else
	return HPCN_RTC_FREQ;	/* 14.318 MHz */
    }


/******************************************************************************
*
* sysHwMemInit - initialize and configure system memory
*
* This routine is called before sysHwInit(). It performs memory auto-sizing
* and updates the system's physical regions table, `sysPhysRgnTbl'. It may
* include the code to do runtime configuration of extra memory controllers.
*
* NOTE: This routine should not be called directly by the user application.  It
* cannot be used to initialize interrupt vectors.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwMemInit (void)
    {
    /* Call sysPhysMemTop() to do memory autosizing if available */

    sysPhysMemTop ();

    }

#ifdef IEEE754_FIX

#define DF_NAN          ((double) (0.0/0.0))

LOCAL void fpPrintAdrs(int address)
{
printf ("0x%08x", address);
}

LOCAL void  fixSpeIssue(ESFPPC *esfppc,SPE_CONTEXT *speCtx)
    {
    REG_SET  *pRegs = &esfppc->regSet;		/* pointer to register on esf */
    UINT32  vecNum = esfppc->vecOffset;	/* exception vector number */
    UINT32 destReg;

    destReg = (*(UINT32*)pRegs->pc & 0x03e00000) >> 21;

#if DEBUGGING

    (*_dbgDsmInstRtn) (pRegs->pc, (int)pRegs->pc, fpPrintAdrs,
		       NULL, NULL);


    if(vecNum == _EXC_OFF_VEC_DATA)
	{

	logMsg("Exception FP Data. \n PC: 0x%x \n FPSCR 0x%x GPR%d: 0x%08x%08x\n",pRegs->pc,pRegs->spefscr,destReg, speCtx->gpr[destReg],pRegs->gpr[destReg],6);

	}


    if(vecNum == _EXC_OFF_VEC_RND)
	{
	logMsg("Exception FP Round.\nPC: 0x%x \n FPSCR 0x%x GPR%d: 0x%08x%08x\n",pRegs->pc,pRegs->spefscr,destReg,speCtx->gpr[destReg],pRegs->gpr[destReg],6);
	}
#endif

    /* Test updating destination reg */
    speCtx->gpr[destReg]=0x7ff80000;
    pRegs->gpr[destReg]=0;

    /* PC ahead after emulation of instruction complete */
    pRegs->pc +=4;
    pRegs->spefscr &= ~0xffffff00;

    }



void dpfpHandler(ESFPPC *esfppc)
    {
    int key;
    UINT32 msrVal;

    key = intCpuLock();
    speSave(speCtx);
    msrVal = vxMsrGet();
    vxMsrSet(msrVal | _PPC_MSR_SPE);

    fixSpeIssue(esfppc,speCtx);

    speRestore(speCtx);
    vxMsrSet(msrVal);
    intCpuUnlock(key);
    };
#endif
/******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the MPC8260 ADS board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/


void sysHwInit (void)
    {
    vxIvprSet((UINT32)VEC_BASE_ADRS);

#ifdef _WRS_VX_AMP
#ifdef INCLUDE_AMP_CPU_00
	sysAmpCpuStatusSet(ampCpuStateReboot,1);
	sysAmpCpuStatusSet(ampCpuStateRunning,0);
#else
	sysAmpCpuStatusSet(ampCpuStateRunning,1);
#endif
#endif /* _WRS_VX_AMP */
#ifdef _WRS_CONFIG_SMP
    /*
     * Here we initialize cpu1 init address so we don't wait until the
     * correct value is initialized
     */
	*(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_RBV;
#endif

    /* 
     * The default compile-time delay funtion are set to sw delay. 
     * If the BSP support the timer, use the BSP version because it is more
     * precise, until this vxb delay driver LIB is initialized and connected.
     * The precision will be noticable when image is rom-resident.
     */

    _vxb_msDelayRtn = sysMsDelay;
    _vxb_usDelayRtn = sysUsDelay;

    /* Disable L1 Icache */
    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* Check for architecture support for 36 bit physical addressing */
#if defined(PPC_e500v2)
    vxHid0Set(_PPC_HID0_MAS7EN|vxHid0Get());
#endif
    /* Enable machine check pin */
    vxHid0Set(HID0_MCP|vxHid0Get());

#ifdef E500_L1_PARITY_RECOVERY
    /* Enable Parity in L1 caches */
    vxL1CSR0Set(vxL1CSR0Get() | _PPC_L1CSR_CPE);
    vxL1CSR1Set(vxL1CSR1Get() | _PPC_L1CSR_CPE);
#endif  /* E500_L1_PARITY_RECOVERY */

    /* enable time base for delay use before DEC interrupt is setup */
    vxDecSet(0xffffffff);
    vxDecarSet(0xffffffff);
    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

    sysTimerClkFreq = OSCILLATOR_FREQ;

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_I2C
    i2cDrvInit(0,0);
    i2cDrvInit(1,0);
#endif

    /* Initialize L2CTL register */
    vxL2CTLSet(0x28000000,M85XX_L2CTL(CCSBAR));

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (!defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL))
    mmuPpcIEnabled=TRUE;
    mmuPpcDEnabled=TRUE;
#else /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */
    if (inFullVxWorksImage==FALSE)
        {
        mmuPpcIEnabled=TRUE;
        mmuPpcDEnabled=TRUE;
        }
    /* Enable I Cache if instruction mmu disabled */
#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    mmuPpcIEnabled=TRUE;
#endif /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */
#if (SW_MMU_ENABLE == TRUE)

        mmuPpcIEnabled=TRUE;
        mmuPpcDEnabled=TRUE;

#endif

    vxHid1Set(HID1_ABE); /* Address Broadcast enable */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2CacheInit();
#endif /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    /* Machine check via RXFE for RIO */

    vxHid1Set(vxHid1Get()| HID1_ASTME | HID1_RXFE); /* Address Stream Enable */
#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    WRS_ASM("isync");

#ifdef IEEE754_FIX

    speCtx = (SPE_CONTEXT*)memalign(_CACHE_ALIGN_SIZE,sizeof(SPE_CONTEXT));


    e500_spe_exc_enable = TRUE;
    e500_spe_exc_mask = (_PPC_SPEFSCR_FINVE |
			 _PPC_SPEFSCR_FDBZE | _PPC_SPEFSCR_FUNFE |
			 _PPC_SPEFSCR_FOVFE);

    excConnect(_EXC_OFF_VEC_DATA,dpfpHandler);
    excConnect(_EXC_OFF_VEC_RND,dpfpHandler);
#else
    e500_spe_exc_enable = FALSE;
    e500_spe_exc_mask = 0;
#endif

    pSysPlbMethods = ads8572PlbMethods;

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */


#ifdef E500_L1_PARITY_RECOVERY
    vxIvor1Set(_EXC_OFF_L1_PARITY);
#endif  /* E500_L1_PARITY_RECOVERY */
#ifdef INCLUDE_L1_IPARITY_HDLR
    installL1ICacheParityErrorRecovery();
#endif /* INCLUDE_L1_IPARITY_HDLR */

#if defined(INCLUDE_L2_SRAM)
#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2SramEnable(TRUE);
#elif (defined(INCLUDE_L2_SRAM))
    sysL2SramEnable(FALSE);
#endif
#endif

    CACHE_PIPE_FLUSH();

    }

#ifdef INCLUDE_L2_SRAM
/*************************************************************************
*
* sysL2SramEnable - enables L2SRAM if L2SRAM only
*
* This routine enables L2SRAM if L2SRAM only or initializes blk
* size etc and leaves the rest to L2 cache code.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL2SramEnable
    (
    BOOL both
    )
    {
    volatile int l2CtlVal;

    /* if INCLUDE_L2_CACHE and CACHE_SUPPORT */
    /* if ((L2_SRAM_SIZE + L2_CACHE_SIZE) > l2Siz) */
    /* Setup Windows for L2SRAM */
    *(M85XX_L2SRBAR0(CCSBAR)) = (UINT32)(L2SRAM_ADDR & M85XX_L2SRBAR_ADDR_MSK);
    /* Get present value */
    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));

    /* Disable L2CTL initially to allow changing of block size */
    l2CtlVal &=(~M85XX_L2CTL_L2E_MSK);
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    l2CtlVal &= ~M85XX_L2CTL_L2SRAM_MSK;

    if (both == TRUE)
        {
        /* Setup as half and half */
        l2CtlVal |= (0x2 << M85XX_L2CTL_L2SRAM_BIT);
        }
    else
        {
	/* All sram */
        l2CtlVal |= (0x1 << M85XX_L2CTL_L2SRAM_BIT);
        }

    /* Setup L2CTL for SRAM */
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));

    if (both == FALSE)
        {
        /* This is done here so L2SRAM is set before enable */
        l2CtlVal |= M85XX_L2CTL_L2E_MSK; /* No cache so go ahead and enable */
        /* Enable L2CTL for SRAM */
        vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
        }

    }
#endif /* INCLUDE_L2_SRAM */

/**************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* RETURNS: The address of the top of physical memory.
*
* ERRNO: N/A
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {

    if (physTop == NULL)
        {
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
        }

    return(physTop) ;
    }

/***************************************************************************
*
* sysMemTop - get the address of the top of VxWorks memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by VxWorks.
*
* The user can reserve memory space by defining the macro USER_RESERVED_MEM
* in config.h.  This routine returns the address of the reserved memory
* area.  The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*
* ERRNO: N/A
*/

char * sysMemTop (void)
    {

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

/**************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot()--which services ^X--and bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*
* ERRNO: N/A
*/

STATUS sysToMonitor
    (
    int startType   /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);   /* Warm reboot */
    int token;
#ifdef _WRS_VX_AMP
    int sysMonDelay;
#endif

    token = intCpuLock();

#ifdef	_WRS_CONFIG_SMP

    /*
     * In SMP mode, sysToMonitor() must run on core 0. If we already happen
     * to already be on core 0, then we can just put CPU 1 into a parking loop
     * and proceed with shutdown. If not, we have to use a cross-processor
     * call to re-invoke ourselves on core 0 first.
     */

    if (vxCpuIndexGet() == 0)
        {
        cpcInvoke (2, (CPC_FUNC)sysCpu1ShutdownLoop,
            NULL, 0, VX_CPC_ASYNC);
        sysMsDelay (100);
        }
    else
        {
        cpcInvoke (1, (CPC_FUNC)sysToMonitor, (void *)startType,
            sizeof(startType), VX_CPC_ASYNC);

        intCpuUnlock (token);

	/* Wait here for sysToMonitor to shut us down */
	for(;;);

        }


#endif	/* _WRS_CONFIG_SMP */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CACHE_SUPPORT
#ifdef	_WRS_CONFIG_SMP

#else	/* _WRS_CONFIG_SMP */

    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#endif

#endif

    vxMsrSet(0);
    /* Clear unnecessary TLBs */
    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

#ifdef  _WRS_CONFIG_SMP
    /*
     * We need to forcibly reset the second core. This is done
     * using the processor reset register in the PIC.
     *
     * Note also that the bits in the EPIC Hard Reset Register are
     * not auto-clearing.
     */
    *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
    sysCpu1Stop ();
    *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
#endif  /* _WRS_CONFIG_SMP */

#ifdef	_WRS_VX_AMP
    /* Clear boot page */
    *M85XX_BPTR(CCSBAR) = 0x0;

    *((volatile int *)CPU1_FUNC_START_ADR) = (int)0; /* SMP specific */
	/* SMP specific not used just loaded */
    *((volatile int *)CPU1_STACK_START_ADR) = (int)RAM_LOW_ADRS - FRAMEBASESZ;
	/* This is used if core waiting after core1 did asynchronous reboot */
    *((volatile int *)CPU1_INIT_START_ADR) = (int)MP_MAGIC_RBV;

    WRS_ASM("sync");

#ifdef INCLUDE_AMP_CPU_00

    sysCpu1Stop();

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */
    for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);

    *M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
    WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */
    for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);

    *M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;
    WRS_ASM("sync;isync");

    /*
     * Do not want to call out from this routine - use hard loop to
     * delay instead
     */
    for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);

    sysCpu1Start();

    sysAmpCpuStatusSet(ampCpuStateReboot,0);
#else

    sysAmpCpuStatusSet(ampCpuStateReboot,1);

#endif

#endif /* _WRS_VX_AMP */

    (*pRom) (startType);	/* jump off to romInit.s */

    return(OK);    /* in case we ever continue from ROM monitor */
    }

volatile unsigned int sysCpu1LoopCount = 0;

#if defined(_WRS_CONFIG_SMP)

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT void excIvorInit();
IMPORT void mmuPpcPidSet(UINT32);
IMPORT void sysCacheEnable();

/******************************************************************************
*
* sysCpuInit - initialize additional core
*
* RETURNS: NONE
*
*/

void sysCpuInit (void)
    {

    vxIvprSet((UINT32)VEC_BASE_ADRS);
    excIvorInit();

#if defined(PPC_e500v2)
    vxHid0Set(_PPC_HID0_MAS7EN|vxHid0Get());
#endif

    /* Enable machine check pin */
    vxHid0Set(HID0_MCP|vxHid0Get());

    vxHid1Set (vxHid1Get() | HID1_ASTME | HID1_ABE | HID1_RXFE);

    sysL2CacheInit();

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    sysCpu1LoopCount=2;

    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    sysCpu1LoopCount=3;

    if(_pSysL2CacheInvFunc!=NULL)
	_pSysL2CacheInvFunc();

    sysCacheEnable();

    if(_pSysL2CacheEnable!=NULL)
	_pSysL2CacheEnable();

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

    mmuPpcPidSet(MMU_ASID_GLOBAL);

#if (SW_MMU_ENABLE == FALSE)
    vxMsrSet(vxMsrGet() | _PPC_MSR_IS | _PPC_MSR_DS);
#endif
    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

#if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))
    /*
     * Set the current timebase read from CPU0 onto CPU1.
     * The timebase is kept halt till it's re-started by the CPU0.
     */

    sysSmpTimeBaseSet (tbHI, tbLO);
#endif	/* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    sysCpu1LoopCount=4;

    }


/******************************************************************************
*
* sysCpu1Loop - CPU 1 loop
*
* This routine is the CPU1 idle loop function which will invoke the startup
* function specified in the sysCpuEnable function.
*
* RETURNS: NONE
*
*/

void sysCpu1Loop (void)
    {
    unsigned int a = 0;
    FUNCPTR p;
    /*
     * The call to vxSdaInit() must be the first operation here.
     * This is because vxSdaInit() sets the r2 and r13 registers to the
     * SDA and SDA2 base registers. No C code must be placed before this
     * call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in sysCpu1Loop() */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

    p = (FUNCPTR)(*(UINT32*)CPU1_FUNC_START_ADR);

    sysCpuInit();

    sysCpu1LoopCount=0x10;

    if (p != NULL)
	{
	p ();
	}

    for (;;)
	{
	if (!(++a % 0x10000))
	    {
	    sysCpu1LoopCount++;
	    }
	}
    }


/******************************************************************************
*
* sysCpuEnable - enable a multi core CPU
*
* This routine brings a CPU out of reset
*
* RETURNS: OK or ERROR
*
*/

STATUS sysCpuEnable
    (
    unsigned int      cpuNum,
    WIND_CPU_STATE  * pCpuState
    )
    {
    STATUS status = OK;
    if (cpuNum != 1)
	{
	return ERROR;
	}

    *((int *)CPU1_FUNC_START_ADR) = (int)pCpuState->regs.pc;
    *((int *)CPU1_STACK_START_ADR) = (int)pCpuState->regs.spReg;
    *((int *)CPU1_INIT_START_ADR) = (int)sysCpu1Loop;

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    {
#ifndef	VX_ENABLE_CPU_TIMEOUT
#define VX_ENABLE_CPU_TIMEOUT	5	/* default timeout five seconds */
#endif	/* VX_ENABLE_CPU_TIMEOUT */

    cpuset_t	cpuset;
    UINT32	i = 0;

    /*
     * PPC Dec timer will not be available while synchronizing timebase
     * between two cores.
     * Use magic number, 0x9000000, to count 1 second independently
     * on PPC Dec timer.
     */

    UINT32	timeout = 0x9000000 * VX_ENABLE_CPU_TIMEOUT;

    /*
     * lock interrupts while Dec Timer is halt to prevent from lock-up
     * in the ISR which performs a timer delay.
     */

    int key = intCpuLock ();

    /*
     * For future reference:  DEVDISR is internal to the MPC8641D chip.
     * Platforms using multiple MPC8641D's (i.e. more than two CPUs) cannot
     * achieve perfect synchronization; the best that can be done is to
     * deassert the multiple DEVDISR[TB] signals as near simultaneously as
     * possible by using CPC.  On such a platform, the |= operation below
     * should be replicated, specifying the appropriate base address to
     * access each MPC8641D's CCSR space; and the &= operation below
     * should be replaced with a call to an assembly-language function
     * which writes the multiple DEVDISR registers using consecutive
     * machine instructions.
     */

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the synchronization between two cores gets
     * completed.
     */
    *(M85XX_DEVDISR(CCSBAR)) |= M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1;

     /*
      * Save the current timebase on CPU0 into static valuable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * synchronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif

    /* Flush Data cache so as to make sure other cores have updated cache */
    {
    volatile char* temp;

    temp = malloc(0x100000);

    memset((char*)temp,0x0,0x100000);

    sysCpu1Start ();

    memset((char*)temp,0x0,0x100000);

    free((char*)temp);
    }

    /* TBEN should be set by now */

#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
    /*
     * usrCpuEnable() or kernelCpuEnable() relies on the timer to count timeout.
     * If the system clock is PPC Dec Timer, it can not be used while timebase
     * is disabled.
     * Use a magic number for busy loop to count timeout here.
     */

    CPUSET_ZERO (cpuset);
    CPUSET_SET (cpuset, cpuNum);
    /* wait till the CPU1 ready */

    while (!(vxCpuEnabledGet () & cpuset))
	{
	if (++i > timeout)
	    {
	    status = ERROR;
	    break;
	    }
	}

    /*
     * Deassert the common time base disable signal.
     * The timebase registers should contain equivalent value between two cores
     * and they will work in sync.
     */

    *(M85XX_DEVDISR(CCSBAR)) &= ~(M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1);

    intCpuUnlock(key);

    }

#endif /* DRV_TIMER_M85XX && INCLUDE_SYS_TIMESTAMP */

    return status;
    }


#if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
/******************************************************************************
*
* sysCpcTimeBaseSet - CPC receiver for sysSmpTimeBaseInit
*
* This function receives the CPC from sysSmpTimeBaseInit,
* and passes the received parameters to sysSmpTimeBaseSet.
*
*/

LOCAL STATUS sysCpcTimeBaseSet
    (
    void	*ptr,	/* ptr to CPC parameters */
    UINT	size	/* size of CPC parameters */
    )
    {
    UINT32	tbHI = *(UINT32 *)ptr;
    UINT32	tbLO = ((UINT32 *)ptr)[1];

    sysSmpTimeBaseSet(tbHI, tbLO);
    return OK;
    }


/******************************************************************************
*
* sysSmpTimeBaseInit - Initialize synchronized timebases
*
* This function sets both CPUs' timebase registers to a common value
* while holding their common disable signal asserted, then enables them
* simultaneously so that their values are synchronized.  It should be called
* late in initialization, on one processor, after all processors are running.
*
* This function is required on all PPC SMP platforms using the timebase for
* timestamping, but may be a no-op on those platforms where synchronization
* is established by other means.
*
* INTERNAL
* For future reference:  DEVDISR is internal to the MPC8641D chip.  Platforms
* using multiple MPC8641D's (i.e. more than two CPUs) cannot achieve perfect
* synchronization; the best that can be done is to deassert the multiple
* DEVDISR[TB] signals as near simultaneously as possible.  On such a platform,
* the |= operation below should be replicated, specifying the appropriate
* base address to access each MPC8641D's CCSR space; and the &= operation
* below should be replaced with a call to an assembly-language function
* which writes the multiple DEVDISR registers using consecutive machine
* instructions.
*
* RETURNS: OK or ERROR
*
*/

LOCAL STATUS sysSmpTimeBaseInit
    (
    UINT32	tbHI,
    UINT32	tbLO
    )
    {
    cpuset_t	cpcInvokeCpuSet;
    UINT32	cpcParam[2];
    int		key;

    CPUSET_ZERO (cpcInvokeCpuSet);	/* => all but self */
    cpcParam[0] = tbHI;
    cpcParam[1] = tbLO;

    /* lock interrupts for a critical section */

    key = intCpuLock ();

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the synchronization between two cores gets
     * completed.
     */
    *(M85XX_DEVDISR(CCSBAR)) |= M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1;

    /*
     * Arrange for each processor to set its TB to the requested value,
     * and set the TBEN bit in its HID0.  Counting will not actually
     * start until the common disable is removed.
     */
    cpcInvoke (cpcInvokeCpuSet, (CPC_FUNC)sysCpcTimeBaseSet,
		(void *)cpcParam, sizeof(cpcParam), VX_CPC_SYNC);

    /* set own TB, since CPC broadcast does not include self */
    sysSmpTimeBaseSet(tbHI, tbLO);

    /*
     * Assert the common time base disable signal.
     * Keep stopping timebase till the synchronization between two cores gets
     * completed.
     */
    *(M85XX_DEVDISR(CCSBAR)) &= ~(M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1);

    intCpuUnlock (key);

    return OK;
    }

/******************************************************************************
*
* sysSmpTimeBaseSync - Synchronize timebases
*
* This function sets both CPUs' timebase registers to a common value
* while holding their common disable signal asserted, then enables them
* simultaneously so that their values are synchronized.  It should be called
* late in initialization, on one processor, after all processors are running.
*
* This function is required on all PPC SMP platforms using the timebase for
* timestamping, but may be a no-op on those platforms where synchronization
* is established by other means.
*
* RETURNS: OK or ERROR
*
*/

STATUS sysSmpTimeBaseSync (void)
    {
    UINT32 tbHi, tbLo;

    /* Synchronize timebases on all enabled CPUs. */

    vxTimeBaseGet (&tbHi, &tbLo);
    return (sysSmpTimeBaseInit (tbHi, tbLo));
    }
#endif	/* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

/***************************************************************************
*
* sysCpuAvailableGet - return the number of CPUs available for SMP
*
* RETURNS: 1 if the board is configured for AMP operation, else
*          the actual number of CPU cores as reported by the EPIC.
*
*/

UINT32 sysCpuAvailableGet (void)
    {

    return physCpuAvailableGet();
    }

/***************************************************************************
*
* physCpuAvailableGet - return the number of CPUs on the board
*
* RETURNS: the number of CPU cores, as reported by the EPIC.
*
*/

LOCAL UINT32 physCpuAvailableGet (void)
    {
    UINT32 avail = 1;

    vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuAvail), (void *)&avail);

    return (avail);
    }
#endif /* if defined(_WRS_CONFIG_SMP) */

/******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit2 (void)
    {

#ifdef  INCLUDE_VXBUS
    vxbDevInit();
#endif /* INCLUDE_VXBUS */

    /*
     * This was previously reqd for errata workaround #29, the workaround
     * has been replaced with patch for spr99776, so it now serves as an
     * example of implementing an l1 instruction parity handler
     */
#ifdef INCLUDE_L1_IPARITY_HDLR_INBSP
    memcpy((void*)_EXC_OFF_L1_PARITY, (void *)jumpIParity, sizeof(INSTR));
    cacheTextUpdate((void *)_EXC_OFF_L1_PARITY, sizeof(INSTR));
    sysIvor1Set(_EXC_OFF_L1_PARITY);
    cacheDisable(INSTRUCTION_CACHE);
    vxL1CSR1Set(vxL1CSR1Get() | _PPC_L1CSR_CPE);
    cacheEnable(INSTRUCTION_CACHE);
#endif  /* INCLUDE_L1_IPARITY_HDLR_INBSP */

    /* initialize serial interrupts */
#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
#endif

#if     defined (INCLUDE_SPE)
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */


#ifdef  INCLUDE_VXBUS
    taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */

    }

/******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board.
*
* ERRNO: N/A
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return(CPU1CHECK);
    }

/******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* Not applicable for the bus-less 8260Ads.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int     procNum         /* processor number */
    )
    {
    sysProcNum = procNum;
    }

/******************************************************************************
*
* sysLocalToBusAdrs - convert a local address to a bus address
*
* This routine gets the VMEbus address that accesses a specified local
* memory address.
*
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* ERRNO: N/A
*
* SEE ALSO: sysBusToLocalAdrs()
*/

STATUS sysLocalToBusAdrs
    (
    int     adrsSpace,  /* bus address space where busAdrs resides */
    char *  localAdrs,  /* local address to convert */
    char ** pBusAdrs    /* where to return bus address */
    )
    {

    *pBusAdrs = localAdrs;

    return(OK);
    }

/******************************************************************************
*
* sysBusToLocalAdrs - convert a bus address to a local address
*
* This routine gets the local address that accesses a specified VMEbus
* physical memory address.
*
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* ERRNO: N/A
*
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
    (
    int     adrsSpace,  /* bus address space where busAdrs resides */
    char *  busAdrs,    /* bus address to convert */
    char ** pLocalAdrs  /* where to return local address */
    )
    {

    *pLocalAdrs = busAdrs;

    return(OK);
    }




/******************************************************************************
*
* sysUsDelay - delay at least the specified amount of time (in microseconds)
*
* This routine will delay for at least the specified amount of time using the
* lower 32 bit "word" of the Time Base register as the timer.
*
* NOTE:  This routine will not relinquish the CPU; it is meant to perform a
* busy loop delay.  The minimum delay that this routine will provide is
* approximately 10 microseconds.  The maximum delay is approximately the
* size of UINT32; however, there is no roll-over compensation for the total
* delay time, so it is necessary to back off two times the system tick rate
* from the maximum.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysUsDelay
    (
    int    delay        /* length of time in microsec to delay */
    )
    {
    register UINT baselineTickCount;
    register UINT curTickCount;
    register UINT terminalTickCount;
    register int actualRollover = 0;
    register int calcRollover = 0;
    UINT ticksToWait;
    UINT requestedDelay;
    UINT oneUsDelay;

    /* Exit if no delay count */

    if ((requestedDelay = delay) == 0)
        return;

    /*
     * Get the Time Base Lower register tick count, this will be used
     * as the baseline.
     */

    baselineTickCount = sysTimeBaseLGet();

    /*
     * Calculate number of ticks equal to 1 microsecond
     *
     * The Time Base register and the Decrementer count at the same rate:
     * once per 8 System Bus cycles.
     *
     * e.g., 199999999 cycles     1 tick      1 second            25 ticks
     *       ----------------  *  ------   *  --------         ~  --------
     *       second               8 cycles    1000000 microsec    microsec
     */

    /* add to round up before div to provide "at least" */
    oneUsDelay = ((sysTimerClkFreq + 1000000) / 1000000);

    /* Convert delay time into ticks */

    ticksToWait = requestedDelay * oneUsDelay;

    /* Compute when to stop */

    terminalTickCount = baselineTickCount + ticksToWait;

    /* Check for expected rollover */

    if (terminalTickCount < baselineTickCount)
        {
        calcRollover = 1;
        }

    do
        {

        /*
         * Get current Time Base Lower register count.
         * The Time Base counts UP from 0 to
         * all F's.
         */

        curTickCount = sysTimeBaseLGet();

        /* Check for actual rollover */

        if (curTickCount < baselineTickCount)
            {
            actualRollover = 1;
            }

        if (((curTickCount >= terminalTickCount)
             && (actualRollover == calcRollover)) ||
            ((curTickCount < terminalTickCount)
             && (actualRollover > calcRollover)))
            {

            /* Delay time met */

            break;
            }
        }
    while (TRUE); /* breaks above when delay time is met */
    }

/*********************************************************************
*
* sysMsDelay - 1ms delay increments
*
* This routine consumes 1ms of time * delay.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    UINT      delay                   /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (int) delay * 1000 );
    }


/*********************************************************************
*
* sysDelay - Fixed 1ms delay
*
* This routine consumes 1ms of time. It just calls sysMsDelay.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }


#ifdef INCLUDE_CACHE_SUPPORT
/***********************************************************************
*
* sysL1CacheQuery - configure L1 cache size and alignment
*
* Populates L1 cache size and alignment from configuration registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL1CacheQuery (void)
    {
    UINT32 temp;
    UINT32 align;
    UINT32 cachesize;

    temp = vxL1CFG0Get();

    cachesize = (temp & 0xFF) << 10;

    align = (temp >> 23) & 0x3;


    switch (align)
        {
        case 0:
            ppcE500CACHE_ALIGN_SIZE=32;
            break;
        case 1:
            ppcE500CACHE_ALIGN_SIZE=64;
            break;
        default:
            ppcE500CACHE_ALIGN_SIZE=32;
            break;
        }

    ppcE500DCACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);
    ppcE500ICACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);

    /*
     * The core manual suggests for a 32 byte cache line and 8 lines per set
     * we actually need 12 unique address loads to flush the set.
     * The number of lines to flush should be ( 3/2 * cache lines )
     */
    ppcE500DCACHE_LINE_NUM = (3*ppcE500DCACHE_LINE_NUM)>>1;
    ppcE500ICACHE_LINE_NUM = (3*ppcE500ICACHE_LINE_NUM)>>1;

    }

#endif /* INCLUDE_CACHE_SUPPORT */

/***************************************************************************
*
* saveExcMsg - write exception message to save area for catastrophic error
*
* The message will be displayed upon rebooting with a bootrom.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void saveExcMsg
    (
    char *errorMsg
    )
    {
    strncpy ((char *)EXC_MSG_OFFSET, errorMsg, 0x100);
    }


void chipErrataCpu29Print (void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    }

/***************************************************************************
*
* sysGetPeripheralBase - Provides CCSRBAR address for security engine drivers
*
* RETURNS: CCSRBAR value
*
* ERRNO: N/A
*/
UINT32 sysGetPeripheralBase (void)
    {
    return(CCSBAR);
    }

/*******************************************************************************
*
* sysIntEnable - enable an interrupt
*
* This function call is used to enable an ISA/PCI interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*/

STATUS sysIntEnable
    (
    int intNum
    )
    {
    return (intEnable(intNum));
    }
/*******************************************************************************
*
* sysIntDisable - disable an interrupt
*
* This function call is used to disable an interrupt.
*
* RETURNS: OK or ERROR if unable to disable interrupt.
*/

STATUS sysIntDisable
    (
    int intNum
    )
    {
    return (intDisable (intNum));
    }



/*******************************************************************************
*
* sysIntConnect - connect the BSP interrupt to the proper epic/i8259 handler
*
* This routine checks the INT level and connects the proper routine.
* pciIntConnect() or intConnect().
*
* RETURNS:
* OK, or ERROR if the interrupt handler cannot be built.
*
*/

STATUS sysIntConnect
    (
    VOIDFUNCPTR *vector,        /* interrupt vector to attach to     */
    VOIDFUNCPTR routine,        /* routine to be called              */
    int parameter               /* parameter to be passed to routine */
    )
    {
    int tmpStat = ERROR;
    UINT32 read;

    if (vxMemProbe ((char *) routine, VX_READ, 4, (char *) &read) != OK)
	{
	logMsg ("Error in sysIntConnect: Cannot access routine.\n",
		1,2,3,4,5,6);
	return (ERROR);
	}

    switch ((int)vector)
	{
        /*
         * add INT_VEC_IRQ0 before calling pciIntConnect since it subtracts
         * it and the EPIC which deals with PCI ints needs to be passed
         * original vector
         */
	default:
	    {
	    tmpStat = intConnect (vector, routine, parameter);
	    break;
	    }
	} /* End switch */

    if (tmpStat == ERROR)
	{
	logMsg ("Error in sysIntConnect: vector = %d.\n",
		(int)vector,2,3,4,5,6);
	}

    return (tmpStat);
    }

#if defined (_GNU_TOOL)
void sysIntHandler (void)
    {
    }

void vxDecInt (void)
    {
    }

int excRtnTbl = 0;
#endif  /* _GNU_TOOL */


#ifdef INCLUDE_SHOW_ROUTINES

/***************************************************************************
*
* coreLbcShow - Show routine for local bus controller
*
* This routine shows the local bus controller registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreLbcShow (void)
    {
    VINT32 tmp, tmp2;

    tmp = * (VINT32 *) M85XX_BR0(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR0(CCSBAR);
    printf("Local bus BR0 = 0x%x\tOR0 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR1(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR1(CCSBAR);
    printf("Local bus BR1 = 0x%x\tOR1 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR2(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR2(CCSBAR);
    printf("Local bus BR2 = 0x%x\tOR2 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR3(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR3(CCSBAR);
    printf("Local bus BR3 = 0x%x\tOR3 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR4(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR4(CCSBAR);
    printf("Local bus BR4 = 0x%x\tOR4 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR5(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR5(CCSBAR);
    printf("Local bus BR5 = 0x%x\tOR5 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR6(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR6(CCSBAR);
    printf("Local bus BR6 = 0x%x\tOR6 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR7(CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR7(CCSBAR);
    printf("Local bus BR7 = 0x%x\tOR7 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_LBCR(CCSBAR);
    printf("Local bus LBCR = 0x%x\n", tmp);

    tmp = * (VINT32 *) M85XX_LCRR(CCSBAR);
    printf("Local bus LCRR = 0x%x\n", tmp);
    }


    #define xbit0(x, n)    ((x & (1 << (31 - n))) >> (31 - n))  /* 0..31 */
    #define xbit32(x, n)   ((x & (1 << (63 - n))) >> (63 - n))  /* 32..63 */
    #define onoff0(x, n)   xbit0(x, n) ? "ON", "OFF"
    #define onoff32(x, n)  xbit32(x, n) ? "ON", "OFF"

/***************************************************************************
*
* coreShow - Show routine for core registers
*
* This routine shows the core registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreShow (void)
    {
    VUINT32 tmp, tmp2;

    tmp = vxMsrGet();
    printf("MSR - 0x%x\n", tmp);
    printf("      UCLE-%x SPE-%x WE-%x CE-%x EE-%x PR-%x ME-%x\n",
           xbit32(tmp,37), xbit32(tmp,38), xbit32(tmp,45), xbit32(tmp,46),
           xbit32(tmp,48), xbit32(tmp,49), xbit32(tmp,51));
    printf("      UBLE-%x DE-%x IS-%x DS-%x PMM-%x\n",
           xbit32(tmp,53), xbit32(tmp,54), xbit32(tmp,58), xbit32(tmp,59),
           xbit32(tmp,61));
    tmp = vxHid0Get();
    tmp2 = vxHid1Get();
    printf("HID0 = 0x%x, HID1 = 0x%x\n", tmp, tmp2);
    tmp = vxL1CSR0Get();
    printf("L1CSR0: cache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = vxL1CSR1Get();
    printf("L1CSR1: Icache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = vxL1CFG0Get();
    tmp2 = vxL1CFG1Get();
    printf("L1CFG0 = 0x%x, L1CFG1 = 0x%x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x20000);
    printf("L2CTL - 0x%x\n", tmp);
    printf("        l2 is %s\n", tmp&0x80000000?"ON":"OFF");
    printf("        l2siz-%x l2blksz-%x l2do-%x l2io-%x\n",
           (xbit0(tmp,2)<<1)|xbit0(tmp,3), (xbit0(tmp,4)<<1)|xbit0(tmp,5),
           xbit0(tmp,9), xbit0(tmp,10));
    printf("        l2pmextdis-%x l2intdis-%x l2sram-%x\n",
           xbit0(tmp,11), xbit0(tmp,12),
           (xbit0(tmp,13)<<2)|(xbit0(tmp,14)<<1)|xbit0(tmp,15));
    tmp = *(VUINT32 *) (CCSBAR + 0x20100);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x20108);
    printf("L2SRBAR0 - 0x%x\n", tmp);
    printf("L2SRBAR1 - 0x%x\n", tmp2);

    printf("Core CPU0 Freq = %3d Hz\n",coreFreq);
    printf("Core CPU1 Freq = %3d Hz\n",core1Freq);
    printf("DDR Freq = %3d Hz\n",ddrFreq);
    printf("CCB Freq = %3d Hz\n",sysClkFreqGet());
    printf("System Freq = %3d Hz\n",systemFreq);

    }

#endif


#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPci1IntRoute [NUM_PCIEX1_SLOTS][4] = {{PCIEX_XINT1_LVL, PCIEX_XINT2_LVL, PCIEX_XINT3_LVL, PCIEX_XINT4_LVL}};

LOCAL UCHAR sysPci2IntRoute [NUM_PCIEX2_SLOTS][4] = {{PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL}};

/* This is only for PCI slots on ULi - info taken for MPC8572DS Workbook */
LOCAL UCHAR sysPci3IntRoute [2][4] = {{2,     3,     4,     1},
				      {3,     4,     1,     2}};

/* info taken for MPC8572DS Workbook - PCI express 3 INTA:D */
LOCAL UCHAR sysPci3IntBridge [4] = { 8, 9, 10, 11 };

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {

    /*
     * Host controller itself (device number is 0) won't be configured;
     */

    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);


    return (OK); /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci2AutoconfigInclude - PCI 2 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci2AutoconfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {

    /*
     * Host controller itself (device number is 0) won't be configured;
     */

    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPci3AutoconfigInclude - PCI 2 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci3AutoconfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {

    /*
     * Unsupported
     */
    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return(ERROR);

    return OK; /* Autoconfigure all devices */

    }



/*******************************************************************************
*
* sysPci1AutoconfigIntrAssign - PCI 1 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci1AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin >= 1) && (pin < 5))				&&
	((pLoc->device) < NUM_PCIEX1_SLOTS))
	{
	tmpChar =
	    sysPci1IntRoute [(pLoc->device)][pin-1];
	}

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci2AutoconfigIntrAssign - PCI 2 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci2AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a reasonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin >=1) && (pin < 4))				&&
	((pLoc->device) < NUM_PCIEX2_SLOTS))
	{
	tmpChar =
	    sysPci2IntRoute [(pLoc->device)][pin-1];

	}

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

/*******************************************************************************
*
* sysPci3AutoconfigIntrAssign - PCI 3 autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPci3AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* Ignore host and bridges */
    if(((pLoc->bus==0)&&(pLoc->device==0))||(pin==0))
	return(tmpChar);

    /*
     * Verify this is slot 1 or slot 2 on ULi then configure interrupt
     */
    if((pLoc->bus==2)&&((pLoc->device==17) ||(pLoc->device==18)))
	{
	/* This should be slot 1 and slot 2 of ULi PCI */
	tmpChar =  sysPci3IntRoute [(pLoc->device - 17)][pin-1];
	return(tmpChar);
	}


    /* These are irqs allocated if going through the bridge on this board */
    return(sysPci3IntBridge[((pLoc->device) + (pin - 1))&3]);

    }

#endif

/***************************************************************************
*
* bspSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* The sysSerialChanGet() routine returns a pointer to the SIO_CHAN
* device associated with a specified serial channel. It is called
* by usrRoot() to obtain pointers when creating the system serial
* devices, `/tyCo/x'. It is also used by the WDB agent to locate its
* serial channel.  The VxBus function requires that the BSP provide a
* function named bspSerialChanGet() to provide the information about
* any non-VxBus serial channels, provided by the BSP.  As this BSP
* does not support non-VxBus serial channels, this routine always
* returns ERROR.
*
* RETURNS: ERROR, always
*
*/

SIO_CHAN * bspSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    return((SIO_CHAN *)ERROR);
    }

LOCAL BOOL sysCpu1Check (void)
    {
    return (CPU1CHECK);
    }

#ifdef _WRS_CONFIG_SMP
/*******************************************************************************
*
* sysCpu1ShutdownLoop - parking loop for CPU1
*
* This routine is launched onto core 1 during system reboot. It locks out
* interrupts and puts the core into a tight loop so that it doesn't interfere
* with the sysToMonitor() routine.
*
* RETURNS: N/A
*/

LOCAL void sysCpu1ShutdownLoop (void)
    {
    int token;

    token = intCpuLock();

    FOREVER
        {
#ifdef INCLUDE_CPU_LIGHT_PWR_MGR
	vxHid0Set(vxHid0Get() | _PPC_HID0_DOZE);
	WRS_ASM("sync");
	vxMsrSet(vxMsrGet()| _PPC_MSR_WE);
	WRS_ASM("isync");
#endif  /* INCLUDE_CPU_LIGHT_PWR_MGR */

	/* expect to lock in DOZE mode until HRESET */
	sysDelay();
        }

    }
#endif /* _WRS_CONFIG_SMP */


#ifdef INCLUDE_WRLOAD

/*****************************************************************************
*
* sysAmpCpuEnable - Starts core1 executing code at entryPt
*
* This routine performs the hardware specific code to start the secondary
* core. It does so using two methods - using boot page translation register
* when first called. After this it uses CPU1_INIT_START_ADR to inform the
* second core where to jump to. The second core if reboot will wait until
* CPU1_INIT_START_ADR is set to something other than MP_MAGIC_RBV which is
* what this routines is doing. The success or failure of this routine cannot
* be determined here.
* This function is required for wrload.
*
* RETURNS: NONE
*
*/

void sysAmpCpuEnable
    (
    FUNCPTR entryPt,
    UINT32 cpu
    )
    {
    /*
     * Here if core already running then maybe be spinning
     * bootimage should then spin waiting for CPU1_INIT_START_ADR to be
     * non-zero
     */
    *((volatile int *)CPU1_FUNC_START_ADR) =
	  (int)0; /* SMP specific */
	/* SMP specific not used just loaded */
    *((volatile int *)CPU1_STACK_START_ADR) =
	  ((int)entryPt - 0x20);
	/* This is used if core waiting after core1 did asynchronous reboot */
    *((volatile int *)CPU1_INIT_START_ADR) =
	  (int)entryPt;

    /* If core1 is not already running then enable it */
    if((*EEB_PCR(CCSBAR)&EEB_PCR_CORE1_ENABLE)==0)
	{
	*M85XX_BPTR(CCSBAR) = ((UINT32)entryPt >> BPTR_BOOT_PAGE_SHIFT ) |
	  BPTR_EN;
	sysCpu1Start();
	}

    }

/*****************************************************************************
*
* sysAmpCpuPrep - Prep cpuId for wrload download and start of AMP image
*
* This function is used by wrload to make a core ready for wrload to
* download and/or start a new image.  The state of the target cpu
* after this call is not specified, and is OS/arch/CPU specific.  If
* return is OK, then wrload should expect to succeed; if ERROR, then
* wrload will abort.
*
* arg argument currently unused, expected to provide finer grain control
* in the future.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysAmpCpuPrep
    (
    UINT32  cpuId,
    void * arg
    )
    {
    if (cpuId == 1)
	{
	/* shutdown ints */
	vxbDevMethodRun (DEVMETHOD_CALL(vxbIntCtlrCpuDisable), (void *)cpuId);

	/* Clear boot page */
	*M85XX_BPTR(CCSBAR) = 0x0;

	*((volatile int *)CPU1_FUNC_START_ADR) = (int)0; /* SMP specific */
	/* SMP specific not used just loaded */
	*((volatile int *)CPU1_STACK_START_ADR) = (int)RAM_LOW_ADRS - FRAMEBASESZ;
	/* This is used if core waiting after core1 did asynchronous reboot */
	*((volatile int *)CPU1_INIT_START_ADR) = (int)MP_MAGIC_RBV;

	WRS_ASM("sync");

	/* hold core in reset while clearing PCR! */
	*M85XX_EPICPIR(CCSBAR) |= M85XX_EPICPIR_HRST_CORE1;
	sysCpu1Stop();
	*M85XX_EPICPIR(CCSBAR) &= ~M85XX_EPICPIR_HRST_CORE1;

	return OK;
	}

    /* Cannot prep cpu 0 */
    return ERROR;
    }


#endif /* INCLUDE_WRLOAD */

#ifdef _WRS_VX_AMP
/*****************************************************************************
*
* sysAmpCpuStatusSet - Sets the status value of a specific CPU
*
* RETURNS: NONE
*
*/

void sysAmpCpuStatusSet
    (
    int status,
    int cpu
    )
    {
    /* only allow CPU1 to set it's own status */
    if( CPU1CHECK )
	cpu = 1;

    switch(cpu)
	{
	case 1:
	    *((VINT32*)AMP_CPU1_STATUS) = status;
	    break;
	case 0:
	    *((VINT32*)AMP_CPU0_STATUS) = status;
	    break;
	default:
	    break;
	}

    }

/*****************************************************************************
*
* sysAmpCpuStatusGet - Gets the status value of a specific CPU
*
* RETURNS: State of the specified CPU or Error if not a valid CPU.
*
*/

int sysAmpCpuStatusGet
    (
    int cpu
    )
    {
    int status = ampCpuStateError;

    switch(cpu)
	{
	case 1:
	    status = *((VINT32*)AMP_CPU1_STATUS);
	    break;
	case 0:
	    status = *((VINT32*)AMP_CPU0_STATUS);
	    break;
	default:
	    break;
	}
    return(status);
    }

#endif /* _WRS_VX_AMP */

#ifdef AMP_LINUX
/*
 * Enable switching serial interrupt between cores - hack to PIC not fit for
 * general use. Linux specific.
 */
void switchSerialVx2Lnx
    (
    BOOL direction
    )
    {
    if(direction == TRUE)
	{
	*(VUINT32*)(EPIC_IN_INT26_VEC_REG | CCSBAR) = 0x0088002a;
	*(VUINT32*)(EPIC_IN_INT26_DES_REG | CCSBAR) = 0x00000001;
	}
    else
	{
	*(VUINT32*)(EPIC_IN_INT26_VEC_REG | CCSBAR) = 0x00830026;
	*(VUINT32*)(EPIC_IN_INT26_DES_REG | CCSBAR) = 0x00000002;
	}

    }

#endif


