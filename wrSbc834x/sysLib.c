/* sysLib.c - Wind River SBC834x board system-dependent library */

/*
 * Copyright (c) 2005-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01t,13jan10,yjw  Remove legacy PCI driver. (WIND00183778)
01s,27jul09,fao  Fixed sysUsDelay accuracy. (WIND00173100)
01r,06jul09,z_l  Remove legacy PCI driver. (WIND00088032)
01q,14apr09,x_z  remove _MMU_UBAT_VP from all static BAT items.(WIND00160963)
01p,03dec08,syt  modified function sysMsDelay for romResident image boot failure.
                 (WIND00128752)
01o,13oct08,x_z  Add embedded-TT support for USB. (WIND00134142)
01n,13jun08,wap  Switch to new VxBus TSEC driver
01m,07sep07,ami  Plb-Based Ehci Controller made vxBus Compliant
01l,28aug07,spw  Change init order of WLAN components
01k,16aug07,mmi  remove call to vxPowerModeSet() since deprecated, fixed
                 #else if to become #elif 
01j,15aug07,spw  Add WLAN support for Broadcom 43XX
01i,08aug07,agf  remove duplicate variable definitions
01h,09jul07,pdg  added vxbus support
01g,12mar07,spw  Add forward declaration for sysDot11BusInit 
01f,16jan07,b_m  WIND00065649, modify sysModel() to use SYS_MODEL.
01f,13sep06,tfo  WLAN support for Atheros AR521X
01e,25aug06,dtr  WIND0061424 - CPU 1 errata causes intermittent crash.
                 WIND0061584 - default image doesn't boot
01d,08mar06,kds  Set optimal SCCR settings for TSEC/USB and SEC engine. Fix
                 PVR check. Add USB support.
01c,28oct05,mdo  SPR#114197 - protect against multiple defines for
                 INCLUDE_PCICFG_SHOW
01b,22jul05,j_b  remove FEI END support
01a,21jun05,j_b  adapted from ads834x (rev 01e); add NV RAM support
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

                        
INCLUDE FILES:

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <vxWorks.h>
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
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>

#include <miiLib.h>

#ifdef INCLUDE_VXBUS
#include "hwif/vxbus/vxBus.h"
#include "hwconf.c"
IMPORT  void    hardWareInterFaceInit (void);
#endif /* INCLUDE_VXBUS */
	    

/* globals */

/*
 * sysBatDesc[] is used to initialize the block address translation (BAT)
 * registers within the PowerPC 603/604 MMU.  BAT hits take precedence
 * over Page Table Entry (PTE) hits and are faster.  Overlap of memory
 * coverage by BATs and PTEs is permitted in cases where either the IBATs
 * or the DBATs do not provide the necessary mapping (PTEs apply to both
 * instruction AND data space, without distinction).
 *
 * The primary means of memory control for VxWorks is the MMU PTE support
 * provided by vmLib and cacheLib.  Use of BAT registers will conflict
 * with vmLib support.  User's may use BAT registers for i/o mapping and
 * other purposes but are cautioned that conflicts with caching and mapping
 * through vmLib may arise.  Be aware that memory spaces mapped through a BAT
 * are not mapped by a PTE and any vmLib() or cacheLib() operations on such
 * areas will not be effective, nor will they report any error conditions.
 *
 * Note: BAT registers CANNOT be disabled - they are always active.
 * For example, setting them all to zero will yield four identical data
 * and instruction memory spaces starting at local address zero, each 128KB
 * in size, and each set as write-back and cache-enabled.  Hence, the BAT regs
 * MUST be configured carefully.
 *
 * With this in mind, it is recommended that the BAT registers be used
 * to map LARGE memory areas external to the processor if possible.
 * If not possible, map sections of high RAM and/or PROM space where
 * fine grained control of memory access is not needed.  This has the
 * beneficial effects of reducing PTE table size (8 bytes per 4k page)
 * and increasing the speed of access to the largest possible memory space.
 * Use the PTE table only for memory which needs fine grained (4KB pages)
 * control or which is too small to be mapped by the BAT regs.
 *
 * All BATs point to PROM/FLASH memory so that end customer may configure
 * them as required.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT + _MMU_NUM_EXTRA_IBAT + _MMU_NUM_EXTRA_DBAT)] =
    {
    /* I BAT 0 */
    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* I BAT 1 */

    0, 0,

    /* I BAT 2 */

    0, 0,

    /* I BAT 3 */

    0, 0,

    /* D BAT 0 */

    0, 0,

    /* D BAT 1 */

    /* use DBAT1 to map CPM DPRAM and internal registers into data space */
    /* NOTE! the internal space cannot be cached and should be guarded */
    ((CCSBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_128K |
    _MMU_UBAT_VS),
    ((CCSBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 */

    ((0x80000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((0x80000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 4 */

    ((0x90000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((0x90000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /*
     * These entries are for the I/D BATs (4-7) on the MPC7455/755/834x/8272.
     * They should be defined in the following order.
     * IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
     * DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
     */

    /* I BAT 4 */

    ((LOCAL_MEM_LOCAL_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M |
    _MMU_UBAT_VS),
    ((LOCAL_MEM_LOCAL_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_MEM_COHERENT),


    /* I BAT 5 */

    0, 0,

    /* I BAT 6 */

    0, 0,

    /* I BAT 7 */

    0, 0,

    /* D BAT 4 */

    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_8M |
    _MMU_UBAT_VS),
    ((FLASH_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 5 */

    0, 0,

    /* D BAT 6 */

    0, 0,

    /* D BAT 7 */

    0, 0


    };

/*
 * mmuPcpBatInitMPC7x5 initializes the standard 4 (0-3) I/D BATs &
 * the additional 4 (4-7) I/D BATs present on the MPC74[45]5 & MPC83xx.
 */

IMPORT void mmuPpcBatInitMPC7x5(UINT32 *pSysBatDesc); /* MPC7x5 and 83xx BAT Support */
IMPORT FUNCPTR _pSysBatInitFunc;

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, EEPROM, 
 * the MPC834x Internal Memory Map, and local FLASH RAM are set here.
 *
 * PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
 * integer powers of two based on amount of memory to be mapped and a
 * minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
 * for 32-bit PowerPCs are:
 *
 * Total mapped memory      Page Table size
 * -------------------      ---------------
 *        8 Meg              64 K
 *       16 Meg             128 K
 *       32 Meg             256 K
 *       64 Meg             512 K
 *      128 Meg               1 Meg
 *  .               .
 *  .               .
 *  .               .
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 *
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {

    {
    /* Vector Table and Interrupt Stack */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_GUARDED |
    VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_GUARDED |
    VM_STATE_CACHEABLE_NOT
    },

    {
    /* Vector Table and Interrupt Stack */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS + RAM_LOW_ADRS,
    LOCAL_MEM_SIZE - RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    },

#ifdef INCLUDE_SECONDARY_DRAM
    {
    /* Vector Table and Interrupt Stack */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS2,
    LOCAL_MEM_SIZE2,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    },
#endif

    {
    /* EEPROM */

    (VIRT_ADDR) EEPROM_BASE_ADRS,
    (PHYS_ADDR) EEPROM_BASE_ADRS,
    EEPROM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    }

    ,
    {
    /* MPC8349 Internal Memory Map */

    (VIRT_ADDR) CCSBAR,
    (PHYS_ADDR) CCSBAR,
    CCSBAR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    }

    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int   sysBus      = BUS;                /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (PPC8260) */
char *sysBootLine = BOOT_LINE_ADRS; /* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;   /* catastrophic message area */
int   sysProcNum;           /* processor number of this CPU */
BOOL  sysVmeEnable = FALSE;     /* by default no VME */
UINT32   baudRateGenClk;
LOCAL UINT32 sysClkFreq;

/* forward declarations */

uint32_t sysBaudClkFreq(void);
UINT32   sysClkFreqGet();

void   sysLedClkRoutine (int arg);
STATUS sysSwitchRead(int);

#ifdef INCLUDE_AR521X_END
  void sysDot11BusInit(void);
#endif /* INCLUDE_AR521X_END */

#ifdef INCLUDE_BCM43XX_END
	void sysBroadComWLBusInit(void);
#endif /* INCLUDE_BCM43XX_END */

#include "sysDuart.c"
#include <intrCtl/quiccIntrCtl.c>

#include <timer/quiccTimer.c>

#if (NV_RAM_SIZE == NONE)
#  include <mem/nullNvRam.c>
#else
#  include "eeprom.c"
#  include <mem/byteNvRam.c>
#endif

#ifdef INCLUDE_SYSLED
#  include "sysLed.c"
#endif /* INCLUDE_SYSLED */

/* defines */

#define ZERO    0

/* Set ACR to park on PCI bus */
#define M83XX_ACR_PARKM_PCI 0x1

/* needed to enable timer base */
#ifdef INCLUDE_PCI
#  define      M8260_DPPC_MASK    0x0C000000 /* bits 4 and 5 */
#  define      M8260_DPPC_VALUE   0x0C000000 /* bits (4,5) should be (1,0) */
#else
#  define      M8260_DPPC_MASK    0x0C000000 /* bits 4 and 5 */
#  define      M8260_DPPC_VALUE   0x08000000 /* bits (4,5) should be (1,0) */
#endif /*INCLUDE_PCI */


/* network support */
#ifdef INCLUDE_END
#  include "sysNet.c"

#endif /* INCLUDE_END */

#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND
  UCHAR   sysInByte(ULONG port)
      {
      return(pciBridgeRegisterReadByte(port));
      }

  USHORT  sysInWord(ULONG port)
      {
      return(pciBridgeRegisterReadWord(port));
      }

  ULONG   sysInLong(ULONG port)
      {
      return(pciBridgeRegisterReadLong(port));
      }
#endif /*PCI_BRIDGE_READ_ERRATA_WORKAROUND */

/******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*/

char * sysModel (void)
    {
    if ((vxPvrGet() & 0xffff0000) == 0x80830000)
    {
    UINT32 temp;
    temp = vxSvrGet();
    switch (temp & 0xffff0000)
        {
        case 0x80500000:
            return (SYS_MODEL "9E");
        case 0x80510000:
            return (SYS_MODEL "9");
        case 0x80520000:
            return (SYS_MODEL "7E TBGA");
        case 0x80530000:
            return (SYS_MODEL "7 TBGA");
        case 0x80540000:
            return (SYS_MODEL "7E PBGA");
        case 0x80550000:
            return (SYS_MODEL "7 PBGA");
        case 0x80560000:
            return (SYS_MODEL "3E");
        case 0x80570000:
            return (SYS_MODEL "3");
        default:
            return (SYS_MODEL "x - Unknown version");
        }
    }
    else
    {
    return ("Unknown processor");
    }
    }

/******************************************************************************
*
* sysBspRev - return the bsp version with the revision eg 1.0/<x>
*
* This function returns a pointer to a bsp version with the revision.
* for eg. 1.0/<x>. BSP_REV defined in config.h is concatenated to
* BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

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
*/

void sysHwMemInit (void)
    {
    /* Call sysPhysMemTop() to do memory autosizing if available */

    sysPhysMemDesc[1].len = (UINT)(sysPhysMemTop () -  RAM_LOW_ADRS);


    }

/******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the SBC834x board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*/

void sysHwInit (void)
    {
    
    /* This is a workaround for errata PCI9 */
#ifdef INCLUDE_PCI
    *M83XX_ACR(CCSBAR) |= M83XX_ACR_PARKM_PCI;  /* Park on PCI */
#else
    *M83XX_ACR(CCSBAR) &= ~M83XX_ACR_PARKM_PCI; /* Park PPC core */
#endif
	    

    /* Initialize Machine Check pin */
    vxHid0Set(0x80000000);

    vxHid2Set(vxHid2Get()|0x04e00000);
    vxMsrSet(vxMsrGet()|0x00000002);

    /* Set TSEC/CSB 1:1 clock ratio for performance */
    *M83XX_SCCR(CCSBAR) &= 0x5fffffff;

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Enable the e300 core timebase */
    *M83XX_SPCR(CCSBAR) |= M83XX_SPCR_TBEN;

    sysDuartHwInit();


    /* put correct memory size in sysPhysMemDesc[1].len for
     * size of local memory
     */
    sysHwMemInit();

    /* set the TSEC/SEC/USB/PCI 1-1 enabled clocking with CSB */

    /* Do we need to set these and if so in romInit.s */

    CACHE_PIPE_FLUSH();

    /* Get the Baud Rate Generator Clock  frequency */

    baudRateGenClk = sysBaudClkFreq();
    sysClkFreq = sysClkFreqGet();

#ifdef INCLUDE_VXBUS
    hardWareInterFaceInit();
#endif

    /* Initialize interrupts */

    quiccIntrInit();


#ifdef INCLUDE_SECURITY_ENGINE
    /* Initialize the Security Block */
    *QUICC_SECMR (CCSBAR) = 0xfffe0000;
    *QUICC_SECBR (CCSBAR) = SEC_ENG_BASE_ADRS | 0x1;

    /* SEC/CSB Engine 1:1 clk ratio for performance and disable DR clock */
    *M83XX_SCCR(CCSBAR) &= 0xfdffffff;
#endif /* INCLUDE_SECURITY_ENGINE */

    }


UINT32 spmfTable[] =
    {16, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ,14, 15};


uint32_t sysBaudClkFreq(void)
    {
    /* Temp until better solution found */
    return(sysClkFreqGet());
    }

UINT32 sysClkFreqGet()
    {
    /* Temp base freq until better solution found */
    UINT32 baseFreq = OSCILLATOR_FREQ;
    UINT32 csb,lbiu,ddr,spmf,clkDiv,clkPll,coreFreq,tempVal;

    /* Read dip switches*/
    tempVal = *M83XX_SPMR(CCSBAR);
    lbiu = M83XX_SPMR_LBIU_VAL(tempVal);
    ddr = M83XX_SPMR_DDR_VAL(tempVal);
    spmf = M83XX_SPMR_SPMF_VAL(tempVal);
    clkDiv = M83XX_SPMR_CLK_DIV(tempVal);
    clkPll = M83XX_SPMR_CLK_PLL(tempVal);

    /* Calculate Freq of Core TBD*/
    csb  = baseFreq * spmfTable[spmf];
    coreFreq = csb * 0x3 /* TBD */;

    return(csb);

    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Determine installed memory by reading memory control registers
* and calculating if one or 2 chip selects are used for SDRAM.
* Use the address mask and valid bit to determine each bank size.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    LOCAL char * physTop = NULL;

    physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

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
*/

char * sysMemTop (void)
    {
    LOCAL char * memTop = NULL;

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
*/

STATUS sysToMonitor
    (
     int startType  /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 8);   /* Warm reboot */

    intLock();

    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif

#if (defined(INCLUDE_MPC834X_USB) && defined(INCLUDE_USB))
 
    /* Make sure the USB controller is stopped. */
    *M83XX_USBCMD(CCSBAR) &= LONGSWAP(~0x1) ;

#endif
										
    vxMsrSet (0);

    (*pRom) (startType);    /* jump to bootrom entry point */

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
*/

void sysHwInit2 (void)
    {
    LOCAL BOOL configured = FALSE;

    if (!configured)
        {

#ifdef INCLUDE_VXBUS
       vxbDevInit();
#endif
        /* initialize serial interrupts */
        sysSerialHwInit2();

        configured = TRUE;

        /* Configure GPIO for user LEDs (output) and user switches (input) */
        *M83XX_GP1DIR(CCSBAR) = (0x0FF << 20);

        sysMsDelay(1);

/* Atheros AR521X WLAN Support */
#ifdef INCLUDE_AR521X_END
    sysDot11BusInit();
#endif /* INCLUDE_AR521X_END */

/* Broadcom BCM43XX WLAN Support */
#ifdef INCLUDE_BCM43XX_END
    sysBroadComWLBusInit();
#endif /* INCLUDE_BCM43XX_END */

#ifdef INCLUDE_SYSLED

        /* cascade the user LEDs */
        sysLedsCascade();

#  if defined(INCLUDE_AUX_CLK)
        /*
         * initialize and start auxiliary clock support
         */
        sysAuxClkEnable ();

        sysAuxClkConnect((FUNCPTR)sysLedClkRoutine, 0);

#  endif /* INCLUDE_AUX_CLK */

#endif /* INCLUDE_SYSLED */

#ifdef INCLUDE_SYSLED
        sysLedHex(0xA);
#endif /* INCLUDE_SYSLED */

#ifdef INCLUDE_VXBUS
    taskSpawn("tDevConn", 11, 0, 10000,
              vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
#endif /* INCLUDE_VXBUS */
        }
    }

/*******************************************************************************
*
* sysUsDelay - delay for at least x microseconds.
*
* Use the timebase register, running at csb, to delay at least
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
* Do not call before sysHwInit determines csb.
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

    /* 
     * Calculate number of ticks for desired elapsed time. Note that the time
     * base register is incremented once every four bus clock cycles
     */

    ticks = delay * ((sysClkFreq + 1000000) / 1000000 / 4);

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

/*******************************************************************************
*
* sysMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by calling sysUsDelay
* with (delay*1000).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysMsDelay
    (
    uint32_t        delay                   /* length of time in MS to delay */
    )
    {
    sysUsDelay ( (UINT32) delay * 1000 );
    }

/*********************************************************************
*
* sysDelay - fixed 1ms delay
*
* This routine calls sysMsDelay.
*
* RETURNS: N/A
*
*/

void sysDelay (void)
    {
    sysMsDelay (1);
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
* Not applicable for the busless 8260Ads.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*
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
* Not applicable for this BSP.
*
* RETURNS: ERROR, always.
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
    return (ERROR);
    }

/******************************************************************************
*
* sysBusToLocalAdrs - convert a bus address to a local address
*
* This routine gets the local address that accesses a specified VMEbus
* physical memory address.
*
* Not applicable for this BSP.
*
* RETURNS: ERROR, always.
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
    return (ERROR);
    }

/******************************************************************************
*
* sysBusTas - test and set a location across the bus
*
* This routine does an atomic test-and-set operation across the backplane.
*
* Not applicable for this BSP.
*
* RETURNS: FALSE, always.
*
* SEE ALSO: vxTas()
*/

BOOL sysBusTas
    (
     char * adrs        /* address to be tested-and-set */
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
*/

void sysBusClearTas
    (
     volatile char * address    /* address to be tested-and-cleared */
    )
    {
    }

#ifdef INCLUDE_SYSLED

/***********************************************************************
*
* sysLedClkRoutine - shows clock activity on LED
*
* This routine blinks the dot on the Hex LED
*
* RETURNS: N/A
*
* ERRNO
*/

void sysLedClkRoutine
    (
    int arg
    )
    {
    static int clkIntCount = 0;
    static INT8 clkCount = 0;

    if((clkIntCount++ % 60) == 0)
        {
        (clkCount++ & 0x1) ?
            sysLedSet(LED_POINT, LED_POINT) : sysLedSet(LED_POINT, 0);
        }
    }

#endif /* INCLUDE_SYSLED */

/*******************************************************************************
*
* sysSwitchRead - read the status of one of the four user switches
*
*
* \NOMANUAL
*
* RETURNS: OK
*/

STATUS sysSwitchRead
    (
    int switchNo
    )
    {
    int switchStatus;


    switch (switchNo)
        {
        case 1:
            switchStatus = ((*M83XX_GP1DAT(CCSBAR) & 0x80000000) >> 31);
            break;

        case 2:
            switchStatus = ((*M83XX_GP1DAT(CCSBAR) & 0x40000000) >> 30);
            break;

        case 3:
            switchStatus = ((*M83XX_GP1DAT(CCSBAR) & 0x20000000) >> 29);
            break;

        case 4:
            switchStatus = ((*M83XX_GP1DAT(CCSBAR) & 0x10000000) >> 28);
            break;

        default:
            printf("Invalid user switch number: %d. Only switches 1-4 are available.\n", switchNo);
            return(ERROR);

        }

    if (switchStatus == 1)
       printf("User switch %d is ON\n", switchNo);
    else
       printf("User switch %d is OFF\n", switchNo);

    return(switchStatus);

    }

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPciIntRoute [NUM_PCI_SLOTS][4] = {
    {PCI_XINT1_LVL, PCI_XINT2_LVL, PCI_XINT3_LVL, PCI_XINT4_LVL}, /* slot 1 */
};

/*******************************************************************************
*
* sysPciAutoconfigInclude - PCI autoconfig support routine
*
* RETURNS: OK or ERROR for the MPC106 or WINBOND devices.
*/

STATUS sysPciAutoConfigInclude
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UINT devVend            /* deviceID/vendorID of device */
    )
    {
    /* Don't want to auto configure the bridge */
    if ((pLoc->bus > 0) ||
        (pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
        return(ERROR);

    return OK; /* Autoconfigure all devices */
    }

/*******************************************************************************
*
* sysPciAutoconfigIntrAssign - PCI autoconfig support routine
*
* RETURNS: PCI interrupt line number given pin mask
*/

UCHAR sysPciAutoConfigIntrAssign
    (
    PCI_SYSTEM * pSys,          /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,         /* pointer to function in question */
    UCHAR pin               /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    /*
     * Ensure this is a resonable value for bus zero.
     * If OK, return INT level, else we return 0xff.
     */
    if (((pin > 0) && (pin < 5))                    &&
    (((pLoc->device) - PCI_SLOT1_DEVNO) < NUM_PCI_SLOTS)    &&
    (((pLoc->device) - PCI_SLOT1_DEVNO) >= 0))
    {
    tmpChar =
        sysPciIntRoute [((pLoc->device) - PCI_SLOT1_DEVNO)][(pin-1)] + INUM_IRQ0;
    }

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }
#endif

#if defined(INCLUDE_USB) && defined(INCLUDE_EHCI)
/******************************************************************************
*
* sysM834xEhciInit - intializes the on-chip EHCI
*
* This routine initializes the USB MPH clock, configures the MPH pins and
* enables the ports, set the "fixupPortNumber" entry in the sysInstParamTable
* parameter table of hwconf.c.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

void sysM834xEhciInit()
    {
    UINT32                      svr;
    UINT32                      chip;
    UINT32                      rev;
    VXB_INST_PARAM_OVERRIDE *   bspEntry;

    /* USB/CSB 1:1 clk ratio for performance and disable DR clk */
    *M83XX_SCCR(CCSBAR)  &= 0xff4fffff;

    /* Make sure USBMPH pins are available */
    *M83XX_SICRL(CCSBAR)   = 0x00000000;

    /* 256 MB */
    *M83XX_SNOOP1(CCSBAR) = 0x0000001b;

    /* Enable ports */
    *M83XX_PORTSC1(CCSBAR) = LONGSWAP(0x80000000);
    *M83XX_PORTSC2(CCSBAR) = LONGSWAP(0x80000000);

    /* Recommended values if not setting repeat mode */
    *M83XX_BURSTSIZE(CCSBAR) = LONGSWAP(0x00000808);

    /* rd prefetch 32byte base on burst size 8 x 4 */
    *M83XX_SI_CTRL(CCSBAR) = 0x00000000;

    /* Recommended prio and age cnt threshold */
    *M83XX_PRI_CTRL(CCSBAR) = 0x0000000c;
    *M83XX_AGE_CNT_THRESH(CCSBAR) = 0x00000040;

    /* Enable the phy */
    *M83XX_CONTROL(CCSBAR) = 0x00000004;

    /* 
     * MPC834x EHCI controller has an errata #14 which needs a workaround to fix
     * up the port number in the QH for the root hub TT support;The errata reads
     * as "USB14 Port numbering in Queue Head violates EHCI specification" in the
     * errata file, and also marked as "Fixed in silicon revision 3.x"; So when
     * working on the very old hardware, must specify the "fixupPortNumber" entry
     * as "TRUE" in the sysInstParamTable parameter table of hwconf.c.
     */

    /* Get SVR */
    svr  = vxSvrGet();

    /* Get chip info and Rev */
    chip = svr >> 16;
    rev  = (svr >> 4) & 0xf;

    /* Erratum #14 on MPC834x Rev 1.0 & 1.1 chips */
    if ((rev == 1) && (chip >= 0x8050) && (chip <= 0x8055))
        {
        /* check BSP-supplied table */
        bspEntry = &sysInstParamTable[0];
        while (bspEntry->instName != NULL)
            {
            if ((strcmp(bspEntry->instName, "vxbPlbUsbEhci") == 0) &&
                 (strcmp(bspEntry->paramName, "fixupPortNumber") == 0))
                {
                bspEntry->value.int32Val = TRUE;
                break;
                }
            bspEntry++;
    	    }
        }
    }
#endif

/* Atheros AR521X WLAN Support */
#ifdef INCLUDE_AR521X_END
    #include "wlan/bsp/sysDot11End.c"
#endif /* INCLUDE_AR521X_END */

#ifdef INCLUDE_BCM43XX_END
        #include <wlan/bsp/sysBroadComWLEnd.c>
#endif /* INCLUDE_BCM43XX_END */


