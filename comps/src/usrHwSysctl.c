/* usrHwSysctl.c - BSP, HW, PAL sysctl support functions */

/*
 * Copyright (c) 2004-2005, 2007, 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
01u,25jan11,pcs  Changed the handler for KERN_AUXCLOCKRATE from
                 sysctl_handle_reg to rtpSysctlSysAuxClkRateGet as the old
                 handler was setting errno to ENOMEM eventhough it was
                 returning the right value for LP64.
01t,04dec09,jpb  Updated return types for SYSCTL_ACCESS members.
01s,31aug07,rec  WIND00103242 archSysctlInit not included for coldfire
01r,01jun07,pad  Replaced inclusion of sys/sysctl.h with private/sysctlP.h.
01q,05feb07,jmt  Fix compiler warning with missing archSysctlInit prototype
01p,05sep05,elp  adding archSysctlInit() for simulators.
01o,01dec04,rec  updated for PPC
01n,30sep04,h_k  added SH support.
01m,13sep04,rec  add ARM/XScale support
01m,24aug04,job  Fix warning: SPR #100114
01l,28jun04,mdo  SPRs 98476, 98481, 98482, 98472, 98473
01k,01jul04,job  Register auxClkRate sysctl here because it must be done
		 conditionally as not all BSPs support it.
01j,28jun04,job  Updated prototype inclusion to remove warnings
01i,09jun04,zl   removed reference to vxVMI
01h,08jun04,job  Removed sys|auxClkRate, sysModel & sysBspRev routines to
                 RTP bring up code.
01g,08jun04,sru  all MMU's now layered underneath MMU AIM, so removed
		 call to PPC-specific mmuArchSysctlInit().
01f,02jun04,rec  renamed file from usrSysctl.c to usrHwSysctl.c
01e,01jun04,mem  call aim*SysctlInit() for all architectures
01d,20may04,mem  call archSysctlInit() for MIPS
01c,05may04,rec  move cache and mmu leaves to arch files
01b,09apr04,rec  review comments
01a,25mar04,rec  written
*/

/*
DESCRIPTION
This module provides initialization functions for hardware specific sysctl
support.
*/

#include <vxWorks.h>
#include <errnoLib.h>
#include <stdio.h>
#include <sysLib.h>
#include <config.h>
#include <private/sysctlP.h>	/* brings in sysctl_add_oid_internal() */

/* needed for sysctl macro expansion */

#ifndef PRINTF
#define _SYSCTL_DEBUG
#ifdef _SYSCTL_DEBUG
#define PRINTF printf
#else
#define PRINTF(...) 
#endif
#endif

/* externals */

SYSCTL_DECL_NODE_EXT (hw);
SYSCTL_DECL_NODE_EXT (sysctl);
SYSCTL_DECL_NODE_EXT (kern);
IMPORT struct sysctl_ctx_list *  pSysctlCtxList;
IMPORT struct sysctl_oid_list sysctl__children;

extern int sysAuxClkRateGet ();

/* locals */

/* globals */

SYSCTL_DECL_NODE (bsp);
SYSCTL_DECL_NODE (pal);
SYSCTL_DECL_NODE (cpu);


/* forward declarations */

#if (defined INCLUDE_CACHE_SUPPORT)
extern STATUS cacheArchSysctlInit (void);
extern STATUS aimCacheSysctlInit (void);
#endif /* INCLUDE_CACHE_SUPPORT */

#if (defined INCLUDE_MMU_BASIC) || (defined INCLUDE_MMU_FULL)
extern STATUS aimMmuSysctlInit (void);
#endif /* (defined INCLUDE_MMU_BASIC) */

#if (CPU_FAMILY==PPC) || (CPU_FAMILY==MIPS) || (CPU_FAMILY==ARM) || \
    (CPU_FAMILY==SH)  || (CPU_FAMILY==I80X86) || (CPU_FAMILY==COLDFIRE) || \
    (defined _WRS_ARCH_IS_SIMULATOR)
extern STATUS archSysctlInit (void); 
#endif
        
#ifdef INCLUDE_AUX_CLK
/*******************************************************************************
*
* rtpSysctlSysAuxClkRateGet - Sysctl handler for reading the aux clock rate
*
* This routine reads the auxillary clock rate and passes it on to the default
* int handler which returns it as output via the system call arguments.
*
* RETURNS: OK or error value
*
* ERRNOS: N/A
*
* \NOMANUAL
*/

LOCAL int rtpSysctlSysAuxClkRateGet
    (
    SYSCTL_HANDLER_ARGS /* all parameters are opaquely passed via this macro */
    )
    {
    int error;
    int auxClkRate = sysAuxClkRateGet();

    error = sysctl_handle_int(oidp, &auxClkRate, 0, req);

    return error;
    }
#endif /* INCLUDE_AUX_CLK */ 

/*******************************************************************************
*
* usrHwSysctlInit - Initialize hardware specific sysctl
*
* This routine initializes cache, mmu, bsp, hardware, and processor 
* abstraction layer sysctl.  It then invokes the initialization function
* for cpuFamily sysctl attributes.
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/

STATUS usrHwSysctlInit (void)
    {
    STATUS localStatus = OK;
   
    /* create nodes */

    SYSCTL_NODE (hw, HW_BSP, bsp, (int)CTLFLAG_RW, NULL, "bsp" );
    SYSCTL_NODE (hw, HW_PAL, pal, (int)CTLFLAG_RW, NULL, "pal" );
    SYSCTL_NODE (pal, HW_PAL_CPU, cpu, (int)CTLFLAG_RW, NULL, "cpu" );

    /* sys capabilities */

    /* sysAuxClkRateGet is not available on all BSPs, so conditionally
     * support it here
     */

#ifdef INCLUDE_AUX_CLK
    SYSCTL_PROC(kern, KERN_AUXCLOCKRATE, auxClockRate,
                (int) (CTLTYPE_INT | CTLFLAG_RD), 0, 0,
                rtpSysctlSysAuxClkRateGet, "IU",
                "the current auxilary clock rate");
#endif /* INCLUDE_AUX_CLK */

    /* bsp attributes */

#ifdef BSP_CPU_STRING                       
    SYSCTL_STRING (cpu, HW_PAL_CPU_PROCESSOR, 
                       name, (int)CTLFLAG_RD, BSP_CPU_STRING, 0, 
                       "BSP processor chip");
#endif


#if (CPU_FAMILY==PPC) || (CPU_FAMILY==MIPS) || (CPU_FAMILY==ARM) || \
    (CPU_FAMILY==SH)  || (CPU_FAMILY==I80X86) || (CPU_FAMILY==COLDFIRE) || \
    (defined _WRS_ARCH_IS_SIMULATOR)

    localStatus = archSysctlInit ();    /* initialize architecture sysctl */
    if (localStatus != OK)
        return (localStatus);
#endif

    /* cache sysctl initialization */

#if (defined INCLUDE_CACHE_SUPPORT)

    localStatus = aimCacheSysctlInit ();
    if (localStatus != OK)
        return (localStatus);

#endif /* INCLUDE_CACHE_SUPPORT */

    /* mmu sysctl initialization */

#if (defined INCLUDE_MMU_BASIC) || (defined INCLUDE_MMU_FULL)

    localStatus = aimMmuSysctlInit ();
    if (localStatus != OK)
        return (localStatus);

#endif /* (defined INCLUDE_MMU_BASIC) || (defined INCLUDE_MMU_FULL) */

#if (CPU_FAMILY==PPC)

    /* cache sysctl initialization */

#if (defined INCLUDE_CACHE_SUPPORT)

    localStatus = cacheArchSysctlInit ();
    if (localStatus != OK)
        return (localStatus);

#endif /* INCLUDE_CACHE_SUPPORT */

#endif /* (CPU_FAMILY==PPC) */

               
    return (localStatus);
    }
