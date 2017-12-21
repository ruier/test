/* sysALib.s - Wind River Systems virtual board for guestOs */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,01jul09,d_c  VBI 2.0 support
01i,03apr09,rec  WIND00144306 - guestOs MSR management
01h,03feb09,rec  remove remaining references to razor
01g,07jan09,rec  change razorConfig to wrhvVbConfig
01f,23dec08,rec  change razorControl to wrhvVbControl
01e,06nov08,dtr  Fix stw usage.
01d,30sep08,md   added sysOutLong, sysOutByte functions
01c,17may07,foo  Update with more Razor passed parameters	
01b,23apr07,foo  added Razor specific startup code	
01a,31jan07,zl   created.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <asm.h>
#include <sysLib.h>
#include "config.h"
#include <wrhv/vbInterface.h>	
	
	FUNC_EXPORT(sysInit)
	FUNC_EXPORT(_sysInit)
	FUNC_EXPORT(sysOutLong)
	FUNC_EXPORT(sysOutByte)

	FUNC_IMPORT(usrInit)
	
	/* DC TODO - wrhvVbConfig/Status/Control should be defined in VBI. */
	
#if (VBI_VERSION_MAJOR == 1) || 1

	DATA_IMPORT(wrhvVbConfig)
	DATA_IMPORT(wrhvBootType)
#if (VBI_VERSION_MAJOR == 1)
	DATA_IMPORT(wrhvId)
#endif /* (VBI_VERSION_MAJOR == 1) */
	DATA_IMPORT(wrhvInSimics)
	
	DATA_IMPORT(wrhvVbStatus)
	DATA_IMPORT(wrhvVbControl)
	
#endif /* (VBI_VERSION_MAJOR == 1) */
	
	_WRS_TEXT_SEG_START
	
/***************************************************************************
*
* sysInit - start after boot
*
* This is the system start-up entry point for VxWorks guest OS.
* It  sets up the stack, initialized the Virtual Board Interface (VBI)
* and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
* 
* SYNOPSIS
*
* The prototype of this function depends on the version of VBI in use.
*	
* For VBI 1.x:		
*
* \ss
* sysInit
*     (
*     VB_CONFIG *wrhvVbConfig,     /@ VB config structure pointer @/
*     int       wrhvBootType       /@ boot option bit mask @/
*     int       wrhvId             /@ virtual board ID @/
*     int       wrhvInSimics       /@ true if running in Simics simulator @/
*     )
* \se
*
* For VBI 2.x:	
*
* \ss
* sysInit
*     (
*     VB_CONFIG *wrhvVbConfig,     /@ VB config structure pointer @/
*     int       wrhvBootType       /@ boot option bit mask @/
*     )
* \se
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*/

FUNC_LABEL(_sysInit)		
FUNC_BEGIN(sysInit)
	
	/* initialize the stack pointer */
	
	lis     sp, HIADJ(RAM_LOW_ADRS)
	addi    sp, sp, LO(RAM_LOW_ADRS)
        addi    sp, sp, -FRAMEBASESZ    /* get frame stack */
	
	/* Initialize the wrhv Configuration */

	/* DC TODO - Fix init sequence when vbiInit() is implemented */
	
#if (VBI_VERSION_MAJOR == 2)
	
	/* Version 2 (and later) VBI initialization */
#if 0
	bl	vbiInit
#endif
	
	/* store paramters wrhvVbControl and wrhvBootType */
	
	lis     r7, HIADJ(wrhvVbConfig)
	addi    r7, r7, LO(wrhvVbConfig)
	stw     p0, 0(r7)

	lis     r7, HIADJ(wrhvBootType)
	addi    r7, r7, LO(wrhvBootType)
	stw     p1, 0(r7)

	/* Initialize wrhvInSimics to zero and retrieve board ID */
	
	lis     r7, HIADJ(wrhvInSimics)
	li      r9, 0
	stw     r9, LO(wrhvInSimics)(r7)
	
	/* Retrive status and control pointers from config structure and
	 * initialize local pointers.
	 */
	
	lis     r7, HIADJ(wrhvVbStatus)
	lwz	r9, VB_CONFIG_VBSTATUS(p0)
	stw	r9, LO(wrhvVbStatus)(r7)
	
	lis     r7, HIADJ(wrhvVbControl)        
	lwz	r9, VB_CONFIG_VBCONTROL(p0)
	stw	r9, LO(wrhvVbControl)(r7)

		
#elif (VBI_VERSION_MAJOR == 1)
	
	/* Version 1.x VBI initialization */
	
	lis     r7, HIADJ(wrhvInSimics)
	addi    r7, r7, LO(wrhvInSimics)
	stw     r6, 0(r7)
	
	lis     r7, HIADJ(wrhvId)
	addi    r7, r7, LO(wrhvId)
	stw     r5, 0(r7)
	
	lis     r7, HIADJ(wrhvBootType)
	addi    r7, r7, LO(wrhvBootType)
	stw     r4, 0(r7)
	
	lis     r7, HIADJ(wrhvVbConfig)
	addi    r7, r7, LO(wrhvVbConfig)
	stw     r3, 0(r7)
	
	lis     r8, HIADJ(wrhvVbStatus)
	lwz	r9, VB_CONFIG_VBSTATUS(r3)
	stw	r9, LO(wrhvVbStatus)(r8)
	
	lis     r8, HIADJ(wrhvVbControl)        
	lwz	r9, VB_CONFIG_VBCONTROL(r3)
	stw	r9, LO(wrhvVbControl)(r8)

#endif /* (VBI_VERSION_MAJOR == 1) */

	li	p1, -1
	stw	p1, VB_CONTROL_INT_DISABLE(r9)  /* lock interrupts */
	
        li      r3, BOOT_WARM_AUTOBOOT /* set the default boot code */

	b usrInit

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
        stwbrx  p1,r0,p0	/* store data as little-endian */
	eieio			/* Sync I/O operation */
	sync
        bclr    20,0
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
	sync
	bclr	20,0		/* Return to caller */
FUNC_END(sysOutByte)
