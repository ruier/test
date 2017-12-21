/* sysLib.c - Wind River SBC8641 system-dependent library */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01s,18nov08,pmr  Fix WIND00137387: conditionalize BAT for upper memory
01r,12nov08,kab  WIND00143297 - Fix build of CPU1 for gnu
01q,06nov08,pmr  WIND00140224: shutdown ints on core1 for wrload
01p,23oct08,kab  Update sysAmpCpuPrep to new sig per design mod
01o,16oct08,kab  Add sysAmpCpuPrep() - wrload/multios support
01n,04sep08,dtr  Add new sysAmpCpuStatusGet/Set calls.
01m,03sep08,dtr  Switch to startcore to sysAmpCpuEnable.
01l,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01k,11aug08,dtr  Add wrload support.
01j,11jul08,to   made vector area cacheable and coherent
01i,24apr08,dtr  Remove hard coded IPI enable - WIND00121614.
01h,17apr08,to   make BAT area not user-accessible
01g,25oct07,to   added CPU1_INIT_START_ADR (WIND00107937)
01f,11sep07,h_k  removed vxbIntToCpuRoute() call from sysCpuEnable().
                 (CQ:104081)
01e,04sep07,wap  Add support for SMP (WIND00103522)
01d,30aug07,wap  Switch to VxBus ETSEC and PCI drivers (WIND00103172)
01c,23aug07,pgh  Fix apigen errors.
01b,30jul07,vik  removed cmdLine.c inclusion
01a,20jun07,x_s  initial creation.
*/

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    sysEpic.c	        - programmable interrupt controller driver
    sysDuart.c          - duart controller config
    ns16550Sio.c        - National Semiconductor 16550 UART driver
    ppcDecTimer.c	- PowerPC decrementer timer library (system clock)
    flashMem.c          - 29F040 flash memory device driver.
    altiVecLib.c        - Altivec support for MPC 7400
    sysCacheLib.s       - L1 and L2 cache lock support

INCLUDE FILES: sysLib.h

SEE ALSO:
.pG "Configuration"
*/

/* includes */

#include <stdio.h>
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
#include <taskLib.h>
#include <vxLib.h>
#include <tyLib.h>
#include <vmLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include "bootFlash.h"


#include "shellLib.h"
#include <private/shellLibP.h>
#include "shellPromptLib.h"


#include  "sysPhyWrRd.c"
#include "bspBoard.c"

#ifdef VXB_M8641_RS485
#include "demo/uart/uartDemo.c"
#include "demo/uart/uartTest.c"
#endif


#ifdef DRV_M86XX_FCAN
#include "canDrv/canDrv.c"

//#include "canDrv/canApp.c"
//#include "canDrv/canTest.c"
#endif

#ifdef DRV_M8641_SYN_UART
#include "vxbm8641SynUart.c"
#include "bspSynUart.c"
#include "demo/synUart/synUartDemo.c"
#endif



#ifdef DRV_M8641_IIC
#include "sysM8641I2c.c"
#include "vxbm8641I2c.c"
//#include "sysAdt7483.c"
#include "sysI2c.c"

#endif




/* add by ff----begin  */
#ifdef INCLUDE_PCI_BUS
#include "m85xxPci.c"
#endif

#ifndef COMPILE_BOOTROM
#include "vxbEpicIntCtlr.c"
#include "vxbEpicIntCtlrIsr.c"
#endif


#include "vxbNs16550Sio.c"

#ifdef DRV_INTCTRL_GPIO
#include "vxbGpioIntCtrl.c"
#endif


#ifdef DRV_INTCTRL_FPGA_TIMER
#include "vxbFpgaTimerIntCtrl.c"
#endif

#ifdef DRV_INTCTRL_I2C
#include "vxbI2cIntCtrl.c"
#endif

#ifdef DRV_M8641_GPIO
#include "vxbGpioDrv.c"
#include "bspGpio.c"
#endif


    
#ifdef INCLUDE_TFFS
#include "wamdmtd.c"
#ifndef COMPILE_BOOTROM
#include "usrFsLib.c"
#include "fflashIO.c"
#endif
#endif
#ifdef DRV_INTCTLR_RKTIO
#include "vxbRktIntCtrl.c"
#endif

#ifdef COMPILE_BOOTROM 
#include "bootFsLoad.c"
#include "bootFlash.c"

#include "bootApp.c"
#include "bootAppShell.c"
#endif
/* add by ff----end  */
#define INCLUDE_RAPIDIO_ENUM_NEW

#ifdef INCLUDE_RAPIDIO_BUS
#include "m85xxRio.c"
#include "sysRapidio.c"

#ifdef INCLUDE_RAPIDIO_ENUM_NEW
#include "rapidIO/rioAssignID.c"
#include "rapidIO/rioException.c"
#include "rapidIO/rioExplore.c"
#include "rapidIO/rioRoute.c"
#include "rapidIO/sysRapidIOEnum.c"
#include "rapidIO/rioPrivate.c"
//#include "rapidIO/rioSwitchInit.c"
#else
#include "sysRapidioEnumerate.c"
#endif

#endif

#ifdef BRIDGE_DMA
#include "bspBridgeDma8641.h"
#endif

#ifdef VXB_M8641_DMA
#include "vxbM8641Dma.c"
#include "sysDma.c"
#endif

#ifdef INCLUDE_PROPHET_DUMMY_MDIO
#include "vxbProphetDummyMdio.c"
#endif

#ifdef INCLUDE_PROPHET_M8640D_MDIO
#include "vxbProphetM8640dMdio.c"
#endif

//#include "demo/ledTest.c"


#ifdef INCLUDE_CACHE_L2
#include "sysL2Cache.h"
#endif

#ifdef INCLUDE_VXBUS
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#endif

#ifdef INCLUDE_PCI_BUS
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciIntLib.h>
#endif

#if defined (INCLUDE_ALTIVEC)
#include <altivecLib.h>
#endif

#ifdef _WRS_CONFIG_SMP
#ifndef VX_CPC_ASYNC
#define VX_CPC_ASYNC	2
typedef void   (*CPC_FUNC) (void *, int);
IMPORT void cpcInvoke (cpuset_t, CPC_FUNC, void *, int, int);
#endif
#endif


#ifdef _WRS_VX_AMP
#error fzz:cannot support AMP now!

#include <vxIpiLib.h>
IMPORT int sysStartType;
#endif /* _WRS_VX_AMP */

#ifdef INCLUDE_NV_RAM
/*#    include "eeprom.c"*/             /* un-included by freeWinder */
#    include <mem/byteNvRam.c>      /* Generic NVRAM routines */
//#else
//#    include <mem/nullNvRam.c>
#endif /* INCLUDE_NV_RAM */

IMPORT int       (* _func_altivecProbeRtn) () ;


/* defines */

#define ZERO		0
#define SYS_MODEL	"Wind River SBC8641D"


IMPORT void bspInit();
IMPORT int sysGetCpuID(void);
IMPORT STATUS sysLedSet(UINT8 ledNo,UINT8 status) ;
IMPORT void sysLedFlash(UINT32 time);
IMPORT STATUS sysPhyInit(UINT8 phyAddr);


void sysCpuLawInit(void);

void  sysSetRioOW();
void sysSetRioIW() ;




#include "wrSbc8641.h"
#include "bootLib.h"
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
 * with vmLib support.  Users may use BAT registers for i/o mapping and
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
 * The BAT configuration for 4xx/6xx-based PPC boards is as follows:
 * All BATs point to PROM/FLASH memory so that end customer may configure
 * them as required.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */


UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT +_MMU_NUM_EXTRA_IBAT+_MMU_NUM_EXTRA_DBAT)] =
    {
    /* I BAT 0 */
    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT),
    /* I BAT 1 */
	0,0,
    /* I BAT 2 */

	0,0,

    /* I BAT 3 */

    0,0,

    /* D BAT 0 */
    ((FLASH_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT),

    /* D BAT 1 */
    ((CCSBAR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M | _MMU_UBAT_VS),
    ((CCSBAR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 2 */

#ifdef _WRS_VX_AMP
    ((0x10000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((0x10000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
     _MMU_LBAT_MEM_COHERENT),
#else
    0,0,
#endif

    /* D BAT 3 */

    0,0

/* These entries are for the I/D BAT's (4-7) on the MPC7455/755.
   They should be defined in the following order.
   IBAT4U,IBAT4L,IBAT5U,IBAT5L,IBAT6U,IBAT6L,IBAT7U,IBAT7L,
   DBAT4U,DBAT4L,DBAT5U,DBAT5L,DBAT6U,DBAT6L,DBAT7U,DBAT7L,
*/
    ,
    /* I BAT 4 */

    0,0,

    /* I BAT 5 */

    0,0,

    /* I BAT 6 */

    0,0,

    /* I BAT 7 */

    0,0,
    /* D BAT 4 */


    ((PERIPH_BASE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_16M | _MMU_UBAT_VS),
    ((PERIPH_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),


    /* D BAT 5 */
    ((0xe0000000 & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_32M | _MMU_UBAT_VS),
    ((0xe0000000 & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW |
    _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* D BAT 6 */

    0,0,

    /* D BAT 7 */

    0,0


    };

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, memory mapped
 * VME A16 space and local PROM/FLASH are set here.
 *
 * All entries in this table both addresses and lengths must be page aligned.
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
 *	.				.
 *	.				.
 *	.				.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {
    {
    /* Vector Table and Interrupt Stack */
    /* Must be sysPhysMemDesc [0] to allow adjustment in sysHwInit() */

    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    RAM_LOW_ADRS,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT | \
    MMU_ATTR_CACHE_COHERENCY
    },

    {
    /* Must be sysPhysMemDesc [1] to allow adjustment in sysHwInit() */

    (VIRT_ADDR) RAM_LOW_ADRS,
    (PHYS_ADDR) RAM_LOW_ADRS,
    LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - USER_RESERVED_MEM - FLASH_SHARE_SIZE - RAM_LOW_ADRS,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT | \
    MMU_ATTR_CACHE_COHERENCY
    },
    {
     /* Must be sysPhysMemDesc [1] to allow adjustment in sysHwInit() */

     (VIRT_ADDR) FLASH_SHARE_MEM,
     (PHYS_ADDR) FLASH_SHARE_MEM,
     FLASH_SHARE_SIZE,    
     MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
     MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
     MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY 
     },
     
    {
    /* Must be sysPhysMemDesc [1] to allow adjustment in sysHwInit() */

    (VIRT_ADDR) (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - USER_RESERVED_MEM),
    (PHYS_ADDR) (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - USER_RESERVED_MEM),
    USER_RESERVED_MEM,    
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
    MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    //MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT | \
    //MMU_ATTR_CACHE_COHERENCY   
    },
      
#ifdef INCLUDE_RAPIDIO_BUS
    {
    (VIRT_ADDR) RAPIDIO_ADRS,
    (PHYS_ADDR) RAPIDIO_ADRS,
    RAPIDIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
    MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY

    },
#endif

#ifdef INCLUDE_PCI_BUS
    {
    (VIRT_ADDR) PCI_MEM_ADRS,
    (PHYS_ADDR) PCI_MEM_ADRS,
    PCI_MEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
    MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCI_MEMIO_ADRS,
    (PHYS_ADDR) PCI_MEMIO_ADRS,
    PCI_MEMIO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
    MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    }
    ,
    {
    (VIRT_ADDR) PCI_IO_ADRS,
    (PHYS_ADDR) PCI_IO_ADRS,
    PCI_IO_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF | \
    MMU_ATTR_CACHE_GUARDED | MMU_ATTR_CACHE_COHERENCY
    }
 
#endif /* INCLUDE_PCI */

#ifdef INCLUDE_L2PM
    {
    /* If private memory being used for MCP 755 or MCP 7410 */

    (VIRT_ADDR) L2PM_SRAM_ADRS,
    (PHYS_ADDR) L2PM_SRAM_ADRS,
    L2PM_PHYMEM_SIZE,
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT
    },
#endif /*INCLUDE_L2PM*/

    };

int   sysPhysMemDescNumEnt;		/* calculated in sysHwInit */


int   sysBus      = VME_BUS;            /* legacy */
int   sysCpu      = CPU;                /* system CPU type (MC680x0) */
char *sysBootLine = BOOT_LINE_ADRS;	/* address of boot line */
char *sysExcMsg   = EXC_MSG_ADRS;	/* catastrophic message area */
int   sysProcNum  = 0;			/* processor number of this CPU */
UINT32  coreFreq;

/* static locals */
LOCAL char sysModelStr [80]	= SYS_MODEL;	/* Sandpoint Model string */
LOCAL char sysInfoStr [200];			/* Sandpoint INFO string */

LOCAL UINT32    core1ampMode;
#ifdef  _WRS_CONFIG_SMP
LOCAL void sysCpu1ShutdownLoop (void);
#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
LOCAL UINT32 tbHI;      /* upper 32 bit value of timebase */
LOCAL UINT32 tbLO;      /* lower 32 bit value of timebase */
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
#endif  /* _VX_SMP */

/* Clock Ratio Tables */
#define MAX_VALUE_PLAT_RATIO 32
UINT32 platRatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
    { 0, 0},
    { 0, 0},
    { 2, 0},
    { 3, 0},
    { 4, 0},
    { 5, 0},
    { 6, 0},
    { 7, 0},
    { 8, 0},
    { 9, 0},
    { 10, 0},
    { 0, 0},
    { 12, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 16, 0},
    { 0, 0},
    { 0, 0},
    { 0, 0},
    { 20, 0},
    { 0, 0}
};

#define MAX_VALUE_E500_RATIO 16
UINT32 e500RatioTable[MAX_VALUE_PLAT_RATIO][2] =
{
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
    { 2, 0},
	{ 0, 0},
    { 5, 1},
	{ 7, 1},
    { 3, 0},
    { 4, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
    { 8, 1},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0}
};

UINT32 sysClkFreqGet(void);
UINT32 sysPicClkFreqGet(UINT32);

/* forward declarations */

char *  sysPhysMemTop		(void);		/* Top of physical memory */
char *	sysInfo			(void);
void    sysMemMapDetect		(void);		/* detect PREP or CHRP map */
void	sysCpuCheck		(void);		/* detect CPU type */
void    sysDelay		(void);		/* delay 1 millisecond */
void	sysMsDelay		(UINT delay);	/* delay N millisecond(s) */
ULONG   sys107RegRead           (ULONG regNum);
void    sys107RegWrite          (ULONG regNum, ULONG regVal);
STATUS  sysIntEnablePIC		(int intNum);	/* Enable i8259 or EPIC */
ULONG   sysEUMBBARRead          (ULONG regNum);
void    sysEUMBBARWrite         (ULONG regNum, ULONG regVal);
int	sysProcNumGet		(void);
UINT32  sysCpuAvailableGet      (void);
LOCAL   UINT32 physCpuAvailableGet (void);

#ifdef INCLUDE_MMU		/* add a sysPhysMemDesc entry */
STATUS  sysMmuMapAdd		(VIRT_ADDR address, UINT32 len,
				 UINT32 initialStateMask, UINT32 initialState);
#endif /* INCLUDE_MMU */

/* external imports */

IMPORT VOID   sysOutWord	(ULONG address, UINT16 data);	/* sysALib.s */
IMPORT VOID   sysOutLong	(ULONG address, ULONG data);	/* sysALib.s */
IMPORT USHORT sysInWord		(ULONG address);		/* sysALib.s */
IMPORT ULONG  sysInLong		(ULONG address);		/* sysALib.s */
IMPORT VOID   sysOutByte	(ULONG, UCHAR);			/* sysALib.s */
IMPORT UCHAR  sysInByte		(ULONG);			/* sysALib.s */
IMPORT STATUS sysMemProbeSup    (int length, char * src, char * dest);
IMPORT UINT32 sysMsscr0Get	(void);
IMPORT UINT32 sysSvrGet		(void);
IMPORT void   vxSdaInit		(void);
IMPORT void   vxbMsDelay	(int delay);	/* delay N millisecond(s) */

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;

IMPORT VOIDFUNCPTR _pSysL3CacheFlushDisableFunc;
IMPORT VOIDFUNCPTR _pSysL3CacheInvalEnableFunc;
IMPORT void sysSerialConnectAll();
/*
 * mmuPpcBatInitMPC74x5 initializes the standard 4 (0-3)  I/D BAT's &
 * the additional 4 (4-7) I/D BAT's present on the MPC74[45]5.
 */
IMPORT void mmuPpcBatInitMPC74x5 (UINT32 *pSysBatDesc);

/* _pSysBatInitFunc needs to be set to either of the above 2 depending
   on which processor it is running to make use of the additional BAT's
   on the MPC 7455/755.
   If this is not set or is set to NULL then the standard mmuPpcBatInit
   fn. would be called which would initialize the (0-3) I/D BAT's
*/
IMPORT FUNCPTR _pSysBatInitFunc;

/*  By default this is set to NULL in the mmu Library and  in that case
    the standard BAT init func. to initialize the 4 I/D BAT's is called.
    If the standard & Extra BAT's need to be initialized then set this
    to the ones supplied by the mmuLib or else implement your own fn. and
    supply it here.
*/


/* BSP DRIVERS */


#ifdef INCLUDE_LCD
#include "sysLcd.c"
#endif /* INCLUDE_LCD */

/* added by freeWinder */

/*
#if defined(INCLUDE_ETSEC_VXB_END)
#include "end/vxbEtsecEnd.c"
#endif
*/


#ifdef  _WRS_CONFIG_SMP
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);
IMPORT STATUS cacheArchDisableFromMmu (CACHE_TYPE cache);
IMPORT STATUS mmuPpcCpuInit (void);
IMPORT void sysSmpTimeBaseSet(UINT32 tbHI, UINT32 tbLO);
#endif  /* _WRS_CONFIG_SMP */

IMPORT void sysCpu1Start (void);
IMPORT void sysCpu1Stop (void);

#ifdef INCLUDE_VXBUS
#ifdef _WRS_VX_AMP 
METHOD_DECL(sysBspDevFilter);
STATUS sysDeviceFilter(VXB_DEVICE_ID pDev);
void sysAmpCpuStatusSet(int status,int cpu);
int sysAmpCpuStatusGet(int cpu);
#endif
IMPORT void hardWareInterFaceInit();
int ns16550Pollprintf(char* pchar);
int ns16550PollOutputInit();
int ns16550PollOutputchar(char outChar);

#include "hwconf.c"

IMPORT device_method_t * pSysPlbMethods;

METHOD_DECL(vxbIntCtlrCpuCheck);
METHOD_DECL(vxbIntCtlrCpuAvail);

LOCAL struct vxbDeviceMethod plbMethods[] =
    {
    DEVMETHOD(vxbIntCtlrCpuCheck, sysProcNumGet),
#ifdef _WRS_VX_AMP 
	/* Only used for AMP filtering per core for now so not built for
	 * anything else
	 */
    DEVMETHOD(sysBspDevFilter, sysDeviceFilter),
#endif
    { 0, 0 }
    };

#endif

#ifdef INCLUDE_FLASH
#include <flashMem.c>	//#include <mem/flashMem.c>
#include <nvRamToFlash.c>
#endif

#ifdef INCLUDE_NETWORK
#    include "./sysNet.c"		/* network setup */
#endif	/* INCLUDE_NETWORK */


#define HID1_ABE 0x800
#define HID1_SYNCBE 0x400
#define HID1_EMCP 0x80000000

#ifdef INCLUDE_SM_COMMON
#include "sysSmEnd.c"
#endif

#ifdef _WRS_VX_AMP

#ifdef INCLUDE_AMP_CPU_01
_WRS_ABSOLUTE_BEGIN(Loadbias)
_WRS_ABSOLUTE(wrloadLoadBias,CORE1T_OFFSET);
_WRS_ABSOLUTE_END
#endif /* INCLUDE_AMP_CPU_01 */

typedef struct {
	char* deviceName;
	int   unit;
	int   cpu;
	}AMP_CPU_TABLE;

/* Table includes only devices we want to filter 
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
* sysDeviceFilter - Called from vxbus to prevent device from being 
*                   announced and therefore used by vxWorks. The default 
*                   is to return OK.
*
* RETURNS: OK or ERROR
*
* /NOMANUAL
*/

STATUS sysDeviceFilter(VXB_DEVICE_ID pDev)
    {

    /* Compare devices name and unit number with those you want to remove 
     * at run-time. Used here to selectively use devices on one cpu vs another.
     */
    int deviceIndex;
    BOOL deviceMatch;

    deviceMatch=FALSE;

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

    /* if device match then we need to decide wether to filter */

    if(ampCpuTable[deviceIndex].cpu!=(sysProcNumGet()))
	return(ERROR);
							 
    return(OK);
    }

#endif /* _WRS_VX_AMP */

/*****************************************************************************
*
* divisorGet - Get the clock divider value.
*
* This routine returns the clock divider value for the given baud rate.
*
* RETURNS: Clock divider value
*
* /NOMANUAL
*/

UINT32 divisorGet
    (
    UINT32 xtal,
    UINT32 baud
    )
    {
    return (xtal / (16 * baud));
    }

/*****************************************************************************
*
* startCPU1 - Starts CPU1 executing code.
*
* RETURNS: NONE
*
* /NOMANUAL
*/

void startCPU1()
    {
    *MCM_PCR(CCSBAR) = MCM_PCR_CORE1_ENABLE | MCM_PCR_CORE0_ENABLE;
    WRS_ASM("isync;sync;eieio");
    }

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

void sysAmpCpuEnable(FUNCPTR entryPt,UINT32 cpu)
    {
	/* Here if core already running then nmaybe be spinning 
     * bootimage should then spin waiting for CPU1_INIT_START_ADR to be 
     * non-zero 
     */
    *((volatile int *)((UINT32)CPU1_FUNC_START_ADR | CORE1T_OFFSET)) = 
	  (int)0; /* SMP specific */
	/* SMP specific not used just loaded */
    *((volatile int *)((UINT32)CPU1_STACK_START_ADR | CORE1T_OFFSET)) = 
	  ((int)entryPt - 0x20); 
	/* This is used if core waiting after core1 did asynchronous reboot */
    *((volatile int *)((UINT32)CPU1_INIT_START_ADR | CORE1T_OFFSET)) = 
	  (int)entryPt;

    /* if cpu already running no need to do this */
    if((*MCM_PCR(CCSBAR)&MCM_PCR_CORE1_ENABLE)==0x0)
	{	
	*M86XX_BPTR(CCSBAR) = 
	  ((((CORE1T_OFFSET |(UINT32)entryPt) >>  BPTR_BOOT_PAGE_SHIFT ) & 
	    BPTR_BOOT_PAGE_MASK) | BPTR_EN);
		
	WRS_ASM("isync;sync");
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
    
	/* Here we have to send to bootrom and let wrload reload image when ready 
	 * bootimage should spin waiting for CPU1_INIT_START_ADR not to be the 
	 * magic pattern 
	 */
    
	*((volatile int *)(CPU1_FUNC_START_ADR | CORE1T_OFFSET)) = (int)0;
	/* SMP specific not used just loaded */
	*((volatile int *)(CPU1_STACK_START_ADR | CORE1T_OFFSET)) = 
	  (int)(RAM_LOW_ADRS - FRAMEBASESZ);
	*((volatile int *)(CPU1_INIT_START_ADR | CORE1T_OFFSET)) = 
	  (int)MP_MAGIC_RBV;

	WRS_ASM("sync");

	sysCpu1Stop();

	return OK;
	}

    /* Cannot prep core 0 or any other core */
    return ERROR;
    }

#endif /* INCLUDE_WRLOAD */

/*****************************************************************************
*
* sysBusTas - Test and Set.
*
* RETURNS: STATUS
*
* /NOMANUAL
*/

STATUS sysBusTas
    (
    char *adrs
    )
    {
	return (vxTas(adrs));
    }

/*****************************************************************************
*
* sysBusClear - Clears a reservation.
*
* RETURNS: NONE
*
* /NOMANUAL
*/

void sysBusClear
    (
    char *adrs
    )
    {
    *adrs=0;
    WRS_ASM("isync;sync;eieio");
    }

/*****************************************************************************
*
* sysBusIntGen - Interrupts adjacent CPU
*
* RETURNS: STATUS
*
* /NOMANUAL
*/

STATUS sysBusIntGen
    (
    int level,          /* interrupt level to generate   */
    int vector          /* interrupt vector for interrupt*/
    )
    {
#if defined(_WRS_VX_AMP)
    vxIpiEmit (0, (1 << !CPU1CHECK));
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


/****************************************************************************
*
* sysMailboxDisable - disable the mailbox interrupt
*
* This routine disables the mailbox interrupt.
*
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

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*/

char * sysModel (void)
    {
    static BOOL first = TRUE;

    if (first)
        {
        /* cut 'D' off end of sysModelStr if running on a single-core 8641 */
        if ((sysSvrGet() & _PPC_SVR_VER_MASK) == _PPC_SVR_MPC8641
            && sysModelStr[strlen(sysModelStr) - 1] == 'D')
            {
            sysModelStr[strlen(sysModelStr) - 1] = '\0';
            }
        /* if MP, report AMP vs SMP configuration */
        else if (core1ampMode)
            strcat(sysModelStr, " (AMP[#])");
        else
            strcat(sysModelStr, " (SMP[#])");
        sysModelStr[strlen(sysModelStr) - 3] = '0' + physCpuAvailableGet();
        first = FALSE;
        }
    return (sysModelStr);
    }

#if     defined (INCLUDE_ALTIVEC)
/*******************************************************************************
*
* sysAltivecProbe - Check if the CPU has ALTIVEC unit.
*
* This routine returns OK it the CPU has an ALTIVEC unit in it.
* Presently it checks for 7400
* RETURNS: OK  -  for 7400 Processor type
*          ERROR - otherwise.
*/

int  sysAltivecProbe (void)
    {
    ULONG regVal;
    int altivecUnitPresent = ERROR;

     /* The CPU type is indicated in the Processor Version Register (PVR) */

     regVal = CPU_TYPE;

     switch (regVal)
       {
          case CPU_TYPE_7400:
          case CPU_TYPE_7410:
          case CPU_TYPE_7450:
          case CPU_TYPE_7455:
          case CPU_TYPE_7457:
          case CPU_TYPE_7447A:
	      altivecUnitPresent = OK;
             break;

          default:
	      altivecUnitPresent = OK;
             break;
         }      /* switch  */

    return (altivecUnitPresent);
    }
#endif  /* INCLUDE_ALTIVEC */

/*******************************************************************************
*
* sysInfo - return some information on the CPU board configuration
*
* This routine returns a string containing the L2 backside cache size,
* if enabled, the PCI map selected, and whether NVRAM is selected.
* The string may contain line feeds. For example,
*
*      512KB L2 backside cache enabled
*      PCI Memory Map B
*      NVRAM enabled
*
* RETURNS: A pointer to the string.
*/

char * sysInfo (void)
    {

#ifdef USER_750_L2BACK
	switch (L2_CACHE_BACK_SIDE_SIZE)
	    {
	    case L2CR_QM:
		 strcpy(sysInfoStr, "256KB L2 backside cache enabled\n");
		 break;

	    case L2CR_HM:
		 strcpy(sysInfoStr, "512KB L2 backside cache enabled\n");
		 break;

	    case L2CR_1M:
		 strcpy(sysInfoStr, "1M L2 backside cache enabled\n");
		 break;

	    default:
		 break;
	    } /*switch */
#endif /* USER_750_L2BACK */

#ifdef  INCLUDE_NVRAM
	strcat(sysInfoStr, "NVRAM enabled\n");
#else
	strcat(sysInfoStr, "No NVRAM enabled\n");
#endif  /* INCLUDE_NVRAM */

    return (sysInfoStr);
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number, for
* example, 1.1/0. BSP_REV is concatenated to BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

UINT32 e500Ratio,platRatio;

UINT32 sysClkFreqGet
(
void
)
    {
    UINT32  sysClkFreq;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(CCSBAR);

#ifdef FORCE_DEFAULT_FREQ
    return(DEFAULT_SYSCLKFREQ);
#endif

    if ((platRatio>MAX_VALUE_PLAT_RATIO)||(platRatioTable[platRatio][0]==0))
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

    sysClkFreq = ((UINT32)(OSCILLATOR_FREQ * platRatioTable[platRatio][0]))>>((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(CCSBAR);
    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0]))>>((UINT32)e500RatioTable[e500Ratio][1]);


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
    UINT32  picClockDivisor     /* 0 => external "RTC" input */
    )
    {
    if (picClockDivisor > 0)
        return sysClkFreqGet() / picClockDivisor;
    else
        return HPCN_RTC_FREQ;   /* 14.318 MHz */
    }


void pcieBootromInit()
{	/* boot hold_off mode ,config CPU B begin----ff---2009.11.05*/
	UINT32 addr_b,value_switch,value=0;
	int CPUID,busNo=0,portNo=0;
	CPUID = sysGetCpuID();
	 if (CPUID == 0)	 
	 {
		 
		 //sjy 2013.07.26 added for 433
			 *(UINT32*)PCI_CFG_ADR_REG = 0x80010070;
			 while(*(UINT32*)PCI_CFG_ADR_REG != 0x80010070)
			 {
				 *(UINT32*)PCI_CFG_ADR_REG = 0x80010070;
			 }
			 *(UINT32*)(PCI_CFG_DATA_REG) = 0x20000000;

			 *(UINT32*)PCI_CFG_ADR_REG = 0x80020070;

			 while(*(UINT32*)PCI_CFG_ADR_REG != 0x80020070)
			 {
				 *(UINT32*)PCI_CFG_ADR_REG = 0x80020070;
			 }
			 *(UINT32*)(PCI_CFG_DATA_REG) = 0x20000000;
//			 *(UINT32*)(PCI_CFG_DATA_REG) = 0x20000000;
//			 *(UINT32*)(0x50000000) = *(UINT32*)PCI_CFG_ADR_REG,PCI_CFG_DATA_REG;
//			 *(UINT32*)(0x50000004) = PCI_CFG_DATA_REG;
//			 *(UINT32*)(0x50000008) = *(UINT32*)(PCI_CFG_DATA_REG);
//			 ns16550Pollprintf("bus 2\n"); //printf("bus 1 0x%x\n",*(UINT32*)(PCI_CFG_ADR_REG));
//			 		 *(UINT32 *)(0x20000004) = *(UINT32*)(PCI_CFG_ADR_REG);
			 		 
			 		 
			 		 
	//printf("bus 2 0x%x\n",*(UINT32*)(PCI_CFG_ADR_REG));
			 //sjy 2013.07.26 added for 433
		 
		 *(UINT32*)PCI_CFG_ADR_REG = 0x80010010;
		 while(*(UINT32*)PCI_CFG_ADR_REG != 0x80010010)
		 		*(UINT32*)PCI_CFG_ADR_REG = 0x80010010;
		 value_switch = LONGSWAP(*(UINT32*)PCI_CFG_DATA_REG);
		 *(UINT32*)(value_switch + 0x70) = 0x20000000;
		 
		 *(UINT32*)PCI_CFG_ADR_REG = 0x80080010; 
		 while(*(UINT32*)PCI_CFG_ADR_REG != 0x80080010)
			 *(UINT32*)PCI_CFG_ADR_REG = 0x80080010;
		 addr_b = LONGSWAP(*(UINT32*)PCI_CFG_DATA_REG);

	
		  	
		 if(((*(UINT32*)(addr_b+0x1010))&0x01000000)==0)
		 {
			value=addr_b;
			busNo = 8;
			portNo = 8; 
			*(UINT32*)(value+0x1010)=0x01000000;
			*(UINT32*)(CCSBAR+0x8000)=0x80000054 + 0x10000*busNo;
			*(UINT32*)(CCSBAR+0x8004)=0x30280000;
			*(UINT32*)(value_switch + 0x70 +portNo*0x1000) = 0x20000000;
		 }
			
	}
}
#ifdef INCLUDE_PCI_BUS
void pcieBootInit()
{	/* boot hold_off mode ,config CPU B begin----ff---2009.11.05*/	
	UINT32 value=0,value_b,value_switch,j;/**/
	int busNo=0,portNo=0;		
	if (sysGetCpuID() == 0)	 
	 {
		
		 //sjy 2013.07.26 added for 433
			 *(UINT32*)PCI_CFG_ADR_REG = 0x80010070;
			 while(*(UINT32*)PCI_CFG_ADR_REG != 0x80010070)
			 {
				 *(UINT32*)PCI_CFG_ADR_REG = 0x80010070;
			 }
			 *(UINT32*)(PCI_CFG_DATA_REG) = 0x20000000;

			 *(UINT32*)PCI_CFG_ADR_REG = 0x80020070;

			 while(*(UINT32*)PCI_CFG_ADR_REG != 0x80020070)
			 {
				 *(UINT32*)PCI_CFG_ADR_REG = 0x80020070;
			 }
			 *(UINT32*)(PCI_CFG_DATA_REG) = 0x20000000;
//			 *(UINT32*)(PCI_CFG_DATA_REG) = 0x20000000;
//			 *(UINT32*)(0x50000000) = *(UINT32*)PCI_CFG_ADR_REG,PCI_CFG_DATA_REG;
//			 *(UINT32*)(0x50000004) = PCI_CFG_DATA_REG;
//			 *(UINT32*)(0x50000008) = *(UINT32*)(PCI_CFG_DATA_REG);
//			 ns16550Pollprintf("bus 2\n"); //printf("bus 1 0x%x\n",*(UINT32*)(PCI_CFG_ADR_REG));
//			 		 *(UINT32 *)(0x20000004) = *(UINT32*)(PCI_CFG_ADR_REG);
			 		 
			 		 
			 		 
	//printf("bus 2 0x%x\n",*(UINT32*)(PCI_CFG_ADR_REG));
			 //sjy 2013.07.26 added for 433
		
		*(UINT32*)PCI_CFG_ADR_REG = 0x80080010; 
		 while(*(UINT32*)PCI_CFG_ADR_REG != 0x80080010)
			 *(UINT32*)PCI_CFG_ADR_REG = 0x80080010;
		 value_b = LONGSWAP(*(UINT32*)PCI_CFG_DATA_REG);				 
					 
		*(UINT32*)PCI_CFG_ADR_REG = 0x80010010;
		while(*(UINT32*)PCI_CFG_ADR_REG != 0x80010010)
			*(UINT32*)PCI_CFG_ADR_REG = 0x80010010;
		value_switch = LONGSWAP(*(UINT32*)PCI_CFG_DATA_REG);
		*(UINT32*)(value_switch + 0x70) = 0x20000000;
		
		value=value_b;
		busNo = 8;
		portNo = 8;
		
		if (value!=0xffff0000)		
		{/*  Set port0 enable bit */
			for(j=0;j<0x10000;j++);

			*(UINT32*)(value+0x8c20)=0x40000;
			*(UINT32*)(value+0x8c28)=0x80000;
			*(UINT32*)(value+0x8c30)=0x8004401c;  /*  1d->1c  */
/* define cpub/c/d outbound */	
			*(UINT32*)(value+0x8c40)=0xa0000;     /*  ff add	*/
			*(UINT32*)(value+0x8c48)=0xa0000;
			*(UINT32*)(value+0x8c50)=0x8004401b;
			
			/* define CPUB/c/d PEXIB_TAR/ATTR 1*/		

			*(UINT32*)(value+0x8000+M85XXPCI_IB_TRANS_ADRS_REG1)=PCIE_MAP_ADDR>>12;
			*(UINT32*)(value+0x8000+M85XXPCI_IB_ATTR_REG1)=PCI_WINDOW_ENABLE_BIT|PCI_IN_ATTR_TGI_LM|PCI_IN_ATTR_RTT_LM_READ_SNOOP|PCI_IN_ATTR_RTT_LM_WRITE_SNOOP|PCI_ATTR_WS_64M;/*0x80F44019*/
/* config cpu b,c,d's pex outbound windown for memory map ----end---ff*/
		
			/*   MSI SUPPORT  ----begin  */
			*(UINT32*)PCI_CFG_ADR_REG = 0x80000070 + 0x10000*busNo;
			*(UINT32*)PCI_CFG_DATA_REG = 0x05000100;
				
			/* 0X74 MSI Addr */
			*(UINT32*)PCI_CFG_ADR_REG = 0x80000074 + 0x10000*busNo;
			*(UINT32*)PCI_CFG_DATA_REG = 0x40170480; /* 0x40170480 MSIIR*/
		
			/*  MSI SUPPORT   -----end   */			
			/*  add for pcie preformance ----begin  */
			*(UINT32*)(CCSBAR+0x8000)=0x80000054 + 0x10000*busNo;
			*(UINT32*)(CCSBAR+0x8004)=0x30280000;
			*(UINT32*)(value_switch + 0x70 +portNo*0x1000) = 0x20000000;			
			/*  add for pcie preformance ----end  */		
		}
	}
}
#endif


void bspMsiInit()
{
	int i;
	UINT32 epic_smsg_reg;
	for(i=0;i<8;i++)
	{
		epic_smsg_reg = EPIC_SMSG_VEC_REG(i) ;
		*(UINT32*)(CCSBAR + epic_smsg_reg) = (EPIC_VEC_SMSG_IRQ0+i)+0x40000;
	}	
}
/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various hardware features of the board.
* It is the first board-specific C code executed, and runs with
* interrupts masked in the processor.
* This routine resets all devices to a quiescent state, typically
* by calling driver initialization routines.
*
* NOTE: Because this routine may be called from sysToMonitor, it must
* shutdown all potential DMA master devices.  If a device is doing DMA
* at reboot time, the DMA could interfere with the reboot. For devices
* on non-local busses, this is easy if the bus reset or sysFail line can
* be asserted.
*
* NOTE: This routine should not be called directly by any user application.
*
* RETURNS: N/A
*/
void sysHwInit (void)
    {
	int cpuId;
#if defined(INCLUDE_MMU)
    PHYS_MEM_DESC * pMmu;
    int ix;
#endif
#ifdef COMPILE_BOOTROM
    int sysSlotNum;
	int i=0;

    BOOT_PARAMS bp;
    UINT8 temp,len=0,ii=0;
	char *pTemp=NULL;
	char *pTemp1=NULL;
#endif	

/* added by freeWinder for open LAW for RapidIO */
   /* sysLedSet(sysGetCpuID()+1,0);*/
    ns16550PollOutputInit();
    ns16550Pollprintf("enter sysHwInit\n");
   sysCpuLawInit();
   cpuId = sysGetCpuID();
#ifdef COMPILE_BOOTROM
     if (*(BOOT_LINE_ADRS) == EOS)
     {
          sysNvRamGet(BOOT_LINE_ADRS,0xff,0);
          (void) bootStringToStruct (BOOT_LINE_ADRS, &bp);
          if(bp.ead!=NULL)
          {
             char *pchar = bp.ead;
             pTemp= pchar;
             while((*pchar!=EOS)&&((pchar-bp.ead)<50)&&(*pchar!=':'))
             {
            	if(*pchar == '.') pTemp = (pchar+1);
            	pchar++;
             }
             temp = (atoi(pTemp) + cpuId);
             sprintf(pTemp1,"%d",temp);
             len = strlen(pTemp1);
             pchar -= len;
             for(ii=0;ii<len;ii++)
            	 *pchar++ = *pTemp1++;
             (void) bootStructToString (BOOT_LINE_ADRS, &bp);
           }
       }
#endif
  
   /******************************
	  if running in CPUA, then start CPUB 
	  *******************************/
#ifdef COMPILE_BOOTROM
	  for(i=0;i<0x40;i++)
	  	*(UINT32*)(i*4)=0;
	   sysSlotNum = sysSlotNumGet();
	   //ns16550Pollprintf("version:2_4.5.1.2/20120514\n");  //usr ddr2 
//	   ns16550Pollprintf("version:1_4.5.1.2/20120514\n");  //usr ddr1 
//	   if (cpuId == 0)
//	   {
//		   ns16550Pollprintf("This is CPUA,");
//		   ns16550Pollprintf("start CPUBCD\n");
//	       *(volatile int*)0xe1000008 |= 0xf;
	  //     if ((sysSlotNum != 0x1))
	 //      		*(int*)(CCSBAR+0xc0060) |= 0xffffff;
//	   }
	   if(sysSlotNum != 0xff)  *(UINT32*)(CCSBAR+0xc0008) = sysSlotNumGet()*4+cpuId;	
	   else *(int*)(CCSBAR+0xc0008) = 0xff;
	   *(int*)(CCSBAR+0xc0060) |= 0xffffff;
#endif


#ifdef INCLUDE_AMP
#ifdef LEGACY_AMP
    BOOT_PARAMS localBootParams;
    memset(&localBootParams,0,sizeof(BOOT_PARAMS));
#endif
#endif

#ifdef _WRS_VX_AMP
#ifndef INCLUDE_AMP_CPU_01
	sysAmpCpuStatusSet(ampCpuStateReboot,1);
	sysAmpCpuStatusSet(ampCpuStateRunning,0);
#else
	sysAmpCpuStatusSet(ampCpuStateRunning,1);
#endif
#endif /* _WRS_VX_AMP */

    /*
     * First, automatically detect our memory mode (PREP-A or CHRP-B)
     * and setup proper global variable values for the mapping.
     * Do this before _anything_ else in sysHwInit or else addresses
     * will not be setup correctly for many other routines.
     * Note that Sandpoint uses CHRP, and PREP is
     * currently untested and unsupported.
     */

     vxHid1Set(vxHid1Get() | HID1_SYNCBE | HID1_ABE | HID1_EMCP);

#ifdef _WRS_CONFIG_SMP
	/* Here we initialize cpu1 init address so we don't wait until the 
	 * correct value is initialized 
	 */
	*(UINT32*)CPU1_INIT_START_ADR = MP_MAGIC_RBV;
#endif

#if defined(INCLUDE_MMU)
/*
 *  Setup the function pointers for the BAT initialization functions.
 *  (Standard 4 I/D BAT's and extra 4 I/D BAT's
 */

/* Setup MMU if needed */

     _pSysBatInitFunc = (FUNCPTR) mmuPpcBatInitMPC74x5;


    /*
     * run-time update of the sysPhysMemDesc MMU entry for main RAM
     * This means SDRAM _must_ be represented by sysPhysMemDesc[1]
     */

    sysPhysMemDesc[1].len = (UINT)(sysMemTop () - RAM_LOW_ADRS);


    /* calculate the number of sysPhysMemDesc entries in use */

    pMmu = &sysPhysMemDesc[0];

    for (ix = 0; ix < NELEMENTS (sysPhysMemDesc); ix++)
	{
        if (pMmu->virtualAddr != (VIRT_ADDR)DUMMY_VIRT_ADDR)
            pMmu++;	/* used entry */
        else
            break;	/* dummy entry, assume no more */
	}

    /* This is the REAL number of sysPhysMemDesc entries in use. */

    sysPhysMemDescNumEnt = ix;
#endif /* INCLUDE_MMU */

    /*
     * Detect core 1 low-memory offset (AMP) mode.
     *
     * This is used for reporting in sysModel(), and to avoid attempting
     * to start up CPU1 as an SMP processor if configured for AMP.
     */

    core1ampMode = M8XXX_PORDEVSR_CORE1TE & *M8XXX_PORDEVSR(CCSBAR);

#ifndef _WRS_CONFIG_SMP
#ifdef INCLUDE_AMP
#ifdef LEGACY_AMP
    /* Here the aim is to keep the boot line the same for both cpu0 and
     * cpu1 except to use shared memory as the network device.
     */

    {
    struct hcfDevice *hcfDeviceListCopy;
    UINT32 len;
    UINT32 loop;
    hcfDeviceListCopy = (struct hcfDevice *)hcfDeviceList;

    if (CPU1CHECK)
	{
        /* This make network use RAM based bootline which we manipulate
           for AMP */
        sysStartType=BOOT_NORMAL;
        loop = 0;

        /* Remove Init of pci if dual boot - PCI will belong to CPU0 */

        /* Leaves only PPC Dec TIMER and serial */
        while (hcfDeviceListCopy[loop].devName != NULL)
            {
            if (strcmp ("m85xxPci", hcfDeviceListCopy[loop].devName) == 0)
                strcpy (hcfDeviceListCopy[loop].devName, "dummy");
            if (strcmp ("openPicTimer", hcfDeviceListCopy[loop].devName) == 0)
                strcpy (hcfDeviceListCopy[loop].devName, "dummy");
            if (strcmp ("m85xxCCSR", hcfDeviceListCopy[loop].devName) == 0)
                strcpy (hcfDeviceListCopy[loop].devName, "dummy");
            loop++;
            }

	bootStringToStruct ((char *)BOOT_LINE_ADRS,(BOOT_PARAMS *)&localBootParams);
	memset (BOOT_LINE_ADRS,0,255);

	/* Only boot device available is sm0 - so setup boot params
	 * accordingly.
	 */
	strcpy (&localBootParams.bootDev[0],"sm");
	localBootParams.unitNum = 0;

	strcpy (&localBootParams.gad[0],&localBootParams.bad[0]);

        /* Change backplane ipAddress to +/- cpu0. Assume not X.X.X.25X */
	len = strlen (&localBootParams.bad[0]);
	if (localBootParams.bad[len-1]=='9')
	    (*(UINT8*)&localBootParams.bad[(len-1)]) -=1;
	else
	    (*(UINT8*)&localBootParams.bad[(len-1)]) +=1;

	localBootParams.procNum = 1;

        strcpy (&localBootParams.ead[0], &localBootParams.bad[0]);

	bootStructToString((char *)BOOT_LINE_ADRS,(BOOT_PARAMS *)&localBootParams);
	}
    else
	{
	/* Copy user bootline. Assume using same image */
	strcpy ((char*)(CORE1T_OFFSET+BOOT_LINE_ADRS),(char*)BOOT_LINE_ADRS);
	}

    if (!core1ampMode)
        {
        strcpy (sysExcMsg, "Board in SMP mode. Switch to low memory "
            "offset mode(AMP) and power down to restart");
        sysToMonitor (BOOT_WARM_NO_AUTOBOOT);
        }

    }
#endif /* LEGACY_AMP */
#endif /* INCLUDE_AMP */    
#endif /* ndef _WRS_CONFIG_SMP */
    
/*add by ff----2010.04.28---end*/  
#ifndef COMPILE_BOOTROM  
#ifdef INCLUDE_PCI_BUS
    
    if (cpuId == 0) pcieBootInit(); //hdz 20121205
#endif
#endif
#ifdef INCLUDE_TFFS_MOUNT
  // if(cpuId != 0) *(UINT8*)(BOOTROM_TFFS_SYNC_REG+1) = 0;
#endif
/*add by ff----2010.04.28---end*/ 
           
#ifdef INCLUDE_VXBUS
    pSysPlbMethods = plbMethods;
    hardWareInterFaceInit();
#endif /* INCLUDE_VXBUS */
    
#ifdef INCLUDE_LCD
    sysLcdInit();
    sysLcdSbcSet();
#endif /* INCLUDE_LCD */

#ifdef INCLUDE_CACHE_SUPPORT

#ifdef INCLUDE_CACHE_L2

    sysL2CacheInit();

    _pSysL2CacheInvFunc = sysL2CacheGlobalInv;
    _pSysL2CacheEnable  = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;

    _pSysL2CacheFlush = sysL2CacheHWFlush;

#endif /* INCLUDE_CACHE_L2 */
#endif /* INCLUDE_CACHE_SUPPORT */
 
#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine,DEFAULT_BOOT_LINE,strlen(DEFAULT_BOOT_LINE)+1);
    *(sysBootLine+55)+=cpuId; 
#endif /* FORCE_DEFAULT_BOOT_LINE */
    
   // bspRs485Enable();
    
    ns16550Pollprintf("exit sysHwInit\n");
    }

/*******************************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupt vectors and configures any
* required features not configured by sysHwInit. It is called from usrRoot()
* in usrConfig.c (via sysClkConnect) after the multitasking kernel has started.
*
* RETURNS: N/A
*/

extern void rioMsgQinit(void);
extern STATUS srioIntSetup();
extern void rioShowRapidIoInfo();
extern void sysLedFlash(UINT32 time);
extern STATUS sysFpgaTimerInit();
//extern  void sysStatusUpdata();
//extern void sysTaskTempUpdata(UINT8 regOffset);

void sysHwInit2 (void)
    {
    static int	initialized;		/* Do this ONCE */
    int cpuId;

    ns16550Pollprintf("enter sysHwInit2\n");
 
    cpuId = sysGetCpuID();
    if (!initialized)
        {
        initialized = TRUE;
        /* initialize the EPIC interrupts */
     
#ifdef  INCLUDE_VXBUS
	vxbDevInit();
#endif /* INCLUDE_VXBUS */
 
#ifdef INCLUDE_SIO_UTILS
    sysSerialConnectAll();
#endif

#ifdef INCLUDE_ALTIVEC
        _func_altivecProbeRtn = sysAltivecProbe;
#endif  /* INCLUDE_ALTIVEC */
/*add by ff----2009.12.10---begin*/
     
#ifdef COMPILE_BOOTROM	 
#if 0
	 if (cpuId == 0)       /* add by ff 2010.08.02  */
     {
     *(unsigned int*)(CCSBAR+0x8da0) = 0xe0000000>>12;
     *(unsigned int*)(CCSBAR+0x8da8) = 0xe0000000>>12;
     *(unsigned int*)(CCSBAR+0x8db0) = 0x80f4401c;
     }	 
#endif
	 if (cpuId == 0)  pcieBootromInit();	  	 
#endif     
#ifdef VXB_M8641_DMA     
     sysDmaInit();
#endif   

#ifndef COMPILE_BOOTROM
#ifdef INCLUDE_TFFS    
    flashIoInit();
#endif   
    bspMsiInit();
#endif 

        


#ifdef	INCLUDE_RAPIDIO_BUS
#if 0
     if((sysGetCpuID() == 0)&&(rioGetID()==0))
        /* if(sysGetCpuID() == 0)*/
    	{
        	ns16550Pollprintf("enumerate rapidIo device.....\n"); 
    		if(rioScanDevice()==ERROR)ns16550Pollprintf("enumerate rapidIo device ERROR!\n");
    		else ns16550Pollprintf("enumerate rapidIo device done!\n"); 
    	}
    	else
#endif
    	{
			*(UINT32*)(CCSBAR+0xc013c) |=0x40000000;
    	}
#if 0
        rioMsgQinit();
        if(srioIntSetup()==ERROR) ns16550Pollprintf(" srioIntSetup ERROR!!!\n");     
#else
        if(rioDoorBellInit()==ERROR) ns16550Pollprintf(" srioIntSetup ERROR!!!\n");    
#endif
#endif    
  
       // sysFpgaTimerInit();//hdz
      /* sysLedSet(sysGetCpuID()+1,1); *//*led on*/
   
#if 0
#ifdef COMPILE_BOOTROM
       taskSpawn ("tLedFlash",200,0,10000,(FUNCPTR)sysLedFlash,60,0,0,0,0,0,0,0,0,0); 

#else
      sysLedSet(cpuId,1); 
#endif
#endif
       
#ifdef  INCLUDE_VXBUS
	taskSpawn ("devConnect",0,0,10000,vxbDevConnect,0,0,0,0,0,0,0,0,0,0);
#endif /* INCLUDE_VXBUS */
	} 
    sysPhyInit(7);
    sysPhyInit(8);
    bspInit(); //hdz add 2012.11.30
#ifndef COMPILE_BOOTROM
#ifdef INCLUDE_TFFS_MOUNT
    if(cpuId != 0)
    {	
    	 
    	UINT beginTimeH,beginTimeL,endTimeH,endTimeL;
    	float timeSpend;
    	//UINT32 clkFreq;
    	vxTimeBaseGet(&beginTimeH,&beginTimeL);
    	ns16550Pollprintf("wait for CPUA PCIE AUTO CONFIG COMPLETE..\n");
    	while(((*(UINT32*)(0x3c000000)) & 0xffffffff) != 0x5a5a5a5a)
    	{
    		vxTimeBaseGet(&endTimeH,&endTimeL);
    		if(endTimeL<beginTimeL) timeSpend = (endTimeL+0xffffffff-beginTimeL) + (endTimeH-1-beginTimeH)*4.0*1024*1024*1024;
    		else  timeSpend = (endTimeL-beginTimeL) + (endTimeH-beginTimeH)*4.0*1024*1024*1024;
    		timeSpend = timeSpend/125000000;
    		if(timeSpend>=10)
    		{
    			//ns16550Pollprintf("timeout\n");
    			break;
    		}  
    	}
    	*(UINT32*)(0x3c000000) =0;
    }
#endif
#endif  
 
   
    ns16550Pollprintf("exit syshwinit2\n"); 
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
* NOTE: Do no adjust LOCAL_MEM_SIZE to reserve memory for application
* use.  See sysMemTop() for more information on reserving memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    static char * physTop = NULL;

    if (physTop == NULL)
	{
#ifdef LOCAL_MEM_AUTOSIZE

	/* TODO - If auto-sizing is possible, this would be the spot.  */

#	error	"Dynamic memory sizing not supported"

#else
	/* Don't do autosizing, size is given */

	physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

#endif /* LOCAL_MEM_AUTOSIZE */
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
    intCpuLock();

    /*
     * Per the MPC8641D manual, the CPU should be in sleep or nap state prior
     * to issuing an HRESET via the processor reset register in the PIC.
     */

    vxHid0Set (vxHid0Get () | _PPC_HID0_NAP);

    FOREVER
        {
        }

    }
#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot()--which services ^X--and by bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* The entry point for a warm boot is defined by the macro ROM_WARM_ADRS
* in config.h.  We do an absolute jump to this address to enter the
* ROM code.
*
* RETURNS: Does not return.
*/

STATUS sysToMonitor
    (
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR	pRom = (FUNCPTR) (ROM_WARM_ADRS);
#ifdef _WRS_CONFIG_SMP
    cpuset_t cpuset;
#endif /* _WRS_CONFIG_SMP */
#ifdef _WRS_VX_AMP
    volatile int sysMonDelay;
#endif
	int    s = intCpuLock ();				/* disable interrupts */
	
	//startType = BOOT_COLD;   //added by sjy 2013.06.18 
	
#ifdef _WRS_CONFIG_SMP

    /*
     * In SMP mode, sysToMonitor() must run on core 0. If we already happen
     * to already be on core 0, then we can just put CPU 1 into a parking loop
     * and proceed with shutdown. If not, we have to use a cross-processor
     * call to re-invoke ourselves on core 0 first.
     */

    CPUSET_ZERO (cpuset);
    if (vxCpuIndexGet() == 0)
        {
        cpcInvoke (cpuset, (CPC_FUNC)sysCpu1ShutdownLoop,
            NULL, 0, VX_CPC_ASYNC);
        sysDelay ();
        }
    else
        {
        cpcInvoke (cpuset, (CPC_FUNC)sysToMonitor, (void *)startType,
            sizeof(startType), VX_CPC_ASYNC);
        intCpuUnlock (s);
        return (OK);
        }

    /* XXX cacheDisable is not supported in SMP */

    cacheArchDisableFromMmu (INSTRUCTION_CACHE);
    cacheArchDisableFromMmu (DATA_CACHE);
#else   /* _WRS_CONFIG_SMP */
    cacheDisable (INSTRUCTION_CACHE);   /* Disable the Instruction Cache */
    cacheDisable (DATA_CACHE);          /* Disable the Data Cache */
#endif  /* _WRS_CONFIG_SMP */


#if     (CPU == PPC604)
    vxHid0Set (vxHid0Get () & ~_PPC_HID0_SIED);	/* Enable Serial Instr Exec */
#endif  /* (CPU == PPC604) */

    /* Turn off timer */

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif

    vxMsrSet (0);		/* Clear the MSR */

#ifdef  _WRS_CONFIG_SMP
    /*
     * We need to forcibly reset the second core. This is done
     * using the processor reset register in the PIC. Note that
     * this register is only available on rev 2.0 and later of the
     * 8641D silicon. Luckily, the Wind River SBC8641D board has
     * a rev 2 chip.
     *
     * Note also that the bits in the EPIC Hard Reset Register are
     * not auto-clearing.
     */

    *(M8XXX_EPICPRR(CCSBAR)) |= M8XXX_EPICPRR_HRST_CORE1; /* Set HRESET */
    sysCpu1Stop ();
    *(M8XXX_EPICPRR(CCSBAR)) &= ~M8XXX_EPICPRR_HRST_CORE1; /* Clear HRESET */
#endif  /* _WRS_CONFIG_SMP */

#ifdef _WRS_VX_AMP

    /* Clear boot page so back to flash */
    *M86XX_BPTR(CCSBAR) = 0x0;
    
    /* Here we have to send to bootrom and let wrload reload image when ready 
     * bootimage should spin waiting for CPU1_INIT_START_ADR not to be the 
     * magic pattern 
     */
#ifdef INCLUDE_AMP_CPU_00	
    
    *((volatile int *)(CPU1_FUNC_START_ADR | CORE1T_OFFSET)) = (int)0;
    /* SMP specific not used just loaded */
    *((volatile int *)(CPU1_STACK_START_ADR | CORE1T_OFFSET)) = 
      (int)(RAM_LOW_ADRS - FRAMEBASESZ);
    *((volatile int *)(CPU1_INIT_START_ADR | CORE1T_OFFSET)) = 
      (int)MP_MAGIC_RBV;

    WRS_ASM("sync");

    sysCpu1Stop();
    /* Do not want to call out from this routine - use hard loop to 
     * delay instead 
     */ 
    for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);

    *(M8XXX_EPICPRR(CCSBAR)) |= M8XXX_EPICPRR_HRST_CORE1; /* Set HRESET */
    WRS_ASM("sync;isync");

    /* Do not want to call out from this routine - use hard loop to 
     * delay instead - delay allow hreset to be held for a short time 
     */ 
    for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);

    *(M8XXX_EPICPRR(CCSBAR)) &= ~M8XXX_EPICPRR_HRST_CORE1; /* Clear HRESET */
    WRS_ASM("sync;isync");

    /* Do not want to call out from this routine - use hard loop to 
     * delay instead 
     */ 
    for(sysMonDelay=0;sysMonDelay<0x10000;sysMonDelay++);
	
    sysCpu1Start();



#else /* INCLUDE_AMP_CPU_00 */

    *((volatile int *)(CPU1_FUNC_START_ADR)) = (int)0;
	/* SMP specific not used just loaded */
    *((volatile int *)(CPU1_STACK_START_ADR)) = (int)RAM_LOW_ADRS - FRAMEBASESZ;
    *((volatile int *)(CPU1_INIT_START_ADR)) = (int)MP_MAGIC_RBV;

    WRS_ASM("sync");

#endif /* INCLUDE_AMP_CPU_00 */
#endif /* _WRS_VX_AMP */
	
    (*pRom) (startType);	/* jump off to romInit.s */

    return (OK);		/* in case we continue from ROM monitor */
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
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return (CPU1CHECK);
    }

/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.
*
* For bus systems, it is assumes that processor 0 is the bus master and
* exports its memory to the bus.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int procNum			/* processor number */
    )
    {
    sysProcNum = procNum;

    if (procNum == 0)
        {
	}
    }

/*******************************************************************************
*
* sysCpuCheck - confirm the CPU type
*
* This routine validates the cpu type.  If the wrong cpu type is discovered
* a message is printed using the serial channel in polled mode.
*
* RETURNS: N/A
*/

void sysCpuCheck (void)
    {
    /* Check for a valid CPU type;  If one is found, just return */

#if (CPU == PPC603)	/* five kinds of 603 */
    if ((CPU_TYPE == CPU_TYPE_603)  ||
	(CPU_TYPE == CPU_TYPE_603E) ||
        (CPU_TYPE == CPU_TYPE_603P) ||
	(CPU_TYPE == CPU_TYPE_750)  ||
	(CPU_TYPE == CPU_TYPE_8245)  ||
	(CPU_TYPE == CPU_TYPE_8240))
	{
	return;
	}

#elif (CPU == PPC604) /* four kinds of 604 */
    if	((CPU_TYPE == CPU_TYPE_604) ||
	(CPU_TYPE == CPU_TYPE_604E) ||
	(CPU_TYPE == CPU_TYPE_604R) ||
	(CPU_TYPE == CPU_TYPE_750)  ||
	(CPU_TYPE == CPU_TYPE_7400) ||
	(CPU_TYPE == CPU_TYPE_7410) ||
	(CPU_TYPE == CPU_TYPE_7450) ||
	(CPU_TYPE == CPU_TYPE_7455) ||
	(CPU_TYPE == CPU_TYPE_7457) ||
	(CPU_TYPE == CPU_TYPE_7448) ||
	(CPU_TYPE == CPU_TYPE_8641) ||
	(CPU_TYPE == CPU_TYPE_7447A))
	{
	return;
	}

#endif	/* (CPU == PPC604) */

	
    /* Invalid CPU type; polling print of error message and bail */

    sysToMonitor (BOOT_NO_AUTOBOOT);

    return;
    }


/*******************************************************************************
*
* sysDelay - delay for approximately one millisecond
*
* Delay for approximately one milli-second.
*
* RETURNS: N/A
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }

/*******************************************************************************
*
* sysMsDelay - delay for the specified amount of time (MS)
*
* This routine will delay for the specified amount of time by counting
* decrementer ticks.
*
* This routine is not dependent on a particular rollover value for
* the decrementer, it should work no matter what the rollover
* value is.
*
* A small amount of count may be lost at the rollover point resulting in
* the sysMsDelay() causing a slightly longer delay than requested.
*
* This routine will produce incorrect results if the delay time requested
* requires a count larger than 0xffffffff to hold the decrementer
* elapsed tick count.  For a System Bus Speed of 67 MHZ this amounts to
* about 258 seconds.
*
* RETURNS: N/A
*/

void sysMsDelay
    (
    UINT        delay                   /* length of time in MS to delay */
    )
    {
#ifdef	INCLUDE_TIMER_STUB
# ifdef DRV_TIMER_DEC_PPC
    register UINT oldval;               /* decrementer value */
    register UINT newval;               /* decrementer value */
    register UINT totalDelta;           /* Dec. delta for entire delay period */
    register UINT decElapsed;           /* cumulative decrementer ticks */

    /*
     * Calculate delta of decrementer ticks for desired elapsed time.
     * The macro DEC_CLOCK_FREQ MUST REFLECT THE PROPER 6xx BUS SPEED.
     */

    totalDelta = ((DEC_CLOCK_FREQ / 4) / 1000) * delay;

    /*
     * Now keep grabbing decrementer value and incrementing "decElapsed" until
     * we hit the desired delay value.  Compensate for the fact that we may
     * read the decrementer at 0xffffffff before the interrupt service
     * routine has a chance to set in the rollover value.
     */

    decElapsed = 0;

    oldval = vxDecGet ();

    while (decElapsed < totalDelta)
        {
        newval = vxDecGet ();

        if ( DELTA(oldval,newval) < 1000 )
            decElapsed += DELTA(oldval,newval);  /* no rollover */
        else
            if (newval > oldval)
                decElapsed += abs((int)oldval);  /* rollover */

        oldval = newval;
        }
# else  /* DRV_TIMER_DEC_PPC */
# error This BSP does not support INCLUDE_TIMER_STUB && !DRV_TIMER_DEC_PPC configuration!
# endif /* DRV_TIMER_DEC_PPC */
#else	/* INCLUDE_TIMER_STUB */
    vxbMsDelay (delay);
#endif	/* INCLUDE_TIMER_STUB */
    }

/*******************************************************************************
*
* sysIntConnect - connect the BSP interrupt to the proper epic/i8259 handler.
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
* sysNvRead - read one byte from NVRAM
*
* This routine reads a single byte from a specified offset in NVRAM.
*
* RETURNS: The byte from the specified NVRAM offset.
*/

UCHAR sysNvRead
    (
    ULONG	offset	/* NVRAM offset to read the byte from */
    )
    {

    return(0);

    }

/*******************************************************************************
*
* sysNvWrite - write one byte to NVRAM
*
* This routine writes a single byte to a specified offset in NVRAM.
*
* RETURNS: N/A
*/

void sysNvWrite
    (
    ULONG	offset,	/* NVRAM offset to write the byte to */
    UCHAR	data	/* datum byte */
    )
    {

    }


#ifdef INCLUDE_MMU
/*******************************************************************************
*
* sysMmuMapAdd - insert a new mmu mapping
*
* This routine adds a new mmu mapping entry to allow dynamic mappings.
*
* RETURNS: OK or ERROR depending on availability of free mappings.
*/

STATUS sysMmuMapAdd
    (
    VIRT_ADDR address,
    UINT32 length,
    UINT32 initialStateMask,
    UINT32 initialState
    )
    {
    PHYS_MEM_DESC *pMmu;
    STATUS result = OK;

    pMmu = &sysPhysMemDesc[sysPhysMemDescNumEnt];

    if(pMmu->virtualAddr != (VIRT_ADDR)DUMMY_VIRT_ADDR)
        result = ERROR;
    else
        {
        pMmu->virtualAddr = address;
        pMmu->physicalAddr = address;
        pMmu->len = length;
        pMmu->initialStateMask = initialStateMask;
        pMmu->initialState = initialState;
        sysPhysMemDescNumEnt += 1;
        }

    return (result);
    }
#endif /* INCLUDE_MMU */


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
* ERRNO
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
* ERRNO
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
* coreShow - display routine for core clocking
*
* RETURNS: NONE
*
* ERRNO
*
*/
void coreShow()
{
	printf("MPX/CCB Bus Speed %d\n",sysClkFreqGet());
	printf("Core Freq %d\n",coreFreq);
}

#ifdef INCLUDE_PCI_BUS

LOCAL UCHAR sysPci1IntRoute [NUM_PCIEX_SLOTS][4] = {{PCIEX_XINT1_LVL, PCIEX_XINT2_LVL, PCIEX_XINT3_LVL, PCIEX_XINT4_LVL}};

LOCAL UCHAR sysPci2IntRoute [NUM_PCIEX_SLOTS][4] = {{PCIEX2_XINT1_LVL, PCIEX2_XINT2_LVL, PCIEX2_XINT3_LVL, PCIEX2_XINT4_LVL}};

/*******************************************************************************
*
* sysPci1AutoconfigInclude - PCI 1 autoconfig support routine
*
* RETURNS: OK or ERROR
*/

STATUS sysPci1AutoconfigInclude
    (
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UINT devVend                        /* deviceID/vendorID of device */
    )
    {

    /*
     * Only support BUS 0;
     * Host controller itself (device number is 0) won't be configured;
     * Bridge on the Arcadia board (device number 17) won't be configured;
     */

    if ((pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
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
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UINT devVend                        /* deviceID/vendorID of device */
    )
    {

    /*
     * Only support BUS 0;
     * Host controller itself (device number is 0) won't be configured;
     */

    if ((pLoc->bus == 0 && pLoc->device == 0 && pLoc->function == 0))
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
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UCHAR pin                           /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    if ((pin > 0) && (pin < 5))
        tmpChar = sysPci1IntRoute [((pLoc->device))][(pin-1)];

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
    PCI_SYSTEM * pSys,                  /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,                     /* pointer to function in question */
    UCHAR pin                           /* contents of PCI int pin register */
    )
    {
    UCHAR tmpChar = 0xff;

    if ((pin > 0) && (pin < 5))
        tmpChar = sysPci2IntRoute [((pLoc->device))][(pin-1)];

    /* return the value to be assigned to the pin */

    return (tmpChar);
    }
#endif /* INCLUDE_PCI_BUS */

#if defined(_WRS_CONFIG_SMP)
/******************************************************************************
*
* sysCpuInit - initialize additional core
*
* RETURNS: NONE
*
*/

void sysCpuInit (void)
    {

    vxHid1Set (vxHid1Get() | HID1_SYNCBE | HID1_ABE | HID1_EMCP);

#ifdef  INCLUDE_CACHE_L2
    sysL2CacheInit();
#endif  /* INCLUDE_CACHE_L2 */

#ifdef  INCLUDE_CACHE_SUPPORT
/* XXX - can't use cacheEnable here, as it's already called by cpu0 */
#ifdef  USER_I_CACHE_ENABLE
    cacheArchEnable (INSTRUCTION_CACHE);        /* enable instruction cache */
#endif  /* USER_I_CACHE_ENABLE */

#ifdef  USER_D_CACHE_ENABLE
    cacheArchEnable (DATA_CACHE);               /* enable data cache */
#ifdef INCLUDE_CACHE_L2
    sysL2CacheEnable (DATA_CACHE);
#endif  /* INCLUDE_CACHE_L2 */
#endif  /* USER_D_CACHE_ENABLE */
#endif  /* INCLUDE_CACHE_SUPPORT */

#ifdef  INCLUDE_MMU
    mmuPpcCpuInit ();
#endif  /* INCLUDE_MMU */

#if (defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP))
    /*
     * Set the current timebase read from CPU0 onto CPU1.
     * The timebase is kept halt till it's re-started by the CPU0.
     */

    sysSmpTimeBaseSet (tbHI, tbLO);
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */
    }


unsigned int sysCpu1LoopCount = 0;

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

void sysCpu1Loop(void)
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

    p = (FUNCPTR)(*(int *)CPU1_FUNC_START_ADR);

    sysCpuInit ();

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

    if (cpuNum != 1 || core1ampMode )
        {
        return ERROR;
        }

    *((volatile int *)CPU1_FUNC_START_ADR) = (int)pCpuState->regs.pc;
    *((volatile int *)CPU1_STACK_START_ADR) = (int)pCpuState->regs.spReg;
    *((volatile int *)CPU1_INIT_START_ADR) = (int)sysCpu1Loop;

#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
    {
#ifndef	VX_ENABLE_CPU_TIMEOUT
#define VX_ENABLE_CPU_TIMEOUT	5	/* default timeout five seconds */
#endif	/* VX_ENABLE_CPU_TIMEOUT */

    cpuset_t    cpuset;
    UINT32      i = 0;

    /*
     * PPC Dec timer will not be available while synchronizing timebase
     * between two cores.
     * Use magic number, 0x9000000, to count 1 second independently
     * on PPC Dec timer.
     */

    UINT32      timeout = 0x9000000 * VX_ENABLE_CPU_TIMEOUT;

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

    *(M8XXX_DEVDISR(CCSBAR)) |= M8641_DEVDISR_TB;

     /*
      * Save the current timebase on CPU0 into static valuable.
      * It will be set on CPU1 later by sysCpuInit() and the timebase will be
      * synchronized between two cores.
      */

    vxTimeBaseGet (&tbHI, &tbLO);
#endif /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    sysCpu1Start ();

#if defined(DRV_TIMER_DEC_PPC) && defined(INCLUDE_TIMESTAMP)
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

    *(M8XXX_DEVDISR(CCSBAR)) &= ~M8641_DEVDISR_TB;

    /* Now PPC Dec timer re-started and safe to unblock interrupts. */

    intCpuUnlock (key);

#ifdef  INCLUDE_LOGGING
    if (status == ERROR)
	logMsg ("sysCpuEnable(): Timeout (%d secs) waiting to enable "
		"additional CPU: %x\n", VX_ENABLE_CPU_TIMEOUT, cpuNum,
		0, 0, 0, 0);
#endif  /* INCLUDE_LOGGING */
    }
#endif  /* DRV_TIMER_DEC_PPC && INCLUDE_TIMESTAMP */

    return status;
    }

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
    if (core1ampMode)
        return 1;

    return physCpuAvailableGet();
    }
#endif /* if defined(_WRS_CONFIG_SMP) */

#ifdef _WRS_VX_AMP
/*****************************************************************************
*
* sysAmpCpuStatusSet - Sets the status value of a specific CPU
*
* RETURNS: NONE
*
*/
void sysAmpCpuStatusSet(int status,int cpu)
    {
    /* only allow CPU1 to set it's own status */
    if( sysProcNumGet() == 1 )
	cpu = 1;

    switch(cpu)
	{
	case 1:
	    *((VINT32*)(AMP_CPU1_STATUS | CORE1T_OFFSET)) = status;
	    break;
	case 0:
	    *((VINT32*)(AMP_CPU0_STATUS | CORE1T_OFFSET)) = status;
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
int sysAmpCpuStatusGet(int cpu)
    {
    int status = ampCpuStateError;

    switch(cpu)
	{
	case 1:
	    status = *((VINT32*)(AMP_CPU1_STATUS | CORE1T_OFFSET));
	    break;
	case 0:
	    status = *((VINT32*)(AMP_CPU0_STATUS | CORE1T_OFFSET));
	    break;
	default:
	    break;
	}
    return(status);
    }

#endif /* _WRS_VX_AMP */

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

void sysCpuLawInit(void)
{
    /* map the 0xe0000000~0xffffffff to LBC in CPUA; to PCIE1 in CPUB/c/d */
	if(sysGetCpuID() == 0)
    {
        
        *M85XX_LAWBAR2(CCSBAR) = (LOCAL_BUS_BASE >> LAWBAR_ADRS_SHIFT);
	    *M85XX_LAWAR2(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_LBC | LOCAL_BUS_SIZE);
	    
	    *(unsigned int*)(CCSBAR+0x8da0) = 0xe0000000>>12;
	    *(unsigned int*)(CCSBAR+0x8da8) = 0xe0000000>>12;
	    *(unsigned int*)(CCSBAR+0x8db0) = 0x80f4401c;
	}
	else
	{
        
        *M85XX_LAWBAR2(CCSBAR) = (LOCAL_BUS_BASE >> LAWBAR_ADRS_SHIFT);
	    *M85XX_LAWAR2(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_PCIEX1 | LOCAL_BUS_SIZE);
	}	

#ifdef INCLUDE_RAPIDIO_BUS
    *M85XX_LAWBAR3(CCSBAR) = (RAPIDIO_ADRS >> LAWBAR_ADRS_SHIFT);
	*M85XX_LAWAR3(CCSBAR) = (LAWAR_ENABLE | LAWAR_TGTIF_RAPIDIO | LAWAR_SIZE_512MB);
#endif	
}
# if 0
extern STATUS sysRioSetOW(UINT8 unit,UINT8 targetId,UINT32 translationAddr,UINT32 baseAddr,UINT8 windowSize);
void  sysSetRioOW()
{
    if(((*(unsigned int*)(CCSBAR+0xe0004))&0x00060000)==0x00060000)
    {

        if(sysGetCpuID() == 0)
        {
        /* 0xc8000000-0xcc000000 to access cpuB  tgtID is 0x1*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x01 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xcc000000-0xd0000000 to access cpuC  tgtID is 0x3*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x03 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xcc000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd0000000-0xd4000000 to access cpuD tgtID is 0x2 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd0000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd4000000-0xd8000000 to access board2's cpuA tgtID is 0x5 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x05 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xdc000000 to access board2's cpuB tgtID is 0x4 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x04 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xe0000000 to access board2's cpuC tgtID is 0x7 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		
	
        }
	    if(sysGetCpuID() == 1)
        {
        /* 0xc4000000-0xc8000000 to access cpuA  tgtID is 0x0*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x00 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xcc000000-0xd0000000 to access cpuC  tgtID is 0x3*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x03 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xcc000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd0000000-0xd4000000 to access cpuD tgtID is 0x2 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd0000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd4000000-0xd8000000 to access board2's cpuA tgtID is 0x5 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x05 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xdc000000 to access board2's cpuB tgtID is 0x4 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x04 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xe0000000 to access board2's cpuC tgtID is 0x7 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		
	
        }
	    if(sysGetCpuID() == 2)
        {
        /* 0xc8000000-0xcc000000 to access cpuB  tgtID is 0x1*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x01 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xc4000000-0xc8000000 to access cpuA  tgtID is 0x0*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x00 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xc4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd0000000-0xd4000000 to access cpuD tgtID is 0x2 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd0000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd4000000-0xd8000000 to access board2's cpuA tgtID is 0x5 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x05 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xdc000000 to access board2's cpuB tgtID is 0x4 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x04 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xe0000000 to access board2's cpuC tgtID is 0x7 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		
	
        }
    	if(sysGetCpuID() == 3)
        {
        /* 0xc8000000-0xcc000000 to access cpuB  tgtID is 0x1*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x01 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xcc000000-0xd0000000 to access cpuC  tgtID is 0x3*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x03 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xcc000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xc4000000-0xc8000000 to access cpuA tgtID is 0x0 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x00 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xc4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd4000000-0xd8000000 to access board2's cpuA tgtID is 0x5 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x05 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xdc000000 to access board2's cpuB tgtID is 0x4 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x04 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xe0000000 to access board2's cpuC tgtID is 0x7 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		
	
        }
    }
	else
	{
	    if(sysGetCpuID() == 0)
        {
        /* 0xc8000000-0xcc000000 to access cpuB  tgtID is 0x1*/
	    	


        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x01 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xcc000000-0xd0000000 to access cpuC  tgtID is 0x3*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x03 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xcc000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd0000000-0xd4000000 to access cpuD tgtID is 0x2 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd0000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xc4000000-0xc8000000 to access boatd1's cpuA tgtID is 0x0 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x00 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xc4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xdc000000 to access board2's cpuB tgtID is 0x4 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x04 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xe0000000 to access board2's cpuC tgtID is 0x7 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
	
	
        }
	    if(sysGetCpuID() == 1)
        {
        /* 0xc8000000-0xcc000000 to access cpuB  tgtID is 0x1*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x01 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xcc000000-0xd0000000 to access cpuC  tgtID is 0x3*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x03 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xcc000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd0000000-0xd4000000 to access cpuD tgtID is 0x2 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd0000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd4000000-0xd8000000 to access board2's cpuA tgtID is 0x5 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x05 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xc4000000-0xc8000000 to access board1's cpuA tgtID is 0x0 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x00 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xc4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xe0000000 to access board2's cpuC tgtID is 0x7 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		
	
        }
	    if(sysGetCpuID() == 2)
        {
        /* 0xc8000000-0xcc000000 to access cpuB  tgtID is 0x1*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x01 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xcc000000-0xd0000000 to access cpuC  tgtID is 0x3*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x03 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xcc000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd0000000-0xd4000000 to access cpuD tgtID is 0x2 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd0000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd4000000-0xd8000000 to access board2's cpuA tgtID is 0x5 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x05 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xdc000000 to access board2's cpuB tgtID is 0x4 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x04 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xc4000000-0xc8000000 to access board1's cpuA tgtID is 0x0 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x00 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xc4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		
	
        }
    	if(sysGetCpuID() == 3)
        {
        /* 0xc8000000-0xcc000000 to access cpuB  tgtID is 0x1*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc40) = (0x01 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc48) = (0xc8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc50) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xcc000000-0xd0000000 to access cpuC  tgtID is 0x3*/
        *(unsigned int*)(CCSBAR+0xd0000+0xc60) = (0x03 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc68) = (0xcc000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc70) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd0000000-0xd4000000 to access cpuD tgtID is 0x2 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd0000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
        /* 0xd4000000-0xd8000000 to access board2's cpuA tgtID is 0x5 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x05 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd4000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xdc000000 to access board2's cpuB tgtID is 0x4 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x04 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		/* 0xd8000000-0xe0000000 to access board2's cpuC tgtID is 0x7 */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc80) = (0x02 << 22) | (0x10000000>>12);  /* ROWTAR */
	    *(unsigned int*)(CCSBAR+0xd0000+0xc88) = (0xd8000000 >> 12);
	    *(unsigned int*)(CCSBAR+0xd0000+0xc90) = 0x80044019;   /* NREAD NWRITE 64M */
		
	
        }
	
	}
	
}
void sysSetRioIW() 
{
    *(unsigned int*)(CCSBAR+0xd0000+0xd60) = (0x30000000>>12);   /* RIWTAR */
	*(unsigned int*)(CCSBAR+0xd0000+0xd68) = (0x10000000>>12);            /* RIWBAR */
	*(unsigned int*)(CCSBAR+0xd0000+0xd70) = 0x80f5501a; /* en,local mem,  snoop read,  snoop write 128M*/
}
#endif




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

	*(char*)(CCSBAR+0x4500+DLL)= (sysClkFreqGet()/(16*CONSOLE_BAUD_RATE))&0xff;
	*(char*)(CCSBAR+0x4500+DLM)= ((sysClkFreqGet()/(16*CONSOLE_BAUD_RATE))&0xff00)>>8;
		/* sjy altered */ 
	//*(char*)(CCSBAR+0x4500+DLL)= (sysClkFreqGet()/(16*115200))&0xff;
	//*(char*)(CCSBAR+0x4500+DLM)= ((sysClkFreqGet()/(16*115200))&0xff00)>>8;
		/* sjy altered */ 
	*(char*)(CCSBAR+0x4500+LCR)=0x03;
	return 0;
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


UINT32 sysUartClkFreqGet(void)
{
	return 250000000;
}

UINT32 sysPvrGet()
{
	return 0x80040010;
}




#if 0
void endValSet(int a)
{
	BOOT_PARAMS bp;
	UINT8 temp,i,len;
	char *pTemp=NULL;
	char *pTemp1=NULL;
//    if (*(BOOT_LINE_ADRS) == EOS)
//    {
        sysNvRamGet(BOOT_LINE_ADRS,0xff,0);
        (void) bootStringToStruct (BOOT_LINE_ADRS, &bp);
        if(bp.ead!=NULL)
        {
         char *pchar = bp.ead;
         while((*pchar!=EOS)&&((pchar-bp.ead)<50)&&(*pchar!=':'))
         {
        	if(*pchar == '.') pTemp = (pchar+1);
        	pchar++;
         }
         temp = (atoi(pTemp) + a);
         sprintf(pTemp1,"%d\0",temp);
         len = strlen(pTemp1);
         pchar -= len;
         for(i=0;i<len;i++)
         {
        	 *pchar++ = *pTemp1++;
         }
         printf("22...pchar =%s,bp.ead=%s \n",pchar,bp.ead); 
         (void) bootStructToString (BOOT_LINE_ADRS, &bp); 
         printf(" bootStruct:%s\n",BOOT_LINE_ADRS);        
         
        }
//    }
}
#endif

