/* usrRtpAppInitString.c - Launch RTP's by parsing a character string */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,15nov04,yvp  Defined pRtpList as a LOCAL array to avoid placing the list
                 in the read-only data section (SPR 103746).
01a,07may04,yvp  written by extracting from stubRtpInit.c.
*/

/*
DESCRIPTION
This file is used to launch a user-defined list of RTP's encoded into a string.
The string format is described in rtpLaunch.c. There is no limit on the length
of the string. 
*/ 

/* includes */

#include "rtpLaunch.c"	/* include the default RTP launch string parser */

/* locals */

#ifndef RTP_APPL_INIT_STRING
#define RTP_APPL_INIT_STRING ""
#endif

LOCAL char pRtpList [] = RTP_APPL_INIT_STRING;


/******************************************************************************
*
* usrRtpAppInitString - Launch a set of RTP's from a formatted text string 
*
* This routine launches one or more RTP's specified in the form of a formatted
* text string. The default parser for the input string is rtpLaunchFromList().
* 
* RETURNS: N/A.
*
* ERRNO:   N/A.
*
* SEE ALSO: rtpLaunchFromList(), rtpSpawn().
* .pG "Basic OS"
*
*/

void usrRtpAppInitString (void)
    {
    rtpLaunchFromList (pRtpList);
    }
