/* usrUsbTgtNetworkInit.c - User Initialization Of The USB Network Function Driver */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,12jul11,s_z  Remove compile warning
01e,28mar11,s_z  Change the default value to synch up with cdf
01d,22mar11,s_z  Changes for unused routines removed
01c,09mar11,s_z  Code clean up
01b,23feb11,s_z  Change the default binder buffer count
01a,04jan11,s_z  written
*/

/*
DESCRIPTION

This file includes the definations and the routines to configure the USB 
network function driver.

INCLUDE FILES: vxWorks.h, usb/usb.h, usb/usbTgt.h, usb/usbTgtRndis.h
               usb/tools/cmdParser.h

*/

/* includes */


#ifndef __INCusrUsbTgtNetworkInitc
#define __INCusrUsbTgtNetworkInitc

#include <vxWorks.h>
#include <usb/usb.h>
#include <usb/usbTgt.h>
#include <usb/usbTgtRndis.h>
#include <usb/tools/cmdParser.h>

/* Include the source files if needed */

#ifdef INCLUDE_USBTGT_VRTL_END
#include <usrUsbTgtVrtlEndInit.c>
#endif /* INCLUDE_USBTGT_VRTL_END */

#ifdef INCLUDE_USBTGT_RNDIS_1
#include <usrUsbTgtRndisInit.c>
#endif /* INCLUDE_USBTGT_RNDIS */


#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#   ifndef USBTGT_NETWORK_DATA_POOL_SIZE
#   define USBTGT_NETWORK_DATA_POOL_SIZE             (1560)
#   endif

#   ifndef USBTGT_NETWORK_DATA_POOL_COUNT
#   define USBTGT_NETWORK_DATA_POOL_COUNT            (8)
#   endif


/* typedefs */

typedef struct usr_usbtgt_network_config
    {
    int dataPoolSize;         /* The binder buffer data pool size */
    int dataPoolCount;        /* The binder buffer data pool count */
    }USR_USBTGT_NETWORK_CONFIG, *pUSR_USBTGT_NETWORK_CONFIG;

/* globals */

USR_USBTGT_NETWORK_CONFIG usrUsbTgtNetworkConfigTable = 
    {
    USBTGT_NETWORK_DATA_POOL_SIZE,
    USBTGT_NETWORK_DATA_POOL_COUNT
    };

/*******************************************************************************
*
* usrUsbTgtNetworkConfigTableGet - get the network configuration table which has 
* been defined.
*
* This routine gets the network configuration table, which has 
* been defined and stored in some external interface. Such as NvRam,
* external flash, USB disk and etc. The user need add his own overwrite 
* interface in this routine, such as using the <'sysNvRamGet'> interface to 
* update the <'usrUsbTgtNetworkConfigTable'>
*
* RETURNS: the status to get the global configuration table.
*          OK, if no user's subroutine added here
*          STATUS, with the user's subrouine's return.
*
* ERRNO: N/A
*/

STATUS usrUsbTgtNetworkConfigTableGet
    (
    void
    )
    {
    STATUS status = OK;

    /* 
     * Add here the user interface to get the external configuration and
     * reset the <'usrUsbTgtNetworkConfigTable'>. such as using the sysNvRamGet
     * interface to update the <'usrUsbTgtNetworkConfigTable'>
     */
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtNetworkConfigTableSet - set the network configuration table.
*
* This routine sets the network configuration table, which maybe stored in 
* some external interface. Such as NvRam, external flash, USB disk and etc.
* The user need add his own overwrite interface in this routine.
*
* RETURNS: OK or ERROR if there is something wrong
*          STATUS, with the user's subrouine's return.
*
* ERRNO: N/A
*/

STATUS usrUsbTgtNetworkConfigTableSet
    (
    pUSR_USBTGT_NETWORK_CONFIG pConfig
    )
    {
    if (NULL == pConfig)
        {
        return ERROR;
        }

    /* 
     * Here
     * Add the subroutine to set the config table if needed, such as 
     * sysNvRamSet to store the configuration table in the NvRam.
     */

    /* 
     * Call the <'usrUsbTgtNetworkConfigTableGet'> to update the 
     * <'usrUsbTgtNetworkConfigTable'> 
     */   

    usrUsbTgtNetworkConfigTableGet();
    
    return OK;
    }


/*******************************************************************************
*
* usrUsbTgtNetworkDataPoolSizeSet - set the network binder buffer data pool 
* size which used by the USB interface and the medium interface.
*
* This routine sets the network binder buffer data pool size which 
* used by the USB interface and the medium interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtNetworkDataPoolSizeSet
    (
    int   size
    )
    {
    int sizeOld = usrUsbTgtNetworkConfigTable.dataPoolSize;

    usrUsbTgtNetworkConfigTable.dataPoolSize = size;

    if (ERROR == usrUsbTgtNetworkConfigTableSet(&usrUsbTgtNetworkConfigTable))
        {
        usrUsbTgtNetworkConfigTable.dataPoolSize = sizeOld;

        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtNetworkDataPoolSizeGet - get the network binder buffer data pool 
* size which used by the USB interface and the medium interface.
*
* This routine gets the network binder buffer data pool size which 
* used by the USB interface and the medium interface.
*
* RETURNS: the data pool size
*
* ERRNO: N/A
*/

int usrUsbTgtNetworkDataPoolSizeGet
    (
    void
    )
    {    
    return usrUsbTgtNetworkConfigTable.dataPoolSize;    
    }

/*******************************************************************************
*
* usrUsbTgtNetworkDataPoolCountSet - set the network data pool buffer count
*
* This routine sets the network data pool count which 
* used by the USB interface and the medium interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtNetworkDataPoolCountSet
    (
    int   count
    )
    {
    int countOld = usrUsbTgtNetworkConfigTable.dataPoolCount;

    usrUsbTgtNetworkConfigTable.dataPoolCount = count;

    if (ERROR == usrUsbTgtNetworkConfigTableSet(&usrUsbTgtNetworkConfigTable))
        {
        usrUsbTgtNetworkConfigTable.dataPoolCount = countOld;

        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtNetworkDataPoolCountGet - get the network data pool count 
*
* This routine gets the network data pool count which 
* used by the USB interface and the medium interface.
*
* RETURNS: the data pool count
*
* ERRNO: N/A
*/

int usrUsbTgtNetworkDataPoolCountGet
    (
    void
    )
    {
    return usrUsbTgtNetworkConfigTable.dataPoolCount;
    }

/*******************************************************************************
*
* usrUsbTgtNetworkInit - initialize the network function drivers
*
* This routine intializes the network function drivers by calling the
* the submodule initialization routine.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtNetworkInit
    (
    void
    )
    {
#ifdef INCLUDE_USBTGT_RNDIS_INIT
    usrUsbTgtRndisInit();
#endif /* INCLUDE_USBTGT_RNDIS_INIT */ 

    return ;
    }

/*******************************************************************************
*
* usrUsbTgtNetworkUnInit - uninitialize the network function drivers
*
* This routine unintializes the network function drivers by calling the
* the submodule uninitialization routine.
*
* RETURNS: OK, or ERROR if there is something wrong.
*
* ERRNO: N/A
*/

STATUS usrUsbTgtNetworkUnInit
    (
    void
    )
    {
    STATUS status = OK;
    
#ifdef INCLUDE_USBTGT_RNDIS_INIT
    status = usrUsbTgtRndisUnInit();
#endif /* INCLUDE_USBTGT_RNDIS_INIT */ 

    return status;
    }

#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsbTgtNetworkInitc */

