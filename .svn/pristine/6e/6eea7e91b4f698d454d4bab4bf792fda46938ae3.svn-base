/* usrUsbTargKbdInit.c - Initialization of the keyboard function driver */

/*
 * Copyright (c) 2004, 2006, 2009-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01l,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01k,13jan10,ghs  vxWorks 6.9 LP64 adapting
01j,15sep09,w_x  Added INCLUDE_AT91_USB_TARG support (WIND00182308)
01i,09sep09,w_x  Added various BSP config options for FSL TCD (WIND00180575)
01h,30jan09,w_x  Added FSL dual role device support
01g,16jan06,ami  Warning message Cleanup (SPR #116698)
01f,15oct04,ami  RefGen Changes
01e,20sep04,ami  NET2280 tested in High Speed
01d,20jul04,???  targChannel changed to kbdTargChannel
01c,19jul04,ami  Coding Convnetion Changes
01b,09jul04,pdg  isp1582 support added
01a,20apr04,pdg  First
*/

/*
DESCRIPTION

This files contains the initialization function for the keyboard function driver
on the peripheral.

INCLUDE FILES: usb/usbPlatform.h, usb/target/usbHalCommon.h,
drv/usb/target/usbTcdPdiusbd12EvalLib.h, usb/target/usbTargLib.h,
usb/usbHid.h, drv/usb/target/usbTargKbdLib.h drv/usb/target/usbTcdFslLib.h

*/

#ifndef __INCusrUsbTargKbdInitc
#define __INCusrUsbTargKbdInitc

/* includes */

#include "usb/usbPlatform.h"
#include "usb/target/usbHalCommon.h"
#include "usb/target/usbTargLib.h"
#include "usb/usbHid.h"
#include "drv/usb/target/usbTargKbdLib.h"

#ifdef INCLUDE_PDIUSBD12
#include "drv/usb/target/usbTcdPdiusbd12EvalLib.h"

#define D12EVAL_DEFAULT_IOBASE    0x368     /* default base I/O address */
#define D12EVAL_DEFAULT_IRQ        7        /* default IRQ channel */
#define D12EVAL_DEFAULT_DMA        3        /* default DMA channel */

#endif

#ifdef INCLUDE_PHILIPS1582
    #include "drv/usb/target/usbTcdIsp1582EvalLib.h"
    extern void sysIsp1582PciInit (long *, long *);
#endif

#ifdef INCLUDE_NET2280
#include "drv/usb/target/usbTcdNET2280Lib.h"
    extern void sys2NET2280PciInit (void);
    extern UINT32 BADDR_NET2280 [NET2280_NO_OF_PCI_BADDR];
    extern UINT16 IRQ_NET2280;
#endif

#ifdef INCLUDE_FSL_TCD
#include "drv/usb/target/usbTcdFslLib.h"
#include "drv/usb/target/usbTcdFslConfig.h"
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_AT91_USB_TARG
#include "usbAt91.h"
#include "usbAt91EvalLib.h"
#endif /* INCLUDE_AT91_USB_TARG */

/* locals */

LOCAL USB_TARG_CHANNEL kbdTargChannel = NULL;   /* target channel */

/***************************************************************************
*
* usrUsbTargKbdInit - initialization function for keyboard.
*
* This is the initialization function which associates the keyboard function
* driver to the TCD.
*
* RETURNS: N/A
*
* ERRNO:
*   None.
*/

void usrUsbTargKbdInit (void)
    {
#ifdef INCLUDE_PDIUSBD12
    USB_TCD_PDIUSBD12_PARAMS params;  /* USB_TCD_PDIUSBD12_PARAMS */
#endif

#ifdef INCLUDE_PHILIPS1582
    USB_TCD_ISP1582_PARAMS   params1582;  /* USB_TCD_ISP1582_PARAMS */
    long        ioBase = 0;               /* base address */
    long        irq = 0;                  /* irq number */
#endif

#ifdef INCLUDE_NET2280
    USB_TCD_NET2280_PARAMS paramsNET2280;
#endif

#ifdef INCLUDE_FSL_TCD
    USB_TCD_FSL_PARAMS_TYPE     paramsFsl;
    UINT32                      ioBase = 0;
    UINT32                      usbIrq = 0;
#ifdef FSL_DR_DMA_SUPPORTED
    UINT16                      dmaIrq = (UINT16)(FSL_DMA_IRQ);
    UINT16                      msgIrq = (UINT16)(FSL_MSG_IRQ);
#endif /* FSL_DR_DMA_SUPPORTED */
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_AT91_USB_TARG
    USB_TCD_AT91_PARAMS paramsAt91;
    UINT32 ioBase = (UINT32)(UDP_BASE);
    UINT16 usbIrq = (UINT16)(INT_LVL_USB_DEVICE);
#endif /* INCLUDE_AT91_USB_TARG */

    pUSB_TARG_CALLBACK_TABLE callbackTable = NULL;   /* callback table */
    pVOID                    callbackParam = NULL;   /* callback param */

    /* Initialize usbTargLib */

    if (usbTargInitialize () == OK)
        {
        printf ("usbTargInitialize() returned OK\n");
        }
    else
        {
        printf ("usbTargInitialize() returned ERROR\n");
        return;
        }

    usbTargKbdCallbackInfo (&callbackTable, &callbackParam);

#ifdef INCLUDE_PDIUSBD12
    /* Attach the Philips TCD to usbTargLib */

    memset (&params, 0, sizeof (params));
    params.ioBase = D12EVAL_DEFAULT_IOBASE;
    params.irq = D12EVAL_DEFAULT_IRQ;
    params.dma = D12EVAL_DEFAULT_DMA;

    if (usbTargTcdAttach (usbTcdPdiusbd12EvalExec,
                         (pVOID) &params,
                          callbackTable,
                          callbackParam,
                          &kbdTargChannel) == OK)
        {
        printf ("usbTargTcdAttach() returned OK\n");
        }
    else
        {
        printf ("usbTargTcdAttach() returned ERROR\n");
        kbdTargChannel = NULL;
        return;
        }

    printf ("targChannel D12 = 0x%x\n", (UINT32) kbdTargChannel);

    /* Call the function to enable the Target Controller */

    if (usbTargEnable (kbdTargChannel) == OK)
        {
        printf ("usbTargEnable() returned OK\n");
        }
    else
        {
        printf ("usbTargEnable() returned ERROR\n");
        }

#endif

#ifdef INCLUDE_PHILIPS1582

    sysIsp1582PciInit (&ioBase , &irq);

    /* Attach the Philips ISP 1582 to usbTargLib */

    memset (&params1582, 0, sizeof (params1582));
    params1582.ioBase = ioBase;
    params1582.irq = (UINT16) irq;
    printf ("Philips ISP 1582: ioBase = 0x%lx, irq = %d\n",
    ioBase, irq);

    if (usbTargTcdAttach (usbTcdIsp1582EvalExec,
                         (pVOID) &params1582,
                          callbackTable,
                          callbackParam,
                          &kbdTargChannel) == OK)
        {
        printf ("usbTargTcdAttach() returned OK\n");
        }
    else
        {
        printf ("usbTargTcdAttach() returned ERROR\n");
        kbdTargChannel = NULL;
        return;
        }

    printf ("targChannel 1582 = 0x%x\n", kbdTargChannel);

    /* Call the function to enable the Target Controller */

    if (usbTargEnable (kbdTargChannel) == OK)
        {
        printf ("usbTargEnable() returned OK\n");
        }
    else
        {
        printf ("usbTargEnable() returned ERROR\n");
        }
#endif

#ifdef INCLUDE_NET2280

    sys2NET2280PciInit ();

    /* Attach the NetChip NET2280 TCD to usbTargLib */

    memset (&paramsNET2280, 0, sizeof (paramsNET2280));

    /* Get the base address for the Configureation Regsiter */

    paramsNET2280.ioBase [0] = BADDR_NET2280 [0];


    /* Get the base address for the 8051 MicroController */

    paramsNET2280.ioBase [1] = BADDR_NET2280 [1];


    /* Get the base address for the FIFO Buffer */

    paramsNET2280.ioBase [2] = BADDR_NET2280 [2];


    paramsNET2280.irq = (UINT16) IRQ_NET2280;


    if (usbTargTcdAttach (usbTcdNET2280Exec,
                          (pVOID)&paramsNET2280,
                          callbackTable,
                          callbackParam,
                          &kbdTargChannel) != OK)
        {
        printf ("usbTargTcdAttach() returned ERROR\n");
        kbdTargChannel = NULL;
        return;
        }

    if (usbTargEnable (kbdTargChannel) != OK)
        {
        printf ("usbTargEnable() returned ERROR\n");
        return;
        }

    printf ("NET2280 TCD initialized\n");

#endif

#ifdef INCLUDE_FSL_TCD

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
                          &kbdTargChannel) == OK)
        {
        printf("usbTargTcdAttach() returned OK\n");
        }
    else
        {
        printf("usbTargTcdAttach() returned ERROR\n");
        kbdTargChannel = NULL;
        return;
        }

    if (usbTargEnable (kbdTargChannel) == OK)
        {
        printf ("usbTargEnable() returned OK\n");
        }
    else
        {
        printf ("usbTargEnable() returned ERROR\n");
        return;
        }
    printf ("FSL TCD initialized\n");
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_AT91_USB_TARG

    /* Attach the AT91 TCD to usbTargLib */

    memset (&paramsAt91, 0, sizeof (paramsAt91));

    /* Get the base address */

    paramsAt91.ioBase = ioBase;

    /* Get the IRQ */

    paramsAt91.irq = usbIrq;

    printf ("AT91 TCD: ioBase = 0x%lx, usbIrq = %d\n",
             ioBase, usbIrq);

    if (usbTargTcdAttach (usbTcdAt91EvalExec,
                          (pVOID)&paramsAt91,
                          callbackTable,
                          callbackParam,
                          &kbdTargChannel) == OK)
        {
        printf("usbTargTcdAttach() returned OK\n");
        }
    else
        {
        printf ("usbTargTcdAttach() returned ERROR\n");
        kbdTargChannel = NULL;
        return;
        }

    if (usbTargEnable (kbdTargChannel) == OK)
        {
        printf ("usbTargEnable() returned OK\n");
        }
    else
        {
        printf ("usbTargEnable() returned ERROR\n");
        return;
        }

    printf ("AT91 TCD initialized\n");

#endif

    return;
    }

#endif /* __INCusrUsbTargKbdInitc */

