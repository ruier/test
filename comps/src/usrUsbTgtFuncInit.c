/* usrUsbTgtFuncInit.c - Initialization of the usb target function drivers */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,12jul11,s_z  Remove compile warning
01k,30jun11,m_y  Modify routine usrUsbTgtFuncUnInit to avoid prj build fail 
                 (WIND00285476)
01j,22apr11,ljg  Add USBTGT printer and USBTGT keyboard emulators support
01i,21apr11,s_z  Correct the description usrUsbTgtFuncVxbUnRegister routine
01h,30mar11,s_z  Adjust the initialization process to support IAD
01g,22mar11,s_z  Changes for unused routines removed
01f,22mar11,ghs  Correct serial emulator marco check
01e,09mar11,s_z  Code clean up
01d,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01c,04feb11,s_z  Add USB target network function driver support
01b,30jan11,ghs  Add target serial emulator support
01a,12oct10,s_z  created
*/

/*
DESCRIPTION

This file includes the user configuration/un-configuration,
initialization/un-intialization routines which used for the USB target stack.

INCLUDE FILES: usb/usb.h, usb/usbOsalDebug.h, usb/usbTgt.h
*/

#ifndef __INCusrUsbTgtFuncInitc
#define __INCusrUsbTgtFuncInitc

/* includes */

#include <usb/usb.h>
#include <usb/usbOsalDebug.h>
#include <usb/usbTgt.h>


/* Include the function driver source files if needed */

#ifdef INCLUDE_USBTGT_NETWORK
#include "usrUsbTgtNetworkInit.c"
#endif /* INCLUDE_USBTGT_NETWORK */

#ifdef INCLUDE_USBTGT_MSC
#include "usrUsbTgtMscInit.c"
#endif /* INCLUDE_USBTGT_MSC */

#ifdef INCLUDE_USBTGT_SER
#include "usrUsbTgtSerInit.c"
#endif /* INCLUDE_USBTGT_SER */

#ifdef INCLUDE_USBTGT_PRN
#include "usrUsbTgtPrnInit.c"
#endif /* INCLUDE_USBTGT_PRN */

#ifdef INCLUDE_USBTGT_KBD
#include "usrUsbTgtKbdInit.c"
#endif /* INCLUDE_USBTGT_KBD */

/*******************************************************************************
*
* usrUsbTgtFuncUnInit - uninitialize the USB target function drivers
*
* This routine is used to uninitialize the USB target function drivers
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtFuncUnInit
    (
    void
    )
    {
    STATUS status;
    BOOL   bError = FALSE;
    
#ifdef INCLUDE_USBTGT_FUNCDEMO_INIT
    status = usrUsbTgtFuncDemoUnInit();

    if (ERROR == status)
        {
        USB_TGT_ERR("uninit function demo driver fail\n",
                    1, 2, 3, 4, 5, 6);
        
        bError = TRUE;
        }
#endif /* INCLUDE_USBTGT_FUNCDEMO_INIT */

    /* 
     * Add the new Function driver support at here 
     * 
     * e.g.
     * #ifdef INCLUDE_USBTGT_XXX_INIT
     * ...
     * ...
     * #endif
     */

#ifdef INCLUDE_USBTGT_NETWORK_INIT
    status = usrUsbTgtNetworkUnInit();

    if (ERROR == status)
        {
        USB_TGT_ERR("Uninit USB network function driver fail\n",
                    1, 2, 3, 4, 5, 6);
        
        bError = TRUE;
        }
#endif /* INCLUDE_USBTGT_NETWORK_INIT */ 

#ifdef INCLUDE_USBTGT_MSC_INIT
    usrUsbTgtMscUnInit();
#endif /* INCLUDE_USBTGT_MS_INIT */

#ifdef INCLUDE_USBTGT_SER_INIT
    usrUsbTgtSerUnInit();
#endif /* INCLUDE_USBTGT_SER_INIT */

#ifdef INCLUDE_USBTGT_PRN_INIT
    status = usrUsbTgtPrnUnInit();

    if (ERROR == status)
        {
        USB_TGT_ERR("Uninit USB printer function driver fail\n",
                    1, 2, 3, 4, 5, 6);
        bError = TRUE;
        }
#endif /* INCLUDE_USBTGT_PRN_INIT */

#ifdef INCLUDE_USBTGT_KBD_INIT
    status = usrUsbTgtKbdUnInit();

    if (ERROR == status)
        {
        USB_TGT_ERR("Uninit USB keyboard function driver fail\n",
                    1, 2, 3, 4, 5, 6);
        bError = TRUE;
        }
#endif /* INCLUDE_USBTGT_KBD_INIT */

    return (bError ? ERROR : OK);
    }

/*******************************************************************************
*
* usrUsbTgtFuncInit - initializes the usb target fucntion drivers
*
* This routine is used to initialize the usb target fucntion drivers.
* If called automatically by the project facility
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtFuncInit
    (
    void
    )
    {
    BOOL   bError = FALSE;

    /* TODO : Remove the configure interface */
    
#ifdef INCLUDE_USBTGT_FUNCDEMO_INIT
    status = usrUsbTgtFuncDemoInit();

    if (ERROR == status)
        {
        USB_TGT_ERR("Init function demo driver fail\n",
                    1, 2, 3, 4, 5, 6);
        
        bError = TRUE;
        }

#endif /* INCLUDE_USBTGT_FUNCDEMO_INIT */

    /* 
     * Add the new Function driver support at here 
     * 
     * e.g.
     * #ifdef INCLUDE_USBTGT_XXX_INIT
     * ...
     * ...
     * #endif
     */

#ifdef INCLUDE_USBTGT_NETWORK_INIT
    usrUsbTgtNetworkInit();
#endif /* INCLUDE_USBTGT_NETWORK_INIT */ 

#ifdef INCLUDE_USBTGT_SER_INIT
    usrUsbTgtSerInit();
#endif /* INCLUDE_USBTGT_SER_INIT */

#ifdef INCLUDE_USBTGT_MSC_INIT
    usrUsbTgtMscInit();    
#endif /* INCLUDE_USBTGT_MSC_INIT */

#ifdef INCLUDE_USBTGT_PRN_INIT
    usrUsbTgtPrnInit();    
#endif /* INCLUDE_USBTGT_PRN_INIT */

#ifdef INCLUDE_USBTGT_KBD_INIT
    usrUsbTgtKbdInit();    
#endif /* INCLUDE_USBTGT_KBD_INIT */

    return (bError ? ERROR : OK);
    }

/*******************************************************************************
*
* usrUsbTgtFuncVxbUnRegister - unregister all the function drivers from vxBus
*
* This routine is used to unregister all the function drivers from vxBus.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtFuncVxbUnRegister
    (
    void
    )
    {    
#ifdef INCLUDE_USBTGT_NETWORK

#ifdef INCLUDE_USBTGT_VRTL_END
    usrUsbTgtVrtlEndVxbUnRegister();
#endif /* INCLUDE_USBTGT_VRTL_END */ 

#endif
    return;
    }

/*******************************************************************************
*
* usrUsbTgtFuncVxbRegister - register all the function drivers to vxBus
*
* This routine is used to register all the function drivers to vxBus.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtFuncVxbRegister
    (
    void
    )
    {    
#ifdef INCLUDE_USBTGT_NETWORK

#ifdef INCLUDE_USBTGT_VRTL_END
    usrUsbTgtVrtlEndVxbRegister();
#endif /* INCLUDE_USBTGT_VRTL_END */ 

#endif /* INCLUDE_USBTGT_NETWORK */ 
    return;
    }

/*******************************************************************************
*
* usrUsbTgtFuncAdd - add the function driver to the tcd 
*
* This routine is used to add the function driver to the tcd 
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtFuncAdd
    (
    char * pFuncName,
    UINT8  uFuncUnit
    )
    {
    pUSBTGT_FUNC_DRIVER pFuncDriver = NULL;
    pUSBTGT_TCD         pTcd        = NULL;
    STATUS              status;
    
    pFuncDriver = usbTgtFindFuncByName(pFuncName, uFuncUnit);

    if ((NULL == pFuncDriver) ||
        (NULL == pFuncDriver->pTcdName))
        {
        USB_TGT_ERR("Can not find the function which named as %s unit %d\n",
                    pFuncName, uFuncUnit, 3, 4, 5, 6);
        
        return ERROR;
        }

    pTcd = usbTgtFindTcdByName(pFuncDriver->pTcdName , pFuncDriver->uTcdUnit);

    if (NULL == pTcd)
        {
        USB_TGT_ERR("Can not find the right TCD which named as %s unit %d\n",
                    pFuncDriver->pTcdName, 
                    pFuncDriver->uTcdUnit,
                    3, 4, 5, 6);
        
        return ERROR;
        }

    /* Attach the function driver */
    
    status = usbTgtFuncAdd(pFuncDriver);
 
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtFuncRemove - remove one function driver from the attached TCD controller
*
* This routine is used to remove one function driver from the attached TCD controller
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtFuncRemove
    (
    char * pFuncName,
    UINT8  uFuncUnit
    )
    {
    pUSBTGT_FUNC_DRIVER pFuncDriver = NULL;
    pUSBTGT_TCD         pTcd        = NULL;
    STATUS              status;
    
    pFuncDriver = usbTgtFindFuncByName(pFuncName, uFuncUnit);

    if ((NULL == pFuncDriver) ||
        (NULL == pFuncDriver->pTcdName))
        {
        USB_TGT_ERR ("Can not find the function which named as %s unit %d\n",
                     pFuncName, uFuncUnit, 3, 4, 5, 6);
        
        return ERROR;
        }

    pTcd = usbTgtFindTcdByName(pFuncDriver->pTcdName , pFuncDriver->uTcdUnit);

    if (NULL == pTcd)
        {
        USB_TGT_ERR ("Can not find the right TCD which named as %s unit %d\n",
                     pFuncDriver->pTcdName, 
                     pFuncDriver->uTcdUnit,
                     3, 4, 5, 6);
        
        return ERROR;
        }
    
    /* Attach the function driver */
    
    status = usbTgtFuncRemove(pFuncDriver);
 
    return status;
    }

#endif /* __INCusrUsbTgtFuncInitc */
