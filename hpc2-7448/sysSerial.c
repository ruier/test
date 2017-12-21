/* sysSerial.c - BSP serial device initialization */

/* Copyright (c) 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,01feb06,dtr  tidyup
01a,03aug05,avb  adopted from sysSerial for wrPpmc7xx 
*/

#include <vxWorks.h>
#include <iv.h>
#include <intLib.h>
#include "config.h"
#include <sysLib.h>
#include "sysTsi108Mpic.h"
#include <drv/sio/ns16552Sio.h>

/* defines */

#define UART_REG(reg,chan) \
        (devParas[chan].baseAdrs + reg * devParas[chan].regSpace)

/* typedefs */

/* device initialization structure */

typedef struct 
    {
    USHORT vector;      /* Interrupt vector */
    ULONG  baseAdrs;        /* Register base address */
    USHORT regSpace;        /* Address Interval */
    USHORT intLevel;        /* Interrupt level */
    } NS16550_CHAN_PARAS;

/* locals */

/* Local data structures */

LOCAL NS16550_CHAN  ns16550Chan[N_UART_CHANNELS];

LOCAL NS16550_CHAN_PARAS devParas[] = {
    { INT_VEC_UART0, COM1_BASE_ADR, UART_REG_ADDR_INTERVAL, INT_VEC_UART0 },
    { INT_VEC_UART1, COM2_BASE_ADR, UART_REG_ADDR_INTERVAL, INT_VEC_UART1 },
};

/***************************************************************************
*
* sysSerialHwInit - initialize the BSP serial devices to a quiesent state
*
* This routine initializes the BSP serial device descriptors and puts the
* devices in a quiesent state.  It is called from sysHwInit() with
* interrupts locked.
*
* RETURNS: N/A
*
* SEE ALSO: sysHwInit()
*
* ERRNO: N/A
*/

void sysSerialHwInit 
    (
    void
    )
    {
    int i;

    for (i = 0; i < N_UART_CHANNELS; i++)
        {
        intDisable (devParas[i].intLevel); 

        ns16550Chan[i].regs     = (UINT8*)devParas[i].baseAdrs;
        ns16550Chan[i].level    = devParas[i].vector;
        ns16550Chan[i].ier      = 0;
        ns16550Chan[i].lcr      = 0;
        ns16550Chan[i].channelMode  = 0;
        ns16550Chan[i].regDelta     = devParas[i].regSpace;
        ns16550Chan[i].baudRate     = DEFAULT_BAUD_RATE;
        ns16550Chan[i].xtal         = UART_XTAL_FREQ;

        ns16550DevInit (&ns16550Chan[i]);
        }
    return;
    }

/***************************************************************************
*
* sysSerialHwInit2 - connect BSP serial device interrupts
*
* This routine connects the BSP serial device interrupts.  It is called from
* sysHwInit2().  Serial device interrupts could not be connected in
* sysSerialHwInit() because the kernel memory allocator was not initialized
* at that point, and intConnect() calls malloc().
*
* RETURNS: N/A
*
* SEE ALSO: sysHwInit2()
*
* ERRNO: N/A
*/

void sysSerialHwInit2 
    (
    void
    )
    {
    int i;

    /* connect serial interrupts */

    for (i = 0; i < N_UART_CHANNELS; i++)
        {
        if (ns16550Chan[i].level)
            {
            intConnect (INUM_TO_IVEC ((int ) ns16550Chan[i].level),
                        ns16550Int, (int)&ns16550Chan[i] );
            intEnable (devParas[i].intLevel); 
            }
        }
    return;
    }

/******************************************************************************
*
* sysSerialHwReset - reset the serial controllers
*
* Shutdown all controllers capable of generating interrupts, especially
* controllers using DMA to transfer channel command blocks.  Most Bug
* monitors presume that a hardware reset precedes entry to the monitor
* code.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*/

void sysSerialHwReset 
    (
    void 
    )
    {
    int i;

    for (i = 0; i < N_UART_CHANNELS; i++)
        ns16550DevInit (&ns16550Chan[i]);
    return;
    }

/******************************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine gets the SIO_CHAN device associated with a specified serial
* channel.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*
* ERRNO: N/A
*/

SIO_CHAN *sysSerialChanGet 
    (
    int channel
    )
    {
    if ((channel < 0) || (channel >= NELEMENTS(ns16550Chan)))
        return((SIO_CHAN *)ERROR);
    return((SIO_CHAN *) &ns16550Chan[channel]);
    }
