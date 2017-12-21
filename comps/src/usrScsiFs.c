/* usrScsiFs.c -- SCSI filesystem initialization */

/*
 * Copyright (c) 2003-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* 
modification history
--------------------
01l,25aug08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
01k,16oct07,pmr  replaced xbdBlkDevCreate() with xbdBlkDevCreateSync()
01j,31jul07,h_k  disabled SCSI support
01i,24apr06,pcm  amended default SCSI partition naming policy (SPR 120242)
01h,23jun05,pcm  fixed up compile errors
01g,09jun05,pcm  removed SCSI_CACHE_SIZE
01f,08jun05,pcm  added XBD blk wrapper and file system monitor support
01e,03jun05,pcm  removed initialization of DOS FS and CDROM FS as part of new
                 file system framework
01d,24sep04,jyo  Removing the transactional device creation. This needs to be
                 done by the user explicitly since the disk might also need
                 transactional formatting.
01c,23sep04,jyo  Changing TRANSBIO to TRANSCBIO to be consistent with the
                 filename.
01b,16sep04,jyo  Creating a transactional filesystem while mounting the DOS
                 filesystem.
01a,20jan03,tor	written
*/

/*
DESCRIPTION
This file is used to configure and initialize a VxWorks filesystem
based on a SCSI device.
This file is included by the prjConfig.c configuration file created
by the Project Manager.


SEE ALSO: usrExtra.c

NOMANUAL
*/


/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <scsiLib.h>
#include <fsMonitor.h>
#include <xbdBlkDev.h>

#ifdef	_WRS_CONFIG_SMP
#error	SCSI is not supported in SMP!
#endif	/* _WRS_CONFIG_SMP */

/* defines */

#define SCSIFS_ERR_RETURN(msg)	\
			{ \
			printErr(msg); \
			return(ERROR); \
			}

/* typedefs */

/* globals */

IMPORT SCSI_CTRL * pSysScsiCtrl;	/* default SCSI_CTRL */

/* locals */

/* forward declarations */

STATUS usrScsiFsConfig
    (
    int		scsiFsDevBusId,
    int		scsiFsDevLun,
    int		scsiFsDevReqSenseLen,
    int		scsiFsDevType,
    BOOL	scsiFsDevRemovable,
    int		scsiFsDevNumBlocks,
    int		scsiFsDevBlkSize,
    int		scsiFsOffset,
    char *	scsiFsMountPoints
    );

/*******************************************************************************
*
* usrScsiFsConfig - Configure a SCSI device
*
* This routine configures a SCSI hard disk or CD-ROM device.
* Parameters:
*
* .IP <SCSI_FS_DEV_BUS_ID>
* the SCSI BUS ID of the device to be mounted
* .IP <SCSI_FS_DEV_LUN>
* the SCSI Logical Unit Number of the device to be mounted
* .IP <SCSI_FS_DEV_REQ_SENSE_LEN>
* the size of the device's REQUEST SENSE frame
* .IP <SCSI_FS_DEV_TYPE>
* the type of drive
* .IP <SCSI_FS_DEV_REMOVABLE>
* whether or not the drive is removable
* .IP <SCSI_FS_DEV_NUM_BLOCKS>
* the number of blocks on the device
* .IP <SCSI_FS_DEV_BLK_SIZE>
* the size of a single block on the device
* .IP <SCSI_FS_OFFSET>
* the offset to the beginning of the filesystem
* .IP <SCSI_FS_MOUNT_POINT>
* the name(s) of the filesystem mount points
*
* The SCSI_FS_MOUNT_POINT argument is a string which specifies
* the names of mount points for the disk partitions.  The string
* consists of a series of comma-separated mount points.  Any
* spaces in the string will be included in the filesystem names,
* so care should be taken not to include them.  There may be up
* to PART_MAX_ENTRIES (24) partitions specified.
* 
* EXAMPLE:
*    usrScsiFsConfig (2,0,0,0,FALSE,0,512,0,"/scsi2a,/scsi2b");
*
*  RETURNS: none
*
*  ERRNO: not set
*/

STATUS usrScsiFsConfig
    (
    int		scsiFsDevBusId,
    int		scsiFsDevLun,
    int		scsiFsDevReqSenseLen,
    int		scsiFsDevType,
    BOOL	scsiFsDevRemovable,
    int		scsiFsDevNumBlocks,
    int		scsiFsDevBlkSize,
    int		scsiFsOffset,
    char *	scsiFsMountPoints
    )
    {
    SCSI_CTRL *		pScsiCtrl;
    SCSI_PHYS_DEV *	pScsiPhysDev;
    BLK_DEV *		pBlkDev;
    char *              pDevNames;
    char *              pName;
    char *              pPartName[PART_MAX_ENTRIES];
    int                 i;
    char                fsmName[16];
    char                scsiName[8];


    /* sanity checks */

    if ((scsiFsDevBusId < SCSI_MIN_BUS_ID) ||
        (scsiFsDevBusId > SCSI_MAX_BUS_ID))
	SCSIFS_ERR_RETURN("usrScsiFsConfig: Illegal BUS ID\n");

    if ((scsiFsDevLun < SCSI_MIN_LUN) || (scsiFsDevLun > SCSI_MAX_LUN))
	SCSIFS_ERR_RETURN("usrScsiFsConfig: Illegal LUN\n");

    /* NOTE: numBlocks specifies maximum of 1Terrabyte disk size */
    if (scsiFsDevNumBlocks < 0)
	SCSIFS_ERR_RETURN("usrScsiFsConfig: Invalid number of blocks\n");

    if (scsiFsDevBlkSize < 64)
	SCSIFS_ERR_RETURN("usrScsiFsConfig: Illegal block size\n");

    if (scsiFsOffset < 0)
	SCSIFS_ERR_RETURN("usrScsiFsConfig: Illegal (negative) offset\n");

    if ((scsiFsMountPoints == NULL) || (*scsiFsMountPoints == EOS))
	SCSIFS_ERR_RETURN("usrScsiFsConfig: No Mount point specified\n");

    /* verify SCSI initialized & present */
    if (pSysScsiCtrl == NULL)
	SCSIFS_ERR_RETURN ("usrScsiFsConfig: SCSI Subsystem not initialized\n");

    pDevNames = malloc (1 + strlen (scsiFsMountPoints));  /* Allocate memory */
    if (pDevNames == NULL)                                /* to create a     */
        {                                                 /* duplicate of    */
        printErr ("usrAtaConfig: malloc returned NULL\n");/* <devNames>.     */

        return (ERROR);
        }

    strcpy (pDevNames, scsiFsMountPoints);
    pName = pDevNames;

    i = 0;
    while (i < PART_MAX_ENTRIES)
        {
        pPartName[i] = pName;
        i++;

        while ((*pName != ',') && (*pName != EOS))
            pName++;

        if (*pName == EOS)                 /* Any more names? */
            break;                         /* Break if no more. */

        *pName = EOS;                      /* Replace comma with EOS */
        pName++;
        }

    for (; i < PART_MAX_ENTRIES; i++)
        pPartName[i] = NULL;

    sprintf (scsiName, "/scsi%d", scsiFsDevLun);
    sprintf (fsmName, "%s:0", scsiName);

    fsmNameInstall (fsmName, pPartName[0][0] == EOS ? scsiName : pPartName[0]);

    for (i = 0; i < PART_MAX_ENTRIES; i++)
        {
        if (pPartName[i] == NULL)
            break;
        else if (*pPartName[i] == EOS)
            continue;

        sprintf (fsmName, "%s:%d", scsiName, i + 1);
        fsmNameInstall (fsmName, pPartName[i]);
        }


    /* probe SCSI bus */
    scsiAutoConfig (pSysScsiCtrl);

#ifdef SYS_SCSI_CONFIG
    /* run BSP configuration */
    sysScsiConfig ();
#endif SYS_SCSI_CONFIG

    /* create physical device for the entire disk */
    pScsiPhysDev = scsiPhysDevCreate (pSysScsiCtrl,
                                      scsiFsDevBusId, scsiFsDevLun,
                                      scsiFsDevReqSenseLen, scsiFsDevType,
                                      scsiFsDevRemovable, scsiFsDevNumBlocks,
                                      scsiFsDevBlkSize);
    if (pScsiPhysDev == NULL)
	{
	if (pSysScsiCtrl != NULL)
	    {
	    /* check if scsiAutoConfig() created physical device already */
	    for (i = 0 ; i < SCSI_MAX_PHYS_DEVS ; i++)
		{
		pScsiPhysDev = pSysScsiCtrl->physDevArr[i];
		if (pScsiPhysDev != NULL)
		    {
		    /*
                     * The SCSI_PHYS_DEV struct stores the LUN of the dev,
		     * but not the ID.  We verify what we're able.
                     */

		    if (pScsiPhysDev->scsiDevLUN == scsiFsDevLun)
		        break;
		    }
		}
	    }
	}

    if (pScsiPhysDev == NULL)
        {
        printErr ("usrScsiFsConfig: Can't create SCSI_PHYS_DEV\n");
        return (ERROR);
        }

    /* create block device for the entire disk */
    pBlkDev = scsiBlkDevCreate (pScsiPhysDev,
                                scsiFsDevNumBlocks, scsiFsOffset);
    if (pBlkDev == NULL)
        {
	printErr ("usrScsiFsConfig: Can't create BLK_DEV\n");
        return (ERROR);
        }

    xbdBlkDevCreateSync (pBlkDev, scsiName);
    free (pDevNames);

    return(OK);
    }
