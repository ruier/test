/* sysSerial.c - solaris & linux serial device initialization */

/*
 * Copyright (c) 1997-2007,2010,2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

/*
modification history
--------------------
01m,12jun13,elp  Added bspSerialChanGet() (WIND004172855).
01l,08feb10,elp  fixed 64bits warnings.
01k,13feb07,elp  merged sio drivers.
01j,26mar04,jeg  added host serial device support (hostSio).
01i,09mar04,jmp  rename vxsimHostFdInit to vxsimHostTtyInit.
01h,28apr03,dbt  Added sysIntHandler() (moved from sysLib.c).
01g,05feb03,jeg  removed PPP specific code. Updated access to host specific			 routines.
01f,19dec01,jmp  fixed sysSerialPppPoll() to no longer use u_select() with
                 VxWorks' fd_set and FD_SETSIZE, use simReadSelect() instead.
01e,06jun01,jmp  added SIMSPARCSOLARIS support.
01d,22dec00,jmp  switched from SLIP to PPP.
01c,01dec00,jmp  added flush of SLIP serial line receive buffer in
                 sysSerialHwInit2.
01b,21nov00,jmp  replaced L_CNOTTY by L_NOCTTY.
		 fixed typo in INCLUDE_SLIP.
01a,10nov00,hbh	 written based on SIMHPPA version.
*/

/*
DESCRIPTION

This library contains routines for linux and solaris serial device
initialization.
*/

/*
DESCRIPTION
This source file is not usually compiled by itself.  It is #included
from sysLib.c.  The purpose of this source file is to isolate the
serial hardware setup from the bulk of the sysLib.c module.  BSPs using
the same serial devices should be able to use the sysSerial.c from
another BSP with very few modifications.

HOST SERIAL DEVICE
If the macro INCLUDE_HOST_SIO is defined into config.h, then the "/tyCo/1" is 
used to access the host serial device defined by HOST_SIO_PORT_NUMBER macro.
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <vxsimHostLib.h>
#include <string.h>
#include <iv.h>
#include <intLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <config.h>
#include <simSio.h>

/* defines */

#ifdef INCLUDE_HOST_SIO
#include "hostSio.c"
#endif

/* globals */

SIM_SIO_CHAN simDefaultChan;	/* Hardware device structure */

/*
 * Array of pointers to all serial channels configured in system.
 * See sioChanGet(). It is this array that maps channel pointers
 * to standard device names.  The first entry will become "/tyCo/0",
 * the second "/tyCo/1", and so forth.
 *
 * See sysSerialHwInit ().
 */

SIO_CHAN * sysSioChans [NUM_TTY];


/* static forward declarations */

#if CPU!=SIMNT
LOCAL void sysSerialIntHandler (void);
#endif /* CPU!=SIMNT */

/******************************************************************************
*
* sysSerialHwInit - initialize the BSP serial devices to a quiesent state
*
* This routine initializes the BSP serial device descriptors and puts the
* devices in a quiescent state.  It is called from sysHwInit() with
* interrupts locked.  Polled mode serial operations are possible, but not
* interrupt mode.  Interrupt mode is enabled in sysSerialHwInit2().
*
* RETURNS: N/A
*/ 

void sysSerialHwInit (void)
    {
    sysSioChans[0] = (SIO_CHAN *) &simDefaultChan;
    snprintf (simDefaultChan.desc, SIM_SIO_DESC_MAX_LEN - 1, "default");
    simSioDevInit (&simDefaultChan);

#ifdef INCLUDE_HOST_SIO
    sysSioChans[1] = (SIO_CHAN *) &hostSioChan;
    hostSioDevInit (&hostSioChan, HOST_SIO_PORT_NUMBER);
#endif
    }

/******************************************************************************
*
* sysSerialHwInit2 - serial initialization, part 2
*
* This routine usually connects the serial device interrupts, and calls the
* second step driver initializations if any.  It is called from
* sysHwInit2().  Serial device interrupts could not be connected in
* sysSerialHwInit() because the kernel memory allocator was not initialized
* at that point, and intConnect() calls malloc().
*
* RETURNS: N/A
*/ 

void sysSerialHwInit2 (void)
    {
    static int done = 0;

    if (done)
	return;
    done = 1;

#if CPU!= SIMNT
    /* ^C */

    (void)intConnect ((VOIDFUNCPTR*)(long)IV_INT, sysSerialIntHandler, 0);
#endif /* CPU != SIMNT */

    /* enable interrupt mode of operation */
    
    if (simDefaultChan.created)
	{
	(void)intConnect ((void *)(long)simDefaultChan.intVec,
			  simSioIntRcv, (_Vx_usr_arg_t) &simDefaultChan);
	simSioDevInit2 (&simDefaultChan);
	}

#ifdef INCLUDE_HOST_SIO
    if (hostSioChan.created)
	{
	/* 
	 * the intConnect can not be done now because the host file descriptor
	 * will be known only when the device will be opened.
	 */

	hostSioDevInit2 (&hostSioChan);
	}
#endif
    }

/******************************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine returns a pointer to the SIO_CHAN device associated
* with a specified serial channel.  It is used by usrRoot to obtain 
* pointers for creating all the system serial devices, "/tyCo/x".  It
* is also how the wdb agent locates the proper serial channel when using
* serial communications to the target server.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*/

SIO_CHAN * sysSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    if (channel < 0 || channel >= NELEMENTS (sysSioChans))
	return ((SIO_CHAN *) ERROR);

    return sysSioChans [channel];
    }


/******************************************************************************
*
* sysSerialReset - reset the serial devices to a quiescent state
*
* This routine resets the serial device. It is called by sysToMonitor().
*
* RETURNS: N/A
*
* SEE ALSO: sysToMonitor()
*/

void sysSerialReset (void)
    {
    simSioDevInit2 (&simDefaultChan);

#ifdef INCLUDE_HOST_SIO
    hostSioDevInit2 (&hostSioChan);
#endif

    }

#if CPU!=SIMNT
/*******************************************************************************
*
* sysSerialIntHandler - Unix SIGINT handler
*
* When Unix SIGINT signal is received, we must send the ^C character on
* the serial line to simulate a real VxWorks target behavior.
*
*/

LOCAL void sysSerialIntHandler (void)
    {

    SIM_SIO_CHAN * pChan = (SIM_SIO_CHAN *) sysSerialChanGet(0);

    /* 0x3 is ^C */

    (*pChan->putRcvChar) (pChan->putRcvArg, 0x3);
    }
#endif /* CPU!=SIMNT */

#ifdef INCLUDE_SIO_UTILS
/******************************************************************************
*
* bspSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* The sysSerialChanGet() routine returns a pointer to the SIO_CHAN
* device associated with a specified serial channel. It is called
* by usrRoot() to obtain pointers when creating the system serial
* devices, `/tyCo/x'. It is also used by the WDB agent to locate its
* serial channel.  The VxBus function requires that the BSP provide a
* function named bspSerialChanGet() to provide the information about
* any non-VxBus serial channels, provided by the BSP.  As this BSP
* does not support VxBus serial channels, this routine always
* returns ERROR.
*
* RETURNS: ERROR, always
*
* ERRNO: N/A
*
* \NOMANUAL
*/

SIO_CHAN * bspSerialChanGet
    (
    int channel     /* serial channel */
    )
    {
    return ((SIO_CHAN *) ERROR);
    }
#endif /* INCLUDE_SIO_UTILS */
