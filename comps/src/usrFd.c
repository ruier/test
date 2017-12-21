/* usrFd.c - floppy disk initialization */

/*
 * Copyright (c) 1992-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01s,25aug05,pcm  replaced xbdBlkDevCreate() with xbdBlkDevCreateSync()
01r,16jun05,pcm  added include "fsMonitor.h" to fix up for BSP build
01q,09jun05,pcm  removed FD_CACHE_SIZE
01p,07jun05,jlk  added XBD blk wrapper and file system monitor support
01o,03jun05,pcm  removed dosFs creation as part of new FS framework
01n,23mar05,dcc  temporal disabling of dosFs creation.
01m,24sep04,jyo  Removing the transactional device creation. This needs to be
                 done by the user explicitly since the disk might also need
                 transactional formatting.
01l,23sep04,rfr  Fixed syntax error
01k,23sep04,jyo  Changing TRANSBIO to TRANSCBIO to be consistent with the
                 filename.
01j,16sep04,jyo  Creating a transactional filesystem while mounting the DOS
                 filesystem.
01i,07may04,zmm  Fix diab compiler warning.
01h,21sep01,jkf  cbio API changes.
01g,21jun00,rsh  upgrade to dosFs 2.0
01f,04nov98,lrn  fixed return value checking for dosFsDevCreate()
01e,14oct98,lrn  modified for DosFs 2.0

01d,28jun95,hdn  doc change.
01c,24jan95,jdi  doc cleanup.
01b,25oct94,hdn  swapped 1st and 2nd parameter of fdDevCreate() and
		 usrFdConfig().
01a,25oct93,hdn  written.
*/

/*
DESCRIPTION
This file is used to configure and initialize the VxWorks floppy disk support.
This file is included by the prjConfig.c configuration file created by thge Project Manager.

NOMANUAL
*/

#include "vxWorks.h"
#include "xbdBlkDev.h"
#include "fsMonitor.h"

/* forward declaration */

/* macro's */

/*******************************************************************************
*
* usrFdConfig - configure a floppy drive
*
* This routine configures a floppy drive device.
*
* The <drive> parameter is the drive number of the floppy disk;
* valid values are 0 to 3.
*
* The <type> parameter specifies the type of diskette, which is described
* in the structure table `fdTypes[]' in sysLib.c.  <type> is an index to
* the table.  Currently the table contains two diskette types:
* .iP "" 4
* A <type> of 0 indicates the first entry in the table (3.5" 2HD, 1.44MB);
* .iP
* A <type> of 1 indicates the second entry in the table (5.25" 2HD, 1.2MB).
* .LP
*
* The <fileName> parameter is the mount point, e.g., `/fd0/'.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
* .pG "I/O System, Local File Systems, Intel i386/i486 Appendix"
*/

STATUS usrFdConfig
    (
    int     drive,	/* drive number of floppy disk (0 - 3) */
    int     type,	/* type of floppy disk */
    char *  fileName	/* mount point */
    )
    {
    BLK_DEV *pBootDev;
    char     bootDir [BOOT_FILE_LEN];
    char     fdName[8];

    if (type == NONE)
	return OK;

    if ((UINT)drive >= FD_MAX_DRIVES)
	{
	printErr ("drive is out of range (0-%d).\n", FD_MAX_DRIVES - 1);
	return (ERROR);
	}

    /* split off boot device from boot file */

    devSplit (fileName, bootDir);

    /* Create a name for the name mapping in the FSM */
    sprintf(fdName, "fd%d:0", drive);

    /* Install name mapping */
    fsmNameInstall (fdName, bootDir);
    
    /* Create block device for floppy */
    if ((pBootDev = fdDevCreate (drive, type, 0, 0)) == NULL)
    	{
        printErr ("fdDevCreate failed.\n");
        return (ERROR);
        }

    sprintf(fdName, "fd%d", drive);

    /* Create an XBD wrapper */
    xbdBlkDevCreateSync (pBootDev, fdName);

    return (OK);
    }

