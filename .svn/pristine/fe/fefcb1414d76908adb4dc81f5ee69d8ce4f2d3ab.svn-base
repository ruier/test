/* usrNfs.c - Configlette for NFS Client */

/*
 * Copyright (c) 2003 - 2004, 2006 - 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01e,19jul07,jlk  Fixed defect WIND00099010
01d,07sep06,mwv  update nfs include paths
01c,31jul06,mwv  add usrNfs3CacheInit from usrNfs3.c, add 
                 usrNfs2CacheInit from usrNfs3.c, and 
		 usrNetNfsMountAll (renamed usrNfsMountAll) from
		 usrNetNfsMount.c 
01b,20oct04,vvv  fixed warnings (SPR #102122)
01a,18oct03,snd  written
*/

/*
DESCRIPTION

This file is used to include support for the NFS Client. The contents of this
file are included in the project configuration file when the NFS Client
component(s) are included. 
It also initializes the Client by calling the appropriate driver routine.

The definable NFS server components are:
INCLUDE_NFS2_CLIENT
INCLUDE_NFS3_CLIENT
INCLUDE_NFS_CLIENT_ALL

NOMANUAL
*/

#include <vxWorks.h>
#include <nfs/nfs2Lib.h>
#include <nfs/nfs2Drv.h>
#include <nfs/nfs3Lib.h>
#include <nfs/nfs3Drv.h>

#ifdef __GNUC__
# ifndef alloca
#  define alloca __builtin_alloca
# endif
#endif

#if defined (INCLUDE_NFS_MOUNT_ALL)
IMPORT BOOL group_export;
#endif

/* externs */
extern char *pAddrString;
#if defined (INCLUDE_NFS2_CLIENT)
extern UINT32 nfs2MaxCacheLines;
extern UINT32 nfs2BytesPerCacheLine;
extern UINT32 nfs2CacheOptions;
#endif
#if defined (INCLUDE_NFS3_CLIENT)
extern UINT32 nfs3MaxCacheLines;
extern UINT32 nfs3BytesPerCacheLine;
extern UINT32 nfs3CacheOptions;
#endif


const int nfsMaxPath = NFS_MAXPATH + 1;

/******************************************************************************
*
* usrNfsInit - initialize NFS
*
* This routine initializes NFS and configures it using the following parameters
* defined by the user via the project facility.
*
*- NFS_USER_ID
*- NFS_GROUP_ID
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/

STATUS usrNfsInit (int version)
    {
    char *devName;

    if (nfsMaxPath < 1)
        {
        printf ("Error initializing NFS, invalid NFS_MAXPATH\n");
        return (ERROR);
        }

    if ((devName = (char *) alloca (nfsMaxPath)) == NULL)
       {
       printf ("Error initializing NFS,    out of memory\n");
       return (ERROR);
       }

    /*
     * The following values are    the default values used    in NFS.
     * They can    be reset here if necessary.
     *
     * nfsMaxMsgLen   = 8192   message   size (decrease only)
     * nfsTimeoutSec  = 5      timeout   seconds
     * nfsTimeoutUSec = 0      timeout   microseconds
     */

#if (CPU==SIMHPPA)
    nfsMaxMsgLen   = 512;
#endif
    if (nfsComponentInit () != OK)
	{
        printf ("Error initializing NFS component, errno = %#x\n", errno);
        return (ERROR);
	}

    nfsAuthUnixSet (sysBootParams.hostName, NFS_USER_ID, NFS_GROUP_ID, 0, 
                    (int *) 0);

    if (version == NFS_V2)
        {
        if (nfs2Drv () == ERROR)     /*    initialize nfs driver */
            printf ("Error initializing    NFS, errno = %#x\n", errno);
        else
            {
            /* if the boot pathname starts with a device name, 
             * e.g. an nfs mount, then set the current directory to that 
             * device
             */

#if defined (_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6) /* VxWorks 6.x */
            (void) iosDevFind (sysBootParams.bootFile,
	                       (const char **) &pAddrString);
#else
            (void) iosDevFind (sysBootParams.bootFile, &pAddrString);
#endif

            if (pAddrString    != sysBootParams.bootFile)
                {
                devName[0] = EOS;
                strncat (devName, sysBootParams.bootFile, 
                          pAddrString - sysBootParams.bootFile);
                ioDefPathSet (devName);
                }
            }
        }
    else if(version ==    NFS_V3)
        {
        if (nfs3Drv () == ERROR)     /*    initialize nfs driver */
            printf ("Error initializing    NFS, errno = %#x\n", errno);
        else
            {
            /* if the boot pathname starts with a device name, 
             * e.g.    an nfs mount, then set the current directory to 
             * that device
             */

#if defined (_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6) /* VxWorks 6.x */
            (void) iosDevFind (sysBootParams.bootFile,
	                       (const char **) &pAddrString);
#else
            (void) iosDevFind (sysBootParams.bootFile, &pAddrString);
#endif
            if (pAddrString    != sysBootParams.bootFile)
                {
                devName[0] = EOS;
                strncat (devName, sysBootParams.bootFile, 
                          pAddrString - sysBootParams.bootFile);
                ioDefPathSet (devName);
                }
            }
        }

    return OK;
    }


#if defined (INCLUDE_NFS_MOUNT_ALL)
/*******************************************************************************
*
* usrNfsMountAll - call to nfsMountAll ()
*
* This routine makes a call to nfsMountAll() to mount all the File systems
* (directories) exported by the host. This is executed only once at BOOT time
*
* RETURNS: N/A
*
* NOMANUAL
*/

void usrNfsMountAll ()
    {
   group_export	= GROUP_EXPORTS;
   printf ("Mounting NFS file systems from host	%s", sysBootParams.hostName);\
   if (sysBootParams.targetName[0] != EOS)\
       printf (" for target %s:\n", sysBootParams.targetName);\
    else \
       printf (":\n"); \
    if (nfsMountAll (sysBootParams.hostName, sysBootParams.targetName, FALSE) != OK) \
        printf ("... nfs mount all failed\n"); \
    else \
        printf ("...done\n"); \
    }
#endif


#if defined (INCLUDE_NFS2_CLIENT)
/*******************************************************************************
*
* usrNfs2CacheInit - initialize the NFS v2 cache parameters
*
* This routine initializes the NFS v2 cache parameters.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void usrNfs2CacheInit
    (
    UINT32   defaultNumLines,
    UINT32   defaultLineSize,
    UINT32   defaultOptions
    )
    {
    nfs2MaxCacheLines     = defaultNumLines;
    nfs2BytesPerCacheLine = defaultLineSize;
    nfs2CacheOptions      = defaultOptions;
    }
#endif


#if defined (INCLUDE_NFS3_CLIENT)
/*******************************************************************************
*
* usrNfs3CacheInit - initialize the NFS v3 cache parameters
*
* This routine initializes the NFS v3 cache parameters.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void usrNfs3CacheInit
    (
    UINT32   defaultNumLines,
    UINT32   defaultLineSize,
    UINT32   defaultOptions
    )
    {
    nfs3MaxCacheLines     = defaultNumLines;
    nfs3BytesPerCacheLine = defaultLineSize;
    nfs3CacheOptions      = defaultOptions;
    }
#endif
