/* sysALib.s - Motorola ads827x system-dependent assembly routines */

/*
 * Copyright (c) 1999, 2001-2003, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,21aug12,l_z  Better implementation for sysPCGet. (WIND00076160)
01h,07oct03,dee  add reference to romSdramInit() to force linking with
                 resident images
01g,01oct03,dee  add common routines for bootrom and vxWorks
01f,24jan03,dtr  Adding PCI Errata workaround. Also enabled machine check pin.
01e,24jun02,dtr  Adding PCI support.
01d,08may01,pch  Add assembler abstractions (FUNC_EXPORT, FUNC_BEGIN, etc.)
01c,15jul99,ms_  make compliant with our coding standards
01b,27apr99,ms_  add cache/mmu initialization
01a,07jan99,ms_  written from mtx603 sysALib.s
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.

This module must be the first specified in the \f3ld\f1 command used to
build the system.  The sysInit() routine is the system start-up code.
*/

#define _ASMLANGUAGE

#include "vxWorks.h"
#include "asm.h"
#include "cacheLib.h"
#include "arch/ppc/cachePpcLib.h"
#include "config.h"
#include "regs.h"	
#include "sysLib.h"

	/* globals */

	FUNC_EXPORT(_sysInit)		/* start of system code */
	FUNC_EXPORT(sysMsrGet)		/* get the value of the MSR register */
#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND
	FUNC_EXPORT(sysWord)
	FUNC_EXPORT(sysByte)
	FUNC_EXPORT(sysLong)
#else
	FUNC_EXPORT(sysInWord)
	FUNC_EXPORT(sysInByte)
	FUNC_EXPORT(sysInLong)
#endif /* PCI_BRIDGE_READ_ERRATA_WORKAROUND */
	FUNC_EXPORT(sysOutLong)
	FUNC_EXPORT(sysOutWord)
	FUNC_EXPORT(sysOutByte)
	FUNC_EXPORT(sysPciRead32)
	FUNC_EXPORT(sysPciWrite32)
	FUNC_EXPORT(sysPciInByte)
	FUNC_EXPORT(sysPciOutByte)
	FUNC_EXPORT(sysPciInWord)
	FUNC_EXPORT(sysPciOutWord)
	FUNC_EXPORT(sysPciInLong)
	FUNC_EXPORT(sysPciOutLong)

	/* externals */
        FUNC_EXPORT(sysPCGet)   /* get the value of the PC register */

	FUNC_IMPORT(usrInit)

	_WRS_TEXT_SEG_START

/****************************************************************************
*
* sysInit - start after boot
*
* This is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  This code does the following:
*
* .CS
*		sets the MSR to all 0 which, among other things:
*		 disables external interrupts
*		 disables machine check interrupts
*		 selects exception prefix 0x000n_nnnn
*		 disables instruction and data address translation
*		clears the SPRGs
*		clears the segment registers
*		places an rfi instruction at the decrementer exception vector
*		initializes the stack pointer
*		disables instruction and data translations in the MMU
*		initializes the BAT registers
*		invalidates all entries in instruction and data TLBs
*		clear HID0 , among other things:
*		 disables instruction and data caches
*		invalidates instruction and data caches
*		sets up the stack
* .CE
*
* At the end it jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*
* sysInit (void)				/@ THIS IS NOT A CALLABLE ROUTINE @/
*
*/

FUNC_BEGIN(_sysInit)

	/*
	 * Initialize all necessary core registers before continuing
	 *
	 * 1. Clear MSR to disable all exceptions, no mmu, no fpu etc
	 * 2. Clear out the SPRGs
	 * 3. Clear out the Segment Registers
	 * 4. Clear Instruction BAT registers
	 * 5. Clear Data BAT registers
	 * 6. Initialize FPU registers
	 *
	 */

	/* initialize the stack pointer */

	mr      r9, r3
	lis	sp, HIADJ(RAM_LOW_ADRS)
	addi	sp, sp, LO(RAM_LOW_ADRS)
	addi	sp, sp, -FRAMEBASESZ	/* get frame stack */

	bl		sysClearMSR
	bl		sysClearSPRGs
	bl		sysClearSRs
	bl		sysClearIBATs
	bl		sysClearDBATs
	bl		sysClearFPRegs

	/* return from decrementer exceptions */

	li		r0, 0
	bl		loadrfi			/* link reg now had address of rfi */
	rfi
loadrfi:
	mflr	r4				/* r4 now had address of rfi instruction */
	lwz		r4, 0(r4)		/* r4 now has opcode of rfi instruction */
	stw		r4, 0x900(r0)	/* rfi instruction now in decr exception vec */



	/* invalidate entries within both TLBs */

	li		r4, 32
	mtctr	r4			/* CTR = 32  */
	li		r3, 0		/* r3 = 0	*/

	isync				/* context sync req'd before tlbie */
sysALoop:
	tlbie   r3
	addi	r3,r3,0x1000	/* increment bits 15-19 */
	bdnz	sysALoop		/* decrement CTR, branch if CTR != 0 */
	sync					/* sync instr req'd after tlbie		*/


	/* clear HID0 */

	li		r3, 0
	sync
	isync
	mtspr   HID0, r3

	/*
	 * Invalidate both caches by setting and then clearing the cache
	 * invalidate bits
	 */

	mfspr   r3, HID0
	lis		r3, HIADJ(_PPC_HID0_ICFI | _PPC_HID0_DCFI | _PPC_HID0_PAR | _PPC_HID0_EMCP)	/* set bits */
	ori		r3, r3, LO(_PPC_HID0_ICFI | _PPC_HID0_DCFI | _PPC_HID0_PAR | _PPC_HID0_EMCP) /* set bits */
	sync
	isync
	mtspr   HID0, r3

	/* clear HID0, which clears cache invalidate bits */

	li		r3, 0
	sync
	isync
	mtspr   HID0, r3

	mr		r3, r9 /* set the startType */
	/* jump to usrInit */
	b   usrInit			/* never returns - starts up kernel */
FUNC_END(_sysInit)

/****************************************************************************
*
* sysMsrGet - Get the value of the MSR (Machine State Register)
*
* This routine returns the value of the MSR.
*
* RETURNS: the Machine State Register (MSR) value.
*
* UINT32 sysMsrGet (void)
*
*/

FUNC_BEGIN(sysMsrGet)
	mfmsr   retval0 /* put msr value into EABI return register */
	blr				/* Return to caller */
FUNC_END(sysMsrGet)


#ifndef PCI_BRIDGE_READ_ERRATA_WORKAROUND	
/****************************************************************************
*
* sysInByte - reads a byte from an io address.
*
* This function reads a byte from a specified io address.
*
* RETURNS: byte from address.
*
* UCHAR sysInByte
*		(
*		UCHAR *  pAddr		/@ Virtual I/O addr to read from @/
*		)
*
*/

FUNC_BEGIN(sysInByte)
	eieio					/* Sync I/O operation */
	sync
	lbzx	retval0,r0,p0	/* Read byte from I/O space */
	blr						/* Return to caller */
FUNC_END(sysInByte)


/****************************************************************************
*
* sysInWord - reads a word from an address, swapping the bytes.
*
* This function reads a swapped word from a specified
* address.
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.

* USHORT sysInWord
*		(
*		ULONG  address,		/@ addr to read from @/
*		)

*/

FUNC_BEGIN(sysInWord)
	eieio					/* Sync I/O operation */
	sync
	lhbrx   retval0,r0,p0	/* Read and swap */
	blr						/* Return to caller */
FUNC_END(sysInWord)

/****************************************************************************
*
* sysInLong - reads a long from an address.
*
* This function reads a long from a specified PCI Config Space
* (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.
*
* ULONG sysInLong
*		(
*		ULONG  address,		/@ Virtual addr to read from @/
*		)
*/

FUNC_BEGIN(sysInLong)
	eieio					/* Sync I/O operation */
	sync
	lwbrx   retval0,r0,p0	/* Read and swap from address */
	blr						/* Return to caller */
FUNC_END(sysInLong)
#else
/****************************************************************************
*
* sysByte - reads a byte from an io address.
*
* This function reads a byte from a specified io address.
*
* RETURNS: byte from address.
*
* UCHAR sysByte
*		(
*		UCHAR *  pAddr		/@ Virtual I/O addr to read from @/
*		)
*
*/

FUNC_BEGIN(sysByte)
	lbzx	retval0,r0,p0	/* Read byte from I/O space */
	blr						/* Return to caller */
FUNC_END(sysByte)


/****************************************************************************
*
* sysWord - reads a word from an address, swapping the bytes.
*
* This function reads a swapped word from a specified
* address.
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.
*
* USHORT sysWord
*		(
*		ULONG  address,		/@ addr to read from @/
*		)
*
*/

FUNC_BEGIN(sysWord)
	lhbrx   retval0,r0,p0	/* Read and swap */
	blr						/* Return to caller */
FUNC_END(sysWord)

/***************************************************************************
*
* sysLong - reads a long from an address.
*
* This function reads a long from a specified little-endian
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.
* ULONG sysLong
*		(
*		ULONG  address,		/@ Virtual addr to read from @/
*		)
*
*/

FUNC_BEGIN(sysLong)
	lwbrx   retval0,r0,p0	/* Read and swap from address */
	blr						/* Return to caller */
FUNC_END(sysLong)
#endif

/****************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This function writes a byte to a specified io address.
*
* RETURNS: N/A
*
* VOID sysOutByte
*		(
*		UCHAR *  pAddr,		/@ Virtual I/O addr to write to @/
*		UCHAR	data		/@ data to be written @/
*		)
*
*/

FUNC_BEGIN(sysOutByte)
	stbx	p1,r0,p0	/* Write a byte to PCI space */
	eieio				/* Sync I/O operation */
	sync
	blr					/* Return to caller */
FUNC_END(sysOutByte)


/***************************************************************************
*
* sysOutWord - writes a word to an address swapping the bytes.
*
* This function writes a swapped word to a specified
* address.
*
* RETURNS: N/A
*
* VOID sysOutWord
*		(
*		ULONG address,		/@ Virtual addr to write to @/
*		UINT16  data		/@ Data to be written		@/
*		)
*
*/

FUNC_BEGIN(sysOutWord)
	sthbrx  p1,r0,p0	/* Write with swap to address */
	eieio			/* Sync I/O operation */
	sync
	blr				/* Return to caller */
FUNC_END(sysOutWord)


/***************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* RETURNS: N/A
*
* VOID sysOutLong
*		(
*		ULONG   address,		/@ Virtual addr to write to @/
*		ULONG   data			/@ Data to be written @/
*		)
*
*/

FUNC_BEGIN(sysOutLong)
	stwbrx  p1,r0,p0	/* store data as little-endian */
	eieio				/* Sync I/O operation */
	sync
	blr					/* Return to caller */
FUNC_END(sysOutLong)

/****************************************************************************
*
* sysPciRead32 - read 32 bit PCI data
*
* This routine will read a 32-bit data item from PCI (I/O or
* memory) space.
*
* RETURNS: N/A
*
* VOID sysPciRead32
*		(
*		ULONG *  pAddr,		/@ Virtual addr to read from @/
*		ULONG *  pResult		/@ location to receive data @/
*		)
*
*/

FUNC_BEGIN(sysPciRead32)
	eieio				/* Sync I/O operation */
	lwbrx   p0,r0,p0	/* get the data and swap the bytes */
	stw		p0,0(p1)	/* store into address ptd. to by p1 */
	blr					/* Return to caller */
FUNC_END(sysPciRead32)

/****************************************************************************
*
* sysPciWrite32 - write a 32 bit data item to PCI space
*
* This routine will store a 32-bit data item (input as big-endian)
* into PCI (I/O or memory) space in little-endian mode.
*
* RETURNS: N/A
*
* VOID sysPciWrite32
*		(
*		ULONG *  pAddr,		/@ Virtual addr to write to @/
*		ULONG   data		/@ Data to be written @/
*		)
*
*/

FUNC_BEGIN(sysPciWrite32)
	stwbrx  p1,r0,p0	/* store data as little-endian */
	blr					/* Return to caller */
FUNC_END(sysPciWrite32)

/****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* RETURNS:
* Returns 8 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xff.
*
* UINT8 sysPciInByte
*		(
*		UINT8 *  pAddr,		/@ Virtual addr to read from @/
*		)
*
*/

FUNC_BEGIN(sysPciInByte)
	eieio					/* Sync I/O operation */
	lbzx	retval0,r0,p0	/* Read byte from PCI space */
	blr						/* Return to caller */
FUNC_END(sysPciInByte)

/****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space
* (little-endian)
* address.
*
* RETURNS:
* Returns 16 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffff.
*
* USHORT sysPciInWord
*		(
*		USHORT *  pAddr,		/@ Virtual addr to read from @/
*		)
*/

FUNC_BEGIN(sysPciInWord)
	eieio					/* Sync I/O operation */
	lhbrx   retval0,r0,p0	/* Read and swap from PCI space */
	blr						/* Return to caller */
FUNC_END(sysPciInWord)

/***************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space
* (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.
*
* ULONG sysPciInLong
*		(
*		ULONG *  pAddr,		/@ Virtual addr to read from @/
*		)
*
*/

FUNC_BEGIN(sysPciInLong)
	eieio					/* Sync I/O operation */
	lwbrx   retval0,r0,p0	/* Read and swap from PCI space */
	blr						/* Return to caller */
FUNC_END(sysPciInLong)

/****************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* RETURNS: N/A
*
* VOID sysPciOutByte
*		(
*		UINT8 *  pAddr,		/@ Virtual addr to write to @/
*		UINT8  data			/@ Data to be written		@/
*		)
*
*/

FUNC_BEGIN(sysPciOutByte)
	stbx	p1,r0,p0	/* Write a byte to PCI space */
	blr					/* Return to caller */
FUNC_END(sysPciOutByte)

/****************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A
*
* VOID sysPciOutWord
*		(
*		USHORT *  pAddr,		/@ Virtual addr to write to @/
*		USHORT  data			/@ Data to be written		@/
*		)
*
*/

FUNC_BEGIN(sysPciOutWord)
	sthbrx  p1,r0,p0	/* Write with swap to PCI space */
	blr					/* Return to caller */
FUNC_END(sysPciOutWord)

/***************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A
*
* VOID sysPciOutLong
*		(
*		ULONG *  pAddr,		/@ Virtual addr to write to @/
*		ULONG  data			/@ Data to be written		@/
*		)
*
*/

FUNC_BEGIN(sysPciOutLong)
	stwbrx  p1,r0,p0	/* Write big-endian long to little-endian */
	mr		p0,p1		/* PCI space */
	blr					/* Return to caller */
FUNC_END(sysPciOutLong)


	
/******************************************************************************
*
* sysPCGet - Get the value of the PC (Program Counter)
*
* This routine returns the value of the PC.
*
* SYNOPSIS
* \ss
* UINT32 sysPCGet
*     (
*     void
*     )
* \se
*
* RETURNS: the Program Counter Register (PC) value.
*/

FUNC_BEGIN(sysPCGet)
	mflr    r3
	blr
FUNC_END(sysPCGet)
