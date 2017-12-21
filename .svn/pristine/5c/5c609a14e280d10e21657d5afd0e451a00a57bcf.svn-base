/* usrNfsd.c  Configlette file for NFS server */

/*
 * Copyright (c) 2003-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01h,07jan07,mwv  remove VIRTUAL_STACK support
01g,07sep06,mwv  update nfs include paths
01f,16mar06,spw  Fix warnings in virtual stack build
01e,10jan06,pcm  fixed SPR 116500 (there was an extra parenthesis)
01d,24oct05,md   changed PATH_MAX count to include null terminator
01c,08jul04,vvv  fixed warning
01b,21jun04,vvv  added virtual stack support
01a,18oct03,msc  written
*/

/*
DESCRIPTION
This file is used to include support for the NFS server. The contents of this
file are included in the project configuration file when the NFS server
component(s) are included. This file initializes the variable nfsServMaxPath
which uses the configuration parameter NFS_MAXPATH to set the maximum file
path length handled by the server. It also initializes the server by calling
the init routine.

The definable NFS server components are:
INCLUDE_NFS2_SERVER
INCLUDE_NFS3_SERVER
INCLUDE_NFS_SERVER_ALL

NOMANUAL
*/

#include <nfs/nfsd.h>
#include <nfs/mountd.h>

int nfsServMaxPath = NFS_MAXPATH + 1;
int nfsMaxFileName = NFS_MAXFILENAME;

int *pnfsdRebootTime = NULL;
FUNCPTR pNlmdInit = NULL;


#if defined (INCLUDE_NFS_SERVER_ALL) || defined (INCLUDE_NFS2_SERVER)
extern void nfs2dRequestEnqueue (); 
extern NFSDCALLBACK nfsv2CallBack;
extern MOUNTCALLBACK mountv1CallBack;    /* call back function for MOUNT v1 */
extern void mount1dRequestProcess ();
extern NFS2_SERVER_STATUS nfs2ServerStatus;
extern NFS2_SERVER_STATUS *pnfs2ServerStat;
#endif

#if defined (INCLUDE_NFS_SERVER_ALL) || defined (INCLUDE_NFS3_SERVER)
extern void nfs3dRequestEnqueue (); 
extern NFSDCALLBACK nfsv3CallBack;
extern MOUNTCALLBACK mountv3CallBack;    /* call back function for MOUNT v3 */
extern void mount3dRequestProcess ();
extern void mountv1dRequestProcess ();
extern STATUS nlmdInit (int, int, int);


extern int  nfsdRebootTime;
extern NFS3_SERVER_STATUS *pnfs3ServerStat;
extern NFS3_SERVER_STATUS nfs3ServerStatus;
#endif

int  finalNfsVersion = -1;

/******************************************************************************
*
* usrNfsdInit - initialize the version flag for NFS server init.
*
* This routine updates the finalNfsVersion flag which will be passed 
* to the nfsdInit() routine. 
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/

STATUS usrNfsdInit (int version)
    {
    if (finalNfsVersion == -1)
        finalNfsVersion = 0;

    finalNfsVersion |= version;
    
    if ((finalNfsVersion & 0x03) == 0x03) 
        finalNfsVersion = 0;

    return OK;
    }

/******************************************************************************
*
* usrNfsdInitInternal - initialize NFS server
*
* This routine initializes the NFS server. 
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/

STATUS usrNfsdInitInternal ()
   {

   if (finalNfsVersion == -1)
       return OK;

   if (nfsServMaxPath < 1)
       {
       printf ("Error initializing NFS server, invalid NFS_MAXPATH\n");
       return (ERROR);
       }

    if (nfsServMaxPath > PATH_MAX)
        nfsServMaxPath = PATH_MAX;

   if ((nfsMaxFileName < 1) || (nfsMaxFileName > NAME_MAX))
       {
       printf ("Error initializing NFS server, invalid NFS_MAXFILENAME\n");
       return (ERROR);
       }

#if defined (INCLUDE_NFS_SERVER_ALL)
    nfsv2CallBack = (NFSDCALLBACK) nfs2dRequestEnqueue;
    mountv3CallBack = mount3dRequestProcess;
    nfsv3CallBack = (NFSDCALLBACK) nfs3dRequestEnqueue;
    mountv1CallBack = mount1dRequestProcess;
    pnfs2ServerStat = &nfs2ServerStatus;
    pnfs3ServerStat = &nfs3ServerStatus;
    pnfsdRebootTime = &nfsdRebootTime;
    pNlmdInit = nlmdInit;

    /* Set up call statistics */
    memset (&nfs3ServerStatus, 0, sizeof (nfs3ServerStatus));
    /* Set up call statistics */
    memset (&nfs2ServerStatus, 0, sizeof (nfs2ServerStatus));
#else
#if defined (INCLUDE_NFS3_SERVER)
       mountv1CallBack = mountdv1RequestProcess;
       mountv3CallBack = mount3dRequestProcess;
       nfsv3CallBack = (NFSDCALLBACK) nfs3dRequestEnqueue;
       pnfs3ServerStat = &nfs3ServerStatus;
       pnfsdRebootTime = &nfsdRebootTime;
       pNlmdInit = nlmdInit;

       /* Set up call statistics */
       memset (&nfs3ServerStatus, 0, sizeof (nfs3ServerStatus));
#endif
#if defined (INCLUDE_NFS2_SERVER)
    nfsv2CallBack = (NFSDCALLBACK) nfs2dRequestEnqueue;
    mountv1CallBack = mount1dRequestProcess;
    /* Set up call statistics */
    memset (&nfs2ServerStatus, 0, sizeof (nfs2ServerStatus));
    pnfs2ServerStat = &nfs2ServerStatus;
#endif   /* else NFS2_SERVER */
#endif   /* else NFS_SERVER_ALL */

   if (nfsdInit (0, 0, 0, 0, 0, finalNfsVersion) == ERROR)
       {
       printf ("Error initializing NFS server\n");
       return (ERROR);
       }

   return (OK);
   }
