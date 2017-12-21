/* pciConfigShow.c - Show routines of PCI bus(IO mapped) library */

/*
 * Copyright (c) 1995-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms of
 * an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01z,03mar09,rgo  Support PCI PROCESSOR class in pciConfigFuncShow(WIND00157117)
01z,15Mar07,tor  Correct MME / MMC in output
01z,06nov06,mdo  WIND00071100 - add the offset instead of bit-wise OR
01y,17oct05,mdo  Add pcie header dump
01x,15aug05,mdo  Fix warnings
01w,25jul05,mdo  Add bridge detection
01v,20jul05,mdo  Add Capabilities Pointer to structure.
01u,23jun05,mdo  Changing several PCI defines to common PCI defined names.

01t,08sep04,mdo  Documentation fixes for apigen
01s,04jun04,dgp  Update markup to apigen style
01r,26may04,mdo  Fix version comments so apigen generates proper man pages.
01q,12dec03,dlk  Correct pciDeviceShow's display of class code; also display
                 revision ID, and functions other than function 0.
01p,23jun03,m_h  Cardbus support
01o,13nov01,tor  Assorted compiler warnings.
01n,25oct01,tor  Add pciConfigTopoShow() and support functions
01m,26feb01,rcs  merge from tornado 3 version main/tor3_x/3
01l,05dec00,dat  merge from sustaining branch to tor2_0_x
01k,16nov00,dat  SPR 36081 driver documentation
01j,17aug00,dat  SPR 33788 DeviceShow does not scan all possible devices
01i,19may00,pai  removed INCLUDE_SHOW_ROUTINES build condition (SPR 27759).
01h,17mar98,tm   documentation cleanup; added drv/pci/.h and config.h includes
01g,11mar98,tm   renamed to pciConfigShow.c from pciIomapShow.c
                 added return to null init routine
01f,04mar98,tm   added include of pciHeaderDefs.h, pciIomapShow.h
01e,28mar97,mas  added IMPORT of pciConfigMech, include of dllLib.h; fixed class
                 display in pciDeviceShow() (SPR 8226).
01d,12jan97,hdn  changed member/variable name "vender" to "vendor".
01c,12jan97,hdn  changed member/variable name "class" to "classCode".
01b,14mar96,hdn  re-written.  changed parameters of the functions.
                 removed BIOS dependent codes.
01a,25feb95,bcs  written
*/


/*
DESCRIPTION

This module contains show routines to see all devices and bridges on the PCI bus.
This module works in conjunction with pciConfigLib.o.
There are two ways to find out an empty device.

\ml
\m  -
check Master Abort bit after the access.
\m  -
check whether the read value is 0xffff.
\me

It uses the second method, since I didn't see the Master Abort bit of
the host/PCI bridge changing.

INCLUDE FILES:
*/



#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dllLib.h>
#include <config.h>
#include <pciClass.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciConfigShow.h>
#include <drv/pci/pciHeaderDefs.h>


/* defines */


/* externs */

IMPORT int pciLibInitDone;
IMPORT int pciLibInitStatus;
IMPORT int pciConfigMech;


/* globals */


/* locals */


/* forward declarations */

LOCAL void pciDheaderPrint  (PCI_HEADER_DEVICE * pD);
LOCAL void pciBheaderPrint  (PCI_HEADER_BRIDGE * pB);
LOCAL void pciCBheaderPrint (PCI_HEADER_CB_BRIDGE * pCB);
LOCAL void capabShow        (UINT, UINT, UINT, UINT8 );


/*******************************************************************************
*
* pciConfigShowInit - initialize the show routines
*
* This routine is used to pull in all routines in this library.
*
* \NOMANUAL
*
* RETURNS: N/A
*
* ERRNO
*/

void pciConfigShowInit (void)
    {
    return;
    }

/*******************************************************************************
*
* pciDeviceShow - print information about PCI devices
*
* This routine prints information about the PCI devices on a given PCI bus
* segment (specified by <busNo>).
*
* RETURNS: OK, or ERROR if the library is not initialized.
*
* ERRNO
*/

STATUS pciDeviceShow
    (
    int busNo       /* bus number */
    )
    {
    int deviceNo;
    UINT16 vendorId;
    UINT16 deviceId;
    UINT32 classCode;
    UINT8 headerType;
    int func;

    if (pciLibInitStatus != OK)         /* sanity check */
        return (ERROR);

    if (pciConfigMech == PCI_MECHANISM_2 && PCI_MAX_DEV > 16)
        {
        printf ("Invalid configuration. PCI_MAX_DEV > 16, PCI mechanism #2\n");
        return ERROR;
        }

    printf ("Scanning functions of each PCI device on bus %d\n", busNo);
    printf ("Using configuration mechanism %d\n", pciConfigMech);
    printf ("bus       device    function  vendorID  deviceID  class/rev\n");

    for (deviceNo=0; deviceNo < PCI_MAX_DEV; deviceNo++)
        {
        for (func = 0; func < 8; func++)
            {
            /* avoid a special bus cycle */

            if ((deviceNo == 0x1f) && (func == 0x07))
                continue;

            pciConfigInWord (busNo, deviceNo, func, PCI_CFG_VENDOR_ID,
                             (UINT16 *)&vendorId);

            /*
             * There are two ways to find out an empty device.
             * 1. check Master Abort bit after the access.
             * 2. check whether the read value is 0xffff.
             * Since I didn't see the Master Abort bit of the host/PCI bridge
             * changing, I use the second method.
             */

            if (vendorId == 0xffff)
                {
                if (func == 0)
                    break; /* next device */
                continue;  /* next function */
                }

            pciConfigInWord (busNo, deviceNo, func, PCI_CFG_DEVICE_ID,
                             (UINT16 *)&deviceId);

            pciConfigInLong (busNo, deviceNo, func, PCI_CFG_REVISION,
                             &classCode);

            pciConfigInByte (busNo, deviceNo, func, PCI_CFG_HEADER_TYPE,
                             &headerType);

            printf ("%7d   %6d    %8d   0x%04x    0x%04x  0x%08x\n",
                    busNo, deviceNo, func, vendorId, deviceId, classCode);

            if (func == 0 && (headerType & PCI_HEADER_MULTI_FUNC) == 0)
                break; /* next device */
            }
        }

    return (OK);
    }

/*******************************************************************************
*
* pciHeaderShow - print a header of the specified PCI device
*
* This routine prints a header of the PCI device specified by busNo, deviceNo,
* and funcNo.
*
* RETURNS: OK, or ERROR if this library is not initialized.
*
* ERRNO
*/

STATUS pciHeaderShow
    (
    int busNo,      /* bus number */
    int deviceNo,   /* device number */
    int funcNo      /* function number */
    )
    {
    PCI_HEADER_DEVICE headerDevice;
    PCI_HEADER_BRIDGE headerBridge;
    PCI_HEADER_CB_BRIDGE headerCBBridge;
    PCI_HEADER_DEVICE * pD = &headerDevice;
    PCI_HEADER_BRIDGE * pB = &headerBridge;
    PCI_HEADER_CB_BRIDGE * pCB = &headerCBBridge;

    if (pciLibInitStatus != OK)         /* sanity check */
        return(ERROR);

    pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_HEADER_TYPE,
                     (UINT8 *)&pD->headerType);

    if (pD->headerType & 0x01)      /* PCI-to-PCI bridge */
        {
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_VENDOR_ID,
                         (UINT16 *)&pB->vendorId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_DEVICE_ID,
                         (UINT16 *)&pB->deviceId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_COMMAND,
                         (UINT16 *)&pB->command);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_STATUS,
                         (UINT16 *)&pB->status);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_REVISION,
                         (UINT8 *)&pB->revisionId);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_PROGRAMMING_IF,
                         (UINT8 *)&pB->progIf);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SUBCLASS,
                         (UINT8 *)&pB->subClass);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_CLASS,
                         (UINT8 *)&pB->classCode);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_CACHE_LINE_SIZE,
                         (UINT8 *)&pB->cacheLine);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_LATENCY_TIMER,
                         (UINT8 *)&pB->latency);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_HEADER_TYPE,
                         (UINT8 *)&pB->headerType);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_BIST,
                         (UINT8 *)&pB->bist);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_0,
                         (UINT32 *)&pB->base0);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_1,
                         (UINT32 *)&pB->base1);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_PRIMARY_BUS,
                         (UINT8 *)&pB->priBus);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SECONDARY_BUS,
                         (UINT8 *)&pB->secBus);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SUBORDINATE_BUS,
                         (UINT8 *)&pB->subBus);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SEC_LATENCY,
                         (UINT8 *)&pB->secLatency);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_IO_BASE,
                         (UINT8 *)&pB->ioBase);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_IO_LIMIT,
                         (UINT8 *)&pB->ioLimit);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_SEC_STATUS,
                         (UINT16 *)&pB->secStatus);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_MEM_BASE,
                         (UINT16 *)&pB->memBase);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_MEM_LIMIT,
                         (UINT16 *)&pB->memLimit);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_PRE_MEM_BASE,
                         (UINT16 *)&pB->preBase);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_PRE_MEM_LIMIT,
                         (UINT16 *)&pB->preLimit);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_PRE_MEM_BASE_U,
                         (UINT32 *)&pB->preBaseUpper);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_PRE_MEM_LIMIT_U,
                         (UINT32 *)&pB->preLimitUpper);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_IO_BASE_U,
                         (UINT16 *)&pB->ioBaseUpper);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_IO_LIMIT_U,
                         (UINT16 *)&pB->ioLimitUpper);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_ROM_BASE,
                         (UINT32 *)&pB->romBase);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_BRG_INT_LINE,
                         (UINT8 *)&pB->intLine);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_BRG_INT_PIN,
                         (UINT8 *)&pB->intPin);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_BRIDGE_CONTROL,
                         (UINT16 *)&pB->control);
        pciBheaderPrint (pB);

        if (pB->status & PCI_STATUS_NEW_CAP)
            capabShow (busNo, deviceNo, funcNo, PCI_CFG_CAP_PTR);

        }
    else if ((pD->headerType & PCI_HEADER_TYPE_MASK) == PCI_HEADER_PCI_CARDBUS)
        {
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_VENDOR_ID,
                         (UINT16 *)&pCB->vendorId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_DEVICE_ID,
                         (UINT16 *)&pCB->deviceId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_COMMAND,
                         (UINT16 *)&pCB->command);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_STATUS,
                         (UINT16 *)&pCB->status);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_REVISION,
                         (UINT8 *)&pCB->revisionId);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_PROGRAMMING_IF,
                         (UINT8 *)&pCB->progIf);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SUBCLASS,
                         (UINT8 *)&pCB->subClass);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_CLASS,
                         (UINT8 *)&pCB->classCode);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_CACHE_LINE_SIZE,
                         (UINT8 *)&pCB->cacheLine);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_LATENCY_TIMER,
                         (UINT8 *)&pCB->latency);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_HEADER_TYPE,
                         (UINT8 *)&pCB->headerType);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_BIST,
                         (UINT8 *)&pCB->bist);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_0,
                         (UINT32 *)&pCB->base0);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_CB_CAP_PTR,
                         (UINT8 *)&pCB->capPtr);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_CB_SEC_STATUS,
                         (UINT16 *)&pCB->secStatus);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_PRIMARY_BUS,
                         (UINT8 *)&pCB->priBus);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SECONDARY_BUS,
                         (UINT8 *)&pCB->secBus);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SUBORDINATE_BUS,
                         (UINT8 *)&pCB->subBus);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SEC_LATENCY,
                         (UINT8 *)&pCB->secLatency);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_MEM_BASE_0,
                         (UINT32 *)&pCB->memBase0);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_MEM_LIMIT_0,
                         (UINT32 *)&pCB->memLimit0);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_MEM_BASE_1,
                         (UINT32 *)&pCB->memBase1);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_MEM_LIMIT_1,
                         (UINT32 *)&pCB->memLimit1);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_IO_BASE_0,
                         (UINT32 *)&pCB->ioBase0);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_IO_LIMIT_0,
                         (UINT32 *)&pCB->ioLimit0);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_IO_BASE_1,
                         (UINT32 *)&pCB->ioBase1);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_IO_LIMIT_1,
                         (UINT32 *)&pCB->ioLimit1);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_BRG_INT_LINE,
                         (UINT8 *)&pCB->intLine);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_BRG_INT_PIN,
                         (UINT8 *)&pCB->intPin);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_BRIDGE_CONTROL,
                         (UINT16 *)&pCB->control);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_CB_SUB_VENDOR_ID,
                         (UINT16 *)&pCB->subVendorId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_CB_SUB_SYSTEM_ID,
                         (UINT16 *)&pCB->subSystemId);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CB_16BIT_LEGACY,
                         (UINT32 *)&pCB->legacyBase);
        pciCBheaderPrint (pCB);

        if (pCB->status & PCI_STATUS_NEW_CAP)
            capabShow (busNo, deviceNo, funcNo, PCI_CFG_CB_CAP_PTR);

        }
    else                    /* PCI device */
        {
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_VENDOR_ID,
                         (UINT16 *)&pD->vendorId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_DEVICE_ID,
                         (UINT16 *)&pD->deviceId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_COMMAND,
                         (UINT16 *)&pD->command);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_STATUS,
                         (UINT16 *)&pD->status);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_REVISION,
                         (UINT8 *)&pD->revisionId);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_PROGRAMMING_IF,
                         (UINT8 *)&pD->progIf);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_SUBCLASS,
                         (UINT8 *)&pD->subClass);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_CLASS,
                         (UINT8 *)&pD->classCode);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_CACHE_LINE_SIZE,
                         (UINT8 *)&pD->cacheLine);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_LATENCY_TIMER,
                         (UINT8 *)&pD->latency);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_HEADER_TYPE,
                         (UINT8 *)&pD->headerType);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_BIST,
                         (UINT8 *)&pD->bist);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_0,
                         (UINT32 *)&pD->base0);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_1,
                         (UINT32 *)&pD->base1);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_2,
                         (UINT32 *)&pD->base2);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_3,
                         (UINT32 *)&pD->base3);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_4,
                         (UINT32 *)&pD->base4);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_5,
                         (UINT32 *)&pD->base5);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_CIS,
                         (UINT32 *)&pD->cis);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_SUB_VENDER_ID,
                         (UINT16 *)&pD->subVendorId);
        pciConfigInWord (busNo, deviceNo, funcNo, PCI_CFG_SUB_SYSTEM_ID,
                         (UINT16 *)&pD->subSystemId);
        pciConfigInLong (busNo, deviceNo, funcNo, PCI_CFG_EXPANSION_ROM,
                         (UINT32 *)&pD->romBase);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_DEV_INT_LINE,
                         (UINT8 *)&pD->intLine);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_DEV_INT_PIN,
                         (UINT8 *)&pD->intPin);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_MIN_GRANT,
                         (UINT8 *)&pD->minGrant);
        pciConfigInByte (busNo, deviceNo, funcNo, PCI_CFG_MAX_LATENCY,
                         (UINT8 *)&pD->maxLatency);
        pciDheaderPrint (pD);

        if (pD->status & PCI_STATUS_NEW_CAP)
            capabShow (busNo, deviceNo, funcNo, PCI_CFG_CAP_PTR);
        }

    return (OK);
    }

/*******************************************************************************
*
* pciFindDeviceShow - find a PCI device and display the information
*
* This routine finds a device by deviceId, then displays the information.
*
* RETURNS: OK, or ERROR if this library is not initialized.
*
* ERRNO
*/

STATUS pciFindDeviceShow
    (
    int vendorId,   /* vendor ID */
    int deviceId,   /* device ID */
    int index       /* desired instance of device */
    )
    {
    int busNo;
    int deviceNo;
    int funcNo;

    if (pciFindDevice (vendorId, deviceId, index, &busNo, &deviceNo, &funcNo)
        == OK)
        {
        printf ("deviceId = 0x%.8x\n", deviceId);
        printf ("vendorId = 0x%.8x\n", vendorId);
        printf ("index =    0x%.8x\n", index);
        printf ("busNo =    0x%.8x\n", busNo);
        printf ("deviceNo = 0x%.8x\n", deviceNo);
        printf ("funcNo =   0x%.8x\n", funcNo);
        return(OK);
        }
    return (ERROR);
    }

/*******************************************************************************
*
* pciFindClassShow - find a device by 24-bit class code
*
* This routine finds a device by its 24-bit PCI class code, then prints its
* information.
*
* RETURNS: OK, or ERROR if this library is not initialized.
*
* ERRNO
*/

STATUS pciFindClassShow
    (
    int classCode,  /* 24-bit class code */
    int index       /* desired instance of device */
    )
    {
    int busNo;
    int deviceNo;
    int funcNo;

    if (pciFindClass (classCode, index, &busNo, &deviceNo, &funcNo) == OK)
        {
        printf ("class code = 0x%.8x\n", classCode);
        printf ("index =      0x%.8x\n", index);
        printf ("busNo =      0x%.8x\n", busNo);
        printf ("deviceNo =   0x%.8x\n", deviceNo);
        printf ("funcNo =     0x%.8x\n", funcNo);
        return(OK);
        }
    return (ERROR);
    }

/*******************************************************************************
*
* pciDheaderPrint - print a PCI device header
*
* This routine prints a PCI device header.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pciDheaderPrint
    (
    PCI_HEADER_DEVICE * pD
    )
    {
    printf ("vendor ID =                   0x%.4x\n", (ushort_t)pD->vendorId);
    printf ("device ID =                   0x%.4x\n", (ushort_t)pD->deviceId);
    printf ("command register =            0x%.4x\n", (ushort_t)pD->command);
    printf ("status register =             0x%.4x\n", (ushort_t)pD->status);
    printf ("revision ID =                 0x%.2x\n", (uchar_t)pD->revisionId);
    printf ("class code =                  0x%.2x\n", (uchar_t)pD->classCode);
    printf ("sub class code =              0x%.2x\n", (uchar_t)pD->subClass);
    printf ("programming interface =       0x%.2x\n", (uchar_t)pD->progIf);
    printf ("cache line =                  0x%.2x\n", (uchar_t)pD->cacheLine);
    printf ("latency time =                0x%.2x\n", (uchar_t)pD->latency);
    printf ("header type =                 0x%.2x\n", (uchar_t)pD->headerType);
    printf ("BIST =                        0x%.2x\n", (uchar_t)pD->bist);
    printf ("base address 0 =              0x%.8x\n", pD->base0);
    printf ("base address 1 =              0x%.8x\n", pD->base1);
    printf ("base address 2 =              0x%.8x\n", pD->base2);
    printf ("base address 3 =              0x%.8x\n", pD->base3);
    printf ("base address 4 =              0x%.8x\n", pD->base4);
    printf ("base address 5 =              0x%.8x\n", pD->base5);
    printf ("cardBus CIS pointer =         0x%.8x\n", pD->cis);
    printf ("sub system vendor ID =        0x%.4x\n", (ushort_t)pD->subVendorId);
    printf ("sub system ID =               0x%.4x\n", (ushort_t)pD->subSystemId);
    printf ("expansion ROM base address =  0x%.8x\n", pD->romBase);
    printf ("interrupt line =              0x%.2x\n", (uchar_t)pD->intLine);
    printf ("interrupt pin =               0x%.2x\n", (uchar_t)pD->intPin);
    printf ("min Grant =                   0x%.2x\n", (uchar_t)pD->minGrant);
    printf ("max Latency =                 0x%.2x\n", (uchar_t)pD->maxLatency);
    }

/*******************************************************************************
*
* pciBheaderPrint - print a PCI-to-PCI bridge header
*
* This routine prints a PCI-to-PCI bridge header.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pciBheaderPrint
    (
    PCI_HEADER_BRIDGE * pB
    )
    {
    printf ("vendor ID =                   0x%.4x\n", (ushort_t)pB->vendorId);
    printf ("device ID =                   0x%.4x\n", (ushort_t)pB->deviceId);
    printf ("command register =            0x%.4x\n", (ushort_t)pB->command);
    printf ("status register =             0x%.4x\n", (ushort_t)pB->status);
    printf ("revision ID =                 0x%.2x\n", (uchar_t)pB->revisionId);
    printf ("class code =                  0x%.2x\n", (uchar_t)pB->classCode);
    printf ("sub class code =              0x%.2x\n", (uchar_t)pB->subClass);
    printf ("programming interface =       0x%.2x\n", (uchar_t)pB->progIf);
    printf ("cache line =                  0x%.2x\n", (uchar_t)pB->cacheLine);
    printf ("latency time =                0x%.2x\n", (uchar_t)pB->latency);
    printf ("header type =                 0x%.2x\n", (uchar_t)pB->headerType);
    printf ("BIST =                        0x%.2x\n", (uchar_t)pB->bist);
    printf ("base address 0 =              0x%.8x\n", pB->base0);
    printf ("base address 1 =              0x%.8x\n", pB->base1);
    printf ("primary bus number =          0x%.2x\n", (uchar_t)pB->priBus);
    printf ("secondary bus number =        0x%.2x\n", (uchar_t)pB->secBus);
    printf ("subordinate bus number =      0x%.2x\n", (uchar_t)pB->subBus);
    printf ("secondary latency timer =     0x%.2x\n", (uchar_t)pB->secLatency);
    printf ("IO base =                     0x%.2x\n", (uchar_t)pB->ioBase);
    printf ("IO limit =                    0x%.2x\n", (uchar_t)pB->ioLimit);
    printf ("secondary status =            0x%.4x\n", (ushort_t)pB->secStatus);
    printf ("memory base =                 0x%.4x\n", (ushort_t)pB->memBase);
    printf ("memory limit =                0x%.4x\n", (ushort_t)pB->memLimit);
    printf ("prefetch memory base =        0x%.4x\n", (ushort_t)pB->preBase);
    printf ("prefetch memory limit =       0x%.4x\n", (ushort_t)pB->preLimit);
    printf ("prefetch memory base upper =  0x%.8x\n", pB->preBaseUpper);
    printf ("prefetch memory limit upper = 0x%.8x\n", pB->preLimitUpper);
    printf ("IO base upper 16 bits =       0x%.4x\n", (ushort_t)pB->ioBaseUpper);
    printf ("IO limit upper 16 bits =      0x%.4x\n", (ushort_t)pB->ioLimitUpper);
    printf ("expansion ROM base address =  0x%.8x\n", pB->romBase);
    printf ("interrupt line =              0x%.2x\n", (uchar_t)pB->intLine);
    printf ("interrupt pin =               0x%.2x\n", (uchar_t)pB->intPin);
    printf ("bridge control =              0x%.4x\n", (ushort_t)pB->control);
    }

/*******************************************************************************
*
* pciCBheaderPrint - print a PCI-to-CardBus bridge header
*
* This routine prints a PCI-to-CardBus bridge header.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pciCBheaderPrint
    (
    PCI_HEADER_CB_BRIDGE * pCB
    )
    {
    printf ("vendor ID =                   0x%.4x\n", (ushort_t)pCB->vendorId);
    printf ("device ID =                   0x%.4x\n", (ushort_t)pCB->deviceId);
    printf ("command register =            0x%.4x\n", (ushort_t)pCB->command);
    printf ("status register =             0x%.4x\n", (ushort_t)pCB->status);
    printf ("revision ID =                 0x%.2x\n", (uchar_t)pCB->revisionId);
    printf ("class code =                  0x%.2x\n", (uchar_t)pCB->classCode);
    printf ("sub class code =              0x%.2x\n", (uchar_t)pCB->subClass);
    printf ("programming interface =       0x%.2x\n", (uchar_t)pCB->progIf);
    printf ("cache line =                  0x%.2x\n", (uchar_t)pCB->cacheLine);
    printf ("latency time =                0x%.2x\n", (uchar_t)pCB->latency);
    printf ("header type =                 0x%.2x\n", (uchar_t)pCB->headerType);
    printf ("BIST =                        0x%.2x\n", (uchar_t)pCB->bist);
    printf ("base address 0 =              0x%.8x\n", pCB->base0);
    printf ("capabilities pointer =        0x%.2x\n", (uchar_t)pCB->capPtr);
    printf ("secondary status =            0x%.4x\n", (ushort_t)pCB->secStatus);
    printf ("primary bus number =          0x%.2x\n", (uchar_t)pCB->priBus);
    printf ("secondary bus number =        0x%.2x\n", (uchar_t)pCB->secBus);
    printf ("subordinate bus number =      0x%.2x\n", (uchar_t)pCB->subBus);
    printf ("secondary latency timer =     0x%.2x\n", (uchar_t)pCB->secLatency);
    printf ("memory base 0 =               0x%.8x\n", pCB->memBase0);
    printf ("memory limit 0 =              0x%.8x\n", pCB->memLimit0);
    printf ("memory base 1 =               0x%.8x\n", pCB->memBase1);
    printf ("memory limit 1 =              0x%.8x\n", pCB->memLimit1);
    printf ("IO base 0 =                   0x%.8x\n", pCB->ioBase0);
    printf ("IO limit 0 =                  0x%.8x\n", pCB->ioLimit0);
    printf ("IO base 1 =                   0x%.8x\n", pCB->ioBase1);
    printf ("IO limit 1 =                  0x%.8x\n", pCB->ioLimit1);
    printf ("interrupt line =              0x%.2x\n", (uchar_t)pCB->intLine);
    printf ("interrupt pin =               0x%.2x\n", (uchar_t)pCB->intPin);
    printf ("bridge control =              0x%.4x\n", (ushort_t)pCB->control);
    printf ("sub system vendor ID =        0x%.4x\n", (ushort_t)pCB->subVendorId);
    printf ("sub system ID =               0x%.4x\n", (ushort_t)pCB->subSystemId);
    printf ("16 bit legacy mode base =     0x%.8x\n", pCB->legacyBase);
    }

/*********************************************************************
*
* pciFuncBarShow - show the value contained in a BAR
*
* This function decodes the value of a single BAR on a single
* bus, device, function, and displays the information.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pciFuncBarShow
    (
    int bus,            /* bus */
    int device,         /* device */
    int function,       /* function */
    int barNo           /* BAR index */
    )
    {
    UINT32 barVal;
    int space;
    int prefetch;
    int offset;

    offset = PCI_CFG_BASE_ADDRESS_0 + (barNo * sizeof(UINT32));
    pciConfigInLong(bus, device, function,
                    offset, &barVal);

    /* check if BAR implemented */
    if ( barVal == 0 || barVal == 0xffffffff )
        return;

    if ( ( barVal & 0x01 ) == 0x01 )
        printf("\tbar%d in I/O space @ 0x%08x\n",
               barNo, (barVal & (~0x00000001)));
    else
        {
        prefetch = ( barVal >> 3 ) & 0x01;
        space = (barVal >> 1 ) & 0x03;
        barVal = barVal & ~ 0x0f;
        printf("\tbar%d in %s%s mem space @ 0x%08x\n", barNo,
               prefetch ? "prefetchable " : "",
               ( space == 0x00 ) ? "32-bit" :
               ( space == 0x02) ? "64-bit" :
               "reserved",
               barVal);
        }
    }

/*********************************************************************
*
* pciStatusWordShow - show the decoded value of the status word
*
* This routine reads the status word from the specified
* bus, device, function, and displays the information.
*
* RETURNS: OK, always.
*
* ERRNO
*/

LOCAL STATUS pciStatusWordShow
    (
    int bus,        /* bus */
    int device,     /* device */
    int function    /* function */
    )
    {
    UINT16 status;

    pciConfigInWord (bus, device, function, PCI_CFG_STATUS, &status);

    printf("\tstatus=0x%04x (", status);

    if ( status & PCI_STATUS_NEW_CAP )
        printf(" CAP");

    if ( status & PCI_STATUS_66_MHZ )
        printf(" 66MHZ");

    if ( status & PCI_STATUS_UDF )
        printf(" UDF");

    if ( status & PCI_STATUS_FAST_BB )
        printf(" FBTB");

    if ( status & PCI_STATUS_DATA_PARITY_ERR )
        printf(" DATA_PARITY_ERR");

    printf(" DEVSEL=%x", ((status & 0x0600) >> 9 ));

    if ( status & PCI_STATUS_TARGET_ABORT_GEN )
        printf(" TGT_ABORT_GEN");

    if ( status & PCI_STATUS_TARGET_ABORT_RCV )
        printf(" TGT_ABORT_RCV");

    if ( status & PCI_STATUS_MASTER_ABORT_RCV )
        printf(" MSTR_ABORT_RCV");

    if ( status & PCI_STATUS_ASSERT_SERR )
        printf(" ASSERT_SERR");

    if ( status & PCI_STATUS_PARITY_ERROR )
        printf(" PARITY_ERR");

    printf(" )\n");

    return(OK);
    }

/*********************************************************************
*
* pciCmdWordShow - show the decoded value of the command word
*
* This routine reads the command word from the specified
* bus, device, function, and displays the information.
*
* RETURNS: OK, always.
*
* ERRNO
*/

LOCAL STATUS pciCmdWordShow
    (
    int bus,        /* bus */
    int device,     /* device */
    int function    /* function */
    )
    {
    UINT16 command;

    pciConfigInWord (bus,device,function,
                     PCI_CFG_COMMAND, &command);

    printf("\tcommand=0x%04x (", command);

    if ( command & PCI_CMD_IO_ENABLE )
        printf(" IO_ENABLE");

    if ( command & PCI_CMD_MEM_ENABLE )
        printf(" MEM_ENABLE");

    if ( command & PCI_CMD_MASTER_ENABLE )
        printf(" MASTER_ENABLE");

    if ( command & PCI_CMD_MON_ENABLE )
        printf(" MON_ENABLE");

    if ( command & PCI_CMD_WI_ENABLE )
        printf(" WI_ENABLE");

    if ( command & PCI_CMD_SNOOP_ENABLE )
        printf(" SNOOP_ENABLE");

    if ( command & PCI_CMD_PERR_ENABLE )
        printf(" PERR_ENABLE");

    if ( command & PCI_CMD_WC_ENABLE )
        printf(" WC_ENABLE");

    if ( command & PCI_CMD_SERR_ENABLE )
        printf(" SERR_ENABLE");

    if ( command & PCI_CMD_FBTB_ENABLE )
        printf(" FBTB_ENABLE");

    printf(" )\n");
    return(OK);
    }

/*********************************************************************
*
* pciConfigStatusWordShow - show the decoded value of the status word
*
* This routine reads the value of the status word for the specified
* bus, device, function and displays the information.
*
* RETURNS: OK, always.
*
* ERRNO
*/

STATUS pciConfigStatusWordShow
    (
    int bus,        /* bus */
    int device,     /* device */
    int function,   /* function */
    void *pArg      /* ignored */
    )
    {
    printf("[%d,%d,%d] ",bus,device,function);
    pciStatusWordShow(bus,device,function);
    return(OK);
    }

/*********************************************************************
*
* pciConfigCmdWordShow - show the decoded value of the command word
*
* This routine reads the value of the command word for the specified
* bus, device, function and displays the information.
*
* RETURNS: OK, always.
*
* ERRNO
*/

STATUS pciConfigCmdWordShow
    (
    int bus,        /* bus */
    int device,     /* device */
    int function,   /* function */
    void *pArg      /* ignored */
    )
    {
    printf("[%d,%d,%d] ",bus,device,function);
    pciCmdWordShow(bus,device,function);
    return(OK);
    }

/*********************************************************************
*
* pciConfigFuncShow - show configuration details about a function
*
* This routine reads various information from the specified
* bus, device, function, and displays the information.
*
* RETURNS: OK, always.
*
* ERRNO
*/

STATUS pciConfigFuncShow
    (
    int bus,        /* bus */
    int device,     /* device */
    int function,   /* function */
    void *pArg      /* ignored */
    )
    {
    UINT8  clsCode;
    UINT8  subClass;
    UINT8  secBus;
    int    numBars = 6; /* most devices have 6, but not bridges */
    UINT16 memBase;
    UINT16 memLimit;
    UINT32 memBaseU;
    UINT32 memLimitU;
    UINT8  ioBase;
    UINT8  ioLimit;
    UINT16 ioBaseU;
    UINT16 ioLimitU;
    UINT8  headerType;
    UINT16 cmdReg;
    int i;

    printf("[%d,%d,%d] type=",bus,device,function);
    pciConfigInByte (bus, device, function,
                     PCI_CFG_CLASS, &clsCode);

    pciConfigInByte(bus,device,function,
                    PCI_CFG_HEADER_TYPE, &headerType);

    if ( ( headerType & PCI_HEADER_TYPE_MASK ) == 1 )
        {
        /* type 1 header has only 2 BARs */
        numBars = 2;
        }

    if ((headerType & PCI_HEADER_TYPE_MASK) == PCI_HEADER_PCI_CARDBUS)
        {
        /* CardBus bridge header has 1 BAR */
        numBars = 1;
        }

    switch (clsCode)
        {
        case PCI_CLASS_PRE_PCI20:     printf("BEFORE_STD\n"); break;
        case PCI_CLASS_MASS_STORAGE:  printf("MASS STORAGE\n"); break;
        case PCI_CLASS_NETWORK_CTLR:  printf("NET_CNTLR\n"); break;
        case PCI_CLASS_DISPLAY_CTLR:  printf("DISP_CNTLR\n"); break;
        case PCI_CLASS_MMEDIA_DEVICE: printf("MULTI_MEDIA\n"); break;
        case PCI_CLASS_MEM_CTLR:      printf("MEM_CNTLR\n"); break;
        case PCI_CLASS_COMM_CTLR:     printf("COMMUNICATION\n"); break;
        case PCI_CLASS_BASE_PERIPH:   printf("PERIPHERAL\n"); break;
        case PCI_CLASS_INPUT_DEVICE:  printf("INPUT\n"); break;
        case PCI_CLASS_DOCK_DEVICE:   printf("DOCKING STATION\n"); break;
        case PCI_CLASS_SERIAL_BUS:    printf("SERIAL BUS\n"); break;
        case PCI_CLASS_WIRELESS:      printf("WIRELESS\n"); break;
        case PCI_CLASS_INTLGNT_IO:    printf("INTELLIGENT_IO\n"); break;
        case PCI_CLASS_SAT_COMM:      printf("SATELLITE\n"); break;
        case PCI_CLASS_EN_DECRYPTION: printf("ENCRYPTION DEV\n"); break;
        case PCI_CLASS_DAQ_DSP:       printf("DATA ACQUISITION DEV\n"); break;
        case PCI_CLASS_UNDEFINED:     printf("OTHER DEVICE\n"); break;

        case PCI_CLASS_PROCESSOR:     
	    if ((headerType & PCI_HEADER_TYPE_MASK) != PCI_HEADER_TYPE_BRIDGE)
		{
		printf("PROCESSOR\n"); 
		break;
		}
	    else
		{
	        secBus = 0;	
	      	pciConfigInByte (bus, device, function,
				 PCI_CFG_SUBCLASS, &subClass);
		    
		switch (subClass)
		    {
		    case PCI_SUBCLASS_PROCESSOR_POWERPC:
			printf("PowerPC");
			pciConfigInByte (bus, device, function,
			  		 PCI_CFG_SECONDARY_BUS, &secBus);
			break;

		    default:
			printf("UNKNOWN (0x%02x)", subClass);
			break;
		    }

		printf(" PROCESSOR"); 

		if ( secBus != 0 )
		    {
		    printf(" to [%d,0,0]", secBus); 
		    printf("\n");

		    pciConfigInWord(bus,device,function,
				    PCI_CFG_COMMAND, &cmdReg);

		    if ( cmdReg & PCI_CMD_MEM_ENABLE )
			{
			pciConfigInWord(bus,device,function,
			    		PCI_CFG_MEM_BASE, &memBase);
			pciConfigInWord(bus,device,function,
			    		PCI_CFG_MEM_LIMIT, &memLimit);
			printf("\tbase/limit:\n");
			printf("\t  mem=   0x%04x0000/0x%04xffff\n",
			       memBase & 0xfff0, memLimit | 0x000f);
			    
			pciConfigInWord(bus,device,function,
			    		PCI_CFG_PRE_MEM_BASE, &memBase);
			pciConfigInWord(bus,device,function,
			   		PCI_CFG_PRE_MEM_LIMIT, &memLimit);
			if ( ( memBase & 0x000f ) == 0x0001 )
			    {
			    /* 64-bit memory */
			    pciConfigInLong(bus,device,function,
					    PCI_CFG_PRE_MEM_BASE_U,
				    	    &memBaseU);
			    pciConfigInLong(bus,device,function,
					    PCI_CFG_PRE_MEM_LIMIT_U,
				    	    &memLimitU);
			    printf("\t  preMem=0x%08x%04x0000/"
			     	   "0x%08x%04xffff\n",
			      	   memBaseU, memBase & 0xfff0, 
			       	   memLimitU, memLimit | 0x000f);
			    }
			else
			    printf("\t  preMem=0x%04x0000/0x%04xffff\n",
			     	   memBase & 0xfff0, memLimit | 0x000f);
			}

		    if ( cmdReg & PCI_CMD_IO_ENABLE )
			{
			pciConfigInByte(bus,device,function,
			    		PCI_CFG_IO_BASE, &ioBase);
			pciConfigInByte(bus,device,function,
			    		PCI_CFG_IO_LIMIT, &ioLimit);
			if ( ( ioBase & 0x0f ) == 0x01 )
			    {
			    /* 32-bit I/O */
			    pciConfigInWord(bus,device,function,
					    PCI_CFG_IO_BASE_U, &ioBaseU);
			    pciConfigInWord(bus,device,function,
					    PCI_CFG_IO_LIMIT_U, &ioLimitU);
			    printf("\t  I/O=   0x%04x%02x00/0x%04x%02xff\n",
			     	   ioBaseU, (ioBase & 0xf0),
			      	   ioLimitU, (ioLimit | 0x0f));
			    }
			else
			    {
			    printf("\t  I/O=   0x%02x00/0x%02xff\n",
				   (ioBase & 0xf0), (ioLimit | 0x0f));
			    }
			}
		    }
		else
		    {
		    printf("\n");
		    }
		}
		break;

        case PCI_CLASS_BRIDGE_CTLR:
            secBus = 0;
            pciConfigInByte (bus, device, function,
                             PCI_CFG_SUBCLASS, &subClass);

            switch (subClass)
                {
                case PCI_SUBCLASS_HOST_PCI_BRIDGE:
                    printf("HOST");
                    break;

                case PCI_SUBCLASS_ISA_BRIDGE:
                    printf("ISA");
                    break;

                case PCI_SUBCLASS_EISA_BRIDGE:
                    printf("EISA");
                    break;

                case PCI_SUBCLASS_MCA_BRIDGE:
                    printf("MC");
                    break;

                case PCI_SUBCLASS_P2P_BRIDGE:
                    printf("P2P");
                    pciConfigInByte (bus, device, function,
                                     PCI_CFG_SECONDARY_BUS, &secBus);
                    break;

                case PCI_SUBCLASS_PCMCIA_BRIDGE:
                    printf("PCMCIA");
                    break;

                case PCI_SUBCLASS_CARDBUS_BRIDGE:
                    printf("CARDBUS");
                    pciConfigInByte (bus, device, function,
                                     PCI_CFG_SECONDARY_BUS, &secBus);
                    break;

                case PCI_SUBCLASS_RACEWAY_BRIDGE:
                    printf("RACEWAY");
                    break;

                default:
                    printf("UNKNOWN (0x%02x)", subClass);
                    break;
                }

            printf(" BRIDGE");
            if ( secBus != 0 )
                {
                printf(" to [%d,0,0]", secBus);
                printf("\n");

                pciConfigInWord(bus,device,function,
                                PCI_CFG_COMMAND, &cmdReg);

                if (subClass == PCI_SUBCLASS_CARDBUS_BRIDGE)
                    {
                    UINT32 memBase;
                    UINT32 memLimit;
                    UINT32 ioBase;
                    UINT32 ioLimit;

                    printf ("\tbase/limit:\n");

                    if (cmdReg & PCI_CMD_MEM_ENABLE)
                        {
                        for (i = 0; i < 2; i++)
                            {
                            pciConfigInLong (bus, device, function,
                                             PCI_CFG_CB_MEM_BASE_0 + i * 8,
                                             &memBase);
                            pciConfigInLong (bus, device, function,
                                             PCI_CFG_CB_MEM_LIMIT_0 + i * 8,
                                             &memLimit);
                            printf ("\t  mem%d=0x%08x/0x%08x\n", i,
                                    memBase, memLimit | 0x0fff);
                            }
                        }

                    if (cmdReg & PCI_CMD_IO_ENABLE)
                        {
                        for (i = 0; i < 2; i++)
                            {
                            pciConfigInLong (bus, device, function,
                                             PCI_CFG_CB_IO_BASE_0 + i * 8,
                                             &ioBase);
                            pciConfigInLong (bus, device, function,
                                             PCI_CFG_CB_IO_LIMIT_0 + i * 8,
                                             &ioLimit);
                            printf ("\t  I/O%d=0x%08x/0x%08x\n", i,
                                    ioBase, ioLimit);
                            }
                        }

                    break;
                    }

                if ( cmdReg & PCI_CMD_MEM_ENABLE )
                    {
                    pciConfigInWord(bus,device,function,
                                    PCI_CFG_MEM_BASE, &memBase);
                    pciConfigInWord(bus,device,function,
                                    PCI_CFG_MEM_LIMIT, &memLimit);
                    printf("\tbase/limit:\n");
                    printf("\t  mem=   0x%04x0000/0x%04xffff\n",
                           memBase & 0xfff0, memLimit | 0x000f);

                    pciConfigInWord(bus,device,function,
                                    PCI_CFG_PRE_MEM_BASE, &memBase);
                    pciConfigInWord(bus,device,function,
                                    PCI_CFG_PRE_MEM_LIMIT, &memLimit);
                    if ( ( memBase & 0x000f ) == 0x0001 )
                        {
                        /* 64-bit memory */
                        pciConfigInLong(bus,device,function,
                                        PCI_CFG_PRE_MEM_BASE_U,
                                        &memBaseU);
                        pciConfigInLong(bus,device,function,
                                        PCI_CFG_PRE_MEM_LIMIT_U,
                                        &memLimitU);
                        printf("\t  preMem=0x%08x%04x0000/"
                               "0x%08x%04xffff\n",
                               memBaseU, memBase & 0xfff0,
                               memLimitU, memLimit | 0x000f);
                        }
                    else
                        printf("\t  preMem=0x%04x0000/0x%04xffff\n",
                               memBase & 0xfff0, memLimit | 0x000f);
                    }

                if ( cmdReg & PCI_CMD_IO_ENABLE )
                    {
                    pciConfigInByte(bus,device,function,
                                    PCI_CFG_IO_BASE, &ioBase);
                    pciConfigInByte(bus,device,function,
                                    PCI_CFG_IO_LIMIT, &ioLimit);
                    if ( ( ioBase & 0x0f ) == 0x01 )
                        {
                        /* 32-bit I/O */
                        pciConfigInWord(bus,device,function,
                                        PCI_CFG_IO_BASE_U, &ioBaseU);
                        pciConfigInWord(bus,device,function,
                                        PCI_CFG_IO_LIMIT_U, &ioLimitU);
                        printf("\t  I/O=   0x%04x%02x00/0x%04x%02xff\n",
                               ioBaseU, (ioBase & 0xf0),
                               ioLimitU, (ioLimit | 0x0f));
                        }
                    else
                        printf("\t  I/O=   0x%02x00/0x%02xff\n",
                               (ioBase & 0xf0), (ioLimit | 0x0f));
                    }
                }
            else
                printf("\n");

            break;

        default:
            printf("UNKNOWN!\n");
        }

    pciStatusWordShow(bus, device, function);
    pciCmdWordShow(bus, device, function);

    for (i = 0; i < numBars; i++)
        pciFuncBarShow(bus, device, function, i);

    return(OK);
    }

/*********************************************************************
*
* pcixCapabShow - show the PCI-X capabilities
*
* This routine displays the PCI-X specific information.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void pcixCapabShow
    (
    UINT        bus,           /* bus */
    UINT        device,        /* device */
    UINT        function,       /* function */
    UINT        offset         /* offset of capability */
    )
    {
    UINT8 headerType;
    UINT16 cmd, status, secStat;
    UINT32 stat, upstr, dwnstr;

    pciConfigInByte (bus, device, function, PCI_CFG_HEADER_TYPE,
                     &headerType);

    switch (headerType)
        {
        case PCI_HEADER_TYPE0: /* Regular Device */
            {
            printf("PCI-X \n");
            pciConfigInWord(bus, device, function, offset+PCI_X_CMD, &cmd);
            pciConfigInLong(bus, device, function, offset+PCI_X_STATUS, &stat);

            printf("\tCommand: 0x%04x\n", cmd);
            printf("\tStatus:  0x%08x\n", stat);
            break;
            }
        case PCI_HEADER_TYPE_BRIDGE:
            {
            printf("PCI-X bridge \n");
            pciConfigInWord(bus, device, function, offset+PCI_X_SECSTAT, &secStat);
            pciConfigInWord(bus, device, function, offset+PCI_X_PRISTAT, &status);
            pciConfigInLong(bus, device, function, offset+PCI_X_UP_SXACTION_CTRL, &upstr);
            pciConfigInLong(bus, device, function, offset+PCI_X_DWN_SXACTION_CTRL, &dwnstr);

            printf("\tSecondary Status:   0x%04x\n", secStat);
            printf("\tPrimary Status:     0x%08x\n", status);
            printf("\tUpstream Control:   0x%08x\n", upstr);
            printf("\tDownstream Control: 0x%08x\n", dwnstr);
            break;
            }
        }
    }

/*********************************************************************
*
* msiCapabShow - show the MSI capabilities
*
* This routine displays the MSI specific information.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void msiCapabShow
    (
    UINT        bus,           /* bus */
    UINT        device,        /* device */
    UINT        function,      /* function */
    UINT        offset         /* offset of capability */
    )
    {
    UINT8 ctlReg;
    UINT16 data;
    UINT32 addr;

    pciConfigInByte (bus, device, function, offset+PCI_MSI_CTL, &ctlReg);

    printf("Message Signaled Interrupts: %s, %s, MME: %d MMC: %d\n",
           (ctlReg & PCI_MSI_CTL_ENABLE) == 0x00  ? "Disabled" : "Enabled",
           (ctlReg & PCI_MSI_CTL_64BIT)  == 0x00  ? "32-bit" : "64-bit",
           (ctlReg & PCI_MSI_CTL_MSG_ALLOC) >> 4,
           (ctlReg & PCI_MSI_CTL_MSG_MAX) >> 1);

        printf("\tAddress: ");
        if (ctlReg & PCI_MSI_CTL_64BIT)
          {
            pciConfigInLong(bus, device, function, offset+PCI_MSI_ADDR_HI, &addr);
            pciConfigInWord(bus, device, function, offset+PCI_MSI_DATA_64, &data);
            printf("%08x", addr);
          }
        else
            pciConfigInWord(bus, device, function, offset+PCI_MSI_DATA_32, &data);

        pciConfigInLong(bus, device, function, offset+PCI_MSI_ADDR_LO, &addr);
        printf("%08x  Data: 0x%04x\n", addr, data);

    }

LOCAL const char *latL0s
    (
    int latVal
    )
    {
      static const char *lat[] =
          {
          "<64ns",
          "<128ns",
          "<256ns",
          "<512ns",
          "<1us",
          "<2us",
          "<4us",
          ">4us"
           };
      return lat[latVal];
    }

LOCAL const char *latL1
    (
    int latVal
    )
    {
    static const char *lat[] =
        {
        "<1us",
        "<2us",
        "<4us",
        "<8us",
        "<16us",
        "<32us",
        "<64us",
        ">64us"
        };
    return lat[latVal];
    }

LOCAL float limScale
    (
    int scaleVal
    )
    {
    static const float scale[4] =
    {
    1.0,
    0.1,
    0.01,
    0.001
    };
    return scale[scaleVal];
    }

/*********************************************************************
*
* pcieDevCap - show the PCIe Device capabilities
*
* This routine displays the PCI Express Device capabilities
* information.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void pcieDevCap
    (
    UINT        bus,           /* bus */
    UINT        device,        /* device */
    UINT        function,      /* function */
    UINT        offset,        /* offset of capability */
    UINT        type           /* device type */
    )
    {
    UINT32 devCap;
    UINT16 devCtl;

    pciConfigInLong(bus, device, function, offset+PCI_EXP_DEVCAP_REG, &devCap);

    printf("\tDevice: Max Payload: %d bytes",
           128 << (devCap & PCI_EXP_DEVCAP_PAYLOAD));
    if (devCap & PCI_EXP_DEVCAP_PHANTOM)
        printf(", Phantom Funcs %d msb",
               (devCap & PCI_EXP_DEVCAP_PHANTOM) >> 3);
    printf(", Extended Tag: %d-bit\n",
           ((devCap & PCI_EXP_DEVCAP_EXT_TAG) >> 5) == 0 ? 5:8);

    printf("\t\tAcceptable Latency: L0 - %s, L1 - %s\n",
           latL0s((devCap & PCI_EXP_DEVCAP_L0S) >> 6),
           latL1((devCap & PCI_EXP_DEVCAP_L1) >> 9));

    if ((type == PCI_EXP_TYPE_ENDPOINT) ||
        (type == PCI_EXP_TYPE_LEG_END)  ||
        (type == PCI_EXP_TYPE_UPSTREAM) ||
        (type == PCI_EXP_TYPE_PCI2EXP))
        {
        if (devCap & PCI_EXP_DEVCAP_ATTN_BUTTON)
            printf("\tAttn Button");
        if (devCap & PCI_EXP_DEVCAP_ATTN_IND)
            printf(", Attn Indicator");
        if (devCap & PCI_EXP_DEVCAP_PWR_IND)
            printf(", Pwr Indicator\n");
        }

    if (type == PCI_EXP_TYPE_UPSTREAM)
        printf("\tSlot Power Limit (Watts) %f\n",
               ((devCap & PCI_EXP_DEVCAP_PWR_VAL_LIMIT) >> 18) *
                limScale((devCap & PCI_EXP_DEVCAP_PWR_SCL_LIMIT) >> 26));

    pciConfigInWord (bus, device, function, offset+PCI_EXP_DEVCTL_REG, &devCtl);

    printf("\t\tErrors Enabled: ");
    if (devCtl &  PCI_EXP_DEVCTL_CERR_ENB)
        printf(" Correctable");
    if (devCtl &  PCI_EXP_DEVCTL_NFERR_ENB)
        printf(" Non-Fatal");
    if (devCtl &  PCI_EXP_DEVCTL_FERR_ENB)
        printf(" Fatal");
    if (devCtl &  PCI_EXP_DEVCTL_URREP_ENB)
        printf(" Unsupported Request");
    if (devCtl &  PCI_EXP_DEVCTL_RLX_ORD_ENB)
        printf("Relaxed Ordering");
    if (devCtl &  PCI_EXP_DEVCTL_EXT_TAG)
        printf(" Extended Tag");
    if (devCtl &  PCI_EXP_DEVCTL_PHANTOM)
        printf(" Phantom Funcs");
    if (devCtl &  PCI_EXP_DEVCTL_AUX_PM_ENB)
        printf(" AUX Pwr PM");
    if (devCtl &  PCI_EXP_DEVCTL_NOSNOOP_ENB)
        printf(" No Snoop");

    printf("\n\t\t");

    if (devCtl & PCI_EXP_DEVCTL_PAYLOAD)
        printf("Max Payload %d bytes ",
               128 << ((devCtl & PCI_EXP_DEVCTL_PAYLOAD) >> 5));
    if (devCtl & PCI_EXP_DEVCTL_READ_REQ)
        printf("Max Read Request %d bytes\n",
               128 << ((devCtl & PCI_EXP_DEVCTL_READ_REQ) >> 12));

    }

LOCAL char *linkSpeed
    (
    UINT speed
    )
    {
    switch (speed)
        {
        case 1:
            return "2.5Gb/s";
        default:
            return "unknown";
        }
    }

LOCAL char *aspm
    (
    int val
    )
    {
    switch (val)
        {
        case 1:
            return "L0s";
        case 3:
            return "L0s & L1";
        default:
            return "unknown";
        }
    }

LOCAL const char *aspmEnable
    (
    int val
    )
    {
    static const char *enab[] =
        {
        "Disabled",
        "L0s",
        "L1",
        "L0s & L1"
        };
    return enab[val];
    }

/*********************************************************************
*
* pcieLinkCap - show the PCIe Link capabilities
*
* This routine displays the PCI Express Link capabilities
* information.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void pcieLinkCap
    (
    UINT        bus,           /* bus */
    UINT        device,        /* device */
    UINT        function,      /* function */
    UINT        offset,        /* offset of capability */
    UINT        type           /* device type */
    )
    {
    UINT32 lnkCap;
    UINT16 lnkVal;

    pciConfigInLong(bus, device, function, offset+PCI_EXP_LNKCAP_REG, &lnkCap);

    printf("\tLink: MAX Speed - %s, MAX Width - by %d Port - %d ASPM - %s\n",
           linkSpeed(lnkCap & PCI_EXP_LNKCAP_LNK_SPD),
           (lnkCap & PCI_EXP_LNKCAP_LNK_WDTH) >> 4,
           lnkCap >> 24,
           aspm((lnkCap & PCI_EXP_LNKCAP_ASPM)>>10) );

    printf("\t\tLatency: L0s - %s, L1 - %s\n",
           latL0s((lnkCap & PCI_EXP_LNKCAP_L0S_LAT) >> 12),
           latL1((lnkCap & PCI_EXP_LNKCAP_L1_LAT) >> 15));

    pciConfigInWord (bus, device, function, offset+PCI_EXP_LNKCTL_REG, &lnkVal);

    printf("\t\tASPM - %s,", aspmEnable(lnkVal & PCI_EXP_LNKCTL_ASPM));

    if ((type == PCI_EXP_TYPE_ROOT_PORT) ||
        (type == PCI_EXP_TYPE_ENDPOINT)  ||
        (type == PCI_EXP_TYPE_LEG_END))

        printf(" RCB - %dbytes", lnkVal & PCI_EXP_LNKCTL_RCB ? 128 : 64);

    if (lnkVal & PCI_EXP_LNKCTL_LNK_DIS)
        printf(" Link Disabled");

    if (lnkVal & PCI_EXP_LNKCTL_CCC)
        printf(" Common Clock");

    if (lnkVal & PCI_EXP_LNKCTL_EXT_SYNC)
        printf(" Extended Sync");

    printf("\n");

    pciConfigInWord (bus, device, function, offset+PCI_EXP_LNKSTA_REG, &lnkVal);
    printf("\t\tSpeed - %s, Width - by %d\n",
           linkSpeed(lnkVal & PCI_EXP_LNKSTA_LNK_SPD),
           (lnkVal & PCI_EXP_LNKSTA_LNK_WDTH) >> 4);
    }

LOCAL const char *indicator
    (
    int val
    )
    {
    static const char *state[] =
        {
        "Unknown",
        "On",
        "Blink",
        "Off"
        };
    return state[val];
    }

LOCAL void pcieSlotCap
    (
    UINT        bus,           /* bus */
    UINT        device,        /* device */
    UINT        function,      /* function */
    UINT        offset         /* offset of capability */
    )
    {
    UINT32 sltCap;
    UINT16 sltCtl;

    pciConfigInLong(bus, device, function, offset+PCI_EXP_SLTCAP_REG, &sltCap);

    printf("\tSlot:");
    if (sltCap & PCI_EXP_SLTCAP_ATTN_BUT)
        printf(" Attn Button");
    if (sltCap & PCI_EXP_SLTCAP_PWR_CTL)
        printf(" Power Controller");
    if (sltCap & PCI_EXP_SLTCAP_MRL)
        printf(" MRL Sensor");
    if (sltCap & PCI_EXP_SLTCAP_PWR_IND)
        printf(" Pwr Indicator");
    if (sltCap & PCI_EXP_SLTCAP_HPL_CAP)
        printf(" Hot-Plug");
    if (sltCap & PCI_EXP_SLTCAP_HPL_SUP)
        printf(" Hot-Plug Surprise");

    printf("\t\tSlot #%d, MAX Slot Power Limit (Watts) %f\n",
           sltCap >> 19,
           ((sltCap & PCI_EXP_SLTCAP_PWR_VAL_LIMIT) >> 7) *
           limScale((sltCap & PCI_EXP_SLTCAP_PWR_SCL_LIMIT) >> 15));

    pciConfigInWord(bus, device, function, offset+PCI_EXP_SLTCTL_REG, &sltCtl);

    printf("\tEnabled:");
    if (sltCtl & PCI_EXP_SLTCTL_ATTN_BUT_ENB)
        printf(" Attn Button Pressed");
    if (sltCtl & PCI_EXP_SLTCTL_PWRFLT_DET_ENB)
        printf(" Power Fault Detected");
    if (sltCtl & PCI_EXP_SLTCTL_MRLSNSR_ENB)
        printf(" MRL Sensor Changed");
    if (sltCtl & PCI_EXP_SLTCTL_PRES_DET_ENB)
        printf(" Presence Detect");
    if (sltCtl & PCI_EXP_SLTCTL_CCMPLT_ENB)
        printf(" Cmd Complete Int");
    if (sltCtl & PCI_EXP_SLTCTL_HPLINT_ENB)
        printf(" Hot-Plug Int");

     printf("\n");

    if (sltCtl & PCI_EXP_SLTCTL_ATTN_INDCTL)
        printf("\t\tAttn Indicator %s",
               indicator((sltCtl & PCI_EXP_SLTCTL_ATTN_INDCTL) >> 6));
    if (sltCtl & PCI_EXP_SLTCTL_PWR_INDCTL)
        printf(" Power Indicator %s",
               indicator((sltCtl & PCI_EXP_SLTCTL_PWR_INDCTL) >> 8));
    if (sltCtl & PCI_EXP_SLTCTL_PWR_CTLRCTL)
        printf(" Power Controller %s",
               (sltCtl & PCI_EXP_SLTCTL_PWR_INDCTL) >> 10 == 0 ?
               "on":"off");
    }

/*********************************************************************
*
* pcieCapabShow - show the MSI capabilities
*
* This routine displays the MSI specific information.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void pcieCapabShow
    (
    UINT        bus,           /* bus */
    UINT        device,        /* device */
    UINT        function,      /* function */
    UINT        offset         /* offset of capability */
    )
    {
    UINT16 capReg, type, slot = 0;

    printf("PCIe: ");

    pciConfigInWord (bus, device, function, offset+PCI_EXP_CAP_REG, &capReg);

    type = (capReg & PCI_EXP_CAP_PORT_TYPE) >> 4;

        switch (type)
          {
          case PCI_EXP_TYPE_ENDPOINT:
            printf("Endpoint");
            break;
          case PCI_EXP_TYPE_LEG_END:
            printf("Legacy Endpoint");
            break;
          case PCI_EXP_TYPE_ROOT_PORT:
            slot = capReg & PCI_EXP_CAP_SLOT_ADDON;
            printf("Root Port");
            if (slot)
               printf("- Slot");
            break;
          case PCI_EXP_TYPE_UPSTREAM:
            printf("Upstream Port");
            break;
          case PCI_EXP_TYPE_DOWNSTREAM:
             slot = capReg & PCI_EXP_CAP_SLOT_ADDON;
            printf("Downstream Port");
            if (slot)
               printf("- Slot");
            break;
          case PCI_EXP_TYPE_PCI2EXP:
            printf("PCI/PCI-X to Express Bridge");
            break;
          default:
            printf("Unknown");
        }
        printf(", IRQ %d\n", (capReg & PCI_EXP_CAP_IRQ) >> 9);

        /* mdo - implement a Verbose mode ??????? */
        pcieDevCap(bus, device, function, offset, type);

        pcieLinkCap(bus, device, function, offset, type);

        if (slot)
          pcieSlotCap(bus, device, function, offset);

    if (type == PCI_EXP_TYPE_ROOT_PORT)
        {
        pciConfigInWord (bus, device, function, offset+PCI_EXP_RTCTL_REG, &capReg);

        printf("\tRoot Control Enabled: ");
        if ( capReg & PCI_EXP_RTCTL_SECE_ENB)
            printf("Correctable ");
        if ( capReg & PCI_EXP_RTCTL_SENFE_ENB)
            printf("System Error (NF) ");
        if ( capReg & PCI_EXP_RTCTL_SEFE_ENB)
            printf("System Error (F) ");
        if ( capReg & PCI_EXP_RTCTL_PMEI_ENB)
            printf("PME Interrupt ");
        printf("\n");
        }

    }

/*********************************************************************
*
* capabShow - show the PCI capabilities
*
* This routine reads the capabilities pointer ID, traverses the
* capability linked list till the pointer is null, and prints the
* information.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void capabShow
    (
    UINT        bus,            /* bus */
    UINT        device,         /* device */
    UINT        function,       /* function */
    UINT8       capOffset       /* Capabilities Ptr Offset */
    )
    {
    UINT8 devCapID, cap_id, cap_id_next;

    /* Capabilities List Implemented: Get first capability ID */

    pciConfigInByte(bus, device, function, capOffset, &devCapID);

    /* Longword Align */
    devCapID &= ~0x03;

    while (devCapID)
        {
        printf("Capabilities - ");
        pciConfigInByte(bus, device, function, devCapID, &cap_id);
        pciConfigInByte(bus, device, function, devCapID+1, &cap_id_next);

        /* longword align */
        cap_id_next &= ~0x03;

        /* Enhancement: Dump specific capabilities regs */
        if (cap_id == 0xff)
            /* Get Out - something is wrong */
            break;

        switch (cap_id)
            {
            case PCI_EXT_CAP_PCI_PM:
                printf("Power Management \n");
                break;
            case PCI_EXT_CAP_AGP:
                printf("AGP ");
                break;
            case PCI_EXT_CAP_VPD:
                printf("Vital Product Data \n");
                break;
            case PCI_EXT_CAP_SLOTID:
                printf("Slot ID \n");
                break;
            case PCI_EXT_CAP_MSI:
                msiCapabShow(bus, device, function, devCapID);
                break;
            case PCI_EXT_CAP_HOT_SWAP:
                printf("cPCI Hot Swap \n");
                break;
            case PCI_EXT_CAP_PCIX:
                pcixCapabShow(bus, device, function, devCapID);
                break;
            case PCI_EXT_CAP_DBG_PORT:
                printf("Debug Port \n");
                break;
            case PCI_EXT_CAP_CPCI_RES:
                printf("cPCI Resource \n");
                break;
            case PCI_EXT_CAP_HPC:
                printf("PCI Hot Plug \n");
                break;
            case PCI_EXT_CAP_EXP:
                pcieCapabShow(bus, device, function, devCapID);
                break;
            case PCI_EXT_CAP_MSIX:
                printf("MSI-X \n");
                break;
            default:
                printf("%02x Unknown\n", cap_id);
            }
        devCapID = cap_id_next;
        }
    printf("\n");
}


/*********************************************************************
*
* pciConfigTopoShow - show PCI topology
*
* This routine traverses the PCI bus and prints assorted information
* about every device found.  The information is intended to present
* the topology of the PCI bus.  It includes: (1) the device type, (2)
* the command and status words, (3) for PCI to PCI bridges the memory
* and I/O space configuration, and (4) the values of all implemented
* BARs.
*
* RETURNS: N/A
*
* ERRNO
*/

void pciConfigTopoShow(void)
    {
    pciConfigForeachFunc(0,TRUE,pciConfigFuncShow,NULL);
    }
