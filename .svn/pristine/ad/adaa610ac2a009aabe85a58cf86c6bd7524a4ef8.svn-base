/* usrUsbTargInit.c - WindView Instrumentation function Definition */

/*
 * Copyright (c) 2004-2005, 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01i,13jan10,ghs  vxWorks 6.9 LP64 adapting
01h,24jan05,hch  Removed the #if 0 code
01g,20jan05,hch  Corrected the _WRS_VXWORKS_MAJOR usage
01f,31oct04,hch  Remove compiler warning for usb2EvLog
01e,15oct04,ami  Apigen Changes
01d,11oct04,ami  Apigen Changes
01c,06oct04,ami  SPR #94684 fix
01b,04oct04,tcr  update WindView logging
01a,12aur04,ami  First
*/

/*
DESCRIPTION

This file consist of the function definition for USB 2.0 Peripheral Stack
WindView Events.

INCLUDE FILES: string.h, wvLib.h
*/

#ifndef __INCusrUsbTargInitc
#define __INCusrUsbTargInitc

/* includes */

#include "string.h"

#ifdef INCLUDE_WINDVIEW
#include "wvLib.h"
#endif


/* defines */
#define USB2_WV_LOGSIZE          80       /* Wind View Event Log Message Size */

/* locals */

#ifdef INCLUDE_WINDVIEW
LOCAL char usb2EvLog[USB2_WV_LOGSIZE];    /* buffer for event logging */
#endif

UINT32     usbPeriphWvFilter = 0x0;       /* user controllable filter */

/*******************************************************************************
*
* usbPeriphWindViewLog - USB2.0 Peripheral Stack WindView Log Events
*
* The USB2 Peripheral Stack is instrumented to provide additional WindView User
* log events for function entries/exits tracing within the Peripheral Stack.
* Logging is based whether or not Filter value has been initialized with
* the appropriate filter mask. If WindView is not enabled and the correct
* filter mask is set, then the WindView user event message will be logged
* to the standard output.
*
* \ss
* #define USB_TARG_WV_FILTER	        0x00000001
* #define USB_HAL_WV_FILTER	        0x00000002
* #define USB_TCD_ISP582_WV_FILTER      0x00000004
* #define USB_TCD_PDIUSBD12_WV_FILTER   0x00000008
* \se
*
* RETURNS: N/A
*
* ERRNO: none
*/

VOID usbPeriphWindViewLog
    (
    UINT32 evId,
    char *buffer,
    UINT32 mask
    )
    {

    if ((mask & usbPeriphWvFilter) != 0)
        {
#ifdef INCLUDE_WINDVIEW
  #if (defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
        int     length;

	strncpy(usb2EvLog, buffer, USB2_WV_LOGSIZE);
        usb2EvLog [USB2_WV_LOGSIZE - 1] = '\0';
        length = strlen (usb2EvLog);
        EVT_STR_N(evId, 0, 0, 0, 0, 0, 0, 0,
                  length, usb2EvLog);
  #else /* _WRS_VXWORKS_MAJOR */
	strncpy(usb2EvLog, buffer, USB2_WV_LOGSIZE);
        usb2EvLog [USB2_WV_LOGSIZE - 1] = '\0';
        wvEvent (evId, usb2EvLog, strlen (usb2EvLog));
  #endif /* _WRS_VXWORKS_MAJOR */
#else /* INCLUDE_WINDVIEW */
  #ifdef INCLUDE_LOGGING
    logMsg("USB2 wvEvent: %s\n",(int)buffer,0,0,0,0,0);
  #endif
#endif /* INCLUDE_WINDVIEW */
        }
    }


/*****************************************************************************
*
* usbPeriLogMsg - dumps the log messages
*
* This function dumps the content of the log messages. Before dumping the
* contents it checks whether the INCLUDE_LOGGING is defined.
*
* RETURNS : N/A
*
* ERRNO: none
*/

void usbPeriLogMsg
    (
    char  * msg,
    long  a1,
    long  a2,
    long  a3,
    long  a4,
    long  a5,
    long  a6
    )
    {

#ifdef INCLUDE_LOGGING
    logMsg (msg,a1,a2,a3,a4,a5,a6);
#endif

    return;
    }

#endif /* __INCusrUsbTargInitc */
