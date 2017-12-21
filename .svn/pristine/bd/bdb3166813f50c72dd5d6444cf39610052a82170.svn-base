/* usrVxbSioChanUtil.c - VxBus SIO Channel Utilities configuration file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,10feb09,h_k  created.
*/

/*
DESCRIPTION

This is the VxBus SIO Channel Utilities configuration file for use with
project builds.

*/

/* includes */

#include <sioLib.h>

/* externs */

extern SIO_CHAN * vxbSerialChanGet (int sioChan);

/******************************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine returns a pointer to the SIO_CHAN device associated
* with a specified serial channel.  It is called by usrRoot() to obtain
* pointers when creating the system serial devices, `/tyCo/x'.  It
* is also used by the WDB agent to locate its serial channel.
*
* This routine first checks for BSP-supplied serial channels, then queries
* the bus subsystem for a serial channel matching the specified channel
* number.
*
* RETURNS: pointer to the SIO_CHAN structure for the channel, or ERROR
* if an invalid channel
*
* ERRNO
*/

SIO_CHAN * sysSerialChanGet
    (
    int sioChan
    )
    {
    return (vxbSerialChanGet (sioChan));
    }
