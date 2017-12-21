/* sysLib.c - wrVbP4080 system-dependent library */

/*
 * Copyright (c) 2009-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01z,27aug13,j_b  CQ:WIND00432161 - fix virtual device filtering
01y,08aug13,j_b  CQ:WIND00429511 - add NULL pointer check in sysDeviceFilter;
                 fix unchecked sscanf() return value
01x,15may12,c_l  Add vbiNsRegister when making use of CONTROL_PLANE_CORE
                 preprocessing. (WIND00281408)
01w,25may11,agf  Make timer and nvRam PTE's conditionally compiled.
                 Replace INCLUDE_FLASH with NV_RAM_SIZE, the latter exists
                 in CDF and can be manipulated by WB and vxprj.
                 Rename the debugging print macro more appropriately.
01v,04may11,agf  Fix sysDeviceFilter logic, also added more devices & comments
01u,07mar11,kab  NAP 2.0 merge: Add TGEC support
01t,08feb11,jlv  Use new vbiNsLookup interface
01s,14feb11,kxb  Fix misplaced comma
01r,31jan11,wap  Fix support for multiple DTSEC ports in a single guest
		 (WIND00241744)
01q,19jan11,wap  Correct defects WIND00227349 and WIND00248622
01p,03aug10,rgo  CQID: Fix WIND226420 - wrload sets bootline correctly
01o,29juk10,rab  Add VBI_VBMGMT_RESET_CLEAR to flags for vbiVbRest
01n,28jul10,rab  Fix sysToMonitor for SMP. Fix vxCpuIdToPhysIndex and
		 vxCpuPhysIndexToId
01m,23jul10,wap  Restrict data plane to one core, make it configurable
01l,25jun10,wap  Dynamically configure MIPC_SM mapping based on hypervisor VBI
		 info
01k,12jun10,rab  Add smp guest support
01j,22feb10,wap  Portal configuration is now automatic
01i,02feb10,wap  Fix defect WIND00198615
01h,25jan10,to   add sysV2pCpuNumGet
01g,14jan10,to   map MIPC shared memory region
01f,08jan10,wap  Provide local version of vxCpuIdGet()
01e,07jan10,wap  Avoid startup race condition between control and data plane
		 images, determine core number from board ID, allow multiple a
		 single instance to have multiple DTSECs
01d,23dec09,wap  Allow dynamic setup of BMAN, QMAN and DTSEC devices, add
		 workaround for determining the current core number, add code
		 to correctly determine DTSEC/TGEC MAC addresses
01c,21dec09,to   remove PIXIS mapping; add openpic timer mapping
		 use sysDeviceFilter for uart configuration
01b,21dec09,wap  Add support for LAW, BMAN, QMAN, FMAN and DTSEC
01a,01dec09,to   created from wrVbE500Common/sysLib.c, v01l
*/

/*
DESCRIPTION
This library provides board-specific routines for the WR P4080 Virtual Board.

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdio.h>
#include <string.h>
#include <sysLib.h>
#include <taskLib.h>
#include <arch/ppc/mmuE500Lib.h>
#include <private/vmLibP.h>
#include <wrhv/vbi.h>
#include <hwif/vxbus/vxBus.h>
#include <vxCpuLib.h>
#include "sysDuart.h"
#include "config.h"
#ifdef _WRS_CONFIG_SMP
#include <private/cpcLibP.h>
#endif

/* defines */

#undef  GUEST_DEBUG
#ifdef  GUEST_DEBUG
#define _GUEST_DEBUG_PRINT(fmt,...) {char logBuf[0x100]; sprintf(logBuf,fmt,__VA_ARGS__); vbiKputs(logBuf);}
#else
#define _GUEST_DEBUG_PRINT(fmt,...)
#endif

/*
 * sysPhysMemDesc[] dummy entries:
 * these create space for updating sysPhysMemDesc table at a later stage
 * mainly to provide plug and play
 */

#define DUMMY_PHYS_ADDR         -1
#define DUMMY_VIRT_ADDR         -1
#define DUMMY_LENGTH            -1
#define DUMMY_INIT_STATE_MASK   -1
#define DUMMY_INIT_STATE        -1

#define DUMMY_MMU_ENTRY         { (VIRT_ADDR) DUMMY_VIRT_ADDR, \
				  (PHYS_ADDR) DUMMY_PHYS_ADDR, \
				  (UINT) DUMMY_LENGTH, \
				  (UINT) DUMMY_INIT_STATE_MASK, \
				  (UINT) DUMMY_INIT_STATE \
				}

/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr, realAddrExt, realAddr, ts | size | attributes | permissions */

#ifdef DRV_SIO_NS16550
    { DUART1_BASE, 0x0, DUART1_BASE, _MMU_TLB_TS_0 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
    { DUART1_BASE, 0x0, DUART1_BASE, _MMU_TLB_TS_1 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
#endif /* DRV_SIO_NS16550 */

#ifdef DRV_RESOURCE_QORIQLAW
    { (CCSBAR + 0xC00), 0x0, (CCSBAR + 0xC00), _MMU_TLB_TS_0 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
    { (CCSBAR + 0xC00), 0x0, (CCSBAR + 0xC00), _MMU_TLB_TS_1 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
#endif /* DRV_RESOURCE_QORIQLAW */

#ifdef DRV_TIMER_OPENPIC
#if (ENABLE_TIMER_A == TRUE)
    { (CCSBAR + 0x41000), 0x0, (CCSBAR + 0x41000), _MMU_TLB_TS_0 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
    { (CCSBAR + 0x41000), 0x0, (CCSBAR + 0x41000), _MMU_TLB_TS_1 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
#endif
#if (ENABLE_TIMER_B == TRUE)
    { (CCSBAR + 0x42000), 0x0, (CCSBAR + 0x42000), _MMU_TLB_TS_0 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
    { (CCSBAR + 0x42000), 0x0, (CCSBAR + 0x42000), _MMU_TLB_TS_1 | _MMU_TLB_SZ_4K |
      _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W | _MMU_TLB_IPROT
    },
#endif
#endif /* DRV_TIMER_OPENPIC */
};

int sysStaticTlbDescNumEnt = NELEMENTS(sysStaticTlbDesc);

PHYS_MEM_DESC sysPhysMemDesc [] =
{
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE      |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    0x1000000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

#if (NV_RAM_SIZE != NONE)
    {
    (VIRT_ADDR) FLASH_ADRS,
    (PHYS_ADDR) FLASH_ADRS,
    NV_RAM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },
#endif /* NV_RAM_SIZE */

#ifdef DRV_RESOURCE_QORIQBMAN
    /*
     * The buffer manager access area is 2MB wide, with the
     * first 1MB set as cacheable and the second 1MB set
     * as cache inhibited.
     */
    {
    (VIRT_ADDR) BMAN_LAW_BASE,
    (PHYS_ADDR) BMAN_LAW_BASE,
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE      |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) BMAN_LAW_BASE + 0x100000,
    (PHYS_ADDR) BMAN_LAW_BASE + 0x100000,
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
    /*
     * The queue manager access area is 2MB wide, with the
     * first 1MB set as cacheable and the second 1MB set
     * as cache inhibited.
     */
    {
    (VIRT_ADDR) QMAN_LAW_BASE,
    (PHYS_ADDR) QMAN_LAW_BASE,
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE      |
    VM_STATE_MEM_COHERENCY
    },

    {
    (VIRT_ADDR) QMAN_LAW_BASE + 0x100000,
    (PHYS_ADDR) QMAN_LAW_BASE + 0x100000,
    0x100000,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },
#endif /* DRV_RESOURCE_QORIQQMAN */

    /* entries for dynamic mappings - create sufficient entries */
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,

    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,

    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY,
    DUMMY_MMU_ENTRY

};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);


int	sysCpu      = CPU;		/* system CPU type (PPC32) */
/*
 * For the wrload bootline mechanism below to properly work, this variable
 * needs to be in the data section. This is the case for Diab but not for
 * GNU unless -fno-zero-initialized-in-bss is specified (which is, on
 * 04aug10, not the default). If the variable ends in the bss section, the
 * usrStartup code handling CLEAR_BSS will clear the bootline set by wrload
 * before we enter sysHwInit().
 */

char  sysBootLineArray [BOOT_LINE_SIZE] = {EOS};
char * sysBootLine = sysBootLineArray;
/*char * sysBootLine = BOOT_LINE_ADRS;*/

char *	sysExcMsg   = EXC_MSG_ADRS;	/* catastrophic message area */
int	sysProcNum  = 0;		/* processor number of this CPU */
int	sysConsoleTty  = TTY_UART1;
int	sysConsoleBaud = 115200;
UINT32	sysCoreNums[8];

void * sysSystemGlobals = (void *) LOCAL_MEM_LOCAL_ADRS;
UINT32 sysSystemGlobalsSize = 0x100;

UINT32	ppcE500ICACHE_LINE_NUM  = (64 * 8);
UINT32	ppcE500DCACHE_LINE_NUM  = (64 * 8);
UINT32	ppcE500CACHE_ALIGN_SIZE = 64;

#ifdef _WRS_CONFIG_SMP
LOCAL UINT32 tbHI;	/* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;	/* lower 32 bit value of timebase */
volatile UINT32 sysCpu1LoopCount = 0;
#endif /* _WRS_CONFIG_SMP */

/* locals */

LOCAL STATUS sysDeviceFilter (VXB_DEVICE_ID pDev);
LOCAL UINT32 sysFmanClkFreqGet (void);
LOCAL UINT64 sysVirtToPhys (void *);
LOCAL void * sysPhysToVirt (UINT64);
LOCAL UINT32 sysV2pCpuNumGet (UINT32 vcoreNum);
LOCAL STATUS sysMmuMapAdd  (void * address, UINT len, UINT initialStateMask,
		      UINT initialState);

LOCAL UINT64 virtToPhysOff;

METHOD_DECL(sysBspDevFilter);
DEVMETHOD_DEF(sysFmanClkFreqGet, "Get Fman clock frequency");
DEVMETHOD_DEF(sysVirtToPhys, "BSP virtual to physical address translation");
DEVMETHOD_DEF(sysPhysToVirt, "BSP physical to virtual address translation");

LOCAL struct vxbDeviceMethod p4080PlbMethods[] =
    {
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
    DEVMETHOD(sysVirtToPhys, sysVirtToPhys),
    DEVMETHOD(sysPhysToVirt, sysPhysToVirt),
    DEVMETHOD(sysFmanClkFreqGet, sysFmanClkFreqGet),
    { 0, 0 }
    };

/* externals */

IMPORT device_method_t * pSysPlbMethods;

IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc,
				  BOOL cacheAllow);
IMPORT void hardWareInterFaceInit (void);
IMPORT UINT32 sysPIRGet (void);
IMPORT UINT32 sysWrhvCoreIdGet (void);
#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif

/* source files */

#include "hwconf.c"

#if (NV_RAM_SIZE != NONE)
#    include "flashMem.c"
#    include <mem/nvRamToFlash.c>
#else
#    include <mem/nullNvRam.c>
#endif /* NV_RAM_SIZE */

#if defined(DRV_VXBEND_DTSEC) || defined(DRV_VXBEND_TGEC)
#    include "sysNet.c"
#endif

/******************************************************************************
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
    return SYS_MODEL;
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
    return (BSP_VERSION BSP_REV);
    }

/******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various hardware features of the board. It sets up
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
    BOOT_PARAMS params;
    VBI_NS_HANDLE vbiNsHandle;
    PHYS_MEM_DESC * pMmu;
    VB_SM_INFO * smInfo;
    char *result = NULL;
    int32_t vector = 0;
    volatile int b;
    int i;

    mmuE500TlbStaticInit (sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    /*
     * run-time update of the sysPhysMemDesc MMU entry for main RAM
     * This means SDRAM _must_ be represented by sysPhysMemDesc[0]
     */

    sysPhysMemDesc[0].len = (msize_t)(sysPhysMemTop() - LOCAL_MEM_LOCAL_ADRS);

    /* initialize the number of active mappings (sysPhysMemDescNumEnt) */

    pMmu = &sysPhysMemDesc[0];

    for (i = 0; i < NELEMENTS (sysPhysMemDesc); i++)
	if (pMmu->virtualAddr != (VIRT_ADDR)DUMMY_VIRT_ADDR)
	    pMmu++;
	else
	    break;

    sysPhysMemDescNumEnt = i;

    /*
     * Map shared memory regions.
     * This mainly accounts for MIPC shared memory sections.
     */

    smInfo  = VBI_CONFIG_ADDR_GET()->sharedMemoryRegionsConfigAddress;

    for (i = 0; i < VBI_CONFIG_ADDR_GET()->numSm; i++, smInfo++)
	{
	sysMmuMapAdd ((void *)smInfo->addr, smInfo->length,
		VM_STATE_MASK_VALID | VM_STATE_MASK_CACHEABLE |
		VM_STATE_MASK_MEM_COHERENCY | VM_STATE_MASK_WRITABLE,
		VM_STATE_VALID | VM_STATE_CACHEABLE |
		VM_STATE_MEM_COHERENCY | VM_STATE_WRITABLE);
	}

    interruptConversionTableSetup();

    /* set the boot line */

    /*
     * only copy bootline from hypervisor config if it has not been set by
     * wrload.
     */

    _GUEST_DEBUG_PRINT("[VB %d] sysBootLine (%p): %s\n"
		 "[VB %d] sysBootLineArray (%p): %s\n"
		 "[VB %d] BOOT_LINE_ADRS (%p): %s\n",
		 VBI_BOARD_ID_GET(), sysBootLine, sysBootLine,
		 VBI_BOARD_ID_GET(), sysBootLineArray, sysBootLineArray,
		 VBI_BOARD_ID_GET(), BOOT_LINE_ADRS, BOOT_LINE_ADRS);

    if (sysBootLine[0] == EOS)
	{
	 _GUEST_DEBUG_PRINT("[VB %d] Getting bootline from hypervisor config\n",
		      VBI_BOARD_ID_GET());
	strncpy(sysBootLine, VBI_BOOTLINE_ADDR_GET(), VB_MAX_BOOTLINE_LENGTH);
	}
    else
	{
	_GUEST_DEBUG_PRINT("[VB %d] Bootline left as is\n", VBI_BOARD_ID_GET());
	}

    /*
     * Copy sysBootLine contents to BOOT_LINE_ADRS so that code relying on
     * BOOT_LINE_ADRS see no difference (sysBootLine is normally set to
     * BOOT_LINE_ADRS).
     * Finally, we move back sysBootLine to BOOT_LINE_ADRS so that both
     * locations now match.
     */

    memcpy (BOOT_LINE_ADRS, sysBootLine, BOOT_LINE_SIZE);
    sysBootLine = BOOT_LINE_ADRS;

    _GUEST_DEBUG_PRINT("[VB %d] Bootline contents at BOOT_LINE_ADRS: %s\n",
		  VBI_BOARD_ID_GET(), BOOT_LINE_ADRS);

    /* extract the console serial port from the boot line */

    result = bootStringToStruct (sysBootLine, &params);
    if ((sysBootLine != NULL) && (result != NULL) && (*result == EOS))
	{
	/* use the console name specified in the o= (other) field */
	if (params.other[0] != '\0')
	    {
	    if (strncmp ("console-", params.other, 8) == 0)
		{
		/* He is picking a new console */
		if (strncmp (&params.other[8], "uart1", 5) == 0)
		    {
		    sysConsoleTty = TTY_UART1;
		    if (params.other[13] == ',')
			{
			(void)sscanf (&params.other[14], "%d", &sysConsoleBaud);
			}
		    }
		else if (strncmp (&params.other[8], "uart2", 5) == 0)
		    {
		    sysConsoleTty = TTY_UART2;
		    if (params.other[13] == ',')
			{
			(void)sscanf (&params.other[14], "%d", &sysConsoleBaud);
			}
		    }
		}
	    }
	}

#ifndef _WRS_CONFIG_SMP
#ifdef DRV_RESOURCE_QORIQFMAN
    /*
     * If we're a data plane image, wait until
     * the control plane comes up. Note that I would
     * prefer to use vbiIdle() here, but apparently that
     * will block indefinitely at this stage since
     * we haven't turned on interrupts yet.
     *
     * The current rule is to test the procNum value
     * specified in the bootline, which comes from the
     * XML for this guest. We assume that processor
     * number 0 is the control plane.
     */

#if (CONTROL_PLANE_CORE != -1)
    if (vxCpuIdGet () != CONTROL_PLANE_CORE)
#else
    if (params.procNum != 0)
#endif
	{
	for (i = 0; i < 10; i++)
	    {
	    if (vbiNsLookup ("vxControl", 0, &vbiNsHandle,
			     VBI_NS_NO_TIMEOUT, VBI_NS_OPTION_NONE) == OK)
		break;
	    b = 100000000;
	    while (b)
		b--;
	    }
	}
#endif /* DRV_RESOURCE_QORIQFMAN */
#endif /* !_WRS_CONFIG_SMP */

    pSysPlbMethods = p4080PlbMethods;
    hardWareInterFaceInit();
    }

/******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.
*
* RETURNS: NA
*
* ERRNO: N/A
*/

void sysHwInit2 (void)
    {
    vbiPhysAddr_t physAddr;
    UINT32 virtAddr;
    volatile UINT32 test;
#if (CONTROL_PLANE_CORE == -1)
    BOOT_PARAMS params;
    char *result = NULL;
#endif

    /* Figure out virt to phys offset */

    virtAddr = (UINT32)&test;
    vbiGuestDmaAddrGet ((vbiGuestPhysAddr_t)virtAddr, &physAddr);
    virtToPhysOff = physAddr - virtAddr;

    vxbDevInit();

# ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
# endif

    /* If we are on core 0, we're the control plane. */

#if (CONTROL_PLANE_CORE != -1)
    if (vxCpuIdGet () == CONTROL_PLANE_CORE)
        {
	vbiNsRegister ("vxControl", 0);
        }
#else
    result = bootStringToStruct (sysBootLine, &params);
    if ((sysBootLine != NULL) && (result != NULL) && (*result == EOS))
	{
	if (params.procNum == 0)
	    vbiNsRegister ("vxControl", 0);
	}
#endif

    taskSpawn ("tDevConn", 11, 0, 10000, vxbDevConnect, 0,1,2,3,4,5,6,7,8,9);

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
    static char * physTop = NULL;

    if (physTop == NULL)
	{
#ifdef  LOCAL_MEM_AUTOSIZE
	physTop = (char *)ROUND_DOWN (VBI_MEM_SIZE_GET(), MMU_PAGE_SIZE);
#else
	physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
#endif
	}

    return physTop;
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
    int startType	/* parameter passed to ROM to tell it how to boot */
    )
    {

#ifdef GUEST_DEBUG
    vbiDebugShellStart (1);
    while (1) vbiIdle(0);
#endif

#ifndef _WRS_CONFIG_SMP
    vbiVbReset (VBI_BOARD_ID_GET(), (vbiCore_t)VBI_VB_CORES_ALL,
		VBI_VBMGMT_RESET_DOWNLOAD | VBI_VBMGMT_RESET_AND_START_CORE0 |
		VBI_VBMGMT_RESET_CLEAR);
#else
    /* For SMP ensure that vbiVbReset gets executed by CPU 0 */

    if (VBI_VCORE_ID_GET() == 0)
	{
	vbiVbReset (VBI_BOARD_ID_GET(), (vbiCore_t)VBI_VB_CORES_ALL,
		    VBI_VBMGMT_RESET_DOWNLOAD |
		    VBI_VBMGMT_RESET_AND_START_CORE0 |
		    VBI_VBMGMT_RESET_CLEAR);
	}
    else
	{
	cpcInvoke (1, (CPC_FUNC)sysToMonitor, (void * )startType,
	    sizeof(startType), VX_CPC_ASYNC);
	}
#endif
    return ERROR;
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
    return sysProcNum;
    }

/******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* This function is not applicable for the bus-less 8260Ads.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: sysProcNumGet()
*
*/

void sysProcNumSet
    (
    int		procNum			/* processor number */
    )
    {
    sysProcNum = procNum;
    }

/******************************************************************************
*
* sysClkFreqGet - get the system clock freq
*
* This function returns the system clock frequency.  This is used for
* Baud rate calculations.
*
* RETURNS: system clock freq
*
* ERRNO: N/A
*/

UINT32 sysClkFreqGet (void)
    {
#ifndef FORCE_DEFAULT_FREQ
    static UINT32 sysClkFreq = 0;

    if (sysClkFreq == 0)
	{
	VBI_BSP_MSG	    requestMsg;
	VBI_BSP_MSG_REPLY   replyMsg;
	VBI_NS_HANDLE	    vbiNsHandle;

	if (vbiNsLookup ("bspServer", 0, &vbiNsHandle,
			 VBI_NS_NO_TIMEOUT, VBI_NS_OPTION_NONE) != OK)
	    {
	    vbiKputs ("sysClkFreqGet: ERROR - can not get bspServer handle\n");
	    return DEFAULT_SYSCLKFREQ;
	    }

	requestMsg.request = VBI_BSP_CLK_FREQ;

	if (vbiSend (vbiNsHandle, &requestMsg, sizeof(requestMsg),
		     &replyMsg, sizeof(replyMsg), NULL, NULL) != OK)
	    {
	    vbiKputs ("sysClkFreqGet: ERROR - vbiSend request failed\n");
	    return DEFAULT_SYSCLKFREQ;
	    }

	sysClkFreq = (UINT32)replyMsg.dataVal;
	}

    if (sysClkFreq != 0)
	return sysClkFreq;
#endif

    return DEFAULT_SYSCLKFREQ;
    }

#ifdef DRV_TIMER_OPENPIC
/*******************************************************************************
*
* sysPicClkFreqGet - return the OpenPIC timer count frequency
*
* Given the selected picClockDivisor value, this routine calculates
* and returns the resulting OpenPIC timer count frequency.
*
* RETURNS: the OpenPIC timer count frequency
*
* ERRNO: N/A
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
#endif /* DRV_TIMER_OPENPIC */

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
* ERROR
*
* RETURNS: ERROR, always
*
*/

SIO_CHAN * bspSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    return (SIO_CHAN *)ERROR;
    }

/*****************************************************************************
*
* sysDeviceFilter - Called from vxbus to prevent device from being
*                   announced and therefore used by vxWorks. The default
*                   is to return OK, allowing device's use.
*
* In the context of a guest Virtual Board, the sysDeviceFilter
* will query the Hypervisor to see if a particular device instance
* is availabe to this VB. This requires the filter "know" and use the
* naming convention used in the Hypervisor XML.
*
* RETURNS: OK or ERROR
*
* \NOMANUAL
*/

LOCAL STATUS sysDeviceFilter
    (
    VXB_DEVICE_ID pDev
    )
    {
    BOOT_PARAMS params;
    VB_CONFIG * pVbiConfig;
    VB_MEM_INFO * pMem;
    HCF_DEVICE * pHcf;
    struct hcfResource * pRes;
    char *result = NULL;
    char picTimer[] = "picTimerX";	/* device's name in Hypervisor's XML */
    char dtsec[] = "dtsecX";		/* device's name in Hypervisor's XML */
    char eth[]   = "ethX_dtsec";	/* device's name in Hypervisor's XML */
    char tgec[]  = "tgecX";		/* device's name in Hypervisor's XML */
    char TenGEC[] = "10GECX";		/* device's name in Hypervisor's XML */
    int coreNum;
    int i;

#ifdef GUEST_DEBUG
    static int debugCount = 0;
#endif

    if (pDev->pName == NULL)
	return OK;

    pVbiConfig = VBI_CONFIG_ADDR_GET();

#ifdef GUEST_DEBUG
    /* first time through print the all the devices mapped to this VB */

    if (debugCount++ == 0)
	{
	_GUEST_DEBUG_PRINT("[VB %d] Start sysDeviceFilter...\n", VBI_BOARD_ID_GET());
	for (i = 0; i < pVbiConfig->numMem; i++)
	    {
	    pMem = &pVbiConfig->memoryRegionsConfigAddress[i];
	    _GUEST_DEBUG_PRINT("\tVB Guest device[%d]: %s\n", i, pMem->name);
	    }
	_GUEST_DEBUG_PRINT("\n", debugCount);
	}
    _GUEST_DEBUG_PRINT("\t%2d:  vxBus device: %s\t unit: %d\n",
		      debugCount, pDev->pName, pDev->unitNumber);
#endif

    /* UART filtering */

    if ((strcmp (pDev->pName, "ns16550") == 0) &&
	(pDev->unitNumber != CONSOLE_TTY))
        {
        _GUEST_DEBUG_PRINT("\t\t  no match (not console) -- %s%d\n",
                           pDev->pName, pDev->unitNumber);
	return ERROR;
        }

    pHcf = hcfDeviceGet (pDev);

    /* 
     * hcfDeviceGet() will return NULL for auto-created devices that are
     * children of other devices (ex., miiBus and genericPhy) or are
     * virtual devices (ex., vnic and stubSio).  These will not be filtered.
     */

    if (pHcf == NULL)
        {
        _GUEST_DEBUG_PRINT("\t\t  pHcf = hcfDeviceGet (0x%0x) is NULL for %s\n",
	                   pDev, pDev->pName);
	return OK;
        }

    pRes = (struct hcfResource *)pHcf->pResource;
    coreNum = vxCpuIdGet ();


    /* openPicTimer filtering */

    if (strcmp (pDev->pName, "openPicTimer") == 0)
	{
	picTimer[8] = 'A' + pDev->unitNumber;

	for (i = 0; i < pVbiConfig->numMem; i++)
	    {
	    pMem = &pVbiConfig->memoryRegionsConfigAddress[i];

	    /* check for device in XML named 'picTimer<group_id>' */

	    if (strstr ((char *)pMem->name, picTimer) != NULL)
		{
		_GUEST_DEBUG_PRINT("\t\t  positive match -- %s%d :: guest[%d] -- %s\n",
				pDev->pName, pDev->unitNumber, i, pMem->name);
		return (OK);
		}
	    }

	/* This timer group is not mapped to this VB so return error.  */

	_GUEST_DEBUG_PRINT("\t\t  no match -- %s%d\n",
			pDev->pName, pDev->unitNumber);
	return (ERROR);
	}


    /* Network/MAC device filtering */

    if (strcmp (pDev->pName, "dtsec") == 0)
	{

	/*
	 * At present there is no devResourceSet() routine. So, use our
	 * knowledge of the hcfResources array in hwconf.c to force setting
	 * of the 'coreNum' resource. Obviously this only works if 'coreNum'
	 * resource is at index 1 in the tgec and dtsec arrays.
	 *
	 * Easiest to unconditionally set the coreNum resource first. There
	 * is no need to confirm the device is actually assigned to this VB
	 * because if it isn't assigned the instance gets disabled anyway.
	 */

	pRes[1].u.integer = coreNum;

	/*
	 * Check to see if we have an explicit memory mapping
	 * for this MAC unit. If so, return success.
	 */

	dtsec[5] = '0' + pDev->unitNumber;
	eth[3]   = '0' + pDev->unitNumber;

	for (i = 0; i < pVbiConfig->numMem; i++)
	    {
	    pMem = &pVbiConfig->memoryRegionsConfigAddress[i];

	    /* check for device in XML named 'dtsec<unit_num>' */

	    if (strstr ((char *)pMem->name, dtsec) != NULL)
		{
		_GUEST_DEBUG_PRINT("\t\t  positive match -- %s%d :: guest[%d] -- %s\n",
				pDev->pName, pDev->unitNumber, i, pMem->name);
		return (OK);
		}

	    /* check for device in XML named 'eth<unit_num>_dtsec' */

	    if (strstr ((char *)pMem->name, eth) != NULL)
		{
		_GUEST_DEBUG_PRINT("\t\t  positive match -- %s%d :: guest[%d] -- %s\n",
				pDev->pName, pDev->unitNumber, i, pMem->name);
		return (OK);
		}
	    }

	/* This MAC unit is not mapped to this VB so return error.  */

	_GUEST_DEBUG_PRINT("\t\t  no match -- %s%d\n",
			pDev->pName, pDev->unitNumber);
	return (ERROR);
	}

    if (strcmp (pDev->pName, "tgec")  == 0)
	{

	/*
	 * At present there is no devResourceSet() routine. So, use our
	 * knowledge of the hcfResources array in hwconf.c to force setting
	 * of the 'coreNum' resource. Obviously this only works if 'coreNum'
	 * resource is at index 1 in the tgec and dtsec arrays.
	 *
	 * Easiest to unconditionally set the coreNum resource first. There
	 * is no need to confirm the device is actually assigned to this VB
	 * because if it isn't assigned the instance gets disabled anyway.
	 */

	pRes[1].u.integer = coreNum;

	/*
	 * Check to see if we have an explicit memory mapping
	 * for this MAC unit. If so, return success.
	 */

	tgec[4]   = '0' + pDev->unitNumber;
	TenGEC[5] = '0' + pDev->unitNumber;

	for (i = 0; i < pVbiConfig->numMem; i++)
	    {
	    pMem = &pVbiConfig->memoryRegionsConfigAddress[i];

	    /* check for device in XML named 'tgec<unit_num>' */

	    if (strstr ((char *)pMem->name, tgec) != NULL)
		{
		_GUEST_DEBUG_PRINT("\t\t  positive match -- %s%d :: guest[%d] -- %s\n",
				pDev->pName, pDev->unitNumber, i, pMem->name);
		return (OK);
		}

	    /* check for device in XML named '10GEC<unit_num>' */

	    if (strstr ((char *)pMem->name, TenGEC) != NULL)
		{
		_GUEST_DEBUG_PRINT("\t\t  positive match -- %s%d :: guest[%d] -- %s\n",
				pDev->pName, pDev->unitNumber, i, pMem->name);
		return (OK);
		}

	    }

	/* This MAC unit is not mapped to this VB so return error.  */

	_GUEST_DEBUG_PRINT("\t\t  no match -- %s%d\n",
			pDev->pName, pDev->unitNumber);
	return (ERROR);
	}


    /* FMAN filtering */

    if (strcmp (pDev->pName, "QorIQFman") == 0)
	{

	/*
	 * Only the control plane -OR- cpu0 should do the fman setup,
	 * all other cores will set the globalInit flag to disabled
	 */

#if (CONTROL_PLANE_CORE != -1)
	if (coreNum != CONTROL_PLANE_CORE)
	    {
	    /*
	     * As with the network MAC's above, the following assignment
	     * relies on us having placed the 'globalInit'
	     * resource at index 1
	     */

    	    _GUEST_DEBUG_PRINT("\t\t  FMAN no setup -- %s%d\n",
			      pDev->pName, pDev->unitNumber);
	    pRes[1].u.integer = 0;
	    }
#else	/* CONTROL_PLANE_CORE */
	result = bootStringToStruct (sysBootLine, &params);
	if ((sysBootLine != NULL) && (result != NULL) && (*result == EOS))
	    {
	    if (params.procNum != 0)
		{
		/*
		 * As with the network MAC's above, the following assignment
		 * relies on us having placed the 'globalInit'
		 * resource at index 1
		 */

    		_GUEST_DEBUG_PRINT("\t\t  FMAN no setup -- %s%d\n",
			      pDev->pName, pDev->unitNumber);
		pRes[1].u.integer = 0;
		}
	    }
#endif	/* CONTROL_PLANE_CORE */

	}

    _GUEST_DEBUG_PRINT("\t\t  default match -- %s%d\n",
			      pDev->pName, pDev->unitNumber);
    return (OK);
    }

/*****************************************************************************
*
* sysFmanClkFreqGet - return the frame manager clock frequency
*
* This function calculates the frame manager clock frequency and
* returns it.
*
* The frame manager clock source is selectable via the reset control word.
* It can either be the platform clock frequency divided by two, or the
* core cluster 3 PLL frequency divided by two. For the P4080DS board,
* the RCW supplied by Freescale uses the platform clock frequency.
*
* RETURNS: frame manager clock in Hz
*
* ERRNO: N/A
*/

LOCAL UINT32 sysFmanClkFreqGet (void)
    {
    return (250000000);
    }

/*****************************************************************************
*
* sysVirtToPhys - perform a virtual to physical address translation
*
* This routine converts a virtual memory address to a 36-bit physical
* address. The virt/phys relationship is derived from the hypervisor
* mapping for this guest instance.
*
* RETURNS: a 36-bit physical address
*
* ERRNO: N/A
*/

LOCAL UINT64 sysVirtToPhys
    (
    void * pVirt
    )
    {
    return (virtToPhysOff + (UINT64)(pVirt));
    }

/*****************************************************************************
*
* sysPhysToVirt - perform a physical to virtual address translation
*
* This routine converts a 36-bit physical address to a virtual memory
* address. The virt/phys relationship is derived from the hypervisor
* mapping for this guest instance.
*
* RETURNS: a 32-bit virtual address
*
* ERRNO: N/A
*/

LOCAL void * sysPhysToVirt
    (
    UINT64 pPhys
    )
    {
    return ((void *)(unsigned long)(pPhys - virtToPhysOff));
    }

/*******************************************************************************
*
* vxCpuIdGet - return the physical CPU number
*
* RETURNS: the physical CPU ID number
*
* ERRNO: N/A
*/

cpuid_t vxCpuIdGet (void)
    {
    return (sysCoreNums[vxCpuIndexGet ()]);
    }

/*******************************************************************************
*
* vxCpuPhysIndexGet - return the physical CPU number
*
* RETURNS: the physical CPU number (logically the same as vxCpuIdGet())
*
* ERRNO: N/A
*/

UINT32 vxCpuPhysIndexGet (void)
    {
    return (sysCoreNums[vxCpuIndexGet ()]);
    }


/*******************************************************************************
*
* vxCpuIdToPhysIndex - return the physical CPU number
*
* RETURNS: physical CPU index
*/

unsigned int vxCpuIdToPhysIndex
    (
    cpuid_t cpuID
    )
    {
    cpuID += vxBaseCpuPhysIndex;
    return (cpuID);
    }

/*******************************************************************************
*
* vxCpuPhysIndexToId - return the physical CPU number
*
* RETURNS: CPU ID
*/

cpuid_t vxCpuPhysIndexToId
    (
    unsigned int cpuID
    )
    {
    cpuID -= vxBaseCpuPhysIndex;
    return (cpuID);
    }

/*******************************************************************************
*
* vxCpuLibInit - init to support CPU identification and index functions
*
* RETURNS: N/A
*/

void vxCpuLibInit (void)
    {
    VB_CONFIG * pVbiConfig;
    int baseCore;
    int i;

    /*
     * Figure out the physical core ID numbers.
     * We obtain the base core ID number by way of
     * a hypercall, and extrapolate the others
     * based on the number of CPUs allocated to
     * this guest. (The assumption is that they will
     * all be assigned contiguously.)
     */

    baseCore = sysV2pCpuNumGet (0);
    pVbiConfig = VBI_CONFIG_ADDR_GET();
    for (i = 0; i < pVbiConfig->numCores; i++)
	sysCoreNums[i] = baseCore + i;

    vxBaseCpuPhysIndex = baseCore;

    return;
    }

/******************************************************************************
*
* sysV2pCpuNumGet - get the physical CPU ID number
*
* This function returns the physical CPU ID number via HV's bspServer
*
* RETURNS: physical CPU ID number
*
* ERRNO: N/A
*/

LOCAL UINT32 sysV2pCpuNumGet
    (
    UINT32 vcoreNum
    )
    {
    VBI_NS_HANDLE	vbiNsHandle;
    VBI_BSP_MSG		requestMsg;
    VBI_BSP_MSG_REPLY	replyMsg;

    if (vbiNsLookup ("bspServer", 0, &vbiNsHandle,
		     VBI_NS_NO_TIMEOUT, VBI_NS_OPTION_NONE) != OK)
	{
	vbiKputs ("sysV2pCpuNumGet: ERROR - can not get bspServer handle\n");
	return 0;
	}

    requestMsg.request = VIRT_TO_PHYS_CPU_NUM;
    requestMsg.arg.v2pCpuNum.vcore = vcoreNum;

    if (vbiSend (vbiNsHandle, &requestMsg, sizeof(requestMsg),
		 &replyMsg, sizeof(replyMsg), NULL, NULL) != OK)
	{
	vbiKputs ("sysV2pCpuNumGet: ERROR - vbiSend request failed\n");
	return 0;
	}

    return (UINT32)replyMsg.dataVal;
    }

/******************************************************************************
*
* sysCpuAvailableGet - return the number of CPUs available for SMP
*
* This function returns the number of VCOREs configured for this virtual
* board.
*
*/

UINT32 sysCpuAvailableGet (void)
    {
    return ((UINT32) VBI_VCORES_COUNT_GET());
    }

#ifdef _WRS_CONFIG_SMP
/******************************************************************************
*
* sysCpuInit - initialize additional SMP cores
*
* RETURNS: NONE
*
*/

void sysCpuInit(void)
    {
    STATUS	status;
    int		vec;

    status = vbiExcBaseSet((void*) VEC_BASE_ADRS);

    mmuE500TlbStaticInit (sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    mmuPpcPidSet (MMU_ASID_GLOBAL);

    vec = vbiIntVecFind ("ipi0", VB_INPUT_INT);
    vbiVioapicIntUnmask (vec);

    vec = vbiIntVecFind ("ipi2", VB_INPUT_INT);
    vbiVioapicIntUnmask (vec);
    }

/******************************************************************************
*
* sysCpuLoop - CPU loop
*
* This routine is the CPU idle loop function which will invoke the startup
* function specified in the sysCpuEnable function.
*
* RETURNS: NONE
*
*/

void sysCpuLoop(void)
    {
    unsigned int a = 0;
    FUNCPTR p;
    int cpu;

    /*
     * The call to vxSdaInit() must be the first operation here.
     * This is because vxSdaInit() sets the r2 and r13 registers to the
     * SDA and SDA2 base registers. No C code must be placed before this
     * call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in sysCpuLoop() */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

    sysCpuInit();

    cpu = VBI_VCORE_ID_GET();

    p = (FUNCPTR)(*(UINT32*)CPUn_FUNC_START_ADR(cpu));

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
    unsigned int  cpuNum,
    WIND_CPU_STATE * pCpuState
    )
    {
    STATUS status = OK;

    if (cpuNum == 0)
    {
    return ERROR;
    }

    /*
     * The cpuNum passed in by the kernel is a physical index. Convert it
     * to a virtual index for vbiVbResume.
     */

    cpuNum = vxCpuPhysIndexToId (cpuNum);

    *((int *)CPUn_FUNC_START_ADR(cpuNum)) = (int)pCpuState->regs.pc;
    *((int *)CPUn_STACK_START_ADR(cpuNum)) = (int)pCpuState->regs.spReg;
    *((int *)CPUn_INIT_START_ADR(cpuNum)) = (int)sysCpuLoop;
    *((int *)CPUn_LOCAL_MEM_ERPN(cpuNum)) = (int)0;

    status = vbiVbResume (VBI_BOARD_ID_GET(), cpuNum);

    return status;
    }
#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* sysMmuMapAdd - insert a new MMU mapping
*
* This routine will create a new <sysPhysMemDesc> table entry for a memory
* region of specified <length> in bytes and with a specified base
* <address>.  The <initialStateMask> and <initialState> parameters specify
* a PHYS_MEM_DESC type state mask and state for the memory region.
*
* CAVEATS
* This routine must be used before the <sysPhysMemDesc> table is
* referenced for the purpose of initializing the MMU or processor address
* space (us. in usrMmuInit()).
*
* The <length> in bytes will be rounded up to a multiple of VM_PAGE_SIZE
* bytes if necessary.
*
* The current implementation assumes a one-to-one mapping of physical to
* virtual addresses.
*
* If the current memory region contains some pages which had been mapped
* with different attributes previously, this routine will do nothing and
* return an ERROR directly.
*
*
* RETURNS: OK or ERROR depending on availability of free mappings.
*
* SEE ALSO: vmLib
*/

LOCAL STATUS sysMmuMapAdd
    (
    void * address,           /* memory region base address */
    UINT   length,            /* memory region length in bytes*/
    UINT   initialStateMask,  /* PHYS_MEM_DESC state mask */
    UINT   initialState       /* PHYS_MEM_DESC state */
    )
    {
    PHYS_MEM_DESC * pMmu;
    PHYS_MEM_DESC * pPmd;

    UINT32   pageAddress;
    UINT32   pageLength;
    UINT32   pageAddressTop = 0;
    UINT32   pageLengthTop = 0;
    UINT32   pageAddressBottom = 0;
    UINT32   pageLengthBottom = 0;

    UINT32   pageStart;
    UINT32   pageEnd;
    UINT32   pageCount;

    UINT32   mapStart;
    UINT32   mapEnd;

    BOOL     pageNotMapped = FALSE;
    BOOL     noOverlap = FALSE;
    BOOL     topOverlap = FALSE;
    BOOL     bottomOverlap = FALSE;

    UINT32   i;

    STATUS   result = OK;

    mapStart = 0;
    mapEnd   = 0;

    /* Calculate(align) the start/end of page address, and the count of pages */

    pageLength = ROUND_UP (length, VM_PAGE_SIZE);

    pageStart = ROUND_DOWN (address, VM_PAGE_SIZE);
    pageEnd   = ((pageStart + (pageLength - 1)) / VM_PAGE_SIZE) * VM_PAGE_SIZE;
    pageCount = (pageEnd - pageStart) / VM_PAGE_SIZE + 1;

    pageAddress  = pageStart;

    for (i=0; i<sysPhysMemDescNumEnt; i++)
	{
	pageNotMapped = TRUE;
	noOverlap = TRUE;
	topOverlap = FALSE;
	bottomOverlap = FALSE;

	pPmd = &sysPhysMemDesc[i];

	mapStart = (UINT32) pPmd->virtualAddr;
	mapEnd   = (UINT32) pPmd->virtualAddr + pPmd->len - VM_PAGE_SIZE;

	if(pageStart >= mapStart && pageEnd <= mapEnd)
	    {
	    /* Mapping fully contained */

	    if (pPmd->initialStateMask != initialStateMask  ||
		pPmd->initialState != initialState  ||
		pPmd->virtualAddr != pPmd->physicalAddr)
		{
		result = ERROR; /* Attributes are different */
		break;
		}

	    noOverlap = FALSE;
	    pageNotMapped = FALSE;
	    break;
	    }

	if((pageStart >= mapStart) && (pageEnd > mapEnd) && (pageStart < mapEnd))
	    {
	    /* Top Overlap */

	    if (pPmd->initialStateMask != initialStateMask  ||
		pPmd->initialState != initialState  ||
		pPmd->virtualAddr != pPmd->physicalAddr)
		{
		result = ERROR; /* Attributes are different */
		break;
		}

	    /* Need to add entry for (pageEnd-mapEnd) */

	    topOverlap = TRUE;
	    pageAddressTop = (mapEnd + VM_PAGE_SIZE);
	    pageLengthTop = (pageEnd - mapEnd);
	    }

	if((pageStart < mapStart) && (pageEnd <= mapEnd) && (pageEnd > mapStart))
	    {
	    /* Bottom Overlap */

	    if (pPmd->initialStateMask != initialStateMask  ||
		pPmd->initialState != initialState  ||
		pPmd->virtualAddr != pPmd->physicalAddr)
		{
		result = ERROR; /* Attributes are different */
		break;
		}

	    /* Need to add entry for (mapStart-pageStart) */

	    bottomOverlap = TRUE;
	    pageAddressBottom = pageStart;
	    pageLengthBottom = (mapStart-pageStart);
	    }

	if(topOverlap || bottomOverlap)
	    break;

	}

    if (pageNotMapped && (result != ERROR))
	{
	    pMmu = &sysPhysMemDesc[sysPhysMemDescNumEnt];
	    if (pMmu->virtualAddr != (VIRT_ADDR) DUMMY_VIRT_ADDR)
		{
		result = ERROR;
	    }
	else
	    {
	    if(noOverlap)
		{
		pMmu->virtualAddr       = (VIRT_ADDR)pageAddress;
		pMmu->physicalAddr      = (PHYS_ADDR)pageAddress;
		pMmu->len               = pageLength;
	    pMmu->initialStateMask  = initialStateMask;
	    pMmu->initialState      = initialState;
	    sysPhysMemDescNumEnt   += 1;
		}

	    if(topOverlap)
		{
		pMmu->virtualAddr       = (VIRT_ADDR)pageAddressTop;
		pMmu->physicalAddr      = (PHYS_ADDR)pageAddressTop;
		pMmu->len               = pageLengthTop;
		pMmu->initialStateMask  = initialStateMask;
		pMmu->initialState      = initialState;
		sysPhysMemDescNumEnt    += 1;
		}

	    if(bottomOverlap)
		{
		pMmu->virtualAddr       = (VIRT_ADDR)pageAddressBottom;
		pMmu->physicalAddr      = (PHYS_ADDR)pageAddressBottom;
		pMmu->len               = pageLengthBottom;
		pMmu->initialStateMask  = initialStateMask;
		pMmu->initialState      = initialState;
		sysPhysMemDescNumEnt    += 1;
		}
	    }
	}

    return (result);
    }
