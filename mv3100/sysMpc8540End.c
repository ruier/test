/* sysMpc8540End.c - system config for the MPC8540 TSEC/FEC Ethernet drivers */

/*
 * Copyright (c) 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01d,18oct07,y_w  Fixed usrNetworkInit() problem. (WIND00108286)
01c,12dec06,jch  (WRS) Updated for VxWorks 6.x compatibility
01b,14jul05,efb  Added function to retrieve interrupt vectors per port.
01a,09may05,efb  NewFile.
*/


/*

DESCRIPTION

This is the system level configuration module for the VxWorks MPC8540 TSEC 
and FEC Ethernet driver.  It performs the dynamic parameterization
of the driver with board specific values. This allows board specific
values to be declared outside of the driver elsewhere in the BSP so 
that they can be managed or modified without recompiling driver source.

*/

/* includes */

#include "vxWorks.h"
#include "end.h"
#include "string.h"
#include "config.h"
#include "stdlib.h" 
#include "stdio.h" 
#include "sysMotVpd.h"
#include "sysEpic.h"

/* defines */

/* typedefs */

/* forward declarations */

END_OBJ *sysMpc8540TsecEndLoad (char *, void *);
END_OBJ *sysMpc8540FsecEndLoad (char *, void *);
void     sysMpc8540EndHwInit (void);
STATUS   sysMpc8540EndGetMacAddr (UCHAR *, UINT32);
UINT32   sysMpc8540EndGetPhyAddr (UINT32);
UINT32   sysMpc8540EndGetDescBaseAddr (UINT32);

/* external references */

IMPORT END_OBJ *motTsecEndLoad (char *);
IMPORT END_OBJ *motFecEndLoad (char *);
IMPORT VPD_PACKET *sysVpdPkts[];      /* board vpd packets */
IMPORT STATUS   sysMotVpdPktGet (UCHAR, UINT32, VPD_PACKET **, VPD_PACKET **);
IMPORT void * cacheDmaMalloc(unsigned int bytes);

#define ETHER_DESC_SIZE 8   /* bytes */

/***************************************************************************    
*
* sysMpc8540TsecEndLoad - load the TSEC device driver
*
* This routine loads the Ethernet device with initial parameters.
* The format of the driver END_LOAD_STRING passed in as paramStr is:
* "<memBase>:<memSize>:<nTfds>:<nRfds>:<flags>:<offset>"
*
* RETURNS: An END object upon success
* ERROR if driver could not be loaded.
*/

END_OBJ *sysMpc8540TsecEndLoad
    (
    char *paramStr,   /* Initialization parameter string */
    void *unused ARG_UNUSED
    )
    {
    char     initStr[END_INIT_STR_MAX]; 
    END_OBJ *endObj = NULL;
    char     string[32];

    unused = NULL;       /* Prevents "unused" warning from compiler */

    if (strlen (paramStr) == 0)
        {

        /*
         * If the paramStr is zero length, this is the first call from the
         * mux layer, so the string is passed on to driver END load function
         * as is.
         */

        endObj = motTsecEndLoad (paramStr);
        
        }
    else
        {

        /*
         * This string will not be empty on the second call from the mux 
         * layer. It will contain the unit number as the first 
         * field. The rest of the parameter string is appended immediately
         * after the unit field. The completed string is then passed
         * down to the driver END load function to complete the load 
         * operation.
         */

        strcpy (initStr, paramStr); 
        strcat (initStr, "-1:0x00:");
        sprintf (string, "0x%x:", ETHER_TX_DESC_NUM);
        strcat (initStr, string);
        sprintf (string, "0x%x:", ETHER_RX_DESC_NUM);
        strcat (initStr, string); 
        strcat (initStr, "0x00:0x00");

        endObj = motTsecEndLoad (initStr);
        }
    return (endObj);
    }

/***************************************************************************    
*
* sysMpc8540FecEndLoad - load the FEC device driver
*
* This routine loads the Ethernet device with initial parameters.
* The format of the driver END_LOAD_STRING passed in as paramStr is:
* "<memBase>:<memSize>:<nTfds>:<nRfds>:<flags>:<offset>"
*
* RETURNS: An END object upon success
* ERROR if driver could not be loaded.
*/

END_OBJ *sysMpc8540FecEndLoad
    (
    char *paramStr,   /* Initialization parameter string */
    void *unused ARG_UNUSED
    )
    {
    char     initStr[END_INIT_STR_MAX]; 
    END_OBJ *endObj = NULL;
    char     string[32];

    unused = NULL;       /* Prevents "unused" warning from compiler */

    if (strlen (paramStr) == 0)
        {

        /*
         * If the paramStr is zero length, this is the first call from the
         * mux layer, so the string is passed on to driver END load function
         * as is.
         */

        endObj = motFecEndLoad (paramStr);
        
        }
    else
        {

        /*
         * This string will not be empty on the second call from the mux 
         * layer. It will contain the unit number as the first 
         * field. The rest of the parameter string is appended immediately
         * after the unit field. The completed string is then passed
         * down to the driver END load function to complete the load 
         * operation.
         */

        strcpy (initStr, paramStr); 
        strcat (initStr, "-1:0x00:");
        sprintf (string, "0x%x:", ETHER_TX_DESC_NUM);
        strcat (initStr, string);
        sprintf (string, "0x%x:", ETHER_RX_DESC_NUM);
        strcat (initStr, string); 
        strcat (initStr, "0x00:0x00");

        endObj = motFecEndLoad (initStr);
        }
    return (endObj);
    }

/***************************************************************************    
*
* sysMpc8540EndGetMacAddr - retrieve the Ethernet MAC address.
*
* This routine retrieves the MAC address for the specified port instance 
* from the previously initialized system VPD array. The MAC address is 
* copied to the macAddrDest buffer provided.
*
* RETURNS: OK upon success.
* ERROR if the destination parameter is null.
*/

STATUS sysMpc8540EndGetMacAddr 
    (
    UCHAR  *macAddrDest,  /* destination buffer */
    UINT32  instance      /* the address to get if multiple Ethernet ports */
    )
    {
    STATUS      status = OK;
    VPD_PACKET *etherVpd = NULL;

    if (macAddrDest != NULL)
        {
        if (sysMotVpdPktGet (VPD_PID_EA, instance, sysVpdPkts, &etherVpd) !=
            ERROR)
            {
            bcopy ((char *)etherVpd->data, (char *)macAddrDest, 
                   (int)etherVpd->size);
            }
        else
            {
 
            /* VPD get failed */

            status = ERROR;
            }
        }
    else 
        {

        /* Invalid parameter */

        status = ERROR;
        }
    return (status);
    }

/***************************************************************************    
*
* sysMpc8540EndGetPhyAddr - retrieve the port PHY address.
*
* This routine retrieves the PHY address for the specified port instance.
*
* RETURNS: The PHY address.
* ERROR if the port instance is out of range.
*/

UINT32 sysMpc8540EndGetPhyAddr 
    (
    UINT32  thePort      /* which Ethernet port */
    )
    {
    UINT32 phyAddr = (UINT32) ERROR;

    switch (thePort)
        {
        case 0 :
            phyAddr = PHY_ADDR_ENET_PORT0;
            break;

        case 1 :
            phyAddr = PHY_ADDR_ENET_PORT1;
            break;

        case 2 :
            phyAddr = PHY_ADDR_ENET_PORT2;
            break;

        default :
            phyAddr = (UINT32) ERROR;
            break;
        }
    return (phyAddr);
    }

/***************************************************************************    
*
* sysMpc8540EndGetIntVecs - retrieve the port interrupt vectors.
*
* This routine retrieves the interrupt vectors for the specified port instance.
* If only one Ethernet interrupt is available it is returned in txIntVec.
*
* RETURNS: OK on success.
* ERROR if the port instance is out of range.
*/

STATUS sysMpc8540EndGetIntVecs 
    (
    UINT32  thePort,      /* which Ethernet port */
    UINT32 *txIntVec,     /* Transmit interrupt vector */
    UINT32 *rxIntVec,     /* Receive interrupt vector */
    UINT32 *erIntVec,     /* Error interrupt vector */
    UINT32 *phyIntVec     /* PHY interrupt vector */
    )
    {
    STATUS status = OK;

    switch (thePort)
        {
        case 0 :
            *txIntVec = EPIC_TSEC1TX_INT_VEC;
            *rxIntVec = EPIC_TSEC1RX_INT_VEC;
            *erIntVec = EPIC_TSEC1ERR_INT_VEC;
            *phyIntVec = EPIC_VEC_EXT_IRQ10;
            break;

        case 1 :
            *txIntVec = EPIC_TSEC2TX_INT_VEC;
            *rxIntVec = EPIC_TSEC2RX_INT_VEC;
            *erIntVec = EPIC_TSEC2ERR_INT_VEC;
            *phyIntVec = EPIC_VEC_EXT_IRQ10;
            break;

        case 2 :
            *txIntVec = EPIC_FEC_INT_VEC;
            *rxIntVec = (UINT32) ERROR;
            *erIntVec = (UINT32) ERROR;
            *phyIntVec = EPIC_VEC_EXT_IRQ10;
            break;

        default :
            *txIntVec = (UINT32) ERROR;
            *rxIntVec = (UINT32) ERROR;
            *erIntVec = (UINT32) ERROR;
            status = ERROR;
            break;
        }
    return (status);
    }

/***************************************************************************    
*
* sysMpc8540EndGetDescBaseAddr - retrieve the descriptor base address.
*
* This routine provides the port's descriptor base address where its 
* transmit and receive descriptor lists will be placed.
*
* Currently the address malloc'ed from main memory.
*
* RETURNS: The base address.
* ERROR if the port instance is out of range.
*/

UINT32 sysMpc8540EndGetDescBaseAddr 
    (
    UINT32  thePort      /* which Ethernet port */
    )
    {
    UINT32 baseAddr = (UINT32) ERROR;

    if (thePort < MAX_ENET_PORTS)
        {
        baseAddr = (UINT32)cacheDmaMalloc((ETHER_TX_DESC_NUM + ETHER_RX_DESC_NUM) *
                                     ETHER_DESC_SIZE);
        }
    return (baseAddr);
    }

/***************************************************************************    
*
* sysMpc8540EndDescBaseFree - free the descriptor base address.
*
* This routine takes whatever action is necessary, if any, to free the
* the address space provided by the routine sysMpc8540EndGetDescBaseAddr.
* The routine must be coded to match how the space was originally provided.
* If the memory space was allocated, the space should be freed. If the 
* memory was at a fixed location then no action should be taken.
*
* Since this address space is malloc'ed from main memory, the address is 
* is simply freed by this routine.
*
* RETURNS: N/A
*/

void sysMpc8540EndDescBaseFree 
    (
    UINT32  addr      /* Base address of descriptor block */
    )
    {
    if (addr != 0x00)
        {
        free ((char *)addr);
        }
    }
