/* sysLib.c - wrVbP2020rdb system-dependent library */

/*
 * Copyright (c) 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,27aug13,j_b  CQ:WIND00432161 - fix virtual device filtering
01d,08aug13,j_b  CQ:WIND00429511 - add NULL pointer check in sysDeviceFilter;
                 fix unchecked sscanf() return value
01c,29mar11,agf  add SPE (hardware fp) support
01b,23mar11,agf  change arguments to vbiNsLookup to match Hv 1.3 API
01a,15feb11,agf  created based on wrVbP4080 rev 01p
*/

/*
DESCRIPTION
This library provides board-specific routines for the WR P2020-RDB Virtual Board.

Note: conditional compile logic for SMP has been left in place to assist with
future migration should a P2040 or P2080 part be released, however for the 
current P2020 part, SMP is NOT supported. A compiler error will be generated
if the BSP is attempted to be built configured for SMP.

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
#include "config.h"

#ifdef _WRS_CONFIG_SMP
#error This BSP does NOT currently support SMP. This code is in place only \
       to assist with future development/porting.
#   include <private/cpcLibP.h>
#endif

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#   include <drv/pci/pciAutoConfigLib.h>
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SPE
#   include <speLib.h>
#endif  /* INCLUDE_SPE */


/* defines */

#undef WRLOAD_DEBUG
#ifdef WRLOAD_DEBUG
#define _WRLOAD(arg) vbiKputs(arg)
#define _WRLOAD_PRINT(fmt,...) {char logBuf[0x100]; sprintf(logBuf,fmt,__VA_ARGS__); vbiKputs(logBuf);}
#else
#define _WRLOAD(arg)
#define _WRLOAD_PRINT(fmt,...)
#endif


/* globals */

#if 1
/*
 * The following variables are now declared in vbiLib.c. However, under
 * VBI VERSION 1 these variables were declared locally in the BSP. To
 * provide backwards compatibility for the older BSPs, they were excluded
 * from vbiLib.c for the E500 variant, which the P2020rdb happens to be.
 * It is expected in the next (vxW6.9.1) release the legacy BSPs will be 
 * reworked to remove the wrhvVbXXXX declarations from the BSPs and rely 
 * solely on the ones in vbiLib.c. At that time, this entire block of code
 * should be deleted.
 *
 * IMPORTANT,
 * leave set to -1 so not in BSS; probably want in persistent space
 */

VB_CONFIG  * wrhvVbConfig  = (void *)-1;
VB_STATUS  * wrhvVbStatus  = (void *)-1;
VB_CONTROL * wrhvVbControl = (void *)-1;
int32_t    wrhvVbIntNested = 0;

/*
 * Similarly, these variables are not used in this BSP, but since we are
 * using the shared sysALib.s, we have to declare them here. It is expected
 * this will be cleaned up in the next (vxW6.9.1) release.
 */
int wrhvBootType = -1;
int wrhvInSimics = -1;

#endif


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
int	sysCpu      = CPU;		/* system CPU type (PPC32) */
#ifdef _WRS_CONFIG_SMP
UINT32	sysCoreNums[VX_MAX_SMP_CPUS];
#endif /* _WRS_CONFIG_SMP */

void * sysSystemGlobals = (void *) LOCAL_MEM_LOCAL_ADRS;
UINT32 sysSystemGlobalsSize = 0x100;

UINT32	ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32	ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32	ppcE500CACHE_ALIGN_SIZE = 32;

#ifdef INCLUDE_SPE
int sysSpeProbe (void);
#endif  /* INCLUDE_SPE */


/* locals */

#ifdef _WRS_CONFIG_SMP
LOCAL UINT32 tbHI;	/* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;	/* lower 32 bit value of timebase */
volatile UINT32 sysCpu1LoopCount = 0;

LOCAL UINT32 sysV2pCpuNumGet (UINT32 vcoreNum);
#endif /* _WRS_CONFIG_SMP */

LOCAL STATUS sysDeviceFilter (VXB_DEVICE_ID pDev);
LOCAL UINT64 sysVirtToPhys (void *);
LOCAL void * sysPhysToVirt (UINT64);

LOCAL UINT64 virtToPhysOff;

METHOD_DECL(sysBspDevFilter);
DEVMETHOD_DEF(sysVirtToPhys, "BSP virtual to physical address translation");
DEVMETHOD_DEF(sysPhysToVirt, "BSP physical to virtual address translation");

LOCAL struct vxbDeviceMethod p2020rdbPlbMethods[] =
    {
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
    DEVMETHOD(sysVirtToPhys, sysVirtToPhys),
    DEVMETHOD(sysPhysToVirt, sysPhysToVirt),
    { 0, 0 }
    };


/* externals */

IMPORT device_method_t * pSysPlbMethods;

IMPORT void hardWareInterFaceInit (void);
IMPORT UINT32 sysWrhvCoreIdGet (void);

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif

#ifdef INCLUDE_SPE
IMPORT int (* _func_speProbeRtn) ();
#endif  /* INCLUDE_SPE */


/* source files */

#include "hwconf.c"

#ifdef INCLUDE_FLASH
#    include "flashMem.c"
#    include <mem/nvRamToFlash.c>
#else
#    include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */

#ifdef INCLUDE_ETSEC_VXB_END
#include "sysNet.c"
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
* sysBspRev - return the BSP version with the revision eg 2.0/<x>
*
* This function returns a pointer to a BSP version with the revision.
* for eg. 2.0/<x>. BSP_REV defined in config.h is concatenated to
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
    char *result = NULL;
    int32_t vector = 0;


    interruptConversionTableSetup();

    /* set the boot line */

    /*
     * only copy bootline from hypervisor config if it has not been set by
     * wrload.
     */

    _WRLOAD_PRINT("[VB %d] sysBootLine (%p): %s\n"
                 "[VB %d] sysBootLineArray (%p): %s\n"
                 "[VB %d] BOOT_LINE_ADRS (%p): %s\n",
                 VBI_BOARD_ID_GET(), sysBootLine, sysBootLine,
                 VBI_BOARD_ID_GET(), sysBootLineArray, sysBootLineArray,
                 VBI_BOARD_ID_GET(), BOOT_LINE_ADRS, BOOT_LINE_ADRS);

    if (sysBootLine[0] == EOS)
        {
         _WRLOAD_PRINT("[VB %d] Getting bootline from hypervisor config\n",
                      VBI_BOARD_ID_GET());
        strncpy(sysBootLine, VBI_BOOTLINE_ADDR_GET(), VB_MAX_BOOTLINE_LENGTH);
        }
    else
        {
        _WRLOAD_PRINT("[VB %d] Bootline left as is\n", VBI_BOARD_ID_GET());
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

    _WRLOAD_PRINT("[VB %d] Bootline contents at BOOT_LINE_ADRS: %s\n",
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

    #if 0
    /*
     * the following line is disabled for now, it will become useful later
     * should we decide to use the device filter functionality for assigning
     * devices to guest VBI's at runtime
     * 
     * guest vxWorks on P4080ds implements this design for the FMAN interface,
     * on the p2020rdb we could use it for the uarts and etsecs
     */

    pSysPlbMethods = p2020rdbPlbMethods;
    #endif

    hardWareInterFaceInit();
    _WRLOAD_PRINT("[VB %d] finished hardWareInterFaceInit \n",
                  VBI_BOARD_ID_GET()  );
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

    vxbDevInit();

# ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
# endif

# ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
# endif

# ifdef INCLUDE_SPE
    _func_speProbeRtn = sysSpeProbe;
# endif

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

    vbiKputs ("sysToMonitor entered, halting VxWorks....\n");
    vbiKputs ("...................................\n\n\n\n");

#ifdef _WRS_CONFIG_SMP
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

        FOREVER;
	}
#endif /* _WRS_CONFIG_SMP */

    /*
     * For development and debugging, it is handy to bring-up the Hypervisor 
     * shell to diagnose the cause of an unexpected reset.
     * 
     * For a deployed system Hypervisor can hard-reset the virtual board,
     * which will automatically restart vxWorks.
     */

#ifdef DEBUG
    vbiDebugShellStart(1);
    while (1) vbiIdle(0);
#else
    vbiVbReset (VBI_BOARD_ID_GET(), (vbiCore_t)VBI_VB_CORES_ALL,
                VBI_VBMGMT_RESET_DOWNLOAD | VBI_VBMGMT_RESET_AND_START_CORE0 |
	        VBI_VBMGMT_RESET_CLEAR);
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

#ifdef FORCE_DEFAULT_FREQ
    return DEFAULT_SYSCLKFREQ;

#else
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
    else
	return DEFAULT_SYSCLKFREQ;

#endif /* FORCE_DEFAULT_FREQ */
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
*                   is to return OK.
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
    char etsec[] = "motetsecX";
    int coreNum;
    int i;

    if (pDev->pName == NULL)
	return OK;

    if ((strcmp (pDev->pName, "ns16550") == 0) &&
	(pDev->unitNumber != CONSOLE_TTY))
	return ERROR;

    pHcf = hcfDeviceGet (pDev);

    /* 
     * hcfDeviceGet() will return NULL for auto-created devices that are
     * children of other devices (ex., miiBus and genericPhy) or are
     * virtual devices (ex., vnic and stubSio).  These will not be filtered.
     */

    if (pHcf == NULL)
	return OK;

    pRes = (struct hcfResource *)pHcf->pResource;
    pVbiConfig = VBI_CONFIG_ADDR_GET();
    pMem = pVbiConfig->memoryRegionsConfigAddress;
    coreNum = vxCpuIdGet ();

    if (strcmp (pDev->pName, "motetsec") == 0)
        {
        pRes[1].u.integer = coreNum;

        /*
         * Check to see if we have an explicit memory mapping
         * for this ETSEC unit. If so, return success.
         */

        etsec[8] = '0' + pDev->unitNumber;
        for (i = 0; i < pVbiConfig->numMem; i++)
            {
            pMem = &pVbiConfig->memoryRegionsConfigAddress[i];
            if (strcmp ((char *)pMem->name, etsec) == 0)
                return (OK);
            }

        /*
         * If there wasn't an explicit mapping, check the boot line.
         * If the device was specified there, we can also enable it,
         * otherwise we bail.
         */

        result = bootStringToStruct (sysBootLine, &params);
        if ((sysBootLine != NULL) && (result != NULL) && (*result == EOS))
            {
            if (pDev->unitNumber != params.unitNum)
                return (ERROR);
            }
        }

    return (OK);
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
    return ((void *)(pPhys - virtToPhysOff));
    }


#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciEx1IntRoute [4][4] = {
    { PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL },
    { PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL },
    { PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL },
    { PCIEX1_XINT4_LVL, PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL }
};

LOCAL UCHAR sysPciEx2IntRoute [4][4] = {
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL },
    { PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL },
    { PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL },
    { PCIEX2_XINT4_LVL, PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL }
};

/*******************************************************************************
*
* sysPciEx1AutoConfigInclude - PCI Express 1 autoconfig support routine
*
* This routine performs the PCI Express 1 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx1AutoConfigInclude
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UINT            devVend     /* deviceID/vendorID of device */
    )
    {
    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* sysPciEx1AutoConfigIntAssign - PCI Express 1 autoconfig support routine
*
* This routine peforms the PCI Express 1 auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO: N/A
*/

UCHAR sysPciEx1AutoConfigIntAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciEx1IntRoute[(pLoc->device) & 0x3][pin - 1];
    }

/*******************************************************************************
*
* sysPciEx2AutoConfigInclude - PCI Express 2 autoconfig support routine
*
* This routine performs the PCI Express 2 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx2AutoConfigInclude
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UINT            devVend     /* deviceID/vendorID of device */
    )
    {
    if (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* sysPciEx2AutoConfigIntAssign - PCI Express 2 autoconfig support routine
*
* This routine peforms the PCI Express 2 auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO: N/A
*/

UCHAR sysPciEx2AutoConfigIntAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciEx2IntRoute[(pLoc->device) & 0x3][pin - 1];
    }

#endif  /* INCLUDE_PCI_BUS */


#ifdef INCLUDE_SPE
/*******************************************************************************
* 
* sysSpeProbe - Check if the CPU has SPE unit
*
* This routine returns OK if the CPU has an SPE unit in it.
* Presently it assumes available.
*   
* RETURNS: OK
*   
* ERRNO: N/A
*/
    
int sysSpeProbe (void)
    {
    return OK;
    }
#endif  /* INCLUDE_SPE */


#ifdef _WRS_CONFIG_SMP
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
