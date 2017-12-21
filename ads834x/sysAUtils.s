/* sysAUtils.s - Motorola ads826x system utility assembly routines */

/* Copyright 1984-2003 Wind River Systems, Inc. */
#include "copyright_wrs.h"
/*
modification history
--------------------
01b,03oct03,dee  fixes per code review
01a,20aug03,dee  add routines common to romInit.s and sysALib.s
*/

/*
DESCRIPTION
This module contains system utility routines written in assembly
language.

NOTE:  These functions are for use before rom images are uncompressed.
	   They are not available for general system use.
*/

#define	_ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"
#include "cacheLib.h"
#include "config.h"
#include "regs.h"	


	FUNC_EXPORT(sysClearSPRGs)
	FUNC_EXPORT(sysClearMSR)
	FUNC_EXPORT(sysClearSRs)
	FUNC_EXPORT(sysClearIBATs)
	FUNC_EXPORT(sysClearDBATs)
	FUNC_EXPORT(sysClearFPRegs)
	FUNC_EXPORT(sysMulDiv64)
	FUNC_EXPORT(sysAbs)
	FUNC_EXPORT(sysDecGet)
	FUNC_EXPORT(sysDecSet)

/*****************************************************************************
*
* sysClearSPRGs - Clears all 4 SPRGx registers
*
* This function zero's r0 (available working register)
* and clears out all 4 SPRG registers.
*
* RETURNS:
*		NONE

* void  sysClearSPGRs
*		(
*		void		
*		)
*/

FUNC_BEGIN(sysClearSPRGs)
	li		r3, 0
	sync				/* not really needed for SPRG, just for consistency */
	mtspr	SPRG0, r3	/* clear SPRG0 */
	mtspr	SPRG1, r3	/* clear SPRG1 */
	mtspr	SPRG2, r3	/* clear SPRG2 */
	mtspr	SPRG3, r3	/* clear SPRG3 */
	isync				/* need context sync instruction after mtspr */
	blr					/* return to caller */
FUNC_END(sysClearSPRGs)

/*****************************************************************************
*
* sysClearMSR - Clears the MSR register
*
* This function zero's r3 (available working register)
* and writes r3 to the machine state register (MSR).
*
* RETURNS:
*		NONE

* void  sysClearMSR
*		(
*		void		
*		)
*/

FUNC_BEGIN(sysClearMSR)
	li		r3, 0	/* clear r3 except for RI */
	sync			/* need synchronizing instruction before mtspr */
	mtmsr	r3		/* msr is now zeroed out, all ints off */
	isync			/* need context synchronizing instruction after mtmsr */
	blr				/* return to caller */
FUNC_END(sysClearMSR)


/*****************************************************************************
*
* sysClearSRs - Clears the 16 segment registers
*
* This function zero's r3 (available working register)
* and writes r3 to the 16 Segment Registers (SR0 - SR15).
*
* RETURNS:
*		NONE

* void  sysClearSRs
*		(
*		void		
*		)
*/

FUNC_BEGIN(sysClearSRs)
	li		r3, 0
	isync
	mtsr	0, r3
	isync
	mtsr	1, r3
	isync
	mtsr	2, r3
	isync
	mtsr	3, r3
	isync
	mtsr	4, r3
	isync
	mtsr	5, r3
	isync
	mtsr	6, r3
	isync
	mtsr	7, r3
	isync
	mtsr	8, r3
	isync
	mtsr	9, r3
	isync
	mtsr	10, r3
	isync
	mtsr	11, r3
	isync
	mtsr	12, r3
	isync
	mtsr	13, r3
	isync
	mtsr	14, r3
	isync
	mtsr	15, r3
	isync
	blr			/* return to caller */
FUNC_END(sysClearSRs)


/*****************************************************************************
*
* sysClearIBATs - Clears the Instruction BAT registers
*
* This function zero's r3 (available working register)
* and writes r3 Instruction BAT registers: IBAT0U/L - IBAT3U/L
*
* RETURNS:
*		NONE

* void  sysClearIBATs
*		(
*		void		
*		)
*/

FUNC_BEGIN(sysClearIBATs)
	li		r3,0			/* clear r3 */
	isync
	mtspr   IBAT0U,r3		/* SPR 528 (IBAT0U) */
	isync
	mtspr   IBAT0L,r3		/* SPR 529 (IBAT0L) */
	isync
	mtspr	IBAT1U,r3		/* SPR 530 (IBAT1U) */
	isync
	mtspr   IBAT1L,r3		/* SPR 531 (IBAT1L) */
	isync
	mtspr   IBAT2U,r3		/* SPR 532 (IBAT2U) */
	isync
	mtspr   IBAT2L,r3		/* SPR 533 (IBAT2L) */
	isync
	mtspr   IBAT3U,r3		/* SPR 534 (IBAT3U) */
	isync
	mtspr   IBAT3L,r3		/* SPR 535 (IBAT3L) */
	isync
	blr						/* return to caller */
FUNC_END(sysClearIBATs)

/*****************************************************************************
*
* sysClearDBATs - Clears the Data BAT registers
*
* This function zero's r3 (available working register)
* and writes r3 Data BAT registers: DBAT0U/L - DBAT3U/L
*
* RETURNS:
*		NONE

* void  sysClearDBATs
*		(
*		void		
*		)
*/

FUNC_BEGIN(sysClearDBATs)
	li		r3,0			/* clear out r3 */
	mtspr   DBAT0U,r3		/* SPR 536 (DBAT0U) */
	isync
	mtspr   DBAT0L,r3		/* SPR 537 (DBAT0L) */
	isync
	mtspr   DBAT1U,r3		/* SPR 538 (DBAT1U) */
	isync
	mtspr   DBAT1L,r3		/* SPR 539 (DBAT1L) */
	isync
	mtspr   DBAT2U,r3		/* SPR 540 (DBAT2U) */
	isync
	mtspr   DBAT2L,r3		/* SPR 541 (DBAT2L) */
	isync
	mtspr   DBAT3U,r3		/* SPR 542 (DBAT3U) */
	isync
	mtspr   DBAT3L,r3		/* SPR 543 (DBAT3L) */
	isync
	blr						/* return to caller */
FUNC_END(sysClearDBATs)

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
	li		r3,128
	mtctr	r3		    /* CTR = 128  */
	li		r3,0		/* r5 = 0    */
	isync			    /* context sync req'd before tlbie */
tlbloop:
	tlbie	r3
	sync			     /* sync instr req'd after tlbie      */
	addi	r3,r3,0x1000 /* increment bits 15-19 */
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
* void  sysClearFPRegs
*		(
*		void		
*		)
*/

FUNC_BEGIN(sysClearFPRegs)

	/* save current value of MSR */
	mfmsr	r3

	/* Turn on FP */
    li      r4, _PPC_MSR_FP		/* turn on FP so we can init it */
	sync
    mtmsr   r4		
    isync

    /* Init the floating point control/status register */
    mtfsfi  7, 0
    mtfsfi  6, 0
    mtfsfi  5, 0
    mtfsfi  4, 0
    mtfsfi  3, 0
    mtfsfi  2, 0
    mtfsfi  1, 0
    mtfsfi  0, 0
    isync

    /*
	 * Initialize the floating point data registers to a known state.
	 * First save the link register.  By doing a bl to ifpdrValue,
	 * the link register has the address of the IEEE value for 1.0
	 * The lr can now be moved to a register for use as a pointer for
	 * the IEEE value of 1.0
	 */

	mflr	r4		/* save link register so we can use it for */
					/* initializing the fp registers with a good number */
	
    bl      ifpdrValue	/* link register now has pointer to IEEE value */
    .long   0x3f800000	/* IEEE single-precision value of 1.0 */

ifpdrValue:
	mflr	r5			/* move current lr to r3 for use as pointer */
	mtlr	r4			/* restore lr to previous value	*/
	lfs		f0, 0(r5)	/* load all 32 FP regs with 1.0 */
	lfs		f1, 0(r5)
	lfs		f2, 0(r5)
	lfs		f3, 0(r5)
	lfs		f4, 0(r5)
	lfs		f5, 0(r5)
	lfs		f6, 0(r5)
	lfs		f7, 0(r5)
	lfs		f8, 0(r5)
	lfs		f9, 0(r5)
	lfs		f10, 0(r5)
	lfs		f11, 0(r5)
	lfs		f12, 0(r5)
	lfs		f13, 0(r5)
	lfs		f14, 0(r5)
	lfs		f15, 0(r5)
	lfs		f16, 0(r5)
	lfs		f17, 0(r5)
	lfs		f18, 0(r5)
	lfs		f19, 0(r5)
	lfs		f20, 0(r5)
	lfs		f21, 0(r5)
	lfs		f22, 0(r5)
	lfs		f23, 0(r5)
	lfs		f24, 0(r5)
	lfs		f25, 0(r5)
	lfs		f26, 0(r5)
	lfs		f27, 0(r5)
	lfs		f28, 0(r5)
	lfs		f29, 0(r5)
	lfs		f30, 0(r5)
	lfs		f31, 0(r5)
	sync

	/*  Restore MPU/MSR to a known state  */

    mtmsr  r3	/* restore saved value */
    isync
	blr			/* Return to caller */
FUNC_END(sysClearFPRegs)


/***************************************************************************
*
* sysMulDiv64 - multiply two 32-bit values to 64-bit,
* then divide by a 32-bit value
*
* Two 32-bit values are multiplied into a 64-bit intermediate value,
* then divided by a third 32-bit value to yield a 32-bit result.
*
* Author: Daniel Goehring
*
* RETURNS: UINT32
*	r3 = (r3 * r4) / r5
*

* UINT32 sysMulDiv64
*	(
*	UINT32	numerator0,	/@ numerator 0 @/
*	UINT32	numerator1,	/@ numerator 1 @/
*	UINT32	denominator	/@ denominator @/
*	);

*/
FUNC_BEGIN(sysMulDiv64)
	mullw   r6, r3, r4      /* R6 = prod[32:63]  low word */
	mulhw   r7, r3, r4      /* R7 = prod[00:31]  high word */

	mfspr   r8, CTR
	addi    r3, r0, 0x20    	/* Loop 32 times. */
	mtspr   CTR, r3
	xor     r3, r3, r3      	/* R3 = result of the divide */
	xor     r4, r4, r4      	/* R4 = 0 */

dLoop:
	rlwinm  r3, r3, 0x1, 0, 30	/* Shift left 1 bit */
	subf    r7, r4, r7
	rlwinm  r7, r7, 0x1, 0, 30	/* Shift left 1 bit */
	rlwinm  r4, r6, 0x1, 31, 31	/* Grab the lower bit  */
	rlwinm  r6, r6, 0x1, 0, 30	/* Shift left 1 bit */
	or      r7, r7, r4		/* Or the two together */
	divw    r4, r7, r5
	add     r3, r3, r4
	mullw   r4, r4, r5
	bdnz    dLoop

	mtspr   CTR, r8
	blr
FUNC_END(sysMulDiv64)

/*******************************************************************************
*
* sysAbs
*
* This routine will get the absolute value of a signed 32 bit number.
*
* RETURNS: | number |
*
* uint32_t  sysAbs
*		(
*		int32_t		
*		)
*/

FUNC_BEGIN(sysAbs)
        srawi	r0, r3, 0x1f
		xor		r3, r0, r3
		subf	r3, r0, r3
        blr
FUNC_END(sysAbs)

/*******************************************************************************
*
* sysDecGet
*
* This routine will get the current value of the Decrementer.
*
* RETURNS: None
*
* uint32_t  sysDecGet
*		(
*		void		
*		)
*/

FUNC_BEGIN(sysDecGet)
        mfdec   p0
        blr
FUNC_END(sysDecGet)

/*******************************************************************************
*
* sysDecSet
*
* This routine will set the value of the Decrementer to the passed parameter.
*
* RETURNS: None
*
* uint32_t  sysDecGet
*		(
*		uint32_t		
*		)
*/

FUNC_BEGIN(sysDecSet)
        mtdec   p0
        blr
FUNC_END(sysDecSet)

