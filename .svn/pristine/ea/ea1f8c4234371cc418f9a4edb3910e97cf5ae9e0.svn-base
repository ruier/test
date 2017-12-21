/* usrRomfs.c - ROMFS initialization */

/* Copyright 2003, 2009 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,14may09,zly  LP64 support 
01b,07jul04,c_c  Don't use romfs_image.h anymore.
01a,15may03,md   written.
*/

/*
DESCRIPTION
This file is included by bootConfig.c and usrConfig.c. The file contains
routines for configuring a ROMFS file system using the system default
image.

SEE ALSO: usrExtra.c

NOMANUAL
*/

#ifndef  __INCusrRomfs
#define  __INCusrRomfs

/* includes */
#include "romfsLib.h"			/* read-only file system interfaces */
#include "romfsDrv.h"			/* romfs block device driver */
#include <stdio.h>


/* 
 * ROM filesystem image will come from romfs_image.o object file, which defines
 * two symbols (see below): 
 */

extern const char romfsImage[];               /* The file system */
extern const char romfsImageEnd[];    /* address of its end */

/* forward declarations */
STATUS usrRomfsConfig (void);

/*******************************************************************************
*
* usrRomfsConfig - mount the file system romfs image
*
* This routine mounts the included file system image on the /romfs mount point.
*
* RETURNS: OK or ERROR.
*
* SEE ALSO:
* .pG "I/O System, Local File Systems"
*/

STATUS usrRomfsConfig (void)
    {
    int romfsImageLen = romfsImageEnd-romfsImage;     /* size of ROM fs */

    if (romfsImageLen != 0)
	{
	BLK_DEV *pBlkDev;
	void *pVolDesc;

	pBlkDev=(BLK_DEV *)romfsBlkDevCreate((char *)&romfsImage,
					  romfsImageLen, 0);
	if (pBlkDev == NULL)
	    {
	    printErr ("Error creating romfs block device!\n");
	    return (ERROR);
	    }
	else
	    {
	    pVolDesc = (void *)romfsFsDevCreate ("/romfs", pBlkDev);
	    }
	}
    return (OK);
    }

#endif /* __INCusrRomfs */
