/* vxbVerChk.uc - VxBus version check */

/*
 * Copyright (C) 2008-2009, 2011 Wind River Systems, Inc. All rights are reserved.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* 
modification history
--------------------
01f,17aug11,dee  supporting file needed for WIND00278138, fix Prevent
                 NO_EFFECT defect
01e,09dec09,h_k  updated major ver to 5 only for LP64.
01d,15aug08,h_k  reverted the version check.
01c,23jul08,h_k  temporary workaround for the VxBus version check. (CQ:128598)
01b,10jul08,tor  prohibit drivers using pAccess (<ver4.0.0) from registering
01a,01may08,tor  created
*/

/*
DESCRIPTION

This module contains code to check the version of drivers
when registering, and ensure that no driver successfully
registers if it makes use of facilities that are not present
in the current configuration.

This module must be compiled in the context of a project or
BSP.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <string.h>
#include <sysLib.h>

/* defines */

/* the latest version */

#define VXB_VERSION			VXB_VER_5_0_0

/* the major version supported till */

#define VXB_VER_MAJOR_SUPPORTED_HIGH	VXB_VER_MAJOR(VXB_VERSION)

/* the major version supported from */

#ifdef	_WRS_CONFIG_LP64
# define VXB_VER_MAJOR_SUPPORTED_LOW	VXB_VER_MAJOR(VXB_VERSION)
#
# ifdef	VXB_LEGACY_ACCESS
#  error VXB_LEGACY_ACCESS is not supported in 64-bit system!
# endif	/* VXB_LEGACY_ACCESS */
#else
# ifdef	VXB_LEGACY_ACCESS
#  define VXB_VER_MAJOR_SUPPORTED_LOW	VXBUS_VERSION_1
# else
#  define VXB_VER_MAJOR_SUPPORTED_LOW	VXB_VER_MAJOR(VXB_VER_4_0_0)
# endif	/* VXB_LEGACY_ACCESS */
#endif	/* _WRS_CONFIG_LP64 */

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

/*********************************************************************
*
* vxbDrvVerCheck - Check for valid driver version
*
*  This routine checks a VxBus driver to ensure that the current
*  VxBus configuration will support the services that the driver
*  uses.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS vxbDrvVerCheck
    (
    struct vxbDevRegInfo * pDevInfo /* per-bus recognition info */
    )
    {
    /* no forward compatability if the major version is 6 or higher */

    if (VXB_VER_MAJOR(pDevInfo->vxbVersion) > VXB_VER_MAJOR_SUPPORTED_HIGH)
	goto vxbDrvVerCheckFail;

#ifndef VXB_LEGACY_ACCESS

    /*
     * Register access changed with VxBus version 4.0.0. The older
     * device register access mechanism, using pDev->pAccess, is no longer
     * available. Instead, drivers use vxbRead*() and vxbWrite*().
     *
     * The LP64 adaptation is done with VxBus version 5.0.0.
     * No backward compatability in LP64 mode.
     */

    if (VXB_VER_MAJOR(pDevInfo->vxbVersion) < VXB_VER_MAJOR_SUPPORTED_LOW)
	goto vxbDrvVerCheckFail;

#else /* VXB_LEGACY_ACCESS */

    /*
     * If VXB_LEGACY_ACCESS is configured, then pDev->pAccess is
     * available and we can accept earlier drivers.  However,
     * we still reject prototype version drivers.
     */

    if (pDevInfo->vxbVersion < VXB_VER_MAJOR_SUPPORTED_LOW)
	goto vxbDrvVerCheckFail;

#endif /* VXB_LEGACY_ACCESS */

    return (OK);

vxbDrvVerCheckFail:

    /*
     * Set the error messages.
     *
     * NOTE:
     * Do not use sprintf() to avoid the dependency between
     * INCLUDE_FORMATTED_OUT_BASIC and INCLUDE_VXBUS.
     */

    strcat (sysExcMsg, "VxBus Version check fails at ");

    sysExcMsg += strlen (sysExcMsg);

    if (pDevInfo->drvName[0] != EOS)
	strcat (sysExcMsg, pDevInfo->drvName);
    else
	strcat (sysExcMsg, "<null>");

    sysExcMsg += strlen (sysExcMsg);

    strcat (sysExcMsg, " driver!\n");

    sysExcMsg += strlen (sysExcMsg);

    return (ERROR);
    }
