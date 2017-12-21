/* usrTffs.c - TFFS initialization */

/*
 * Copyright (c) 1997-1999, 2001, 2004-2006, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
01p,28jun12,zly  Fix WIND00341863: adding INCLUDE_TFFS_START_SECTOR_ZERO to 
                 config if compatible with old format.
01o,12dec10,sye  Remove devSplit() in usrTffsConfig(). (WIND00243043)
01n,09feb06,dee  SPR 114124, use xbdBlkDevCreateSync when creating block
		 device
01m,28sep05,dee  SPR#112977 cleanup warning "xbdBlkDevCreate not declared"
01l,21jun05,pcm  added XBD blk wrapper and file system monitor support
01k,23nov04,mil  Fixed compilation warning.
01j,24sep04,jyo  Removing the transactional device creation. This needs to be
		 done by the user explicitly since the disk might also need
		 transactional formatting.
01i,23sep04,jyo  Changing TRANSBIO to TRANSCBIO to be consistent with the
		 filename.
01h,16sep04,jyo  Creating a transactional filesystem while mounting the DOS
		 filesystem.
01g,07dec01,nrv  fixed diab warnings
01f,01oct01,yp   merging in T3 version, removed refs to dosFs compatibility layer
01e,02feb99,yp	 added function usrTffsLnConfig() to support DOS_OPT_LONGNAMES
01d,01apr98,hdn  moved tffsLoad() back to bootConfig.c.
01c,31dec97,yp	 doc cleanup
01b,01dec97,hdn  added tffsLoad() to minimize change to bootConfig.c.
01a,07nov97,hdn  written.
*/

/*
DESCRIPTION
This file is included by bootConfig.c and usrConfig.c. The file contains
routines for configuring a TFFS Flash disk to be used as file system as
with dosFs. The routines are used by the boot process to find and load
vxWorks images. 

SEE ALSO: usrExtra.c

NOMANUAL
*/

#ifndef  __INCusrTffs
#define  __INCusrTffs

/* includes */
#include <tffs/tffsDrv.h>
#include <tffs/flsocket.h>
#include <xbdBlkDev.h>
#include <fsMonitor.h>

/* forward declarations */
extern int flFileSysSectorStart;

/*******************************************************************************
*
* usrTffsConfig - mount the DOS file system on a TFFS Flash disk
*
* This routine mounts the vxWorks DOS file system on a TFFS Flash drive.
*
* The <drive> parameter is the drive number of the TFFS Flash drive;
* valid values are 0 through the number of socket interfaces in BSP.
*
* The <fileName> parameter is the mount point, e.g., `/tffs0/'.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
* .pG "I/O System, Local File Systems"
*/

STATUS usrTffsConfig
    (
    int     drive,	/* drive number of TFFS */
    int     removable,	/* 0 - nonremovable flash media */
    char *  fileName	/* mount point */
    )
    {
    BLK_DEV *  pBlkDev;
    char       tffsName[16];
    char       fsmName[16];

    if ((UINT)drive >= noOfDrives)
	{
	printErr ("drive is out of range (0-%d).\n", noOfDrives - 1);
	return (ERROR);
	}

    if ((fileName == NULL) || (*fileName == EOS))
        {
        printErr ("Mount point is not specified.\n");  
        return (ERROR);
        }

    #ifdef INCLUDE_TFFS_START_SECTOR_ZERO
	flFileSysSectorStart = 0;   /* 0 for dosFS, 1 for Reliance */
    #else
	flFileSysSectorStart = 1;
    #endif /* INCLUDE_TFFS_START_SECTOR_ZERO */

    sprintf (tffsName, "/tffs%d", drive);
    sprintf (fsmName, "%s:0", tffsName);     /* Create name for mapping. */
    if (fsmNameInstall (fsmName, fileName) == ERROR)	     /* Install name mapping. */
        {
        printErr ("Install name mapping failed.\n"); 
        return (ERROR);
        }

    /* create a block device spanning entire disk (non-distructive!) */

    if ((pBlkDev = tffsDevCreate (drive, removable)) == NULL)
	{
	printErr ("tffsDevCreate failed.\n");
	return (ERROR);
	}

    if (xbdBlkDevCreateSync (pBlkDev, tffsName) == NULLDEV)
        {
        printErr ("xbdBlkDevCreateSync failed.\n"); 
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* usrTffsLnConfig - mount the DOS file system on a TFFS Flash disk
*
* This routine mounts the vxWorks DOS file system on a TFFS Flash drive with
* long file name support.
*
* OBSOLETE: Version II of dosFs does not need special treatment for supporting 
* long filenames. This routine is provided to support legacy code only and
* will become obsolete soon.
*
* The <drive> parameter is the drive number of the TFFS Flash drive;
* valid values are 0 through the number of socket interfaces in BSP.
*
* The <fileName> parameter is the mount point, e.g., `/tffs0/'.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
* .pG "I/O System, Local File Systems"
*
* NOMANUAL
*/

STATUS usrTffsLnConfig
    (
    int     drive,	/* drive number of TFFS */
    int     removable,	/* 0 - nonremovable flash media */
    char *  fileName	/* mount point */
    )
    {
    
    if (usrTffsConfig (drive, removable, fileName) != OK)
	return (ERROR);

    return (OK);
    }
#endif /* __INCusrTffs */
