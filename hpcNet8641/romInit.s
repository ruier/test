/* romInit.s - Freescale ROM initialization module for HPC-NET 8641 */

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind RIver License agreement.
 */

/*
modification history
--------------------
01m,27aug08,dtr  Fix GNU compiler error.
01l,14aug08,dtr  Update to boot for AMP.
01k,26oct07,to   SMP ROMable image support (WIND00107937)
01j,25jul07,pch  SMP cleanup; remove dead code.
01i,08mar07,dtr  Update DDR setting for latest board.
01h,16jan07,pch  change hard-coded 0xff700000 to CCSBAR_RESET
01g,28dec06,dtr  adjust PIXIS and DDR clocking
01f,17nov06,pch  handle SMP warm reboot
01e,07aug06,dtr  merged to palbspdrv and fixed DDR init. No longer support
		 BOOT_CPU1 option, simplify dual boot vs smp.
01d,12jun06,kk   remerged with ITER9_FRZ33, modified for SMP support.
01c,06jun06,kk   rolled back merge from FRZ33.
01b,04apr06,mig  added SMP support
01a,10feb06,dtr  written from sp7447
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

INCLUDE FILES:
*/


/* includes */

#define	_ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include "config.h"



/* defines */

/*
 * Some releases of h/arch/ppc/toolPpc.h had a bad definition of
 * LOADPTR. So we will define it correctly. [REMOVE WHEN NO LONGER NEEDED].
 * This is tracked at WRS under SPR#20104.
 * LOADPTR initializes a register with a 32 bit constant, presumably the
 * address of something.
 */

#undef LOADPTR
#define	LOADPTR(reg, const32) \
	  addis reg, r0, HIADJ(const32); addi reg, reg, LO(const32)

/*
 * LOADVAR initializes a register with the contents of a specified memory
 * address. The difference being that the value loaded is the contents of
 * the memory location and not just the address of it. [REMOVE WHEN NO LONGER
 * NEEDED].
 * This is tracked at WRS under SPR#20104.
 */

#undef LOADVAR
#define	LOADVAR(reg, addr32) \
	  addis reg, r0, HIADJ(addr32); \
	  lwz reg, LO(addr32)(reg)

#define WRITEADR(reg1, reg2, addr32, val) \
	lis	reg1, HI(addr32); \
	ori	reg1, reg1, LO(addr32); \
	lis	reg2, HI(val); \
	ori	reg2, reg2, LO(val); \
	stw	reg2, 0(reg1)

#define WRITEOFFSET(regbase, reg2, offset, val) \
	lis	reg2, HI(val); \
	ori	reg2, reg2, LO(val); \
	stw	reg2, offset(regbase);

/*
 * NOTE: Cannot use ROM_ADRS macro with HIADJ and LO macro
 * functions, for PPC
 */

	/* Exported internal functions */

	.data
	FUNC_EXPORT(_romInit)		/* start of system code */
	FUNC_EXPORT(romInit)		/* start of system code */
	FUNC_EXPORT(_romInitWarm)	/* start of system code */
	FUNC_EXPORT(romInitWarm)	/* start of system code */

	/* externals */

	FUNC_IMPORT(romStart)		/* system initialization routine */

	_WRS_TEXT_SEG_START

/******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*

* romInit
*     (
*     int startType	/@ only used by 2nd entry point @/
*     )

*/

	/*
	 * After reset, the processor fetches from ROM_BASE_ADRS + 0x100.
	 * We offset the _romInit entry by 0x100 bytes to compensate.
	 * In other WRS PPC BSP's, this offset was accounted for in the
	 * elfHex stage of building the rom image by setting HEX_FLAGS
	 * to "-a 100".  Explictly adding the space here seems more
	 * straitforward, and it also helps when not using elfToHex
	 * such as in a flash programmed binary image.   Therefore
	 * the Yellowknife BSP Makefile uses HEX_FLAGS of "-a 0".
	 * This also means that ROM_TEXT_ADRS must be defined
	 * equivalent to ROM_BASE_ADRS + 0x100 in config.h and Makefile.
	 */

	.space (0x100)

FUNC_LABEL(_romInit)
FUNC_BEGIN(romInit)

	/* This is the cold boot entry (ROM_TEXT_ADRS) */

	/*
	 * According to a comment in the DINK32 startup code, a 7400
	 * erratum requires an isync to be the first instruction executed
	 * here.  The isync plus the branch amount to 8 bytes, so omit the
	 * nop which would otherwise follow the branch.
	 */
	isync
	sync
	bl	cold

	/*
	 * This warm boot entry point is defined as ROM_WARM_ADRS in config.h.
	 * It is defined as an offset from ROM_TEXT_ADRS.  Please make sure
	 * that the offset from _romInit to romInitWarm matches that specified
	 * in config.h.  (WRS is standardizing this offset to 8 bytes.)
	 */

_romInitWarm:
romInitWarm:

	/*
	 * See above comment regarding a 7400 erratum.  Since DINK32
	 * uses a common entry point for both cold and warm boot, it's
	 * not clear which one requires the isync.  isync is harmless,
	 * so do it both places to be safe.
	 */
	isync
	sync
	bl	warm

	/*
	 * copyright notice appears at beginning of ROM (in TEXT segment)
	 * This is also useful for debugging images.
	 */

	.ascii   "vxWorks boot image"
	.balign 4

platpllTable:
	.long 0x00000203
	.long 0x04050607
	.long 0x08090a0b
	.long 0xc0d00000
	.long 0x00000000
	.long 0x00000000

cold:
	li	r11, BOOT_COLD
	li	r12, 0
	bl	start		/* skip over next instruction */

warm:
	or	r11, r3, r3	/* startType to r11 */
	li	r12, 1
start:

	/*
	 * DINK32 does a sync after the isync.  It may or may not
	 * be necessary, but should at least be harmless.
	 */
	sync

	/* Zero-out the CPU's registers */

	xor	r0, r0, 0		/* DINK32 uses xor instead of addis */
	mtspr   SPRG0, r0
	mtspr   SPRG1, r0
	mtspr   SPRG2, r0
	mtspr   SPRG3, r0

	/* initialize the stack pointer */

	LOADPTR (sp, STACK_ADRS)

	/* Set MPU/MSR to a known state. Turn on FP */

	LOADPTR (r3, _PPC_MSR_FP)
	sync
	mtmsr	r3
	isync
	sync		/* DINK32 does both isync and sync after mtmsr */

	/* Init the floating point control/status register */

	mtfsfi  7, 0x0
	mtfsfi  6, 0x0
	mtfsfi  5, 0x0
	mtfsfi  4, 0x0
	mtfsfi  3, 0x0
	mtfsfi  2, 0x0
	mtfsfi  1, 0x0
	mtfsfi  0, 0x0
	isync

	/* Initialize the floating point data regsiters to a known state */

	bl	ifpdr_value
	.long	0x3f800000	/* 1.0 */

ifpdr_value:
	mfspr	r3, LR
	lfs	f0, 0(r3)
	lfs	f1, 0(r3)
	lfs	f2, 0(r3)
	lfs	f3, 0(r3)
	lfs	f4, 0(r3)
	lfs	f5, 0(r3)
	lfs	f6, 0(r3)
	lfs	f7, 0(r3)
	lfs	f8, 0(r3)
	lfs	f9, 0(r3)
	lfs	f10, 0(r3)
	lfs	f11, 0(r3)
	lfs	f12, 0(r3)
	lfs	f13, 0(r3)
	lfs	f14, 0(r3)
	lfs	f15, 0(r3)
	lfs	f16, 0(r3)
	lfs	f17, 0(r3)
	lfs	f18, 0(r3)
	lfs	f19, 0(r3)
	lfs	f20, 0(r3)
	lfs	f21, 0(r3)
	lfs	f22, 0(r3)
	lfs	f23, 0(r3)
	lfs	f24, 0(r3)
	lfs	f25, 0(r3)
	lfs	f26, 0(r3)
	lfs	f27, 0(r3)
	lfs	f28, 0(r3)
	lfs	f29, 0(r3)
	lfs	f30, 0(r3)
	lfs	f31, 0(r3)
	sync

	/* Set MPU/MSR to a known state. Turn off FP */

	andi.	r3, r3, 0
	sync
	mtmsr	r3
	isync
	sync		/* DINK32 does both isync and sync after mtmsr */

	/* Init the Segment registers */

	andi.	r3, r3, 0
	isync
	mtsr    0, r3
	isync
	mtsr    1, r3
	isync
	mtsr    2, r3
	isync
	mtsr    3, r3
	isync
	mtsr    4, r3
	isync
	mtsr    5, r3
	isync
	mtsr    6, r3
	isync
	mtsr    7, r3
	isync
	mtsr    8, r3
	isync
	mtsr    9, r3
	isync
	mtsr    10, r3
	isync
	mtsr    11, r3
	isync
	mtsr    12, r3
	isync
	mtsr    13, r3
	isync
	mtsr    14, r3
	isync
	mtsr    15, r3
	isync

	/* Turn off data and instruction cache control bits */

	mfspr   r3, HID0
	isync
	rlwinm	r4, r3, 0, _PPC_HID0_BIT_DCE + 1, _PPC_HID0_BIT_ICE - 1
	sync
	isync
	mtspr	HID0, r4		/* HID0 = r4 */
	isync

	/* invalidate the MPU's data/instruction caches */

	mfspr   r3, HID0
	sync
	oris    r3, r3, 0x0400            # Set TBEN
	oris    r3, r3, 0x0010            # Set DPM
	mtspr   HID0, r3
	sync

	addis   r4, r0, 0x0000    /* Clear r4 */
	ori     r4, r4, 0x8800    /* Setup bit pattern for ICE/ICFI */
	or      r3, r4, r3
	isync
	mtspr   HID0, r3         /* set ICE/ICFI */
	isync

	addis   r4, r0, 0x0000    /* Clear r4 */
	ori     r4, r4, 0x0800    /* Setup bit pattern for ICFI */
	andc    r3, r3, r4
	isync
	mtspr   HID0, r3         /* clear IFCI (bit 16) */
	isync

	addis   r4, r0, 0x0000    /* Clear r4 */
	ori     r4, r4, 0x2000    /* Setup bit pattern for ILOCK */
	andc    r3, r3, r4
	isync
	mtspr   HID0, r3         /* clear ILOCK (bit 18) */
	isync
	sync

	/* NOTE: This code is for vxWorks ROMable image, not for bootrom */

	/* initialize PIR with cpu index */

	mfspr	r3, MSSCR0
	rlwinm.	r3, r3, _PPC_MSSCR0_BIT_ID + 1, 31, 31
	mtspr	PIR, r3
	beq	1f				/* branch if CPU0 */

	/* running on CPU1 only in this code */
	/* set up initial stack pointer */

	LOADVAR (sp, CPU1_STACK_START_ADR)
	stwu	r0, -FRAMEBASESZ(sp)		/* save one frame stack */

	/* jump to sysCpu1Loop or sysInit */
waitForInitStartSet:
	LOADVAR (r3, CPU1_INIT_START_ADR)
	/* Check that init start address is not zero before jumping 
	 * This allows cpu1 to reboot and wait for cpu0 to reload image */
	lis     r4,HI(MP_MAGIC_RBV)
	ori     r4, r4, LO(MP_MAGIC_RBV)
	cmpw    r3, r4
	beq     waitForInitStartSet	
	mtctr	r3
	bctr
1:

	cmpwi	r12, 1         /* Check to see if reboot or power on */
	beq     startCSetup   /* If reboot don't touch ccsrbar or DDR etc */

	/* Configure MPX/CCB arbiter */

	WRITEADR(r6, r7, MCM_DBCR(CCSBAR_RESET), 0x40000000)
	isync
	eieio
	/* Errata form revI of 8641d errata
	 * MCM8 - use round robin
	 * MCM7 - stream cnt should be 3
	 */
	WRITEADR(r6, r7, MCM_ABCR(CCSBAR_RESET), 0x00001003)
	isync
	eieio

	/* Setup the memory mapped register address */

	isync
	sync

	lis     r6, HI(CCSBAR)
	ori     r6, r6, LO(CCSBAR)
	isync

	srwi    r5, r6, 12
	lis     r7, HI(CCSBAR_RESET)
	ori     r7, r7, LO(CCSBAR_RESET)
	lwz     r4, 0(r7)

	isync
	sync
	eieio

	stw     r5, 0(r7)

	sync
	isync
	eieio

	lis    r5, 0xffff
	ori    r5, r5, 0xf800
	lwz    r4, 0 (r5)
	isync

	li     r4, 0x2000
	mtctr   r4
ccsrbarWait:
	nop
	bdnz    ccsrbarWait

	/* Memory mapped region base address */
	/* what replaces LAWBAR */
	WRITEADR(r6, r7, M8XXX_LAWBAR0(CCSBAR), 0xFE000)

	WRITEADR(r6, r7, M8XXX_LAWAR0(CCSBAR),
		 LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_32MB )

	isync

	/* PIXIS - registers */
	WRITEADR(r6, r7, M8XXX_LAWBAR2(CCSBAR), 0xF8100)

	WRITEADR(r6, r7, M8XXX_LAWAR2(CCSBAR),
		 LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_4MB )
	isync

	WRITEADR(r6, r7, M8XXX_BR3 (CCSBAR), 0xF8100801)
	/* load OR1 */
	WRITEADR(r6, r7, M8XXX_OR3 (CCSBAR), 0xFFFF0ff7)
	isync
	sync

	/* load BR1 */
	WRITEADR(r6, r7, M8XXX_BR1 (CCSBAR), 0xFE001001)
	/* load OR1 */
	WRITEADR(r6, r7, M8XXX_OR1 (CCSBAR), 0xFF0000f0)
	isync
	sync
	eieio
	isync
	/* load BR0 */
	WRITEADR(r6, r7, M8XXX_BR0 (CCSBAR), 0xFF001001)
	/* load OR0 */
	WRITEADR(r6, r7, M8XXX_OR0 (CCSBAR), 0xFF0000f0)
	isync
	sync
	eieio

	/* Initialise the Local Bus Controller */
	li      r4, 0x2000
	mtctr   r4
	WRITEADR(r6, r7, M8XXX_LCRR(CCSBAR), 0x80000008)
	isync
	sync
	eieio
dllDelay4:
	nop
	bdnz    dllDelay4

	/* Interleaved DDR mode ???? */


#ifdef INCLUDE_DDR_SDRAM
	/* Memory mapped region base address */

	WRITEADR(r6, r7, M8XXX_LAWBAR1(CCSBAR),
		 DDR_SDRAM_LOCAL_ADRS >> LAWBAR_ADRS_SHIFT)

	WRITEADR(r6, r7, M8XXX_LAWAR1(CCSBAR),
		 LAWAR_ENABLE | LAWAR_TGTIF_DDR1 | LAWAR_SIZE_512MB )

	isync

	lis	r7, HI(M8XXX_PORPLLSR(CCSBAR))
	ori     r7, r7, LO(M8XXX_PORPLLSR(CCSBAR))
	lwz     r7, 0(r7)
	andi.   r7, r7, 0x3e
	srwi    r7, r7, 1


	/* Get multiplier from table */
	lis     r8, HI(ROM_TEXT_ADRS)
	ori     r8, r8, LO(platpllTable)
	add     r8, r8, r7
	lbz     r8, 0(r8)

	cmpwi   r8, 0 /* Test for unsupported freq */
	beq     romInit  /* loop in boot code */

	/* Initialize the DDR Memory controller */
	lis	r6, HI(DDR1BA)
	ori	r6, r6, LO(DDR1BA)		/* r6 = DDR base */


	WRITEOFFSET(r6, r7, (CS0_BNDS), 0x0000001f)
	WRITEOFFSET(r6, r7, (CS1_BNDS), 0x0020003f)
	WRITEOFFSET(r6, r7, (CS2_BNDS), 0x00000000)
	WRITEOFFSET(r6, r7, (CS3_BNDS), 0x00000000)
	WRITEOFFSET(r6, r7, (CS0_CONFIG), 0x80010202)
	WRITEOFFSET(r6, r7, (CS1_CONFIG), 0x80010202)
	WRITEOFFSET(r6, r7, (CS2_CONFIG), 0x00000000)
	WRITEOFFSET(r6, r7, (CS3_CONFIG), 0x00000000)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CLK_CTRL), 0x03800000)
	WRITEOFFSET(r6, r7, (DDR_DATA_INIT), 0x0000000)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG_2), 0x00000000)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_MD_CNTL), 0x0)

        /* If BSP not configured correctly use worst case */

	lis     r7, HIADJ(PIXIS_OSC(PIXIS_BASE))
	lbz     r9, LO(PIXIS_OSC(PIXIS_BASE))(r7)
	andi.   r9, r9, 7

	/* If value not that configured by default use slowest DDR settings */
	cmpwi   r9, PIXIS_OSC_40_MHZ
	beq     useBuildOscFreq
	cmpwi   r9, PIXIS_OSC_50_MHZ
	/* Unsupported oscillator clock assume worst case DDR settings reqd*/
	bne     defaultDDRInit
	addi    r8, r8, 2 /* Adjust multiplier for definitions */
useBuildOscFreq:
	/* Actually there is only one set of DDR settings but left this code
	 * Incase we get optimized values from Freescale */
	cmpwi   r8, PLAT_RATIO_533_MHZ
	beq     defaultDDRInit
	cmpwi   r8, PLAT_RATIO_400_MHZ
	beq     DDRInit400
	cmpwi   r8, PLAT_RATIO_333_MHZ
	beq     DDRInit333

defaultDDRInit:

	WRITEOFFSET(r6, r7, (TIMING_CFG_3), 0x00000000)
	WRITEOFFSET(r6, r7, (TIMING_CFG_0), 0x00260802)
	WRITEOFFSET(r6, r7, (TIMING_CFG_1), 0x4c473422)
	WRITEOFFSET(r6, r7, (TIMING_CFG_2), 0x05184CCA)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0x63000000)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), 0x04400000)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), 0x00400442)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), 0x079E0100)

	b finalDDRInit

DDRInit400:

	WRITEOFFSET(r6, r7, (TIMING_CFG_3), 0x00000000)
	WRITEOFFSET(r6, r7, (TIMING_CFG_0), 0x00260802)
	WRITEOFFSET(r6, r7, (TIMING_CFG_1), 0x38377322)
	WRITEOFFSET(r6, r7, (TIMING_CFG_2), 0x04984CC8)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0x63000000)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), 0x24400010)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), 0x00400442)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), 0x06090100)

	b finalDDRInit

DDRInit333:

	WRITEOFFSET(r6, r7, (TIMING_CFG_3), 0x00000000)
	WRITEOFFSET(r6, r7, (TIMING_CFG_0), 0x00260802)
	WRITEOFFSET(r6, r7, (TIMING_CFG_1), 0x38377322)
	WRITEOFFSET(r6, r7, (TIMING_CFG_2), 0x04984CC8)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0x63000000)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG_2), 0x24400010)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_MODE_CFG), 0x00400442)
	WRITEOFFSET(r6, r7, (DDR_SDRAM_INTERVAL), 0x06090100)

finalDDRInit:

	isync
	li      r4, 0x2000
	mtctr   r4
ddrDelay:
	nop
	bdnz    ddrDelay

#ifdef INCLUDE_DDR_ECC
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0xE3000000)
#else /* INCLUDE_DDR_ECC */
	WRITEOFFSET(r6, r7, (DDR_SDRAM_CFG), 0xC3000000)
#endif /* INCLUDE_DDR_ECC */

	isync

#endif /* INCLUDE_DDR_SDRAM */

#ifdef	RBV_ADRS
	/*
	 * Check for the sentinel value from sysToMonitor, which
	 * if found indicates that we are really doing a warm boot
	 * even though we had to reset the board -- and therefore
	 * reinitialize the hardware -- to get here.  This has to be
	 * done even in non-SMP builds, because a non-SMP bootrom
	 * should work with an SMP vxWorks.  The unrelocated absolute
	 * address is correct, because it is not related to where
	 * this code will be located.
	 */
	lis	r6, HIADJ(RBV_ADRS)
	addi	r6, r6, LO(RBV_ADRS)
	lwz	r7, OFF_RBV_MAGIC1(r6)
	lis	r3, HI(_WRS_BOOT_MAGIC_1)
	ori	r3, r3, LO(_WRS_BOOT_MAGIC_1)
	cmpw	r3, r7
	bne	clrSA
	lwz	r7, OFF_RBV_MAGIC2(r6)
	lis	r3, HI(_WRS_BOOT_MAGIC_2)
	ori	r3, r3, LO(_WRS_BOOT_MAGIC_2)
	cmpw	r3, r7
	bne	clrSA
	lwz	r7, OFF_RBV_MAGIC3(r6)
	lis	r3, HI(_WRS_BOOT_MAGIC_3)
	ori	r3, r3, LO(_WRS_BOOT_MAGIC_3)
	cmpw	r3, r7
	bne	clrSA
	lwz	r11, OFF_RBV_STARTTYPE(r6)

clrSA:
	/* Clear the sentinel area */
	stw	r0, OFF_RBV_STARTTYPE(r6)
	stw	r0, OFF_RBV_MAGIC1(r6)
	stw	r0, OFF_RBV_MAGIC2(r6)
	stw	r0, OFF_RBV_MAGIC3(r6)
#endif	/* RBV_ADRS */

startCSetup:

	/* go to C entry point */

	or	r3, r11, r11		/* put startType in r3 (p0) */
	addi	sp, sp, -FRAMEBASESZ	/* save one frame stack */

	LOADPTR (r6, romStart)
	LOADPTR (r7, romInit)
	LOADPTR (r8, ROM_TEXT_ADRS)

	sub	r6, r6, r7
	add	r6, r6, r8

	mtlr	r6		/* romStart - romInit + ROM_TEXT_ADRS */
	blr
FUNC_END(romInit)
