/* bootFsLoad.c - Boot loader application file system load support component */

/* 
*  Copyright (c) 2005,2007-2009 Wind River Systems, Inc.
* 
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01j,03aug09,pgh  Fix boot device parsing.
01i,21apr09,pgh  Fix the parsing of boot devices.
01h,02dec08,pgh  Add file system support.
01g,07jul08,pgh  Add SATA disk boot support.
01f,02mar07,jmt  Move devs command from bootAppShell.c
01e,27feb07,jmt  Update help format
01d,06feb07,jmt  Add bootAppPrintf to optimize size
01c,29jan07,jmt  Add call to bootDevFormatHelpAdd
01b,23jan07,jmt  Remove devSplit routine, use version in usrExtra.c
01a,12may05,jmt   written
*/

/*
DESCRIPTION
This module contains the support routines for the VxWorks boot loader 
application file system component.  This module is added to the boot 
loader application when the INCLUDE_BOOT_FILESYSTEMS component is 
included.  This module consists of an initialization routine for the 
file system load component, a help string display routine, routines to 
load the file from the file system, and file system command handlers.  
The help strings are added to the boot shell by the bootPrintDevsFuncAdd() 
and bootDevFormatHelpAdd() routines.  The file system shell commands are 
added to the boot shell by the bootCommandHandlerAdd() routine.  The file 
load handler is added to the boot loader application by the bootLoaderAdd() 
routine.  The file system command handlers provide support for the 
following boot shell commands:

\is
\i Print devices
The 'devs' command prints a list of the available devices in the boot 
loader application.
\i Change directory
The 'cd' command changes the current directory path to the path specified on 
the command line.
\i Print working directory
The 'pwd' command prints the current directory path.
\i Remove file
The 'rm' command removes the file specified on the command line.
\i List contents
The 'ls' command lists the contents of the specified directory path.  If no 
directory is specified, the current directory path contents are displayed.
\i Copy file
The 'cp' command copies the specified file from the source location to the specified destination.
\ie


INCLUDE FILES: bootFsLoad.h vxWorks.h 
*/ 

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <iosLib.h>
#include <private/iosLibP.h>
#include <stat.h>
#include <usrConfig.h>
#include <bootApp.h>
#include <bootLoadLib.h>
#include <private/bootFsLoad.h>

#include <bootFlash.h>
/* defines */

/* typedefs */

/* globals */

extern bootShellPrintfPrototype * bootShellPrintfFunc;

/* locals */


/* forward declarations */

LOCAL void bootFsPrintDevs();
STATUS bootFsLoad (char *, FUNCPTR *);

/* local command handlers */

LOCAL int bootAppDevsShow (char * cmd);
LOCAL int bootAppChdir (char * cmd);
LOCAL int bootAppGetcwd (char * cmd);
LOCAL int bootAppRemove (char * cmd);
LOCAL int bootAppListDir (char * cmd);
LOCAL int bootAppCopyFile (char * cmd);

/*******************************************************************************
*
* bootFsLoadInit - Initialize file system load component
*
* Initializes the boot loader application file system load component.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void bootFsLoadInit()
    {
    /* Add print devs function */
    bootPrintDevsFuncAdd(bootFsPrintDevs);

    /* Add commands to boot shell */
    bootCommandHandlerAdd("devs", bootAppDevsShow, BOOT_CMD_MATCH_STRICT,
 		"devs", "- print system devices");
    bootCommandHandlerAdd("cd", bootAppChdir, BOOT_CMD_MATCH_STRICT,
 		"cd path", "- change current directory path");
    bootCommandHandlerAdd("pwd", bootAppGetcwd, BOOT_CMD_MATCH_STRICT,
 		"pwd", "- print current directory path");
    bootCommandHandlerAdd("rm", bootAppRemove, BOOT_CMD_MATCH_STRICT,
 		"rm file", "- remove file");
    bootCommandHandlerAdd("ls", bootAppListDir, BOOT_CMD_MATCH_STRICT,
 		"ls path", "- list directory path");
    bootCommandHandlerAdd("cp", bootAppCopyFile, BOOT_CMD_MATCH_STRICT,
 		"cp src dst", "- copy file from src to dst");

    /* register file system loader component */
    (void) bootLoaderAdd((bootLoaderPrototype *) bootFsLoad);

    /* add command help */
    bootDevFormatHelpAdd(
        "boot device: fs\n "
        "file name: /FileSystemDeviceName/vxWorks\n "
        "other: network device name\n");
    }

/*******************************************************************************
*
* bootFsPrintDevs - Displays file system devices
*
* This routine is executed by the boot shell to output the header for the list 
* of file system devices.  It follows this header with a list of each file 
* system device that is available in the system.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void bootFsPrintDevs()
    {
    STATUS          rtv;
    FAST DEV_HDR *  pDevHdr;
    struct stat     devData;
    char *          pStr;

    bootAppPrintf ("\n  Filesystem Devices: ",0,0,0,0,0,0);
    for (pDevHdr = (DEV_HDR *) DLL_FIRST (&iosDvList);
            pDevHdr != NULL;
                pDevHdr = (DEV_HDR *) DLL_NEXT (&pDevHdr->node))
        {
        /* stat() won't return for tgtsvr if not running on host */

        pStr = strstr(pDevHdr->name, "tgtsvr");
        if (pStr != NULL)
            {
            bootAppPrintf ("%s ", (int)pDevHdr->name,0,0,0,0,0);
            }
        else
            {
            rtv = stat (pDevHdr->name, &devData);
            if (rtv == OK)
                {
                /* File system device if a directory */

                if (S_ISDIR(devData.st_mode))
                    bootAppPrintf ("%s ", (int)pDevHdr->name,0,0,0,0,0);
                }
            }
        }
    }

/******************************************************************************
*
* bootFsFileLoad - Load a VxWorks image file from a file system
*
* This routine loads a file from an already mounted file system.  This routine
* is called by the various file system device load handlers to load the VxWorks
* image file.
* 
* RETURNS: 
*   OK if the file is loaded.
*   ERROR if file can not be loaded.
*
* ERRNO: N/A
*/

STATUS bootFsFileLoad
    (
    char *      fileName,  /* path and file name */
    FUNCPTR *   pEntry     /* address to start execution */
    )
    {
    STATUS status = OK;
    int fd;

    /* load the boot file */
    bootAppPrintf ("Loading %s...", (int) fileName, 0,0,0,0,0);

    if ((fd = open (fileName, O_RDONLY, 0)) == ERROR)
		{
        bootAppPrintf ("\nCannot open \"%s\".\n", (int) fileName, 0,0,0,0,0);
        return (ERROR);
		}

    if (bootLoadModule (fd, pEntry) != OK)
  		{
		bootAppPrintf ("\nLoad Module failed.\n", 0,0,0,0,0,0);
        status = ERROR;
		}

    close (fd);
    return (status);
    }

/*******************************************************************************
*
* bootFsLoad - File system load handler
*
* This routine is the file system load handler that is registered with the
* boot loader application to load files from file system devices.  It checks that the
* boot device is a file system device, and if so loads the file specified by
* the boot line parameters.
* 
* RETURNS: 
*   OK if file loaded successfully,
*   ERROR if an error occurs while loading the file
*   BOOT_STATUS_CONTINUE if the boot device is not a file system device
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS bootFsLoad
    (
    char *      bootString, /* boot command string */
    FUNCPTR *   pEntry      /* address to start execution */
    )
    {
    STATUS  status = BOOT_STATUS_CONTINUE;
    char    bootDev[BOOT_DEV_LEN];
    int     i;

    int CPUID;
    UINT32 entryAddr=0;
    UINT32 fnLenOffset,fnOffset,fileOffset=0,fileLen=0;
    CPUID = sysGetCpuID();
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
    if ((strcmp (bootDev, "fs") == 0) ||
        (strcmp (bootDev, "fd") == 0) ||
        (strcmp (bootDev, "ata") == 0) ||
        (strcmp (bootDev, "ide") == 0) ||
        (strcmp (bootDev, "scsi") == 0) ||
        (strcmp (bootDev, "tffs") == 0) ||
        (strcmp (bootDev, "pcmcia") == 0))
        {
        /* load boot file */
    	/* add by ff 2011.07.09-------begin */
    	if((strcmp (bootDev, "tffs") == 0))
    	{
    		if(CPUID==0)
    		 {
    		    queryTask();
    		    if (bootFsFileLoad (sysBootParams.bootFile, pEntry) != OK)
    		    {
    		         bootAppPrintf ("\n TFFS: Error loading file: errno = 0x%x.\n", errno, 0,0,0,0,0);
    		         status = ERROR;
    		    }
    		    else
    		    {
    		       status = OK;
    		     }
    		  }
    		 else
    		 {
    		    bootAppPrintf("Loading %s...\n",sysBootParams.bootFile,0,0,0,0,0);
    		   	fnLenOffset = CPUA_BAR2_ADDR+CPUB_REG_BASE+FNLEN_OFFSET;
				fnOffset = CPUA_BAR2_ADDR+CPUB_REG_BASE+FLNAME_OFFSET;
				fileOffset = CPUA_BAR2_ADDR+FL_CPUB_BASE+FL_OFFSET+0xc0;     //6.7: 0xa0   6.8:0xc0		 		
				filenameSet(sysBootParams.bootFile,fnLenOffset,fnOffset);
				//*(UINT32*)(CPUA_BAR2_ADDR+EP_FLAG_OFFSET) = *(UINT32*)(CPUA_BAR2_ADDR+EP_FLAG_OFFSET)|0x01;    		
				//while(((*(UINT32*)LOCAL_BUS_ADDR_CS3)&0x01)!=0x01);
				*(UINT8*)(CPUA_BAR2_ADDR+EP_FLAG_OFFSET+1) = 0x5a; 
				while((*(UINT8*)(BOOTROM_TFFS_SYNC_REG+1)) != 0x5a);
				fileLen = *(UINT32*)(CPUA_BAR2_ADDR+CPUB_REG_BASE+FLLEN_OFFSET);
				entryAddr = *(UINT32*)(CPUA_BAR2_ADDR+FL_CPUB_BASE+FL_OFFSET+ENTRY_OFFSET);
    		    
    		#ifndef  VXB_M8641_DMA
    		    ddrTrans(fileOffset,entryAddr,fileLen); 	 /* 0x100000 --->entryAddr*/
    		#else
    		    sysDmaSend(0,fileOffset,entryAddr,fileLen);
    		#endif
    		    go((FUNCPTR)entryAddr);    	
    		}
    	}
    	else
    	{
    	/* add by ff 2011.07.09-------end */
        if (bootFsFileLoad (sysBootParams.bootFile, pEntry) == OK)
            {
            status = OK;
            }
        else
            {
            bootAppPrintf ("\nError loading file: errno = 0x%x.\n", errno,0,0,0,0,0);
            status = ERROR;
            }
    	}   // add by ff 2011.07.09
        }

    return (status);
    }
    

/******************************************************************************
*
* bootAppDevsShow - Display system devices
* 
* This routine is the command handler for the "devs" boot shell command.  It
* calls the I/O system to display the mounted devices.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppDevsShow
    (
    char * cmd  /* input command string */
    )
    {
    iosDevShow (); 
    return BOOT_STATUS_COMPLETE;
    }

/******************************************************************************
*
* bootAppChdir - Change current directory path
* 
* This routine is the command handler for the "cd" boot shell command to 
* change to a new current default directory path.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppChdir
    (
    char * cmd  /* input command string */
    )
    {
    STATUS  rtv;

    /* skip the command name and point to first parameter */
    bootCmdNameSkip (&cmd, 2);

    /* change to directory path specified by the user */
    rtv = chdir (cmd); 
    if (rtv == ERROR)
        {
        switch (errno)
            {
            case S_ioLib_NO_DEVICE_NAME_IN_PATH:
                bootAppPrintf ("No device name found in path.\n", 
                    0,0,0,0,0,0);
                break;
            case S_ioLib_NAME_TOO_LONG:
                bootAppPrintf ("Path name exceeds internal limits.\n", 
                    0,0,0,0,0,0);
                break;
            default:
                bootAppPrintf ("Could not change directory to %s.\n", (int)cmd, 
                    0,0,0,0,0);
                break;
            }
        }

    return BOOT_STATUS_COMPLETE;
    }


/******************************************************************************
*
* bootAppGetcwd - Get current directory path
* 
* This routine is the command handler for the "pwd" boot shell command to 
* print the current default directory path.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppGetcwd
    (
    char * cmd  /* input command string */
    )
    {
    char    pwdBuf[256];
    char *  pBuf;

    /* change to directory path specified by the user */
    pBuf = getcwd (pwdBuf, 256); 

    if (pBuf == pwdBuf)
        {
        bootAppPrintf ("%s\n", (int)pwdBuf, 0,0,0,0,0);
        }

    return BOOT_STATUS_COMPLETE;
    }


/******************************************************************************
*
* bootAppRemove - Remove a file
* 
* This routine is the command handler for the "rm" boot shell command to 
* remove a file.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppRemove
    (
    char * cmd  /* input command string */
    )
    {
    STATUS  rtv;

    /* skip the command name and point to first parameter */
    bootCmdNameSkip (&cmd, 2);

    /* remove the file specified by the user */
    rtv = remove (cmd); 

    if (rtv == ERROR)
        bootAppPrintf ("ERROR: could not remove %s\n", (int)cmd,0,0,0,0,0);

    return BOOT_STATUS_COMPLETE;
    }


/******************************************************************************
*
* bootAppListDir - list directory contents
* 
* This routine is the command handler for the "ls" boot shell command to list 
* the directory contents of the input command string.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppListDir
    (
    char * cmd  /* input command string */
    )
    {
    STATUS  rtv;

    /* skip the command name and point to first parameter */
    bootCmdNameSkip (&cmd, 2);

    /* list the contents of the directory specified by the user */
    rtv = ls (cmd, FALSE); 

    if (rtv == ERROR)
        bootAppPrintf ("ERROR: could not read %s\n", (int)cmd,0,0,0,0,0);

    return BOOT_STATUS_COMPLETE;
    }


/******************************************************************************
*
* bootAppCopyFile - Copy file from source to destination
* 
* This routine is the command handler for the "cp" shell command to copy a 
* file from the source location to the destination.  The source and destination
* are specified in the input command string
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppCopyFile
    (
    char * cmd  /* input command string */
    )
    {
    STATUS  rtv;
    char *  pStr;
    char    src[200];

    /* skip the command name and point to first parameter */
    bootCmdNameSkip (&cmd, 2);

    /* get the source argument */
    pStr = &src[0];

    /* skip to the next argument */
    while (!(isspace ((UINT) *cmd)))
        {
        *pStr++ = *cmd++;
        }
    *pStr++ = '\0';


    /*
     * move past the space between the parameters to 
     * get the destination argument.
     */

    bootSpaceSkip (&cmd);

    /* copy the source to the destination */
    rtv = cp (&src[0], cmd); 

    if (rtv == ERROR)
        bootAppPrintf ("ERROR: could not copy file %s\n", (int)src,0,0,0,0,0);

    return BOOT_STATUS_COMPLETE;
    }

