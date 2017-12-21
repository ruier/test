/* sysALib.s - wrVbP4080 system-dependent assembly routines */

/*
 * Copyright (c) 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,12jun10,rab  Add smp guest support
01b,22mar10,wap  Add sysEprGet for direct interrupt support
01a,13nov09,to   created
*/

/*
DESCRIPTION
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <vsbConfig.h>
#include <asm.h>
#include <sysLib.h>
#include <wrhv/vbi.h>
#include "config.h"

	FUNC_EXPORT(sysInit)
	FUNC_EXPORT(_sysInit)
	FUNC_EXPORT(sysOutLong)
	FUNC_EXPORT(sysOutByte)
	FUNC_EXPORT(sysEprGet)
	FUNC_EXPORT(sysSmpTimeBaseSet)
	FUNC_EXPORT(sysPirGet)

	FUNC_IMPORT(usrInit)

	DATA_IMPORT(wrhvVbConfig)
	DATA_IMPORT(wrhvVbStatus)
	DATA_IMPORT(wrhvVbControl)

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysInit - start after boot
*
* This is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up
* the stack, and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A

* sysInit (VB_CONFIG * config)      /@ THIS IS NOT A CALLABLE ROUTINE @/

*/

FUNC_LABEL(_sysInit)
FUNC_BEGIN(sysInit)

	lis	sp, HIADJ(RAM_LOW_ADRS)
	addi	sp, sp, LO(RAM_LOW_ADRS)
	addi	sp, sp, -FRAMEBASESZ

	lis	p7, HIADJ(wrhvVbConfig)
	stw	p0, LO(wrhvVbConfig)(p7)

	lwz	p6, VB_CONFIG_VBSTATUS(p0)
	lis	p7, HIADJ(wrhvVbStatus)
	stw	p6, LO(wrhvVbStatus)(p7)

	lwz	p6, VB_CONFIG_VBCONTROL(p0)
	lis	p7, HIADJ(wrhvVbControl)
	stw	p6, LO(wrhvVbControl)(p7)

#ifdef _WRS_CONFIG_SMP
	lwz     p6, VB_CONFIG_VCORE_ID(p0) 
	cmpwi   p6, 0
	beq     1f

	li	p0, BOOT_NORMAL
	b       sysCpuLoop
1:
#endif /* _WRS_CONFIG_SMP */

	li	p0, BOOT_NORMAL
	b	usrInit

FUNC_END(sysInit)


/******************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* RETURNS: N/A

* VOID sysOutLong
*     (
*     ULONG   address,		/@ Virtual addr to write to @/
*     ULONG   data		/@ Data to be written @/
*     )

*/

FUNC_BEGIN(sysOutLong)
	stwbrx	p1,r0,p0	/* store data as little-endian */
	eieio			/* Sync I/O operation */
	blr
FUNC_END(sysOutLong)

/******************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This function writes a byte to a specified io address.
*
* RETURNS: N/A

* VOID sysOutByte
*     (
*     UCHAR *  pAddr,		/@ Virtual I/O addr to write to @/
*     UCHAR    data		/@ data to be written @/
*     )

*/

FUNC_BEGIN(sysOutByte)
	stbx	p1,r0,p0	/* Write a byte to PCI space */
	eieio			/* Sync I/O operation */
	blr
FUNC_END(sysOutByte)

/*******************************************************************************
*
* sysEprGet - this routine returns the vector value in EPR.
*
* RETURN: Value in External Proxy Register (EPR).
*/

FUNC_BEGIN(sysEprGet)
        mfspr  r3, EPR
        isync
        blr
FUNC_END(sysEprGet)

