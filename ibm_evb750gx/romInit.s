/* romInit.s - IBM750CL Evaluation Board ROM initialization module */

/*
* Copyright (c) 2007 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/* Copyright 2005-2006 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01d,30oct07,y_w  Correct the copyright time.
01c,26jul07,y_w  Add support for Hackberry Evaluation Board.
01b,13jul06,avb  Cleanup
01a,21jul05,avb  written from Motorola's sp7447 
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

INCLUDE FILES: tsi108.h
*/


/* includes */

#define	_ASMLANGUAGE
#include <vxWorks.h>
#include <sysLib.h>
#include <asm.h>
#include <regs.h>
#include "config.h"
#include "tsi108.h"

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

/*
 * NOTE: Cannot use ROM_ADRS macro with HIADJ and LO macro
 * functions, for PPC
 */

	/* Exported internal functions */

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
	bl	warm

	/* copyright notice appears at beginning of ROM (in TEXT segment) */

	.ascii   "Copyright 1984-2007 Wind River Systems, Inc."
	.align 2

cold:
	li	r11, BOOT_COLD
	bl	start		/* skip over next instruction */

warm:
	or	r11, r3, r3	/* startType to r11 */

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

CPU_IS_750:
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

TSI108_INIT:
	/* All processor types end up here */
	/*
	 * Code below provides minimal setup for the TSI108 companion chip
	 * to configure Processor Bus (PB) interface and SDRAM Controller.
	 */

	/* Initialize pointer to Tsi108 register space */
	LOADPTR(r29,TSI108_CSR_BASE_RST) /* r29 - tsi108 CSR space */

	/* Initialize Tsi108 chip */

do_tsi108_init:

	/*--------------------------------------------------------------------
	 * Initialize PB interface
	 *------------------------------------------------------------------*/

	ori	r4,r29,TSI108_PB_OFFSET /* r4 - ptr to PB block registers */

#if (TSI108_CSR_BASE != TSI108_CSR_BASE_RST)
	/* 
	 * Relocate (if required) Tsi108 registers.
	 * NOTE: we are in the 32-bit address mode.
	 */
	LOADPTR(r5,(TSI108_CSR_BASE | 0x01)) /* PB_REG_BAR =(new BA+EN) */
	stw	r5,PB_REG_BAR(r4)
	andis.	r29,r5,0xFFFF
	sync
	ori	r4,r29,TSI108_PB_OFFSET
#endif

	/* Set PB Slave configuration register */

	LOADPTR(r5,0x00002481) /* normal value for PB_SCR: TEA enabled */
	lwz	r3, PB_RSR(r4)      /* get PB bus mode */
	andi.	r3,r3,0x0001      /* mask PB_BMODE */
	xori	r3,r3,0x0001       /* mask PB_BMODE: r3 -> (0 = 60X, 1 = MPX) */
	rlwimi	r5,r3,14,17,17  /* for MPX: set DTI_MODE bit */
	stw	r5,PB_SCR(r4)
	sync

	/* Configure PB Arbiter */

	lwz	r5,PB_ARB_CTRL(r4)	/* Read PB Arbiter Control Register */
	li	r3, 0x00F0		/* ARB_PIPELINE_DEP mask - set to 0 */
#ifdef DISABLE_PBM
	ori	r3,r3,0x1000	/* add PBM_EN to clear (enabled by default) */
#endif
	andc	r5,r5,r3	/* Clear the masked bit fields */
	ori	r5,r5,0x0003	/* Set bus parking mode - parked on M0 */
	stw	r5,PB_ARB_CTRL(r4)

#ifdef DISABLE_PVT
	/* Tsi108 Errata 19: Disable PVT */
	li	r5,0x01 /* disable calibration control */
	stw	r5,PB_PVT_CTRL2(r4)
#endif

	sync

	/*--------------------------------------------------------------------
	 * Initialize SDRAM controller.
	 *------------------------------------------------------------------*/

#ifndef SDC_HARDCODED_INIT
	/* get SDC clock prior doing sdram controller autoconfig */
	ori	r4,r29,TSI108_CG_OFFSET    /* r4 - ptr to CG registers */
	lwz	r3, CG_PWRUP_STATUS(r4)	    /* get CG configuration */
	rlwinm	r3,r3,12,29,31		    /* r3 - SD clk */	

	cmpi	0,r3,1			    /* if 001 then sync w/ PB */
	bne	use_sdc_mult

	lwz	r3, CG_PWRUP_STATUS(r4)	    /* get CG configuration */
	rlwinm	r3,r3,16,29,31		    /* r3 - PB clk */	

use_sdc_mult:
	bl load_sdc_clk_sync

	/* MHz: 0,0,183,100,133,167,200,233 */ 
	.long	0,0,  6, 10,  8,  6,  5,  4  /* nSec */ 
load_sdc_clk_sync:
	mfspr	r5,LR
	rlwinm	r3,r3,2,0,31
	lwzx	r9,r5,r3	/* get SD clk rate in nSec */
	/* ATTN: r9 will be used by SPD routine */
#endif /* !SDC_HARDCODED_INIT */

	ori	r4,r29,TSI108_SDC_OFFSET /* r4 - ptr to SDRAM registers */

	/* Initialize SDRAM controller: SDRAM Size = 512MB, One DIMM. */

	xor	r0, r0, r0
	stw	r0,SD_INT_ENABLE(r4)  /* Ensure that interrupts are disabled */
#ifdef ENABLE_SDRAM_ECC
	li	r5,0x01
#endif
	stw	r5,SD_ECC_CTRL(r4)		/* Enable/Disable ECC */
	sync

#ifdef SDC_HARDCODED_INIT /* config sdram controller with hardcoded values */
	/* Set refresh rate and timing parameters */
	LOADPTR(r5,VAL_SD_REFRESH)
	stw	r5,SD_REFRESH(r4)
	LOADPTR(r5,VAL_SD_TIMING)
	stw	r5,SD_TIMING(r4)
	sync

	/* Initialize DIMM0 control and BAR registers */
	LOADPTR(r5,VAL_SD_D0_CTRL)	/* auto-precharge disabled */
#ifdef SDC_AUTOPRECH_EN
	oris	r5,r5,0x0001   /* set auto precharge EN bit */
#endif
	stw	r5,SD_DX_CTRL(0)(r4)
	LOADPTR(r5,VAL_SD_D0_BAR)
	stw	r5,SD_DX_BAR(0)(r4)
	sync

	/* Initialize DIMM1 control and BAR registers
	 *  (same as dimm 0, next 512MB, disabled)
	 */
	LOADPTR(r5,VAL_SD_D1_CTRL)  /* auto-precharge disabled */
#ifdef SDC_AUTOPRECH_EN
	oris	r5,r5,0x0001   /* set auto precharge EN bit */
#endif
	stw	r5,SD_DX_CTRL(1)(r4)
	LOADPTR(r5,VAL_SD_D1_BAR)
	stw	r5,SD_DX_BAR(1)(r4)
	sync

#else /* !SDC_HARDCODED_INIT */
	bl	tsi108_sdram_spd	/* automatically detect SDC settings */
#endif /* SDC_HARDCODED_INIT */

#ifdef DISABLE_PBM
	LOADPTR(r5,0x00000030)	/* PB_EN + OCN_EN */
#else
	LOADPTR(r5,0x00000230)	/* PB_EN + OCN_EN + PB/OCN=80/20 */
#endif /* DISABLE_PBM */

	stw	r5,SD_CTRL(r4)
	eieio
	sync

	/* Enable SDRAM access */
	oris	r5,r5,0x8000   /* start SDC: set SD_CTRL[ENABLE] bit */
	stw	r5,SD_CTRL(r4)
	sync

wait_init_complete:
	lwz	r5,SD_STATUS(r4)
	andi.	r5,r5,0x0001
	beq	wait_init_complete  /* wait until SDRAM init is complete */

	/*--------------------------------------------------------------------
	 * Map SDRAM into the processor bus address space
	 *------------------------------------------------------------------*/

	ori	r4,r29,TSI108_PB_OFFSET

	/* Setup BARs associated with direct path PB<->SDRAM */

	/* 
	 * PB_SDRAM_BAR1:
	 * provides a direct path to the main system memory (cacheable SDRAM)
	 * PB_SDRAM_BAR1 size is set to 1GB to accomodate memory extensions 
	 */

	LOADPTR(r5, 0x00000021)  /* BA=0,Size=1GB,ENable,No Addr.Transl */
	stw	r5,PB_SDRAM_BAR1(r4)
	sync

	/* 
	 * Make sure that PB_SDRAM_BAR1 decoder is set
	 * (to allow following immediate read from SDRAM)
	 */
	lwz	r5,PB_SDRAM_BAR1(r4)
	sync

	/*
	 * PB_SDRAM_BAR2: disabled
	 */

	LOADPTR(r5, 0x00000000)
	stw	r5,PB_SDRAM_BAR2(r4)
	sync

	/*
	 * Make sure that PB_SDRAM_BAR2 decoder is set
	 * (to allow following immediate read from SDRAM)
	 */
	lwz	r5,PB_SDRAM_BAR2(r4)
	sync

tsi108_init_done:

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


#ifndef SDC_HARDCODED_INIT

/*******************************************************************************
* SDRAM SPD Support
*/

/* READ_SPD requirements:
 * byte - byte address in SPD device (0 - 255)
 * r3 = will return data read from I2C Byte location
 * r4 - unchanged (SDC base addr)
 * r5 - clobbered in routine (I2C status)
 * r10 - number of DDR slot where first SPD device is detected
 */

#define READ_SPD(byte_num)	\
	addis	r3,0,byte_num@l;\
	or	r3,r3,r10;\
	ori	r3,r3,0x0A;\
	stw	r3,SD_I2C_CTRL1(r4);\
	li	r3,I2C_CNTRL2_START;\
	stw	r3,SD_I2C_CTRL2(r4);\
	eieio;\
	sync;\
	li	r3,0x100;\
1:				;\
	addic.	r3,r3,-1;\
 	bne	1b;\
2:		  ;\
	lwz	r5,SD_I2C_CTRL2(r4);\
	rlwinm.	r3,r5,0,23,23;\
	bne	2b;\
	rlwinm.	r3,r5,0,3,3;\
	lwz	r3,SD_I2C_RD_DATA(r4)

#define SPD_MIN_RFRSH	(0x00)
#define SPD_MAX_RFRSH	(0x05)

/*******************************************************************************
* tsi108_sdram_spd
*
* Initializes SDRAM Controller using DDR2 DIMM Serial Presence Detect data
* Uses registers: r4 - SDC base address (not changed)
*                 r9 - SDC clocking period in nSec
* Changes registers: r3,r5,r6,r7,r8,r10,r12
*/

tsi108_sdram_spd:
	
	mflr    r19          /* Save LR to be able return later. */
	li	r10,SPD_DIMM0
	xor	r12,r12,r12	/* DIMM Base Address: starts from 0 */

do_first_dimm:

	/**************************************
	 * Program Refresh Rate	Register
	 */

	READ_SPD(12)	/* get Refresh Rate */
	beq	check_next_slot
	li	r5, ERR_RFRSH_RATE
	andi.	r3,r3,0x7F	    /* Ignore self-refresh flag */
	cmpi	0,0,r3,SPD_MIN_RFRSH
	ble	spd_fail
	cmpi	0,0,r3,SPD_MAX_RFRSH
	bgt	spd_fail
	rlwinm	r3,r3,2,0,31
	bl	load_refresh_rates

refresh_rates: /* in nSec */
	.long	15625	/* Normal (0x00) */
	.long	3900	/* Reduced 0.25x (0x01) */ 
	.long	7800	/* Reduced 0.5x (0x02) */
	.long	31300	/* Extended 2x (0x03) */
	.long	62500	/* Extended 4x (0x04) */
	.long	125000	/* Extended 8x (0x05) */

load_refresh_rates:
	mfspr	r5,LR
	lwzx	r5,r5,r3	    /* get refresh rate in nSec */
	divwu	r5,r5,r9	    /* calculate # of SDC clocks */
	stw	r5,SD_REFRESH(r4)   /* Set refresh rate */
	sync

	/**************************************
	 * Program SD Timing Register
	 */

	li	r7, 0	/* clear r7 prior parameter collection */

	READ_SPD(20)	/* get DIMM type: Registered or Unbuffered */
	beq	spd_read_fail
	li	r5, ERR_DIMM_TYPE
	cmpi	0,0,r3,SPD_UDIMM
	beq	do_cl
	cmpi	0,0,r3,SPD_RDIMM
	bne	spd_fail
	oris	r7,r7,0x1000	/* set SD_TIMING[DIMM_TYPE] bit */

do_cl:
	READ_SPD(18)	/* Get CAS Latency */
	beq	spd_read_fail
	li	r5,ERR_CL_VALUE
	andi.	r6,r3,SPD_CAS_3
	beq	cl_4
	li	r6,3
	b	set_cl
cl_4:	
	andi.	r6,r3,SPD_CAS_4
	beq	cl_5
	li	r6,4
	b	set_cl
cl_5:
	andi.	r6,r3,SPD_CAS_5
	beq	spd_fail
	li	r6,5
set_cl:
	rlwimi	r7,r6,24,5,7

	READ_SPD(30)	/* Get tRAS */
	beq	spd_read_fail
	divwu	r6,r3,r9
	mullw	r8,r6,r9
	subf.	r8,r8,r3
	beq	set_tras
	addi	r6,r6,1
set_tras:
	li	r5,ERR_TRAS_FAIL
	cmpi	0,0,r6,0x0F	/* max supported value */
	bgt	spd_fail
	rlwimi	r7,r6,16,12,15

	READ_SPD(29)	/* Get tRCD */
	beq	spd_read_fail
	rlwinm	r3,r3,30,2,31	/* right shift tRCD by 2 as per DDR2 spec */
	divwu	r6,r3,r9
	mullw	r8,r6,r9
	subf.	r8,r8,r3
	beq	set_trcd
	addi	r6,r6,1
set_trcd:
	li	r5,ERR_TRCD_FAIL
	cmpi	0,0,r6,0x07	/* max supported value */
	bgt	spd_fail
	rlwimi	r7,r6,12,17,19

	READ_SPD(27)	/* Get tRP value */
	beq	spd_read_fail
	rlwinm	r3,r3,30,2,31	/* right shift tRP by 2 as per DDR2 spec */
	divwu	r6,r3,r9
	mullw	r8,r6,r9
	subf.	r8,r8,r3
	beq	set_trp
	addi	r6,r6,1
set_trp:
	li	r5,ERR_TRP_FAIL
	cmpi	0,0,r6,0x07	/* max supported value */
	bgt	spd_fail
	rlwimi	r7,r6,8,21,23

	READ_SPD(36)	/* Get tWR value */
	beq	spd_read_fail
	rlwinm	r3,r3,30,2,31	/* right shift tWR by 2 as per DDR2 spec */
	divwu	r6,r3,r9
	mullw	r8,r6,r9
	subf.	r8,r8,r3
	beq	set_twr
	addi	r6,r6,1
set_twr:
	li	r5,ERR_TWR_FAIL
	cmpi	0,0,r6,0x07	/* max supported value */
	bgt	spd_fail
	rlwimi	r7,r6,5,24,26

	READ_SPD(42)	/* Get tRFC */
	beq	spd_read_fail
	li	r5, ERR_TRFC_FAIL
	/* Tsi108 spec: tRFC=(tRFC + 1)/2 */
	addi	r3,r3,1
	rlwinm.	r3,r3,31,1,31 /* divide by 2 */
	beq	spd_fail
	divwu	r6,r3,r9
	mullw	r8,r6,r9
	subf.	r8,r8,r3
	beq	set_trfc
	addi	r6,r6,1
set_trfc:
	cmpi	0,0,r6,0x1F	/* max supported value */
	bgt	spd_fail
	rlwimi	r7,r6,0,27,31

	stw	r7,SD_TIMING(r4)
	sync

	/*======================================================================
	 * The following two registers are set on per-DIMM basis.
	 * The SD_REFRESH and SD_TIMING settings are common for both DIMMS
	 *======================================================================
	 */

do_each_dimm:

	/*****************************************
	 * Program SDRAM DIMM Control Register
	 */

	li	r7, 0		/* clear r7 prior parameter collection */

	READ_SPD(13)		/* Get Primary SDRAM Width */ 
	beq	spd_read_fail
	cmpi	0,0,r3,4	/* Check for 4-bit SDRAM */
	beq	do_nbank
	oris	r7,r7,0x0010	/* Set MEM_WIDTH bit */

do_nbank:
	READ_SPD(17)		/* Get Number of banks on SDRAM device */
	beq	spd_read_fail
	/* Grendel only distinguish betw. 4 or 8-bank memory parts */
	li	r5,ERR_UNKNOWN_PART	/* non-supported memory part */
	cmpi	0,0,r3,4
	beq	do_nrank
	cmpi	0,0,r3,8
	bne	spd_fail
	ori	r7,r7,0x1000

do_nrank:
	READ_SPD(5)	/* Get # of Ranks */
	beq	spd_read_fail
	li	r5,ERR_NRANK_INVALID
	andi.	r6,r3,0x7	/* Use bits [2..0] only */
	beq	do_addr_mode
	cmpi	0,0,r6,1
	bgt	spd_fail
	rlwimi	r7,r6,8,23,23

do_addr_mode:
	READ_SPD(4)	/* Get # of Column Addresses */
	beq	spd_read_fail
	li	r5, ERR_ADDR_MODE
	andi.	r3,r3,0x0f	/* cut off reserved bits */
	cmpi	0,0,r3,8
	ble	spd_fail
	cmpi	0,0,r3,15
	bgt	spd_fail
	addi	r6,r3,-8	/* calculate ADDR_MODE parameter */
	rlwimi	r7,r6,4,24,27	/* set ADDR_MODE field */

set_dimm_ctrl:
#ifdef SDC_AUTOPRECH_EN
	oris	r7,r7,0x0001   /* set auto precharge EN bit */
#endif
	ori	r7,r7,1	/* set ENABLE bit */
	cmpi	0,0,r10,SPD_DIMM0
	bne	1f
	stw	r7,SD_DX_CTRL(0)(r4)
	sync
	b	set_dimm_bar
1:
	stw	r7,SD_DX_CTRL(1)(r4)
	sync


	/********************************************
	 * Program SDRAM DIMMx Base Address Register
	 */

set_dimm_bar:
	READ_SPD(5)		/* get # of Ranks */
	beq	spd_read_fail
	andi.	r7,r3,0x7
	addi	r7,r7,1
	READ_SPD(31)	/* Read DIMM rank density */
	beq	spd_read_fail
	rlwinm	r5,r3,27,29,31
	rlwinm	r6,r3,3,24,28
	or	r5,r6,r5		/* r5 = Normalized Rank Density byte */
	lis	r8, 0x0080		/* 128MB >> 4 */
	mullw	r8,r8,r5		/* r8 = (rank_size >> 4) */
	mullw	r8,r8,r7		/* r8 = (DIMM_size >> 4) */
	neg	r7,r8			
	rlwinm	r7,r7,28,4,31
	or	r7,r7,r12		/* set ADDR field */
	rlwinm	r8,r8,12,20,31
	add	r12,r12,r8		/* set Base Addr for next DIMM */

	cmpi	0,0,r10,SPD_DIMM0
	bne	set_dimm1_size
	stw	r7,SD_DX_BAR(0)(r4)
	sync
	li	r10,SPD_DIMM1
	READ_SPD(0)
	bne	do_each_dimm
	b	spd_done

set_dimm1_size:
	stw	r7,SD_DX_BAR(1)(r4)
	sync
spd_done:
	/* All done. Restore LR and return. */
	mtlr	r19
	blr

check_next_slot:
	cmpi	0,0,r10,SPD_DIMM1
	beq	spd_read_fail
	li	r10,SPD_DIMM1
	b	do_first_dimm
spd_read_fail:
	ori	r3,r0,0xdead
	b	err_hung
spd_fail:
	li	r3,0x0bad
	sync
err_hung:   /* hang here for debugging */
	nop
	nop
	b	err_hung

#endif /* !SDC_HARDCODED_INIT */

FUNC_END(romInit)

