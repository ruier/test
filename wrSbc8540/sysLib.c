/* sysLib.c - Wind River SBC 85xx board system-dependent library */

/*
 * Copyright 2002-2008, 2010, 2011 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03d,19dec11,agf  Moved L1 parity handling to arch lib. (WIND00309426)
03c,24jun10,pmr  clean up exception extended vector support
03b,31oct08,h_k  fixed a compile error at sysHwInit() when
                 INCLUDE_BRANCH_PREDICTION and INCLUDE_CPM are defined.
03a,30oct08,x_z  add vxEnableBP to sysHwInit and replace disableBranchPrediction
                 with vxDisableBP.(WIND00127193)
02z,18jun08,h_k  removed pAccess.
02y,23aug07,wap  Remove unneeded declaration for fccEnetEnable (WIND00101790)
02x,21aug07,mmi  remove vxPowerModeSet() since deprecated
02w,08aug07,agf  remove duplicate variable definitions
02v,27jul07,vik  Fix for WIND00099581, removed cmdLine.c
02u,20jun07,dcc  fixed _wrs_kernel_data_start definition.
02t,16mar07,wap  Fix merge errors
02s,05mar07,dtr  Remove old PCI lib includes.
02r,06feb07,d_z  Exclude mpc8540 device ID from sysPciAutoconfigInclude.
02q,06feb07,wap  defect 83778: serial initialization
02p,23jan07,wap  Correct project build errors
02o,29aug06,wap  Add VxBus FCC driver support
02n,19jan07,pmr  defect 83778: serial initialization
02m,24oct06,jmt  Fix Aux Clock init to use default define
02l,13sep06,tfo  Remove WLAN support for Atheros AR521X, not supported
02k,30aug06,tfo  WLAN support for Atheros AR521X
02j,28Jul06,tor  Modified for VxBus
02i,06feb06,wap  Don't include sysMotTsecEnd.c in HEND case
02h,27jan06,wap  Fix command line builds with TSEC driver
02g,06jan06,wap  invoke vxbDevConnect() via taskSpawn()
02f,03jan06,wap  Allow command line build with vxBus support 
02e,13dec05,pch  SPR 113596: networking dependencies
02d,28oct05,mdo  SPR#114197 - protect against multiple defines for
                 INCLUDE_PCICFG_SHOW
02c,13sep05,to   add vxb prefix, vxbus cleanups.
02b,08aug05,mdo  Change WINDBUS to VXBUS
02a,10jan05,mil  Added support for INCLUDE_ECC.
01z,19nov04,mil  Added enabling of Icache when Immu is not enabled.
01y,11nov04,dtr  Adding user mode access for system memory.
01x,21oct04,dtr  Move ASTME enable outside L2 conditional.
01w,15oct04,mdo  SPR 98956, SPR #98944 - intLock in sysToMonitor
01v,04oct04,mdo  Documentation fixes for apigen
01u,27sep04,mil  Added cacheTextUpdate for _EXC_OFF_L1_PARITY after install.
01t,10sep04,mil  Changed macro L1_ERRATA_FIX_REV2 to INCLUDE_L1_IPARITY_HDLR.
01s,19aug04,dtr  Mod for ICacheParity handler.
01r,28jul04,mil  Added saveExcMsg for reboot error display.
01q,14jul04,mil  Fixed sysMsDelay accuracy, removed HID1[RFXE].
01p,08jul04,mil  Added setting of HID1[RFXE].
01o,18aug04,md   PM_RESERVED_MEM is dependent on INCLUDE_EDR_PM
01n,10jun04,mil  Added L1 errata workaround.
01m,09jun04,mil  Added writethrough cache mode for parity recovery.
01l,01jun04,mil  Fixed L2 not enabled by default.
01k,05may04,mil  Changed TLB mappings and various updates.
01j,02feb04,mil  Added enabling of HID1 bits.
01i,24oct03,mil  Cleanup.
01h,12sep03,mil  Updated TLB entries and added clock sensing, among others.
01g,20aug03,dtr  Adding in TSEC configlette file.
                 Adding new FCC2 End driver support.
                 Test for bootrom then initialise static TLB entries.
                 Adding dynamic config of L1 cache.
01f,04aug03,dtr  Adding in support for LBC SDRAM.
01e,29jul03,dtr  Removing CCSBAR magic number,support for MMU 'off' and
                 support for SNOOP when MMU 'off'.
01d,24jul03,mil  Added storing of flash params.
01c,19jun03,mil  Changed ROM TLB entry to writable, added SCC and FCC.
01b,07jan03,dtr  Adding TLB static table entries.
01a,13oct02,dtr  More complete file for further development.
*/

/*
DESCRIPTION
This library provides board-specific routines for WRSBC85XX.

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <vxWorks.h>
#include <vme.h>
#include <memLib.h>
#include <cacheLib.h>
#include <sysLib.h>
#include <config.h>
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

#ifdef	INCLUDE_SPE
#   include "speLib.h"
#endif	/* INCLUDE_SPE */

#ifdef INCLUDE_CPM
#   include "drv/parallel/m8260IOPort.h"
#   include "drv/sio/m8260Cp.h"
#   include "drv/sio/m8260Scc.h"
#   include "drv/sio/m8260CpmMux.h"
#   include "m8560CpmIntrCtl.h"
#endif  /* INCLUDE_CPM */

#ifdef INCLUDE_VXBUS
#    include "hwif/vxbus/vxBus.h"
#    include "hwconf.c"

#include <hwif/vxbus/vxbPlbLib.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>

#ifdef	INCLUDE_CPM
# ifdef	VXB_LEGACY_ACCESS
LOCAL void m85xxPlbAccessFunctionOverride (struct vxbAccessList *);
# endif	/* VXB_LEGACY_ACCESS */
#endif	/* INCLUDE_CPM */

LOCAL STATUS m85xxPlbDevControl (VXB_DEVICE_ID, pVXB_DEVCTL_HDR);

#ifdef INCLUDE_FCC_VXB_END
#include <../src/hwif/h/end/fccVxbEnd.h>
STATUS sysFccEnetEnable (VXB_DEVICE_ID, UINT8);
#endif

#ifdef INCLUDE_MDIO
#include <../src/hwif/h/mii/mdio.h>
STATUS sysMdioEnable (VXB_DEVICE_ID);
#endif

#ifdef DRV_SIO_NS16550
STATUS sysNs16550Enable (VXB_DEVICE_ID);
#endif

/* XXX these should be imported with METHOD_DECL */
IMPORT char * mdioEnable_desc;
METHOD_DECL(sioEnable)

IMPORT device_method_t * pSysPlbMethods;

LOCAL struct vxbDeviceMethod m85xxPlbMethods[] =
    {
#ifdef INCLUDE_FCC_VXB_END
    DEVMETHOD(fccEnetEnable, sysFccEnetEnable),
#endif
#ifdef INCLUDE_MDIO
    DEVMETHOD(mdioEnable, sysMdioEnable),
#endif
#ifdef DRV_SIO_NS16550
    DEVMETHOD(sioEnable, sysNs16550Enable),
#endif
    { 0, 0 }
    };

#endif /* INCLUDE_VXBUS */

#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciIntLib.h>

/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    {
    /* 16 MB (8 real) of boot flash, needed be FIRST entry here */
    BOOT_FLASH_MAP_ADRS, 0x0, BOOT_FLASH_MAP_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_16M | _MMU_TLB_IPROT | 
#ifdef INCLUDE_TFFS  /* writable when using TFFS */
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_ATTR_M
#else /* INCLUDE_TFFS */
    _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_ATTR_M
#endif /* INCLUDE_TFFS */
    }
    ,
    {
    /* 2x256 MB of DDR RAM, two entries because 256 max, needed be SECOND */
    DDR_SDRAM_ADRS1, 0x0, DDR_SDRAM_ADRS1,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_IPROT | 
#ifdef  SW_MMU_ENABLE
#if (SW_MMU_ENABLE == 1)
    _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX | 
#endif
#endif  /* SW_MMU_ENABLE */
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE
    }
    ,
    {
    /* 2x256 MB of DDR RAM, 2nd half */
    DDR_SDRAM_ADRS2, 0x0, DDR_SDRAM_ADRS2,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M | _MMU_TLB_IPROT |
#ifdef  SW_MMU_ENABLE
#if (SW_MMU_ENABLE == 1)
    _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX | 
#endif
#endif  /* SW_MMU_ENABLE */
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE
    }
    ,
    {
    /* 1 MB of CCSRBAR area, TS=0 */
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_ATTR_M
    }
    ,
    {
    /* 16(12) MB of WR SBC 85XX util area */ 
    UTIL_ADRS, 0x0, UTIL_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_16M | _MMU_TLB_IPROT |
    _MMU_TLB_PERM_W | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_ATTR_M
    }
#ifdef INCLUDE_LOCAL_SDRAM
    ,
    {
    /* 2x64 MB of LBC SDRAM, 1st half */
    LOCAL_SDRAM1_ADRS, 0x0, LOCAL_SDRAM1_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M |
#ifdef  SW_MMU_ENABLE
#if (SW_MMU_ENABLE == 1)
    _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX | 
#endif
#endif  /* SW_MMU_ENABLE */
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE
    }
    ,
    {
    /* 2x64 MB of LBC SDRAM, 2nd half */
    LOCAL_SDRAM2_ADRS, 0x0, LOCAL_SDRAM2_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M |
#ifdef  SW_MMU_ENABLE
#if (SW_MMU_ENABLE == 1)
    _MMU_TLB_PERM_UR | _MMU_TLB_PERM_UW | _MMU_TLB_PERM_UX | 
#endif
#endif  /* SW_MMU_ENABLE */
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE
    }
#endif  /* INCLUDE_LOCAL_SDRAM */
#ifdef INCLUDE_USR_FLASH
    ,
    {
    /* 2x64 MB of LBC flash, 1st half */
    USR_FLASH1_ADRS, 0x0, USR_FLASH1_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M |
#ifdef INCLUDE_TFFS  /* writable when using TFFS */
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_ATTR_M
#else /* INCLUDE_TFFS */
    _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_ATTR_M
#endif /* INCLUDE_TFFS */
    }
    ,
    {
    /* 2x64 MB of LBC flash, 2nd half */
    USR_FLASH2_ADRS, 0x0, USR_FLASH2_ADRS,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M |
#ifdef INCLUDE_TFFS  /* writable when using TFFS */
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | \
    _MMU_TLB_ATTR_M
#else /* INCLUDE_TFFS */
    _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_ATTR_M
#endif /* INCLUDE_TFFS */
    }
#endif  /* INCLUDE_USR_FLASH */
    ,
    {
    /* 256 KB of L2SRAM */
    L2SRAM_ADDR, 0x0, L2SRAM_ADDR,
    _MMU_TLB_TS_0 | _MMU_TLB_SZ_256K |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I
    }
    ,
    /* PCI */
    {
    PCI_MEM_ADRS, 0x0, PCI_MEM_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_16M |
    _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
    {
    PCI_MEMIO_ADRS, 0x0, PCI_MEMIO_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_16M |
    _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    },
    {
    PCI_IO_ADRS, 0x0, PCI_IO_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_16M |
    _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }

};  /* end of sysStaticTlbDesc[] */

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);


/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * The MINIMUM recommended Page Table sizes for 32-bit PowerPCs are:
 *
 * Total mapped memory		Page Table size
 * -------------------		---------------
 *        8 Meg			     64 K
 *       16 Meg			    128 K
 *       32 Meg			    256 K
 *       64 Meg			    512 K
 *      128 Meg			      1 Meg
 * 	.				.
 * 	.				.
 * 	.				.
 *
 * [Ref: Ch 7, PowerPC Microprocessor Family: The Programming Environments]
 *
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
{
    {
    /* Vector Table and Interrupt Stack */
    /* Must be sysPhysMemDesc [0] to allow adjustment in sysHwInit() */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE
    }
    ,
    {
    /* Must be sysPhysMemDesc [1] to allow adjustment in sysHwInit() */
    (VIRT_ADDR) RAM_LOW_ADRS,
    (PHYS_ADDR) RAM_LOW_ADRS,
    LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE -  RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE
    }
    ,
    {
    (VIRT_ADDR) BOOT_FLASH_MAP_ADRS,
    (PHYS_ADDR) BOOT_FLASH_MAP_ADRS,
    BOOT_FLASH_MAP_SIZE,
#ifdef INCLUDE_TFFS  /* writable when using TFFS */
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
#else /* INCLUDE_TFFS */
    VM_STATE_MASK_VALID | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
#endif /* INCLUDE_TFFS */
    }
    ,
    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) UTIL_ADRS,
    (PHYS_ADDR) UTIL_ADRS,
    16 * 0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }

#ifdef INCLUDE_LOCAL_SDRAM
    ,
    {
    (VIRT_ADDR) LOCAL_SDRAM1_ADRS,
    (PHYS_ADDR) LOCAL_SDRAM1_ADRS,
    (LOCAL_SDRAM2_ADRS - LOCAL_SDRAM1_ADRS) * 2,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | TLB_CACHE_MODE
    }
#endif  /* INCLUDE_LOCAL_SDRAM */

#ifdef INCLUDE_USR_FLASH
    ,
    {
    (VIRT_ADDR) USR_FLASH1_ADRS,
    (PHYS_ADDR) USR_FLASH1_ADRS,
    (USR_FLASH2_ADRS - USR_FLASH1_ADRS) * 2,
#ifdef INCLUDE_TFFS  /* writable when using TFFS */
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
#else /* INCLUDE_TFFS */
    VM_STATE_MASK_VALID | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
#endif /* INCLUDE_TFFS */
    }
#endif  /* INCLUDE_USR_FLASH */

    ,
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    256 * 1024,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    }
    ,
    /* PCI */
    {
    (VIRT_ADDR) PCI_MEM_ADRS,
    (PHYS_ADDR) PCI_MEM_ADRS,
    16 * 1024 * 1024,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCI_MEMIO_ADRS,
    (PHYS_ADDR) PCI_MEMIO_ADRS,
    16 * 1024 * 1024,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCI_IO_ADRS,
    (PHYS_ADDR) PCI_IO_ADRS,
    16 * 1024 * 1024,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | \
    VM_STATE_MASK_GUARDED | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | \
    VM_STATE_GUARDED      | VM_STATE_MEM_COHERENCY
    }

};  /* end of sysPhysMemDesc[] */

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);


int	sysBus = BUS;			/* system bus type (VME_BUS, etc) */
int	sysCpu = CPU;			/* system CPU type (PPC8260) */
char	*sysBootLine = BOOT_LINE_ADRS;	/* address of boot line */
#ifdef	USE_ALT_BOOTLINE
char	*altBootLine = ALT_BOOT_LINE;	/* address of alt boot line */
#endif
char	*sysExcMsg = EXC_MSG_ADRS;	/* catastrophic message area */
int	sysProcNum;			/* processor number of this CPU */
BOOL	sysVmeEnable = FALSE;		/* by default no VME */
UINT32	sysTimerClkFreq = PLAT_FREQ_DEFAULT >> 3;  /* clock div is 8 */
UINT32	inFullVxWorksImage = FALSE;
UINT32	ppcE500ICACHE_LINE_NUM = (128 * 12);
UINT32	ppcE500DCACHE_LINE_NUM = (128 * 12);
UINT32	ppcE500CACHE_ALIGN_SIZE = 32;

#ifdef  INCLUDE_SCC_SERIAL
UINT32	baudRateGenClk;
#endif  /* INCLUDE_SCC_SERIAL */

#ifdef  INCLUDE_FCC_VXB_END
/* Set the following array to a unique Ethernet hardware address.
 * The last 5 nibbles are board specific, initialized in sysHwInit
 */
unsigned char sysFccEnetAddr [2][6] =
    {
    {0x08, 0x00, 0x3e, 0x33, 0x02, 0x01},
    {0x08, 0x00, 0x3e, 0x33, 0x02, 0x02}
    };
#endif  /* INCLUDE_FCC_VXB_END */

/* forward declarations */

IMPORT  char    _wrs_kernel_data_start[];
IMPORT	BOOL	mmuPpcIEnabled;
IMPORT	BOOL	mmuPpcDEnabled;
IMPORT	void	mmuE500TlbDynamicInvalidate ();
IMPORT	void	mmuE500TlbStaticInvalidate ();
IMPORT	void	mmuE500TlbStaticInit (int numDescs, 
				      TLB_ENTRY_DESC *pTlbDesc, 
				      BOOL cacheAllow);
#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void     vxEnableBP();
IMPORT void     vxDisableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

IMPORT  void    sysL1Csr1Set(UINT32);
IMPORT  UINT    sysTimeBaseLGet(void);

UINT32		sysClkFreqGet (void);
LOCAL	void	sysL1CacheQuery (void);
LOCAL	void	sysL2SramEnable (BOOL both);
void    chipErrataCpu29Print(void);
void    ddrDeviceIntHandler(void);

#ifdef  INCLUDE_VXBUS

IMPORT  void    hardWareInterFaceInit (void);

#ifdef INCLUDE_SIO_UTILS
IMPORT void    sysSerialConnectAll(void);
#endif /* INCLUDE_SIO_UTILS */

#endif  /* INCLUDE_VXBUS */

#ifdef	INCLUDE_SPE
IMPORT	int	(* _func_speProbeRtn) () ;
#endif	/* INCLUDE_SPE */

#ifdef INCLUDE_CPM
UINT32	vxImmrGet ();
void	m85xxCpmInt (void);
#endif  /* INCLUDE_CPM */

/* include BSP specific files */


#include "m85xxTimer.c"

#if defined(INCLUDE_NV_RAM)
#  include "./sysNvRam.c"
#elif defined(INCLUDE_FLASH)
#  include "mem/flashMem.c"
#  include "mem/nvRamToFlash.c"
#else
#  include "mem/nullNvRam.c"
#endif  /* INCLUDE_NV_RAM || INCLUDE_FLASH */

#include "sysEpic.c"

#include "sysL2Cache.c"

#ifdef INCLUDE_CPM
#   include "mem/m82xxDpramLib.c"
#   include "m8560CpmIntrCtl.c"
#   ifdef INCLUDE_SCC_SERIAL
#       include "m8260Sio.c"
#       include "sysScc.c"
#   endif  /* INCLUDE_SCC_SERIAL */
#endif  /* INCLUDE_CPM */

#ifdef INCLUDE_END
#include "sysNet.c"
#endif

/* defines */

#define END_OF_TABLE	0
#define ZERO		0

#define DELTA(a,b)	(abs((int)a - (int)b))
#define HID0_MCP	0x80000000
#define HID1_ABE	0x00001000
#define HID1_ASTME	0x00002000


/* functions */

#if     defined (INCLUDE_SPE)
/*******************************************************************************
*
* sysSpeProbe - Check if the CPU has SPE unit.
*
* This routine returns OK it the CPU has an SPE unit in it.
* Presently it assumes available.
*
* RETURNS: OK 
*
* ERRNO
*/

int  sysSpeProbe (void)
    {
    ULONG regVal;
    int   speUnitPresent = OK;

     /* The CPU type is indicated in the Processor Version Register (PVR) */
 
     regVal = 0;

    switch (regVal)
        {
        case 0:
        default:
            speUnitPresent = OK;
            break;
        }      /* switch  */

    return (speUnitPresent);
    }
#endif  /* INCLUDE_SPE */


/******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*
* ERRNO
*/

char * sysModel (void)
    {
    UINT8 id;

    id = *(UINT8 *) UTIL_SR2;
    if ((id & BRD_SR2_8540SEL_MSK) == BRD_SR2_MODEL_8540)
        return(SYS_MODEL_8540);
    else
        return (SYS_MODEL_8560);
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
* ERRNO
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

/******************************************************************************
*
* sysCoreFreqGet - Determines the E500 Core Operating Frequency
*
* This routine returns the E500 Core Operating Frequency
*
* RETURNS: Core clock frequency in Hertz
*
* ERRNO
*/

UINT32 sysCoreFreqGet (void)
    {
    int coreRatio;

    coreRatio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    switch (coreRatio)
        {
        case 4:
        case 5:
        case 6:
        case 7:
            return (sysClkFreqGet() * coreRatio >> 1);
        break;

        default:
            return (0);
        }
    }

/******************************************************************************
*
* sysClkFreqGet - Determines the CCB Operating Frequency
*
* This routine returns the CBB Operating Frequency.
*
* RETURNS: CCB clock frequency in Hertz
*
* ERRNO
*/

UINT32 sysClkFreqGet (void)
    {
    int platRatio;		/* all ratio values are x:1 */

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);
    switch (platRatio)
        {
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
        case 10:
        case 12:
        case 16:
            return (OSCILLATOR_FREQ * platRatio);
        break;

        default:
            return (0);
        }
    }

/******************************************************************************
*
* sysCpmFreqGet - Determines the CPM Operating Frequency
*
* This routine returns the CPM Operating Frequency.
* 
* RETURNS: CPM clock frequency in Hertz
*
* ERRNO
*/

UINT32 sysCpmFreqGet (void)
    {
    return  (sysClkFreqGet());	/* same as CCB freq */
    }

#ifdef INCLUDE_SCC_SERIAL
/******************************************************************************
*
* sysBaudClkFreq - Obtains frequency of the BRG_CLK in Hz
*
* From page 9-5 in Rev. 0 MPC8260 PowerQUICC II User's Manual
*
*     baud clock = 2*cpm_freq/2^2*(DFBRG+1) where DFBRG = 01
*                = 2*cpm_freq/16
*
* RETURNS: frequency of the BRG_CLK in Hz
*
* ERRNO
*/

int sysBaudClkFreq
    (
     void
    )
    {
    UINT32 cpmFreq = sysCpmFreqGet();

    if (cpmFreq == (UINT32) ERROR)
        return ERROR;
    else
        return cpmFreq*2/16;
    }
#endif  /* INCLUDE_SCC_SERIAL */

/******************************************************************************
*
* sysHwMemInit - initialize and configure system memory.
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
* ERRNO
*/

void sysHwMemInit (void)
    {
    static char * physTop;

    /* Call sysPhysMemTop() to do memory autosizing if available */

    physTop = sysPhysMemTop ();
    }

/******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the WR SBC85xx board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysHwInit (void)
    {
#ifdef INCLUDE_CPM
    UINT32 immrVal;
#endif /* INCLUDE_CPM */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CPM
    immrVal = vxImmrGet();
    memset((void *)(CCSBAR+0x80000),0,0x1000);
    m82xxDpramLibInit();

    /* Zero IO Port registers */
    /* reset the parallel ports */

    * M8260_IOP_PADIR(immrVal) = 0x00000000;
    * M8260_IOP_PAPAR(immrVal) = 0x00000000;
    * M8260_IOP_PASO(immrVal) = 0x00000000;
    * M8260_IOP_PAODR(immrVal) = 0x00000000;
    * M8260_IOP_PADAT(immrVal) = 0x00000000;

    * M8260_IOP_PBDIR(immrVal) = 0x00000000;
    * M8260_IOP_PBPAR(immrVal) = 0x00000000;
    * M8260_IOP_PBSO(immrVal) = 0x00000000;
    * M8260_IOP_PBODR(immrVal) = 0x00000000;
    * M8260_IOP_PBDAT(immrVal) = 0x00000000;

    * M8260_IOP_PCDIR(immrVal) = 0x00000000;
    * M8260_IOP_PCPAR(immrVal) = 0x00000000;
    * M8260_IOP_PCSO(immrVal) = 0x00000000;
    * M8260_IOP_PCODR(immrVal) = 0x00000000;
    * M8260_IOP_PCDAT(immrVal) = 0x00000000;

    * M8260_IOP_PDDIR(immrVal) = 0x00000000;
    * M8260_IOP_PDPAR(immrVal) = 0x00000000;
    * M8260_IOP_PDSO(immrVal) = 0x00000000;
    * M8260_IOP_PDODR(immrVal) = 0x00000000;
    * M8260_IOP_PDDAT(immrVal) = 0x00000000;
#endif  /* INCLUDE_CPM */

    /* Disable L1 Icache */
    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* Put cachePpcReadOrigin at the start of data section */
    cachePpcReadOrigin = &_wrs_kernel_data_start;

    /* Enable machine check pin */
    vxHid0Set(vxHid0Get() | HID0_MCP);

    sysTimerClkFreq = sysClkFreqGet() >> 3;	/* clock div is 8 */
    if (sysTimerClkFreq == 0)
        sysTimerClkFreq = PLAT_FREQ_DEFAULT >> 3;

    /* enable time base for delay use before DEC interrupt is setup */
    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(AUX_CLK_RATE);
#endif

#ifdef INCLUDE_CACHE_SUPPORT
     sysL1CacheQuery(); 
#endif /* INCLUDE_CACHE_SUPPORT */

    /* Initialize L2CTL register */
    vxL2CTLSet(0x28000000, M85XX_L2CTL(CCSBAR));

    /* Need to setup static TLB entries for bootrom or any non-MMU 
     * enabled images */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (!defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL))
    mmuPpcIEnabled=TRUE;
    mmuPpcDEnabled=TRUE;
#else  /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

    if (inFullVxWorksImage==FALSE)
        {
        mmuPpcIEnabled=TRUE;
        mmuPpcDEnabled=TRUE;
        }

#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    /* Enable I Cache if instruction mmu disabled */
    mmuPpcIEnabled=TRUE;
#endif /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif  /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */


#ifdef INCLUDE_SCC_SERIAL
    * M85XX_CPM_SCCR(CCSBAR) = BRGCLK_DIV_FACTOR;
#endif

#ifdef INCLUDE_CPM
    /* reset the Communications Processor */
    *M8260_CPCR(CCSBAR + 0x80000) = 0x80010000;
#ifdef  INCLUDE_SCC_SERIAL
    baudRateGenClk = 0xFDAD68;
#endif  /* INCLUDE_SCC_SERIAL */
#endif  /* INCLUDE_CPM */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    vxHid1Set(vxHid1Get() | HID1_ABE );
    sysL2CacheInit();
#endif /* INCLUDE_L2_CACHE && INCLUDE_CACHE_SUPPORT*/

    vxHid1Set(vxHid1Get() | HID1_ASTME); /* Address streaming enable */

    /* Needed for the now obsolete NO_VX_BOOTROM */
    * (VINT32 *) M85XX_BR0(CCSBAR) = (VINT32) (BOOT_FLASH_ADRS | 0x0801);
    * (VINT32 *) M85XX_OR0(CCSBAR) = (VINT32) (BOOT_FLASH_ADRS | 0x0797);

#ifdef USE_ALT_BOOTLINE
    strcpy (BOOT_LINE_ADRS, altBootLine);
#endif

    /* Initialize the Embedded Programmable Interrupt Controller */
    sysEpicInit();

#ifdef INCLUDE_VXBUS
# ifdef INCLUDE_CPM
#  ifdef VXB_LEGACY_ACCESS
    pPlbAccessFunctionOverride = m85xxPlbAccessFunctionOverride;
#  else  /* VXB_LEGACY_ACCESS */
    pPlbDevControl = (VOIDFUNCPTR) m85xxPlbDevControl;
#  endif /* VXB_LEGACY_ACCESS */
    pSysPlbMethods = m85xxPlbMethods;
# endif /* INCLUDE_CPM */
    hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_SCC_SERIAL
    sysSerialHwInit();
#endif

    CACHE_PIPE_FLUSH();

#ifdef INCLUDE_L2_SRAM
# if (defined(INCLUDE_CACHE_SUPPORT) && defined(INCLUDE_L2_CACHE))
    sysL2SramEnable(TRUE);
# else
    sysL2SramEnable(FALSE);
# endif
#endif  /* INCLUDE_L2_SRAM */
    }


#ifdef INCLUDE_L2_SRAM
/******************************************************************************
*
* sysL2SramEnable - enables L2SRAM if L2SRAM
*
* This routine enables L2SRAM if L2SRAM only or initializes blk size etc
* and leaves the rest to L2 cache code.
*
* RETURNS: N/A
*
* ERRNO
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
    *(M85XX_L2SRBAR0(CCSBAR)) = (VUINT32)(L2SRAM_ADDR & M85XX_L2SRBAR_ADDR_MSK); 
    /* Get present value */
    l2CtlVal = vxL2CTLGet(M85XX_L2CTL(CCSBAR));

    /* Disable L2CTL initially to allow changing of block size */
    l2CtlVal&=(~M85XX_L2CTL_L2E_MSK);
    vxL2CTLSet(l2CtlVal,M85XX_L2CTL(CCSBAR));
    l2CtlVal &= ~M85XX_L2CTL_L2BLKSIZ_MSK;
    l2CtlVal &= ~M85XX_L2CTL_L2SRAM_MSK;

    if (both == TRUE)
        {
        /* Setup size of SRAM */
        l2CtlVal |= (L2SIZ_128KB << M85XX_L2CTL_L2BLKSIZ_BIT) | 
                    (0x2 << M85XX_L2CTL_L2SRAM_BIT);
        }
    else
        {
        l2CtlVal |= (L2SIZ_256KB << M85XX_L2CTL_L2BLKSIZ_BIT) | 
                    (0x1 << M85XX_L2CTL_L2SRAM_BIT);
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

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
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
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
        }

    return (physTop) ;
    }

/*******************************************************************************
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


/******************************************************************************
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
* ERRNO
*/

STATUS sysToMonitor
    (
     int startType	/* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);	/* Warm reboot */

    intLock ();     /* disable interrupts */

#ifdef INCLUDE_BRANCH_PREDICTION
   vxDisableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
    sysClkDisable();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif
    /* sysHwInit(); */                /* reset the serial device */
    vxMsrSet (0);                     /* ??? */

    (*pRom) (startType);	/* jump to bootrom entry point */
    
    return (OK);    /* in case we ever continue from ROM monitor */
    }

/******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysHwInit2 (void)
    {
#ifdef  INCLUDE_VXBUS
    vxbDevInit();
#endif /* INCLUDE_VXBUS */
	
    excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_DECR, (VOIDFUNCPTR) sysClkInt);
    sysClkEnable();


#ifdef INCLUDE_AUX_CLK
    excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_FIT, (VOIDFUNCPTR) sysAuxClkInt);
#endif

    /* initialize the EPIC interrupts */
    sysEpicIntrInit ();

#ifdef INCLUDE_CPM 
    sysCpmHwInit2();
#endif

#ifdef INCLUDE_ECC
    intConnect ( (VOIDFUNCPTR*)EPIC_DDR_INT_VEC, ddrDeviceIntHandler, 0);
#endif  /* INCLUDE_ECC */

#if defined(INCLUDE_VXBUS) && defined(INCLUDE_SIO_UTILS)
    sysSerialConnectAll();
#endif /* INCLUDE_VXBUS && INCLUDE_SIO_UTILS */

#if     defined (INCLUDE_SPE)
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */

#ifdef  INCLUDE_VXBUS
    taskSpawn("tDevConn", 11, 0, 10000,
              vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

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
* ERRNO
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return (sysProcNum);
    }

/******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* Not applicable for this board.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int 	procNum			/* processor number */
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
* Not applicable for this board.
*
* RETURNS: ERROR, always.
*
* ERRNO
*
* SEE ALSO: sysBusToLocalAdrs()
*/
 
STATUS sysLocalToBusAdrs
    (
    int 	adrsSpace,	/* bus address space where busAdrs resides */
    char *	localAdrs,	/* local address to convert */ 
    char **	pBusAdrs	/* where to return bus address */ 
    )
    {
    return (ERROR);
    }

/******************************************************************************
*
* sysBusToLocalAdrs - convert a bus address to a local address
*
* This routine gets the local address that accesses a specified VMEbus
* physical memory address.
*
* Not applicable for this board.
*
* RETURNS: ERROR, always.
*
* ERRNO
*
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
    (
    int    adrsSpace,  /* bus address space where busAdrs resides */
    char * busAdrs,    /* bus address to convert */
    char **    pLocalAdrs  /* where to return local address */
    )
    {
    return(ERROR);
    }

/******************************************************************************
*
* sysBusTas - test and set a location across the bus
*
* This routine does an atomic test-and-set operation across the backplane.
*
* Not applicable for this board.
*
* RETURNS: FALSE, always.
*
* ERRNO
*
* SEE ALSO: vxTas()
*/

BOOL sysBusTas
    (
     char *	adrs		/* address to be tested-and-set */
    )
    {
    return (FALSE);
    }

/******************************************************************************
*
* sysBusClearTas - test and clear 
*
* This routine is a null function.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysBusClearTas
    (
     volatile char * address	/* address to be tested-and-cleared */
    )
    {
    } 


#ifdef INCLUDE_MOT_FCC_END
/*******************************************************************************
*
* sysFccEnetAddrGet - get the hardware Ethernet address
*
* This routine provides the six byte Ethernet hardware address that will be
* used by each individual FCC device unit.  This routine must copy
* the six byte address to the space provided by <addr>.
*
* RETURNS: OK, or ERROR if the Ethernet address cannot be returned.
*
* ERRNO
*/

STATUS sysFccEnetAddrGet
    (
    int         unit,           /* base address of the on-chip RAM */
    UCHAR *     addr            /* where to copy the Ethernet address */
    )
    {

    bcopy ((char *) &sysFccEnetAddr[unit][0], (char *) addr, 6);

    return (OK);
    }

#endif /* INCLUDE_MOT_FCC_END */

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
* ERRNO
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

void sysMsDelay
    (
    UINT      delay                   /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (UINT32) delay * 1000 );
    }


/*********************************************************************
*
* sysDelay - fixed 1ms delay
*
* This routine is a fixed 1ms delay. It just calls sysMsDelay.
*
* RETURNS : NONE
*
* ERRNO
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }


#ifdef INCLUDE_CACHE_SUPPORT
/***********************************************************************
*
* sysL1CacheQuery - populate L1 cache size
*
* This routine populates L1 cache size and alignment from
* configuration registers.
*
* RETURNS : NONE
*
* ERRNO
*/

LOCAL void sysL1CacheQuery()
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

    /* The core manual suggests for a 32 byte cache line and 8 lines per set 
       we actually need 12 unique address loads to flush the set. 
       The number of lines to flush should be ( 3/2 * cache lines ) */
    ppcE500DCACHE_LINE_NUM = (3*ppcE500DCACHE_LINE_NUM)>>1;
    ppcE500ICACHE_LINE_NUM = (3*ppcE500ICACHE_LINE_NUM)>>1;

    }
#endif /* INCLUDE_CACHE_SUPPORT */


/***************************************************************************
*
* vxImmrGet - get the CPM DP Ram base address
*
* This routine returns the CPM DP Ram base address for CPM device drivers.
*
* RETURNS:
*
* ERRNO
*/

UINT32 vxImmrGet (void)
    {
    return (CCSBAR + 0x80000);
    }


/***************************************************************************
*
* saveExcMsg - write exception message to save area for catastrophic error
*
* The message will be displayed upon rebooting with a bootrom.
*
* RETURNS: N/A
*
* ERRNO
*/

void saveExcMsg (
    char *errorMsg
    )
    {
    strcpy ((char *)EXC_MSG_OFFSET, errorMsg);
    }


void chipErrataCpu29Print(void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    }

void ddrDeviceIntHandler(void)
    {
    saveExcMsg("ECC fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    sysToMonitor(BOOT_NO_AUTOBOOT);
    }

#ifdef _GNU_TOOL
/*
*
* To avoid symbol loading error when BSP is compiled with GCC, the
* following dummy functions and variable are declared here.
*/
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
* coreLbcShow - show routine for local bus controller
*
* This routine displays the data for the local bus controller.
*
* RETURNS: NONE
*
* ERRNO
*/

void coreLbcShow(void)
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

/***************************************************************************
*
* coreShow - show routine for core registers
*
* This routine displays the core registers.
*
* RETURNS: NONE
*
* ERRNO
*/

void coreShow(void)
    {
    VUINT32 tmp, tmp2;

    printf("System: %s\t\n", sysModel() );
    printf("Core Speed: %3d MHz\t\tPlatform Speed: %3d MHz\n",
           sysCoreFreqGet()/1000000, sysClkFreqGet()/1000000 );

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
    printf("L1CSR0: Dcache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
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
    tmp = *(VUINT32 *) (CCSBAR + 0x0c08);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c10);
    printf("LAWBAR0 = 0x%8x\t LAWAR0 = 0x%8x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c28);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c30);
    printf("LAWBAR1 = 0x%8x\t LAWAR1 = 0x%8x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c48);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c50);
    printf("LAWBAR2 = 0x%8x\t LAWAR2 = 0x%8x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c68);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c70);
    printf("LAWBAR3 = 0x%8x\t LAWAR3 = 0x%8x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0c88);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0c90);
    printf("LAWBAR4 = 0x%8x\t LAWAR4 = 0x%8x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0ca8);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0cb0);
    printf("LAWBAR5 = 0x%8x\t LAWAR5 = 0x%8x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0cc8);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0cd0);
    printf("LAWBAR6 = 0x%8x\t LAWAR6 = 0x%8x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (CCSBAR + 0x0ce8);
    tmp2 = *(VUINT32 *) (CCSBAR + 0x0cf0);
    printf("LAWBAR7 = 0x%8x\t LAWAR7 = 0x%8x\n", tmp, tmp2);
    }
#endif  /* INCLUDE_SHOW_ROUTINES */

/* INT LINE TO IRQ assignment for wrSbc85xx-PCI board. */

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] = {

    /* slot 1 (dev 16 / 0x10) ; J4 */
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL},

    /* slot 4 (dev 17 / 0x11) ; J5 */
    {PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL},

    /* slot 3 (dev 18 / 0x12) ; J6 */
    {PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL, PCI_XINT2_LVL},

    /* slot 0 (dev 19 / 0x13, PMC) */
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL},

    /* slot 2 (dev 23 / 0x17) ; J7 */
    {PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL, PCI_XINT1_LVL},
};

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*/

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    )
    {
    
    /* Don't want to auto configure the bridge */ 
     if ( (devVend == PCI_DEV_ID_8540) || (devVend == PCI_DEV_ID_8560) ||
          (devVend == PCI_DEV_ID_82XX))
      return(ERROR);
 
    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - PCI autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /* 
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin > 0) && (pin < 5))       				&& 
        ((pLoc->device) < 0x18)                         	&&
	(((pLoc->device) - PCI_SLOT1_DEVNO) >= 0))
	{
          int idx;
          
          idx = ((pLoc->device) - PCI_SLOT1_DEVNO); /* sebf */
          if (idx > NUM_PCI_SLOTS) idx = NUM_PCI_SLOTS - 1;
          
          tmpChar = sysPciIntRoute [idx] [(pin-1)];
	}

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }

#ifdef notdef

/* locals */

#define MAC_ADRS_LEN 6
#define MAX_MAC_ADRS 4

/* include source */


/* globals */

unsigned char glbEnetAddr [MAX_MAC_ADRS][MAC_ADRS_LEN] = {
    { USR_MAC5, USR_MAC4, USR_MAC3a, WR_MAC2, WR_MAC1, WR_MAC0 },
    { USR_MAC5, USR_MAC4, USR_MAC3b, WR_MAC2, WR_MAC1, WR_MAC0 },
    { USR_MAC5, USR_MAC4, USR_MAC3c, WR_MAC2, WR_MAC1, WR_MAC0 },
    { USR_MAC5, USR_MAC4, USR_MAC3d, WR_MAC2, WR_MAC1, WR_MAC0 }
};

/***********************************************************************
*
* sysEnetAddrSet - sets the 6 byte ethernet address
*
* This routine sets the 6 byte ethernet address used by the ethernet device.
*
* RETURNS: N/A
*
* SEE ALSO: sysEnetAddrGet()
*/

void sysEnetAddrSet
    (
    UCHAR byte5,
    UCHAR byte4,
    UCHAR byte3,
    UCHAR byte2,
    UCHAR byte1,
    UCHAR byte0
    )
    {
    int unit = 0;

    glbEnetAddr[unit][5] = byte5;
    glbEnetAddr[unit][4] = byte4;
    glbEnetAddr[unit][3] = byte3;
    glbEnetAddr[unit][2] = byte2;
    glbEnetAddr[unit][1] = byte1;
    glbEnetAddr[unit][0] = byte0;

#ifdef INCLUDE_NV_RAM
#if (NV_RAM_SIZE != 0)

    sysNvRamSet ((char *)&glbEnetAddr[unit][0], MAC_ADRS_LEN,
                 NV_MAC_ADRS_OFFSET + unit * MAC_ADRS_LEN);

#endif /* (NV_RAM_SIZE != NONE) */
#endif  /* INCLUDE_NV_RAM */
    }

/***********************************************************************
*
* sysEnetAddrGet - gets the 6 byte ethernet address
*
* This routine gets the 6 byte ethernet address used by the ethernet device.
*
* RETURNS: OK
*
* SEE ALSO: sysEnetAddrSet()
*/

STATUS sysEnetAddrGet
    (
    int     unit,
    UCHAR * addr     /* where address is returned in */
    )
    {
    int n;
    UCHAR   addr2[MAC_ADRS_LEN];

    for (n = 0; n < MAC_ADRS_LEN; n++)
	 *(addr+n) = glbEnetAddr[unit][n];

    /* If NvRAM supported overwrite it with data from NvRAM */

#ifdef INCLUDE_NV_RAM
#if (NV_RAM_SIZE != 0)

    sysNvRamGet (&addr2[0], MAC_ADRS_LEN,
                 NV_MAC_ADRS_OFFSET + unit * MAC_ADRS_LEN);

    if (((addr2[5] != 0x00) || (addr2[4] != 0xa0) || (addr2[3] != 0x1e)) ||
        ((addr2[2] == 0xff) && (addr2[2] == 0xff) && (addr2[2] == 0xff)))
        {
        return (OK);  /* just use default add from glbEnetAddr[][] */
        }
    memcpy (addr, &addr2[0], MAC_ADRS_LEN);

#endif /* (NV_RAM_SIZE != NONE) */
#endif  /* INCLUDE_NV_RAM */

    return (OK);
    }

#endif /* INCLUDE_END */ 

/******************************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine gets the SIO_CHAN device associated with a specified serial
* channel.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*/

#ifdef INCLUDE_SIO_UTILS
#define BSP_SERIAL_CHAN_GET	bspSerialChanGet
#else /* INCLUDE_SIO_UTILS */
#define BSP_SERIAL_CHAN_GET	sysSerialChanGet
#endif /* INCLUDE_SIO_UTILS */

SIO_CHAN *BSP_SERIAL_CHAN_GET 
    (
    int channel
    )
    {
    return((SIO_CHAN *)ERROR);
    }

#ifdef DRV_SIO_NS16550
void bspNs16550Connect (int unit)
    {
    sioPollOutput(sysSerialChanGet(unit), '\r');
    }
#endif /* DRV_SIO_NS16550 */
/***********************************************************************
*
* sysEnetAddrGet - gets the 6 byte ethernet address
*
* This routine gets the 6 byte ethernet address used by the ethernet device.
*
* RETURNS: OK
*
* SEE ALSO: sysEnetAddrSet()
*/

STATUS sysEnetAddrGet
    (
    int     unit,
    UCHAR * addr     /* where address is returned in */
    )
    {
    return (sysNetMacNVRamAddrGet ("mottsec", unit, addr, MAC_ADRS_LEN));
    }


#ifdef INCLUDE_VXBUS
#ifdef INCLUDE_CPM

LOCAL STATUS m85xxPlbDevControl
    (
    VXB_DEVICE_ID       pDev,          /* device info */
    pVXB_DEVCTL_HDR     pBusDevControl  /* parameter */
    )
    {
    STATUS status = OK;
    UINT32 vector, source;
    struct plbIntrEntry * pIntr;
    pVXB_ACCESS_INTERRUPT_INFO pIntInfo;
    pVXB_ACCESS_INT_VECTOR_GET pIntVector;
    UINT32 * pVector;

    /* check the validity of the parameters */

    if ((pDev == NULL) || (pBusDevControl == NULL))
        return (ERROR);

    pIntr = pDev->pIntrInfo;
    if (pIntr == NULL)
        return (ERROR);

    pIntInfo = (pVXB_ACCESS_INTERRUPT_INFO)pBusDevControl;
    pIntVector = (pVXB_ACCESS_INT_VECTOR_GET)pIntInfo;

    if (pIntInfo->intIndex >= pIntr->numVectors)
        return (ERROR);

    vector = pIntr->intVecList[pIntInfo->intIndex];
    source = pIntr->intLvlList[pIntInfo->intIndex];

    switch (pBusDevControl->vxbAccessId)
        {
        case VXB_ACCESS_INT_CONNECT:
            if (strcmp(pDev->pName, "motfcc") == 0)
                {
                m85xxCpmIntConnect (vector,
                    pIntInfo->pISR, (int *)pIntInfo->pParameter);
                status = OK;
                }
            else
                status = intConnect ((VOIDFUNCPTR *)vector,
                    pIntInfo->pISR, (int)pIntInfo->pParameter);
            break;
        case VXB_ACCESS_INT_DISCONN:
            if (strcmp(pDev->pName, "motfcc") == 0)
                {
                m85xxCpmIntDisconnect (vector,
                    pIntInfo->pISR, (int *)pIntInfo->pParameter);
                status = OK;
                }
            else
                status = intDisconnect ((VOIDFUNCPTR *)vector,
                    pIntInfo->pISR, (int)pIntInfo->pParameter);
            break;
        case VXB_ACCESS_INT_VEC_GET:
            pVector = pIntr->intVecList;
            pIntVector->pIntVector =
                (VOIDFUNCPTR)pVector[pIntInfo->intIndex];
            status = OK;
            break;
        case VXB_ACCESS_INT_ENABLE:
            if (strcmp(pDev->pName, "motfcc") == 0)
                {
                m85xxCpmIntEnable (vector);
                status = OK;
                }
            else
                status = intEnable (vector);
            break;
        case VXB_ACCESS_INT_DISABLE:
            if (strcmp(pDev->pName, "motfcc") == 0)
                {
                m85xxCpmIntDisable (vector);
                status = OK;
                }
            else
                status = intDisable (vector);
            break;
        default:
            status = ERROR;
            break;
        }

    return (status);
    }

# ifdef	VXB_LEGACY_ACCESS
LOCAL void m85xxPlbAccessFunctionOverride
    (
    struct vxbAccessList * pAccessList
    )
    {
    pAccessList->vxbDevControl = m85xxPlbDevControl;
    return;
    }
# endif	/* VXB_LEGACY_ACCESS */
#endif /* INCLUDE_CPM */

#ifdef INCLUDE_MDIO

STATUS sysMdioEnable
    (
    VXB_DEVICE_ID pDev
    )
    {
    UINT32 immrVal;
    int s;

    immrVal = vxImmrGet ();

    s = intLock ();

    /* Enable access to MDIO management pins */

    *M8260_IOP_PCPAR(immrVal) &= ~(PC10 | PC9);
    *M8260_IOP_PCDIR(immrVal) |= (PC10);

    intUnlock (s);

    return (OK);
    }

#endif /* INCLUDE_MDIO */

#ifdef INCLUDE_FCC_VXB_END

STATUS sysFccEnetEnable
    (
    VXB_DEVICE_ID pDev,
    UINT8 fccNum   /* FCC being used */
    )
    {
    UINT32 immrVal;
    int intLevel;
    int rval = OK;

    immrVal = vxImmrGet ();
    intLevel = intLock ();


    switch (fccNum)
        {
        case 2:

            *M8260_IOP_PCPAR(immrVal) |=  (PC19 | PC18);
            *M8260_IOP_PCSO(immrVal)  &= ~(PC19 | PC18);
            *M8260_IOP_PCDIR(immrVal) &= ~(PC19 | PC18);

            *M8260_IOP_PBPAR(immrVal) |=  (PB31 | PB30 | PB29 | PB28 | PB27 | 
                                           PB26 | PB25 | PB24 | PB23 | PB22 | 
                                           PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBDIR(immrVal) |=  (PB31 | PB29 | PB25 | PB24 | PB23 | 
                                           PB22);
            *M8260_IOP_PBDIR(immrVal) &= ~(PB30 | PB28 | PB27 | PB26 | PB21 | 
                                           PB20 | PB19 | PB18);
            *M8260_IOP_PBSO(immrVal)  &= ~(PB31 | PB30 | PB28 | PB27 | PB26 | 
                                           PB25 | PB24 | PB23 | PB22 | PB21 | 
                                           PB20 | PB19 | PB18);
            *M8260_IOP_PBSO(immrVal)  |=  (PB29);

            /* connect FCC2 clocks */

            *M8260_CMXFCR (immrVal)  |= (M8260_CMXFCR_R2CS_CLK13 | 
                                         M8260_CMXFCR_T2CS_CLK14);
            *M8260_CMXFCR (immrVal)  &= ~(M8260_CMXFCR_FC2_MUX);
            break;

        case 3:

            *M8260_IOP_PDSO(immrVal) |= PD4;
            *M8260_IOP_PCSO(immrVal) &= ~(PC17 | PC16);
            *M8260_IOP_PBDIR(immrVal) |= (PB15 | PB14 | PB7 | PB6 | PB5 | PB4);
            *M8260_IOP_PCDIR(immrVal) |= (PC27);
            *M8260_IOP_PDDIR(immrVal) |= (PD4);
            *M8260_IOP_PCPAR(immrVal) |= (PC16 | PC17);
            *M8260_IOP_PCPAR(immrVal) |= (PC3 | PC2 | PC27);
            *M8260_IOP_PBPAR(immrVal) |= (PB17 | PB16 | PB15 | PB14 | PB13 | 
                                          PB12 | PB11 | PB10 | PB9  | PB8  | 
                                          PB7  | PB6  | PB5  | PB4);
            *M8260_IOP_PDPAR(immrVal) |= (PD4);
    
            /* connect FCC3 clocks */

            *M8260_CMXFCR (immrVal)  |= (M8260_CMXFCR_R3CS_CLK15 | 
                                         M8260_CMXFCR_T3CS_CLK16);

            /* NMSI mode */

            *M8260_CMXFCR (immrVal)  &= ~(M8260_CMXFCR_FC3_MUX);         

            break;

        default:
            rval = ERROR;
            break;
        }

    intUnlock (intLevel);

    return (rval);
    }

#endif /* INCLUDE_FCC_VXB_END */

#ifdef DRV_SIO_NS16550
STATUS sysNs16550Enable
    (
    VXB_DEVICE_ID pDev
    )
    {
    sioPollOutput (sysSerialChanGet(pDev->unitNumber), '\r');
    return (OK);
    }
#endif /* DRV_SIO_NS16550 */

#endif /* INCLUDE_VXBUS */
