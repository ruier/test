/* usrUsb2KbdInit.c - Generation 2 USB Keyboard class driver configlette file */

/*
 * Copyright (c) 2008-2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01s,03sep13,wyy  Delete unused code base on WIND00429264
01r,09aug13,wyy  Make INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH cowork with
                 INCLUDE_PC_CONSOLE (WIND00429264)
01q,06may13,s_z  Remove compiler warning (WIND00356717)
01p,02may13,ghs  Modify attach to shell work mode (WIND00415711)
01o,12aug11,ghs  Allow only one keyboard attach to shell (WIND00291443)
01n,08jul11,ghs  Give a delay when shell ready for attached (WIND00286684)
01m,23dec10,ghs  Fix build error for bootshell attach (WIND00247103)
01l,23nov10,ghs  Correct including filename (WIND00242712)
01k,05jul10,m_y  Modify for coding convention
01j,11may10,m_y  Correct usb2KbdRcvCallback parameter to align with
                 prototype (WIND00211587)
01i,23mar10,j_x  Changed for USB debug (WIND00184542)
01h,03mar10,ghs  Fix LP64 adapting(WIND00202098)
01g,13jan10,ghs  vxWorks 6.9 LP64 adapting
01f,29oct09,ghs  Define new GEN2 debug message(WIND00188349)
01e,11sep09,ghs  Remove warning message
01d,09sep09,y_l  Add bootAppShell support (WIND00130193)
01c,24apr09,w_x  added default macro definitions
01b,17apr09,adh  added feature to attach keyboard to shell
01a,11nov08,adh  written
*/



#ifndef __INCusrUsb2KbdInitc
#define __INCusrUsb2KbdInitc


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <usb2/usb2Kbd.h>
#include <usb2/usb2KbdMap.h>

/* 
 * USB keyboard supports two ways as a SHELL input device. One way is a   
 * keyboard bound up with a TTY device. Serial channel will show output of
 * the keyboard in this way. The other way is a keyboard worked as a 
 * PC_CONSOLE device. Graphic channel will show output of the keyboard in 
 * this way.
 * Define INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH or 
 * INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH will bind a keyboard to a TTY
 * device or a PC_CONSOLE device. The binding object depends on whether 
 * INCLUDE_PC_CONSOLE is defined.
 */

#if defined (INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH) || \
    defined (INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH)

#include <tyLib.h>

extern DEV_HDR * iosDevMatch (const char * name);

#endif /* INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH ||
          INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH */

/* Default keyboard mapping used */

extern USB2_KBD_MAP usb2Kbd_Map_US;


/*******************************************************************************
 *
 * usrUsb2KbdInit - initializes the GEN2 Keyboard class driver.
 *
 * This function is initializing the GEN2 Keyboard class driver.
 * If called automatically by the project facility
 *
 * RETURNS: If the keyboard driver could be initialized or not
 *
 * ERRNO: N/A
 */

STATUS usrUsb2KbdInit()
    {
#ifdef USB_GEN2_KEYBOARD_TYPEMATIC_MS
    UINT32 idleRate = (UINT32)USB_GEN2_KEYBOARD_TYPEMATIC_MS;
#else
    UINT32 idleRate = 128;
#endif

    /* Allocate & create the keyboard mappings */

    usb2KbdMapInit();

    usb2KbdMapAdd(&usb2Kbd_Map_US);

    /*
     * Initialize the driver itself
     */

#ifdef USB_GEN2_KEYBOARD_NAME
    return usb2KbdInit(USB_GEN2_KEYBOARD_NAME, idleRate, 0);
#else
    return usb2KbdInit(NULL, idleRate, 0);
#endif
    }

/*******************************************************************************
*
* usb2KbdGetConsole - return console instance for keyboard attach
*
* This routine returns console instance for keyboard attach. The return value
* depends on configuration. If shell attach component is configured, it will
* return tty device instance; If boot shell attach component is configured. it
* will return PC console instance. If no shell attach is configured, it will
* return NULL.
*
* RETURNS: device instance.
*
* ERRNO: N/A
*
* \NOMANUAL
*/

TY_DEV_ID usb2KbdGetConsole
    (
    void
    )
    {
#if defined (INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH) || \
    defined (INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH)
#if !defined (INCLUDE_PC_CONSOLE)
    char ttyDevName[20];
    
    snprintf(ttyDevName, sizeof(ttyDevName), "/tyCo/%d", CONSOLE_TTY);
    
    return (TY_DEV_ID)iosDevMatch(ttyDevName);
#else
    return (TY_DEV_ID)pcConDevBind(PC_CONSOLE, NULL, NULL);
#endif
#else
    return NULL;
#endif
    }

/*******************************************************************************
*
* usb2KbdIsRedirectTty - return if shell attach is configured
*
* This routine returns a boolean value if INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH
* is defined.
*
* RETURNS: TRUE, or FALSE
*
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL usb2KbdIsRedirectTty
    (
    void
    )
    {
#if defined (INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH) || \
    defined (INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH)
#if !defined (INCLUDE_PC_CONSOLE)
    return TRUE;
#else
    return FALSE;
#endif
#else 
    return FALSE;
#endif
    }

#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsb2KbdInitc */

