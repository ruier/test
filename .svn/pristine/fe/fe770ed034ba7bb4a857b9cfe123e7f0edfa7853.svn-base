/* usrUsbInit.c - configlette and initialization of the USB stack */

/*
 * Copyright (c) 2000, 2003-2007, 2009-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
02u,06may13,s_z  Remove compiler warning (WIND00356717)
02t,28apr13,ghs  Add support of USB message (WIND00411952)
02s,19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
02r,07jul11,ghs  Add usrUsbDebug.c to this file (WIND00286556)
02q,17mar11,j_x  Pick off USB configlette files from usrDepend.c
02p,16mar11,j_x  Add multiple inclusion guard symbol (WIND00247788)
02o,14jan11,w_x  Initial USB OTG support changes (WIND00211541)
02n,14jan11,ghs  Add GEN2 printer driver to class driver initialization
                 routine (WIND00250645)
02m,07jan11,ghs  Clean up compile warnings (WIND00247082)
02l,14dec10,ghs  Change the usage of the reboot hook APIs to specific
                 rebootHookAdd/rebootHookDelete (WIND00240804)
02k,07dec10,ghs  Combine USB HCD initialization to one group (WIND00245236)
02j,06sep10,j_x  Fix usb reboot hang issue (WIND00205038)
02i,05jul10,m_y  Modify for coding convention
02h,24may10,m_y  Add USB_WAIT_TIME_OUT_VALE define (WIND00183499)
02g,04mar10,m_y  Add USB_CHECK_RESUBMIT_URB_FLAG define.(WIND00200272)
02f,15mar10,j_x  Changed for USB debug (WIND00184542)
02e,13jan10,ghs  vxWorks 6.9 LP64 adapting
02d,09sep09,adh  Add USB_MEM_PART_SIZE define (WIND00142070)
02c,21aug09,ghs  Add USB_HUB_POLLING_INTERVAL define(WIND00160843)
02b,11jul07,jrp  Removing stdout announcements
02a,20mar07,sup  remove reference to the target/h/usb2 directory
01z,06oct06,ami  Changes for USB-vxBus porting
01y,20apr06,ami  typo in description of the file (SPR#120283)
01x,26jul05,ami  Function for deinitialization of stack added (SPR #110691)
01w,24jan05,hch  Removed #if 0 code
01v,20jan05,hch  Corrected the usage of _WRS_VXWORKS_MAJOR macro
01u,26oct04,ami  Debug Messages Changes
01t,15oct04,ami  Apigen Changes
01s,11oct04,ami  Apigen Changes
01r,06oct04,ami  SPR #94684 Fix
01q,04oct04,tcr  update WindView logging
01p,30aug04,pdg  ARM target support
01o,16aug04,pdg  Fix for long booting time of OHCI and EHCI Host controllers
                 in pentium
01n,02jun04,hch  Remove compiler warning for usb2EvLog.
01m,06apr04,cfc  Fix compiler warnings
01l,29oct03,cfc  USB2 Merge
01k,26sep03,cfc  Fix compiler warnings
01j,23sep03,cfc  Support for additional usb2 windview logging
01i,21sep03,cfc  Fix compiler warning
01h,18sep03,cfc  remove erroneous '\' char
01g,16sep03,cfc  Add WindView user event stub
01f,08sep03,cfc  usb Host Stack will initialize the Hub and usbd layers
01e,27aug03,cfc  Call HCD PCI setup routines
01d,11aug03,cfc  Support for USB2 Host Stack
01c,07aug03,cfc  support for usb2.0
01b,27feb00,wef  removed unecessary printfs, fixed return codes
01a,23aug00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes a USB stack. This process is done at
boot and is usually followed by the attachment of a USB controller.
The configlette usrUsbHcdXXXXInit.c handles the respective EHCI, OHCI
or UHCI host controller attachment.
*/

#ifndef __INCusrUsbInitc
#define __INCusrUsbInitc


/* includes */

#include <usb/usbdLib.h>
#include <usb/usbHst.h>
#include <usb/usbHubInitialization.h>
#include <usb/usbOtg.h>
#include <hwif/vxbus/vxBus.h>
#if defined(INCLUDE_BOOT_APP)
#include <private/bootUsbLoad.h>
#endif
#ifdef INCLUDE_WINDVIEW
#include <wvLib.h>
#endif
#include <string.h>

/* debug level define */

#include <usrUsbDebug.c>

/* Configlettes */

#undef USB_TOOL_MUTEX

#ifdef INCLUDE_USBTOOL
    #define USB_TOOL_MUTEX
    #define INCLUDE_USB
    #include <usrUsbTool.c>
#endif /* INCLUDE_USBTOOL */

#ifdef INCLUDE_USB_SHOW
    #define INCLUDE_USB
    #include <usrUsbShow.c>
#endif /* INCLUDE_USBSHOW */

#ifdef USB_TOOL_MUTEX
#undef INCLUDE_USB_INIT
#undef INCLUDE_USB_HEADSET_DEMO
#undef INCLUDE_USB_SPEAKER_INIT
#undef INCLUDE_USB_PEGASUS_END_INIT
#undef INCLUDE_USB_PRINTER_INIT
#undef INCLUDE_USB_KEYBOARD_INIT
#undef INCLUDE_USB_MOUSE_INIT
#undef INCLUDE_USB_MS_BULKONLY_INIT
#undef INCLUDE_USB_MS_CBI_INIT
#undef INCLUDE_USB_GEN2_KEYBOARD
#undef INCLUDE_USB_GEN2_KEYBOARD_INIT
#undef INCLUDE_USB_GEN2_MOUSE
#undef INCLUDE_USB_GEN2_MOUSE_INIT
#undef INCLUDE_USB_GEN2_PRINTER
#undef INCLUDE_USB_GEN2_PRINTER_INIT
#undef INCLUDE_USB_GEN2_STORAGE
#undef INCLUDE_USB_GEN2_STORAGE_INIT
#undef INCLUDE_USB_GEN2_END_INIT
#undef INCLUDE_USB_GEN2_SERIAL_INIT
#undef INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT
#undef INCLUDE_MS_EMULATOR_INIT
#undef INCLUDE_KBD_EMULATOR_INIT
#undef INCLUDE_PRN_EMULATOR_INIT
#undef INCLUDE_SER_EMULATOR_INIT
#endif /* USB_INIT_TOOL_MUTEX */

/* INIT components */

#ifdef INCLUDE_USB_INIT
    #define INCLUDE_USB
#endif /* INCLUDE_USB_INIT */
#ifdef INCLUDE_USB_HEADSET_DEMO
    #define INCLUDE_USB_SPEAKER_INIT
    #include <usrUsbBrcmAudio.c>
#endif /* INCLUDE_USB_HEADSET_DEMO */
#ifdef INCLUDE_USB_SPEAKER_INIT
    #define INCLUDE_USB_SPEAKER
#endif /* INCLUDE_USB_SPEAKER_INIT */
#ifdef INCLUDE_USB_PEGASUS_END_INIT
    #define INCLUDE_USB_PEGASUS_END
#endif /* INCLUDE_USB_PEGASUS_END_INIT */
#ifdef INCLUDE_USB_PRINTER_INIT
    #define INCLUDE_USB_PRINTER
#endif /* INCLUDE_USB_PRINTER_INIT */
#ifdef INCLUDE_USB_KEYBOARD_INIT
    #define INCLUDE_USB_KEYBOARD
#endif /* INCLUDE_USB_KEYBOARD_INIT */
#ifdef INCLUDE_USB_MOUSE_INIT
    #define INCLUDE_USB_MOUSE
#endif /* INCLUDE_USB_MOUSE_INIT */
#ifdef INCLUDE_USB_MS_BULKONLY_INIT
    #define INCLUDE_USB_MS_BULKONLY
#endif /* INCLUDE_USB_MS_BULKONLY_INIT */
#ifdef INCLUDE_USB_MS_CBI_INIT
    #define INCLUDE_USB_MS_CBI
#endif /* INCLUDE_USB_MS_CBI_INIT */
#ifdef INCLUDE_USB_GEN2_KEYBOARD_INIT
    #define INCLUDE_USB_GEN2_KEYBOARD
#endif /* INCLUDE_USB_GEN2_KEYBOARD_INIT */
#ifdef INCLUDE_USB_GEN2_MOUSE_INIT
    #define INCLUDE_USB_GEN2_MOUSE
#endif /* INCLUDE_USB_GEN2_MOUSE_INIT */
#ifdef INCLUDE_USB_GEN2_PRINTER_INIT
    #define INCLUDE_USB_GEN2_PRINTER
#endif /* INCLUDE_USB_GEN2_PRINTER_INIT */
#ifdef INCLUDE_USB_GEN2_STORAGE_INIT
    #define INCLUDE_USB_GEN2_STORAGE
#endif /* INCLUDE_USB_GEN2_STORAGE_INIT */
#ifdef INCLUDE_USB_GEN2_END_INIT
    #include <usrUsb2EndInit.c>
#endif /* INCLUDE_USB_GEN2_END_INIT */
#ifdef INCLUDE_USB_GEN2_SERIAL_INIT
    #include <usrUsb2SerialInit.c>
#endif /* INCLUDE_USB_GEN2_SERIAL_INIT */
#ifdef INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT
    #include <usrUsb2SerialPcConInit.c>
#endif /* INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT */
#ifdef INCLUDE_MS_EMULATOR_INIT
    #define INCLUDE_USB_TARG
    #include <usrUsbTargMsInit.c>
#endif /* INCLUDE_MS_EMULATOR_INIT */
#ifdef INCLUDE_KBD_EMULATOR_INIT
    #define INCLUDE_USB_TARG
    #include <usrUsbTargKbdInit.c>
#endif /* INCLUDE_KBD_EMULATOR_INIT */
#ifdef INCLUDE_PRN_EMULATOR_INIT
    #define INCLUDE_USB_TARG
    #include <usrUsbTargPrnInit.c>
#endif /* INCLUDE_PRN_EMULATOR_INIT */
#ifdef INCLUDE_SER_EMULATOR_INIT
    #define INCLUDE_USB_TARG
    #include <usrUsbTargSerInit.c>
#endif /* INCLUDE_SER_EMULATOR_INIT */

/* Non-INIT Components */

#ifdef INCLUDE_USB_AUDIO_DEMO
    #define INCLUDE_USB_SPEAKER
    #include <usrUsbAudioDemo.c>
#endif /* INCLUDE_USB_AUDIO_DEMO */
#ifdef INCLUDE_USB_SPEAKER
    #include <usrUsbSpkrInit.c>
#endif /* INCLUDE_USB_SPEAKER */
#ifdef INCLUDE_USB_MOUSE
    #include <usrUsbMseInit.c>
#endif /* INCLUDE_USB_MOUSE */
#ifdef INCLUDE_USB_KEYBOARD
    #include <usrUsbKbdInit.c>
#endif /* INCLUDE_USB_KEYBOARD */
#ifdef INCLUDE_USB_PRINTER
    #include <usrUsbPrnInit.c>
#endif /* INCLUDE_USB_PRINTER */
#ifdef INCLUDE_USB_MS_BULKONLY
    #include <usrUsbBulkDevInit.c>
#endif /* INCLUDE_USB_MS_BULKONLY */
#ifdef INCLUDE_USB_MS_CBI
    #include <usrUsbCbiUfiDevInit.c>
#endif /* INCLUDE_USB_MS_CBI */
#ifdef INCLUDE_USB_PEGASUS_END
    #include <usrUsbPegasusEndInit.c>
#endif  /* INCLUDE_USB_PEGASUS_END */
#ifdef INCLUDE_USB_GEN2_MOUSE
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2MseInit.c>
#endif /* INCLUDE_USB_GEN2_MOUSE */
#ifdef INCLUDE_USB_GEN2_KEYBOARD
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2KbdInit.c>
#endif /* INCLUDE_USB_GEN2_KEYBOARD */
#ifdef INCLUDE_USB_GEN2_PRINTER
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2PrnInit.c>
#endif /* INCLUDE_USB_GEN2_PRINTER */
#ifdef INCLUDE_USB_GEN2_STORAGE
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2MscInit.c>
#endif /* INCLUDE_USB_GEN2_STORAGE */
#ifdef INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2HelperInit.c>
#endif /* INCLUDE_USB_GEN2_HELPER */

#ifdef INCLUDE_USB_TARG
    #include <usrUsbTargPciInit.c>
#endif /* INCLUDE_USB_TARG */

#ifdef INCLUDE_USB_TGT
#include <usrUsbTgtInit.c>
#endif

#ifdef INCLUDE_USB_OTG
#include <usrUsbOtgInit.c>
#endif


/* defines */

#define USB2_WV_LOGSIZE          80       /* Wind View Event Log Message Size */

#ifndef USB_HUB_POLLING_INTERVAL
#define USB_HUB_POLLING_INTERVAL 2        /* Default polling interval */
#endif

#ifndef USB_MEM_PART_SIZE
#define USB_MEM_PART_SIZE        0x200000 /* Default Memory Partition Size */
#endif

#ifndef USB_CHECK_RESUBMIT_URB_FLAG
#define USB_CHECK_RESUBMIT_URB_FLAG  FALSE
#endif

#ifndef USB_WAIT_TIME_OUT_VALUE
#define USB_WAIT_TIME_OUT_VALUE  (5000)   /* Default wait time out value */
#endif

#define USB_WAIT_TIME_OUT_MIN_VALUE (5000)

/* globals */

#if defined(INCLUDE_BOOT_APP)

#ifdef INCLUDE_UHCI_INIT
extern int usbUhcdDisableHC(int startType);
#endif

#ifdef INCLUDE_OHCI_INIT
extern int usbOhciDisable(int startType);
#endif

#ifdef INCLUDE_EHCI_INIT
extern int usbEhcdDisableHC(int startType);
#endif

void bootUsbSupportInit(void);

void usbCtlrQuiescent (int startType);

#endif /* defined(INCLUDE_BOOT_APP) */

#ifdef INCLUDE_EHCI
extern VOID usbEhcdInstantiate();
#endif

#ifdef INCLUDE_OHCI
extern VOID usbOhciInstantiate();
#endif

#ifdef INCLUDE_UHCI
extern VOID usbUhcdInstantiate();
#endif

#ifdef INCLUDE_USB_MHDRC_HCD
extern VOID usbVxbMhdrcHcdInstantiate();
#endif

#ifdef INCLUDE_SYNOPSYSHCI
extern VOID usbSynopsysHcdInstantiate();
#endif

#ifdef INCLUDE_EHCI_INIT
extern VOID vxbUsbEhciRegister();
extern STATUS usbEhcdInit();
extern STATUS usbEhcdExit();
#endif

#ifdef INCLUDE_OHCI_INIT
extern VOID vxbUsbOhciRegister();
extern STATUS usbOhcdInit();
extern STATUS usbOhcdExit();
#endif

#ifdef INCLUDE_UHCI_INIT
extern VOID vxbUsbUhciRegister();
extern STATUS usbUhcdInit();
extern STATUS usbUhcdExit();
#endif

#if defined(INCLUDE_USB_MHDRC_HCD_INIT) || defined(INCLUDE_USB_MHDRC_OCD_HCD_INIT)
extern VOID vxbUsbMhdrcHcdRegister();
extern STATUS usbMhdrcHcdInit();
extern STATUS usbMhdrcHcdExit();
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
extern VOID vxbUsbSynopsysHciRegister();
extern STATUS usbSynopsysHcdInit();
extern STATUS usbSynopsysHcdExit();
#endif

#ifdef INCLUDE_USB_XHCI_HCD_INIT
extern VOID vxbUsbXhciRegister();
extern STATUS usbXhcdInit();
extern STATUS usbXhcdExit();
#endif

extern STATUS usbMsgLibInit (int maxCallback, int maxPendingMsg);

/* locals */

LOCAL BOOL usbdInitByKernel = FALSE;

#ifdef INCLUDE_WINDVIEW
    LOCAL char usb2EvLog[USB2_WV_LOGSIZE];/* buffer for event logging */
#endif

LOCAL UINT32 usrUsbHubPollingInterval = USB_HUB_POLLING_INTERVAL;
LOCAL BOOL   usrUsbReSubmitUrbCheckFlag = USB_CHECK_RESUBMIT_URB_FLAG;
LOCAL int    usrUsbWaitTimeOutValue = USB_WAIT_TIME_OUT_VALUE;

UINT32     usb2WvFilter = 0;                  /* user controllable filter */

/* USB OTG ERF events */

usbOtgEventRaisePrototype usbOtgEventRaiseFunc = NULL;
usbOtgEventDataGetPrototype usbOtgEventDataGetFunc = NULL;
usbOtgEventDataPutPrototype usbOtgEventDataPutFunc = NULL;

/*****************************************************************************
*
* usbdTranslationInit - initialize the USBD 1.1 Translation Layer
*
* This function initializes the USBD 1.1 backwards compatibility layer that
* allows class drivers written with for the USB1.1 Host Stack to operate with
* the USB2.0 Host Stack
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/


STATUS usbdTranslationInit( void )
    {

    UINT16 verStatus;
    UINT16 usbdVersion;
    char   usbdMfg [USBD_NAME_LEN+1];


    if (usbdInitByKernel)
        if (usbdInitialize() != OK)
            {
            printf(" Failed to initialize USBD \n");
            return ERROR;
            }

    if ((verStatus = usbdVersionGet (&usbdVersion, usbdMfg)) != OK)
        {
        printf ("usbdVersionGet() failed..returned %d\n", verStatus);
        return ERROR;
        }

#ifdef DEBUG_ANNOUNCE_USB
    printf("USBD  %s %i Initialized\n", usbdMfg, usbdVersion);
#endif
    return OK;
    }


/*****************************************************************************
*
* usrUsbHubInit - initialize the USB Hub Driver
*
* This function initializes the USB2.0 Hub Driver.
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/

STATUS usrUsbHubInit(void)
    {
    if (usbdInitByKernel)
        {
        if (0 == usbHubInit())
            {
#ifdef DEBUG_ANNOUNCE_USB
            printf("USB Hub Driver Initialized \n");
#endif
            return OK;
            }
        else
            {
            printf("USB Hub Driver Failed to Initialize\n");
            return ERROR;
            }
        }
    return ERROR;
    }

/*****************************************************************************
*
* usbHcdInit - initialize USB host controller
*
* This routine initializes USB host controller
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbHcdInit(void)
    {
#ifdef INCLUDE_EHCI_INIT
    usbEhcdInit();
#endif

#ifdef INCLUDE_OHCI_INIT
    usbOhcdInit();
#endif

#ifdef INCLUDE_UHCI_INIT
    usbUhcdInit();
#endif

#if defined(INCLUDE_USB_MHDRC_HCD_INIT) || defined(INCLUDE_USB_MHDRC_OCD_HCD_INIT)
    usbMhdrcHcdInit();
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
    usbSynopsysHcdInit();
#endif

#ifdef INCLUDE_USB_XHCI_HCD_INIT
    usbXhcdInit();
#endif
    }

/*****************************************************************************
*
* usbInit - initialize the USB2 stack
*
* This function initializes the USB2 Stack
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/

STATUS usbInit (void)
    {

    if (!usbdInitByKernel)
        {
        /* Add usb boot hook */

#if defined(INCLUDE_BOOT_APP)
        bootUsbSupportInit();
#endif

        /* Set the default memory partition size */

        ossPartSizeSet(USB_MEM_PART_SIZE);

        /* Initialize message library */

        usbMsgLibInit(0, SEM_RW_MAX_CONCURRENT_READERS);

        /* Initialize the USB2 Host Stack */

        if (usbdInit() == OK)
            {
            usbdInitByKernel = TRUE;
#ifdef DEBUG_ANNOUNCE_USB
            printf("USB2 Host Stack Initialized\n");
#endif

            if ((usrUsbHubInit() != OK) ||
                (usbdTranslationInit() != OK))
                {
                return ERROR;
                }
            }

        usbHcdInit();

#ifdef INCLUDE_USB_TGT
        usrUsbTgtInit();
#endif /* INCLUDE_USB_TGT */
        }
    else
        {
        printf("USB2 Stack Already Initialized\n");
        }

    return OK;
    }

/*****************************************************************************
*
* usbHcdExit - un - initialize USB host controller
*
* This function un - initializes USB host controller
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: N/A
*/

STATUS usbHcdExit(void)
    {
#ifdef INCLUDE_EHCI_INIT
    if (usbEhcdExit() != OK)
        {
        printf("Error exiting EHCI driver\n");
        return ERROR;
        }
#endif
#ifdef INCLUDE_OHCI_INIT
    if (usbOhcdExit() != OK)
        {
        printf("Error exiting OHCI driver\n");
        return ERROR;
        }
#endif

#ifdef INCLUDE_UHCI_INIT
    if (usbUhcdExit() != OK)
        {
        printf("Error exiting UHCI driver\n");
        return ERROR;
        }
#endif

#if defined(INCLUDE_USB_MHDRC_HCD_INIT) || defined(INCLUDE_USB_MHDRC_OCD_HCD_INIT)
    if (usbMhdrcHcdExit() != OK)
        {
        printf("Error exiting MHCI driver\n");
        return ERROR;
        }
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
    if (usbSynopsysHcdExit() != OK)
        {
        printf("Error exiting SynopsysHCI driver\n");
        return ERROR;
        }
#endif

#ifdef INCLUDE_USB_XHCI_HCD_INIT
    if (usbXhcdExit() != OK)
        {
        printf("Error exiting xHCI HCD driver\n");
        return ERROR;
        }
#endif

    return OK;
    }

/*****************************************************************************
*
* usbExit - un - initialize the USB2 stack
*
* This function un - initializes the USB2 Stack
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/

STATUS usbExit (void)
    {

    if (usbdInitByKernel)
        {
#ifdef INCLUDE_USB_TGT
        usrUsbTgtUnInit();
#endif

        /* Exit HCD before exit hub and usbd */

        if (usbHcdExit() != OK)
            return ERROR;

        /* un - intialize the hub class driver */

        if (usbHubExit() != 0)
            {
            printf ("Error un - initializing the Hub Class\n");
            return ERROR;
            }
#ifdef DEBUG_ANNOUNCE_USB
        printf("Hub Class Driver successfully un-initialized\n");
#endif

        /* Un - initialize the translation unit */

        if (usbdShutdown() == ERROR)
            {
            printf ("Error un - initializing the Translational Unit\n");
            return ERROR;
            }

#ifdef DEBUG_ANNOUNCE_USB
        printf("USBD successfully un-initialized\n");
#endif

        /* Set usbdInitByKernel to false */

        usbdInitByKernel = FALSE;
        }
    else
        {
        printf ("USB Stack is not initialized \n");
        return ERROR;
        }

#ifdef DEBUG_ANNOUNCE_USB
    printf("Stack successfully un-initialized\n");
#endif

    return OK;
    }

/*****************************************************************************
*
* usbHcdInstantiate - instantiate the USB Host Controller Driver
*
* This routine instantiates the USB Host Controller Driver and allows
* the controller driver to be included with the vxWorks image and
* not be registered with vxBus. Devices will remain orphan devices
* until the Host Controller Init routine is called
*
* The routine call Host Controller's instantiate routine, itself does nothing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbHcdInstantiate (void)
    {
#ifdef INCLUDE_EHCI
    usbEhcdInstantiate();
#endif

#ifdef INCLUDE_OHCI
    usbOhciInstantiate();
#endif

#ifdef INCLUDE_UHCI
    usbUhcdInstantiate();
#endif

#ifdef INCLUDE_USB_MHDRC_HCD
    usbVxbMhdrcHcdInstantiate();
#endif

#ifdef INCLUDE_SYNOPSYSHCI
    usbSynopsysHcdInstantiate();
#endif

    return;
    }

/*******************************************************************************
*
* vxbUsbHciRegister - register host controller driver with vxBus
*
* This routine registers the host controller driver with vxBus. The registration is
* done for both PCI and Local bus type by calling the routine vxbDevRegister().
*
* Once the driver is registered, this routine also registers the
* Host Controller Root hub as bus-controller type with vxBus
*
* This routine call Host Controller's register routine, itself does nothing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/


void vxbUsbHciRegister (void)
    {
#ifdef INCLUDE_EHCI_INIT
    vxbUsbEhciRegister();
#endif

#ifdef INCLUDE_OHCI_INIT
    vxbUsbOhciRegister();
#endif

#ifdef INCLUDE_UHCI_INIT
    vxbUsbUhciRegister();
#endif

#if defined(INCLUDE_USB_MHDRC_HCD_INIT) || defined(INCLUDE_USB_MHDRC_OCD_HCD_INIT)
    vxbUsbMhdrcHcdRegister();
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
    vxbUsbSynopsysHciRegister();
#endif

#ifdef INCLUDE_USB_XHCI_HCD_INIT
    vxbUsbXhciRegister();
#endif

    return;
    }

/*******************************************************************************
*
* usbHstClassInit - initialize USB host class driver
*
* This routine initializes USB host class driver. This routine call each class
* driver's initialization that included by configuration.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbHstClassInit (void)
    {
#ifdef INCLUDE_USB_GEN2_HELPER
    usrUsb2HelperInit();
#endif

#ifdef INCLUDE_USB_PEGASUS_END_INIT
    usrUsbPegasusEndInit();
#endif

#ifdef INCLUDE_USB_HEADSET_DEMO
    usbBrcmAudioStart();
#endif

#ifdef INCLUDE_USB_SPEAKER_INIT
    usrUsbSpkrInit();
#endif

#ifdef INCLUDE_USB_PRINTER_INIT
    usrUsbPrnInit();
#endif

#ifdef INCLUDE_USB_KEYBOARD_INIT
    usrUsbKbdInit();
#endif

#ifdef INCLUDE_USB_MOUSE_INIT
    usrUsbMseInit();
#endif

#ifdef INCLUDE_USB_AUDIO_DEMO
    usrUsbAudioDemo();
#endif

#ifdef INCLUDE_USB_GEN2_SERIAL_INIT
    usrUsb2SerialInit();
#endif

#ifdef INCLUDE_USB_GEN2_END_INIT
    usrUsb2EndInit();
#endif

#ifdef INCLUDE_USB_GEN2_KEYBOARD_INIT
    usrUsb2KbdInit();
#endif

#ifdef INCLUDE_USB_GEN2_MOUSE_INIT
    usrUsb2MseInit();
#endif

#ifdef INCLUDE_USB_GEN2_PRINTER_INIT
    usrUsb2PrnInit();
#endif
    }

/*****************************************************************************
*
* usb2WindViewLog - Log USB2 Host Stack WindView events
*
* The USB2 Host Stack is instrumented to provide additional WindView User
* log events for function entries/exits tracing within the Host Controllers.
* Logging is based whether or not usbHcdWvFilter has been initialized with
* the appropriate filter mask. If WindView is not enabled and the correct
* filter mask is set, then the WindView user event message will be logged
* to the standard output.
*
* \ss
*        USB_EHCD_WV_FILTER                0x00000001
*        USB_OHCD_WV_FILTER                0x00000002
*        USB_UHCD_WV_FILTER                0x00000004
*        USB_USBD_WV_FILTER                0x00000008
*        USB_HUB_WV_FILTER                0x0000000F
* \se
*
* RETURNS: N/A
*
* ERRNO: none
*/

void usb2WindViewLog
    (
    UINT32 evId,
    char *buffer,
    UINT32 mask
    )
    {

    if ((usb2WvFilter & mask) == mask)
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
        logMsg("USB2 wvEvent: %s\n",(long)buffer,0,0,0,0,0);
  #endif
#endif /* INCLUDE_WINDVIEW */
        }
    }


/*****************************************************************************
*
* usbHubPollingIntervalGet - Get usb hub polling interval
*
* This routine return usb hub polling interval. This value can be configured
* by user to modify the define of USB_HUB_POLLING_INTERVAL.
*
* RETURNS : Value of hub polling interval by milliseconds.
*
* ERRNO: none
*/

UINT32 usbHubPollingIntervalGet(void)
    {
    return usrUsbHubPollingInterval;
    }


/*****************************************************************************
*
* usbHubPollingIntervalSet - Set usb hub polling interval
*
* This routine is used to set usb hub polling interval. User can also modify
* USB_HUB_POLLING_INTERVAL to change this value.
*
* RETURNS : N/A
*
* ERRNO: none
*/

void usbHubPollingIntervalSet(UINT32 interval)
    {
    usrUsbHubPollingInterval = interval;
    return;
    }

/*****************************************************************************
*
* usrUsbResubmitUrbCheckEnabled - get the resubmit urb check flag
*
* This routine is used to get the resubmit urb check flag.
*
* RETURNS : Value of resubmit urb check flag.
*
* ERRNO: none
*/

BOOL usrUsbResubmitUrbCheckEnabled
    (
    void
    )
    {
    return usrUsbReSubmitUrbCheckFlag;
    }

/*****************************************************************************
*
* usrUsbWaitTimeOutValueGet - Get usb wait time out value
*
* This routine return usb wait time out value. This value can be configured
* by user to modify the define of USB_WAIT_TIME_OUT_VALUE.
*
* RETURNS : Value of usb wait time out by milliseconds.
*
* ERRNO: none
*/

int usrUsbWaitTimeOutValueGet(void)
    {
    /*
     * If user set this value smaller than "5000", still return "5000"
     * as this is the short time limit.
     */

    if (usrUsbWaitTimeOutValue == WAIT_FOREVER)
        return WAIT_FOREVER;
    if (usrUsbWaitTimeOutValue < USB_WAIT_TIME_OUT_MIN_VALUE)
        return OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(USB_WAIT_TIME_OUT_MIN_VALUE);
    else
        return OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(usrUsbWaitTimeOutValue);
    }

#if defined(INCLUDE_BOOT_APP)

/*******************************************************************************
*
* bootUsbSupportInit - initialize boot loader USB support
*
* This routine initializes the boot loader application USB support.
* It adds a routine to the boot shell to reset all the usb host controllers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void bootUsbSupportInit(void)
    {
    /* Setup the function pointer to stop all USB controllers */

    bootUsbStopAllFunc = usbCtlrQuiescent;

    return;
    }


/*******************************************************************************
*
* usbCtlrQuiescent - reset and put all USB controllers in quiescent mode
*
* This routine will reset and put all USB controllers in quiescent mode.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usbCtlrQuiescent(int startType)
    {
#ifdef INCLUDE_UHCI_INIT
    usbUhcdDisableHC(startType);
#endif

#ifdef INCLUDE_OHCI_INIT
    usbOhciDisable(startType);
#endif

#ifdef INCLUDE_EHCI_INIT
    usbEhcdDisableHC(startType);
#endif

   /*
    * If other USB contorllers (either host or device controllers)
    * are to be included in bootrom, then apropriate code can be
    * added here to reset and put these controllers into known state.
    */

    return;
    }

#endif /* defined(INCLUDE_BOOT_APP) */

#endif /* __INCusrUsbInitc */
