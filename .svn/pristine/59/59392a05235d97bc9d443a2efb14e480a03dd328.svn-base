/* usrDosDisk.c - DOS disk configuration file */

/* Copyright 1984-2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,16jun03,dbt  Fixed u_progname use.
01a,02jun98,ms   written
*/

/*
DESCRIPTION

User configurable DOS disk intialization used by VxSim.
*/

/******************************************************************************
*
* usrDosDiskInit - initialize the DOS pass-through FS.
*/ 

STATUS usrDosDiskInit (void)
    {
    char unixName [80];
    extern void unixDrv ();
    extern void unixDiskInit ();
    char *pLastSlash;

    unixDrv ();

    pLastSlash = strrchr (sysBootParams.bootFile, '/');
    pLastSlash = (pLastSlash == NULL) ? sysBootParams.bootFile :
    					(pLastSlash + 1);
    sprintf (unixName, "/tmp/%s%d.dos", pLastSlash, sysProcNumGet());
    unixDiskInit (unixName, "A:", 0);

    return (OK);
    }
