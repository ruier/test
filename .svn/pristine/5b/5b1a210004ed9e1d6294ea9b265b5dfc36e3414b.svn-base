/* usrUsbTargSerInit.c - Initialization of the CDC Serernet function driver */

/*
 * Copyright (c) 2009-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01f,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01e,13jun10,ghs  Save console tty number
01d,13jan10,ghs  vxWorks 6.9 LP64 adapting
01c,15sep09,w_x  Added INCLUDE_AT91_USB_TARG support (WIND00182308)
01b,09sep09,w_x  Added various BSP config options for FSL TCD (WIND00180575)
01a,05mar09,w_x  Created
*/

/*
DESCRIPTION

This files contains the initialization function for the Ser function
driver on the peripheral.

INCLUDE FILES: vxWorks.h, stdio.h, usb/usbPlatform.h, usb/usb.h,
usb/usbLib.h, usb/target/usbTargLib.h, usb/target/usbTargSerLib.h,
usb/target/usbTcdPdiusbd12EvalLib.h, usb/target/usbTcdIsp1582EvalLib.h,
usb/target/usbTcdNET2280Lib.h, drv/usb/target/usbTcdFslLib.h
*/

#ifndef __INCusrUsbTargSerInitc
#define __INCusrUsbTargSerInitc

/* includes */

#include "vxWorks.h"
#include "stdio.h"
#include "string.h"                     /* memset(), memcpy() */
#include "usb/usbPlatform.h"            /* Basic definitions  */
#include "usb/usb.h"
#include "usb/usbLib.h"
#include "usb/usbPciLib.h"              /* PCI interface      */
#include "usb/target/usbTargLib.h"
#include "usb/usbCdc.h"

#ifdef INCLUDE_PDIUSBD12
#include "drv/usb/target/usbTcdPdiusbd12EvalLib.h"
#endif /* INCLUDE_PDIUSBD12 */

#ifdef INCLUDE_PHILIPS1582
#include "drv/usb/target/usbTcdIsp1582EvalLib.h"
#endif /* INCLUDE_PHILIPS1582 */

#ifdef INCLUDE_NET2280
#include "drv/usb/target/usbTcdNET2280Lib.h"
#endif /* INCLUDE_NET2280 */

#ifdef INCLUDE_FSL_TCD
#include "drv/usb/target/usbTcdFslLib.h"
#include "drv/usb/target/usbTcdFslConfig.h"
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_AT91_USB_TARG
#include "usbAt91.h"
#include "usbAt91EvalLib.h"
#endif /* INCLUDE_AT91_USB_TARG */

INT32 usbTargSerSysConNum = CONSOLE_TTY;

LOCAL BOOL                      targInit = FALSE;
LOCAL USB_TARG_CHANNEL          SerTargChannel = NULL;
LOCAL BOOL                      targEnable = FALSE;

IMPORT void usbTargSerSioDrvRegister (void);

IMPORT VOID usbTargSerCallbackInfo
        (
        struct usbTargCallbackTable **  ppCallbacks,   /* USB_TARG_CALLBACK_TABLE */
        pVOID *                         pCallbackParam /* Callback Parameter */
        );

#ifdef INCLUDE_FSL_TCD
/*******************************************************************************
*
* usrUsbSerInitFsl - attaches peripheral stack to the FSL Target Controller
*
* This function initializes the peripheral stack and attaches it with the
* FSL Target Controller by calling function usbTargTcdAttach with
* appropiate parameters. The Callback table inforamtion is obtained prior
* to attachment process by calling funtion usbTargMsCallbackInfo.
* The TCD defined parameters are obtained by calling the pci routines
*
* RETURNS: OK or ERROR if not able to attach to target controller
*
* ERRNO: none
*
*\NOMANUAL
*/

STATUS usrUsbSerInitFsl
    (
    void
    )
    {
    USB_TCD_FSL_PARAMS_TYPE     paramsFsl;
    pUSB_TARG_CALLBACK_TABLE    callbackTable;
    pVOID                       callbackParam;

#ifdef FSL_DR_DMA_SUPPORTED

    UINT16                      dmaIrq = (UINT16)(FSL_DMA_IRQ);

    UINT16                      msgIrq = (UINT16)(FSL_MSG_IRQ);

#endif /* FSL_DR_DMA_SUPPORTED */

    FILE *                      fout = stdioFp(1);

    /* if already initialized, just show a warning */

    if (targInit)
        {
        fprintf (fout,"Already initialized.\n");

        return (OK);
        }

    usbTargSerCallbackInfo (&callbackTable, &callbackParam);

    /* Initialize usbTargLib */

    if (usbTargInitialize () == OK)
        {
        fprintf (fout,"usbTargInitialize() returned OK\n");
        }
    else
        {
        fprintf (fout,"usbTargInitialize() returned ERROR\n");

        return (ERROR);
        }

    targInit = TRUE;

    /* Attach the FSL TCD to usbTargLib */

    memset (&paramsFsl, 0, sizeof (paramsFsl));

    /*
     * These config options should be configured
     * according to the silicon implementation
     */

    /* Get the base address */

    paramsFsl.ioBase = FSL_USB_DR_DEVICE_IO_BASE;

    /* Get the IRQ number */

    paramsFsl.usbIrq = FSL_USB_DR_DEVICE_INT_NUM;

    /*
     * Get the hardware initialization routine for
     * basic hardware such as IOMUX initlialization.
     */

    paramsFsl.hwInitRoutine = (FUNCPTR)FSL_USB_DR_DEVICE_HW_INIT;

    /* Get the PHY mode */

    paramsFsl.phyMode = FSL_USB_DR_DEVICE_PHY_MODE;

    /* Does the silicon include sysconfig registers ? */

    paramsFsl.hasSysConfig = FSL_USB_DR_DEVICE_SYS_CONF;

    /* Are the USB operational registers (e.g. USBCMD) in big endian mode ? */

    paramsFsl.hasBigEndianDesc = FSL_USB_DR_DEVICE_BE_DESC;

    /* Are the USB descriptors (e.g. dQH and dTD) in big endian mode ? */

    paramsFsl.hasBigEndianReg = FSL_USB_DR_DEVICE_BE_REG;

    /* Is the setup packet buffer in big endian ? */

    paramsFsl.hasBigEndianSetup = FSL_USB_DR_DEVICE_BE_SETUP;

    /* Is the USBMODE has Ednain Selection field ? */

    paramsFsl.hasBufferES = FSL_USB_DR_DEVICE_BUFFER_ES;

#ifdef FSL_DR_DMA_SUPPORTED

    paramsFsl.dmaIrq = dmaIrq;

    paramsFsl.msgIrq = msgIrq;
#endif /* FSL_DR_DMA_SUPPORTED */

    if (usbTargTcdAttach (usbTcdFslExec,
                          (pVOID) &paramsFsl,
                          callbackTable,
                          callbackParam,
                          &SerTargChannel) == OK)
        {
        fprintf(fout,"usbTargTcdAttach() returned OK\n");
        }
    else
        {
        fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
        SerTargChannel = NULL;
        return ERROR;
        }

    fprintf (fout, "targChannel = 0x%x\n", (UINT32) SerTargChannel);

    if (usbTargEnable (SerTargChannel) == OK)
        {
        fprintf (fout, "usbTargEnable() returned OK\n");
        targEnable = TRUE;
        }
    else
        {
        fprintf (fout, "usbTargEnable() returned ERROR\n");
        return ERROR;
        }

    fprintf (fout, "FSL TCD initialized\n");

    return (OK);
    }
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_AT91_USB_TARG

/*******************************************************************************
*
* usrUsbSerInitAt91 - attaches peripheral stack to the AT91 Target Controller
*
* This function initializes the peripheral stack and attaches it with the
* AT91 Target Controller by calling function usbTargTcdAttach with
* appropiate parameters. The Callback table inforamtion is obtained prior
* to attachment process by calling funtion usbTargMsCallbackInfo.
* The TCD defined parameters are obtained by calling the pci routines
*
* RETURNS: OK or ERROR if not able to attach to target controller
*
* ERRNO: none
*
*\NOMANUAL
*/

STATUS usrUsbSerInitAt91
    (
    void
    )
    {
    USB_TCD_AT91_PARAMS paramsAt91;
    pUSB_TARG_CALLBACK_TABLE callbackTable;
    pVOID callbackParam;
    UINT32 ioBase=(UINT32)(UDP_BASE);
    UINT16 usbIrq=(UINT16)(INT_LVL_USB_DEVICE);

    FILE *fout = stdioFp(1);    /* stream for output (if any) */

    /* if already initialized, just show a warning */

    if (targInit)
        {
        fprintf (fout,"Already initialized.\n");
        return (OK);
        }

    usbTargSerCallbackInfo (&callbackTable, &callbackParam);

    /* Initialize usbTargLib */

    if (usbTargInitialize () == OK)
        {
        fprintf (fout,"usbTargInitialize() returned OK\n");
        }
    else
        {
        fprintf (fout,"usbTargInitialize() returned ERROR\n");
        return (ERROR);
        }

    targInit = TRUE;

    /* Attach the AT91 TCD to usbTargLib */

    memset (&paramsAt91, 0, sizeof (paramsAt91));

    /* Get the base address */

    paramsAt91.ioBase = ioBase;

    /* Get the IRQ */

    paramsAt91.irq = usbIrq;

    fprintf (fout,"AT91 TCD: ioBase = 0x%lx, usbIrq = %d\n",
             ioBase, usbIrq);

    if (usbTargTcdAttach (usbTcdAt91EvalExec, (pVOID) &paramsAt91,
                          callbackTable, callbackParam, &SerTargChannel) == OK)
        {
        fprintf(fout,"usbTargTcdAttach() returned OK\n");
        }
    else
        {
        fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
        SerTargChannel = NULL;
        return ERROR;
        }

    fprintf (fout, "targChannel = 0x%x\n", SerTargChannel);

    if (usbTargEnable (SerTargChannel) == OK)
        {
        fprintf (fout, "usbTargEnable() returned OK\n");
        targEnable = TRUE;
        }
    else
        {
        fprintf (fout, "usbTargEnable() returned ERROR\n");
        return ERROR;
        }

    fprintf (fout, "AT91 TCD initialized\n");

    return (OK);
    }
#endif /* INCLUDE_AT91_USB_TARG */

/*******************************************************************************
*
* usbTargSerInit - initializes USB CDC serial emulator functionality driver
*
* This routine initializes the peripheral stack and attaches it with
* the included Target Controller with USB CDC serial emulator functionality.
*
* RETURNS: OK or ERROR if not able to attach to the TCD
*
* ERRNO: none
*/

void usbTargSerInit (void)
    {
    STATUS sts = ERROR;

#ifdef INCLUDE_FSL_TCD
    sts = usrUsbSerInitFsl();
#elif defined(INCLUDE_AT91_USB_TARG)
    sts = usrUsbSerInitAt91();
#endif

    if (sts == OK)
        {
        usbTargSerSioDrvRegister();
        }

    return;
    }

#endif /* __INCusrUsbTargSerInitc */
