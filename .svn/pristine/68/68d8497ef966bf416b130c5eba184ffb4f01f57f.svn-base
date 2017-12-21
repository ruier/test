/* usrUsbTgtInit.c - Initialization of the usb target */

/*
 * Copyright (c) 2010-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,06may13,s_z  Remove compiler warning (WIND00356717)
01l,03sep12,s_z  Fix BOS buffer using issue (WIND00374269)
01k,16may12,s_z  Add the USB 3.0 target support (WIND00306012)
01j,12jul11,s_z  Remove compile warning and add IAD support by default
01i,21apr11,s_z  Correct the description vxbUsbTcdUnRegister routine
01h,30mar11,s_z  Add the multiple languages support
01g,28mar11,s_z  Remove unused definition and routines
01f,22mar11,s_z  Changes for unused routines removed
01e,09mar11,s_z  Code clean up
01d,23feb11,s_z  Add IAD descriptor supported parameter
01c,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01b,04feb11,s_z  Add user interface to use external configuration
01a,12oct10,s_z  created
*/

/*
DESCRIPTION

This file includes the definations and the routines to configure the USB 
target stack driver.

This file also includes the usrUsbTgtTcdInit.c and usrUsbTgtFuncInit.c source 
files to initialize the TCD controllers and function drivers.

INCLUDE FILES: usb/usb.h, usb/usbTgt.h

*/

#ifndef __INCusrUsbTgtInitc
#define __INCusrUsbTgtInitc

/* includes */

#include <usb/usb.h>
#include <usb/usbTgt.h>

/* Add the initialization entry of TCDs and function drivers */

#include "usrUsbTgtTcdInit.c"
#include "usrUsbTgtFuncInit.c"

/* defines */


#ifndef USBTGT_IAD_DESC_ENABLE
#   define USBTGT_IAD_DESC_ENABLE               (TRUE) 
#endif /* USBTGT_IAD_DESC_ENABLE */

/*
 * USBTGT_DEV_DESC_FUNC_ENABLE value:
 * 
 * TRUE:
 * The USB target stack will get the device descriptor from the function driver,
 * using the function name and unit number which defined by 
 * USBTGT_DEV_DESC_OF_FUNC_NAME and USBTGT_DEV_DESC_OF_FUNC_UNIT.
 * If the target stack find the function driver, and the function driver dose
 * has one device descriptor, the target stack will use it as the device's 
 * device descriptor.
 *
 * FLASE:
 * The target stack will using the devcie descritpor defined in the 
 * configuration table (usrUsbTgtConfigTable). And the user can set his 
 * configuration table's value by adding his own sub-routine in 
 * usrUsbTgtConfigTableGet() and usrUsbTgtConfigTableSet(). In this way,
 * the target stack can get the configuration which stored in the external 
 * devices, such as NvRAM, EEPROM, or external flash and etc.
 * If the user do not adding his sub-routine in the two routines, the target
 * stack will using the default devcie descriptors, which also can be update
 * by the paramenters configured by TCD. (Such as the VID/PID of the devcie
 * descriptor).
 */

#ifndef USBTGT_DEV_DESC_FUNC_ENABLE
#   define USBTGT_DEV_DESC_FUNC_ENABLE          (FALSE) 
#endif /* USBTGT_DEV_DESC_FUNC_ENABLE */

#ifndef USBTGT_DEV_DESC_OF_FUNC_NAME
#   define USBTGT_DEV_DESC_OF_FUNC_NAME         ("") 
#endif /* USBTGT_DEV_DESC_OF_FUNC_NAME */

#ifndef USBTGT_DEV_DESC_OF_FUNC_UNIT
#   define USBTGT_DEV_DESC_OF_FUNC_UNIT         (0) 
#endif /* USBTGT_DEV_DESC_OF_FUNC_UNIT */

/* globals */

/* The max kinds of language types to be supported */

#define USBTGT_MAX_LANGID_COUNT 0x8

/* 
 * The user's language descriptor structure to support mulitple
 * languages.
 * To be simple, we only support 8 more kinds of languages.
 * The user can change the definition of USBTGT_MAX_LANGID_COUNT
 * to support more.
 */
 
typedef struct usr_usbtgt_lang_desc
    {
    UINT8  length;               /* bLength */
    UINT8  descriptorType;       /* bDescriptorType */
    UINT16 uLangId[USBTGT_MAX_LANGID_COUNT];
    }USR_USBTGT_LANG_DESC;


/*
 * In the stack, we support ENGLISH language as the default string language
 * type.
 * 
 * If the user want to use other kind of language.
 * Change the USBTGT_UNICODE_ENGLISH to the language type's unicode,
 * and corresponding strings to support other language.
 * Such as changing the manufacture string, product string to Germany which 
 * defined for every TCD, and function drivers interface string which defined 
 * in the right function driver.
 * 
 * If the user want to support multiple languages.
 * Add your language IDs in the <'gUsbTgtLangIDs'> table, 
 * and add your sub-routine interface to get the specific string descriptors
 * in <'usrUsbTgtStringDescGet()'> and <'usrUsbTgtStringDescSet()'> routines.
 * And in this way, you can get/set the descriptors which you defined even 
 * stored in NvRam, EEPROM or external flash.
 */

LOCAL UINT16 gUsbTgtLangIDs[] =
    {
    TO_LITTLEW (USBTGT_UNICODE_ENGLISH)
    /* Add the user's language ID supported */
    };

#define USBTGT_LANGID_SUPPORTED_COUNT NELEMENTS(gUsbTgtLangIDs)

/* The defualt language descripor */

LOCAL USR_USBTGT_LANG_DESC gUsbTgtDefaultLangDescr;

/* The default device descriptor */

LOCAL USB_DEVICE_DESCR usbTgtDefaultDevDescr = 
    {
    USB_DEVICE_DESCR_LEN,                   /* bLength */
    USB_DESCR_DEVICE,                       /* bDescriptorType */
    TO_LITTLEW (USBTGT_VERSION_SUPPORTED),  /* bcdUsb */
    0,                                      /* bDeviceClass */
    0,                                      /* bDeviceSubclass */
    0,                                      /* bDeviceProtocol */
    USB_MIN_CTRL_PACKET_SIZE,               /* maxPacketSize0 */
    0,                                      /* idVendor */
    0,                                      /* idProduct */
    0,                                      /* bcdDevice */
    0,                                      /* iManufacturer */
    0,                                      /* iProduct */
    0,                                      /* iSerialNumber */
    USBTGT_MIN_CONFIG_NUM                   /* bNumConfigurations */
    };

/* The default configuration descriptor */

LOCAL USB_CONFIG_DESCR usbTgtDefalutConfigDescr =     
    {
    USB_CONFIG_DESCR_LEN,                   /* bLength */
    USB_DESCR_CONFIGURATION,                /* bDescriptorType */
    TO_LITTLEW (USB_CONFIG_DESCR_LEN),      /* wTotalLength */
    0,                                      /* bNumInterfaces */
    USBTGT_MIN_CONFIG_NUM,                  /* bConfigurationValue */
    0,                                      /* iConfiguration */
    0x80 | USB_ATTR_SELF_POWERED,           /* bmAttributes */
    0                                       /* MaxPower */
    };

/* 
 * Binary device object store descriptors
 * Includes
 * - the BOS descriptor, 
 * - USB2.0 Extension descriptor
 * - SuperSpeed USB Device Capability descriptor
 */

LOCAL UINT8 pUsbTgtBosDesBuf[] = 
    {
    /*
     * BOS descripor
     * contains 2 following descriptors,
     * with the total length 0x16 
     */
     
    0x05, 0x0F, 0x16, 0x00, 0x02,

    /*
     * USB 2.0 Extension descriptor
     * support LPM for USB 2.0 speed
     */
     
    0x07, 0x10, 0x02, 0x02, 0x00, 0x00, 0x00,
    
    /*
     * SuperSpeed USB Device Capability descriptor
     * support LPM for USB 2.0 speed
     * - Support Full, High, Super speed
     * - The lowest speed is Full
     */
     
    0x0A, 0x10, 0x03, 0x00, 0x0E, 0x00, 0x02, 0x0A, 0xFF, 0x07
    };

/*
 * Please refor to chapter 9.6.7 in USB 2.0 Spec.
 * The strings in the USB device may support multiple languages.
 * The language descriptor contains the array of the language UNICODE
 * which can supported by the devices.
 * eg.
 * Offset  Field          Size    Value       Description
 *  0      bLength         1       N+2        Size of this descriptor in bytes
 *  1      bDescriptorType 1       Constant   STRING Descriptor Type
 *  2      wLANGID[0]      2       Number     LANGID code zero
 * ... ... ... ... ...
 *  N      wLANGID[x]      2       Number     LANGID code x
 *
 * The user can set the language descriptor to configurate the LANGID by
 * the routine usrUsbTgtLangDescrSet. If not, the default language descriptor
 * will be used to support the ENGLISH language.
 */

typedef struct usr_usbTgt_config_table
    {
    USB_DEVICE_DESCR *  pDevDesc;
    USB_CONFIG_DESCR *  pConfigDesc;
    USR_USBTGT_LANG_DESC * pLanguageDesc;
    UINT8 *                pBosDesBuf;
    char  devDescOfFuncName[USBTGT_MAX_NAME_SZ];/* The function name who provides the device desc */
    UINT8 uDevDescOfFuncUnit;    /* The function unit who provides the device desc */
    BOOL  bUseFuncDevDesc;       /* Use the funcion driver's devcie desc */
    BOOL  bIadDescEnable;        /* Add the IAD descriptor to the configuration desc */          
    }USR_USBTGT_CONFIG_TABLE, *pUSR_USBTGT_CONFIG_TABLE;

/* The default configure table */

USR_USBTGT_CONFIG_TABLE usrUsbTgtConfigTable =
    {
    &usbTgtDefaultDevDescr,               /* pDevDesc */
    &usbTgtDefalutConfigDescr,            /* pConfigDesc */
    &gUsbTgtDefaultLangDescr,             /* pLanguageDesc */
    pUsbTgtBosDesBuf,                     /* pBosDesc */
    USBTGT_DEV_DESC_OF_FUNC_NAME,         /* devDescOfFuncName */ 
    USBTGT_DEV_DESC_OF_FUNC_UNIT,         /* devDescOfFuncUnit */ 
    USBTGT_DEV_DESC_FUNC_ENABLE,          /* bUseFuncDevDesc */
    USBTGT_IAD_DESC_ENABLE                /* bIadDescEnable */
    }; 

/*******************************************************************************
*
* usrUsbTgtConfigTableGet - get the usb target configuration table which has 
* been defined.
*
* This routine gets the usb target configuration table which has 
* been defined and stored in some external interface. Such as the NvRam,
* external flash, USB disk and etc. The user need add his own overwrite 
* interface in this routine.such as using the <'sysNvRamGet'> interface to 
* update the <'usrUsbTgtConfigTable'>
*
* RETURNS: OK, ERROR, or other status returned from the user's interface
*
* ERRNO: N/A
*/

STATUS usrUsbTgtConfigTableGet
    (
    UINT8                    cmd,
    pUSR_USBTGT_CONFIG_TABLE pConfig,
    void *                   pBuf
    )
    {
    STATUS status = OK;

    /* 
     * Add here the user interface to get the external configuration and
     * reset the <'usrUsbTgtConfigTable'>. such as using the sysNvRamGet
     * interface to update the <'usrUsbTgtConfigTable'>
     */
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtConfigTableSet - set the usb target configuration table.
*
* This routine sets the usb target configuration table, which maybe stored in 
* some external interface. Such as the NvRam, external flash, USB disk and etc.
* The user need add his own overwrite interface in this routine.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtConfigTableSet
    (
    pUSR_USBTGT_CONFIG_TABLE pConfig
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
     * Call the <'usrUsbTgtConfigTableGet'> to update the 
     * <'usrUsbTgtConfigTable'> 
     */   

    usrUsbTgtConfigTableGet(0, &usrUsbTgtConfigTable, NULL);
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtIadDescEnableGet - check if the IAD descriptor supported or not
*
* This routine checks if the IAD descriptor supported or not.
*
* RETURNS: TRUE or FALSE
*
* ERRNO: N/A
*/

BOOL usrUsbTgtIadDescEnableGet
    (
    void
    )
    {
    return usrUsbTgtConfigTable.bIadDescEnable;
    }

/*******************************************************************************
*
* usrUsbTgtIadDescEnableSet - set if the IAD descriptor supported or not
*
* This routine sets if the IAD descriptor supported or not.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtIadDescEnableSet
    (
    BOOL bIadEnabled
    )
    {
    BOOL bIadEOld = usrUsbTgtConfigTable.bIadDescEnable;

    usrUsbTgtConfigTable.bIadDescEnable = bIadEnabled;

    if (ERROR == usrUsbTgtConfigTableSet(&usrUsbTgtConfigTable))
        {
        usrUsbTgtConfigTable.bIadDescEnable = bIadEOld;

        return ERROR;
        }
    
    return OK;    
    }

/*******************************************************************************
*
* usrUsbTgtDevDescFuncEnableGet - check if the external descriptor or not
*
* This routine checks if the external descriptor or not
*
* RETURNS: TRUE or FALSE
*
* ERRNO: N/A
*/

BOOL usrUsbTgtDevDescFuncEnableGet
    (
    void
    )
    {
    return usrUsbTgtConfigTable.bUseFuncDevDesc;
    }

/*******************************************************************************
*
* usrUsbTgtDevDescFuncEnableSet - set use the external descriptor or not
*
* This routine sets use the external descriptor or not
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtDevDescFuncEnableSet
    (
    BOOL bUseFuncDesc
    )
    {
    BOOL bUseFuncDescOld = usrUsbTgtConfigTable.bUseFuncDevDesc;

    usrUsbTgtConfigTable.bUseFuncDevDesc = bUseFuncDesc;

    if (ERROR == usrUsbTgtConfigTableSet(&usrUsbTgtConfigTable))
        {
        usrUsbTgtConfigTable.bUseFuncDevDesc = bUseFuncDescOld;

        return ERROR;
        }
    
    return OK;    
    }

/*******************************************************************************
*
* usrUsbTgtDevDescOfFuncNameSet - set the function name who has the device   
* descriptor for the target using
*
* This routine sets the the function name who has the device   
* descriptor for the target using
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtDevDescOfFuncNameSet
    (
    char * pName
    )
    {
    char pNameOld[USBTGT_MAX_NAME_SZ];
    char pNameNew[USBTGT_MAX_NAME_SZ];
    
    if (NULL == pName)
        {
        return ERROR;
        }
    
    strncpy(pNameOld, usrUsbTgtConfigTable.devDescOfFuncName, 
            USBTGT_MAX_NAME_SZ - 1);

    /* 
     * The function driver name length should less than 
     * USBTGT_MAX_NAME_SZ 
     */

    if (strlen(pName) >= USBTGT_MAX_NAME_SZ)
        {
        memcpy(pNameNew, pName, USBTGT_MAX_NAME_SZ - 2);
        pNameNew[USBTGT_MAX_NAME_SZ - 2] = '\0';
        strncpy(usrUsbTgtConfigTable.devDescOfFuncName, pNameNew,
                USBTGT_MAX_NAME_SZ - 1);       
        }
    else
        {    
        strncpy(usrUsbTgtConfigTable.devDescOfFuncName, pName,
                USBTGT_MAX_NAME_SZ - 1);
        }

    if (ERROR == usrUsbTgtConfigTableSet(&usrUsbTgtConfigTable))
        {
        strncpy(usrUsbTgtConfigTable.devDescOfFuncName, pNameOld, 
                USBTGT_MAX_NAME_SZ - 1);
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtDevDescOfFuncNameGet - get the function name who has the device   
* descriptor for the target using
*
* This routine gets the function name who has the device   
* descriptor for the target using
*
* RETURNS: NULL or the name of the function driver
*
* ERRNO: N/A
*/

char * usrUsbTgtDevDescOfFuncNameGet
    (
    void
    )
    {
    char * pName = NULL;

    pName = usrUsbTgtConfigTable.devDescOfFuncName;
    
    return pName;
    }

/*******************************************************************************
*
* usrUsbTgtDevDescOfFuncUnitSet - set the function unit who has the device   
* descriptor for the target using
*
* This routine sets the function unit who has the device   
* descriptor for the target using
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtDevDescOfFuncUnitSet
    (
    int   index,
    UINT8 uRndisUnit
    )
    {
    UINT8 uUnitOld = usrUsbTgtConfigTable.uDevDescOfFuncUnit;

    usrUsbTgtConfigTable.uDevDescOfFuncUnit = uRndisUnit;

    if (ERROR == usrUsbTgtConfigTableSet(&usrUsbTgtConfigTable))
        {
        usrUsbTgtConfigTable.uDevDescOfFuncUnit = uUnitOld;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtDevDescOfFuncUnitGet - get the function unit who has the device   
* descriptor for the target using
*
* This routine gets the function unit who has the device   
* descriptor for the target using
*
* RETURNS: the function unit
*
* ERRNO: N/A
*/

UINT8 usrUsbTgtDevDescOfFuncUnitGet
    (
    void
    )
    {
    return usrUsbTgtConfigTable.uDevDescOfFuncUnit;
    }

/*******************************************************************************
*
* usrUsbTgtDescSet - set the descriptors according to the type
*
* This routine sets the descriptors according to the type <'uDescType'>.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtDescSet
    (
    UINT8    uDescType,
    char *   pDescBuf
    )
    {
    STATUS status = ERROR;
    USB_DEVICE_DESCR DevDesc;
    USB_CONFIG_DESCR ConfigDesc;
    
    if (NULL == pDescBuf)
        {
        return ERROR;
        }

    switch (uDescType)
        {
        case USB_DESCR_DEVICE:
            
            memcpy ((void *)&DevDesc,
                    (void *)usrUsbTgtConfigTable.pDevDesc,
                    sizeof (USB_DEVICE_DESCR));

            memcpy ((void *)usrUsbTgtConfigTable.pDevDesc,
                    (void *)pDescBuf,
                    sizeof (USB_DEVICE_DESCR));
            
            status = usrUsbTgtConfigTableSet(&usrUsbTgtConfigTable);
            
            if (ERROR == status)
                {
                memcpy ((void *)usrUsbTgtConfigTable.pDevDesc,
                        (void *)&DevDesc,
                        sizeof (USB_DEVICE_DESCR));
                return ERROR;
                }
            break;
        case USB_DESCR_CONFIGURATION:

            memcpy ((void *)&ConfigDesc,
                    (void *)usrUsbTgtConfigTable.pConfigDesc,
                    sizeof (USB_CONFIG_DESCR));

            memcpy ((void *)usrUsbTgtConfigTable.pConfigDesc,
                    (void *)pDescBuf,
                    sizeof (USB_CONFIG_DESCR));
            
            status = usrUsbTgtConfigTableSet(&usrUsbTgtConfigTable);
            
            if (ERROR == status)
                {
                memcpy ((void *)usrUsbTgtConfigTable.pConfigDesc,
                        (void *)&ConfigDesc,
                        sizeof (USB_CONFIG_DESCR));
                return ERROR;
                }
            break;
        case USB_DESCR_BOS:

            if (NULL != usrUsbTgtConfigTable.pBosDesBuf)
                {
                memcpy ((void *)usrUsbTgtConfigTable.pBosDesBuf,
                        (void *)pDescBuf,
                        USB_BOS_DESCR_LEN);            
                
                memcpy ((void *)usrUsbTgtConfigTable.pBosDesBuf,
                        (void *)pDescBuf,
                TO_LITTLEW (*(UINT16 *)&usrUsbTgtConfigTable.pBosDesBuf[2]));            
                }
            break;
        default:
            status = ERROR;
            break;
        }
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtDescGet - get the descriptors according to the type
*
* This routine returns the descriptors according to the type <'uDescType'>.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtDescGet
    (
    UINT8    uDescType,   
    char *   pDescBuf
    )
    {
    STATUS status = ERROR;
    
    if (NULL == pDescBuf)
        {
        return ERROR;
        }
    
    switch (uDescType)
        {
        case USB_DESCR_DEVICE:
            
            status = usrUsbTgtConfigTableGet(uDescType,
                     &usrUsbTgtConfigTable,
                     NULL);

            memcpy ((void *)pDescBuf,
                    (void *)usrUsbTgtConfigTable.pDevDesc,
                    sizeof (USB_DEVICE_DESCR));
            
            break;
        case USB_DESCR_CONFIGURATION:
            
            status = usrUsbTgtConfigTableGet(uDescType,
                     &usrUsbTgtConfigTable,
                     NULL);
            
            memcpy ((void *)pDescBuf,
                    (void *)usrUsbTgtConfigTable.pConfigDesc,
                    sizeof (USB_CONFIG_DESCR));            
            break;
        case USB_DESCR_BOS:

            status = usrUsbTgtConfigTableGet(uDescType,
                     &usrUsbTgtConfigTable,
                     NULL);

            if (NULL != usrUsbTgtConfigTable.pBosDesBuf)
                {
                memcpy ((void *)pDescBuf,
                        (void *)usrUsbTgtConfigTable.pBosDesBuf,
                TO_LITTLEW (*(UINT16 *)&usrUsbTgtConfigTable.pBosDesBuf[2]));            
                }
            break;
        default:
            status = ERROR;
            break;
        }
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtLangDescrSet - set the usb target language descriptor
*
* This routine is used to set the usb target language descriptor.
*
* RETURNS: OK or ERROR if there is something wrong.
*
* ERRNO: N/A
*/

 STATUS usrUsbTgtLangDescrSet
    (
    USB_LANGUAGE_DESCR * pLangDesc
    )
    {
    USR_USBTGT_LANG_DESC langDesOld;
    UINT8                uNewLangDataLen;
    
    if (NULL == pLangDesc)
        {
        return ERROR;
        }

    memcpy ((void *)&langDesOld,
            (void *)usrUsbTgtConfigTable.pLanguageDesc,
            sizeof (USR_USBTGT_LANG_DESC));

    /* 
     * The global language descriptor table only support 
     * USBTGT_MAX_LANGID_COUNT IDs, do not accept more than that.
     */
     
    uNewLangDataLen = min(pLangDesc->length,
                   (sizeof (USB_DESCR_HDR) + 
                    USBTGT_MAX_LANGID_COUNT * sizeof (UINT16)));

    memcpy ((void *)usrUsbTgtConfigTable.pLanguageDesc,
            (void *)pLangDesc,
            uNewLangDataLen);

    if (pLangDesc->length > uNewLangDataLen)
        {
        usrUsbTgtConfigTable.pLanguageDesc->length = uNewLangDataLen;
        }

    if (ERROR == usrUsbTgtConfigTableSet(&usrUsbTgtConfigTable))
        {
        memcpy ((void *)usrUsbTgtConfigTable.pLanguageDesc,
                (void *)&langDesOld,
                sizeof (USR_USBTGT_LANG_DESC));
        
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtLangDescrGet - get the usb target language descriptor
*
* This routine returns the usb target language descriptor set by the user
*
* RETURNS: NULL or the language descriptor
*
* ERRNO: N/A
*/

USB_LANGUAGE_DESCR * usrUsbTgtLangDescrGet
    (
    void
    )
    {
    USR_USBTGT_LANG_DESC * pLangDesc = NULL; 
    UINT8 i = 0;

    pLangDesc = usrUsbTgtConfigTable.pLanguageDesc;

    if (NULL == pLangDesc)
        return NULL;

    /* Not the valid language descriptor */
    
    if ((pLangDesc->length == 0) ||
        (pLangDesc->descriptorType != USB_DESCR_STRING))
        {
        /* The length is the supported language IDs with the USB_DESCR_HDR */
        
        pLangDesc->length = USBTGT_LANGID_SUPPORTED_COUNT * sizeof (UINT16) + 
                            sizeof (USB_DESCR_HDR);
        pLangDesc->descriptorType = USB_DESCR_STRING;

        for (i = 0; i < USBTGT_LANGID_SUPPORTED_COUNT; i ++)
            {
            pLangDesc->uLangId[0] = gUsbTgtLangIDs[i];
            }
        }
    
    return (USB_LANGUAGE_DESCR *)pLangDesc;
    }

/*******************************************************************************
*
* usrUsbTgtStringDescSet - set the string descriptors by user
*
* This routine is used to set the string descriptors according to the
* <'uLangId'> and <'uStringIndex'>.
*
* Note:
* This routine is ready for the user to add his sub-routine to store the string
* descriptor to the external devices, such as NvRam, EEPROM or external flash, 
* and etc.
*
* <'uLangId'> the language ID, which can be any UNICODE, include ENGLISH
* <'uLangIndex'> the index of the strings, which can be 
* USBTGT_MFG_STRING_INDEX    : For manufacturer string
* USBTGT_PROD_STRING_INDEX   : For product string
* USBTGT_SERIAL_STRING_INDEX : For serial number string
* 
* RETURNS: ERROR if the user do not have any interface to set the string
* descriptors, or STATUS with the return of users sub-routine.
*
* ERRNO: N/A
*/

STATUS usrUsbTgtStringDescSet
    (
    UINT16   uLangId,
    UINT8    uStringIndex,
    UINT8 *  pDescBuf
    )
    {
    STATUS status = ERROR;
    
    if (NULL == pDescBuf)
        {
        return ERROR;
        }   
    
    /* Add the users sub-routine here, and with the return of STATUS */
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtStringDescGet - get the string descriptors set by the user
*
* This routine is used to get the string descriptors set by the user, according
* to the <'uLangId'> and <'uStringIndex'>.
*
* Note:
* This routine is ready for the user to add his sub-routine to get the string
* descriptor from the external devices, such as NvRam, EEPROM or external flash,
* and etc.
*
* <'uLangId'> the language ID, which can be any UNICODE, include ENGLISH
* <'uLangIndex'> the index of the strings, which can be 
* USBTGT_MFG_STRING_INDEX    : For manufacturer string
* USBTGT_PROD_STRING_INDEX   : For product string
* USBTGT_SERIAL_STRING_INDEX : For serial number string
* <uSrtLen> is the reqired length of the string or the max buffer length of the
* the buffer <'pDescBuf'>.
* <'pDescBuf'> is the point of the buffer to store the sting descriptor
* <'pActLen'> is the act length of the string descriptor data length.
*
* RETURNS: ERROR if the user do not have any interface to set the string
* descriptors, or STATUS with the return of users sub-routine.
*
* ERRNO: N/A
*/

STATUS usrUsbTgtStringDescGet
    (
    UINT16   uLangId,
    UINT8    uStringIndex,
    UINT16   uSrtLen,
    UINT8 *  pDescBuf,
    UINT16 * pActLen
    )
    {
    STATUS status = ERROR;
    
    if (NULL == pDescBuf)
        {
        return ERROR;
        }    
    
    /* Add the users sub-routine here, and with the return of STATUS */
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtInit - initialize the usb target stack
*
* This routine is used to initialize the usb target stack by calling the routine
* <'usbTgtInit()'>.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtInit
    (
    void
    )
    {
    /* 
     * This is the entry of all the USB GEN2 target stack initialization
     * From 
     */
     
    if (usbTgtInit() == ERROR)
        {
        return ERROR;
        }
    
    /* 
     * This routine will first collect all the information from the user 
     * configuration for the TCD and the functions
     */
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtUnInit - uninitialize the usb target stack
*
* This routine is used to uninitialize the usb target stack. It will 
* uninitialize all the function drivers and unregister all the TCDs.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtUnInit
    (
    void
    )
    {
    BOOL   bError = FALSE;

    if (ERROR == usrUsbTgtFuncUnInit())
        {
        bError = TRUE;
        }
    
    vxbUsbTcdUnRegister();
    
    bError = FALSE;
    
    if (ERROR == usbTgtExit())
        {
        bError = TRUE;
        }
    
    return (bError ? ERROR : OK);
    
    }

#endif /* __INCusrUsbTgtInitc */

