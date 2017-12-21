/* sysIOPort.c - SBC8260 I/O Port driver */

/*
 * Copyright (c) 2001, 2004, 2005, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,27nov12,ylu  Modify the sysIOPortGetPortAdrs() to get
                 the correct port address. (WIND00076411) 
01d,26aug05,mil  Converted logMsg to _func_logMsg.
01c,08jul05,mil  Decoupled logMsg in minimal kernel.
01b,22sep04,mdo  Documentation fixes for apigen
01a,07jul01,g_h  created from James Linick application note.
*/

/*
DESCRIPTION

This module contains the SBC8260 I/O port driver routines.
*/

/* include */
#include <vxWorks.h>
#include <logLib.h>
#include <intLib.h>
#include <drv/mem/m8260Siu.h>
#include <drv/intrCtl/m8260IntrCtl.h>
#include <drv/parallel/m8260IOPort.h>

/* globals */

/* external */

/* local */
LOCAL void sysIOPortCISR(void);

/* forward declaration */

void   sysIOPortWritePin (char port, UINT8 pin, BOOL high);
STATUS sysIOPortReadPin (char port, UINT8 pin);
UINT32 sysIOPortGetPortAdrs (char port, UINT8 portNum);
void   sysIOPortConnectISR (void);

/***********************************************************************
*
* sysIOPortWritePin - write to pin
*
* This routine writes data to an I/O pin.
*
* RETURNS: None
*
* ERRNO
*
* SEE ALSO: sysIOPortGetPortAdrs(), sysIOPortConnectISR(),
*           sysIOPortReadPin()
*/

void sysIOPortWritePin
    (
    char  port, 
    UINT8 pin, 
    BOOL  high
    )
    {
    UINT32 pinAdrs;

    pinAdrs = sysIOPortGetPortAdrs (port, 32 - pin);

    switch (port)
        {
        case 'A':
        case 'a':
            *M8260_IOP_PAPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PASO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PADIR(INTERNAL_MEM_MAP_ADDR) |=  pinAdrs;
            if (high)
                *M8260_IOP_PADAT(INTERNAL_MEM_MAP_ADDR) |=  pinAdrs;
            else
                *M8260_IOP_PADAT(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            break;
        case 'B':
        case 'b':
            *M8260_IOP_PBPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PBSO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PBDIR(INTERNAL_MEM_MAP_ADDR) |=  pinAdrs;
            if (high)
                *M8260_IOP_PBDAT(INTERNAL_MEM_MAP_ADDR) |=  pinAdrs;
            else
                *M8260_IOP_PBDAT(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            break;
        case 'C':
        case 'c':
            *M8260_IOP_PCPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PCSO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PCDIR(INTERNAL_MEM_MAP_ADDR) |=  pinAdrs;
            if (high)
                *M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR) |=  pinAdrs;
            else
                *M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            break;
        case 'D':
        case 'd':
            *M8260_IOP_PDPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PDSO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PDDIR(INTERNAL_MEM_MAP_ADDR) |=  pinAdrs;
            if (high)
                *M8260_IOP_PDDAT(INTERNAL_MEM_MAP_ADDR) |= pinAdrs;
            else
                *M8260_IOP_PDDAT(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            break;
        default:
            break;
        }
    }

/***********************************************************************
*
* sysIOPortReadPin - read from pin
*
* This routine reads the data from an I/O pin.
*
* RETURNS: OK or ERROR
*
* ERRNO
*
* SEE ALSO: sysIOPortGetPortAdrs(), sysIOPortWritePin(), 
*           sysIOPortConnectISR()
*/

STATUS sysIOPortReadPin
    (
    char  port, 
    UINT8 pin
    )
    {
    UINT32 pinAdrs;
    STATUS retVal = ERROR;


    pin = 32 - pin;
    
    pinAdrs = sysIOPortGetPortAdrs (port, pin);

    switch (port)
        {
        case 'A':
        case 'a':
            *M8260_IOP_PAPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PASO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PADIR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            if (*M8260_IOP_PADAT(INTERNAL_MEM_MAP_ADDR) & pinAdrs)
                retVal = OK;
            break;
        case 'B':
        case 'b':
            *M8260_IOP_PBPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PBSO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PBDIR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            if (*M8260_IOP_PBDAT(INTERNAL_MEM_MAP_ADDR) & pinAdrs)
                retVal = OK;
            break;
        case 'C':
        case 'c':
            *M8260_IOP_PCPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PCSO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PCDIR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            if (*M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR) & pinAdrs)
                retVal = OK;
            break;
        case 'D':
        case 'd':
            *M8260_IOP_PDPAR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PDSO (INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            *M8260_IOP_PDDIR(INTERNAL_MEM_MAP_ADDR) &= ~pinAdrs;
            if (*M8260_IOP_PDDAT(INTERNAL_MEM_MAP_ADDR) & pinAdrs)
                retVal = OK;
            break;
        default:
            break;
        }

    return (retVal);
    }

/***********************************************************************
*
* sysIOPortGetPortAdrs - get port address
*
* This routine gets the port address.
*
* RETURNS: Port address
*
* ERRNO
*
* SEE ALSO: sysIOPortReadPin(), sysIOPortWritePin(), 
*           sysIOPortConnectISR()
*/

UINT32 sysIOPortGetPortAdrs
    (
    char  port, 
    UINT8 portNum
    )
    {
    UINT32 adrs = 0x00000000;

    switch (port)
        {
        case 'A':
        case 'a':
            if (portNum < 32)
                adrs = PA0 >> portNum;
            else
                if (_func_logMsg != NULL)
                    _func_logMsg("Invalid port number for port A!\n",
                                 0, 0, 0, 0, 0, 0);
            break;
        case 'B':
        case 'b':
            if (portNum < 32 && portNum > 3)
                adrs = PB4 >> (portNum - 4);
            else
                if (_func_logMsg != NULL)
                    _func_logMsg("Invalid port number for port B!\n",
                                 0, 0, 0, 0, 0, 0);
            break;
        case 'C':
        case 'c':
            if (portNum < 32)
                adrs = PC0 >> portNum;
            else
                if (_func_logMsg != NULL)
                    _func_logMsg("Invalid port number for port C!\n",
                                 0, 0, 0, 0, 0, 0);
            break;
        case 'D':
        case 'd':
            if (portNum < 32 && portNum > 3)
                adrs = PD4 >> (portNum - 4);
            else
                if (_func_logMsg != NULL)
                    _func_logMsg("Invalid port number for port D!\n",
                                 0, 0, 0, 0, 0, 0);
            break;
        default:
            if (_func_logMsg != NULL)
                _func_logMsg("Invalid port specified!\n", 0, 0, 0, 0, 0, 0);
            break;
        }

    return (adrs);
    } 

/***********************************************************************
*
* sysIOPortConnectISR - Connect port C to ISR
*
* This routine connects port C to the ISR.
*
* NOTE: In the following example, the function sysIOPortConnectISR is used 
*       to configure the interrupt and connect the handler. Pin A32 is 
*       connected to pin C32 and calls to sysIOPortWritePin('A', 32, 1) 
*       and then sysIOPortWritePin('A', 32, 0) are used to transition pin 
*       A32 state and generate an interrupt through pin C32. Because the 
*       SIEXR bit is high, the interrupt only occurs on a high to low 
*       transition.
*
* RETURNS: None
*
* ERRNO
*
* SEE ALSO: sysIOPortGetPortAdrs(), sysIOPortWritePin(), 
*           sysIOPortReadPin()
*/

void sysIOPortConnectISR
    (
    void
    )
    {
    UINT32 pinAdrs;

    intConnect (m8260InumToIvec(INUM_PC0), (VOIDFUNCPTR)sysIOPortCISR, 0);

    pinAdrs = sysIOPortGetPortAdrs ('C', 0);

    *M8260_IOP_PCSO(INTERNAL_MEM_MAP_ADDR)   &= ~pinAdrs;
    *M8260_IOP_PCDIR(INTERNAL_MEM_MAP_ADDR)  &= ~pinAdrs;
    *M8260_IOP_PCPAR(INTERNAL_MEM_MAP_ADDR)  &= ~pinAdrs;
    *M8260_SIEXR(INTERNAL_MEM_MAP_ADDR)      |=  pinAdrs;
    *M8260_SIMR_H(INTERNAL_MEM_MAP_ADDR)     |=  pinAdrs;

    intEnable (INUM_PC0);
    }

/***********************************************************************
*
* sysIOPortCISR - Port C ISR
*
* This routine is the Port C ISR.
*
* RETURNS: None
*
* ERRNO
*
* SEE ALSO: sysIOPortGetPortAdrs(), sysIOPortWritePin(), 
*           sysIOPortReadPin()
*/

LOCAL void sysIOPortCISR
    (
    void
    )
    {
	/* TO DO - Add your code here */
    }
