/* usrEdrInit.c - ED&R configurable initialization */

/*
 * Copyright (c) 2003-2004, 2007, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01o,23aug10,cww  Include private header file
01n,13apr10,cww  LP64 adaptation
01m,01oct07,md   Defect 00095404, conditionally define parameters
01l,26aug04,md   add configurable record size 
01k,17aug04,md   use VM_PAGE_SIZE macro to get page size
01j,14may04,md   ensure default log size is useable
01i,15sep03,dbs  introduce N-deep buffering
01h,11sep03,dbs  move edr init diagnosis into configlette
01g,29aug03,dbs  update after review comments - remove printf's
01f,06aug03,dbs  allow for more than one PM arena
01e,31jul03,dbs  improve static buffer configuration
01d,28jul03,dbs  add better diagnostics to failure cases
                 move bulk of init into library proper
01c,18jul03,dbs  add more validation
01b,17jul03,dbs  accomodate changes to memLogLib API
01a,16jul03,dbs  written
*/

#include <vxWorks.h>
#include <pmLib.h>
#include <edrLib.h>
#include <private/edrLibP.h>

/* configuration parameters */

/*
 * Identifies the PM arena to be used by edrLib. To override this, you
 * need to define a function conforming to the PM_ARENA_DEF typedef,
 * and set this macro to refer to that function.
 */

#if !defined(EDR_PM_ARENA)
# define EDR_PM_ARENA pmDefaultArena
#endif

/*
 * Identifies the name by which ED&R refers to its region within the
 * PM arena.
 */

#if !defined(EDR_ERRLOG_NAME)
# define EDR_ERRLOG_NAME "edrErrorLog"
#endif

/* externs */

/**************************************************************************
*
* edrPmArena - defines the PM arena that ED&R will use for the error log
*
* This function uses the configuration parameter EDR_PM_ARENA to define
* the arena which ED&R will use for its error log.
*
* NOMANUAL
*/

void * edrPmArena
    (
    size_t *	pSize
    )
    {
    return EDR_PM_ARENA (pSize);
    }

/**************************************************************************
*
* edrPmRegion - returns the region-identifier of the ED&R region
*
* This function means that edrLib does not need a global variable to
* store its region identifier. Rather, it looks up the region (by
* name) whenever it needs to know it. This makes it less vulnerable to
* corruption since it has effectively no global state.
*
* NOMANUAL
*/

int edrPmRegion (void)
    {
    return pmRegionOpen (edrPmArena, EDR_ERRLOG_NAME);
    }

/**************************************************************************
*
* usrEdrInit - initializes the ED&R library edrLib
*
* This function opens a PM region in edrLib's chosen PM arena (which
* is, by default, the default PM arena as identified by
* pmDefaultArena)
*
* NOMANUAL
*/

STATUS usrEdrInit (void)
    {
    int  region;
    BOOL isNewRegion;

    /*
     * Need to get some memory from pmLib - first we init the arena
     * that ED&R wants to use.
     */

    if (pmInit (edrPmArena) == ERROR)
	{
	edrInitStatus = EDR_INIT_PM_FAILURE;
	return ERROR;
	}
    
    /* Try to open an existing region, if that fails, create a new one. */

    if ((region = pmRegionOpen (edrPmArena, EDR_ERRLOG_NAME)) == ERROR)
	{
	size_t size;

	/* determine size of error log */

#ifdef	EDR_ERRLOG_SIZE

	/* use specified size */

	size = EDR_ERRLOG_SIZE;
#else
	/* by default we'll use one half of the available space */

	size = pmFreeSpace (EDR_PM_ARENA) / 2;

	/* we need at least two pages to create the log */

	if (size < VM_PAGE_SIZE * 2)
	    size = VM_PAGE_SIZE * 2;
#endif

	/* region doesn't exist (or is corrupt) - create a new one */

	if ((region = pmRegionCreate (edrPmArena,
				      EDR_ERRLOG_NAME,
				      size,
				      PM_PROT_RDWR)) == ERROR)
	    {
	    edrInitStatus = EDR_INIT_pmRegionCreate_FAILED;
	    return ERROR;
	    }

	/* indicate that this was a new region */

	isNewRegion = TRUE;
	}
    else
	{
	/* Re-use existing region - indicate that this was an old region */

	isNewRegion = FALSE;
	}
	
    /* initialize the ED&R library fully */

    if (edrLibInit (isNewRegion, EDR_RECORD_SIZE) == ERROR)
	{
#ifdef INCLUDE_STDIO
	if (ioGlobalStdGet (STD_OUT) != ERROR)
	    {
	    const char* s = edrStatusMsg (edrInitStatus);

	    if (s != NULL)
		printf ("edrLibInit failed : %s\n", s);
	    else
		printf ("edrLibInit failed (reason unknown)\n");
	    }
#endif
	return ERROR;
	}

    return OK;
    }
