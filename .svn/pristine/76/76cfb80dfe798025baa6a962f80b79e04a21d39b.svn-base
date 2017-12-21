/* 10dosfs2.cdf - Kernel component definitions for dosfs2 */

/*
 * Copyright (c) 1998-2007, 2009, 2010, 2012 - 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02o,31jan13,xwu  Fix WIND00399652 dosFsCacheCreate() user shall be able to
                 define smaller cache size
02n,24dec12,ghs  Fix WIND00395476
02m,03jul12,yjl  Fix WIND00352399, FAT32: directory entry NT flag
                 compatibility with Windows should be configurable
02l,10jun10,jxu  removed INCLUDE_CBIO and INCLUDE_DISK_PART from LP64 build.
02k,14aug09,hps  update dosfs cache name
02j,03aug09,hps  requirement WIND00142043, group cache with background flush
02i,22jul09,yjl  requirement WIND00142043,change dosfs cache
02h,08jul09,zly  Added INCLUDE_DOSFS_VOL_BIO_BUFFER_SIZE component
                 (WIND00161794)
02g,22jun09,zly  Added INCLUDE_DOSFS_PRTMSG_LEVEL component
02f,02oct07,dcc  fixed defect WIND00104041
02e,27sep07,dcc  added INCLUDE_IO_FILE_SYSTEM to DOSFS REQUIRES list
02d,03nov06,pcm  added configuration parameters for DosFS Cache
02c,25oct06,jlk  added INCLUDE_DOSFS_SHOW to INCLUDE_DOSFS
02b,21aug06,pcm  added INCLUDE_DOSFS_SHOW
02a,15aug06,kuo  added SYNOPSIS field to INCLUDE_DOSFS_MAIN and 
                 INCLUDE_DOSFS_FAT (WIND00024111)
01z,18jul06,mwv  correcting typos (WIND00057495)
01y,17jul06,mwv  updating DOSFS and HRFS default cache sizes (WIND00059268)
01x,07mar06,jlk  SPR 118681. Removed MODULES line from INCLUDE_RAM_DISK
01w,01mar06,pcm  added SYNOPSIS field to INCLUDE_RAM_DISK (SPR 118008)
01v,13feb06,pcm  fixed typo in INCLUDE_DOSFS_DIR_FIXED component
01u,13sep05,pcm  fixed xbdRamDiskDevCreate() parameters
01t,17aug05,act  add xbdTransInit to INCLUDE_XBD_TRANS component
01s,09aug05,dcc  fixed parameters passed to usrDosfsInit().
01r,03aug05,dcc  added cache size parameter.
01q,03aug05,pcm  added DosFs Parameters DOSFS_DEFAULT_xxx
01p,28jul05,pcm  cleaned up DosFs components
01o,23jun05,jlk  Added INCLUDE_FS_EVENT_UTIL
01n,13jun05,pcm  DOSFS_FMT requires RAWFS instead of DOSFS_MAIN
01m,08jun05,pcm  Changed TRANSCBIO to XBD_TRANS and DISK_CACHE to DOSFS_CACHE
01l,07jun05,jlk  fixed trailing '\' in FOLDER_DOSFS2 InitGroup usrDosFsInit
01k,06jun05,pcm  changed INCLUDE_DOSFS_MAIN's INIT_RTN to usrDosfsInit()
01j,06jun05,pcm  removed some CBIO dependencies and fixed up INCLUDE_RAM_DISK
                 for use with file system monitor
01i,26may05,pcm  removed INCLUDE_RAWFS (already existed in 00vxWorks.cdf)
01h,01oct04,fle  Removed // comments
01j,24sep04,rfr  Added usrTransLib.o
01i,23sep04,jyo  Changing TRANSBIO to TRANSCBIO to be consistent with the file
                 name.
01h,15sep04,jyo  Adding the component descriptions for Transactional File
                 System.
01g,27mar02,jkf  SPR#74716, moved INCLUDE_DISK_UTIL to 00vxWorks.cdf
01f,07mar02,jkf  fixed SPR#73968, removed INCLUDE_FTPD_ANONYMOUS_ACCESS
01e,03oct01,jkf  removed ramDiskDevCreate declaration.
01d,27sep01,jkf  veloce changes, added new headers
01c,10oct99,jkf  changed for cbioLib public API.
01b,15oct98,lrn  added RAWFS section override
01a,07oct98,lrn  written


DESCRIPTION
  This file contains descriptions for DosFs 2.0 component release

*/

/* DosFs 2.0 module description */

Parameter DOSFS_DEFAULT_MAX_FILES
        {
        NAME            Default Max Open Files (DosFs)
        TYPE            int
        DEFAULT         20
        }

Parameter DOSFS_DEFAULT_CREATE_OPTIONS
        {
        NAME            Default DosFs Create Options
        TYPE            int
        DEFAULT         0x04      /* DOS_CHK_NONE */
        }

Parameter DOSFS_COMPAT_NT
        {
        NAME            dosFs Windows NT Compatible Option
        TYPE            bool
        DEFAULT         FALSE      
        }

Parameter DOSFS_DEFAULT_DATA_DIR_CACHE_SIZE
        {
        NAME            Default per-dosFs-volume disk data and directory group cache size
        TYPE            unsigned
        DEFAULT         0x800000 /* 8M */
        }

Parameter DOSFS_DEFAULT_FAT_CACHE_SIZE
        {
        NAME            Default per-dosFs-volume disk FAT group cache size
        TYPE            unsigned
        DEFAULT         0x100000 /* 1M */
        }

Parameter DOSFS_BLOCKS_PER_DATA_DIR_CACHE_GROUP
        {
        NAME            Default size in blocks of cache group per-data-dir
        TYPE            unsigned
        DEFAULT         512
        }

Parameter DOSFS_BLOCKS_PER_FAT_CACHE_GROUP
        {
        NAME            Default size in blocks of cache group per-fat
        TYPE            unsigned
        DEFAULT         32
        }

Parameter DOSFS_CACHE_BACKGROUND_FLUSH_TASK_ENABLE
        {
        NAME            enable background flush task for dosFs cache
        TYPE            bool
        DEFAULT         FALSE
        }

Parameter DOSFS_PRTMSG_LEVEL 
        {
        NAME        print message level
        SYNOPSIS    print message according to this value. 
        TYPE        UINT32
        DEFAULT     1
        }

Parameter DOSFS_DEFAULT_VOL_BIO_BUFFER_SIZE 
        {
        NAME        Default size in blocks of a bio data buffer
        SYNOPSIS    Default value is 32 (blocks)
        TYPE        UINT32
        DEFAULT     32
        }

Folder FOLDER_DOSFS2
        {
        NAME            dosFs File System Components (dosFs2)
        SYNOPSIS        DOS File System, and related components
        _CHILDREN       FOLDER_IO_SYSTEM
        CHILDREN        INCLUDE_DOSFS_MAIN    \
                        INCLUDE_DOSFS_FAT     \
                        SELECT_DOSFS_DIR      \
                        INCLUDE_DOSFS_FMT     \
                        INCLUDE_DOSFS_CHKDSK  \
                        INCLUDE_XBD_TRANS     \
                        INCLUDE_DOSFS_CACHE   \
                        INCLUDE_DOSFS_SHOW    \
                        INCLUDE_TAR           \
                        INCLUDE_DOSFS_PRTMSG_LEVEL \
                        INCLUDE_DOSFS_VOL_BIO_BUFFER_SIZE

        DEFAULTS        INCLUDE_DOSFS_MAIN     \
                        INCLUDE_DOSFS_FAT      \
                        INCLUDE_DOSFS_SHOW     \
                        INCLUDE_DOSFS_DIR_VFAT \
                        INCLUDE_DOSFS_PRTMSG_LEVEL \
                        INCLUDE_DOSFS_VOL_BIO_BUFFER_SIZE
        }

Selection SELECT_DOSFS_DIR
        {
        NAME            DOS File System Directory Handlers
        COUNT           1-
        CHILDREN        INCLUDE_DOSFS_DIR_VFAT INCLUDE_DOSFS_DIR_FIXED
        DEFAULTS        INCLUDE_DOSFS_DIR_VFAT
        }

Component INCLUDE_DOSFS_MAIN
        {
        NAME            dosfs File System Main Module (dosFs2)
        SYNOPSIS        MS-DOS-Compatible File System: dosFs main module
        MODULES         dosFsLib.o
        INIT_RTN        usrDosfsInit (DOSFS_DEFAULT_MAX_FILES, DOSFS_DEFAULT_CREATE_OPTIONS);
        CONFIGLETTES    usrDosfs.c
        CFG_PARAMS      DOSFS_DEFAULT_MAX_FILES       \
                        DOSFS_DEFAULT_CREATE_OPTIONS
        REQUIRES        INCLUDE_DOSFS_FAT      \
                        SELECT_DOSFS_DIR       \
                        INCLUDE_XBD            \
                        INCLUDE_FS_MONITOR     \
                        INCLUDE_IO_FILE_SYSTEM  \
                        INCLUDE_FS_EVENT_UTIL  \
                        INCLUDE_DOSFS_VOL_BIO_BUFFER_SIZE
        }

Component INCLUDE_DOSFS_FAT
        {
        NAME            DOS File System FAT12/16/32 Handler
        SYNOPSIS        dosFs FAT12/16/32 FAT handler
        MODULES         dosFsFat.o
        REQUIRES        INCLUDE_DOSFS_MAIN
        }

Component INCLUDE_DOSFS_DIR_VFAT
        {
        NAME            DOS File System VFAT Directory Handler 
        SYNOPSIS        VFAT Variable-length file names support, Win95/NT compatible
        MODULES         dosVDirLib.o
        REQUIRES        INCLUDE_DOSFS_MAIN
        CFG_PARAMS      DOSFS_COMPAT_NT
        }

Component INCLUDE_DOSFS_DIR_FIXED
        {
        NAME            DOS File System Old Directory Format Handler 
        SYNOPSIS        Strict 8.3 and VxLongs proprietary long names
        MODULES         dosDirOldLib.o
        REQUIRES        INCLUDE_DOSFS_MAIN
        }

Component INCLUDE_DOSFS_FMT
        {
        NAME            DOS File System Volume Formatter Module
        SYNOPSIS        High level formatting of DOS volumes
        MODULES         dosFsFmtLib.o
        REQUIRES        INCLUDE_RAWFS        \
                        INCLUDE_DOSFS_MAIN
        }

Component INCLUDE_DOSFS_CHKDSK
        {
        NAME            DOS File System Consistency Checker
        SYNOPSIS        Consistency checking set on per-device basis
        MODULES         dosChkLib.o
        REQUIRES        INCLUDE_DOSFS_MAIN
        }

#ifndef  _WRS_CONFIG_LP64
Component INCLUDE_CBIO
        {
        NAME            CBIO (Cached Block I/O) Support, cbioLib
        MODULES         cbioLib.o
        INIT_RTN        cbioLibInit();
        HDR_FILES       cbioLib.h
        }
#endif

Component INCLUDE_XBD_TRANS
        {
        NAME            Transactional Block Layer
        SYNOPSIS        Transactional Block layer is set on per-device basis
        MODULES         xbdTrans.o usrTransLib.o
        INIT_RTN        xbdTransInit();
        HDR_FILES       transCbio.h usrTransLib.h
        LINK_SYMS       transDevCreate usrFormatTrans
        REQUIRES        INCLUDE_XBD \
                        INCLUDE_FS_MONITOR \
                        INCLUDE_FS_EVENT_UTIL
        }

Component INCLUDE_DOSFS_CACHE
        {
        NAME            Dos FS Cache Handler
        SYNOPSIS        Dos FS Cache size is set on per-device basis
        MODULES         dosFsCacheLib.o
        HDR_FILES       dosFsLib.h
        INIT_RTN        dosFsCacheLibInit (DOSFS_DEFAULT_DATA_DIR_CACHE_SIZE, \
                                           DOSFS_DEFAULT_FAT_CACHE_SIZE, \
                                           DOSFS_CACHE_BACKGROUND_FLUSH_TASK_ENABLE);
        LINK_SYMS       dosFsCacheCreate
        CFG_PARAMS      DOSFS_DEFAULT_DATA_DIR_CACHE_SIZE \
                        DOSFS_BLOCKS_PER_DATA_DIR_CACHE_GROUP \
                        DOSFS_DEFAULT_FAT_CACHE_SIZE \
                        DOSFS_BLOCKS_PER_FAT_CACHE_GROUP \
                        DOSFS_CACHE_BACKGROUND_FLUSH_TASK_ENABLE
        REQUIRES        INCLUDE_DOSFS_MAIN
        }

Component INCLUDE_DOSFS_SHOW
        {
        NAME            Dos FS Show Routines
        SYNOPSIS        Show routines for Dos FS
        MODULES         dosFsShow.o
        HDR_FILES       dosFsLib.h
        INIT_RTN        dosFsShowInit();
        REQUIRES        INCLUDE_DOSFS_MAIN
        INCLUDE_WHEN    INCLUDE_DOSFS_MAIN
        }

#ifndef _WRS_CONFIG_LP64
Component INCLUDE_DISK_PART
        {
        NAME            Disk Partition Handler
        SYNOPSIS        Supports disk partition tables
        MODULES         dpartCbio.o usrFdiskPartLib.o
        HDR_FILES       dpartCbio.h usrFdiskPartLib.h
        LINK_SYMS       dpartDevCreate usrFdiskPartRead
        }
#endif

Component INCLUDE_TAR
        {
        NAME            File System Backup and Archival
        SYNOPSIS        UNIX-compatible TAR facility
        MODULES         tarLib.o
        LINK_SYMS       tarHelp
        HDR_FILES       tarLib.h
        }

Component INCLUDE_DOSFS_PRTMSG_LEVEL {
        NAME            print message level
        SYNOPSIS        print message accoding to DOSFS_PRTMSG_LEVEL
        CFG_PARAMS      DOSFS_PRTMSG_LEVEL
}

Component INCLUDE_DOSFS_VOL_BIO_BUFFER_SIZE {
        NAME            Dos FS BIO buffer size 
        SYNOPSIS        volume bio buffer size can be set for performance
        CFG_PARAMS      DOSFS_DEFAULT_VOL_BIO_BUFFER_SIZE
        REQUIRES        INCLUDE_DOSFS_MAIN
}

/* Backward compatible configuration */

Component INCLUDE_DOSFS {
        NAME            DOS filesystem backward-compatibility
        SYNOPSIS        Old dosFs API module, depreciated

        REQUIRES        INCLUDE_DOSFS_MAIN        \
                        INCLUDE_DOSFS_DIR_VFAT    \
                        INCLUDE_DOSFS_DIR_FIXED   \
                        INCLUDE_DOSFS_FAT         \
                        INCLUDE_DOSFS_CHKDSK      \
                        INCLUDE_DOSFS_FMT
#ifndef _WRS_CONFIG_LP64
        INIT_BEFORE     INCLUDE_SCSI \
                        INCLUDE_FD \
                        INCLUDE_IDE \
                        INCLUDE_ATA \
                        INCLUDE_PCMCIA \
                        INCLUDE_TFFS 
#else  /* !_WRS_CONFIG_LP64 */
        INIT_BEFORE     INCLUDE_SCSI \
                        INCLUDE_FD \
                        INCLUDE_IDE \
                        INCLUDE_ATA \
                        INCLUDE_PCMCIA 
#endif    /* _WRS_CONFIG_LP64 */
       }

/*
 * This component used to create a CBIO RAM disk and install DosFs2 onto it.
 * However, with the new framework, the RAM disk is created, but no file
 * system is mounted on it.  The file system monitor takes care of that.
 */

Component INCLUDE_RAM_DISK
        {
        NAME            RAM Disk
        SYNOPSIS        Create an XBD based RAM disk
        CFG_PARAMS      RAM_DISK_BLK_SIZE      \
                        RAM_DISK_SIZE          \
                        RAM_DISK_DEV_NAME
        INIT_RTN        xbdRamDiskDevCreate (RAM_DISK_BLK_SIZE,        \
                                             RAM_DISK_SIZE,            \
                                             FALSE,                    \
                                             RAM_DISK_DEV_NAME);
        _CHILDREN       FOLDER_PERIPHERALS
        HDR_FILES       xbdRamDisk.h
        REQUIRES        INCLUDE_XBD_RAMDRV
        }

Parameter RAM_DISK_BLK_SIZE
        {
        NAME            RAM Disk block size
        TYPE            unsigned
        DEFAULT         512
        }

Parameter RAM_DISK_SIZE
        {
        NAME            Size of RAM (in bytes) allocated for RAM Disk
        TYPE            unsigned
        DEFAULT         0x10000
        }

Parameter RAM_DISK_DEV_NAME
        {
        NAME            RAM Disk logical device name
        TYPE            string
        DEFAULT         "/ram0"
        }
