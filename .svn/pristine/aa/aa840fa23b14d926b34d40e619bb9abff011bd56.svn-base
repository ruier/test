/*  romInit.s - Wind River SBC8641 ROM initialization module */

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
01e,27aug08,dtr  Fix GNU compiler error.
01d,13aug08,dtr  Modify boot for AMP.
01c,25oct07,to   SMP ROMable image support (WIND00107937)
01b,25jun07,x_s  Modified to support wrSbc8641d.
01a,22aug06,kds  Modifed from hpcNet8641/romInit.s/01e

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
#define	LOADPTR(reg,const32) \
	  addis reg,r0,HIADJ(const32); addi reg,reg,LO(const32)

/*
 * LOADVAR initializes a register with the contents of a specified memory
 * address. The difference being that the value loaded is the contents of
 * the memory location and not just the address of it. [REMOVE WHEN NO LONGER
 * NEEDED].
 * This is tracked at WRS under SPR#20104.
 */

#undef LOADVAR
#define	LOADVAR(reg,addr32) \
	  addis reg,r0,HIADJ(addr32); lwz reg,LO(addr32)(reg)

#define WRITEADR(reg1,reg2,addr32,val) \
	lis	reg1, HI(addr32); \
	ori	reg1, reg1, LO(addr32); \
	lis	reg2, HI(val); \
	ori	reg2, reg2, LO(val); \
	stw	reg2, 0(reg1)

#define WRITEOFFSET(regbase,reg2,offset,val) \
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

	FUNC_IMPORT(romStart)   	/* system initialization routine */

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
	.long 0x08090a00
	.long 0x0c000000
	.long 0x10000000
	.long 0x14000000

cold:
	li	r11, BOOT_COLD
	li  r12, 0
	bl	start		/* skip over next instruction */

warm:
	or	r11, r3, r3	/* startType to r11 */
	li  r12, 1
start:

	/*
	 * DINK32 does a sync after the isync.  It may or may not
	 * be necessary, but should at least be harmless.
	 */
	sync

	/* Zero-out the CPU's registers */

	xor	r0,r0,0		/* DINK32 uses xor instead of addis */
	mtspr   SPRG0,r0
	mtspr   SPRG1,r0
	mtspr   SPRG2,r0
	mtspr   SPRG3,r0

        /* initialize the stack pointer */

	LOADPTR (sp, STACK_ADRS)

	/* Set MPU/MSR to a known state. Turn on FP */

	LOADPTR (r3, _PPC_MSR_FP)
	sync
	mtmsr 	r3
	isync
	sync		/* DINK32 does both isync and sync after mtmsr */

	/* Init the floating point control/status register */

	mtfsfi  7,0x0
	mtfsfi  6,0x0
	mtfsfi  5,0x0
	mtfsfi  4,0x0
	mtfsfi  3,0x0
	mtfsfi  2,0x0
	mtfsfi  1,0x0
	mtfsfi  0,0x0
	isync

	/* Initialize the floating point data regsiters to a known state */

	bl	ifpdr_value
	.long	0x3f800000	/* 1.0 */

ifpdr_value:
	mfspr	r3,LR
	lfs	f0,0(r3)
	lfs	f1,0(r3)
	lfs	f2,0(r3)
	lfs	f3,0(r3)
	lfs	f4,0(r3)
	lfs	f5,0(r3)
	lfs	f6,0(r3)
	lfs	f7,0(r3)
	lfs	f8,0(r3)
	lfs	f9,0(r3)
	lfs	f10,0(r3)
	lfs	f11,0(r3)
	lfs	f12,0(r3)
	lfs	f13,0(r3)
	lfs	f14,0(r3)
	lfs	f15,0(r3)
	lfs	f16,0(r3)
	lfs	f17,0(r3)
	lfs	f18,0(r3)
	lfs	f19,0(r3)
	lfs	f20,0(r3)
	lfs	f21,0(r3)
	lfs	f22,0(r3)
	lfs	f23,0(r3)
	lfs	f24,0(r3)
	lfs	f25,0(r3)
	lfs	f26,0(r3)
	lfs	f27,0(r3)
	lfs	f28,0(r3)
	lfs	f29,0(r3)
	lfs	f30,0(r3)
	lfs	f31,0(r3)
	sync

	/* Set MPU/MSR to a known state. Turn off FP */

	andi.	r3, r3, 0
	sync
	mtmsr 	r3
	isync
	sync		/* DINK32 does both isync and sync after mtmsr */

	/* Init the Segment registers */

	andi.	r3, r3, 0
	isync
	mtsr    0,r3
	isync
	mtsr    1,r3
	isync
	mtsr    2,r3
	isync
	mtsr    3,r3
	isync
	mtsr    4,r3
	isync
	mtsr    5,r3
	isync
	mtsr    6,r3
	isync
	mtsr    7,r3
	isync
	mtsr    8,r3
	isync
	mtsr    9,r3
	isync
	mtsr    10,r3
	isync
	mtsr    11,r3
	isync
	mtsr    12,r3
	isync
	mtsr    13,r3
	isync
	mtsr    14,r3
	isync
	mtsr    15,r3
	isync

	/* Turn off data and instruction cache control bits */

	mfspr   r3, HID0
	isync
	rlwinm	r4, r3, 0, 18, 15	/* r4 has ICE and DCE bits cleared */
	sync
	isync
	mtspr	HID0, r4		/* HID0 = r4 */
	isync



	/* Get cpu type */

	mfspr   r28, PVR
	rlwinm  r28, r28, 16, 16, 31

	/* invalidate the MPU's data/instruction caches */

	lis	r3, 0x0
	cmpli   0, 0, r28, CPU_TYPE_750
	beq     CPU_IS_750
	cmpli   0, 0, r28, CPU_TYPE_7400
	beq     CPU_IS_750
	cmpli   0, 0, r28, CPU_TYPE_7410
	beq     CPU_IS_7410
	cmpli   0, 0, r28, CPU_TYPE_7450
	beq     CPU_IS_7450
	cmpli   0, 0, r28, CPU_TYPE_7455
	beq     CPU_IS_7450
	cmpli   0, 0, r28, CPU_TYPE_7457
	beq     CPU_IS_7450
	cmpli   0, 0, r28, CPU_TYPE_7447A
	beq     CPU_IS_7450
	cmpli   0, 0, r28, CPU_TYPE_7448
	beq     CPU_IS_7450
	cmpli   0, 0, r28, CPU_TYPE_8641
	beq     CPU_IS_7450
	cmpli	0, 0, r28, CPU_TYPE_603
	beq	CPU_IS_603
	cmpli	0, 0, r28, CPU_TYPE_603E
	beq	CPU_IS_603
        cmpli   0, 0, r28, CPU_TYPE_603P
        beq     CPU_IS_603
        cmpli   0, 0, r28, CPU_TYPE_604R
        bne     CPU_NOT_604R

	/* CPU is 604R */
        isync			/* isync before changing cache enables */
        mtspr   HID0, r3        /* disable the caches */
        isync
        ori     r4, r4, 0x0002  /* disable BTAC by setting bit 30 */

CPU_NOT_604R:
	/* Unrecognized CPU types jump here, and 604R falls in */
	ori	r3, r3, (_PPC_HID0_ICFI | _PPC_HID0_DCFI)

CPU_IS_603:
	/* 603, 603E, 603P jump here; 604R and unrecognized fall in */
	ori	r3, r3, (_PPC_HID0_ICE | _PPC_HID0_DCE)
	or	r4, r4, r3		/* set bits */
	sync
	isync
	mtspr   HID0, r4		/* HID0 = r4 */
	isync
	andc	r4, r4, r3		/* clear bits */
	cmpli   0, 0, r28, CPU_TYPE_604
	beq	CPU_IS_604
	cmpli   0, 0, r28, CPU_TYPE_604E
	beq	CPU_IS_604
        cmpli   0, 0, r28, CPU_TYPE_604R
        beq     CPU_IS_604
/* Following can't happen -- CPU_TYPE_750 is found earlier and never gets here
 *      cmpli   0, 0, r28, CPU_TYPE_750
 *      beq     CPU_IS_750
 */
	/* 603, 603E, 603P, unrecognized */
	isync
	mtspr	HID0, r4
	isync

#ifdef USER_I_CACHE_ENABLE
	b	I_CACHE_ON_603
#else
	b	configureOnChipDevices
#endif

CPU_IS_7410:
	andi.	r3, r3, 0
	mtspr	1016, r3	/* 7410 bug, L2PMCR not cleared on reset */

CPU_IS_750:
	/* Handle 750, 7400, 7410, 744x, 745x */
        mfspr   r3,HID0

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x8800    /* Setup bit pattern for ICE/ICFI */
        or      r3,r4,r3
        isync
        mtspr   HID0,r3         /* set ICE/ICFI */
        isync

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x0800    /* Setup bit pattern for ICFI */
        andc    r3,r3,r4
        isync
        mtspr   HID0,r3         /* clear IFCI (bit 16) */
        isync

        addis   r4,r0,0x0000    /* Clear r4 */
        ori     r4,r4,0x2000    /* Setup bit pattern for ILOCK */
        andc    r3,r3,r4
        isync
        mtspr   HID0,r3         /* clear ILOCK (bit 18) */
        isync
        sync
        b       configureOnChipDevices
CPU_IS_7450:

	mfspr   r3, HID0
        sync
        oris    r3,r3,0x0400            # Set TBEN
        oris    r3,r3,0x0010            # Set DPM
        mtspr   HID0, r3
        sync
        b       CPU_IS_750

CPU_IS_604:
	/* 604, 604E, 604R, only */
	/* Enables and invalidates have been set in HID0,
	 * and the corresponding bits are cleared in r4.
	 * Loop count not increased for 7400, since it doesn't come here.
	 */
	LOADPTR (r5, 0x1000)		/* loop count, 0x1000 */
	mtspr	CTR, r5

LOOP_DELAY:
	nop
	bdnz	LOOP_DELAY
	isync
	mtspr 	HID0, r4
	isync

	/* turn the Instruction cache ON for faster FLASH ROM boots */

#ifdef USER_I_CACHE_ENABLE

        ori     r4, r4, (_PPC_HID0_ICE | _PPC_HID0_ICFI)
	isync				/* Synchronize for ICE enable */
        mtspr   HID0, r4                /* Enable Instr Cache & Inval cache */
	isync
	b    	configureOnChipDevices

I_CACHE_ON_603:
	/* 603, 603E, 603P, unrecognized */
	ori	r4, r4, (_PPC_HID0_ICE | _PPC_HID0_ICFI)
        rlwinm  r3, r4, 0, 21, 19	/* clear the ICFI bit */

        /*
         * The setting of the instruction cache enable (ICE) bit must be
         * preceded by an isync instruction to prevent the cache from being
         * enabled or disabled while an instruction access is in progress.
         */
	isync
	mtspr   HID0, r4                /* Enable Instr Cache & Inval cache */
	mtspr   HID0, r3                /* using 2 consec instructions */
                                        /* PPC603 recommendation */
	isync
#endif

configureOnChipDevices:

	/* NOTE: This code is for vxWorks ROMable image, not for bootrom */

	/* initialize PIR with cpu index */

	mfspr	r3, MSSCR0
	rlwinm.	r3, r3, _PPC_MSSCR0_BIT_ID + 1, 31, 31
	mtspr	PIR, r3
	beq	1f				/* branch if CPU0 */

	/* running on CPU1 */
	/* set up initial stack pointer */

	LOADVAR (sp, CPU1_STACK_START_ADR)
	stwu	r0, -FRAMEBASESZ(sp)		/* save one frame stack */

	/* jump to sysCpu1Loop - sysInit for AMP */
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
	cmpwi	r12,1         /* Check to see if reboot or power on */
	beq     startCSetup   /* If reboot don't touch ccsrbar or DDR etc*/

doCcsr:

	/* Re-map CCSBAR above 16MB boundary */

	isync
	sync
	lis     r7,HI(0xff700000)
	ori     r7,r7,LO(0xff700000)
	isync
	sync
	lis     r6,HI(0x000E8000)
	ori     r6,r6,LO(0x000E8000)
	stw     r6, 0(r7)
	isync
	sync

	/* Configure MPX/CCB arbiter */

	/* MPC8641D Errata:
	 * MCM1 - disable early reads
	 * MCM3 - disable early writes
	 */

	/* WRITEADR(r6,r7,MCM_DBCR(CCSBAR),0x40000000) */
	WRITEADR(r6,r7,MCM_DBCR(CCSBAR),0xC0000003)
	isync
	eieio

	/* MPC8641D Errata:
	 * MCM7 - stream cnt should be 3
	 * MCM8 - use round robin
	 */

	WRITEADR(r6,r7,MCM_ABCR(CCSBAR),0x00001003)
	isync
	eieio

	/* Setup the memory mapped register address */

	isync
	sync

	lis     r6,HI(CCSBAR)
	ori     r6,r6, LO(CCSBAR)
	isync

	srwi    r5, r6, 12
	/* lis     r7,HI(0xff700000) */
	lis     r7,HI(CCSBAR)
	/*ori     r7,r7,LO(0xff700000) */
	ori     r7,r7,LO(CCSBAR)
	lwz     r4, 0(r7)

	isync
	sync
	eieio

	stw     r5,0(r7)

	sync
	isync
	eieio

	lis    r5,0xffff
	ori    r5, r5,0xE800
	lwz    r4, 0 (r5)
	isync

	li     r4,0x2000
	mtctr   r4
ccsrbarWait:
	nop
	bdnz    ccsrbarWait

	/* Memory mapped region base address */
	/* what replaces LAWBAR */
/*	WRITEADR(r6,r7,M85XX_LAWBAR0(CCSBAR), 0x000E0000)

	WRITEADR(r6,r7,M85XX_LAWAR0(CCSBAR),
		 LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_256MB )

	isync
*/
	/* Peripheral devices */
/*	WRITEADR(r6,r7,M85XX_LAWBAR2(CCSBAR), PERIPH_BASE)

	WRITEADR(r6,r7,M85XX_LAWAR2(CCSBAR),
		 LAWAR_ENABLE | LAWAR_TGTIF_LBC | LAWAR_SIZE_4MB )

	isync             commented by freeWinder for no use*/

#ifdef INCLUDE_LCD
        /* load BR6 */
	WRITEADR(r6,r7,M85XX_BR6 (CCSBAR),0xF4000801)
	/* load OR6 */
	WRITEADR(r6,r7,M85XX_OR6 (CCSBAR),0xFFF06E65)
	isync
	sync
	eieio
	isync
#endif /* INCLUDE_LCD */

#ifdef INCLUDE_LB_SDRAM
	/* load BR4 */
	WRITEADR(r6,r7,M85XX_BR4 (CCSBAR),0xE4001861)
	/* load BR4 */
	WRITEADR(r6,r7,M85XX_OR4 (CCSBAR),0xFC006CC0)
	/* load BR3 */
	WRITEADR(r6,r7,M85XX_BR3 (CCSBAR),0xE0001861)
	/* load BR3 */
	WRITEADR(r6,r7,M85XX_OR3 (CCSBAR),0xFC006CC0)
	isync
	sync
#endif /* INCLUDE_LB_SDRAM */


	/* load BR2 */
	WRITEADR(r6,r7,M85XX_BR2 (CCSBAR),0xe1001801)
	/* load OR2 */
	WRITEADR(r6,r7,M85XX_OR2 (CCSBAR),0xfff00ff7)
	isync
	sync

	eieio
	isync
	/* load BR1 */
	WRITEADR(r6,r7,M85XX_BR1 (CCSBAR),0xE0000801)
	/* load OR1 */
	WRITEADR(r6,r7,M85XX_OR1 (CCSBAR),0xFFFE0FF7)
	isync
	sync
	eieio
	isync
	/* load BR0 */ /* 0xFF001801---->0xF0001801*/
	WRITEADR(r6,r7,M85XX_BR0 (CCSBAR),0xF0001801)
	/* load OR0 */
	WRITEADR(r6,r7,M85XX_OR0 (CCSBAR),0xF0006EF7)
	isync
	sync
	eieio

	/* Initialise the Local Bus Controller ,modify by ff */	
	WRITEADR(r6,r7,M85XX_LCRR(CCSBAR),0x80000002)
	isync
	sync
	eieio
	
	li      r4,0x2000
	mtctr   r4
dllDelay4:
	nop
	bdnz    dllDelay4


	/* Interleaved DDR mode ???? */
#ifdef INCLUDE_DDR_SDRAM
	/* Memory mapped region base address */

	WRITEADR(r6,r7,M85XX_LAWBAR0(CCSBAR),0x40000000 >> LAWBAR_ADRS_SHIFT)

	WRITEADR(r6,r7,M85XX_LAWAR0(CCSBAR),
		 LAWAR_ENABLE | LAWAR_TGTIF_DDR1 | LAWAR_SIZE_1GB )// sjy 20130617 modefied LAWAR_SIZE_512MB )

	isync

	lis	r7, HI(M85XX_PORPLLSR(CCSBAR))
	ori     r7, r7, LO(M85XX_PORPLLSR(CCSBAR))
	lwz     r7, 0(r7)
	andi.   r7, r7, 0x3e
	srwi    r7, r7, 1

	/* Get multiplier from table */
	lis     r8, HI(ROM_TEXT_ADRS)
	ori     r8, r8, LO(platpllTable)
	add     r8, r8, r7
	lbz     r8, 0(r8)

	cmpwi   r8,0 /* Test for unsupported freq */
	beq     romInit  /* loop in boot code */

	/* Initialize the DDR Memory controller */
	lis	r6, HI(DDR1BA)
	ori	r6, r6, LO(DDR1BA)		/* r6 = DDR base */


	WRITEOFFSET(r6,r7,(CS0_BNDS), 0x0040007F)/*WRITEOFFSET(r6,r7,(CS0_BNDS), 0x0020003F)*/
	WRITEOFFSET(r6,r7,(CS1_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS0_CONFIG), 0x80014202)
	WRITEOFFSET(r6,r7,(CS1_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CTRL), 0x02000000)
	WRITEOFFSET(r6,r7,(DDR_DATA_INIT), 0x0000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG_2), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MD_CNTL), 0x0)

	/* comparison assumes 33Mhz clk */
	cmpwi   r8, PLAT_RATIO_533_MHZ
	beq     defaultDDRInit
	cmpwi   r8, PLAT_RATIO_400_MHZ
	beq     DDRInit400
	cmpwi   r8, PLAT_RATIO_333_MHZ
	beq     DDRInit333

defaultDDRInit:

	WRITEOFFSET(r6,r7,(TIMING_CFG_3), 0x00010000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x0022080C)
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x5c57b533)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x021868ce)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x43000000)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24401000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x20040842)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x03000100)

	b finalDDRInit

DDRInit400:

	WRITEOFFSET(r6,r7,(TIMING_CFG_3), 0x00000000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x00220802)
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x38377322)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x002040C7)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x43008008)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24401000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x23C00542)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x05080100)

	b finalDDRInit

DDRInit333:

	WRITEOFFSET(r6,r7,(TIMING_CFG_3), 0x00000000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x00260802)
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x38377322)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x04984CC8)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x63008000)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24400010)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x00400442)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x06090100)

finalDDRInit:

  	isync
	li      r4,0x2000
	mtctr   r4
ddrDelay:
	nop
	bdnz    ddrDelay

#ifdef INCLUDE_DDR_ECC
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xE3000000)
#else /* INCLUDE_DDR_ECC */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xC3000000)
#endif /* INCLUDE_DDR_ECC */

	isync

#endif /* INCLUDE_DDR_SDRAM */
#ifdef INCLUDE_DDR_SDRAM2
	/* Memory mapped region base address */

	WRITEADR(r6,r7,M85XX_LAWBAR1(CCSBAR),DDR_SDRAM_LOCAL_ADRS >> LAWBAR_ADRS_SHIFT)

	WRITEADR(r6,r7,M85XX_LAWAR1(CCSBAR),
		 LAWAR_ENABLE | LAWAR_TGTIF_DDR2| LAWAR_SIZE_1GB )   //sjy 20130617 modefied

	isync

	lis	r7, HI(M85XX_PORPLLSR(CCSBAR))
	ori     r7, r7, LO(M85XX_PORPLLSR(CCSBAR))
	lwz     r7, 0(r7)
	andi.   r7, r7, 0x3e
	srwi    r7, r7, 1

	/* Get multiplier from table */
	lis     r8, HI(ROM_TEXT_ADRS)
	ori     r8, r8, LO(platpllTable)
	add     r8, r8, r7
	lbz     r8, 0(r8)

	cmpwi   r8,0 /* Test for unsupported freq */
	beq     romInit  /* loop in boot code */

	/* Initialize the DDR Memory controller */
	lis	r6, HI(DDR2BA)
	ori	r6, r6, LO(DDR2BA)		/* r6 = DDR base */


	WRITEOFFSET(r6,r7,(CS0_BNDS), 0x0000003F)
	WRITEOFFSET(r6,r7,(CS1_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_BNDS), 0x00000000)
	WRITEOFFSET(r6,r7,(CS0_CONFIG), 0x80014202)
	WRITEOFFSET(r6,r7,(CS1_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS2_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(CS3_CONFIG), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CLK_CTRL), 0x02000000)
	WRITEOFFSET(r6,r7,(DDR_DATA_INIT), 0x0000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG_2), 0x00000000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MD_CNTL), 0x0)

	/* comparison assumes 33Mhz clk */
	cmpwi   r8, PLAT_RATIO_533_MHZ
	beq     defaultDDRInit2
	cmpwi   r8, PLAT_RATIO_400_MHZ
	beq     DDRInit4002
	cmpwi   r8, PLAT_RATIO_333_MHZ
	beq     DDRInit3332

defaultDDRInit2:

	WRITEOFFSET(r6,r7,(TIMING_CFG_3), 0x00010000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x0022080C)
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x5c57b533)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x021868ce)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x43000000)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24401000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x20040842)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x03000100)

	b finalDDRInit2

DDRInit4002:

	WRITEOFFSET(r6,r7,(TIMING_CFG_3), 0x00000000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x00220802)
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x38377322)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x002040C7)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x43008008)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24401000)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x23C00542)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x05080100)

	b finalDDRInit2

DDRInit3332:

	WRITEOFFSET(r6,r7,(TIMING_CFG_3), 0x00000000)
	WRITEOFFSET(r6,r7,(TIMING_CFG_0), 0x00260802)
	WRITEOFFSET(r6,r7,(TIMING_CFG_1), 0x38377322)
	WRITEOFFSET(r6,r7,(TIMING_CFG_2), 0x04984CC8)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0x63008000)
	/* Errata DDR1 - disable refresh reset */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG_2), 0x24400010)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_MODE_CFG), 0x00400442)
	WRITEOFFSET(r6,r7,(DDR_SDRAM_INTERVAL), 0x06090100)

finalDDRInit2:

  	isync
	li      r4,0x2000
	mtctr   r4
ddrDelay2:
	nop
	bdnz    ddrDelay2

#ifdef INCLUDE_DDR_ECC
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xE3000000)
#else /* INCLUDE_DDR_ECC */
	WRITEOFFSET(r6,r7,(DDR_SDRAM_CFG), 0xC3000000)
#endif /* INCLUDE_DDR_ECC */

	isync

#endif /* INCLUDE_DDR_SDRAM */


startCSetup:

#if	FALSE	/* EABI SDA not supported yet */

        /* initialize r2 and r13 according to EABI standard */

	LOADPTR (r2, _SDA2_BASE_)
	LOADPTR (r13, _SDA_BASE_)

#endif	/* FALSE */

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

