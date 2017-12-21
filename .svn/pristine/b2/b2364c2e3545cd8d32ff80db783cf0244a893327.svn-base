/* sysLib.c - Motorola ads 827x board system-dependent library */

/*
 * Copyright (c) 1999-2008, 2010 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02q,30apr10,dhy  delete _MMU_UBAT_VP attribute(WIND00162540)
02p,25apr08,l_g  add the vxBus support
02o,21aug07,mmi  remove vxPowerModeSet() since deprecated
02n,08aug07,agf  remove duplicate variable definitions
02m,22feb07,rec  WIND00071187 - sysModel fails if MMU not initialized
02l,20feb07,rec  WIND00088466 - fix sysModel test
02k,10aug06,dtr  No need to swap mach address around.
02j,03may06,dtr  SPR#120513 - check status of sysNetMacNVRamAddrGet.
02i,27oct05,mdo  SPR#114197 - protect against multiple defines for
                 INCLUDE_PCICFG_SHOW
02h,20sep05,dtr  Wrap sysAuxClkDisable in ifdef.
02g,02sep05,dtr  Enable IRQ5 routing.s
02f,26apr05,kab  Doc fixes (SPR 108362)
02e,29mar05,dtr  Support for extra BATs.
02d,24mar05,dtr  Add in support for EDR.
02c,04oct04,dtr  Add support for latest security engine drivers
02b,28jan04,dtr  Use m82xxSio.c from target/src/drv/sio.
02a,08jan04,dtr  Add MMU mapping for security processor.
01z,08dec03,dtr  Modified for ads827x BSP.
01y,08oct03,dee  nvram routines check limits of nvram offset
01x,01oct03,dee  use sysHwInit to adjust mmu tables depending on size of SDRAM
01w,08jan03,dtr  Added support for PCI DMA and Error Handling.
                 Implemented workaround for PCI Bridge read errata.
01v,13jul02,dtr  Add support for PCI.
01u,08mar02,jnz  add support for ads8266
01t,12dec01,jrs  Add BAT table entry to correct mem map.
01s,10dec01,jrs  change copyright date
01r,30nov01,gls  fixed sysBatDesc definition (SPR #20321)
01q,17oct01,jrs  Upgrade to veloce
                 set M8260_SCCR to BRGCLK_DIV_FACTOR,
                 added global variable baudRateGenClk-set by sysBaudClkFreq(),
                 added sysCpmFreqGet() and sysBaudClkFreq() functions,
                 added 8260 Reset Configuration Table - SPR66989
                 changed INCLUDE_MOT_FCC to INCLUDE_MOTFCCEND - SPR #33914
01p,14mar00,ms_  add support for PILOT revision of board
01o,04mar00,mtl  minor changes in macros to be consistent
01n,18oct99,ms_  vxImmrGet must return only bits 14:0 (SPR 28533)
01m,18sep99,ms_  fix comment for ram on 60x bus
01l,16sep99,ms_  sysMotFccEnd.c is local to bsp after all...
01k,16sep99,ms_  fix include files path
01j,16sep99,ms_  some included files come from src/drv instead of locally
01i,16sep99,ms_  get miiLib.h from h/drv instead of locally
01h,13sep99,cn   added sysMiiOptRegsHandle () (SPR# 28305).
01g,08jun99,ms_  remove definition of M8260_SIUMCR that doesn't belong in here
01f,17apr99,ms_  unnesessary setting of MAMR
01e,17apr99,cn   added a temporary fix to initialize the boot line.
01d,17apr99,ms_  final cleanup for EAR
01c,14apr99,cn   added support for motFccEnd
01b,06apr99,ms_  reset the CPM in sysHwInit()
01a,28jan99,ms_  adapted from ads860/sysLib.c version 01j
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
#include <drv/mem/m8260Siu.h>
#include <drv/mem/m8260Memc.h>
#include <drv/timer/m8260Clock.h>
#include <drv/sio/m8260Cp.h>
#include <drv/sio/m8260CpmMux.h>
#include <drv/mem/m82xxDpramLib.h>
#include <miiLib.h>

#ifdef INCLUDE_PCI
#    include <drv/pci/pciConfigLib.h>
#    include <drv/pci/pciIntLib.h>
#endif /* INCLUDE_PCI */

/* vxBus */

#include <hwif/vxbus/vxBus.h>
#include "hwconf.c"

#include <hwif/vxbus/vxbPlbLib.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>


#include <../src/hwif/h/end/fccVxbEnd.h>
STATUS sysFccEnetEnable (VXB_DEVICE_ID, UINT8);

#include <../src/hwif/h/mii/mdio.h>
STATUS sysMdioEnable (VXB_DEVICE_ID);

IMPORT device_method_t * pSysPlbMethods;
IMPORT  void    hardWareInterFaceInit (void);

LOCAL struct vxbDeviceMethod m827xPlbMethods[] =
    {
    DEVMETHOD(fccEnetEnable, sysFccEnetEnable),
    DEVMETHOD(mdioEnable, sysMdioEnable),
    { 0, 0 }
    };


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
 * other purposes but are cautioned that conflicts with cacheing and mapping
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
    _MMU_LBAT_CACHE_INHIBIT),

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
    ((INTERNAL_MEM_MAP_ADDR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M |
    _MMU_UBAT_VS),
    ((INTERNAL_MEM_MAP_ADDR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 */

    0,0,

    /* D BAT 3 */

    0,0,

    /*
     * These entries are for the I/D BATs (4-7) on the MPC7455/755/834x/8272.
     * They should be defined in the following order.
     * IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
     * DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
     */

    /* I BAT 4 */

    0, 0,

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
    _MMU_LBAT_CACHE_INHIBIT),


    /* D BAT 5 */

    0, 0,

    /* D BAT 6 */

    0, 0,

    /* D BAT 7 */

    0, 0


    };

/*
 * mmuPcpBatInitMPC7x5 initializes the standard 4 (0-3) I/D BATs &
 * the additional 4 (4-7) I/D BATs present on the MPC74[45]5 & G2_LE core
 */

IMPORT void mmuPpcBatInitMPC7x5(UINT32 *pSysBatDesc); /* MPC7x5 and G2_LE BAT Support */
IMPORT FUNCPTR _pSysBatInitFunc;

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, the Board Control and
 * Status registers, the MPC8260 Internal Memory Map, and local FLASH RAM are set here.
 *
 * PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
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
 * 	.				.
 * 	.				.
 * 	.				.
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
    RAM_LOW_ADRS - LOCAL_MEM_LOCAL_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
    },

    {
    /* RAM on 60x bus - Must be second entry for Auto Sizing */

    (VIRT_ADDR) RAM_LOW_ADRS,
    (PHYS_ADDR) RAM_LOW_ADRS,
    LOCAL_MEM_SIZE -  RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
    VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      |
    VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY
    },

    {
    /* Board Control and Status Registers */

    (VIRT_ADDR) BCSR_BASE_ADRS,
    (PHYS_ADDR) BCSR_BASE_ADRS,
    BCSRS_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    /* MPC8260 Internal Memory Map */

    (VIRT_ADDR) INTERNAL_MEM_MAP_ADDR,
    (PHYS_ADDR) INTERNAL_MEM_MAP_ADDR,
    INTERNAL_MEM_MAP_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

#ifdef INCLUDE_PCI
    {
    (VIRT_ADDR)CPU_PCI_MEM_ADRS,
    (PHYS_ADDR)CPU_PCI_MEM_ADRS,
    CPU_PCI_MEM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)CPU_PCI_MEMIO_ADRS,
    (PHYS_ADDR)CPU_PCI_MEMIO_ADRS,
    CPU_PCI_MEMIO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    (VIRT_ADDR)CPU_PCI_IO_ADRS,
    (PHYS_ADDR)CPU_PCI_IO_ADRS,
    CPU_PCI_IO_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },
    {
    /* MPC8266 PCI Interrupt Controller  */
    (VIRT_ADDR) PCI_BRIDGE_INTR_CTL_BA,
    (PHYS_ADDR) PCI_BRIDGE_INTR_CTL_BA,
    PCI_BRIDGE_INTR_CTL_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    },

    {
    /* MPC8260 Internal Memory Map */

    (VIRT_ADDR) PCI_AGENT_IMMR_BA,
    (PHYS_ADDR) PCI_AGENT_IMMR_BA,
    PCI_AGENT_IMMR_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED
    }
#endif

#ifdef INCLUDE_SECURITY_ENGINE
    ,
    {
    (VIRT_ADDR) SEC_ENG_BASE_ADRS,
    (PHYS_ADDR) SEC_ENG_BASE_ADRS,
    SEC_ENG_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE |
    VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  |
    VM_STATE_GUARDED

    }
#endif /* INCLUDE_SECURITY_ENGINE */
    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

int   sysBus      = BUS;                /* system bus type (VME_BUS, etc) */
int   sysCpu      = CPU;                /* system CPU type (PPC8260) */
char *sysBootLine = BOOT_LINE_ADRS;	/* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;	/* catastrophic message area */
int   sysProcNum;			/* processor number of this CPU */
BOOL  sysVmeEnable = FALSE;		/* by default no VME */
UINT32   baudRateGenClk;
LOCAL char sysModelStr[80];



/* forward declarations */

uint32_t sysDecGet(void);

#ifdef INCLUDE_PCI
STATUS sysPciSpecialCycle (int busNo, UINT32 message);
STATUS sysPciConfigRead   (int busNo, int deviceNo, int funcNo,
			         int offset, int width, void * pData);
STATUS sysPciConfigWrite  (int busNo, int deviceNo, int funcNo,
			         int offset, int width, ULONG data);

#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND
/* Partial Fix for Errata on read access to PCI bridge registers */
/* Uses IDMA2 to access instead of direct read.  */
	UINT8   pciBridgeRegisterReadByte(int);
	UINT16  pciBridgeRegisterReadWord(int);
	UINT32  pciBridgeRegisterReadLong(int);

#	undef  PCI_IN_BYTE
#	undef  PCI_IN_WORD
#	undef  PCI_IN_LONG
#	define PCI_IN_BYTE(x) pciBridgeRegisterReadByte(x)
#	define PCI_IN_WORD(x) pciBridgeRegisterReadWord(x)
#	define PCI_IN_LONG(x) pciBridgeRegisterReadLong(x)
#endif  /* PCI_BRIDGE_READ_ERRATA_WORKAROUND */

ULONG sysPciConfAddr = (PCI_CFG_ADR_REG | INTERNAL_MEM_MAP_ADDR);          /* PCI Configuration Address */
ULONG sysPciConfData = (PCI_CFG_DATA_REG | INTERNAL_MEM_MAP_ADDR);          /* PCI Configuration Data */


#include <pci/pciIntLib.c>           /* PCI int support */
#include <pci/pciConfigLib.c>        /* pci configuration library */

/* use show routine */
#if (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD))
#include <pci/pciConfigShow.c>       /* pci configuration show routines */
#endif /* (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD)) */

/* use pci auto config */

#include <pci/pciAutoConfigLib.c>    /* automatic PCI configuration */
#include "sysBusPci.c"               /* pciAutoConfig BSP support file */
#include "mot82xxPci.c"

#endif /* INCLUDE_PCI */

UINT32 vxImmrGet();

#include "sysSerial.c"
#include "m827xIntrCtl.c"
#include <timer/m8260Timer.c>
#define M82XX_SIO_FIXES
#include <sio/m8260Sio.c>

#ifdef INCLUDE_FLASH
#include <mem/flashMem.c>
#include <mem/nvRamToFlash.c>
#else
#include <mem/nullNvRam.c>
#endif

#include <mem/m82xxDpramLib.c>

/* locals */
STATUS sysFccEnetDisable (UINT32 immrVal, UINT8 fccNum);
void sysControlLED ( int LED, int on);

/* defines */

#define ZERO    0
#define SYS_MODEL_DEF   "Motorola MPC82xx ADS"
#define SYS_MODEL_HIP4  "Motorola MPC8260 ADS - HIP4"
#define SYS_MODEL_HIP7  "Motorola MPC8260 ADS - HIP7"
#define SYS_MODEL_8272  "Motorola MPC8272 ADS"
#define SYS_MODEL_8272E  "Motorola MPC8272 ADS - Encryption"

/* needed to enable timer base via SIUMCR */
#ifdef INCLUDE_PCI
#define      M8260_DPPC_MASK	0x0C000000 /* bits 4 and 5 */
#define      M8260_DPPC_VALUE	0x0C000000 /* bits (4,5) should be (1,0) */
#else
#define      M8260_DPPC_MASK	0x0C000000 /* bits 4 and 5 */
#define      M8260_DPPC_VALUE	0x08000000 /* bits (4,5) should be (1,0) */
#endif /*INCLUDE_PCI */

/* Enable IRQ5 for Phy via SIUMCR */
#define M8272_BAC_MASK (0x03 << 24)
#define M8272_BAC_VALUE (0x01 << 24)

/* Enable bit for Security engine in SIUMCR */
#define M8272_SEC_ENG_ENABLE 0x200

#include <drv/sio/m8260Sio.h>

#include "sysNet.c"

#ifdef INCLUDE_FEI_END
#   include "sysFei82557End.c"
#endif /* INCLUDE_FEI_END */

#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND
UCHAR	sysInByte(ULONG port)
{
return(pciBridgeRegisterReadByte(port));
}
USHORT	sysInWord(ULONG port)
{
return(pciBridgeRegisterReadWord(port));
}
ULONG	sysInLong(ULONG port)
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
    UINT device;
    UINT32 maskNum = 0;

    if (!VM_INSTALLED)
        {
        strcpy (sysModelStr, SYS_MODEL_DEF);
        }
    else
        {
        /* Read Immr for processor info and Svr TBD */ 
        maskNum = ((*(UINT32*)(PQII_REG_BASE | IMMR_OFFSET)) & 0xfff0);

        if (((device = vxPvrGet ()) & HIP4_MASK) == HIP4_ID)

            strcpy (sysModelStr,SYS_MODEL_HIP4);

        else if (((device = vxPvrGet()) & HIP4_MASK) == HIP7_ID)
	    {
	    if ((maskNum==0xC00) || (maskNum==0xD00))
	        {		
	        strcpy (sysModelStr,SYS_MODEL_8272);
	        }

	    else if ((maskNum==0xC10) || (maskNum==0xD10))
	        {		
	        strcpy (sysModelStr,SYS_MODEL_8272E);
	        }
	    else
	        {
	        strcpy (sysModelStr,SYS_MODEL_HIP7);
	        }
            }
        else 
            strcpy (sysModelStr,SYS_MODEL_DEF);
    }
    return (sysModelStr);
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
#ifdef INCLUDE_PCI
    sysPhysMemDesc[4].len = (UINT)(sysPhysMemTop ());
#endif
}

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
*/

void sysHwInit (void)
    {
    int	immrVal = vxImmrGet();

    /* put correct memory size in sysPhysMemDesc[1].len for
     * size of local memory
     */
    sysHwMemInit();

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

    m82xxDpramLibInit();

    /* set the BRGCLK division factor */

    * M8260_SCCR(immrVal) = BRGCLK_DIV_FACTOR;

    /* set DPPC in SIUMCR to 10 so that timer is enabled (TBEN) */
	
    * M8260_SIUMCR(immrVal) &= ~M8260_DPPC_MASK;  /* clear the dppc */

    * M8260_SIUMCR(immrVal) |= M8260_DPPC_VALUE;  /* or in the desired value */

    /* Setup IRQ5 routing to interrupt controller */
    * M8260_SIUMCR(immrVal) &= ~M8272_BAC_MASK;

    * M8260_SIUMCR(immrVal) |= M8272_BAC_VALUE;    

    /* Enable Security Engine */
    * M8260_SIUMCR(immrVal) &= ~M8272_SEC_ENG_ENABLE;    

    /* Use rising edge as trigger for IRQ5 */ 
    * M8260_SIEXR(immrVal) |= 0x400;

    /* reset the Communications Processor */

    *M8260_CPCR(immrVal) = M8260_CPCR_RESET | M8260_CPCR_FLG;

    CACHE_PIPE_FLUSH();

    /* set pointer to BAT initialization functions */

    _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC7x5;

    /* Get the Baud Rate Generator Clock  frequency */

    baudRateGenClk = sysBaudClkFreq();

#ifdef INCLUDE_PCI

#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND
    /* Setup to do IDMA for PCI Bridge reads */
    pciBridgeIdmaInit();
#endif /*PCI_BRIDGE_READ_ERRATA_WORKAROUND */

    /* config pci */
    if (pciConfigLibInit (PCI_MECHANISM_0,(ULONG) sysPciConfigRead,
                              (ULONG) sysPciConfigWrite,(ULONG) sysPciSpecialCycle) != OK)
        {
        sysToMonitor (BOOT_NO_AUTOBOOT);  /* BAIL */
        }

    /*  Initialize PCI interrupt library. */
    if ((pciIntLibInit ()) != OK)
        {
        sysToMonitor (BOOT_NO_AUTOBOOT);
        }

    if (*((char*)PCI_AUTO_CONFIG_ADRS)==FALSE)
        {
	mot82xxBridgeInit();
#if (PCI_CFG_TYPE == PCI_CFG_AUTO)
            sysPciAutoConfig();
#endif
        *((char*)PCI_AUTO_CONFIG_ADRS)=TRUE;
	}
#endif /* INCLUDE_PCI */


    /* Reset serial channels */

    sysSerialHwInit();

    /* Initialize interrupts */

    m8260IntrInit();


    /* Initialize the Security Block  - should match TLB entry */
    *M82XX_SECMR (immrVal) = SEC_ENG_SIZE_MASK;    
    *M82XX_SECBR (immrVal) = SEC_ENG_BASE_ADRS | 0x1;

    pSysPlbMethods = m827xPlbMethods;
    hardWareInterFaceInit();
    
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
* ERRNO
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
     int startType	/* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 8);	/* Warm reboot */

    intLock();

    cacheDisable(INSTRUCTION_CACHE);
    cacheDisable(DATA_CACHE);

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif /* INCLUDE_AUX_CLK */

    /* disable both RS232 ports on the board */

    *BCSR1 |= (BCSR1_RS232EN_1_L);
    *BCSR1 |= (BCSR1_RS232EN_2_L);

    sysSerialReset();		/* reset the serial device */

    vxMsrSet (0);

    (*pRom) (startType);	/* jump to bootrom entry point */

    return (OK);	/* in case we ever continue from ROM monitor */
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
    int immrVal = vxImmrGet();

    CACHE_PIPE_FLUSH();

    if (!configured)
    {
   
    vxbDevInit();

    /* initialize serial interrupts */

    sysSerialHwInit2();

    * M8260_SCCR(immrVal) &= ~M8260_SCCR_TBS;
    CACHE_PIPE_FLUSH();

    configured = TRUE;

#ifdef INCLUDE_FEI_END
    sys557PciInit ();            /* Intel 82557/9 device */
#endif /* INCLUDE_FEI_END */

#ifdef INCLUDE_PCI_DMA
    pciDmaInit();
#endif /* INCLUDE_PCI_DMA */

#ifdef INCLUDE_PCI_ERROR_HANDLING
    pciErrorHandlingInit();
#endif /* INCLUDE_PCI_ERROR_HANDLING */

    taskSpawn("tDevConn", 11, 0, 10000,
        vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

    }
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
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
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
* Not applicable for the 8260Ads
*
* RETURNS: ERROR, always.
*
* SEE ALSO: sysLocalToBusAdrs()
*/

STATUS sysBusToLocalAdrs
    (
     int  	adrsSpace, 	/* bus address space where busAdrs resides */
     char *	busAdrs,   	/* bus address to convert */
     char **	pLocalAdrs 	/* where to return local address */
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
* Not applicable for the 8260Ads.
*
* RETURNS: FALSE, always.
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
*/

void sysBusClearTas
    (
     volatile char * address	/* address to be tested-and-cleared */
    )
    {
    }

/*******************************************************************************
*
* vxImmrGet - return the current IMMR value
*
* This routine returns the current IMMR value
*
* RETURNS: current IMMR value
*
*/

UINT32  vxImmrGet (void)
    {

    return (INTERNAL_MEM_MAP_ADDR & IMMR_ISB_MASK);

    }

/***************************************************************************
*
* sysGetPeripheralBase   - Provides CCSRBAR address fro security engine 
*                          drivers.
*
* RETURNS:
*
* ERRNO
*/
UINT32 sysGetPeripheralBase()
    {
    return(vxImmrGet());
    }

/**************************************************************************
*
* sysFlashLED - flash one of the signal lamps
*
* This routine will flash the indicated signal lamp for about 1/4 second
*
* SEE ALSO:
*
* RETURNS: N/A
*/
void sysFlashLED
    (
    int LED	/* bitmap of LED to flash */
    )
    {

    sysControlLED(LED, BCSR0_LED_ON);

    taskDelay(15);

    sysControlLED(LED, BCSR0_LED_OFF);

    }

/**************************************************************************
*
* sysControlLED - control one of the signal lamps
*
* This routine will turn the indicated signal lamp on or off, as
* requested
*
* SEE ALSO:
*
* RETURNS: N/A
*/

void sysControlLED
    (
    int LED, 	/* bitmap of LED to control */
    int on	/* if TRUE turn LED ON; otherwise, turn LED OFF */
    )
    {

    CACHE_PIPE_FLUSH();			/* always before first read */

    if (on)
	*(UINT32 *)BCSR0 &= ~LED; /* clearing bit turns LED on */
    else
	*(UINT32 *)BCSR0 |= LED; /* setting bit turns LED off */

    }


#ifdef INCLUDE_PCI 	/* board level PCI routines */
/*******************************************************************************
*
* sysPciSpecialCycle - generate a special cycle with a message
*
* This routine generates a special cycle with a message.
*
* \NOMANUAL
*
* RETURNS: OK
*/

STATUS sysPciSpecialCycle
    (
    int		busNo,
    UINT32	message
    )
    {
    int deviceNo	= 0x0000001f;
    int funcNo		= 0x00000007;

    PCI_OUT_LONG (sysPciConfAddr,
                  pciConfigBdfPack (busNo, deviceNo, funcNo) |
                  0x80000000);

    PCI_OUT_LONG (sysPciConfData, message);

    return (OK);
    }

/*******************************************************************************
*
* sysPciConfigRead - read from the PCI configuration space
*
* This routine reads either a byte, word or a long word specified by
* the argument <width>, from the PCI configuration space
* This routine works around a problem in the hardware which hangs
* PCI bus if device no 12 is accessed from the PCI configuration space.
*
* \NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*/

STATUS sysPciConfigRead
    (
    int	busNo,    /* bus number */
    int	deviceNo, /* device number */
    int	funcNo,	  /* function number */
    int	offset,	  /* offset into the configuration space */
    int width,	  /* width to be read */
    void * pData /* data read from the offset */
    )
    {
    UINT8  retValByte = 0;
    UINT16 retValWord = 0;
    UINT32 retValLong = 0;
    STATUS retStat = ERROR;

    if ((busNo == 0) && (deviceNo == 12))
        return (ERROR);

    switch (width)
        {
        case 1:	/* byte */
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);

            retValByte = PCI_IN_BYTE (sysPciConfData + (offset & 0x3));
            *((UINT8 *)pData) = retValByte;
	    retStat = OK;
            break;

        case 2: /* word */
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);

	    retValWord = PCI_IN_WORD (sysPciConfData + (offset & 0x2));
            *((UINT16 *)pData) = retValWord;
	    retStat = OK;
	    break;

        case 4: /* long */
	    PCI_OUT_LONG (sysPciConfAddr,
		          pciConfigBdfPack (busNo, deviceNo, funcNo) |
		          (offset & 0xfc) | 0x80000000);
	    retValLong = PCI_IN_LONG (sysPciConfData);
            *((UINT32 *)pData) = retValLong;
	    retStat = OK;
            break;

        default:
            retStat = ERROR;
            break;
        }

    return (retStat);
    }

/*******************************************************************************
*
* sysPciConfigWrite - write to the PCI configuration space
*
* This routine writes either a byte, word or a long word specified by
* the argument <width>, to the PCI configuration space
* This routine works around a problem in the hardware which hangs
* PCI bus if device no 12 is accessed from the PCI configuration space.
*
* \NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*/

STATUS sysPciConfigWrite
    (
    int	busNo,    /* bus number */
    int	deviceNo, /* device number */
    int	funcNo,	  /* function number */
    int	offset,	  /* offset into the configuration space */
    int width,	  /* width to write */
    ULONG data	  /* data to write */
    )
    {
    if ((busNo == 0) && (deviceNo == 12))
        return (ERROR);

    switch (width)
        {
        case 1:	/* byte */
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);
	    PCI_OUT_BYTE ((sysPciConfData + (offset & 0x3)), data);
            break;

        case 2: /* word */
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);
	    PCI_OUT_WORD ((sysPciConfData + (offset & 0x2)), data);
	    break;

        case 4: /* long */
	    PCI_OUT_LONG (sysPciConfAddr,
		          pciConfigBdfPack (busNo, deviceNo, funcNo) |
		          (offset & 0xfc) | 0x80000000);
	    PCI_OUT_LONG (sysPciConfData, data);
            break;

        default:
            return (ERROR);

        }
    return (OK);
    }
#endif /* INCLUDE_PCI */

/***********************************************************************
*
* sysMdioEnable - enable MDIO access
*
* This routine is to enable the MDIO access.
*
* RETURNS: OK
*/
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

    *M8260_IOP_PCPAR(immrVal) &= ~(PC19 | PC18);
    *M8260_IOP_PCDIR(immrVal) |= (PC19);

    intUnlock (s);

    return (OK);
    }

/***********************************************************************
*
* sysFccEnetEnable - enable the MII interface to the FCC controller
*
* This routine is expected to perform any target specific functions required
* to enable the Ethernet device and to connect the MII interface to the FCC.
*
* RETURNS: OK, or ERROR if the FCC controller cannot be enabled.
*/

STATUS sysFccEnetEnable
    (
    VXB_DEVICE_ID pDev,
    UINT8   fccNum  /* FCC being used */
    )
    {
    int             intLevel;
    UINT32 immrVal;

    intLevel = intLock ();
    immrVal = vxImmrGet ();

    /* assert the enable signal */

    if (fccNum==2)
      {
     /* de-assert the enable signal */
    *BCSR3 |= 0x10000000;
     /* assert the reset signal */
    *BCSR3 &= ~0x08000000;
     sysMsDelay(1);

    *BCSR3 |=  0x08000000;
     sysMsDelay(1);

    *BCSR3 &= ~0x10000000;
     sysMsDelay(10);

    *M8260_IOP_PCPAR(immrVal) |=  (PC17 | PC16);
    *M8260_IOP_PCSO(immrVal)  &= ~(PC17 | PC16);
    *M8260_IOP_PCDIR(immrVal) &= ~(PC17 | PC16);

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

    *M8260_CMXFCR (immrVal)  &= ~0xff0000;
    *M8260_CMXFCR (immrVal)  |= (M8260_CMXFCR_R2CS_CLK15 | 
                     M8260_CMXFCR_T2CS_CLK16);
    *M8260_CMXFCR (immrVal)  &= ~(M8260_CMXFCR_FC2_MUX);
    }

    if (fccNum==1)
      {
     /* de-assert the enable signal */
    *BCSR1 |= 0x08000000;
     /* assert the reset signal */
    *BCSR1 &= ~0x04000000;
     sysMsDelay(1);

    *BCSR1 |=  0x04000000;
     sysMsDelay(1);

    *BCSR1 &= ~0x08000000;
     sysMsDelay(10);

    *M8260_IOP_PCPAR(immrVal) |=  (PC21 | PC22);
    *M8260_IOP_PCSO(immrVal)  &= ~(PC21 | PC22);
    *M8260_IOP_PCDIR(immrVal) &= ~(PC21 | PC22);

    *M8260_IOP_PASO(immrVal)  |= (PD29 | PA28 | PA27 | PA26);    
    *M8260_IOP_PADIR(immrVal) |= (PB29 | PB28 | PB21 | PB20 | PB19 | PB18);
    *M8260_IOP_PCSO(immrVal)  |= (PC15 | PC14 | PC13 | PC12 | PC7 | PC6);
    *M8260_IOP_PCDIR(immrVal) |= (PC15 | PC14 | PC13 | PC12 | PC7 | PC6); 
    *M8260_IOP_PDSO(immrVal)  |= (PD29 | PD7);    
    *M8260_IOP_PDDIR(immrVal) |= (PD29 | PD19 | PD18 | PD7);    
    *M8260_IOP_PAPAR(immrVal) =  (PA29 | PA28 | PA27 | PA26 | PA21 | PA20 |
                                  PA19 | PA18 | PA17 | PA16 | PA15 | PA14);    
    *M8260_IOP_PCPAR(immrVal) |= (PC15 | PC14 | PC13 | PC12 | PC7 | PC6);    
    *M8260_IOP_PDPAR(immrVal) |= (PD29 | PD19 | PD18);    
    
    /* connect FCC1 clocks */

    *M8260_CMXFCR (immrVal)  &= ~0xff000000;
    *M8260_CMXFCR (immrVal)  |= (M8260_CMXFCR_R1CS_CLK11 | 
                 M8260_CMXFCR_T1CS_CLK10);

    /* NMSI mode */

    *M8260_CMXFCR (immrVal)  &= ~(M8260_CMXFCR_FC1_MUX);         
      }

    intUnlock (intLevel);

    sysMsDelay(250);

    return(OK);
    }


