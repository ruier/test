/* sysAta.c - ATA initialization for sysLib.c */

/*
 * Copyright (c) 2005, 2009, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1997,1998,2000-2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01c,13jul10,sye  Added PCI autoconfig of SATA controller. (WIND00135823)
01b,04aug09,e_d  Added register interval to ATA_RESOURCE. (WIND00171937)
01a,10may05,efb  Ported from ver 01b, mcpm905/sysAta.c.
*/

/* 
Description

This file contains the sysAtaInit() necessary for
initializing the ATA/EIDE subsystem. 
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "drv/pci/pciConfigLib.h"

/* local defines */

#define PCI_CFG_SUBCLASS_MASK 0x00ff

/* external declarations */

/* global declarations */

/* forward declarations */

STATUS sysIntEnablePIC (int);
void   sysInWordString (ULONG, UINT16 *, int);
void   sysInWordStringRev (ULONG, UINT16 *, int);
void   sysOutWordString (ULONG, UINT16 *, int);
void   sysInLongString (ULONG, ULONG *, int);
void   sysOutLongString (ULONG, ULONG *, int);
void   sysAtaInit (int, int, int);

/******************************************************************************
*
* sysIntEnablePIC - enable an ISA/PCI interrupt
*
* This function call is used to enable an ISA/PCI interrupt.
*
* RETURNS: OK or ERROR if unable to enable interrupt.
*/

STATUS sysIntEnablePIC
    (
    int intNum              /* Interrupt number to enable */
    )
    {
    return (intEnable (intNum));
    }

/*****************************************************************************
*
* sysInWordString - reads a string of words from an io address.
*
* This function reads a word string from a specified io address.
*
* RETURNS: N/A
*/

void sysInWordString
    (
    ULONG   ioAddr,    /* Address to read */
    UINT16 *bufPtr,    /* Destination buffer address */
    int     nWords     /* Number of words to read */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *bufPtr++ = *(short *)ioAddr;
    }

/*****************************************************************************
*
* sysInWordStringRev - reads a string of words that are byte reversed
*
* This function reads a string of words that are byte reversed from a
* specified io address.
*
* RETURNS: N/A
*/

void sysInWordStringRev
    (
    ULONG   ioAddr, /* Address to read */
    UINT16 *bufPtr, /* Destination buffer address */
    int     nWords  /* Number of words to read */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *bufPtr++ = PCI_IN_WORD(ioAddr);
    }

/*****************************************************************************
*
* sysOutWordString - writes a string of words to an io address.
*
* This function writes a word string from a specified io address.
*
* RETURNS: N/A
*/

void sysOutWordString
    (
    ULONG       ioAddr,  /* Address to write */
    UINT16 *    bufPtr,  /* Source buffer address */
    int         nWords   /* Number of words to write */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *(short *)ioAddr = *bufPtr++;
    SYNC;
    }

/*****************************************************************************
*
* sysInLongString - reads a string of longwords from an io address.
*
* This function reads a longword string from a specified io address.
*
* RETURNS: N/A
*/

void sysInLongString
    (
    ULONG  ioAddr,  /* Address to read */
    ULONG *bufPtr,  /* Destination buffer address */
    int    nLongs   /* Number of words to read */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nLongs; loopCtr++)
        *bufPtr++ = *(int *)ioAddr;
    }

/*****************************************************************************
*
* sysOutLongString - writes a string of longwords to an io address.
*
* This function writes a longword string from a specified io address.
*
* RETURNS: N/A
*/

void sysOutLongString
    (
    ULONG  ioAddr,  /* Address to write */
    ULONG *bufPtr,  /* Source buffer address */
    int    nLongs   /* Number of words to write */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nLongs; loopCtr++)
        *(int *)ioAddr = *bufPtr++;
    SYNC;
    }

/******************************************************************************
*
* sysAtaInit - initialize the EIDE/ATA interface
*
* Perform the necessary initialization required before starting up the
* ATA/EIDE driver.
*
* RETURNS: N/A
*/

void sysAtaInit
    (
    int pciBusNo,    /* PCI bus number of ATA device to initialize */
    int pciDevNo,    /* PCI device number of ATA device to initialize */
    int pciFuncNo    /* PCI function number of ATA device to initialize */
    )
    {
    UINT   baseAddr;
    UINT16 subClass;

    /*
     * Disable PCI device by clearing its control (command) register
     * in its configuration header.
     */

    pciConfigOutWord (pciBusNo, pciDevNo, pciFuncNo, PCI_CFG_COMMAND, 0);

    /*
     * Check for legacy IDE mode. Abort with error message if sATA DPA mode.
     * Board configuration switch must be set to legacy mode for VxWorks.
     */

    pciConfigInWord (pciBusNo, pciDevNo, pciFuncNo, PCI_CFG_SUBCLASS, 
                     &subClass);

    if ((subClass & PCI_CFG_SUBCLASS_MASK) == SATA_DPA_MODE)
        {
        sysDebugMsg ("sysAta: Cannot initialize for sATA DPA! Legacy IDE mode required. \r\n** Set board configuration DIP switch #7 to OFF.\r\n",
                     DEFAULT_BSP_ERROR_BEHAVIOR);
        }

    /* Configure Cache Line Size Register to max value */

    pciConfigOutByte (pciBusNo, pciDevNo, pciFuncNo, 
                      PCI_CFG_CACHE_LINE_SIZE, 8);

    /* Configure Latency Timer to max value */

    pciConfigOutByte (pciBusNo, pciDevNo, pciFuncNo, 
                      PCI_CFG_LATENCY_TIMER, PCI_LAT_TIMER);

    /*
     * Configure device control (command) register to enable Bus Master
     * (for future DMA capability) and I/O Space.
     */

    pciConfigOutWord (pciBusNo, pciDevNo, pciFuncNo, PCI_CFG_COMMAND,
                      (UINT16)(PCI_CMD_MASTER_ENABLE | PCI_CMD_IO_ENABLE));

    /* Get and save Base Address of Command Reg for Primary Controller */

    pciConfigInLong (pciBusNo, pciDevNo, pciFuncNo, PCI_CFG_BASE_ADDRESS_0,
                     &baseAddr);
    ataResources[0].resource.ioStart[0] = (baseAddr & PCI_IOBASE_MASK);

    /*
     * Get and save an adjusted Base Address of Control Reg for
     * Primary Controller.
     */

    pciConfigInLong(pciBusNo, pciDevNo, pciFuncNo, PCI_CFG_BASE_ADDRESS_1,
                    &baseAddr);
    ataResources[0].resource.ioStart[1] = ((baseAddr & PCI_IOBASE_MASK) + 2);

    /*
     * Initialize the remainder of the ataResources structure for the
     * Primary Controller.
     */

    ataResources[0].ctrlType = IDE_LOCAL;
    ataResources[0].drives = ATA_MAX_DRIVES;
    ataResources[0].intVector = (int)PCI_SATA_INT;
    ataResources[0].intLevel = (int)PCI_SATA_INT;
    ataResources[0].configType = (ATA_DMA_AUTO | ATA_GEO_PHYSICAL);
    ataResources[0].semTimeout = 20;
    ataResources[0].wdgTimeout = 20;
    ataResources[0].regInterval = 1;

    /* Get and save Base Address of Command Reg for Secondary Controller */

    pciConfigInLong(pciBusNo, pciDevNo, pciFuncNo, PCI_CFG_BASE_ADDRESS_2,
                    &baseAddr);
    ataResources[1].resource.ioStart[0] = (baseAddr & PCI_IOBASE_MASK);

    /*
     * Get and save an adjusted Base Address of Control Reg for
     * Secondary Controller.
     */

    pciConfigInLong(pciBusNo, pciDevNo, pciFuncNo, PCI_CFG_BASE_ADDRESS_3,
                    &baseAddr);
    ataResources[1].resource.ioStart[1] = ((baseAddr & PCI_IOBASE_MASK) + 2);

    /*
     * Initialize the remainder of the ataResources structure for the
     * Secondary Controller.
     */

    ataResources[1].ctrlType = IDE_LOCAL;
    ataResources[1].drives = ATA_MAX_DRIVES - 1;
    ataResources[1].intVector = (int)PCI_SATA_INT;
    ataResources[1].intLevel = (int)PCI_SATA_INT;
    ataResources[1].configType = (ATA_DMA_AUTO | ATA_GEO_PHYSICAL);
    ataResources[1].semTimeout = 20;
    ataResources[1].wdgTimeout = 20;
    ataResources[1].regInterval = 1;    
    }
