/* usrUsbTool.c - USB Exerciser. */

/*
 * Copyright 1999-2005, 2007-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
03f,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
03e,22feb11,w_x  Correct MHDRC HCD support for usbTool
03d,13jan11,w_x  Initial USB OTG support changes (WIND00211541)
03c,07jan11,ghs  Clean up compile warnings (WIND00247082)
03b,14dec10,ghs  Change the usage of the reboot hook APIs to specific 
                 rebootHookAdd/rebootHookDelete (WIND00240804)
03a,16sep10,m_y  Update the prototype for MHCI/SYNOPSYSHCI init/exit 
                 routine (WIND00232860)
02z,08sep10,ghs  Add MHCI/SYNOPSYSHCI support
02y,01sep10,j_x  Remove EHCD hook add function in cmdAttach (WIND00229326)
22x,09feb10,m_y  modify detach messages for synopsys HCD
22w,07dec09,m_y  Add support for synopsys HCD (WIND00123426)
02x,23aug10,m_y  Update prototype for EHCI/OHCI/UHCI init routine
                 (WIND00229662)
02w,23jun10,ghs  Fix display issue in LP64 mode(WIND00219849)
02v,13jan10,ghs  vxWorks 6.9 LP64 adapting
02u,22jun09,w_x  Update prototype for usbUhcdInit()/usbEhcdInit() (WIND00170966)
02t,28jul08,j_x  audioThread routine changed for headset
                 application (WIND00129121)
02s,10oct07,p_g  Exception on print cancellation commant (stop) fixed
02r,29aug07,ami  register with vxBus (CQ: WIND00102576 Defect)
02q,01jun07,jrp  vxBus Update
02p,22sep05,ami  Prototype for prnTxCallback() routine changed
02o,22apr05,pdg  Support for non-pci USB
02n,18apr05,ami  warning messages removed.
02m,02apr05,ami  Exception on doing a kbdPoll before kbdInit fixed
02l,09feb05,mta  SPR 106166: Speaker Disconnect Issue
02k,02feb05,ami  Exception of attaching host controller before initializing
                 the USB stack handled
02j,16oct04,hch  remove usbHcdXhciLib.h
02i,15oct04,hch  Remove referencing to drv/usb/usbUhci.h and drv/usb/usbOhci.h
02h,15oct04,ami  Apigen Changes
02g,20sep04,ami  NET2280 tested for High Speed
02f,20sep04,ami  Fix Compiler Changes. File Copied from Intergration Branch
02e,12aug04,hch  Fixed compiler warning
02d,23jul04,???  Change in Description
02c,21jul04,hch  merged from development view
                 corrected the file versions for modification history
02b,20jul04,???  Removal of usrUsbTargPciInit.c
02a,16jul04,???  Organised host and peripheral include files and fixed
                 warnings
01z,09jul04,pdg  Macro usage corrected
                 Added ctrl-z at the end of sending
                 characters from a to z in kbdReport
01y,30jun04,???  Bug Fix for uhci
01x,04jun04,hch  Fixed compiler warning
01w,18may04,mat  Removed the control-z boot report being sent
01v,11may04,hch  merge after D12 driver testing
01u,05may04,???  Adding the warm reboot fix for EHCI
01t,05may04,???  Fix for Divide by zero error
01s,04may04,???  Additional printer test cases
01r,30apr04,???  Automation of kbdReport command
01q,22apr04,???  Support for USB 2.0 peripheral stack
01p,15dec03,dat  Copyright 2004
01o,16mar02,wef  fixed a bug w/ speaker operation
01n,13dec01,wef  merge from veloce view
01m,16sep00,wef  changed all device component names to have _USB_ in them
01l,12apr00,wef  reverted to older version of usrUsbTool.c
01k,07mar00,rcb  Add casts to convert GENERIC_HANDLEs to UINT32 where necesary
01j,29jan00,wef  made usbTool in to a component configlette
01i,17jan00,rcb  Add functions to test usbSpeakerLib
01h,29nov99,rcb  Increase frame number fields to 32-bits in
                 cmdGetCurrentFrame()
01g,23nov99,rcb  Added "Attach" and "Detach" commands to control attach/detach
                 of UHCI or OHCI HCD modules.  Removed automatic attachment
                 from "UsbInit" command
01f,23nov99,rcb  Change #include ../xxx references to lower case
01e,12nov99,rcb  Shorted path names...affects "#include" directives
01d,07oct99,rcb  Add code to test usbMouseLib.c
01c,04oct99,rcb  Add "dump" command
01b,24sep99,rcb  Add "print" function to dump files to printer
01a,01jun99,rcb  First
*/

/*
DESCRIPTION

usbTool is a command line-driven program which allows the user to exercise
USB capabilities for the USB Host and the Peripheral Stack.

In the vxWorks environment, the user starts usbTool by invoking the "usbTool"
entry point from the vxWorks shell. usbTool then displays a prompt:

    USB>

The user may now enter commands which will be parsed and executed by usbTool.
Each command follows the format:

    USB>command [optional parameter(s)]

Commands are executed after the user presses [enter].  The user may enter
the "help" or "?" commands to see a list of currently supported commands.

Multiple commands may be entered on the same command line separated by
semicolons (';').  usbTool which execute each command as if it had been
entered on a separate line (unless a command terminates with an error, in
which case all remaining commands entered on the same line will be ignored).

The "quit"/"exit"/"bye" command terminates usbTool.

INCLUDE FILES: stdio.h, stdLib.h, string.h. ioLib.h, ctype.h, usb/usbPlatform.h,
               usb/tools/cmdParser.h, usb/usbPciLib.h, usb/usbdLib.h,
               usb/usbHst.h, usb2/usbHubInitialization.h,
               usb/usbEhcdInitExit.h, usb/usbHid.h,
               drv/usb/usbKeyboardLib.h, drv/usb/usbMouseLib.h, usb/usbPrinter.h,
               drv/usb/usbPrinterLib.h, usb/tools/wavFormat.h, usb/usbAudio.h,
               drv/usb/usbSpeakerLib.h, usb/target/usbHalCommon.h,
               usb/target/usbTargLib.h, drv/usb/target/usbTargKbdLib.h,
               drv/usb/target/usbTargPrnLib.h, drv/usb/target/usbPdiusbd12Eval.h,
               drv/usb/target/usbTcdPdiusbd12EvalLib.h,
               drv/usb/target/usbTargPhilipsD12EvalLib.h,
               drv/usb/target/usbTcdIsp1582EvalLib.h,
               drv/usb/target/usbTcdNET2280Lib.h

*/

#ifndef __INCusrUsbToolc
#define __INCusrUsbToolc


/* Include files */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ioLib.h"
#include "ctype.h"

#include "usb/usbPlatform.h"            /* Basic definitions */
#include "usb/tools/cmdParser.h"        /* Command parser util funcs */

#ifdef INCLUDE_VXBUS
#include "usb/usbPciLib.h"              /* PCI interface */
#endif

#ifdef INCLUDE_USB
#include "usb/usbdLib.h"                /* USBD interface */
#include "usb/usbHst.h"
#include "usb/usbHubInitialization.h"
#endif

#ifdef INCLUDE_UHCI
    extern STATUS usbUhcdInit(void);
    extern STATUS usbUhcdExit(void);
    extern VOID vxbUsbUhciRegister (void);
    LOCAL GENERIC_HANDLE * uhciAttachToken = NULL;
#endif

#ifdef INCLUDE_OHCI
    extern STATUS usbOhcdInit (void);
    extern STATUS usbOhcdExit (void);
    extern VOID vxbUsbOhciRegister (void);
    LOCAL GENERIC_HANDLE * ohciAttachToken = NULL;
#endif

#ifdef INCLUDE_EHCI
    extern STATUS usbEhcdInit(void);
    extern STATUS usbEhcdExit(void);
    extern VOID vxbUsbEhciRegister (void);
    LOCAL GENERIC_HANDLE * ehciAttachToken = NULL;
#endif

#ifdef INCLUDE_SYNOPSYSHCI
    extern STATUS usbSynopsysHcdInit(void);
    extern STATUS usbSynopsysHcdExit(void);
    extern VOID vxbUsbSynopsysHciRegister (void);
    LOCAL GENERIC_HANDLE synopsysHciAttachToken = NULL;
#endif

#ifdef INCLUDE_USB_MHDRC_HCD
    extern STATUS usbMhdrcHcdInit(void);
    extern STATUS usbMhdrcHcdExit(void);
    extern STATUS vxbUsbMhdrcHcdRegister (void);
    LOCAL GENERIC_HANDLE mhciAttachToken = NULL;
#endif

#include "usb/usbHid.h"                         /* USB HID definitions */

#ifdef INCLUDE_USB_KEYBOARD
    #include "drv/usb/usbKeyboardLib.h"         /* USB keyboard SIO driver */
#endif

#ifdef INCLUDE_USB_MOUSE
    #include "drv/usb/usbMouseLib.h"            /* USB mouse SIO driver */
#endif

#ifdef INCLUDE_USB_PRINTER
    #include "usb/usbPrinter.h"
    #include "drv/usb/usbPrinterLib.h"          /* USB printer SIO driver */
#endif

#ifdef INCLUDE_USB_SPEAKER
    #include "usb/tools/wavFormat.h"            /* Microsoft .wav file format */
    #include "usb/usbAudio.h"                   /* USB audio definitions */
    #include "drv/usb/usbSpeakerLib.h"          /* USB speaker SEQ_DEV driver */
#endif

#ifdef INCLUDE_USB_TARG
    #include "usb/target/usbHalCommon.h"        /* HAL Common Library */
    #include "usb/target/usbTargLib.h"          /* USB target library */
    #include "drv/usb/target/usbTargKbdLib.h"   /* USB keyboard emulator */
    #include "drv/usb/target/usbTargPrnLib.h"   /* USB printer emulator */
#endif

#ifdef INCLUDE_PDIUSBD12
    #include "drv/usb/target/usbPdiusbd12Eval.h"    /* Philips target eval */
    #include "drv/usb/target/usbTcdPdiusbd12EvalLib.h"  /* Philips TCD */
    #include "drv/usb/target/usbTargPhilipsD12EvalLib.h"/* Philips eval target */
#endif

#ifdef INCLUDE_PHILIPS1582
    #include "drv/usb/target/usbTcdIsp1582EvalLib.h"
    extern void  sysIsp1582PciInit(long * pBaseaddr, long * pIrq);
#endif

#ifdef INCLUDE_NET2280
    #include "drv/usb/target/usbTcdNET2280Lib.h"
    extern void sys2NET2280PciInit (void);
    extern UINT32 BADDR_NET2280 [NET2280_NO_OF_PCI_BADDR];
    extern UINT16 IRQ_NET2280;
#endif


/* defines */

#define PGM_NAME        "usbTool"
#define PGM_DESCR       "USB exerciser"
#define PGM_VERSION     "02A"
#define PGM_COPYRIGHT   "Copyright (c) 2007, Wind River Systems, Inc.\n"


/* Prompt definition */

#define PROMPT      "usb>"      /* Program prompt */

#define INDENT      2           /* indent used during bus enumerate */


/* misc defines */

#define INT_PIPE_SRVC_INTERVAL      20  /* milliseconds */

#define CTRL_Z                      26  /* ASCI code for ^Z */

#define MAX_MFG_STR_LEN         30
#define MAX_PROD_STR_LEN        30

#define TX_BFR_SIZE             0x1000
#define AUDIO_BFR_SIZE          0x8000


#define GENERIC_USB_BFR         256
#define TEST_PRINT_PERFORMANCE

#ifdef  TEST_PRINT_PERFORMANCE
ULONG tickStart = 0;
ULONG tickEnd = 0;
ULONG tickForOperation = 0;
ULONG uDataRate = 0;
int nClockRate = 0;
#endif
long blocks;

/* locals */

#ifdef  INCLUDE_USB

LOCAL BOOL initialized = FALSE;
LOCAL USBD_CLIENT_HANDLE usbdClientHandle = NULL;

BOOL enterPressed;
LOCAL BOOL irpCallbackInvoked;

#endif

#ifdef  INCLUDE_USB_PRINTER

LOCAL BOOL patternTest;

LOCAL UINT32 txCharCount;       /* count of chars to "print" */

LOCAL UINT16 nextCharVal;       /* next value for pattern test */

LOCAL FILE *txFile = NULL;      /* file being "printed" */
LOCAL char txBfr [TX_BFR_SIZE]; /* Working buffer */
LOCAL UINT16 txBfrCount;        /* count of chars in working bfr */
LOCAL UINT16 txBfrIndex;        /* ptr to next char in bfr */

#endif

#ifdef INCLUDE_USB_KEYBOARD

LOCAL BOOL  kbdInitialized = FALSE;

#endif

#ifdef  INCLUDE_USB_TARG

LOCAL BOOL targInit = FALSE;
LOCAL USB_TARG_CHANNEL targChannel = NULL;
LOCAL BOOL targEnable = FALSE;

#endif  /* #ifdef INCLUDE_USB_TARG */

#define UHCI_TOKEN 0x55484349 /* "UHCI" */
#define OHCI_TOKEN 0x4F484349 /* "OHCI" */
#define EHCI_TOKEN 0x45484349 /* "EHCI" */
#define SYNOPSYSHCI_TOKEN 0x53484349    /* SYNOPSYSHCI */
#define MHCI_TOKEN 0x4D484349 /* "MHCI" */

/* Mutex for the synchronising the audio operations */
SEM_ID audioMutex = NULL;

#ifdef INCLUDE_USB
/*************************************************************************
*
* cmdUsbInit - Initialize USBD
*
* Initializes the USBD Layer
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdUsbInit
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )
    {
    UINT16 usbdVersion;
    char usbdMfg [USBD_NAME_LEN+1];

    UINT16 s;

    /* if already initialized, just show a warning */

    if (initialized)
        {
        fprintf (fout, "Already initialized.\n");
        return RET_CONTINUE;
        }


    /* Initialize the USB2 Host Stack */
    if (usbInit() == OK)
        {
        fprintf(fout, "usbdInit() returned OK\n");
        }
    else
        return RET_CONTINUE;
    /* Initialize the USBD */

    s = usbdInitialize ();

    fprintf (fout, "usbdInitialize() returned %d\n", s);

    if (s == OK)
        {

        /* Register ourselves with the USBD */

        s = usbdClientRegister (PGM_NAME, &usbdClientHandle);

        fprintf (fout, "usbdClientRegister() returned %d\n", s);

        if (s == OK)
            {

            fprintf (fout, "usbdClientHandle = %p\n", usbdClientHandle);

            /* Display the USBD version */

            if ((s = usbdVersionGet (&usbdVersion, usbdMfg)) != OK)
                {
                fprintf (fout, "usbdVersionGet() returned %d\n", s);
                }
            else
                {
                fprintf (fout, "USBD version = 0x%4.4x\n", usbdVersion);
                fprintf (fout, "USBD mfg = '%s'\n", usbdMfg);
                }

            if (s == OK)
                initialized = TRUE;

            }
        }

    if (s != OK)
        {
        fprintf (fout, "initialization failed\n");
        }

    return RET_CONTINUE;

    }


/*************************************************************************
*
* cmdUsbDown - Shut down USBD
*
* Shutdowns the USBD layer
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdUsbDown
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    UINT16 s;


    initialized = FALSE;

    #ifdef INCLUDE_UHCI
     uhciAttachToken = NULL;
    #endif

    #ifdef INCLUDE_OHCI
     ohciAttachToken = NULL;
    #endif


    /* Unregister our client */

    if (usbdClientHandle == NULL)
        {
        fprintf (fout, "usbdClientHandle == NULL.  not registered\n");
        }
    else
        {
        s = usbdClientUnregister (usbdClientHandle);
        usbdClientHandle = NULL;

        fprintf (fout, "usbdClientUnregister() returned %d\n", s);
        }


    /* shut down USBD */

    s = usbdShutdown ();

    fprintf (fout, "usbdShutdown() returned %d\n", s);


    return RET_CONTINUE;
    }


/*************************************************************************
*
* getHcdType - Determines type of HCD specified by caller
*
* This function determines the type of HCD
*
* RETURNS: N/A
*
* ERRNO: none
*/

LOCAL VOID getHcdType
    (
    char **ppCmd,               /* Ptr to remainder of cmd line */
    FILE *fout,                 /* stream for output (if any) */
    HCD_EXEC_FUNC *execFunc,    /* pointer to HCD exec func */
    GENERIC_HANDLE **ppHandle,  /* pointer to generic handle pointer */
    pUINT8 pPciClass,           /* pointer to pci class var */
    pUINT8 pPciSubclass,        /* pointer to pci subclass var */
    pUINT8 pPciPgmIf,           /* pointer to pci pgmif var */
    char **ppHcdName            /* pointer to name string var */
    )

    {
    char hcdName [32];

    /* Get HCD type */

    *ppCmd = GetNextToken (*ppCmd, hcdName, sizeof (hcdName));


    if (KeywordMatch (hcdName, "uhci", 4) == 0)
        {
#ifdef INCLUDE_UHCI
        *ppHandle = (GENERIC_HANDLE *)UHCI_TOKEN;
#else
        fprintf (fout, "UHCI Component Not Included.\n");
        *ppHandle = NULL;
        *execFunc = NULL;
#endif
        return;
        }
    else if (KeywordMatch (hcdName, "ohci", 4) == 0)
        {
#ifdef INCLUDE_OHCI
        *ppHandle = (GENERIC_HANDLE *)OHCI_TOKEN;
#else
        fprintf (fout, "OHCI Component Not Included.\n");
        *ppHandle = NULL;
        *execFunc = NULL;
#endif
        return;
        }
    else if (KeywordMatch (hcdName, "ehci", 4) == 0)
        {
#ifdef INCLUDE_EHCI
        *ppHandle = (GENERIC_HANDLE *)EHCI_TOKEN;
#else
        fprintf (fout, "EHCI Component Not Included.\n");
        *ppHandle = NULL;
        *execFunc = NULL;
#endif
        return;
        }
    else if(KeywordMatch (hcdName, "synopsyshci", 11) == 0)
        {
#ifdef INCLUDE_SYNOPSYSHCI
        *ppHandle = (GENERIC_HANDLE *)SYNOPSYSHCI_TOKEN;
#else
        fprintf (fout, "SYNOPSYSHCI Component Not Included.\n");
        *ppHandle = NULL;
        *execFunc = NULL;
#endif
        return;
        }
    else if (KeywordMatch (hcdName, "mhci", 4) == 0)
        {
#ifdef INCLUDE_USB_MHDRC_HCD
        *ppHandle = (GENERIC_HANDLE *)MHCI_TOKEN;
#else
        fprintf (fout, "MHDRC HCD Component Not Included.\n");
        *ppHandle = NULL;
        *execFunc = NULL;
#endif
        return;
	}
    else
        {
        fprintf (fout, "Must specify HCD type as 'uhci', 'ohci', 'ehci', 'mhci' or 'synopsyshci'.\n");
        *execFunc = NULL;
        *ppHandle = NULL;
        }
    return;
    }


/*************************************************************************
*
* cmdAttach - Attaches HCD to USBD
*
* This function attaches the HCD to the USBD layer
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdAttach
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    HCD_EXEC_FUNC execFunc;
    GENERIC_HANDLE *pToken;
    UINT8 pciClass;
    UINT8 pciSubclass;
    UINT8 pciPgmIf;
    char *hcdName;

    /*
     * Check whether the USB Stack is initialized. Host Contollers cannot be
     * attached before initializing the USB Stack
     */

    if (initialized == FALSE)
        {
        printf ("USB Stack not initialized.\nInitialize the USB Stack before "
                 "attaching the host controller \n");
        return RET_CONTINUE;
        }

    getHcdType (ppCmd, fout, &execFunc, &pToken, &pciClass, &pciSubclass,
                &pciPgmIf, &hcdName);

    if (pToken == NULL)
        return RET_CONTINUE;

#ifdef INCLUDE_UHCI
    else if (pToken == (GENERIC_HANDLE *)UHCI_TOKEN)
        {
        if (ERROR == usbUhcdInit())
            {
            printf ("Error attaching UHCI host controller.\n");
            return RET_CONTINUE;
            }
        else
            {
            /* register the driver with vxBus */

            vxbUsbUhciRegister ();

            printf ("attaching UHCI host controller SUCCESSFUL\n");
            uhciAttachToken = pToken;
            return RET_CONTINUE;
            }
        }
#endif

#ifdef INCLUDE_OHCI
    else if (pToken == (GENERIC_HANDLE *)OHCI_TOKEN)
        {
        if (ERROR == usbOhcdInit())
            {
            printf ("Error attaching OHCI host controller.\n");
            return RET_CONTINUE;
            }
        else
            {
            /* register the driver with vxBus */

            vxbUsbOhciRegister ();

            printf ("attaching OHCI host controller SUCCESSFUL.\n");
            ohciAttachToken = pToken;
            return RET_CONTINUE;
            }
        }
#endif

#ifdef INCLUDE_EHCI
    else if (pToken == (GENERIC_HANDLE *)EHCI_TOKEN)
        {
        if (ERROR == usbEhcdInit())
            {
            printf ("Error attaching EHCI host controller.\n");
            return RET_CONTINUE;
            }
        else
            {

            /* register the driver with vxBus */

            vxbUsbEhciRegister ();

            printf ("attaching EHCI host controller SUCCESSFUL.\n");
            ehciAttachToken = pToken;
            return RET_CONTINUE;
            }
        }
#endif

#ifdef INCLUDE_SYNOPSYSHCI

    else if (pToken == (GENERIC_HANDLE *)SYNOPSYSHCI_TOKEN)
        {
        if (ERROR == usbSynopsysHcdInit())
            {
            printf ("Error attaching SYNOPSYSHCI host controller.\n");
            return RET_CONTINUE;
            }
        else
            {

            /* register the driver with vxBus */

            vxbUsbSynopsysHciRegister ();

            printf ("attaching SYNOPSYSHCI host controller SUCCESSFUL.\n");
            synopsysHciAttachToken = pToken;
            return RET_CONTINUE;
            }
        }
#endif

#ifdef INCLUDE_USB_MHDRC_HCD
    else if(pToken == (GENERIC_HANDLE *)MHCI_TOKEN)
        {
        if (ERROR == usbMhdrcHcdInit())
            {
            printf ("Error attaching MHDRC host controller.\n");
            return RET_CONTINUE;
            }
        else
            {

            /* register the driver with vxBus */

            vxbUsbMhdrcHcdRegister ();

            printf ("attaching MHDRC host controller SUCCESSFUL.\n");
            
            mhciAttachToken = pToken;
            
            return RET_CONTINUE;
            }
        }
#endif
   
    else
        return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdDetach - Detaches HCD from USBD
*
* This function detaches the specifed HCD from the USBD layer
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdDetach
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    HCD_EXEC_FUNC execFunc;
    GENERIC_HANDLE *pToken;
    UINT8 pciClass;
    UINT8 pciSubclass;
    UINT8 pciPgmIf;
    char *hcdName;

    getHcdType (ppCmd, fout, &execFunc, &pToken, &pciClass, &pciSubclass,
                &pciPgmIf, &hcdName);


    if (pToken == NULL)
        {
        return RET_CONTINUE;
        }

#ifdef INCLUDE_UHCI
    else if (pToken == (GENERIC_HANDLE *)UHCI_TOKEN)
        {
        if( uhciAttachToken == NULL)
            fprintf (fout, "UHCI HCD not attached.\n");
        else
            {
            fprintf (fout, "Detaching UHCI HCD.\n");
            if (usbUhcdExit() == ERROR)
               fprintf (fout, "Detaching UHCI HCD returned FAILURE.\n");
            else
               fprintf (fout, "Detaching UHCI HCD returned SUCCESS.\n");
            uhciAttachToken = NULL;
            }
        }
#endif

#ifdef INCLUDE_OHCI
    else if(pToken == (GENERIC_HANDLE *)OHCI_TOKEN)
        {
        if (ohciAttachToken == NULL)
            fprintf (fout, "OHCI HCD not attached.\n");
        else
            {
            fprintf (fout, "Detaching OHCI HCD.\n");
            if (usbOhcdExit() == ERROR)
                fprintf (fout, "Detaching OHCI HCD returned FAILURE.\n");
            else
                fprintf (fout, "Detaching OHCI HCD returned SUCCESS.\n");
            ohciAttachToken = NULL;
            }
        }
#endif

#ifdef INCLUDE_EHCI
    else if (pToken == (GENERIC_HANDLE *)EHCI_TOKEN)
        {
        if (ehciAttachToken == NULL)
            fprintf (fout, "EHCI HCD not attached.\n");
        else
            {
            fprintf (fout, "Detaching EHCI HCD.\n");
            if (usbEhcdExit() == ERROR)
                fprintf (fout, "Detaching EHCI HCD returned FAILURE.\n");
            else
                fprintf (fout, "Detaching EHCI HCD returned SUCCESS.\n");
            ehciAttachToken = NULL;
            }
        }
#endif

#ifdef INCLUDE_SYNOPSYSHCI
    else if (pToken == (GENERIC_HANDLE *)SYNOPSYSHCI_TOKEN)
        {
        if (synopsysHciAttachToken == NULL)
            fprintf (fout, "SYNOPSYSHCI HCD not attached.\n");
        else
            {
            fprintf (fout, "Detaching SYNOPSYSHCI HCD.\n");
            if (usbSynopsysHcdExit() == ERROR)
                fprintf (fout, "Detaching SYNOPSYSHCI HCD returned FAILURE.\n");
            else
                fprintf (fout, "Detaching SYNOPSYSHCI HCD returned SUCCESS.\n");
            synopsysHciAttachToken = NULL;
            }
        }
#endif

#ifdef INCLUDE_USB_MHDRC_HCD
    else if(pToken == (GENERIC_HANDLE *)MHCI_TOKEN)
        {
        if( mhciAttachToken == NULL)
            fprintf (fout, "MHDRC HCD not attached.\n");
        else
            {
            fprintf (fout, "Detaching MHDRC HCD.\n");
            if (usbMhdrcHcdExit() == ERROR)
                fprintf (fout, "Detaching MHDRC HCD returned FAILURE.\n");
            else
                fprintf (fout, "Detaching MHDRC HCD returned SUCCESS.\n");
            mhciAttachToken = NULL;
            }
        }
#endif

    return RET_CONTINUE;
    }


/***************************************************************************
*
* showUsbString - reads a string descriptor and displays it
*
* This function reads the string descriptor and displays it
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID showUsbString
    (
    USBD_CLIENT_HANDLE clientHandle,
    USBD_NODE_ID nodeId,
    FILE *fout,
    UINT8 stringId,
    UINT16 maxLen
    )

    {
    USB_STRING_DESCR * pString;
    UINT16 actLen;
    UINT16 i;
    char c;

    if ((pString = OSS_MALLOC (USB_MAX_DESCR_LEN)) == NULL)
        return;

    /* Read device descriptor to get string Ids for mfg/model */

    if (usbdDescriptorGet (clientHandle,
                           nodeId,
                           USB_RT_STANDARD | USB_RT_DEVICE,
                           USB_DESCR_STRING,
                           stringId,
                           0x0409,  /* unicode code for English */
                           USB_MAX_DESCR_LEN,
                           (UINT8 *) pString,
                           &actLen)
                        != OK)
        {
        fprintf (fout, "<<can't read string>>");
        OSS_FREE (pString);
        return;
        }

    for (i = 0; i + 2 < actLen && i < maxLen * 2; i += 2)
        {
        c = pString->string [i];
        fprintf (fout, "%c", (isprint ((int) c)) ? c : '_');
        }

    if (i + 2 < actLen)
        fprintf (fout, "...");

    OSS_FREE (pString);

    }


/***************************************************************************
*
* showMfgModel - Display mfg/model strings for a USB device
*
* This function displays the mfg/model string of USB Device
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID showMfgModel
    (
    USBD_CLIENT_HANDLE clientHandle,
    USBD_NODE_ID nodeId,
    FILE *fout
    )

    {
    USB_DEVICE_DESCR * pDevDescr;
    UINT16 actLen;


    if((pDevDescr = OSS_MALLOC (USB_DEVICE_DESCR_LEN)) == NULL)
        return;

    /* Read device descriptor to get string Ids for mfg/model */

    if (usbdDescriptorGet (clientHandle,
                           nodeId,
                           USB_RT_STANDARD | USB_RT_DEVICE,
                           USB_DESCR_DEVICE,
                           0,
                           0,
                           USB_DEVICE_DESCR_LEN,
                           (UINT8 *) pDevDescr,
                           &actLen)
                        != OK ||
          actLen < USB_DEVICE_DESCR_LEN)
        {
        OSS_FREE (pDevDescr);
        return;
        }

    if (pDevDescr->manufacturerIndex != 0 || pDevDescr->productIndex != 0)
        fprintf (fout, " = ");

    if (pDevDescr->manufacturerIndex != 0)
        {
        showUsbString (clientHandle, nodeId, fout, pDevDescr->manufacturerIndex,
                       MAX_MFG_STR_LEN);
        fprintf (fout, "/");
        }

    if (pDevDescr->productIndex != 0)
        {
        showUsbString (clientHandle, nodeId, fout, pDevDescr->productIndex,
            MAX_PROD_STR_LEN);
        }
    }


/***************************************************************************
*
* HubEnumerate - Enumerate all ports on the specified hub
*
* This routine enumerates all devices from the specified HubId down.
* <clientHandle> must be a valid USBD_CLIENT_HANDLE.  <hubId> specifies
* the Node Id of the first USB hub to be enumerated.
*
* RETURNS: OK, or RET_CONTINUE if USBD returns an error.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 HubEnumerate
    (
    USBD_CLIENT_HANDLE clientHandle,    /* Caller�s USBD client handle */
    USBD_NODE_ID hubId,                 /* Node Id for hub to enumerate */
    FILE *fout,
    UINT16 indent
    )

    {
    UINT16 portCount;                   /* Number of ports on this hub */
    UINT16 portIndex;                   /* current port index */
    UINT16 nodeType;                    /* type of node being enumerated */
    USBD_NODE_ID nodeId;                /* id of node being enumerated */
    UINT16 s;

    /* Retrieve the number of ports for this hub. */

    fprintf (fout, "%*shub 0x%x", indent, "", hubId);
    showMfgModel (clientHandle, hubId, fout);
    fprintf (fout, "\n");

    if ((s = usbdHubPortCountGet (clientHandle, hubId, &portCount))
        != OK)
        {
        fprintf (fout, "usbdHubPortCountGet() returned %d\n", s);
        return RET_CONTINUE;
        }

    fprintf (fout, "%*sport count = %d\n", indent+INDENT, "", portCount);

    /* See if a device is attached to each of the hub�s ports. */

    for (portIndex = 0; portIndex < portCount; portIndex++)
        {
        if ((s = usbdNodeIdGet (clientHandle, hubId, portIndex, &nodeType,
            &nodeId)) != OK)
            {
            fprintf (fout, "%*susbdNodeIdGet() returned %d\n", indent+INDENT, "",
                     s);
            return RET_CONTINUE;
            }

        switch (nodeType)
            {
            case USB_NODETYPE_NONE:     /* No device attached. */

                fprintf (fout, "%*sport %d not connected\n", indent+INDENT, "",
                         portIndex);
                break;

            case USB_NODETYPE_HUB:      /* Another hub found. */

                fprintf (fout, "%*sport %d is hub 0x%x\n", indent+INDENT, "",
                         portIndex, nodeId);

                if (HubEnumerate (clientHandle, nodeId, fout, indent+INDENT)
                    != OK)
                    return RET_CONTINUE;

                break;

            case USB_NODETYPE_DEVICE:   /* Device attached to port. */

                fprintf (fout, "%*sport %d is device 0x%x", indent+INDENT, "",
                    portIndex, nodeId);

                showMfgModel (clientHandle, nodeId, fout);
                fprintf (fout, "\n");

                break;

            default:                    /* Unknown node type code */

                fprintf (fout, "%*snode type not recognized for node 0x%x\n",
                         indent+INDENT, "", nodeId);
                break;
            }
        }

    return OK;
    }


/***************************************************************************
*
* USBEnumerate - Enumerate all USB host controllers in the system.
*
* This routine enumerates all USB host controllers, hubs, and devices
* currently connected to the system.  The caller must register with the
* USBD prior to calling this function and supply its USBD_CLIENT_HANDLE
* in <clientHandle>.
*
* RETURNS: OK, or RET_CONTINUE if USBD returns an error.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 USBEnumerate
    (
    USBD_CLIENT_HANDLE clientHandle,    /* Caller�s USBD client handle */
    FILE *fout
    )

    {
    UINT16 busCount;                    /* Number of USB host controllers */
    UINT16 busIndex;                    /* current bus index */
    USBD_NODE_ID rootId;                /* Root hub id for current bus */
    UINT16 s;


    /* Retrieve the number of USB host controllers in the system. */

    if ((s = usbdBusCountGet (clientHandle, &busCount)) != OK)
        {
        fprintf (fout, "usbdBusCountGet() returned %d\n", s);
        return RET_CONTINUE;
        }

    fprintf (fout, "bus count = %d\n", busCount);

    /* Retrieve the root hub id for each host controller and enumerate it. */

    for (busIndex = 0; busIndex < busCount; busIndex++)
        {
        if ((s = usbdRootNodeIdGet (clientHandle, busIndex, &rootId))
            != OK)
            {
            fprintf (fout, "usbdRootNodeIdGet() returned %d\n", s);
            return RET_CONTINUE;
            }

        fprintf (fout, "enumerating bus %d\n", busIndex);

        if (HubEnumerate (clientHandle, rootId, fout, INDENT) != OK)
            return RET_CONTINUE;
        }

    return OK;
    }


/*************************************************************************
*
* cmdUsbEnum - Enumerate USBs attached to system
*
* This function enumerates the USB devices attached to system
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdUsbEnum
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    USBEnumerate (usbdClientHandle, fout);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdUsbStats - Show bus statistics
*
* This function shows bus statistics
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdUsbStats
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    long nodeId;
    USBD_STATS stats;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);


    /* Validate parameters */

    if (nodeId == -1)
        {
        fprintf (fout, "Must specify a node id.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    if ((s = usbdStatisticsGet (usbdClientHandle, (USBD_NODE_ID) nodeId, &stats, sizeof (stats)))
            != OK)
        fprintf (fout, "usbdStatisticsGet() returned %d\n", s);
    else
        {
        fprintf (fout, "totalTransfersIn    = %ld\n", (long) stats.totalTransfersIn);
        fprintf (fout, "totalTransfersout   = %ld\n", (long) stats.totalTransfersOut);
        fprintf (fout, "totalReceiveErrors  = %ld\n", (long) stats.totalReceiveErrors);
        fprintf (fout, "totalTransmitErrors = %ld\n", (long) stats.totalTransmitErrors);
        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdGetConfig - get current configuration value for a device
*
* This function get the current configuration of the device
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdGetConfig
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    long nodeId;
    UINT16 * pConfiguration;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);


    /* Validate parameters */

    if (nodeId == -1)
        {
        fprintf (fout, "Must specify a node id.\n");
        return RET_CONTINUE;
        }

    if ((pConfiguration = OSS_MALLOC (sizeof (UINT16))) == NULL)
        return RET_CONTINUE;

    /* Execute function. */

    if ((s = usbdConfigurationGet (usbdClientHandle,
                                   (USBD_NODE_ID) nodeId,
                                   pConfiguration))
                                != OK)
        fprintf (fout, "usbdConfigurationGet() returned %d\n", s);
    else
        fprintf (fout, "current configuration = 0x%x\n", *pConfiguration);

    OSS_FREE (pConfiguration);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdSetConfig - set current configuration value for a device
*
* This function sets current configuration value for a device
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdSetConfig
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long configuration;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &configuration, -1);


    /* Validate parameters */

    if (nodeId == -1 || configuration == -1)
        {
        fprintf (fout, "Must specify a node id and configuration value.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    s = usbdConfigurationSet (usbdClientHandle, (USBD_NODE_ID) nodeId, (UINT16) configuration, 0);
    fprintf (fout, "usbdConfigurationSet() returned %d\n", s);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdGetInterface - get alt. setting for an interface
*
* This function get alt. setting for an interface.
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdGetInterface
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long index;
    UINT16 * pAltSetting;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &index, -1);


    /* Validate parameters */

    if (nodeId == -1 || index == -1)
        {
        fprintf (fout, "Must specify a node id and interface number.\n");
        return RET_CONTINUE;
        }

    if ((pAltSetting = OSS_MALLOC (sizeof (UINT16))) == NULL)
        return RET_CONTINUE;


    /* Execute function. */

    if ((s = usbdInterfaceGet (usbdClientHandle,
                               (USBD_NODE_ID) nodeId,
                               index,
                               pAltSetting))
                            != OK)
        fprintf (fout, "usbdInterfaceGet() returned %d\n", s);
    else
        fprintf (fout, "alternate setting = 0x%x\n", *pAltSetting);

    OSS_FREE (pAltSetting);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdSetInterface - set alt. setting for an interface
*
* This function set alt. setting for an interface
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdSetInterface
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long index;
    long altSetting;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &index, -1);
    *ppCmd = GetHexToken (*ppCmd, &altSetting, -1);


    /* Validate parameters */

    if (nodeId == -1 || index == -1 || altSetting == -1)
        {
        fprintf (fout, "Must specify a node id, interface number, and alt. setting.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    s = usbdInterfaceSet (usbdClientHandle, (USBD_NODE_ID) nodeId, index, altSetting);
    fprintf (fout, "usbdInterfaceSet() returned %d\n", s);

    return RET_CONTINUE;
    }
#endif  /* INCLUDE_USB from way above*/


/*************************************************************************
*
* displayMem - dumps memory to display
*
* This function dumps the memory contents to display
*
* RETURNS:  N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID displayMem
    (
    UINT32 bfrLen,
    pUINT8 pBfr,
    FILE *fout
    )

    {
    int i,j;

    for (i = 0; i < bfrLen; i += 16)
        {
        for (j = i; j < bfrLen && j < i+16; j++)
            {
            fprintf (fout, "%2.2x ", pBfr [j]);
            if (j % 16 == 7)
                fprintf (fout, " ");
            }
        fprintf (fout, " ");
        for (j = i; j < bfrLen && j < i+16; j++)
            {
            fprintf (fout, "%c", (pBfr [j] >= 32 && pBfr [j] <= 127) ?
                     pBfr [j] : '.');
            }
        fprintf (fout, "\n");
        }
    }

#ifdef INCLUDE_USB

/*************************************************************************
*
* cmdGetStatus - get status for a device/interface/endpoint
*
* This function get status for a device/interface/endpoint to execute the
* GET_STATUS request
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdGetStatus
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long statusType;
    long index;
    long length;
    UINT8 * pStatusBfr;
    UINT16 actLen;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &statusType, -1);
    *ppCmd = GetHexToken (*ppCmd, &index, -1);
    *ppCmd = GetHexToken (*ppCmd, &length, -1);


    /* Validate parameters */

    if (nodeId == -1 || statusType == -1 || index == -1 || length == -1)
        {
        fprintf (fout, "Must specify a node id, status type, and index.\n");
        return RET_CONTINUE;
        }

    if (length > GENERIC_USB_BFR)
        {
        fprintf (fout, "length must be 0x%x or less.\n",
                 (unsigned int) GENERIC_USB_BFR);
        return RET_CONTINUE;
        }

    if ((pStatusBfr = OSS_MALLOC (GENERIC_USB_BFR)) == NULL)
        return RET_CONTINUE;


    /* Execute function. */

    if ((s = usbdStatusGet (usbdClientHandle,
                            (USBD_NODE_ID) nodeId,
                            statusType,
                            index,
                            length,
                            pStatusBfr,
                        &actLen)) != OK)
        fprintf (fout, "usbdStatusGet() returned %d\n", s);
    else
        {
        fprintf (fout, "actLen = 0x%x (%d) bytes\n", actLen, actLen);
        displayMem (actLen, pStatusBfr, fout);
        }

    OSS_FREE (pStatusBfr);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdGetAddress - get USB address for a node
*
* This function get the address of for the node
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdGetAddress
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    UINT16 address;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);


    /* Validate parameters */

    if (nodeId == -1)
        {
        fprintf (fout, "Must specify a node id.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    if ((s = usbdAddressGet (usbdClientHandle, (USBD_NODE_ID) nodeId, &address))
            != OK)
        fprintf (fout, "usbdAddressGet() returned %d\n", s);
    else
        fprintf (fout, "current address = 0x%x\n", address);


    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdSetAddress - set USB address for a node
*
* This function set USB address for a node
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdSetAddress
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long address;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &address, -1);


    /* Validate parameters */

    if (nodeId == -1 || address == -1)
        {
        fprintf (fout, "Must specify a node id and address value.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    s = usbdAddressSet (usbdClientHandle, (USBD_NODE_ID) nodeId, (UINT16) address);
    fprintf (fout, "usbdAddressnSet() returned %d\n", s);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdSetFeature - set a USB feature
*
* Thie fucntion sets the feature of the USB devices
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdSetFeature
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long featureType;
    long feature;
    long index;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &featureType, -1);
    *ppCmd = GetHexToken (*ppCmd, &feature, -1);
    *ppCmd = GetHexToken (*ppCmd, &index, -1);


    /* Validate parameters */

    if (nodeId == -1 || featureType == -1 || feature == -1 || index == -1)
        {
        fprintf (fout, "Must specify a node id, feature type, selector, and index.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    s = usbdFeatureSet (usbdClientHandle, (USBD_NODE_ID) nodeId, featureType, feature, index);
    fprintf (fout, "usbdFeatureSet() returned %d\n", s);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdClrFeature - clear a USB feature
*
* This function clears the feature of the USB device
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdClrFeature
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long featureType;
    long feature;
    long index;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &featureType, -1);
    *ppCmd = GetHexToken (*ppCmd, &feature, -1);
    *ppCmd = GetHexToken (*ppCmd, &index, -1);


    /* Validate parameters */

    if (nodeId == -1 || featureType == -1 || feature == -1 || index == -1)
        {
        fprintf (fout, "Must specify a node id, feature type, selector, and index.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    s = usbdFeatureClear (usbdClientHandle, (USBD_NODE_ID) nodeId, featureType, feature, index);
    fprintf (fout, "usbdFeatureClear() returned %d\n", s);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdGetDescr - retrieve a USB descriptor
*
* This function retrieves a USB descriptor
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdGetDescr
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long requestType;
    long descriptorType;
    long index;
    long languageId;
    long length;
    UINT8 * pDescrBuf;
    UINT16 actLen;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &requestType, -1);
    *ppCmd = GetHexToken (*ppCmd, &descriptorType, -1);
    *ppCmd = GetHexToken (*ppCmd, &index, -1);
    *ppCmd = GetHexToken (*ppCmd, &languageId, -1);
    *ppCmd = GetHexToken (*ppCmd, &length, -1);


    /* Validate parameters */

    if (nodeId == -1 || requestType == -1 || descriptorType == -1 || index == -1
        || languageId == -1 || length == -1)
        {
        fprintf (fout, "Must specify a node id, descr. type, index, language, and length.\n");
        return RET_CONTINUE;
        }

    if (length > GENERIC_USB_BFR)
        {
        fprintf (fout, "length must be 0x%x or less.\n",
                 (unsigned int) GENERIC_USB_BFR);
        return RET_CONTINUE;
        }

    if ((pDescrBuf = OSS_MALLOC (GENERIC_USB_BFR)) == NULL)
        return RET_CONTINUE;

    /* Execute function. */

    s = usbdDescriptorGet (usbdClientHandle,
                           (USBD_NODE_ID) nodeId,
                           requestType,
                           descriptorType,
                           index,
                           languageId,
                           length,
                           pDescrBuf,
                           &actLen);

    if (s != OK)
        fprintf (fout, "usbdDescriptorGet() returned %d\n", s);
    else
        {
        fprintf (fout, "actLen = 0x%x (%d) bytes\n", actLen, actLen);
        displayMem (actLen, pDescrBuf, fout);
        }

    OSS_FREE (pDescrBuf);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdGetSynchFrame - retrieve current synch frame from a device endpoint
*
* This function retrieve current synch frame from a device endpoint
*
* RETURNS:  RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdGetSynchFrame
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    long endpoint;
    UINT16 * pSynchFrame;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &endpoint, -1);


    /* Validate parameters */

    if (nodeId == -1 || endpoint == -1)
        {
        fprintf (fout, "Must specify a node id and endpoint.\n");
        return RET_CONTINUE;
        }

    if ((pSynchFrame = OSS_MALLOC (sizeof (UINT16))) == NULL)
        return RET_CONTINUE;

    /* Execute function. */

    if ((s = usbdSynchFrameGet (usbdClientHandle,
                                (USBD_NODE_ID) nodeId,
                                endpoint,
                                pSynchFrame))
                            != OK)
        fprintf (fout, "usbdSynchFrameGet() returned %d\n", s);
    else
        fprintf (fout, "synch frame = 0x%4.4x\n", FROM_LITTLEW (*pSynchFrame));

    OSS_FREE (pSynchFrame);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdGetCurrentFrame - retrieve current frame for a USB
*
* This function retrieves current frame for a USB
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdGetCurrentFrame
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    long nodeId;
    UINT32 frameNo;
    UINT32 frameWindow;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);


    /* Validate parameters */

    if (nodeId == -1)
        {
        fprintf (fout, "Must specify a node id.\n");
        return RET_CONTINUE;
        }


    /* Execute function. */

    if ((s = usbdCurrentFrameGet (usbdClientHandle, (USBD_NODE_ID) nodeId,
            &frameNo, &frameWindow)) != OK)
        fprintf (fout, "usbdCurrentFrameGet() returned %d\n", s);
    else
        {
        fprintf (fout, "frame number = %ld\n", (long) frameNo);
        fprintf (fout, "frame window = %ld\n", (long) frameWindow);
        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* enterThread - waits for user to press [enter]
*
* This is a thread which waits for the use to press enter
*
* RETURNS:  N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID enterThread
    (
    pVOID param
    )

    {
    FILE *fout = (FILE *) param;
    char bfr [256];

    fprintf (fout, "Press [enter] to terminate polling.\n");
    gets (bfr);
    enterPressed = TRUE;
    }


/*************************************************************************
*
* irpCallback - called when IRP completes
*
* This is the callback function which is called when the IRP completes
*
* RETURNS:  N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID irpCallback
    (
    pVOID pIrp
    )

    {
    irpCallbackInvoked = TRUE;
    }


/*************************************************************************
*
* cmdIntPoll - polls interrupt endpoint for input
*
* Continuously reads IN packets from interrupt endpoint until key
* pressed.
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdIntPoll
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    THREAD_HANDLE thread;
    long nodeId;
    long endpoint;
    long length;
    USBD_PIPE_HANDLE handle;
    unsigned char bfr [256];
    USB_IRP irp;
    UINT16 s;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &nodeId, -1);
    *ppCmd = GetHexToken (*ppCmd, &endpoint, -1);
    *ppCmd = GetHexToken (*ppCmd, &length, USB_MIN_CTRL_PACKET_SIZE);


    /* Validate parameters */

    if (nodeId == -1 || endpoint == -1)
        {
        fprintf (fout, "Must specify a node id and endpoint.\n");
        return RET_CONTINUE;
        }

    if (length > sizeof (bfr))
        {
        fprintf (fout, "Length must be %d or less.\n", sizeof (bfr));
        return RET_CONTINUE;
        }


    /* Create a pipe to talk to this endpoint */

    if ((s = usbdPipeCreate (usbdClientHandle, (USBD_NODE_ID) nodeId, (UINT16) endpoint, 0, 0,
        USB_XFRTYPE_INTERRUPT, USB_DIR_IN, length, length, INT_PIPE_SRVC_INTERVAL,
        &handle)) != OK)
        {
        fprintf (fout, "usbdPipeCreate() returned %d\n", s);
        return RET_CONTINUE;
        }


    /* Create thread to watch for keypress */

    enterPressed = FALSE;

    if (OSS_THREAD_CREATE (enterThread, (pVOID) fout, OSS_PRIORITY_INHERIT, "tEnter",
        &thread) != OK)
        goto pipe_done;

    while (!enterPressed)
        {
        /* initialize IRP */

        memset (&irp, 0, sizeof (irp));

        irp.irpLen = sizeof (irp);
        irp.userCallback = irpCallback;
        irp.transferLen = length;
        irp.bfrCount = 1;
        irp.bfrList [0].pid = USB_PID_IN;
        irp.bfrList [0].pBfr = bfr;
        irp.bfrList [0].bfrLen = length;

        irpCallbackInvoked = FALSE;

        if ((s = usbdTransfer (usbdClientHandle, handle, &irp)) != OK)
            {
            fprintf (fout, "usbdTransfer() returned %d\n", s);
            break;
            }

        while (!enterPressed && !irpCallbackInvoked)
            OSS_THREAD_SLEEP (1);

        if (irpCallbackInvoked)
            {
            if (irp.result != OK)
                {
                fprintf (fout, "irp.result == %d\n", irp.result);
                break;
                }

            fprintf (fout, "actLen = %d: ", irp.bfrList [0].actLen);
            displayMem (irp.bfrList [0].actLen, bfr, fout);
            }
        else
            {
            if ((s = usbdTransferAbort (usbdClientHandle, handle, &irp))
                != OK)
                {
                fprintf (fout, "usbdTransferAbort() returned %d\n", s);
                break;
                }

            /* wait for the callback to be invoked */

            while (!irpCallbackInvoked)
                OSS_THREAD_SLEEP (1);
            }
        }

    OSS_THREAD_DESTROY (thread);

pipe_done:

    if ((s = usbdPipeDestroy (usbdClientHandle, handle)) != OK)
        {
        fprintf (fout, "usbdPipeDestroy() returned %d\n", s);
        return RET_CONTINUE;
        }

    return RET_CONTINUE;
    }


#ifdef INCLUDE_USB_KEYBOARD
/*************************************************************************
*
* cmdKbdInit - initializes USB keyboard SIO driver
*
* This function initializes the USB Keyboard SIO driver
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdKbdInit
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    if (kbdInitialized == TRUE)
        {
        fprintf (fout, "Keyboard already initialized \n");
        return  RET_CONTINUE;
        }

    if (usbKeyboardDevInit () == OK)
        {
        fprintf (fout, "usbKeyboardDevInit() returned OK\n");
        kbdInitialized = TRUE;
        }
    else
        fprintf (fout, "usbKeyboardDevInit() returned ERROR\n");

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdKbdDown - shuts down USB keyboard SIO driver
*
* This function shuts down the USB keybard SIO Driver
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdKbdDown
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    if (kbdInitialized == FALSE)
        {
        fprintf (fout,"Keyboard not initialized \n");
        return  RET_CONTINUE;
        }

    if (usbKeyboardDevShutdown () == OK)
        {
        fprintf (fout, "usbKeyboardDevShutdown() returned OK\n");
        kbdInitialized = FALSE;
        }
    else
        fprintf (fout, "usbKeyboardDevShutdown() returned ERROR\n");

    return RET_CONTINUE;
    }

LOCAL SIO_CHAN *pKbdSioChan;

/*************************************************************************
*
* kbdAttachCallback - receives callbacks from USB keyboard SIO driver
*
* This is a callback function which is called everytime a device is
* connected or removed.
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID kbdAttachCallback
    (
    pVOID arg,                  /* caller-defined argument */
    SIO_CHAN *pChan,            /* pointer to affected SIO_CHAN */
    UINT16 attachCode           /* defined as USB_KBD_xxxx */
    )

    {
    FILE *fout = (FILE *) arg;

    fprintf (fout, "pChan = %p, attach code = %s\n", (void *) pChan,
            (attachCode == USB_KBD_ATTACH) ? "USB_KBD_ATTACH" : "USB_KBD_REMOVE");

    if (attachCode == USB_KBD_ATTACH)
        {
        if (pKbdSioChan == NULL)
            {
            if (usbKeyboardSioChanLock (pChan) != OK)
                fprintf (fout, "usbKeyboardSioChanLock() returned ERROR\n");
            else
                pKbdSioChan = pChan;
            }
        else
            {
            fprintf (fout, "Another channel already in use, ignored.\n");
            }
        }
    else
        {
        if (pChan == pKbdSioChan)
            {
            if (usbKeyboardSioChanUnlock (pChan) != OK)
                fprintf (fout, "usbKeyboardSioChanUnlock() returned ERROR\n");

            pKbdSioChan = NULL;
            }
        }
    }


/*************************************************************************
*
* cmdKbdPoll - Polls keyboard SIO driver for input
*
* This function polls the keyboard driver of input
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdKbdPoll
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    THREAD_HANDLE thread;
    char inChar;

    /* register for callbacks */

    pKbdSioChan = NULL;

    if (kbdInitialized == FALSE)
        {
        fprintf (fout,"Keybard Driver not initialized\n");
        return  RET_CONTINUE;
        }

    if (usbKeyboardDynamicAttachRegister (kbdAttachCallback, (pVOID) fout) != OK)
        {
        fprintf (fout, "usbKeyboardDynamicAttachRegister() returned ERROR\n");
        return RET_CONTINUE;
        }


    /* Poll for input or until user presses CTRL-Z on USB keyboard or
     * [enter] on main keyboard. */

    /* Create thread to watch for keypress */

    enterPressed = FALSE;

    if (OSS_THREAD_CREATE (enterThread, (pVOID) fout, OSS_PRIORITY_INHERIT, "tEnter",
            &thread) != OK)
        goto pipe_done;

    fprintf (fout, "Press CTRL-Z to terminate polling.\n");

    while (!enterPressed)
        {
        if (pKbdSioChan != NULL)
            {
            if ((*pKbdSioChan->pDrvFuncs->pollInput) (pKbdSioChan, &inChar) == OK)
                {
                fprintf (fout, "ASCII %3d", inChar);
                if (inChar >= 32)
                    fprintf (fout, " '%c'", inChar);
                fprintf (fout, "\n");

                if (inChar == CTRL_Z)
                    {
                    fprintf (fout, "Stopped by CTRL-Z\n");
                    break;
                    }
                }
            }

        OSS_THREAD_SLEEP (1);
        }

    OSS_THREAD_DESTROY (thread);

pipe_done:

    /* unregister */

    if (usbKeyboardDynamicAttachUnRegister (kbdAttachCallback, (pVOID) fout) != OK)
        fprintf (fout, "usbKeyboardDynamicAttachUnRegister() returned ERROR\n");

    return RET_CONTINUE;
    }
#endif  /*INCLUDE_USB_KEYBOARD*/


#ifdef INCLUDE_USB_MOUSE

LOCAL INT32 mouseX;
LOCAL INT32 mouseY;
LOCAL SIO_CHAN *pMseSioChan;

/*************************************************************************
*
* showMouseState - Shows the state of mouse
*
* This function shows the state of mouse. It shows the mouse co-ordinates and
* buttons pressed or releaseds
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID showMouseState
    (
    FILE *fout,
    UINT8 buttons
    )

    {
    fprintf (fout, "\rx:%5d  y:%5d  B1:%3.3s B2:%3.3s B3:%3.3s",
             mouseX, mouseY,
             (buttons & MOUSE_BUTTON_1) == 0 ? "UP " : "DWN",
             (buttons & MOUSE_BUTTON_2) == 0 ? "UP " : "DWN",
             (buttons & MOUSE_BUTTON_3) == 0 ? "UP " : "DWN");
    }


/*************************************************************************
*
* signExtend - extends sign from char to long
*
* This function extends sign from char to long
*
* RETURNS: sign-extended long value
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL INT32 signExtend
    (
    char value
    )

    {
    if ((value & 0x80) != 0)
        return value | 0xffffff00;

    return value;
    }


/*************************************************************************
*
* mseRptCallback - invoked when reports received from mouse
*
* This is a callback function which is invoked when reports are received
* from the mouse device.
*
* RETURNS: OK
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS mseRptCallback
    (
    void *arg,
    pHID_MSE_BOOT_REPORT pReport
    )

    {
    FILE *fout = (FILE *) arg;
    INT32 xChange = signExtend (pReport->xDisplacement);
    INT32 yChange = signExtend (pReport->yDisplacement);

    mouseX += xChange;
    mouseY += yChange;

    showMouseState (fout, pReport->buttonState);

    return OK;
    }

/*************************************************************************
*
* mseAttachCallback - receives callbacks from USB mouse SIO driver
*
* This function is called everytime the mouse is connected or disconnected
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID mseAttachCallback
    (
    pVOID arg,                  /* caller-defined argument */
    SIO_CHAN *pChan,            /* pointer to affected SIO_CHAN */
    UINT16 attachCode           /* defined as USB_KBD_xxxx */
    )

    {
    FILE *fout = (FILE *) arg;

    fprintf (fout, "pChan = %p, attach code = %s\n", (void *) pChan,
        (attachCode == USB_MSE_ATTACH) ? "USB_MSE_ATTACH" : "USB_MSE_REMOVE");

    if (attachCode == USB_MSE_ATTACH)
        {
        if (pMseSioChan == NULL)
            {
            if (usbMouseSioChanLock (pChan) != OK)
                fprintf (fout, "usbMouseSioChanLock() returned ERROR\n");
            else
                {
                pMseSioChan = pChan;

                /* Register for report callbacks */

                if (pMseSioChan != NULL)
                    {
                    if ((*pMseSioChan->pDrvFuncs->callbackInstall) (pMseSioChan,
                                SIO_CALLBACK_PUT_MOUSE_REPORT,
                                (STATUS  (*) (void *, ...)) mseRptCallback,
                        fout) != OK)
                        {
                        fprintf (fout, "callbackInstall() returned ERROR.\n");
                        }
                    }
                }
            }
        else
            {
            fprintf (fout, "Another channel already in use, ignored.\n");
            }
        }
    else
        {
        if (pChan == pMseSioChan)
            {
            if (usbMouseSioChanUnlock (pChan) != OK)
                fprintf (fout, "usbMouseSioChanUnlock() returned ERROR\n");

            pMseSioChan = NULL;
            }
        }
    }


/*************************************************************************
*
* cmdMouseTest - Tests mouse SIO driver for input
*
* This function tests the mouse SIO Driver for input
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdMouseTest
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    THREAD_HANDLE thread;


    /* Initialize usbMouseLib. */

    if (usbMouseDevInit () != OK)
        {
        fprintf (fout, "usbMouseDevInit() returned ERROR\n");
        return RET_CONTINUE;
        }


    /* register for attach callbacks */

    pMseSioChan = NULL;

    mouseX = 0;
    mouseY = 0;

    if (usbMouseDynamicAttachRegister (mseAttachCallback, (pVOID) fout) != OK)
        {
        fprintf (fout, "usbMouseDynamicAttachRegister() returned ERROR\n");
        goto mouseDown;
        }


    /* Wait for use to press [enter] to terminate test. */

    /* Create thread to watch for keypress */

    enterPressed = FALSE;

    if (OSS_THREAD_CREATE (enterThread, (pVOID) fout, OSS_PRIORITY_INHERIT, "tEnter",
        &thread) != OK)
        {
        fprintf (fout, "Failed to create tEnter thread.\n");
        goto mouseUnreg;
        }


    /* Wait for mouse to be attached */

    while (pMseSioChan == NULL)
        {
        if (enterPressed)
            goto threadDone;
        OSS_THREAD_SLEEP (1);
        }


    /* Let background threads show mouse state */

    showMouseState (fout, 0);

    while (!enterPressed)
        OSS_THREAD_SLEEP (1);

    fprintf (fout, "\n");


threadDone:

    OSS_THREAD_DESTROY (thread);


mouseUnreg:

    /* unregister for attach callbacks */
fprintf(fout, "mouseUnreg\n");
    if (usbMouseDynamicAttachUnRegister (mseAttachCallback, (pVOID) fout) != OK)
        fprintf (fout, "usbMouseDynamicAttachUnRegister() returned ERROR\n");


    /* Terminate report callbacks */

    if (pMseSioChan != NULL)
        if ((*pMseSioChan->pDrvFuncs->callbackInstall) (pMseSioChan,
            SIO_CALLBACK_PUT_MOUSE_REPORT, NULL, NULL) != OK)
            {
            fprintf (fout, "callbackInstall() returned ERROR.\n");
            }

mouseDown:
    fprintf(fout, "usbMouseDevShutdown\n");

    if (usbMouseDevShutdown () != OK)
        fprintf (fout, "usbMouseDevShutdown() returned ERROR\n");


    return RET_CONTINUE;
    }
#endif  /*INCLUDE_USB_KEYBOARD*/


#ifdef INCLUDE_USB_PRINTER

LOCAL SIO_CHAN *pPrnSioChan;

/*************************************************************************
*
* closeTxFile - closes any open print tx file
*
* This function closes any file open for print
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID closeTxFile (void)
    {
    txCharCount = 0;

    if (txFile != NULL)
        {
        fclose (txFile);
        txFile = NULL;
        }
    }


/*************************************************************************
*
* prnTxCallback - feeds characters to USB printer SIO driver
*
* This function feeds characters in the printer to print. The characters
* are fed into the buffer <txBfr>.
*
* RETURNS: OK
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS prnTxCallback
    (
    void *callbackParam,
    char *txChar,
    STATUS irpStatus
    )

    {
    /* If no more chars to send, return an error */

    if ((txCharCount == 0) ||
        (irpStatus == ERROR))
        {
#ifdef TEST_PRINT_PERFORMANCE
        /* End the counter */
        tickEnd = tickGet();

        /* Calculate the number of ticks */
        tickForOperation = tickEnd - tickStart;

        if (tickForOperation == 0)
            tickForOperation =1;

        /* Calculate the data rate */
        uDataRate = (blocks * 4096 * 8 * nClockRate) / tickForOperation;
        printf ("The Rate of Transfer is %ld \n",uDataRate);
#endif
        return ERROR;

        }

    txCharCount--;


    /* Check if running a pattern test or a file dump */

    if (patternTest)
        {
        /* Running a pattern test.  Return the next pattern char */

        if ((nextCharVal & 1) == 0)
            *txChar = nextCharVal >> 8;
        else
            *txChar = nextCharVal & 0xff;

        nextCharVal++;
        }
    else
        {
        /* Running a file dump test.  Return the next char from file */

        if (txBfrCount == 0 && txCharCount > 0)
            {
            /* Read next buffer from file */

            txBfrCount = fread (txBfr, sizeof (char), sizeof (txBfr), txFile);
            txBfrIndex = 0;
            }

        if (txCharCount == 0 || txBfrCount == 0)
            {
            closeTxFile ();
            txCharCount = 0;
            }

        if (txBfrCount == 0)
            return ERROR;

        *txChar = txBfr [txBfrIndex++];
        --txBfrCount;
        }

    return OK;
    }


/*************************************************************************
*
* prnAttachCallback - receives attach callbacks from printer SIO driver
*
* This function is called whenever the printer device is connected or
* dis-connected.
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID prnAttachCallback
    (
    pVOID arg,                  /* caller-defined argument */
    SIO_CHAN *pChan,            /* pointer to affected SIO_CHAN */
    UINT16 attachCode           /* defined as USB_KBD_xxxx */
    )

    {
    FILE *fout = (FILE *) arg;

    fprintf (fout, "pChan = %p, attach code = %s\n", (void *)pChan,
        (attachCode == USB_PRN_ATTACH) ? "USB_PRN_ATTACH" : "USB_PRN_REMOVE");

    if (attachCode == USB_PRN_ATTACH)
        {
        if (pPrnSioChan == NULL)
            {
            if (usbPrinterSioChanLock (pChan) != OK)
                fprintf (fout, "usbPrinterSioChanLock() returned ERROR\n");
            else
                {
                pPrnSioChan = pChan;

                if ((*pPrnSioChan->pDrvFuncs->callbackInstall) (pPrnSioChan,
                        SIO_CALLBACK_GET_TX_CHAR,
                        (STATUS  (*) (void *, ...)) prnTxCallback,
                    NULL) != OK)
                    {
                    fprintf (fout, "callbackInstall() returned ERROR.\n");
                    }
                }
            }
        else
            {
            fprintf (fout, "Another channel already in use, ignored.\n");
            }
        }
    else
        {
        if (pChan == pPrnSioChan)
            {
            if (usbPrinterSioChanUnlock (pChan) != OK)
                fprintf (fout, "usbPrinterSioChanUnlock() returned ERROR\n");

            pPrnSioChan = NULL;
            }
        }
    }

LOCAL BOOL prnInitialized = FALSE;

/*************************************************************************
*
* cmdPrnInit - initializes USB printer SIO driver
*
* This function initializes the USB pritner SIO Driver. If the printer
* is already initialized it returns
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPrnInit
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    if (prnInitialized)
        {
        fprintf (fout, "USB printer SIO driver already initialized.\n");
        return RET_CONTINUE;
        }

    if (usbPrinterDevInit () == OK)
        {
        fprintf (fout, "usbPrinterDevInit() returned OK\n");
        prnInitialized = TRUE;

        /* Register for attach notification */

        if (usbPrinterDynamicAttachRegister (prnAttachCallback, (pVOID) fout) != OK)
            {
            fprintf (fout, "usbPrinterDynamicAttachRegister() returned ERROR\n");
            return RET_CONTINUE;
            }
        }
    else
        fprintf (fout, "usbPrinterDevInit() returned ERROR\n");

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPrnDown - shuts down USB printer SIO driver
*
* This function shut downs the USB Printer SIO Driver
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPrnDown
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    if (!prnInitialized)
        {
        fprintf (fout, "USB printer SIO driver not initialized.\n");
        return RET_CONTINUE;
        }

    prnInitialized = FALSE;
    pPrnSioChan = NULL;

    /* unregister */

    if (usbPrinterDynamicAttachUnRegister (prnAttachCallback, (pVOID) fout) != OK)
        fprintf (fout, "usbPrinterDynamicAttachUnRegister() returned ERROR\n");

    if (usbPrinterDevShutdown () == OK)
        fprintf (fout, "usbPrinterDevShutdown() returned OK\n");
    else
        fprintf (fout, "usbPrinterDevShutdown() returned ERROR\n");

    return RET_CONTINUE;
    }


/*************************************************************************
*
* waitForPrinter - waits for a printer to be connected
*
* This is a thread which waits for a printer to be connected.
*
* RETURNS: OK if printer connected, else ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS waitForPrinter
    (
    FILE *fout
    )

    {
    THREAD_HANDLE thread;
    UINT8 * pBfr;
    USB_PRINTER_CAPABILITIES * pCaps;
    UINT16 idLen;
    UINT8 protocol;
    UINT16 i;


    /* Create thread to watch for keypress */

    enterPressed = FALSE;

    if (OSS_THREAD_CREATE (enterThread,
                           (pVOID) fout,
                           OSS_PRIORITY_INHERIT,
                           "tEnter",
                           &thread)
                        != OK)
        {
        fprintf (fout, "Error creating thread.\n");
        return ERROR;
        }


    /* Wait for a printer to be attached. */

    if (pPrnSioChan == NULL)
        {
        fprintf (fout, "Waiting for printer to be attached...\n");
        while (!enterPressed && pPrnSioChan == NULL)
            OSS_THREAD_SLEEP (1);
        }


    /* kill keypress thread */

    OSS_THREAD_DESTROY (thread);

    if (enterPressed)
        return ERROR;

    if ((pBfr = OSS_MALLOC (USB_PRN_MAX_DEVICE_ID_LEN)) == NULL)
        return ERROR;

    pCaps = (USB_PRINTER_CAPABILITIES *) pBfr;

    /* Display the printer characteristics. */

    if ((*pPrnSioChan->pDrvFuncs->ioctl) (pPrnSioChan,
                                          SIO_USB_PRN_DEVICE_ID_GET,
                                          (void *) pBfr)
                                        != OK)
        fprintf (fout, "ioctl (SIO_USB_PRN_DEVICE_ID_GET) returned ERROR.\n");
    else
        {
        idLen = FROM_BIGW (pCaps->length);
        fprintf (fout, "Device ID length = %d\n", idLen);
        fprintf (fout, "Device ID = ");
        for (i = 0; i < idLen - 2; i++)
            fprintf (fout, "%c", pCaps->caps [i]);
        fprintf (fout, "\n");
        }

    if ((*pPrnSioChan->pDrvFuncs->ioctl) (pPrnSioChan, SIO_USB_PRN_PROTOCOL_GET,
        (void *) &protocol) != OK)
        fprintf (fout, "ioctl (SIO_USB_PRN_PROTOCOL_GET) returned ERROR.\n");
    else
        {
        fprintf (fout, "protocol = 0x%x ", protocol);
        switch (protocol)
            {
            case USB_PROTOCOL_PRINTER_UNIDIR:
                fprintf (fout, "(USB_PROTOCOL_PRINTER_UNIDIR)\n");
                break;

            case USB_PROTOCOL_PRINTER_BIDIR:
                fprintf (fout, "(USB_PROTOCOL_PRINTER_BIDIR)\n");
                break;

            default:
                fprintf (fout, "(unknown)\n");
                break;
            }
        }

    OSS_FREE (pBfr);

    return OK;
    }


/*************************************************************************
*
* cmdPrint - performs printer test
*
* This function performs the print test. It send blocks of 4096 bytes
* for printing
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPrint
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    /* Get parameters */

    *ppCmd = GetHexToken (*ppCmd, &blocks, 1);


    /* Wait for a printer to be connected */

    if (waitForPrinter (fout) == OK)
        {
        /* trigger a transmission. */

        patternTest = TRUE;
        nextCharVal = 0;
        txCharCount = blocks * 4096;

        fprintf (fout, "sending %d 4k blocks to printer...\n", (UINT16) blocks);

#ifdef TEST_PRINT_PERFORMANCE  /* performance */
        /* Determine the clock rate */
        nClockRate = sysClkRateGet();
        /* Start the counter */
        tickStart = tickGet();
#endif

        if ((*pPrnSioChan->pDrvFuncs->txStartup) (pPrnSioChan) != OK)
            fprintf (fout, "txStartup() returned ERROR.\n");
        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPrintFnm - sends file to printer
*
* This function send a file for printing. It read the content of the file
* in a buffer and sends that buffer.
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPrintFnm
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    char fnm [MAX_CMD_LEN];
    FILE *f;
    long fsize;


    /* Get filename parameter */

    *ppCmd = GetNextToken (*ppCmd, fnm, sizeof (fnm));


    /* Terminate any current dump test */

    closeTxFile ();


    /* Attempt to open file */

    if ((f = fopen (fnm, "rb")) == NULL)
        {
        fprintf (fout, "Unable to open '%s'.\n", fnm);
        return RET_CONTINUE;
        }


    /* Determine file size */

    fseek (f, 0, SEEK_END);
    fsize = ftell (f);

    fprintf (fout, "file '%s' is %ld bytes.\n", fnm, fsize);

    fseek (f, 0, SEEK_SET);


    /* Wait for printer */

    if (waitForPrinter (fout) == OK)
        {
        /* Initiate transmission */

        patternTest = FALSE;
        txFile = f;
        txBfrCount = 0;
        txCharCount = fsize;

        fprintf (fout, "sending %ld bytes to printer...\n", fsize);

        if ((*pPrnSioChan->pDrvFuncs->txStartup) (pPrnSioChan) != OK)
            {
            fprintf (fout, "txStartup() returned ERROR.\n");
            closeTxFile ();
            }

        /* NOTE: If this path is successful, the file will be closed by
         * the prnTxCallback().
         */

        }
    else
        {

        /* Close input file */

        fclose (f);
        }

    return RET_CONTINUE;
    }


/*****************************************************************************
* stop - Stops the file to print
*
* This is used to stop the file to print.
*
* RETURNS: RET_CONTINUE;
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdStop
    (
    pVOID Param,                /* Generic parameter passed down */
    char **ppCmd,               /* Ptr to remainder of cmd line */
    FILE *fin,                  /* stream for input (if any) */
    FILE *fout                  /* stream for output (if any) */
    )
    {

    if (pPrnSioChan == NULL)
        {
        fprintf (stdout,"Printer is not connected...\n ");
        }


    if ((*pPrnSioChan->pDrvFuncs->ioctl) (pPrnSioChan, SIO_USB_PRN_CANCEL, NULL)
        != OK)
        {
        fprintf (stdout, "stop() returned ERROR.\n");
        }

    return RET_CONTINUE;
    }

#endif  /*INCLUDE_USB_PRINTER*/


#ifdef INCLUDE_USB_SPEAKER

LOCAL SEQ_DEV *pSpkrSeqDev = NULL;
LOCAL BOOL audioThreadBusy = FALSE;
LOCAL UINT32 wavDataLen;
LOCAL BOOL spkrInitialized = FALSE;

/*************************************************************************
*
* spkrAttachCallback - receives attach callbacks from speaker SEQ_DEV driver
*
* This is a callback function which is called everytime the speaker device is
* connected or disconnected
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID spkrAttachCallback
    (
    pVOID arg,                  /* caller-defined argument */
    SEQ_DEV *pSeqDev,           /* pointer to affected SEQ_DEV */
    UINT16 attachCode           /* defined as USB_KBD_xxxx */
    )

    {
    FILE *fout = (FILE *) arg;

    fprintf (fout, "pSeqDev = %p, attach code = %s\n", (void *)pSeqDev,
        (attachCode == USB_SPKR_ATTACH) ? "USB_SPKR_ATTACH" : "USB_SPKR_REMOVE");

    /* Exclusively access the mutex */
    semTake(audioMutex, WAIT_FOREVER);

    if (attachCode == USB_SPKR_ATTACH)
        {
        if (pSpkrSeqDev == NULL)
            {
            if (usbSpeakerSeqDevLock (pSeqDev) != OK)
                fprintf (fout, "usbSpeakerSeqDevLock() returned ERROR\n");
            else
                {
                pSpkrSeqDev = pSeqDev;
                }
            }
        else
            {
            fprintf (fout, "Another channel already in use, ignored.\n");
            }
        }
    else
        {
        if (pSeqDev == pSpkrSeqDev)
            {
            if (usbSpeakerSeqDevUnlock (pSeqDev) != OK)
                fprintf (fout, "usbSpeakerSeqDevUnlock() returned ERROR\n");

            pSpkrSeqDev = NULL;
            }
        }
    /* Release the mutex */
    semGive(audioMutex);
    }


/***************************************************************************
*
* audioThread - Dumps audio data to usbSpeakerLib
*
* By convention, <param> is the file handle for the file to be played and
* the global "wavDataLength" should be the length of the data chunk.  The
* file position should be set to the beginning of the data in the data chunk.
*
* This thread closes the file after reading all data.
*
* RETURNS: N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID audioThread
    (
    pVOID param
    )

    {
    FILE *wavFile = (FILE *) param;
    pUINT8 pBfr;
    UINT32 remDataLen = wavDataLen;
    UINT32 actLen;

    /* Exclusively access the mutex */
    semTake(audioMutex, WAIT_FOREVER);

    /* Create a buffer for audio data */

    if ((pBfr = OSS_MALLOC (AUDIO_BFR_SIZE)) == NULL)
        {
        printf ("Out of memory creating audio buffer.\n");
        }
    else
        {
        /* Read audio data and pass it to usbSpeakerLib. */
        while (remDataLen > 0 &&
              (actLen = fread (pBfr, 1, min (remDataLen, AUDIO_BFR_SIZE),
               wavFile)) > 0)
            {
            if ((*pSpkrSeqDev->sd_seqWrt) (pSpkrSeqDev, actLen,
                pBfr, FALSE) == ERROR)
                {
                printf ("sd_seqWrt() returned ERROR.\n");
                break;
                }
            else
                {
                remDataLen -= actLen;
                }
           }

        /* Mark the end of the audio stream. */

        if ((*pSpkrSeqDev->sd_ioctl) (pSpkrSeqDev,
            USB_SPKR_IOCTL_CLOSE_AUDIO_STREAM, 0) != OK)
            {
            printf ("IOCTL CLOSE_AUDIO_STREAM returned ERROR.\n");
            }

        OSS_FREE (pBfr);
        }
    /* Release the exclusive access */
    semGive(audioMutex);


    /* Close the input file. */

    fclose (wavFile);

    audioThreadBusy = FALSE;
    }


/*************************************************************************
*
* cmdSpkrInit - initializes USB speaker SEQ_DEV driver
*
* This function initializes USB speaker SEQ_DEV driver. If the speaker is
* already initialized it returns. Once initialized it registes the
* speaker calback function
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdSpkrInit
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    if (spkrInitialized)
        {
        fprintf (fout, "USB speaker SEQ_DEV driver already initialized.\n");
        return RET_CONTINUE;
        }

    /* Create the mutex used for synchronising the request
     * and the device removal
     */
    audioMutex = semMCreate(SEM_Q_FIFO);

    /* Check if semaphore creation is successful */

    if (audioMutex == NULL)
        {
        printf("Mutex creation is erroneous\n");
        return RET_CONTINUE;
        }

    if (usbSpeakerDevInit () == OK)
        {
        fprintf (fout, "usbSpeakerDevInit() returned OK\n");
        spkrInitialized = TRUE;

        /* Register for attach notification */

        if (usbSpeakerDynamicAttachRegister (spkrAttachCallback, (pVOID) fout) != OK)
            {
            if (semDelete(audioMutex) != OK)
                fprintf (fout, "semDelete Failed \n");
            fprintf (fout, "usbSpeakerDynamicAttachRegister() returned ERROR\n");
            return RET_CONTINUE;
            }
        }
    else
        {
        if (semDelete(audioMutex) != OK)
            fprintf (fout, "semDelete Failed \n");
        fprintf (fout, "usbSpeakerDevInit() returned ERROR\n");
        }

    audioThreadBusy = FALSE;

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdSpkrDown - shuts down USB speaker SEQ_DEV driver
*
* This function shuts down USB speaker SEQ_DEV driver
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdSpkrDown
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    if (!spkrInitialized)
        {
        fprintf (fout, "USB speaker SEQ_DEV driver not initialized.\n");
        return RET_CONTINUE;
        }

    if (audioThreadBusy)
        {
        fprintf (fout, "audioThread is busy.\n");
        return RET_CONTINUE;
        }

    /* Exclusively access the mutex */
    semTake(audioMutex, WAIT_FOREVER);

    spkrInitialized = FALSE;
    pSpkrSeqDev = NULL;

    /* unregister */

    if (usbSpeakerDynamicAttachUnRegister (spkrAttachCallback, (pVOID) fout) != OK)
        fprintf (fout, "usbSpeakerDynamicAttachUnRegister() returned ERROR\n");

    if (usbSpeakerDevShutdown () == OK)
        fprintf (fout, "usbSpeakerDevShutdown() returned OK\n");
    else
        fprintf (fout, "usbSpeakerDevShutdown() returned ERROR\n");

    if (semDelete(audioMutex) != OK)
       fprintf (fout, "Invalid semaphore ID \n");

    return RET_CONTINUE;
    }


/*************************************************************************
*
* waitForSpeaker - waits for a speaker to be connected
*
* This is a thread which waits for a speaker to the connected
*
* RETURNS: OK if speaker connected, else ERROR
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS waitForSpeaker
    (
    FILE *fout
    )

    {
    THREAD_HANDLE thread;


    /* Create thread to watch for keypress */

    enterPressed = FALSE;

    if (OSS_THREAD_CREATE (enterThread, (pVOID) fout, OSS_PRIORITY_INHERIT, "tEnter",
        &thread) != OK)
        {
        fprintf (fout, "Error creating thread.\n");
        return ERROR;
        }


    /* Wait for a speaker to be attached. */

    if (pSpkrSeqDev == NULL)
        {
        fprintf (fout, "Waiting for speaker to be attached...\n");
        while (!enterPressed && pSpkrSeqDev == NULL)
            OSS_THREAD_SLEEP (1);
        }


    /* kill keypress thread */

    OSS_THREAD_DESTROY (thread);

    if (enterPressed)
        return ERROR;


    return OK;
    }


/*************************************************************************
*
* cmdSpkrFmt - Displays available speaker formats
*
* This function displays the available speaker formats. It carries out
* this using the ioctl command USB_SPKR_IOCTL_GET_FORMAT_COUNT.
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdSpkrFmt
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    UINT16 fmtCount, i;
    pUSB_SPKR_AUDIO_FORMAT pFmt;

    if (!spkrInitialized)
        {
        fprintf (fout, "USB speaker SEQ_DEV driver not initialized.\n");
        return RET_CONTINUE;
        }

    if (waitForSpeaker (fout) != OK)
        return RET_CONTINUE;

    /* Exclusively access the mutex */
    semTake(audioMutex, WAIT_FOREVER);

    /* Get number and info about speaker formats */

    if ((*pSpkrSeqDev->sd_ioctl) (pSpkrSeqDev,
        USB_SPKR_IOCTL_GET_FORMAT_COUNT, &fmtCount) != OK)
        {
        fprintf (fout, "IOCTL GET_FORMAT_COUNT returned ERROR\n");
        semGive(audioMutex);
        return RET_CONTINUE;
        }

    fprintf (fout, "Speaker supports %d formats.\n", fmtCount);

    if ((*pSpkrSeqDev->sd_ioctl) (pSpkrSeqDev,
        USB_SPKR_IOCTL_GET_FORMAT_LIST, &pFmt) != OK)
        {
        fprintf (fout, "IOCTL GET_FORMAT_LIST returned ERROR\n");
        semGive(audioMutex);
        return RET_CONTINUE;
        }
    /* Release the exclusive access */
    semGive(audioMutex);

    for (i = 0; i < fmtCount; i++)
        {
        fprintf (fout, "format [%d]:\n", i);
        fprintf (fout, "  interface = %d, alt setting = %d\n",
            pFmt [i].interface, pFmt [i].altSetting);
        fprintf (fout, "  endpoint = 0x%x, maxPacketSize = %d, delay = %d\n",
            pFmt [i].endpoint, pFmt [i].maxPacketSize, pFmt [i].delay);
        fprintf (fout, "  formatTag = 0x%x, formatType = %d\n",
            pFmt [i].formatTag, pFmt [i].formatType);

        switch (pFmt [i].formatType)
            {
            case USB_AUDIO_FORMAT_TYPE1:
            case USB_AUDIO_FORMAT_TYPE3:

                fprintf (fout, "  channels = %d\n", pFmt [i].channels);
                fprintf (fout, "  subFrameSize = %d\n", pFmt [i].subFrameSize);
                fprintf (fout, "  bitRes = %d\n", pFmt [i].bitRes);
                break;

            case USB_AUDIO_FORMAT_TYPE2:

                fprintf (fout, "  maxBitRate = %d\n", pFmt [i].maxBitRate);
                fprintf (fout, "  samplesPerFrame = %d\n", pFmt [i].samplesPerFrame);
                break;

            default:

                fprintf (fout, "  <<unrecognized format type>>\n");
                break;
            }
        }

    return RET_CONTINUE;
    }


/***************************************************************************
*
* parseWavFile - parses and displays info about a .wav file
*
* Attempts to play the .wav file.
*
* NOTE: If this function returns TRUE, the caller SHOULD NOT close the
* wavFile.  That will be done automatically when playing is finished.
*
* RETURNS: OK if able to play file, else ERROR.
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL STATUS parseWavFile
    (
    FILE *wavFile,
    FILE *fout
    )

    {
    RIFF_HDR riffHdr;
    char wavSig [RIFF_WAV_DATA_SIG_LEN];
    RIFF_CHUNK_HDR chunkHdr;
    WAV_FORMAT_CHUNK fmtChunk;
    UINT32 fileLen;
    UINT32 chunkLen;
    int i;

    USB_SPKR_AUDIO_FORMAT fmt;
    THREAD_HANDLE thread;


    /* Check the RIFF/WAV header. */

    if (fseek (wavFile, 0L, SEEK_SET) != 0)
        {
        fprintf (fout, "Cannot seek to beginning of file.\n");
        return ERROR;
        }

    if (fread (&riffHdr, 1, sizeof (riffHdr), wavFile) < sizeof (riffHdr) ||
        fread (&wavSig, 1, sizeof (wavSig), wavFile) < sizeof (wavSig))
        {
        fprintf (fout, "Unexpected end of file reading RIFF header.\n");
        return ERROR;
        }

    if (memcmp (&riffHdr.signature, RIFF_HDR_SIG, RIFF_HDR_SIG_LEN) != 0 ||
        memcmp (wavSig, RIFF_WAV_DATA_SIG, RIFF_WAV_DATA_SIG_LEN) != 0)
        {
        fprintf (fout, "Not a .wav file.\n");
        return ERROR;
        }


    /* Read and display known chunks */

    fileLen = FROM_LITTLEL (riffHdr.length) + sizeof (riffHdr);
    fprintf (fout, ".wav file size = %lu bytes.\n", (unsigned long) fileLen);

    while ((UINT32) ftell (wavFile) < fileLen)
        {
        fprintf (fout, "\n");

        /* Read the next chunk header. */

        if (fread (&chunkHdr, 1, sizeof (chunkHdr), wavFile) < sizeof (chunkHdr))
            {
            fprintf (fout, "Unexpected end of file reading chunk header.\n");
            return ERROR;
            }

        fprintf (fout, "ChunkId = ");
        for (i = 0; i < RIFF_CHUNK_ID_LEN; i++)
            fprintf (fout, "%c", chunkHdr.chunkId [i]);

        chunkLen = FROM_LITTLEL (chunkHdr.length);
        fprintf (fout, "\nChunkLen = %lu\n", (unsigned long) chunkLen);


        /* If we recognize the chunkId, then display the chunk. */

        if (memcmp (chunkHdr.chunkId, RIFF_WAV_FMT_CHUNK_ID, RIFF_CHUNK_ID_LEN) == 0)
            {
            /* Read the format chunk. */

            if (fread (&fmtChunk, 1, sizeof (fmtChunk), wavFile) < sizeof (fmtChunk))
                {
                fprintf (fout, "Unexpected end of file reading format chunk.\n");
                return ERROR;
                }

            /* Display the format chunk */

            fprintf (fout, "formatTag = 0x%4.4x\n", FROM_LITTLEW (fmtChunk.formatTag));
            fprintf (fout, "channels = %d\n", FROM_LITTLEW (fmtChunk.channels));
            fprintf (fout, "samplesPerSec = %lu\n",
                (unsigned long) FROM_LITTLEL (fmtChunk.samplesPerSec));
            fprintf (fout, "avgBytesPerSec = %lu\n",
                (unsigned long) FROM_LITTLEL (fmtChunk.avgBytesPerSec));
            fprintf (fout, "blockAlign = %d\n", FROM_LITTLEW (fmtChunk.blockAlign));

            if (FROM_LITTLEW (fmtChunk.formatTag) == WAV_FMT_MS_PCM)
                fprintf (fout, "bitsPerSample = %d\n",
                    FROM_LITTLEW (fmtChunk.fmt.msPcm.bitsPerSample));


            /* Attempt to set the audio format to match */

            if (FROM_LITTLEW (fmtChunk.formatTag) == WAV_FMT_MS_PCM)
                {
                memset (&fmt, 0, sizeof (fmt));

                fmt.formatTag = USB_AUDIO_TYPE1_PCM;
                fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
                fmt.channels = FROM_LITTLEW (fmtChunk.channels);
                fmt.subFrameSize = FROM_LITTLEW (fmtChunk.blockAlign) /
                    FROM_LITTLEW (fmtChunk.channels);
                fmt.bitRes = FROM_LITTLEW (fmtChunk.fmt.msPcm.bitsPerSample);
                fmt.sampleFrequency = FROM_LITTLEL (fmtChunk.samplesPerSec);

                /* Exclusively access the mutex */
                semTake(audioMutex, WAIT_FOREVER);

                if (pSpkrSeqDev->sd_ioctl (pSpkrSeqDev,
                    USB_SPKR_IOCTL_SET_AUDIO_FORMAT, &fmt) == OK)
                    {
                    fprintf (fout, "\nusbSpeakerLib format set successfully.\n");
                    }
                else
                    {
                    fprintf (fout, "\nFailed to set usbSpeakerLib format.\n");
                    semGive(audioMutex);
                    return ERROR;
                    }
                    semGive(audioMutex);
                }
            }
        else if (memcmp (chunkHdr.chunkId, RIFF_WAV_DATA_CHUNK_SIG, RIFF_CHUNK_ID_LEN) == 0)
            {
            /* data chunk found */

            /* launch thread to dump audio data. */

            wavDataLen = FROM_LITTLEL (chunkHdr.length);

            if (OSS_THREAD_CREATE (audioThread, (pVOID) wavFile, OSS_PRIORITY_INHERIT,
                "tPlay", &thread) != OK)
                {
                fprintf (fout, "Cannot create audio play thread.\n");
                return ERROR;
                }
            else
                {
                audioThreadBusy = TRUE;
                fprintf (fout, "Thread created to play audio data.\n");
                return OK;
                }
            }
        else
            {
            /* Skip over the chunk. */

            fseek (wavFile, chunkLen, SEEK_CUR);
            }
        }

    return ERROR;
    }


/*************************************************************************
*
* cmdPlay - sends a .wav file to a speaker
*
* This function plays the wave file
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPlay
    (
    pVOID Param,    /* Generic parameter passed down */
    char **ppCmd,   /* Ptr to remainder of cmd line */
    FILE *fin,      /* stream for input (if any) */
    FILE *fout      /* stream for output (if any) */
    )

    {
    char fnm [MAX_CMD_LEN];
    FILE *f;


    /* Get filename parameter */

    *ppCmd = GetNextToken (*ppCmd, fnm, sizeof (fnm));


    /* Make sure usbSpeakerLib is initialized and a speaker is available */

    if (!spkrInitialized)
        {
        fprintf (fout, "USB speaker SEQ_DEV driver not initialized.\n");
        return RET_CONTINUE;
        }

    if (audioThreadBusy)
        {
        fprintf (fout, "audioThread is busy.\n");
        return RET_CONTINUE;
        }

    if (waitForSpeaker (fout) != OK)
        return RET_CONTINUE;


    /* Attempt to open the file. */

    if ((f = fopen (fnm, "rb")) == NULL)
        {
        fprintf (fout, "Unable to open '%s'.\n", fnm);
        return RET_CONTINUE;
        }


    /* Parse the file */

    if (parseWavFile (f, fout) != OK)
        fclose (f);


    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdVolume - displays or sets speaker volume
*
* The function sets the volume of the speaker
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdVolume
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    long volume;
    short volSetting;
    UINT16 channels;
    pUSB_SPKR_CHANNEL_CAPS pCaps;
    UINT16 i;


    /* verify a speaker is available */

    if (!spkrInitialized)
        {
        fprintf (fout, "USB speaker SEQ_DEV driver not initialized.\n");
        return RET_CONTINUE;
        }

    if (waitForSpeaker (fout) != OK)
        return RET_CONTINUE;

    /* Exclusively access the mutex */
    semTake(audioMutex, WAIT_FOREVER);

    /* Try to get channel information. */

    if ((*pSpkrSeqDev->sd_ioctl) (pSpkrSeqDev, USB_SPKR_IOCTL_GET_CHANNEL_COUNT,
        &channels) != OK)
        {
        fprintf (fout, "IOCTL GET_CHANNEL_COUNT returned ERROR.\n");
        semGive(audioMutex);
        return RET_CONTINUE;
        }

    if ((*pSpkrSeqDev->sd_ioctl) (pSpkrSeqDev, USB_SPKR_IOCTL_GET_CHANNEL_CAPS,
        &pCaps) != OK)
        {
        fprintf (fout, "IOCTL GET_CHANNEL_CAPS returned ERROR.\n");
        semGive(audioMutex);
        return RET_CONTINUE;
        }


    /* Get volume parameter (if specified). */

    *ppCmd = GetHexToken (*ppCmd, &volume, -1);


    /* If volume specified, then set it, else display current volume info */

    for (i = 0; i <= channels; i++)
        {
        fprintf (fout, "Channel %d: ", i);

        if (!pCaps [i].volume.supported)
            {
            fprintf (fout, "Volume not supported.\n");
            }
        else
            {
            if (volume != -1)
                {
                /* Set volume */

                volSetting = (short) (volume & 0xffff);

                if ((*pSpkrSeqDev->sd_ioctl) (pSpkrSeqDev,
                    USB_SPKR_IOCTL_SET_VOLUME,
                    ((i << 16) | ((UINT16) volSetting))) == OK)
                    {
                    fprintf (fout, "Volume set to %hx.\n", volSetting);
                    }
                else
                    {
                    fprintf (fout, "Error setting volume.\n");
                    }
                }
            else
                {
                /* Show volume settings. */

                fprintf (fout, "res = %hx, min = %hx, max = %hx, cur = %hx.\n",
                    pCaps [i].volume.res, pCaps [i].volume.min,
                    pCaps [i].volume.max, pCaps [i].volume.cur);
                }
            }
        }

    /* Release the exclusive access */
    semGive(audioMutex);

    return RET_CONTINUE;
    }

#endif  /*INCLUDE_USB_SPEAKER*/

#endif  /*INCLUDE_USB  from way above*/

#ifdef  INCLUDE_USB_TARG
/*************************************************************************
*
* cmdTargInit - initializes USB target stack
*
* This function initializes the USB Target Stack. The target stack can be
* intialized for three controller PDIUSBD12, ISP1582 & NET2280 for
* two functionalites keyboard and printer.
* Once the stack is initalized, it attaches the fucntionality specifed
* with the appropiate Target Controller Driver
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdTargInit
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
#ifdef INCLUDE_PDIUSBD12
    USB_TCD_PDIUSBD12_PARAMS paramsD12;
    long dma;
#endif
#ifdef INCLUDE_PHILIPS1582
    USB_TCD_ISP1582_PARAMS params1582;
#endif

#ifdef INCLUDE_NET2280
    USB_TCD_NET2280_PARAMS paramsNET2280;
#endif

    pUSB_TARG_CALLBACK_TABLE callbackTable;
    pVOID callbackParam;
    long ioBase;
    long irq;

    char tcdType  [MAX_CMD_LEN];
    char targType [MAX_CMD_LEN];



    /* if already initialized, just show a warning */

    if (targInit)
        {
        fprintf (fout, "Already initialized.\n");
        return RET_CONTINUE;
        }


    /* get parameters */
    *ppCmd = GetNextToken (*ppCmd, tcdType, MAX_CMD_LEN);

    printf ("TCD Type = %s..\n",tcdType);

    *ppCmd = GetNextToken (*ppCmd, targType, MAX_CMD_LEN);

    if (strcmp (tcdType , "d12") == 0)
        {
#ifdef INCLUDE_PDIUSBD12
        *ppCmd = GetHexToken (*ppCmd, &ioBase, D12EVAL_DEFAULT_IOBASE);
        *ppCmd = GetHexToken (*ppCmd, &irq, D12EVAL_DEFAULT_IRQ);
        *ppCmd = GetHexToken (*ppCmd, &dma, D12EVAL_DEFAULT_DMA);
#else
        fprintf(fout, "D12 target controller not supported.\n");
        return RET_CONTINUE;
#endif
        }
    else if (strcmp (tcdType , "isp1582") == 0)
        {
#ifdef INCLUDE_PHILIPS1582
        sysIsp1582PciInit (&ioBase , &irq);
#else
        fprintf(fout, "isp1582 target controller not supported.\n");
        return RET_CONTINUE;
#endif
        }
    else if (strcmp (tcdType , "net2280") == 0)
        {
#ifdef INCLUDE_NET2280

        sys2NET2280PciInit ();

#else
        fprintf(fout, "NET2280 target controller not supported.\n");
        return RET_CONTINUE;
#endif
        }

    else
        {
        fprintf (fout, "Must specify tcd type as 'isp1582', net2280 or 'd12'.\n");
        return RET_CONTINUE;
        }

    /* Validate target type */

    if (strcmp (targType, "kbd") == 0)
        usbTargKbdCallbackInfo (&callbackTable, &callbackParam);

    else if (strcmp (targType, "prn") == 0)
        usbTargPrnCallbackInfo (&callbackTable, &callbackParam);

#if 0
    else if (strcmp (targType, "d12") == 0)
    usbTargPhilipsD12EvalCallbackInfo (&callbackTable, &callbackParam);
#endif
    else
        {
        fprintf (fout, "Must specify target type as 'kbd', 'prn', or 'd12'.\n");
        return RET_CONTINUE;
        }


    /* Initialize usbTargLib */

    if (usbTargInitialize () == OK)
        fprintf (fout, "usbTargInitialize() returned OK\n");
    else
        {
        fprintf (fout, "usbTargInitialize() returned ERROR\n");
        return RET_CONTINUE;
        }

    targInit = TRUE;

    if (strcmp (tcdType , "d12") == 0)
        {
#ifdef INCLUDE_PDIUSBD12
        /* Attach the Philips TCD to usbTargLib */

        memset (&paramsD12, 0, sizeof (paramsD12));
        paramsD12.ioBase = ioBase;
        paramsD12.irq = (UINT16) irq;
        paramsD12.dma = (UINT16) dma;
        fprintf (fout, "Philips PDIUSBD12: ioBase = 0x%lx, irq = %d, dma = %d\n",
                 ioBase, irq, dma);
        if (usbTargTcdAttach (usbTcdPdiusbd12EvalExec, (pVOID) &paramsD12,
            callbackTable, callbackParam, &targChannel) == OK)
            {
            fprintf (fout, "usbTargTcdAttach() returned OK\n");
            }
        else
            {
            fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
            targChannel = NULL;
            return RET_CONTINUE;
            }
#endif
        }
    else if (strcmp (tcdType , "isp1582") == 0)
        {

#ifdef INCLUDE_PHILIPS1582
        /* Attach the Philips ISP 1582 to usbTargLib */

        memset (&params1582, 0, sizeof (params1582));
        params1582.ioBase = (UINT32) ioBase;
        params1582.irq = (UINT16) irq;
        fprintf (fout, "Philips ISP 1582: ioBase = 0x%lx, irq = %d\n",
                 ioBase, irq);
        if (usbTargTcdAttach (usbTcdIsp1582EvalExec, (pVOID) &params1582,
            callbackTable, callbackParam, &targChannel) == OK)
            {
            fprintf (fout, "usbTargTcdAttach() returned OK\n");
            }
        else
            {
            fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
            targChannel = NULL;
            return RET_CONTINUE;
            }
#endif
        }
    else if (strcmp (tcdType , "net2280") == 0)
        {
#ifdef INCLUDE_NET2280

        /* Attach the NetChip NET2280 TCD to usbTargLib */

        memset (&paramsNET2280, 0, sizeof (paramsNET2280));

        /* Get the base address for the Configureation Regsiter */

        paramsNET2280.ioBase [0] = BADDR_NET2280 [0];

        /* Get the base address for the 8051 MicroController */

        paramsNET2280.ioBase [1] = BADDR_NET2280 [1];

        /* Get the base address for the FIFO Buffer */

        paramsNET2280.ioBase [2] = BADDR_NET2280 [2];

        paramsNET2280.irq = (UINT16) IRQ_NET2280;

        if (usbTargTcdAttach (usbTcdNET2280Exec, (pVOID) &paramsNET2280,
            callbackTable, callbackParam, &targChannel) == OK)
            {
            fprintf (fout, "usbTargTcdAttach() returned OK\n");
            }
        else
            {
            fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
            targChannel = NULL;
            return RET_CONTINUE;
            }


#endif
        }


    fprintf (fout, "targChannel = 0x%x\n", (UINT32) targChannel);

    if (usbTargEnable (targChannel) == OK)
        {
        fprintf (fout, "usbTargEnable() returned OK\n");
        targEnable = TRUE;
        }
    else
        fprintf (fout, "usbTargEnable() returned ERROR\n");

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdTargDown - shuts down USB target stack
*
* This function shutdowns the USB Target Stack.
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdTargDown
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    /* Disabled the TCD */

    if (targEnable)
        {
        if (usbTargDisable (targChannel) == OK)
            fprintf (fout, "usbTargDisable() returned OK\n");
        else
            fprintf (fout, "usbTargDisable() returned ERROR\n");

        targEnable = FALSE;
        }


    /* Detach the TCD */

    if (targChannel != NULL)
        {
        if (usbTargTcdDetach (targChannel) == OK)
            fprintf (fout, "usbTargTcdDetach() returned OK\n");
        else
            fprintf (fout, "usbTargTcdDetach() returned ERROR\n");

        targChannel = NULL;
        }

    /* Shut down usbTargLib */

    if (targInit)
        {
        if (usbTargShutdown () == OK)
            fprintf (fout, "usbTargShutdown() returned OK\n");
        else
            fprintf (fout, "usbTargShutdown() returned ERROR\n");

        targInit = FALSE;
        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdKbdReport - injects a keyboard report into keyboard emulator
*
* This function injects a keyboard report into keyboard emulator
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdKbdReport
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    HID_KBD_BOOT_REPORT report;
    pUINT8 pBfr = (pUINT8) &report;
    long value;
    UINT16 i;

    if (**ppCmd != 0)
        {
    /* The following fills a report with as much info as provided by user */

    memset (&report, 0, sizeof (report));

    for (i = 0; i < sizeof (report); i++)
        {
        *ppCmd = GetHexToken (*ppCmd, &value, 0);
        pBfr [i] = (UINT8) value;
        }
        if (usbTargKbdInjectReport (&report, sizeof (report)) == OK)
            fprintf (fout, "usbTargKbdInjectReport() returned OK\n");
        else
            fprintf (fout, "usbTargKbdInjectReport() return ERROR\n");
        }
    else
        {
        /* If the user does not give any report, the data will be
         * from 'a' to z followed by a ctrl-z
         */
        for (i = 0; i < 26; i++)
            {
            memset (&report, 0, sizeof (report));
            report.scanCodes[0] = 0x04 + i;
            if (usbTargKbdInjectReport (&report, sizeof (report)) == OK)
                fprintf (fout, "usbTargKbdInjectReport() returned OK\n");
            else
                fprintf (fout, "usbTargKbdInjectReport() return ERROR\n");

            memset (&report, 0, sizeof (report));

            if (usbTargKbdInjectReport (&report, sizeof (report)) == OK)
                fprintf (fout, "usbTargKbdInjectReport() returned OK\n");
            else
                fprintf (fout, "usbTargKbdInjectReport() return ERROR\n");
            }

        /* Send a ctrl-z to terminate polling */
        memset (&report, 0, sizeof (report));
        report.modifiers = 0x01;
        if (usbTargKbdInjectReport (&report, sizeof (report)) == OK)
            fprintf (fout, "usbTargKbdInjectReport() returned OK\n");
        else
            fprintf (fout, "usbTargKbdInjectReport() return ERROR\n");

        memset (&report, 0, sizeof (report));
        report.modifiers = 0x01;
        report.scanCodes[0] = 0x04 + 25;
        if (usbTargKbdInjectReport (&report, sizeof (report)) == OK)
            fprintf (fout, "usbTargKbdInjectReport() returned OK\n");
        else
            fprintf (fout, "usbTargKbdInjectReport() return ERROR\n");

        memset (&report, 0, sizeof (report));
        if (usbTargKbdInjectReport (&report, sizeof (report)) == OK)
            fprintf (fout, "usbTargKbdInjectReport() returned OK\n");
        else
            fprintf (fout, "usbTargKbdInjectReport() return ERROR\n");

        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPrnDump - dumps printer data received by printer emulator
*
* This fucntion dumps the data received by the pritner emulator
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPrnDump
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    long length;
    pUINT8 pBfr;
    UINT16 actLen;

    *ppCmd = GetHexToken (*ppCmd, &length, 128);

    /* See if the printer emulator has received any data */

    if (usbTargPrnDataInfo (&pBfr, &actLen) != OK)
        {
        fprintf (fout, "Printer has no data.\n");
        }
    else
        {
        fprintf (fout, "Printer received %d bytes.\n", actLen);
        displayMem (min (length, actLen), pBfr, fout);
        }

    if (usbTargPrnDataRestart () != OK)
        fprintf (fout, "usbTargPrnDataRestart() returned ERROR.\n");

    return RET_CONTINUE;
    }

#endif  /* #ifdef INCLUDE_USB_TARG */

#if defined(INCLUDE_PCI) && !defined(INCLUDE_VXBUS)
/*************************************************************************
*
* PciClassSearch - Lists devices within a specific Class/Sub-Class/PgmIf
*
* List all devices which match the <Class>, <SubClass>, <PgmIf> spec.
*
* RETURNS:  N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL void PciClassSearch
    (
    UINT8 pciClass,     /* Class to match */
    UINT8 subClass,     /* Subclass to match */
    UINT8 pgmIf,        /* Programming interface to match */
    FILE *fout          /* output stream */
    )

    {
    int i;
    UINT8 busNo;
    UINT8 deviceNo;
    UINT8 funcNo;

    i = 0;

    while (usbPciClassFind (pciClass, subClass, pgmIf, i, &busNo, &deviceNo, &funcNo))
        {
        fprintf (fout,
            "Class 0x%2.2x%2.2x%2.2x index %d: {Bus,Device,Func} = {0x%x,0x%x,0x%x}\n",
            pciClass, subClass, pgmIf, i, busNo, deviceNo, funcNo);
        i++;
        }
    }


/*************************************************************************
*
* PciBusSearch - Search for a range of classes/sub-classes/pgmif's
*
* Search for devices matching the ranges <classMin>..<classMax>,
* <subClassMin>..<subClassMax>, <pgmIfMin>..<pgmIfMax>.  Display
* matching devices on <fout>.
*
* RETURNS:  N/A
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL void PciBusSearch
    (
    UINT8 classMin,         /* Low class */
    UINT8 classMax,         /* High class */
    UINT8 subClassMin,      /* Low subclass */
    UINT8 subClassMax,      /* High subclass */
    UINT8 pgmIfMin,         /* Low pgm if */
    UINT8 pgmIfMax,         /* High pgm if */
    FILE *fout              /* output stream */
    )

    {
    UINT16 pciClass;
    UINT16 subClass;
    UINT16 pgmIf;

    /* Loop through all PCI class/sub-class combinations, searching for
    devices. */

    for (pciClass = classMin; pciClass <= classMax; pciClass++)
        {
        for (subClass = subClassMin; subClass <= subClassMax; subClass++)
            {
            for (pgmIf = pgmIfMin; pgmIf <= pgmIfMax; pgmIf++)
                {
                PciClassSearch (pciClass, subClass, pgmIf, fout);
                }
            }
        }
    }


/*************************************************************************
*
* cmdPciEnumerate - Enumerates devices on PCI bus
*
* Enumerates PCI devices of all classes.  <ppCmd> is parsed for options
* and updated on return to point to the remainder of the command line.
* <Param> is ignored.  Output is written to <fout>.
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciEnumerate
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )
    {
    long pgmIfMax;

    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &pgmIfMax, 0);


    /* Enumerate bus */

    fprintf (fout, "Enumerating PCI bus...\n");

    PciBusSearch (0, 255, 0, 255, 0, pgmIfMax, fout);

    fprintf (fout, "Enumeration complete.\n");

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPciFindClass - list all PCI device matching class/sub-class/pgmif
*
* Lists all PCI devices matching the class/sub-class/pgmif specification.
* <ppCmd> is parsed for the class, sub-class, and option pgmif.  <ppCmd>
* is updated on return to point to remainder of command line.  <Param>
* is ignored.
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciFindClass
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    long pciClass;
    long subClass;
    long pgmIf;

    UINT8 subClassMin, subClassMax;
    UINT8 pgmIfMin, pgmIfMax;


    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &pciClass, -1);
    *ppCmd = GetHexToken (*ppCmd, &subClass, -1);
    *ppCmd = GetHexToken (*ppCmd, &pgmIf, -1);


    /* Validate parameters */

    if (pciClass < 0)
        {
        fprintf (fout, "No class specified.\n");
        return RET_CONTINUE;
        }

    if (subClass < 0)
        {
        subClassMin = 0;
        subClassMax = 255;
        }
    else
        subClassMin = subClassMax = (UINT8) subClass;

    if (pgmIf < 0)
        {
        pgmIfMin = 0;
        pgmIfMax = 255;
        }
    else
        pgmIfMin = pgmIfMax = (UINT8) pgmIf;


    /* Search for devices. */

    PciBusSearch ((UINT8) pciClass, (UINT8) pciClass, subClassMin, subClassMax,
        pgmIfMin, pgmIfMax, fout);


    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPciDevInfo - Displays configuration info for specified device.
*
* Displays PCI configuration information for specified device.  <ppCmd>
* is parsed for a bus number, device number, and optional function number.
* <ppCmd> is updated upon return to point to the remainder of the command
* line.
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciDevInfo
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    int j;
    long bus;
    long device;
    long func;

    PCI_CFG_HEADER cfgHdr;


    /* Extract parameters. */

    *ppCmd = GetHexToken (*ppCmd, &bus, -1);
    *ppCmd = GetHexToken (*ppCmd, &device, -1);
    *ppCmd = GetHexToken (*ppCmd, &func, -1);

    /* Validate parameters */

    if (bus < 0 || device < 0 || func < 0)
        {
        fprintf (fout, "Must specify bus, device, and function numbers.\n");
        return RET_CONTINUE;
        }

    /* Get device info. */

    usbPciConfigHeaderGet ((UINT8) bus, (UINT8) device, (UINT8) func, &cfgHdr);

    fprintf (fout, "Bus 0x%2.2lx Device 0x%2.2lx Func 0x%2.2lx:\n", bus, device, func);

    fprintf (fout, "Vendor Id     = 0x%4.4x (%d)\n", cfgHdr.vendorId, cfgHdr.vendorId);
    fprintf (fout, "Device Id     = 0x%4.4x (%d)\n", cfgHdr.deviceId, cfgHdr.deviceId);
    fprintf (fout, "Command       = 0x%4.4x\n", cfgHdr.command);
    fprintf (fout, "Status        = 0x%4.4x\n", cfgHdr.status);
    fprintf (fout, "Revision Id   = 0x%2.2x\n", cfgHdr.revisionId);
    fprintf (fout, "Class         = 0x%2.2x\n", cfgHdr.pciClass);
    fprintf (fout, "SubClass      = 0x%2.2x\n", cfgHdr.subClass);
    fprintf (fout, "PgmIf         = 0x%2.2x\n", cfgHdr.pgmIf);
/*  fprintf (fout, "BIST          = 0x%2.2x\n", cfgHdr.bist);  */
/*  fprintf (fout, "HeaderType    = 0x%2.2x\n", cfgHdr.headerType);  */
    fprintf (fout, "LatencyTimer  = 0x%2.2x\n", cfgHdr.latencyTimer);
    fprintf (fout, "CacheLineSize = 0x%2.2x\n", cfgHdr.cacheLineSize);

    for (j = 0; j < 6; j++)
        if (cfgHdr.baseReg [j] != 0)
            fprintf (fout, "BaseReg %d     = 0x%8.8x\n", j, cfgHdr.baseReg [j]);

    fprintf (fout, "RomBase       = 0x%8.8x\n", cfgHdr.romBase);
    fprintf (fout, "Int Pin       = 0x%2.2x\n", cfgHdr.intPin);
    fprintf (fout, "Int Line      = 0x%2.2x\n", cfgHdr.intLine);
    fprintf (fout, "MinGrant      = 0x%2.2x\n", cfgHdr.minGrant);
    fprintf (fout, "MaxLatency    = 0x%2.2x\n", cfgHdr.maxLatency);


    return RET_CONTINUE;
    }

/*************************************************************************
*
* cmdChdir - sets the current default path
*
* sets the current default path
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdChdir
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    char path [MAX_CMD_LEN];


    /* Get filename parameter */

    *ppCmd = GetNextToken (*ppCmd, path, sizeof (path));

    fprintf (fout, "setting path = %s\n", path);


    /* Change the default directory */

    if (chdir (path) != OK)
        fprintf (fout, "chdir() returned ERROR.\n");

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPath - displays current default path
*
* This function dispalys the current default path.
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPath
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    char path [MAX_CMD_LEN];


    /* Change the default directory */

    if (getcwd (path, sizeof (path)) == NULL)
        fprintf (fout, "getcwd() returned ERROR.\n");
    else
        fprintf (fout, "path = %s\n", path);

    return RET_CONTINUE;
    }

#endif /* end of INCLUDE_PCI */

#if defined(INCLUDE_PCI) && !defined(INCLUDE_VXBUS)
/*************************************************************************
*
* cmdPciInByte - Input a byte from a PCI I/O address
*
* This function reads a byte from PCI i/o Address
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciInByte
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    long port;
    long count;
    UINT8 value;

    *ppCmd = GetHexToken (*ppCmd, &port, -1);
    *ppCmd = GetHexToken (*ppCmd, &count, 1);

    if (port == -1)
        {
        fprintf (fout, "Must specify port address.\n");
        return RET_CONTINUE;
        }

    for (; count > 0; count--)
        {
        value = USB_PCI_BYTE_IN (port);
        fprintf (fout, "0x%2.2x (%u)\n", value, value);
        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPciInWord - Input a word from a PCI I/O address
*
* This function reads a word from PCI I/O Address
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciInWord
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    long port;
    long count;
    UINT16 value;

    *ppCmd = GetHexToken (*ppCmd, &port, -1);
    *ppCmd = GetHexToken (*ppCmd, &count, 1);

    if (port == -1)
        {
        fprintf (fout, "Must specify port address.\n");
        return RET_CONTINUE;
        }

    for (; count > 0; count--)
        {
        value = USB_PCI_WORD_IN (port);
        fprintf (fout, "0x%4.4x (%u)\n", value, value);
        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPciInDword - input a dword from a PCI I/O address
*
* This function input a dword from a PCI I/O address.
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciInDword
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    long port;
    long count;
    UINT32 value;

    *ppCmd = GetHexToken (*ppCmd, &port, -1);
    *ppCmd = GetHexToken (*ppCmd, &count, 1);

    if (port == -1)
        {
        fprintf (fout, "Must specify port address.\n");
        return RET_CONTINUE;
        }

    for (; count > 0; count--)
        {
        value = USB_PCI_DWORD_IN (port);
        fprintf (fout, "0x%8.8lx (%lu)\n", (long) value, (long) value);
        }

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPciOutByte - Output a byte to a PCI I/O address
*
* This function output a byte to a PCI I/O address.
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciOutByte
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    long port;
    long value;

    *ppCmd = GetHexToken (*ppCmd, &port, -1);
    *ppCmd = GetHexToken (*ppCmd, &value, -1);

    if (port == -1 || value == -1)
        {
        fprintf (fout, "Must specify port address and a value.\n");
        return RET_CONTINUE;
        }

    USB_PCI_BYTE_OUT (port, (UINT8) value);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPciOutWord - Output a word to a PCI I/O address
*
* This function output a word to a PCI I/O address
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciOutWord
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    long port;
    long value;

    *ppCmd = GetHexToken (*ppCmd, &port, -1);
    *ppCmd = GetHexToken (*ppCmd, &value, -1);

    if (port == -1 || value == -1)
        {
        fprintf (fout, "Must specify port address and a value.\n");
        return RET_CONTINUE;
        }

    USB_PCI_WORD_OUT (port, (UINT16) value);

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdPciOutDword - Output a dword to a PCI I/O address
*
* This function output a dword to a PCI I/O address.
*
* RETURNS:  RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdPciOutDword
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    long port;
    long value;

    *ppCmd = GetHexToken (*ppCmd, &port, -1);
    *ppCmd = GetHexToken (*ppCmd, &value, -1);

    if (port == -1 || value == -1)
        {
        fprintf (fout, "Must specify port address and a value.\n");
        return RET_CONTINUE;
        }

    USB_PCI_DWORD_OUT (port, (UINT32) value);

    return RET_CONTINUE;
    }

#endif /* end of INCLUDE_PCI */

/*************************************************************************
*
* cmdDump - dumps memory
*
* This function dumps the memory
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdDump
    (
    pVOID Param,            /* Generic parameter passed down */
    char **ppCmd,           /* Ptr to remainder of cmd line */
    FILE *fin,              /* stream for input (if any) */
    FILE *fout              /* stream for output (if any) */
    )

    {
    static long adrs = 0;
    long temp;
    long len;

    *ppCmd = GetHexToken (*ppCmd, &temp, -1);
    *ppCmd = GetHexToken (*ppCmd, &len, -1);

    if (temp == -1 && len == -1)
        {
        /* cmd entered without args...Dump from previous location. */

        len = 128;
        }
    else
        {
        adrs = temp;

        if (len == -1)
            len = 128;
        }

    displayMem ((UINT16) len, (pUINT8) adrs, fout);

    adrs += len;

    return RET_CONTINUE;
    }


/*************************************************************************
*
* cmdErrno - Show current errno
*
* This function shows the current errno.
*
* RETURNS: RET_CONTINUE
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL UINT16 cmdErrno
    (
    pVOID Param,        /* Generic parameter passed down */
    char **ppCmd,       /* Ptr to remainder of cmd line */
    FILE *fin,          /* stream for input (if any) */
    FILE *fout          /* stream for output (if any) */
    )

    {
    fprintf (fout, "errno = 0x%x\n", errno);

    return RET_CONTINUE;
    }


/*
 * cmdTable - Table of supported commands
 */

LOCAL CMD_DESCR cmdTable [] =
    {

#ifdef INCLUDE_USB
    {"UsbInit", 4, "UsbInit", "Initialize USBD", cmdUsbInit},
    {"UsbDown", 4, "UsbDown", "Shut down USBD", cmdUsbDown},
    {"Attach", 2, "Attach uhci | ohci | ehci | mhci | synopsyshci", "Attaches HCD to USBD", cmdAttach},
    {"Detach", 2, "Detach uhci | ohci | ehci | mhci | synopsyshci", "Detaches HCD from USBD", cmdDetach},
    {"UsbEnum", 4, "UsbEnum", "Enumerate USBs attached to system", cmdUsbEnum},

    {"UsbStats", 4, "UsbStats n", "Display statistics for bus connected to node n", cmdUsbStats},
    {"stats", 4, NULL, NULL, cmdUsbStats},

    {"GetConfig", 4, "GetConfig n", "Get USB configuration for node n", cmdGetConfig},
    {"gc", 2, NULL, NULL, cmdGetConfig},
    {"SetConfig", 4, "SetConfig n v", "Set USB configuration to v for node n", cmdSetConfig},
    {"sc", 2, NULL, NULL, cmdSetConfig},
    {"GetInterface", 4, "GetInterface n i", "Get alt. setting for node n interface i",
        cmdGetInterface},
    {"gi", 2, NULL, NULL, cmdGetInterface},
    {"SetInterface", 4, "SetInterface n i as", "Set alt. setting as for node n interface i",
        cmdSetInterface},
    {"si", 2, NULL, NULL, cmdSetInterface},
    {"GetStatus", 4, "GetStatus n typ idx len", "Get USB status typ/idx for node n", cmdGetStatus},
    {"gs", 2, NULL, NULL, cmdGetStatus},
    {"GetAddress", 4, "GetAddress n", "Get USB address for node n", cmdGetAddress},
    {"ga", 2, NULL, NULL, cmdGetAddress},
    {"SetAddress", 4, "SetAddress n a", "Set USB address to a for node n", cmdSetAddress},
    {"sa", 2, NULL, NULL, cmdSetAddress},
    {"SetFeature", 4, "SetFeature n t fs idx", "Set USB feature t/fs/idx on node n",
        cmdSetFeature},
    {"sf", 2, NULL, NULL, cmdSetFeature},
    {"ClrFeature", 4, "ClrFeature n t fs idx", "Clear USB feature t/fs/idx on node n",
        cmdClrFeature},
    {"cf", 2, NULL, NULL, cmdClrFeature},
    {"GetDescr", 4, "GetDescr n rt t idx l len", "Get USB descr rt/t/idx (lang = l) on node n",
        cmdGetDescr},
    {"gd", 2, NULL, NULL, cmdGetDescr},
    {"SynchFrame", 4, "SynchFrame n e", "Get current synch frame for endpoint e on node n",
        cmdGetSynchFrame},
    {"CurrentFrame", 3, "CurrentFrame n", "Get current frame number for bus connected to node n",
        cmdGetCurrentFrame},
    {"IntPoll", 4, "IntPoll n e [len]", "Polls interrupt endpoint e on node n for input",
        cmdIntPoll},
#endif

#ifdef INCLUDE_USB_KEYBOARD
    {"kbdInit", 4, "kbdInit", "Initializes USB keyboard SIO driver", cmdKbdInit},
    {"kbdDown", 4, "kbdDown", "Shuts down USB keyboard SIO driver", cmdKbdDown},
    {"kbdPoll", 4, "kbdPoll", "Polls USB keyboard SIO driver for input", cmdKbdPoll},
#endif

#ifdef INCLUDE_USB_MOUSE
    {"mouseTest", 6, "mouseTest", "Tests USB mouse SIO driver", cmdMouseTest},
#endif

#ifdef INCLUDE_USB_PRINTER
    {"prnInit", 4, "prnInit", "Initializes USB printer SIO driver", cmdPrnInit},
    {"prnDown", 4, "prnDown", "Shuts down USB printer SIO driver", cmdPrnDown},
    {"print4k", 7, "print4k [n]", "Sends n 4k blocks of printer data", cmdPrint},
    {"print", 5, "print filename", "Sends file to printer", cmdPrintFnm},
    {"stop", 3, "stop"," Stops a file to print", cmdStop},
#endif

#ifdef INCLUDE_USB_SPEAKER
    {"spkrInit", 5, "spkrInit", "Initializes USDB speaker SEQ_DEV driver", cmdSpkrInit},
    {"spkrDown", 5, "spkrDown", "Shuts down USB speaker SEQ_DEV driver", cmdSpkrDown},
    {"spkrFmt", 5, "spkrFmt", "Displays available speaker formats", cmdSpkrFmt},
    {"play", 4, "play filename", "Plays the specified .wav file", cmdPlay},
    {"volume", 3, "volume [level]", "Displays/sets volume", cmdVolume},
#endif

#ifdef  INCLUDE_USB_TARG
    {"targInit", 5, "targInit ['d12' | 'isp1582'] ['kbd' | 'prn' | 'd12'] [io [irq [dma]]]", "Initializes USB target stack", cmdTargInit},
    {"targDown", 5, "targDown", "Shuts down USB target stack", cmdTargDown},
    {"kbdReport", 4, "kbdReport [xxh x 8]", "Injects a keboard report", cmdKbdReport},
    {"kr", 2, NULL, NULL, cmdKbdReport},
    {"prnDump", 5, "prnDump [length]", "Dumps data received by printer emulator", cmdPrnDump},
    {"pdu", 3, NULL, NULL, cmdPrnDump},
#endif

#ifdef INCLUDE_PCI
#ifndef INCLUDE_VXBUS
    {"PCIEnum", 4, "PCIEnum [p]", "Enumerates PCI devices thru PgmIf p", cmdPciEnumerate},
    {"PCIFindClass", 8, "PCIFindClass c [s [p]]",
        "Lists PCI devices of Class c, Sub-class s, PgmIf p", cmdPciFindClass},
    {"pfc", 3, NULL, NULL, cmdPciFindClass},
    {"PCIDevInfo", 7, "PCIDevInfo b d f",
        "Displays info for PCI Device d Func f on Bus b", cmdPciDevInfo},
    {"pdi", 3, NULL, NULL, cmdPciDevInfo},

    {"chdir", 5, "chdir directory", "Sets the current default path", cmdChdir},
    {"path", 4, "path", "Displays current default path", cmdPath},

/*  {"InByte", 3, "InByte a [n]", "Input a byte from PCI I/O address a", cmdPciInByte}, */
    {"ib", 1, NULL, NULL, cmdPciInByte},
/*  {"InWord", 3, "InWord a [n]", "Input a word from PCI I/O address a", cmdPciInWord}, */
    {"iw", 2, NULL, NULL, cmdPciInWord},
/*  {"InDword", 3, "InDword a [n]", "Input a dword from PCI I/O address a", cmdPciInDword}, */
    {"id", 2, NULL, NULL, cmdPciInDword},
/*  {"OutByte", 4, "OutByte a v", "Output byte value v to PCI I/O address a", cmdPciOutByte}, */
    {"ob", 1, NULL, NULL, cmdPciOutByte},
/*  {"OutWord", 4, "OutWord a v", "Output word value v to PCI I/O address a", cmdPciOutWord}, */
    {"ow", 2, NULL, NULL, cmdPciOutWord},
/*  {"OutDword", 4, "OutDword a v", "Output dword value v to PCI I/O address a", cmdPciOutDword}, */
    {"od", 2, NULL, NULL, cmdPciOutDword},
#endif
#endif
    {"dump", 1, "dump [adrs [len]]", "Dumps memory at specified adrs", cmdDump},

    {"errno", 3, NULL, NULL, cmdErrno},

    {"Help", 4, "Help/?", "Displays list of supported commands.", CmdParserHelpFunc},
    {"?", 1, NULL, NULL, CmdParserHelpFunc},
    {"Exit", 4, "Exit/Quit/Bye", "Exits program.", CmdParserExitFunc},
    {"Quit", 1, NULL, NULL, CmdParserExitFunc},
    {"Bye", 3, NULL, NULL, CmdParserExitFunc},
    {NULL, 0, NULL, NULL, NULL}
    };


/*************************************************************************
*
* usbTool - Primary entry point for USB bus exerciser.
*
* usbTool presents the user with a command prompt.  The user enters
* commands to invoke USB functions through the USBD and USB HCD.
*
* RETURNS:  RET_OK for normal termination;
*           RET_ERROR for program failure.
*
* ERRNO: none
*/

UINT16 usbTool (void)
    {
    UINT16 s;           /* Execution status */

    /* Set buffering for stdin/stdout to "no buffering" so screen updates
    will be timely. */

    setvbuf (stdin, NULL, _IONBF, 0);
    setvbuf (stdout, NULL, _IONBF, 0);


    /* Display program banner */

    fprintf (stdout, PGM_NAME ": " PGM_DESCR ", version " PGM_VERSION "\n"
             PGM_COPYRIGHT "\n\n");


    /* Prompt the user for commands so long as the execution status
    remains ok. */

    while ((s = PromptAndExecCmd (NULL, PROMPT, stdin, stdout, cmdTable))
            == RET_CONTINUE)
        ;


    /* Display reason for termination */

    if (s == RET_OK)
        fprintf (stdout, PGM_NAME " terminating normally.\n");
    else if (s == RET_ERROR)
        fprintf (stdout, PGM_NAME " terminating with error.\n");
    else
        fprintf (stdout, PGM_NAME " terminating with code %d.\n", s);


    return s;
    }

#endif /* __INCusrUsbToolc */

/* End of file. */



