/* romInit.s - wrSbcPowerQuiccII ROM initialization module */

/*
 * Copyright (c) 1984-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,02aug05,mil  Updated copyright message.
01i,10nov04,j_b  project support for boot Flash selection (SPR 103800)
01h,26sep03,j_b  SIMM Flash boot tweak
01g,08aug03,j_b  fix addr masks for Flash chip selects
01f,18jun03,j_b  wait states added to Flash for HiP7 CPUs; cleanup
01e,09may03,scm  add pci adjustments...
01d,30apr03,j_b  replace PQII_ register names with PQII_
01c,23apr03,j_b  use PQII_REG_BASE in IMMR address calculation
01b,15apr03,j_b  add boot progress tracking on LED
01a,01apr03,j_b  adapted from wrSbc8260Atm (ver 01a)
*/

/*
DESCRIPTION
This module contains the entry code for the VxWorks bootrom.
The entry point romInit, is the first code executed on power-up.
It sets the BOOT_COLD parameter to be passed to the generic
romStart() routine.

The routine sysToMonitor() jumps to the location 4 bytes
past the beginning of romInit, to perform a "warm boot".
This entry point allows a parameter to be passed to romStart().

*/

#define	_ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"
#include "cacheLib.h"
#include "config.h"
#include "wrSbcPowerQuiccII.h"
#include "regs.h"
#include "sysLib.h"
#include "drv/timer/m8260Clock.h"
#include "drv/mem/m8260Siu.h"
#include "drv/mem/m8260Memc.h"
#include "sysLed.h"

	/* internals */

	.globl	_romInit	/* start of system code */
	.globl	romInit

	/* externals */

	.extern romStart	/* system initialization routine */

	.text
	.align 2

/******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*
*
* romInit
*     (
*     int startType	/@ only used by 2nd entry point @/
*     )

*/

_romInit:
romInit:
	bl	cold		/* jump to the cold boot initialization */
	nop
	bl	start		/* jump to the warm boot initialization */

	/* copyright notice appears at beginning of ROM (in TEXT segment) */

	.ascii   "Copyright 1984-2003 Wind River Systems, Inc."
	.align 2

cold:
	li	    r3,BOOT_COLD	/* set cold boot as start type */

start:
    /* disable external interrupts (by zeroing out msr) */
	xor	    r5,r5,r5
	isync
	mtmsr	r5
	isync

    /* invalidate and disable the MPU's data/instruction caches */
	mfspr	r6,HID0
	ori	    r5,r5,0xC000
	andc	r6,r6,r5	 /* clear cache enable bits in r6 */
	mr	    r5,r6
	ori	    r5,r5,0xCC00 /* r5 has DCE,ICE,ICFI,DCFI set */
	sync
	mtspr	HID0,r5		 /* invalidate both caches with 2 stores */
	mtspr	HID0,r6		 /* leaving them both disabled */
	isync

	mtspr   SPRG0,r3
	bl	    romClearBATs
	bl	    romInvalidateTLBs
	bl	    romClearSegs
	bl	    romClearFPRegs
	mfspr   r3,SPRG0

        /* Zero-out registers: r0 & SPRGs */
    xor	    r0,r0,r0
	mtspr	SPRG0,r0
	mtspr	SPRG1,r0
	mtspr	SPRG2,r0
	mtspr	SPRG3,r0

    /* MSR: clear DCE,ICE,EE,DR,IR -- set ME,RI	*/
    mfmsr   r4
	ori     r4,r0,0x1002
	mtmsr   r4
	isync

	/*
	 * initialize the IMMR register before any non-core registers
	 * modification.
	 */

	lis	    r4, HIADJ(INTERNAL_MEM_MAP_ADDR + PQII_REG_BASE)
	addi	r4, r4, LO(INTERNAL_MEM_MAP_ADDR + PQII_REG_BASE)
	sync

	lis	    r5, HIADJ(INTERNAL_MEM_MAP_ADDR)
	addi	r5, r5, LO(INTERNAL_MEM_MAP_ADDR)
	sync

	stw	    r5, PQII_IMMR(r4)		/* initialize the IMMR register */

	/********************************************************
	 * initialize the SIU.
	 ********************************************************/

	bl	romSiuInit

	/********************************************************
	 * initialize the MEMC.
	 ********************************************************/

	bl	romMemcInit


	/****************************************************
	 * Initialize Instruction and Data Caches
	 ****************************************************/

  	bl	romCacheInit

#ifdef USE_LEDS_FOR_BOOT_DEBUG
	li	r5,LED_3			/* end of romCacheInit */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	/* initialize the stack pointer */

	lis	    sp, HIADJ(STACK_ADRS)
	addi	sp, sp, LO(STACK_ADRS)

	/* go to C entry point */

	addi	sp, sp, -FRAMEBASESZ		/* get frame stack */

	/*
	 * calculate C entry point: routine - entry point + ROM base
	 * routine	= romStart	= R6
	 * entry point	= romInit	= R7
	 * ROM base	= ROM_TEXT_ADRS = R8
	 * C entry point: R6 - R7 + R8
	 */

    lis	    r6, HIADJ(romStart)
    addi	r6, r6, LO(romStart)	/* load R6 with C entry point */

	lis	    r7, HIADJ(romInit)
	addi	r7, r7, LO(romInit)

	lis	    r8, HIADJ(ROM_TEXT_ADRS)
	addi	r8, r8, LO(ROM_TEXT_ADRS)

	sub	    r6, r6, r7	/* routine - entry point */
	add	    r6, r6, r8 	/* + ROM base */

	mtlr	r6			/* move C entry point to LR */
	blr		    		/* jump to the C entry point */

/*******************************************************************************
*
* romClearBATs -
*
* This routine will zero the BAT registers.
*
* RETURNS: None
*
*/

romClearBATs:			/* zero out the BAT registers */
	xor		r5,r5,r5
	isync
	mtspr	IBAT0U,r5	/* clear all upper BATS first */
	mtspr	IBAT1U,r5
	mtspr	IBAT2U,r5
	mtspr	IBAT3U,r5
	mtspr	DBAT0U,r5
	mtspr	DBAT1U,r5
	mtspr	DBAT2U,r5
	mtspr	DBAT3U,r5

	mtspr	IBAT0L,r5	/* then clear lower BATS */
	mtspr	IBAT1L,r5
	mtspr	IBAT2L,r5
	mtspr	IBAT3L,r5
	mtspr	DBAT0L,r5
	mtspr	DBAT1L,r5
	mtspr	DBAT2L,r5
	mtspr	DBAT3L,r5
	isync


	blr

/*******************************************************************************
*
* romClearSegs -
*
* This routine will zero the MMU's segment registers.
*
* RETURNS: None
*
*/

romClearSegs:		/* Init the Segment registers */
	xor		r5, r5, r5
    isync
    mtsr	0,r5
    mtsr    1,r5
    mtsr    2,r5
    mtsr    3,r5
    mtsr    4,r5
    mtsr    5,r5
    mtsr    6,r5
    mtsr    7,r5
    mtsr    8,r5
    mtsr    9,r5
    mtsr    10,r5
    mtsr    11,r5
    mtsr    12,r5
    mtsr    13,r5
    mtsr    14,r5
    mtsr    15,r5
    isync

	blr

/*******************************************************************************
*
* romInvalidateTLBs - invalidate all TLB entries
*
* This routine will invalidate instruction and data TLB entries.
*
* RETURNS: None
*
*/

romInvalidateTLBs:
	isync
	/* invalidate entries within both 64-entry TLBs */
	li		r5,128
	mtctr	r5		  /* CTR = 128  */
	xor		r5,r5,r5  /* r5 = 0    */
	isync			  /* context sync req'd before tlbie */
tlbloop:
	tlbie	r5
	sync			     /* sync instr req'd after tlbie      */
	addi	r5,r5,0x1000 /* increment bits 15-19 */
	bdnz	tlbloop		 /* decrement CTR, branch if CTR != 0 */
	isync
	blr

/*******************************************************************************
*
* romClearFPRegs
*
* This routine will initialize the FPU's registers.
*
* RETURNS: None
*
*/

romClearFPRegs:
	mflr	r30

	/* Turn on FP */
    li      r3,_PPC_MSR_FP
    mtmsr   r3
    sync

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

    /* Initialize the floating point data registers to a known state */

    bl      ifpdrValue
    .long   0x3f800000      /* IEEE single-precision value of 1.0 */

ifpdrValue:
    mflr    r3
    lfs     f0,0(r3)
    lfs     f1,0(r3)
    lfs     f2,0(r3)
    lfs     f3,0(r3)
    lfs     f4,0(r3)
    lfs     f5,0(r3)
    lfs     f6,0(r3)
    lfs     f7,0(r3)
    lfs     f8,0(r3)
    lfs     f9,0(r3)
    lfs     f10,0(r3)
    lfs     f11,0(r3)
    lfs     f12,0(r3)
    lfs     f13,0(r3)
    lfs     f14,0(r3)
    lfs     f15,0(r3)
    lfs     f16,0(r3)
    lfs     f17,0(r3)
    lfs     f18,0(r3)
    lfs     f19,0(r3)
    lfs     f20,0(r3)
    lfs     f21,0(r3)
    lfs     f22,0(r3)
    lfs     f23,0(r3)
    lfs     f24,0(r3)
    lfs     f25,0(r3)
    lfs     f26,0(r3)
    lfs     f27,0(r3)
    lfs     f28,0(r3)
    lfs     f29,0(r3)
    lfs     f30,0(r3)
    lfs     f31,0(r3)
    sync

    /*
     *     Set MPU/MSR to a known state
     *     Turn off FP
     */

        andi.  r3,r3,0
        sync
        mtmsr  r3
        isync

	mtlr	r30
	bclr	20,0		/* Return to caller */

/******************************************************************************
*
* romSiuInit - initialize the general SIU.
*
*
* RETURNS: N/A.
*/

romSiuInit:

#ifdef INCLUDE_PCI
  lis     r5,HIADJ(0x4C600000)
  addi    r5,r5,LO(0x4C600000)
  stw     r5,PQII_SIUMCR(r4)

  lis     r5,HIADJ(0xFFFFFF83)
  addi    r5,r5,LO(0xFFFFFF83)
  stw     r5,PQII_SYPCR(r4)
#else
  lis     r5,HIADJ(0x4C200000)
  addi    r5,r5,LO(0x4C200000)
  stw     r5,PQII_SIUMCR(r4)

  lis     r5,HIADJ(0xFFFFFFC3)
  addi    r5,r5,LO(0xFFFFFFC3)
  stw     r5,PQII_SYPCR(r4)
#endif

	lis	    r5,HIADJ(0xFFFF0000)
	addi	r5,r5,LO(0xFFFF0000)
	stw		r5,PQII_SWT(r4)

	lis	    r5,HIADJ(0x0000000E)
	addi	r5,r5,LO(0x0000000E)
	sth		r5,PQII_SWSR(r4)

	lis	    r5,HIADJ(0xB0006000)
	addi	r5,r5,LO(0xB0006000)
	stw		r5,PQII_BCR(r4)

	li	    r5,0x02
	stb	    r5,PQII_PPC_ACR(r4)

	lis	    r5,HIADJ(0x01234567)
	addi	r5,r5,LO(0x01234567)
	stw		r5,PQII_PPC_ALRH(r4)

	lis	    r5,HIADJ(0x89ABCDEF)
	addi	r5,r5,LO(0x89ABCDEF)
	stw		r5,PQII_PPC_ALRL(r4)

	li		r5,0x02
	stb		r5,PQII_LCL_ACR(r4)

	lis	    r5,HIADJ(0x01234567)
	addi	r5,r5,LO(0x01234567)
	stw		r5,PQII_LCL_ALRH(r4)

	lis	    r5,HIADJ(0x89ABCDEF)
	addi	r5,r5,LO(0x89ABCDEF)
	stw		r5,PQII_LCL_ALRL(r4)

	lis 	r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_TESCR1(r4)

	lis 	r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_TESCR2(r4)

	lis 	r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_LTESCR1(r4)

	lis 	r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_LTESCR2(r4)

#ifdef INCLUDE_PCI
  lis     r5, HIADJ(M8260_SCCR_DFBRG_4 | M8260_SCCR_PCI_MODE_BIT)  /* divide by 4 */
  addi    r5, r5, LO(M8260_SCCR_DFBRG_4 | M8260_SCCR_PCI_MODE_BIT) /* divide by 4 */
#else
  lis     r5, HIADJ(M8260_SCCR_DFBRG_4)  /* divide by 4 */
  addi    r5, r5, LO(M8260_SCCR_DFBRG_4) /* divide by 4 */
#endif
	stw	    r5, PQII_SCCR(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_PDTEA(r4)

	li  	r5,0x00
	stb	    r5,PQII_PDTEM(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_LDTEA(r4)

	li  	r5,0x00
	stb	    r5,PQII_LDTEM(r4)

	bclr   20,0

/******************************************************************************
*
* romMemcInit - initialize the memory controller and SDRAM.
*
*
* RETURNS: N/A.
*/

romMemcInit:

	mfspr	r30,LR

	bl romChipSelectInit

#ifdef USE_LEDS_FOR_BOOT_DEBUG
	li	r5,LED_ALLOFF			/* end of romChipSelectInit */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	lis	    r5,HIADJ(0x00000200)
	addi	r5,r5,LO(0x00000200)
	stw		r5,PQII_MAR(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_MAMR(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_MBMR(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_MCMR(r4)

	li	    r5,0x3200
	sth		r5,PQII_MPTPR(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_MDR(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_PSDMR(r4)

	li		r5,0x08
	stb		r5,PQII_PURT(r4)

	li		r5,0x13
	stb		r5,PQII_PSRT(r4)

	li		r5,0x00
	stb		r5,PQII_LURT(r4)

	li		r5,0x13
	stb		r5,PQII_LSRT(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_PCIBR0(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_PCIBR1(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_PCIMSK0(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_PCIMSK1(r4)

	li		r5,0x0000
	sth		r5,PQII_RMR(r4)

#ifdef USE_LEDS_FOR_BOOT_DEBUG
	li	r5,LED_0		/* end of basic mem controller init */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	bl	memcSdram60xBusInit

#ifdef USE_LEDS_FOR_BOOT_DEBUG
	li	r5,LED_1		/* end of memcSdram60xBusInit */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

#ifndef INCLUDE_PCI
	bl	memcSdramLocalBusInit
#endif

#ifdef USE_LEDS_FOR_BOOT_DEBUG
	li	r5,LED_2		/* end of memcSdramLocalBusInit */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	mtspr	LR,r30
	bclr	20,0

/******************************************************************************
*
* romChipSelectInit - initialize the PowerQUICC II chip selects
*
*
* RETURNS: N/A.
*/

romChipSelectInit:

	mfspr	r29,LR

	/*********************************************************
	 * CS0/1/6: 64-128MB 32-bit Flash SIMM and 2MB 8-bit Flash
	 *********************************************************/

#if (BOOT_FLASH == ON_BOARD_FLASH)

    lis     r5,HIADJ(0xFE000896) /* CS0 to On Board FLASH */
	addi	r5,r5,LO(0xFE000896)
	stw		r5,PQII_OR0(r4)

	lis	    r5,HIADJ(0xFE000801)
	addi	r5,r5,LO(0xFE000801)
	stw		r5,PQII_BR0(r4)

	lis	    r5,HIADJ(0xFC000896) /* CS1 to SIMM FLASH > 64MB */
	addi	r5,r5,LO(0xFC000896)
	stw		r5,PQII_OR1(r4)

	lis	    r5,HIADJ(0xE4001801)
	addi	r5,r5,LO(0xE4001801)
	stw		r5,PQII_BR1(r4)

	lis	    r5,HIADJ(0xFC000896) /* CS6 to SIMM FLASH */
	addi	r5,r5,LO(0xFC000896)
	stw		r5,PQII_OR6(r4)

	lis	    r5,HIADJ(0xE0001801)
	addi	r5,r5,LO(0xE0001801)
	stw		r5,PQII_BR6(r4)

#else /* (BOOT_FLASH == SIMM_FLASH) */

    lis     r5,HIADJ(0xFC000896) /* CS0 to SIMM FLASH */
	addi	r5,r5,LO(0xFC000896)
	stw		r5,PQII_OR0(r4)

	lis	    r5,HIADJ(0xFC001801)
	addi	r5,r5,LO(0xFC001801)
	stw		r5,PQII_BR0(r4)

	lis	    r5,HIADJ(0xFC000896) /* CS1 to SIMM FLASH > 64MB */
	addi	r5,r5,LO(0xFC000896)
	stw		r5,PQII_OR1(r4)

	lis	    r5,HIADJ(0xE4001801)
	addi	r5,r5,LO(0xE4001801)
	stw		r5,PQII_BR1(r4)

	lis	    r5,HIADJ(0xFC000896) /* CS6 to ON Board FLASH */
	addi	r5,r5,LO(0xFC000896)
	stw		r5,PQII_OR6(r4)

	lis	    r5,HIADJ(0xE0000801)
	addi	r5,r5,LO(0xE0000801)
	stw		r5,PQII_BR6(r4)

#endif /* (BOOT_FLASH == ON_BOARD_FLASH) */


	/****************************************************
	 * CS1: Remaining 32-bit Flash SIMM, with CS0 or CS6
	 ****************************************************/

    /* Set up with CS0 */

	/****************************************************
	 * CS2/CS3: 60x Bus SRAM.
	 ****************************************************/

	lis	    r5,HIADJ(0xF8002500)
	addi	r5,r5,LO(0xF8002500)
	stw		r5,PQII_OR2(r4)

	lis	    r5,HIADJ(0x00000041)
	addi	r5,r5,LO(0x00000041)
	stw		r5,PQII_BR2(r4)

	lis	    r5,HIADJ(0xF8002500)
	addi	r5,r5,LO(0xF8002500)
	stw		r5,PQII_OR3(r4)

	lis	    r5,HIADJ(0x08000041)
	addi	r5,r5,LO(0x08000041)
	stw		r5,PQII_BR3(r4)

	/****************************************************
	 * CS4: Local Bus SRAM.
	 ****************************************************/

#ifndef INCLUDE_PCI
	lis	    r5,HIADJ(0xFF0030C0)
	addi	r5,r5,LO(0xFF0030C0)
	stw		r5,PQII_OR4(r4)

	lis	    r5,HIADJ(0x20001861)
	addi	r5,r5,LO(0x20001861)
	stw		r5,PQII_BR4(r4)
#endif

	/****************************************************
	 * CS5: 8Kb, 8-bit, EEPROM; User Switches/LEDs;
	 *      visionPort; Status; Ext. Interrupt Controller
	 ****************************************************/

#ifdef INCLUDE_PCI
  lis     r5,HIADJ(0xFFF00856)
  addi    r5,r5,LO(0xFFF00856)
  stw     r5,PQII_OR5(r4)
#else
	lis	    r5,HIADJ(0xFFFF0856)
	addi	r5,r5,LO(0xFFFF0856)
	stw		r5,PQII_OR5(r4)
#endif

	lis	    r5,HIADJ(0x22000801)
	addi	r5,r5,LO(0x22000801)
	stw		r5,PQII_BR5(r4)

	/****************************************************
	 * CS6: 64MB 32-bit Flash SIMM or 2MB 8-bit Flash
	 ****************************************************/

    /* Set up with CS0 */

	/****************************************************
	 * CS7: Control EPLD
	 ****************************************************/

	lis	    r5,HIADJ(0xFFFF0856)
	addi	r5,r5,LO(0xFFFF0856)
	stw		r5,PQII_OR7(r4)

	lis	    r5,HIADJ(0x21000801)
	addi	r5,r5,LO(0x21000801)
	stw		r5,PQII_BR7(r4)

	/****************************************************
	 * CS8: ATM - Disabled
	 ****************************************************/

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_OR8(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_BR8(r4)

	/****************************************************
	 * CS9: Spare
	 ****************************************************/

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_OR9(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_BR9(r4)

	/****************************************************
	 * CS10: Spare
	 ****************************************************/

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_OR10(r4)

	lis	    r5,HIADJ(0x00000000)
	addi	r5,r5,LO(0x00000000)
	stw		r5,PQII_OR10(r4)

	/****************************************************
	 * CS11: Real-Time Clock
	 ****************************************************/

	lis	    r5,HIADJ(0xFFFF0836)
	addi	r5,r5,LO(0xFFFF0836)
	stw		r5,PQII_OR11(r4)

	lis	    r5,HIADJ(0x80000801)
	addi	r5,r5,LO(0x80000801)
	stw		r5,PQII_BR11(r4)

	mtspr	LR,r29
	bclr	20,0

/******************************************************************************
*
* memcSdram60xBusInit - initialize 60x Bus SDRAM.
*
*
* RETURNS: N/A.
*/

memcSdram60xBusInit:

	mfspr	r29,LR

	lis		r5,0x0000
	ori		r5,r5,0x0000

	/**********************************************
	 * Initialize 60x Bus SDRAM.
	 **********************************************/

	li		r6,0x13
	stb		r6,PQII_PSRT(r4)

	lis	    r6,HIADJ(0x292EB45A)
	addi	r6,r6,LO(0x292EB45A)
	stw		r6,PQII_PSDMR1(r4)

	li		r6,0xFF
	stb		r6,PQII_MEM0(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM0(r5)

	lis	    r6,HIADJ(0x092EB45A)
	addi	r6,r6,LO(0x092EB45A)
	stw		r6,PQII_PSDMR2(r4)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM1(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM1(r5)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM2(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM2(r5)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM3(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM3(r5)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM4(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM4(r5)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM5(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM5(r5)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM6(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM6(r5)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM7(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM7(r5)

	li		r6,0xFF
	lis     r5,0x0
	stb		r6,PQII_MEM8(r5)

	li		r6,0xFF
	lis		r5,0x0800
	stb		r6,PQII_MEM8(r5)

	lis	    r6,HIADJ(0x192EB45A)
	addi	r6,r6,LO(0x192EB45A)
	stw		r6,PQII_PSDMR3(r4)

	li		r6,0xFF
	lis     r5,0x0
	ori		r5,r5,0x110
	stb		r6,PQII_MEM9(r5)

	li		r6,0xFF
	lis		r5,0x0800
	ori		r5,r5,0x110
	stb		r6,PQII_MEM9(r5)

	lis	    r6,HIADJ(0x412EB45A)
	addi	r6,r6,LO(0x412EB45A)
	stw		r6,PQII_PSDMR4(r4)

	mtspr	LR,r29
	bclr	20,0

/******************************************************************************
*
* memcSdramLocalBusInit - initialize Local Bus SDRAM.
*
*
* RETURNS: N/A.
*/

memcSdramLocalBusInit:

	mfspr	r29,LR

	lis		r5,0x0400
	ori		r5,r5,0x0000

	/**********************************************
	 * Initialize Local Bus SDRAM.
	 **********************************************/

	li		r6,0x13
	stb		r6,PQII_LSRT(r4)

	lis	    r6,HIADJ(0x2866A552)
	addi	r6,r6,LO(0x2866A552)
	stw		r6,PQII_LSDMR1(r4)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM0(r5)

	lis	    r6,HIADJ(0x0866A552)
	addi	r6,r6,LO(0x0866A552)
	stw		r6,PQII_LSDMR2(r4)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM1(r5)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM2(r5)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM3(r5)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM5(r5)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM6(r5)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM7(r5)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM8(r5)

	lis	    r6,HIADJ(0x1866A552)
	addi	r6,r6,LO(0x1866A552)
	stw		r6,PQII_LSDMR3(r4)

	li		r6,0xFF
	lis		r5,0x2000
	stb		r6,PQII_MEM9(r5)

	lis	    r6,HIADJ(0x4066A552)
	addi	r6,r6,LO(0x4066A552)
	stw		r6,PQII_LSDMR4(r4)

	mtspr	LR,r29
	bclr	20,0

/******************************************************************************
*
* romCacheInit -
*
*
*
*/

romCacheInit:
	/* turn the instruction cache ON for faster FLASH ROM boots */

	mfspr	r5,HID0
	ori	    r5,r5,_PPC_HID0_ICE
	isync

	/*
	 * The setting of the instruction cache enable (ICE) bit must be
	 * preceded by an isync instruction to prevent the cache from being
	 * enabled or disabled while an instruction access is in progress.
	 */

	mtspr	HID0,r5
	sync
	isync
	bclr	20,0

