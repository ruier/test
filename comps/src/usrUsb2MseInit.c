/* usrUsb2MseInit.c - Generation 2 USB mouse class driver configlette file */

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

#ifndef __INCusrUsb2MseInitc
#define __INCusrUsb2MseInitc


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <usb2/usb2Mse.h>

/*******************************************************************************
 *
 * usrUsb2MseInit - initialize the GEN2 mouse class driver.
 *
 * This routine initializes the GEN2 Mouse class driver.
 * It will be called automatically by the project facility
 *
 * RETURNS: If the Mouse driver could be initialized or not
 *
 * ERRNO: N/A
 */

STATUS usrUsb2MseInit()
    {
#ifdef USB_GEN2_MOUSE_NAME
    return usb2MseInit(USB_GEN2_MOUSE_NAME);
#else
    return usb2MseInit(NULL);
#endif
    }


#ifdef __cplusplus
}
#endif
#endif  /* __INCusbUsb2MseInitc */
