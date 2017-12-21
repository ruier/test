/* sysBusPci.c - Walnut PCI autoconfig support */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2000-2004, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,17sep09,b_m  fix compiler warnings (WIND00068436)
01e,30aug04,mdo  Documentation fixes for apigen
01d,23oct03,pch  SPR 80911: get PPC4xx cache dimensions from PVR
01c,25jan02,pch  Add comment re Diab warning
01b,19nov01,pch  cleanup
01a,09may00,mcg  created from templatePpc/sysBusPci.c version 01b

*/
/*
DESCRIPTION

This file contains functions needed to initialize and kick off the PCI
auto-configuration process, functions needed by the PCI auto-configurations
routines to perform configuration cycles (mechanism 0), and a function to
initialize the PCI bridge in the 405GP.

*/

/* includes */

#include "vxWorks.h"
#include "logLib.h"
#include "taskLib.h"
#include "config.h"

#include "drv/pci/pciConfigLib.h"
#include "drv/pci/pciAutoConfigLib.h"

/* defines */

/* typedefs */

/* globals */

PCI_SYSTEM sysParams;

/*
 * For each of the 4 PCI expansion slot on the Walnut board, the INTA, INTB,
 * INTC, and INTD pins are wire-ORed together.  Each wire-ORed signal is then
 * connected to a unique pin on the 405GP Universal Interrupt Controller.
 */

#define DEV1_INT    INT_VEC_PCI_SLOT3     /* INTA/B/C/D for PCI slot 3 (J25) */
#define DEV2_INT    INT_VEC_PCI_SLOT2     /* INTA/B/C/D for PCI slot 2 (J26) */
#define DEV3_INT    INT_VEC_PCI_SLOT1     /* INTA/B/C/D for PCI slot 1 (J29) */
#define DEV4_INT    INT_VEC_PCI_SLOT0     /* INTA/B/C/D for PCI slot 0 (J31) */

static UCHAR intLine [][4] =
    {
    /*    IntA      IntB      IntC      IntD                            */
        { 0xff,     0xff,     0xff,     0xff     },  /* device number 0 */
        { DEV1_INT, DEV1_INT, DEV1_INT, DEV1_INT },  /* device number 1 */
        { DEV2_INT, DEV2_INT, DEV2_INT, DEV2_INT },  /* device number 2 */
        { DEV3_INT, DEV3_INT, DEV3_INT, DEV3_INT },  /* device number 3 */
        { DEV4_INT, DEV4_INT, DEV4_INT, DEV4_INT }   /* device number 4 */
    };

#ifdef _PPC_PVR_CACHE_SIZE_FIELD
/*
 * If using a newer version of the architecture cache library, which
 * determines the cache sizes by examining the PVR, the following variable
 * is defined there instead of in the BSP and needs to be imported.
 */
IMPORT int ppc405CACHE_ALIGN_SIZE;
#endif	/* _PPC_PVR_CACHE_SIZE_FIELD */

/* forward declarations */

LOCAL UCHAR sysPciAutoConfigIntAsgn ( PCI_SYSTEM * pSys, PCI_LOC * pFunc,
    UCHAR intPin );
LOCAL STATUS sysPciAutoConfigInclude ( PCI_SYSTEM *pSys, PCI_LOC *pciLoc,
    UINT devVend );

/* subroutines */

/******************************************************************************
*
* sysPciAutoConfigInclude - Determine if function is to be autoConfigured
*
* This function is called with PCI bus, device, function, and vendor
* information.  It returns an indication of whether or not the particular
* function should be included in the automatic configuration process.
* This capability is useful if it is desired that a particular function
* NOT be automatically configured.  Of course, if the device is not
* included in automatic configuration, it will be unusable unless the
* user's code made provisions to configure the function outside of the
* the automatic process.
*
* RETURNS: TRUE if function is to be included in automatic configuration,
* FALSE otherwise.
*/

LOCAL STATUS sysPciAutoConfigInclude
    (
    PCI_SYSTEM *pSys,           /* input: AutoConfig system information */
    PCI_LOC *pciLoc,            /* input: PCI address of this function */
    UINT     devVend            /* input: Device/vendor ID number      */
    )
    {
    BOOL retVal = OK;

    /* If it's the host bridge then exclude it */

    if ((pciLoc->bus == 0) && (pciLoc->device == 0) && (pciLoc->function == 0))
        return ERROR;


    switch(devVend)
        {

        /* TODO - add any excluded devices by device/vendor ID here */

        default:
            retVal = OK;
            break;
        }

    return retVal;
    }

/******************************************************************************
*
* sysPciAutoConfigIntAssign - Assign the "interrupt line" value
*
* RETURNS: "interrupt line" value.
*
*/

LOCAL UCHAR sysPciAutoConfigIntAsgn
    (
    PCI_SYSTEM * pSys,          /* input: AutoConfig system information */
    PCI_LOC * pFunc,
    UCHAR intPin                /* input: interrupt pin number */
    )
    {
    UCHAR irqValue = 0xff;    /* Calculated value                */


    if (intPin == 0)
        return irqValue;

    irqValue = intLine [(pFunc->device)][(intPin - 1)];

    PCI_AUTO_DEBUG_MSG("intAssign called for device [%d %d %d] IRQ: %d\n",
                pFunc->bus, pFunc->device, pFunc->function,
                irqValue, 0, 0 );

    return (irqValue);
    }

/*******************************************************************************
*
* sysPciAutoConfig - PCI autoConfig support routine
*
* This routine instantiates the PCI_SYSTEM structure needed to configure
* the system. This consists of assigning address ranges to each category
* of PCI system resource: Prefetchable and Non-Prefetchable 32-bit Memory, and
* 16- and 32-bit I/O. Global values for the Cache Line Size and Maximum
* Latency are also specified. Finally, the four supplemental routines for
* device inclusion/exclusion, interrupt assignment, and pre- and
* post-enumeration bridge initialization are specified.
*
* RETURNS: N/A
*/

void sysPciAutoConfig (void)
    {

    /* 32-bit Non-prefetchable Memory Space */

    sysParams.pciMemIo32 = PCI_MSTR_MEMIO_BUS;
    sysParams.pciMemIo32Size = PCI_MSTR_MEMIO_SIZE;

    /* 32-bit Prefetchable Memory Space */

    sysParams.pciMem32= PCI_MSTR_MEM_BUS;
    sysParams.pciMem32Size = PCI_MSTR_MEM_SIZE;

    /* 16-bit ISA I/O Space not supported */

    sysParams.pciIo16 = 0;
    sysParams.pciIo16Size = 0;

    /* 32-bit PCI I/O Space */

    sysParams.pciIo32 = PCI_MSTR_IO_BUS;
    sysParams.pciIo32Size = PCI_MSTR_IO_SIZE;

    /* Configuration space parameters */

    sysParams.cacheSize = (ppc405CACHE_ALIGN_SIZE/4);
    sysParams.maxLatency = PCI_LAT_TIMER;
    sysParams.autoIntRouting = TRUE;
    sysParams.includeRtn = sysPciAutoConfigInclude;
    sysParams.intAssignRtn = sysPciAutoConfigIntAsgn;
    sysParams.bridgePreConfigInit = NULL;
    sysParams.bridgePostConfigInit = NULL;

    /* Perform AutoConfig */

    pciAutoConfig (&sysParams);

    return;
    }

/*******************************************************************************
*
* ibmPciConfigWrite - writes one PCI configuration space location
*
* This routine writes to one PCI configuration register location of the
* specified size (1, 2 or 4 bytes).
*
* RETURNS: N/A
*
*/
STATUS ibmPciConfigWrite
    (
    int bus,
    int device,
    int function,
    int offset,     /* offset into the configuration space */
    int width,      /* data width                          */
    UINT data       /* data to be written                  */
    )
    {
    int         key;
    UINT        regAddr;
    UINT        busDevFunc;

    /*
     * bit 31 must be 1 and bits 1:0 must be 0 (note LE bit notation)
     */
    busDevFunc = pciConfigBdfPack(bus, device, function);
    regAddr = 0x80000000 | ((offset|busDevFunc) & 0xFFFFFFFC);

    key = intLock();

    /*
     * Write config register address to the PCI config address register
     */
    sysPciOutLong(PCI_CFGADDR, regAddr);

    /*
     * Write value to be written to the PCI config data register
     */
    switch (width)
        {
        case 1: sysPciOutByte(PCI_CFGDATA | (offset & 0x3),
                              (UCHAR)(data & 0xFF));
            break;
        case 2: sysPciOutWord(PCI_CFGDATA | (offset & 0x3),
                              (UINT16)(data & 0xFFFF));
            break;
        case 4: sysPciOutLong(PCI_CFGDATA | (offset & 0x3), data);
            break;
        }

#ifdef PPC405GP_REVB
    sysPciOutLong(PCI_CFGADDR, 0x00000000);     /* For 405GP rev B errata # 6 */
#endif

    intUnlock (key);                            /* mutual exclusion stop */

    return(OK);
    }

/*******************************************************************************
*
* ibmPciConfigRead - reads one PCI configuration space register location
*
* This routine reads one PCI configuration register location of the
* specified size (1, 2 or 4 bytes).
*
* RETURNS : contents of specified register
*/
STATUS ibmPciConfigRead
    (
    int    bus,
    int    device,
    int    function,
    int    offset,     /* offset into the configuration space */
    int    width,      /* data width                          */
    void  *pResult
    )
    {
    int         key;
    UINT        regAddr;
    UINT        data = 0;
    UINT        busDevFunc;
    UCHAR *     pCfgByte;
    UINT16 *    pCfgWord;
    UINT *      pCfgLong;

    /*
     * bit 31 must be 1 and bits 1:0 must be 0 (note Little Endian bit notation)
     */
    busDevFunc = pciConfigBdfPack(bus, device, function);
    regAddr = 0x80000000 | ((offset|busDevFunc) & 0xFFFFFFFC);

    key = intLock();

    /*
     * Write config register address to the PCI config address register
     */
    sysPciOutLong(PCI_CFGADDR, regAddr);

    /*
     * Read value from the PCI config data register
     */
    switch (width)
        {
        case 1: data = (unsigned int)sysPciInByte(PCI_CFGDATA | (offset & 0x3));
            pCfgByte = (UCHAR *)pResult;
            *pCfgByte = (UCHAR)data;
            break;
        case 2: data = (unsigned int)sysPciInWord(PCI_CFGDATA | (offset & 0x3));
            pCfgWord = (UINT16 *)pResult;
            *pCfgWord = (UINT16)data;
            break;
        case 4: data = (sysPciInLong(PCI_CFGDATA | (offset & 0x3)));
            pCfgLong = (UINT *)pResult;
            *pCfgLong = (UINT)data;
            break;
        }

#ifdef PPC405GP_REVB
    sysPciOutLong(PCI_CFGADDR, 0x00000000);     /* For 405GP rev B errata # 6 */
#endif

    intUnlock (key);                            /* mutual exclusion stop */

    return(OK);
    }

/*******************************************************************************
* sysPciHostBridgeInit - Initialize the 405GP PCI Host Bridge
*
* Initializes the PCI bridge so it can operate as both a PCI master and slave.
* Parameters set are:
*     CPU->PCI (master/initiator) address translation
*     PCI->CPU (slave/target) address translation
*
* RETURNS: N/A
*
*/
STATUS sysPciHostBridgeInit(void)
    {
    UINT16 temp_short;

    /*
     * Disable all PCI Master regions initially
     */

    sysPciOutLong(PCIL_PMM0MA, PMM_UNUSED);
    sysPciOutLong(PCIL_PMM1MA, PMM_UNUSED);
    sysPciOutLong(PCIL_PMM2MA, PMM_UNUSED);

    /*
     * Disable PCI Target region 2 initially. Region 1 is hardwired to always
     * be active.
     */

    sysPciOutLong(PCIL_PTM2MS, PTM_UNUSED);

    /*
     * Drive PCI Reset out.  This is done for warm start.
     * Reset must be held for at least 1ms.
     */

    ibmPciConfigRead(PCI_HOST_BUS, PCI_HOST_DEVICE, PCI_HOST_FUNCTION,
                     PCI_CFG_BRDGOPT2, 2, &temp_short);

    ibmPciConfigWrite(PCI_HOST_BUS, PCI_HOST_DEVICE, PCI_HOST_FUNCTION,
                      PCI_CFG_BRDGOPT2, 2, temp_short | 0x1000);

    sysLocalDelay(1);                           /* kernel may not be up yet!! */

    ibmPciConfigWrite(PCI_HOST_BUS, PCI_HOST_DEVICE, PCI_HOST_FUNCTION,
                      PCI_CFG_BRDGOPT2, 2, temp_short);

    /*
     * Set up the PCI Master configuration (PMM).  This is the local memory
     * address to PCI memory address mapping.
     * See config.h
     */
    sysPciOutLong(PCIL_PMM0LA,    PMM0_LOCAL_ADRS);          /* PMM region 0 */
    sysPciOutLong(PCIL_PMM0PCILA, PMM0_PCI_LOW_ADRS);
    sysPciOutLong(PCIL_PMM0PCIHA, PMM0_PCI_HIGH_ADRS);
    sysPciOutLong(PCIL_PMM0MA,    PMM0_PCI_MASK_ATTRIB);

    sysPciOutLong(PCIL_PMM1LA,    PMM1_LOCAL_ADRS);          /* PMM region 1 */
    sysPciOutLong(PCIL_PMM1PCILA, PMM1_PCI_LOW_ADRS);
    sysPciOutLong(PCIL_PMM1PCIHA, PMM1_PCI_HIGH_ADRS);
    sysPciOutLong(PCIL_PMM1MA,    PMM1_PCI_MASK_ATTRIB);

    sysPciOutLong(PCIL_PMM2LA,    PMM2_LOCAL_ADRS);          /* PMM region 2 */
    sysPciOutLong(PCIL_PMM2PCILA, PMM2_PCI_LOW_ADRS);
    sysPciOutLong(PCIL_PMM2PCIHA, PMM2_PCI_HIGH_ADRS);
    sysPciOutLong(PCIL_PMM2MA,    PMM2_PCI_MASK_ATTRIB);

    /*
     * Set up the PCI Target configuration (PTM).  This is the PCI memory
     * address to local memory address mapping (slave window).
     * See config.h.  The enable bit for region 1 is hardwired on.
     */
    sysPciOutLong(PCIL_PTM1LA,    PTM1_LOCAL_ADRS);
    sysPciOutLong(PCIL_PTM1MS,    PTM1_SIZE_ATTRIB);
    ibmPciConfigWrite(PCI_HOST_BUS, PCI_HOST_DEVICE, PCI_HOST_FUNCTION,
                      PCI_CFG_BASE_ADDRESS_1, 4, PCI_SLV_MEM_BUS);

    /*
     * It is possible that the enable bit in PCIL_PTM2MS could be set at power
     * up.  If PTM2 is not going to be used, we must make sure that PTM2 is
     * properly disabled to avoid PCI target conflicts. PTM2MS must be enabled
     * to write PTM 2 BAR. Zero out PTM 2 BAR, then disable via PTM2MS.
     *
     * Note that the following conditional expression is a compile-time
     * constant.  Some compilers may warn about this, and/or about one
     * of the branches being unreachable.
     */

#if (PTM2_SIZE_ATTRIB == PTM_UNUSED)                /* PTM2 not being used */
    sysPciOutLong(PCIL_PTM2LA,    PTM_UNUSED);
    sysPciOutLong(PCIL_PTM2MS,    PTM_ENABLE);      /* enable temporarily */
    ibmPciConfigWrite(PCI_HOST_BUS, PCI_HOST_DEVICE, PCI_HOST_FUNCTION,
                      PCI_CFG_BASE_ADDRESS_2, 4, PTM_UNUSED);
    sysPciOutLong(PCIL_PTM2MS, PTM_UNUSED);         /* now disable */
#else                                               /* PTM2 will be used */
    sysPciOutLong(PCIL_PTM2LA,    PTM2_LOCAL_ADRS);
    sysPciOutLong(PCIL_PTM2MS,    PTM2_SIZE_ATTRIB);
#endif

    /*
     * Write the 405GP PCI Configuration regs.
     *   Enable 405GP to be a master on the PCI bus (PMM).
     *   Enable 405GP to act as a PCI memory target (PTM).
     */
    ibmPciConfigRead(PCI_HOST_BUS, PCI_HOST_DEVICE, PCI_HOST_FUNCTION,
                     PCI_CFG_COMMAND, 2, &temp_short);
    temp_short |= (PCI_CMD_MASTER_ENABLE | PCI_CMD_MEM_ENABLE);
    ibmPciConfigWrite(PCI_HOST_BUS, PCI_HOST_DEVICE, PCI_HOST_FUNCTION,
                      PCI_CFG_COMMAND, 2, temp_short);

    /*
     * Default value of the Bridge Options1 register is OK (0xFF60).
     * No need to change it.
     */

    /* Default value of the Bridge Options2 register is OK (0x0100) for
     * 405GP Rev B and beyond.
     */

    return(OK);
    }
