/* 00comp_hrfs.cdf - Highly Reliable File System component */

/*
 * Copyright 2005-2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02d,30dec11,zly  WIND00217466: add a direct mode for xbd. 
02c,15aug11,zly  fix for WIND00201989:xbd priority inversion issue. Change default 
                 priority from 50 to 200.
02b,03oct15,hui  remove INCLUDE_POSIX_PTHREADS and update some synopsis.
02a,03jul09,hui  add high speed write mode to HRFS
                 and put HRFS components into FOLDER_HRFS
01z,27sep07,dcc  added INCLUDE_IO_FILE_SYSTEM to HRFS REQUIRES list
01y,23aug07,act  io footprint work
01x,31jul07,h_k  updated INCLUDE_FS_MONITOR and replaced INCLUDE_WHEN with
                 _REQUIRES.
01w,12dec06,dcc  fixed defect WIND0070704:"xbdBlkDevCreate creates a task with  
                 hardcoded priority."
01v,01jun06,jlk  added commit policy stuff
01u,18jul06,mwv  correcting typos (WIND00057495)
01t,17jul06,mwv  updating DOSFS and HRFS default cache sizes (WIND00059268)
01s,18nov05,md   make INCLUDE_HRFS_READONLY and INCLUDE_HRFS exclusive
01r,15sep05,pcm  removed MODULES line from INCLUDE_XBD_RAMDRV
01q,15jul05,jlk  fixed a bug with the INCLUDE_XBD_BLK_DEV component
01p,26aug05,jlk  included INCLUDE_TRFS instead of INCLUDE_XBD_TRANS
01o,15jul05,vvv  fixed typo in INCLUDE_FS_EVENT_UTIL definition
01n,05aug05,jlk  added HRFS_INCLUDE_CHKDSK component
01m,03aug05,pcm  added HRFS_DEFAULT_xxx parameters
01l,15jul05,jlk  fixed a bug with the INCLUDE_XBD_BLK_DEV component
01k,11jul05,pcm  added LINK_SYMS xbdRamDiskDevCreate to INCLUDE_XBD_RAMDRV
01j,04jul05,jlk  added INCLUDE_XBD_PART_LIB component
01i,23jun05,jlk  added INCLUDE_FS_EVENT_UTIL
01h,16jun05,pcm  added more dependencies to INCLUDE_FS_MONITOR
01g,03jun05,pcm  INCLUDE_XBD depends upon INCLUDE_DEVICE_MANAGER
01f,02jun05,pcm  added xbdInit() to INCLUDE_XBD, and fsMonitorInit() to
                 INCLUDE_FS_MONITOR
01e,27may05,pcm  added INCLUDE_HRFS_READONLY
01d,26may05,pcm  added FS_MONITOR and XBD, and XBD_RAMDRV components
01c,09mar05,pcm  added INCLUDE_HRFS_FORMAT
01b,09mar05,pcm  added INCLUDE_XBD_BLK_DEV
01a,03mar05,pcm  written.
*/

/*
DESCRIPTION
This file contains the description for the HRFS component.
*/

Component INCLUDE_FS_EVENT_UTIL
    {
    NAME           File System Event Utilities
    SYNOPSIS       File System Event Utilities
    MODULES        fsEventUtilLib.o
    HDR_FILES      fsEventUtilLib.h
    INIT_RTN       fsEventUtilLibInit ();
    REQUIRES       INCLUDE_ERF
    INCLUDE_WHEN   INCLUDE_HRFS          \
                   INCLUDE_HRFS_READONLY \
                   INCLUDE_RAWFS         \
                   INCLUDE_CDROMFS       \
                   INCLUDE_XBD_TRANS     \
                   INCLUDE_DOSFS
    }

Component INCLUDE_FS_MONITOR
    {
    NAME           File System Monitor
    SYNOPSIS       File System Monitor
    MODULES        fsMonitor.o           \
                   xbdPartition.o
    HDR_FILES      fsMonitor.h
    INIT_RTN       fsMonitorInit ();
    REQUIRES       INCLUDE_RAWFS
    _REQUIRES      INCLUDE_CDROMFS       \
                   INCLUDE_DOSFS
    }

Parameter XBD_BLK_DEV_TASK_PRIORITY
    {
    NAME           Service task priority
    TYPE           int
    DEFAULT        200
    }

Parameter XBD_BLK_DEV_DIRECT_BIO_HANDLING
    {
    NAME		Direct BIO Handling Mode
    TYPE		BOOL
    DEFAULT		FALSE
}

Component INCLUDE_XBD
    {
    NAME           Extended Block Device
    SYNOPSIS       Extended Block Device
    MODULES        xbd.o                  \
                   bio.o
    INIT_RTN       xbdInit ();
    REQUIRES       INCLUDE_DEVICE_MANAGER \
                   INCLUDE_IO_REMOVABLE
    INCLUDE_WHEN   INCLUDE_HRFS           \
                   INCLUDE_HRFS_READONLY  \
                   INCLUDE_RAWFS          \
                   INCLUDE_CDROMFS        \
                   INCLUDE_XBD_TRANS      \
                   INCLUDE_DOSFS
    }

Component INCLUDE_XBD_BLK_DEV
    {
    NAME           XBD Block Device
    SYNOPSIS       XBD Block Device Wrapper
    HDR_FILES      xbdBlkDev.h
    INIT_RTN       xbdBlkDevLibInit (XBD_BLK_DEV_TASK_PRIORITY, \
                                     XBD_BLK_DEV_DIRECT_BIO_HANDLING);
    MODULES        xbdBlkDev.o
    CFG_PARAMS     XBD_BLK_DEV_TASK_PRIORITY \
                   XBD_BLK_DEV_DIRECT_BIO_HANDLING
    REQUIRES       INCLUDE_XBD
    }

Component INCLUDE_XBD_RAMDRV
    {
    NAME           XBD Ram Drive
    SYNOPSIS       XBD Ram Drive
    LINK_SYMS      xbdRamDiskDevCreate
    REQUIRES       INCLUDE_XBD
    }

Component INCLUDE_XBD_PART_LIB
    {
    NAME           XBD Disk Partition Handler
    SYNOPSIS       Supports disk partition tables on XBD devices
    REQUIRES       INCLUDE_XBD \
                   INCLUDE_RAWFS
    LINK_SYMS      partLibCreate xbdCreatePartition
    }


Parameter HRFS_DEFAULT_MAX_FILES
    {
    NAME           Default Max Open Files (HRFS)
    SYNOPSIS       Maximum number of files that can be open on an HRFS volume
    TYPE           int
    DEFAULT        10
    }

Parameter HRFS_DEFAULT_MAX_BUFFERS
    {
    NAME           Default Max Buffers (HRFS)
    SYNOPSIS       Maximum number of buffers
    TYPE           int
    DEFAULT        16
    }

Component INCLUDE_HRFS_DEFAULT_WRITE_MODE
    {
    NAME           HRFS Default Write Mode 
    SYNOPSIS       Commits performed every write
    REQUIRES       INCLUDE_HRFS
    }

Component INCLUDE_HRFS_HISPEED_WRITE_MODE
    {
    NAME           HRFS High Speed Write Mode
    SYNOPSIS       Commits performed when cache usage is over 40% or when 5 seconds have elapsed, if current transaction still active
    REQUIRES       INCLUDE_HRFS
    }

Component INCLUDE_HRFS
    {
    NAME           Highly Reliable File System
    SYNOPSIS       Highly Reliable File System 
    INIT_RTN       usrHrfsInit (HRFS_DEFAULT_MAX_BUFFERS,HRFS_DEFAULT_MAX_FILES);
    CONFIGLETTES   usrHrfs.c
    HDR_FILES      hrFsLib.h
    EXCLUDES       INCLUDE_HRFS_READONLY
    CFG_PARAMS     HRFS_DEFAULT_MAX_BUFFERS     \
                   HRFS_DEFAULT_MAX_FILES
    REQUIRES       INCLUDE_XBD                  \
                   INCLUDE_IO_FILE_SYSTEM       \
                   INCLUDE_FS_MONITOR           \
                   SELECT_HRFS_WRITE_MODE_INIT
    }

Component INCLUDE_HRFS_READONLY
    {
    NAME           Read-Only Highly Reliable File System
    SYNOPSIS       Read-Only Highly Reliable File System
    INIT_RTN       usrHrfsInit (HRFS_DEFAULT_MAX_BUFFERS,HRFS_DEFAULT_MAX_FILES);
    CONFIGLETTES   usrHrfs.c
    CFG_PARAMS     HRFS_DEFAULT_MAX_BUFFERS    \
                   HRFS_DEFAULT_MAX_FILES
    EXCLUDES	   SELECT_HRFS_WRITE_MODE_INIT \
                   INCLUDE_HRFS       
    REQUIRES       INCLUDE_XBD                 \
                   INCLUDE_IO_FILE_SYSTEM      \
                   INCLUDE_FS_MONITOR
    }

Component INCLUDE_HRFS_ACCESS_TIMESTAMP
    {
    NAME           Highly Reliable File System Access Time Stamp
    SYNOPSIS       Support access time stamp on HRFS volumes
    REQUIRES       INCLUDE_HRFS
    }

Component INCLUDE_HRFS_FORMAT
    {
    NAME           HRFS Format
    SYNOPSIS       Module for formatting backing media for HRFS
    MODULES        hrfsFormatLib.o
    INIT_RTN       hrfsFormatLibInit ();
    HDR_FILES      hrFsLib.h
    REQUIRES       INCLUDE_RAWFS \
                   INCLUDE_HRFS
    INIT_AFTER     INCLUDE_RAWFS
    }

Component INCLUDE_HRFS_CHKDSK
    {
    NAME           HRFS File System Consistency Checker
    SYNOPSIS       HRFS File System Consistency Checker
    MODULES        hrfsChkDskLib.o
    LINK_SYMS      hrfsChkDsk
    REQUIRES       INCLUDE_HRFS
    }

Selection SELECT_HRFS_WRITE_MODE_INIT
    {
    NAME           select HRFS work mode
    SYNOPSIS       default mode or high speed mode
    COUNT          1-1
    CHILDREN       INCLUDE_HRFS_DEFAULT_WRITE_MODE \
                   INCLUDE_HRFS_HISPEED_WRITE_MODE
    DEFAULTS       INCLUDE_HRFS_DEFAULT_WRITE_MODE
    }

Folder FOLDER_HRFS
    {
    NAME           Highly Reliable File System Components (HRFS)
    SYNOPSIS       Highly Reliable File System, and related components
    CHILDREN       SELECT_HRFS_WRITE_MODE_INIT   \
                   INCLUDE_HRFS                  \
                   INCLUDE_HRFS_READONLY         \
                   INCLUDE_HRFS_ACCESS_TIMESTAMP \
                   INCLUDE_HRFS_FORMAT           \
                   INCLUDE_HRFS_CHKDSK
    DEFAULTS       INCLUDE_HRFS
                   SELECT_HRFS_WRITE_MODE_INIT
    }
