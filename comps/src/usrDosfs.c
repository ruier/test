/* usrDosfs.c - DOS file system library */

/*
 * Copyright 2005, 2009, 2012 - 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
01i,31jan13,xwu  Fix WIND00399652 dosFsCacheCreate() user shall be able
                 to define smaller cache size
01h,03jul12,yjl  Fix WIND00352399, FAT32: directory entry NT flag
                 compatibility with Windows should be configurable
01g,08jul09,zly  added global variable " UINT dosFsVolumeBioBufferSize" 
                 (WIND00161794) 
01f,18jun09,zly  added global variable " int dosFsMsgLevel"
01e,09aug05,dcc  removed disk cache initialization.
01d,03aug05,dcc  added disk cache initialization.
01c,03aug05,pcm  added parameters to usrDosfsInit()
01b,28jul05,pcm  added dosFsInit () to initialize DosFs components
01a,03jun05,pcm  written.
*/

/*
DESCRIPTION
*/

/* includes */

#include "vxWorks.h"

#include "dosFsLib.h"
#include "fsMonitor.h"

/* 
 * user`s printing message level :
 * 0  - print nothing 
 * 1  - print messages at level-1 
 * >1 - for future extension 
 */

int dosFsMsgLevel = 1;

/* size in blocks of cache group */

UINT BLOCKS_PER_DATA_DIR_CACHE_GROUP = 512;
UINT BLOCKS_PER_FAT_CACHE_GROUP      = 32;


/* size in blocks of a bio data buffer */

UINT dosFsVolumeBioBufferSize = 32;

LOCAL BOOL dosFsInitCalled = FALSE;

BOOL dosFsCompatNT = FALSE; /* dosFs Windows NT Compatible option */
/***************************************************************************
*
* dosFsInit -
*/

LOCAL STATUS dosFsInit
    (
    int  maxFiles,
    int  options
    )
    {
    if (dosFsInitCalled)
        return (OK);

    /* First initialize the main module */
    if (dosFsLibInit (maxFiles, options) == ERROR)
    return (ERROR);

    /* Initialize sub-modules */

#ifdef INCLUDE_DOSFS_FAT
    dosFsFatInit ();             /* Sub-module: FAT12/FAT16/FAT32 */
#endif                           /* FAT Handler */

#ifdef INCLUDE_DOSFS_DIR_VFAT
    dosVDirLibInit ();           /* Sub-module: VFAT Directory Handler */
#endif

#ifdef INCLUDE_DOSFS_DIR_FIXED
    dosDirOldLibInit ();         /* Sub-module: Vintage 8.2 and */
#endif                           /* VxLong Directory Handler */

#ifdef INCLUDE_DOSFS_CHKDSK
    dosChkLibInit ();            /* Sub-module: Consistency check handler */
#endif

#ifdef INCLUDE_DOSFS_FMT
    dosFsFmtLibInit ();          /* Sub-module: Formatter */
#endif

#ifdef INCLUDE_DOSFS_PRTMSG_LEVEL
    dosFsMsgLevel = DOSFS_PRTMSG_LEVEL;
#endif

#ifdef INCLUDE_DOSFS_DIR_VFAT
    dosFsCompatNT = DOSFS_COMPAT_NT;
#endif

#ifdef INCLUDE_DOSFS_VOL_BIO_BUFFER_SIZE
    dosFsVolumeBioBufferSize = DOSFS_DEFAULT_VOL_BIO_BUFFER_SIZE;
#endif

#ifdef INCLUDE_DOSFS_CACHE
    BLOCKS_PER_DATA_DIR_CACHE_GROUP =  DOSFS_BLOCKS_PER_DATA_DIR_CACHE_GROUP;
    BLOCKS_PER_FAT_CACHE_GROUP      =  DOSFS_BLOCKS_PER_FAT_CACHE_GROUP;
#endif

    dosFsInitCalled = TRUE ;
    return (OK);
    }

/***************************************************************************
*
* usrDosfsInit -
*/

STATUS usrDosfsInit
    (
    int  maxFiles,
    int  options
    )
    {
    int  error;

    if (dosFsInit (maxFiles, options) != OK)
        return (ERROR);

    error = fsmProbeInstall (dosFsDiskProbe, dosFsMonitorDevCreate);

    return ((error != OK) ? ERROR : OK);
    }
