/* sysLib.c - Motorola ads 85xx board system-dependent library */

/*
 * Copyright (c) 2002-2009, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
03d,06mar13,d_l  fix build errors when INCLUDE_TICK_TIMERS is defined. 
                 (WIND00415168)
03c,28jul09,my_  fix VIP build error when INCULDE_PCI_CFGSHOW defined
                 (WIND00173132)
03b,24apr09,z_l  Removed PCI config done flag check. (WIND00158834)
03a,15oct08,x_z  fix missing "sysBus" api error for BSPVTS(WIND00135289) 
02z,24apr08,x_f  Add vxBus support, replace legacy serail port, DEC and FIT
                 clock timer driver with vxBus version.
02y,21dec07,y_x  Fix sysPhysMemTop() computing problem. (WIND00100664)
02x,17oct07,y_w  Fix compiler warnings.
02w,30jul07,agf  smUtilTasClearRtn is set only if sm is included
02v,20mar07,y_w  Add dummy functions and variable to avoid symbol loading error
                 (WIND00038065).
02u,11dec06,jch  (WRS) Updated for VxWorks 6.x compatibility, fixed assignment
                 typo in sysPhbCapt()
02t,01sep05,efb  Added Wind River USB support and controller config.
02s,31aug05,cak  TFFS and flash programming support.
02r,18aug05,pjh  Support for vxWorks AuxClk.
02q,28aug05,efb  Added core freq get routine, separated from cpu bus speed.
02p,18aug05,efb  Added SMC DDR and PCI/X error handling support.
02o,15aug05,efb  Added hard reset, user LED control, and SPE support.
02n,12aug05,pjh  Support for global and external tick timers.
02m,02aug05,efb  Added Watchdog Timer support.
02l,02aug05,cak  Added EREADY (PrPMC) support, added code to disable the 
                 Tsi148's outbound windows in sysHwInit() in order to support
                 firmware booting, and fixed a GNU compiler warning in 
                 sysPhysMemTop(). 
02k,22jul05,efb  Added LBC interrupt error handling.
02j,19jul05,efb  Added ECM interrupt error handling.
02i,25jun05,efb  Added performance monitor support.
02h,21jun05,cak  DS1375 RTC with Alarm support.
02g,02jun05,pjh  Added Quad UART EXAR ST16C554D support.
02f,15may05,efb  Added sATA support.
02e,03may05,cak  Bus Probing Support.
02d,22apr05,cak  DS1621 Temperature Sensor Support.
02c,12apr05,cak  Set the 'M', or memory coherency bit in the DRAM entry in
                 sysPhysMemDesc[].
02b,25mar05,cak  MPC8540 DMA Support.
02a,29mar05,cak  Removed localI2c.c since this is I2C support for
                 the XMC8540's own I2C Bus.
01z,22feb05,cak  Error Handling Support fixes/enhancements.
01y,09feb05,cak  L2 Cache/SRAM support.
01x,31jan05,cak  VPD Support.
01w,27jan05,cak  NVRAM support.
01v,11jan05,cak  Added shared memory support. 
01u,06jan05,cak  Added entries to sysStaticTlbDesc[] to cover 1GB of
                 DRAM, so that all of DRAM is available in the bootrom. 
01t,17dec04,cak  Phase 1 of the Tsi148 integration.
01s,13dec04,cak  PCI/PCI-X Bus Interface support. 
01r,24nov04,cak  Memory controller support and fixed sysL2SramEnable()
                 routine. 
01q,19nov04,scb  Split off show routines into "sysMpc8540Show.c".
01p,09nov04,scb  Remove DELTA macro.
01o,11oct04,scb  Move flash banks to 0xf0000000.
01n,23aug04,scb  Starting point for "Big Easy".
01m,03nov03,dtr  Minor fixes..
01l,27oct03,dtr  Use GNU_TOOL instead of __GCC__.
01k,21oct03,dtr  Remove EUMBAR read/write.
01j,17oct03,dtr  Modifying Pvr lkup.
01i,29sep03,dtr  Set default aux clock setting if included.
01h,09sep03,dtr  Clear L2 control register prior to Enable.
01g,20aug03,dtr  Adding in TSEC configlette file. 
                 Adding new FCC2 End driver support.
                 Test for bootrom then initialize static TLB entries.
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
This library provides board-specific routines for ADS85XX.

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
#include <arch/ppc/mmuE500Lib.h>
#include <arch/ppc/vxPpcLib.h>
#include <private/vmLibP.h>
#include <miiLib.h>
#include "errorHandler.h"

#ifdef INCLUDE_VXBUS
#   include <hwif/vxbus/vxBus.h>
#   include "hwconf.c"
#endif

#include "drv/pci/pciConfigLib.h"

#ifdef INCLUDE_ATA
#   include "drv/hdisk/ataDrv.h"
#endif

#ifdef INCLUDE_SPE
#   include "speLib.h"
#endif /* INCLUDE_SPE */

#include "mpc8540.h"

/* defines */

#undef  BUS_PROBE_TEST        /* test routines for sysBusProbe support */

#define ZERO    0

#define xbit0(x, n)    ((x & (1 << (31 - n))) >> (31 - n))  /* 0..31 */
#define xbit32(x, n)   ((x & (1 << (63 - n))) >> (63 - n))  /* 32..63 */
#define onoff0(x, n)   xbit0(x, n) ? "ON", "OFF"
#define onoff32(x, n)  xbit32(x, n) ? "ON", "OFF"

/* needed to enable timer base */

#define HID0_MCP                0x80000000
#define MAX_VALUE_PLAT_RATIO    32
#define MAX_VALUE_E500_RATIO    8
#define END_OF_TABLE            0
#define EXT_VEC_IRQ0            56
#define EXT_NUM_IRQ0            EXT_VEC_IRQ0
#define EXT_MAX_IRQS            200

#define WB_MAX_IRQS             256

/* typedefs */

typedef struct phbWinStruct
    {
    UINT32 winType;   /* mem or i/o */
    UINT32 winBase;   /* start of window */
    UINT32 winLimit;  /* end of window */
    } PHB_WIN_STRUCT;

/* globals */

ERROR_HANDLER_INIT _func_errorHandlerInit = {NULL};
UINT32 cpuBusSpeed = 133333333;

TLB_ENTRY_DESC sysStaticTlbDesc [] =
    {

    /*
     * This table only affects TLB1, not TLB0.
     * TLB1 is loaded with such that ESEL matches the entry number in the
     * table below:
     *
     * effAddr,  Unused,  realAddr, ts | size | attributes | permissions
     *
     * Some entries below cover the same address range, once with the
     * TS (Translation Space) bit set to 0 and once set to 1.  Any
     * access made during bootrom code or after sysToMonitor() has been
     * executed (and MSR cleared) must have an entry whose TS bit is
     * set to zero.  After mmuInit() is run and we are operating in
     * the kernel, accesses require the TS bit to be 1.  Thus the need
     * for two separate TLB entries.  Also, note that when the kernel
     * comes up, after executing usrMmuInit(), that all accesses (other
     * than the CCSBAR, or Configuration, Control, and Status registers) 
     * will be through TLB0 via MMU page table mechanism.  The MMU page 
     * table extent is coded inside of sysPhysMemDesc[].
     *
     * The first four entries below have the IPROT bit set (protecting
     * them during TLB flash invalidate operations) and also have the
     * translation space bit (TS) set to zero.  When a ctl-x or 'reboot'
     * command is executed the MSR is cleared and only those entries
     * whose TS = 0 bit in the TLB1 array will be translated.  It is
     * recommended that the first four entries in the table below match
     * the first four entries created by romInit during a cold boot.
     * Doing so will ensure that the boot bank, CCSBAR space, IO bus space
     * and low order 256MB of DRAM are still mapped after sysToMonitor()
     * is executed as a result of a reboot command.
     *
     * NOTE: TLB1 only allows for up to 16 entries.
     */

        /* ESEL = 0 Flash bank mapping - required for 'reboot' (TS = 0) */

        { 
        FLASH_BASE_ADRS, 0x0, FLASH_BASE_ADRS, _MMU_TLB_TS_0 | 
          FLASH_MMU_TLB_SZ | _MMU_TLB_IPROT | _MMU_TLB_PERM_W |
        _MMU_TLB_PERM_X | _MMU_TLB_ATTR_I
        },

        /* ESEL = 1 CCSBAR registers (TS = 0) */

        { 
        CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_0 | _MMU_TLB_SZ_1M |
        _MMU_TLB_IPROT | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        }, 

        /* ESEL = 2 Local Bus I/O (TS = 0) */

        { 
        LOCAL_BOARD_ACCESS_ADRS, 0x0, LOCAL_BOARD_ACCESS_ADRS, 
        _MMU_TLB_TS_0 | LOCAL_BOARD_ACCESS_MMU_TBL_SZ |
        _MMU_TLB_IPROT | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        }, 

        /*
         * ESEL = 3 Maps low memory, this is required by the bootrom.
         * The bootrom does not execute usrMmuInit() (which enables use
         * of TLB0 to map DRAM), so all DRAM access which the bootrom makes
         * must be mapped by a TLB1 entry with TS = 0.  AFter the kernel
         * comes up, the DRAM mappings change to TLB0 with TS = 1 (as a 
         * result of executing usrMmuInit.  
         *
         * Another time where this entry is actually used is during that 
         * short period in sysToMonitor() after the MSR is cleared and before
         * we are actually executing out of flash.  As soon as the MSR
         * is cleared the TLB entries with TS = 0 become the ones which
         * are hit on the translation.
         *
         * The following entries will support up to 1GB of DRAM.  If your
         * board has more than 1GB of DRAM you will need additional 
         * entries in the table.  Note, the maximum size for each TLB
         * entry is 256MB, which is why 4 entries are used to cover 1GB.
         */

        { 
        0x00000000, 0x0, 0x00000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
        _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M 
        },

        { 
        0x10000000, 0x0, 0x10000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
        _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M 
        },

        { 
        0x20000000, 0x0, 0x20000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
        _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M 
        },

        { 
        0x30000000, 0x0, 0x30000000, _MMU_TLB_TS_0 | _MMU_TLB_SZ_256M |
        _MMU_TLB_IPROT | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | _MMU_TLB_ATTR_M 
        },

        /* ESEL = 5 CCSBAR registers (TS = 1) */

        { 
        CCSBAR, 0x0, CCSBAR, _MMU_TLB_TS_1 | _MMU_TLB_SZ_1M |
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        },

        { 
        MPC8540_L2_SRAM_BASE, 0x0, MPC8540_L2_SRAM_BASE, _MMU_TLB_TS_0 | 
        _MMU_TLB_SZ_256K | _MMU_TLB_PERM_W | _MMU_TLB_PERM_X | 
        _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G
        },

        /*
         * The next entry covers the PCI space.  The entry covers
         * 256MB, the maximum, allowed by a single TLB1 entry.  
         * TS is set to 0 to allow access to PCI space from the 
         * bootrom. The total PCI space covered is 256MB
         * which is more than the default BSP allots for PCI space.
         * If you increase the amount of PCI space it may be necessary
         * to add more entries here. 
         *
         * This entry is needed to allow access to the beginning of PCI
         * memory space - where the Tsi148 PCI to VME bridge chip 
         * is mapped - to allow us to read/write its registers
         * from the bootrom, in order to configure the chip.
         */

        { 
        PCI_MEM_LOCAL_START + 0x00000000 , 0x0, 
        PCI_MEM_LOCAL_START + 0x00000000 , _MMU_TLB_TS_0 | 
        _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        },

        { 
        PCI_MEM_LOCAL_START + 0x00000000 , 0x0, 
        PCI_MEM_LOCAL_START + 0x00000000 , _MMU_TLB_TS_1 | 
        _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        },

        /* 
         * The next entries are PCI as well but focused at VME 
         * access space.  In total 512MB of VME space is covered, which
         * is as much as the current memory map will allow, without
         * running into PCI space.  The TS = 0 entries are required to
         * support shared memory, both with the shared memory anchor
         * onboard as well as offboard.
         */

        { 
        VME_MEM_LOCAL_START + 0x00000000 , 0x0, 
        VME_MEM_LOCAL_START + 0x00000000 , _MMU_TLB_TS_0 | 
        _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        },
        
        { 
        VME_MEM_LOCAL_START + 0x00000000 , 0x0, 
        VME_MEM_LOCAL_START + 0x00000000 , _MMU_TLB_TS_1 | 
        _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        },
        
        { 
        VME_MEM_LOCAL_START + 0x10000000 , 0x0, 
        VME_MEM_LOCAL_START + 0x10000000 , _MMU_TLB_TS_0 | 
        _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        },
        
        { 
        VME_MEM_LOCAL_START + 0x10000000 , 0x0, 
        VME_MEM_LOCAL_START + 0x10000000 , _MMU_TLB_TS_1 | 
        _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I | _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W
        },
    };

int sysStaticTlbDescNumEnt = NELEMENTS (sysStaticTlbDesc);

/*
 * sysPhysMemDesc[] is used to initialize the Page Table Entry (PTE) array
 * used by the MMU to translate addresses with single page (4k) granularity.
 * PTE memory space should not, in general, overlap BAT memory space but
 * may be allowed if only Data or Instruction access is mapped via BAT.
 *
 * Address translations for local RAM, memory mapped PCI bus, the Board 
 * Control and Status registers, the MPC8260 Internal Memory Map, and 
 * local FLASH RAM are set here.
 *
 * PTEs are held, strangely enough, in a Page Table.  Page Table sizes are
 * integer powers of two based on amount of memory to be mapped and a
 * minimum size of 64 kbytes.  The MINIMUM recommended Page Table sizes
 * for 32-bit PowerPCs are:
 *
 * Total mapped memory        Page Table size
 * -------------------        ---------------
 *        8 Meg                 64 K
 *       16 Meg                128 K
 *       32 Meg                256 K
 *       64 Meg                512 K
 *      128 Meg                  1 Meg
 *     .                .
 *     .                .
 *     .                .
 *
 * [Ref: chapter 7, PowerPC Microprocessor Family: The Programming Environments]
 *
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {

    /*
     * Note that sysPhysMemDesc[] is used to initialze the MMU via usrMmuInit()
     * called from inside of "usrConfig.c".  Ultimately all MMU hits will be 
     * cached inside of the TLB0 array, except for the CCSBAR.  The TLB1
     * mappings are guided by the previously encountered sysStaticTlbDesc[]
     * table.  The TLB0 mappings are ultimately guided by sysPhysMemDesc[].
     */

        {

        /* dynamically adjusted in sysHwInit() */

        LOCAL_MEM_LOCAL_ADRS,
        LOCAL_MEM_LOCAL_ADRS,
        0x0,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT |
        MMU_ATTR_CACHE_COHERENCY
        },

        { 

        /* Boot Flash bank mapping */

        FLASH_BOOT_ADRS, 
        FLASH_BOOT_ADRS,
        0x0,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
        MMU_ATTR_CACHE_GUARDED
        },

        {

        /* Local Bus I/O */
 
        LOCAL_BOARD_ACCESS_ADRS, 
        LOCAL_BOARD_ACCESS_ADRS,
        LOCAL_BOARD_ACCESS_SIZE,    
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
                                             MMU_ATTR_CACHE_GUARDED
        },

        { 

        /* L2 SRAM */
        
        MPC8540_L2_SRAM_BASE, 
        MPC8540_L2_SRAM_BASE,
        0,                            /* dynamically filled in sysHwInit() */    
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_DEFAULT
        },

        {
        
        /* Access to VME A32 space */
        
        VME_A32_MSTR_LOCAL,
        VME_A32_MSTR_LOCAL,
        VME_A32_MSTR_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
        MMU_ATTR_CACHE_GUARDED
        },

        {
        
        /* Access to VME A24 space */
        
        VME_A24_MSTR_LOCAL,
        VME_A24_MSTR_LOCAL,
        VME_A24_MSTR_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
        MMU_ATTR_CACHE_GUARDED
        },
        
        {
        
        /* Access to VME A16 space */
        
        VME_A16_MSTR_LOCAL,
        VME_A16_MSTR_LOCAL,
        VME_A16_MSTR_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
        MMU_ATTR_CACHE_GUARDED
        },
        
        {
        
        /* Access to PCI non-prefetchable memory space */
        
        PCI_MSTR_MEMIO_LOCAL,
        PCI_MSTR_MEMIO_LOCAL,
        PCI_MSTR_MEMIO_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
        MMU_ATTR_CACHE_GUARDED
        },
        
        {
        
        /* Access to PCI prefetchable memory space */
        
        PCI_MSTR_MEM_LOCAL,
        PCI_MSTR_MEM_LOCAL,
        PCI_MSTR_MEM_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF
        },
        
        {
        
        /* Access to PCI 16-bit ISA I/O space */
        
        ISA_MSTR_IO_LOCAL,
        ISA_MSTR_IO_LOCAL,
        ISA_MSTR_IO_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
        MMU_ATTR_CACHE_GUARDED
        },
        
        {
        
        /* Access to PCI 32-bit I/O space */
        
        PCI_MSTR_IO_LOCAL,
        PCI_MSTR_IO_LOCAL,
        PCI_MSTR_IO_SIZE,
        MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
        MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_CACHE_OFF |
        MMU_ATTR_CACHE_GUARDED
        }
    };

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

const UINT32 busFreq[] =
    {
    FREQ_33_MHZ, FREQ_66_MHZ, FREQ_100_MHZ, FREQ_133_MHZ
    };

PHB_WIN_STRUCT sysPhbCpuToPciWin [7] = { { 0, 0, 0 } };
PHB_WIN_STRUCT sysPhbPciToCpuWin [7] = { { 0, 0, 0 } };
int     sysValidPhbWindows = 0;    /* number of valid entries */
int     phbToPciWinCount = 0;      /* number of valid Phb to PCI windows */
int     pciToPhbWinCount = 0;      /* number of valid PCI to PHB windows */
int     sysBus      = 0;               /* system bus type (VME_BUS, etc) */
int     sysCpu      = CPU;             /* system CPU type */
char *  sysBootLine = BOOT_LINE_ADRS;  /* address of boot line */
char *  sysExcMsg   = EXC_MSG_ADRS;    /* catastrophic message area */
int     sysProcNum;                    /* processor number of this CPU */
BOOL    sysVmeEnable = FALSE;          /* by default no VME */

/*
 * PPC85XX Cache Support
 *
 * The following definitions are required by the PPC85XX Cache (L1) Support
 * libraries, and so therefore must be defined within the BSP.
 */

UINT32 ppcE500ICACHE_LINE_NUM = (128 * 12);
UINT32 ppcE500DCACHE_LINE_NUM = (128 * 12);
UINT32 ppcE500CACHE_ALIGN_SIZE = 32;

#ifdef INCLUDE_ATA

ATA_RESOURCE   ataResources[ATA_MAX_CTRLS];

/*
 *  sATA support is limited to legacy IDE mode only and uses the standard
 *  Wind River ATA driver and supporting data structures. DIP switch #7
 *  on the board configuration switch bank must be set to OFF (default) to
 *  place the sATA controller in legacy IDE mode. In legacy IDE mode the
 *  sATA controller operates as a typical dual IDE controller supporting
 *  master and slave drives on sATA ports 0 and 1 respectively for the 
 *  primary controller, and likewise on sATA ports 2 and 3 for the secondary
 *  controller. sATA port 3 (the secondary's slave) is not used on the MV3100.
 *
 *  The first member in the ATA_TYPE struct has a dual purpose.
 *  1) If cylinders == 0, the device location is not probed at startup.
 *  2) If cylinders != 0, the device location is probed, and if a device
 *     is found, the driver will fill in the first 3 members of the struct
 *     with the number of cylinders, number of heads, and sectors per track.
 *
 *  The last 2 members of the struct are static and should not be changed.
 *
 *  The ATA_TYPE struct has the following layout:
 *      int cylinders;
 *      int heads;
 *      int sectorsTrack;
 *      int bytesSector;
 *      int precomp;
 *
 *  Note:   If configType == ATA_GEO_FORCE, the user needs to fill in
 *          values for cylinders, heads, and sectorsTrack.
 */

ATA_TYPE    ataTypes [ATA_MAX_CTRLS][ATA_MAX_DRIVES] =
    {
        {
            { 1, 0, 0, 512, 0xff },   /* controller 0, drive 0 */
            { 0, 0, 0, 512, 0xff }    /* controller 0, drive 1 */
        },
        {
            {0, 0, 0, 512, 0xff },  /* controller 1, drive 0 */
            {0, 0, 0, 512, 0xff }   /* controller 1, drive 1 (not available) */
        }
    };
#endif /* INCLUDE_ATA */

/* locals */

LOCAL char sysModelStr[80];

/* forward declarations */

#ifdef INCLUDE_SPE
    int  sysSpeProbe (void);
#endif /* INCLUDE_SPE */

char *  sysModel (void);
char *  sysBspRev (void);
UINT32  sysBaudClkFreq (void);
void    sysHwInit (void);
void    sysMpc8540StaticInit (void);
char *  sysPhysMemTop (void);
char *  sysMemTop (void);
UINT32  sysDramSize (void);
STATUS  sysToMonitor (int startType);
void    sysHwInit2 (void);
int     sysProcNumGet (void);
void    sysProcNumSet (int procNum);
STATUS  sysLocalToBusAdrs (int adrsSpace, char *    localAdrs, char ** pBusAdrs);
STATUS  sysBusToLocalAdrs (int adrsSpace, char *    busAdrs, char ** pLocalAdrs);
BOOL    sysBusTas (char * adrs);
#if (defined(INCLUDE_SM_NET) || defined(INCLUDE_SM_OBJ) ||    \
     defined(INCLUDE_TIPC_MEDIA_SM))
     LOCAL void sysBusTasClear (char *);
#endif
LOCAL BOOL  sysVmeBusLockTas (char *);
LOCAL BOOL  sysVmeRmwTas (char *);
LOCAL BOOL  sysVmeRmw (char *);
UCHAR   sysNvRead (ULONG, UINT32);
void    sysNvWrite (ULONG, UCHAR, UINT32);
void    sysDebugMsg (char *, UINT32);
void    sysFlashSwWriteProtectSet (BOOL setOn);
BOOL    sysFlashIsHwWriteProtected (void); 

#ifdef INCLUDE_MOTFCCEND
    STATUS sysFccEnetAddrGet (int unit, UCHAR * addr);
#endif

LOCAL   void sysPhbCapt (void);
STATUS  sysPciSpecialCycle (int busNo, UINT32 message);
STATUS  sysPciConfigRead   (int busNo, int deviceNo, int funcNo,
                            int offset, int width, void * pData);
STATUS  sysPciConfigWrite  (int busNo, int deviceNo, int funcNo,
                            int offset, int width, ULONG data);
void    sysUsDelay (UINT32 delay); 
void    sysDelay (void);

#ifdef  INCLUDE_CACHE_SUPPORT
    LOCAL void sysL1CacheInit (void);
#endif
void    sysSramMmuInit (void);
void    sysApplInit (void);
UINT32  getFlashBaseAddress (void);
void    necPd720101ExtConfig (void);

/* externals */

IMPORT VOIDFUNCPTR  smUtilTasClearRtn;
IMPORT void   mmuE500TlbDynamicInvalidate();
IMPORT void   mmuE500TlbStaticInvalidate();
IMPORT void   mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, 
                                    BOOL cacheAllow);
IMPORT void   sysMpc8540PhbInit (void);

IMPORT void   sysTempeReset (void);
IMPORT void   sysTempeInit (void);
IMPORT STATUS sysVmeToPciAdrs (int, char *, char **);
IMPORT STATUS sysPciToVmeAdrs (int, char *, char **);

IMPORT STATUS sysMotI2cRead  (UINT32 devAddr, UINT32 devOffset,
                              UINT32 devAddrBytes, UCHAR *spdData,
                              UINT32 numBytes);
IMPORT STATUS sysMotI2cWrite (UINT32 devAddr, UINT32 devOffset, 
                              UINT32 devAddrBytes, UINT8  *pBfr, 
                              UINT32 numBytes);
IMPORT UCHAR  sysProductStr[];
IMPORT UCHAR  sysCpuStr[];
IMPORT BOOL   isMappedToVME (char *, char **, UINT32 *);
IMPORT STATUS sysVMEProbe (char *, int, int, char *);

IMPORT void   hardWareInterFaceInit(void);
IMPORT void   sysSerialConnectAll(void);

#ifdef INCLUDE_SPE
    IMPORT int  (* _func_speProbeRtn) () ;
#endif /* INCLUDE_SPE */

#if ((defined INCLUDE_RTC) && (defined INCLUDE_DOSFS))
    IMPORT void dosFsDateTimeInstall(FUNCPTR);
#endif /* INCLUDE_RTC, INCLUDE_DOSFS */

#ifdef INCLUDE_TFFS
    IMPORT void sysTffsMountDevs (void);
#endif /* INCLUDE_TFFS */

/* BSP DRIVERS */

#include "sysMpc8540Init.c"
#include "sysEpic.c"
#ifdef INCLUDE_L2_CACHE_SRAM 
#   include "sysL2Cache.c"        /* L2 Cache/SRAM Support */
#endif /* INCLUDE_L2_CACHE_SRAM */
#include "sysMpc8540Phb.c"        /* MPC8540 PCI/PCI-X Bus Interface */
#include "pci/pciConfigLib.c"     /* PCI Support */
#include "pci/pciIntLib.c"        /* PCI Support */
#include "pciAutoConfigLib.c"     /* automatic PCI configuration */
#include "sysBusPci.c"            /* pciAutoConfig BSP support file */
#include "sysTempe.c"
#include "mem/byteNvRam.c"        /* NVRAM Support */
#include "sysMotVpd.c"            /* VPD Support */

#ifdef INCLUDE_MPC8540_DMA
#   include "sysMpc8540Dma.c"     /* MPC8540 DMA */
#endif /* INCLUDE_MPC8540_DMA */

#ifdef INCLUDE_PERFMON
#   include "sysMpc8540PerfMonEvent.c"  /* Performance Monitor */
#   include "sysMpc8540PerfMon.c"    
#endif /* INCLUDE_PERFMON */

#ifdef INCLUDE_DS1621
#   include "sysDs1621TempSensor.c"     /* DS1621 Temp Sensor Support */
#endif /* INCLUDE_DS1621 */

#ifdef INCLUDE_RTC
#   include "sysDs1375.c"               /* Maxim DS1375 RTC with Alarm */
#endif /* INCLUDE_RTC */

#ifdef INCLUDE_SHOW_ROUTINE
#   if defined(INCLUDE_PCI_CFGSHOW)
#       include "pciCfgShow.c"
#       if !defined(PRJ_BUILD)
#           include "pci/pciConfigShow.c"       /* pci configuration show routines */
#       endif /* !defined(PRJ_BUILD) */
#   endif /* INCLUDE_PCI_CFGSHOW */
#   include "sysMpc8540Show.c"
#   include "sysMotVpdShow.c"           /* VPD Show Routines */
#   include "sysStatusRegsShow.c"
#endif /* INCLUDE_SHOW_ROUTINES */


#if ((defined INCLUDE_TFFS)  || (defined INCLUDE_BOOT_IMAGE_PROGRAM))
#   include "sysAmdS29GLxxxN.c"
#endif /* INCLUDE_TFFS, INCLUDE_BOOT_IMAGE_PROGRAM */

#ifdef INCLUDE_BOOT_IMAGE_PROGRAM
#   include "sysFlashUtil.c"            /* FLASH driver interface support */
#   include "sysProgramBootImage.c"     /* Boot Image Programming support */
#endif /* INCLUDE_BOOT_IMAGE_PROGRAM */

#ifdef  INCLUDE_MOTFCCEND
#   include "sysMotFcc2End.c"
#endif /* INCLUDE_MOTFCCEND */

#ifdef INCLUDE_ERROR_HANDLING
#   include "sysErrorHandler.c"
#   include "sysMpc8540Error.c"
#   include "sysMpc8540EcmError.c"
#   include "sysMpc8540LbcError.c"
#   include "sysMpc8540PciError.c"
#   include "sysMpc8540SmcError.c"
#endif

#ifdef INCLUDE_ATA
#   include "sysAta.c"
#endif /* INCLUDE_ATA */

#ifdef INCLUDE_WDOG_TIMER 
#   include "sysMpc8540Wdog.c"
#endif /* INCLUDE_WDOG_TIMER */

#ifdef INCLUDE_SMEND
#   include "sysSmEnd.c"
#endif /* INCLUDE_SMEND */

#ifdef INCLUDE_TICK_TIMERS
#   include "sysMpc8540Tmr.c"
#endif /* INCLUDE_TICK_TIMERS */

/*******************************************************************************
*
* sysApplInit - routines to be called upon kernel initialization
*
* This routine is used to call user applications during kernel
* initialization.
*
* RETURNS: NA
*/

void sysApplInit (void)
    {
#if (defined INCLUDE_TFFS)
#   if (defined INCLUDE_BOOT_IMAGE_PROGRAM)
        sysTffsMountDevs();
        sysFlashDevsMount();
#   else
        sysTffsMountDevs();
#   endif /* INCLUDE_BOOT_IMAGE_PROGRAM */
#else
#   if (defined INCLUDE_BOOT_IMAGE_PROGRAM)
        sysFlashDevsMount();
#   endif
#endif /* INCLUDE_TFFS */
    }

/*******************************************************************************
*
* getFlashBaseAddress - return the base address of the flash bank
*
* This routine will return the base address of the flash bank.
*
* RETURNS: base address of flash bank
*/

UINT32 getFlashBaseAddress (void)
    {
    return(CCSR_READ32 (CCSBAR, CCSR_BR0) & 0xFFFF8000);
    }

/*******************************************************************************
*
* sysFlashMappingConfig - modify flash bank mapping
*
* This routine modifies the flash entry in sysPhysMemDesc and also
* adjusts the programming of the local bus controller to match the
* actual size of the flash bank.
*
* RETURNS: NA
*/

void sysFlashMappingConfig (void)
    {
    UINT   i;
    VPD_PACKET ** pVpdPkts;     /* board vpd packets */
    UCHAR type;                 /* board vpd packet type */
    UINT32 flashSize;
    UINT32 flashBaseAddress;
    UINT32 br0;
    UINT32 or0;

    /*
     * if the first entry in the vpd packet pointer array is null, then
     * the vpd data needs to be read into memory and parsed.
     */

    if (sysVpdPkts[0] == NULL)
        {
        printf ("Initialize VPD packets.\n\r");
        if (sysMotVpdInit() != OK )
            {
            printf ("VPD structure invalid.\n");
        return;
            }
        }

    /* Initialize the vpd packet pointer and type to the first packet. */

    pVpdPkts = (VPD_PACKET **)&sysVpdPkts[0];
    type = (*pVpdPkts)->type;

    /* Find the VPD packet for the FLASH memory */

    while (type != VPD_PID_TERM && type != VPD_PID_FMC)
        {
        pVpdPkts++;
        type = (*pVpdPkts)->type;
        }

    if (type != VPD_PID_FMC)
        {
        printf ("Cannot read the FLASH memory VPD.\n\r");
        return;
        }

    flashSize = (1 << ((*pVpdPkts)->data[VPD_FMC_SIZE] + 18));
    flashBaseAddress = (0xFFFFFFFF - flashSize) + 1;

    for (i = 0; i < (sizeof(sysPhysMemDesc)/sizeof(PHYS_MEM_DESC)); i++)
    {
    if (sysPhysMemDesc[i].virtualAddr == FLASH_BOOT_ADRS)
        {
        sysPhysMemDesc[i].physicalAddr = flashBaseAddress;
        sysPhysMemDesc[i].virtualAddr = flashBaseAddress;
        sysPhysMemDesc[i].len = flashSize;
        break;
        }
    }

    /* local bus controller */

    br0 = CCSR_READ32 (CCSBAR, CCSR_BR0);
    br0 &= 0x00007FFF;
    br0 |= flashBaseAddress;
    CCSR_WRITE32 (CCSBAR, CCSR_BR0, br0);

    or0 = CCSR_READ32 (CCSBAR, CCSR_OR0);
    or0 &= 0x00007FFF;
    or0 |= (CCSR_ORn_AM_VAL(flashSize));
    CCSR_WRITE32 (CCSBAR, CCSR_OR0, or0); 
    }

/*******************************************************************************
*
* sysSramMmuInit - Modify SRAM entry in sysPhysMemDesc
*
* This routine modifies the L2 SRAM entry in sysPhysMemDesc to match
* the configuration that the user has chosen.
*
* RETURNS: NA
*/

void sysSramMmuInit (void)
    {
#if ((L2_CACHE_SRAM_CONFIG == L2_CACHE_SRAM_CONFIG_3) || \
     (L2_CACHE_SRAM_CONFIG == L2_CACHE_SRAM_CONFIG_6))
    UINT   i;

    for (i = 0; i < (sizeof(sysPhysMemDesc)/sizeof(PHYS_MEM_DESC)); i++)
        {
        if (sysPhysMemDesc[i].virtualAddr == (VIRT_ADDR) MPC8540_L2_SRAM_BASE)
            {
            sysPhysMemDesc[i].len = 0x20000;
            break;
            }
        }
#elif ((L2_CACHE_SRAM_CONFIG == L2_CACHE_SRAM_CONFIG_4) || \
       (L2_CACHE_SRAM_CONFIG == L2_CACHE_SRAM_CONFIG_5))
    UINT   i;

    for (i = 0; i < (sizeof(sysPhysMemDesc)/sizeof(PHYS_MEM_DESC)); i++)
        {
        if (sysPhysMemDesc[i].virtualAddr == (VIRT_ADDR) MPC8540_L2_SRAM_BASE)
            {
            sysPhysMemDesc[i].len = 0x40000;
            break;
            }
        }
#endif
    }

#ifdef INCLUDE_SPE
/*******************************************************************************
*
* sysSpeProbe - Check if the CPU has SPE unit.
*
* This routine returns OK if the CPU has an SPE unit in it.
* Since the SPE is a key feature of the e500 core in the MPC8540 this
* function will always return OK for a PowerQUICC III devices. SPE units are
* not expected to be part of future PowerQUICC generations.
*
* RETURNS: OK 
* 
*/

STATUS sysSpeProbe (void)
    {
    ULONG  regVal;
    STATUS speUnitPresent = OK;

    /* The CPU type is indicated in the Processor Version Register (PVR) */
 
    regVal = CCSR_READ32 (CCSBAR, CCSR_SVR);

    /* Mask off processor version and shift to lower word. */

    regVal &= 0xFFFF0000;

    if ((regVal >> 16) != CPU_TYPE_MPC8540)
        {
        speUnitPresent = ERROR;
        }

    return (speUnitPresent);
    }

#endif  /* INCLUDE_SPE */

/******************************************************************************
*
* isPciAdrs - determine if local address is a PCI address.
*
* This routine determines if a given address maps to the PCI interface
* through a host bridge PCI mapping.
*
* RETURNS: TRUE if address is mapped to PCI interface, FALSE if not.
*/

BOOL isPciAdrs
    (
    char * adrs,        /* Local address to examine to see if it's PCI */
    char ** pciTrans    /* If it's PCI this will be the PCI bus address */
    )
    {
    int i;
    BOOL stat = FALSE;

    for (i = 0; i < phbToPciWinCount; i++)
        {
        if (((UINT32)adrs >= sysPhbCpuToPciWin[i].winBase) &&
            ((UINT32)adrs <=  sysPhbCpuToPciWin[i].winLimit) )
            {
            stat = TRUE;
            *(UINT32 *)(pciTrans) = sysPhbPciToCpuWin[i].winBase +
                                  ((UINT32)adrs - sysPhbCpuToPciWin[i].winBase);
            break;
            }
        }
    return (stat);
    }

/******************************************************************************
*
* isVmeAdrs - determine if local address is a VME address.
*
* This routine determines if a given address maps to the VME interface
* through a Tempe outbound window.
*
* RETURNS: TRUE if address is mapped to VME interface, FALSE if not.
* If return value is TRUE, the VME bus translated address and the address
* of the associated Outbound control register is returned.
*/

BOOL isVmeAdrs
    (
    char   * adrs,      /* Local address to examine to see if it's VME */
    char  ** vmeTrans,  /* If it's VME, this will be the VME bus address */
    UINT32 * outRegSet  /* Number of outbound register set */
    )
    {
    BOOL stat = FALSE;
    char * pciTrans;

    /*
     * First determine if it's a PCI address since on this board,
     * any VME address must first be a PCI address.  If it is not
     * a PCI address we can return FALSE right away.
     */

    if (!isPciAdrs(adrs, &pciTrans))
        return FALSE;

    /* Now check the legitimate PCI address to see if it maps to VME */

    stat = isMappedToVME (adrs, vmeTrans, outRegSet);

    return (stat);
    }

/******************************************************************************
*
* sysProbeErrClr - clear errors associated with probing an address on a bus.
*
* This routine clears the error flags and conditions in the Host Bridge and 
* PowerPC registers arising from probing addresses.
*
* RETURNS: N/A
*/

void sysProbeErrClr (void)
    {
    UINT16 pciStatus = 0;

    /* Clear relevant host bridge error registers */

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DR, 0xFFFFFFFF);

    sysPciConfigRead (PCI_HOST_BRIDGE_BUS_NUM,
                      PCI_HOST_BRIDGE_DEV_NUM,
                      PCI_HOST_BRIDGE_FUNC_NUM,
                      0x06,2,&pciStatus);
    pciStatus |= 0xF900;
    sysPciConfigWrite (PCI_HOST_BRIDGE_BUS_NUM,
                       PCI_HOST_BRIDGE_DEV_NUM,
                       PCI_HOST_BRIDGE_FUNC_NUM,
                       0x06,2,pciStatus);

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_ATTRIB, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_ADDR, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_EXT_ADDR, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DL, 0x0);
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DH, 0x0);

    /* Clear PowerPC `Data Storage' Exception Registers */

    vxDearSet (0);
    vxSrr0Set  (0);
    vxSrr1Set  (0);
    }

/******************************************************************************
*
* checkHbStat - check for master abort or target abort on host bridge.
*
* This routine checks the appropriate host bridge registers to determine
* if a master abort or target abort has occurred since the last time these
* error status were cleared.
*
* RETURNS: ERROR if master or target abort is indicated, OK otherwise.
*/

LOCAL STATUS checkHbStat (void)
    {
    STATUS stat;
    UINT16 status;

    sysPciConfigRead (PCI_HOST_BRIDGE_BUS_NUM,
                      PCI_HOST_BRIDGE_DEV_NUM,
                      PCI_HOST_BRIDGE_FUNC_NUM,
                      0x06,2,&status);

    if ((status & (PCI_STATUS_TARGET_ABORT_RCV | 
           PCI_STATUS_MASTER_ABORT_RCV)) == 0)
        {
        stat = OK;
        }
    else
        {
        stat = ERROR;
        }

    return (stat);
    }

/******************************************************************************
*
* sysPciProbe - probe a PCI bus address
*
* This routine probes an address on the PCI bus. All probing is done with
* interrupts disabled.
*
* RETURNS: OK or ERROR if address cannot be probed
*/

STATUS sysPciProbe
    (
    char   * adrs,    /* address to be probed */
    int      mode,    /* VX_READ or VX_WRITE */
    int      length,  /* 1, 2 or 4 byte probe */
    char   * pVal     /* address of value to write OR */
                      /* address of location to place value read */
    )
    {
    STATUS status = ERROR;
    int    oldLevel;
    char   reg8;

    /* probe performed with interrupts disabled */

    oldLevel = intLock ();

    /* flush PCI posted write buffer(s) */

    if ((status = vxMemArchProbe (adrs, mode, length, pVal)) == OK )
        {
        if (mode == VX_WRITE)
            {
            reg8 = *adrs;               /* flush the write */
            }
        status = checkHbStat();
        }

    /* safe to re-enable interrupts */

    intUnlock (oldLevel);

    return (status);
    }

/******************************************************************************
*
* sysBusProbe - probe a bus address based on bus type.
*
* This routine is a function hook into vxMemProbe.  It determines which bus,
* PCI, VME or local is being probed based on the address to be probed.
* If the PCI bus is being probed, the sysPciProbe() routine is called to do the
* special PCI probing. If the local bus is being probed, the routine calls an
* architecture-specific probe routine. If the VME bus is being probed, the
* sysVMEProbe() routine is called to do the special VME probing.
*
* RETURNS: ERROR if the probed address does not respond or causes a MMU fault.
* Returns OK if the probed address responds.
*/

STATUS  sysBusProbe
    (
    char   * adrs,    /* address to be probed */
    int      mode,    /* VX_READ or VX_WRITE */
    int      length,    /* 1, 2 or 4 byte probe */
    char   * pVal    /* address of value to write OR */
            /* address of location to place value read */
    )
    {
    STATUS stat;
    UINT32 outRegSet;
    char   * pciTrans;
    char   * vmeTrans;
    int    key;

    key = intLock ();   /* Perform probe with interrupts locked */

    /* Clear any existing errors/exceptions */

    sysProbeErrClr ();

    /* If the address is a DRAM address ... */

    if (((adrs == LOCAL_MEM_LOCAL_ADRS) ||
         (adrs >  LOCAL_MEM_LOCAL_ADRS)) &&
         (adrs < sysPhysMemTop()))
        stat = vxMemArchProbe (adrs, mode, length, pVal);

    /* ... else if the address is a VME address */

    else if (isVmeAdrs(adrs, &vmeTrans, &outRegSet))
        stat = sysVMEProbe(adrs, mode, length, pVal);

    /* ... else if the address is a PCI address */

    else if (isPciAdrs(adrs, &pciTrans))
        stat = sysPciProbe (adrs, mode, length, pVal);

    /*
     * ... else, the address is not DRAM, VME, or PCI but it could be
     * something like host bridge register space, an address which is none
     * of the above but it is still mapped and will respond to a probe.
     * If that's the case then vxMemArchProbe() should tell us so.
     */

    else
        stat = vxMemArchProbe (adrs, mode, length, pVal);

    /* Clear any errors/exceptions before exiting */

    sysProbeErrClr ();

    intUnlock (key);

    return (stat);
    }

/******************************************************************************
*
* sysNvRead - read one byte from NVRAM
*
* This routine reads a single byte from a specified offset in NVRAM.
* The "NVRAM" on this board is implemented via an EEPROM.
*
* RETURNS: The byte from the specified NVRAM offset.
*/

UCHAR sysNvRead
    (
    ULONG    offset,    /* NVRAM offset to read the byte from */
    UINT32   i2cAddr    /* I2C address of EEPROM */
    )
    {
    UCHAR data = 0x0;

    sysMotI2cRead (i2cAddr, offset, 2, &data, 1);

    return (data);
    }

/******************************************************************************
*
* sysNvWrite - write one byte to NVRAM
*
* This routine writes a single byte to a specified offset in NVRAM.
* The "NVRAM" on this board is implemented via an EEPROM.
*
* RETURNS: N/A
*/

void sysNvWrite
    (
    ULONG    offset, /* NVRAM offset to write the byte to */
    UCHAR    data,   /* datum byte */
    UINT32   i2cAddr /* I2C Address of EEPROM */
    )
    {
    UCHAR writeData = data;

    sysMotI2cWrite (i2cAddr, offset, 2, &writeData, 1);
    }

/******************************************************************************
*
* sysDebugMsg - print a debug string to the console in polled mode.
*
* This routine prints a message to the system console in polled mode and
* optionally exits to the monitor.
*
* RETURNS: N/A
*
*/

void sysDebugMsg
    (
    char * str,
    UINT32  recovery
    )
    {
    int msgSize;
    int msgIx;
    SIO_CHAN * pSioChan;        /* serial I/O channel */

    msgSize = strlen (str);

    pSioChan = sysSerialChanGet (0);

    sioIoctl (pSioChan, SIO_MODE_SET, (void *) SIO_MODE_POLL);

    for (msgIx = 0; msgIx < msgSize; msgIx++)
        {
        while (sioPollOutput (pSioChan, str[msgIx]) == EAGAIN);
        }

    /* follow caller's error recovery policy. */

    if (recovery == EXIT_TO_SYSTEM_MONITOR)
        sysToMonitor (BOOT_NO_AUTOBOOT);
    }

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
    char cpuStr[80];

    device = CPU_TYPE;

    if ((device) == CPU_TYPE_MPC8540)
        {
        sprintf(cpuStr, "MPC8540");
        sprintf (sysModelStr, "Motorola %s - %s", sysProductStr, cpuStr);
        return (sysModelStr);
        }
    else
        {
        return ("Model Unknown");
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

/*******************************************************************************
*
* sysDramSize - returns the real top of local DRAM.
*
* RETURNS: The address of the top of DRAM.
*
*/

UINT32 sysDramSize (void)
    {
    UINT32 localDram = 0x00000000;
    UINT32 temp = 0x0;

    temp = CCSR_READ32(CCSBAR,CCSR_CS0_CONFIG);
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS0_BNDS);
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT) | 0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS1_CONFIG);
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS1_BNDS);
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT) | 0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS2_CONFIG);
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS2_BNDS);
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT) | 0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }

    temp = CCSR_READ32(CCSBAR,CCSR_CS3_CONFIG);
    if (temp && DDR_SDRAM_CSn_CONFIG_CS_EN_MASK)
        {
        temp = CCSR_READ32(CCSBAR,CCSR_CS3_BNDS);
        temp = ((((temp & DDR_SDRAM_CSn_BNDS_EA_MASK) <<
                 DDR_SDRAM_CSn_BNDS_EA_SHIFT) | 0x00FFFFFF) + 1);
        if (temp > localDram)
            localDram = temp;
        }
    return (localDram);
    }

/******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various features of the CPU board.  It is called
* by usrInit() in usrConfig.c.  This routine sets up the control registers
* and initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user application.  It
* cannot be used to initialize interrupt vectors.
* RETURNS: NA
*/

void sysHwInit (void)
    {
    UINT32  timeOut;
    UINT32  tempeCgr;
    int     pciBusNo;       /* PCI bus number */
    int     pciDevNo;       /* PCI device number */
    int     pciFuncNo;      /* PCI function number */
    UINT    x;

    vxHid0Set(HID0_MCP|vxHid0Get());

#ifdef INCLUDE_AUX_CLK
    sysAuxClkRateSet(127);
#endif

#ifdef INCLUDE_CACHE_SUPPORT

    /*
     * This routine initializes several variables which are used
     * by the PPC85XX cache (L1) support libraries.  This routine
     * does not initialize or enable the L1 cache, that is performed
     * by the cache support libraries.
     */

    sysL1CacheInit(); 
#endif /* INCLUDE_CACHE_SUPPORT */

    /* set pointer to bus probing hook */

    _func_vxMemProbeHook = (FUNCPTR)sysBusProbe;

    sysMpc8540StaticInit ();

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU 
     * enabled images 
     */

    mmuE500TlbDynamicInvalidate(); /* Invalidate TLB0 */
    mmuE500TlbStaticInvalidate();  /* Invalidate TLB1 */

    /* Build the static (TLB1) entries from sysStaticTlbDesc */

    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);

#if (defined(INCLUDE_L2_CACHE_SRAM) && defined(INCLUDE_CACHE_SUPPORT))

    /* Initialize the L2 Cache */

    sysL2CacheInit();
#endif /* INCLUDE_L2_CACHE_SRAM && INCLUDE_CACHE_SUPPORT*/

    /* Adjust SRAM entry in sysPhysMemDesc */

    sysSramMmuInit();

    /* Initialize the VPD information, VPD located on I2C bus */

    (void) sysMotVpdInit();

    /* 
     * Adjust flash entry in sysPhysMemDesc and adjust the 
     * flash bank programming in the local bus controller.
     */

    sysFlashMappingConfig();

    /* Initialize the PCI/PCI-X Bus Interface */

    sysMpc8540PhbInit ();

    /* Initialize the Embedded Programmable Interrupt Controller */

    sysEpicInit();

    CACHE_PIPE_FLUSH();

    /* capture Phb window configuration */

    sysPhbCapt ();

    /*
     * Configure PCI libraries.  Note that we need our own routines
     * (not default routines) because CAR access is big Endian.  Hence
     * the use of PCI_MECHANISM_0.
     */

    if (pciConfigLibInit (PCI_MECHANISM_0, (ULONG)sysPciConfigRead,
        (ULONG)sysPciConfigWrite, (ULONG)sysPciSpecialCycle) != OK)
        {
        sysToMonitor (BOOT_NO_AUTOBOOT);  /* BAIL */
        }

    /*  Initialize PCI interrupt library. */

    if ((pciIntLibInit ()) != OK)
        {
        sysToMonitor (BOOT_NO_AUTOBOOT);
        }

    /*
     * PrPMC Enumeration
     *
     * Poll ERDY2 and ERDY1 in the PCI Bus B Status Register 
     * to wait for any installed PrPMCs to signal that they are
     * ready for enumeration.
     */

    for (timeOut = 10; timeOut > 0; timeOut--)
        {
        if ((*(UCHAR *)(BRD_PCI_BUS_B_STAT_REG) & 
             BRD_PCI_BUS_B_STAT_REG_ERDY2_MASK) &&
            (*(UCHAR *)(BRD_PCI_BUS_B_STAT_REG) & 
             BRD_PCI_BUS_B_STAT_REG_ERDY1_MASK))
            break;
        sysUsDelay (1000000);
        }

    /* perform PCI configuration */

    /*
     * Disable TSi148 Outbound Mapping Windows
     *
     * When using the MotLoad firmware to boot vxWorks we must
     * disable the TSi148's outbound mapping windows before
     * reconfiguring the TSi148's register access BAR and attempting
     * to access the windows from this new address.  This is
     * necessary because MotLoad configures the TSi148's outbound
     * windows for a region of memory that overlaps the region of
     * memory where vxWorks wants to place the TSi148's register
     * set, which would result in overlapping windows and an
     * inability to access the registers at all.
     */

    if (pciFindDevice ((PCI_ID_TEMPE & 0xFFFF), (PCI_ID_TEMPE >> 16)
                       & 0xFFFF, 0, &pciBusNo, &pciDevNo, &pciFuncNo)
        != ERROR)
        {
        pciConfigInLong(pciBusNo, pciDevNo, pciFuncNo,
                        PCI_CFG_BASE_ADDRESS_0, &tempeCgr);
        tempeCgr &= PCI_MEMBASE_MASK;
        if (tempeCgr != 0x0)
            {
            for (x = 0; x < 8; x++)
                {

                /*
                 * Clear all 8 outbound translation attribute
                 * registers.  This will disable each of the
                 * outbound translation windows.
                 */

                TEMPE_WRITE32_PUSH(tempeCgr,(TEMPE_OTAT0 + (x*0x20)),
                                   0x00000000);
                }
            }
        }

    sysPciAutoConfig();

    /* Reset and Initialize the Tempe PCI-VME bridge chip */

    sysTempeReset ();
    sysTempeInit ();

#ifdef INCLUDE_USB
        necPd720101ExtConfig ();
#endif /* INCLUDE_USB */

#if FALSE
        sysIntConnect ( (VOIDFUNCPTR*)EPIC_PCI_INT_VEC, pciDeviceIntHandler);
#endif  /* FALSE */

#ifdef INCLUDE_ATA

        /* Perform global structure initialization required for ATA driver */

        if (pciFindDevice ((PCI_ID_SATA & 0xFFFF), (PCI_ID_SATA >> 16) & 0xFFFF,
                           0, &pciBusNo, &pciDevNo, &pciFuncNo) != ERROR)
            {
            sysAtaInit (pciBusNo, pciDevNo, pciFuncNo);
            }
#endif /* INCLUDE_ATA */

    hardWareInterFaceInit();


#if (MPC8540_BOARD_TYPE == MPC8540_BOARD_TYPE_SBC)

    /* Upon completion, clear BFL (Board Fail) LED */

    *(UCHAR *)(BRD_SYSTEM_STATUS_INDICATOR_REG) &= 
    ~BRD_SYSTEM_STATUS_INDICATOR_BRD_FAIL_MASK;
#endif
    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.  This routine also adjusts the
* DRAM entry in sysPhysMemDesc to be the actual address of the top of
* physical memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    LOCAL char * physTop = NULL;
    UINT32 localDram = 0;
    int i;

    if (physTop == NULL)
        {
        localDram = sysDramSize();
#ifdef LOCAL_MEM_AUTOSIZE
        physTop = (char *)localDram;
#else /* not LOCAL_MEM_AUTOSIZE, use defined constants. */
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
#endif /* LOCAL_MEM_AUTOSIZE */

        /* Adjust initial DRAM size to actual physical memory. */

        for (i = 0; i < (int)(sizeof(sysPhysMemDesc)/sizeof(PHYS_MEM_DESC)); 
             i++)
            {
            if (sysPhysMemDesc[i].virtualAddr == LOCAL_MEM_LOCAL_ADRS)
                {
                sysPhysMemDesc[i].len =
                    (ULONG)physTop - (ULONG)sysPhysMemDesc[i].physicalAddr;
                break;
                }
            }
        }

    return (physTop);
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

    return (memTop);
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
    int startType    /* parameter passed to ROM to tell it how to boot */
    )
    {
    FUNCPTR pRom = (FUNCPTR) (ROM_TEXT_ADRS + 4);    /* Warm reboot */

#ifdef INCLUDE_CACHE_SUPPORT
    cacheDisable(INSTRUCTION_CACHE);    /* Disable the L1 Instruction Cache */
    cacheDisable(DATA_CACHE);        /* Disable the L1 Data Cache */
#endif

    sysClkDisable();

#ifdef INCLUDE_AUX_CLK
    sysAuxClkDisable();
#endif

    vxMsrSet (0);                     

#if (MPC8540_BOARD_TYPE == MPC8540_BOARD_TYPE_SBC)

    /* Turn on Board Fail LED */

    *(UCHAR *)(BRD_SYSTEM_STATUS_INDICATOR_REG) |= 
    BRD_SYSTEM_STATUS_INDICATOR_BRD_FAIL_MASK;
#endif

    /* Reset PCI Autoconfig Flag */

    PCI_AUTOCONFIG_FLAG = FALSE;

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
* RETURNS: NA
*/

void sysHwInit2 (void)
    {
    vxbDevInit();
    /* intialize the EPIC interrupts */

    sysEpicIntrInit ();

#ifdef INCLUDE_ERROR_HANDLING

    /* Set pointer to error handling code */

    _func_errorHandlerInit = (ERROR_HANDLER_INIT) sysErrorHandlerInit;
    (void) sysErrorHandlerInit (0, NULL, NULL);

    /* Initialize MPC8540 Error Handling */

    sysMpc8540ErrorInit();
#endif /* INCLUDE_ERROR_HANDLING */

    /* Connect Tempe (PCI-VME bridge) interrupts */

    sysTempeIntInit ();

    /* Enable Tempe interrupts at the EPIC level */

    intEnable (TEMPE_INT_LVL);


#ifdef INCLUDE_SPE
    _func_speProbeRtn = sysSpeProbe;
#endif  /* INCLUDE_SPE */

    /* 2nd phase of Tempe chip init */

    sysTempeInit2 ();

#ifdef INCLUDE_ERROR_HANDLING

    /* Enable MPC8540 Error Handling */

    sysMpc8540ErrIntEnable ();
#endif

    /* Set function hook for special sysBusTasClear routine */

#if (defined(INCLUDE_SM_NET) || defined(INCLUDE_SM_OBJ) ||    \
     defined(INCLUDE_TIPC_MEDIA_SM))
    smUtilTasClearRtn = (VOIDFUNCPTR)sysBusTasClear;
#endif

    /* MPC8540 DMA */

#ifdef INCLUDE_MPC8540_DMA
    sysMpc8540DmaInit();
#endif /* INCLUDE_MPC8540_DMA */

#if ((defined INCLUDE_RTC) && (defined INCLUDE_DOSFS))
    dosFsDateTimeInstall((FUNCPTR)sysDs1375DateTimeHook);
#endif /* INCLUDE_RTC, INCLUDE_DOSFS */

#ifdef INCLUDE_PERFMON
    sysPerfMonInit ();
#endif /* INCLUDE_PERFMON */

#ifdef INCLUDE_TICK_TIMERS
    sysTickTmrInit();
#endif /* INCLUDE_TICK_TIMERS */

    sysSerialConnectAll();

    taskSpawn("tDevConn", 11, 0, 10000, vxbDevConnect, 0,1,2,3,4,5,6,7,8,9);
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
* RETURNS: NA
*
* SEE ALSO: sysProcNumGet()
*
*/

void sysProcNumSet
    (
    int     procNum            /* processor number */
    )
    {
    LOCAL int procNumSet = FALSE;

    sysProcNum = procNum;

    /*
     * Note that sysProcNumSet() is always called during bootrom
     * initialization from bootConfig.c, function bootLoad().  It is
     * only called from the kernel initialization if INCLUDE_NET_INIT
     * is #define'd.  That particular call is done indirectly through
     * the line "usrNetInit (BOOT_LINE_ADRS);" inside of the
     * usrConfig.c file, function usrRoot().  Note that this line of
     * code in usrConfig.c is NOT compiled if INCLUDE_NET_INIT is not
     * #define'd.  Since we must know the node (processor) number in
     * order to correctly configure the Tempe ITx slave decoders
     * (see commentary below), we must call the 2nd phase of Tempe
     * initialization code here.  If we don't have INCLUDE_NET_INIT
     * #define'd, this 2nd phase of Tempe initialization is only
     * done once (during bootrom initialization).  If INCLUDE_NET_INIT
     * is #defined, the 2nd phase of initialization will be done twice,
     * once during bootrom initialization and once during kernel
     * initialization.
     */

    if (procNumSet == FALSE)
        {

        /*
         * Set up the Tempe's CRG slave decoders.  We have to wait until
         * we know our processor number to do this since the decoder
         * address location is dependent on the processor number.
         */

        if (TEMPE_REG_BASE != 0)
            {
            sysTempeInit2 ();
            }
        }

    procNumSet = TRUE;
    }

/******************************************************************************
*
* sysPhbWinCapt - capture translation window information
*
* This routine captures the configuration of the host bridge PPC and PCI slave
* registers. This information is used to perform address translations from
* CPU to PCI addresses and vice versa.
*
* RETURNS: N/A
*
* SEE ALSO: sysBusToLocalAdrs(), sysLocalToBusAdrs(), sysCpuToPciAdrs(),
* sysPciToCpuAdrs(), and sysPhbTransAdrs().
*/

LOCAL void sysPhbCapt (void)
    {
    PHB_WIN_STRUCT * pPhbCpu; /* pointer to cpu windows */
    PHB_WIN_STRUCT * pPhbPci; /* pointer to pci windows */
    UINT32 size;
    UINT32 powar;
    UINT32 powbar;
    UINT32 potar;
    UINT32 wtt;
    int x;

    /* initialize number of valid windows found */

    sysValidPhbWindows = 0;
    phbToPciWinCount = 0;
    pciToPhbWinCount = 0;

    /* point to window save arrays */

    pPhbCpu = &sysPhbCpuToPciWin[0];
    pPhbPci = &sysPhbPciToCpuWin[0];

    /*
     * Outbound Windows
     *
     * Window 0 is ignored since it is the default mapping that
     * covers the entire memory map.  We need to know if there is
     * another window for which the given address falls into.
     */

    for (x = 0; x < 4; x++)
        {
        powar = CCSR_READ32(CCSBAR, (CCSR_POWAR1 + (x * 0x20)));
        powbar = CCSR_READ32(CCSBAR, (CCSR_POWBAR1 + (x * 0x20)));
        potar = CCSR_READ32(CCSBAR, (CCSR_POTAR1 + (x * 0x20)));
        
        if (CCSR_POWAR_EN_VAL(powar) != 0)
            {
            sysValidPhbWindows++;
            size = MPC8540_PCI_SIZE_DECODE(CCSR_POWAR_OWS_VAL(powar));
            if (size != 0)    
            {
            wtt = CCSR_POWAR_WTT_VAL(powar);
            pPhbCpu->winType =
                ((wtt == 0x8)?PCI_BAR_SPACE_IO:PCI_BAR_SPACE_MEM);
            pPhbCpu->winBase = CCSR_PAR_ADDR_DECODE(powbar);
            pPhbCpu->winLimit = CCSR_PAR_ADDR_DECODE(powbar) + size - 1;
        
            pPhbPci->winType = pPhbCpu->winType;
            pPhbPci->winBase = CCSR_PAR_ADDR_DECODE(potar);
            pPhbPci->winLimit = CCSR_PAR_ADDR_DECODE(potar) + size - 1;
            pPhbCpu++;
            pPhbPci++;
            phbToPciWinCount++;
            }
            } 
        } 

    /* Inbound Windows */

    for (x = 0; x < 3; x++)
        {
        powar = CCSR_READ32(CCSBAR, (CCSR_PIWAR3 + (x * 0x20)));
        powbar = CCSR_READ32(CCSBAR, (CCSR_PIWBAR3 + (x * 0x20)));
        potar = CCSR_READ32(CCSBAR, (CCSR_PITAR3 + (x * 0x20)));
        
        if (CCSR_POWAR_EN_VAL(powar) != 0)
            {
            sysValidPhbWindows++;
            size = MPC8540_PCI_SIZE_DECODE(CCSR_POWAR_OWS_VAL(powar));
            if (size != 0)    
            {
            pPhbPci->winType = 0;
            pPhbPci->winBase = CCSR_PAR_ADDR_DECODE(powbar);
            pPhbPci->winLimit = CCSR_PAR_ADDR_DECODE(powbar) + size - 1;
        
            pPhbCpu->winType = pPhbPci->winType;
            pPhbCpu->winBase = CCSR_PAR_ADDR_DECODE(potar);
            pPhbCpu->winLimit = CCSR_PAR_ADDR_DECODE(potar) + size - 1;
        
            pPhbCpu++;
            pPhbPci++;
            pciToPhbWinCount++;
            }
            } 
        } 
    }

/*****************************************************************************
*
* translationWindowShow - Display PHB window translation values
*
* This function displays the translation values which are used in the
* sysBusToLocalAdrs() and sysLocalToBusAdrs() routines.
*
* RETURNS: N/A
*/

void translationWindowShow ()
    {
    int i;

    printf("           sysPhbCpuToPciWin       sysPhbPciToCpuWin\n\n");

    for (i=0; i< sysValidPhbWindows; i++)
        {
        printf("Window %d\n",i);
        printf("  winType    0x%08x              0x%08x\n",
               sysPhbCpuToPciWin[i].winType, sysPhbPciToCpuWin[i].winType);
        printf("  winBase    0x%08x              0x%08x\n",
               sysPhbCpuToPciWin[i].winBase, sysPhbPciToCpuWin[i].winBase);
        printf("  winLimit   0x%08x              0x%08x\n",
               sysPhbCpuToPciWin[i].winLimit, sysPhbPciToCpuWin[i].winLimit);
        printf("\n");
        }
    }

/******************************************************************************
*
* sysPhbTransAdrs - translate an address that passes through the Phb.
*
* This routine converts an address from a cpu to pci address or vice versa. It
* uses a pair of window arrays built during sysHwInit2 to guide the
* translation. The adrs parameter is the address to convert.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
* SEE ALSO: sysPhbCapt(), sysPciToCpuAdrs(), sysBusToLocalAdrs(),
* sysLocalToBusAdrs(),  and sysPhbTransAdrs().
*
*/

LOCAL STATUS sysPhbTransAdrs
    (
    UINT32            adrsSpace,   /* address space (memory or i/o ) */
    UINT32            adrs,        /* known address */
    UINT32 *          pTransAdrs,  /* pointer to the translated address */
    UINT32            winCnt,      /* number of open windows */
    PHB_WIN_STRUCT * pSrc,         /* pointer to the source windows */
    PHB_WIN_STRUCT * pDest         /* pointer to the destination windows */
    )
    {

    while (winCnt--)
        {

        /* check for a match on window type and in bounds */

        if ( (pSrc->winType == adrsSpace) &&
             (adrs >= pSrc->winBase) &&
             (adrs <= pSrc->winLimit) )
            {
            *pTransAdrs = ( adrs - pSrc->winBase + pDest->winBase );

            return (OK);
            };

        /* advance to next window */

        pSrc++;
        pDest++;
        }

    /*
     * no window was found to contain adrs. indicate that translation was
     * not possible.
     */

    return (ERROR);

    }

/******************************************************************************
*
* sysCpuToPciAdrs - translate a cpu address to a pci bus address
*
* This routine converts an address as seen from the cpu to the equivalent pci
* address, if it exists. The input address is the address as seen by the cpu.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
*
* SEE ALSO: sysPciToCpuAdrs(), sysBusToLocalAdrs(), sysLocalToBusAdrs()
* sysPhbCapt(), and sysPhbTransAdrs().
*/

LOCAL STATUS sysCpuToPciAdrs
    (
    int     adrsSpace,    /* bus address space where busAdrs resides */
    char *  localAdrs,    /* local address to convert */
    char ** pBusAdrs      /* where to return bus address */
    )
    {
    return (sysPhbTransAdrs (adrsSpace, (UINT32)localAdrs,
                             (UINT32 *)pBusAdrs, sysValidPhbWindows,
                             &sysPhbCpuToPciWin[0],
                             &sysPhbPciToCpuWin[0]) );
    }

/******************************************************************************
*
* sysPciToCpuAdrs - translate a pci bus address to a cpu address
*
* This routine converts an address as seen from the pci bus to the equivalent
* cpu address, if it exists. The input address is the address as seen by the
* pci bus.
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
* SEE ALSO: sysCpuToPciAdrs(), sysBusToLocalAdrs(), sysLocalToBusAdrs()
* sysPhbCapt(), and sysPhbTransAdrs().
*/

LOCAL STATUS sysPciToCpuAdrs
    (
    int     adrsSpace,    /* bus address space where busAdrs resides */
    char *  localAdrs,    /* local address to convert */
    char ** pBusAdrs      /* where to return bus address */
    )
    {
    return (sysPhbTransAdrs (adrsSpace, (UINT32)localAdrs,
                             (UINT32 *)pBusAdrs, sysValidPhbWindows,
                             &sysPhbPciToCpuWin[0],
                             &sysPhbCpuToPciWin[0]) );
    }

/******************************************************************************
*
* sysLocalToBusAdrs - convert a local address to a bus address
*
* Given a CPU address, this routine returns a corresponding local PCI
* bus address provided that such an address exists.  The target PCI
* bus is selected by the adrsSpace parameter.  Legal values for this
* parameter are found in "pci.h".
*
* RETURNS: OK, or ERROR if the address space is unknown or the mapping is not
* possible.
*
* SEE ALSO: sysBusToLocalAdrs()
*/
 
STATUS sysLocalToBusAdrs
    (
    int     adrsSpace,    /* bus address space where busAdrs resides */
    char *    localAdrs,    /* local address to convert */ 
    char **    pBusAdrs    /* where to return bus address */ 
    )
    {
    char * pciBusAdrs;

    if (IS_VME_ADDR_MOD(adrsSpace)) /* Is it a VME address? ... */
        {
        if (sysLocalToBusAdrs (PCI_SPACE_MEM_PRI, localAdrs,
                               &pciBusAdrs) == ERROR)
            return (ERROR);
        else
            return (sysPciToVmeAdrs (adrsSpace, pciBusAdrs, pBusAdrs));
        }

    switch (adrsSpace)
        {

        case PCI_SPACE_IO_PRI:

            /* convert from cpu address space to local pci space */

            if (sysCpuToPciAdrs (PCI_BAR_SPACE_IO, localAdrs, pBusAdrs) != OK )
                return (ERROR);

            break;

        case PCI_SPACE_MEMIO_PRI:
        case PCI_SPACE_MEM_PRI:

            /* convert from cpu address space to local pci address space */

            if (sysCpuToPciAdrs (PCI_BAR_SPACE_MEM, localAdrs, pBusAdrs) != OK )
                return (ERROR);

            break;

        default:
            return (ERROR);
        }
    return (OK);
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
    int      adrsSpace,     /* bus address space where busAdrs resides */
    char *    busAdrs,       /* bus address to convert */
    char **    pLocalAdrs     /* where to return local address */
    )
    {
    char * pciBusAdrs;

    if (IS_VME_ADDR_MOD(adrsSpace)) /* Is it a VME address? ... */
        {
        if (sysVmeToPciAdrs (adrsSpace, busAdrs, &pciBusAdrs) == OK)
            return (sysBusToLocalAdrs(PCI_SPACE_MEMIO_PRI, pciBusAdrs,
                    pLocalAdrs));
        else
            return (ERROR);
        }

    switch (adrsSpace)
        {

        case PCI_SPACE_IO_PRI:

            /*
             * translate from local PCI address space to CPU address
             * space.
             */

            return ( sysPciToCpuAdrs (PCI_BAR_SPACE_IO, busAdrs, pLocalAdrs) );

        case PCI_SPACE_MEMIO_PRI:
        case PCI_SPACE_MEM_PRI:

            /*
             * translate from local PCI address space to CPU address
             * space.
             */

            return (sysPciToCpuAdrs (PCI_BAR_SPACE_MEM, busAdrs, pLocalAdrs) );

        default:
            return (ERROR);
        }
    }

/*******************************************************************************
*
* sysBusTas - test and set a location across the VME bus
*
* This is a required VxWorks function which supports a test and set across the
* VME bus, a necessary component of shared-memory operation.  MVME3100s
* which have Rev 1 of the Tempe (Tsi148) chip on them have only one viable way
* of performing the busTas and that is through generation of a RMW cycle
* across the VME bus.  MVME3100s with Rev 2 or later chips will be able
* to be configured to use bus locking as part of the busTas function.
*
* RETURNS: TRUE if the value had not been set but is now, or
* FALSE if the value was set already.
*/

BOOL sysBusTas
    (
    char * adrs      /* address to be tested and set */
    )
    {
    BOOL state = FALSE;
    UINT32 vmeTasHwAssist = VME_TAS_HW_ASSIST;

    if ((vmeTasHwAssist - VME_BUS_RMW) == 0)
        {
        state = sysVmeRmwTas(adrs);
        }
    else if ((vmeTasHwAssist - VME_BUS_LOCK) == 0)
        {
        state = sysVmeBusLockTas(adrs);
        }

    return (state);
    }

/*******************************************************************************
*
* sysVmeBusLockTas - test and set a location across the VMEbus with bus locking
*
* This routine performs a test-and-set (TAS) instruction on the specified
* address.  Atomicity is enforced by locking processor interrupts during
* the operation (which keeps other processor resident agents from
* simultaneously affecting the semaphore) and by locking the VME bus (which
* keeps another VME agent from simultaneously updating the semaphore).
*
* RETURNS: TRUE if the value had not been set but is now.
* FALSE if the VMEbus cannot be locked or the value was already set.
*
* SEE ALSO: sysBusTasClear
*/

LOCAL BOOL sysVmeBusLockTas
    (
    char * adrs          /* address to be tested and set */
    )
    {
    BOOL state = FALSE;  /* semaphore state */
    int  lockKey;        /* interrupt lock key */
    UINT32 lockValue;
    volatile UINT32 * lockAdrs = (UINT32 *)adrs;

    lockKey = intLock();

    if ((state = sysVmeBusLock ()) == TRUE)
        {

        state = FALSE;
        lockValue =  ((0x80 + sysProcNumGet ())<< 24);

        if (*(UINT32 *)lockAdrs == 0)
            {

            /* No one else has the lock, grab it */

            while (*(UINT32 *)lockAdrs != lockValue)
                {
                /* Reread it to push it out */

                *(UINT32 *)lockAdrs = lockValue;
                }
            state = TRUE;
            }

        sysVmeBusUnlock ();
        intUnlock (lockKey);

        }

    return (state);
    }


#if (defined(INCLUDE_SM_NET) || defined(INCLUDE_SM_OBJ) ||    \
     defined(INCLUDE_TIPC_MEDIA_SM))
/******************************************************************************
*
* sysBusTasClear - Clear a semaphore over the VME bus
*
* Using a VME bus access, unlock the semaphore at the specified address by
* writing a zero to it.
*
* RETURNS: NA
*
* SEE ALSO: sysBusTas
*/

LOCAL void sysBusTasClear
    (
    char * locAdrs
    )
    {
    char * vmeAdrs;
    char * adrs;

    adrs = locAdrs;

    /*
     * If the address to clear is a local DRAM address then we must first
     * convert it to a VME bus address.
     */

    if (IS_DRAM_ADDRESS(locAdrs))
        {

        /* First convert local DRAM access to a VME bus address */

        if (sysLocalToBusAdrs (VME_AM_EXT_SUP_DATA, locAdrs,
                                &vmeAdrs) == ERROR)
            return;

        /*
         * Now convert VME bus address to a local address which provides
         * access over the VME bus.
         */

        if (sysBusToLocalAdrs (VME_AM_EXT_SUP_DATA, vmeAdrs, &adrs) == ERROR)
            return;

        }

    *(UINT32 *)adrs = 0;        /* Clear the semaphore */
    (void) sysInLong ((UINT32)adrs);    /* Push it out */
    }
#endif

/*******************************************************************************
*
* sysVmeRmwTas - test and set a location across the VMEbus using RMW.
*
* This routine performs a test-and-set (TAS) instruction on the specified
* address.  Atomicity is enforced by using the Tempe's RMW capability to
* perform an indivisible operation across the VME bus.  To be successful
* managing semaphores across the bus it is necessary for both the setting
* (acquiring) and clearing (releasing) to be done across the VME bus.
* Setting (acquiring) is done with a Tempe RMW cycle.  Clearing (releasing)
* is done with a simple write of zero done across the VME bus.
*
* RETURNS: TRUE if the value had not been set but is now, FALSE otherwise.
*
* SEE ALSO: sysBusTasClear
*/

LOCAL BOOL sysVmeRmwTas
    (
    char *  locAdrs              /* local address of lock variable */
    )
    {
    char *  vmeAdrs;
    char *  adrs;

    adrs = locAdrs;

    /*
     * If the address to clear is a local DRAM address then we must first
     * convert it to a VME bus address.
     */

    if (IS_DRAM_ADDRESS(locAdrs))
        {

        /* First convert local DRAM access to a VME bus address */

        if (sysLocalToBusAdrs (VME_AM_EXT_SUP_DATA, locAdrs,
                                &vmeAdrs) == ERROR)
            return (FALSE);

        /*
         * Now convert VME bus address to a local address which provides
         * access over the VME bus.
         */

        if (sysBusToLocalAdrs (VME_AM_EXT_SUP_DATA, vmeAdrs, &adrs) == ERROR)
            return (FALSE);

        }

    return (sysVmeRmw(adrs));
    }

/******************************************************************************
*
* sysVmeRmw - Perform RMW cycle accross the VME bus
*
* This routine performs a RMW cycle across the VME bus.  To prevent
* deadlocks, interrupts are disabled during the RMW operation.
*
* RETURNS: TRUE if the value had not been set but is now, FALSE otherwise.
*/

LOCAL BOOL sysVmeRmw
    (
    char * vmeAdrs          /* VME address to be tested and set */
    )
    {

    BOOL state = FALSE;  /* semaphore state */
    int  lockKey;        /* interrupt lock key */

    lockKey = intLock ();

    /* Enable RMW cycle */

    sysBusRmwEnable(VME_RMW_ENABLE_BITS,                /* enable bits */
                    VME_RMW_COMPARE_BITS,               /* cmpare */
                    VME_RMW_SWAP_TO_SET,                /* swap */
                    (char *)vmeAdrs);

    /* perform RMW to try and set TAS location */

    state = *((UINT32 *)vmeAdrs);
    EIEIO_SYNC;

    /* Disable RMW cycle */

    sysBusRmwDisable();

    /* unlock the interrupt */

    intUnlock (lockKey);

    /* return TAS test result */

    return (state ? FALSE : TRUE);
    }

#ifdef INCLUDE_MOTFCCEND
/*******************************************************************************
*
* sysFccEnetAddrGet - get the hardware Ethernet address
*
* This routine provides the six byte Ethernet hardware address that will be
* used by each individual FCC device unit.  This routine must copy
* the six byte address to the space provided by <addr>.
*
* RETURNS: OK, or ERROR if the Ethernet address cannot be returned.
*/
 
STATUS sysFccEnetAddrGet
    (
    int         unit,          /* base address of the on-chip RAM */
    UCHAR *     addr            /* where to copy the Ethernet address */
    )
    {

    bcopy ((char *) &sysFccEnetAddr[unit][0], (char *) addr, 6);
 
    return (OK);
    }

#endif /* INCLUDE_MOTFCCEND */

/*******************************************************************************
*
* sysPciSpecialCycle - generate a special cycle with a message
*
* This routine generates a special cycle with a message.
*
* NOMANUAL
*
* RETURNS: OK
*/

STATUS sysPciSpecialCycle
    (
    int        busNo,
    UINT32    message
    )
    {
    int deviceNo    = 0x0000001f;
    int funcNo      = 0x00000007;

    PCI_OUT_LONG (PCI_PRIMARY_CAR,
                  (LONGSWAP((UINT32)pciConfigBdfPack (busNo, deviceNo, funcNo) |
                   0x80000000)));

    PCI_OUT_LONG (PCI_PRIMARY_CDR, message);

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
* NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*/

STATUS sysPciConfigRead
    (
    int    busNo,    /* bus number */
    int    deviceNo, /* device number */
    int    funcNo,      /* function number */
    int    offset,      /* offset into the configuration space */
    int    width,      /* width to be read */
    void * pData /* data read from the offset */
    )
    {
    UINT8  retValByte = 0;
    UINT16 retValWord = 0;
    UINT32 retValLong = 0;
    STATUS retStat = ERROR;
    UINT32 regVal = 0;

    if ((busNo == 0) && (deviceNo == 0x1f) 

    /* simulator doesn't like this device being used */)

        return (ERROR);

    /* Disable Master Abort/SCM Errors during config cycles */

    regVal = CCSR_READ32 (CCSBAR, CCSR_ERR_CAP_DR);

#ifdef INCLUDE_ERROR_HANDLING
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_CAP_DR, regVal |
                  PCI_ERROR_DETECT_MSTR_ABORT_MASK | PCI_ERROR_DETECT_SCM_MASK);
#endif /* INCLUDE_ERROR_HANDLING */

    switch (width)
        {
        case 1:    /* byte */
            PCI_OUT_LONG (PCI_PRIMARY_CAR,
                (LONGSWAP((UINT32)pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000)));
            
            retValByte = PCI_IN_BYTE (PCI_PRIMARY_CDR + (offset & 0x3));
            *((UINT8 *)pData) = retValByte;
            retStat = OK;
            break;

        case 2: /* word */
            PCI_OUT_LONG (PCI_PRIMARY_CAR,
                (LONGSWAP((UINT32)pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000)));

            retValWord = PCI_IN_WORD (PCI_PRIMARY_CDR + (offset & 0x2));
                *((UINT16 *)pData) = retValWord;
            retStat = OK;
        break;

        case 4: /* long */
            PCI_OUT_LONG (PCI_PRIMARY_CAR,
                (LONGSWAP((UINT32)pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000)));
            retValLong = PCI_IN_LONG (PCI_PRIMARY_CDR);
                *((UINT32 *)pData) = retValLong;
            retStat = OK;
            break;

        default:
            retStat = ERROR;
            break;
        }

#ifdef INCLUDE_ERROR_HANDLING
    regVal &= ~(PCI_ERROR_DETECT_MSTR_ABORT_MASK | PCI_ERROR_DETECT_SCM_MASK);
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_CAP_DR, regVal);
#endif /* INCLUDE_ERROR_HANDLING */

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
* NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*/

STATUS sysPciConfigWrite
    (
    int    busNo,    /* bus number */
    int    deviceNo, /* device number */
    int    funcNo,      /* function number */
    int    offset,      /* offset into the configuration space */
    int width,      /* width to write */
    ULONG data      /* data to write */
    )
    {
    UINT32 regVal = 0;
    STATUS status = OK;

    if ((busNo == 0) && (deviceNo == 0x1f))
        {
        return (ERROR);
        }

    /* Disable Master Abort/SCM Errors during config cycles */

    regVal = CCSR_READ32 (CCSBAR, CCSR_ERR_CAP_DR);

#ifdef INCLUDE_ERROR_HANDLING
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_CAP_DR, regVal |
                  PCI_ERROR_DETECT_MSTR_ABORT_MASK | PCI_ERROR_DETECT_SCM_MASK);
#endif /* INCLUDE_ERROR_HANDLING */

    switch (width)
        {
        case 1:    /* byte */
            PCI_OUT_LONG (PCI_PRIMARY_CAR,
                (LONGSWAP((UINT32)pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000)));

            PCI_OUT_BYTE ((PCI_PRIMARY_CDR + (offset & 0x3)), data);
            break;

        case 2: /* word */
            PCI_OUT_LONG (PCI_PRIMARY_CAR,
                (LONGSWAP((UINT32)pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000)));

            PCI_OUT_WORD ((PCI_PRIMARY_CDR + (offset & 0x2)), data);
            break;

        case 4: /* long */
            PCI_OUT_LONG (PCI_PRIMARY_CAR,
                (LONGSWAP((UINT32)pciConfigBdfPack (busNo, deviceNo, funcNo) |
                          (offset & 0xfc) | 0x80000000)));

            PCI_OUT_LONG (PCI_PRIMARY_CDR, data);
            break;

        default:
            status = ERROR;
        }

#ifdef INCLUDE_ERROR_HANDLING
    regVal &= ~(PCI_ERROR_DETECT_MSTR_ABORT_MASK | PCI_ERROR_DETECT_SCM_MASK);
    CCSR_WRITE32 (CCSBAR, CCSR_ERR_CAP_DR, regVal);
#endif /* INCLUDE_ERROR_HANDLING */

    return (status);
    }

/******************************************************************************
*
* sysUsDelay - delay at least the specified amount of time (in microseconds) 
*
* This routine will delay for at least the specified amount of time using the
* lower 32 bit "word" of the Time Base register as the timer.  The accuracy of
* the delay increases as the requested delay increases due to a certain amount
* of overhead.  As an example, a requested delay of 10 microseconds is
* accurate within approximately twenty percent, and a requested delay of 100
* microseconds is accurate within approximately two percent.
*
* NOTE:  This routine will not relinquish the CPU; it is meant to perform a
* busy loop delay.  The minimum delay that this routine will provide is
* approximately 10 microseconds.  The maximum delay is approximately the
* size of UINT32; however, there is no roll-over compensation for the total
* delay time, so it is necessary to back off two times the system tick rate
* from the maximum.
*
* RETURNS: N/A
*/

void sysUsDelay
    (
    UINT32 delay    /* number of milliseconds to delay */
    )
    {
    UINT32 baselineTickCount;
    UINT32 ticksToWait;

    /*
     * Get the Time Base Lower register tick count, this will be used
     * as the baseline.
     */

    baselineTickCount = sysTimeBaseLGet();

    /*
     * Convert delay time into ticks
     *
     * The Time Base register and the Decrementer count at the same rate:
     * once per 8 processory cycles.
     *
     * e.g., 333333333 cycles    1 tick      1 second              42 tick
     *       ---------------  *  ------   *  --------          =  ----------
     *       1 second            8 cycles    1000000 millisec      millisec
     */

    if ((ticksToWait = delay * ((DEFAULT_CCB_SPD / 8) / 1000000)) == 0)
        return;

    while ((sysTimeBaseLGet() - baselineTickCount) < ticksToWait)
        ;

    return;
    }

/*********************************************************************
*
* sysDelay - Delay for 1 ms.
*
* Call sysUsDelay with a 1000 uSec delay value to delay 1 ms.
*
* RETURNS : NONE
*
*/

void sysDelay (void)
    {
    sysUsDelay (1000);
    }

/*********************************************************************
*
* sysCoreFreqGet - get the e500 core frequency
*
* This routine returns the e500 core frequency in Hertz.
*
* RETURNS : e500 core frequency.
*/

UINT32 sysCoreFreqGet (void) 
    {
    UINT32 e500Ratio = 1;
    UINT32 e500Numer = 1;
    UINT32 e500Denom = 1;
    UINT32 ccsrPorpllsr = 0;

    ccsrPorpllsr = CCSR_READ32 (CCSBAR, CCSR_PORPLLSR);
    e500Ratio = CCSR_PORPLLSR_E500_RATIO_VAL (ccsrPorpllsr);

    if (e500Ratio == 4)
        {
        e500Numer = 2;
        e500Denom = 1;
        }
    else if (e500Ratio == 5)
        {
        e500Numer = 5;
        e500Denom = 2;
        }
    else if (e500Ratio == 6)
        {
        e500Numer = 3;
        e500Denom = 1;
        }
    else if (e500Ratio == 7)
        {
        e500Numer = 7;
        e500Denom = 2;
        }

    return (sysCpuBusSpd () * e500Numer) / e500Denom;
    }

/*********************************************************************
*
* sysUserLedSet - Turn user LEDs on or off.
*
* Turn specified user LED (1, 2, or 3) on or off.
* Any other LED number results in all three LEDs being acted upon.
* User LED 1 is on the front panel. User LEDs' 2 and 3 are on the
* board surface (non-component side) near the front panel edge.
*
* RETURNS : N/A.
*/

void sysUserLedSet 
    (
    UINT8 whichLed,  /* User LED denoted by 1, 2, or 3 */
    BOOL  setOn      /* Turn on when TRUE, off when FALSE */
    )
    {
    UINT8 mask = 0x1;
    UINT8 shift = whichLed;
    UINT8 regVal = *(UINT8 *)BRD_SYSTEM_STATUS_INDICATOR_REG;

    if (whichLed > 3 || whichLed < 1)
        {
        shift = 0;
        mask  = BRD_SYSTEM_STATUS_INDICATOR_USR_LED_MASK;
        }

    if (setOn)
        {
        regVal |= (mask << shift);
        }
    else
        {
        regVal &= ~(mask << shift);
        } 
 
    *(UINT8 *)BRD_SYSTEM_STATUS_INDICATOR_REG = regVal;
    }

/*********************************************************************
*
* sysBrdHardReset - hard reset the board.
*
* This function forces an immediate and complete hard reset of the
* board.
*
* RETURNS : N/A.
*/

void sysBrdHardReset (void) 
    {
    UINT8 regVal = *(UINT8 *)BRD_SYSTEM_CONTROL_REG;

    regVal |= BRD_SYSTEM_CONTROL_RESET_VAL;
    *(UINT8 *)BRD_SYSTEM_CONTROL_REG = regVal;
    }

/*********************************************************************
*
* sysBootBlkAIsSelected - returns the select state of Boot Block A.
*
* This function returns TRUE when boot block A is the active 
* boot block. If FALSE is returned the selected boot is necessarily B.
*
* RETURNS : TRUE if Boot Block A is selected. FALSE when Boot Block B
* is selected.
*/

BOOL sysBootBlkAIsSelected (void) 
    {
    UINT8 regVal = *(UINT8 *)BRD_FLASH_CONTROL_REG;
  
    return ((regVal & BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_MASK) == 0);
    }

/*********************************************************************
*
* sysEepromWriteProtectSet - set the EEPROM write protect state.
*
* This function sets the EEPROM write protection to on or off.
*
* RETURNS: N/A.
*/

void sysEepromWriteProtectSet 
    (
    BOOL setOn  /* TRUE sets write protection. FALSE enables writing. */
    ) 
    {
    UINT8 regVal = *(UINT8 *)BRD_SYSTEM_CONTROL_REG;

    if (setOn)
        {
        regVal |= BRD_SYSTEM_CONTROL_EEPROMWP_MASK;
        }
    else
        { 
        regVal &= ~BRD_SYSTEM_CONTROL_EEPROMWP_MASK;
        }

    *(UINT8 *)BRD_SYSTEM_CONTROL_REG = regVal;
    }

/*********************************************************************
*
* sysFlashIsHwWriteProtected - returns the FLASH write protect state.
*
* This function returns TRUE when the FLASH bank is hardware 
* write protected.
*
* RETURNS : TRUE when hardware write protectd, FALSE when hardware
* write enabled.
*/

BOOL sysFlashIsHwWriteProtected (void) 
    {
    UINT8 regVal = *(UINT8 *)BRD_FLASH_CONTROL_REG;
  
    return ((regVal & BRD_FLASH_CONTROL_HARDWARE_WP_MASK) != 0);
    }

/*********************************************************************
*
* sysFlashSwWriteProtectSet - set FLASH write protect on or off.
*
* This function sets the FLASH software write protection to on or off.
*
* RETURNS : N/A.
*/

void sysFlashSwWriteProtectSet 
    (
    BOOL setOn   /* TRUE sets write protection, FALSE enables writing */
    ) 
    {
    UINT8 regVal = *(UINT8 *)BRD_FLASH_CONTROL_REG;

    if (setOn)
        {
        regVal |= BRD_FLASH_CONTROL_SOFTWARE_WP_MASK;
        }
    else
        { 
        regVal &= ~BRD_FLASH_CONTROL_SOFTWARE_WP_MASK;
        }

    *(UINT8 *)BRD_FLASH_CONTROL_REG = regVal;
    }

/*********************************************************************
*
* sysPciBusASpdGet - returns PCI Bus A Frequency
*
* This function returns the frequency of PCI Bus A in Hertz.
*
* RETURNS : PCI Bus A frequency (speed) in Hz.
*/

UINT32 sysPciBusASpdGet (void) 
    {
    UINT8 regVal = 0x00;

    regVal = *(UINT8 *)BRD_PCI_BUS_A_STAT_REG &
             BRD_PCI_BUS_A_STAT_SPD_MASK;

    return (busFreq[regVal]);
    }

/*********************************************************************
*
* sysPciBusBSpdGet - returns PCI Bus B Frequency
*
* This function returns the frequency of PCI Bus B in Hertz.
*
* RETURNS : PCI Bus B frequency (speed) in Hz.
*/

UINT32 sysPciBusBSpdGet (void) 
    {
    UINT8 regVal = 0x00;

    regVal = *(UINT8 *)BRD_PCI_BUS_B_STAT_REG &
             BRD_PCI_BUS_B_STAT_SPD_MASK;

    return (busFreq[regVal]);
    }

/*********************************************************************
*
* sysPciBusCSpdGet - returns PCI Bus C Frequency
*
* This function returns the frequency of PCI Bus C in Hertz.
*
* RETURNS : PCI Bus C frequency (speed) in Hz.
*/

UINT32 sysPciBusCSpdGet (void) 
    {
    UINT8 regVal = 0x00;

    regVal = *(UINT8 *)BRD_PCI_BUS_C_STAT_REG &
             BRD_PCI_BUS_C_STAT_SPD_MASK;

    return (busFreq[regVal]);
    }


/***************************************************************************
*
* necPd720101ExtConfig - configures the Ext registers in the config header
*
* This routine configures the Ext registers in the vendor specific
* area of the USB controller's PCI configuration space. This configuration
* is unique to the NEC uPD720101 controller as used on the MV3100.
* The controller is set the higher clock speed in the EXT2 config
* register. The controller has three functions. Functions 0 and 2 have
* EXT2 registers that must be programmed. All controllers are configured.
*
* RETURNS: N/A
*/

void necPd720101ExtConfig (void)
    {
    int pciBusNo;
    int pciDevNo;
    int pciFuncNo;
    int instance = 0;
    STATUS status = OK;

    while (status == OK)
        {
        status = pciFindDevice ((PCI_ID_USB & 0xFFFF),
                                (PCI_ID_USB >> 16) & 0xFFFF,
                                instance, &pciBusNo, &pciDevNo, &pciFuncNo);

        if (status == OK)
            {
            sysPciConfigWrite (pciBusNo, pciDevNo, 0,
                               PCI_CONFIG_EXT2_OFFSET, 4,
                               HIGH_CLOCK_SPD_SEL);
            sysPciConfigWrite (pciBusNo, pciDevNo, 2,
                               PCI_CONFIG_EXT2_OFFSET, 4,
                               HIGH_CLOCK_SPD_SEL);
            }

        ++instance;
        }
    }

#ifdef _GNU_TOOL
/***************************************************************

* To avoid symbol loading error when BSP is compiled with GCC,
* the following dummy functions and variable are declared here.
*/
void sysIntHandler (void)
{
}

void vxDecInt (void)
{
}

int excRtnTbl = 0;
#endif /* _GNU_TOOL */

#ifdef INCLUDE_CACHE_SUPPORT
/***********************************************************************
* 
* sysL1CacheInit - Initialize L1 Cache Variables 
*
* This routine initializes variables used by the cache support
* libraries.
*
* RETURNS : NONE
*/

LOCAL void sysL1CacheInit (void)
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

#ifdef BUS_PROBE_TEST

#define START_PCI_TEST 0x00000000
#define STOP_PCI_TEST  0xffffffff

/***********************************************************************
* 
* pciAdrsTest - test isPciAdrs routine  
*
* This test is designed to test the proper functioning of the 
* isPciAdrs() routine which is used for bus probing support.  This test
* will evaluate each address in the range of 0x00000000 to 0xfffffff to 
* determine if it is a PCI address.  Must visually verify that the 
* addresses identified as PCI space are indeed PCI addresses.
*
* RETURNS : N/A
*/

void pciAdrsTest (void)
    {
    UINT32 i;
    UINT32 lastState = 1234;
    UINT32 state;
    UINT32 trans = 4321;
    UINT32 lastDiff = 1234;
    UINT32 newDiff;
    printf("isPciAdrs() address test start = 0x%08x, stop = 0x%08x\n\n",
    START_PCI_TEST,STOP_PCI_TEST);
    for (i = START_PCI_TEST; ((i<= STOP_PCI_TEST) &&
         (i != 0xffffffff)); i++)
        {
        state = isPciAdrs((char *)i,(char **)&trans);
        newDiff = i - trans;
        if ((state != lastState) ||
            ((state == TRUE) && (newDiff != lastDiff)))
            {
            printf("          \r");
            if ((state == TRUE) ||
                 ((state == TRUE) && (newDiff != lastDiff)))
                {
                printf("\n0x%08x TRUE, pciTrans = 0x%08x\n",i,trans);
                lastDiff = newDiff;
                }
                if (state == FALSE)
                    printf("0x%08x FALSE\n",i);
            lastState = state;
            }
        else
            {
            if ((i % 0x1000000) == 0)
                printf("0x%08x\r",i);
            }
        }
    printf("          \r");
    }

#define START_VME_TEST 0x00000000
#define STOP_VME_TEST  0xffffffff

/***********************************************************************
* 
* vmeAdrsTest - test isVmeAdrs routine  
*
* This test is designed to test the proper functioning of the 
* isVmeAdrs() routine which is used for bus probing support.  This test
* will evaluate each address in the range of 0x00000000 to 0xfffffff to 
* determine if it is a PCI address.  Must visually verify that the 
* addresses identified as VME space are indeed VME addresses.
*
* RETURNS : N/A
*/

void vmeAdrsTest (void)
    {
    UINT32 i;
    UINT32 lastState = 1234;
    UINT32 state;
    UINT32 trans = 4321;
    UINT32 outRegSet;
    UINT32 lastDiff = 1234;
    UINT32 newDiff = 0;
    printf("isVmeAdrs() address test start = 0x%08x, stop = 0x%08x\n\n",
            START_VME_TEST,STOP_VME_TEST);
    for (i = START_VME_TEST;
         ((i<= STOP_VME_TEST) && (i != 0xffffffff)); i++)
        {
        state = isVmeAdrs((char *)i,(char **)&trans,&outRegSet);
        newDiff = i - trans;
        if ((state != lastState) ||
            ((state == TRUE) && (newDiff != lastDiff)))
            {
            printf("          \r");
            if ((state == TRUE) ||
                ((state == TRUE) && (newDiff != lastDiff)))
                {
                printf("\n0x%08x TRUE, vmeTrans = 0x%08x, "
                       "outRegSet = 0x%08x\n",
                       i,trans,outRegSet);
                lastDiff = newDiff;
                }
            if (state == FALSE)
                printf("0x%08x FALSE\n",i);
            lastState = state;
            }
        else
            {
            if ((i % 0x1000000) == 0)
                printf("0x%08x\r",i);
            }
        }
    printf("          \r");
    }

#define START_RPROBE1_TEST 0x00000000
#define STOP_RPROBE1_TEST  0xFFFFFFFF
#define INTERVAL_RPROBE1_TEST 0x100

/***********************************************************************
* 
* rprobe1test - read probe test  
*
* This test is designed to test the proper functioning of the 
* sysBusProbe() routine when performing read probes.  This test
* will evaluate each address in the range of 0x00000000 to 0xfffffff to 
* determine if it is read probeable.  Must visually verify that the 
* addresses identified as read probeable are indeed read probeable.
*
* RETURNS : N/A
*/

void rprobe1test (void)
    {
    UINT32 i;
    UINT32 lastState = 1234;
    UINT32 state;
    UINT8  r8;
    UINT32 lasti = 0;
    printf("rprobe1()\n address test start = 0x%08x, stop = 0x%08x, "
           "interval = 0x%08x\n\n",
           START_RPROBE1_TEST,STOP_RPROBE1_TEST,INTERVAL_RPROBE1_TEST);
    for (i = START_RPROBE1_TEST;;)
        {
        state = sysBusProbe((char *)i, VX_READ, 1, &r8);
        if (state != lastState)
            {
            printf("          \r");
            if (state == OK)
                printf("\n0x%08x OK\n",i);
            if (state == ERROR)
                printf("0x%08x ERROR\n",i);
            lastState = state;
            }
        else
            {
            if ((i % 0x100000) == 0)
                printf("0x%08x\r",i);
            }
        lasti = i;
        i+= INTERVAL_RPROBE1_TEST;
        if ((i < lasti) || (i > STOP_RPROBE1_TEST))
            break;
        }
    printf("          \r");
    }

/***********************************************************************
* 
* wprobeit - write probe test support routine  
*
* This routine is not meant to be called directly from the kernel
* prompt, it is a support routine for the wprobe routine that follows.
*
* RETURNS : N/A
*/

void wprobeit
    (
    UINT32 adrs, 
    int size, 
    STATUS expect
    )
    {
    UINT32 temp = 12345678;
    STATUS stat;
    printf("write probing size= %d, addr = 0x%08x, result = ",
            size, (char *)adrs);
    stat = sysBusProbe((char *)adrs, VX_WRITE, size, &temp);
    printf("%s ",(stat == OK)?"OK   ":"ERROR");
    if (stat == expect)
        printf(" TEST passed\n");
    else
        printf(" TEST FAILED\n");
    }
        
/***********************************************************************
* 
* wprobe - write probe test  
*
* This test is designed to test the proper functioning of the 
* sysBusProbe() routine when performing write probes.  This test
* will evaluate specific "safe" addresses to determine whether or not
* they are write probeable.  We must be careful since write-probing
* can be very destructive.  Must visually verify that the addresses
* are indeed write probeable. 
*
* RETURNS : N/A
*/

void wprobe (void)
    {
    wprobeit(0x1f0, 4, OK);           /* DRAM */
    wprobeit(0x1ffffffe, 2, OK);      /* Last 2-bytes of DRAM */
    wprobeit(0x20000000, 1, ERROR);   /* 1st byte beyond DRAM */
                            
    wprobeit(0x80000000, 1, OK);      /* VME A32 maps to DRAM win 1*/
    wprobeit(0x80100000, 2, ERROR);   /* Just outside VME window 1 */
                            
    wprobeit(0x8fff0000, 1, OK);      /* Just inside VME win 0 */
    wprobeit(0x8fff0fff, 1, OK);      /* inside VME win 0 my CSR */
    wprobeit(0x8fff1000, 1, ERROR);   /* outside VME win 0 outside CSR */
                            
    wprobeit(0x8ffeffff, 1, ERROR);   /* Just before VME win 0 */
    wprobeit(0x90000000, 1, ERROR);   /* Just inside VME win 2 A24 */
    wprobeit(0x91000000, 1, ERROR);   /* Just inside VME win 3 A16 */
                            
    wprobeit(0xa0000000, 4, OK);      /* PCI with responder */
    wprobeit(0xa0700000, 4, ERROR);   /* PCI with no responder */
                            
    wprobeit(0xf8000000, 1, OK);      /* Boot Flash bank */
    }
#endif /* BUS_PROBE_TEST */

/*******************************************************************************
*
* bspSerialChanGet - get the serial channel
*
* This routine is called by the vxbus sio driver.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

SIO_CHAN *bspSerialChanGet
    (
    int channel
    )
    {
    return ((SIO_CHAN *)ERROR);
    }

