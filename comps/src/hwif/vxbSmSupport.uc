/* vxbSmSupport.uc - support for shared memory  */

/*
 * Copyright (c) 2005-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,28apr09,h_k  updated for LP64.
01g,01jul08,tor  library build
01f,04sep07,pdg  corrected apigen errors
01e,15aug07,h_k  removed sharedMemSupportAPIGet_desc.
01d,07aug07,dtr  Move hooks definition to here.
01c,13jun07,tor  remove VIRT_ADDR
01b,12may06,dgp  doc: fix name/description formatting, ERROR -> ERRNO
01a,04jun05,dtr  File created.
*/

/*
DESCRIPTION

This library implements the hardware interface memory management,

INCLUDE FILES : vxbSmSupport.h
*/

#ifdef INCLUDE_VXBUS_SM_SUPPORT

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <logLib.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbSmSupport.h>

/* hooks */

void (*vxbBusTasClearHook)(char*) = NULL;
BOOL (*vxbBusTasHook)(char*) = NULL;
STATUS (*vxbBusIntGenHook)(int,int) = NULL;
STATUS (*vxbBusIntAckHook)(int) = NULL;

/* Shared Memory Support Functions  */

/*********************************************************
*
* sysBusTasClear - clear a reservation for a particular address
*
* This routine clears a reservation for a particular address. If no direct 
* hook is available, it calls the VxBus helper function to find the correct 
* call.
*
* RETURNS: NONE
*
* ERRNO:
*/

void sysBusTasClear
    (
    volatile char * adrs
    )
    {
    if ( vxbBusTasClearHook != NULL )
        {
	(*vxbBusTasClearHook)((char*)adrs);
	return;
	}

    vxbDevIterate((FUNCPTR)sysBusTasClearHelper, (void*)adrs,
		  VXB_ITERATE_INSTANCES);
    }

/*********************************************************
*
* sysBusTasClearHelper - locate and clear a reservation for a particular address 
*
* This routine clears a reservation for a particular address 
* by calling all devices which support this functionality.
*
* RETURNS: NONE
*
* ERRNO:
*/
void sysBusTasClearHelper
    (
    VXB_DEVICE_ID	pInst,
    void *		adrs
    )
    {
    FUNCPTR				method;
    struct sharedMemSupportAPI *	pApi;

    method = vxbDevMethodGet(pInst,
			     (VXB_METHOD_ID)&sharedMemSupportAPIGet_desc[0]);
    if ( method != NULL )
        {
	pApi = (struct sharedMemSupportAPI *)(*method)(pInst, (char *)adrs);
	if ( ( pApi != NULL ) && ( pApi->tasClear != NULL ) )
	    (*pApi->tasClear)(adrs);
	}
    }

/*********************************************************
*
* sysBusTas - establish a reservation for a particular address
* 
* This routine establishes a reservation for a particular address. If no 
* direct hook is available, it calls the VxBus helper function to find 
* the correct call.
*
* RETURNS: NONE
*
* ERRNO:
*/
BOOL sysBusTas
    (
    char * adrs
    )
    {
    ULONG tasArgs[2];
    BOOL state;

    /* Pass address to TAS function */
    tasArgs[0] = (ULONG)adrs;
    
    if ( vxbBusTasHook != NULL )
        {
	state = (*vxbBusTasHook)(adrs);
	return(state);
	}

    vxbDevIterate((FUNCPTR)sysBusTasHelper, (void*)tasArgs,
		  VXB_ITERATE_INSTANCES);
    /* Requires state TRUE/FALSE returned for SM interface */
    state = (BOOL)tasArgs[1];
    
    return(state);
    }

/*********************************************************
*
* sysBusTasHelper - locate the correct device to do TAS
*
* This routine iterates through all VxBus devices to find the correct device 
* to do TAS.
*
* RETURNS: TRUE/FALSE via tasArgs 
*
* ERRNO:
*/
void sysBusTasHelper
    (
    VXB_DEVICE_ID	pInst,
    ULONG *		tasArgs
    )
    {
    BOOL state = FALSE;
    FUNCPTR				method;
    struct sharedMemSupportAPI *	pApi;
    char* adrs;

    adrs = (char *)tasArgs[0];

    method = vxbDevMethodGet(pInst,
			     (VXB_METHOD_ID)&sharedMemSupportAPIGet_desc[0]);

    if ( method != NULL )
        {
	pApi = (struct sharedMemSupportAPI *)(*method)(pInst, adrs);
	if ( ( pApi != NULL ) && ( pApi->tas != NULL ) )
	    {
	    state = (*pApi->tas)(adrs);
	    tasArgs[1] = (ULONG)state;

	    }
	}

    }

/*********************************************************
*
* sysBusIntGen - call interrupt generator function hook
* 
* This routine either calls interrupt generator function hook directly
* or calls VxBus helper to find the correct device and then calls the
* interrupt generator function hook. 
*
* RETURNS: STATUS
*
* ERRNO:
*/
STATUS sysBusIntGen
    (
    int level,          /* interrupt level to generate   */
    int vector          /* interrupt vector for interrupt*/
    )
    {
    UINT32 pArgs[2];
    STATUS status = OK;
    if ( vxbBusIntGenHook != NULL )
        {
	status = (*vxbBusIntGenHook)(level,vector);
	return(status);
	}

    pArgs[0] = level;
    pArgs[1] = vector;

    vxbDevIterate((FUNCPTR)sysBusIntGenHelper, (void*)&pArgs[0],
		  VXB_ITERATE_INSTANCES);

    return(OK);

    }

/*********************************************************
*
* sysBusIntGenHelpr - locate the correct call to generate an interrupt
*
* This routine iterates through all devices looking for the correct 
* call to generate the interrupt.
*
* RETURNS: NONE
*
* ERRNO:
*/
void sysBusIntGenHelper
    (
    VXB_DEVICE_ID	pInst,
    void *		pArgs
    )
    {
    FUNCPTR				method;
    struct sharedMemSupportAPI *	pApi;
    UINT32 *args  = (UINT32*)pArgs;


    method = vxbDevMethodGet(pInst,
			     (VXB_METHOD_ID)&sharedMemSupportAPIGet_desc[0]);
    if ( method != NULL )
        {
	pApi = (struct sharedMemSupportAPI *)(*method)(pInst, (char *)pArgs);
	if ( ( pApi != NULL ) && ( pApi->intGen != NULL ) )
	    (*pApi->intGen)(args[0],args[1]);
	}
    }

/*********************************************************
*
* sysBusIntAck - acknowledge an interrupt
*
* This routine calls a hook or helper function to acknowledge interrupt.
* 
* RETURNS: OK
*
* ERRNO:
*/
int sysBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge*/
    )
    {
    STATUS status;

    if ( vxbBusIntAckHook != NULL )
        {
	status = (*vxbBusIntAckHook)(intLevel);
	return(status);
	}

    vxbDevIterate((FUNCPTR)sysBusIntAckHelper, (void*)intLevel,
		  VXB_ITERATE_INSTANCES);

    return(OK);

    }

/*********************************************************
*
* sysBusIntAckHelpr - locate the correct call to acknowledge an interrupt
*
* This routine iterates through all devices looking for the correct 
* call to acknowledge an interrupt.
*
* RETURNS: NONE
*
* ERRNO:
*/
void sysBusIntAckHelper
    (
    VXB_DEVICE_ID	pInst,
    int 		intLevel
    )
    {
    FUNCPTR				method;
    struct sharedMemSupportAPI *	pApi;

    method = vxbDevMethodGet(pInst,
			     (VXB_METHOD_ID)&sharedMemSupportAPIGet_desc[0]);
    if ( method != NULL )
        {
	pApi = (struct sharedMemSupportAPI *)(*method)(pInst, (char *)intLevel);
	if ( ( pApi != NULL ) && ( pApi->intAck != NULL ) )
	    (*pApi->intAck)(intLevel);
	}
    }


/*********************************************************
*
* sysMailboxConnect - TBD
*
* RETURNS: STATUS
*
* ERRNO:
*/
STATUS sysMailboxConnect
    (
    FUNCPTR isr,
    int level
    )
    {
    return(OK);
    }

/*********************************************************
*
* sysMailboxEnable - TBD
*
* RETURNS: STATUS
*
* ERRNO:
*/
STATUS sysMailboxEnable
    (
    char * level
    )
    {
    return(OK);
    }

/*******************************************************************************
*
* bootLineAdrsGet -  retrieve boot line address from BSP configuration
*
*  This routine returns the address of the boot line, available from
*  the BSP macro BOOT_LINE_ADRS.
*
*  RETURNS: the value of BOOT_LINE_ADRS, specified by the BSP
*
*  ERRNO: not set
*/

char * bootLineAdrsGet(void)
    {
    return(BOOT_LINE_ADRS);
    }

/*******************************************************************************
*
* localMemLocalAdrsGet - Return the value of LOCAL_MEM_LOCAL_ADRS
*
*  This routine roeturns the address specified in the BSP by
*  the LOCAL_MEM_LOCAL_ADRS macro
*
*  RETURNS: the value of LOCAL_MEM_LOCAL_ADRS specified by the BSP,
*           or -1 if not specified
*
*  ERRNO: not set
*/

char * localMemLocalAdrsGet(void)
    {
#ifdef LOCAL_MEM_LOCAL_ADRS
    return(LOCAL_MEM_LOCAL_ADRS);
#else /* LOCAL_MEM_LOCAL_ADRS */
    return((char *)-1);
#endif /* LOCAL_MEM_LOCAL_ADRS */
    }

/***********************************************************************
*
* smEndFlagsGet - Return optional SMEND flags
*
*  This routine returns the flag bits for optional components
*  used by shared memory.
*
*  RETURNS: the flag bits corresponding to optional
*           shared memory components
*
*  ERRNO: not set
*/

UINT32 smEndFlagsGet()
    {
    UINT32 configFlag = 0;

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

    return(configFlag);
    }

#endif /* INCLUDE_VXBUS_SM_SUPPORT */
