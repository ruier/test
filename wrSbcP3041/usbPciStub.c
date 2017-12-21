/* usbPciStub.c - system specific USB functions */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,04nov11,y_y  add usb target support.
01a,04jul11,b_m  written.
*/

#include <vxWorks.h>
#include "config.h"
#include <usb/usbPlatform.h>
#include <usb/pciConstants.h>

/* USB register base addresses */

LOCAL UINT32 usbPhyBase[]  = { USB1_PHY_BASE, USB2_PHY_BASE };
LOCAL UINT32 usbEhciBase[] = { USB1_BASE,     USB2_BASE     };

/*******************************************************************************
*
* ehciInit - initialize the on-chip EHCI controller
*
* This routine initializes the on-chip EHCI controller.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ehciInit
    (
    int unit
    )
    {    
    /* enable usb phy */

    *(volatile UINT32 *)USB_PHY_EOR(usbPhyBase[unit]) = USB_PHY_EN_OVERRIDE;

    /* snoop size 4 GB */

    *(volatile UINT32 *)EHCI_SNOOP1(usbEhciBase[unit]) = EHCI_SNOOP_SIZE_2G;
    *(volatile UINT32 *)EHCI_SNOOP2(usbEhciBase[unit]) = 0x80000000 | EHCI_SNOOP_SIZE_2G;

    /* recommended priority and age count threshold */

    *(volatile UINT32 *)EHCI_PRI_CTRL(usbEhciBase[unit]) = EHCI_PRI_EN;
    *(volatile UINT32 *)EHCI_AGE_CNT_THRESH(usbEhciBase[unit]) = 0x00000080;

    /* rd prefetch 32 byte base on burst size 8 x 4 */

    *(volatile UINT32 *)EHCI_SI_CTRL(usbEhciBase[unit]) = 0x00000001;

    /* enable the usb interface */

    *(volatile UINT32 *)EHCI_CONTROL(usbEhciBase[unit]) = (EHCI_CONTROL_UTMI_PHY_EN |
                                                   EHCI_CONTROL_USB_EN);

    /*
     * enable ports
     *
     * Note:
     * On P5020, unlike other preivous Freescale SOCs, the internal USB PHY
     * has been changed to UTMI, instead of ULPI.
     *
     * The write to PORTSC register to select UTMI PHY must be put in the last,
     * otherwise accessing other EHCI registers will make the CPU hang forever.
     */

    *(volatile UINT32 *)EHCI_PORTSC(usbEhciBase[unit]) = LONGSWAP(EHCI_PORTSC_UTMI);
    }

/*******************************************************************************
*
* ehciPostResetHook - post reset hook of the on-chip EHCI controller
*
* This routine sets the on-chip EHCI controller to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void ehciPostResetHook
    (
    int unit
    )
    {
    *(volatile UINT32 *)EHCI_USBMODE(usbEhciBase[unit]) |= LONGSWAP(EHCI_USBMODE_HOST);
    }

/*******************************************************************************
*
* ehci0Init - initialize the on-chip EHCI controller (USB1)
*
* This routine initializes the on-chip EHCI controller (USB1).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci0Init (void)
    {
    ehciInit(0);
    }

/*******************************************************************************
*
* ehci0PostResetHook - post reset hook of the on-chip EHCI controller (USB1)
*
* This routine sets the on-chip EHCI controller (USB1) to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci0PostResetHook (void)
    {
    ehciPostResetHook(0);
    }

/*******************************************************************************
*
* ehci1Init - initialize the on-chip EHCI controller (USB2)
*
* This routine initializes the on-chip EHCI controller (USB2).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci1Init (void)
    {
    ehciInit(1);
    }

/*******************************************************************************
*
* ehci1PostResetHook - post reset hook of the on-chip EHCI controller (USB2)
*
* This routine sets the on-chip EHCI controller (USB2) to host mode after reset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehci1PostResetHook (void)
    {
    ehciPostResetHook(1);
    }

#if defined (INCLUDE_FSL_TCD)

/*******************************************************************************
*
* fslUsbDrDeviceHwInit - initialize hardware configuration for device module
*
* This routine initializes the hardware configuration related to device module.
*
* RETURNS: N/A
*/

void fslUsbDrDeviceHwInit (void)
    {
    /* enable usb phy */

    *(volatile UINT32 *)USB_PHY_EOR(usbPhyBase[1]) = USB_PHY_EN_OVERRIDE;

    /* enable the usb interface */

    *(volatile UINT32 *)EHCI_CONTROL(usbEhciBase[1]) = (EHCI_CONTROL_UTMI_PHY_EN);

    return;
    }

UINT16 usbPciWordIn
    (
    UINT32 pMem                  /* memory address to convert */
    )
    {
    return *(UINT32 *)pMem;
    }

UINT32 usbPciDwordIn
    (
    UINT32 pMem                  /* memory address to convert */
    )
    {
    return *(UINT32 *)pMem;
    }

UINT8 usbPciByteIn
    (
    UINT32 pMem                  /* memory address to convert */
    )
    {
    return *(UINT32 *)pMem;
    }

void usbPciWordOut
    (
    UINT32 pMem,                  /* memory address to convert */
    UINT16 data
    )
    {
    *(UINT32 *)pMem = data;
    }

void usbPciDwordOut
    (
    UINT32 pMem,
    UINT32 data                  /* memory address to convert */
    )
    {
    *(UINT32 *)pMem = data;
    }

void usbPciByteOut
    (
    UINT32 pMem,                  /* memory address to convert */
    UINT8 data
    )
    {
    *(UINT32 *)pMem = data;
    }

/*******************************************************************************
*
* usbPciWordGet - Returns a UINT16 configuration value
*
* This function returns the UINT16 value at offset <regOffset> from
* the PCI configuration space of the device identified by <busNo>,
* <deviceNo>, and <funcNo>.
*
* NOTE: This function adjusts for big vs. little endian environments.
*
* RETURNS: UINT16 value read from device configuration space
*/

UINT32 usbPciWordGet
    (
    UINT8 busNo,     /* Bus number of device */
    UINT8 deviceNo,  /* Device number of device */
    UINT8 funcNo,    /* Function number of device */
    UINT16 regOffset /* Offset into PCI config space */
    )
    {
    return 0;
    }

/*******************************************************************************
*
* usbPciDwordGet - Returns a UINT32 configuration value
*
* This function returns the UINT32 value at offset <regOffset> from
* the PCI configuration space of the device identified by <busNo>,
* <deviceNo>, and <funcNo>.
*
* NOTE: This functions returns 0.  There is no configuration space on this
* board..
*
* RETURNS: UINT32 value read from device configuration space
*/

UINT32 usbPciDwordGet
    (
    UINT8 busNo,                /* Bus number of device */
    UINT8 deviceNo,             /* Device number of device */
    UINT8 funcNo,               /* Function number of device */
    UINT16 regOffset            /* Offset into PCI config space */
    )
    {
    return 0;
    }

/*******************************************************************************
*
* usbPciConfigHeaderGet - Reads a device's PCI configuration header
*
* This function reads the PCI configuration header for the device
* identified by <busNo>, <deviceNo>, and <funcNo>.  The configuration
* header is stored in the <pCfgHdr> buffer provided by the caller.
*
* This function initializes the <pCfgHdr> structure to zeros.  Any
* fields which cannot be read from the device's configuration header
* will remain zero upon return.  This function does not attempt to read
* fields defined as "reserved" in the PCI configuration header.
*
* RETURNS: N/A
*/

VOID usbPciConfigHeaderGet
    (
    UINT8 busNo,     /* Bus number of device */
    UINT8 deviceNo,  /* Device number of device */
    UINT8 funcNo,    /* Function number of device */
    pPCI_CFG_HEADER pCfgHdr    /* Buffer provided by caller */
    )
    {
    return;
    }

/*******************************************************************************
*
* usbPciIntDisable - Disables system interrupts
*
* This routine is used to disable the interrupts for IRQ specifed in
* <nIRQNumber>.
*
* RETURNS: status of the function called to disable the interrupt
*
* ERRNO: None
*/

int usbPciIntDisable
    (
    UINT16 nIRQNumber
    )
    {
    int retStat = 0;
    retStat = (int)intDisable(nIRQNumber);
    return (retStat);
    }

/*******************************************************************************
*
* usbPciIntEnable - Enable system interrupts
*
* This routine is used to enable the interrupts for IRQ specifed in
* <nIRQNumber>.
*
* RETURNS: status of the function called to enable the interrupt
*
* ERRNO: none
*/

int usbPciIntEnable
    (
    UINT16 nIRQNumber
    )
    {
    int retStat = 0;
    retStat = (int)intEnable(nIRQNumber);
    return (retStat);
    }

/*******************************************************************************
*
* usbPciIntConnect - Connect to a interrupt vector
*
* Connects the <func> to the interrupt number <intNo>.  <param> is an
* application-specific value which will be passed to <func> each time
* the interrupt handler is invoked.
*
* RETURNS: OK, or ERROR if unable to connect/enable interrupt
*/

STATUS usbPciIntConnect
    (
    VOIDFUNCPTR func,     /* new interrupt handler */
    pVOID param,          /* parameter for int handler */
    UINT16 intNo          /* interrupt vector number */
    )
    {
    UINT32 vec = (UINT32)intNo ;

    if (intConnect((VOIDFUNCPTR *)vec, func, (int)param) != OK)
        return ERROR;

    if (intEnable(vec) != OK)
        {
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* usbMemToPci - Convert a memory address to a PCI-reachable memory address
*
* Converts <pMem> to an equivalent 32-bit memory address visible from the
* PCI bus.  This conversion is necessary to allow PCI bus masters to address
* the same memory viewed by the processor.
*
* RETURNS: converted memory address
*/

UINT32 usbMemToPci
    (
    pVOID pMem          /* memory address to convert */
    )
    {

    /*
     * The conversion is a two-step process.  First, we need to convert the
     * logical processor address (virtual) to a physical address.  Then, we
     * convert the physical address to one which can be seen from the PCI
     * bus.
     */

    return (UINT32)pMem;
    }

/*******************************************************************************
*
* usbPciToMem - Convert a PCI-reachable address to a CPU-reachable pointer
*
* Converts <pciAdrs> to an equivalent CPU memory address.
*
* RETURNS: pointer to PCI memory
*/

pVOID usbPciToMem
    (
    UINT32 pciAdrs      /* 32-bit PCI address to be converted */
    )
    {
    return (void *)(pciAdrs);
    }

UINT32 usbPciMemioOffset (void)
    {
    return (0);
    }

/*******************************************************************************
*
* usbPciIntDisconnect - Removes an interrupt handler
*
* Removes an interrupt handler installed by usbPciIntConnect().  <func>,
* <param>, and <intNo> must match the corresponding parameters from an earlier
* call to usbPciIntConnect().
*
* RETURNS: N/A
*/

VOID usbPciIntRestore
    (
    VOIDFUNCPTR func,   /* int handler to be removed */
    pVOID param,        /* parameter for int handler */
    UINT16 intNo        /* interrupt vector number */
    )
    {
    UINT32 vec = (UINT32)intNo ;
    intDisable(vec);
    }

/*******************************************************************************
*
* usbPciMemInvalidate - Invalidate cache for a region of memory
*
* When another bus master, such as a PCI bus master, writes to memory, the
* cache may need to be invalidated for that region of memory.
*
* NOTE: Returns immediately if size == 0.
*
* RETURNS: N/A
*/

VOID usbPciMemInvalidate
    (
    pVOID pMem,         /* base of memory region to invalidate */
    UINT32 size         /* size of region to invalidate */
    )
    {
    if (size != 0)
        CACHE_USER_INVALIDATE (pMem, size);
    }

/*******************************************************************************
*
* usbPciMemFlush - Flush a region of memory through the cache
*
* In systems which implement a non-write-thru cache, the processor may have
* written data to memory which has not yet been flushed to the actual system
* memory.  Before other bus masters may interrogate this memory, it may be
* necessary to flush the cache.
*
* NOTE: Returns immediately if size == 0.
*
* RETURNS: N/A
*/

VOID usbPciMemFlush
    (
    pVOID pMem,         /* base of memory region to invalidate */
    UINT32 size         /* size of region to invalidate */
    )
    {
    if (size != 0)
        CACHE_USER_FLUSH (pMem, size);
    }

#endif /* INCLUDE_FSL_TCD */

