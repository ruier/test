/* usrSalSnsInit.c - init routine for the kernel version of SNS */

/*
 * Copyright (c) 2003-2005, 2011_2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,11jul12,shi  clean up warnings (WIND00357529)
01l,19jul11,shi  LP64 conversion
01k,10may05,ebh  allow definable SNS server type and instance
01j,20apr05,ebh  added support for distributed SAL
01j,05jan05,bwa  made listen socket's backlog configurable (SPR#102955).
01i,10sep04,ymz  Modified to use SNS_PRIORITY and SNS_STACK_SIZE.
01h,09sep04,ymz  renamed cleanup routine.
01g,26aug04,ymz  changed to use funcbind for rtpHandleIdGet (SPR98240).
01f,11jun04,ymz  modified snsCleanupInit to fix dependency issue.
01e,29apr04,ymz  removed temporary workaround code.
01d,15apr04,p_r  change rtp function with function pointer
01c,05apr04,ymz  added cleanup trigger
01b,02dec03,p_r  added header files.
01a,25nov03,p_r  written.
*/

/* includes */
#include <dsi/snsLib.h>
#include <taskLib.h>

/* externs */

extern FUNCPTR     _func_rtpDeleteHookAdd;
extern FUNCPTR     _func_rtpHandleIdGet;

extern STATUS      snsRtpHookInit (int pri);

/*******************************************************************************
*
* salSnsTaskInit - starts the SNS Server daemon in the kernel and setup cleanup
*
* This routine spawns the SNS Server Task and calls the SNS directory cleanup
* mechanism.  Task priority and task size should not be normally changed. 
*
* RETURNS: OK if the initilization is successful, ERROR otherwise
*
* NOMANUAL
*/

STATUS salSnsTaskInit (void)
    {
    TASK_ID taskId;     /* taskId for SNS server */

    if((taskId = taskSpawn ("tSnsServer",
                             SNS_PRIORITY,   /* priority */
                             0,              /* task options */
                             SNS_STACK_SIZE, /* stack space */
                             (FUNCPTR) snsServer,
			     SNS_LISTEN_BACKLOG,
#ifdef INCLUDE_SNS_MP
                             1,		/* activate distributed version */
                             SNS_DISTRIBUTED_SERVER_TYPE,
                             SNS_DISTRIBUTED_SERVER_INSTANCE,
#else
			     0,		/* activate standalone version */
			     0,		/* do not define a server type */
			     0,		/* do not define a server instance */
#endif
			     0, 0, 0, 0, 0, 0 )) == TASK_ID_ERROR)
        {
        printf(" Cannot spawn SNS server task ...!\n");
        return ERROR;
        }

#ifdef INCLUDE_RTP
    if (_func_rtpDeleteHookAdd != NULL && _func_rtpHandleIdGet != NULL)
	snsRtpHookInit (SNS_PRIORITY + 1);  /* Task priority is lower so we know */
                                            /* the SNS server has already started */
#endif

#ifdef INCLUDE_SNS_MP
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

    return OK;
    }


