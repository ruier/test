/* usrUsn2PrnInit.c - Generation 2 USB Printer class driver configlette file */

/*
 * Copyright (c) 2008 - 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,02apr10,ghs  Remove meaningless code(WIND00206008)
01b,24apr09,w_x  added default macro definitions
01a,11nov08,adh  written
*/

#ifndef __INCusrUsb2PrnInitc
#define __INCusrUsb2PrnInitc


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <usb2/usb2Prn.h>

/*******************************************************************************
 *
 * usrUsb2PrnInit - initializes the GEN2 Printer class driver.
 *
 * This function is initializing the GEN2 Printer class driver.
 * If called automatically by the project facility
 *
 * RETURNS: If the printer driver could be initialized or not
 *
 * ERRNO: N/A
 */

STATUS usrUsb2PrnInit()
    {
#ifdef USB_GEN2_PRINTER_NAME
    return usb2PrnInit(USB_GEN2_PRINTER_NAME);
#else
    return usb2PrnInit(NULL);
#endif
    }


#ifdef __cplusplus
}
#endif
#endif  /* __INCusrUsb2PrnInitc */
