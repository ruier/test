/* smc8260Sio.c - Motorola MPC8260 SMC UART serial driver */

/*
 * Copyright (c) 1984-2005, 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,21jan11,agf  clean-up compiler warnings
01k,09mar09,pcs  Updated to reflect the signature change of bcopyLongs.
01j,03aug05,mil  Corrected SIO_AVAIL_MODES_GET.
01i,01jul05,mil  Added poll only driver feature.
01h,27sep04,jln  SPR# 101300
01g,22sep04,mdo  Documentation fixes for apigen
01f,22oct03,ckj  Change the Macro DEFAULT_BAUD to CONSOLE_BAUD_RATE(SPR #85661)
01e,22oct01,g_h  Clean warning.
01d,12oct01,g_h  Clean for T2.2
01c,07may01,g_h  rename to smc8260Sio.c
01b,26apr99,elk  added SMC2.
01a,10mar99,elk	 adapted from ppc860Sio.c (ver 01b).
*/

/*
DESCRIPTION
This is the driver for the SMCs in the internal Communications Processor (CP)
of the Motorola MPC8260.  This driver only supports the SMCs in 
asynchronous UART mode.

USAGE
A PPC8260SMC_CHAN structure is used to describe the chip.
The BSP's sysHwInit() routine typically calls sysSerialHwInit(),
which initializes all the values in the PPC8260SMC_CHAN structure (except
the SIO_DRV_FUNCS) before calling smc8260DevInit().

The BSP's sysHwInit2() routine typically calls sysSerialHwInit2() which
connects the chip's interrupts via intConnect().

INCLUDE FILES: smc8260Sio.h
*/

/* includes */

#include <vxWorks.h>
#include <intLib.h>
#include <errno.h>
#include <sioLib.h>
#include <drv/mem/m8260Siu.h>
#include <drv/sio/m8260Brg.h>
#include <drv/sio/m8260Cp.h>
#include <drv/sio/m8260CpmMux.h>
#include "smc8260Sio.h"

void bcopyLongs(char *source, char *destination, size_t nlongs);

/* defines */

/* forward declarations */

LOCAL STATUS smc8260Ioctl (PPC8260SMC_CHAN *pChan,int request,int arg);
LOCAL void   smc8260ResetChannel (PPC8260SMC_CHAN *pChan);
LOCAL int    smc8260PollOutput (SIO_CHAN *,char);
LOCAL int    smc8260PollInput (SIO_CHAN *,char *);
#ifdef INCLUDE_TTY_DEV
LOCAL void   smc8260Startup (PPC8260SMC_CHAN *);
LOCAL int    smc8260CallbackInstall (SIO_CHAN *, int, STATUS (*)(), void *);
#endif  /* INCLUDE_TTY_DEV */

/* local driver function table */

LOCAL SIO_DRV_FUNCS smc8260SioDrvFuncs =
    {
    (int (*)())                smc8260Ioctl,
#ifdef INCLUDE_TTY_DEV
    (int (*)())                smc8260Startup,
    (int (*)())                smc8260CallbackInstall,
#else  /* INCLUDE_TTY_DEV */
    (int (*)())                NULL,
    (int (*)())                NULL,
#endif  /* INCLUDE_TTY_DEV */
    (int (*)())                smc8260PollInput,
    (int (*)(SIO_CHAN *,char)) smc8260PollOutput
    };

/*******************************************************************************
*
* smc8260DevInit - initialize the SMC
*
* This routine is called to initialize the chip to a quiescent state.
* Note that the `smcNum' field of PPC8260SMC_CHAN must be either 1 or 2.
*
* RETURNS: N/A.
*
* ERRNO
*/

void smc8260DevInit
    (
    PPC8260SMC_CHAN *pChan
    )
    {
    /* masks off this SMC's interrupt. */
	
    *M8260_SIMR_L(pChan->regBase) &= (~(0x00001000 >> (pChan->uart.smcNum - 1)));

    pChan->baudRate  = CONSOLE_BAUD_RATE;
    pChan->pDrvFuncs = &smc8260SioDrvFuncs;
    }

/*******************************************************************************
*
* smc8260ResetChannel - reset the SMC channel
*
* This routine resets the given SMC channel
*
* RETURNS: N/A.
*
* ERRNO
*/

LOCAL void smc8260ResetChannel 
    (
    PPC8260SMC_CHAN *pChan
    )
    {
    int smc;			/* the SMC number being initialized */
    int baud;			/* the baud rate generator being used */
    int frame;

    int oldlevel = intLock ();	/* lock interrupts */ 

    smc  = pChan->uart.smcNum - 1;		/* get SMC number */
    baud = pChan->bgrNum - 1;			/* get BRG number */

    pChan->uart.intMask = 0x00001000 >> smc;

    /* set up SMC as NMSI, select Baud Rate Generator */

    switch ( baud )
        {
        default:        /* default to BRG1 */
        case 0:         /* Select BRG1 */ 
            * CMXSMR(pChan->regBase) |= (0x00 >> (4 * smc));
            break;
        case 1:         /* Select BRG2 */ 
            * CMXSMR(pChan->regBase) |= (0x00 >> (4 * smc));
            break;
        case 6:         /* select BRG7 */
            * CMXSMR(pChan->regBase) |= (0x10 >> (4 * smc));
            break;
        case 7:         /* select BRG8 */
            * CMXSMR(pChan->regBase) |= (0x10 >> (4 * smc));
            break;
        }
 
    /* reset baud rate generator, wait for reset to clear... */
 
    *pChan->pBaud |= M8260_BRGC_RST;
    while (*pChan->pBaud & M8260_BRGC_RST);

    smc8260Ioctl (pChan, SIO_BAUD_SET, pChan->baudRate);

    /* set up transmit buffer descriptors */

    pChan->uart.txBdBase = (SMC_BUF *) (pChan->regBase +
			 ((UINT32) pChan->uart.txBdBase ));
    pChan->uart.pSmc->param.tbase = (UINT16) ((UINT32) pChan->uart.txBdBase);
    pChan->uart.pSmc->param.tbptr = (UINT16) ((UINT32) pChan->uart.txBdBase);
    pChan->uart.txBdNext = 0;

    /* initialize each transmit buffer descriptor */

    for (frame = 0; frame < pChan->uart.txBdNum; frame++)
        {
        pChan->uart.txBdBase[frame].statusMode = BD_TX_INTERRUPT_BIT;

        pChan->uart.txBdBase[frame].dataPointer = pChan->uart.txBufBase +
                                                (frame * pChan->uart.txBufSize);
        }

    /* set the last BD to wrap to the first */

    pChan->uart.txBdBase[(frame - 1)].statusMode |= BD_TX_WRAP_BIT;

    /* set up receive buffer descriptors */

    pChan->uart.rxBdBase = (SMC_BUF *) (pChan->regBase +
		         ((UINT32) pChan->uart.rxBdBase ));

    pChan->uart.pSmc->param.rbase = (UINT16) ((UINT32) pChan->uart.rxBdBase);
    pChan->uart.pSmc->param.rbptr = (UINT16) ((UINT32) pChan->uart.rxBdBase);
    pChan->uart.rxBdNext = 0;

    /* initialize each receive buffer descriptor */
	
    for (frame = 0; frame < pChan->uart.rxBdNum; frame++)
        {
        pChan->uart.rxBdBase[frame].statusMode = BD_RX_EMPTY_BIT |
						 BD_RX_INTERRUPT_BIT;
        pChan->uart.rxBdBase[frame].dataLength = 1; /* char oriented */
        pChan->uart.rxBdBase[frame].dataPointer = pChan->uart.rxBufBase + frame;
        }

    /* set the last BD to wrap to the first */

    pChan->uart.rxBdBase[(frame - 1)].statusMode |= BD_RX_WRAP_BIT;


    /* set SMC attributes to standard UART mode */

    pChan->uart.pSmcReg->smcmr = SMCMR_STD_MODE;

    /* initialize parameter RAM area for this SMC */

    pChan->uart.pSmc->param.rfcr   = 0x18;	/* supervisor data access */
    pChan->uart.pSmc->param.tfcr   = 0x18;	/* supervisor data access */
    pChan->uart.pSmc->param.mrblr  = 0x1;	/* one character rx buffers */
    pChan->uart.pSmc->param.maxidl = 0x0;	/* no idle features */
    pChan->uart.pSmc->param.brkln  = 0x0;	/* no breaks received yet */
    pChan->uart.pSmc->param.brkec  = 0x0;	/* zero break condition ctr */
    pChan->uart.pSmc->param.brkcr  = 0x1;	/* xmit 1 BRK on stop */

    /* clear all events */

    pChan->uart.pSmcReg->smce = SMCE_ALL_EVENTS;

    /* enables the transmitter and receiver  */

    pChan->uart.pSmcReg->smcmr |= SMCMR_TEN | SMCMR_REN;

    /* unmask interrupt (Tx, Rx only) */

    pChan->uart.pSmcReg->smcm  = SMCM_TX_MSK | SMCM_RX_MSK;
    *M8260_SIMR_L(pChan->regBase) |= pChan->uart.intMask;

                     
    intUnlock (oldlevel);			/* UNLOCK INTERRUPTS */
    }

/*******************************************************************************
*
* smc8260Ioctl - special device control
*
* This routine is the special device control.
*
* RETURNS: OK on success, EIO on device error, ENOSYS on unsupported
*          request.
*
* ERRNO
*/

LOCAL STATUS smc8260Ioctl
    (
    PPC8260SMC_CHAN *	pChan,		/* device to control */
    int			request,	/* request code */
    int			arg		/* some argument */
    )
    {
    int		baudRate;
    int 	oldlevel;
    STATUS 	status = OK;

    switch (request)
        {
        case SIO_BAUD_SET:
            if (arg >=  SMC_MIN_BAUD && arg <= SMC_MAX_BAUD)
                {
                /* calculate proper counter value, then enable BRG */

                baudRate = (pChan->clockRate + (8 * arg)) / (16 * arg);

                if (--baudRate > 0xfff)
                    *pChan->pBaud = (BRGC_CD_MSK &
                                     (((baudRate + 8) / 16) << M8260_BRGC_CD_SHIFT)) | M8260_BRGC_EN |
                                    M8260_BRGC_DIV16;
                else
                    *pChan->pBaud = (BRGC_CD_MSK & 
                                     (baudRate << 1)) | M8260_BRGC_EN;

                pChan->baudRate = arg;
                }
            else
                status = EIO;
            break;

        case SIO_BAUD_GET:
            * (int *) arg = pChan->baudRate;
            break;

        case SIO_MODE_SET:
            if (!((int) arg == SIO_MODE_POLL || (int) arg == SIO_MODE_INT))
                {
                status = EIO;
                break;
                }

            /* lock interrupt  */

            oldlevel = intLock();

            /* initialize channel on first MODE_SET */

            if (!pChan->channelMode)
                smc8260ResetChannel(pChan);

            if ((pChan->channelMode == SIO_MODE_POLL) && (arg == SIO_MODE_INT))
                {
                int i;
		char discardChar;

                /*
                 * if switching from POLL to INT mode, wait for all characters 
                 * to clear the output pins
                 */

                for (i=0; i < pChan->uart.txBdNum; i++)
                    while (pChan->uart.txBdBase
                           [(pChan->uart.txBdNext + i) % pChan->uart.txBdNum].
                           statusMode & BD_TX_READY_BIT);

		/* clean up the Rx Buffer before switching modes */

                 while (smc8260PollInput ((SIO_CHAN *)pChan, 
                                     (char *)&discardChar) == OK);
                }

            if (arg == SIO_MODE_INT)
                {
                * M8260_SIPNR_L(pChan->regBase) = pChan->uart.intMask;
                /* reset the SMC's interrupt status bit */

                * M8260_SIMR_L(pChan->regBase) |= pChan->uart.intMask;
                /* enable this SMC's interrupt  */

                pChan->uart.pSmcReg->smce = SMCE_RX;    
                /* reset the receiver status bit */ 

                pChan->uart.pSmcReg->smcm = SMCM_RX_MSK | SMCM_TX_MSK;
                /* enables receive and transmit interrupts */
                }
            else
                {
                pChan->uart.pSmcReg->smcm = 0;
                /* mask off the receive and transmit intrs */

                * M8260_SIMR_L(pChan->regBase) &= (~(pChan->uart.intMask));
                /* mask off this SMC's interrupt */ 

                }

            pChan->channelMode = arg;

            intUnlock(oldlevel);

            break;

        case SIO_MODE_GET:
            * (int *) arg = pChan->channelMode;
            break;

        case SIO_AVAIL_MODES_GET:
            *(int *)arg = SIO_MODE_POLL
#ifdef INCLUDE_TTY_DEV
                          | SIO_MODE_INT
#endif  /* INCLUDE_TTY_DEV */
                          ;
            break;

        default:
            status = ENOSYS;
        }

    return (status);
    }

#ifdef INCLUDE_TTY_DEV
/*******************************************************************************
*
* smc8260Int - handle an SMC interrupt
*
* This routine is called to handle SMC interrupts.
*
* RETURNS: N/A
*
* ERRNO
*/

void smc8260Int
    (
    PPC8260SMC_CHAN *pChan
    )
    {
    char		outChar;
    FAST UINT16		dataLen = 0;


    /* check for a receive event */

    if (pChan->uart.pSmcReg->smce & SMCE_RX)
        {
        pChan->uart.pSmcReg->smce = SMCE_RX;

        while (!(pChan->uart.rxBdBase [pChan->uart.rxBdNext].statusMode &
                 BD_RX_EMPTY_BIT))
            {
            /* process all filled receive buffers */

            outChar = pChan->uart.rxBdBase[pChan->uart.rxBdNext].dataPointer[0];

            pChan->uart.rxBdBase[pChan->uart.rxBdNext].statusMode |=
            BD_RX_EMPTY_BIT;

            /* incr BD count */

            pChan->uart.rxBdNext = (pChan->uart.rxBdNext + 1) %
                                   pChan->uart.rxBdNum;

            /* acknowledge interrupt ??? multiple events ??? */

            pChan->uart.pSmcReg->smce = SMCE_RX;

            (*pChan->putRcvChar) (pChan->putRcvArg,outChar);

            if (pChan->channelMode == SIO_MODE_POLL)
                break;
            }
        }

    /* check for a transmit event and if a character needs to be output */

    if ((pChan->uart.pSmcReg->smce & SMCE_TX) &&
        (pChan->channelMode != SIO_MODE_POLL))
        {
        pChan->uart.pSmcReg->smce = SMCE_TX;

        if ((*pChan->getTxChar) (pChan->getTxArg, &outChar) == OK)
            {
            do
                {
                pChan->uart.txBdBase[pChan->uart.txBdNext].dataPointer[dataLen++]
                = outChar;

                if (pChan->channelMode == SIO_MODE_POLL)
                    break;
                }
            while ((dataLen < pChan->uart.txBufSize) &&
                   ((*pChan->getTxChar) (pChan->getTxArg, &outChar)
                    == OK));

            pChan->uart.txBdBase[pChan->uart.txBdNext].dataLength = dataLen;

            /* acknowledge interrupt */

            pChan->uart.pSmcReg->smce = SMCE_TX;

            /* send transmit buffer */

            pChan->uart.txBdBase[pChan->uart.txBdNext].statusMode |=
            BD_TX_READY_BIT;

            /* incr BD count */

            pChan->uart.txBdNext = (pChan->uart.txBdNext+ 1) %
                                   pChan->uart.txBdNum;
            }
        }

    /* acknowledge all other interrupts - ignore events */

    pChan->uart.pSmcReg->smce = (pChan->uart.pSmcReg->smce & 
                                 ~(SMCE_RX | SMCE_TX));

    * M8260_SIPNR_L(pChan->regBase) = pChan->uart.intMask;
    }

/*******************************************************************************
*
* smc8260Startup - transmitter startup routine
*
* This routine is the transmitter startup routine.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void smc8260Startup
    (
    PPC8260SMC_CHAN *pChan		/* ty device to start up */
    )
    {
    char outChar;
    FAST UINT16 dataLen = 0;

    if (pChan->channelMode == SIO_MODE_POLL)
        return;

    /* check if buffer is ready and if a character needs to be output */

    if ((!(pChan->uart.txBdBase[pChan->uart.txBdNext].statusMode &
           BD_TX_READY_BIT)) &&
        ((*pChan->getTxChar) (pChan->getTxArg, &outChar) == OK))
        {
        do
            {
            pChan->uart.txBdBase[pChan->uart.txBdNext].dataPointer[dataLen++] =
            outChar;
            }
        while ((dataLen < pChan->uart.txBufSize) &&
               ((*pChan->getTxChar) (pChan->getTxArg, &outChar) == OK));
        /* fill buffer */

        /* send transmit buffer */

        pChan->uart.txBdBase[pChan->uart.txBdNext].dataLength  = dataLen;
        pChan->uart.txBdBase[pChan->uart.txBdNext].statusMode |=
        BD_TX_READY_BIT;

        /* incr BD count */

        pChan->uart.txBdNext = (pChan->uart.txBdNext + 1) % pChan->uart.txBdNum;
        }
    }
#endif  /* INCLUDE_TTY_DEV */

/******************************************************************************
*
* smc8260PollInput - poll the device for input.
*
* This routine receive data from the SMC channel in polling mode.
*
* RETURNS: OK if a character arrived, ERROR on device error, EAGAIN
*          if the input buffer is empty.
*
* ERRNO
*/

LOCAL int smc8260PollInput
    (
    SIO_CHAN *  pSioChan,
    char *  thisChar
    )
    {

    PPC8260SMC_CHAN * pChan = (PPC8260SMC_CHAN *) pSioChan;

    if (!(pChan->uart.pSmcReg->smce & SMCE_RX))
        return(EAGAIN);

    if (pChan->uart.rxBdBase[pChan->uart.rxBdNext].statusMode & BD_RX_EMPTY_BIT)
        return(EAGAIN);

    /* get a character */

    *thisChar = pChan->uart.rxBdBase[pChan->uart.rxBdNext].dataPointer[0];

    /* set the empty bit */

    pChan->uart.rxBdBase[pChan->uart.rxBdNext].statusMode |= BD_RX_EMPTY_BIT;

    /* incr BD count */

    pChan->uart.rxBdNext = (pChan->uart.rxBdNext + 1) % pChan->uart.rxBdNum;

    /* only clear RX event if no more characters are ready */

    if (pChan->uart.rxBdBase[pChan->uart.rxBdNext].statusMode & BD_RX_EMPTY_BIT)
        pChan->uart.pSmcReg->smce = SMCE_RX;

    return (OK);
    }

/******************************************************************************
*
* smc8260PollOutput - output a character in polled mode.
*
* This routine send data to the SMC channel in polling mode.
*
* RETURNS: OK if a character arrived, ERROR on device error, EAGAIN
*          if the output buffer if full.
*
* ERRNO
*/

LOCAL int smc8260PollOutput
    (
    SIO_CHAN *	pSioChan,
    char	outChar
    )
    {
    PPC8260SMC_CHAN *	pChan = (PPC8260SMC_CHAN *) pSioChan;
    int			i;

    /* wait a bit for the last character to get out */
    /* because the PPC603 is a very fast processor */
    /* ???  make the 10000 value a #define */

    i = 0;

    while ( (i<10000) && (pChan->uart.txBdBase[pChan->uart.txBdNext].statusMode & BD_TX_READY_BIT) )
        {
        i = i + 1;    
        }

    /* is the transmitter ready to accept a character? */
    /* if still not, we have a problem */

    if (pChan->uart.txBdBase[pChan->uart.txBdNext].statusMode &
        BD_TX_READY_BIT)
        return(EAGAIN);

    /* reset the transmitter status bit */

    pChan->uart.pSmcReg->smce = SMCE_TX;

    /* write out the character */

    pChan->uart.txBdBase[pChan->uart.txBdNext].dataPointer[0] = outChar;

    pChan->uart.txBdBase[pChan->uart.txBdNext].dataLength  = 1;

    /* send transmit buffer */

    pChan->uart.txBdBase[pChan->uart.txBdNext].statusMode |= BD_TX_READY_BIT;

    pChan->uart.txBdNext = (pChan->uart.txBdNext + 1) % pChan->uart.txBdNum;

    return (OK);
    }

#ifdef INCLUDE_TTY_DEV
/******************************************************************************
*
* smc8260CallbackInstall - install ISR callbacks to get put chars.
*
* This routine is the call back install routine.
*
* RETURNS: OK or ENOSYS
*
* ERRNO
*/

LOCAL int smc8260CallbackInstall
    (
    SIO_CHAN *	pSioChan,
    int		callbackType,
    STATUS	(* callback)(),
    void *	callbackArg
    )
    {
    PPC8260SMC_CHAN * pChan = (PPC8260SMC_CHAN *) pSioChan;

    switch (callbackType)
        {
        case SIO_CALLBACK_GET_TX_CHAR:
            pChan->getTxChar    = callback;
            pChan->getTxArg     = callbackArg;
	    break;

        case SIO_CALLBACK_PUT_RCV_CHAR:
            pChan->putRcvChar   = callback;
            pChan->putRcvArg    = callbackArg;
	    break;

        default:
            return (ENOSYS);
        }

    return (OK);
    }
#endif  /* INCLUDE_TTY_DEV */
