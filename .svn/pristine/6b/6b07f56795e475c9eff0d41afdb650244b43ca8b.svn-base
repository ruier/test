/* usrSmnet.c - shared memory network initialization */

/* 
 * Copyright (c) 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,12sep07,bwa  fixed gnu building error (WIND00104291)
01d,18dec06,pmr  use forward slashes
01c,06nov06,mze  further modified to work with ipnet release
01b,06oct06,kch  Removed the call to usrNetIpAttachCommon() for seperate
                 IPv6 attach.
01a,19sep06,mze  created for WIND00058841 recreating BSP logic for 
                 INCLUDE_SM_NET
*/

/*
DESCRIPTION
This file is used to configure and initialize the VxWorks shared memory
network support.  

SEE ALSO: 

NOMANUAL
*/

#if (SM_TAS_TYPE != SM_TAS_HARD)
#error Error initializing shared memory objects, hardware test-and-set required.
#endif

#include <vxWorks.h>
#include <semLib.h>
#include <taskLib.h>
#include <errnoLib.h>
#include <wrn/coreip/ipProto.h>

/******************************************************************************
*
* usrSmNetInit - initialize shared memory network
*
* This routine initializes the shared memory network facility.  
*
* RETURNS: OK, or ERROR if unsuccessful.
*
* INTERNAL
* The delayed start for slave processors used below is required.  The anchor
* may not yet be mapped to the bus.  So, probing of shared memory locations is
* used to overcome Bus Errors which occur on many boards if the slave accesses
* shared memory (SM) before the master has finished initializing it.  The code
* here simply delays access to the SM region until the SM master has finished
* initializing it.
*
* The method used is to repetitively probe key locations in the SM region
* after delay periods until valid values are detected or a time-out occurs.
* The initial delay period is set based on processor number.  (The master
* processor does not delay.)  If the first probe of a location fails, an
* exponential increase in delay period is used to reduce bus contention on
* subsequent probes.
*
* This method is no better than receiving raw BERRs and does reduce bus
* contention and the number of BERRs.
*
* NOMANUAL
*/

STATUS usrSmNetInit (void)
    {
	
    if (!backplaneBoot)
        {
        unsigned int bpmask = 0;
        END_OBJ*   pEnd;

        pEnd = endFindByName ("sm", 0);
        if (pEnd == NULL)
			{
            printf ("Can not find the shared memory END device.");
            printf (" Check address assignments for SM master.\n");
	    return (ERROR);
            }
        else 
	    {
#ifdef INCLUDE_IPATTACH  
            /* Attach shared memory END device to the stack. The device
             * will be attached to all supported network-layer protocols
             * built into the stack. IPNet stack does not support attach
             * a device to IPv4 and IPv6 separately.
             */

            usrNetIpAttachCommon ("sm", 0, "IPv4", ipAttach);
#endif /* INCLUDE_IPATTACH */
            }

        /* 
         * The load routine for the shared memory END device determines
         * an IP address for the shared memory master using the boot
         * parameter values. If none is available, the load fails and
         * the previous endFindByName() call will prevent this configuration
         * attempt.
         *
         * The shared memory slaves do not require an address from the boot
         * parameters. If the backplane address is not given, those devices
         * will retrieve an address from the shared memory master.
         */ 

        if (sysBootParams.bad[0] == EOS)
            { 
            if (smEndInetAddrGet ("sm", 0, NONE, sysBootParams.bad) != OK)
                printf ("Error: sm0 addressing is not setup correctly\n");
			    return (ERROR);
            }
        else
            {
            if (bootNetmaskExtract (sysBootParams.bad, (int*)&bpmask) <1)
				bpmask = 0xffffff00;   /* set to a default class C */
			    printf("smNet: set mask = 0x%x for %s\n",bpmask,sysBootParams.bad);
            }
		
		if (bpmask == 0)		
			printf("smNet: mask = 0x%x no ip traffic allowed on %s\n",bpmask,sysBootParams.bad);
        /* SM configuration with boot parameters */

        usrNetBootConfig ("sm", 0, sysBootParams.bad,
                          bpmask, sysBootParams.gad); 
        }

#ifdef INCLUDE_SM_NET_SHOW
    smEndShowInit ();
#endif /* INCLUDE_SM_NET_SHOW */

    return (OK);
    }


