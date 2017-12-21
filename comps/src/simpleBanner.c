/* simpleBanner.c - print a simple banner to the console */

/*
 * Copyright (c) 2005, 2007-2010 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01a,24feb10,mdk  update copyright to 2010
01c,06jul09,???  Updating version number for VxWorks 6.8
01b,08nov07,kk   update copyright for banner
01a,13oct05,yvp   written
*/

/*
DESCRIPTION
Print a short banner containing the runtime version and creation date
*/

#include <vxWorks.h>
#include <fioLib.h>


/******************************************************************************
*
* simpleBanner - print a simple banner to the console 
*
* This routine prints out a simple banner to the console containing the
* runtime version and creation date. This banner is only used in the 
* scaled down kernel profiles. 
*
* RETURNS: N/A.
*
* \NOMANUAL
*/
 
void simpleBanner (void)
    {
#if (defined INCLUDE_SHELL_BANNER) || (defined INCLUDE_WDB_BANNER)
    return;
#else

    printf ("\nCopyright 1984-2010 Wind River Systems, Inc.\n");
    printf ("%s %s\n", runtimeName, runtimeVersion);
    printf ("Created: %s\n\n", creationDate);
#endif /* (defined INCLUDE_SHELL_BANNER) || (defined INCLUDE_WDB_BANNER) */
    }
