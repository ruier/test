/* usbPciStub.c - USB Configuration File */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01a,16jun10,syt  derived from version 01e of fsl_rdb8313/usbPciStub.c
*/

/*
DESCRIPTION

This configlette initializes a USB Host Controller Interface (EHCI)

INCLUDE FILES: usb/pciConstants.h, usb/usbPciLib.h
*/

/* includes */

#include "vxWorks.h"
#include "config.h"

#include <usb/usbPlatform.h>    /* Basic definitions */
#include <usb/usbPciLib.h>      /* Our API */

IMPORT void sysMsDelay(uint32_t);
IMPORT void sysUsbInterfaceInit (void);

#ifdef INCLUDE_EHCI

/*******************************************************************************
*
* sysM8308EhciInit - intializes the on-chip EHCI
*
* This routine initializes the USB MPH clock, configures the MPH pins and
* enables the ports.
*
* RETURNS: N/A
*
* ERRNO: none
*
*\NOMANUAL
*/

VOID sysEhciInit
    (
    VOID
    )
    {
    /* Set host mode */

    *M83XX_USBMODE |= LONGSWAP(0x13);

    /* Setup Snooping for all the 4GB space */

    *M83XX_SNOOP1 = 0x0000001e;
    *M83XX_SNOOP2 = 0x8000001e;

    /* Ports Configuration */

    *M83XX_PORTSC1 |= LONGSWAP(0x80000000);

    /* Enable USB phy */

    *M83XX_CONTROL |= 0x00000004;

    /* Recommended prio and age cnt threshold */

    *M83XX_PRI_CTRL = 0x0c;
    *M83XX_AGE_CNT_THRESH = 0x40;

    /* rd prefetch 32byte base on burst size 8 x 4 */

    *M83XX_SI_CTRL = 0x00000001;
    }

/*******************************************************************************
*
* usbOpAfterReset - operation after usb ehcd controller reset
*
* set MPC837x usb DR controller to host mode after reset.
*
*/

VOID usbOpAfterReset
    (
    VOID
    )
    {

    *M83XX_USBMODE |= LONGSWAP(0x03);

    sysEhciInit();
    }

#endif /* INCLUDE_EHCI */

#if defined (INCLUDE_FSL_TCD)

/*******************************************************************************
*
* fslUsbDrDeviceHwInit - initialize hardware configuration for device module
*
* This routine initializes the hardware configuration related to device module.
*
* RETURNS: N/A
*/

VOID fslUsbDrDeviceHwInit 
    (
    VOID
    )
    {
    /* Enable USB phy */

    *M83XX_CONTROL |= 0x00000004;

    return;
    }

UINT16 usbPciWordIn
    (
    UINT32 pMem                  /* memory address to convert */
    ) 
    {
    return *(UINT32*)pMem;
    }

UINT32 usbPciDwordIn
    (
    UINT32 pMem                  /* memory address to convert */
    ) 
    {
    return *(UINT32*)pMem;
    }

UINT8 usbPciByteIn
    (
    UINT32 pMem                  /* memory address to convert */
    ) 
    {
    return *(UINT32*)pMem;
    }

void usbPciWordOut
    (
    UINT32 pMem,                  /* memory address to convert */
    UINT16 data
    ) 
    {
    *(UINT32*)pMem = data;
    }

void usbPciDwordOut
    (
    UINT32 pMem,
    UINT32 data                  /* memory address to convert */
    ) 
    {
    *(UINT32*)pMem = data;
    }

void usbPciByteOut
    (
    UINT32 pMem,                  /* memory address to convert */
    UINT8 data
    ) 
    {
    *(UINT32*)pMem = data;
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

/* XXX this name needs to be changed */

int usbPciIntDisable
    (
    UINT16 nIRQNumber
    )
    {
    int retStat = 0;
    retStat = (int)intDisable(nIRQNumber);
    return(retStat);
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

/* XXX this name needs to be changed */

int usbPciIntEnable
    (
    UINT16 nIRQNumber
    )
    {
    int retStat = 0;
    retStat = (int)intEnable(nIRQNumber);
    return(retStat);
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
    pVOID pMem                  /* memory address to convert */
    )

    {

    /* The conversion is a two-step process.  First, we need to convert the
     * logical processor address (virtual) to a physical address.  Then, we
     * convert the physical address to one which can be seen from the PCI
     * bus.
     */

    /* 1:1 */
    return (UINT32)pMem ;

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
    UINT32 pciAdrs              /* 32-bit PCI address to be converted */
    )

    {

    /* 1:1 */
    return(void *)(pciAdrs);

    }

UINT32 usbPciMemioOffset
    (
    void
    )
    {
    return(0);
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
    VOIDFUNCPTR func,     /* int handler to be removed */
    pVOID param,                    /* parameter for int handler */
    UINT16 intNo                    /* interrupt vector number */
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
    pVOID pMem,    /* base of memory region to invalidate */
    UINT32 size    /* size of region to invalidate */
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
    pVOID pMem,    /* base of memory region to invalidate */
    UINT32 size    /* size of region to invalidate */
    )
    {
    if (size != 0)
        CACHE_USER_FLUSH (pMem, size);
    }

#endif /* INCLUDE_FSL_TCD */

