/* rtpLaunch.c - Launch RTP's by parsing a character string */

/*
 * Copyright (c) 2004, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,28may10,jpb  Updated for LP64 adaptation.
01c,22sep04,yvp  Fixed GNU compiler warnings (SPR 100594).
01b,17may04,yvp  Removed debug code, cleaned up documentation.
01a,07may04,yvp  written by extracting from stubRtpInit.c.
*/

/*
DESCRIPTION
This file implements the rtpLaunchFromList() to parse a character string for
a certain format encoding of a list of RTP's to be launched on system startup.
*/ 

#ifndef __INCrtpLaunchC
#define __INCrtpLaunchC

/* includes */

#include <vxWorks.h>
#include <bootLib.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rtpLib.h>
#include <rtpLibCommon.h>
#include <taskLib.h>

/* defines */

#define ARGS_MAX   (BOOT_FILE_LEN/2)   /* Max number of arguments to the RTP */
 
#define DEFAULT_RTP_TASK_PRIORITY	220
#define DEFAULT_RTP_TASK_STACKSZ	(64*1024)
#define DEFAULT_RTP_OPTIONS		0
#define DEFAULT_INIT_TASK_OPTIONS	VX_FP_TASK

/******************************************************************************
*
* rtpLaunchFromList - Launch a set of RTP's from a formatted text string 
*
* This routine launches one or more RTP's specified in the form of a formatted
* text string. A '#' character is used as a delimiter for each new RTP to 
* launch. Within an RTP command, arguments are delimited by either '^' 
* characters or a space character. 
* 
* This function is used as the parser for the INCLUDE_RTP_APPL_INIT_STRING
* and INCLUDE_RTP_APPL_INIT_BOOTLINE components. For the latter component,
* a space should *NOT* be used to delimit RTP arguments or options.
* 
* A sample RTP invocation line may look like this:
*
* EXAMPLE:
* .CS
*	#/romfs/rtp1.vxe#/romfs/rtp2.vxe^arg1^arg2#/romfs/rtp3.vxe^arg3
* .CE
* 
* The above string will launch 3 RTP's. /romfs/rtp1.vxe will be launched 
* without any arguments. /romfs/rtp2.vxe will be launched with arguments 
* arg1 and arg2, while /romfs/rtp3.vxe will be launched with a single 
* argument arg3. 
* 
* RETURNS: N/A.
*
* ERRNO:   N/A.
*
* SEE ALSO: rtpSpawn().
* .pG "Basic OS"
*
*/

void rtpLaunchFromList (char * list)
    {
    char * tok;
    const char * argv [ARGS_MAX];
    char * cmds [ARGS_MAX];
    char * s;
    char   c;
    int    argc = 0;
    int    numCmds = 0;
    int    i;
    int    total;
    int    base;
    int    initTaskPrio;
    size_t initTaskStackSz;
    int    initTaskOptions;
    int    rtpOptions;
    RTP_ID rtpId;
    
    if (list == NULL)
	return;
    
    bzero ((char *)argv, ARGS_MAX  *sizeof (char *));
    
    while (((c = *list) == '#') || isspace((unsigned int)c))
	list++;        /* skip over the first # character and spaces */

    tok = strtok (list, "#");
    while (tok != NULL)
	{
	cmds [numCmds++] = tok;
	tok = strtok (NULL, "#");
	}

    for (i = 0; i < numCmds; i++)
	{
	initTaskPrio = DEFAULT_RTP_TASK_PRIORITY;
	initTaskStackSz = DEFAULT_RTP_TASK_STACKSZ;
	initTaskOptions = DEFAULT_INIT_TASK_OPTIONS;
	rtpOptions = DEFAULT_RTP_OPTIONS;

	tok = strtok (cmds[i], "^ ");
	while (tok != NULL)
	    {
	    if ((tok[0] == '%') && (tok[2] == '='))
                {
		total = 0;

		if ((strncmp(&tok[3], "0x", 2) == 0) || 
		    (strncmp(&tok[3], "0X", 2) == 0))
		    {
		    base = 16;
		    s = &tok[5];
		    }
                else
		    {
		    base = 10;
		    s = &tok[3];
		    }

	        while ((c = tolower ((unsigned int) *s++)))
		    {
		    if (isdigit ((unsigned int) c) || 
			(base == 16 && isxdigit ((unsigned int) c)))
		        {
			total *= base;
			if (base == 10)
			    total += c - '0';
			else
			    total += (c > '9') ? (10 + (c - 'a')) :
			             (c - '0');
			}
		    else
			{
			printf ("invalid number %s - ignored\n", tok+3);
			goto out;
			}
		    }

		   switch (tok[1])
		       {
		       case 'p':
			   initTaskPrio = total;
			   break;
		       case 's':
			   initTaskStackSz = total;
			   break;
		       case 'o':
			   rtpOptions = total;
			   break;
		       case 't':
			   initTaskOptions = total;
			   break;
		       default:
			   printf ("invalid option %s - ignored\n", tok);
		       }
		   }
	       else
		   argv[argc++] = tok;

out:
	       tok = strtok (NULL, "^ ");
	       }

	printf ("Spawning RTP: %s \n", argv[0]);
	rtpId = rtpSpawn (argv[0], argv, NULL, initTaskPrio, initTaskStackSz,
			  rtpOptions, initTaskOptions);

	if (rtpId == RTP_ID_ERROR)
	    printf ("Error: Failed to spawn RTP %s   errno = 0x%x\n", 
		    argv[0], errnoGet());

	bzero ((char *)argv, ARGS_MAX * sizeof (char *));
	argc = 0;
	}
    }
#endif /* __INCrtpLaunchC */
