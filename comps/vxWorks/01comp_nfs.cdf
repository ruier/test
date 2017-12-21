/* 01comp_nfs.cdf - NFS Component configuration file */

/*
 * Copyright (c) 2005 - 2007, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,22feb13,yjl  Fix WIND00403400, To enhance nfs debugging, add some debug
                 information
01i,27sep07,dcc  added INCLUDE_IO_FILE_SYSTEM to REQUIRES list
01h,30oct06,mwv  remove INCLUDE_PACKET_POOL_3 due to changes in packet pool
                 organization.
01g,19sep06,mwv  fix a type-o in INCLUDE_CORE_NFS_SERVER (WIND00065978)
01f,07sep06,mwv  update HDR_FILES paths
01e,31jul06,mwv  update CONFIGLETTES
01d,24jul06,mwv  modify to support updated stack 
01c,13mar06,pcm  fixed parameters in call to usrNfs3CacheInit() (SPR 118964)
01b,06feb06,pcm  added nfs3StableWriteSet() to INCLUDE_NFS3_CLIENT
01a,16dec05,pcm  merged all NFS cdf files into one.
*/

Parameter NFS2_CLIENT_CACHE_DEFAULT_NUM_LINES
    {
    NAME        Default number of cache lines
    TYPE        UINT32
    DEFAULT     16
    }

Parameter NFS2_CLIENT_CACHE_DEFAULT_LINE_SIZE
    {
    NAME        Default number of bytes in cache line
    TYPE        UINT32
    DEFAULT     16384
    }

Parameter NFS2_CLIENT_CACHE_DEFAULT_OPTIONS
    {
    NAME        Default cache options
    TYPE        UINT32
    DEFAULT     0
    }

Component INCLUDE_NFS2_CLIENT
    {
    NAME           NFS v2 client
    SYNOPSIS       Network file system(v2) Library
    HDR_FILES      nfs/nfs2Lib.h   \
                   nfs/nfs2Drv.h   \
                   nfs/nfsCommon.h
    CONFIGLETTES   usrNfs.c 
    MODULES        nfs2Lib.o \
                   nfs2Drv.o
    REQUIRES       INCLUDE_CORE_NFS_CLIENT
    INIT_RTN       usrNfsInit (2);                                          \
                   usrNfs2CacheInit (NFS2_CLIENT_CACHE_DEFAULT_NUM_LINES,   \
                                     NFS2_CLIENT_CACHE_DEFAULT_LINE_SIZE,   \
                                     NFS2_CLIENT_CACHE_DEFAULT_OPTIONS);
    CFG_PARAMS     NFS2_CLIENT_CACHE_DEFAULT_NUM_LINES  \
                   NFS2_CLIENT_CACHE_DEFAULT_LINE_SIZE  \
                   NFS2_CLIENT_CACHE_DEFAULT_OPTIONS
    }

Parameter NFS3_CLIENT_CACHE_DEFAULT_NUM_LINES
    {
    NAME        Default number of cache lines
    TYPE        UINT32
    DEFAULT     16
    }

Parameter NFS3_CLIENT_CACHE_DEFAULT_LINE_SIZE
    {
    NAME        Default number of bytes in cache line
    TYPE        UINT32
    DEFAULT     16384
    }

Parameter NFS3_CLIENT_CACHE_DEFAULT_OPTIONS
    {
    NAME        Default cache options
    TYPE        UINT32
    DEFAULT     0
    }

Component INCLUDE_NFS3_CLIENT
    {
    NAME           NFS v3 client
    SYNOPSIS       Network file system (v3) Library
    HDR_FILES      nfs/nfs3Lib.h   \
                   nfs/nfs3Drv.h   \
                   nfs/nfsCommon.h
    CONFIGLETTES   usrNfs.c
    MODULES        nfs3Lib.o \
                   nfs3Drv.o
    REQUIRES       INCLUDE_CORE_NFS_CLIENT
    INIT_RTN       usrNfsInit (3);                                          \
                   usrNfs3CacheInit (NFS3_CLIENT_CACHE_DEFAULT_NUM_LINES,   \
                                     NFS3_CLIENT_CACHE_DEFAULT_LINE_SIZE,   \
                                     NFS3_CLIENT_CACHE_DEFAULT_OPTIONS);    \
                   nfs3StableWriteSet (0);
    CFG_PARAMS     NFS3_CLIENT_CACHE_DEFAULT_NUM_LINES  \
                   NFS3_CLIENT_CACHE_DEFAULT_LINE_SIZE  \
                   NFS3_CLIENT_CACHE_DEFAULT_OPTIONS
    }

Component INCLUDE_NFS_CLIENT_ALL
    {
    NAME         NFS client All
    SYNOPSIS     Network file system Client Library
    REQUIRES     INCLUDE_NFS2_CLIENT INCLUDE_NFS3_CLIENT
    }

Component INCLUDE_CORE_NFS_CLIENT 
    {
    NAME         Core NFS client
    SYNOPSIS     Include Core NFS client
    HDR_FILES    nfs/nfsCommon.h
    MODULES      nfsCommon.o
    REQUIRES     INCLUDE_IO_FILE_SYSTEM INCLUDE_RPC
    CFG_PARAMS   NFS_USER_ID NFS_GROUP_ID NFS_MAXPATH
    }

Parameter NFS_USER_ID
    {
    NAME         user identifier for NFS access
    TYPE         int
    DEFAULT      2001
    }

Parameter NFS_GROUP_ID
    {
    NAME         group identifier for NFS access
    TYPE         int
    DEFAULT      100
    }

Parameter NFS_MAXPATH
    {
    NAME         Maximum file path length
    SYNOPSIS     Indicates maximum length of a file path
    DEFAULT      255
    }

Component INCLUDE_NFS_MOUNT_ALL
    {
    NAME            NFS mount all
    SYNOPSIS        Mount all remote file systems at boot time
    CONFIGLETTES    usrNfs.c
    INIT_RTN        usrNfsMountAll();
    REQUIRES        INCLUDE_NFS_CLIENT_ALL
    CFG_PARAMS      GROUP_EXPORTS
    }

Parameter GROUP_EXPORTS
    {
    NAME        Remove check for GroupName during MountAll
    TYPE        BOOL
    DEFAULT     FALSE
    }

Component INCLUDE_NFS2_SERVER
    {
    NAME           NFS server V2
    SYNOPSIS       Network file system (version 2) server
    HDR_FILES      nfs/nfs2dLib.h \
                   nfs/mountd.h
    CONFIGLETTES   usrNfsd.c
    MODULES        nfs2dLib.o \
                   mount1Lib.o
    INIT_RTN       usrNfsdInit (1);
    REQUIRES       INCLUDE_CORE_NFS_SERVER INCLUDE_NFS_SERVER_INSTALL
    }

Component INCLUDE_NFS3_SERVER
    {
    NAME           NFS server V3
    SYNOPSIS       Network file system (version 3) server
    HDR_FILES      nfs/nfs3dLib.h
    CONFIGLETTES   usrNfsd.c
    MODULES        nfs3dLib.o
    INIT_RTN       usrNfsdInit (2);
    REQUIRES       INCLUDE_CORE_NFS_SERVER INCLUDE_NFS_SERVER_INSTALL
    }

Component INCLUDE_NFS_SERVER_ALL
    {
    NAME           NFS server All
    SYNOPSIS       Network file system server(s)
    HDR_FILES      nfs/nfsd.h
    REQUIRES       INCLUDE_NFS3_SERVER INCLUDE_NFS2_SERVER \
                   INCLUDE_NFS_SERVER_INSTALL
    }

Component INCLUDE_CORE_NFS_SERVER
   {
   NAME        NFS server
   SYNOPSIS    Core component of NFS server(s)
   HDR_FILES   nfs/nfsd.h
   MODULES     nfsd.o
   REQUIRES    INCLUDE_RPC
   CFG_PARAMS  NFS_USER_ID NFS_GROUP_ID NFS_MAXPATH NFS_MAXFILENAME
   }

Parameter NFS_MAXPATH
    {
    NAME        Maximum file path length
    SYNOPSIS    Indicates maximum length of a file path
    DEFAULT     255
    }

Parameter NFS_USER_ID
    {
    NAME      user identifier for NFS access
    TYPE      int
    DEFAULT   2001
    }

Parameter NFS_MAXFILENAME
    {
    NAME        Maximum file name length
    SYNOPSIS    Indicates maximum length of a filename. If a filename \
                is longer than this value then it is truncated before \
                returning it to the client. This parameter should be  \
                set carefully since it affects the amount of memory   \
                allocated by the server. Valid values range from 1 to \
                99.
    DEFAULT     40
    }

Parameter NFS_GROUP_ID
    {
    NAME      group identifier for NFS access
    TYPE      int
    DEFAULT   100
    }

Component INCLUDE_NFS_SERVER_INSTALL
   {
   NAME           Install NFS server
   SYNOPSIS       Install Network file system server(s)
   CONFIGLETTES   usrNfsd.c
   INIT_RTN       usrNfsdInitInternal();
   REQUIRES       INCLUDE_CORE_NFS_SERVER
   }

Parameter NFS_DEBUG_LEVEL
    {
    NAME          nfs debug level
    SYNOPSIS      set nfs debug level. When this value greater than \
                  corresponding ERROR or NORMAL infomation level, \
                  debug infomation can be output. NFS_NONE_DEBUG 0, \ 
                  NFS_ERROR_DEBUG 1, NFS_WARNING_DEBUG 2, \ 
                  NFS_NORMAL_DEBUG 3, NFS_VERBOSE_DEBUG 4.
    TYPE          int
    DEFAULT       0
    }

Component INCLUDE_NFS_DEBUG
    {
    NAME          nfs debug
    SYNOPSIS      implement nfs debug info
    HDR_FILES     nfs/nfsDebug.h
    MODULES       nfsDebug.o
    INIT_RTN      nfsDebugInit(NFS_DEBUG_LEVEL);
    CFG_PARAMS    NFS_DEBUG_LEVEL
    }

Folder FOLDER_NFS
     {
     NAME      NFS Components
     SYNOPSIS  Network File System
     _CHILDREN FOLDER_IO_SYSTEM
     CHILDREN  INCLUDE_CORE_NFS_CLIENT \
               INCLUDE_CORE_NFS_SERVER \
               INCLUDE_NFS_MOUNT_ALL \
               INCLUDE_NFS_CLIENT_ALL \
               INCLUDE_NFS3_CLIENT    \
               INCLUDE_NFS2_CLIENT   \
               INCLUDE_NFS_SERVER_ALL \
               INCLUDE_NFS2_SERVER \
               INCLUDE_NFS3_SERVER \
               INCLUDE_NFS_DEBUG
     }

