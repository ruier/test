/* bootAppShell.c - Boot loader application command shell component */

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
01o,30jul09,pgh  Expand number of devices
01n,13jul09,pgh  Fix return value for bootNetSettingsUpdate()
01m,27apr09,pgh  Update documentation.
01l,21aug08,pgh  Fix WIND00130193.
01k,18jun08,pgh  Add support for SATA.
01j,15jan08,pgh  Fix help formatting.
01i,23oct07,h_k  updated Copyright info in printBootLogo(). (CQ:109120)
01h,23apr07,jmt  Defect 93088 - networking not setup correctly for some
                 bootline updates
01g,02mar07,jmt  Move devs command to bootFsLoad.c
01f,23feb07,jmt  Add devs command
01e,06feb07,jmt  Add bootAppPrintf to optimize size
01d,25jan07,jmt  Update network settings after bootline update.
01c,14aug06,jmt  Fix problem with bootAppGo parameter parsing
                 Move LoadAndGo to bootApp.c
01b,20jul06,jmt  Add banner print to shell init function
                 Modify LoadAndGo command to use loose matching.
                 vxTestV2 uses loose version for testing.
01a,07jul05,jmt  written
*/

/*
DESCRIPTION
This module contains routines to implement the VxWorks boot loader application 
command shell component.  This module is added to the boot loader application 
when the INCLUDE_BOOT_SHELL component is included.  This module consists of a
component intialization routine, which adds help display routines and 
command handlers to the boot loader application.  This module implements the 
boot shell, which processes user command line inputs.  In addition, it contains 
the routines that implement the following features:

\is
\i Command help
Allows other boot loader application components to add command help strings.
\i Device help
Allows other boot loader application components to add device format help 
strings.
\i Boot flag help
Allows other boot loader application components to add boot flag help strings.
\i List devices
Allows other boot loader application components to add to the list of devices 
that are displayed.
\i Text display
Provides formatted text display, with limited arguments, to the boot shell and 
boot loader application components.
\ie

The bootCommandHandlerAdd() routine is used to add the following commands to
the boot shell:

\is
\i Help
The '?' and 'h' commands display all of the available command, their formats, and help on the various devices and boot flags.
\i Load and go
The '@' command loads the file specified by the boot line parameters, and 
executes it.
\i Print parameters
The 'p' command displays the current values of the boot line parameters.
\i Change parameters
The 'c' command prompts the user for new values for each of the boot line 
parameters.
\i Load file
The 'l' command loads the file specified by the boot line parameters.
\i Execute at address
The 'g' command starts execution at the specified address.
\i Print exception
The 'e' command displays information on exceptions.
\i Version
The 'v' command displays the version information for the boot loader 
application.
\ie

This module also uses the bootDevFormatHelpAdd() routine to add help strings 
for device formats, and the bootFlagHelpAdd() routine to add help strings for
the boot flags.

INCLUDE FILES: bootApp.h vxWorks.h 
*/ 

/* includes */

#include <vxWorks.h>
#include <bootLib.h>
#include <ctype.h>
#include <fioLib.h>
#include <ioLib.h>
#include <rebootLib.h>
#include <selectLib.h>
#include <stdio.h> 
#include <string.h>
#include <sysLib.h>
#include <taskLib.h>
#include <tickLib.h>

#include <bootApp.h>
#include <private/bootAppShell.h>
#include <private/bootNetLoad.h>

/* defines */

#define MAX_HELP_ENTRIES            50      /* maximum number of help entries */
#define HELP_SIZE                   ((MAX_HELP_ENTRIES+1) * 2 * sizeof (char *))
#define MAX_DEV_FORMAT_HELP_ENTRIES 20      /* maximum number of Device Format help entries */
#define HELP_DEV_FORMAT_SIZE        ((MAX_DEV_FORMAT_HELP_ENTRIES+1) * sizeof (char *))
#define MAX_FLAGS_HELP_ENTRIES      32      /* maximum number of flag help entries */
#define HELP_FLAG_SIZE              ((MAX_FLAGS_HELP_ENTRIES+1) * sizeof (char *))
#define MAX_PRINT_DEVS_ENTRIES      50      /* maximum number of Print devices help entries */
#define MAX_PRINT_DEVS_SIZE         (MAX_PRINT_DEVS_ENTRIES * sizeof (bootVoidPrototype *))

/* globals */

extern int sysStartType;
extern bootPrintHeaderPrototype * bootAppPrintHeaderFunc;
extern bootShellAutobootPrototype * bootShellAutobootFunc;
extern bootShellProcPrototype * bootAppShellFunc;
extern bootShellHelpAddPrototype * bootShellHelpAddFunc;
extern bootShellDevFormatHelpAddPrototype * bootShellDevFormatHelpAddFunc;
extern bootShellFlagHelpAddPrototype * bootShellFlagHelpAddFunc;
extern bootShellPrintfPrototype * bootShellPrintfFunc;
extern bootShellPrintDevsAddPrototype * bootShellPrintDevAddFunc;
extern bootNetSettingsUpdatePrototype * bootNetSettingsUpdateFunc;

extern int bootAppBootFileLoadAndGo (char * cmd);
extern STATUS usrBootLineCrack (char * bootString, BOOT_PARAMS * pParams);
extern int bootAppTaskId;
extern BOOL printBootShellPrompt;

extern char * sysBoard (UINT8 flag);
/* locals */

static char bootShellKey = 0;

static char * helpMsg[MAX_HELP_ENTRIES+1];
static char * helpDevFormatMsg[MAX_DEV_FORMAT_HELP_ENTRIES+1];
static char * helpFlagMsg[MAX_FLAGS_HELP_ENTRIES+1];
static bootVoidPrototype * helpPrintDevsFuncs[MAX_PRINT_DEVS_ENTRIES];
int    numberOfHelpEntries;
int    numberOfDevFormatHelpEntries;
int    numberOfPrintDevsEntries;
static fd_set bootReadFds;
static struct timeval bootSelectTimeout;
 
/* forward declarations */

LOCAL char * bootAppShell (char * cmd, int length);
LOCAL STATUS bootAppPrintHeader (int startType);
LOCAL void bootAppShellAutoboot (int timeout);
LOCAL void bootAppShellHelpAdd (char * helpProto, char * helpDesc);
LOCAL void bootAppShellDevFormatHelpAdd (char * helpDesc);
LOCAL void bootAppShellFlagHelpAdd (UINT32 flag, char * helpDesc);
LOCAL void bootAppShellPrintf(char * x, int a, int b, int c, int d, int e, int f);
LOCAL void bootAppShellPrintDevsAdd(bootVoidPrototype * pPrintDevsFunc);

LOCAL void bootAppExcMsgPrint (char *string);
LOCAL int printBootLogo (char * cmd);
LOCAL char autoboot (int timeout);

/* local command handlers */

LOCAL int printBootLogo (char * cmd);
LOCAL int bootHelp (char * cmd);
LOCAL int bootAppSysExcMsgPrint (char * cmd);
LOCAL int bootAppParamsShow (char * cmd);
LOCAL int bootAppParamsChange (char * cmd);
LOCAL int bootAppBootFileLoad (char * cmd);
LOCAL int bootAppGo (char * cmd);

/*******************************************************************************
*
* bootAppShellInit - Initializes the boot shell component
*
* This routine initializes the boot shell variables, and adds the standard set 
* of boot shell commands to the command handler.  In addition, it initializes 
* the boot shell device and boot flag help strings.
*
* RETURNS: OK
*
* ERRNO: N/A
*
* \NOMANUAL
*/
STATUS bootAppShellInit()
    {
	extern int sysStartType;
	
	/* initialize help message area */
	bfill((char *) helpMsg, HELP_SIZE, 0);
	bfill((char *) helpDevFormatMsg, HELP_DEV_FORMAT_SIZE, 0);
	bfill((char *) helpFlagMsg, HELP_FLAG_SIZE, 0);
	bfill((char *) helpPrintDevsFuncs, MAX_PRINT_DEVS_SIZE, 0);
	
	/* Initialize variables */
	numberOfHelpEntries = 0;
	numberOfDevFormatHelpEntries = 0;
	numberOfPrintDevsEntries = 0;
	
    /* initialize bootReadFds for select */
    FD_ZERO(&bootReadFds);
    FD_SET(STD_IN, &bootReadFds);
    bootSelectTimeout.tv_sec = 0;
    bootSelectTimeout.tv_usec = 100;

	/* initialize processing routine pointer */
	bootAppShellFunc = (bootShellProcPrototype *) bootAppShell;
	bootAppPrintHeaderFunc = (bootPrintHeaderPrototype *) bootAppPrintHeader;
	bootShellAutobootFunc = (bootShellAutobootPrototype *) bootAppShellAutoboot;
	bootShellHelpAddFunc = (bootShellHelpAddPrototype *) bootAppShellHelpAdd;
	bootShellDevFormatHelpAddFunc = 
	    (bootShellDevFormatHelpAddPrototype *) bootAppShellDevFormatHelpAdd;
	bootShellFlagHelpAddFunc = 
		(bootShellFlagHelpAddPrototype *) bootAppShellFlagHelpAdd;
	bootShellPrintfFunc = (bootShellPrintfPrototype *) bootAppShellPrintf;
    bootShellPrintDevAddFunc = 
        (bootShellPrintDevsAddPrototype *) bootAppShellPrintDevsAdd;

    /* initialize commands */
    bootCommandHandlerAdd("?", bootHelp, BOOT_CMD_MATCH_STRICT, 
 		"?", "- print this list");
    bootCommandHandlerAdd("h", bootHelp, BOOT_CMD_MATCH_STRICT, 
 		NULL, NULL);
    /* "@" already registered */
    bootShellHelpAddFunc("@",  "- boot (load and go)"); 
    bootCommandHandlerAdd("p", bootAppParamsShow, BOOT_CMD_MATCH_STRICT,
 		"p",  "- print boot params");
    bootCommandHandlerAdd("c", bootAppParamsChange, BOOT_CMD_MATCH_STRICT,
 		"c",  "- change boot params");
    bootCommandHandlerAdd("l", bootAppBootFileLoad, BOOT_CMD_MATCH_STRICT,
 		"l",  "- load boot file");
    bootCommandHandlerAdd("g", bootAppGo, BOOT_CMD_MATCH_STRICT,
 		"g adrs",  "- go to adrs");
    bootCommandHandlerAdd("e", bootAppSysExcMsgPrint, BOOT_CMD_MATCH_STRICT,
 		"e", "- print fatal exception");
    bootCommandHandlerAdd("v", printBootLogo, BOOT_CMD_MATCH_STRICT,
 		"v", "- print boot logo with version");
    bootCommandHandlerAdd("$", bootAppBootFileLoadAndGo, BOOT_CMD_MATCH_LOOSE,
 		NULL,NULL);
    

    /* add bootline help */
    bootDevFormatHelpAdd (
		"\nBootline Format:\n"  
		"  $dev(0,procnum)host:/file h=# e=# b=# g=# u=usr [pw=passwd] f=#"  
 		"\n                          tn=targetname s=script o=other"    
		"\n\nFile System Boot Device Formats:");
        
	/* initialize flag help */
	/* Flag 0x01 not currently used 
     * bootFlagHelpAdd(0x01, "  don't be system controller");
     */
	bootFlagHelpAdd(0x02, "  load local system symbols");
	bootFlagHelpAdd(0x04, "  don't autoboot");
	bootFlagHelpAdd(0x08, "  quick autoboot (no countdown)");
	
	/* print header */
    bootAppPrintHeader (sysStartType);

    return (OK);
    }
    
/*******************************************************************************
*
* bootAppPrintHeader - Prints the header for the boot loader application
*
* This routine prints the header for the boot loader application on a cold 
* boot, then it displays any new system exception messages.
*
* RETURNS: OK
*
* ERRNO: N/A
*
* \NOMANUAL
*/
LOCAL STATUS bootAppPrintHeader
	(
	int startType		/* boot start type for system */
	)
    {
	/* only print Boot logo during cold boot */	
    if (startType & BOOT_CLEAR)
       printBootLogo (NULL);

     /* 
     * print out any new exception message.  The first byte is zeroed after 
     * printing so that we won't print it again automatically.  However, the
     * 'e' command will still print out the remainder. 
     */

    bootAppExcMsgPrint (sysExcMsg);
    *sysExcMsg = EOS;       /* indicate exception message is old */

    return (OK);
    }


/*******************************************************************************
*
* bootAppShell - Boot shell command line processing
*
* This routine implements the boot shell command line processing.  It prompts 
* the user for a command input, then reads the text input by the user, and
* returns a pointer to the users input string.
*
* RETURNS: The input command string
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL char * bootAppShell
	(
	char * line,    /* command line input buffer */
	int    length   /* length of input buffer */
	)
    {
    char *pLine;
	
	if (bootShellKey == '@')
		{
		line [0] = bootShellKey;
		line [1] = EOS;
		bootShellKey = 0;
		}
	else
		{

    /* check for valid bootReadFds */
    if (FD_ISSET(STD_IN, &bootReadFds) == 0)
        {
        /* STD_IN is not part of set, add it. */
        FD_ZERO(&bootReadFds);
        FD_SET(STD_IN, &bootReadFds);
        }

        if (select(FD_SETSIZE, &bootReadFds, NULL, NULL, &bootSelectTimeout) > 0)
            {
            fioRdString (STD_IN, line, length);

            /* setup for new shell prompt.
             * In case a return was hit without a command
             */
            printBootShellPrompt = TRUE;
            }
        else
            {
            line[0] = EOS;
            }
		}

	/* take blanks off end of line */

    if (strlen(line) > 0)
        {
	pLine = line + strlen (line) - 1;       /* point at last char */
	while ((pLine >= line) && (*pLine == ' '))
		{
		*pLine = EOS;
		pLine--;
		}
        }

	/* take blanks off beginning of line */
	
	pLine = line;
	bootSpaceSkip (&pLine);

    return pLine;
    }

/*******************************************************************************
*
* bootAppShellHelpAdd - Add help string for a boot shell command
*
* This routine adds a boot shell command help string to the list of command help
* messages output by the boot shell.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void bootAppShellHelpAdd 
	(
	char * helpProto,  /* command prototype string */
	char * helpDesc    /* help description for command */
	)
	{
	if ((numberOfHelpEntries < MAX_HELP_ENTRIES) && 
		(helpProto != NULL))
		{
			/* save help Prototype and Description */
			helpMsg[2*numberOfHelpEntries] = helpProto;
			helpMsg[2*numberOfHelpEntries + 1] = helpDesc;
			numberOfHelpEntries++;
		}
	}

/*******************************************************************************
*
* bootAppShellDevFormatHelpAdd - Add help string for a device format
*
* This routine adds a device format help string to the list of device format
* help messages output by the boot shell.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void bootAppShellDevFormatHelpAdd 
	(
	char * helpDesc    /* format description for device */
	)
	{
	if ((numberOfDevFormatHelpEntries < MAX_DEV_FORMAT_HELP_ENTRIES) && 
		(helpDesc != NULL))
		{
			/* save Device Format Description */
			helpDevFormatMsg[numberOfDevFormatHelpEntries] = helpDesc;
			numberOfDevFormatHelpEntries++;
		}
	}

/*******************************************************************************
*
* bootAppShellFlagHelpAdd - Add help string for a boot flag
*
* This routine adds a boot flag description string to the list of boot flag
* description strings output by the boot shell.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void bootAppShellFlagHelpAdd 
	(
	UINT32 flag,      /* boot flag */
	char * helpDesc   /* help description for flag */
	)
	{
	int     bit = 0;
	UINT32  tempFlag = flag;

	/* Determine which bit flag this string applies to. */
	while ((tempFlag & 1) == 0)
		{
		tempFlag >>= 1;
		bit++;
		}
	
	/* save help Description */
	helpFlagMsg[bit] = helpDesc;
	}

/*******************************************************************************
*
* bootAppShellPrintDevsAdd - Add a device to the list of supported devices
*
* This routine adds a new device print routine to the list of device print
* routines executed by the boot shell.  This list of routines is executed to 
* display the list of supported devices for the help command.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
LOCAL void bootAppShellPrintDevsAdd
	(
	bootVoidPrototype * pPrintDevsFunc  /* Print device function */
	)
	{
	if (numberOfPrintDevsEntries < MAX_PRINT_DEVS_ENTRIES)
		{
		helpPrintDevsFuncs[numberOfPrintDevsEntries] = pPrintDevsFunc;
		numberOfPrintDevsEntries++;
		}
	}

/*******************************************************************************
*
* bootAppShellPrintf - Boot shell printf routine
*
* This routine executes the printf() routine passing it the specified print
* string, and six variables.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* /NOMANUAL
*/

LOCAL void bootAppShellPrintf
	(
	char * x,   /* print format string */
	int a,      /* first print argument */
	int b,      /* second print argument */
	int c,      /* third print argument */
	int d,      /* fourth print argument */
	int e,      /* fifth print argument */
	int f       /* sixth print argument */
	)
	{
	printf(x, a, b, c, d, e, f);
	}
	
/*******************************************************************************
*
* bootAppExcMsgPrint - Print exception message
*
* This routine is used to display exception messages, and avoids printing 
* possible control characters in exception message area.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void bootAppExcMsgPrint
    (
    char *string
    )
    {
    bootAppPrintf ("\n", 0,0,0,0,0,0);

    while (isascii ((UINT) * string) && (isprint ((UINT) * string) ||
                        isspace ((UINT) * string)))
    bootAppPrintf ("%c", * string++, 0,0,0,0,0);

    bootAppPrintf ("\n", 0,0,0,0,0,0);
    }

/******************************************************************************
*
* printBootLogo - Print initial boot banner page
* 
* This routine prints the initial boot banner for the boot shell.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int printBootLogo 
	(
	char * cmd
	)
    {
    bootAppPrintf ("\n\n\n\n\n\n\n\n\n\n\n",0,0,0,0,0,0);
    bootAppPrintf ("%28s%s",(int)"",(int)"VxWorks System Boot",0,0,0,0);
    bootAppPrintf 
        ("\n\n\nCopyright 1984-2008  Wind River Systems, Inc.\n\n\n\n\n\n",
		 0,0,0,0,0,0);
    bootAppPrintf ("CPU: %s\n", (int) sysModel (),0,0,0,0,0);
    bootAppPrintf ("Version: %s\n", (int) vxWorksVersion,0,0,0,0,0);
    bootAppPrintf ("Board number: %s\n",(int)sysBoard(0), 0,0,0,0,0);
    bootAppPrintf ("ssd status: %s\n",(int)sysBoard(1), 0,0,0,0,0);
    bootAppPrintf ("BSP version: %s\n", (int) sysBspRev (), 0,0,0,0,0);
    bootAppPrintf ("Creation date: %s\n\n", (int) creationDate,0,0,0,0,0);
    return BOOT_STATUS_COMPLETE;
    }

/******************************************************************************
*
* bootAppShellAutoboot - do automatic boot sequence
*
* This routine implements the automatic boot sequence.  It first checks
* if the automatic boot routine has been turned off, and just returns if it 
* has.  It then checks to see if a quick automatic boot has been configured,
* and changes the count down from 7 seconds to 1 if it has been set.  The
* autoboot() routine is then called to execute the countdown followed by
* the image boot.  If the image boot is successful, this routine does not
* return.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL void bootAppShellAutoboot
    (
    int timeout     /* timeout time in seconds */
    )
    {
	BOOT_PARAMS params;
 
 	/* start autoboot, unless no-autoboot specified */

    bootStringToStruct (sysBootLine, &params);
    sysFlags = params.flags;
	
	if (!(sysStartType & BOOT_NO_AUTOBOOT) &&
 		!(sysFlags & SYSFLG_NO_AUTOBOOT))
		{
		if ((sysStartType & BOOT_QUICK_AUTOBOOT) ||
			(sysFlags & SYSFLG_QUICK_AUTOBOOT))
			{
			timeout = 1;
			}
	
		bootShellKey = autoboot (timeout);   /* doesn't return if successful */
		}
		
	/* did not autoboot, return */
    bootAppPrintf ("\n",0,0,0,0,0,0);
	}
	
/******************************************************************************
*
* autoboot - Execute the automatic boot sequence
*
* This routine executes the specified countdown, while looking for character
* input on the console, then boots the image specified by the boot line 
* parameters.  If the image boot is successful, this routine does not
* return.  Otherwise, it returns the character input on the console.
*
* RETURNS: Character input on the console port.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL char autoboot
    (
    int timeout     /* timeout time in seconds */
    )
    {
    int     timeLeft;
    int     bytesRead = 0;
    int     i;
    FUNCPTR entry;
    char    key;

	/*(void) ioctl (STD_IN, FIOBAUDRATE, CONSOLE_BAUD_RATE);*/
	(void) ioctl (STD_IN, FIOSETOPTIONS,
		OPT_ECHO | OPT_CRMOD | OPT_TANDEM | OPT_7_BIT);
 
    if (timeout > 0)
		{
        bootAppPrintf ("\nPress any key to stop auto-boot...\n",0,0,0,0,0,0);
	
		/* Loop looking for a char, or timeout after specified seconds */
	
        for (timeLeft = timeout; (timeLeft > 0) && (bytesRead == 0); timeLeft--)
            {
            bootAppPrintf ("%2d\r", timeLeft,0,0,0,0,0);
            for (i = 10; i > 0; i--)
                {
                (void) ioctl (STD_IN, FIONREAD, (int) &bytesRead);
                if (bytesRead != 0)
                    break;

                taskDelay(sysClkRateGet() / 10);
                }
            }
		}

    if (bytesRead == 0)    /* nothing typed so auto-boot */
		{
		/* put the console back in line mode so it echoes (so's you can bang
		 * on it to see if it's still alive) */
	
		(void) ioctl (STD_IN, FIOSETOPTIONS, OPT_TERMINAL);
	
        bootAppPrintf ("\nauto-booting...\n\n",0,0,0,0,0,0);
	
		if (bootLoad (sysBootLine, &entry) == OK)
			go (entry);             /* ... and never return */
		else
			{
            bootAppPrintf ("Can't load boot file!!\n",0,0,0,0,0,0);
			taskDelay (sysClkRateGet ());   /* pause a second */
			reboot (BOOT_NO_AUTOBOOT);      /* something is awry */
			}
			
		/* should never get here */
		key = (char) ERROR;
		}
    else
		{
		/* read the key that stopped autoboot */
	
		read (STD_IN, &key, 1);
		key = key & 0x7f;        /* mask off parity in raw mode */
		}

	/* put it back into line mode */
    (void) ioctl (STD_IN, FIOSETOPTIONS, OPT_TERMINAL);

    return (key);
    }

/*******************************************************************************
*
* bootHelp - Display the boot shell help
* 
* This routine is the command handler for the '?' command.  This routine
* displays the help strings associated with the boot shell commands, followed 
* by the device format strings associated with the devices, followed by the 
* help strings associated with the boot flags, and finally the device print 
* routines are executed to display information on the devices.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootHelp 
	(
	char * cmd
	)
    {
    FAST char **pMsg;
    int offset;
    UINT32 flag;


	/* print command help */
    bootAppPrintf ("\nCommands:\n",0,0,0,0,0,0);
    for (pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
 		{
         bootAppPrintf (" %-21s %s\n",
						    (int) *pMsg, 
						    (int) *(pMsg + 1),
						    0,0,0,0);
 		}

	/* print Device Format help */
    for (pMsg = helpDevFormatMsg; *pMsg != NULL; pMsg += 1)
 		{
         bootAppPrintf (" %s\n",
                       (int) *pMsg, 
                       0,0,0,0,0);
 		}

	/* print boot flags */
    bootAppPrintf("\nBoot Flags:\n",0,0,0,0,0,0);
	flag = 1;
    for (offset = 0; offset < MAX_FLAGS_HELP_ENTRIES; offset++, flag <<= 1)
 		{
 		if (helpFlagMsg[offset] != NULL)
            bootAppPrintf ("  0x%4.4x - %s\n", 
							    (int) flag, 
							    (int) helpFlagMsg[offset],
							    0,0,0,0);
 		}

	/* print the devices */
    
    bootAppPrintf ("\nAvailable Boot Devices:",0,0,0,0,0,0);
   	for (offset = 0; offset < numberOfPrintDevsEntries; offset++)
		{
		/* call print devices function */
		if (helpPrintDevsFuncs[offset] != NULL)
			helpPrintDevsFuncs[offset]();
		}

    bootAppPrintf ("\n",0,0,0,0,0,0);
    return BOOT_STATUS_COMPLETE;
    }

/******************************************************************************
*
* bootAppSysExcMsgPrint - Display system exception message
* 
* This routine is the command handler for the 'e' boot shell command.  It
* displays the exception message from the system exception message string.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppSysExcMsgPrint 
	(
	char * cmd
	)
	{
	bootAppExcMsgPrint(sysExcMsg+1);
	return BOOT_STATUS_COMPLETE;
	}

/******************************************************************************
*
* bootAppParamsShow - Print boot parameters
* 
* This routine is the command handler for the 'p' boot shell command.  It
* displays the current settings for each of the boot parameters.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppParamsShow 
	(
	char * cmd
	)
	{
	bootParamsShow (sysBootLine);
	return BOOT_STATUS_COMPLETE;
	}

/******************************************************************************
*
* bootAppParamsChange - Change boot parameters
* 
* This routine is the command handler for the 'c' boot shell command.  It
* loops through each boot parameter, displaying the current settings for the
* parameter, and waiting a new value to be input before moving to the next
* boot parameter.  Once all of the boot parameters have been processed, the
* new boot parameters are written to NVRAM, and the network configuration is
* updated with the changes.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppParamsChange
	(
	char * cmd
	)
	{
    BOOT_PARAMS oldBootParams;
    
    bootParamsPrompt (sysBootLine);
    if (strlen(sysBootLine) <= BOOT_CMD_MAX_LINE)
        {
        if ((sysNvRamSet (sysBootLine,
            strlen (sysBootLine) + 1, 0)) == ERROR)
            {
            bootAppPrintf("\nNOTE: Bootline not saved to NVRAM\n",0,0,0,0,0,0);
            }
            
        if (bootNetSettingsUpdateFunc != NULL)
            {
            /* save old network settings */
            memcpy(&oldBootParams, &sysBootParams, sizeof(BOOT_PARAMS)); 
            }
            
        if (usrBootLineCrack(sysBootLine, &sysBootParams) == ERROR)
            {
            bootAppPrintf("\nNOTE: Error parsing Bootline.  Settings may be corrupted.\n",0,0,0,0,0,0);
            return BOOT_STATUS_COMPLETE;
            }
            
        /* update network settings */
        if (bootNetSettingsUpdateFunc != NULL)
            {
            /* networking enabled, setup network settings */
            (void) bootNetSettingsUpdateFunc(&oldBootParams, &sysBootParams); 
            }
        }
    else
        bootAppPrintf("\nWARNING:Bootline longer than BOOT_LINE_SIZE "
            "(%d bytes). Not saved to NVRAM\n",
            (int) BOOT_CMD_MAX_LINE,0,0,0,0,0);

    return BOOT_STATUS_COMPLETE;
    }
        
/******************************************************************************
*
* bootAppBootFileLoad - Load boot file
* 
* This routine is the command handler for the 'l' boot shell command.  It 
* loads the file the file specified by the boot parameters into memory.  If
* the load operation fails, this routine delays for one second, then reboots
* with the system start type set to disable the auto boot routine.  If the
* file is loaded successfully, then this routine returns normally.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppBootFileLoad
	(
	char * cmd
	)
	{
    FUNCPTR entry;

	/* skip the command name and point to first parameter */
	bootCmdNameSkip (&cmd, 1);

 	if (bootLoad (cmd, &entry) == OK)
		{
        bootAppPrintf ("entry = 0x%x\n", (int) entry,0,0,0,0,0);
		}
	else
		{
		taskDelay (sysClkRateGet ());   /* pause a second */
		reboot (BOOT_NO_AUTOBOOT);      /* something is awry */
		}
 
  	return BOOT_STATUS_COMPLETE;
	}

/******************************************************************************
*
* bootAppGo - Go to Address
* 
* This routine is the command handler for the 'g' boot shell command.  It 
* causes execution to begin at the address specified on the command line.
*
* RETURNS: BOOT_STATUS_COMPLETE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL int bootAppGo
	(
	char * cmd
	)
	{
    FUNCPTR entry;
	int entryAddr;

    /* skip the command name and point to first parameter */
    bootCmdNameSkip (&cmd, 1);

	if (getArg (&cmd, &entryAddr, HEX, !OPT) == OK)
		{
		entry = (FUNCPTR)entryAddr;
		go (entry);
		}
  
  	return BOOT_STATUS_COMPLETE;
	}

