/* usrMfs.c - Configlette for MFS Server and Client */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
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

#ifdef  INCLUDE_MFS_CLIENT
#include <mfs/mfs_clnt.h>

STATUS usrMfsClntInit
    (
    void
    )
    {
    return (mfs_clntinit(MFS_BUS_NAME, MFS_REQUEST_PORT,
                         MFS_CLIENT_SOCKETS));
    }
#endif

#ifdef  INCLUDE_MFS_SERVER
#include <mfs/mfs_serv.h>

STATUS usrMfsServInit
    (
    void
    )
    {
    if (mfs_servinit(MFS_BUS_NAME, MFS_REQUEST_PORT,
                     MFS_SERVER_RECV_TASK_PRIO,
                     MFS_SERVER_PROC_TASK_PRIO, MFS_SERVER_PROC_TASKS) < 0)
        return (ERROR);

#ifdef  INCLUDE_MFS_SERVER_FD_CACHE
    {
    extern int mfs_fdcinit(int numslots, int chkfreq, int closetskprio);

    if (mfs_fdcinit(MFS_SERVER_FD_CACHE_SIZE,
                    MFS_SERVER_FD_CACHE_AUDIT_PERIOD,
                    MFS_SERVER_FD_CACHE_AUDIT_TASK_PRIO) < 0)
        return (ERROR);
    }
#endif

    return (OK);
    }
#endif
