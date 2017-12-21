/* sysLib.c - Freescale P2020DS board system-dependent library */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,22apr09,syt  derived from version 01q of ads8572/sysLib.c
*/

/*
DESCRIPTION
This library provides board-specific routines for P2020DS

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
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
#include <hwif/vxbus/vxBus.h>



#ifdef _WRS_CONFIG_SMP
#include <private/cpcLibP.h>
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_PCI_BUS
#   include <drv/pci/pciConfigLib.h>
#   include <drv/pci/pciIntLib.h>
#   include "mot85xxPci.h"
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SPE
#   include <speLib.h>
#endif  /* INCLUDE_SPE */

#include "p2020.h"


#include "sysCfg.c"


#include <iprip.h>
#include <ipftps.h>    /*用于sysFtpAuth函数*/


/* globals */

TLB_ENTRY_DESC sysStaticTlbDesc [] =
{
    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash */

    /* needed be first entry here */

    {
    FLASH_BASE_ADRS, 0x0, FLASH_BASE_ADRS,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M  | _MMU_TLB_IPROT   |
    _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G
    },

	/*
	 * LOCAL MEMORY needed be second entry here  -
	 * one TLB would be 256MB so use to get required 512MB
	 */
#if 1     
	{
	LOCAL_MEM_LOCAL_ADRS, 0x0, LOCAL_MEM_LOCAL_ADRS,
	_MMU_TLB_TS_0	| _MMU_TLB_SZ_1G	  | _MMU_TLB_PERM_W  |
	_MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT |
	_MMU_TLB_ATTR_M
	},
#else    /*default*/
	{
	LOCAL_MEM_LOCAL_ADRS, 0x0, LOCAL_MEM_LOCAL_ADRS,
	_MMU_TLB_TS_0	| _MMU_TLB_SZ_256M	  | _MMU_TLB_PERM_W  |
	_MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT |
	_MMU_TLB_ATTR_M
	},
	{
	0x10000000, 0x0, 0x10000000,
	_MMU_TLB_TS_0	| _MMU_TLB_SZ_256M	  | _MMU_TLB_PERM_W  |
	_MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT |
	_MMU_TLB_ATTR_M
	},

#endif

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W   | _MMU_TLB_IPROT
    },

    /*add device space cfg */
#ifndef COMPILE_BOOTROM	
    {
    RAPIDIO1_ADRS, 0x0, RAPIDIO1_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
    _MMU_TLB_ATTR_I | _MMU_TLB_PERM_W | _MMU_TLB_ATTR_G
    },

    {
    RAPIDIO2_ADRS, 0x0, RAPIDIO2_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
    _MMU_TLB_ATTR_I | _MMU_TLB_PERM_W | _MMU_TLB_ATTR_G
    },

    {
    RAPIDIO1_MAIN_ADRS, 0x0, RAPIDIO1_MAIN_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
    _MMU_TLB_ATTR_I | _MMU_TLB_PERM_W | _MMU_TLB_ATTR_G
    },

    {
    RAPIDIO2_MAIN_ADRS, 0x0, RAPIDIO2_MAIN_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
    _MMU_TLB_ATTR_I | _MMU_TLB_PERM_W | _MMU_TLB_ATTR_G
    },

    /*CS1 到K7*/
    {
    FPGA_K7_ADRS, 0x0, FPGA_K7_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_64M |
    _MMU_TLB_ATTR_I | _MMU_TLB_PERM_W | _MMU_TLB_ATTR_G
    },

    /*CS3 到CAN*/
    {
    V5_INTERRAM_ADRS, 0x0, V5_INTERRAM_ADRS, _MMU_TLB_TS_0 | _MMU_TLB_SZ_4M |
    _MMU_TLB_ATTR_I | _MMU_TLB_PERM_W | _MMU_TLB_ATTR_G
    },
#endif
	
#ifdef INCLUDE_L2_SRAM
    {
    L2SRAM_ADDR, 0x0, L2SRAM_ADDR,
    _MMU_TLB_TS_0   | _MMU_TLB_SZ_256K  | _MMU_TLB_PERM_W  |
    _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I   | _MMU_TLB_ATTR_G
    },
#endif /* INCLUDE_L2_SRAM */

   /* All these are not protected */

    {
    CCSBAR, 0x0, CCSBAR,
    _MMU_TLB_TS_1   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
    _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
    }
};

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

#ifdef MMU_ASID_MAX     /* Base 6 MMU library in effect */

/* macro to compose 64-bit PHYS_ADDRs */

# define PHYS_64BIT_ADDR(h, l)  (((PHYS_ADDR)(h) << 32) + (l))
#endif  /* MMU_ASID_MAX */

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
* Total mapped memory       Page Table size
* -------------------       ---------------
*        8 Meg               64 K
*       16 Meg              128 K
*       32 Meg              256 K
*       64 Meg              512 K
*      128 Meg                1 Meg
*   .               .
*   .               .
*   .               .
*
* [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
*
*/

PHYS_MEM_DESC sysPhysMemDesc [] =
{
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
    MMU_ATTR_CACHE_COHERENCY
    },

#ifdef INCLUDE_L2_SRAM
    {
    (VIRT_ADDR) L2SRAM_ADDR,
    (PHYS_ADDR) L2SRAM_ADDR,
    L2SRAM_WINDOW_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
    },
#endif /* INCLUDE_L2_SRAM */

    {
    (VIRT_ADDR) CCSR_BASE,
    (PHYS_ADDR) CCSR_BASE,
    CCSR_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

#ifndef COMPILE_BOOTROM	

   {	 
    (VIRT_ADDR) (RAPIDIO1_ADRS),
    (PHYS_ADDR) (RAPIDIO1_ADRS),
    RAPIDIO1_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID  | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
    MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    },  
   
    {	
     (VIRT_ADDR) (RAPIDIO2_ADRS),
     (PHYS_ADDR) (RAPIDIO2_ADRS),
     RAPIDIO2_SIZE,
     MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
     MMU_ATTR_VALID 	| MMU_ATTR_SUP_RWX	| MMU_ATTR_CACHE_OFF | \
     MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    },

    {	 
     (VIRT_ADDR) (RAPIDIO1_MAIN_ADRS),
     (PHYS_ADDR) (RAPIDIO1_MAIN_ADRS),
     RAPIDIO1_MAIN_SIZE,
     MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
     MMU_ATTR_VALID	 | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
     MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    },	 

    {	  
     (VIRT_ADDR) (RAPIDIO2_MAIN_ADRS),
     (PHYS_ADDR) (RAPIDIO2_MAIN_ADRS),
     RAPIDIO2_MAIN_SIZE,
     MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
     MMU_ATTR_VALID  | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
     MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    },   

    /*PPC经V5到K7*/
    {	  
     (VIRT_ADDR) (FPGA_K7_ADRS),
     (PHYS_ADDR) (FPGA_K7_ADRS),
     FPGA_K7_SIZE,
     MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
     MMU_ATTR_VALID  | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
     MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    },

    /*PPC经V5到CAN*/
    {	   
     (VIRT_ADDR) (V5_INTERRAM_ADRS),
     (PHYS_ADDR) (V5_INTERRAM_ADRS),
     V5_INTERRAM_SIZE,
     MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
     MMU_ATTR_VALID  | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
     MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    },
#endif

    {
    (VIRT_ADDR) FLASH_BASE_ADRS,
    (PHYS_ADDR) FLASH_BASE_ADRS,
    FLASH_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
    MMU_ATTR_CACHE_COHERENCY | MMU_ATTR_CACHE_GUARDED
    },

};

int sysPhysMemDescNumEnt = NELEMENTS(sysPhysMemDesc);

int     sysBus      = BUS_TYPE_NONE;    /* system bus type */
int     sysCpu      = CPU;              /* system CPU type */
char *  sysBootLine = BOOT_LINE_ADRS;   /* address of boot line */
char *  sysExcMsg   = EXC_MSG_ADRS;     /* catastrophic message area */
int     sysProcNum  = 0;                /* processor number of this CPU */

UINT32 inFullVxWorksImage = FALSE;

BOOL    sysVmeEnable = FALSE;           /* by default no VME */
UINT32  coreFreq;
UINT32  core1Freq;
UINT32  ddrFreq;
UINT32  systemFreq;

UINT32  sysClkFreqGet(void);
UINT32  ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;

#ifdef _WRS_CONFIG_SMP
#   if defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;                      /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;                      /* lower 32 bit value of timebase */
#   endif   /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
#endif /* _WRS_CONFIG_SMP */

/* Clock Ratio Tables */

#define MAX_VALUE_DDR_RATIO     16
UINT32 ddrRatioTable[MAX_VALUE_DDR_RATIO] = {
    0,  0,  0,  3,  4,  0xffffffff, 6,
    0,  8,  0,  10, 0,  12, 0,  14, 0
};

#define MAX_VALUE_PLAT_RATIO    32
UINT32 platRatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 3, 0 },
    { 4,  0 }, { 5, 0 }, { 6,  0 }, { 0, 0 },
    { 8,  0 }, { 9, 0 }, { 10, 0 }, { 0, 0 },
    { 12, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 20, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 }
};

#define MAX_VALUE_E500_RATIO    10
UINT32 e500RatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 3, 1 }, { 2, 0 },
    { 5, 1 }, { 3, 0 }, { 7, 1 }, { 4, 0 }, { 9, 1 }
};

/* externs */

IMPORT void mmuE500TlbDynamicInvalidate();
IMPORT void mmuE500TlbStaticInvalidate();
IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, BOOL cacheAllow);
IMPORT BOOL mmuPpcIEnabled;
IMPORT BOOL mmuPpcDEnabled;
IMPORT void sysIvprSet(UINT32);
IMPORT device_method_t * pSysPlbMethods;

IMPORT BOOL e500_spe_exc_enable;
IMPORT BOOL e500_spe_exc_mask;

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

IMPORT void vxSdaInit (void);

#ifdef INCLUDE_BRANCH_PREDICTION
IMPORT void vxEnableBP();
IMPORT void vxDisableBP();
#endif /* INCLUDE_BRANCH_PREDICTION */

#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheArchDisableFromMmu (CACHE_TYPE cache);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
IMPORT STATUS vxbIntToCpuRoute (unsigned int cpu);
#endif  /* _WRS_CONFIG_SMP */

#ifdef  INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#ifdef  INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif   /* INCLUDE_SIO_UTILS */

#ifdef  _WRS_CONFIG_SMP
IMPORT void sysCpu1Start (void);
IMPORT void sysCpu1Stop (void);
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_SPE
IMPORT int  (* _func_speProbeRtn) ();
#endif /* INCLUDE_SPE */

IMPORT void sysL1Csr1Set(UINT32);
IMPORT UINT sysTimeBaseLGet(void);

/* forward declarations */

VOID    sysUsDelay          (int);
VOID    sysMsDelay          (UINT);
STATUS  sysIntEnablePIC     (int intNum);   /* Enable i8259 or EPIC */
STATUS  sysCascadeIntEnable (int intNum);
STATUS  sysCascadeIntDisable(int intNum);

#ifdef  _WRS_CONFIG_SMP
LOCAL   void sysCpu1ShutdownLoop (void);
LOCAL   UINT32 physCpuAvailableGet (void);
#   if (defined(DRV_TIMER_M85XX) && defined(INCLUDE_TIMESTAMP))
STATUS  sysSmpTimeBaseSync (void);
#   endif   /* DRV_TIMER_DEC_PPC && INCLUDE_SYS_TIMESTAMP */
#endif  /* _WRS_CONFIG_SMP */

LOCAL BOOL sysCpu1Check (void);

#ifdef INCLUDE_CACHE_SUPPORT
LOCAL   void sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_L2_SRAM
LOCAL void sysL2SramEnable(BOOL both);
#endif  /* INCLUDE_L2_SRAM */

LOCAL struct vxbDeviceMethod p2020PlbMethods[] =
    {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysCpu1Check),
    { 0, 0 }
    };

UINT32 sysTimerClkFreq;
LOCAL char * physTop = NULL;
LOCAL char * memTop  = NULL;

#ifdef IEEE754_FIX
LOCAL SPE_CONTEXT *speCtx;
#endif  /* IEEE754_FIX */

/* includes */

#include "sysL2Cache.c"

#ifdef INCLUDE_L1_IPARITY_HDLR
#include "sysL1ICacheParity.c"
#endif  /* INCLUDE_L1_IPARITY_HDLR */


#ifdef INCLUDE_FLASH
#include "flashMem.c"
#include "nvRamToFlash.c"
#else  /* INCLUDE_FLASH */
#include <mem/nullNvRam.c>
#endif  /* INCLUDE_FLASH */

#ifdef INCLUDE_NETWORK
#include "sysNet.c"
#endif  /* INCLUDE_NETWORK */

#include "hwconf.c"

/*#include "sysGpio.c"*/

/* defines */

#define SYS_MODEL_P2020E        "Freescale P2020E - Security Engine"
#define SYS_MODEL_P2020         "Freescale P2020"
#define SYS_MODEL               "Freescale P2020"

#define SYS_MODEL_E500          "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN       "Freescale Unknown processor"

#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000


/**/
int ns16550Pollprintf(char* pchar);
int ns16550PollOutputInit();
int ns16550PollOutputchar(char outChar);
INT32 uartMPCPrintHEX(UINT32  iData);


/* some useful includes */
#define INCLUDE_SHELL           /* interactive c-expression interpreter */
#define INCLUDE_SHELL_VI_MODE   /* vi editing mode for the shell */
#define INCLUDE_SHELL_EMACS_MODE /* emacs editing mode for the shell */
#define INCLUDE_SHELL_INTERP_C  /* C interpreter */
#define INCLUDE_SHELL_INTERP_CMD /* shell command interpreter */

#define INCLUDE_DISK_UTIL_SHELL_CMD   /*包含ls copy等命令*/


#define INCLUDE_VXBUS_SHOW
#define INCLUDE_IFCONFIG
#define INCLUDE_NET_IF_SHOW
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_IPPING_CMD
#define INCLUDE_PING
/*end*/

#define INCLUDE_IPATTACH





#if defined (INCLUDE_SPE)

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

/*******************************************************************************
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
        case 0x80E20000:
            retChar = SYS_MODEL_P2020;
            break;
        case 0x80EA0000:
            retChar = SYS_MODEL_P2020E;
            break;
        default:
            retChar = SYS_MODEL_E500;
            break;
        }

    device = *M85XX_PVR(CCSBAR);

    if ((device & 0xffff0000) != 0x80210000)
        retChar =SYS_MODEL_UNKNOWN;

    return(retChar);
    }

/*******************************************************************************
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




/*******************************************************************************
*
* sysClkFreqGet - returns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*/

/*changed by dongyang*/
#if 1
UINT32 sysClkFreqGet (void)
{
    UINT32  sysClkFreq;
    UINT32  e500Ratio;
    UINT32  e5001Ratio;
    UINT32  platRatio;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);

#ifdef FORCE_DEFAULT_FREQ
    return(DEFAULT_SYSCLKFREQ);
#endif

    if ((platRatio > MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio][0] == 0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

    systemFreq = OSCILLATOR_FREQ;

    sysClkFreq = ((UINT32)(systemFreq * platRatioTable[platRatio][0])) >> \
                            ((UINT32)platRatioTable[platRatio][1]);

    ddrFreq = systemFreq * 4;

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    e5001Ratio = M85XX_PORPLLSR_E500_1_RATIO(CCSBAR);

    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0])) >> \
                            ((UINT32)e500RatioTable[e500Ratio][1]);
    core1Freq = ((UINT32)(sysClkFreq * e500RatioTable[e5001Ratio][0]))>> \
                            ((UINT32)e500RatioTable[e5001Ratio][1]);

    return(sysClkFreq);
}

#else
UINT32 sysClkFreqGet (void)
    {
    UINT32  sysClkFreq;
    UINT32  oscFreq;
    UINT32  e500Ratio;
    UINT32  e5001Ratio;
    UINT32  platRatio;
    UINT32  ddrRatio;
    UINT8   osc;
    UINT8   ddr;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);
    ddrRatio = M85XX_PORPLLSR_DDR_RATIO(CCSBAR);

#ifdef FORCE_DEFAULT_FREQ
    return(DEFAULT_SYSCLKFREQ);
#endif

    if ((platRatio > MAX_VALUE_PLAT_RATIO) || (platRatioTable[platRatio][0] == 0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */
    osc = *PIXIS_SPD(PIXIS_BASE) & 0x7;

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
        case PIXIS_OSC_166_MHZ:
            oscFreq = FREQ_166_MHZ;
            break;

        default:
            oscFreq = OSCILLATOR_FREQ;
            break;
        }

    systemFreq = oscFreq;

    ddr = (*PIXIS_SPD(PIXIS_BASE) >> 3) & 0x7;

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
        case PIXIS_DDR_166_MHZ:
            ddrFreq = FREQ_166_MHZ;
            break;
        default:
            ddrFreq = OSCILLATOR_FREQ;
            break;
    }

    ddrFreq = ddrFreq * ddrRatioTable[ddrRatio];

    sysClkFreq = ((UINT32)(oscFreq * platRatioTable[platRatio][0])) >> \
                            ((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    e5001Ratio = M85XX_PORPLLSR_E500_1_RATIO(CCSBAR);

    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0])) >> \
                            ((UINT32)e500RatioTable[e500Ratio][1]);
    core1Freq = ((UINT32)(sysClkFreq * e500RatioTable[e5001Ratio][0]))>> \
                            ((UINT32)e500RatioTable[e5001Ratio][1]);

    return(sysClkFreq);
    }
#endif


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
    UINT32  picClockDivisor /* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
        return sysClkFreqGet() / picClockDivisor;
    else
        return HPCN_RTC_FREQ;   /* 14.318 MHz */
    }


UINT32 divisorGet
    (
    UINT32 xtal,
    UINT32 baud
    )
    {
    return (xtal / (16 * baud));
    }

/*******************************************************************************
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



int mem_test(unsigned long start, unsigned long len);


/*****************************************************************************
 函 数 名  : sysCpuLawInit
 功能描述  : 设置Local Access Window
 输入参数  : void  
 输出参数  : 
 返回值    :
*****************************************************************************/
void sysCpuLawInit(void)
{
	UINT32 uiTemp = 0;

	*M85XX_LAWBAR3(CCSBAR) = (RAPIDIO1_ADRS >> LAWBAR_ADRS_SHIFT);
	*M85XX_LAWAR3(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_RAPIDIO | LAWAR_SIZE_256MB);
	
	*M85XX_LAWBAR4(CCSBAR) = (RAPIDIO2_ADRS >> LAWBAR_ADRS_SHIFT);
	*M85XX_LAWAR4(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_RAPIDIO2 | LAWAR_SIZE_256MB);

	/*SRIO 维护包使用的地址*/
	*M85XX_LAWBAR5(CCSBAR) = (RAPIDIO1_MAIN_ADRS >> LAWBAR_ADRS_SHIFT);
	*M85XX_LAWAR5(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_RAPIDIO | LAWAR_SIZE_256MB);

	*M85XX_LAWBAR6(CCSBAR) = (RAPIDIO2_MAIN_ADRS >> LAWBAR_ADRS_SHIFT);
	*M85XX_LAWAR6(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_RAPIDIO2 | LAWAR_SIZE_256MB);


	/*该window 用于CS1 从V5到K7*/
	*M85XX_LAWBAR7(CCSBAR) = (FPGA_K7_ADRS >> LAWBAR_ADRS_SHIFT);
	*M85XX_LAWAR7(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_64MB);


	/*该window 用于CS2 从V5到CAN*/
	*M85XX_LAWBAR8(CCSBAR) = (V5_INTERRAM_ADRS >> LAWBAR_ADRS_SHIFT);
	*M85XX_LAWAR8(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_2MB);

	/*CS1, 用于通过V5 访问K7*/	
	*M85XX_BR1(CCSBAR)=0x90001001;	        /*16bit valid*/
	*M85XX_OR1(CCSBAR)=0xFC000FF7;        /*64MB*/
	
	/*CS2, 用于通过V5 访问CAN*/	
	*M85XX_BR2(CCSBAR)=0x95001001;	        /*16bit valid*/
	*M85XX_OR2(CCSBAR)=0xFFE00FF7;        /*2MB*/

	*M85XX_LCRR(CCSBAR) = 2;
	uiTemp = *M85XX_LCRR(CCSBAR);   /*读一下才可以生效*/
	WRS_ASM("isync");
}


/*****************************************************************************
 函 数 名  : sysCpuLawShow
 功能描述  : 显示LAW的配置情况, 将该函数放在此处是因为其与LAW的设置函数挨着,
             比较便于查找
 输入参数  : 
 输出参数  : 
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2015年3月27日
    作    者   : wang dong yang
    修改内容   : Created function 
*****************************************************************************/
void sysCpuLawShow()
{
	UINT32 uiLoop = 0;
	VINT32 tmp, tmp2;

	for (uiLoop=0; uiLoop<12; uiLoop++)
	{
		tmp = * (VUINT32 *)(CCSBAR + 0xc08 + uiLoop * 0x20);
		tmp2 = * (VUINT32 *)(CCSBAR + 0xc10 + uiLoop * 0x20);

		printf("Law Bar%d = 0x%x \t Ar = 0x%08x\n", uiLoop, tmp, tmp2);
	}
}


/*******************************************************************************
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

	
#ifndef COMPILE_BOOTROM	
	sysCpuLawInit();
	rioRstHostId();  
#endif

    vxIvprSet((UINT32)VEC_BASE_ADRS);

#ifdef _WRS_CONFIG_SMP

/*
 * Here we initialize cpu1 init address so we don't wait until the
 * correct value is initialized
 */

    *(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_RBV;
#endif  /* _WRS_CONFIG_SMP */

    /* Disable L1 Icache */

    sysL1Csr1Set(vxL1CSR1Get() & ~0x1);

    /* Check for architecture support for 36 bit physical addressing */

#if defined(PPC_e500v2)
    vxHid0Set(_PPC_HID0_MAS7EN|vxHid0Get());
#endif  /* PPC_e500v2 */

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

#ifdef E500_L1_PARITY_RECOVERY

    /* Enable Parity in L1 caches */

    vxL1CSR0Set(vxL1CSR0Get() | _PPC_L1CSR_CPE);
    vxL1CSR1Set(vxL1CSR1Get() | _PPC_L1CSR_CPE);
#endif  /* E500_L1_PARITY_RECOVERY */

    /* enable time base for delay use before DEC interrupt is setup */

    vxDecSet(0xffffffff);
    vxDecarSet(0xffffffff);
    vxHid0Set(vxHid0Get() | _PPC_HID0_TBEN);

    /* sysTimerClkFreq = OSCILLATOR_FREQ; */

    sysTimerClkFreq = sysClkFreqGet () >> 3;

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif  /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_CACHE_SUPPORT
    sysL1CacheQuery();
#endif  /* INCLUDE_CACHE_SUPPORT */

    /* Initialize L2CTL register */

    vxL2CTLSet(0x28000000, M85XX_L2CTL(CCSBAR));

  /*
   * Need to setup static TLB entries for bootrom or any non-MMU
   * enabled images
   */

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (!defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL))
    mmuPpcIEnabled = TRUE;
    mmuPpcDEnabled = TRUE;
#else  /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

    if (inFullVxWorksImage==FALSE)
        {
        mmuPpcIEnabled = TRUE;
        mmuPpcDEnabled = TRUE;
        }

    /* Enable I Cache if instruction mmu disabled */

#if (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE))
    mmuPpcIEnabled = TRUE;
#endif  /* (defined(USER_I_CACHE_ENABLE) && !defined(USER_I_MMU_ENABLE)) */

#endif  /* !defined(INCLUDE_MMU_BASIC) && !defined(INCLUDE_MMU_FULL) */

#if (SW_MMU_ENABLE == TRUE)
    mmuPpcIEnabled = TRUE;
    mmuPpcDEnabled = TRUE;
#endif  /* SW_MMU_ENABLE == TRUE */

    vxHid1Set(HID1_ABE); /* Address Broadcast enable */

#if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
    sysL2CacheInit();
#endif  /* INCLUDE_L2_CACHE  && INCLUDE_CACHE_SUPPORT*/

    /* Machine check via RXFE for RIO */

    vxHid1Set (vxHid1Get()| HID1_ASTME | HID1_RXFE);

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

    WRS_ASM("isync");

    e500_spe_exc_enable = FALSE;
    e500_spe_exc_mask = 0;

    pSysPlbMethods = p2020PlbMethods;

#ifdef INCLUDE_VXBUS
	hardWareInterFaceInit();
#endif  /* INCLUDE_VXBUS */

#ifdef INCLUDE_I2C
    i2cDrvInit (0, 0);
    i2cDrvInit (1, 0);
#endif  /* INCLUDE_I2C */

#ifdef E500_L1_PARITY_RECOVERY
    vxIvor1Set(_EXC_OFF_L1_PARITY);
#endif  /* E500_L1_PARITY_RECOVERY */

#ifdef INCLUDE_L1_IPARITY_HDLR
    installL1ICacheParityErrorRecovery();
#endif  /* INCLUDE_L1_IPARITY_HDLR */

#ifdef INCLUDE_L2_SRAM
#   if (defined(INCLUDE_L2_CACHE) && defined(INCLUDE_CACHE_SUPPORT))
        sysL2SramEnable (TRUE);
#   elif (defined(INCLUDE_L2_SRAM))
        sysL2SramEnable (FALSE);
#   endif
#endif

    CACHE_PIPE_FLUSH();


#ifndef  COMPILE_BOOTROM 
    dmaDrvRegister();
#endif

}

#ifdef INCLUDE_L2_SRAM

/*******************************************************************************
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

    /* if INCLUDE_L2_CACHE and CACHE_SUPPORT
     * if ((L2_SRAM_SIZE + L2_CACHE_SIZE) > l2Siz)
     * Setup Windows for L2SRAM
     */

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

/*******************************************************************************
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
#endif  /* INCLUDE_EDR_PM */
        }

    return memTop;
    }

/*******************************************************************************
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

    token = intCpuLock();

#ifdef _WRS_CONFIG_SMP

    /*
     * In SMP mode, sysToMonitor() must run on core 0. If we already happen
     * to already be on core 0, then we can just put CPU 1 into a parking loop
     * and proceed with shutdown. If not, we have to use a cross-processor
     * call to re-invoke ourselves on core 0 first.
     */

    if (vxCpuIndexGet() == 0)
        {
        cpcInvoke (2, (CPC_FUNC)sysCpu1ShutdownLoop, NULL, 0, VX_CPC_ASYNC);
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
#endif  /* _WRS_CONFIG_SMP */

#ifdef INCLUDE_BRANCH_PREDICTION
    vxDisableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

#ifdef INCLUDE_CACHE_SUPPORT
#   ifdef _WRS_CONFIG_SMP
#   else  /* _WRS_CONFIG_SMP */
    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);
#   endif  /* _WRS_CONFIG_SMP */
#endif  /* INCLUDE_CACHE_SUPPORT */

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

    (*pRom) (startType);    /* jump off to romInit.s */

    return(OK);    /* in case we ever continue from ROM monitor */
    }

#if defined(_WRS_CONFIG_SMP)

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT void excIvorInit();
IMPORT void mmuPpcPidSet(UINT32);
IMPORT void sysCacheEnable();

/*******************************************************************************
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
    vxHid0Set(_PPC_HID0_MAS7EN | vxHid0Get());
#endif  /* PPC_e500v2 */

    /* Enable machine check pin */

    vxHid0Set(HID0_MCP | vxHid0Get());

    vxHid1Set (vxHid1Get() | HID1_ASTME | HID1_ABE | HID1_RXFE);

    sysL2CacheInit();

    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();

    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

    if(_pSysL2CacheInvFunc!=NULL)
    _pSysL2CacheInvFunc();

    sysCacheEnable();

    if(_pSysL2CacheEnable!=NULL)
    _pSysL2CacheEnable();

#ifdef INCLUDE_BRANCH_PREDICTION
    vxEnableBP();
#endif  /* INCLUDE_BRANCH_PREDICTION */

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

#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    }

/*******************************************************************************
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
    FUNCPTR  p;

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

    if (p != NULL)
        {
        p ();
        }

    for (;;);

    }

/*******************************************************************************
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
#   ifndef VX_ENABLE_CPU_TIMEOUT
#       define VX_ENABLE_CPU_TIMEOUT   5   /* default timeout five seconds */
#   endif  /* VX_ENABLE_CPU_TIMEOUT */

    cpuset_t    cpuset;
    UINT32  i = 0;

    /*
     * PPC Dec timer will not be available while synchronizing timebase
     * between two cores.
     * Use magic number, 0x9000000, to count 1 second independently
     * on PPC Dec timer.
     */

    UINT32  timeout = 0x9000000 * VX_ENABLE_CPU_TIMEOUT;

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

    *(M85XX_DEVDISR(CCSBAR)) |= (M85XX_DEVDISR_TB0 | M85XX_DEVDISR_TB1);

     /*
      * Save the current timebase on CPU0 into static valuable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * synchronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif  /* DRV_TIMER_M85XX && INCLUDE_TIMESTAMP */

    /* Flush Data cache so as to make sure other cores have updated cache */

    {
    volatile char* temp;

    temp = malloc(0x100000);

    memset((char*)temp, 0x0, 0x100000);

    sysCpu1Start ();

    memset((char*)temp, 0x0, 0x100000);

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
/*******************************************************************************
*
* sysCpcTimeBaseSet - CPC receiver for sysSmpTimeBaseInit
*
* This function receives the CPC from sysSmpTimeBaseInit,
* and passes the received parameters to sysSmpTimeBaseSet.
*
*/

LOCAL STATUS sysCpcTimeBaseSet
    (
    void    *ptr,   /* ptr to CPC parameters */
    UINT    size    /* size of CPC parameters */
    )
    {
    UINT32  tbHI = *(UINT32 *)ptr;
    UINT32  tbLO = ((UINT32 *)ptr)[1];

    sysSmpTimeBaseSet(tbHI, tbLO);
    return OK;
    }


/*******************************************************************************
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
    UINT32  tbHI,
    UINT32  tbLO
    )
    {
    cpuset_t  cpcInvokeCpuSet;
    UINT32    cpcParam[2];
    int       key;

    CPUSET_ZERO (cpcInvokeCpuSet);  /* => all but self */
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

/*******************************************************************************
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
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

/*******************************************************************************
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

/*******************************************************************************
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
#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
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
#endif  /* INCLUDE_VXBUS */

    /* initialize serial interrupts */

#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
#endif  /* INCLUDE_SIO_UTILS */

#if  defined (INCLUDE_SPE)
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */

#ifdef  INCLUDE_VXBUS
    taskSpawn("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_FSL_SPI
    sysSpiInit ();
#endif /* INCLUDE_FSL_SPI */


	/*修改LBC 速度 20140924*/
	/*
	*M85XX_LCRR(CCSBAR) = 0x10002;
	*/

}

/*******************************************************************************
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

/*******************************************************************************
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

/*******************************************************************************
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

/*******************************************************************************
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


/*******************************************************************************
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
/*******************************************************************************
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
            ppcE500CACHE_ALIGN_SIZE = 32;
            break;
        case 1:
            ppcE500CACHE_ALIGN_SIZE = 64;
            break;
        default:
            ppcE500CACHE_ALIGN_SIZE = 32;
            break;
        }

    ppcE500DCACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);
    ppcE500ICACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);

    /*
     * The core manual suggests for a 32 byte cache line and 8 lines per set
     * we actually need 12 unique address loads to flush the set.
     * The number of lines to flush should be ( 3/2 * cache lines )
     */

    ppcE500DCACHE_LINE_NUM = (3 * ppcE500DCACHE_LINE_NUM) >> 1;
    ppcE500ICACHE_LINE_NUM = (3 * ppcE500ICACHE_LINE_NUM) >> 1;

    }

#endif /* INCLUDE_CACHE_SUPPORT */

/*******************************************************************************
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
    strcpy ((char *)EXC_MSG_OFFSET, errorMsg);
    }

/*******************************************************************************
*
* chipErrataCpu29Print - save Silicon fault message to save area
*
* The message will save Silicon fault message to save area for catastrophic
* error.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void chipErrataCpu29Print (void)
    {
    saveExcMsg("Silicon fault detected, possible machine state corruption.\nSystem rebooted to limit damage.");
    }

/*******************************************************************************
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

#ifdef INCLUDE_SHOW_ROUTINES

/*******************************************************************************
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

/*******************************************************************************
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

    printf("Core CPU0 Freq = %3d Hz\n", coreFreq);
    printf("Core CPU1 Freq = %3d Hz\n", core1Freq);
    printf("DDR Freq = %3d Hz\n", ddrFreq);
    printf("CCB Freq = %3d Hz\n", sysClkFreqGet());
    printf("System Freq = %3d Hz\n", systemFreq);

    }

#endif  /* INCLUDE_SHOW_ROUTINES */

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciEx1IntRoute [NUM_PCIEX1_SLOTS][4] =
{
    { PCIEX1_XINT1_LVL, PCIEX1_XINT2_LVL, PCIEX1_XINT3_LVL, PCIEX1_XINT4_LVL }
};

LOCAL UCHAR sysPciEx2IntRoute [NUM_PCIEX2_SLOTS][4] =
{
    { PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL }
};

LOCAL UCHAR sysPciEx3IntRoute [NUM_PCIEX3_SLOTS][4] =
{
    { PCIEX3_XINT1_LVL, PCIEX3_XINT2_LVL, PCIEX3_XINT3_LVL, PCIEX3_XINT4_LVL }
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
    return sysPciEx1IntRoute[0][pin - 1];
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
    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
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
    return sysPciEx2IntRoute[0][pin - 1];
    }

/*******************************************************************************
*
* sysPciEx3AutoConfigInclude - PCI Express 3 autoconfig support routine
*
* This routine performs the PCI Express 3 auto configuration support function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS sysPciEx3AutoConfigInclude
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
* sysPciEx3AutoConfigIntAssign - PCI Express 3 autoconfig support routine
*
* This routine peforms the PCI Express 3 auto configuration interrupt assignment
* support function.
*
* RETURNS: PCI interrupt line number given pin mask
*
* ERRNO: N/A
*/

UCHAR sysPciEx3AutoConfigIntAssign
    (
    PCI_SYSTEM *    pSys,       /* PCI_SYSTEM structure pointer */
    PCI_LOC *       pLoc,       /* pointer to function in question */
    UCHAR           pin         /* contents of PCI int pin register */
    )
    {
    return sysPciEx3IntRoute[0][pin - 1];
    }

#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_SIO_UTILS
#   define BSP_SERIAL_CHAN_GET  bspSerialChanGet
#else
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
#ifdef INCLUDE_CPU_PWR_MGMT
        vxHid0Set(vxHid0Get() | _PPC_HID0_DOZE);
        WRS_ASM("sync");
        vxMsrSet(vxMsrGet()| _PPC_MSR_WE);
        WRS_ASM("isync");
#endif  /* INCLUDE_CPU_PWR_MGMT */

        /* expect to lock in DOZE mode until HRESET */

        sysDelay();
        }
    }
#endif /* _WRS_CONFIG_SMP */




int sysFtpAuth(Ipftps_session *session, char *password)
{
    if(strcmp(session->username, "root") == 0 && strcmp(password, "root") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}







/*********************************************************************************
**********************************************************************************
**********************************************************************************/
#define RBR     0x00    /* receiver buffer register */
#define THR     0x00    /* transmit holding register */
#define DLL     0x00    /* divisor latch */
#define IER     0x01    /* interrupt enable register */
#define DLM     0x01    /* divisor latch(MS) */
#define IIR     0x02    /* interrupt identification register */
#define FCR     0x02    /* FIFO control register */
#define LCR     0x03    /* line control register */
#define MCR     0x04    /* modem control register */
#define LSR     0x05    /* line status register */
#define MSR     0x06    /* modem status register */
#define SCR     0x07    /* scratch register */



/* added by freeWinder for test */
int ns16550PollOutputInit()
{
	/* set baud */
	*(char*)(CCSBAR+0x4500+LCR)=0x83;
	*(char*)(CCSBAR+0x4500+DLL)= (sysClkFreqGet()/(16*9600))&0xff;
	*(char*)(CCSBAR+0x4500+DLM)= ((sysClkFreqGet()/(16*9600))&0xff00)>>8;
	*(char*)(CCSBAR+0x4500+LCR)=0x03;
	return 0;
}

/******************************
 print integer:HEX output 
*******************************/
#define STATIC                     static

typedef char                       CHAR;

#define RETURN_ERROR     -1
#define RETURN_OK        0

#define MPC_PRINT_HEX             1
#define MPC_PRINT_DEC             2
#define MPC_INT_DATA_LEN_HEX      8
#define MPC_INT_DATA_LEN_DEC      10
#define MPC_INT_DATA_MAX_DEC      1000000000

#define FALSH_MPC_BASE_ADDR 0xfc000000

#define FREQUENCY        500000000
#define UART_BAUD_RATE   9600

#define MPC_WRITE_REG(addr,data) ((*addr) = data)
#define MPC_READ_REG(addr)       (*addr)

#define    UART_RBR     (CCSBAR+0x4500+0x00)    /* receiver buffer register */
#define    UART_THR     (CCSBAR+0x4500+0x00)    /* transmit holding register */
#define    UART_DLL     (CCSBAR+0x4500+0x00)    /* divisor latch */
#define    UART_IER     (CCSBAR+0x4500+0x01)    /* interrupt enable register */
#define    UART_DLM     (CCSBAR+0x4500+0x01)    /* divisor latch(MS) */
#define    UART_IIR     (CCSBAR+0x4500+0x02)    /* interrupt identification register */
#define    UART_FCR     (CCSBAR+0x4500+0x02)    /* FIFO control register */
#define    UART_LCR     (CCSBAR+0x4500+0x03)    /* line control register */
#define    UART_MCR     (CCSBAR+0x4500+0x04)    /* modem control register */
#define    UART_LSR     (CCSBAR+0x4500+0x05)    /* line status register */
#define    UART_MSR     (CCSBAR+0x4500+0x06)    /* modem status register */
#define    UART_SCR     (CCSBAR+0x4500+0x07)    /* scratch register */

#define    DEVDISR      (CCSBAR+0xe0000+0x70)

#define    GPIOCR       (CCSBAR+0xe0000+0x30)
#define    GPOUTDR      (CCSBAR+0xe0000+0x40)

static INT32 uartMPCPutChar(CHAR  ucChar)
{
	UINT8      ucStatus = 0;
	UINT32     uiLoop = 0;

	ucStatus = MPC_READ_REG((CHAR*)UART_LSR);
	for(uiLoop = 100000; uiLoop > 0 ;uiLoop--)
	{
		if(0x00 != (ucStatus & 0x20))
		{
			break;
		}
		ucStatus = MPC_READ_REG((CHAR*)UART_LSR);
	}

	if(0x00 == uiLoop)
	{
		return RETURN_ERROR;
	}
	
	MPC_WRITE_REG((CHAR*)UART_THR,ucChar);

	return RETURN_OK;
}


INT32 uartMPCPrintHEX(UINT32  iData)
{
    UINT8  ucBuf[MPC_INT_DATA_LEN_HEX] = {0};
    UINT8  ucLoop = 0;
    UINT32   uiBegin = 0;
    
    for(ucLoop = 0;ucLoop < MPC_INT_DATA_LEN_HEX;ucLoop++)
    {
        ucBuf[ucLoop] = (UINT8)((iData >> (ucLoop * 4)) & 0xf);
        if(ucBuf[ucLoop] < 10)
        {
            ucBuf[ucLoop] += '0'; 
        }
        else
        {
            ucBuf[ucLoop] += 'a';
            ucBuf[ucLoop] -= 10;
        }
    }
    for(ucLoop = 0;ucLoop < MPC_INT_DATA_LEN_HEX;ucLoop++)
    {
        if(('0' == ucBuf[MPC_INT_DATA_LEN_HEX - ucLoop - 1]) && (0x00 == uiBegin))
        {
            continue;
        }
        uiBegin = 1;
        if(RETURN_OK != uartMPCPutChar(ucBuf[MPC_INT_DATA_LEN_HEX - ucLoop - 1]))
		{
			return RETURN_ERROR;
		}
    }
}



int ns16550PollOutputchar
    (
    char            outChar /* char to send */
    )
{
	UINT8 pollStatus;

	pollStatus = *(char*)(CCSBAR+0x4500+LSR);

	/* is the transmitter ready to accept a character? */
	if ((pollStatus & 0x20) == 0x00)
	{
		return(-1);
	}
	
	*(char*)(CCSBAR+0x4500+THR) = outChar;

	return(0);

}

int ns16550Pollprintf(char* pchar)
{
	while(*(pchar)!='\0')
	{
		if (*pchar == '\n')
		{
			while (ns16550PollOutputchar('\r')!=0);
			while (ns16550PollOutputchar('\n')!=0);
		}
		else
			while (ns16550PollOutputchar(*pchar)!=0);
			
		pchar++; 
	}
	return 0;
}







/*******************************************************************/

typedef unsigned long ul;
typedef unsigned long long ull;
typedef unsigned long volatile ulv;


typedef struct tagTEST_TEST_S
{
    char *name;
    int (*fp)();
}TEST_TEST_S;

#if 0
#define memPrint(fmt,arg...) printf(fmt,##arg);
#else
#define memPrint(fmt,arg...)  ns16550Pollprintf(fmt);

#endif


#define rand32() ((unsigned int) rand() | ( (unsigned int) rand() << 16))
#define rand_ul() rand32()
#define UL_ONEBITS 0xffffffff
#define UL_LEN 32
#define CHECKERBOARD1 0x55555555
#define CHECKERBOARD2 0xaaaaaaaa
#define UL_BYTE(x) ((x | x << 8 | x << 16 | x << 24))


char progress[] = "-\\|/";
#define PROGRESSLEN 4
#define PROGRESSOFTEN 2500

/* Function definitions. */

int compare_regions(ulv *bufa, ulv *bufb, size_t count) 
{
    size_t i;
    ulv *p1 = bufa;
    ulv *p2 = bufb;

    for (i = 0; i < count; i++, p1++, p2++) 
    {
        if (*p1 != *p2) 
        {
            return -1;
        }
    }
    
    return 0;
}

int test_stuck_address(ulv bufa,ul count) 
{
    ulv *p1 = NULL;
    unsigned int j;
    ul i;

    memPrint("test stuck address start\n");
    for (j = 0; j < 16; j++) 
    {
        p1 = (ulv *) bufa;

        for (i = 0; i < count; i++) 
        {
            *p1 = ((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1);
            *p1++;
        }

        p1 = (ulv *) bufa;
        for (i = 0; i < count; i++, p1++) 
        {
            if (*p1 != (((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1))) 
            {
                memPrint("Skipping to next test...\n");
                return -1;
            }
        }
    }
    
    memPrint("test stuck address succeed\n");
    
    return 0;
}

int test_random_value(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i = 0;


    for (i = 0; i < count; i++) 
    {
        *p1++ = *p2++ = rand_ul();
        if (0 == (i % PROGRESSOFTEN)) 
        {
            memPrint(".");
        }
    }
    
    memPrint("\n");
    
    return compare_regions(bufa, bufb, count);
}

int test_xor_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ ^= q;
        *p2++ ^= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_sub_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ -= q;
        *p2++ -= q;
    }
    return compare_regions(bufa, bufb, count);
}

int test_mul_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ *= q;
        *p2++ *= q;
    }
    return compare_regions(bufa, bufb, count);
}

int test_div_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        if (0 == q) 
        {
            q++;
        }
        *p1++ |= q;
        *p2++ |= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_or_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ /= q;
        *p2++ /= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_and_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ &= q;
        *p2++ &= q;
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_seqinc_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++) 
    {
        *p1++ = *p2++ = (i + q);
    }
    
    return compare_regions(bufa, bufb, count);
}

int test_solidbits_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    ul q;
    size_t i;

    memPrint("test_solidbits_comparison \n");
    
    for (j = 0; j < 64; j++) 
    {
        q = (j % 2) == 0 ? UL_ONEBITS : 0;
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        
        memPrint(".");
        
        if (0 != compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_solidbits_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_solidbits_comparison succeed\n");
    
    return 0;
}

int test_checkerboard_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j = 0;
    ul q = 0;
    size_t i = 0;

    memPrint("test_checkerboard_comparison\n");
   
    for (j = 0; j < 64; j++) 
    {
        q = (j % 2) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++) 
        {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_checkerboard_comparison failed\n");
            return -1;
        }
    }
    
    memPrint("\n");
    memPrint("test_checkerboard_comparison succeed\n");

    
    return 0;
}

int test_blockseq_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    memPrint("test_blockseq_comparison\n");
    
    for (j = 0; j < 256; j++) 
    {
    
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            *p1++ = *p2++ = (ul) UL_BYTE(j);
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_blockseq_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_blockseq_comparison succeed\n");

    return 0;
}

int test_walkbits0_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    memPrint("test_walkbits0_comparison\n");
    
    for (j = 0; j < UL_LEN * 2; j++) 
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            if (j < UL_LEN) 
            { /* Walk it up. */
                *p1++ = *p2++ = 0x00000001 << j;
            } 
            else 
            { /* Walk it back down. */
                *p1++ = *p2++ = 0x00000001 << (UL_LEN * 2 - j - 1);
            }
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_walkbits0_comparison failed\n");
            return -1;
        }
    }
    
    memPrint("\n");
    memPrint("test_walkbits0_comparison succeed\n");
    
    return 0;
}

int test_walkbits1_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;


    memPrint("test_walkbits1_comparison\n");
    
    for (j = 0; j < UL_LEN * 2; j++) 
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            if (j < UL_LEN) 
            { /* Walk it up. */
                *p1++ = *p2++ = UL_ONEBITS ^ (0x00000001 << j);
            } else 
            { /* Walk it back down. */
                *p1++ = *p2++ = UL_ONEBITS ^ (0x00000001 << (UL_LEN * 2 - j - 1));
            }
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_walkbits1_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_walkbits1_comparison succeed\n");

    return 0;
}

int test_bitspread_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    memPrint("test_bitspread_comparison\n");
    
    for (j = 0; j < UL_LEN * 2; j++)
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        
        memPrint(".");
        
        for (i = 0; i < count; i++) 
        {
            if (j < UL_LEN) 
            { /* Walk it up. */
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (0x00000001 << j) | (0x00000001 << (j + 2))
                    : UL_ONEBITS ^ ((0x00000001 << j)
                                    | (0x00000001 << (j + 2)));
            } 
            else 
            { /* Walk it back down. */
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (0x00000001 << (UL_LEN * 2 - 1 - j)) | (0x00000001 << (UL_LEN * 2 + 1 - j))
                    : UL_ONEBITS ^ (0x00000001 << (UL_LEN * 2 - 1 - j)
                                    | (0x00000001 << (UL_LEN * 2 + 1 - j)));
            }
        }
        
        memPrint(".");
        
        if (compare_regions(bufa, bufb, count)) 
        {
            memPrint("\ntest_bitspread_comparison failed\n");
            return -1;
        }
    }

    memPrint("\n");
    memPrint("test_bitspread_comparison succeed\n");
    
    return 0;
}

int test_bitflip_comparison(ulv *bufa, ulv *bufb, size_t count) 
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j, k;
    ul q;
    size_t i;

    memPrint("test_bitflip_comparison\n");
    
    for (k = 0; k < UL_LEN; k++) 
    {
        q = 0x00000001 << k;
        for (j = 0; j < 8; j++) 
        {
            q = ~q;
            
            memPrint(".");
            
            p1 = (ulv *) bufa;
            p2 = (ulv *) bufb;
            for (i = 0; i < count; i++) 
            {
                *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
            }
            
            memPrint(".");
            
            if (compare_regions(bufa, bufb, count)) 
            {
                memPrint("\ntest_bitflip_comparison failed\n");
                return -1;
            }
        }
    }

    memPrint("\n");
    memPrint("test_bitflip_comparison succeed\n");
    
    return 0;
}













#define EXIT_FAIL_NONSTARTER    0x01
#define EXIT_FAIL_ADDRESSLINES  0x02
#define EXIT_FAIL_OTHERTEST     0x04

TEST_TEST_S gstTests[] = 
{
    { "Random Value", test_random_value },
    { "Compare XOR", test_xor_comparison },
    { "Compare SUB", test_sub_comparison },
    { "Compare MUL", test_mul_comparison },
    { "Compare DIV",test_div_comparison },
    { "Compare OR", test_or_comparison },
    { "Compare AND", test_and_comparison },
    { "Sequential Increment", test_seqinc_comparison },
    { "Solid Bits", test_solidbits_comparison },
    { "Block Sequential", test_blockseq_comparison },
    { "Checkerboard", test_checkerboard_comparison },
    { "Bit Spread", test_bitspread_comparison },
    { "Bit Flip", test_bitflip_comparison },
    { "Walking Ones", test_walkbits1_comparison },
    { "Walking Zeroes", test_walkbits0_comparison },
    { NULL, NULL }
};

int mem_test(ul start,ul len)
{
    ulv *bufa = NULL;
    ulv *bufb = NULL;
    ul   count = 0;
    ul   loop = 0;

    memPrint("\n");

    count = len / (2 * sizeof(ul));
    bufa = (ulv *) (start & (~3));
    bufb = (ulv *) ((start + (len / 2)) & (~3));
    
    if (0 != test_stuck_address(start,count)) 
    {
        return -1;
    }
    for (loop = 0;loop < sizeof(gstTests)/sizeof(TEST_TEST_S);loop++) 
    {
        if (NULL == gstTests[loop].name) 
        {
            break;
        }
        
        memPrint("over one \n");
        
        if (!gstTests[loop].fp(bufa, bufb, count)) 
        {
            memPrint("ok\n");
        } 
        else 
        {
            memPrint("failed\n");
            
            return -1;
        }
    }
    memPrint("\n");

    return 0;
}







