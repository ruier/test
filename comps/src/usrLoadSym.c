/* usrLoadSym.c - development symbol table loader */

/*
 * Copyright (c) 1992-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
02e,03aug09,pgh  Fix boot device parsing.
02d,21apr09,pgh  Fix the parsing of boot devices.
02c,11jul08,spw  Remove DHCP delay
02b,07jul08,pgh  Add support for SATA.
02a,06jun08,spw  Wait for successful DHCP client negotiation to complete before 
                 attempting to load symbol file.
01z,30apr08,v_r  Network symbol table special flags handling now done in
                 loader code.
01y,03mar07,jmt  Defect 89654- Symbol file does not load when booting from USB
                 drive
01x,24feb07,jmt  Remove calls to Config routines.  This is done in
                 usrConfig.c.
01w,30jan07,jmt  Add support for USB disks
01v,10jan06,bpn  Moved system symbol table initialisation to usrSysSymTbl.c
01u,02mar05,v_r  Included loadLibP.h
01t,02feb05,elp  fixed SPR #104666 (moved bootDevIsNetwork out of ifdef).
01s,26jan05,v_r  Fixed SPR #30900 (return value of symTblCreate() not checked).
		 Updated copyright notice for 2005. Removed PRJ_BUILD define
		 around usrLoadSyms() as it is a generic wrapper now.
01r,30aug04,jn   Apply code review changes
01q,02jun04,dtr  Fixed prioritySet placement for SPR 97947.
01p,03jun04,jn   Do not delay task if load is OK
01o,05apr04,jn   Comply with coding conventions
01n,25feb04,jn   Minor code reorganization.  Prepare to put the
                 "creation of boot device file system" code into a 
		 separate component
01m,22jul03,jmp  added support for INCLUDE_PASSFS_SYM_TBL.
01l,24may02,wap  made previous fix conditional on INCLUDE_NET_INIT
01k,10may02,wap  Reduce task priority below tNetTask's before doing netLoad()
                 (SPR #76107)
01j,29oct01,jkf  added rip's SPR 62094 & 62122 fixes, made a symlink from
                 src/config/usrLoadSym.c to this version.  Changed DESC.
01i,15apr99,sn   fixed SPR 26726 (=SPR 22644):
                 prevent netSymTbl from ever firing linked C++ ctors
01h,03jun98,ms   modfied for config tool
01g,19nov97,hdn  added support for TFFS.
01f,18nov96,dbt  ANSIfied function headers (SPR #7427.)
01e,01nov96,hdn  added support for PCMCIA.
01d,23jul96,hdn  added support for ATA driver.
01c,25oct94,hdn  swapped 1st and 2nd parameter of usrFdConfig().
01b,13nov93,hdn  added support for floppy and IDE drive.
01a,18sep92,jcf  written.
*/

/*
DESCRIPTION
This file is used to load the development symbol table from an
external source into the target runtime.  Possible sources are
the network, or from a local disc (floppy, ata/ide, scsi, etc).
This file is included as a component of either usrConfig.c for
BSP directory builds, or from the project facility.

NOMANUAL
*/

#ifndef  __INCusrLoadSymc
#define  __INCusrLoadSymc

/* includes */
#include <private/loadLibP.h>

#if 0
#define DEBUG 1   
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x) 
#endif


#if (defined(INCLUDE_NET_SYM_TBL) || defined(INCLUDE_PASSFS_SYM_TBL))
/*******************************************************************************
*
* symTblFilenameMake - construct filename for loading system symbol table
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/

LOCAL STATUS symTblFilenameMake 
    (
    char * pFilenameBuf,       /* buffer of size PATH_MAX + 1 */
    BOOL * pBootDevIsNetwork   /* tell caller if the boot device is network */
    )
    {
    char    bootDev[BOOT_DEV_LEN];
    int     i;

    /* Extract the new boot device name. */

    for (i = 0; i < BOOT_DEV_LEN; i++)
        {
        if (isalpha ((int)sysBootParams.bootDev[i]))
            {
            bootDev[i] = sysBootParams.bootDev[i];
            }
        else
            {
            bootDev[i] = 0;
            break;
            }
        }

    /* setup for non-network booting first */
    *pBootDevIsNetwork = FALSE;

    /* To avoid allocation during system startup, we take a buffer
       as an argument rather than allocating space for the string 
       we construct here. */

    if (strcmp (bootDev, "scsi") == 0)
	sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
#ifdef INCLUDE_IDE
    else if (strcmp (bootDev, "ide") == 0)
	{
        int drive = 0;
        int type = 0;

	if (strlen (sysBootParams.bootDev) == 3)
	    return ERROR;
	else
	    sscanf (sysBootParams.bootDev, "%*3s%*c%d%*c%d", &drive, &type);

	if (usrIdeConfig (drive, sysBootParams.bootFile) != OK)
	    return ERROR;

	sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
	}
#endif /* INCLUDE_IDE */
    else if (strcmp (bootDev, "fs") == 0)
        {
        sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
        }
#ifdef INCLUDE_ATA
    else if (strcmp (bootDev, "ata") == 0)
        {
        sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
        }
#endif /* INCLUDE_ATA */
#ifdef INCLUDE_FD
    else if (strcmp (bootDev, "fd") == 0)
	{
	sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
	}
#endif /* INCLUDE_FD */
#ifdef INCLUDE_PCMCIA
    else if (strcmp (bootDev, "pcmcia") == 0)
	{
	sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
	}
#endif /* INCLUDE_PCMCIA */
#ifdef INCLUDE_TFFS
    else if (strcmp (bootDev, "tffs") == 0)
	{
	sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
	}
#endif /* INCLUDE_TFFS */
#ifdef INCLUDE_USB
    else if (strcmp (bootDev, "usb") == 0)
        {
        int try = 0;
        BOOL success = FALSE;
        int symfd = ERROR;
        
        sprintf (pFilenameBuf, "%s.sym", sysBootParams.bootFile);
        
        /* check to see if the drive is mounted and ready */
        while ((++try < 11) && (success == FALSE))
            {
            symfd = open (pFilenameBuf, O_RDONLY, 0);

            if (symfd == ERROR)
                {
                /* first time, print out wait message */
                if (try == 1)
                    {
                    printf ("Waiting for USB drive to mount before loading symbol file\n");
                    }

                /* If it is the last time, just return
                 * otherwise, wait for a second before trying again.
                 */
                if (try == 10)
                    {
                    printf("\n");
                    }
                else
                    {
                    printf (".");
                    
                    /* wait and then try again */
                    taskDelay(sysClkRateGet());
                    }
                }
            else
                {
                /* disk is mounted */
                success = TRUE;
                if (try > 1)
                    {
                    printf("\n");
                    }
                
                /* close the file */
                close (symfd);
                }
            }
        }
#endif /* INCLUDE_USB */
    else
        {
        /* default */

        *pBootDevIsNetwork = TRUE;

        sprintf (pFilenameBuf, "%s:%s.sym", sysBootParams.hostName,
                 sysBootParams.bootFile);
        }

    return OK;
    }

/********************************************************************************
* netLoadSymTbl - load system symbol table from network (or from SCSI)
*
* This routine creates a system symbol table and loads it across the network.
* The name of the symbol table file is constructed as:
*      <host>:<bootFile>.sym
* The constant PATH_MAX is a system-wide constant, defined in limits.h.
* 
* RETURNS: OK, or ERROR if symbol table was not loaded.
*
* NOMANUAL
*/

STATUS netLoadSymTbl ()
    {
    char symTblFile [PATH_MAX + 1];   /* name of symbol table file */
    STATUS status = ERROR;  
    int symfd = ERROR;
    BOOL bootDevIsNetwork = FALSE;
    BOOL allSyms = FALSE;

#ifdef INCLUDE_NET_INIT
    IMPORT int netTaskPriority;
    int oldTaskPriority;

    taskPriorityGet (0, &oldTaskPriority);
#endif /* INCLUDE_NET_INIT */

    if (symTblFilenameMake (symTblFile, &bootDevIsNetwork) != OK) 
        return ERROR;  

#ifdef INCLUDE_NET_INIT
    if (bootDevIsNetwork)
        taskPrioritySet (0, netTaskPriority + 1);
#endif /* INCLUDE_NET_INIT */

    symfd = open (symTblFile, O_RDONLY, 0);

    if (symfd == ERROR)
		{
    	printf ("Failed. \n");  

    	printf ("Error opening %s: status = 0x%x\n", symTblFile, errno);
    	return ERROR;
		}

    DBG ( printf ("\nDEBUG: File opened successfully. \n")); 



    printf ("Loading symbol table from %s ...", symTblFile);

    /* Load system symbol table */

    if ((sysFlags & SYSFLG_DEBUG) == SYSFLG_DEBUG)
        allSyms = TRUE;

    status = loadSysSymTblLoad (symfd, allSyms); 
				
    if (close (symfd) != OK)
        status = ERROR;

    if (status == ERROR)
        {
        printf ("failed\n");

	printf ("errno = 0x%x\n", errno);  

	/* wait 3 seconds to allow error message to print */

	/* XXX jn - only delay in the case of failure, not in the 
	   case of success.  Delaying in the success case causes 
	   problems for simulators. */

	taskDelay (sysClkRateGet () * 3);	
	}
    else
        printf ("done\n");
 
#ifdef INCLUDE_NET_INIT
    taskPrioritySet (0, oldTaskPriority);
#endif /* INCLUDE_NET_INIT */

    return status;
    }

/******************************************************************************
*
* usrLoadSyms - load symbols
*
* A wrapper function is needed for the project build, because complex
* calls cannot be included in the initialization sequence.
*
* It would be possible to make the project and bsp build initialization
* of the networked symbol table the same by having both call the function
* usrLoadSyms.
*
* \NOMANUAL
*/ 

void usrLoadSyms (void)
    {

    if (sysSymTbl == NULL)
        return;

    netLoadSymTbl ();
    }
#endif  /* INCLUDE_NET_SYM_TBL || INCLUDE_PASSFS_SYM_TBL */
#endif  /* __INCusrLoadSymc */
