/* sysNet.c - VxSim BSP network susbsystem support */

/*
 * Copyright (c) 2003-2006,2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */


/*
modification history
--------------------
01r,15dec09,dbt  Added NAT support.
01q,22sep06,mze  adding SM_PKTS_SIZE and SM_MAX_PKTS for WIND00058841
01p,21sep06,dbt  Initialize network interface only if IPCOM support is
                 enabled.
01o,08sep06,dbt  Updated to reflect new interface names.
		 Removed unneeded inclusion of ifLib.h.
01n,31aug06,kch  Removed the unnecessary routeLib.h include statement.
01m,02aug06,dbt  Updated for new network stack.
01l,26aug05,dlk  Fix a compiler warning when INCLUDE_NET_BOOT_CONFIG is not
		 defined.
01k,15apr05,elp  no longer use sysBootParams gateway with additional interface,
		 no longer call usrNetBootConfig() without ipAddr.
01j,16feb05,elp  remove other bootparam usage SPR# 106176.
01i,28may04,jeg  added smEnd driver support
01h,01apr04,elp  added IPv6 attachment.
01g,22mar04,jmp  added simnetEnd.c inclusion.
01f,03feb04,elp  updated sysNetIfConfig() to call usrNetwork.c routines,
		 removed SIMIP support.
01e,27oct03,dbt  Updated for new network stack.
01d,10oct03,dbt  Put back IP to ethernet resolution routine needed by Windows
                 simip network driver.
01c,07oct03,elp  renamed vxsimNetAddIfGet(),
		 added attachment message for additional interfaces,
		 moved sysNetStop() on host side.
01b,02oct03,dbt  Add support for SIMIP (it replaces ULIP).
01a,01sep03,elp  build <endDevTbl> from command line options.
01a 29apr03,dbt  written from sysLib.c.
*/

/*
DESCRIPTION
This library contains BSP-specific routines for network subsystems,
including network interface card device driver configuration and
support routines.

Note that the routines implemented in this file will not necessarily
be included in future versions of Wind River Systems' VxSim for Windows
BSP.

SEE ALSO:
.pG "Configuration"

*/

/* includes */

#include <end.h>
#include <stdlib.h>
#include <muxLib.h>
#include <ipProto.h>
#include <config.h>
#include <sysLib.h>
#include <net/utils/routeCmd.h>
#include <drv/end/smEnd.h>
#include <net/utils/ifconfig.h>

#include <vxsimHostLib.h>

/* externals */

#ifdef INCLUDE_SIMNET
extern END_OBJ * simnetLoad (char *, void *);
extern END_OBJ * simnetNatLoad (char *, void *);
#endif /* INCLUDE_SIMNET */

#include "simnetEnd.c"	/* include simulator Enhanced Network Driver */

/* defines */

#define	MAX_CMD_LENGTH	300	/* max ifconfig command string length */

/* globals */

END_TBL_ENTRY endDevTbl [IP_MAX_UNITS + 1];

#ifdef INCLUDE_SM_NET
/**********************************************************************
*
* sysSmEndLoad - load the shared memory END driver 
*
* This function packs the loadstring, and calls the smEndLoad to load the 
* the shared memory END driver
*
* RETURN: pointer to the END object or NULL
*
* ERRNO: N/A
*
* SEE ALSO: smEndLoad() 
*/

END_OBJ * sysSmEndLoad
    (
    char *	pParamStr,       /* ptr to the init parameter string 	*/ 
    void *	unused           /* unused argument 			*/
    )
    {
    END_OBJ *	pEnd;			/* returned end driver object pointer */
    char paramStr [END_INIT_STR_MAX];   /* from end.h 			      */

    /* 
     * The smEnd loading string should be:  
     * "<unit>:<pAnchor>:<smAddr>:<memSize>:<tasType>:
     *  <maxCpus>:<masterCpu>:<localCpu>:<maxPktBytes>:<maxInputPkts>:
     *  <intType>:<intArg1>:<intArg2>:<intArg3>:<mbNum>:<cbNum>:
     *  <configFlg>:<pBootParams>"
     */

    if (strlen (pParamStr) == 0)
        {
	/* 
	 * If empty string, the driver is expected to return the 
	 * driver name in <pParamsStr>
	 */

        pEnd = (END_OBJ *)smEndLoad (pParamStr);
        }
    else
	{
        /*
         * On the second pass though here, we actually create 
         * the initialization parameter string on the fly.   
         * Note that we will be handed our unit number on the 
         * second pass through and we need to preserve that information.
         * So we use the unit number handed from the input string.
         */

	char *		compStr; 		/* composed string 	  */
        UINT32          configFlag = 0;		/* configuration flag 	  */
        char *          bootStr;		/* boot parameters string */
        BOOT_PARAMS	bootParams;		/* VxSim boot parameters  */
        char *          bootString = sysBootLine;   /* config.h */

#ifdef INCLUDE_SM_SEQ_ADDR
        configFlag |= SMEND_SM_SEQ_ADDR;
#endif /* INCLUDE_SM_SEQ_ADDR */

#ifdef INCLUDE_PROXY_SERVER
        configFlag |= SMEND_PROXY_SERVER_FLAG;
#endif /* INCLUDE_PROXY_SERVER */

#ifdef INCLUDE_PROXY_CLIENT
        configFlag |= SMEND_PROXY_CLIENT_FLAG;
#endif /* INCLUDE_PROXY_CLIENT */

#ifdef INCLUDE_PROXY_DEFAULT_ADDR
        configFlag |= SMEND_PROXY_DEFAULT_ADDR_FLAG;
#endif /* INCLUDE_PROXY_DEFAULT_ADDR */

        /* get the boot parameters from the bootline */
  
	if (bootString == NULL)
	    bootString = BOOT_LINE_ADRS;

	bootStr= bootStringToStruct (bootString, &bootParams);

	if (*bootStr != EOS)
	    {
	    bootParamsErrorPrint (bootString, bootStr);
	    printf ("sysSmEndLoad: Error to get the boot parameters \n");
	    return (NULL);
	    }

	/* compose the new string, compStr points to paramStr */

	compStr = strcpy (paramStr, pParamStr);

	/* Now, we advance compStr, by finding the end of the string */

	compStr += strlen (paramStr);
	
	/* finish off the initialization parameter string */

	sprintf (compStr, "0x%x:0x%x:0x%x:%d:%d:%d:%d:%d:%d:%d:0x%x:0x%x:0x%x:"
		     "0x%x:0x%x:0x%x:0x%x",
		 (UINT32)sysSmAnchorAdrs,     /* ANCHOR address 	     */
		 (UINT32)SM_MEM_ADRS,         /* SM memory address 	     */
		 SM_MEM_SIZE,                 /* SM memory size 	     */
		 SM_TAS_HARD,                 /* SM_TAS_TYPE 		     */
		 0,                           /* max CPU#, 0 default 	     */
		 0,                           /* Master CPU#, 0 default	     */
		 (UINT32)(sysProcNumGet ()),  /* Local CPU# 		     */
		 SM_PKTS_SIZE,                /* Max bytes per packet, 0 def */
		 SM_MAX_PKTS,                 /* Max input packet, 0 default */
		 SM_INT_TYPE,                 /* SM interrupt type 	     */
		 SM_INT_ARG1,                 /* SM interrupt argu 1 	     */ 
		 SM_INT_ARG2,                 /* SM interrupt argu 2 	     */ 
		 SM_INT_ARG3,                 /* SM interrupt argu 3 	     */ 
		 0,                 	      /* Mblk num, 0 default 	     */
		 0,                 	      /* cBlk num, 0 default 	     */
		 configFlag,                  /* configuration flag 	     */
		 (UINT32)&bootParams          /* pointer to the BOOT_PARAMS  */
		 );

        if ((pEnd = smEndLoad (paramStr)) == (END_OBJ *)NULL)
	    {
            printf ("Error: device failed sysSmEndLoad routine.\n");
	    }
	}

    return (pEnd);
    }
#endif /* INCLUDE_SM_NET */

/******************************************************************************
*
* sysNetEndEntryInit - initialize END entry for a device
*
* This routine initializes load routine function depending on device name
*
* RETURNS: OK or ERROR if no initialization routine is found
*/

STATUS sysNetEndEntryInit
    (
    END_TBL_ENTRY *	pEndTblEntry,
    char *		device,
    char *		endLoadString
    )
    {

    pEndTblEntry->endLoadFunc = NULL;

#ifdef INCLUDE_SIMNET 
    if (strcmp (device, "simnet") == 0)
    	{
	pEndTblEntry->endLoadFunc = simnetLoad;
	}
    else if (strcmp (device, "simnet_nat") == 0)
    	{
	pEndTblEntry->endLoadFunc = simnetNatLoad;
	}
#endif /* INCLUDE_SIMNET */

#ifdef INCLUDE_SM_NET
    if (strncmp (device, "sm", 2) == 0)
    	{
	pEndTblEntry->endLoadFunc = sysSmEndLoad;
	}
#endif /* INCLUDE_SM_NET */

    /* 
     * check if a endLoadFunc has been defined. It means check if SIMNET
     * and/or SM_NET is included.
     */

    if (pEndTblEntry->endLoadFunc == NULL)
    	return (ERROR);	/* return ERROR when unknown device is passed */

    pEndTblEntry->endLoan = 0;

    /* get subnet to attach to */

    if (endLoadString)
	pEndTblEntry->endLoadString = endLoadString;

    return (OK);
    }

/******************************************************************************
*
* sysNetEndInit - fill endDevTbl
*
* This routine fills <endDevTbl> with entries passed on VxSim command line.
*
* RETURNS: N/A
*/

void sysNetEndInit (void)
    {
    BOOT_PARAMS *	pParams = &sysBootParams;
    int			ix = 0;		/* END entry index */
    int			addIx;		/* additional if index */
    char *		device;
    int			unit;
    char *		devStr;
    char *		ipAddr;
    char *		ipMask;

    bzero ((char *)endDevTbl, (IP_MAX_UNITS + 1) * sizeof(END_TBL_ENTRY));

    /* store boot device information if any */
    
    if (strncmp (pParams->bootDev, "simnet", strlen ("simnet")) == 0)
	{
	endDevTbl[ix].unit = pParams->unitNum;
	if (sysNetEndEntryInit (&endDevTbl[ix], pParams->bootDev, NULL) == OK)
	    ix++;
	else
	    printErr ("error initializing endDevTbl[%d]\n", ix);
	}

    for (addIx = 0; ix < IP_MAX_UNITS; ix ++, addIx++)
    	{
	if (vxsimHostNetAddIfGet (addIx, &device, &unit, &devStr,
				  &ipAddr, &ipMask) == OK)
	    {
	    endDevTbl[ix].unit = unit;

	    if (sysNetEndEntryInit (&endDevTbl[ix], device, devStr) == ERROR)
	    	printErr ("error initializing endDevTbl[%d]\n", ix);
	    }
	else
	    {
#ifdef INCLUDE_SM_NET
	    /* 
	     * if SM_NET included, then the last end node 
	     * is the shared memory end driver node.
	     */

	    endDevTbl[ix].unit = 0;
	    if (sysNetEndEntryInit (&endDevTbl[ix], "sm", "") == OK)
		ix++;
	    else
		printErr ("error initializing endDevTbl[%d]\n", ix);
#endif /* INCLUDE_SM_NET */
	    return; /* end of additional interfaces found */
	    }
	}

    /*
     * reach end of endDevTbl, print a warning if there are more interfaces to
     * configure to indicate IP_MAX_UNITS should be increased.
     */
    if (vxsimHostNetAddIfGet (addIx, &device, &unit, &devStr, &ipAddr, &ipMask)
    	== OK)
	{
	printf
	    (
	    "Warning: Can't configure every interfaces : too many interfaces\n"
	    );
	}
    }

/******************************************************************************
*
* sysNetIfConfig - configure IP address for additional interfaces
*
* RETURNS: N/A
*/

void sysNetIfConfig ()
    {
#ifdef	INCLUDE_IPNET
    int		addIx = 0;		/* number of additional interfaces */
    char *	device;			/* device name */
    int		unit;			/* unit number */
    char *	devStr;			/* additional device parameter string */
    char *	ipAddr;			/* ip address */
    char *	ipMask;			/* ip mask */
    struct in_addr netmask;		/* network mask */
    char	cmd [MAX_CMD_LENGTH];	/* ifconfig command string */

    /* Configure all additional network interfaces if requested */

    while (vxsimHostNetAddIfGet (addIx, &device, &unit, &devStr, &ipAddr,
			      &ipMask) == OK)
	{
	addIx ++;

	if (ipAttach (unit, device) == ERROR)
	    {
	    vxsimHostPrintf ("Failed to attach to device %s%d", device, unit);
	    continue;
	    }

	if (ipAddr != NULL && (strlen(ipAddr) != 0))
	    {
	    /* Configure network interface */

	    snprintf (cmd, MAX_CMD_LENGTH, "%s%d %s", device, unit, ipAddr);
	    cmd [MAX_CMD_LENGTH - 1] = EOS;

	    if (ipMask != NULL)
		{
		netmask.s_addr = strtoul (ipMask, NULL, 16);

		if (netmask.s_addr != 0)
		    {
		    netmask.s_addr = htonl(netmask.s_addr);
		    strncat (cmd, " netmask ", MAX_CMD_LENGTH);
		    inet_ntoa_b (netmask, cmd + strlen (cmd));
		    cmd [MAX_CMD_LENGTH - 1] = EOS;
		    }
		}
	    strncat (cmd, " up", MAX_CMD_LENGTH);
	    cmd [MAX_CMD_LENGTH - 1] = EOS;

	    if (ifconfig (cmd) != OK)
	    	{
	    	vxsimHostPrintf ("Error configuring network " \
					"interface %s%d\n", device, unit);
		}
	    }

	}
#endif	/* INCLUDE_IPNET */
    }
