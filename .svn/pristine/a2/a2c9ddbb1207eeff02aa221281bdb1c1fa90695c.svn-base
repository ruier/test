/* usrWindview.c - configuration file for System Viewer on the target */

/* 
 * Copyright (c) 1997-2005,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01o,21may10,tcr  64bit adaptation
01n,13jan05,tcr  Fix SPR 101137: WindView upload task priority and stacksize
                 should be parameters
01m,15apr04,tcr  rBuff error handler to use error definitions
                 change WindView to 'System Viewer'
01l,09oct03,tcr  limit number of logs created
01k,19sep03,tcr  clear logging level before setting a new one
01j,26aug03,tcr  changes for Base6
01i,03mar99,dgp  update to reference project facility instead of config.h
01h,28aug98,dgp  FCS man page edit
01g,13may98,cth  changed minimum buffers, added wvUploadStop to error handling
01f,06may98,dgp  clean up man pages for WV 2.0 beta release
01e,20apr98,cth  changed rbuff options, moved wvEvtLogInit, added
		 wvLogHeaderCreate and Upload
01d,17apr98,cth  added partition id to wvEvtLogInit
01c,15apr98,cth  more clean up
01b,27jan98,cth  cleaned up and made work
01a,18dec97,cth  written
*/

/*
DESCRIPTION
This file is included in usrConfig.c.

This file contains routines that configure and initialize the Wind River
System Viewer's target components based on directives in the configuration
files. The routines wvOn() and wvOff() are also defined here. They provide an
easy way to start and stop a typical instance of the System Viewer from a shell,
rather than using the Syetem Viewer host user interface. wvOn() and wvOff()
simplify the  process of starting and stopping the System Viewer by bundling
the calls necessary to do so into a single routine.  The System View host GUI
does not call wvOn() or wvOff() explicitly, so it is possible to modify them
to meet users' specific requirements.

INTERNAL
The typical instance of the System Viewer includes using rBuff buffers; one of
file, socket, or TSFS-socket upload paths (determined by the value of
WV_DEFAULT_UPLOAD_TYPE set below); and uploading events continuously.
There are several upload paths that can be configured into the vxWorks
image.  The upload paths are included by defining INCLUDE_WVUPLOAD_<type>
in configAll.h.  It is common to include more than one type of path,
allowing the host user interface to be used to select between the types
without recompiling the target.  If no upload path is explicitly included
within configAll.h, then INCLUDE_WVUPLOAD_FILE is added by default.  See
usrDepend.c for other dependencies.

SEE ALSO
.I Wind River System Viewer User's Guide
*/

#ifndef __INCusrWindviewc
#define __INCusrWindviewc


#include "vxWorks.h"
#include "inetLib.h"
#include "logLib.h"
#include "wvLib.h"
#include "wvTmrLib.h"
#include "rBuffLib.h"

#include "private/wvBufferP.h"
#include "private/wvUploadPathP.h"
#include "private/seqDrvP.h"
#include "private/funcBindP.h"
#include "private/wvFileUploadPathLibP.h"
#include "private/wvSockUploadPathLibP.h"
#include "private/wvTsfsUploadPathLibP.h"

/* defines */

#define WV_FILE         1               /* file upload-path type */
#define WV_SOCK         2               /* socket upload-path type */
#define WV_TSFSSOCK     3               /* tsfs socket upload-path type */

#ifndef WV_DEFAULT_BUF_SIZE
#define WV_DEFAULT_BUF_SIZE     0x8000
#endif

#ifndef WV_DEFAULT_BUF_MIN
#define WV_DEFAULT_BUF_MIN      4
#endif

#ifndef WV_DEFAULT_BUF_MAX
#define WV_DEFAULT_BUF_MAX      16
#endif

#ifndef WV_DEFAULT_BUF_THRESH
#define WV_DEFAULT_BUF_THRESH   0x4000
#endif

#ifndef WV_DEFAULT_BUF_OPTIONS
#define WV_DEFAULT_BUF_OPTIONS  0x0
#endif

#ifndef WV_NUM_LOGS_DEFAULT
#define WV_NUM_LOGS_DEFAULT     5
#endif

#ifndef WV_MAX_LOGS
#define WV_MAX_LOGS             5
#endif

#ifndef WV_RBUFF_MGR_PRIORITY
#define WV_RBUFF_MGR_PRIORITY   100
#endif

#ifndef WV_UPLOAD_PRIORITY
#define WV_UPLOAD_PRIORITY      150
#endif

#ifndef WV_UPLOAD_STACKSIZE
#define WV_UPLOAD_STACKSIZE     5000
#endif


/* globals */

int wvDefaultBufSize;		/* default size of a buffer */
int wvDefaultBufMax;		/* default max number of buffers */
int wvDefaultBufMin;		/* default min number of buffers */
int wvDefaultBufThresh;		/* default threshold for uploading, in bytes */
int wvDefaultBufOptions;	/* default rBuff options */

int wvRBuffMgrPriorityDefault;  /* default priority of rBuff manager task */

rBuffCreateParamsType   wvDefaultRBuffParams;  /* contains the above values */

BUFFER_ID	  wvBufId;	/* event-buffer id used by wvOn/Off */
UPLOAD_ID	  wvUpPathId;	/* upload-path id used by wvOn/Off */
WV_UPLOADTASK_ID  wvUpTaskId;	/* upload-task id used by wvOn/Off */


/* forward declarations */

int wvRBuffErrorHandler (int errorType);


/******************************************************************************
*
* windviewConfig - configure and initialize System Viewer components
*
* NOMANUAL
*
*/

STATUS windviewConfig (void)
    {

    /*
     * This is not mandatory at this point, but by setting these global
     * variables to the default values here, they can be changed from the
     * shell before wvOn/wvOff use them.
     */

    wvDefaultBufSize     	= WV_DEFAULT_BUF_SIZE;
    wvDefaultBufMax	 	= WV_DEFAULT_BUF_MAX;
    wvDefaultBufMin	 	= WV_DEFAULT_BUF_MIN;
    wvDefaultBufThresh	 	= WV_DEFAULT_BUF_THRESH;
    wvDefaultBufOptions	 	= WV_DEFAULT_BUF_OPTIONS;

    wvDefaultRBuffParams.minimum	  = wvDefaultBufMin;
    wvDefaultRBuffParams.maximum	  = wvDefaultBufMax;
    wvDefaultRBuffParams.buffSize	  = wvDefaultBufSize;
    wvDefaultRBuffParams.threshold	  = wvDefaultBufThresh;
    wvDefaultRBuffParams.options	  = wvDefaultBufOptions;

    wvRBuffMgrPriorityDefault   = WV_RBUFF_MGR_PRIORITY;
    wvMaximumLogs = WV_MAX_LOGS;

    /*
     * Set the default partition for logging. This can be overridden here,
     * or a different partition selected by wvPartitionSet()
     */

    wvPartitionSet (memSysPartId);
    wvDefaultRBuffParams.sourcePartition  = wvPartitionGet ();

    wvDefaultRBuffParams.errorHandler	  = wvRBuffErrorHandler;

    /*
     * Set configurable parameters for upload task
     */

    wvUploadTaskPriority      = WV_UPLOAD_PRIORITY;
    wvUploadTaskStackSize     = WV_UPLOAD_STACKSIZE;

    /*
     * Include and complete initialization for wvLib.
     */

    wvLibInit2 ();

    return (OK);
    }

/******************************************************************************
*
* wvOn - start a typical instance of System Viewer event logging and upload
*
* This routine starts a typical instance of System Viewer event logging and
* event upload.  rBuff buffers are used to store events.  The file upload path
* is TSFS-socket, socket, or file.  Event data is uploaded to the path
* continuously.
*
* Event logging is started for events belonging to <class>.  The rBuff buffer
* is created using the values in 'wvDefaultRBuffParams', which are taken from
* the globals 'wvDefaultBufxxx', configurable from usrWindview.c.  The upload
* path used to transfer events away from the event buffer is determined by
* the value of WV_DEFAULT_UPLOAD_TYPE, also set in usrWindview.c to one
* of WV_FILE, WV_TSFSSOCK or WV_SOCK.  The upload path is opened using the
* remaining parameters as follows:
*
* .TS
* tab(|)
* lf3 lf3 lf3
* l l l.
*         | WV_FILE        | WV_TSFSOCK / WV_SOCK
* _
* arg2    | char *filename | char *hostIp
* arg3    | openFlags      | short port
* arg4    | not used       | not used
* .TE
*
* The arguments are of generic type and name to allow the addition of new
* types of upload paths or customization of wvOn().
*
* RETURNS: OK, or ERROR if the upload path can not be created, the upload task
* can not be created, or the rBuff buffer can not be created.
*
* SEE ALSO: wvOff()
*
*/

STATUS wvOn
    (
    int 	class,	/* class of events to be logged */
    char * 	arg2,	/* filename, hostIp, etc. */
    int 	arg3,	/* port number, openFlags etc. */
    int 	arg4	/* for future expansion */
    )
    {
    /* Get a partition, and create a log list */

    if (wvPartitionGet () == NULL)
        {
        wvPartitionSet (memSysPartId);
        wvDefaultRBuffParams.sourcePartition = wvPartitionGet ();
        logMsg ("wvOn: Select memSysPart for System Viewer\n",
                0, 0 ,0 ,0 ,0 ,0);
        }

    if (wvCurrentLogListGet () == NULL)
        if (wvLogListCreate () == NULL)
            {
            logMsg ("wvOn: Error creating loglist\n",
                    0, 0, 0, 0, 0, 0);
            return (ERROR);
            }

    /* Create a buffer. */

    if ((wvBufId = rBuffCreate (& wvDefaultRBuffParams)) == NULL)
        {
        logMsg ("wvOn: error creating buffer.\n",0,0,0,0,0,0);
        return (ERROR);
        }

    /*
     * Create the System Viewer log, letting the event logging routines know
     * which buffer to log events to.
     */

    if (wvLogCreate (wvBufId) == NULL)
        {
        logMsg ("wvOn: Error creating log\n", 0, 0, 0, 0, 0, 0);
        rBuffDestroy (wvBufId);

        return (ERROR);
        }

    if (_func_wvOnUploadPathCreate != NULL)
	if ((wvUpPathId = _func_wvOnUploadPathCreate (arg2, arg3)) == NULL)
            {
            logMsg ("wvOn: error creating upload path(%s%d %s%d).\n",
                    (_Vx_usr_arg_t)wvLogMsgString1, (_Vx_usr_arg_t)arg2,
		    (_Vx_usr_arg_t)wvLogMsgString2, arg3, 0, 0);
            wvLogDelete (wvCurrentLogListGet (), wvCurrentLogGet ());
            return (ERROR);
            }

    /*
     * Turn on event logging.
     */

    wvEvtClassClear (7);
    wvEvtClassSet (class);
    wvEvtLogStart ();

    /*
     * Start the upload task.  Uploading will be continuous after this
     * is started.
     */

    logMsg ("wvOn: wvBufId %d wvUpPathId %d.\n", (_Vx_usr_arg_t)wvBufId,
	    (_Vx_usr_arg_t)wvUpPathId, 0,0,0,0);

    if ((wvUpTaskId = wvUploadStart (wvCurrentLogGet (), wvUpPathId, TRUE)) == NULL)
        {
        logMsg ("wvOn: error launching upload.\n",0,0,0,0,0,0);
        wvEvtLogStop ();
        wvLogDelete (wvCurrentLogListGet (), wvCurrentLogGet ());

        if (_func_wvOnUploadPathClose != NULL)
            _func_wvOnUploadPathClose (wvUpPathId);

        return (ERROR);
        }


    return (OK);
    }

/******************************************************************************
*
* wvOff - stop a typical instance of System Viewer event logging and upload
*
* This routine stops event logging and uploading of events that was started
* with wvOn().  First, event logging is stopped.  Next the event upload task
* flushes the buffers and is then deleted.  Finally, the upload path is
* closed and the event buffer is deleted.
*
* The type of upload path is determined by the value of WV_DEFUALT_UPLOAD_TYPE,
* set in usrWindview.c.  The upload task ID, buffer ID, and upload path ID are
* stored in the 'wvUpTaskId', 'wvBufId', and 'wvUpPathId' globals, respectively.
*
* RETURNS: N/A
*
* SEE ALSO: wvOn()
*
*/

void wvOff (void)
    {

    /* Stop event logging. */

    wvEvtLogStop ();

    /*
     * Stop continuous upload of events.  This will block until the buffer
     * is emptied.
     */
    logMsg ("calling wvUploadStop()\n", 0, 0, 0, 0, 0, 0);
    if (( wvUpTaskId != NULL) && wvUploadStop (wvUpTaskId) == ERROR)
	{
	logMsg ("wvOff: error killing upload task.\n", 0,0,0,0,0,0);
	}
    logMsg ("DONE\n", 0, 0, 0, 0, 0, 0);

    /* Close the upload path. */

    if (_func_wvOnUploadPathClose != NULL)
        _func_wvOnUploadPathClose (wvUpPathId);

    /* Destroy the event buffer. */

    wvLogDelete (wvCurrentLogListGet (), wvCurrentLogGet ());
    }

/*******************************************************************************
* wvRBuffErrorHandler - error handler for the rBuffer created by  wvOn,wvOff
*
* NOMANUAL
*
*/
int wvRBuffErrorHandler
    (
    int         errorType
    )
    {
    if (errorType == RBUFF_ERR_FULL)
        logMsg ("wvRBuffErrorHandler: ring buffer full.\n", 0,0,0,0,0,0);
    else
        logMsg ("wvRBuffErrorHandler: error occurred.\n", 0,0,0,0,0,0);
    return OK;
    }


#endif /* __INCusrWindviewc */
