/* usrSysctl.c - Basic Sysctl initialization */

/*
 * Copyright (c) 2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,30apr07,pad  removed oidsInList initialization from here since it is
		 already done in kern_sysctl.c
01c,14jun04,job  Renamed usrSysctl2Init() to usrSysctlInit()
01b,23may04,job  Renamed usrSysctlInit() in usrSysctl2Init().
01a,20may04,job  written.
*/

/*
DESCRIPTION

This library contains the routines to configure the sysctl mechanism.

NOMANUAL
*/

#ifndef  __INCusrSysctl
#define  __INCusrSysctl

/* includes */

#include <sys/sysctl.h>
#include <sysctlLib.h>

/* externs */

IMPORT struct sysctl_ctx_list   sysctlCtxList;
IMPORT struct sysctl_ctx_list * pSysctlCtxList;

/*******************************************************************************
*
* usrSysctlInit - configure the sysctl capability
*
* This routine configures the basic sysctl capability and instantiates some
* basic sysctl groups.
*
* RETURNS: OK 
*
*/

STATUS usrSysctlInit (void)
    {
    pSysctlCtxList = &sysctlCtxList;
    sysctl_ctx_init (pSysctlCtxList);

    sysctlInit();

    return (OK);
    }

#endif /* __INCusrSysctl */
