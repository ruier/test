/* sysLib.c - IBM 405GP Walnut system-dependent routines */

/*

*****************************************************************************
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

********************************************************************************
*/

/* 
 * Copyright (c) 1999-2009 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02n,20apr09,wap  Switch to VxBus EMAC driver
02m,04sep08,l_z  Fix compile warning
02l,02sep08,x_f  Remove sysDcr.h include, replace sysDcrInLong with dcrInLong, 
                 sysDcrOutLong with dcrOutLong
02k,21apr08,fao  change ns16550 serial driver to support vxbus 
02j,17sep07,kab  Doc cleanup
02i,08aug07,agf  remove duplicate variable definitions
02h,25apr06,jmt  Modify sysModelGet to use SYS_MODEL
02g,28oct05,mdo  SPR#114197 - protect against multiple defines for
                 INCLUDE_PCICFG_SHOW
02f,27oct04,pch  SPR 103186: PM_RESERVED_MEM
02e,30aug04,mdo  Documentation fixes for apigen
02d,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
02c,10dec03,jtp  Support MMU_FULL
02b,02dec03,agf  fix compiler warnings
02a,26aug03,jln  change malLib.c to ibmMalDma.c
01z,23jul03,jln  update to support Emac driver in drv/end/emacEnd.c
01y,23oct03,pch  SPR 80911: get PPC4xx cache dimensions from PVR
01x,29jan03,jtp  SPR 77088: include sysPhysMemDesc only when MMU supported
01w,12sep02,pch  SPR 80225: defer connecting sysUicIntHandler until sysHwInit2
                 SPR 82333: sysToMonitor must disable MMU before cache
                 SPR 82335: fix warm boot offset in sysToMonitor
01v,20aug02,pch  405GP / 405GPr interoperability
01u,18jun02,mcg  add 405GPr support, add #INCLUDE sysDcr.h, add FPGA to
                 sysPhysMemDesc, fix ICCR and DCCR initialization when more
                 than 128MB of SDRAM is detected.
01t,25jun02,pch  SPR 77135: Don't include or call emac/mal unless
                 INCLUDE_NETWORK
01s,02may02,pch  SPR 73648: common build for both Rev D and Rev E processors
01r,24jan02,pch  Fix Diab warnings
01q,17jan02,pch  Add #ifdef INCLUDE_AUX_CLK to isolate the AUX clock
01p,09jan02,pch  Change MAL reference to MAL0 for consistency with ebony
01o,06dec01,kab  Update for T2.2; fix compiler warning
01n,30nov01,pch  Add call to pciIntLibInit()
01m,05sep01,kab  grouped all sys[In|Out]* funcs in sysALib.s
01l,27jul01,kab  fixed sysToMonitor (SPR 68699); cleanup
01k,10may01,mcg  Support for Emac driver multi-channel changes
01j,06dec00,s_m  changed sysModel to handle Rev E
01i,27nov00,s_m  moved END and timer drivers back to bsp dir
01h,23oct00,s_m  fixed comment
01g,23oct00,s_m  fixed CPU comment
01f,23oct00,s_m  added comment for sysPhysMemDesc
01e,25may00,mcg  added sysInfoGet() and sysInfoShow()
01d,11may00,mcg  removed PCI configuration code added calls to VxWorks
                 PCI autoconfiguration code
01c,29mar00,mcg  moved sysTimerClkFreq initialization to the beginning of
                 sysHwInit to fix sysLocalDelay
01b,22jan00,mcg  added LOCAL_MEM_AUTOSIZE support and RTC support
01a,20sep99,mcg  created from 403evb version 02e.
*/

/*
DESCRIPTION
This library provides board-specific routines.   The chip drivers included are:

uicIntr.c  - on-chip Universal Interrupt Controller library
d1643RTC.c - driver for the real time clock portion of the
             Dallas Semiconductor DS1643 Nonvolatile Timekeeping RAM

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include "vxWorks.h"
#include "cacheLib.h"
#include "asm.h"
#include "vme.h"
#include "iv.h"
#include "esf.h"
#include "ioLib.h"
#include "sysLib.h"
#include "config.h"
#include "memLib.h"
#include "excLib.h"
#include "logLib.h"
#include "vxLib.h"
#include "string.h"
#include "intLib.h"
#include "taskLib.h"
#include "ppc405GP.h"
#include <private/vmLibP.h>
#include <arch/ppc/dcr4xxALib.h>

/*
 * On the 405 CPU we cannot enable the MMU unless the caches are initialized
 * (unless of course we mark the entire address space as un-cacheable). There
 * is no separate way of disabling the caches when the MMU is enabled, other
 * than on a page-by-page basis. Hence it is recommended that caches always
 * be used whenever the MMU is used.
 */
#if (defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL))
    #ifndef INCLUDE_CACHE_SUPPORT
    #error "You need to include Cache support for MMU support."
    #endif
#endif

/* externals */

IMPORT STATUS   excIntConnectTimer (VOIDFUNCPTR * vector, VOIDFUNCPTR routine);
IMPORT void     excIntHandle (void);
IMPORT UINT32   ppc405IccrVal;          /* cacheability state for instruction */
IMPORT UINT32   ppc405DccrVal;          /* cacheability state for data */
#ifdef INCLUDE_VXBUS
IMPORT void hardWareInterFaceInit(void);
#   ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll(void);
#   endif /* INCLUDE_SIO_UTILS */
#endif /* INCLUDE_VXBUS */

/* globals */

int   sysBus;                           /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (PPC405GP) */
char *sysBootLine = BOOT_LINE_ADRS;     /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;       /* catastrophic message area */
int   sysTimerClkFreq;                  /* Timer clock frequency             */
SYS_INFO systemInfo;                    /* PLL and system frequencies        */
int   sysProcNum;			/* processor number of this CPU */

#ifndef _PPC_PVR_CACHE_SIZE_FIELD
/*
 * The following variables specify the cache line size and the number of
 * cache lines for the 405GP and 405GPr, for use by older versions of the
 * architecture cache libraries.  Strictly speaking, the line counts should
 * be different for the two processors, however the values are actually used
 * only as loop counts when flushing or invalidating the entire cache.
 * It is harmless -- and much more convenient -- to use the larger value
 * in all cases.
 *
 * Newer versions of the cache libraries avoid this problem altogether by
 * examining the PVR to determine the cache sizes.
 */
const int ppc405ICACHE_LINE_NUM  = _ICACHE_LINE_NUM_405GPR; /* lines in Icache */
const int ppc405DCACHE_LINE_NUM  = _DCACHE_LINE_NUM_405GPR; /* lines in Dcache */
const int ppc405CACHE_ALIGN_SIZE = _CACHE_ALIGN_SIZE;	   /* cache line size */
#endif	/* _PPC_PVR_CACHE_SIZE_FIELD */

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 *
 * Address translations for local RAM, memory mapped PCI bus, memory mapped
 * IO space and local PROM/FLASH are set here.
 *
 * PTEs are held in a 2-level page table. There is one Level 1 page table
 * and several Level 2 page tables. The size of the Level 1 table is 4K
 * and the size of each Level 2 page table is 8K. Each Level 2 table can
 * map upto 4MB of contiguous memory space.
 *
 * Calculating size of page table required:
 * =======================================
 * For the following memory map we can calculate the page table size
 * required as follows:
 *
 *      Memory Area                Size               # of Level 2 pages
 *      ===========                ====               ==================
 *      1. Local Memory            32MB                      8
 *      2. PCI Memory              64MB                     16
 *      3. PCI IO Regn 1            64K                      1
 *      4. PCI IO Regn 2            1MB                      1
 *      5. PCI CFG                   4K                      1
 *      6. PCI IACK                  4K                      0 @
 *      7. PP Bridge                 4K                      1
 *      8. UART IO Space             4K                      0 @
 *      9. NVRAM Space               8K                      1
 *      10. Flash                   512K                     1
 *      @ - included in previous L2 page
 *
 *      Total # of L2 pages =                               30
 *      Total Memory Required for page table = 30 * 8 + 4 = 244 K.
 *
 * Also note that since each L2 page is 8k aligned + 8 bytes of block-info
 * overhead, there will be a (4096-8) = 4088-byte unallocated block between
 * L2 pages.  This can look like some kind of memory fragmentation problem:
 * E.g,
 * > memShow(1)
 *
 * FREE LIST:
 *    num     addr      size
 *    --- ---------- ----------
 *      1  0x1ffb1e8      13720
 * ...
 *      5  0x1ffb000        472
 *      6  0x1ff9000       4088
 *      7  0x1ff6000       4088
 * ... repeats		   4088
 *     34  0x1ff3000       4088
 *     35  0x1ff0000       4088
 *     36  0x1f9f000       4072
 *     37  0x1f81800     116728
 *     38  0x1f3b000        488
 */

#if (defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL))
PHYS_MEM_DESC sysPhysMemDesc [] =
    {
    {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    LOCAL_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE
    },

    {
    (VIRT_ADDR) PCI_MEMORY_START,
    (PHYS_ADDR) PCI_MEMORY_START,
    PCI_MEMORY_MAP_END - PCI_MEMORY_START + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PLB_PCI_IO_REGION_1_START,
    (PHYS_ADDR) PLB_PCI_IO_REGION_1_START,
    PLB_PCI_IO_REGION_1_END - PLB_PCI_IO_REGION_1_START + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PLB_PCI_IO_REGION_2_START,
    (PHYS_ADDR) PLB_PCI_IO_REGION_2_START,
    PLB_PCI_IO_REGION_2_MAP_END - PLB_PCI_IO_REGION_2_START + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCI_CFGADDR,
    (PHYS_ADDR) PCI_CFGADDR,
    PCI_CFGEND - PCI_CFGADDR + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PCI_INTERRUPT_ACK,
    (PHYS_ADDR) PCI_INTERRUPT_ACK,
    PCI_INTERRUPT_ACK_END - PCI_INTERRUPT_ACK + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) PP_BRIDGE_CR,
    (PHYS_ADDR) PP_BRIDGE_CR,
    PP_BRIDGE_CR_END - PP_BRIDGE_CR + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) UART_MEMORY_START,
    (PHYS_ADDR) UART_MEMORY_START,
    UART_MEMORY_END - UART_MEMORY_START + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) NV_RAM_ADRS,
    (PHYS_ADDR) NV_RAM_ADRS,
    NV_RAM_SIZE_ALIGNED,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) FPGA_BASE_ADRS,
    (PHYS_ADDR) FPGA_BASE_ADRS,
    4*1024,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR) 0xFFF00000,
    (PHYS_ADDR) 0xFFF00000,
    512 * 1024,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    },

    {
    (VIRT_ADDR) FLASH_START,
    (PHYS_ADDR) FLASH_START,
    FLASH_END - FLASH_START + 1,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    }
    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);
#endif /* INCLUDE_MMU_BASIC || INCLUDE_MMU_FULL */

/* Structure for internal serial clock array */

typedef struct
    {
    UINT freqProcMhz;
    UINT baudRate;
    UINT uartClkDivisor;
    } INT_SERIAL_INFO;

/*
 * Table of CPU speeds and baud rates used to determine the CPU:UART divisor.
 * The CPU clock is divided by the CPU:UART divisor to get the UART clock which
 * is fed into the UARTs.  The UART clock is then divided again by the UART to
 * obtain the baud rate.  The table of CPU:UART divisors below helps to select
 * a value that will result in the lowest % error on the baud rate.
 * NOTE:
 * The CPU:UART divisor must be high enough so the resulting UART clock is
 * less than 1/2 of the OPB bus frequency.
 */

INT_SERIAL_INFO uartClkTbl[] =
    {
      /* CPU Freq     Baud   CPU:UART divisor */

        {  133,       1200,     15  },
        {  133,       2400,     16  },
        {  133,       4800,     28  },
        {  133,       9600,     14  },
        {  133,      19200,     14  },
        {  133,      28800,     17  },
        {  133,      33600,     31  },
        {  133,      38400,     31  },
        {  133,      57600,     29  },
        {  133,     115200,     12  },
        {  133,     307200,     27  },

        {  200,       1200,     11  },
        {  200,       2400,     28  },
        {  200,       4800,     28  },
        {  200,       9600,     14  },
        {  200,      19200,     31  },
        {  200,      28800,     31  },
        {  200,      33600,     31  },
        {  200,      38400,     25  },
        {  200,      57600,     31  },
        {  200,     115200,     12  },
        {  200,     307200,     20  },

        {  266,       1200,     19  },
        {  266,       2400,     31  },
        {  266,       4800,     28  },
        {  266,       9600,     28  },
        {  266,      19200,     28  },
        {  266,      28800,     29  },
        {  266,      33600,     31  },
        {  266,      38400,     31  },
        {  266,      57600,     29  },
        {  266,     115200,     29  },
        {  266,     307200,     27  },

        {  333,       1200,     27  },
        {  333,       2400,     31  },
        {  333,       4800,     31  },
        {  333,       9600,     31  },
        {  333,      19200,     31  },
        {  333,      28800,     29  },
        {  333,      33600,     31  },
        {  333,      38400,     17  },
        {  333,      57600,     30  },
        {  333,     115200,     30  },
        {  333,     307200,     17  },

        {  400,       1200,     22  },
        {  400,       2400,     31  },
        {  400,       4800,     31  },
        {  400,       9600,     31  },
        {  400,      19200,     31  },
        {  400,      28800,     31  },
        {  400,      33600,     31  },
        {  400,      38400,     31  },
        {  400,      57600,     31  },
        {  400,     115200,     31  },
        {  400,     307200,     27  }

    };


/* forward declarations */

STATUS sysInfoGet(SYS_INFO * sysInfo, int verbose);
void   sysLocalDelay(UINT32 ms_delay);

UCHAR  sysPciInByte(ULONG address);
void   sysPciOutByte(ULONG address, UCHAR data);
UINT16 sysPciInWord(ULONG address);
void   sysPciOutWord(ULONG address, UINT16 data);
UINT   sysPciInLong(ULONG address);
void   sysPciOutLong(ULONG address, UINT data);



/* locals */

/*
 * Feedback clock sources
 *
 * Values other than PLB_FDBK apply only to 405GPr in "new mode."
 */
enum feedback { PLB_FDBK, CPU_FDBK, PCI_FDBK, EBC_FDBK };


#ifdef DOC
#define INCLUDE_TIMESTAMP
#endif

/* BSP Drivers */

#include "mem/byteNvRam.c"
#include "vme/nullVme.c"
#include "uicIntr.c"                    /* UIC interrupt library */
#include "ppc405Timer.c"                /* ppc405 timer driver */

#ifdef INCLUDE_PCI
#   include "pci/pciConfigLib.c"
#   include "pci/pciIntLib.c"
#if (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD))
#   include "pci/pciConfigShow.c"
#endif /* (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD)) */
#   ifdef INCLUDE_PCI_AUTOCONF
#       include "pci/pciAutoConfigLib.c"
#       include "sysBusPci.c"
#   endif /* INCLUDE_PCI_AUTOCONF */
#endif /* INCLUDE_PCI */

#include "sysNet.c"

#ifdef INCLUDE_VXBUS
#   include <hwif/vxbus/vxBus.h>
#   include "hwconf.c"
#endif /* INCLUDE_VXBUS */

#ifdef INCLUDE_RTC
#include "d1643RTC.c"                   /* Real Time Clock */
#endif	/* INCLUDE_RTC */

/*******************************************************************************
*
* sysModelGet - determine the model name of the CPU board
*
* This routine examines PVR and sets up the string to be returned by
* sysModel().  It also sets the isGpr member of the systemInfo structure.
* The SYS_MODEL define is used as the base model string.
*
* RETURNS: N/A
*/

LOCAL void sysModelGet (void)
    {
    UINT32 pvr = vxPvrGet();

    systemInfo.isGpr = FALSE;
#ifdef PPC405GP_REVB
    if (pvr == PVR_405GP_RB)
        sprintf(systemInfo.sysModelStr, SYS_MODEL " %s Rev. %s", "405GP", "B");
    else
#endif	/* PPC405GP_REVB */

#ifdef PPC405GP_REVC
    if (pvr == PVR_405GP_RC)
        sprintf(systemInfo.sysModelStr, SYS_MODEL " %s Rev. %s", "405GP", "C");
    else
#endif	/* PPC405GP_REVC */

#ifdef PPC405GP_REVD_OR_E
    if (pvr == PVR_405GP_RD)
        sprintf(systemInfo.sysModelStr, SYS_MODEL " %s Rev. %s", "405GP", "D");
    else

    if (pvr == PVR_405GP_RE)
        sprintf(systemInfo.sysModelStr, SYS_MODEL " %s Rev. %s", "405GP", "E");
    else
#endif	/* PPC405GP_REVD_OR_E */

#ifdef PPC405GPR_REVB
    if (pvr == PVR_405GPR_RB)
	{
	systemInfo.isGpr = TRUE;
	sprintf(systemInfo.sysModelStr, SYS_MODEL " %s Rev. %s", "405GPr", "1.1");
	}
    else
#endif

    /*
     * The size of sysModelStr[] is set in walnut.h.
     * Check before lengthening message.
     */

    sprintf(systemInfo.sysModelStr, "Unsupported processor, PVR=0x%x", pvr);
    }

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string "IBM PowerPC 405GP Rev. x".
*/

char * sysModel (void)
    {
    if (systemInfo.sysModelStr[0] == '\0')
	sysModelGet();
    return systemInfo.sysModelStr;
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number, for
* example, 1.1/0.  BSP_REV is concatenated to BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the IBM 405GP Walnut board. This
* routine clears and turns off the external interrupts.  It is called from
* usrInit() in usrConfig.c.
*
* NOTE: This routine should not be called directly by the user application.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {

#ifdef  PPC405GPR
    UINT32  ecr;
#endif /* PPC405GPR */
#ifdef  INCLUDE_CACHE_SUPPORT
    UINT32  memSize;
    UINT32  cacheBit;
#endif /* INCLUDE_CACHE_SUPPORT */

    /* Fill in global system information structure */

    sysInfoGet(&systemInfo, FALSE);

    /* Set the 405 Exception Vector Prefix Register */

    vxEvprSet (WALNUT_EVPR_VAL);

    /* Interrupt system initialization */

    sysUicIntrInit ();

    /*
     * Initialize timer clock mode determine the timer clock frequency
     */

#ifdef TIMER_CLOCK_EXTERNAL
    /*
     * Set the external timer clock bit in Chip Control Reg 1.
     */
    sysDcrCr1Set(sysDcrCr1Get() | CR1_EXT_TIMER_EN);
    sysTimerClkFreq = EXT_TIMER_CLK_FREQ;

#else  /* Timers run at the processor clock speed */

    sysTimerClkFreq = systemInfo.freqProcessor;

#endif


#ifdef PPC405GPR
    /*
     * If 405GPr, initialize the Chip Edge Conditioning Register to the
     * recommended value.
     */

    if (systemInfo.isGpr)
        {
        ecr = dcrInLong(CPC0_ECR);
        ecr &= ~(ECR_EMAC_RX_MASK | ECR_EMAC_TX_MASK | ECR_UIC_MASK);
        dcrOutLong(CPC0_ECR, ecr | ECR_EMAC_RX_3 | ECR_EMAC_TX_3 | ECR_UIC_3);
        }
#endif /* PPC405GPR */

/* Initialize the PCI configuration library, and the 405GP PCI bridge */

#ifdef INCLUDE_PCI
    pciIntLibInit();
    pciConfigLibInit(PCI_MECHANISM_0, (ULONG)ibmPciConfigRead,
                     (ULONG)ibmPciConfigWrite, 0);
    sysPciHostBridgeInit();
#endif	/* INCLUDE_PCI */

/* Perform auto-configuration of devices on the PCI bus  */

#ifdef INCLUDE_PCI_AUTOCONF
    sysPciAutoConfig();
#endif	/* INCLUDE_PCI_AUTOCONF */

#ifdef INCLUDE_END
    sysNetHwInit();             /* network interface */
#endif  /* INCLUDE_END */

    /* initialize the serial driver(s) */

    hardWareInterFaceInit();

    /*
     * ppc405IccrVal and ppc405DccrVal define respectively the cacheability
     * state of the total effective address space at a 128 MB granularity
     * for instruction and data (in real mode). Start with the default
     * values, then turn on the appropriate number of bits in ICCR and
     * DCCR to correspond with the amount of SDRAM detected.
     */

#ifdef  INCLUDE_CACHE_SUPPORT
    /* Start with setting the bit that controls caching between 0 and 128MB */
    cacheBit = 0x80000000;
    ppc405IccrVal = _PPC405_ICCR_DEFAULT_VAL | cacheBit;
    ppc405DccrVal = _PPC405_DCCR_DEFAULT_VAL | cacheBit;

    /* If the SDRAM is larger than 128MB keep turning on bits */
    memSize = (UINT32)sysPhysMemTop();
    while (memSize > 0x08000000)
        {
        cacheBit = cacheBit >> 1;
        ppc405IccrVal |= cacheBit;
        ppc405DccrVal |= cacheBit;
        memSize -= 0x08000000;
        }
#endif	/* INCLUDE_CACHE_SUPPORT */

    }

/*******************************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupts and configures any required features
* not configured by sysHwInit().
*
* RETURNS: N/A
*/

void sysHwInit2 (void)
    {
    static BOOL configured = FALSE;
        
    if (!configured)
        {

        vxbDevInit();

        /*
         * Connect the UIC interrupt handler to both the external interrupt
         * and the critical interrupt sources.
         */

        excIntConnect ((VOIDFUNCPTR *) _EXC_OFF_INTR, sysUicIntHandler);
        excIntCrtConnect ((VOIDFUNCPTR *) _EXC_OFF_CRTL, sysUicIntHandler);

        /* Connect system clock interrupt */

        excIntConnectTimer ((VOIDFUNCPTR *) _EXC_OFF_PIT,
                            (VOIDFUNCPTR) sysClkInt);

#ifdef INCLUDE_AUX_CLK
        /* connect auxiliary clock interrupt */

        excIntConnectTimer ((VOIDFUNCPTR *) _EXC_OFF_FIT,
                            (VOIDFUNCPTR) sysAuxClkInt);
#endif /* INCLUDE_AUX_CLK */

#ifdef INCLUDE_END
        sysNetHwInit2 ();       /* network interface */
#endif  /* INCLUDE_END */

#ifdef INCLUDE_SIO_UTILS
        sysSerialConnectAll();
#endif

#ifdef INCLUDE_VXBUS
        taskSpawn("tDevConn", 11, 0, 10000,
                  vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
#endif /* INCLUDE_VXBUS */

        configured = TRUE;
        }
    }


/*******************************************************************************
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
*/

char * sysPhysMemTop (void)
    {
    static char * physTop = NULL;
#ifdef LOCAL_MEM_AUTOSIZE
    UINT32 sdramBankCfg[4];
    int          memBank;
#endif

    if (physTop == NULL)
        {
#ifdef LOCAL_MEM_AUTOSIZE
        sdramBankCfg[0] = sysDcrSdramGet(SDRAM_B0CR);
        sdramBankCfg[1] = sysDcrSdramGet(SDRAM_B1CR);
        sdramBankCfg[2] = sysDcrSdramGet(SDRAM_B2CR);
        sdramBankCfg[3] = sysDcrSdramGet(SDRAM_B3CR);

        /*
         * Check each of the SDRAM banks in the controller. If a bank is
         * enabled get the size it is configured to and add to the total.
         */

        for (memBank = 0; memBank < 4; memBank++)
            {
            if (sdramBankCfg[memBank] & SDRAM_BXCR_SDX_BE)
                {
                switch(sdramBankCfg[memBank] & SDRAM_BXCR_SDX_SZ)
                    {
                    case SDRAM_BXCR_SDX_SZ_4:
                      physTop = (char *)(4 * 1024 * 1024);           /* 4MB */
                      break;
                    case SDRAM_BXCR_SDX_SZ_8:
                      physTop = physTop + (8 * 1024 * 1024);         /* 8MB */
                      break;
                    case SDRAM_BXCR_SDX_SZ_16:
                      physTop = physTop + (16 * 1024 * 1024);        /* 16MB */
                      break;
                    case SDRAM_BXCR_SDX_SZ_32:
                      physTop = physTop + (32 * 1024 * 1024);        /* 32MB */
                      break;
                    case SDRAM_BXCR_SDX_SZ_64:
                      physTop = physTop + (64 * 1024 * 1024);        /* 64MB */
                      break;
                    case SDRAM_BXCR_SDX_SZ_128:
                      physTop = physTop + (128 * 1024 * 1024);       /* 128MB */
                      break;
                    case SDRAM_BXCR_SDX_SZ_256:
                      physTop = physTop + (256 * 1024 * 1024);       /* 256MB */
                      break;
                    case SDRAM_BXCR_SDX_SZ_512:
                      physTop = physTop + (512 * 1024 * 1024);       /* 512MB */
                      break;
                    }
                }
            }

    /*
     * based on the size of the memory we calculated, update the sysPhysMemDesc
     * structure, so that we map the right size of RAM.
     * We assume that the first element in the sysPhysMemDesc structure is
     * referring to RAM.
     */
#if (defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL))
    sysPhysMemDesc[0].len = (UINT32)physTop - LOCAL_MEM_LOCAL_ADRS;
#endif /* INCLUDE_MMU_BASIC || INCLUDE_MMU_FULL */

#else /*LOCAL_MEM_AUTOSIZE*/

        /* Don't do autosizing, size is given */

        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

#endif /*LOCAL_MEM_AUTOSIZE*/
        }

    return physTop;
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
*/

char * sysMemTop (void)
    {
    static char * memTop = NULL;

    if (memTop == NULL)
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;
#ifdef	INCLUDE_EDR_PM
	/* account for ED&R persistent memory */
	memTop -= PM_RESERVED_MEM;
#endif	/* INCLUDE_EDR_PM */
        }

    return memTop;
    }

/********************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot()--which services ^X--and by bus errors at interrupt level.
* However, in special circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* RETURNS: OK (but only if ROM monitor returns)
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_WARM_ADRS);	/* Warm reboot entry point */

    vxMsrSet (0);			/* clear MSR */

    cacheDisable (0);			/* disable icache */
    cacheDisable (1);			/* disable dcache */

    sysDcrUicsrClear(0xffffffff);	/* clear pending interrupts */

    (*pRom) (startType);		/* jump to bootrom */

    return (OK);		/* in case we ever continue from ROM monitor */
    }

/*******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board, always 0 (zero).
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return (0);
    }

/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.
* Processor numbers should be unique on a single backplane.
*
* NOTE: This routine has no effect, since there is no VMEbus.
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet(),
*/

void sysProcNumSet
    (
    int procNum /* processor number (ignored in Walnut BSP) */
    )
    {
    sysProcNum = sysProcNumGet();
    }

/*******************************************************************************
*
* sysIntLockLevelSet - set the current interrupt lock-out level
*
* This routine sets the current interrupt lock-out level.
*
* NOTE: This routine has no effect, since the facility is currently not
* implemented.
*
* RETURNS: ERROR, always
*/

int sysIntLockLevelSet
    (
    int newLvl    /* new interrupt level */
    )
    {
    return (ERROR);
    }

/*******************************************************************************
*
* sysLocalDelay - delay of <ms_delay> milliseconds used before kernel is up
*
*
* RETURNS: N/A
*/
void sysLocalDelay
    (
    UINT32 ms_delay
    )
    {
    UINT32 start_upper;
    UINT32 start_lower;
    UINT32 end_upper;
    UINT32 end_lower;
    UINT32 upper;
    UINT32 lower;
    UINT32 delay;
    BOOL timesup = FALSE;

    /*
     *  Read the timebase twice to start
     */
    vxTimeBaseGet (&start_upper, &start_lower);
    vxTimeBaseGet (&end_upper, &end_lower);

    while (timesup == FALSE)
        {
        /*
         * Determine if the delay has been long enough
         */
        upper = end_upper - start_upper;
        if (start_lower > end_lower)
            upper--;
        lower = end_lower - start_lower;

        delay =  1000 * upper * (0xFFFFFFFF / sysTimerClkFreq);
        delay += lower / (sysTimerClkFreq / 1000);

        if (delay > ms_delay)
            {
            timesup = TRUE;
            }
            else
            {
            vxTimeBaseGet (&end_upper, &end_lower);
            }
        }
    return;
    }


/*******************************************************************************
*
* sysInfoGet - Determines system information based on board switch settings.
*
* This routine returns information about the current operating environment
* of the board.  Optionally, it prints the information out.
*
* RETURNS: OK or ERROR
*/

STATUS sysInfoGet
    (
    SYS_INFO    * sysInfo,
    int           verbose
    )
    {
    UINT32  pllmr;
    UINT32  psr;
    UINT32  fpga;
    UINT32  sysClkPeriodNs = ONE_BILLION / SYS_CLK_FREQ;
    UINT32  feedbackSource;
    char *  feedbackStr="";
    BOOL    bypassMode = FALSE;
    UINT32  multM = 1;
    UINT32  vcoMin = VCO_MIN_GP;	/* Initialize to 405GP setting */
    UINT32  vcoMax = VCO_MAX_GP;	/* Initialize to 405GP setting */

    bzero((char *)sysInfo, sizeof(SYS_INFO));

    /* set sysInfo->isGpr appropriately */
    sysModelGet();

    /*
     * Read PLL Mode register
     */
    pllmr = dcrInLong(CPC0_PLLMR);

    /*
     * Read the pin strapping register
     */
    psr = dcrInLong(CPC0_PSR);

    if (verbose)
        printf("\n");

    /*
     * Determine FWD_DIV for 405GP, or FWD_DIVA for 405GPr.
     */

    sysInfo->pllFwdDiv = 8 - ((pllmr & PLLMR_FWD_DIV_MASK) >> PLLMR_FWD_DIV_BIT_SHIFT);

#ifdef PPC405GPR
    /*
     * Determine FWD_DIVB for 405GPr.
     */
    if (sysInfo->isGpr)
	sysInfo->pllFwdDivB = 8 - ((pllmr & PLLMR_FWD_DIVB_MASK) >> PLLMR_FWD_DIVB_BIT_SHIFT);
#endif

    /*
     * Determine FBK_DIV.
     */

    sysInfo->pllFbkDiv = (pllmr & PLLMR_FB_DIV_MASK) >> PLLMR_FB_DIV_BIT_SHIFT;
    if (sysInfo->pllFbkDiv == 0)
        sysInfo->pllFbkDiv = 16;

    /*
     * Determine PLB_DIV.
     */

    sysInfo->pllPlbDiv = 1 + ((pllmr & PLLMR_CPU_TO_PLB_MASK) >> PLLMR_CPU_TO_PLB_BIT_SHIFT);

    /*
     * Determine PCI_DIV.
     */

    sysInfo->pllPciDiv = 1 + ((pllmr & PLLMR_PCI_TO_PLB_MASK) >> PLLMR_PCI_TO_PLB_BIT_SHIFT);

    /*
     * Determine EXTBUS_DIV.
     */

    sysInfo->pllExtBusDiv = 2 + ((pllmr & PLLMR_EXB_TO_PLB_MASK) >> PLLMR_EXB_TO_PLB_BIT_SHIFT);

    /*
     * Determine OPB_DIV.
     */

    sysInfo->pllOpbDiv = 1 + ((pllmr & PLLMR_OPB_TO_PLB_MASK) >> PLLMR_OPB_TO_PLB_BIT_SHIFT);

    /*
     * Determine the source of the feedback clock.  For 405GP it is always the
     * the PLB clock.  For 405GPr it depends on whether the chip is running
     * in legacy mode or new mode, and the following strap values:
     *       CPC0_PSR[PAME] - PCI async mode enable
     *       CPC0_PSR[EBDP] - PLB to external bus divisor
     *       CPC0_PSR[PFBD] - PLL Feedback divisor
     *       CPC0_PSR[ESME] - External bus sync mode enable
     */

    feedbackSource = PLB_FDBK;

#ifdef PPC405GPR
    if (sysInfo->isGpr)
        {
        /*
         * See Feedback Selection table
         * in clocking chapter of 405GPr User Manual
         */

        if ((psr & PSR_PCI_ASYNC_EN) == 0)      /* Synchronous PCI  */
            feedbackSource = PCI_FDBK;
        else                        /* Asynchronous PCI */
            {
            if (psr & PSR_NEW_MODE)         /* New Mode     */
                {
                if (psr & PSR_PERCLK_SYNC)
                    feedbackSource = EBC_FDBK;
                else
                    feedbackSource = CPU_FDBK;
                }
            else                    /* Legacy Mode  */
                {
                if (sysInfo->pllFbkDiv == sysInfo->pllExtBusDiv)
                    feedbackSource = EBC_FDBK;
                else
                    feedbackSource = PLB_FDBK;
                }
            }
        }
#endif  /* PPC405GPR */

    /*
     * Check to see if the chip is running in PLL bypass mode.
     *    405GP  : forward divisor = 1
     *    405GPr : bypass bit is set in the PLLMR register
     */
#ifdef PPC405GP
    if ((!sysInfo->isGpr) && sysInfo->pllFwdDiv == 1)
        bypassMode = TRUE;
#endif	/* PPC405GP */
#ifdef PPC405GPR
    if (sysInfo->isGpr)
        {
        vcoMin = VCO_MIN_GPR;
        vcoMax = VCO_MAX_GPR;
        if (pllmr & PLLMR_BYPASS_MODE)
            bypassMode = TRUE;
        }
#endif  /* PPC405GPR */

    /*
     * If running in bypass mode the CPU speed is equal to the SYS_CLK_FREQ.
     * If not in bypass mode, check VCO to make sure it is within range.
     *    spec:    VCO = SYS_CLOCK x multM
     */

    if (bypassMode)
        {
        sysInfo->freqVCOMhz = 1000 / sysClkPeriodNs;
        sysInfo->freqProcessor = SYS_CLK_FREQ;
        sysInfo->freqPLB = SYS_CLK_FREQ/sysInfo->pllPlbDiv;
        }
    else
        {
        switch (feedbackSource)
            {
            case PLB_FDBK :
                feedbackStr = "PLB";
#ifdef PPC405GPR
		if (sysInfo->isGpr)
		    multM = sysInfo->pllFbkDiv * sysInfo->pllFwdDivB *
			sysInfo->pllPlbDiv;
		else
#endif	/* PPC405GPR */
		    multM = sysInfo->pllFbkDiv * sysInfo->pllFwdDiv *
			sysInfo->pllPlbDiv;
                break;
#ifdef PPC405GPR
            case PCI_FDBK :
                /* Assume the feedback divisor is forced to 1 */
                multM = sysInfo->pllFwdDivB * sysInfo->pllPlbDiv *
                        sysInfo->pllPciDiv;
                feedbackStr = "PCI";
                break;
            case EBC_FDBK :
                /* Assume the feedback divisor is forced to 1 */
                multM = sysInfo->pllFwdDivB * sysInfo->pllPlbDiv *
                        sysInfo->pllExtBusDiv;
                feedbackStr = "EBC";
                break;
            case CPU_FDBK :
                multM = sysInfo->pllFbkDiv * sysInfo->pllFwdDiv;
                feedbackStr = "CPU";
                break;
#endif /* PPC405GPR */
            default:
                break;
            }

        /*
         * Calculate PLL VCO speed in MHz
         *     vco = multM * SYS_CLK_FREQ
         *     Use slightly different method to avoid
         *     rounding error.
         */
        sysInfo->freqVCOMhz = (1000 * multM) / sysClkPeriodNs;

        if (sysInfo->freqVCOMhz >= vcoMin && sysInfo->freqVCOMhz <= vcoMax)
            {
            sysInfo->freqProcessor = (sysInfo->freqVCOMhz * 1000000)
                                        / sysInfo->pllFwdDiv;
#ifdef PPC405GPR
	    if (sysInfo->isGpr)
		sysInfo->freqPLB = (sysInfo->freqVCOMhz * 1000000)
				   / (sysInfo->pllFwdDivB * sysInfo->pllPlbDiv);
	    else
#endif  /* PPC405GPR */
		sysInfo->freqPLB = (sysInfo->freqVCOMhz * 1000000)
				   / (sysInfo->pllFwdDiv * sysInfo->pllPlbDiv);
            }
        else
            {
            if (verbose)
                {
                printf("Invalid VCO frequency calculated :  %d MHz \a\n",
                       sysInfo->freqVCOMhz);
                printf("It must be between %d-%d MHz \a\n", vcoMin, vcoMax);
                printf("PLL Mode reg           :  %8.8x\a\n", pllmr);
                }
            return(ERROR);
            }
        }

    /*
     * Output info.
     */

    if (verbose)
        {
        printf(" %s\n", sysModel());

#ifdef PPC405GPR
	if (sysInfo->isGpr)
	    {
	    if (psr & PSR_NEW_MODE)
		printf(" New Mode\n");
	    else
		printf(" Legacy Mode\n");
	    }
#endif   /* PPC405GPR */

        printf(" PLL VCO speed     = %d MHz\n", sysInfo->freqVCOMhz);
        printf(" Feedback source   = %s clock \n", feedbackStr);
        printf(" Processor speed   = %d MHz\n", sysInfo->freqProcessor/1000000);
        printf(" PLB speed         = %d MHz\n", sysInfo->freqPLB / 1000000);
        printf(" OPB speed         = %d MHz\n",
                           sysInfo->freqPLB / sysInfo->pllOpbDiv / 1000000);
        printf(" Ext Bus speed     = %d MHz\n",
                           sysInfo->freqPLB / sysInfo->pllExtBusDiv / 1000000);
        }

    /*
     * Read the STRAP (PSR) register to determine if PCI clock is async or
     * sync.  If async, read FPGA reg to determine PCI clock source; either
     * derived internally on-board or supplied via external source.  If
     * derived on-board, check FPGA bits to determine speed. If external,
     * we can not determine the speed.
     */

    if (psr & PSR_PCI_ASYNC_EN)
        {
#ifdef PPC405GP
	if (!sysInfo->isGpr)
	    {
	    /* Code specific to the 405GP Walnut board */

	    fpga = sysInByte(FPGA_BRDS2);
            if (fpga & SW_CLK_SRC1)
                {
                switch (fpga & FSEL_A)
                    {
                    case FSEL_INVALID1:
                    case FSEL_INVALID2:
                        if (verbose)
                            {
                            printf(" PCI is in Async Internal mode, but\n");
                            printf(" FSEL_A0 & FSEL_A1 bit settings to\n");
                            printf(" select the PCI speed are invalid.\n");
                            printf(" Cannot determine the PCI bus speed.\n");
                            }
                        return(ERROR);
                        break;
                    case FSEL_66:
                        sysInfo->freqPCI=66666666;
                        break;
                    case FSEL_33:
                        sysInfo->freqPCI=33333333;
                        break;
                    }
                if (verbose)
                    {
                    printf(" PCI Bus speed     = %d MHz (Async)\n",
                           sysInfo->freqPCI / 1000000);
                    }
                }
            else
                {                             /* must be async internal clock */
                if (verbose)
                    printf(" PCI Async Ext Clock being used.\n");
                }
            }
#endif	/* PPC405GP */

#ifdef PPC405GPR
        if (sysInfo->isGpr)
            {
            /* Code specific to the 405GPr Sycamore board */

            fpga = sysInByte(FPGA_BRDS1);

            if (fpga & PCI_CLK_SEL)
                {
                /*
                 * PCI Async Internal clock is being used.
                 * Look at the status of M66EN and the force 33 bits
                 * to determine the speed of the PCI clock.
                 */
                if ((fpga & (PCI_M66EN | PCI_FORCE_33)) == PCI_M66EN)
                    sysInfo->freqPCI=66666666;
                else
                    sysInfo->freqPCI=33333333;
                }
            if (verbose)
                {
                printf(" PCI Bus speed     = %d MHz (Async)\n",
                       sysInfo->freqPCI / 1000000);
                }
            else
                {                             /* must be async internal clock */
                if (verbose)
                    printf(" PCI Async Ext Clock being used.\n");

                }
            }
#endif  /* PPC405GPR */
        }
    else         /* PCI clock is synchronous */
        {
        sysInfo->freqPCI = sysInfo->freqPLB / sysInfo->pllPciDiv;
        sysInfo->pciClkSync = TRUE;
        if (verbose)
            printf(" PCI Bus speed     = %d MHz (Sync)\n",
                   sysInfo->freqPCI / 1000000);
        if (sysInfo->freqPCI > 33333333)
            {
            if (verbose)
                printf("\n PCI Speed should not be above 33Mhz in Sync mode.\n");
            return(ERROR);
            }
        }

    /*
     * Check strapping via STRAP reg (PSR) to determine which of the two PCI
     * arbiters is enabled.
     */

    if (psr & PSR_PCI_ARBIT_EN)
        {
        sysInfo->pciIntArbEn = TRUE;
        if (verbose)
            printf(" Internal PCI arbiter enabled\n\n");
        }
    else
        {
        sysInfo->pciIntArbEn = FALSE;
        if (verbose)
            printf(" External PCI arbiter enabled\n\n");
        }

    return(OK);
    }

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
* RETURNS: SIO_CHAN *
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

#ifdef INCLUDE_SIO_UTILS

/***********************************************************************
*
* vxbSysGetClkFreq - determine CPU clock to UART clock divisor
*
* This routine determines the optimal CPU clock to UART clock divisor for a
* given CPU frequency and desired baud rate.  This function is required only
* when the serial clock is being derived from the CPU clock.
*
* RETURNS: The optimal CPU to UART clock divisor, or ERROR
*
* ERRNO
*/
UINT32 vxbSysGetClkFreq
    (
    struct vxbDev * pDev
    )
    {
    UINT32  cr0;
#ifndef UART_CLOCK_EXTERNAL    
    int     i;
    int     numEntries;
    UINT    divisor;
#endif

    /*
     * Disable and clear serial interrupts in the UIC for both UARTs
     */
     
    intDisable (INT_LVL_UART0);
    intDisable (INT_LVL_UART1);

    /*
     * Set the CPC0_CR0 register for internal or external UART clock source.
     * Set UART1 to work in CTS/RTS mode by turning on the CTS enable bit
     * in the Chip Control Reg 0 plus setting the DEFAULT_UART_1 control
     * bit in the Walnut board's FPGA_BRDC register.  The FPGA routes the
     * correct signal from the connector to the chip.  Other boards sharing
     * this file (wrSbc405gp) may not implement FPGA_BRDC.
     */

#ifdef UART_CLOCK_EXTERNAL
    cr0 = sysDcrCr0Get ();
    sysDcrCr0Set (cr0 | CR0_UART1_CTS_EN | 
                  CR0_UART0_EXT_CLK | CR0_UART1_EXT_CLK);
    return EXT_SER_CLK_FREQ;
#else    /* UART_CLOCK_EXTERNAL */

    numEntries = sizeof(uartClkTbl) / sizeof(INT_SERIAL_INFO);

    /* CPU to UART clock divisor is determined and placed into CPC0_CR0 */
    
    cr0 = sysDcrCr0Get ();
    cr0 &= ~(CR0_UART_DIV_MASK | CR0_UART0_EXT_CLK | CR0_UART1_EXT_CLK );
    
    /*
     * Search the table of processor frequencies and Baud rates to determine
     * the optimal divisor for the UART clock speed
     */

    for (i=0; i < numEntries; i++)
        {
        if ((uartClkTbl[i].freqProcMhz == (systemInfo.freqProcessor / 1000000)) &&
            (uartClkTbl[i].baudRate == CONSOLE_BAUD_RATE))
            {
            divisor = uartClkTbl[i].uartClkDivisor;
            break;
            }
        }

    sysDcrCr0Set (cr0 | CR0_UART1_CTS_EN | ((divisor - 1) << 1));
    return systemInfo.freqProcessor / divisor;

#endif /* UART_CLOCK_EXTERNAL */
    }

#endif /* INCLUDE_SIO_UTILS */

