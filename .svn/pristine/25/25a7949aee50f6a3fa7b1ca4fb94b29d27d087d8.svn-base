/* usrTextProtect.c - text protection component */

/*
 * Copyright (c) 2004-2005, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,20jul10,jpb  Updated for LP64 adaptation.
01c,07feb05,zl   enable protection for kernel-loaded modules (SPR #105868).
01b,19oct04,zl   disable calling vmTextProtect for ROM-resident images.
01a,17sep04,sru  Created.
*/

/*
DESCRIPTION

This file is used to invoke the appropriate routines to write protect
the kernel text segment.  The MIPS architecture does not require this
routine, because text protection is handled early in the boot process.

NOMANUAL
*/

#ifndef  __INCusrTextProtectc
#define  __INCusrTextProtectc

#include <vsbConfig.h>
#include <edrLib.h>

/***************************************************************************
*
* usrTextProtect - write-protect the kernel text segment.
*
* This routine write-protects the kernel text segment on those architectures
* that require it.
*
* NOMANUAL
*/

void usrTextProtect ()
    {
    BOOL setState = FALSE;

#ifndef _WRS_CONFIG_LP64
#if (CPU_FAMILY != MIPS)
    extern char wrs_kernel_text_start[];

    /* 
     * protect text only if it's in system RAM. This is normally the case,
     * except for ROM-resident images.
     */

    if ((wrs_kernel_text_start >= (char *) LOCAL_MEM_LOCAL_ADRS) &&
        (wrs_kernel_text_start < sysMemTop()))
	{
	setState = TRUE;
	}
#endif /* (CPU_FAMILY != MIPS) */
#endif /* !_WRS_CONFIG_LP64 */

    if (vmTextProtect (setState) == ERROR)
	{
	EDR_INJECT (EDR_SEVERITY_NONFATAL | EDR_FACILITY_KERNEL, __FILE__,
		    __LINE__, NULL, NULL, NULL,
		    "kernel text protection failed");
	}
    }
#endif  /*  __INCusrTextProtectc */

