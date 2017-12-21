/* sysClkALib.s - IBM 440GX clock speed determination assembly routines */

/***************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

***************************************************************************/

/* Copyright 2001-2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,11dec03,pch  change MSYNC opcode to lower case
01b,11aug03,jtp  Formal code inspection changes: sysClkPeriodGet takes
		 TLB Entry Number argument and is relocatable.
01a,16may03,pch  moved mulDiv_64 and autoCalcSpeed from sysALib.s
*/

/*
DESCRIPTION

This module supplies a function, autoCalcSpeed, which returns the CPU
frequency. The basis of this calcuation proceeds by configuring the PPC
Core Timebase to count cycles from a known external clock source, and
then counting the number of instructions that occur while it advances.

This is relocatable code.  The initial bootrom code is typically loaded
at a different address than it was linked.  The only memory facility
available to these routines is the stack, initialized to the PPC440's
onchip SRAM.  For these reasons, all code in this module must be
position-independent, make sparing use only of auto (stack) variables,
and must avoid use of data and BSS memory and static variables.
Finally, compiler output must be inspected to make sure that no jump
tables with absolute branches have been generated; switch statements and
if-else if-... statements are the leading culprits.
*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "config.h"
#include "asm.h"
#include "regs.h"

	/* globals */

	FUNC_EXPORT(autoCalcSpeed)
	FUNC_EXPORT(mulDiv64)

	_WRS_TEXT_SEG_START

/***************************************************************************
*
* mulDiv64 - multiply two 32-bit values to 64-bit, divide by a 32-bit value
*
* Two 32-bit values are multiplied into a 64-bit intermediate value,
* then divided by a third 32-bit value to yield a 32-bit result.
*
* Author: Daniel Goehring
*
* RETURNS: UINT32
*	r3 = (r3 * r4) / r5
*

* UINT32 mulDiv64
*	(
*	UINT32	numerator0,	/@ numerator 0 @/
*	UINT32	numerator1,	/@ numerator 1 @/
*	UINT32	denominator	/@ denominator @/
*	);

*/
FUNC_BEGIN(mulDiv64)
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
FUNC_END(mulDiv64)

/*****************************************************************************
*
* autoCalcSpeed - calculate the cpu clock speed
*
* This function uses the external timebase clk as a reference, and
* counts instructions executed during the reference period, to derive
* the CPU clock frequency.
*
* RETURNS
*
* cpu clock speed, or 0 if it could not be determined
*
* INTERNAL
*
* In order to assume one instruction per clock cycle, autoCalcSpeed must
* cache the loop instructions in the i-cache prior to execution. As a
* result, autoCalcSpeed verifies that the cache is enabled on the memory
* location where it is loaded.
*
* The caller must ensure that external interrupts are disabled.
*

* UINT32 autoCalcSpeed
*    (
*    UINT32 tlbNum		/@ TLB Entry number mapping autoCalcSpeed @/
*    )

*/

FUNC_BEGIN(autoCalcSpeed)
	mr	r5,r3		    /* save tlbNum to r5 */

	/*
	 * Clear the counter (the return value)
	 */
	xor   	r3,r3,r3

	/*
	 * Make sure ICache is enabled for tlbNum
	 */
	tlbre	r4,r5,0x0002	    /* get cache info for TLB # tlbNum */
	addi  	r5,r0,0x0400        /* Mask for the TLB cache inhibited bit */
	and   	r4,r4,r5
	cmplw 	r4,r5
	bne   	auto_calc_speed_return	/* inhibited, give up */

	/*
	 * Make sure the external clocking is the source for the timebase
	 */
	lis	r5, HI(_PPC_CCR1_TCS)
	ori  	r5, r5, LO(_PPC_CCR1_TCS)
	lis 	r6, HI(_PPC_CCR1_TCS_EXT)
	ori   	r6, r6, LO(_PPC_CCR1_TCS_EXT)
	mfspr 	r4,CCR1
	and   	r4,r4,r5
	cmplw 	r4,r6
	bne   	auto_calc_speed_return

	/*
	 * Get the external clock frequency base (1 second)
	 */
	lis 	r4, HI(EXT_TIMER_CLK_FREQ)
	ori   	r4, r4, LO(EXT_TIMER_CLK_FREQ)
	
	/*
	 * Check if the external clk supplied frequency is zero.  Return if so.
	 * Divide the count by 128 so that the function runs faster (but less
	 * accurate).
	 */
	cmplwi 	r4,0x0000 
	beq   	auto_calc_speed_return 
	rlwinm	r4,r4,25,7,31

	/*
	 * Bring the loop instructions into the cache
	 *
	 * Remember this is relocatable code and needs to be position
	 * independent. loop1 therefore may be loaded in memory at a
	 * different location than the linker linked it.  We can
	 * discover the current PC by doing a relative branch to getPC,
	 * examining LR, then we can we can synthesize loop1 by finding the
	 * difference between getPC and loop1 and adding that to the PC at
	 * getPC.
	 */

	mflr	r5		     /* save original LR */
	bl	getPC		     /* branch relative to next instr */
getPC:
	mflr	r7		     /* LR contains getPC loaded address */
	mtlr	r5		     /* restore original LR */
	li	r5, (loop1 - getPC)  /* offset between loop1 and getPC */
	add	r5, r5, r7	     /* add to PC of getPC to get real loop1 */
	icbt  	r0,r5              
	addi  	r5,r5,_CACHE_ALIGN_SIZE
	icbt  	r0,r5    
	addi  	r5,r5,_CACHE_ALIGN_SIZE
	icbt 	r0,r5   
	addi  	r5,r5,_CACHE_ALIGN_SIZE
	icbt  	r0,r5  

	/*
	 * Synchronize memory accesses (msync and isync) to make sure
	 * all icbts have completed.
	 * msync: This step forces any data PLB operations that may have
	 *	  been in progress prior operation to complete, thereby
	 *	  allowing subsequent data accesses to be initiated and
	 *	  completed properly.
  	 * isync: All instructions have completed.
	 */
	msync
	isync
	addi  	r7,r0,0x0001

	/*
	 * Check which way the compare should go.
	 */
	mfspr	r5,TBL_R
	add	r9,r4,r5
	cmpw	r9,r4
	blt	loop2
	cmpw	r9,r5
	blt	loop2

	/*
	 * Main loop for counting.
	 */

loop1: 
	addi  	r3,r3,1
	mfspr 	r6,TBL_R
	cmplw 	r6,r9
	bgt   	calculate
	addo   	r3,r3,r7
	addo   	r4,r4,r7   
	cmplw  	r6,r5
	bge    	loop1
	b	calculate
loop2:	
	addi	r3,r3,1
	mfspr	r6,TBL_R
	cmplw	r6,r9
	blt	loop2
	addo	r3,r3,r7
	addo    r4,r4,r7   
	cmplw   r6,r5
	bge     loop2

	/*
	 * Each instruction takes one cycle to execute so multiply counter
	 * by 4 * 8
	 */
calculate:
	rlwinm 	r3,r3,9,0,22

auto_calc_speed_return:
	blr

FUNC_END(autoCalcSpeed)
