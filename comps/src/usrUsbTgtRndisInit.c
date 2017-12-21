/* usrUsbTgtRndisInit.c - User Initialization Of The USB RNDIS Function Driver */

/*
 * Copyright (c) 2010, 2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,06may13,s_z  Remove compiler warning (WIND00356717) 
01e,12jul11,s_z  Remove compiler warning
01d,11apr11,s_z  Add Open Specifications Documentation description
01c,22mar11,s_z  Changes for unused routines removed
01b,09mar11,s_z  Code clean up
01a,12oct10,s_z  created
*/

/*
DESCRIPTION

This file includes the definations and the routines to configure the USB 
RNDIS function driver.

This RNDIS function driver follows the [MS-RNDIS]:Remote Network Driver 
Interface Specification (RNDIS) Protocol Specification 1.0, which is one of the
Open Specifications Documentation.

Following is the Intellectual Property Rights Notice for Open Specifications 
Documentation:

\h Intellectual Property Rights Notice for Open Specifications Documentation

\h Technical Documentation. 

Microsoft publishes Open Specifications documentation for protocols, 
file formats, languages, standards as well as overviews of the interaction 
among each of these technologies.

\h Copyrights. 

This documentation is covered by Microsoft copyrights. Regardless of any other 
terms that are contained in the terms of use for the Microsoft website that
hosts this documentation, you may make copies of it in order to develop 
implementations of the technologies described in the Open Specifications 
and may distribute portions of it in your implementations using these 
technologies or your documentation as necessary to properly document the 
implementation. You may also distribute in your implementation, with or without 
modification, any schema, IDL's, or code samples that are included in the 
documentation. This permission also applies to any documents that are 
referenced in the Open Specifications.

\h No Trade Secrets.

Microsoft does not claim any trade secret rights in this documentation.

\h Patents.

Microsoft has patents that may cover your implementations of the technologies 
described in the Open Specifications. Neither this notice nor Microsoft's 
delivery of the documentation grants any licenses under those or any other 
Microsoft patents. However, a given Open Specification may be covered by 
Microsoft's Open Specification Promise (available here: 
http://www.microsoft.com/interop/osp) or the Community Promise 
(available here: http://www.microsoft.com/interop/cp/default.mspx).
If you would prefer a written license, or if the technologies described in 
the Open Specifications are not covered by the Open Specifications Promise 
or Community Promise, as applicable, patent licenses are available by 
contacting iplg@microsoft.com.

\h Trademarks. 

The names of companies and products contained in this 
documentation may be covered by trademarks or similar intellectual 
property rights. This notice does not grant any licenses under those rights.

\h Fictitious Names.

The example companies, organizations, products, 
domain names, e-mail addresses, logos, people, places, and events depicted in 
this documentation are fictitious. No association with any real company, 
organization, product, domain name, email address, logo, person, place, or 
event is intended or should be inferred.

\h Reservation of Rights. 

All other rights are reserved, and this notice does 
not grant any rights other than specifically described above, whether by 
implication, estoppel, or otherwise.

\h Tools. 

The Open Specifications do not require the use of Microsoft 
programming tools or programming environments in order for you to develop an 
implementation. If you have access to Microsoft programming tools and 
environments you are free to take advantage of them. Certain Open 
Specifications are intended for use in conjunction with publicly available 
standard specifications and network programming art, and assumes that the 
reader either is familiar with the aforementioned material or has immediate 
access to it.

INCLUDE FILES: vxWorks.h, usb/usb.h, usb/usbTgt.h, usb/usbTgtRndis.h
               usb/tools/cmdParser.h
*/

/* includes */

#ifndef __INCusrUsbTgtRndisInitc
#define __INCusrUsbTgtRndisInitc

#include <vxWorks.h>
#include <usb/usb.h>
#include <usb/usbTgt.h>
#include <usb/usbTgtRndis.h>
#include <usb/tools/cmdParser.h>


#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define USBTGT_RNDIS_TOOL_NAME      "usbTgtRndis"
#define USBTGT_RNDIS_TOOL_DESCR     "USB Target RNDIS Funcion Configurater & Tester"
#define USBTGT_RNDIS_TOOL_VERSION   "V0.1"
#define USBTGT_RNDIS_TOOL_COPYRIGHT "Copyright (c) 2011, Wind River Systems, Inc.\n"
#define USBTGT_RNDIS_TOOL_PROMPT    "Rndis>"      /* Program prompt */

#if defined(INCLUDE_USBTGT_RNDIS_2)
#define INCLUDE_USBTGT_RNDIS_1
#endif


#if defined(INCLUDE_USBTGT_RNDIS_1)

/* 
 * The TCD name and unit which attached by the RNDIS function driver 
 */
 
#ifndef USBTGT_RNDIS_TCD_NAME1
#   define USBTGT_RNDIS_TCD_NAME1    "/usbTgt"
#endif

#ifndef USBTGT_RNDIS_TCD_UNIT1
#   define USBTGT_RNDIS_TCD_UNIT1    0
#endif

/* 
 * The configuration number which built in by the RNDIS function driver
 * 
 * If the user need configure one usb target with more configurations, or need 
 * bind the right function driver to the specific configuration, the user need 
 * set this value.
 *
 * If the user do not set this value, the usb target management will  
 * automaticlly bind the function driver to the default configuration 1.
 */

#   ifndef USBTGT_RNDIS_CONFIG_NUM1
#   define USBTGT_RNDIS_CONFIG_NUM1       (1)
#   endif

#   ifndef USBTGT_RNDIS_NAME1
#   define USBTGT_RNDIS_NAME1             "usbTgtRndis"
#   endif

#   ifndef USBTGT_RNDIS_UNIT1
#   define USBTGT_RNDIS_UNIT1             0
#   endif

#   ifndef USBTGT_RNDIS_PM_ENABLE1
#   define USBTGT_RNDIS_PM_ENABLE1        0
#   endif

#   ifndef USBTGT_RNDIS_TASK_PRIORITY1
#   define USBTGT_RNDIS_TASK_PRIORITY1    100
#   endif

/* Define the medium type */

#if (defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE1) &&  \
     defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END1)) || \
    (!defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE1) &&  \
     !defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END1))
#   error "Please select one medium type ? END_BRIDGE1 or VIRTUAL_END1"
#endif

#ifdef INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE1

#   ifndef USBTGT_RNDIS_MEDIUM_TYPE1
#   define USBTGT_RNDIS_MEDIUM_TYPE1               MEDIUM_TYPE_END_BRIDGE
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME1
#   define USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME1   "sme"
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT1
#   define USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT1   0
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_NAME1
#   define USBTGT_RNDIS_MEDIUM_NAME1   USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME1
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_UNIT1
#   define USBTGT_RNDIS_MEDIUM_UNIT1   USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT1
#   endif

#else

#   ifndef USBTGT_RNDIS_MEDIUM_TYPE1
#   define USBTGT_RNDIS_MEDIUM_TYPE1              MEDIUM_TYPE_END_EMULATOR
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_VRTL_END_NAME1
#   define USBTGT_RNDIS_MEDIUM_VRTL_END_NAME1     "usbEnd"
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT1
#   define USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT1     0
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_NAME1
#   define USBTGT_RNDIS_MEDIUM_NAME1   USBTGT_RNDIS_MEDIUM_VRTL_END_NAME1
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_UNIT1
#   define USBTGT_RNDIS_MEDIUM_UNIT1   USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT1
#   endif


#endif /* ! INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE_1 */

#endif /* INCLUDE_USBTGT_RNDIS_1 */


#if defined(INCLUDE_USBTGT_RNDIS_2)

/* 
 * The TCD name and unit which attached by the RNDIS function driver 
 */
 
#ifndef USBTGT_RNDIS_TCD_NAME2
#   define USBTGT_RNDIS_TCD_NAME2    "/usbTgt"
#endif

#ifndef USBTGT_RNDIS_TCD_UNIT2
#   define USBTGT_RNDIS_TCD_UNIT2    0
#endif

/* 
 * The configuration number which built in by the RNDIS function driver
 * 
 * If the user need configure one usb target with more configurations, or need 
 * bind the right function driver to the specific configuration, the user need 
 * set this value.
 *
 * If the user do not set this value, the usb target management will  
 * automaticlly bind the function driver to the default configuration 1.
 */

#   ifndef USBTGT_RNDIS_CONFIG_NUM2
#   define USBTGT_RNDIS_CONFIG_NUM2       (1)
#   endif

#   ifndef USBTGT_RNDIS_NAME2
#   define USBTGT_RNDIS_NAME2             "usbTgtRndis"
#   endif

#   ifndef USBTGT_RNDIS_UNIT2
#   define USBTGT_RNDIS_UNIT2             0
#   endif

#   ifndef USBTGT_RNDIS_PM_ENABLE2
#   define USBTGT_RNDIS_PM_ENABLE2        0
#   endif

#   ifndef USBTGT_RNDIS_TASK_PRIORITY2
#   define USBTGT_RNDIS_TASK_PRIORITY2    100
#   endif

#if (defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE2) &&  \
     defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END2)) || \
    (!defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE2) &&  \
     !defined(INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END2))
#   error "Please select one medium type ? END_BRIDGE2 or VIRTUAL_END2"
#endif


/* Define the medium type */

#ifdef INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE2

#   ifndef USBTGT_RNDIS_MEDIUM_TYPE2
#   define USBTGT_RNDIS_MEDIUM_TYPE2               MEDIUM_TYPE_END_BRIDGE
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME2
#   define USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME2   "sme"
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT2
#   define USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT2   0
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_NAME2
#   define USBTGT_RNDIS_MEDIUM_NAME2   USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME2
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_UNIT2
#   define USBTGT_RNDIS_MEDIUM_UNIT2   USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT2
#   endif

#else

#   ifndef USBTGT_RNDIS_MEDIUM_TYPE2
#   define USBTGT_RNDIS_MEDIUM_TYPE2              MEDIUM_TYPE_END_EMULATOR
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_VRTL_END_NAME2
#   define USBTGT_RNDIS_MEDIUM_VRTL_END_NAME2     "usbEnd"
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT2
#   define USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT2     0
#   endif


#   ifndef USBTGT_RNDIS_MEDIUM_NAME2
#   define USBTGT_RNDIS_MEDIUM_NAME2   USBTGT_RNDIS_MEDIUM_VRTL_END_NAME2
#   endif

#   ifndef USBTGT_RNDIS_MEDIUM_UNIT2
#   define USBTGT_RNDIS_MEDIUM_UNIT2   USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT2
#   endif

#endif /* ! INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE_2 */

#endif /* INCLUDE_USBTGT_RNDIS_2 */

/* globals */

/* USB target RNDIS configurable options */

typedef struct usr_usbTgt_rnids_config
    {
    int   taskPriority;      /* The task priority of RNDIS function driver */
    char  tcdName[USBTGT_MAX_NAME_SZ];/* The TCD name to be attached */
    UINT8 uTcdUnit;          /* The TCD unit to be attached */
    char  rndisName[USBTGT_MAX_NAME_SZ];/* The RNDIS function driver name */
    UINT8 uRndisUnit;        /* The RNDIS function driver unit */
    char  mediumName[USBTGT_MAX_NAME_SZ];/* The medium name to be bound to RNDIS */
    UINT8 uMediumUnit;       /* The medium unit to be bound to RNDIS */
    UINT8 uMediumType;       /* The medium type to be bound to RNDIS */
    UINT8 uAimConfigNum;     /* The configuration num build in the TCD */
    BOOL  bPMEnabled;        /* Power management enabled of the RNDIS */
    }USR_USBTGT_RNDIS_CONFIG, *pUSR_USBTGT_RNDIS_CONFIG;

USR_USBTGT_RNDIS_CONFIG usrUsbTgtRndisConfigTable[] =
    {
#if defined(INCLUDE_USBTGT_RNDIS_1)
        {
        USBTGT_RNDIS_TASK_PRIORITY1, /* taskPriority */
        USBTGT_RNDIS_TCD_NAME1,      /* tcdName */ 
        USBTGT_RNDIS_TCD_UNIT1,      /* tcdUnit */ 
        USBTGT_RNDIS_NAME1,          /* rndisName */ 
        USBTGT_RNDIS_UNIT1,          /* rndisUnit */
        USBTGT_RNDIS_MEDIUM_NAME1,   /* mediumName, accroding to the type */
        USBTGT_RNDIS_MEDIUM_UNIT1,   /* uMediumUnit */
        USBTGT_RNDIS_MEDIUM_TYPE1,   /* uMediumType */
        USBTGT_RNDIS_CONFIG_NUM1,    /* uAimCOnfigNum */
        USBTGT_RNDIS_PM_ENABLE1      /* bPMEnabled */
        },
#endif /* INCLUDE_USBTGT_RNDIS_1 */

#if defined (INCLUDE_USBTGT_RNDIS_2)
        {
        USBTGT_RNDIS_TASK_PRIORITY2, /* taskPriority */
        USBTGT_RNDIS_TCD_NAME2,      /* tcdName */ 
        USBTGT_RNDIS_TCD_UNIT2,      /* tcdUnit */ 
        USBTGT_RNDIS_NAME2,          /* rndisName */ 
        USBTGT_RNDIS_UNIT2,          /* rndisUnit */
        USBTGT_RNDIS_MEDIUM_NAME2,   /* mediumName, accroding to the type */
        USBTGT_RNDIS_MEDIUM_UNIT2,   /* uMediumUnit */
        USBTGT_RNDIS_MEDIUM_TYPE2,   /* uMediumType */
        USBTGT_RNDIS_CONFIG_NUM2,    /* uAimCOnfigNum */
        USBTGT_RNDIS_PM_ENABLE2      /* bPMEnabled */
        }
#endif  /* INCLUDE_USBTGT_RNDIS_2 */
    }; 

#define USBTGT_RNDIS_COUNT NELEMENTS(usrUsbTgtRndisConfigTable)


/*******************************************************************************
*
* usrUsbTgRndisConfigTableGet - get the RNDIS configuration table which has 
* been defined.
*
* This routine gets the RNDIS configuration table which has 
* been defined and stored in some external interface. Such as the NvRom ,
* external flash, USB disk .etc.The user need add his own overwrite 
* interface in this routine.such as using the <'sysNvRamGet'> interface to 
* update the <'usrUsbTgtRndisConfigTable'>
*
* RETURNS: the returned STATUS to get the configuration table by the user 
*          sub-routine. Usually OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgRndisConfigTableGet
    (
    void
    )
    {
    STATUS status = OK;

    /* 
     * Add here the user interface to get the external configuration and
     * reset the <'usrUsbTgtRndisConfigTable'>. such as using the sysNvRamGet
     * interface to update the <'usrUsbTgtRndisConfigTable'>
     */
    
    return status;
    }

/*******************************************************************************
*
* usrUsbTgtRndisConfigTableSet - set the RNDIS configuration table.
*
* This routine sets the RNDIS configuration table.which maybe stored in 
* some external interface. Such as the NvRom , external flash, USB disk .etc.
* The user need add his own overwrite interface in this routine.
*
* RETURNS: the returned STATUS to set the configuration table by the user 
*          sub-routine. Usually OK, or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisConfigTableSet
    (
    pUSR_USBTGT_RNDIS_CONFIG pConfigTable
    )
    {
    if (NULL == pConfigTable)
        {
        return ERROR;
        }

    /* 
     * Here
     * Add the subroutine to set the config table if needed, such as 
     * sysNvRamSet to store the configuration table in the NvRam.
     */

    /* 
     * Call the <'usrUsbTgtNetworkConfigGet'> to update the 
     * <'usrUsbTgtRndisConfigTable'> 
     */   

    usrUsbTgRndisConfigTableGet();
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisAttachCallback - user callback routine when RNDIS function
* attached to (target management level)TML.
*
* This routine is the user callback routine when RNDIS function
* attached to (target management level)TML. The user can add his specific
* callback routine in this interface.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtRndisAttachCallback
    (
    void
    )
    {
    /* 
     * Callback to specify the user's use case when the function driver 
     * attached to the TML.
     *
     * Add the user's callback routine here 
     */

    return;
    }

/*******************************************************************************
*
* usrUsbTgtRndisDetachCallback - user callback routine when RNDIS function
* detached from target management level(TML).
*
* This routine is the user callback routine when RNDIS function
* detached from target management level(TML).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbTgtRndisDetachCallback
    (
    void
    )
    {
    /* 
     * Callback to specify the user's use case when the function driver 
     * detached from the TML.
     *
     * Add the user's callback routine here 
     */

    return;
    }


/*******************************************************************************
*
* usrUsbTgtRndisCountGet - get the RNDIS function instance count
*
* This routine returns the the RNDIS function instance count
*
* RETURNS: the count of the RNDIS function instance
*
* ERRNO: N/A
*/

int usrUsbTgtRndisCountGet
    (
    void
    )
    {
    return USBTGT_RNDIS_COUNT;
    }

/*******************************************************************************
*
* usrUsbTgtRndisTaskPrioritySet - set the RNDIS management task priority
*
* This routine sets RNDIS management task priority.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisTaskPrioritySet
    (
    int index,
    int taskPriority
    )
    {
    int taskOld;
    
    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;
    
    taskOld = usrUsbTgtRndisConfigTable[index].taskPriority;

    usrUsbTgtRndisConfigTable[index].taskPriority = taskPriority;

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        usrUsbTgtRndisConfigTable[index].taskPriority = taskOld;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisTaskPriorityGet - get the RNDIS management task priority
*
* This routine returns the RNDIS management task priority
*
* RETURNS: the RNDIS management task priority
*
* ERRNO: N/A
*/

int usrUsbTgtRndisTaskPriorityGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].taskPriority;
    
    return usrUsbTgtRndisConfigTable[index].taskPriority;
    }


/*******************************************************************************
*
* usrUsbTgtRndisTcdNameSet - set the TCD name which attached by RNDIS interface 
*
* This routine sets the TCD name which attached by RNDIS interface 
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisTcdNameSet
    (
    int    index,
    char * pName
    )
    {
    char pNameOld[USBTGT_MAX_NAME_SZ];
    char pNameNew[USBTGT_MAX_NAME_SZ];

    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;

    strncpy(pNameOld, usrUsbTgtRndisConfigTable[index].tcdName, 
            USBTGT_MAX_NAME_SZ - 1);

    /* 
     * The function driver name length should less than 
     * USBTGT_MAX_NAME_SZ 
     */

    if (strlen(pName) >= USBTGT_MAX_NAME_SZ)
        {
        memcpy(pNameNew, pName, USBTGT_MAX_NAME_SZ - 2);
        pNameNew[USBTGT_MAX_NAME_SZ - 2] = '\0';
        strncpy(usrUsbTgtRndisConfigTable[index].tcdName, pNameNew, 
                USBTGT_MAX_NAME_SZ - 1);       
        }
    else
        {    
        strncpy(usrUsbTgtRndisConfigTable[index].tcdName, pName, 
                USBTGT_MAX_NAME_SZ - 1);
        }
    
    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        strncpy(usrUsbTgtRndisConfigTable[index].tcdName, pNameOld, 
                USBTGT_MAX_NAME_SZ - 1);
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisTcdNameGet - get the TCD name which attached by RNDIS interface 
*
* This routine gets the TCD name which attached by RNDIS interface 
*
* RETURNS: NULL or the name of the attached TCD
*
* ERRNO: N/A
*/

char * usrUsbTgtRndisTcdNameGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].tcdName;
    
    return usrUsbTgtRndisConfigTable[index].tcdName;
    }

/*******************************************************************************
*
* usrUsbTgtRndisTcdUnitSet - set the TCD unit which attached by RNDIS interface 
*
* This routine sets the TCD unit which attached by RNDIS interface 
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisTcdUnitSet
    (
    int   index,
    UINT8 uTcdUnit
    )
    {
    UINT8 uUnitOld; 

    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;

    uUnitOld = usrUsbTgtRndisConfigTable[index].uTcdUnit;
    usrUsbTgtRndisConfigTable[index].uTcdUnit = uTcdUnit;

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        usrUsbTgtRndisConfigTable[index].uTcdUnit = uUnitOld;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisTcdUnitGet - get the TCD unit which attached by RNDIS interface
*
* This routine gets the TCD unit which attached by RNDIS interface
*
* RETURNS: the attached TCD unit number
*
* ERRNO: N/A
*/

UINT8 usrUsbTgtRndisTcdUnitGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].uTcdUnit;
    
    return usrUsbTgtRndisConfigTable[index].uTcdUnit;
    }

/*******************************************************************************
*
* usrUsbTgtRndisConfigNumSet - set the RNDIS attached TCD unit which 
* binded to the USB RNDIS interface.
*
* This routine sets the RNDIS attached TCD unit which 
* binded to the USB RNDIS interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisConfigNumSet
    (
    int   index,
    UINT8 uConfigNum
    )
    {
    UINT8 uConfigOld;
    
    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;
    
    uConfigOld = usrUsbTgtRndisConfigTable[index].uAimConfigNum;
    usrUsbTgtRndisConfigTable[index].uAimConfigNum = uConfigNum;

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        usrUsbTgtRndisConfigTable[index].uAimConfigNum = uConfigOld;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisConfigNumGet - get the RNDIS attached TCD unit which 
* binded to the USB RNDIS interface.
*
* This routine gets the RNDIS attached TCD unit which 
* binded to the USB RNDIS interface.
*
* RETURNS: the attached TCD unit number
*
* ERRNO: N/A
*/

UINT8 usrUsbTgtRndisConfigNumGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].uAimConfigNum;
    
    return usrUsbTgtRndisConfigTable[index].uAimConfigNum;
    }

/*******************************************************************************
*
* usrUsbTgtRndisNameSet - set the RNDIS function name which identified  
* the USB RNDIS interface.
*
* This routine sets the RNDIS function name which identified  
* the USB RNDIS interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisNameSet
    (
    int    index,
    char * pName
    )
    {
    char pNameOld[USBTGT_MAX_NAME_SZ];
    char pNameNew[USBTGT_MAX_NAME_SZ];

    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;
    
    strncpy(pNameOld, usrUsbTgtRndisConfigTable[index].rndisName, 
            USBTGT_MAX_NAME_SZ - 1);

    /* 
     * The function driver name length should less than 
     * USBTGT_MAX_NAME_SZ 
     */

    if (strlen(pName) >= USBTGT_MAX_NAME_SZ)
        {
        memcpy(pNameNew, pName, USBTGT_MAX_NAME_SZ - 2);
        pNameNew[USBTGT_MAX_NAME_SZ - 2] = '\0';
        strncpy(usrUsbTgtRndisConfigTable[index].rndisName, pNameNew, 
               USBTGT_MAX_NAME_SZ - 1);       
        }
    else
        {    
        strncpy(usrUsbTgtRndisConfigTable[index].rndisName, pName, 
                USBTGT_MAX_NAME_SZ - 1);
        }

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        strncpy(usrUsbTgtRndisConfigTable[index].rndisName, pNameOld, 
                USBTGT_MAX_NAME_SZ - 1);
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisNameGet - get the RNDIS function name which identified  
* the USB RNDIS interface.
*
* This routine gets the RNDIS function name which identified  
* the USB RNDIS interface.
*
* RETURNS: NULL or the name of the RNDIS media
*
* ERRNO: N/A
*/

char * usrUsbTgtRndisNameGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].rndisName;
    
    return usrUsbTgtRndisConfigTable[index].rndisName;
    }

/*******************************************************************************
*
* usrUsbTgtRndisUnitSet - set the RNDIS function unit which identified  
* the USB RNDIS interface.
*
* This routine sets the RNDIS function unit which identified  
* the USB RNDIS interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisUnitSet
    (
    int   index,
    UINT8 uRndisUnit
    )
    {
    UINT8 uUnitOld;
    
    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;
    
    uUnitOld = usrUsbTgtRndisConfigTable[index].uRndisUnit;
    usrUsbTgtRndisConfigTable[index].uRndisUnit = uRndisUnit;

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        usrUsbTgtRndisConfigTable[index].uRndisUnit = uUnitOld;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisUnitGet - get the RNDIS function unit which identified  
* the USB RNDIS interface.
*
* This routine gets the RNDIS function unit which identified  
* the USB RNDIS interface.
*
* RETURNS: the RNDIS uint number
*
* ERRNO: N/A
*/

UINT8 usrUsbTgtRndisUnitGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].uRndisUnit;
    
    return usrUsbTgtRndisConfigTable[index].uRndisUnit;
    }

/*******************************************************************************
*
* usrUsbTgtRndisMediumNameSet - set the RNDIS medium name which binded to the 
* USB RNDIS interface.
*
* This routine sets the RNDIS medium name which binded to the 
* USB RNDIS interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisMediumNameSet
    (
    int    index,
    char * pName
    )
    {
    char pNameOld[USBTGT_MAX_NAME_SZ];
    char pNameNew[USBTGT_MAX_NAME_SZ];
    
    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;
    
    /* This medium name has been set */

    strncpy(pNameOld, usrUsbTgtRndisConfigTable[index].mediumName, USBTGT_MAX_NAME_SZ - 1);
        
    if (strlen(pName) >= USBTGT_MAX_NAME_SZ)
        {
        memcpy(pNameNew, pName, USBTGT_MAX_NAME_SZ - 1);
        pNameNew[USBTGT_MAX_NAME_SZ - 1] = '\0';
        strncpy(usrUsbTgtRndisConfigTable[index].mediumName, 
                pNameNew, USBTGT_MAX_NAME_SZ - 1);       
        }
    else
        {    
        strncpy(usrUsbTgtRndisConfigTable[index].mediumName, pName, 
                USBTGT_MAX_NAME_SZ - 1);
        }

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        strncpy(usrUsbTgtRndisConfigTable[index].mediumName, pNameOld, 
                USBTGT_MAX_NAME_SZ - 1);
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisMediumNameGet - get the RNDIS medium name which bind to the 
* USB RNDIS interface.
*
* This routine gets the RNDIS medium name which bind to the 
* USB RNDIS interface.
*
* RETURNS: NULL or the name of the RNDIS medium
*
* ERRNO: N/A
*/

char * usrUsbTgtRndisMediumNameGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].mediumName; 
    
    return usrUsbTgtRndisConfigTable[index].mediumName;;
    }

/*******************************************************************************
*
* usrUsbTgtRndisMediumUnitSet - set the RNDIS medium unit which binded to the 
* USB RNDIS interface.
*
* This routine sets the RNDIS medium unit which binded to the 
* USB RNDIS interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisMediumUnitSet
    (
    int   index,
    UINT8 uMediumUnit
    )
    {
    UINT8 uUnitOld;
    
    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;
    
    uUnitOld = usrUsbTgtRndisConfigTable[index].uMediumUnit;
    usrUsbTgtRndisConfigTable[index].uMediumUnit = uMediumUnit;

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        usrUsbTgtRndisConfigTable[index].uMediumUnit = uUnitOld;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisMediumUnitGet - get the RNDIS medium unit which bind to the 
* USB RNDIS interface.
*
* This routine gets the RNDIS media unit which bind to the 
* USB RNDIS interface.
*
* RETURNS: the medium unit
*
* ERRNO: N/A
*/

UINT8 usrUsbTgtRndisMediumUnitGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].uMediumUnit; 
    
    return usrUsbTgtRndisConfigTable[index].uMediumUnit;
    }

/*******************************************************************************
*
* usrUsbTgtRndisMediumTypeSet - set the RNDIS medium type which bind to the 
* USB RNDIS interface.
*
* This routine sets the RNDIS media type which bind to the 
* USB RNDIS interface.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisMediumTypeSet
    (
    int   index,
    UINT8 uMediumType
    )
    {
    UINT8 uTypeOld;

    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;

    uTypeOld = usrUsbTgtRndisConfigTable[index].uMediumType;
    usrUsbTgtRndisConfigTable[index].uMediumType = uMediumType;

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        usrUsbTgtRndisConfigTable[index].uMediumType = uTypeOld;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisMediumTypeGet - get the RNDIS media type which bind to the RNDIS
* need be attached
*
* This routine gets the RNDIS media type which bind to the RNDIS
* need be attached.
*
* RETURNS: the medium type
*
* ERRNO: N/A
*/

UINT8 usrUsbTgtRndisMediumTypeGet
    (
    int index
    )
    {
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].uMediumType; 
    
    return usrUsbTgtRndisConfigTable[index].uMediumType;
    }


/*******************************************************************************
*
* usrUsbTgtRndisPMEnableSet - set the flag to indicate RNDIS power management
* enable or not
*
* This routine sets the flag to indicate RNDIS power management
* enable or not.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisPMEnableSet
    (
    int   index,
    BOOL  bRndisPMEnabled
    )
    {
    BOOL bPmEnabled;
    
    if (index >= USBTGT_RNDIS_COUNT)
        return ERROR;
    
    bPmEnabled = usrUsbTgtRndisConfigTable[index].bPMEnabled;
    usrUsbTgtRndisConfigTable[index].bPMEnabled = bRndisPMEnabled;

    if (ERROR == usrUsbTgtRndisConfigTableSet(&usrUsbTgtRndisConfigTable[0]))
        {
        usrUsbTgtRndisConfigTable[index].bPMEnabled = bPmEnabled;
        return ERROR;
        }
    
    return OK;
    }

/*******************************************************************************
*
* usrUsbTgtRndisPMEnableGet - get the flag to check if RNDIS power management
* has been enabled.
*
* This routine gets the flag to check if RNDIS power management
* has been enabled.
*
* RETURNS: the RNDIS power management enable flag, TRUE or FALSE 
*
* ERRNO: N/A
*/

BOOL usrUsbTgtRndisPMEnableGet
    (
    int index
    )
    {   
    if (index >= USBTGT_RNDIS_COUNT)
        return usrUsbTgtRndisConfigTable[0].bPMEnabled; 
    
    return usrUsbTgtRndisConfigTable[index].bPMEnabled;
    }

/*******************************************************************************
*
* usrUsbTgtRndisInit - initialize the RNDIS fucntion drivers
*
* This routine intializes the RNDIS fucntion drivers by calling the
* usbTgtRndisInit() routine.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisInit
    (
    void
    )
    {
    return usbTgtRndisInit();
    }

/*******************************************************************************
*
* usrUsbTgtRndisUnInit - uninitialize the RNDIS fucntion drivers
*
* This routine unintializes the RNDIS fucntion drivers by calling the
* usbTgtRndisUnInit() routine.
*
* RETURNS: OK or ERROR if there is something wrong
*
* ERRNO: N/A
*/

STATUS usrUsbTgtRndisUnInit
    (
    void
    )
    {
    return usbTgtRndisUnInit();
    }

/* 
 * This is the command table which can be used by the user to configure and 
 * test the RNDIS function device.
 */
 
LOCAL CMD_DESCR usbTgtRndisCmdTable [] =
    {
        {
            "Help", 4, "Help/?", "Displays list of supported commands.", 
            CmdParserHelpFunc
        },
        {
            "?", 1, NULL, NULL,
            CmdParserHelpFunc
        },
        {
            "Exit", 4, "Exit/Quit/Bye", "Exits program.", 
            CmdParserExitFunc
        },
        {
            "Quit", 1, NULL, NULL, 
            CmdParserExitFunc
        },
        {
            "Bye", 3, NULL, NULL, 
            CmdParserExitFunc
        },
        {NULL, 0, NULL, NULL, NULL}
    };

/*************************************************************************
*
* usbTgtRndisTool - Primary entry point for USB bus exerciser.
*
* usbTool presents the user with a command prompt.  The user enters
* commands to invoke USB functions through the USBD and USB HCD.
*
* RETURNS:  RET_OK for normal termination;
*           RET_ERROR for program failure.
*
* ERRNO: N/A
*/

UINT16 usbTgtRndisTool 
    (
    void
    )
    {
    UINT16 s;           /* Execution status */

    /* Set buffering for stdin/stdout to "no buffering" so screen updates
    will be timely. */

    setvbuf (stdin, NULL, _IONBF, 0);
    setvbuf (stdout, NULL, _IONBF, 0);


    /* Display program banner */

    fprintf (stdout, USBTGT_RNDIS_TOOL_NAME ": \n" 
             USBTGT_RNDIS_TOOL_DESCR ", version " 
             USBTGT_RNDIS_TOOL_VERSION "\n"
             USBTGT_RNDIS_TOOL_COPYRIGHT "\n\n");


    /* Prompt the user for commands so long as the execution status
    remains ok. */

    while ((s = PromptAndExecCmd (NULL, 
                                  USBTGT_RNDIS_TOOL_PROMPT, 
                                  stdin,
                                  stdout,
                                  usbTgtRndisCmdTable))
            == RET_CONTINUE)
        ;


    /* Display reason for termination */

    if (s == RET_OK)
        fprintf (stdout, USBTGT_RNDIS_TOOL_NAME " terminating normally\n");
    else if (s == RET_ERROR)
        fprintf (stdout, USBTGT_RNDIS_TOOL_NAME " terminating with error\n");
    else
        fprintf (stdout, USBTGT_RNDIS_TOOL_NAME " terminating with code %d\n", s);


    return s;
    }


#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsbTgtRndisInitc */

