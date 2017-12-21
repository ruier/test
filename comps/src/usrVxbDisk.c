/* usrVxbDisk.c - VxBus disk drive configuration file */

/*
 * Copyright (c) 2009  Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,04nov09,h_k  written. (WIND00189354)
*/

/*
DESCRIPTION

This is the VxBus disk drive configuration file.
*/

/* includes */

#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>

/* definitions */

/* externs */

/******************************************************************************
*
* usrVxbDiskDrv - create disk drive
*
* This routine executes the 'vxbXbdDevCreate' method routines for all the VxBus
* device instances support the VxBus method to create disk drive(s).
*
* RETURNS: N/A.
*
* ERRNO: N/A.
*
*/

void usrVxbDiskDrv (void)
    {
    vxbDevMethodRun (VXB_DRIVER_METHOD(vxbXbdDevCreate), NULL);
    }
