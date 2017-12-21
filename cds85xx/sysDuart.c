/* sysDuart.c -  Freescale cds85xx Duart device initialization */

/* Copyright 1984-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,22mar05,dtr  Fix typos.
01a,10sep04,jln  modified from ads85xx/sysDuart.c/01e
*/

/*
DESCRIPTION
This file contains board-specific routines for FreeScale cds85xx
Duart device initialization. 
*/

#include "vxWorks.h"
#include "config.h"
#include "intLib.h"
#include "iv.h"
#include "sysLib.h"
#include "config.h"

#include "sysEpic.h"
#include "sysDuart.h"		        /* MPC8241/8245 duart driver */
#include "drv/sio/ns16552Sio.h"		/* ns16552Sio driver */

/* device description structs */

typedef struct
    {
    USHORT vector;			/* Interrupt vector */
    ULONG  baseAdrs;			/* Register base address */
    USHORT regSpace;			/* Address Interval */
    USHORT intLevel;			/* Interrupt level */
    } NS16552_CHAN_PARAS;

/* external variable */

IMPORT int epicIntTrace;                /* trace epic internal interrupts */

/* global variable */

NS16550_CHAN  ns16550Chan[N_DUART_CHANNELS];

char  *eumbbar_base;                    /* eumbbar base address */

/* static variables */

/* NOTE: !!!
 * Only the second DUART channel is wired in the cds85xx board
 */

LOCAL NS16552_CHAN_PARAS devDuartParas[] = 
    {
    {EPIC_DUART_INT_VEC, COM2_ADR, DUART_REG_ADDR_INTERVAL, EPIC_DUART_INT_NUM},
    {EPIC_DUART_INT_VEC, COM1_ADR, DUART_REG_ADDR_INTERVAL, EPIC_DUART_INT_NUM}
    };

/*
 * Array of pointers to all serial channels configured in PPMC 8245 module.
 * See sysDuartChanGet(). It is this array that maps channel pointers
 * to standard device names.  The first entry will become "/tyCo/2",
 * the second "/tyCo/3".
 */

SIO_CHAN * sysSerialSioChans [N_SIO_CHANNELS] =
    {
    (SIO_CHAN *)&ns16550Chan[0].pDrvFuncs,
    (SIO_CHAN *)&ns16550Chan[1].pDrvFuncs
    };

/* definitions */

#define DUART_REG(reg, chan) \
        *(volatile UINT8 *)(devDuartParas[chan].baseAdrs + reg * devDuartParas[chan].regSpace)


/******************************************************************************
*
* sysDuartHwInit - initialize 8245 duart devices to a quiescent state
*
* This routine initializes the MPC 8245 Duart device descriptors and puts
* the devices in a quiescent state.  It is called from sysHwInit() with
* interrupts locked.   Polled mode serial operations are possible, but
* not interrupt mode operations which are enabled by sysDuartHwInit2().
*
* RETURNS: N/A
*
* SEE ALSO: sysHwInit()
*/ 

void sysDuartHwInit (void)
    {
    int i;

    eumbbar_base = (char *)CCSBAR;

    for (i = 0; i < N_DUART_CHANNELS; i++)
        {
	ns16550Chan[i].regs        = (UINT8 *)devDuartParas[i].baseAdrs;
	ns16550Chan[i].level	   = devDuartParas[i].intLevel;
	ns16550Chan[i].channelMode = SIO_MODE_INT;
	ns16550Chan[i].regDelta    = devDuartParas[i].regSpace;
	ns16550Chan[i].baudRate	   = DUART_BAUD;
	ns16550Chan[i].xtal	   = sysClkFreqGet();

	ns16550DevInit (&ns16550Chan[i]);
        }

    if (ns16550Chan[0].channelMode == SIO_MODE_INT)
	{
        eumbbar_base[UDCR2] = 0x01;  /* set duart mode */
        eumbbar_base[ULCR2] = 0x80;  /* open DLAB */
        eumbbar_base[UAFR2] = 0x00;
        eumbbar_base[UDMB2] = 0x06;  /* MSB 9600bps @266Mhz */
	eumbbar_base[UDLB2] = 0xc8;  /* LSB */
	eumbbar_base[ULCR2] = 0x03;  /* clear DLAB, no-parity, 1stop bit, 8bit data */
	eumbbar_base[UMCR2] = 0x02;  /* disable loopback mode */
	eumbbar_base[UIER2] = 0x03;  /* Tx empty, Rx interrupt enable */
	}

    } /* sysDuartHwInit () */


/******************************************************************************
*
* sysDuartHwInit2 - connect MPC8241/8245 duart device interrupts
*
* This routine connects the MPC8241/8245 duart device interrupts. It is called
* from sysHwInit2().  
*
* Serial device interrupts cannot be connected in sysDuartHwInit() because
* the kernel memory allocator is not initialized at that point and
* intConnect() calls malloc().
*
* RETURNS: N/A
*
* SEE ALSO: sysHwInit2()
*/ 

void sysSerialHwInit2 (void)
    {
    int i;

    /* connect serial interrupts */
    for (i = 0; i < N_DUART_CHANNELS; i++)
        {
        (void) sysEpicIntConnect ((VOIDFUNCPTR *)((int)devDuartParas[i].vector),
                           (VOIDFUNCPTR)ns16550Int, (int)&ns16550Chan[i] );

        sysEpicIntEnable (devDuartParas[i].vector); 
        }

    } /* sysDuartHwInit2 () */


/******************************************************************************
*
* sysDuartChanGet - get the SIO_CHAN device associated with MPC8245 duart channel
*
* This routine returns a pointer to the SIO_CHAN device associated
* with a specified serial channel on MPC8245.  It is called by usrRoot() to
* obtain pointers when creating the system serial devices, `/tyCo/x'.  It
* is also used by the WDB agent to locate its serial channel.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*/

SIO_CHAN * sysSerialChanGet
    (
    int channel         /* serial channel */
    )
    {
    if ( (channel < 0) ||
         (channel >= (int)NELEMENTS(sysSerialSioChans)) )
	return (SIO_CHAN *) ERROR;

    return sysSerialSioChans[channel];
    }



