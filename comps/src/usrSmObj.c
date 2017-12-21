/* usrSmObj.c - shared memory object initialization */

/* 
 * Copyright (c) 1992-2002,2005-2007 2010-2012 
 * Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,17oct12,whu  fixed defect WIND00381666. Replaced SM_MASTER with smMaster.
01m,15dec11,swg  update usrSmObjInit() to get current processor number
                 (CQ:WIND00300028)
01l,04nov10,swg  add the explicit declaration for _procNumWasSet
                 (to fix WIND00237118).
01k,27jun07,bwa  made MIPS mapped kernel check more generic.
01j,21dec06,bwa  added fix to support multi-core MIPS boards on mapped kernels.
01i,12jul06,bwa  reverted SM_OBJ_MEM_ADRS preprocessor check to a runtime one.
01h,02jun06,bwa  Fixed problem of missed interrupts (WIND00039499, WIND00045175)
01g,12may05,bwa  warnings cleanup
01f,17apr05,bwa  made SM_CPUS_MAX the number of CPUs (SPR107525).
01e,13feb02,mas  fixed staged delay/check for slave node startup (SPR 73189)
01d,02oct01,mas  added staged delay/check for slave node startup (SPR 62128)
01c,13sep01,jws  fix smObjSpinTries initialization (SPR68418)
01b,20jan99,scb  modified to use "sm=" before SM_ANCHOR_ADRS (23035)
01a,16feb97,ms   based on old 01i version.
*/

/*
DESCRIPTION
This file is used to configure and initialize the VxWorks shared memory
object support.  This file is included by usrConfig.c.

SEE ALSO: usrExtra.c

NOMANUAL
*/

#if (SM_TAS_TYPE != SM_TAS_HARD)
#error Error initializing shared memory objects, hardware test-and-set required.
#endif

#include <vxWorks.h>
#include <semLib.h>
#include <taskLib.h>
#include <errnoLib.h>

extern SEM_ID smObjEventSemId;	/* signals events to be serviced to smObjTask */
extern int smObjEventTaskId;	/* task ID of task-level ISR */

extern void smObjTask (void);	/* task-level ISR */
extern BOOL _procNumWasSet;
/******************************************************************************
*
* usrSmObjInit - initialize shared memory objects
*
* This routine initializes the shared memory objects facility.  It sets up
* the shared memory objects facility if called from processor 0.
* Then it initializes a shared memory descriptor and calls smObjAttach()
* to attach this CPU to the shared memory object facility.
*
* When the shared memory pool resides on the local CPU dual ported memory,
* SM_OBJ_MEM_ADRS must be set to NONE in configAll.h and the shared memory
* objects pool is allocated from the VxWorks system pool.
*
* RETURNS: OK, or ERROR if unsuccessful.
*
* INTERNAL
* The delayed start for slave processors used below is required.  The anchor
* may not yet be mapped to the bus.  So, probing of shared memory locations is
* used to overcome Bus Errors which occur on many boards if the slave accesses
* shared memory (SM) before the master has finished initializing it.  The code
* here simply delays access to the SM region until the SM master has finished
* initializing it.
*
* The method used is to repetitively probe key locations in the SM region
* after delay periods until valid values are detected or a time-out occurs.
* The initial delay period is set based on processor number.  (The master
* processor does not delay.)  If the first probe of a location fails, an
* exponential increase in delay period is used to reduce bus contention on
* subsequent probes.
*
* This method is no better than receiving raw BERRs and does reduce bus
* contention and the number of BERRs.
*
* NOMANUAL
*/

STATUS usrSmObjInit 
    (
    char * bootString		/* boot parameter string */
    )
    {
    char *           smAnchor;		/* anchor address */
    volatile char *  smObjFreeAdrs;	/* free pool address */
    int              smObjMemSize;	/* pool size */
    BOOT_PARAMS      params;		/* boot paramters */
    BOOL             allocatedPool;	/* TRUE if pool is maloced */
    SM_OBJ_PARAMS    smObjParams;	/* smObj setup parameters */
    char 	     bb;		/* bit bucket for vxMemProbe */
    int              tics;		/* SM probe delay period */
    UINT             temp;		/* temp for smUtilMemProbe() */
    UINT             maxWait   = SM_MAX_WAIT;
    char *           cp;
    SM_OBJ_MEM_HDR * pSmObjHdr = NULL;	/* ptr to SMO header */

    allocatedPool = FALSE;

    /* Check for hardware test and set availability */

    if (smObjLibInit () == ERROR)	/* initialize shared memory objects */
	{
        printf("\nERROR smObjLibInit : shared memory objects already initialized.\n");
	return (ERROR);
	}

    if (bootString == NULL)
        bootString = BOOT_LINE_ADRS;

    /* interpret boot command */

    if (usrBootLineCrack (bootString, &params) != OK)
        return (ERROR);

    /* set processor number: may establish vme bus access, etc. */

    if (_procNumWasSet != TRUE)
	{
    	sysProcNumSet (params.procNum);
	_procNumWasSet = TRUE;
	}
    /* get processor number */
    params.procNum = sysProcNumGet();

    /* if we booted via the sm device use the same anchor address for smObj */

    if (strncmp (params.bootDev, "sm=", 3) == 0)
        {
        if (bootBpAnchorExtract (params.bootDev, &smAnchor) < 0)
            {
	    printf ("\nError initializing shared memory objects, invalid ");
            printf ("anchor address specified: \"%s\"\n", params.bootDev);
            return (ERROR);
            }
        }
    else
        {
	smAnchor = (char *) SM_ANCHOR_ADRS;         /* default anchor */
        }

    /* If we are shared memory master CPU, set up shared memory object (SMO) */

    if (params.procNum == smMaster)
        {
        smObjFreeAdrs = (char *) SM_OBJ_MEM_ADRS;
        smObjMemSize  = SM_OBJ_MEM_SIZE;

        /* allocate the shared memory object pool if needed */

        if (smObjFreeAdrs == (char *) NONE)
            {
            /* check cache configuration - must be read and write coherent */

            if (!CACHE_DMA_IS_WRITE_COHERENT() || !CACHE_DMA_IS_READ_COHERENT())
                {
                printf ("usrSmObjInit - cache coherent buffer not available. Giving up.  \n");
                return (ERROR);
                }

            allocatedPool = TRUE;

            smObjFreeAdrs = (char *) cacheDmaMalloc (SM_OBJ_MEM_SIZE);

            if (smObjFreeAdrs == NULL)
                {
                printf ("usrSmObjInit - cannot allocate shared memory pool. Giving up.\n");
                return (ERROR);
                }

#ifdef INCLUDE_SM_MIPS_K0_SEGMENT
    #ifdef INCLUDE_MAPPED_KERNEL
	    printf ("MIPS memory mapped kernel: "
		    "moving the SM OBJ address into the K0 segment\n");
	    smObjFreeAdrs = (char *) KM_TO_K0 (smObjFreeAdrs);
    #endif
#endif
            }

        if (!allocatedPool)
            {
            /* free memory pool must be behind the anchor */
            smObjFreeAdrs += sizeof (SM_ANCHOR);

	    /* adjust pool size */
	    smObjMemSize = SM_OBJ_MEM_SIZE - sizeof (SM_ANCHOR);
            }

	/* probe anchor address */

	if (vxMemProbe (smAnchor, VX_READ, sizeof (char), &bb) != OK)
	    {
	    printf ("usrSmObjInit - anchor address %#x unreachable. Giving up.\n", (unsigned int) smAnchor);
	    return (ERROR);
	    }

	/* probe beginning of shared memory */

	if (vxMemProbe ((char*)smObjFreeAdrs, VX_WRITE, sizeof (char), &bb) != OK)
	    {
	    printf ("usrSmObjInit - shared memory address %#x unreachable. Giving up.\n", (unsigned int) smObjFreeAdrs);
	    return (ERROR);
	    }

        /* set up shared memory objects */
        
        smObjSpinTries = SM_OBJ_MAX_TRIES; /* must do before smObjSetup() */

        smObjParams.allocatedPool = allocatedPool;
        smObjParams.pAnchor       = (SM_ANCHOR *) smAnchor;
        smObjParams.smObjFreeAdrs = (char *) smObjFreeAdrs;
        smObjParams.smObjMemSize  = smObjMemSize;
        smObjParams.maxCpus       = SM_CPUS_MAX;
        smObjParams.maxTasks      = SM_OBJ_MAX_TASK;
        smObjParams.maxSems       = SM_OBJ_MAX_SEM;
        smObjParams.maxMsgQueues  = SM_OBJ_MAX_MSG_Q;
        smObjParams.maxMemParts   = SM_OBJ_MAX_MEM_PART;
        smObjParams.maxNames      = SM_OBJ_MAX_NAME;

        if (smObjSetup (&smObjParams) != OK)
            {
            if (errno == S_smObjLib_SHARED_MEM_TOO_SMALL)
               printf("\nERROR smObjSetup : shared memory pool too small.\n");

            if (allocatedPool)
                free ((char *)smObjFreeAdrs);	/* cleanup */

            return (ERROR);
            }
        }

    /*
     * Else, we are not the master CPU, so wait until the anchor and SMO
     * header regions are accessible (the master CPU has initialized them)
     * before continuing.
     */

    else
        {
        /* first wait for valid anchor region */

        tics = params.procNum;
        for (tics <<= 1; tics < maxWait; tics <<= 1)
            {
            smUtilDelay (NULL, tics);
            if ((smUtilMemProbe (smAnchor, VX_READ, sizeof (UINT),
                                 (char *)&temp) == OK) &&
                (ntohl (temp) == SM_READY))
                {
                break;
                }
            }

        if (tics >= maxWait)
            {
            printf ("usrSmObjInit: anchor @ %p unreachable. Giving up.\n",
                    smAnchor);
            return (ERROR);
            }

        /* Finally, wait for master to init SM Object facility */

        tics = params.procNum;
        cp = (char *)(&((SM_ANCHOR *)smAnchor)->smObjHeader);
        for (tics <<= 1; tics < maxWait; tics <<= 1)
            {
            if ((smUtilMemProbe (cp, VX_READ, sizeof (UINT), (char *)&temp)
                              == OK) &&
                (temp != (UINT)~0)   &&
                (temp != 0))
                {
                break;
                }
            smUtilDelay (NULL, tics);
            }

        pSmObjHdr = SM_OFFSET_TO_LOCAL (ntohl (temp), (int)smAnchor,
                                        SM_OBJ_MEM_HDR *);

        cp = (char *)(&pSmObjHdr->initDone);
        for ( ; tics < maxWait; tics <<= 1)
            {
            if ((smUtilMemProbe (cp, VX_READ, sizeof (UINT), (char *)&temp)
                              == OK) &&
                (ntohl (temp) == TRUE))
                {
                break;
                }
            smUtilDelay (NULL, tics);
            }

        if (tics >= maxWait)
            {
            printf ("usrSmObjInit: Error: time out awaiting SM Object init\n");
            return (ERROR);
            }
        }

    /* WIND00039499, WIND00045175: 
     * Set up task-level handling system: a task now waits on a semaphore, ready
     * to handle the events coming in from a remote CPU. The ISR work has been
     * cut to only signaling the task, instead of dequeueing and handling the
     * events itself. The workload proved in some occasions to take too long for
     * an ISR and the time spent in the ISR could even be unbounded on some
     * special cases.
     */
    smObjEventSemId = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
    if (NULL == smObjEventSemId)
	{
	printf ("usrSmObj: "
		"can't create event handler semaphore (errno = %#x)\n",
		errnoGet());
	return ERROR;
	}
    smObjEventTaskId = taskSpawn ("tSmObjTask", SM_OBJ_TASK_PRIORITY,
                                    SM_OBJ_TASK_OPTIONS, 0x2000,
				    (FUNCPTR)smObjTask,
				    0,0,0,0,0,0,0,0,0,0);
    if (ERROR == smObjEventTaskId)
	{
	printf ("usrSmObj: can't spawn event handler task (errno = %#x)\n",
		errnoGet());
	semDelete (smObjEventSemId);
	return ERROR;
	}
    /*
     * initialize shared memory descriptor
     *
     *  Note that SM_OBJ_MAX_TRIES is passed to smObjInit(),
     *  and is used to set the global int smObjSpinTries.
     *  This has already been done above to fix SPR68418.
     */

    smObjInit (&smObjDesc, (SM_ANCHOR *) smAnchor, sysClkRateGet (),
               SM_OBJ_MAX_TRIES, SM_INT_TYPE, SM_INT_ARG1,
	       SM_INT_ARG2, SM_INT_ARG3);

    /* attach to shared memory object facility */

    printf ("Attaching shared memory objects at %#x... ", (int) smAnchor);

    if (smObjAttach (&smObjDesc) != OK)
	{
	printf ("failed: errno = %#x.\n", errno);
	taskDelete (smObjEventTaskId);
	semDelete (smObjEventSemId);
        return (ERROR);
	}

    printf ("done\n");
    return (OK);
    }

