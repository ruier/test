/* usrSsi.c - SSI initialization */

/* Copyright 2006, 2010-2011 Wind River Systems, Inc. */

/*
modification history
--------------------
01h,20dec11,shi  fixed for WIND00317123,disable SSI only when build for 64bit system.
01g,09sep10,y_t  fixed for WIND00232105,temporarily disable SSI for 64bit.
01f,07mar06,tkf  SNMP dependency on DSI only when AGENTX is included (SPR#118700).
01e,04mar06,xli  added the local socket show init routines and #ifdef 
01d,14feb06,xli  remove the hardcoded comps table and get the info from Workbenck GUI 
01c,09feb06,xli  added the SSI_PRINTF and dynamic comp macro for comp table
01b,25jan06,xli  added #ifdef INCLUDE_CNS_COMP to the comps table
01a,02aug06,tkf  Created
*/

/*
DESCRIPTION

SSI initialization code
*/

#ifndef _WRS_CONFIG_LP64
/* includes */
/* #include "usrSsiCfg.h" */
#include <vxWorks.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lstLib.h>
#include "ssi/ssm.h"
#include "ssi/csm.h"
#include "ssi/cnsLib.h"

/* defines */
#define SSM_COMP_NAME_SYSTEM        "system"

/* forward declarations */
#ifdef INCLUDE_DSI_DYNAMIC_INIT
STATUS usrSsiDsiInit
    (long * pStatus, char * pArgs);
#endif

#ifdef INCLUDE_SNMP_DYNAMIC_INIT
STATUS usrSsiSnmpInit
    (long * pStatus, char * pArgs);
#endif

#define DEFAULT_SSI_COMPONENTS 100

ssiCompRegInfo_t comps[DEFAULT_SSI_COMPONENTS];

LOCAL void ssiCompInitEntryParser()
{                                                                            

    int i;
    ssiCompRegInfo_t ssiComp;
    char* pRemains;
    pRemains = NULL;
    i = 0;
#ifdef  INCLUDE_DSI_DYNAMIC_INIT
    ssiComp.pName      = SSI_DSI_NAME;
    ssiComp.pDependOn  = SSI_DSI_DEPENDON;
    ssiComp.launchFunc = SSI_DSI_INIT_ROUTINE;
    ssiComp.pArgs      = SSI_DSI_ARGS;
    ssiComp.pOpts      = SSI_DSI_OPTIONS;
    comps[i] =  ssiComp;
    i++; 
#endif

#ifdef  INCLUDE_SNMP_DYNAMIC_INIT
    ssiComp.pName      = SSI_SNMP_NAME;
#ifdef INCLUDE_DSI_DYNAMIC_INIT
    ssiComp.pDependOn  = SSI_SNMP_DEPENDON;
#else
    ssiComp.pDependOn  = NULL;
#endif
    ssiComp.launchFunc = SSI_SNMP_INIT_ROUTINE;
    ssiComp.pArgs      = SSI_SNMP_ARGS;
    ssiComp.pOpts      = SSI_SNMP_OPTIONS;
    comps[i] =  ssiComp;
    i++; 
    
#endif 
     
  
#ifdef  INCLUDE_SSI_HELLO
    ssiComp.pName      = SSI_HELLO_NAME;
    ssiComp.pDependOn  = SSI_HELLO_DEPENDON;
    ssiComp.launchFunc = SSI_HELLO_INIT_ROUTINE;
    ssiComp.pArgs      = SSI_HELLO_ARGS;
    ssiComp.pOpts      = SSI_HELLO_OPTIONS;

    comps[i] =  ssiComp;
    i++;
#endif
    
#ifdef  INCLUDE_SSI_HELLORTP
    ssiComp.pName      = SSI_HELLORTP_NAME;
    ssiComp.pDependOn  = SSI_HELLORTP_DEPENDON;
    ssiComp.launchFunc = SSI_HELLORTP_INIT_ROUTINE;
    ssiComp.pArgs      = SSI_HELLORTP_ARGS;
    ssiComp.pOpts      = SSI_HELLORTP_OPTIONS;
    comps[i] =  ssiComp;
    i++;
#endif
    ssiComp.pName = NULL;    
    ssiComp.pDependOn = NULL;
    ssiComp.launchFunc = NULL; 
    ssiComp.pArgs      = 0;
    ssiComp.pOpts      = 0;
    comps[i] = ssiComp;
     

}

/*****************************************************************************
* usrSsiInit - initializes SSI
*
* This function initializes SSI
*
* RETURNS: ERROR if initialization problems occur
* ERRNO: N/A
*/
void usrSsiInit(void) 
{   
    ssiCompInitEntryParser();
    /* Initialize the SSI data base */
    SSI_PRINTF ("usrSsiInit: Initializing SSM ... ");
    if (ssiDbInit (comps) == OK)
        SSI_PRINTF ("Done.\n");
    else
    {
        SSI_PRINTF ("Could not intialize SSI database !!!\n");
        return;
    }

    SSI_PRINTF ("usrSsiInit: Initializing CSM ... ");
    if (csmInit (FALSE) == OK)
        SSI_PRINTF ("Done.\n");
    else
    {
        SSI_PRINTF ("Failed !!!\n");
        return;
    }

    /* Launch all registered components */
    if (ssmCompLaunchAll () == ERROR)
    {
        SSI_PRINTF ("usrSsiInit: ssmCompLaunchAll() failed.\n");
        return;
    }

    /*
     * This starts the CSM. All components that have no dependency other than the
     * "system" are allowed to initialize
    */

    ssmSysDependencyReady (SSM_COMP_NAME_SYSTEM);
}

#ifdef INCLUDE_DSI_DYNAMIC_INIT
STATUS usrSsiDsiInit
    (
    long *  pStatus,
    char *  pArgs
    )
{
    *pStatus = CSM_STATE_INITIALIZE;
   
#ifdef INCLUDE_DSI_SOCKET     
    usrDsiSocket();         /* Installs and initializes the DSI backend */
#endif

#ifdef INCLUDE_DSI_POOL  
    usrDsiPoolConfig ();    /* Number of DSI sockets, amount of data clusters */
#endif

#ifdef INCLUDE_UN
    usrUnInit ();           /* AF_LOCAL address domain and harness */
#endif

#ifdef INCLUDE_UN_COMP    
    usrUnCompInit ();
#endif

#ifdef INCLUDE_UN_SHOW
    unShowInit ();
#endif

#ifdef INCLUDE_UN_COMP_SHOW
     uncompShowInit ();
#endif

    *pStatus = CSM_STATE_READY;

    return OK;
}
#endif

#ifdef INCLUDE_SNMP_DYNAMIC_INIT

STATUS usrSsiSnmpInit
    (
    long *  pStatus,
    char *  pArgs
    )
{
    *pStatus = CSM_STATE_INITIALIZE;

    usrSnmpCfgInit();

    *pStatus = CSM_STATE_READY;

    return OK;
}
#endif
#endif
