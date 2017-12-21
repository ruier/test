/* usrMemEdr.c - memory ED&R initialization */

/*
 * Copyright 2003-2004, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */

/*
modification history
--------------------
01c,17jan11,cww  Removed use of malloc in memEdrInit2
01b,24mar04,zl   added INCLUDE_MEM_EDR_RTC.
01a,02dec03,zl 	 created
*/

/*
DESCRIPTION
The file contains routines for configuring the memory error detection and
reporting tools.

SEE ALSO: memEdrLib, memEdrShow
*/

/* includes */

#include <vxWorks.h>
#include <private/memEdrLibP.h>

#ifdef INCLUDE_MEM_EDR_SHOW
extern STATUS  memEdrShowInit ();
#endif

#ifdef INCLUDE_MEM_EDR_RTC
extern void __rtc_init ();
#endif

/*******************************************************************************
*
* memEdrInit - init routine called before heap is created
*
* This routine performs library initialization that must be done before the
* system memory partition (kernel heap) is created.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void memEdrInit (void)
    {
    /* set configuration */

    memEdrConfigInit (MEDR_EXTENDED_ENABLE, MEDR_FILL_FREE_ENABLE, 
		      MEDR_FREE_QUEUE_LEN, MEDR_BLOCK_GUARD_ENABLE);

    memEdrDb.isKernel = TRUE;
    }


/*******************************************************************************
*
* memEdrInit2 - init routine called after heap is created
*
* This routine performs library initialization that must be done after the
* system memory partition (kernel heap) is created.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void memEdrInit2 (void)
    {
    size_t poolSize = MEDR_POOL_SIZE;

    /* initialize infrastructure */

    if (memEdrMemInit (NULL, poolSize) == ERROR)
	{

	/* undo any initialization performed up to this point */

	memEdrConfigClear ();

	sysExcMsg += sprintf (sysExcMsg, "memEdr: error initializing memEdr "
			      "infrastructure\n");
	return;
	}

    /* done with initializations */

#ifdef INCLUDE_MEM_EDR_RTC
    __rtc_init ();
#endif

    memEdrDb.memEdrInitialized = TRUE;
    }
