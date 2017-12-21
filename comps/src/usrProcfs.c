/* usrProcfs.c - PROCFS initialization */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,12may06,md   add global variables for versioning
01a,28jul04,md   written.
*/

/*
DESCRIPTION
This file is included by bootConfig.c and usrConfig.c. The file contains
routines for configuring the process based file system.

SEE ALSO: usrExtra.c

NOMANUAL
*/

#ifndef  __INCusrProcfs
#define  __INCusrProcfs

/* includes */

#include "vxWorks.h"

/* forward declarations */

#define	MOUNT_POINT	"/proc"
#define	NUM_BUFFERS	5

/* values used by various files in /proc/kernel */

char *procfsBSP_v = BSP_VERSION;
char *procfsBSP_r = BSP_REV;
int procfsMemLocal = LOCAL_MEM_LOCAL_ADRS;
int procfsCPUType = CPU;

/*******************************************************************************
*
* usrProcfsConfig - mount the process based file system
*
* This routine mounts the process based file system on the /proc mount point.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
* .pG "I/O System, Local File Systems"
*/

STATUS usrProcfsConfig (void)
    {
    extern int procfsFsDevCreate (char *, int);
    extern int rtpProcfsInit (void);
    extern int edrProcfsInit (void);

    procfsFsDevCreate (MOUNT_POINT, NUM_BUFFERS);

#ifdef	INCLUDE_RTP
    rtpProcfsInit();	/* install RTP specific entries */
#endif /* INCLUDE_RTP */

#ifdef	INCLUDE_EDR_ERRLOG
    edrProcfsInit();	/* install ED&R specific entries */
#endif /* INCLUDE_EDR_ERRLOG */

    return (OK);
    }

#endif /* __INCusrProcfs */
