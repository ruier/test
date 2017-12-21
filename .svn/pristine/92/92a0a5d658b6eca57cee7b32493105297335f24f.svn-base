/* usrAta.c - ATA/ATAPI initialization */

/*
 * Copyright (c) 1995, 1997-2002, 2004-2005, 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02e,15nov10,sye  Fix page fault crash when booting with INCLUDE_PROTECT_TEXT.
                 (WIND00237214)
02d,15mar10,sye  Add ATA device existence check before calling ataDrv().
                 (WIND00090656)
02c,29jul09,e_d  Mount the exist ATA device to file system.(WIND00139792)
02b,10nov05,dee  SPR 114032; fix gnu compiler warnings.
02a,13oct05,dee  SPR# 113433, better error handling for non-existent ATA
                 drives
01z,15sep05,dee  moved pBlkDev into #if for block wrapper usage
01y,08sep05,act  SPR111810: map "partition 0" same as "partition 1", not
                 separately
01x,29aug05,dee  USE_ATA_BLOCK_WRAPPER to choose between native ata XBD
		 or block wrapper implementation.
01w,25aug05,pcm  replaced xbdBlkDevCreate() with xbdBlkDevCreateSync()
01v,09jun05,pcm  removed ATA_CACHE_SIZE
01u,08jun05,jlk  fixed bug with partition name mapping
01t,07jun05,jlk  fixed minor bug with name of partition 0
01s,07jun05,pcm  added XBD blk wrapper and file system monitor support
01r,03jun05,pcm  removed creation of DOS FS & CDROM FS as part of the
                 new FS framework
01p,22mar05,dcc  provisional disabling of DOS fs creation in usrAtaConfig()
01o,24sep04,jyo  Removing the transactional device creation. This needs to be
                 done by the user explicitly since the disk might also need
                 transactional formatting.
01n,23sep04,jyo  Changing TRANSBIO to TRANSCBIO to be consistent with the
                 filename.
01m,16sep04,jyo  Creating a transactional filesystem while mounting the DOS
                 filesystem.
01l,02mar02,jkf  SPR#73841, fixed (cdrom) trailing paran indexing error.
                 fixed warning: pn unitialized if INCLUDE_CDROMFS.
01k,26nov01,jac  added file system type check to allow support for specifying
                 CDROM filesystem.
01j,02ocy01,jkf  fixing SPR#70377, usrAtaConfig altered char * arg, this 
                 modifies .text seg and caused checksum error for tgtsvr. 
01i,21sep01,jkf  cbio API changes.
01h,21jun00,rsh  upgrade to dosFs 2.0
01g,14oct98,lrn  modified for DosFs 2.0
01f,30jul99,jkf  SPR#4429.
01e,02jun98,ms   created configlette.
01d,30oct97,db   added call to ATA_SWAP in usrAtaPartition. 
01c,30oct97,dat  added #include pccardLib.h and ataDrv.h
01b,14jul97,dgp  doc: update to match hard-copy
01a,11may95,hdn  re-written for ATA driver.
*/

/*
DESCRIPTION
This file is used to configure and initialize the VxWorks ATA support.
This file is included by the prjConfig.c configuration file created by the
Project Manager.


SEE ALSO: usrExtra.c

NOMANUAL
*/

#include <vxWorks.h>
#include <drv/hdisk/ataDrv.h>

#include <fsMonitor.h>
#include <xbdBlkDev.h>

/* imports */

IMPORT ATA_CTRL     ataCtrl[];

/* forward declaration */

/* macro's */

/*******************************************************************************
*
* usrAtaConfig - configure an ATA drive (hard disk or cdrom drive)
*
* This routine configures an ATA hard disk. Parameters:
*
* .IP <drive> 
* the drive number of the hard disk; 0 is `C:' and 1 is `D:'.
* .IP <devName>
* the mount point for all partitions which are expected to be present
* on the disk, separated with commas, for example "/ata0,/ata1" or "C:,D:".
* Blanks are not allowed in this string.
* .LP
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
* `src/config/usrAta.c',
* .pG "I/O System, Local File Systems, Intel i386/i486/Pentium"
*/

STATUS usrAtaConfig
    (
    int     ctrl,	 /* 0: primary address, 1: secondary address */
    int     drive,	 /* drive number of hard disk (0 or 1) */
    char    *devNames    /* mount points for each partition */
    )
    {
    char *       pDevNames;
    char *       pName;
    char *       pPartName[PART_MAX_ENTRIES];      
    ATA_DRIVE *  pDrive;
    int          i;
    char         fsmName[MAX_NAME_MAP];
    char         ataName[8];
    char         tmpName[8];
    int          nameLen;

    /* check argument sanity */

    if (ATA_MAX_DRIVES <= (UINT) drive)
	{
	printErr ("usrAtaConfig: drive is out of range (0-%d).\n", 
                  ATA_MAX_DRIVES - 1);

	return (ERROR);
	}

    if (ATA_MAX_CTRLS <= (UINT) ctrl)
        {
        printErr ("usrAtaConfig: controller is out of range (0-%d).\n",
                  ATA_MAX_CTRLS - 1);

        return (ERROR);
        }

    pDrive = &(ataCtrl[ctrl].drive[drive]);
    if(pDrive->type == ATA_TYPE_NONE)
        {
        return (ERROR);
        }
        
    if ((devNames == NULL) || (*devNames == EOS))
        {        
        pDevNames = tmpName;
        sprintf (pDevNames, "/ata%d%c", ctrl, 'a'+drive);
        nameLen = -1;
        }
    else
        {
        nameLen = strlen(devNames);
        if (nameLen > (MAX_NAME_MAP * PART_MAX_ENTRIES))
            {
            printErr("usrAtaConfig: length of devNames is out of range\n");
            return (ERROR);
            }
        pDevNames = calloc(nameLen + 1, sizeof(char));
        if (pDevNames == NULL)
            {
            printErr("usrAtaConfig: calloc returned NULL\n");
            return (ERROR);
            }
        strcpy(pDevNames, devNames);
        }
    pName = pDevNames;

    for (i = 0; i < PART_MAX_ENTRIES; i++)
        {
        pPartName[i] = pName;

        while ((*pName != ',') && (*pName != EOS))
            pName++;

        if (*pName == EOS)                  /* Any more names? */
            {
            i++;
            break;                          /* Break if no more. */
            }

        *pName = EOS;                       /* NULL terminate and go again. */
        pName++;
        }

    for (; i < PART_MAX_ENTRIES; i++)
        pPartName[i] = NULL;

    sprintf (ataName, "/ata%d%d", ctrl, drive);
    sprintf (fsmName, "%s:0", ataName);

    /*
     * The ATA driver always uses partitions, so the names of the XBDs
     * will be "/ata00:0" (if there is no partition table) or "/ata00:1"
     * through "/ata00:4" (if there are partitions; all assuming ctrl and
     * drive are both 0 in this case).
     *
     * If the supplied set of partition names was, e.g., "/p1,/p2", we
     * want to map "/ata00:0" and "/ata00:1" to "/fs1" and "/ata00:2"
     * to "/fs2".  In other words, even if there are no partitions, we
     * act as if the entire disk were "partition one" (/p1).
     *
     * Note that pPartName[0] != NULL (because *devNames != EOS), but
     * if the string begins with a comma (e.g., ",/p2"), pPartName[0]
     * could be an empty string.  In this case we go with the ata-name
     * (though perhaps we should forbid this case instead?).
     */
    fsmNameInstall (fsmName, pPartName[0][0] == EOS ? ataName : pPartName[0]);

    for (i = 0; i < PART_MAX_ENTRIES; i++)
        {
        if (pPartName[i] == NULL)
            break;
        else if (*pPartName[i] == EOS)
            continue;

        sprintf (fsmName, "%s:%d", ataName, i + 1);
        fsmNameInstall (fsmName, pPartName[i]);
        }

    if (nameLen != -1)
        {
        free(pDevNames);
        }

#if defined (USE_ATA_BLOCK_WRAPPER)
    {
    /* create block device for the entire disk, */
    BLK_DEV *    pBlkDev;
    
    if ((pBlkDev = ataDevCreate (ctrl, drive, 0, 0)) == (BLK_DEV *) NULL)
        {
        printErr ("Error during ataDevCreate: %d/%d %s %x\n",
		   ctrl, drive, ataName, errno);
        return (ERROR);
        }
    xbdBlkDevCreateSync (pBlkDev, ataName);
    }

#else
    if (ataXbdDevCreate(ctrl, drive, 0, 0, ataName) == (device_t)NULL)
        {
        printErr ("Error during ataXbdDevCreate: %d/%d %s %x\n",
		   ctrl, drive, ataName, errno);
        return (ERROR);
        }
#endif
    return (OK);
    }

/******************************************************************************
*
* usrAtaInit - intialize the hard disk driver
*
* This routine is called from usrConfig.c to initialize the hard drive.
*/

void usrAtaInit (void)
    {
    int ix;
    ATA_RESOURCE *pAtaResource;

    for (ix = 0; ix < ATA_MAX_CTRLS; ix++)
        {
        pAtaResource = &ataResources[ix];

        if ((pAtaResource->ctrlType == IDE_LOCAL) && 
            (pAtaResource->drives != 0))

            if ((ataDrv (ix, pAtaResource->drives, pAtaResource->intVector,
                   pAtaResource->intLevel, pAtaResource->configType,
                   pAtaResource->semTimeout, pAtaResource->wdgTimeout))
               == ERROR)
                {
                printf ("ataDrv returned ERROR from usrRoot.\n");
                }
        }
    }

