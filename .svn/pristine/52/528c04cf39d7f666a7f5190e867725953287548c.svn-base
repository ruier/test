/* usrRtpAppInitBootline.c - Launch RTP's by parsing a character string */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,07may04,yvp  written by extracting from stubRtpInit.c.
*/

/*
DESCRIPTION
This file is used to launch a user-defined list of RTP's encoded into a string.
The string is extracted from the Startup Script field of the bootline. The
string format is described in rtpLaunch.c. The total length of the string is
limited to 160 characters, and must contain no white space characters.
*/ 

/* includes */

#include "rtpLaunch.c"	/* include the default RTP launch string parser */

/* externals */

extern char *      pRtpStartupField;
extern char *      startupScriptFieldSplit (char *);
extern BOOT_PARAMS sysBootParams;


/******************************************************************************
*
* usrRtpAppInitBootline - Launch a set of RTP's from a bootline field.
*
* This routine launches one or more RTP's specified in the form of a formatted
* text string taken from the Startup Script field of the VxWorks boot line.
* 
* RETURNS: N/A.
*
* ERRNO:   N/A.
*
* SEE ALSO: rtpLaunchFromList(), rtpSpawn().
* .pG "Basic OS"
*
*/

void usrRtpAppInitBootline (void)
    {
    /* user code may be added before or after the following 2 lines */

    startupScriptFieldSplit (sysBootParams.startupScript);
    rtpLaunchFromList (pRtpStartupField);
    }
