/* usrTffs.c - TFFS initialization */

/*
 * Copyright (c) 2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
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
#include <private/bootFsLoad.h>

/* forward declarations */
//extern STATUS getFlashWLock();
//extern STATUS freeFlashLock();
BOOL flashInit = FALSE;
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

IMPORT STATUS sysGetBootLock();
IMPORT STATUS sysFreeBootLock();
IMPORT UINT32 sysFpgaTimerClkFreqGet();


STATUS usrTffsConfig
    (
    int     drive,	/* drive number of TFFS */
    int     removable,	/* 0 - nonremovable flash media */
    char *  fileName	/* mount point */
    )
    {
    BLK_DEV *  pBlkDev;
    char       devName [BOOT_FILE_LEN];
    char       tffsName[16];
    char       fsmName[16];
	UINT beginTimeH,beginTimeL,endTimeH,endTimeL;
	float timeSpend;
#ifdef INCLUDE_TFFS_MOUNT 
    //int cpuId;
#endif
   	   
    if ((UINT)drive >= noOfDrives)
	{
	printErr ("drive is out of range (0-%d).\n", noOfDrives - 1);
	return (ERROR);
	}
    ns16550Pollprintf("usrTffsConfig:wait for boot lock\n");
    {
    	vxTimeBaseGet(&beginTimeH,&beginTimeL);
    	while(sysGetBootLock() != OK)
		{
			vxTimeBaseGet(&endTimeH,&endTimeL);
			if(endTimeL<beginTimeL) timeSpend = (endTimeL+0xffffffff-beginTimeL) + (endTimeH-1-beginTimeH)*4.0*1024*1024*1024;
			else  timeSpend = (endTimeL-beginTimeL) + (endTimeH-beginTimeH)*4.0*1024*1024*1024;
			timeSpend = timeSpend/125000000;
			if(timeSpend>=10)
			{
				ns16550Pollprintf("timeout\n");
				break;
			}  
		}
    }
   
    ns16550Pollprintf("usrTffsConfig:get boot lock\n");
    sprintf (tffsName, "/tffs%d", drive);
    sprintf (fsmName, "%s:0", tffsName);     /* Create name for mapping. */
    devSplit (fileName, devName);	     /* Split boot device from file. */
    fsmNameInstall (fsmName, devName);	     /* Install name mapping. */
    //ns16550Pollprintf("usrTffsConfig1\n");
    /* create a block device spanning entire disk (non-distructive!) */
    if ((pBlkDev = tffsDevCreate (drive, removable)) == NULL)
	{
	printErr ("tffsDevCreate failed.\n");
	if(sysFreeBootLock() != OK) printErr ("free lock fail!!.\n");
	ns16550Pollprintf("usrTffsConfig:free boot lock\n");
	return (ERROR);
	}
    //ns16550Pollprintf("usrTffsConfig2\n");
    xbdBlkDevCreateSync (pBlkDev, tffsName);
    if(sysFreeBootLock() != OK) printErr ("free lock fail!!.\n");
	ns16550Pollprintf("free boot lock\n");
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
