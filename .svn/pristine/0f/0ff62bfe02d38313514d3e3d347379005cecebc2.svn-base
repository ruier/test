/* usrUsbTgtVrtlEndInit.c - USB Target Virtual END Initialization Module */

/*
 * Copyright (c) 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,06may13,s_z  Remove compiler warning (WIND00356717)
01e,12jul11,s_z  Remove compile warning
01d,10apr11,s_z  Correct the definition of USBTGT_VRTL_END_LOCAL_MAC
                 (WIND00266219)
01c,22mar11,s_z  Changes for unused routines removed
01b,09mar11,s_z  Code clean up
01a,04jan11,s_z  written
*/

/*
DESCRIPTION

This file includes the defination and the routines to configure and initialize
the USB target virtual END driver.

INCLUDE FILES: vxWorks.h, usb/usbOsal.h, usb/usbOsalDebug.h
*/


#ifndef __INCusrUsbTgtVrtlEndInitc
#define __INCusrUsbTgtVrtlEndInitc

/* includes */

#include <vxWorks.h>
#include <usb/usbOsal.h>
#include <usb/usbOsalDebug.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* Default max END device supported 1 */

#ifndef USBTGT_VRTL_END_MAX_COUNT
#define USBTGT_VRTL_END_MAX_COUNT        (1)
#endif  /* USBTGT_VRTL_END_MAX_COUNT */

/* Default device name by the user */

#ifndef USBTGT_VRTL_END_NAME
#define USBTGT_VRTL_END_NAME             ("usbEnd")
#endif  /* USBTGT_VRTL_END_NAME */

/* Default END speed 100M */

#ifndef USBTGT_VRTL_END_SPEED
#define USBTGT_VRTL_END_SPEED            (100) 
#endif  /* USBTGT_VRTL_END_SPEED */

/* Default END local MAC address */

#ifndef USBTGT_VRTL_END_LOCAL_MAC
#define USBTGT_VRTL_END_LOCAL_MAC    {"00:11:22:33:44:00"}
#endif  /* USBTGT_VRTL_END_LOCAL_MAC */

/* Default END host MAC address */

#ifndef USBTGT_VRTL_END_REMOTE_MAC
#define USBTGT_VRTL_END_REMOTE_MAC   {"00:11:22:33:55:00"} 
#endif  /* USBTGT_VRTL_END_REMOTE_MAC */


/* 
 * Default END device type 
 * 0 : Legacy END, not supported for this is vxBus Driver 
 * 1 : VxBus END driver supported
 * 2 : vxBus END2 driver TODO
 * 
 * The default END type is vxBus END driver.
 */

#ifndef USBTGT_VRTL_END_TYPE
#define USBTGT_VRTL_END_TYPE            (1) 
#endif  /* USBTGT_VRTL_END_TYPE */

/* typedefs */

typedef struct usbtgt_vrtl_end_config
    {
    int    speedByM;
    char   localMacAddr[USBTGT_VRTL_END_MAX_COUNT][6];
    char   remoteMacAddr[USBTGT_VRTL_END_MAX_COUNT][6];
    char   endName[USBTGT_MAX_NAME_SZ];
    UINT8  endType;
    UINT8  uEndCount;
    
    }USBTGT_VRTL_END_CONFIG, *pUSBTGT_VRTL_END_CONFIG;

/* globals */

char localMacAddrStr[USBTGT_VRTL_END_MAX_COUNT][USBTGT_MAX_NAME_SZ] 
                     = USBTGT_VRTL_END_LOCAL_MAC;
char remoteMacAddrStr[USBTGT_VRTL_END_MAX_COUNT][USBTGT_MAX_NAME_SZ] 
                     = USBTGT_VRTL_END_REMOTE_MAC;  

/* 
 * Not add the MAC address string in the configuation 
 * It will be easier to used the real data than the string
 */

USBTGT_VRTL_END_CONFIG usrUbTgtVrtlEndConfiguration = 
    {
    USBTGT_VRTL_END_SPEED,         /* speedByM */
    {
        {00}
    },                          /* localMacAddr */
    {
        {00}
    },                          /* remoteMacAddr */
    USBTGT_VRTL_END_NAME,          /* endName */
    USBTGT_VRTL_END_TYPE,          /* endType */
    USBTGT_VRTL_END_MAX_COUNT      /* uEndCount */
    };

/* externels */

IMPORT struct vxbDeviceMethod usbTgtVrtlEndMethods[];
IMPORT struct vxbBusPresent * pPlbBus;

IMPORT STATUS usbTgtVrtlEndDataInit
    (
    struct vxbDev * pDev
    );

/* locals */

LOCAL void usrUsbTgtVrtlEndInstInit(struct vxbDev *);

LOCAL void usrUsbTgtVrtlEndInstInit2(struct vxbDev *);

LOCAL void usrUsbTgtVrtlEndInstConnect(struct vxbDev *);

LOCAL struct drvBusFuncs usbTgtVrtlEndVxbFuncs =
    {
    usrUsbTgtVrtlEndInstInit,   /* devInstanceInit */
    usrUsbTgtVrtlEndInstInit2,  /* devInstanceInit2 */
    usrUsbTgtVrtlEndInstConnect /* devConnect */
    };

/* vxBus registration for pc console driver */

LOCAL struct vxbDevRegInfo usbTgtVrtlEndVxbRegistration[USBTGT_VRTL_END_MAX_COUNT];
LOCAL BOOL usbTgtVrtlEndDevInited = FALSE;

/*******************************************************************************
*
* usrUsbTgtVrtlEndConfigTableGet - get the virtual END configuration table 
* which has been defined.
*
* This routine gets the virtual END configuration table which has 
* been defined and stored in some external interface. Such as the NvRom ,
* external flash, USB disk .etc.The user need add his own overwrite 
* interface in this routine.such as using the <'sysNvRamGet'> interface to 
* update the <'usrUbTgtVrtlEndConfiguration'>
*
* RETURNS: the returned STATUS to get the configuration table by the user 
*          sub-routine. Usually OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndConfigTableGet
    (
    void
    )
    {
    STATUS status = OK;

    /* 
     * Add here the user interface to get the external configuration and
     * reset the <'usrUbTgtVrtlEndConfiguration'>. such as using the sysNvRamGet
     * interface to update the <'usrUbTgtVrtlEndConfiguration'>
     */
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndConfigTableSet - set the virtual END configuration table.
*
* This routine sets the virtual END configuration table.which maybe stored in 
* some external interface. Such as the NvRom , external flash, USB disk .etc.
* The user need add his own overwrite interface in this routine.
*
* RETURNS: the returned STATUS to set the configuration table by the user 
*          sub-routine. Usually OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndConfigTableSet
    (
    pUSBTGT_VRTL_END_CONFIG pConfig
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
     * Call the <'usrUsbTgtVrtlEndConfigTableGet'> to update the 
     * <'usrUbTgtVrtlEndConfiguration'> 
     */   

    usrUsbTgtVrtlEndConfigTableGet();
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndMacAddrValid - check if the MAC address Valid or not
*
* This routine is used to check if the MAC address Valid or not.<'pMacAddr'>
* is the pointer of buffer who record the MAC address to be checked.
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndMacAddrValid 
    (
    char * pMacAddr  /* Buffer to store the MAC address */
    )
    {
    if (NULL == pMacAddr)
        {
        return ERROR;
        }

    /* My Virtual END MAC address should not be all '0' or '0xFF' */
    
    if (((pMacAddr[0] == 0x00) &&
         (pMacAddr[1] == 0x00) &&
         (pMacAddr[2] == 0x00) &&
         (pMacAddr[3] == 0x00) &&
         (pMacAddr[4] == 0x00) &&
         (pMacAddr[5] == 0x00))  ||
        ((pMacAddr[0] == 0xFF) &&
         (pMacAddr[1] == 0xFF) &&
         (pMacAddr[2] == 0xFF) &&
         (pMacAddr[3] == 0xFF) &&
         (pMacAddr[4] == 0xFF) &&
         (pMacAddr[5] == 0xFF)))
        {
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndLocalMacAddrSet - set the END local MAC address which by the
* user configuration.
*
* This routine is used to set the END local MAC address which the user 
* configuration.<'index'> indicates which unit of the END device which named as 
* <'USBTGT_VRTL_END_NAME'>. And the MAC address will be stored in the buffer 
* <'pMacAddr'>.
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndLocalMacAddrSet 
    (
    int    index,     /* The END device unit index */
    char * pMacAddr,  /* Buffer to store the MAC address */
    BOOL   bMacString /* TRUE, one string ; FALSE : real data */
    )
    {
    char*  holder = NULL;
    char   pMacDataOld[6];
    char * pMac;
    char * pTemp = NULL;
    char   pMacData[6];
    int    i;
   
    if ((NULL == pMacAddr) ||
        (index >= USBTGT_VRTL_END_MAX_COUNT))
        {
        return ERROR;
        }
    
    bcopy(&usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
          pMacDataOld,
          6);

    if (bMacString)
        {
        holder = pMacAddr;
        
        for (i = 0; i < 6; i ++)
            {
            pMac = strtok_r (NULL, ":",&holder );
            if (pMac != NULL)
                pMacData[i] = (char)strtol (pMac, &pTemp, 16);
            }
        
        if (OK == usrUsbTgtVrtlEndMacAddrValid(pMacData))
            {
            /* Update the configuration */
            
            bcopy(pMacData,
                  &usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
                  6);

            if (ERROR == usrUsbTgtVrtlEndConfigTableSet(&usrUbTgtVrtlEndConfiguration))
                {
                bcopy(pMacDataOld, 
                      &usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
                      6);
                
                return ERROR;
                }         
            }
        }
    else
        {
        /* The pBuf is the real MAC address data */

        bcopy(pMacAddr, 
              &usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
              6);
        
        if (ERROR == usrUsbTgtVrtlEndConfigTableSet(&usrUbTgtVrtlEndConfiguration))
            {
            bcopy(pMacDataOld, 
                  &usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
                  6);
            
            return ERROR;
            }         
        }
        
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndLocalMacAddrGet - get the END local MAC address which by the
* user configuration.
*
* This routine is used to get the END local MAC address which the user 
* configuration. <'index'> indicates which unit of the END device which named as 
* <'USBTGT_VRTL_END_NAME'>. And the MAC address will be stored in the buffer 
* <'pMacAddr'>.
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndLocalMacAddrGet 
    (
    int    index,    /* The END device unit index */
    char * pMacAddr  /* Buffer to store the MAC address */
    )
    {
    char * pMac;
    char * holder = NULL;
    char * pTemp = NULL;
    char   pMacData[6];
    int    i;
    
    if ((NULL == pMacAddr) ||
        (index >= USBTGT_VRTL_END_MAX_COUNT))
        {
        return ERROR;
        }

    /* If the Mac address is valided */

    if (OK == usrUsbTgtVrtlEndMacAddrValid(
              &usrUbTgtVrtlEndConfiguration.localMacAddr[index][0]))
        {
        
        bcopy(&usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
              pMacAddr, 
              6);

        return OK;
        }
    else 
        {
        /* Get the remote Mac address string */
        
        holder = localMacAddrStr [index];
        
        for (i = 0; i < 6; i ++)
            {
            pMac = strtok_r (NULL, ":",&holder );
            if (pMac != NULL)
                pMacData[i] = (char)strtol (pMac, &pTemp, 16);
            }
        
        if (OK == usrUsbTgtVrtlEndMacAddrValid(pMacData))
            {
            /* Update the configuration */
            
            bcopy(pMacData,
                  &usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
                  6);
            
            bcopy(&usrUbTgtVrtlEndConfiguration.localMacAddr[index][0],
                  pMacAddr, 
                  6);

            return OK;
            }
        }
    
    return ERROR;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndRemoteMacAddrSet - set the END remote MAC address which by the
* user configuration.
*
* This routine is used to set the END remote MAC address which the user 
* configuration.<'index'> indicates which unit of the END device which named as 
* <'USBTGT_VRTL_END_NAME'>. And the MAC address will be stored in the buffer 
* <'pMacAddr'>.
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndRemoteMacAddrSet 
    (
    int    index,    /* The END device unit index */
    char * pMacAddr, /* Buffer to store the MAC address */
    BOOL   bMacString /* The pMacAddress is string or the real Data */
    )
    {
    char*  holder = NULL;
    char   pMacDataOld[6];
    char * pMac;
    char * pTemp = NULL;
    char   pMacData[6];
    int    i;
   
    if ((NULL == pMacAddr) ||
        (index >= USBTGT_VRTL_END_MAX_COUNT))
        {
        return ERROR;
        }
    
    bcopy(&usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
          pMacDataOld,
          6);

    if (bMacString)
        {
        holder = pMacAddr;
        
        for (i = 0; i < 6; i ++)
            {
            pMac = strtok_r (NULL, ":",&holder );
            if (pMac != NULL)
                pMacData[i] = (char)strtol (pMac, &pTemp, 16);
            }
        
        if (OK == usrUsbTgtVrtlEndMacAddrValid(pMacData))
            {
            /* Update the configuration */
            bcopy(pMacData,
                  &usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
                  6);

            if (ERROR == usrUsbTgtVrtlEndConfigTableSet(&usrUbTgtVrtlEndConfiguration))
                {
                bcopy(pMacDataOld, 
                      &usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
                      6);
                
                return ERROR;
                }         
            }
        }
    else
        {
        /* The pBuf is the real MAC address data, good */

        bcopy(pMacAddr, 
              &usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
              6);
        
        if (ERROR == usrUsbTgtVrtlEndConfigTableSet(&usrUbTgtVrtlEndConfiguration))
            {
            bcopy(pMacDataOld, 
                  &usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
                  6);
            
            return ERROR;
            }         
        }
        
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndRemoteMacAddrGet - get the END remote MAC address which by the
* user configuration.
*
* This routine is used to get the END remote MAC address which the user 
* configuration. <'index'> indicates which unit of the END device which named as 
* <'USBTGT_VRTL_END_NAME'>. And the MAC address will be stored in the buffer 
* <'pMacAddr'>.
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndRemoteMacAddrGet 
    (
    int    index,    /* The END device unit index */
    char * pMacAddr  /* Buffer to store the MAC address */
    )
    {
    char * pMac;
    char * holder = NULL;
    char * pTemp = NULL;
    char   pMacData[6];
    int    i;
    
    if ((NULL == pMacAddr) ||
        (index >= USBTGT_VRTL_END_MAX_COUNT))
        {
        return ERROR;
        }

    /* If the Mac address is valided */

    if (OK == usrUsbTgtVrtlEndMacAddrValid(
              &usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0]))
        {
        
        bcopy(&usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
              pMacAddr, 
              6);

        return OK;
        }
    else 
        {
        /* Get the remote Mac address string */
        
        holder = remoteMacAddrStr[index];
        
        for (i = 0; i < 6; i ++)
            {
            pMac = strtok_r (NULL, ":",&holder );
            if (pMac != NULL)
                pMacData[i] = (char)strtol (pMac, &pTemp, 16);
            }
        if (OK == usrUsbTgtVrtlEndMacAddrValid(pMacData))
            {
            /* Update the configuration */
            
            bcopy(pMacData,
                  &usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
                  6);
            
            bcopy(&usrUbTgtVrtlEndConfiguration.remoteMacAddr[index][0],
                  pMacAddr, 
                  6);

            return OK;
            }
        }

    return ERROR;
    }


/*******************************************************************************
*
* usrUsbTgtVrtlEndNameGet - get the virtual END name which the user configured.
*
* This routine is used to get the virtual END name which the user configured.
* 
* RETURNS: NULL or the virtual END name.
*
* ERRNO: N/A
*/

char * usrUsbTgtVrtlEndNameGet 
    (
    void 
    )
    {
    /* Return the END name */

    return usrUbTgtVrtlEndConfiguration.endName;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndNameSet - set the END device name by the user.
*
* This routine is used to set the END device name by the user.The user need 
* un-register/re-register the driver to make this parameter changed.
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtVrtlEndNameSet 
    (
    char * pName
    )
    {
    char pNameOld[USBTGT_MAX_NAME_SZ];
    char pNameNew[USBTGT_MAX_NAME_SZ];
    
    if (NULL == pName)
        return ERROR;

    strncpy(pNameOld, usrUbTgtVrtlEndConfiguration.endName, 
            USBTGT_MAX_NAME_SZ - 1);

    /* 
     * The function driver name length should less than 
     * USBTGT_MAX_NAME_SZ 
     */

    if (strlen(pName) >= USBTGT_MAX_NAME_SZ)
        {
        memcpy(pNameNew, pName, USBTGT_MAX_NAME_SZ - 2);
        pNameNew[USBTGT_MAX_NAME_SZ - 2] = '\0';
        strncpy(usrUbTgtVrtlEndConfiguration.endName, pNameNew, 
               USBTGT_MAX_NAME_SZ - 1);
        }
    else
        {    
        strncpy(usrUbTgtVrtlEndConfiguration.endName, pName, 
                USBTGT_MAX_NAME_SZ - 1);
        }
    
    if (ERROR == usrUsbTgtVrtlEndConfigTableSet(&usrUbTgtVrtlEndConfiguration))
        {
        strncpy(usrUbTgtVrtlEndConfiguration.endName,
                pNameOld, USBTGT_MAX_NAME_SZ - 1);
        return ERROR;
        }
    
    return OK; 
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndSpeedGet - get the END speed which the user configured.
*
* This routine is used to get the END speed which the user configured.The user 
* configure the speed by Mbps, this routine translate the speed to be use. 
*
* RETURNS: the virtual END speed value
*
* ERRNO: N/A
*/

int usrUsbTgtVrtlEndSpeedGet 
    (
    int index
    )
    {
    return (usrUbTgtVrtlEndConfiguration.speedByM * 1000 * 1000);
    }

/*******************************************************************************
*
* usrUsbTgtEndVrtlSpeedSet - set the END speed by the user
*
* This routine is used to set the END speed by the user. The speed is configured
* by Mbsp. (such as 10 means 10Mbps)
*
* RETURNS: OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtEndVrtlSpeedSet 
    (
    int speed
    )
    {
    int speedOld = usrUbTgtVrtlEndConfiguration.speedByM;
    
    usrUbTgtVrtlEndConfiguration.speedByM = speed;

    if (ERROR == usrUsbTgtVrtlEndConfigTableSet(&usrUbTgtVrtlEndConfiguration))
        {
        usrUbTgtVrtlEndConfiguration.speedByM = speedOld;
        return ERROR;
        }
    
    return OK; 
    }

/*****************************************************************************
*
* usrUsbTgtVrtlEndInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler for the USB virtual END
* device instance.The only thing done here is to select a unit
* number for the device.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void usrUsbTgtVrtlEndInstInit
    (
    struct vxbDev * pDev
    )
    {    
    vxbNextUnitGet (pDev);
    }

/*****************************************************************************
*
* usrUsbTgtVrtlEndInstInit2 - VxBus instInit2 handler 
*
* This function implements the VxBus instInit handler for the USB virtual END
* device instance.It is safe to create the device structure and get the END 
* configuration by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void usrUsbTgtVrtlEndInstInit2
    (
    struct vxbDev * pDev
    )
    {
    /* Validate parameters */
    
    if (NULL == pDev)
        {
        return;
        }

    /* Create the END data structure */
    
    usbTgtVrtlEndDataInit(pDev);

    return;
    }

/*****************************************************************************
*
* usrUsbTgtVrtlEndInstConnect -  VxBus instConnect handler
*
* This function implements the VxBus instConnect handler for the USB virtual END
* device instance. Nothing to do here.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void usrUsbTgtVrtlEndInstConnect
    (
    struct vxbDev * pDev
    )
    {
    
    /* Nothing to do */

    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndVxbAnnounce - announce the USB virtual END device to vxBus
*
* This routine announces the USB virtual END device to vxBus, alloc a device
* struct and announce this device to vxBus
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtVrtlEndVxbAnnounce
    (
    int     usbTgtVrtlEndDevId,
    char *  devName
    )
    {
    struct vxbDev * pDev;

    pDev = vxbDevStructAlloc(WAIT_FOREVER);

    pDev->pParentBus = pPlbBus;

    pDev->pMethods = &usbTgtVrtlEndMethods[0];

    /* The device name is same as driver name register in vxBus */

    pDev->pName = devName;

    pDev->busID = VXB_BUSID_PLB;

    /* Announce it */

    (void) vxbDeviceAnnounce(pDev);

    return;
    }
    
/*******************************************************************************
*
* usrUsbTgtVrtlEndVxbRegister - register the USB virtual END device driver to 
* vxBus
*
* This routine registers the USB virtual END device driver to vxBus. This driver
* is a PLB device driver, driver name come from user configure. After
* register the driver immediately, the routine call
* <'usbTgtVrtlEndVxbAnnounce()'> to announce this device.
*
* Note: normally, the vxBus END driver register routine will be called in 
* <'hardWareInterFaceBusInit'>, and the END devices will bind to the network
* stack by the network initialization routines.But if the END driver registered 
* to vxBus later than the network stack intialization routine. The device can 
* not connect to the MUX. So, <'usrUsbTgtVrtlEndConnect'> need be call to 
* connect the device to MUX.
* 
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtVrtlEndVxbRegister
    (
    void
    )
    {
    int     devId;
    char    devName[MAX_DRV_NAME_LEN + 1] = {0};

    if (usbTgtVrtlEndDevInited == TRUE)
        {
        return;
        }
    
    usbTgtVrtlEndDevInited = TRUE;

    for (devId = 0; devId < usrUbTgtVrtlEndConfiguration.uEndCount; devId++)
        {
        usbTgtVrtlEndVxbRegistration[devId].devID = VXB_DEVID_DEVICE;
        usbTgtVrtlEndVxbRegistration[devId].busID = VXB_BUSID_PLB;
        usbTgtVrtlEndVxbRegistration[devId].vxbVersion = USB_VXB_VERSIONID;
        usbTgtVrtlEndVxbRegistration[devId].pDrvBusFuncs = &usbTgtVrtlEndVxbFuncs;
            
        strncpy(devName, usrUbTgtVrtlEndConfiguration.endName, MAX_DRV_NAME_LEN);

        devName[MAX_DRV_NAME_LEN - 1] = '\0';

        strncpy(usbTgtVrtlEndVxbRegistration[devId].drvName, devName, MAX_DRV_NAME_LEN);

        /* Register it */

        vxbDevRegister(&usbTgtVrtlEndVxbRegistration[devId]);

        /* Announce the devcie */
        
        usrUsbTgtVrtlEndVxbAnnounce(devId, usbTgtVrtlEndVxbRegistration[devId].drvName); 
        }
    
    return;
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndConnect - connect the END to the MUX
*
* This routine connects the END device to the MUX. Normally, the END device will
* be connected to the MUX automatically by the intialization routine of network
* stack. But if the END driver registered to vxBus later than the network stack
* intialization routine. The connection will not be called. So, this API 
* defined to used for the case which <'usrUsbTgtVrtlEndVxbRegister'> called
* after the network stack intialization.
*
* NOTE: When this routine be called, you may still can not see the device by
* <'ifconfig -a'>. <'ipAttach'> still be need to attach the ip stack.Eg.
* ipAttach (0, "usbEnd").
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtVrtlEndConnect
    (
    void
    )
    {
    vxbDevMethodRun((VXB_METHOD_ID)&muxDevConnect_desc, NULL);
    }

/*******************************************************************************
*
* usrUsbTgtVrtlEndVxbUnRegister - un-register the virtual END driver.
*
* This routine un-registers the virtual END driver.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtVrtlEndVxbUnRegister
    (
    void
    )
    {
    int     devId;
    
    if (usbTgtVrtlEndDevInited == FALSE)
        {
        return;
        }
    
    usbTgtVrtlEndDevInited = FALSE;

    for (devId = 0; devId < usrUbTgtVrtlEndConfiguration.uEndCount; devId++)
        {
        vxbDriverUnregister ((struct vxbDevRegInfo *)
                             &usbTgtVrtlEndVxbRegistration[devId]);
        }

    return;
    }

#ifdef __cplusplus
}
#endif

#endif  /* __INCusrUsbTgtVrtlEndInitc */


