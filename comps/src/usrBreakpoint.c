/* usrBreakpoint.c - user configurable breakpoint management */

/* 
 * Copyright (c) 1998-2007, 2010 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */ 

/*
modification history
--------------------
01h,21sep10,rlp  Added breakpoint write mode routine (WIND00038087).
01g,27sep07,rlp  Added a cache flush policy parameter to usrBreakpointSet()
                 (CQ:WIND00104297, CQ:WIND00104323, CQ:WIND00104325).
01f,30oct06,rlp  Modified for VxDBG library (REQ00007655).
01e,12apr06,dbt  Fixed cache support (SPR #119927).
01d,03aug05,bpn  Do not updated cache if the write into TEXT area failed
                 (SPR#108298).
01c,27nov03,elg  Memory context is needed to unprotect page before installing
                 breakpoint.
01b,02oct03,elg  Rework way breakpoints are planted.
01a,13jan98,dbt  written.
*/

/*
DESCRIPTION

This file contains user configurable breakpoint management routines used 
by target shell debugger and WDB debugger.
This file is included by usrConfig.c.

SEE ALSO: usrExtra.c

NOMANUAL
*/

#ifndef	__INC_usrBreakpointc
#define	__INC_usrBreakpointc

/* includes */

#include <vxWorks.h>
#include <cacheLib.h>

#include <private/vxdbgLibP.h>
#include <private/vxdbgUtilLibP.h>
#include <private/rtpLibP.h>

/* typedefs */

typedef enum bpMemWrite
    {
    MEM_WRITE_DIRECT	= 0,	/* Bp instruction write directly in memory */
    MEM_WRITE_BY_VM	= 1	/* VM library used to write Bp instruction */
    } BP_MEM_WRITE;

/* statics */

LOCAL BP_MEM_WRITE bpMemWriteMode = MEM_WRITE_BY_VM;

#ifdef  INCLUDE_WDB_PRE_KERNEL_INIT
LOCAL BOOL	bpMemWriteModeDetermined = FALSE;
#endif	/* INCLUDE_WDB_PRE_KERNEL_INIT */

/******************************************************************************
*
* usrBreakpointSet - set a text breakpoint.
*
* This routine is used by both target shell and WDB debuggers to set a 
* text breakpoint in memory.
* This routine can be modified by the user to set a breakpoint differently
* (for example to set a text breakpoint in a ROM emulator).
*
* RETURNS : N/A
*
* NOMANUAL
*/ 

void usrBreakpointSet
    (
    INSTR *		addr,		/* breakpoint address */
    INSTR 		value,		/* breakpoint instruction */
    RTP_ID		rtpId,		/* process ID */
    VXDBG_CACHE_POLICY	policy		/* cache flush policy */
    )
    {
    STATUS		status = OK;

    if (addr == (INSTR *) NULL)
        return;			/* no error messages available! */

#ifdef  INCLUDE_WDB_PRE_KERNEL_INIT
    /*
     * In pre-kernel init mode, usrBreakpointInit() gets called before the
     * VM library is installed (if configured in the system). This means that
     * once the VM library gets installed, bpMemWriteMode will potentially no
     * longer be correct. To avoid issues while setting breakpoints after VM
     * library initialization, we must recompute bpMemWriteMode by re-calling
     * usrBreakpointInit().
     */

    if (VM_INSTALLED && !bpMemWriteModeDetermined)
        {
        usrBreakpointInit ();
        bpMemWriteModeDetermined = TRUE;
        }
#endif /* INCLUDE_WDB_PRE_KERNEL_INIT */

    if ((bpMemWriteMode & MEM_WRITE_BY_VM) == MEM_WRITE_BY_VM)
	{
	VM_CONTEXT_ID	vmCtx = NULL;

	if (rtpId != NULL)
	    vmCtx = RTP_VM_CONTEXT_GET (rtpId);

	status = VM_CONTEXT_BUFFER_WRITE (vmCtx, &value, addr, sizeof (INSTR));
	}
    else
	*addr = value;

    if (status == OK)
	{
	switch (policy)
	    {
	    case VXDBG_LOCAL_FLUSH :

		CACHE_TEXT_LOCAL_UPDATE (addr, sizeof (INSTR));
	    	break;

	    case VXDBG_FLUSH :

		CACHE_TEXT_UPDATE (addr, sizeof (INSTR));
	    	break;

	    case VXDBG_NO_FLUSH :
	    default :

	    	break;
	    }
	}
    }

/******************************************************************************
*
* usrBreakpointInit - initialize the breakpoint memory write mode
*
* This routine initializes the mode used by the usrBreakpointSet() routine to
* write the breakpoint instruction in memory.
*
* RETURNS : N/A
*
* NOMANUAL
*/ 

void usrBreakpointInit (void)
    {
    /*
     * The VM mode is set if the kernel is built with the RTP support or if its
     * text section is protected and is mapped via PTEs.
     */

#if	defined(INCLUDE_RTP) || defined(INCLUDE_PROTECT_TEXT)
    UINT32	mmuAttr = 0;

    if ((!VM_INSTALLED) ||
	(VM_STATE_GET (NULL, ROUND_DOWN (usrBreakpointInit,
    				VM_PAGE_SIZE_GET()), &mmuAttr) == ERROR) ||
	((mmuAttr & MMU_ATTR_VALID_MSK) != MMU_ATTR_VALID))
    	{
	bpMemWriteMode = MEM_WRITE_DIRECT;
	return;
	}

    bpMemWriteMode = MEM_WRITE_BY_VM;
#else	/* defined(INCLUDE_RTP) || defined(INCLUDE_PROTECT_TEXT) */
    bpMemWriteMode = MEM_WRITE_DIRECT;
#endif	/* defined(INCLUDE_RTP) || defined(INCLUDE_PROTECT_TEXT) */
    }

#endif	/* __INC_usrBreakpointc */
