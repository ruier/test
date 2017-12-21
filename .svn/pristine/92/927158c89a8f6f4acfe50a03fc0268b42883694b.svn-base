/* usrSalSnsRtp.c - wrapper for the RTP version of SNS */

/*
 * Copyright (c) 2003-2005, 2011-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,11jul12,shi  clean up warning (WIND00357529)
01m,19jul11,shi  LP64 conversion
01l,10may05,ebh  allow definable SNS server type and instance
01k,20apr05,ebh  added support for distributed SAL
01j,21feb05,ebh  update manually (zippy merge was not running)
01i,05jan05,bwa  made listen socket's backlog configurable (SPR #102955).
01h,05nov04,p_r  fixed prototype definition
01g,09sep04,ymz  renamed cleanup routine.
01f,11jun04,ymz  fixed RTP dependency.
01e,15apr04,p_r  rearranged cleanup initialization
01d,09apr04,pad  Adapted to change in rtpSpawn()'s return value on failure
                 (SPR #96057).
01c,05apr04,ymz  added event based SNS directory cleanup.
01b,15dec03,als  fixed typos causing compilation error (SPR #92606)
01a,18nov03,p_r  written.
*/

/* includes */

#include <rtpLib.h>

/* forward declarations */

extern STATUS snsRtpHookInit (int pri);

/*******************************************************************************
*
* salSnsRtpInit - starts the RTP SNS daemon
*
* This routine is just a wrapper to create the RTP version of the SNS server.
* The pathname is provided by the configuration tool. The priority and size 
* should not be normally changed.
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/

STATUS salSnsRtpInit (void)
    {
    int prio, size;                   /* SNS rtp priority and size */
    char * pathname = SNS_PATHNAME;   /* SNS rtp image location */
    char * ppArgv [6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    char pBacklog [12];               /* 12: max len of int + 1 */
    char distributed [2];             /*  2: "1" for distributed, "0" for not */
    char pServerType [12];            /* 12: max len of int + 1 */
    char pServerInst [12];            /* 12: max len of int + 1 */

    sprintf (pBacklog, "%d", SNS_LISTEN_BACKLOG);
#ifdef INCLUDE_SNS_MP_RTP
    sprintf (distributed, "%s", "1");	/* active distributed version */
    sprintf (pServerType, "%d", SNS_DISTRIBUTED_SERVER_TYPE);	  /* TYPE */
    sprintf (pServerInst, "%d", SNS_DISTRIBUTED_SERVER_INSTANCE); /* INSTANCE */
#else
    sprintf (distributed, "%s", "0");	/* active standalone version */
    sprintf (pServerType, "%d", 0);  /* TYPE */
    sprintf (pServerInst, "%d", 0);  /* INSTANCE */
#endif
    ppArgv [0] = pathname;
    ppArgv [1] = pBacklog;
    ppArgv [2] = distributed;
    ppArgv [3] = pServerType;
    ppArgv [4] = pServerInst;
    prio = SNS_PRIORITY;
    size = SNS_STACK_SIZE;
    
    if ((pathname == NULL) || (prio <= 0) || (size <= 0))
	return (ERROR);

    if (rtpSpawn (pathname, ppArgv, NULL, prio, size, 0, 0) == RTP_ID_ERROR)
	return (ERROR);

#ifdef INCLUDE_SNS_MP_RTP
    /* need to start the dsalMonitor task here */
    if ( taskSpawn ("tDsalMonitor",
                     SNS_PRIORITY,           /* priority */
                     0,             /* task options */
                     20000,         /* stack space */
                     (FUNCPTR) dsalMonitor,
                     SNS_DISTRIBUTED_SERVER_TYPE,
                     SNS_DISTRIBUTED_SERVER_INSTANCE,
                     0, 0, 0, 0, 0, 0, 0, 0 ) == TASK_ID_ERROR)
        {
	/* system will run, but not in full distributed mode */
        printf(" Cannot spawn dsalMonitor for distributed SAL mode ...!\n");
	return (ERROR);
        }
#endif

    snsRtpHookInit (prio + 1);

    return (OK);
    }

