/* usrTipc.c - TIPC configlettes */

/*
 * Copyright (c) 2004-2008 Wind River Systems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
Modification history
--------------------
01q,24jul12,h_x  Fix WIND00364438
01p,30may12,h_x Fix WIND00249918
01o,08oct08,als  Update to TIPC 1.7.6 (Defect WIND00127984)
01n,27jul07,ebh  Fix AMP bundle inclusion of TIPC Configuration (WIND00099633)
01m,09may07,mze  added usrTipcDefsInit and the ifdefs to support the BSP builds
01l,21dec06,bwa  added dSHM media.
01k,20jun06,mze  code review changes for TIPC portability 
01j,05jun06,ggz  added TIPC1.5 HEND support for VxWorks6.2
01i,13mar06,ebh  updated default configuration string for simulators
01h,08feb06,wap  Use new parameter system
01g,16sep05,dsk  added TIPC HEND initialization routine usrTipcHendAttach()
01f,11apr05,als  added licensing info; incorporate code inspection fixes
01e,18mar05,bwa  added shared memory support.
01d,16feb05,als  Added support for multiple media types
01c,10feb05,als  Enabled Ethernet support in VxSim; declare tipc_init()
01b,07sep04,als  Incorporated code inspection feedback
01a,04jul04,als  Created
*/

/*
DESCRIPTION
This configlette generates the declarations needed for the user-selectable
aspects of TIPC, including:
    - binding in the requested media type(s)
    - getting the requested configuration string(s)

Static (compile-time) information is specified by TIPC_CONFIG_STR, which is
a pointer to a configuration string.

Dynamic (run-time) information is specified by TIPC_CONFIG_HOOK, which is a 
pointer to a configuration hook routine.  At most one configuration hook may
be selected by the user.  If a system-supplied configuration hook is selected
it is included in the output of this configlette, otherwise it is excluded
from the resulting image.

If both static and dynamic configuration information is specified by the user,
dynamic values will override static values in the event of a conflict.

TIPC HEND initialization routine determines if the list of TIPC priority
interfaces configured by the user can be attached as HEND interfaces, and 
attaches them if possible. In this way, TIPC frames can be handled at higher 
priority than IP frames. 

List of TIPC priority interfaces is defined in parameter TIPC_HEND_CONFIG_STR

INCLUDE_FILES: tipc.h, vxbParamSys.h, hEnd.h, jobQueueLib.h, string.h
*/

/* includes */


#include <tipc/tipc.h>

#ifdef INCLUDE_TIPC_HEND_INIT  


#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR == 2))
/*
 * vxbParamSys.h does not exist in VxWorks6.2. This file 
 * is excluded to provide TIPC portability. 
 */
#else
#include <hwif/util/vxbParamSys.h>
#endif 

#include <../src/hwif/h/hEnd/hEnd.h>
#include <jobQueueLib.h>             
#include <string.h>
    

#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR == 2))
    IMPORT HEND_INST_SEARCH * sysHEndSearch (void );
    IMPORT void sysHEndParamAttach (void * pParam, UINT32 paramType, 
                                   UINT32 storageType, 
                                   char * pName, UINT32 unit);
#endif /* VxWorks6.2 */

#endif

/* 
 * Declare array of pointers to media type descriptors used by TIPC.
 * (Array must be terminated by null pointer.)
 */

extern struct tipc_media_descr eth_media_desc;
extern struct tipc_media_descr sm_media_desc;
extern struct tipc_media_descr dshm_media_desc;

/* defines */

#ifndef TIPC_DEF_NET_ID
#define TIPC_DEF_NET_ID 4711
#endif
#ifndef TIPC_DEF_MAX_PORTS
#define TIPC_DEF_MAX_PORTS 8191
#endif
#ifndef TIPC_DEF_MAX_ZONES
#define TIPC_DEF_MAX_ZONES 4
#endif
#ifndef TIPC_DEF_MAX_CLUSTERS
#define TIPC_DEF_MAX_CLUSTERS 8
#endif
#ifndef TIPC_DEF_MAX_NODES
#define TIPC_DEF_MAX_NODES 255
#endif
#ifndef TIPC_DEF_MAX_REMOTES
#define TIPC_DEF_MAX_REMOTES 8
#endif
#ifndef TIPC_DEF_MAX_PUBS
#define TIPC_DEF_MAX_PUBS 10000
#endif
#ifndef TIPC_DEF_MAX_SUBS
#define TIPC_DEF_MAX_SUBS 2000
#endif
#ifndef TIPC_DEF_LOG_SIZE
#define TIPC_DEF_LOG_SIZE 0
#endif
#ifndef TIPC_DEF_REMOTE_MGT
#define TIPC_DEF_REMOTE_MGT 1
#endif

#ifdef INCLUDE_TIPC_HEND_INIT  
/* #define DEBUG_TIPC_HEND */
#define TIPC_HEND_IF_NAME_MAXLEN    20     /* one interface name */
#define TIPC_HEND_CONFIG_STR_MAXLEN 80     /* string of interface names */
#define TIPC_HEND_MAX_UNITNUM       99     /* max num of HEND interfaces */
#define TIPC_HEND_IF_UNIT_MAXLEN     2     /* 2 digit unit number, 0-99 */
				                           /* if TIPC_HEND_MAX_UNITNUM */
				                           /* is changed to require less */
				                           /* or more digits, also change */
				                           /* TIPC_HEND_IF_UNIT_MAXLEN */
#define T_TIPC_RX_TASK_PRIORITY     45     /* bigger than 50 = tNetTask */
#define T_TIPC_RX_TASK_STACK_SIZE   10000  /* same as tNetTask */
#define TIPC_HEND_MAX_NUM_INTERF    40     /* limit HEND interfaces */
#endif 

/* typedefs */

struct tipc_media_descr * tipc_media_list[] = 
    {
#ifdef INCLUDE_TIPC_MEDIA_ETH
    &eth_media_desc,
#endif
#ifdef INCLUDE_TIPC_MEDIA_SM
    &sm_media_desc,
#endif
#ifdef INCLUDE_DSHM_SVC_TIPC
    &dshm_media_desc,
#endif
    NULL
    };

/* Declare TIPC initialization routine */

extern STATUS tipc_init (char *configInfo, FUNCPTR configHook);

/* 
 * Handle case where no static configuration string is supplied
 * by updating TIPC_CONFIG_STR to indicate that no such string exists.
 */

#ifndef TIPC_CONFIG_STR
#define TIPC_CONFIG_STR NULL
#endif

/* 
 * Handle case where no configuration hook is selected
 * by updating TIPC_CONFIG_HOOK to indicate that no hook routine exists.
 * (Any define of type INCLUDE_TIPC_CONFIG_HOOK_xxx that follows will supersede
 * this default value.)
 */

#define TIPC_CONFIG_HOOK NULL

/* 
 * Handle selection of user-defined configuration hook
 * by updating TIPC_CONFIG_HOOK and declaring prototype for hook routine.
 */

#ifdef INCLUDE_TIPC_CONFIG_HOOK_USER
#undef  TIPC_CONFIG_HOOK
#define TIPC_CONFIG_HOOK tipcConfigInfoGet
extern STATUS tipcConfigInfoGet (char * buffer, UINT bufferlen);
#endif

/* globals */

#ifdef DEBUG_TIPC_HEND
    JOB_QUEUE_ID tipcJobQId; 
#endif

/* 
 * Handle selection of system-supplied bootline configuration hook
 * by updating TIPC_CONFIG_HOOK and defining the hook routine body.
 */

#ifdef INCLUDE_TIPC_CONFIG_HOOK_BOOT
#undef  TIPC_CONFIG_HOOK
#define TIPC_CONFIG_HOOK tipcBootInfoGet


/******************************************************************************
*
* tipcBootInfoGet - get TIPC configuration info from boot line
*
* This routine fills "buffer" with the run-time configuration info for TIPC
* stored in the 'other' field of the boot line.
*
* RETURNS: ERROR if unable to copy boot line info to buffer, otherwise OK
*
*/ 

STATUS tipcBootInfoGet
    (
    char * buffer,                      /* buffer to fill with config info */ 
    UINT bufferLen                      /* size of buffer (in bytes) */
    )
{
    BOOT_PARAMS bootParams;
    char * pParseEnd;
	char * ptr;
	char * ptr1;

    /* Take configuration info from "other" field of boot parameters. */
    
    pParseEnd = bootStringToStruct (BOOT_LINE_ADRS, &bootParams);
    if (*pParseEnd != EOS)
        return ERROR;
    strncpy (buffer, bootParams.other, bufferLen);
    if (buffer [bufferLen - 1] != '\0')
        return ERROR;

	if(buffer[0] != '\0')
	{
		ptr = strstr(buffer,"tipc:");
		if(ptr == NULL)
		{
			goto end;
		}
		ptr1 = strstr(buffer,":end");
		if(ptr1 == NULL)
		{
			return ERROR;
		}
		memcpy(buffer,(ULONG)ptr+5,(ULONG)ptr1-((ULONG)ptr+5));
		buffer[(ULONG)ptr1-((ULONG)ptr+5)] = '\0';
	}

end:

#ifdef _WRS_ARCH_IS_SIMULATOR
    /* In VxSim, use default configuration info if user doesn't supply any */
    
    if (buffer[0] == '\0')
        sprintf (buffer, "a=1.1.%d;be=eth:simnet0/1.0.0;log=1024",
		sysProcNumGet () + 1);
#else
#ifdef _WRS_VX_AMP
    /* FIXME: may need to adjust on a per BSP basis */
    /* Following is currently needed for hpcNet8641 */
#define DSHM_TIPC_BOOT_LINE "be=dshm:plb0;a=1.1.1X;log=1024"
    if (buffer[0] == '\0')
	{
	char *pX;
	if (bufferLen < (strlen (DSHM_TIPC_BOOT_LINE) + 1))
	    return ERROR;
	strcpy (buffer, DSHM_TIPC_BOOT_LINE);
	pX = (strchr(buffer, 'X'));
	*pX = '0' + (char)sysProcNumGet();
	return OK;
	}
#undef DSHM_TIPC_BOOT_LINE
#endif	/* _WRS_VX_AMP */
#endif	/* _WRS_ARCH_IS_SIMULATOR */

    return OK;
}

#endif



#ifdef INCLUDE_TIPC_HEND_INIT

/******************************************************************************
*
* usrTipcInterfacesParseAndAttach - parse interface names and attach to HEND
*
* This routine parses the list of user-configured TIPC HEND interfaces, 
* determines if they are really HEND interfaces and attaches these interfaces to
* HEND for prioritized processing.
* Frames received from TIPC HEND interfaces attached to HEND are stored in the
* receive queue described by pRxParam and processed by the tTipcRxTask at
* higher priority than IP frames.
*
* The list of user-configured TIPC HEND (example "mottsec0,mottsec1") is the
* contents of parameter TIPC_HEND_CONFIG_STR of component INCLUDE_TIPC_HEND_INIT
*
* RETURNS: OK, if all user-configured interfaces listed in TIPC_HEND_CONFIG_STR 
*               are found to be HEND interfaces 
*          ERROR otherwise
*/ 

LOCAL STATUS usrTipcInterfacesParseAndAttach
    (
    HEND_RX_QUEUE_PARAM *   pRxParam         /* receive queue descriptor */
    )
    {
    char *  pTipcIfNames;             /* current interface in list */
    char *  pNextTipcIfNames;         /* next interface in list */
    int     tipcIfNameLen;            /* length of TIPC HEND Interface Name */
    STATUS  status = OK;              /* return code */
    int     num_iter = 0;             /* index in list of TIPC config interf. */

#if (!((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR == 2)))

/* The following variables are used in post VxWorks6.2 releases */
    
    char    pInstName[TIPC_HEND_IF_NAME_MAXLEN]; /* HEND interface type string */

    char    pQueueName[sizeof("RxQueue") + 3]; /* HEND queue name string */

    char *  pUnitStr = NULL;          /* interface unit string */
    int     unit = 0;                 /* interface unit number */
    VXB_DEVICE_ID vxb_dev_id = NULL;  /* VXB device ID (pointer to VXB device) */
    VXB_INST_PARAM_VALUE paramValue;  /* VXB device parameter values */

#else

/* 
 * VxWorks6.2: The following variables are added to provide TIPC1.5 
 * portability for vxWorks6.2
 */
    
    HEND_INST_SEARCH *  pHEndInstSearch; /* HEND interface descriptors */
                                         /* names and unit numbers.  */
                                         /* Ex. "mottsec0", has HEND */
                                         /* interface name */
                                         /* "mottsecHend", unit 0 */
    int     instance;                 /* index in array of HEND interfaces */
    HEND_INST_ID *  pHEndInstId;      /* descriptor of HEND interface */
    int     hendIfNameLen;            /* length of HEND Interface name */
    char    hEndIfName [TIPC_HEND_IF_NAME_MAXLEN];/* HEND interface */
    
#endif 

/*
 * VxWorks6.2: This code is added to provide TIPC1.5 portability for vxWorks6.2.
 * It is excluded for the later VxWorks releases. 
 */

#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR == 2))
    
    /* Query BSP to detect installed HEND interfaces */

    pHEndInstSearch = sysHEndSearch ();

    /* It is not necessary to do the check below */

    if (pHEndInstSearch == NULL)
        {
        printf ("No HEND interfaces descriptor found \n");
        return ERROR;
        }
    if (pHEndInstSearch->pHEndInstId == NULL)
        {
        printf ("No HEND interfaces array found \n");
        return ERROR;
        }
    if (pHEndInstSearch->count >= TIPC_HEND_MAX_NUM_INTERF)
        {
        printf("Number of HEND interfaces too large (%d) \n", 
              pHEndInstSearch->count);
        /* do not free pHEndInstSearch and its HEND instance pointers, 
         * they are HEND's own structures 
         */

        return ERROR;
        }

#endif /* VxWorks6.2 */

    pNextTipcIfNames = TIPC_HEND_CONFIG_STR;

    while ((pNextTipcIfNames != NULL) && (num_iter < TIPC_HEND_MAX_NUM_INTERF ))
        {
        /* parse string of configured HEND interfaces up to next interface.
         * Interface names in list are delimited by "," 
         */

        pTipcIfNames = pNextTipcIfNames;
        pNextTipcIfNames = strpbrk ( pTipcIfNames, ",");
#ifdef DEBUG_TIPC_HEND
        printf ("\t pTipcIfNames %s, pNextTipcIfNames %s \n", 
               pTipcIfNames, pNextTipcIfNames);
#endif
        num_iter++;
        if (pNextTipcIfNames == NULL) 
            {
            tipcIfNameLen = strlen (pTipcIfNames);
            }
        else
            {
            tipcIfNameLen = strlen(pTipcIfNames) - strlen(pNextTipcIfNames);

            /* skip "," delimiter */

            pNextTipcIfNames++;

            if (tipcIfNameLen == 0)
                {
                    printf(" Empty interface name configured %s \n", 
                          pTipcIfNames);
                    return ERROR;
                }
            }

#if ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR == 2))
/* 
 * VxWorks6.2: This code provides TIPC1.5 portability for vxWorks6.2. 
 * It is excluded for the later VxWorks releasees. 
 */ 
        
        /* Search TIPC HEND configured interface in array of 
         * HEND interfaces detected by BSP query 
         */

        for (instance = 0, pHEndInstId = pHEndInstSearch->pHEndInstId; 
             instance < pHEndInstSearch->count; 
             instance++, pHEndInstId++) 
            {   
            /* determine TIPC HEND interface name (example "mottsec0") from 
             * HEND interface name (example "mottsecHend") and HEND unit number
             */

            hendIfNameLen = strlen (pHEndInstId->name) - strlen ("HEnd");
            
            /* allow space for HEND unit number after HEND name */
            
            /* The check below is not necessary and can be removed */ 
            
            if (hendIfNameLen >= (TIPC_HEND_IF_NAME_MAXLEN - 
                                  TIPC_HEND_IF_UNIT_MAXLEN)) 
                {
                printf (" HEND Interface name too long %d, maxlen %d \n", 
                       hendIfNameLen, 
                       TIPC_HEND_IF_NAME_MAXLEN - TIPC_HEND_IF_UNIT_MAXLEN);
                continue;
                }
            if ((hendIfNameLen < 0) ||
                (strcmp(pHEndInstId->name + hendIfNameLen, "HEnd") != 0))
                {
                printf (" Improper HEND Interface Name %s \n",
                       pHEndInstId->name); 
                continue;
                }
            if (pHEndInstId->unit > TIPC_HEND_MAX_UNITNUM)
                {
                printf (" Improper HEND Unit Number %s \n",
                       pHEndInstId->name); 
                continue;
                }
            
            strncpy (hEndIfName, pHEndInstId->name, hendIfNameLen);
            
            hendIfNameLen += sprintf (hEndIfName + hendIfNameLen, "%d", pHEndInstId->unit);

            /* compare TIPC HEND interface name with HEND interface name */
            
            if (strncmp (pTipcIfNames, hEndIfName, hendIfNameLen) == 0) 
                {   
                sysHEndParamAttach (pRxParam, HEND_RX_Q_PARAM, HEND_VOID_PTR, 
                                    pHEndInstId->name, pHEndInstId->unit);  

                break;
                }
            } 
        if (instance == pHEndInstSearch->count)
            {
            strncpy(hEndIfName, pTipcIfNames, tipcIfNameLen);
            printf (" Interface \"%s\" is not an HEND interface \n", hEndIfName);
            status = ERROR; 
            }

#else /* release > 6.2  */ 
        
        /* create interface name string and unit number
           For example determine "mottsec" and int 0 for interface "mottsec0" */
        strncpy(pInstName, pTipcIfNames, tipcIfNameLen);

        /* determine interface name string and integer unit number */
        pUnitStr = strpbrk (pInstName, "0123456789");
        if (pUnitStr == NULL)
            {
                printf(" No unit number in interface name %s \n", pInstName);
                return ERROR;
            }
        /* determine interface number from interface number string */
        unit = (int) strtol (pUnitStr, (char **)NULL, 10);

        /* obtain interface type name in pInstName */
        *pUnitStr = 0;

	/* append HEnd to device name so VxBus can find it */
        sprintf (pInstName, "%sHEnd", pInstName);


        vxb_dev_id = vxbInstByNameFind (pInstName, unit);

        /* attach HEND to the interface by setting parameters for the device */
        if (vxb_dev_id != NULL)
            {
            /*
            sysHEndParamAttach (pRxParam, HEND_RX_Q_PARAM, HEND_VOID_PTR, 
                                    pHEndInstId->name, pHEndInstId->unit);  
            */
	    int q = 0;

            paramValue.pValue = pRxParam;
	    
	    /* configure ALL receive queues to work with TIPC */
	    do
                sprintf(pQueueName, "%s%02d", "rxQueue", q++);
            while (OK == vxbInstParamSet (vxb_dev_id, pQueueName, 
					  VXB_PARAM_POINTER, &paramValue));

            }


#endif
        } /* while */
    
    return status;
    }

/******************************************************************************
*
* usrTipcHendAttach - Init routine of component INCLUDE_TIPC_HEND_INIT
* 
* This routine creates an infrastructure (receive queue and task, registration 
* with HEND) that allows TIPC frames to be processed at higher priority than IP
* for a user-configured subset of interfaces. 
* The user-configured subset of interfaces is specified by the parameter 
* TIPC_HEND_CONFIG_STR of component INCLUDE_TIPC_HEND_INIT. 
* These interfaces will be attached to tTipcRxTask instead of tNetTask
* for receive frame processing 
*          
* The routine must be called before muxDevLoad ()
*
* RETURNS: OK if all infrastructure could be created and 
*               all TIPC interfaces listed in TIPC_HEND_CONFIG_STR component 
*               parameter were found to really be HEND interfaces 
*          ERROR otherwise 
*/ 

STATUS usrTipcHendAttach (void)
    {
    HEND_RX_QUEUE_PARAM *   pRxParam;                 /* HEND Receive queue */
    STATUS                  status = OK;              /* return code */

#ifdef DEBUG_TIPC_HEND
    printf ("\n\n INCLUDE_TIPC_HEND_INIT component, INIT_RTN: usrTipcHendAttach\n");
#endif

    if (TIPC_HEND_CONFIG_STR == NULL)
        {
        printf ("Parameter TIPC_HEND_CONFIG_STR (%s) not set\n", 
                TIPC_HEND_CONFIG_STR);
        return ERROR;
        }
    if (strlen (TIPC_HEND_CONFIG_STR) >= TIPC_HEND_CONFIG_STR_MAXLEN) 
        {
        printf ("Parameter TIPC_HEND_CONFIG_STR (%s) string too long\n", 
                TIPC_HEND_CONFIG_STR);
        return ERROR;
        }
    
#ifdef DEBUG_TIPC_HEND
    printf ("\n\n  Configured HEND interfaces: %s \n", TIPC_HEND_CONFIG_STR);
#endif
    
    /* create TIPC receive jobQueue and its handler task, tTipcRxTask */

    pRxParam = malloc (sizeof (HEND_RX_QUEUE_PARAM));
    if (pRxParam == NULL) 
        {
        printf (" Could not allocate memory for TIPC HEND infrastructure \n");
        return ERROR;
        }
    bzero ( (char *)pRxParam, (sizeof (HEND_RX_QUEUE_PARAM)));

    /* create jobQueue with empty standard queue */

    pRxParam->jobQueId = jobQueueCreate (NULL); 
    if (pRxParam->jobQueId == NULL) 
        {
        free (pRxParam);
        printf (" could not create TIPC job Queue, jobQueueCreate failed \n");
        return ERROR;
        }
#ifdef DEBUG_TIPC_HEND
    tipcJobQId = pRxParam->jobQueId; /* global, for debug only */
#endif
    /* priority of default hardware queue (if any) for the interface */

    pRxParam->priority = 1; /* high priority queue */
    pRxParam->rbdNum = 128; /* number of RX bd's */
        
    /* spawn task that will process TIPC frames on the configured interfaces */

    if (taskSpawn ("tTipcRxTask", T_TIPC_RX_TASK_PRIORITY,
            	   VX_UNBREAKABLE, T_TIPC_RX_TASK_STACK_SIZE, 
                   (FUNCPTR) jobQueueProcess, (int) pRxParam->jobQueId, 
                   0, 0, 0, 0, 0, 0, 0, 0, 0)
        == ERROR) 
        {
        jobQueueDelete (pRxParam->jobQueId);
        free (pRxParam);
        printf (" Could not create tTipcRxTask (taskSpawn failed) \n");
        return ERROR;
        }
    
    /* Find TIPC HEND configured interfaces that really are HEND interfaces 
     * and attach them to HEND 
     */

    status = usrTipcInterfacesParseAndAttach (pRxParam);
#ifdef DEBUG_TIPC_HEND
    printf(" TIPC-HEND init status: %d \n", status);
#endif
    return status;
    }

#endif	 /* INCLUDE_TIPC_HEND_INIT */



/******************************************************************************
*
* usrTipcDefsInit - Init routine of component INCLUDE_TIPC_DEFINES
* 
* This routine sets the global variables to the values of the parameters in the cdf 
*          

*
* RETURNS: NA 
*/
void usrTipcDefsInit(void)
{
    tipc_net_id = TIPC_DEF_NET_ID;
    tipc_max_ports = TIPC_DEF_MAX_PORTS;          
    tipc_max_zones = TIPC_DEF_MAX_ZONES;             
    tipc_max_clusters = TIPC_DEF_MAX_CLUSTERS;          
    tipc_max_nodes = TIPC_DEF_MAX_NODES; 
    tipc_max_remotes = TIPC_DEF_MAX_REMOTES;                     
    tipc_max_publications = TIPC_DEF_MAX_PUBS;  
    tipc_max_subscriptions = TIPC_DEF_MAX_SUBS; 
    tipc_log_size = TIPC_DEF_LOG_SIZE;
    tipc_remote_management = TIPC_DEF_REMOTE_MGT;
}
           
