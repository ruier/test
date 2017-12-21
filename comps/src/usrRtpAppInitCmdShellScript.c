/* usrRtpAppInitCmdShellScript.c - Launch a command shell script */

/*
 * Copyright (c) 2004, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable Wind
 * River license agreement.
 */

/*
modification history
--------------------
01b,20aug09,rlp  Fixed a typo which prevents to launch a shell session
                 (Defect CQ:WIND00177956).
01a,17may04,yvp  written.
*/

/*
DESCRIPTION
This file is used to launch a user-defined command shell script. The script
is parsed and executed by the command shell. This component depends on the
INCLUDE_SHELL and INCLUDE_SHELL_INTERP_CMD components. 

The user has an option not to automatically start the shell task when the
system boots (i.e. it will be launched by this component when it launches).
This can be done by setting the shell parameter SHELL_START_AT_BOOT to 
FALSE in your project. 
*/ 

#define SHELL_TASK_NAME "tShellScript"


/******************************************************************************
*
* usrRtpAppInitCmdShellScriptRun - Run a command shell script
*
* This routine executes the given command shell script via the shell.
* 
* RETURNS: N/A.
*
* ERRNO:   N/A.
*
* SEE ALSO: N/A.
* .pG "Basic OS"
*
*/

void usrRtpAppInitCmdShellScriptRun (char * script)
    {
    int fd;

    if (script == NULL || script[0] == EOS)
	return;	/* no valid file name */

    if (((fd = open (script, O_RDONLY, 0))) == ERROR)
	{
        printErr ("Error: unable to open startup script '%s'.\n", script);
        return;
        }

    printf ("Executing startup script '%s'...\n", script);

    /* Execute script */

    if (shellGenericInit (SHELL_CFG_INTERP"=Cmd", SHELL_STACK_SIZE, 
			  SHELL_TASK_NAME, NULL, FALSE, FALSE,
			  fd, ioGlobalStdGet (STD_OUT),
			  ioGlobalStdGet (STD_ERR)) != OK)
	{
        printErr ("Error: unable to execute startup script '%s'.\n", script);
        return;
	}

    /* Wait for shell to finish */

    do
	taskDelay (sysClkRateGet ());
    while (taskNameToId (SHELL_TASK_NAME) != ERROR);

    close (fd);

    printf ("\nDone executing startup script '%s'.\n", script);
    }



