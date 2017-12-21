/* sysAUtil.s - system-dependent assembly routines */

/*
 * Copyright (c) 2004-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01f,02aug05,efb Added function to clear both timebase registers.
01e,12apr05,cak Removed the sysCacheFlush() routine.
01d,02feb05,cak Added sysCacheErrorTest routine for testing. 
01c,09nov04,scb Added time base register access support.
01b,11oct04,scb Tab formatting.
01a,22sep04,scb Created, moved some former sysALib.s routines over.
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.
*/

#define _ASMLANGUAGE

/* includes */

#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "regs.h"
#include "asm.h"
#include "mpc8540.h"
#include "sysL2Cache.h"
#include "arch/ppc/mmuE500Lib.h"

/* defines */

#define CACHE_ALIGN_SHIFT       5       /* Cache line size == 2**5 */

/* globals */

	FUNC_EXPORT(sysL2CTLSet)
	FUNC_EXPORT(sysL2CTLGet)
	FUNC_EXPORT(sysInWord)
	FUNC_EXPORT(sysOutWord)
	FUNC_EXPORT(sysInLong)
	FUNC_EXPORT(sysOutLong)
	FUNC_EXPORT(sysInByte)
	FUNC_EXPORT(sysOutByte)
	FUNC_EXPORT(sysPciRead32)
	FUNC_EXPORT(sysPciWrite32)
	FUNC_EXPORT(sysPciInByte)
	FUNC_EXPORT(sysPciOutByte)
	FUNC_EXPORT(sysPciInWord)
	FUNC_EXPORT(sysPciOutWord)
	FUNC_EXPORT(sysPciInLong)
	FUNC_EXPORT(sysPciOutLong)
	FUNC_EXPORT(sysPCGet)   /* get the value of the PC register */
	FUNC_EXPORT(tlbEntryRead)
	FUNC_EXPORT(sysTimeBaseLPut)
	FUNC_EXPORT(sysTimeBaseLGet)
	FUNC_EXPORT(sysTimeBaseUPut)
	FUNC_EXPORT(sysTimeBaseUGet)
	FUNC_EXPORT(sysTimeBaseClr)
	FUNC_EXPORT(sysCacheErrorTest)

/* externals */

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysCacheErrorTest - used to test error handling for L2 cache/SRAM 
*
* VOID sysCacheErrorTest
* (
* UINT32  addr	     	/@ address of cacheable data value @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysCacheErrorTest)
        xor     r0,r0,r0                /* clear r0 */
        dcbz    r0,p0
	dcbtls	1,r0,p0
        lwz     r4,0(p0)
	blr
FUNC_END(sysCacheErrorTest)

/*******************************************************************************
*
* sysL2CTLSet - Set the L2 control register
*
* This routine is used to set the L2 control register.  A special sequence
* is needed to set this register.
*
* VOID sysL2CTLset
* (
* UINT32  val	     	/@ value to write to L2 control register @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysL2CTLSet)
        lis     p1,HIADJ(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
        addi    p1,p1,LO(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
	mbar	0 
	isync
	stw	p0,0(p1)
	lwz	p0,0(p1)
	mbar	0
	isync
	blr
FUNC_END(sysL2CTLSet)

	.balign 32

/*******************************************************************************
*
* sysL2CTLGet - Get the L2 control register
*
* This routine is used to retrieve the contents of the L2 control register. 
*
* UINT32 sysL2CTLGet ()
*
* RETURNS: 
* r3 (p0) = Value read from L2 control register.
*/

FUNC_BEGIN(sysL2CTLGet)
        lis     p1,HIADJ(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
        addi    p1,p1,LO(CCSR_ADDR(CCSBAR,CCSR_L2CTL))
	mbar    0
	isync
	lwz	p0,0x0(p1)
	mbar    0
	isync
	blr
FUNC_END(sysL2CTLGet)

/*****************************************************************************
*
* sysInByte - reads a byte from an io address.
*
* This function reads a byte from a specified io address.
*
* UCHAR sysInByte
* (
* UCHAR *  pAddr		/@ Virtual I/O addr to read from @/
* )
*
* RETURNS: byte from address.
*/

FUNC_BEGIN(sysInByte)
	eieio			/* Sync I/O operation */
	sync
	lbzx	p0,r0,p0	/* Read byte from I/O space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysInByte)
	
/******************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This function writes a byte to a specified io address.
*
* VOID sysOutByte
* (
* UCHAR *  pAddr,		/@ Virtual I/O addr to write to @/
* UCHAR    data			/@ data to be written @/
* )
* 
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutByte)
	stbx	p1,r0,p0	/* Write a byte to PCI space */
	eieio			/* Sync I/O operation */
	sync
	bclr	20,0		/* Return to caller */
FUNC_END(sysOutByte)
	
/*****************************************************************************
*
* sysInWord - reads a word from an address, swapping the bytes.
*
* This function reads a swapped word from a specified 
* address.
*
* USHORT sysInWord
* (
* ULONG  address		/@ addr to read from @/
* )
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.
*/

FUNC_BEGIN(sysInWord)
	eieio			/* Sync I/O operation */
	sync
	lhbrx	p0,r0,p0	/* Read and swap */
	bclr	20,0		/* Return to caller */
FUNC_END(sysInWord)

/*****************************************************************************
*
* sysOutWord - writes a word to an address swapping the bytes.
*
* This function writes a swapped word to a specified 
* address.
*
* VOID sysOutWord
* (
* ULONG address,		/@ Virtual addr to write to @/
* UINT16  data			/@ Data to be written       @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutWord)
	sthbrx	p1,r0,p0	/* Write with swap to address */
	eieio			/* Sync I/O operation */
	sync
	bclr	20,0		/* Return to caller */
FUNC_END(sysOutWord)

/*****************************************************************************
*
* sysInLong - reads a long from an address.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* ULONG sysInLong
* (
* ULONG  address,		/@ Virtual addr to read from @/
* )
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.
*/

FUNC_BEGIN(sysInLong)
	eieio			/* Sync I/O operation */
	sync
	lwbrx	p0,r0,p0	/* Read and swap from address */
	bclr	20,0		/* Return to caller */
FUNC_END(sysInLong)

/******************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* VOID sysOutLong
* (
* ULONG   address,		/@ Virtual addr to write to @/
* ULONG   data		        /@ Data to be written @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutLong)
	stwbrx	p1,r0,p0	/* store data as little-endian */
	eieio			/* Sync I/O operation */
	sync
	bclr	20,0
FUNC_END(sysOutLong)

/******************************************************************************
*
* sysPciRead32 - read 32 bit PCI data
*
* This routine will read a 32-bit data item from PCI (I/O or
* memory) space.
*
* VOID sysPciRead32
* (
* ULONG *  pAddr,		/@ Virtual addr to read from @/
* ULONG *  pResult		/@ location to receive data @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciRead32)
	eieio			/* Sync I/O operation */
	lwbrx	p0,r0,p0	/* get the data and swap the bytes */
	stw	p0,0(p1)	/* store into address ptd. to by p1 */
	bclr	20,0
FUNC_END(sysPciRead32)

/******************************************************************************
*
* sysPciWrite32 - write a 32 bit data item to PCI space
*
* This routine will store a 32-bit data item (input as big-endian)
* into PCI (I/O or memory) space in little-endian mode.
*
* VOID sysPciWrite32
* (
* ULONG *  pAddr,		/@ Virtual addr to write to @/
* ULONG    data			/@ Data to be written @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciWrite32)
	stwbrx	p1,r0,p0	/* store data as little-endian */
	bclr	20,0
FUNC_END(sysPciWrite32)

/*****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* UINT8 sysPciInByte
* (
* UINT8 *  pAddr		/@ Virtual addr to read from @/
* )
*
* RETURNS:
* Returns 8 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xff.
*/

FUNC_BEGIN(sysPciInByte)
	eieio			/* Sync I/O operation */
	lbzx	p0,r0,p0	/* Read byte from PCI space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysPciInByte)
	
/*****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.
*
* USHORT sysPciInWord
* (
* USHORT *  pAddr		/@ Virtual addr to read from @/
* )
*
* RETURNS:
* Returns 16 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffff.
*/

FUNC_BEGIN(sysPciInWord)
	eieio			/* Sync I/O operation */
	lhbrx	p0,r0,p0	/* Read and swap from PCI space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysPciInWord)
	
/*****************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* ULONG sysPciInLong
* (
* ULONG *  pAddr		/@ Virtual addr to read from @/
* )
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.
*/

FUNC_BEGIN(sysPciInLong)
	eieio			/* Sync I/O operation */
	lwbrx	p0,r0,p0	/* Read and swap from PCI space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysPciInLong)
	
/******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* VOID sysPciOutByte
* (
* UINT8  * pAddr,		/@ Virtual addr to write to @/
* UINT8  data			/@ Data to be written       @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutByte)
	stbx	p1,r0,p0	/* Write a byte to PCI space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysPciOutByte)
	
/******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.
*
* VOID sysPciOutWord
* (
* USHORT *  pAddr,		/@ Virtual addr to write to @/
* USHORT  data			/@ Data to be written       @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutWord)
	sthbrx	p1,r0,p0	/* Write with swap to PCI space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysPciOutWord)
	
/******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.
*
* VOID sysPciOutLong
* (
* ULONG *  pAddr,		/@ Virtual addr to write to @/
* ULONG  data			/@ Data to be written       @/
* )
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutLong)
	stwbrx	p1,r0,p0	/* Write big-endian long to little-endian */
	mr	p0,p1		/* PCI space */
	bclr	20,0		/* Return to caller */
FUNC_END(sysPciOutLong)

/******************************************************************************
*
* sysPCGet - Get the value of the PC (Program Counter)
*
* This routine returns the value of the PC.
*
* UINT32 sysPCGet (void)
*
* RETURNS: the Program Counter Register (PC) value.
*/

FUNC_BEGIN(sysPCGet)
	mflr	r4		/* Save LR value */
	bl	Next		/* Set PC */
Next:
	mflr	r3		/* Get PC */
	mtlr	r4		/* Restore LR value */
	blr
FUNC_END(sysPCGet)


/******************************************************************************
*
* tlbEntryRead - Read (and return) a TLB entry.
*
* This routine takes formatted mas0 and mas2 registers and issues a
* 'tlbre' instruction to read the actual TLB entry.  The 4-register
* result is returned to the caller.
*
* void tlbEntryRead
* (
* &mas,		/@ 4-UINT32 structure to hold mas[0-4] on return @/
* mas0,		/@ Formatted mas0 image - input to 'tlbre' @/
* mas2 		/@ Formatted mas2 image - input to 'tlbre' @/
* )
*
* RETURNS: In mas structure, four registers (mas0, mas1, mas2, mas3)
*/

FUNC_BEGIN(tlbEntryRead)
        mtspr  MAS0,r4
	mtspr	MAS2,r5
	tlbre
	mfspr	r6,MAS0
	stw	r6,0(r3)

	mfspr	r6,MAS1
	stw	r6,4(r3)

	mfspr	r6,MAS2
	stw	r6,8(r3)

	mfspr	r6,MAS3
	stw	r6,0xc(r3)

	blr
FUNC_END(tlbEntryRead)

/*******************************************************************************
*
* sysTimeBaseLPut - write to lower half of Time Base register
*
* This routine will write the contents of r3 to the lower half of the time 
* base register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysTimeBaseLPut
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysTimeBaseLPut)
	sync
	mtspr	TBLR_W,r3
	isync
	bclr	20,0
FUNC_END(sysTimeBaseLPut)

/******************************************************************************
*
* sysTimeBaseLGet - Get lower half of Time Base Register
*
* This routine will read the contents the lower half of the Time
* Base Register.
*
* From a C point of view, the routine is defined as follows:
*
* UINT32 sysTimeBaseLGet(void)
*
* RETURNS: value of lower half of time base register (in r3)
*/

FUNC_BEGIN(sysTimeBaseLGet)
	mfspr	r3,TBLR_R
	bclr	20,0
FUNC_END(sysTimeBaseLGet)

/*******************************************************************************
*
* sysTimeBaseUPut - write to upper half of Time Base register
*
* This routine will write the contents of r3 to the upper half of the Time
* Base register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysTimeBaseUPut
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysTimeBaseUPut)
	sync
	mtspr	TBUR_W,r3
	isync
	bclr	20,0
FUNC_END(sysTimeBaseUPut)

/******************************************************************************
*
* sysTimeBaseUGet - Get upper half of Time Base Register
*
* This routine will read the contents of the upper half of the Time
* Base Register.
*
* From a C point of view, the routine is defined as follows:
*
* UINT32 sysTimeBaseUGet(void)
*
* RETURNS: value of upper half of time base register (in r3)
*/

FUNC_BEGIN(sysTimeBaseUGet)
	mfspr	r3,TBUR_R
	bclr	20,0
FUNC_END(sysTimeBaseUGet)

/******************************************************************************
*
* sysTimeBaseClr - Clear the entire Time Base Register to zero
*
* This routine will zero the contents of the lower and upper halves of 
* the Time Base Register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysTimeBaseClr(void)
*
* RETURNS: NA
*/

FUNC_BEGIN(sysTimeBaseClr)
	sync
        xor     r3,r3,r3
	mtspr	TBLR_W,r3
	mtspr	TBUR_W,r3
	isync
	bclr	20,0
FUNC_END(sysTimeBaseClr)
