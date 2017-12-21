/* usrPassFs.c - VxSim pass-through filesystem initialization */

/*
 * Copyright (c) 1984-2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */


/*
modification history
--------------------
91h,26oct06,elp  include vxsimHostLibP.h
91g,13sep04,jmp  made passFs cache configurable.
91f,13aug03,dbt  Fixed buffer length.
91e,22may03,jmp  fixed passFsInit code.
91d,23apr03,jmp  made usrPassFsInit() common to all simulators.
01c,22apr03,dbt  Added SIMLINUX support.
01b,02apr03,jeg  adapted to be used with simsolaris and simnt. Cleanup.
01a,03jun99,ms   written
*/

/*
DESCRIPTION

User configurable pass-through filesystem intialization for VxSim.
*/

#include <passFsLib.h>
#include <private/vxsimHostLibP.h>

/******************************************************************************
*
* usrPassFsInit - initialize pass-through filesystem
* 
* This routine is used to initialize VxSim pass-through filesystem. It is used
* by both bsp and Project Facility build.
*
* RETURNS: OK if the filesystem was correctly initialized, ERROR otherwise.
*
* NOMANUAL
*/ 

STATUS usrPassFsInit
    (
    BOOL	cacheEnable	/* enable cache ? */
    )
    {
    char 	pathName [MAX_FILENAME_LENGTH];
    int		length;

    if (passFsInit (1, cacheEnable) != OK)
	return (ERROR);

    sprintf (pathName, "%s:", sysBootParams.hostName);

    if (passFsDevInit (pathName) == NULL)
	{
	printErr ("passFsDevInit failed for <%s>\n", pathName);
	return (ERROR);
	}

    length = strlen (pathName);

    /* Append current directory to path name */

    if (vxsimHostCurrentPathGet
		    (pathName + length, MAX_FILENAME_LENGTH - length) == ERROR)
	{
	printErr ("Unable to get current working directory\n");
	return (ERROR);
	}

    /* Set the current default path */

    ioDefPathSet (pathName);

    return (OK);
    }
