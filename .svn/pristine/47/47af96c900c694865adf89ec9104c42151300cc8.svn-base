/* usrUsbTargMsInit.c - Initialization of the mass storage function driver */

/*
 * Copyright (c) 2004, 2006, 2009-2011, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01r,29Aug13,wyy  Make MS emulator support non-Dritect_BIO mode (WIND00432485)
01q,09aug13,ljg  Set USB_SBUSCFG to INCR8 for MPC5125 errata workaround 
                 (WIND00405093)
01p,06may13,s_z  Remove compiler warning (WIND00356717)
01o,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01n,13jan10,ghs  vxWorks 6.9 LP64 adapting
01m,15sep09,w_x  Added INCLUDE_AT91_USB_TARG support (WIND00182308)
01l,09sep09,w_x  Added various BSP config options for FSL TCD (WIND00180575)
01k,30jan09,w_x  Added FSL dual role device support
01j,16jan06,ami  Warning Message Cleanup (SPR #116698)
01i,15oct04,ami  RefGen Changes
01h,11oct04,ami  Apigen Changes
01g,20sep04,ami  Net2280 Working in High Speed
01f,05aug04,hch  fix compiler warning
01e,04aug04,hch  change include file name from vxworks.h to vxWorks.h
01d,29jul04,pdg  Diab warnings fixed
01c,20jul04,???  targChannel changed to msTargChannel
01b,19jul04,hch  created the file element
01a,20apr04,jac  First
*/

/*
DESCRIPTION

This files contains the initialization function for the Mass Storage function
driver on the peripheral.

INCLUDE FILES: vxWorks.h, stdio.h, blkIo.h, usb/usbPlatform.h, usb/usb.h,
usb/usbLib.h, usb/target/usbTargLib.h, drv/usb/usbBulkDevLib.h,
usb/target/usbTargMsLib.h, usb/target/usbTcdPdiusbd12EvalLib.h,
usb/target/usbTcdIsp1582EvalLib.h, usb/target/usbTcdNET2280Lib.h,
usb/target/usbTcdFslEvalLib.h
*/

#ifndef __INCusrUsbTargMsInitc
#define __INCusrUsbTargMsInitc

/* includes */

#include "vxWorks.h"
#include "stdio.h"
#include "string.h"                     /* memset(), memcpy() */
#include "blkIo.h"
#include "xbdRamDisk.h"
#include "usb/usbPlatform.h"            /* Basic definitions  */
#include "usb/usb.h"
#include "usb/usbLib.h"
#include "usb/usbPciLib.h"              /* PCI interface      */
#include "usb/target/usbTargLib.h"
#include "drv/usb/target/usbTargMsLib.h"

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

LOCAL BOOL                      targInit = FALSE;
LOCAL USB_TARG_CHANNEL          msTargChannel = (USB_TARG_CHANNEL)NULL;
LOCAL BOOL                      targEnable = FALSE;

#ifdef USB1_1
LOCAL UINT16                    numEndpoints = 0;
LOCAL pUSB_TARG_ENDPOINT_INFO   pEndpoints = NULL;
#endif

#ifdef INCLUDE_PDIUSBD12
STATUS msInitD12(void);
#endif /* INCLUDE_PDIUSBD12 */

#ifdef INCLUDE_PHILIPS1582
extern void  sysIsp1582PciInit
    (
    long    * pBaseaddr,
    long    * pIrq
    );
STATUS msInit1582(void);
#endif /* INCLUDE_PHILIPS1582 */

#ifdef INCLUDE_NET2280
extern void sys2NET2280PciInit (void);
extern UINT32 BADDR_NET2280 [NET2280_NO_OF_PCI_BADDR];
extern UINT16 IRQ_NET2280;
STATUS msInitNet2280(void);
#endif /* INCLUDE_NET2280 */

#ifdef INCLUDE_FSL_TCD
void fslTcdCtlrInfoGet
    (
    UINT32 *    pBaseAddr,
    UINT32 *    pIrqNum
    );
STATUS msInitFsl(void);
#endif /* INCLUDE_FSL_TCD */

#ifdef INCLUDE_AT91_USB_TARG
STATUS msInitAt91(void);
#endif /* INCLUDE_AT91_USB_TARG */

extern pVOID g_rbcBlkDev;
extern SEM_ID g_rbcSync; 

/******************************************************************************
*
* usbTargMsBlockDevCreate - create an RBC BLK_DEV device.
*
* This routine creates an RBC BLK I/O device. The RAM driver will be used for
* the actual implementation.
*
* RETURNS: OK or ERROR, if not able to create the RAM Disk.
*
* ERRNO:
*  none.
*/

STATUS usbTargMsBlockDevCreate (void)
    {

    device_t  deviceId;

    if (g_rbcBlkDev != NULL)
        return(OK);

    printf("Trying to create RAM disk %s\n", BLK_DEV_NAME);

    deviceId = xbdRamDiskDevCreate
            (
            BYTES_PER_BLOCK,                /* number of bytes per block */
            NUM_BLOCKS * BYTES_PER_BLOCK,   /* number of bytes on this device */
            FALSE,
            BLK_DEV_NAME
            );

    if (deviceId == NULLDEV)
        {
        printf("usbTargRbcBlockDevCreated:"
               "ramDiskDevCreate returned NULL\n");

        return(ERROR);
        }

    printf("OK. Trying to format RAM disk %s as dosFs\n", BLK_DEV_NAME);

    /* format the RAM disk, ignore memory contents */

    if (dosFsVolFormat(BLK_DEV_NAME,
                       DOS_OPT_BLANK | DOS_OPT_QUIET | DOS_OPT_FAT16,
                       NULL) != OK)
        {
        printf("dosFsVolFormat failed\n");

        return ERROR;
        }

    /* save the pointer */

    g_rbcBlkDev = (pVOID)deviceId;

    return (OK);
    }

/*************************************************************************
*
* usbTargMsInit - initializes USB mass strorage functionality driver
*
* This function initializes the peripheral stack and attaches it with
* the included Target Controller with mass storage functionality.
*
* RETURNS: None
*
* ERRNO: none
*/

void usbTargMsInit (void)
    {
    STATUS retVal = ERROR;

    /* Create the RAM disk for use first */

    retVal = usbTargMsBlockDevCreate();

    if (retVal != OK)
        {
        printf("usbTargMsBlockDevCreate failed\n");

        return;
        }

    /* Create semaphore */
    g_rbcSync = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    if (g_rbcSync == SEM_ID_NULL)
         {
         printf ("usbTargMsInit: create semaphore failed\n");
         (void)usbTargMsDestroy();
         return;
         }

#ifdef INCLUDE_PDIUSBD12
    retVal = msInitD12();
#elif defined(INCLUDE_PHILIPS1582)
    retVal = msInit1582();
#elif defined(INCLUDE_NET2280)
    retVal = msInitNet2280();
#elif defined(INCLUDE_FSL_TCD)
    retVal = msInitFsl();
#elif defined(INCLUDE_AT91_USB_TARG)
    retVal = msInitAt91();
#endif

    return;
    }

/*************************************************************************
*
* usbTargMsDestroy - tears down USB mass strorage functionality driver
*
* RETURNS: OK or ERROR
*
* ERRNO: none
*/

STATUS usbTargMsDestroy()
    {
    STATUS stat=OK;
    FILE *fout = stdioFp(1);    /* stream for output (if any) */

    if (targInit == FALSE)
        {
        fprintf (fout,"Not initialized.\n");
        return OK;
        }

    if (usbTargDisable (msTargChannel) != OK)
        {
        fprintf (fout,"usbTargDisable() returned ERROR\n");
        stat=ERROR;
        }

    if (usbTargTcdDetach (msTargChannel) != OK)
        {
        fprintf (fout,"usbTargTcdDetach() returned ERROR\n");
        stat=ERROR;
        }

    if (usbTargShutdown () != OK)
        {
        fprintf (fout,"usbTargInitialize() returned OK\n");
        stat=ERROR;
        }

    if (g_rbcSync != SEM_ID_NULL)
        {
        (void)semDelete(g_rbcSync); 
        g_rbcSync = SEM_ID_NULL;
        }

    fprintf (fout, " MS TCD destroyed\n");

    targInit = FALSE;

    return (stat);
    }

#ifdef INCLUDE_PDIUSBD12

/*******************************************************************************
*
* msInitD12 - attaches peripheral stack to the PDIUSBD12 Target Controller
*
* This function initializes the peripheral stack and attaches it with the
* PDIUSBD12 Target Controller by calling function usbTargTcdAttach with
* appropiate parameters. The Callback table inforamtion is obtained prior
* to attachment process by calling funtion usbTargMsCallbackInfo.
* The TCD defined parameters are hard-coded.
* Base Address = 0x368
* IRQ          = 0x7
* DMA          = 0x3
*
* RETURNS: OK or ERROR if not able to attach to target controller
*
* ERRNO: none
*
*\NOMANUAL
*/

STATUS msInitD12
    (
    void
    )
    {
    USB_TCD_PDIUSBD12_PARAMS params;
    pUSB_TARG_CALLBACK_TABLE callbackTable;
    pVOID callbackParam;

    long ioBase = 0x368;
    long irq = 0x7;
    long dma = 0x3;

    FILE *fout = stdioFp(1);    /* stream for output (if any) */

    /* if already initialized, just show a warning */

    if (targInit)
        {
        fprintf (fout,"Already initialized.\n");
        return (OK);
        }

    usbTargMsCallbackInfo (&callbackTable, &callbackParam);

    /* Initialize usbTargLib */

    if (usbTargInitialize () == OK)
        fprintf (fout,"usbTargInitialize() returned OK\n");
    else
        {
        fprintf (fout,"usbTargInitialize() returned ERROR\n");
        return (ERROR);
        }

    targInit = TRUE;


    /* Attach the Philips TCD to usbTargLib */

    memset (&params, 0, sizeof (params));
    params.ioBase = ioBase;
    params.irq = (UINT16) irq;
    params.dma = (UINT16) dma;

    fprintf (fout,"Philips PDIUSBD12: ioBase = 0x%lx, irq = %d, dma = %d\n",
    ioBase,  irq, dma);

#ifdef USB1_1
    if (usbTargTcdAttach (usbTcdPdiusbd12EvalExec, (pVOID) &params,
                          callbackTable, callbackParam, &msTargChannel,
                          &numEndpoints, &pEndpoints) == OK)
#else
    if (usbTargTcdAttach (usbTcdPdiusbd12EvalExec, (pVOID) &params,
                          callbackTable, callbackParam, &msTargChannel) == OK)
#endif
        {
        fprintf(fout,"usbTargTcdAttach() returned OK\n");
        }
    else
        {
        fprintf (fout,"usbTargTcdAttach() returned ERROR\n");
        msTargChannel = (USB_TARG_CHANNEL)NULL;
        return (ERROR);
        }

    fprintf (fout, "targChannel = 0x%x\n", (UINT32) msTargChannel);
#ifdef USB1_1
    fprintf (fout, "numEndpoints = %d\n", numEndpoints);
    fprintf (fout, "pEndpoints = 0x%lx\n", (long) pEndpoints);
#endif

    if (usbTargEnable (msTargChannel) == OK)
        {
        fprintf (fout, "usbTargEnable() returned OK\n");
        targEnable = TRUE;
        }
    else
        fprintf (fout, "usbTargEnable() returned ERROR\n");

    return (OK);
    }

#endif

#ifdef INCLUDE_PHILIPS1582
/*******************************************************************************
*
* msInit1582 - attaches peripheral stack to the ISP1582 Target Controller
*
* This function initializes the peripheral stack and attaches it with the
* ISP1582 Target Controller by calling function usbTargTcdAttach with
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

STATUS msInit1582
    (
    void
    )
    {
    USB_TCD_ISP1582_PARAMS params;
    pUSB_TARG_CALLBACK_TABLE callbackTable;
    pVOID callbackParam;

    long ioBase = 0x0; /* TODO get from PCI header */
    long irq    = 0x0; /* TODO get from PCI header */
    long dma    = 0x0; /* TODO get from PCI header */

    FILE *fout = stdioFp(1);    /* stream for output (if any) */

    /* if already initialized, just show a warning */

    if (targInit)
        {
        fprintf (fout,"Already initialized.\n");
        return (OK);
        }

    usbTargMsCallbackInfo (&callbackTable, &callbackParam);

    /* Initialize usbTargLib */

    if (usbTargInitialize () == OK)
        fprintf (fout,"usbTargInitialize() returned OK\n");
    else
        {
        fprintf (fout,"usbTargInitialize() returned ERROR\n");
        return (ERROR);
        }

    targInit = TRUE;

    sysIsp1582PciInit (&ioBase,&irq);
    /* Attach the Philips TCD to usbTargLib */

    memset (&params, 0, sizeof (params));
    params.ioBase = (UINT32) ioBase;
    params.irq = (UINT16) irq;
    params.dma = (UINT16) dma;

    fprintf (fout,"Philips ISP1582: ioBase = 0x%lx, irq = %d, dma = %d\n",
    ioBase, (int) irq, (int) dma);

#ifdef USB1_1
    if (usbTargTcdAttach (usbTcdPdiusbd12EvalExec, (pVOID) &params,
                          callbackTable, callbackParam, &msTargChannel,
                          &numEndpoints, &pEndpoints) == OK)
#else
    if (usbTargTcdAttach (usbTcdIsp1582EvalExec, (pVOID) &params,
                          callbackTable, callbackParam, &msTargChannel) == OK)
#endif
        {
        fprintf(fout,"usbTargTcdAttach() returned OK\n");
        }
    else
        {
        fprintf (fout,"usbTargTcdAttach() returned ERROR\n");
        msTargChannel = (USB_TARG_CHANNEL)NULL;
        return (ERROR);
        }

    fprintf (fout, "targChannel = 0x%x\n", (UINT32) msTargChannel);

#ifdef USB1_1
    fprintf (fout, "numEndpoints = %d\n", numEndpoints);
    fprintf (fout, "pEndpoints = 0x%lx\n", (long) pEndpoints);
#endif

    if (usbTargEnable (msTargChannel) == OK)
        {
        fprintf (fout, "usbTargEnable() returned OK\n");
        targEnable = TRUE;
        }
    else
        fprintf (fout, "usbTargEnable() returned ERROR\n");

    return (OK);
    }
#endif

#ifdef INCLUDE_NET2280
/*******************************************************************************
*
* msInitNet2280 - attaches peripheral stack to the NET2280 Target Controller
*
* This function initializes the peripheral stack and attaches it with the
* NET2280 Target Controller by calling function usbTargTcdAttach with
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


STATUS msInitNet2280
    (
    void
    )
    {
    USB_TCD_NET2280_PARAMS paramsNET2280;
    pUSB_TARG_CALLBACK_TABLE callbackTable;
    pVOID callbackParam;

    FILE *fout = stdioFp(1);    /* stream for output (if any) */

    /* if already initialized, just show a warning */

    if (targInit)
        {
        fprintf (fout,"Already initialized.\n");
        return (OK);
        }


    usbTargMsCallbackInfo (&callbackTable, &callbackParam);

    /* Initialize usbTargLib */

    if (usbTargInitialize () == OK)
        fprintf (fout,"usbTargInitialize() returned OK\n");
    else
        {
        fprintf (fout,"usbTargInitialize() returned ERROR\n");
        return (ERROR);
        }

    targInit = TRUE;

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


    if (usbTargTcdAttach (usbTcdNET2280Exec, (pVOID) &paramsNET2280,
            callbackTable, callbackParam, &msTargChannel) != OK)
        {
        fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
        msTargChannel = (USB_TARG_CHANNEL)NULL;
        return ERROR;
        }

    if (usbTargEnable (msTargChannel) != OK)
        {
         fprintf (fout, "usbTargEnable() returned ERROR\n");
        return ERROR;
        }

    targEnable = TRUE;
    fprintf (fout, "NET2280 TCD initialized\n");

    return (OK);
    }
#endif

#ifdef INCLUDE_FSL_TCD
/*******************************************************************************
*
* msInitFsl - attaches peripheral stack to the FSL Target Controller
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

STATUS msInitFsl
    (
    void
    )
    {
    USB_TCD_FSL_PARAMS_TYPE paramsFsl;
    pUSB_TARG_CALLBACK_TABLE callbackTable;
    pVOID callbackParam;

#ifdef FSL_DR_DMA_SUPPORTED
    UINT16 dmaIrq = (UINT16)(FSL_DMA_IRQ); /* DMA IRQ */
    UINT16 msgIrq = (UINT16)(FSL_MSG_IRQ); /* Message/Doorbell IRQ */
#endif /* FSL_DR_DMA_SUPPORTED */

    FILE *fout = stdioFp(1);    /* stream for output (if any) */

    /* if already initialized, just show a warning */
    if (targInit)
        {
        fprintf (fout,"Already initialized.\n");
        return (OK);
        }

    usbTargMsCallbackInfo (&callbackTable, &callbackParam);

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

    /* Get the platform type */

    paramsFsl.platformType = FSL_USB_DR_DEVICE_PLATFORM_TYPE;

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

    if (usbTargTcdAttach (usbTcdFslExec, (pVOID) &paramsFsl,
                          callbackTable, callbackParam,
                          &msTargChannel) == OK)
        {
        fprintf(fout,"usbTargTcdAttach() returned OK\n");
        }
    else
        {
        fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
        msTargChannel = (USB_TARG_CHANNEL)NULL;
        return ERROR;
        }

    fprintf (fout, "targChannel = 0x%x\n", (UINT32) msTargChannel);

    if (usbTargEnable (msTargChannel) == OK)
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
* msInitAt91 - attaches peripheral stack to the AT91 Target Controller
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

STATUS msInitAt91
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

    usbTargMsCallbackInfo (&callbackTable, &callbackParam);

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
                          callbackTable, callbackParam, &msTargChannel) == OK)
        {
        fprintf(fout,"usbTargTcdAttach() returned OK\n");
        }
    else
        {
        fprintf (fout, "usbTargTcdAttach() returned ERROR\n");
        msTargChannel = (USB_TARG_CHANNEL)NULL;
        return ERROR;
        }

    fprintf (fout, "targChannel = 0x%x\n", msTargChannel);

    if (usbTargEnable (msTargChannel) == OK)
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

#endif /* __INCusrUsbTargMsInitc */
