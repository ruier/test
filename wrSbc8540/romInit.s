/* romInit.s - Wind River SBC 85xx ROM initialization module */

/* Copyright 1984-2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01m,30jul06,tor  merge comment
01l,16dec04,mil  Added support for INCLUDE_ECC.
01k,10sep04,mil  Added romInitWarm and padded to offset 0x8 from romInit.
01j,04aug04,mil  Updated overrides of DDRDLLCR and LBCDLLCR.
01i,03aug04,mil  Updated OR5 and LSDMR values.
01h,23jun04,mil  Enabled Icache during boot.
01g,05may04,mil  Changed TLB mappings.
01f,02feb04,mil  Added DDRDLLCR setting and startType support for BOOT_CLEAR.
01e,24oct03,mil  Cleanup.
01d,12sep03,mil  Updated CCSRBAR write per UM.
01c,29jul03,dtr  Remove some magic numbers CCSBAR.
01b,22jul03,mil  Saved boot type and allow cold boot.
01a,03oct02,dtr  Created.
*/

/*

DESCRIPTION
This module contains the entry code for VxWorks images that start
running from ROM, such as 'bootrom' and 'vxWorks_rom'.
The entry point, romInit(), is the first code executed on power-up.
It performs the minimal setup needed to call
the generic C routine romStart() with parameter BOOT_COLD.

RomInit() typically masks interrupts in the processor, sets the initial
stack pointer (to STACK_ADRS which is defined in configAll.h), and
readies system memory by configuring the DRAM controller if necessary.
Other hardware and device initialization is performed later in the
BSP's sysHwInit() routine.

A second entry point in romInit.s is called romInitWarm(). It is called
by sysToMonitor() in sysLib.c to perform a warm boot.
The warm-start entry point must be written to allow a parameter on
the stack to be passed to romStart().

WARNING:
This code must be Position Independent Code (PIC).  This means that it
should not contain any absolute address references.  If an absolute address
must be used, it must be relocated by the macro ROM_ADRS(x).  This macro
will convert the absolute reference to the appropriate address within
ROM space no matter how the boot code was linked. (For PPC, ROM_ADRS does
not work.  You must subtract _romInit and add ROM_TEXT_ADRS to each
absolute address). (NOTE: ROM_ADRS(x) macro does not work for current
PPC compiler).

This code should not call out to subroutines declared in other modules,
specifically sysLib.o, and sysALib.o.  If an outside module is absolutely
necessary, it can be linked into the system by adding the module
to the makefile variable BOOT_EXTRA.  If the same module is referenced by
other BSP code, then that module must be added to MACH_EXTRA as well.
Note that some C compilers can generate code with absolute addresses.
Such code should not be called from this module.  If absolute addresses
cannot be avoided, then only ROM resident code can be generated from this
module.  Compressed and uncompressed bootroms or VxWorks images will not
work if absolute addresses are not processed by the macro ROM_ADRS.

WARNING:
The most common mistake in BSP development is to attempt to do too much
in romInit.s.  This is not the main hardware initialization routine.
Only do enough device initialization to get memory functioning.  All other
device setup should be done in sysLib.c, as part of sysHwInit().

Unlike other RTOS systems, VxWorks does not use a single linear device
initialization phase.  It is common for inexperienced BSP writers to take
a BSP from another RTOS, extract the assembly language hardware setup
code and try to paste it into this file.  Because VxWorks provides 3
different memory configurations, compressed, uncompressed, and rom-resident,
this strategy will usually not work successfully.

WARNING:
The second most common mistake made by BSP writers is to assume that
hardware or CPU setup functions done by romInit.o do not need to be
repeated in sysALib.s or sysLib.c.  A vxWorks image needs only the following
from a boot program: The startType code, and the boot parameters string
in memory.  Each VxWorks image will completely reset the CPU and all
hardware upon startup.  The image should not rely on the boot program to
initialize any part of the system (it may assume that the memory controller
is initialized).

This means that all initialization done by romInit.s must be repeated in
either sysALib.s or sysLib.c.  The only exception here could be the
memory controller.  However, in most cases even that can be
reinitialized without harm.
Failure to follow this rule may require users to rebuild bootrom's for
minor changes in configuration.  It is WRS policy that bootroms and vxWorks
images should not be linked in this manner.
*/

#define	_ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "config.h"
#include "regs.h"
#include "arch/ppc/excPpcLib.h"
		
	/* globals */
	FUNC_EXPORT(romInit)
	FUNC_EXPORT(_romInit)
	
	/* externals */
	FUNC_IMPORT(romStart)

	_WRS_TEXT_SEG_START
	
	.fill 0x100,1,0

/***************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
* SYNOPSIS
* \ss
* romInit
*     (
*     int startType     /@ only used by 2nd entry point @/
*     )
* \se
*/

FUNC_BEGIN(romInit)	
FUNC_LABEL(_romInit)

	bl	resetEntry
	nop

FUNC_LABEL(_romInitWarm)
FUNC_LABEL(romInitWarm)
	bl	warm

	.ascii	"Copyright 1984-2004 Wind River Systems, Inc."
	.balign	4

cold:
	li	r2, BOOT_COLD		/* r2 = BOOT_COLD */
	b	start

warm:
	mr	r2, r3			/* r2 = start type */

start:	
        /* turn off all exceptions and emulate real mode */

	mfmsr	r3			/* r3 = msr		*/
	INT_MASK(r3, r4)		/* mask EE and CE bits	*/
	rlwinm	r4, r4, 0, 20, 18	/* mask ME bit		*/
	rlwinm	r4, r4, 0, 28, 25	/* mask IS and DS bits	*/
	mtmsr	r4			/* msr = r4		*/
	isync

	xor     r0, r0, r0		/* r0 = 0 */
	addi    r1, r0, -1		/* r1 = 0xffffffff */

	/* clear some registers */

	mtspr   DEC, r0
	mtspr   TBL, r0
	mtspr   TBU, r0
	mtspr   TSR, r1
	mtspr   TCR, r0
	mtspr   ESR, r0
	mtspr   XER, r0

	/* setup vector prefix and offsets to debug early problems */

        mtspr	IVPR, r0		/* IVPR = 0x0 */

        addi	r3, r0, IVOR0_VAL	/* = _EXC_OFF_CRTL = 0x100 */
        mtspr	IVOR0, r3
        addi	r4, r0, IVOR1_VAL	/* = _EXC_OFF_MACH = 0x200 */
        mtspr	IVOR1, r4
        addi	r5, r0, IVOR2_VAL	/* = _EXC_OFF_DATA = 0x300 */
        mtspr	IVOR2, r5
        addi	r6, r0, IVOR3_VAL	/* = _EXC_OFF_INST = 0x400 */
        mtspr	IVOR3, r6
        addi	r7, r0, IVOR4_VAL	/* = _EXC_OFF_INTR = 0x500 */
        mtspr	IVOR4, r7
        addi	r3, r0, IVOR5_VAL	/* = _EXC_OFF_ALIGN = 0x600 */
        mtspr	IVOR5, r3
        addi	r4, r0, IVOR6_VAL	/* = _EXC_OFF_PROG = 0x700 */
        mtspr	IVOR6, r4
        /* skipping IVOR7 0x800: no FPU on e500 */
        addi	r5, r0, IVOR8_VAL	/* = _EXC_OFF_SYSCALL = 0x900 */
        mtspr	IVOR8, r5
        /* skipping IVOR9 0xa00: no aux processor on e500 */
        addi	r6, r0, IVOR10_VAL	/* = _EXC_OFF_DECR = 0xb00 */
        mtspr	IVOR10, r6
        addi	r7, r0, IVOR11_VAL	/* = _EXC_OFF_FIT = 0xc00 */
        mtspr	IVOR11, r7
        addi	r3, r0, IVOR12_VAL	/* = _EXC_OFF_WD = 0xd00 */
        mtspr	IVOR12, r3
        addi	r4, r0, IVOR13_VAL	/* = _EXC_OFF_DATA_MISS = 0xe00 */
        mtspr	IVOR13, r4
        addi	r5, r0, IVOR14_VAL	/* = _EXC_OFF_INST_MISS = 0xf00 */
        mtspr	IVOR14, r5
        addi	r6, r0, IVOR15_VAL	/* = _EXC_OFF_DBG = 0x1000 */
        mtspr	IVOR15, r6
        addi	r7, r0, IVOR32_VAL	/* = _EXC_OFF_SPE = 0x1100 */
        mtspr	IVOR32, r7
        addi	r3, r0, IVOR33_VAL	/* = _EXC_OFF_VEC_DATA = 0x1200 */
        mtspr	IVOR33, r3
        addi	r4, r0, IVOR34_VAL	/* = _EXC_OFF_VEC_RND = 0x1300 */
        mtspr	IVOR34, r4
        addi	r5, r0, IVOR35_VAL	/* = _EXC_OFF_PERF_MON = 0x1400 */
        mtspr	IVOR35, r5

	/* setup cache clean */

        li	r6, 0x0000
	msync
	isync
	mtspr	L1CSR0, r6		/* disable data cache */

	/* XXX - should add flush here if warm booting, though not needed */

        li	r7, 0x0002		/* r7 = DCFI */
	msync
	isync
	mtspr	L1CSR0, r7		/* invalidate the data cache */

        li	r6, 0x0000
        msync
	isync
	mtspr	L1CSR1, r6		/* disable instrunction cache */

        li	r7, 0x0002
        msync
	isync
	mtspr L1CSR1, r7		/* invalidate the instruction cache */

#if defined(INCLUDE_CACHE_SUPPORT) && defined(USER_I_CACHE_ENABLE)
        isync

        li	r7, 0x0001
	msync
	isync
	mtspr	L1CSR1, r7		/* enable the instruction cache */
#endif  /* INCLUDE_CACHE_SUPPORT && USER_I_CACHE_ENABLE */
	msync
	isync

	/*
	 * skip initializing DDR SDRAM and LBC SDRAM if warm booting.
         * For cold boot, add back unprotected TLB for LBC SDRAM before
	 * reinitialize memory.
	 */

	li	r5, BOOT_COLD
	and.	r5, r5, r2		/* r2 has start type */
	beq	skipCold		/* skip if not cold booting */

bootCold:

        /*
         * Default timing of DDR SDRAM and Local bus SDRAM are
         * set up for the following speeds:
         * DDR            SDRAM
         * 200 MHz        100 MHz
         * 266 MHz        133 MHz
         * The macro DDR333SDRAM82 sets up timing for:
         * 333 MHz        82 MHz
         * This optional speed is not supported by the current BSP.
         * A new bootrom is required for changes to take effect.
         */

#ifdef   INCLUDE_DDR_333MHZ  /* not supported by current BSP */
# define DDR333SDRAM82
#endif   /* INCLUDE_DDR_333MHZ */

	/* defines for memory initialization */

#define WRITEADR(regaddr,reg2,addr32,val)     \
	lis	regaddr, HI(addr32);          \
	ori	regaddr, regaddr, LO(addr32); \
	lis	reg2, HI(val);                \
	ori	reg2, reg2, LO(val);          \
	stw	reg2, 0(regaddr)

#define WRITEOFFSET(regbase,reg2,offset,val)  \
        lis	reg2, HI(val);                \
        ori	reg2, reg2, LO(val);          \
	stw	reg2, offset(regbase);

        /* workaround for LBCDLLCR */

        /* initialize the LCRR */
        WRITEADR(r6,r7,M85XX_LCRR(CCSBAR),0x10000004)
        WRITEADR(r6,r7,M85XX_LCRR(CCSBAR),0x00030004)
        mbar    0

        lis     r7, HIADJ(M85XX_LBCDLLCR(CCSBAR))
        addi    r7, r7, LO(M85XX_LBCDLLCR(CCSBAR))    /* r7 = address */
        lwz     r4, 0(r7)
        andi.   r4, r4, 0x7ff                         /* r4 = tap1 */

        /* set max iteration to ten times */
        li      r6, 11                                /* r6 = loop index */
retestLbcDll:
        addic.  r6, r6, -1
        ble     checkStop       /* check stop and reset */

        /* delay no more than 2500 CCB clock cycles */
        li      r3, 5000
        mtctr   r3
lbcDllDelay:
        bdnz    lbcDllDelay

        /* read latest tap value */
        lwz     r5, 0(r7)
        andi.   r5, r5, 0x7ff                         /* r5 = tap2 */

        subf    r3, r4, r5                            /* r3 = diff */
        mr      r4, r5                                /* r4 = last tap */

        /* check difference is not greater than 10 */
        cmpwi   r3, 10
        bgt     retestLbcDll
        cmpwi   r3, -10
        blt     retestLbcDll

        /* lock DLL */
        slwi    r5, r5, 16
        lis     r3, 0x8000
        or      r5, r5, r3
        stw     r5, 0(r7)                             /* store */
        isync
        sync
        mbar 0

        /* workaround for DDRDLLCR */

        /* override DDR DLL */
        WRITEADR(r7,r6,M85XX_DDRDLLCR(CCSBAR),0x81000000) /* r7 = address */
        mbar    0

        /* ensure 200us delay, assume max 1GHz * 0.000200 = 0x30d40 */
        lis     r6, HI(0x30d40)
        ori     r6, r6, LO(0x30d40)
        mtctr   r6
delayLoop:
        bdnz    delayLoop

        li      r4, 0x1                               /* r4 = loop index */
        lis     r6, HI(0x81000100)                    /* r6 = 0x81000100 */
        ori     r6, r6 ,LO(0x81000100)
retestDdrDll:
        lwz     r3, 0(r7)                             /* read DDRDLLCR */
        cmpw    r3, r6
        beq     ddrDllDone

        lis     r5, HI(M85XX_DEVDISR(CCSBAR))
        ori     r5, r5, LO(M85XX_DEVDISR(CCSBAR))     /* r5 = devdisr addr */
        lwz     r3, 0(r5)                             /* read devdisr */
        oris    r3, r3, 0x1                           /* disable DDR */
        stw     r3, 0(r5)                             /* store */
        mbar    0

        mtctr   r4
ddrDllDelay:
        bdnz    ddrDllDelay

        lwz     r3, 0(r5)                             /* read devdisr */
        rlwinm  r3, r3, 0, 16, 14                     /* enable DDR */
        stw     r3, 0(r5)                             /* store */
        mbar    0

        addi    r4, r4, 1
        b       retestDdrDll

checkStop:
        ba      0x0     /* cause a crash here */

ddrDllDone:

        /* DDR window */
	WRITEADR(r6, r7, M85XX_LAWBAR0(CCSBAR), (DDR_SDRAM_ADRS1 >> 12))
	WRITEADR(r6, r7, M85XX_LAWAR0(CCSBAR),  0x80f0001c)	/* 512 MB */

	/* local bus SDRAM window */
	WRITEADR(r6, r7, M85XX_LAWBAR1(CCSBAR), (LOCAL_SDRAM1_ADRS >> 12))
	WRITEADR(r6, r7, M85XX_LAWAR1(CCSBAR),  0x8040001b)	/* 256 MB */

#if FALSE
	/* local bus boot flash window */
	WRITEADR(r6, r7, M85XX_LAWBAR2(CCSBAR), (BOOT_FLASH_MAP_ADRS >> 12))
	WRITEADR(r6, r7, M85XX_LAWAR2(CCSBAR),  0x80400017)	/* 16 MB */
#else
	/* flash window not needed if 8 MB only at 0xff800000 */
#endif

	/* local bus user flash window */
	WRITEADR(r6, r7, M85XX_LAWBAR3(CCSBAR), (USR_FLASH1_ADRS >> 12))
	WRITEADR(r6, r7, M85XX_LAWAR3(CCSBAR),  0x8040001a)	/* 128 MB */

	/* chip select setup */
	/* boot flash */
	/* sysToMonitor(0x2) does not work if use BOOT_FLASH_MAP_ADRS
	 * and mask changed to 0xff00xxxx, even with LAWBAR2 changed.
	 * Hence need to use BOOT_FLASH_ADRS and mask of 0xff80xxxx. */
	WRITEADR(r6, r7, M85XX_BR0(CCSBAR), (BOOT_FLASH_ADRS | 0x0801))
	WRITEADR(r6, r7, M85XX_OR0(CCSBAR), 0xff800797)
	/* DDR */
        lis	r6, HI(DDRBA)
        ori	r6, r6, LO(DDRBA)		/* r6 = DDR base */
	WRITEOFFSET(r6, r7, CS0_BNDS, 0x0000000f)
	WRITEOFFSET(r6, r7, CS0_CONFIG, 0x80000102)
	WRITEOFFSET(r6, r7, CS1_BNDS, 0x0010001f)
	WRITEOFFSET(r6, r7, CS1_CONFIG, 0x80000102)
	WRITEOFFSET(r6, r7, CS2_BNDS, 0x0)
	WRITEOFFSET(r6, r7, CS2_CONFIG, 0x0)
	WRITEOFFSET(r6, r7, CS3_BNDS, 0x0)
	WRITEOFFSET(r6, r7, CS3_CONFIG, 0x0)
#ifdef DDR333SDRAM82
	WRITEOFFSET(r6, r7, TIMING_CFG_1, 0x37544321)
#else
	WRITEOFFSET(r6, r7, TIMING_CFG_1, 0x37334321)
#endif
	WRITEOFFSET(r6, r7, TIMING_CFG_2, 0x00000800)
	WRITEOFFSET(r6, r7, DDR_SDRAM_CFG, 0x42000000)
#ifdef DDR333SDRAM82
	WRITEOFFSET(r6, r7, DDR_SDRAM_MODE_CFG, 0x00000062)
#else
	WRITEOFFSET(r6, r7, DDR_SDRAM_MODE_CFG, 0x00000022)
#endif
	WRITEOFFSET(r6, r7, DDR_SDRAM_INTERVAL, 0x05200100)
	WRITEOFFSET(r6, r7, DDR_ERR_SBE, 0x00ff0000)

#ifndef INCLUDE_ECC
	WRITEOFFSET(r6, r7, DDR_SDRAM_CFG, 0xc2000000)
#else   /* INCLUDE_ECC */
	WRITEOFFSET(r6, r7, DDR_ERR_DISABLE, 0x0000000d)
	WRITEOFFSET(r6, r7, DDR_ERR_DETECT, 0x8000000d)
	WRITEOFFSET(r6, r7, DDR_ERR_DISABLE, 0x00000000)
	WRITEOFFSET(r6, r7, DDR_ERR_SBE, 0x00ff0000)
	WRITEOFFSET(r6, r7, DDR_ERR_INT_EN, 0x0000000d)  /* TBE 0xd */
	isync
	sync
	mbar	0
	lis	r7, 0x1
	mtspr	9, r7
eccDelayLoop:
	nop
	bdnz	eccDelayLoop
	WRITEOFFSET(r6, r7, DDR_SDRAM_CFG, 0x62000000)
	WRITEOFFSET(r6, r7, DDR_SDRAM_CFG, 0xe2000000)
	isync
	sync
	mbar	0
#endif  /* INCLUDE_ECC */

	/* LBC chip select */
	WRITEADR(r6, r7, M85XX_BR3(CCSBAR), (LOCAL_SDRAM1_ADRS | 0x1861))
	WRITEADR(r6, r7, M85XX_OR3(CCSBAR), 0xfc000cc1)
	WRITEADR(r6, r7, M85XX_BR4(CCSBAR), (LOCAL_SDRAM2_ADRS | 0x1861))
	WRITEADR(r6, r7, M85XX_OR4(CCSBAR), 0xfc000cc1)
	/* peripheral chip select */
	WRITEADR(r6, r7, M85XX_BR5(CCSBAR), (UTIL_ADRS | 0x0801))
	WRITEADR(r6, r7, M85XX_OR5(CCSBAR), 0xff0008f4)
	/* user flash chip select */
	WRITEADR(r6, r7, M85XX_BR6(CCSBAR), (USR_FLASH1_ADRS | 0x1801))
	WRITEADR(r6, r7, M85XX_OR6(CCSBAR), 0xfc000797)
	WRITEADR(r6, r7, M85XX_BR1(CCSBAR), (USR_FLASH2_ADRS | 0x1801))
	WRITEADR(r6, r7, M85XX_OR1(CCSBAR), 0xfc000797)
	/* LBC refresh and clock divide */
#ifdef DDR333SDRAM82
	WRITEADR(r6, r7, M85XX_LCRR(CCSBAR), 0x00000004)
#else
	WRITEADR(r6, r7, M85XX_LCRR(CCSBAR), 0x00000002)
#endif
	WRITEADR(r6, r7, M85XX_LSRT(CCSBAR), 0x0c000000)

        /*
         * Rewrite TLB to set WINGE to 00000
         * TLB1 #1:  RAM - Protected, global, TS=0, cached
         * 0x00000000..0x0fffffff => 0x000000000..0x0fffffff (256 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1001		/* TLBSEL = TLB1(CAM), ESEL = 1 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0xC000		/* V = 1, IPROT = 1, TID = 0 */
	ori	r5, r5, 0x0900		/* TS = 0, TSIZE = 256 MB */
	mtspr	MAS1, r5
	addis	r6, 0, DDR_SDRAM_ADRS1 >> 16	/* EPN = 0x00000000 */
	ori	r6, r6, 0x0000		/* WIMGE = 00000 */
	mtspr	MAS2, r6
	addis	r7, 0, DDR_SDRAM_ADRS1 >> 16	/* RPN = 0x00000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

        /*
         * Rewrite TLB to set WINGE to 00000
         * TLB1 #2:  RAM - Protected, global, TS=0, cached
         * 0x10000000..0x1fffffff => 0x100000000..0x1fffffff (256 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1002		/* TLBSEL = TLB1(CAM), ESEL = 2 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0xC000		/* V = 1, IPROT = 1, TID = 0 */
	ori	r5, r5, 0x0900		/* TS = 0, TSIZE = 256 MB */
	mtspr	MAS1, r5
	addis	r6, 0, DDR_SDRAM_ADRS2 >> 16	/* EPN = 0x10000000 */
	ori	r6, r6, 0x0000		/* WIMGE = 00000 */
	mtspr	MAS2, r6
	addis	r7, 0, DDR_SDRAM_ADRS2 >> 16	/* RPN = 0x10000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

#if FALSE  /* not using L2SRAM in ROM */
	/* L2SRAM setup */
	WRITEADR(r6, r7, M85XX_L2CTL(CCSBAR), 0xa8010000)
	WRITEADR(r6, r7, M85XX_L2SRBAR0(CCSBAR), L2SRAM_ADDR)
#endif  /* FALSE */

#ifdef INCLUDE_LOCAL_SDRAM
	/* (re)map TLB that were not protected before (re)initialize */

        /*
         * TLB1 #5:  SDRAM - Global, TS=0, cached
         * 0x20000000..0x23ffffff => 0x200000000..0x23ffffff (64 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1005		/* TLBSEL = TLB1(CAM), ESEL = 5 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0x8000		/* V = 1, IPROT = 0, TID = 0 */
	ori	r5, r5, 0x0800		/* TS = 0, TSIZE = 64 MB */
	mtspr	MAS1, r5
	addis	r6, 0, LOCAL_SDRAM1_ADRS >> 16	/* EPN = 0x20000000 */
	ori	r6, r6, 0x0006		/* WIMGE = 00110 */
	mtspr	MAS2, r6
	addis	r7, 0, LOCAL_SDRAM1_ADRS >> 16	/* RPN = 0x20000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

        /*
         * TLB1 #6:  SDRAM - Global, TS=0, cached
         * 0x24000000..0x27ffffff => 0x240000000..0x27ffffff (64 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1006		/* TLBSEL = TLB1(CAM), ESEL = 6 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0x8000		/* V = 1, IPROT = 0, TID = 0 */
	ori	r5, r5, 0x0800		/* TS = 0, TSIZE = 64 MB */
	mtspr	MAS1, r5
	addis	r6, 0, LOCAL_SDRAM2_ADRS >> 16	/* EPN = 0x24000000 */
	ori	r6, r6, 0x0006		/* WIMGE = 00110 */
	mtspr	MAS2, r6
	addis	r7, 0, LOCAL_SDRAM2_ADRS >> 16	/* RPN = 0x24000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

	WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), 0x2862a527)
	WRITEADR(r6, r7, 0x20000000, 0xff)
	WRITEADR(r6, r7, 0x24000000, 0xff)
	mbar	0
	WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), 0x0862a527)
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR2_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR2_2 */
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR3_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR3_2 */
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR4_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR4_2 */
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR5_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR5_2 */
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR6_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR6_2 */
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR7_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR7_2 */
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR8_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR8_2 */
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR9_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR9_2 */
	WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), 0x1862a527)
	mbar	0
	WRITEADR(r6, r7, 0x20000000, 0xff)		/* DWR10_1 */
	WRITEADR(r6, r7, 0x24000000, 0xff)		/* DWR10_2 */
	WRITEADR(r6, r7, M85XX_LSDMR(CCSBAR), 0x4062a527)
	mbar	0

        /*
         * Change WIMGE
         * TLB1 #5:  SDRAM - Global, TS=0, cached
         * 0x20000000..0x23ffffff => 0x200000000..0x23ffffff (64 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1005		/* TLBSEL = TLB1(CAM), ESEL = 5 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0x8000		/* V = 1, IPROT = 0, TID = 0 */
	ori	r5, r5, 0x0800		/* TS = 0, TSIZE = 64 MB */
	mtspr	MAS1, r5
	addis	r6, 0, LOCAL_SDRAM1_ADRS >> 16	/* EPN = 0x20000000 */
	ori	r6, r6, 0x0000		/* WIMGE = 00000 */
	mtspr	MAS2, r6
	addis	r7, 0, LOCAL_SDRAM1_ADRS >> 16	/* RPN = 0x20000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

        /*
         * Change WIMGE
         * TLB1 #6:  SDRAM - Global, TS=0, cached
         * 0x24000000..0x27ffffff => 0x240000000..0x27ffffff (64 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1006		/* TLBSEL = TLB1(CAM), ESEL = 6 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0x8000		/* V = 1, IPROT = 0, TID = 0 */
	ori	r5, r5, 0x0800		/* TS = 0, TSIZE = 64 MB */
	mtspr	MAS1, r5
	addis	r6, 0, LOCAL_SDRAM2_ADRS >> 16	/* EPN = 0x24000000 */
	ori	r6, r6, 0x0000		/* WIMGE = 00000 */
	mtspr	MAS2, r6
	addis	r7, 0, LOCAL_SDRAM2_ADRS >> 16	/* RPN = 0x24000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
#endif  /* INCLUDE_LOCAL_SDRAM */

#if FALSE	/* this doesn't work yet */
	/*
	 * flash invalidate L1 I and D TLB entries, L2 TLB0 entries,
	 * and unprotected L2TLB1 entries to prevent overlap.
	 */

	li	r4, 0x2			/* r4 = L2TLB1_FI */
tstTlbClear0:
	mfspr	r5, MMUCSR0		/* r5 = MMUCSR0 */
	and.	r5, r5, r4
	bne	tstTlbClear0		/* loop to wait for clear */
	mtspr	MMUCSR0, r4		/* flash invalidate L2TLB1 */
tstTlbClear1:
	mfspr	r5, MMUCSR0		/* r5 = MMUCSR0 */
	and.	r5, r5, r4
	bne	tstTlbClear1		/* loop to wait for clear */
	li	r4, 0x1c		/* IL1MMU_FI | DL1MMU_FI | L2TLB0_FI */
	mtspr	MMUCSR0, r4		/* flash invalidate L1 and L2 */
        tlbsync
#endif  /* FALSE */


skipCold:

#ifdef INCLUDE_PCI
        /* Set the pciAutoconfig check to FALSE */
	xor     r5, r5, r5                /* Zero r5 */
	lis     r6, HIADJ(PCI_AUTO_CONFIG_ADRS)                   
	addi	r6, r6, LO(PCI_AUTO_CONFIG_ADRS)
	stw     r5, 0(r6)
#endif /* INCLUDE_PCI */

         /* Clear the CPU reservation bit */
#if FALSE  /* does not work, errata? */
        li      r0, 0
        lwarx   r3, r0, r0
        stwcx.  r3, r0, r0
#endif  /* does not work, errata? */
	
	xor	r6, r6 ,r6
	xor	r7, r7 ,r7
	mullw	r7, r7 ,r6
	lis	sp, HI(STACK_ADRS)
	ori	sp, sp, LO(STACK_ADRS)
	addi	sp, sp, -FRAMEBASESZ
	lis	r6, HI(romInit)
	ori	r6, r6, LO(romInit)
	lis	r7, HI(romStart)
	ori	r7, r7, LO(romStart)
	lis	r8, HI(ROM_TEXT_ADRS)
	ori	r8, r8, LO(ROM_TEXT_ADRS)
	sub	r6, r7, r6		/* routine - entry point */
	add	r6, r6, r8 		/* + ROM base */
	mtspr	LR, r6

	mr	r3, r2			/* restore startType */
	blr 
FUNC_END(_romInit)
FUNC_END(romInit)


/***************************************************************************
*
* resetEntry - rom entry point
*
* resetEntry is the first function to be called by the Book E core after
* reset. It is located at ROM address 0xfffff000. It is responsible for
* initializing key registers to a quiescent state, initializing the the
* TLB with a static memory mapping adequate for running the bootrom, and
* clearing the rest of the TLB. It sets up device access registers so
* the rest of the SRAM (or Boot Flash) can be correctly accessed.
* Finally, it sets up SRR0 and SRR1 with a safe MSR value and the
* instruction address of the romInit cold start entry point, and
* 'returns from interrupt' to synchronize processor context and continue
* the boot process in romInit.
*
* NOMANUAL
*
*/

#if   defined(_GNU_TOOL)
	.section .boot, "ax", @progbits
#elif defined(_DIAB_TOOL)
        .section .boot, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif

FUNC_BEGIN(resetEntry)
FUNC_LABEL(_resetEntry)

        /* clear some registers */

	xor	r0, r0, r0
	mtspr	SRR0, r0
	mtspr	SRR1, r0
	mtspr	CSRR0, r0
	mtspr	CSRR1, r0
	mtspr	MCSRR0, r0
	mtspr	MCSRR1, r0
	mtspr	PID0, r0
	mtspr	PID1, r0
#ifdef	INIT_SKIP_PID2  /* sometimes causes TLB miss */
	nop
#else
	mtspr	PID2, r0
#endif	/* INIT_SKIP_PID2 */

	/*
	 * XXX - need to add RAM first (vector space?) or can get TLB misses
	 */

        /*
         * TLB1 #0:  Should have mapped the last 4kB by reset logic.
         *
         * TLB1 #1:  RAM - Protected, global, TS=0, cached
         * 0x00000000..0x0fffffff => 0x000000000..0x0fffffff (256 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1001		/* TLBSEL = TLB1(CAM), ESEL = 1 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0xC000		/* V = 1, IPROT = 1, TID = 0 */
	ori	r5, r5, 0x0900		/* TS = 0, TSIZE = 256 MB */
	mtspr	MAS1, r5
	addis	r6, 0, DDR_SDRAM_ADRS1 >> 16	/* EPN = 0x00000000 */
	ori	r6, r6, 0x0006		/* WIMGE = 00110 */
	mtspr	MAS2, r6
	addis	r7, 0, DDR_SDRAM_ADRS1 >> 16	/* RPN = 0x00000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

        /*
         * TLB1 #2:  RAM - Protected, global, TS=0, cached
         * 0x10000000..0x1fffffff => 0x100000000..0x1fffffff (256 MB)
         * Attributes: UXSX-UWSW-URSR (all)
         */

	addis	r4, 0, 0x1002		/* TLBSEL = TLB1(CAM), ESEL = 2 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0xC000		/* V = 1, IPROT = 1, TID = 0 */
	ori	r5, r5, 0x0900		/* TS = 0, TSIZE = 256 MB */
	mtspr	MAS1, r5
	addis	r6, 0, DDR_SDRAM_ADRS2 >> 16	/* EPN = 0x10000000 */
	ori	r6, r6, 0x0006		/* WIMGE = 00110 */
	mtspr	MAS2, r6
	addis	r7, 0, DDR_SDRAM_ADRS2 >> 16	/* RPN = 0x10000000 */
	ori	r7, r7, 0x003f		/* UXSX-UWSW-URSR = 11-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

	/*
	 * setup a TLB entry to move CCSRBAR from default reset location
	 * of 0xff700000 to 0xfe000000.  This will allow a maximum boot
	 * flash size of 16 MB instead of 8 MB to reside at the end of
	 * the memory space.
	 *
	 * In order to have a protected TLB entry to map 16 MB of flash
	 * and not to overlap the reset CCSRBAR location, this TLB entry
	 * will be overwritten after CCSRBAR is moved.
	 */

        /*
         * TLB1 #4:  CCSBAR_RST - Global, TS=0, non-cached
         * 0xff700000..0xff7fffff => 0xff7000000..0xff7fffff (1 MB)
         * Attributes: SW-URSR (no exec, user not writable)
         */

	addis	r4, 0, 0x1004		/* TLBSEL = TLB1(CAM), ESEL = 4 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0x8000		/* V = 1, IPROT = 0, TID = 0 */
	ori	r5, r5, 0x0500		/* TS = 0, TSIZE = 1 MB */
	mtspr	MAS1, r5
	addis	r6, 0, CCSBAR_RST >> 16	/* EPN = 0xff700000 */
	ori	r6, r6, 0x001e		/* WIMGE = 11110 */
	mtspr	MAS2, r6
	addis	r7, 0, CCSBAR_RST >> 16	/* RPN = 0xff700000 */
	ori	r7, r7, 0x0007		/* UXSX-UWSW-URSR = 00-01-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

        /*
         * TLB1 #3:  CCSRBAR - Protected, global, TS=0, non-cached
         * 0xfe000000..0xfe0fffff => 0xfe0000000..0xfe0fffff (1 MB)
         * Attributes: SW-URSR (no exec, user not writable)
         */

	addis	r4, 0, 0x1003		/* TLBSEL = TLB1(CAM), ESEL = 3 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0xC000		/* V = 1, IPROT = 1, TID = 0 */
	ori	r5, r5, 0x0500		/* TS = 0, TSIZE = 1 MB */
	mtspr	MAS1, r5
	addis	r6, 0, CCSBAR >> 16	/* EPN = 0xff700000 */
	ori	r6, r6, 0x001e		/* WIMGE = 11110 */
	mtspr	MAS2, r6
	addis	r7, 0, CCSBAR >> 16	/* RPN = 0xff700000 */
	ori	r7, r7, 0x0007		/* UXSX-UWSW-URSR = 00-01-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

	/* move CCSRBAR base address from reset of 0xff700000 */

	lis	r7, HIADJ(CCSBAR_RST)
	addi	r7, r7,LO(CCSBAR_RST)		/* r7 = 0xff700000 */
	lwz	r6, 0(r7)			/* r6 = current CCSBAR val */
	isync					/* context sync */
	lis	r6, HIADJ(CCSBAR >> 12)
	addi	r6, r6, LO(CCSBAR >> 12)
	stw	r6, 0(r7)			/* CCSRBAR = CCSBAR>>12 */
	lis	r7, HIADJ(resetEntry)
	addi	r7, r7,LO(resetEntry)		/* r7 = 0xfffff800 */
	lwz	r6, 0(r7)			/* r6 = don't cares */
	isync					/* context sync */
	lis	r7, HIADJ(CCSBAR)
	addi	r7, r7,LO(CCSBAR)		/* r7 = CCSBAR */
	lwz	r6, 0(r7)			/* r6 = new CCSBAR val */
	isync					/* context sync */

	/* overwrite TLB entry of CCSBAR_RST to prevent overlap with ROM */

        /*
         * TLB1 #4:  UTIL_ADRS - Protected, global, TS=0, non-cached
         * 0x28000000..0x28ffffff => 0x280000000..0x28ffffff (16 MB)
         * Attributes: UWSW-URSR (no exec)
         */

	addis	r4, 0, 0x1004		/* TLBSEL = TLB1(CAM), ESEL = 4 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0xC000		/* V = 1, IPROT = 1, TID = 0 */
	ori	r5, r5, 0x0700		/* TS = 0, TSIZE = 16 MB */
	mtspr	MAS1, r5
	addis	r6, 0, UTIL_ADRS >> 16	/* EPN = 0x28000000 */
	ori	r6, r6, 0x001e		/* WIMGE = 11110 */
	mtspr	MAS2, r6
	addis	r7, 0, UTIL_ADRS >> 16	/* RPN = 0x28000000 */
	ori	r7, r7, 0x000f		/* UXSX-UWSW-URSR = 00-11-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

        /*
         * TLB1 #0:  ROM - Protected, global, TS=0, non-cached
         * 0xff000000..0xffffffff => 0xff0000000..0xffffffff (16 MB)
         * Attributes: UXSX-URSR (not writable)
         */

	addis	r4, 0, 0x1000		/* TLBSEL = TLB1(CAM), ESEL = 0 */ 
	ori	r4, r4, 0x0000
	mtspr	MAS0, r4
	addis	r5, 0, 0xC000		/* V = 1, IPROT = 1, TID = 0 */
	ori	r5, r5, 0x0700		/* TS = 0, TSIZE = 16 MB */
	mtspr	MAS1, r5
	addis	r6, 0, BOOT_FLASH_MAP_ADRS >> 16 /* EPN = 0xff000000 */
	ori	r6, r6, 0x0016		/* WIMGE = 11110 */
	mtspr	MAS2, r6
	addis	r7, 0, BOOT_FLASH_MAP_ADRS >> 16 /* RPN = 0xff000000 */
	ori	r7, r7, 0x0033		/* UXSX-UWSW-URSR = 11-00-11 */
	mtspr	MAS3, r7
	tlbwe
	tlbsync

#undef USE_RFI	/* not supported */
#ifdef USE_RFI
	mtspr	SRR0, r4
	mfmsr	r0
	mtspr	SRR1, r0
	rfi
	/* mtspr	LR, r4 */
#else	/* USE_RFI */
	b	cold
#endif	/* USE_RFI */

FUNC_END(resetEntry)


/***************************************************************************
*
* resetVector - hardware reset vector
*
* At power-on, the processor starts executing at ROM address 0xfffffffc --
* the top of the address space -- which must be a jump to resetEntry.
* This is defined as a separate "section" to assist the linker in locating
* it properly.
*
* NOMANUAL
*
*/

#if   defined(_GNU_TOOL)
	.section .reset, "ax", @progbits
#elif defined(_DIAB_TOOL)
	.section .reset, 4, "rx"
#else
#error "Please add a correctly spelled .section directive for your toolchain."
#endif

FUNC_BEGIN(resetVector)
	b	resetEntry
FUNC_END(resetVector)

