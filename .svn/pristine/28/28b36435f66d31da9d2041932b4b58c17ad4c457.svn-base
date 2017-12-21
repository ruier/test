/* 20comp_mfs.cdf - configuration for MFS client and server */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,26nov10,hps  make priorities of server tasks user configurable
                 and other minor changes
01a,03sep10,hps  written
*/

Folder FOLDER_MFS
    {
    NAME        MIPC File System
    SYNOPSIS    MIPC remote file system
    _CHILDREN   FOLDER_MIPC
    CHILDREN    INCLUDE_MFS
                INCLUDE_MFS_CLIENT
                INCLUDE_MFS_SERVER
                INCLUDE_MFS_SERVER_FD_CACHE
    }

Component INCLUDE_MFS 
    {
    NAME            MFS Bus and Port
    SYNOPSIS        MFS bus and port configuration
    _CHILDREN       FOLDER_MFS
    REQUIRES        INCLUDE_MIPC_SM
    MODULES         mfs_common.o
    CFG_PARAMS      MFS_BUS_NAME    \
                    MFS_REQUEST_PORT
    }

Parameter MFS_BUS_NAME
    {
    NAME        MFS Bus Name
    SYNOPSIS    Specify the MIPC bus for MFS servers and clients
    TYPE        char *
    DEFAULT     "main"
    }

Parameter MFS_REQUEST_PORT
    {
    NAME        MFS Request Port
    SYNOPSIS    Specify the MIPC port for server to receive request messages
    TYPE        int
    DEFAULT     25
    }

Component INCLUDE_MFS_CLIENT
    {
    NAME            MFS Client
    SYNOPSIS        MFS client component
    _CHILDREN       FOLDER_MFS
    REQUIRES        INCLUDE_MFS
    MODULES         mfs_clnt.o      \
                    mfs_clntlib.o   \
                    mfsClntLibVx.o  \
                    mfsDrv.o
    LINK_SYMS       mfs_mount
    CFG_PARAMS      MFS_CLIENT_SOCKETS
    CONFIGLETTES    usrMfs.c
    INIT_RTN        usrMfsClntInit();
    _INIT_ORDER     usrIosCoreInit
    INIT_AFTER      INCLUDE_TTY_DEV
    }

Parameter MFS_CLIENT_SOCKETS
    {
    NAME        Number of Sockets MFS Client Allocates
    SYNOPSIS    The number must be within the range [64, 1], otherwise the default value is used
    TYPE        int
    DEFAULT     3
    }

Component INCLUDE_MFS_SERVER
    {
    NAME            MFS Server
    SYNOPSIS        MFS server component
    _CHILDREN       FOLDER_MFS
    REQUIRES        INCLUDE_MFS
    MODULES         mfs_serv.o                  \
                    mfs_servlib.o               \
                    mfsServLibVx.o
    LINK_SYMS       mfs_export
    CFG_PARAMS      MFS_SERVER_RECV_TASK_PRIO   \
                    MFS_SERVER_PROC_TASK_PRIO   \
                    MFS_SERVER_PROC_TASKS
    CONFIGLETTES    usrMfs.c
    INIT_RTN        usrMfsServInit();
    _INIT_ORDER     usrIosCoreInit
    INIT_AFTER      INCLUDE_TTY_DEV
    }

Parameter MFS_SERVER_RECV_TASK_PRIO
    {
    NAME        Server Receiving Task Priority
    SYNOPSIS    The priority must be within the range [100, 10], otherwise the default value is used
    TYPE        int
    DEFAULT     55
    }

Parameter MFS_SERVER_PROC_TASK_PRIO
    {
    NAME        Server Processing Task Priority
    SYNOPSIS    The priority must be within the range [100, 10], otherwise the default value is used
    TYPE        int
    DEFAULT     60
    }

Parameter MFS_SERVER_PROC_TASKS
    {
    NAME        Number of Server Processing Tasks
    SYNOPSIS    The number must be within the range [8, 1], otherwise the default value is used
    TYPE        int
    DEFAULT     2
    }

Component INCLUDE_MFS_SERVER_FD_CACHE
    {
    NAME            MFS Server File Descriptor Cache
    SYNOPSIS        File descriptor cache on server side to improve read/write speed
    _CHILDREN       FOLDER_MFS
    REQUIRES        INCLUDE_MFS_SERVER
    MODULES         mfs_fdcache.o
    CFG_PARAMS      MFS_SERVER_FD_CACHE_SIZE            \
                    MFS_SERVER_FD_CACHE_AUDIT_PERIOD    \
                    MFS_SERVER_FD_CACHE_AUDIT_TASK_PRIO
    }

Parameter MFS_SERVER_FD_CACHE_SIZE
    {
    NAME        Number of File Descriptors The MFS Server Can Cache
    SYNOPSIS    The number must be within the range [64, 1], otherwise the default value is used
    TYPE        int
    DEFAULT     16
    }

Parameter MFS_SERVER_FD_CACHE_AUDIT_PERIOD
    {
    NAME        MFS Server File Descriptor Cache Audit Period
    SYNOPSIS    The period in seconds that the MFS server file descriptor cache is audited, must be within the range [300, 10], otherwise the default value is used
    TYPE        int
    DEFAULT     60
    }

Parameter MFS_SERVER_FD_CACHE_AUDIT_TASK_PRIO
    {
    NAME        MFS Server File Descriptor Cache Audit Task Priority
    SYNOPSIS    The priority must be within the range [255, 100], otherwise the default value is used
    TYPE        int
    DEFAULT     200
    }
