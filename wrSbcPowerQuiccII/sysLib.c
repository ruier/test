/* sysLib.c - system-dependent library */

/*
 * Copyright (c) 2003-2008, 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02w,12nov12,ylu  correct sysModckHGet() return value. (WIND00057961)
02v,21jan11,agf  clean-up compiler warnings
02u,30apr10,dhy  delete _MMU_UBAT_VP attribute(WIND00162540)
02t,17oct08,jmt  Setup default value for RAM_HIGH_ADRS/RAM_LOW_ADRS
02s,18jun08,h_k  removed pAccess.
02r,08aug07,agf  remove duplicate variable definitions
02q,11jun07,vik  removed cmdLine.c inclusion
02p,19jan07,wap  Correct project build errors
02o,06sep06,jmt  Remove INCLUDE_USER_APPL code
02p,21aug06,wap  Correct name of mdio header file
02o,21aug06,wap  Add VxBus support
02n,11may06,jmt  Modify sysModel to use SYS_MODEL define
02m,13dec05,pch  SPR 113596: networking dependencies
02l,26aug05,mil  Added include of m8260Cp.h and use of INCLUDE_MMU_GLOBAL_MAP.
02k,23jul05,mil  Added conditional for memory regions and cacheLock.
                 Moved up SMC port init.
02j,08jul05,mil  Added layer support.  
02i,17dec04,ckj  WLAN support for Atheros AR521X
02h,23sep04,mdo  Documentation fixes for apigen
02g,23aug04,j_b  fix network dependencies (SPR 99747)
02g,18aug04,md   PM_RESERVED_MEM is dependent on INCLUDE_EDR_PM
02f,04jun04,j_b  conditionally compile LED use
02e,03jun04,j_b  SPR 95624: fix pciConfigShow.c inclusion
02d,29apr04,j_b  fix compiler warnings
02c,26apr04,j_b  fix build failure with INCLUDE_FEI_END defined (SPR 96050)
02b,29mar04,m_h  Fix SPR 91917 link problem from project
02a,19feb04,j_b  merge power management
01z,22jan04,j_b  clarify PCI and i8259A dependencies (SPR#93321)
01y,19jan04,kab  Temp workaround SPR 91917
01x,09dec03,agf  add PM_RESERVED_MEM
01w,02dec03,agf  fix compiler warnings
01v,15oct03,j_b  add sysCpu
01u,08aug03,scm  remove references to GEI for release...
01t,29jul03,scm  modify sysToMonitor
01s,23jul03,j_b  disable all FCCs in sysToMonitor
01r,21jul03,j_b  remove motfcc0 END device init for ATM build
01q,23jun03,j_b  add RTC support; remove vware support
01p,20jun03,j_b  fix main PLL calculation for HiP7 CPUs; cleanup
01o,03jun03,scm  add better delay routine based off TBL reg...
01n,29may03,j_b  remove INCLUDE_FEI_END condition for Delay routines
01m,23may03,scm  add PCI wrapper checks for proc support...
01l,21may03,jb3  debugging.
01k,20may03,jb3  change myAppInit.
01j,09may03,scm  update pci...
01i,05may03,j_b  read PCI mode from status reg
01h,30apr03,j_b  modified clock configurations for multiple input frequencies
                 & PCI mode
01g,28apr03,scm  update PCI...
01f,25apr03,j_b  get MODCK_H from status register when PCI enabled
01e,25apr03,scm  update for pci...
01d,24apr03,jb3  Add myAppInit for multi ethernet support.
01c,24apr03,j_b  remove obsolete silicon rev usage; fix sysModck13Get
01c,22apr03,scm  add i8259A external interrupt support...
01b,15apr03,j_b  add hardware init progress tracking on Hex LED
01a,01apr03,j_b  adapted from wrSbc8260Atm (ver 01c)
*/

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
#include <muxLib.h>
#include <ipProto.h>
#include <drv/mem/m82xxDpramLib.h>
#include <arch/ppc/vxPpcLib.h>
#include <arch/ppc/mmu603Lib.h>
#include <private/vmLibP.h>
#include <drv/sio/m8260Cp.h>
#include <drv/mem/m8260Siu.h>
#include <drv/timer/m8260Clock.h>
#include <drv/intrCtl/m8260IntrCtl.h>
#include <drv/parallel/m8260IOPort.h>
#include "smc8260Sio.h"

#if (NV_RAM_ADRS == EEPROM_ADRS)
#  include "eeprom.h"
#endif /* NV_RAM_SIZE */

#include "wrSbcPowerQuiccII.h"

#ifdef INCLUDE_VXBUS
#include "hwif/vxbus/vxBus.h"
IMPORT void    hardWareInterFaceInit();
#include "hwconf.c"
#include <hwif/vxbus/vxbPlbLib.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>

#ifdef	VXB_LEGACY_ACCESS
LOCAL void m8260PlbAccessFunctionOverride (struct vxbAccessList *);
#endif	/* VXB_LEGACY_ACCESS */
LOCAL STATUS m8260PlbDevControl (VXB_DEVICE_ID, pVXB_DEVCTL_HDR);

#ifdef INCLUDE_FCC_VXB_END
#include <../src/hwif/h/end/fccVxbEnd.h>
STATUS sysFccEnetEnable (VXB_DEVICE_ID, UINT8);
#endif

#ifdef INCLUDE_SCC_VXB_END
#include <../src/hwif/h/end/sccVxbEnd.h>
STATUS sysSccEnetEnable (VXB_DEVICE_ID, UINT8);
#endif

#ifdef INCLUDE_MDIO
#include <../src/hwif/h/mii/mdio.h>
STATUS sysMdioEnable (VXB_DEVICE_ID);
#endif

IMPORT device_method_t * pSysPlbMethods;

LOCAL struct vxbDeviceMethod m8265PlbMethods[] =
    {
#ifdef INCLUDE_FCC_VXB_END
    DEVMETHOD(fccEnetEnable, sysFccEnetEnable),
#endif
#ifdef INCLUDE_SCC_VXB_END
    DEVMETHOD(sccEnetEnable, sysSccEnetEnable),
#endif
#ifdef INCLUDE_MDIO
    DEVMETHOD(mdioEnable, sysMdioEnable),
#endif
    { 0, 0 }
    };

#endif /* INCLUDE_VXBUS */

#if defined(INCLUDE_MOTFCCEND) && defined(INCLUDE_END)
  IMPORT END_TBL_ENTRY endDevTbl[];
#endif /* INCLUDE_MOTFCCEND && INCLUDE_END */


#ifdef INCLUDE_i8259A
#  ifdef INCLUDE_PCI
#    include <drv/pci/pciConfigLib.h>
#    include <drv/pci/pciIntLib.h>
#  endif /* INCLUDE_PCI */
#endif /* INCLUDE_i8259A */

#ifdef INCLUDE_PCMCIA
#  include <pcmcia/pccardLib.c>
#  include <pcmcia/pccardShow.c>
#endif /* INCLUDE_PCMCIA */

#ifdef INCLUDE_i8259A
  IMPORT void intEnt (void);
  IMPORT UINT32 sysTimeBaseLGet(void);
#endif /* INCLUDE_i8259A */

#ifdef RAM_LOW_ADRS
#define SYS_RAM_LOW_ADRS RAM_LOW_ADRS
#else
#define SYS_RAM_LOW_ADRS 0x00100000
#endif /* RAM_LOW_ADRS */

#ifdef RAM_HIGH_ADRS
#define SYS_RAM_HIGH_ADRS RAM_HIGH_ADRS
#else
#define SYS_RAM_HIGH_ADRS 0x01C00000
#endif /* RAM_HIGH_ADRS */

/* Global data */

#if defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_GLOBAL_MAP)
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
 * with vmLib support.  BAT registers may be used for i/o mapping and
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

UINT32 sysBatDesc [2 * (_MMU_NUM_IBAT + _MMU_NUM_DBAT)] =
    {
    /* use IBAT0 to map flash execution space */
    ((ROM_BASE_ADRS & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M | _MMU_UBAT_VS),
    ((ROM_BASE_ADRS & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW | _MMU_LBAT_CACHE_INHIBIT ),

    /* The other IBATS are being disabled here.... */

    0, 0, /* I BAT 1 */

    0, 0, /* I BAT 2 */

    0, 0, /* I BAT 3 */

    /* use DBAT0 to map the highest 256M (IMMR,flash, etc) into data space */
    /* NOTE! this space cannot be cached (for data accesses) and should be guarded */
    ((INTERNAL_MEM_MAP_ADDR & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_256M | _MMU_UBAT_VS),
    ((INTERNAL_MEM_MAP_ADDR & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW | _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

#ifdef INCLUDE_i8259A
#  ifdef INCLUDE_PCI
    /* use DBAT1 to map the PCI memory window to its actual physical address and size */
    /* NOTE! this space cannot be cached and should be guarded */
    ((PCI_MEM_BASE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_64M | _MMU_UBAT_VS),
    ((PCI_MEM_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW  | _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

    /* use DBAT2 to map local bus SDRAM into data space -- if user wants it */
    /* NOTE! the local bus SDRAM cannot be cached -- period -- but does not need guarded */
    ((PCI_IO_BASE & _MMU_UBAT_BEPI_MASK) | _MMU_UBAT_BL_1M | _MMU_UBAT_VS),
    ((PCI_IO_BASE & _MMU_LBAT_BRPN_MASK) | _MMU_LBAT_PP_RW | _MMU_LBAT_CACHE_INHIBIT | _MMU_LBAT_GUARDED),

        0, 0, /* D BAT 3 */
#  else
    /* The other IBATS are being disabled here.... */

        0, 0, /* D BAT 1 */

        0, 0, /* D BAT 2 */

        0, 0, /* D BAT 3 */

#  endif /* INCLUDE_PCI */
#endif  /* INCLUDE_i8259A */
    };

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * PTEs are held in a Page Table.  Page Table sizes are integer powers of two
 * based on the amount of memory to be mapped and are a minimum of 64KB.
 * The MINIMUM recommended Page Table sizes for 32-bit PowerPCs are:
 *
 *	Total mapped memory		Page Table size
 *	-------------------		---------------
 *	       8 Meg			     64 K
 *	      16 Meg			    128 K
 *	      32 Meg			    256 K
 *	      64 Meg			    512 K
 *	     128 Meg			      1 Meg
 *		.				.
 *		.				.
 *		.				.
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
{
  /* The following maps the Vector Table and Interrupt Stack */
  {
    (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
    (PHYS_ADDR) LOCAL_MEM_LOCAL_ADRS,
    SYS_RAM_LOW_ADRS - LOCAL_MEM_LOCAL_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE      | VM_STATE_MEM_COHERENCY
  },

  /* The following maps the Local SDRAM */
  {
    (VIRT_ADDR) SYS_RAM_LOW_ADRS,
    (PHYS_ADDR) SYS_RAM_LOW_ADRS,
    LOCAL_MEM_SIZE -  SYS_RAM_LOW_ADRS,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_MEM_COHERENCY,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE      | VM_STATE_MEM_COHERENCY
  },

#if (NV_RAM_SIZE >= 0)
  /* The following maps the NV RAM memory */
  {
    (VIRT_ADDR) NV_RAM_ADRS,
    (PHYS_ADDR) NV_RAM_ADRS,
    NV_RAM_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
  },
#endif  /* (NV_RAM_ADRS >= 0) */

  /* The following maps the User Switches */
  {
    (VIRT_ADDR) BSCR_USER_SWITCH,
    (PHYS_ADDR) BSCR_USER_SWITCH,
    BSCR_REG_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE_NOT  | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
  },

  /* The following maps the Board Status */
  {
    (VIRT_ADDR) BSCR_STATUS,
    (PHYS_ADDR) BSCR_STATUS,
    BSCR_REG_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
  },

#ifdef INCLUDE_i8259A
  /* The following maps the Interrupt Controller */
  {
    (VIRT_ADDR) PIC1_BASE_ADR,
    (PHYS_ADDR) PIC1_BASE_ADR,
    BSCR_REG_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
  },
#endif  /* INCLUDE_i8259A */

  /* The following maps the User LEDs */
  {
    (VIRT_ADDR) BSCR_LED_REGISTER,
    (PHYS_ADDR) BSCR_LED_REGISTER,
    BSCR_REG_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
  },

  /* The following maps the FLASH on CS0 to its actual physical address and size */
  {
    (VIRT_ADDR) CS0_FLASH_ADRS,
    (PHYS_ADDR) CS0_FLASH_ADRS,
    CS0_FLASH_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | VM_STATE_GUARDED
  },

  /* The following maps the flash on CS6 to its actual physical address and size */
  {
    (VIRT_ADDR) CS6_FLASH_ADRS,
    (PHYS_ADDR) CS6_FLASH_ADRS,
    CS6_FLASH_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT | VM_STATE_GUARDED
  },

  /* The following maps the Real-Time Clock & SRAM */
  {
    (VIRT_ADDR) RTC_ADRS,
    (PHYS_ADDR) RTC_ADRS,
    RTC_SIZE,
    VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE | VM_STATE_MASK_GUARDED,
    VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT  | VM_STATE_GUARDED
  }
};

int sysPhysMemDescNumEnt = NELEMENTS( sysPhysMemDesc ) ;

#endif  /* INCLUDE_MMU_BASIC | INCLUDE_MMU_GLOBAL_MAP */


int   sysBus       = BUS            ; /* system bus type (NONE)            */
int   sysCpu       = CPU            ; /* system CPU type (PPC603)          */
char *sysBootLine  = BOOT_LINE_ADRS ; /* address of boot line              */
char *sysExcMsg    = EXC_MSG_ADRS   ; /* catastrophic message area         */
                                      /*                                   */
int   sysProcNum                    ; /* processor number of this CPU      */

/*
 * 60x Bus-to-Core multiplier table, each element consisting of a
 * numerator and denominator (or multiplier and divider).
 * This table is indexed by the Core PLL configuration bits (CORECNF) of
 * the PowerQUICC II System Clock Mode Register (SCMR).
 *
 * NOTE: From '60x Bus-to-Core Frequency', page 15 in Rev. 6 of
 *       MPC8260 PowerQUICC II User's Manual Errata
 *
 *       Special CORECNF values:
 *           0x3 and 0x13 are PLL Bypassed, 1x bus-to-core multiplier
 *           0xf and 0x1f are PLL Off, 0x bus-to-core multiplier (no core clock)
 *           0x18 is reserved, core clock = -1 (ERROR)
 */
struct corecnf_tbl
{
    INT32 numerator;
    UINT32 denominator;
} bus_to_core[] =
    {
      { 3,2}, { 1,1}, { 1,1}, { 1,1}, { 2,1}, { 2,1}, { 5,2}, { 9,2},
      { 3,1}, {11,5}, { 4,1}, { 5,1}, { 3,2}, { 6,1}, { 7,2}, { 0,1},
      { 3,1}, { 5,2}, {13,2}, { 1,1}, { 7,1}, { 2,1}, {15,2}, { 9,2},
      {-1,1}, {11,2}, { 4,1}, { 5,1}, { 8,1}, { 6,1}, { 7,2}, { 0,1}
    };

#ifdef INCLUDE_i8259A
BOOL    sysBp     = TRUE;    /* TRUE for BP, FALSE for AP */

/* forward declarations */
VOID sysIntLock        (void);
VOID sysIntUnlock      (void);
VOID sysIntEoi         (void);
int  sysIntLevel       (void);

#  ifdef INCLUDE_PCI
  STATUS sysPciSpecialCycle (int busNo, UINT32 message);
  STATUS sysPciConfigRead   (int busNo, int deviceNo, int funcNo,
			         int offset, int width, void * pData);
  STATUS sysPciConfigWrite  (int busNo, int deviceNo, int funcNo,
			         int offset, int width, ULONG data);
#  endif  /* INCLUDE_PCI */
#endif  /* INCLUDE_i8259A */

/* Forward function references   */
UINT32  vxImmrGet (void);
void 	vxImmrSet (UINT32 value);
UINT32  sysChipRev(void);
void    sysCpmReset(void);
UINT8   sysUserSwitchGet(void);
UINT32  sysCoreFreqGet(void);
UINT32  sysCpmFreqGet(void);
UINT32  sysInputFreqGet(void);
UINT16  sysModck13Get (void);
UINT16  sysModckHGet (void);
void    sysPQIIClkRoutine (int arg);
BOOL    sysCpuHasPci(void);
BOOL    sysPciEnabled(void);

#ifdef INCLUDE_i8259A
  LOCAL void sysIntInitPIC (void);
#endif

/* locals */
LOCAL UINT32 immrAddress = (UINT32) INTERNAL_MEM_MAP_ADDR;
LOCAL UINT32 sysMainPllFreqGet(void);

/* externals */
UINT16 sysStatusRegGet(void);
UINT32 sysCPUIdGet(void);

/* Additional Components */
#if (NV_RAM_SIZE == NONE)
#  include <mem/nullNvRam.c>
#else
#  include "eeprom.c"
#  include <mem/byteNvRam.c>
#endif

#include <vme/nullVme.c>
#include <mem/m82xxDpramLib.c>
#if defined(INCLUDE_TTY_DEV) || defined(INCLUDE_SIO_POLL)
#include "sysSerial.c"
#endif  /* INCLUDE_TTY_DEV || INCLUDE_SIO_POLL */
#include <intrCtl/m8260IntrCtl.c>

#ifdef INCLUDE_SYSLED
#  include "sysLed.c"
#endif /* INCLUDE_SYSLED */

    /*
     * Defining POWER_MGT_INSTRUMENT configures the timer routines
     * to indicate the state of the system clock.
     * This toggles the decimal point LED with each interrupt and
     * displays the least significant digit of current time on the
     * hex display.
     */

#undef POWER_MGT_INSTRUMENT
#if (defined POWER_MGT_INSTRUMENT && defined INCLUDE_SYSLED)
/* test code used to instrument the board */
#include <time.h>
LOCAL int ledTemp = 0;
LOCAL int ledTemp1 = 0;
LOCAL int ledTemp2 = 0;
#define POWER_MGT_INSTRUMENT_CLK_INT \
    ledTemp1 = time (0); \
    if ( ledTemp1 != ledTemp2) \
        { \
        ledTemp2 = ledTemp1; \
        sysLedHex ( (UINT8) ledTemp1); \
        } \
    ((ledTemp++) & 1) ? sysLedSet (LED_POINT, LED_POINT) : sysLedSet (LED_POINT, 0);


#define POWER_MGT_INSTRUMENT_OS_DIS \
    ledTemp1 = time (0); \
    if ( ledTemp1 != ledTemp2) \
        { \
        ledTemp2 = ledTemp1; \
        sysLedHex ( (UINT8) ledTemp1); \
        } \
    ((ledTemp++) & 1) ? sysLedSet (LED_POINT, LED_POINT) : sysLedSet (LED_POINT, 0);
#endif /* POWER_MGT_INSTRUMENT && INCLUDE_SYSLED */

#include <timer/m8260Timer.c>

#include "sysIOPort.c"

#ifdef INCLUDE_CACHE_SUPPORT
#  include "sysCacheLockLib.c"
#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef  INCLUDE_END
#  include "sysNet.h"
#  include "sysNet.c"

#  ifdef INCLUDE_MOTFCCEND
#    include "sysMotFccEnd.c"
#  endif /* INCLUDE_MOTFCCEND */

#  ifdef  INCLUDE_MOTSCCEND
#    include "sysMotSccEnd.c"
#  endif /* INCLUDE_MOTSCCEND */
#endif /* INCLUDE_END */

#ifdef INCLUDE_i8259A
/* interrupt controller driver */
#  include "i8259Intr.c"

#  ifdef INCLUDE_PCI
#    include <pci/pciIntLib.c>           /* PCI int support */
#    include <pci/pciConfigLib.c>        /* PCI configuration library */

/* use show routine */
#      if (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD))
#        include <pci/pciConfigShow.c>     /* PCI configuration show routines */
#      endif /* (defined(INCLUDE_PCI_CFGSHOW) && !defined(PRJ_BUILD)) */

/* use PCI auto config */

#    include <pci/pciAutoConfigLib.c>    /* automatic PCI configuration */
#    include "sysBusPci.c"               /* pciAutoConfig BSP support file */
#    include "mot82xxPci.c"

  ULONG sysPciConfAddr = PCI_ADDRESS_REGISTER;  /* PCI Configuration Address */
  ULONG sysPciConfData = PCI_DATA_REGISTER;     /* PCI Configuration Data */

#    if defined(INCLUDE_FEI_END) && defined(INCLUDE_END)
#      include "sysFei82557End.c"
#    endif /* INCLUDE_FEI_END && INCLUDE_END */

#    ifdef INCLUDE_PCCARD
#      include "sysPccard.c"
#    endif /* INCLUDE_PCCARD */

#  endif /* INCLUDE_PCI */

#endif  /* INCLUDE_i8259A */

#ifdef M48T59_RTC_SRAM
#  include "stm48t59Rtc.c"
#endif /* M48T59_RTC_SRAM */

#ifdef INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT
#    include "sysCpuPwr.c"
#endif /* INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT */

/* Atheros AR521X WLAN Support */

#ifdef INCLUDE_AR521X_END
    #include <wlan/bsp/sysDot11End.c>
#endif /* INCLUDE_AR521X_END */

/***********************************************************************
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
    return( SYS_MODEL ) ;
    }

/***********************************************************************
*
* sysBspRev - return the BSP version and revision number
*
* This routine returns a pointer to a BSP version and revision number. For
* example, 1.2/0. BSP_REV is concatenated to BSP_VERSION and returned.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO
*/

char * sysBspRev (void)
    {
    return( BSP_VERSION BSP_REV );
    }

/***********************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the hardware.
* Normally this routine is called from usrInit().
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysHwInit (void)
    {
    int             ix;
    ULONG         * pDPRAM1;
#if defined(INCLUDE_MOTFCCEND) && defined(INCLUDE_END)
    int             count;
    END_TBL_ENTRY * pDevTbl;
#endif /* INCLUDE_MOTFCCEND && INCLUDE_END */

    /* Get the physical location of the IMMR register */
    int immrVal = vxImmrGet ();

    /* Issue a Software Reset Command to the CPM core */
    sysCpmReset ();

#ifdef INCLUDE_SYSLED
    /* Clear the Hex LED */
    sysLedInit ();
#endif /* INCLUDE_SYSLED */

    /* Initialize DPRAM1 to zero.  */
    pDPRAM1 = (unsigned long *)immrVal;

    for (ix = 0; ix < 0x1000; ix++)
        *pDPRAM1++ = 0;

    /*  reset port A, B, C, and D */
    *M8260_IOP_PAPAR( immrVal ) = 0x00000000;
    *M8260_IOP_PADIR( immrVal ) = 0x00000000;
    *M8260_IOP_PAODR( immrVal ) = 0x00000000;

    *M8260_IOP_PBPAR( immrVal ) = 0x00000000;
    *M8260_IOP_PBDIR( immrVal ) = 0x00000000;
    *M8260_IOP_PBODR( immrVal ) = 0x00000000;

    *M8260_IOP_PCPAR( immrVal ) = 0x00000000;
    *M8260_IOP_PCDIR( immrVal ) = 0x00000000;
    *M8260_IOP_PCSO( immrVal )  = 0x00000000;

    *M8260_IOP_PDPAR( immrVal ) = 0x00000000;
    *M8260_IOP_PDDIR( immrVal ) = 0x00000000;
    *M8260_IOP_PDSO( immrVal )  = 0x00000000;

    /*
     * Initialize SICR.
     */
    *M8260_SICR( immrVal ) = 0x00000000;

    /*
     * Initialize interrupts - default interrupt level.
     */
    m8260IntrInit ();

#ifdef INCLUDE_i8259A
    if (sysCpuHasPci())
        {
        /* initialize the 8259 interrupt controller */
        sysIntInitPIC();
        }
#endif  /* INCLUDE_i8259A */

#ifdef USE_LEDS_FOR_BOOT_DEBUG
    /* start hardware init tracking backwards from 'F' to 'A' */
    sysLedHex(0xF);
#endif /* USE_LEDS_FOR_BOOT_DEBUG */

    /*
     * Init the DPRAM lib
     */

    m82xxDpramLibInit ();

#ifdef USE_LEDS_FOR_BOOT_DEBUG
    sysLedHex(0xE);
#endif /* USE_LEDS_FOR_BOOT_DEBUG */

#ifdef INCLUDE_i8259A
#ifdef INCLUDE_PCI
    if (sysCpuHasPci())
        {

        /* config PCI */
        if (pciConfigLibInit (PCI_MECHANISM_0,(ULONG) sysPciConfigRead,
                              (ULONG) sysPciConfigWrite,(ULONG) sysPciSpecialCycle) != OK)
            {
            logMsg("sysHwInit: pciConfigLibInit failed\n", 0, 0, 0, 0, 0, 0);
            sysToMonitor (BOOT_NO_AUTOBOOT);  /* BAIL */
            }

#if (defined PCI_DEBUG && defined INCLUDE_SYSLED)
        sysLedHex(0x1);
#endif /* PCI_DEBUG */

        /*  Initialize PCI interrupt library. */
        if ((pciIntLibInit ()) != OK)
            {
            logMsg("sysHwInit: pciIntLibInit failed\n", 0, 0, 0, 0, 0, 0);
            sysToMonitor (BOOT_NO_AUTOBOOT);
            }

#if (defined PCI_DEBUG && defined INCLUDE_SYSLED)
    sysLedHex(0x2);
#endif /* PCI_DEBUG */

    mot82xxBridgeInit();

#if (defined PCI_DEBUG && defined INCLUDE_SYSLED)
    sysLedHex(0x3);
#endif /* PCI_DEBUG */

#ifdef INCLUDE_PCI_AUTOCONF
      sysPciAutoConfig();
#endif /* INCLUDE_PCI_AUTOCONF */

       }
#endif /* INCLUDE_PCI */
#endif  /* INCLUDE_i8259A */

    /*
     * Setup port D for SMC1.
     */

    *M8260_IOP_PDPAR( immrVal ) |= (PD8 | PD9);
    *M8260_IOP_PDDIR( immrVal ) |=  PD9;

    /*
     * Setup port A for SMC2.
     */

    *M8260_IOP_PAPAR( immrVal ) |= (PA8 | PA9);
    *M8260_IOP_PADIR( immrVal ) |=  PA9;

    /*
     * Reset serial channels.
     */

#if defined(INCLUDE_TTY_DEV) || defined(INCLUDE_SIO_POLL)
    sysSerialHwInit ();
#endif  /* INCLUDE_TTY_DEV || INCLUDE_SIO_POLL */

#ifdef USE_LEDS_FOR_BOOT_DEBUG
    sysLedHex(0xD);
#endif /* USE_LEDS_FOR_BOOT_DEBUG */

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy (sysBootLine,DEFAULT_BOOT_LINE,strlen(DEFAULT_BOOT_LINE)+1);
#endif /* FORCE_DEFAULT_BOOT_LINE */

    /* disable Ethernet channels */
#if defined(INCLUDE_MOTSCCEND) && defined(INCLUDE_END)
    sysSccEnetDisable   ( 0 ) ;
    sysSccEnetIntDisable( 0 ) ;
#endif /* INCLUDE_MOTSCCEND && INCLUDE_END */

#if defined(INCLUDE_MOTFCCEND) && defined(INCLUDE_END)
    sysFccEnetDisable (immrVal, 1);
    sysFccEnetDisable (immrVal, 2);
    sysFccEnetDisable (immrVal, 3);

    /* motfcc0 is unavailable with ATM build, so don't initialize it */
    if( !(sysStatusRegGet() & STATUS_BUILD_MASK) )
        {
        for (count = 0, pDevTbl = endDevTbl ;
	     pDevTbl->endLoadFunc != END_TBL_END ;
	     pDevTbl++, count++)
            {
            /* prevent motfcc0 driver from being loaded */
            if (pDevTbl->unit == 0 && pDevTbl->endLoadFunc == sysMotFccEndLoad)
                {
                pDevTbl->processed = TRUE;
                break;
                }
            }
        }
#endif /* INCLUDE_MOTFCCEND && INCLUDE_END */

#ifdef M48T59_RTC_SRAM
    /* initialize the RTC */
    m48t59Reset();

    /* enable machine check input, IRQ0, for RTC alarm/watchdog interrupts */
    vxHid0Set(vxHid0Get() | _PPC_HID0_EMCP);
#endif /* M48T59_RTC_SRAM */


#ifdef USE_LEDS_FOR_BOOT_DEBUG
    sysLedHex(0xC);
#endif /* USE_LEDS_FOR_BOOT_DEBUG */

#ifdef INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT
    sysCpuPwrInit ();
#endif /* INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT */

#ifdef INCLUDE_VXBUS
# ifdef	VXB_LEGACY_ACCESS
    pPlbAccessFunctionOverride = m8260PlbAccessFunctionOverride;
# else	/* VXB_LEGACY_ACCESS */
    pPlbDevControl = (VOIDFUNCPTR) m8260PlbDevControl;
# endif	/* VXB_LEGACY_ACCESS */

    pSysPlbMethods = m8265PlbMethods;
    hardWareInterFaceInit();
#endif

    }

/***********************************************************************
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
* ERRNO
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop
    (
    void
    )
    {
    static char * physTop = NULL;

    if (physTop == NULL)
        {
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
        }

    return( physTop ) ;
    }

/***********************************************************************
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

char *sysMemTop
    (
    void
    )
    {
    static char * memTop = NULL;

    if ( memTop == NULL )
        {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;

#ifdef INCLUDE_EDR_PM
        /* account for ED&R persistent memory */

        memTop = memTop - PM_RESERVED_MEM;
#endif
        }

    return memTop ;
    }

/***********************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  Normally, it is called
* only by reboot()--which services ^X--and by bus errors at interrupt level.
* However, in some circumstances, the user may wish to introduce a
* <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*
* ERRNO
*/

STATUS sysToMonitor
    (
       int startType    /* parameter passed to ROM to tell it how to boot   */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);       /* Warm reboot      */

#if defined(INCLUDE_FEI_END) && defined(INCLUDE_END)
    END_OBJ * pEnd;
    UINT32      i;
#endif  /* INCLUDE_FEI_END && INCLUDE_END */

#if defined(INCLUDE_MOTFCCEND) && defined(INCLUDE_END)
    /*                                                                      */
    /* Get the physical location of the IMMR register                       */
    /*                                                                      */
    int immrVal = vxImmrGet ();
#endif /* INCLUDE_MOTFCCEND && INCLUDE_END */

#ifdef INCLUDE_i8259A
    sysIntLock ();
#endif
    intLock ();

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable (INSTRUCTION_CACHE);
    cacheDisable (DATA_CACHE);
#endif

#if defined(INCLUDE_AUX_CLK)
    sysAuxClkDisable();
#endif /* INCLUDE_AUX_CLK */

#if defined(INCLUDE_FEI_END) && defined(INCLUDE_END)
    for(i = 0; i < SYS_FEI_UNITS; i++)
        {
        if ( (pEnd = endFindByName ("fei", i)) != NULL)
            pEnd->pFuncTable->stop(pEnd->devObject.pDevice);
        }
#endif /* INCLUDE_FEI_END && INCLUDE_END */

#if defined(INCLUDE_MOTSCCEND) && defined(INCLUDE_END)
    sysSccEnetDisable ( 0 );     /* disable the ethernet device              */
    sysSccEnetIntDisable ( 0 );  /* disable the ethernet device interrupt    */
#endif /* INCLUDE_MOTSCCEND && INCLUDE_END */

#if defined(INCLUDE_MOTFCCEND) && defined(INCLUDE_END)
    /* disable the FCCs */
    sysFccEnetDisable (immrVal, 1);
    sysFccEnetDisable (immrVal, 2);
    sysFccEnetDisable (immrVal, 3);
#endif /* INCLUDE_MOTFCCEND && INCLUDE_END */

#if defined(INCLUDE_TTY_DEV) || defined(INCLUDE_SIO_POLL)
    sysSerialReset ();          /* reset the serial device                  */
#endif  /* INCLUDE_TTY_DEV || INCLUDE_SIO_POLL */

#ifdef M48T59_RTC_SRAM
    /* disable machine check input IRQ0, used by RTC alarm/watchdog interrupts */
    vxHid0Set(vxHid0Get() & ~_PPC_HID0_EMCP);
    /* reset the RTC */
    m48t59Reset();
#endif /* M48T59_RTC_SRAM */

    vxMsrSet(0);

    (*pRom) (startType);        /* jump to bootrom entry point              */

    return( OK );               /* in case we continue from ROM monitor     */
    }

/***********************************************************************
*
* sysHwInit2 - initialize additional system hardware
*
* This routine connects system interrupt vectors and configures any
* required features not configured by sysHwInit().
*
* RETURNS: N/A
*
* ERRNO
*/

void sysHwInit2
    (
    void
    )
    {
    static BOOL configured = FALSE ;

    if ( ! configured )
        {
#if defined(INCLUDE_AUX_CLK)
        /*
         * initialize and start auxiliary clock support
         */
        sysAuxClkEnable ();

#ifdef USE_LEDS_FOR_BOOT_DEBUG
        sysLedHex(0xB);
#endif /* USE_LEDS_FOR_BOOT_DEBUG */

		sysAuxClkConnect((FUNCPTR)sysPQIIClkRoutine, 0);

#endif /* INCLUDE_AUX_CLK */

#if defined(INCLUDE_TTY_DEV) || defined(INCLUDE_SIO_POLL)
        /*
         * initialize serial interrupts
         */
        sysSerialHwInit2 ();
#endif  /* INCLUDE_TTY_DEV || INCLUDE_SIO_POLL */

        if (sysCpuHasPci())
            {
#ifdef INCLUDE_i8259A
#  ifdef INCLUDE_PCI
#    if defined(INCLUDE_FEI_END) && defined(INCLUDE_END)
        /* map FEI Ethernet PCI device memory and I/O addresses */
        sys557PciInit ();               /* Intel 82557/9 device */
#    endif /* INCLUDE_FEI_END && INCLUDE_END */

/* Atheros AR521X WLAN Support */

#    ifdef INCLUDE_AR521X_END
        sysDot11BusInit();
#    endif /* INCLUDE_AR521X_END */

#  endif /* INCLUDE_PCI */
#endif /* INCLUDE_i8259A */
            }

#ifdef USE_LEDS_FOR_BOOT_DEBUG
        sysLedHex(0xA);
#endif /* USE_LEDS_FOR_BOOT_DEBUG */

#ifdef INCLUDE_VXBUS
        vxbDevInit ();
        taskSpawn("tDevConn", 11, 0, 10000,
              vxbDevConnect, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
#endif

        /*
         * Indicate we have been through this procedure for reentrancy.
         */
        configured = TRUE;
        }
    }

/***********************************************************************
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

int sysProcNumGet
    (
    void
    )
    {
    return( sysProcNum );
    }

/***********************************************************************
*
* sysProcNumSet - set the processor number
*
* This routine sets the processor number for the CPU board.  Processor numbers
* should be unique on a single backplane.  It also maps local resources onto
* the VMEbus.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysProcNumGet()
*
*/

void sysProcNumSet
    (
    int procNum         /* processor number */
    )
    {
    sysProcNum = procNum;
    }

/***********************************************************************
*
* vxImmrSet - Set the IMMR to a specific value
*
* This routine sets the IMMR to a specific value
*
* RETURNS: N/A
*
* ERRNO
*/

void vxImmrSet
    (
    UINT32 value
    )
    {
    immrAddress = value;
    return;
    }

/***********************************************************************
*
* vxImmrGet - return the current IMMR value
*
* This routine returns the current IMMR value
*
* RETURNS: current IMMR value
*
* ERRNO
*/

UINT32  vxImmrGet
    (
    void
    )
    {
    return (immrAddress);
    }

/***********************************************************************
*
* sysBaudClkFreq - returns the frequency of the BRG clock
*
* This routine returns the frequency of the BRG clock
*
* NOTE: From 'System PLL Block Diagram', page 9-5 in Rev0 of
*       MPC8260 PowerQUICC II User's Manual
*       and 'MPC8280 Clock Block Diagram' page 4-1 in Rev 0.1 of
*       MPC8280 PowerQUICC II Specification
*
* RETURNS: Frequency in HZ
*
* ERRNO
*/

int sysBaudClkFreq
    (
    void
    )
    {

    UINT32 mainFreq, sccrDfbrg;

    /* Get the physical location of the IMMR register */
    UINT32 immrVal = vxImmrGet ();

    sccrDfbrg  = (UINT32)(*(M8260_SCCR(immrVal)));
    sccrDfbrg &= (~M8260_SCCR_RES_MSK);

    mainFreq = sysMainPllFreqGet ();

    switch (sccrDfbrg)
        {
        case M8260_SCCR_DFBRG_4:
	    return mainFreq >> 2;	/* divide by 4 */
        case M8260_SCCR_DFBRG_16:
	    return mainFreq >> 4;	/* divide by 16 */
        case M8260_SCCR_DFBRG_64:
	    return mainFreq >> 6;	/* divide by 64 */
	case M8260_SCCR_DFBRG_128:
	    return mainFreq >> 8;	/* divide by 256 */
        default:
	    return mainFreq >> 4;	/* divide by 16 */
        }
     }

/***********************************************************************
*
* sysInputFreqGet - returns the Input Oscillator clock frequency
*
* This routine returns the Input Oscillator clock frequency, as set in
* config.h.  This must be set correctly to determine CPU and CPM frequencies.
* The input frequency is the same as the 60x Bus frequency.
*
* RETURNS: Input frequency in HZ
*
* ERRNO
*/

UINT32 sysInputFreqGet
    (
    void
    )
    {
    return OSCILLATOR_FREQ;
    }

/***********************************************************************
*
* sysMainPllFreqGet - returns the PowerQUICCII main PLL clock frequency
*
* This routine determines the output frequency of the PowerQUICCII main PLL.
* This frequency is the basis for determining the CPM, CPU and PCI frequencies.
*
* NOTE: From 'Main PLL', page 9-5 in Rev0 of MPC8260 PowerQUICC II User's Manual
*       and 'MPC8280 Clock Block Diagram' page 4-1 in Rev 0.1 of
*       MPC8280 PowerQUICC II Specification
*
* RETURNS: PowerQUICCII main PLL clock frequency in HZ
*
* ERRNO
*/

LOCAL UINT32 sysMainPllFreqGet
    (
    void
    )
    {
    static BOOL calculated = FALSE;  /* only need to calculate mainFreq once */
    static UINT32 mainFreq;
    UINT32  scmrAddr, scmrValue, multFactor, divFactor;
    UINT32  cpuId;

    if (!calculated)
        {
        scmrAddr = vxImmrGet () + (PQII_REG_BASE + PQII_SCMR);

        /* read System Clock Mode Register (SCMR) */
        scmrValue = *(UINT32 *)scmrAddr;

        cpuId = sysCPUIdGet();

        /* calculate output of main PLL (SCMR and equation are CPU dependent) */

        if (cpuId == CPU_ID_8270 || cpuId == CPU_ID_8280)
            {
	    /* MAIN_CLK = CLKIN * SCMR[PLLMF] */
            multFactor = (scmrValue & SCMR_PLLMF_MASK_HIP7) + 1;
            mainFreq = OSCILLATOR_FREQ * multFactor;
            }
        else  /* cpuId = CPU_ID_825x || cpuId == CPU_ID_826x */
            {
            /* MAIN_CLK = CLKIN * (2 * (SCMR[PLLMF] + 1) / (SCMR[PLLDF] + 1)) */
            multFactor = ( (scmrValue & SCMR_PLLMF_MASK_HIP4) + 1 ) << 1;
            divFactor = ( (scmrValue & SCMR_PLLDF_MASK_HIP4) >> 12) + 1;
            mainFreq = OSCILLATOR_FREQ * (multFactor / divFactor);
            }

        calculated = TRUE;
        }

    return (mainFreq);
    }

/***********************************************************************
*
* sysCpmFreqGet - determines the CPM operating frequency
*
* This routine determines the CPM operating frequency
*
* NOTE: From 'Main PLL', page 9-5 in Rev0 of MPC8260 PowerQUICC II User's Manual
*       and 'MPC8280 Clock Block Diagram' page 4-1 in Rev 0.1 of
*       MPC8280 PowerQUICC II Specification
*
* RETURNS: CPM frequency in HZ
*
* ERRNO
*/

UINT32 sysCpmFreqGet
    (
    void
    )
    {
    /* CPM division factor is constant for all MPC82xx CPUs */
    return (sysMainPllFreqGet() / PQII_CPMDF);
    }

/***********************************************************************
*
* sysCoreFreqGet - determines the Core operating frequency
*
* This routine determines the Core operating frequency
*
* NOTE: From '60x Bus-to-Core Frequency', page 15 in Rev. 6 of
*       MPC8260 PowerQUICC II User's Manual Errata
*
*       Special CORECNF values:
*           0x3 and 0x13 are PLL Bypassed, 1x bus-to-core multiplier
*           0xf and 0x1f are PLL Off, 0x bus-to-core multiplier (no core clock)
*           0x18 is reserved, core clock = -1 (ERROR)
*
* RETURNS: Core frequency in HZ
*
* ERRNO
*/

UINT32 sysCoreFreqGet
    (
    void
    )
    {
    static BOOL calculated = FALSE;  /* only need to calculate coreFreq once */
    static UINT32 coreFreq;
    UINT32  scmrAddr, scmrValue, corecnf, multFactor, divFactor;

    if (!calculated)
        {
        scmrAddr = vxImmrGet () + (PQII_REG_BASE + PQII_SCMR);

        /* read System Clock Mode Register (SCMR) */
        scmrValue = *(UINT32 *)scmrAddr;

        corecnf = (scmrValue & SCMR_CORECNF_MASK) >> 24;

	multFactor = (UINT32)(bus_to_core[corecnf].numerator);
	divFactor = bus_to_core[corecnf].denominator;

        /* Use bus-to-core multiplier to determine core clock */
	coreFreq = (OSCILLATOR_FREQ * multFactor) / divFactor;

	calculated = TRUE;
        }

    return (coreFreq);

    }

/***********************************************************************
*
* sysPciFreqGet - determines the PCI operating frequency
*
* This routine determines the PCI operating frequency
*
* NOTE: From 'Main PLL', page 9-5 in Rev0 of MPC8260 PowerQUICC II User's Manual
*       and 'MPC8280 Clock Block Diagram' page 4-1 in Rev 0.1 of
*       MPC8280 PowerQUICC II Specification
*
* RETURNS: PCI frequency in HZ or zero if PCI mode is not enabled in the CPU
*
* ERRNO
*/

UINT32 sysPciFreqGet
    (
    void
    )
    {
    UINT32  sccrAddr, sccrValue, divFactor;
    UINT32  pciFreq;

    if (sysPciEnabled())
        {
        sccrAddr = vxImmrGet () + (PQII_REG_BASE + PQII_SCCR);

        /* read System Clock Mode Register (SCMR) */
        sccrValue = *(UINT32 *)sccrAddr;

        divFactor = ( (sccrValue & SCCR_PCIDF_MASK) >> 3 ) + 1;

        /* Use PCI division factor to determine PCI clock */
        pciFreq = sysMainPllFreqGet() / divFactor;
        }
    else  /* PCI not enabled */
        pciFreq = 0;

    return (pciFreq);
    }

/***********************************************************************
*
* sysModckHGet - determines the value of MODCK_H reset configuration value
*
* This routine determines the value of MODCK_H reset configuration value
*
* RETURNS: MODCK_H value
*
* ERRNO
*/

UINT16 sysModckHGet
    (
    void
    )
    {
	UINT16 modck_h;

#if defined(INCLUDE_i8259A) && defined(INCLUDE_PCI)
    /*
     * In PCI mode, MODCK_H is determined by on-board switches and
     * reflected in the status register.
     */
    if (sysPciEnabled())
      {   
 	modck_h = (sysStatusRegGet() & STATUS_MODCKH_MASK) >> 4;
        modck_h = ((modck_h >> 3) & STATUS_MODCKH0_MASK) | 
                  ((modck_h >> 1) & STATUS_MODCKH1_MASK) | 
                  ((modck_h << 1) & STATUS_MODCKH2_MASK) | 
                  ((modck_h << 3) & STATUS_MODCKH3_MASK); 
      } 
 
   else
#endif /* INCLUDE_8259A && INCLUDE_PCI */

	/* If no PCI, MODCK_H is part of hardware reset configuration word */
        modck_h = (UINT16)(*(UINT8 *)HRDW_CONFIG_BYTE4 & MODCK_H_MASK);

    return modck_h;
    }

/***********************************************************************
*
* sysModck13Get - reads the MODCK[1-3] settings
*
* This routine reads the value of MODCK[1-3] set via on-board switches.
* DIP Switch Bank 1, switches 2, 3 and 4 are reflected in the status register.
*
* NOTE: The value returned are the current MODCK[1-3] settings. Though not
*       recommended, the switch settings could have been changed since being
*       sampled at reset.
*
* RETURNS: MODCK[1-3] value
*
* ERRNO
*/

UINT16 sysModck13Get
    (
    void
    )
    {
    /* Get MODCK[1-3] from the status register */
    return ( (sysStatusRegGet() & STATUS_MODCK13_MASK) >> 12 );
    }

/***********************************************************************
*
* sysChipRev - determines revision of Chip installed
*
* This routine determines the revision of Chip installed
*
* RETURNS: Chip revision
*
* ERRNO
*/

UINT32 sysChipRev
    (
    void
    )
    {
    UINT32  immrRegAddr = vxImmrGet ();
    UINT32  immrValue;

    immrRegAddr += (PQII_REG_BASE + PQII_IMMR);
    immrValue = *(UINT32 *)immrRegAddr;
    immrValue &= IMMR_MASKNUM_MASK;

    return (immrValue);
    }

/***********************************************************************
*
* sysCpmReset - issues a CPM reset command
*
* This routine issues a CPM reset command
*
* RETURNS: N/A
*
* ERRNO
*/

void sysCpmReset
    (
    void
    )
    {
    /* Get the location of the IMMR register.                       */
    int immrVal = vxImmrGet ();

    /* Wait for any previous commands to finish                     */
    while ( *M8260_CPCR( immrVal ) & M8260_CPCR_FLG )
	{}

    *M8260_CPCR( immrVal ) =  M8260_CPCR_RESET | M8260_CPCR_FLG;

    /* See if the command has been accepted.                        */
    while ( *M8260_CPCR( immrVal ) & M8260_CPCR_FLG )
    {}

    return;
    }

/***********************************************************************
*
* sysUserSwitchGet - returns the value of the User Dip Switch
*
* This routine returns the value of the User Dip Switch
*
* NOTE: Reverse bits so S1 is MSB S8 is LSB
*
* RETURNS: Unsigned 8 bit value
*
* ERRNO
*/

UINT8  sysUserSwitchGet
    (
    void
    )
    {
    UINT8   swValue;
    UINT8   modChar;

    swValue = *((volatile UINT8 *)(BSCR_USER_SWITCH));
    modChar = 0;

    /* skip obvious equalities */
    if ( swValue == 0x00 || swValue == 0xFF )
        return swValue;

    if (swValue & 0x80) modChar += 0x01;
    if (swValue & 0x40) modChar += 0x02;
    if (swValue & 0x20) modChar += 0x04;
    if (swValue & 0x10) modChar += 0x08;
    if (swValue & 0x08) modChar += 0x10;
    if (swValue & 0x04) modChar += 0x20;
    if (swValue & 0x02) modChar += 0x40;
    if (swValue & 0x01) modChar += 0x80;

    return modChar;
    }


/***********************************************************************
*
* sysPQIIClkRoutine - shows clock activity on LED
*
* This routine blinks the dot on the Hex LED
*
* RETURNS: N/A
*
* ERRNO
*/

void sysPQIIClkRoutine
    (
    int arg
    )
    {
#ifdef INCLUDE_SYSLED
    static int clkIntCount = 0;
    static INT8 clkCount = 0;

    if((clkIntCount++ % 60) == 0)
	    {
	    (clkCount++ & BIT(0)) ?
		    sysLedSet(LED_POINT, LED_POINT) : sysLedSet(LED_POINT, 0);
		}
#endif /* INCLUDE_SYSLED */
    }

/***********************************************************************
*
* sysCpuHasPci - determines processor PCI capability
*
* This routine returns whether the processor contains a PCI bridge.
* It does not determine if PCI is enabled.
*
* RETURNS: TRUE if processor supports PCI, FALSE otherwise
*
* ERRNO
*/

BOOL sysCpuHasPci
    (
    void
    )
    {
    static BOOL determined = FALSE;  /* only need to determine once */
    static BOOL pciCapable;

    if (!determined)
        {
        switch (sysCPUIdGet())
            {
            case CPU_ID_8255:
            case CPU_ID_8260:
            case CPU_ID_8264:
                pciCapable = FALSE;
                break;
            default:
                pciCapable = TRUE;
                break;
            }
        determined = TRUE;
        }

    return (pciCapable);
    }

/***********************************************************************
*
* sysPciEnabled - determines if processor PCI mode is enabled
*
* This routine returns whether the processor has enabled PCI mode.
*
* RETURNS: TRUE if processor PCI mode is enabled, FALSE otherwise
*
* ERRNO
*/

BOOL sysPciEnabled
    (
    void
    )
    {
    BOOL pciEnabled = FALSE;

    if (sysCpuHasPci())
        {
        /* read Status Register */
        pciEnabled = ( sysStatusRegGet() & STATUS_PCIMODE_MASK ) ? FALSE : TRUE;
        }

    return (pciEnabled);
    }


#ifdef INCLUDE_i8259A
/*******************************************************************************
*
* sysIntInitPIC - initialize the interrupt controller
*
* This routine initializes the interrupt controller.
*
* RETURNS: N/A
*
* ERRNO
*
*/

LOCAL void sysIntInitPIC (void)
    {
    i8259Init ();
    }

/*******************************************************************************
*
* sysIntLock - lock out all interrupts
*
* This routine saves the mask and locks out all interrupts.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysIntUnlock()
*
*/

VOID sysIntLock (void)

    {
    i8259IntLock ();
    }

/*******************************************************************************
*
* sysIntUnlock - unlock the PIC interrupts
*
* This routine restores the mask and unlocks the PIC interrupts
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysIntLock()
*
*/

VOID sysIntUnlock (void)

    {
    i8259IntUnlock ();
    }

/*******************************************************************************
*
* sysIntDisablePIC - disable a bus interrupt level
*
* This routine disables a specified bus interrupt level.
*
* RETURNS: OK, or ERROR if failed.
*
* ERRNO
*
*/

STATUS sysIntDisablePIC
    (
    int irqNo     /* IRQ(PIC) or INTIN(APIC) number to disable */
    )
    {
    return (i8259IntDisable (irqNo));
    }

/*******************************************************************************
*
* sysIntEnablePIC - enable a bus interrupt level
*
* This routine enables a specified bus interrupt level.
*
* RETURNS: OK, or ERROR if failed.
*
* ERRNO
*
*/

STATUS sysIntEnablePIC
    (
    int irqNo     /* IRQ(PIC) or INTIN(APIC) number to enable */
    )
    {
    return (i8259IntEnable (irqNo));
    }

/*******************************************************************************
*
* sysIntEoi - send EOI(end of interrupt) signal to the PIC.
*
* This routine is called at the end of the interrupt handler.
*
* RETURNS: N/A
*
* ERRNO
*
*/

void sysIntEoi(void)
    {
    i8259IntEoi ();
    }

/*******************************************************************************
*
* sysIntLevel - Get interrupt level by reading Interrupt Service Register.
*
* This routine is called to get an interrupt level in service.
*
* RETURNS: 0 - 15.
*
* ERRNO
*
*/
int sysIntLevel(void)
    {
    return (i8259IntLevel ());
    }

#  ifdef INCLUDE_PCI      /* board level PCI routines */
/*******************************************************************************
*
* sysPciSpecialCycle - generate a special cycle with a message
*
* This routine generates a special cycle with a message.
*
* RETURNS: OK
*
* ERRNO
*
*\NOMANUAL
*/

STATUS sysPciSpecialCycle
    (
    int         busNo,
    UINT32      message
    )
    {
    int deviceNo = 0x0000001f;
    int funcNo   = 0x00000007;

    sysIntLock ();
    PCI_OUT_LONG (sysPciConfAddr,
                  pciConfigBdfPack (busNo, deviceNo, funcNo) |
                  0x80000000);

    PCI_OUT_LONG (sysPciConfData, message);
    sysIntUnlock ();

    return (OK);
    }

/*******************************************************************************
*
* sysPciConfigRead - read from the PCI configuration space
*
* This routine reads either a byte, word or a long word specified by
* the argument <width>, from the PCI configuration space.
* This routine works around a problem in the hardware which hangs
* PCI bus if device no 12 is accessed from the PCI configuration space.
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* ERRNO
*
* \NOMANUAL
*/

STATUS sysPciConfigRead
    (
    int busNo,    /* bus number */
    int deviceNo, /* device number */
    int funcNo,   /* function number */
    int offset,   /* offset into the configuration space */
    int width,    /* width to be read */
    void * pData  /* data read from the offset */
    )
    {
    UINT8  retValByte = 0;
    UINT16 retValWord = 0;
    UINT32 retValLong = 0;
    STATUS retStat = ERROR;

    switch (width)
        {
        case 1: /* byte */
            sysIntLock ();
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);

            retValByte = PCI_IN_BYTE (sysPciConfData + (offset & 0x3));
            sysIntUnlock ();
            *((UINT8 *)pData) = retValByte;
            retStat = OK;
            break;

        case 2: /* word */
            sysIntLock ();
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);

            retValWord = PCI_IN_WORD (sysPciConfData + (offset & 0x2));
            sysIntUnlock ();
            *((UINT16 *)pData) = retValWord;
            retStat = OK;
            break;

        case 4: /* long */
            sysIntLock ();
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);
            retValLong = PCI_IN_LONG (sysPciConfData);
            sysIntUnlock ();
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
* the argument <width>, to the PCI configuration space.
* This routine works around a problem in the hardware which hangs
* PCI bus if device no 12 is accessed from the PCI configuration space.
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* ERRNO
*
* \NOMANUAL
*/

STATUS sysPciConfigWrite
    (
    int busNo,    /* bus number */
    int deviceNo, /* device number */
    int funcNo,   /* function number */
    int offset,   /* offset into the configuration space */
    int width,    /* width to write */
    ULONG data    /* data to write */
    )
    {

    switch (width)
        {
        case 1: /* byte */
            sysIntLock ();
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);
            PCI_OUT_BYTE ((sysPciConfData + (offset & 0x3)), data);
            sysIntUnlock ();
            break;

        case 2: /* word */
            sysIntLock ();
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);
            PCI_OUT_WORD ((sysPciConfData + (offset & 0x2)), data);
            sysIntUnlock ();
            break;

        case 4: /* long */
            sysIntLock ();
            PCI_OUT_LONG (sysPciConfAddr,
                          pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000);
            PCI_OUT_LONG (sysPciConfData, data);
            sysIntUnlock ();
            break;

        default:
            return (ERROR);

        }
    return (OK);
    }

/******************************************************************************
*
* sysUsDelay - delay at least the specified amount of time (in microseconds)
*
* This routine delays for at least the specified amount of time using the
* lower 32 bit "word" of the Time Base register as the timer.  The accuracy of
* the delay increases as the requested delay increases due to a certain amount
* of overhead.  As an example, a requested delay of 10 microseconds is
* accurate within approximately twenty percent, and a requested delay of 100
* microseconds is accurate within approximately two percent.
*
* NOTE:  This routine does not relinquish the CPU; it is meant to perform a
* busy loop delay.  The minimum delay that this routine provides is
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
    UINT32    delay        /* length of time in microsec to delay */
    )
    {
    FAST UINT32 baselineTickCount;
    FAST UINT32 ticksToWait;

    /*
     * Get the Time Base Lower register tick count, this will be used
     * as the baseline.
     */

    baselineTickCount = sysTimeBaseLGet();

    /*
     * Convert delay time into ticks
     *
     * The Time Base register and the Decrementer count at the same rate:
     * once per 4 System Bus cycles.
     *
     * e.g., 66666666 cycles     1 tick      1 second             16 tick
     *       ---------------  *  ------   *  --------          =  ----------
     *       second              4 cycles    1000000 microsec    microsec
     */

    if ((ticksToWait = delay * ((DEC_CLOCK_FREQ / 4) / 1000000)) == 0)
        return;

    while ((sysTimeBaseLGet() - baselineTickCount) < ticksToWait);
    }

/*********************************************************************
*
* sysMsDelay - Uses the decrementer to calculate time elapsed
*
* void sysMsDelay
*   (
*   UINT        delay              * length of time in MS to delay *
*   )
*
* RETURNS : NONE
*
* ERRNO
*/

void sysMsDelay
    (
    UINT        delay                   /* length of time in MS to delay */
    )
    {
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
    }


/*********************************************************************
*
* sysDelay - Fixed 1ms delay. Just calls sysMsDelay
*
* sysDelay(void)
*
* RETURNS : NONE
*
* ERRNO
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }
#  endif  /* INCLUDE_PCI */
#endif  /* INCLUDE_i8259A */

#ifdef INCLUDE_VXBUS

LOCAL STATUS m8260PlbDevControl
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

    vector = (UINT32)pIntr->intVecList[pIntInfo->intIndex];
    source = pIntr->intLvlList[pIntInfo->intIndex];

    switch (pBusDevControl->vxbAccessId)
        {
        case VXB_ACCESS_INT_CONNECT:
            status = intConnect (m8260InumToIvec(vector),
                pIntInfo->pISR, (int)pIntInfo->pParameter);
            break;
        case VXB_ACCESS_INT_DISCONN:
            status = intDisconnect (m8260InumToIvec(vector),
                pIntInfo->pISR, (int)pIntInfo->pParameter);
            break;
        case VXB_ACCESS_INT_VEC_GET:
            pVector = (UINT32 *)pIntr->intVecList;
            pIntVector->pIntVector =
                (VOIDFUNCPTR *)pVector[pIntInfo->intIndex];
            status = OK;
            break;
        case VXB_ACCESS_INT_ENABLE:
            status = intEnable (vector);
            break;
        case VXB_ACCESS_INT_DISABLE:
            status = intDisable (vector);
            break;
        default:
            status = ERROR;
            break;
        }

    return (status);
    }

#ifdef	VXB_LEGACY_ACCESS
LOCAL void m8260PlbAccessFunctionOverride
    (
    struct vxbAccessList * pAccessList
    )
    {
    pAccessList->vxbDevControl = m8260PlbDevControl;
    return;
    }
#endif	/* VXB_LEGACY_ACCESS */

#ifdef INCLUDE_FCC_VXB_END

/***********************************************************************
*
* sysFccEnetEnable - enable an FCC controller
*
* This routine is expected to perform any target specific functions required
* to enable the Ethernet device and to connect the MII interface to the FCC.
*
* RETURNS: OK, or ERROR if the FCC controller cannot be enabled.
*
* ERRNO
*/

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
        case 1:
            /* This port is only available on the non ATM boards.*/
            /* set Port A and C to use MII signals */

            /* RXCK  CLK9 & TXCK CLK10 */
            *M8260_IOP_PCPAR(immrVal) |=  (PC23 | PC22);
            /* select as - clk9 & clk10 */
            *M8260_IOP_PCSO(immrVal)  &= ~(PC23 | PC22);
            /* they are inputs */
            *M8260_IOP_PCDIR(immrVal) &= ~(PC23 | PC22);

            /* MCI0 & MDC are general io*/
            *M8260_IOP_PCPAR(immrVal) &= ~(PC10 | PC9);

            *M8260_IOP_PAPAR(immrVal) |= 
                (PA31 | PA30 | PA29 | PA28 | PA27 | PA26 | PA21 |
                 PA20 | PA19 | PA18 | PA17 | PA16 | PA15 | PA14);
            *M8260_IOP_PADIR(immrVal) |=
                (PA29 | PA28 | PA21 | PA20 | PA19 | PA18);
            *M8260_IOP_PADIR(immrVal) &=
                ~(PA31 | PA30 | PA27 | PA26 | PA17 | PA16 | PA15 | PA14);
            *M8260_IOP_PASO(immrVal)  &=
                ~(PA21 | PA20 | PA19 | PA18 | PA17 | PA16 | PA15 | PA14);
            *M8260_IOP_PASO(immrVal)  |=
                (PA31 | PA30 | PA29 | PA28 | PA27 | PA26);

            /* connect FCC1 clocks */

            /* RXCK-CLK9  TXCK-CLK10 */
            *M8260_CMXFCR (immrVal)  |=  (0x25000000);
            *M8260_CMXFCR (immrVal)  &= ~(0x40000000);

            break;

        case 2:
            /* set Port B and C to use MII signals */

            /* Rx CLK13 and Tx CLK14 */
            *M8260_IOP_PCPAR(immrVal) |=  (PC19 | PC18);
            *M8260_IOP_PCSO(immrVal)  &= ~(PC19 | PC18);
            *M8260_IOP_PCDIR(immrVal) &= ~(PC19 | PC18);

            *M8260_IOP_PBPAR(immrVal) |=
                (PB31 | PB30 | PB29 | PB28 | PB27 | PB26 | PB25 |
                 PB24 | PB23 | PB22 | PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBDIR(immrVal) |=
                (PB31 | PB29 | PB25 | PB24 | PB23 | PB22);
            *M8260_IOP_PBDIR(immrVal) &=
                ~(PB30 | PB28 | PB27 | PB26 | PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBSO(immrVal)  &=
                ~(PB31 | PB30 | PB28 | PB27 | PB26 | PB25 | PB24 |
                  PB23 | PB22 | PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBSO(immrVal)  |= (PB29);

            /* connect FCC2 clocks */

            *M8260_CMXFCR (immrVal)  |=  (0x00250000);
            *M8260_CMXFCR (immrVal)  &= ~(0x004a0000);

            break;

        case 3:
            /* set Port B and C to use MII signals */

            /* Tx CLK15 and Rx CLK16 */
            *M8260_IOP_PCPAR(immrVal) |=  (PC17 | PC16);
            *M8260_IOP_PCSO(immrVal)  &= ~(PC17 | PC16);
            *M8260_IOP_PCDIR(immrVal) &= ~(PC17 | PC16);

            *M8260_IOP_PBPAR(immrVal) |=
                (PB13 | PB12 | PB15 | PB14 | PB17 | PB16 | PB4 |
                 PB5 | PB6 | PB7 | PB8 | PB9 | PB10 | PB11);
            *M8260_IOP_PBDIR(immrVal) |=
                (PB15 | PB14 | PB4 | PB5 | PB6 | PB7);
            *M8260_IOP_PBDIR(immrVal) &=
                ~(PB13 | PB12 | PB17 | PB16 | PB8 | PB9 | PB10 | PB11);
            *M8260_IOP_PBSO(immrVal)  &=
                ~(PB13 | PB12 | PB15 | PB14 | PB17 | PB16 | PB4 |
                  PB5 | PB6 | PB7 | PB8 | PB9 | PB10 | PB11);

            /* connect FCC3 clocks */

            /* RXCK=CLK15 TXCK=CLK16 */
            *M8260_CMXFCR (immrVal)  |=  (0x00003700);
            *M8260_CMXFCR (immrVal)  &= ~(0x00004800);

            break;
        default:
            rval = ERROR;
            break;
        }

    intUnlock (intLevel);

    return (rval);
    }

#endif /* INCLUDE_FCC_VXB_END */

#if defined(INCLUDE_FCC_VXB_END) || defined(INCLUDE_SCC_VXB_END)

/*********************************************************************** 
*
* sysEnetAddrGet - get the hardware Ethernet address
*
* This routine provides the six byte Ethernet hardware address that will be
* used by each individual FCC device unit.  This routine must copy
* the six byte address to the space provided by <addr>.
* The unit number must not be zero this is used for the scc.
*
* RETURNS: OK, or ERROR if the Ethernet address cannot be returned.
*
* ERRNO
*/
STATUS sysEnetAddrGet
    (
    int unit, 
    UCHAR * pAddr
    )
    {
    /* need extra room for a 'EOS' -- driver may not provide it */
    UINT8 eAdrs[MAC_ADRS_LEN + 2];
    STATUS  status;
    int ix;

    status = sysNetMacNVRamAddrGet ("motfcc", unit, eAdrs, MAC_ADRS_LEN);
    
    if (status == OK)
        {

        for (ix = 0; ix < MAC_ADRS_LEN; ix++)
            pAddr[ix] = eAdrs[ix];
        }
    return status;
    }
#endif

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

#ifdef INCLUDE_SCC_VXB_END

/***********************************************************************
*
* sysSccEnetEnable - enable the Ethernet controller
*
* This routine is expected to perform any target specific functions required
* to enable the Ethernet controller.  These functions typically include
* enabling the Transmit Enable signal (TENA) and connecting the transmit
* and receive clocks to the SCC.
*
* RETURNS: OK, or ERROR if the Ethernet controller cannot be enabled.
*
* ERRNO
*/

STATUS sysSccEnetEnable
    (
    VXB_DEVICE_ID pDev,
    UINT8 unit    /* not used - only slave SCC1 is wired to port */
    )
    {
    UINT32 temp;
    int immrVal = vxImmrGet ();

    /*
     * Configure port D pins to enable TXD1 and RXD1.
     *
     * Set PPARD[30,31] and PDIRD[30] and clear PDIRD[31]
     * and PSORD[31].  Set PSORD[30].
     */
    *M8260_IOP_PDPAR(immrVal) |=  (PD30 | PD31);  /* Set PD30 and PD31 */
    *M8260_IOP_PDDIR(immrVal) |=  (PD30);         /* Set PD30 */
    *M8260_IOP_PDDIR(immrVal) &= ~(PD31);         /* Clear PD31 */
    *M8260_IOP_PDSO(immrVal)  |=  (PD30);         /* Set PD30 */
    *M8260_IOP_PDSO(immrVal)  &= ~(PD31);         /* Clear PD31 */

    /*
     * Configure ports C and D to enable TENA1 (RTS1), CLSN1 (CTS1),
     * and RENA1 (CD1).
     *
     * Set PPARD[29], PPARC[14,29], PDIRD[29], and PSORC[29] and clear
     * PDIRC[14,29], PSORC[14] and PSORD[29].
     */
    *M8260_IOP_PDPAR(immrVal) |=  (PD29);         /* Set PD29 */
    *M8260_IOP_PCPAR(immrVal) |=  (PC14 | PC29);  /* Set PC14 and PC29 */
    *M8260_IOP_PDDIR(immrVal) |=  (PD29);         /* Set PD29 */
    *M8260_IOP_PCDIR(immrVal) &= ~(PC14 | PC29);  /* Clear PC14 and PC29 */
    *M8260_IOP_PCSO(immrVal)  &= ~(PC14);         /* Clear PC14 */
    *M8260_IOP_PDSO(immrVal)  &= ~(PD29);         /* Clear PD29 */

    /*
     * Configure port C pins to enable CLK11 and CLK12.
     *
     * Set PPARC[20,21] and clear PDIRC[20,21] and PSORC[20,21].
     */
    *M8260_IOP_PCPAR(immrVal) |=  (PC20 | PC21);  /* Set PC20 and PC21 */
    *M8260_IOP_PCDIR(immrVal) &= ~(PC20 | PC21);  /* Clear PC20 and PC21 */
    *M8260_IOP_PCSO(immrVal)  &= ~(PC20 | PC21);  /* Clear PC20 and PC21 */


    /*
     * Connect CLK11 to the SCC1 receiver and CLK12 to the
     * transmitter using the CPM mux.  Also, connect the SCC1
     * to the NMSI.
     *
     * Program CMXSCR[R1CS] to 0b100 and CMXSCR[T1CS] to 0b101.
     * Clear CMXSCR[SC1].
     */
    temp = (*M8260_CMXSCR(immrVal) & 0x00ffffff);
    *M8260_CMXSCR(immrVal) = (temp | 0x25000000);

    /*
     * The SBC boards come with an Intel LXT905 10Mbps transceiver,
     * which supports both full and half duplex modes. The FDE pin
     * is wired to port C, pin 2. For some unfathomable reason, the
     * reset default is for full duplex mode. This is wrong: this
     * transceiver can't do autoneg, so there's no way for the link
     * partner to know the duplex mode of the transceiver. In the
     * absence of NWAY autoneg, the link partner will default to
     * parallel detection, and select 10Mbps half-duplex, not full.
     * The sensible default is therefore half duplex. We need to
     * configure port C pin 2 as an output and force it high here
     * to change the setting.
     */

    *M8260_IOP_PCDIR(immrVal) |= PC2;
    *M8260_IOP_PCPAR(immrVal) |= PC2;

    return (OK);
    }

#endif /* INCLUDE_SCC_VXB_END */

#endif /* INCLUDE_VXBUS */
