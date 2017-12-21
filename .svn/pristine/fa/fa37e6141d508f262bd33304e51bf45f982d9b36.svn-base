/* bootApp.c - VxWorks boot loader application */

/* 
 *  Copyright (c) 2005-2009 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01x,03aug09,pgh  Fix boot device parsing.
01w,23jul09,pgh  Fix return value for bootNetSettingsUpdate()
01v,15jul09,mdk  fix merge error
01u,14jul09,mdk  merge from networking branch
01t,04jun09,pgh  Check for boot string error before saving it to system boot
                 parameters.
01s,21apr09,pgh  Fix the parsing of boot devices.
01r,02jul08,spw  Port changes for WIND00122447
01q,16may08,pgh  Add support SATA boot disk.
01p,27feb08,jmt  Add support for BSP defined go routines
01o,15jan08,pgh  Add validity check function to bootLoad().
01n,14aug07,jmt  Remove deprecated APIs
01m,23apr07,jmt  Defect 93088 - networking not setup correctly for some
                 bootline updates
01l,10apr07,jmt  Add error message is no command is found
01k,27feb07,jmt  Update for review comments
01j,06feb07,jmt  Modify bootAppPrintf to optimize size
01i,25jan07,jmt  Add Network Update routine pointer
01h,14nov06,jmt  Defect 81465: END Interrupt not disabled before jumping to
                 new image
                 Defect 81462: Need to disable MMU before booting new image 
                 for some BSPs
01g,08sep06,jmt  Change cacheClear calls to cacheTextUpdate
01f,23aug06,jmt  Move bootAppBootFileLoadAndGo to bootApp.c
                 Change malloc calls to KMEM_ALLOC 
01e,19jul06,jmt  Remove banner print so that it is displayed earlier.
                 Moved to bootAppShellInit.
01d,30mar06,pes  Add support for passing arguments and start type to bootApp.
01c,12sep05,jmt  Modify init to spawn bootAppTask
01b,27jul05,jmt  Fix PPC compile problem
01a,28apr05,jmt   written
 */

/*
DESCRIPTION

This module contains the initialization routine and main processing routines 
for the VxWorks boot loader application.  To create a boot loader application,
use a VxWorks image project (VIP) with the PROFILE_BOOTAPP configuration 
profile.  The boot loader application can be run out of RAM for testing 
purposes, or it can be programmed into a non-volatile device (such as flash 
memory).  The boot loader application implements the following features:

\is
\i Main processing task
The main processing task of the boot loader application executes the boot 
shell's count down and auto-boot routine, and if the auto-boot routine 
returns, the boot shell is executed.  If the boot shell is not included, the 
boot process proceeds using the default (statically-defined) boot parameters.
\i Boot shell command processing support
The routine bootSpaceSkip() is used to skip over white space when parsing the 
command line of the boot shell.  The routine bootCmdNameSkip() is used to 
skip over the command name in the command string.  The getArg() routine is 
used to extract arguments from the command line.
\i Boot shell help string support
Support for boot shell help string is provided by the routine 
bootPrintDevsFuncAdd(), which adds a device display routine to the boot shell, 
the routine bootDevFormatHelpAdd(), which adds device format help strings to 
the boot shell, and the routine bootFlagHelpAdd(), which adds help strings 
for the boot flags.  In addition, the bootAppPrintf() routine provides the
ability to output formatted text strings.
\i Boot shell command support
The boot loader application provides the routine bootCommandHandlerAdd() to add 
new commands and command handler routines to the boot shell, and the routine 
bootCommandHelpAdd() to add command help strings to the boot shell.
\i File loader support
The boot loader application provides the routine bootLoaderAdd() to add new 
file load handlers, and the routines bootLoad() and bootAppBootFileLoadAndGo() 
to process each of the file load handlers.
\i File execution support
The boot loader application provides a the go() routine to start execution of 
the file images loaded by the boot loader application.
\ie


INCLUDE FILES: bootApp.h vxWorks.h 
 */ 

/* includes */

#include <vxWorks.h>
#include <bootLib.h>
#include <ctype.h>
#include <string.h>
#include <sysLib.h>
#include <usrConfig.h>

#include <bootApp.h>
#include <private/bootAppShell.h>
#include <private/bootNetLoad.h>

/* defines */

/* maximum line length for bootCmdLoop() command line input */

#define AUTOBOOT_TIMEOUT    4        /* number of seconds before auto-boot */
/* MMU disable call */
#ifndef _WRS_BOOT_DISABLE_MMU
#define _WRS_BOOT_DISABLE_MMU  VM_ENABLE(FALSE)
#endif

/* task defines */

#define BOOTAPP_TASK_PRIORITY    60
#define BOOTAPP_TASK_OPTIONS      0
#define BOOTAPP_TASK_STACK_SIZE  10240

/* Typedefs */

/* information for a boot command */
typedef struct bootCommandInfoStruct
{
	char * commandString;                     /* command string to match */
	char   length;                            /* length of command string */
	char   matchingRule;                      /* how to match string */
	bootCmdHandlerPrototype * commandHandler; /* command proc func */

	struct bootCommandInfoStruct * next;      /* next command */ 
} BOOT_COMMAND_INFO_STRUCT;

/* information for a boot loader */
typedef struct bootLoaderInfoStruct
{
	bootLoaderPrototype * loadHandler;        /* loader proc func */

	struct bootLoaderInfoStruct * next;       /* next command */ 
} BOOT_LOADER_INFO_STRUCT;


/* globals */

extern int sysStartType;
BOOL bootDelayedInit;
bootPrintHeaderPrototype * bootAppPrintHeaderFunc;
bootShellAutobootPrototype * bootShellAutobootFunc;
bootShellProcPrototype * bootAppShellFunc;
bootShellHelpAddPrototype * bootShellHelpAddFunc;
bootShellFlagHelpAddPrototype * bootShellFlagHelpAddFunc;
bootShellDevFormatHelpAddPrototype * bootShellDevFormatHelpAddFunc;
bootShellPrintfPrototype * bootShellPrintfFunc;
bootShellPrintDevsAddPrototype * bootShellPrintDevAddFunc;
bootNetDevStopAllPrototype * bootNetDevStopAllFunc;
bootNetSettingsUpdatePrototype * bootNetSettingsUpdateFunc;
bootGoPrototype * bootGoOverrideFunc;

IMPORT VOID	sysIntLock	(void);

/* locals */

int     numberOfCmdHandlers;
struct bootCommandInfoStruct *    firstCmd;
struct bootCommandInfoStruct *    lastCmd;

int        numberOfLoaders;
struct bootLoaderInfoStruct *    firstLoader;
struct bootLoaderInfoStruct *    lastLoader;

UINT32 bootMemBase;
int bootAppTaskId = 0;
char * bootShellPrompt = "[VxWorks Boot]: ";
BOOL printBootShellPrompt = TRUE;
BOOL bootDisableMmuBeforeEntry = TRUE;

/* forward declarations */

void bootAppTask(void);
STATUS bootParamCheck(BOOT_PARAMS *);
void go (FUNCPTR);

/* local command handlers */

int bootAppBootFileLoadAndGo (char * cmd);

/*******************************************************************************
 *
 * bootAppInit - Initializes the boot loader application
 *
 * This routine initializes the boot loader application.
 *
 * RETURNS: OK
 *
 * ERRNO: N/A
 *
 */
STATUS bootAppInit
(
		BOOL delayedInit,    /* TRUE for delayed boot device initialization */
		UINT32 memBase,      /* LOCAL_MEM_LOCAL_ADRS */
		BOOL disableMmuBeforeEntry /* TRUE if MMU should be disabled */
)
{
	/* Initialize variables */
	bootDelayedInit = delayedInit;
	bootMemBase = memBase;
	bootDisableMmuBeforeEntry = disableMmuBeforeEntry;
	numberOfCmdHandlers = 0;
	firstCmd = NULL;
	lastCmd = NULL;

	numberOfLoaders = 0;
	firstLoader = NULL;
	lastLoader = NULL;

	/* initialize handlers */
	bootAppShellFunc = NULL;
	bootAppPrintHeaderFunc = NULL;
	bootShellAutobootFunc = NULL;
	bootShellHelpAddFunc = NULL;
	bootShellDevFormatHelpAddFunc = NULL;
	bootShellFlagHelpAddFunc = NULL;
	bootShellPrintDevAddFunc = NULL;

	/* initialize misc func pointers */
	bootNetDevStopAllFunc = NULL;
	bootNetSettingsUpdateFunc = NULL;
	bootGoOverrideFunc = NULL;

	/* register loadAndGo function */
	bootCommandHandlerAdd("@", bootAppBootFileLoadAndGo, BOOT_CMD_MATCH_LOOSE,
			"@",  "- boot (load and go)");

	return (OK);
}

/*******************************************************************************
 *
 * bootApp - Spawn the boot loader application main loop 
 *
 * This routine spawns the boot loader application task, which contains the main 
 * processing loop for the boot loader application.
 *
 * RETURNS: N/A
 *
 * ERRNO: N/A
 *
 * \NOMANUAL
 */
void bootApp()
{
	/* spawn the main loop */
	bootAppTaskId = taskSpawn
			("bootApp",
					BOOTAPP_TASK_PRIORITY,
					BOOTAPP_TASK_OPTIONS,
					BOOTAPP_TASK_STACK_SIZE,
					(FUNCPTR) bootAppTask,
					0,0,0,0,0,0,0,0,0,0  /* args */
			);
}

/*******************************************************************************
 *
 * bootAppTask - Main processing loop for the boot loader application
 *
 * This routine processes input for the boot loader application.  This 
 * routine first calls the count down and auto-boot routine.  If the 
 * auto-boot routine returns, then this routine looks for input for 
 * the boot shell.  Boot shell input is matched to command strings that 
 * have been added to the boot shell by boot loader application components.  
 * When a match is found, the command handler associated with the command 
 * string is executed.
 *
 * RETURNS: N/A
 *
 * ERRNO: N/A
 *
 */

void bootAppTask()
{
	char line [BOOT_CMD_MAX_LINE];
	char *pLine;
	struct bootCommandInfoStruct *    currentCmd;
	int procStatus;

	if (sysStartType < 0)
		sysStartType = BOOT_CLEAR;

	/* check for autoboot function */
	if (bootShellAutobootFunc != NULL)
		bootShellAutobootFunc(AUTOBOOT_TIMEOUT);

	/* If we're here, either we aren't auto-booting, or we got an error
	 * auto-booting, or the auto-booting was stopped. 
	 */

	bootAppPrintf("\n",0,0,0,0,0,0);

	FOREVER
	{
		/* see if the Boot App Shell is installed */
		if (bootAppShellFunc != NULL)
		{
			if (printBootShellPrompt == TRUE)
			{
				bootAppPrintf (bootShellPrompt,0,0,0,0,0,0);
				printBootShellPrompt = FALSE;
			}

			/* get the next command line */
			pLine = bootAppShellFunc(line, BOOT_CMD_MAX_LINE);
		}
		else
		{
			/* default, boot */
			pLine = "@";
		}

		if (strlen(pLine) > 0)
		{
			/* process the command */
			currentCmd = firstCmd;
			procStatus = BOOT_STATUS_CONTINUE;
			while ((currentCmd != NULL) &&
					(procStatus == BOOT_STATUS_CONTINUE))
			{
				/* Check to see if the incoming string matches the command.
				 * Requires that the commandString matches the command AND
				 * there is a space and EOS after the command OR
				 * the command matching for this command is "loose".
				 * "loose" command checking allows for parameters to follow
				 * the command without a space between the command and parameter.
				 */
				if ((strncmp(pLine, currentCmd->commandString, 
						currentCmd->length) == 0) &&
						((currentCmd->matchingRule == BOOT_CMD_MATCH_LOOSE) ||
								(pLine[(int) currentCmd->length] == EOS) ||
								(pLine[(int) currentCmd->length] == ' ')))
				{
					procStatus = 
							currentCmd->commandHandler(pLine);
				}

				/* check next command */
				currentCmd = currentCmd->next;
			}

			/* if command was not found, print error message */
			if (procStatus == BOOT_STATUS_CONTINUE)
			{
				bootAppPrintf("Unrecognized command. Type '?' for help.\n",
						0,0,0,0,0,0); 
			}

			/* setup for next command */
			printBootShellPrompt = TRUE;
		}
	} /* FOREVER */
}

/*******************************************************************************
 *
 * bootAppPrintf - Print a formatted string to the boot loader application shell
 *
 * This routine prints the specified formatted string with associated arguments 
 * to the boot loader application shell.  This routine is included with all 
 * boot loader application images, but only outputs the formatted string if 
 * the boot shell component (INCLUDE_BOOT_SHELL) is included.  The number of 
 * parameters is limited in order to prevent the variable argument library 
 * modules from being included in the boot loader application image.  This was
 * done to keep the size of the boot loader application to a minimum.
 *
 * RETURNS: N/A
 *
 * ERRNO: N/A
 *
 */
void bootAppPrintf
(
		char * x,   /* format string */
		int a,      /* parameter 1 */
		int b,      /* parameter 2 */
		int c,      /* parameter 3 */
		int d,      /* parameter 4 */
		int e,      /* parameter 5 */
		int f       /* parameter 6 */
)
{
	if (bootShellPrintfFunc != NULL)        
		bootShellPrintfFunc(x,a,b,c,d,e,f); 
}

/*******************************************************************************
 *
 * bootCommandHandlerAdd - Add a command and handler to the boot shell
 *
 * This routine adds a command string and an associated command handler to the 
 * list of commands processed by the boot shell.  If the helpProto string is 
 * NULL, the command will be added to the boot shell, but no help will be 
 * registered for this command.
 *
 * RETURNS: 
 *   OK if the command was added to the boot shell
 *   ERROR if memory could not be allocated for the new command
 *
 * ERRNO: N/A
 */

STATUS bootCommandHandlerAdd
(
		char *                      commandString,  /* text for new command */
		bootCmdHandlerPrototype *   handler,        /* routine to handle command */
		char                        matchingRule,   /* strict or loose parsing */
		char *                      helpProto,      /* command prototype */
		char *                      helpDesc        /* help description */
)
{
	STATUS status = OK;
	struct bootCommandInfoStruct * newCommand;

	/* create a new structure for the list */
	newCommand = KMEM_ALLOC(sizeof(struct bootCommandInfoStruct));

	if (newCommand != NULL)
	{
		/* save the information in the structure */
		newCommand->commandString = commandString;
		newCommand->commandHandler = handler;
		newCommand->matchingRule = matchingRule;
		newCommand->length = strlen(commandString);
		newCommand->next = NULL;

		/* save the structure */
		if (lastCmd != NULL)
		{
			/* add to list */
			lastCmd->next = newCommand;
		}
		else
		{
			/* first command */
			firstCmd = newCommand;
		}
		/* update last command */
		lastCmd = newCommand;
	}
	else
	{
		status = ERROR;
	}

	/* add help strings to help display */
	if ((bootShellHelpAddFunc != NULL) &&
			(helpProto != NULL))
		bootShellHelpAddFunc(helpProto, helpDesc);

	return status;
}

/*******************************************************************************
 *
 * bootLoaderAdd - Add a file load handler to the boot loader application
 *
 * This routine adds a file load handler to the list of file load handlers 
 * processed by the boot loader application.  File load handlers are the 
 * routines used to load VxWorks image files from specific devices, examples
 * are network devices and file system devices.  The file load handlers 
 * implement the device specific details of loading a file.
 *
 * RETURNS: 
 *   OK if the load handler was added to the boot loader application
 *   ERROR if memory could not be allocated for the new handler
 *
 * ERRNO: N/A
 */

STATUS bootLoaderAdd
(
		bootLoaderPrototype * loader    /* new image file loader function */
)
{
	STATUS status = OK;
	struct bootLoaderInfoStruct * newLoader;

	/* create a new structure for the list */
	newLoader = (struct bootLoaderInfoStruct *) 
        		KMEM_ALLOC(sizeof(struct bootLoaderInfoStruct));

	if (newLoader != NULL)
	{
		/* save the information in the structure */
		newLoader->loadHandler = loader;
		newLoader->next = NULL;

		/* save the structure */
		if (lastLoader != NULL)
		{
			/* add to list */
			lastLoader->next = newLoader;
		}
		else
		{
			/* first command */
			firstLoader = newLoader;
		}
		/* update last command */
		lastLoader = newLoader;
	}
	else
	{
		/* Memory allocation error */
		status = ERROR;
	}

	return status;
}


/*******************************************************************************
 *
 * bootCommandHelpAdd - Add a help string for a command to the help display
 *
 * This routine adds a command help string to the boot shell's help display.  
 * This routine does nothing if the boot loader application shell component
 * (INCLUDE_BOOT_SHELL) is not included.  The boot shell loops through each 
 * added help string, displaying each of them to the user.
 *
 * RETURNS: 
 *   OK if the help string was added
 *   ERROR if the boot shell is not included
 *
 * ERRNO: N/A
 */

STATUS bootCommandHelpAdd 
(
		char * helpProto,   /* new command prototype string */
		char * helpDesc     /* help description for new command */
)
{
	STATUS status = OK;
	/* add help strings to help display */
	if (bootShellHelpAddFunc != NULL)
	{
		bootShellHelpAddFunc(helpProto, helpDesc);
	}
	else
		status = ERROR;

	return status;
}

/*******************************************************************************
 *
 * bootPrintDevsFuncAdd - Add a device display routine to the help display
 *
 * This routine adds a device display routine to the boot shell help display.  
 * This routine does nothing if the boot loader application shell component 
 * (INCLUDE_BOOT_SHELL) is not included.  The boot shell loops through each 
 * added device display routine to display all available devices to the user.
 * The device display routines show the names and format details of the 
 * devices available to the boot loader application.
 *
 * RETURNS:
 *   OK if the device display routine was added
 *   ERROR if the boot shell is not included
 *
 * ERRNO: N/A
 */

STATUS bootPrintDevsFuncAdd
(
		bootVoidPrototype * pPrintDevFunc   /* Print Devs routine */
) 
{
	STATUS status = OK;
	/* add help strings to help display */
	if (bootShellPrintDevAddFunc != NULL)
	{
		bootShellPrintDevAddFunc(pPrintDevFunc);
	}
	else
		status = ERROR;

	return status;
}

/*******************************************************************************
 *
 * bootDevFormatHelpAdd - Add a device format help string to the help display
 *
 * This routine adds a device format help string to the boot shell help display.
 * This routine does nothing if the boot loader application shell component 
 * (INCLUDE_BOOT_SHELL) is not included.  The boot shell loops through the added
 * device format help strings, to display device format help for each device 
 * supported by the boot loader application.  The device format help strings 
 * provide the correct syntax for specifying the devices available to the boot 
 * loader application.
 *
 * RETURNS:
 *   OK if the device display format help string was added
 *   ERROR if the boot shell is not included
 *
 * ERRNO: N/A
 */

STATUS bootDevFormatHelpAdd 
(
		char * helpDesc     /* device format description help string */
)
{
	STATUS status = OK;

	/* add help strings to help display */
	if (bootShellDevFormatHelpAddFunc != NULL)
	{
		bootShellDevFormatHelpAddFunc(helpDesc);
	}
	else
		status = ERROR;

	return status;
}

/*******************************************************************************
 *
 * bootFlagHelpAdd - Add a boot flag help string to the help display
 *
 * This routine adds a help string to the boot shell help display for the flags 
 * boot parameter.  This routine does nothing if the boot loader application 
 * shell component (INCLUDE_BOOT_SHELL) is not included.  The boot shell loops 
 * through the added flag help strings, to display help for each of the boot 
 * flags.
 *
 * RETURNS: 
 *   OK if the boot flag help string was added
 *   ERROR if the boot shell is not included
 */

STATUS bootFlagHelpAdd 
(
		UINT32 flag,        /* boot flag */
		char * helpDesc     /* boot flag description help string */
)
{
	STATUS status = OK;
	/* add help strings to help display */
	if (bootShellFlagHelpAddFunc != NULL)
	{
		bootShellFlagHelpAddFunc(flag, helpDesc);
	}
	else
		status = ERROR;

	return status;
}


/*******************************************************************************
 *
 * bootSpaceSkip - Advance command string pointer past white space
 *
 * This routine takes the specified string pointer parameter, and increments 
 * the string pointer to point to the next non-white-space character in the 
 * string.  This routine is used when parsing the boot shell input command line 
 * to skip over white space characters.
 * 
 * RETURNS: N/A
 *
 * ERRNO: N/A
 */

void bootSpaceSkip
(
		FAST char **strptr  /* pointer to string pointer */
)
{
	while (isspace ((UINT) ** strptr))
	{
		++ * strptr;
	}
}

/*******************************************************************************
 *
 * bootCmdNameSkip - Advance command string pointer past command name
 *
 * This routine is passed the boot shell input command string, and the length 
 * of the command portion of the string.  This routine increments this command 
 * string pointer to point to the next non-white-space character in the string 
 * that follows the command string.  This routine is used when parsing the boot 
 * shell input command line to skip to the command arguments.
 * 
 * RETURNS: N/A
 *
 * ERRNO: N/A
 *
 */

void bootCmdNameSkip
(
		FAST char **strptr,         /* pointer to command string pointer */
		char        cmdNameLength   /* length of command string */
)
{
	/* skip command name */
	*strptr += cmdNameLength;

	/* go to next non-space character */
	while (isspace ((UINT) ** strptr))
	{
		++ * strptr;
	}
}

/******************************************************************************
 *
 * getArg - Get argument from input command string
 *
 * This routine is given a pointer to the boot shell input command string 
 * pointer, and extracts the next numerical argument from the command string.  
 * If the argument is not optional, and no argument is found, then an error is 
 * reported.  The pointer to the command string is updated to point to the new 
 * position in the command string that follows the extracted numerical argument.
 * This routine is used to parse the boot shell input command string for 
 * numerical arguments to boot shell commands.
 *
 * RETURNS: 
 *   OK if the argument is found and returned
 *   OK if the argument is optional and not present
 *   ERROR if the argument is not optional and not present
 *   ERROR if the argument is not numerical
 *
 * ERRNO: N/A
 */

STATUS getArg
(
		FAST char **    ppString,   /* pointer to command string pointer */
		int *           pValue,     /* point to return value */
		BOOL            defaultHex, /* TRUE if argument is hex (even w/o 0x) */
		BOOL            optional    /* TRUE if argument is optional */
)
{
	/* skip the command name */
	bootSpaceSkip (ppString);

	/* if nothing left, complain if arg is not optional */

	if (**ppString == EOS)
	{
		if (!optional)
		{
			bootAppPrintf ("missing parameter\n",0,0,0,0,0,0);
			return (ERROR);
		}
		else
			return (OK);
	}


	/* scan arg */

	if (bootScanNum (ppString, pValue, defaultHex) != OK)
	{
		bootAppPrintf("invalid parameter\n",0,0,0,0,0,0);
		return (ERROR);
	}

	bootSpaceSkip (ppString);

	/* if we encountered ',' delimiter, step over it */

	if (**ppString == ',')
	{
		++*ppString;
		return (OK);
	}

	/* if end of line, scan is ok */

	if (**ppString == EOS)
		return (OK);

	/* we got stopped by something else */

	bootAppPrintf ("invalid parameter\n",0,0,0,0,0,0);
	return (ERROR);
}

/*******************************************************************************
 *
 * bootParamCheck - Validate boot parameters
 *
 * This routine validates the boot parameters and displays an error message
 * for improperly set parameters.  This routine only checks the boot parameters
 * that contain IP addresses.  For IP address parameters, the routine determines 
 * whether or not they should be set.  If so, it determines whether or not they
 * appear to contain valid IP addresses.
 *
 * RETURNS: 
 *   OK if no errors are found in the boot line parameters
 *   ERROR if an error was found in the boot line parameters
 *
 * ERRNO: N/A
 *
 */

STATUS bootParamCheck
(
		BOOT_PARAMS *   pBootParams
)
{
	int     lenIPadr;
	int     lenFileName;
	char    bootDev[BOOT_DEV_LEN];
	char *  pIpType;
	int     i;

	/* Extract the boot device name. */

	for (i = 0; i < BOOT_DEV_LEN; i++)
	{
		if (isalpha ((int)pBootParams->bootDev[i]))
		{
			bootDev[i] = pBootParams->bootDev[i];
		}
		else
		{
			bootDev[i] = 0;
			break;
		}
	}

	/* Is it a network boot device? */
	if ((strcmp (bootDev, "scsi") != 0) &&
			(strcmp (bootDev, "ide") != 0) &&
			(strcmp (bootDev, "ata") != 0) &&
			(strcmp (bootDev, "fd") != 0)  &&
			(strcmp (bootDev, "fs") != 0)  &&
			(strcmp (bootDev, "usb") != 0)  &&
			(strcmp (bootDev, "tsfs") != 0)  &&
			(strcmp (bootDev, "tffs") != 0))
	{
		if ((pBootParams->flags & SYSFLG_AUTOCONFIG) != SYSFLG_AUTOCONFIG)
		{
			if ((strcmp (bootDev, "sm") == 0) || (strcmp (bootDev, "bp") == 0))
			{
				/* Has a shared memory IP address been set? */
				lenIPadr = strspn(&pBootParams->bad[0], 
						"0123456789abcdefABCDEF.:");
				pIpType = "backplane";
			}
			else
			{
				/* Has an ethernet IP address been set? */
				lenIPadr = strspn(&pBootParams->ead[0], 
						"0123456789abcdefABCDEF.:");
				pIpType = "ethernet";
			}
			if (lenIPadr < 7)
			{
				bootAppPrintf ("\n\"inet on %s\" boot parameter not set.\n",
						(int)pIpType,0,0,0,0,0);
				return (ERROR);
			}
			/* Has a host address been set? */
			lenIPadr = strspn(&pBootParams->had[0], "0123456789abcdefABCDEF.:");
			if (lenIPadr < 7)
			{
				bootAppPrintf ("\n\"host inet\" boot parameter not set.\n",
						0,0,0,0,0,0);
				return (ERROR);
			}
			/* Has a file name been set? */
			lenFileName = strlen(&pBootParams->bootFile[0]);
			if (lenFileName < 1)
			{
				bootAppPrintf ("\n\"file name\" boot parameter not set.\n",
						0,0,0,0,0,0);
				return (ERROR);
			}
		}
		else if ((pBootParams->flags & SYSFLG_AUTOFILE) != SYSFLG_AUTOFILE)
		{
			/* Has a host address been set? */
			lenIPadr = strspn(&pBootParams->had[0], "0123456789abcdefABCDEF.:");
			if (lenIPadr < 7)
			{
				bootAppPrintf ("\n\"host inet\" boot parameter not set.\n",
						0,0,0,0,0,0);
				return (ERROR);
			}
			/* Has a file name been set? */
			lenFileName = strlen(&pBootParams->bootFile[0]);
			if (lenFileName < 1)
			{
				bootAppPrintf ("\n\"file name\" boot parameter not set.\n",
						0,0,0,0,0,0);
				return (ERROR);
			}
		}
	}
	return (OK);
}

/*******************************************************************************
 *
 * bootLoad - Load a VxWorks image file into memory
 *
 * This routine loads the VxWorks image file specified by the boot command 
 * string into memory.  This routine updates the network configuration with 
 * the new boot parameters, displays the boot parameters, checks the boot 
 * parameters for errors, and finds the appropriate file load handler to load 
 * the file.
 *
 * RETURNS: 
 *   OK if the file was loaded
 *   ERROR if errors are found in the boot line parameters
 *   ERROR if none of the load handlers loaded the file
 *
 * ERRNO: N/A
 *
 */

STATUS bootLoad
(
		char *   bootString,    /* boot command string */
		FUNCPTR *pEntry         /* starting executable address */
)
{
	STATUS      status = OK;
	BOOT_PARAMS tBootParams;
	struct bootLoaderInfoStruct * currentLoader;
	/* copy bootString to low mem address, if specified */

	if ((bootString != NULL) && (*bootString != EOS))
	{
		BOOT_PARAMS oldBootParams;

		if (bootNetSettingsUpdateFunc != NULL)
		{
			/* save old network settings */
			memcpy(&oldBootParams, &sysBootParams, sizeof(BOOT_PARAMS)); 
		}

		/* interpret boot command */

		if (usrBootLineCrack (bootString, &tBootParams) != OK)
			return (ERROR);

		/* update network settings */
		if (bootNetSettingsUpdateFunc != NULL)
		{
			/* networking enabled, setup network settings */
			status = bootNetSettingsUpdateFunc(&oldBootParams, &tBootParams); 
			if (status == ERROR)
				return (status);
		}
	}
	else
	{
		/* interpret boot command */

		if (usrBootLineCrack (sysBootLine, &tBootParams) != OK)
			return (ERROR);
		bootString = sysBootLine;
	}

	/* Display boot parameters */

	bootParamsShow (bootString);

	/* validate bootline parameters */

	if (bootParamCheck(&tBootParams) != OK)
		return (ERROR);

	    /* 
	    * No errors found in the boot string parameters, so copy the these
	    * boot parameters to the system boot parameters. 
	    */
	
	    memcpy (&sysBootParams, &tBootParams, sizeof(BOOT_PARAMS));
	//    strcpy (sysBootLine, bootString);

	/* set our processor number: may establish vme access, etc. */
	
    sysFlags = sysBootParams.flags;
    sysProcNumSet (sysBootParams.procNum);

	status = BOOT_STATUS_CONTINUE;  /* boot device not found yet */

	/* loop through the loaders until boot device is found */
	currentLoader = firstLoader;
	while ((currentLoader != NULL) &&
			(status == BOOT_STATUS_CONTINUE))
	{
		status = currentLoader->loadHandler(bootString, pEntry);

		/* check next command */
		currentLoader = currentLoader->next;
	}

	if (status == BOOT_STATUS_CONTINUE)
	{
		bootAppPrintf ("\nError loading file: no valid boot device found.\n",
				0,0,0,0,0,0);
		status = ERROR;
	}
	return (status);
}

/******************************************************************************
 *
 * bootAppBootFileLoadAndGo - Load a VxWorks image file and execute it
 * 
 * This routine calls bootLoad() to load the VxWorks image file specified by 
 * the command string into memory and executes it.  This routine does not 
 * return.  If an error occurs during the loading of the file, then the target 
 * reboots.
 *
 * RETURNS: N/A
 *
 * ERRNO: N/A
 *
 */

int bootAppBootFileLoadAndGo
(
		char * cmd  /* pointer to the command string */
)
{
	FUNCPTR entry;
	char * bootLine;
    BOOT_PARAMS bp;
    char *pchar;

	bootLine = (char *) (NV_RAM_ADRS+NV_PAREM_OFFSET +NV_BOOT_OFFSET_A_DFT); 
	strcpy (sysBootLine,bootLine);    // add by sjy 2013.06.18    	 

	
#if 1   // add by sjy 2013.07.22
//		strcpy (BOOT_LINE_ADRS,bootLine); 
		(void) bootStringToStruct (sysBootLine, &bp);
		if(bp.ead!=NULL)
		{
			pchar = bp.ead;
			while ((*pchar!=EOS)&&(*pchar!=':')&&((pchar-bp.ead)<50))
				pchar++;
			pchar--;
			*pchar += sysGetCpuID();
			(void) bootStructToString (sysBootLine, &bp);
			printf("%s\n",sysBootLine);
		}
#endif  
	
	/* skip the command name and point to first parameter */
//	bootCmdNameSkip (&cmd, 1);

	if (bootLoad (sysBootLine, &entry) == OK)
	{
		go (entry);
	}
	else
	{
		taskDelay (sysClkRateGet ());   /* pause a second */
		reboot (BOOT_NO_AUTOBOOT);      /* something is awry */
	}

	return BOOT_STATUS_COMPLETE;
}

/*******************************************************************************
 *
 * go - Start execution at the specified address
 *
 * This routine causes execution of the VxWorks image to start at the specified 
 * address, and does not return.
 *
 * RETURNS: N/A
 *
 * ERRNO: N/A
 *
 */

void go
(
		FUNCPTR entry
)
{
	bootAppPrintf ("Starting at 0x%x...\n\n", (int) entry,0,0,0,0,0);

	taskDelay (sysClkRateGet ());   /* give the network a moment to close */

	/* Stop all ENDs to restore to known state for interrupts and DMA */

	if (bootNetDevStopAllFunc != NULL)
		(void) bootNetDevStopAllFunc (0);

	/* clear the cache before running */
	cacheTextUpdate ((void *) (bootMemBase),   
			(size_t) (sysMemTop() - bootMemBase));

#if (CPU_FAMILY == I80X86)
	sysClkDisable ();           /* disable the system clock interrupt */
	sysIntLock ();          /* lock the used/owned interrupts */
#   if defined (SYMMETRIC_IO_MODE) || defined (VIRTUAL_WIRE_MODE)
	{
		extern void loApicEnable ();
		loApicEnable (FALSE);       /* disable the LOAPIC */
	}
#       if defined (SYMMETRIC_IO_MODE)
	{
		extern BOOL sysBp;          /* TRUE for BP, FALSE for AP */
		extern void ioApicEnable ();
		if (sysBp)
			ioApicEnable (FALSE);       /* disable the IO APIC */
	}
#       endif /* (SYMMETRIC_IO_MODE) */
#   endif /* (SYMMETRIC_IO_MODE) || (VIRTUAL_WIRE_MODE) */
#endif  /* (CPU_FAMILY == I80X86) */

	/* Lock interrupts before jumping out of boot image. The interrupts
	 * enabled during system initialization in sysHwInit()
	 */

	intCpuLock();

	/*
	 * attempt callback to BSP to deal with setting up any boot params. 
	 */
	if (_func_SysBootParamsGet != NULL)
		_func_SysBootParamsGet(entry);

	/* disable the MMU */
	if (bootDisableMmuBeforeEntry == TRUE)
		_WRS_BOOT_DISABLE_MMU;

	/* go to entry point - never to return */
	if (bootGoOverrideFunc != NULL)
		/* user defined Go routine */
		bootGoOverrideFunc(entry, sysStartType);
	else
		/* default - jump to image */
		(entry) (sysStartType);     /* go to entry point - never to return */
}

