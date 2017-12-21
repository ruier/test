/* sysALib.s - sysInit assembly routine */

/* Copyright 1984-2002 Wind River Systems, Inc. */
/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

#include "copyright_wrs.h"

/*
modification history
--------------------
01o,11dec06,jch  (WRS) Updated for VxWorks 6.x compatibility.
01n,21mar05,efb  Removed unneeded #include mcp8540.h to fix compile error.
01m,07feb05,cak  Moved sysCacheFlush to sysAUtil.s. 
01l,11oct04,scb  Remove useless LAWBAR4 programming.
01k,22sep04,scb  Moved some functions out of here to 'sysAUtil.s'.
01j,23aug04,scb  Starting point for "Big Easy".
01i,22oct03,dtr  Removing compiler warning.
01h,02oct03,dtr  Ensure contiguous cache flush.
01g,17sep03,dtr  Remove LCRR update and removing PCI CAM entry.Set LAWBAR4 
	         to cover max flash of 16MB.
01f,04sep03,dtr  Adding in stack frame pointer init.
01e,02sep03,dtr  Adding sysPCGet function to query if rom resident image.
01d,04aug03,dtr  Fix vision vxworks image.
01c,29jul03,dtr  Removing magic numbers.
01b,07jul03,mil  Added VISION_VXWORKS for no bootrom and local bus access
                 window with bootrom.
01a,03oct02,dtr  Created.
*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"
#include "config.h"
#include "sysLib.h"
	
	FUNC_EXPORT(sysInit)
	FUNC_EXPORT(_sysInit)
	FUNC_IMPORT(usrInit)

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysInit - start after boot
*
* This is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables cacheing sets up
* the stack, and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*/

FUNC_LABEL(_sysInit)
FUNC_BEGIN(sysInit)
	or	r31,r3,r3		/* save startType */
	xor	p0,p0,p0
	mtspr	TCR,p0
	xor	r6,r6,r6
	msync
	isync
	mtspr	L1CSR0,r6		/* Disable the Data cache */
	li	r6,0x0002
	msync
	isync
	mtspr	L1CSR0,r6		/* Invalidate the Data cache */

	li	r6,0x0000
	msync
	isync
	mtspr	L1CSR1,r6		/* Disable the Instruction cache */
	li	r6,0x0002
	msync
	isync
	mtspr	L1CSR1,r6		/* Invalidate the Instruction cache */

	/* initialize the stack	pointer	*/

	lis	sp,HIADJ(RAM_LOW_ADRS)
	addi	sp,sp,LO(RAM_LOW_ADRS)
	addi	sp,sp,-FRAMEBASESZ	/* get frame stack */
	or	r3,r31,r31		/* restore startType */
	b	usrInit
FUNC_END(sysInit)

	.balign	32
