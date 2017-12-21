/* sysSerial.c - BSP serial device initialization */

/*
 * Copyright (c) 1984-2005, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,25sep10,jxu  Added inclusion of iosLibP.h for _func_consoleOut.
01i,10oct05,h_k  included sysSerialChanGet for lower layer. (SPR #113407)
01h,08jul05,mil  Added polling kprintf support.
01g,24sep04,mdo  Documentation fixes for apigen
01f,30oct03,ckj  changed the sysSerialReset() to reset the device 
                 and mask the interrupts (SPR #85201).
01e,01jun04,j_b  SPR 97939: reflect proper MPC8260 SMC Parameter RAM map size
01d,10dec01,g_h  Move to use the generic m82xxDpramLib
01c,17jul01,g_h  changed to use the dynamic DPRAM memory allocation
01b,26apr99,elk  added SMC2.
01a,05mar99,elk  adopted from sysSerial/estMDP8xx (ver 01a).
*/

#include <vxWorks.h>
#include <iv.h>
#include <intLib.h>
#include "config.h"
#include <sysLib.h>
#include "smc8260Sio.h"
#include <drv/mem/m82xxDpramLib.h>
#include <drv/sio/m8260Cp.h>
#include <drv/sio/m8260CpmMux.h>
#include <drv/mem/m8260Siu.h>
#include <drv/intrCtl/m8260IntrCtl.h>
#include <private/iosLibP.h>		/* _func_consoleOut */

#include "smc8260Sio.c"

/* locals */

static PPC8260SMC_CHAN ppcChn[N_SIO_CHANNELS] ;

#ifdef INCLUDE_SIO_POLL
LOCAL int sysSerialPollConsoleOut (int arg, char *buf, int len);
#endif

/* External References */
IMPORT int sysBaudClkFreq(void);

/******************************************************************************
*
* sysSerialHwInit - initialize the BSP serial devices to a quiesent state
*
* This routine initializes the BSP serial device descriptors and puts the
* devices in a quiesent state.  It is called from sysHwInit() with
* interrupts locked.
*
* RETURNS: N/A
*
* ERRNO
*
* SEE ALSO: sysHwInit()
*/

void sysSerialHwInit
    (
    void
    )
    {
    int    i;
    UINT32 dpram1Base;
    UINT32 allocatedAdrs;

    for (i = 0; i < N_SIO_CHANNELS; i++)
        {
        /*
         * intialize the chips device descriptors.
         */

        ppcChn[i].regBase        = vxImmrGet ()       ; /* IMMR reg has base adr */
        ppcChn[i].clockRate      = sysBaudClkFreq ()  ; /* Baud clock freq (Hz)  */
        ppcChn[i].bgrNum         = 1 + i              ; /* use BRGx              */
        ppcChn[i].uart.smcNum    = 1 + i              ; /* SMCx wired for rs232  */
        ppcChn[i].uart.txBdNum   = 1                  ; /* use 1 transmit BD     */
        ppcChn[i].uart.rxBdNum   = 0x10               ; /* use 16 receive BD     */

        /* transmit BD base adrs */
        if ((allocatedAdrs = (0x0000ffff & (UINT32)m82xxDpramAlignedMalloc (8,8))) != 0)
            ppcChn[i].uart.txBdBase  = (SMC_BUF *) allocatedAdrs;
        else
            {
            sysToMonitor (BOOT_NO_AUTOBOOT);
            }

        /* receive BD base adrs  */
        if ((allocatedAdrs = (0x0000ffff & (UINT32)m82xxDpramAlignedMalloc (128,8))) != 0)
            ppcChn[i].uart.rxBdBase  = (SMC_BUF *) allocatedAdrs;
        else
            {
            sysToMonitor (BOOT_NO_AUTOBOOT);
            }

        ppcChn[i].uart.txBufSize = 0x1                ; /* transmit buffer size  */

        dpram1Base = MPC8260_DPRAM1_BASE( ppcChn[i].regBase );

        if ((allocatedAdrs = (UINT32)m82xxDpramAlignedMalloc (0x80,2)) != 0)
            ppcChn[i].uart.txBufBase = (u_char *) allocatedAdrs;
        else
            {
            sysToMonitor (BOOT_NO_AUTOBOOT);
            }

        if ((allocatedAdrs = (UINT32)m82xxDpramAlignedMalloc (0x80,2)) != 0)
            ppcChn[i].uart.rxBufBase = (u_char *)allocatedAdrs;
        else
            {
            sysToMonitor (BOOT_NO_AUTOBOOT);
            }

        if ((allocatedAdrs = (0x0000ffff & (UINT32)m82xxDpramAlignedMalloc (0x34,64))) != 0)
            *PPC8260_DPR_SMC1(dpram1Base + (0x100 * i)) = allocatedAdrs;
        else
            {
            sysToMonitor (BOOT_NO_AUTOBOOT);
            }


        /* DPRAM address of SMCx parameter RAM. */
        ppcChn[i].uart.pSmc = (SMC *) ( (UINT32)
                                        (*PPC8260_DPR_SMC1(dpram1Base + (0x100 * i)) + ppcChn[i].regBase) );

        /*
         * SMCMR1 for SMC1.
         */

        ppcChn[i].uart.pSmcReg =
        (SMC_REG *) ( (UINT32) SMCMR1( ppcChn[i].regBase + (0x10 * i)) ) ;

        ppcChn[i].pBaud = (VINT32 *) BRGC1(ppcChn[i].regBase + (0x4 * i)) ;
        ppcChn[i].channelMode = 0                                       ;


        /*
         * reset the device.
         */

        smc8260DevInit ( &ppcChn[i] ) ;
        }

#ifdef INCLUDE_SIO_POLL
    smc8260Ioctl(&ppcChn[SIO_POLL_CONSOLE], SIO_MODE_SET, SIO_MODE_POLL);
    _func_consoleOut = (ssize_t (*)(int, char*, size_t))sysSerialPollConsoleOut;
#endif
    }

/******************************************************************************
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
* ERRNO
*
* SEE ALSO: sysHwInit2()
*/ 

void sysSerialHwInit2
    ( 
    void 
    )
    {
#ifdef INCLUDE_TTY_DEV
    UINT32 immrVal = vxImmrGet ();
    int i;

    for (i = 0; i < N_SIO_CHANNELS; i++)
        {
        /* Connect serial interrupts */

        *SMCE1( immrVal + (0x10 * i) ) = *SMCE1( immrVal + (0x10 * i) ) ;

        (void) intConnect ( INUM_TO_IVEC(4 + (1 * i)), (VOIDFUNCPTR) smc8260Int, (int) &ppcChn[i] ) ;

        /*
         * Enable SMCi interrupts.
         */
        *M8260_SIMR_L( immrVal ) |= (0x00001000 >> i);
        } 
#endif  /* INCLUDE_TTY_DEV */
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
* ERRNO
*/

SIO_CHAN * sysSerialChanGet
    (
       int channel		/* serial channel - [0, 1]    */
    )
    {

    /* Check the parameters for validity.             */

    if ( channel >= N_SIO_CHANNELS )
        {
	return( (SIO_CHAN *) ERROR ) ;
        }

    /* Return a pointer to the private space.         */

    return( (SIO_CHAN *) &ppcChn[channel] ) ;
    }

/*******************************************************************************
*
* sysSerialReset - reset the serail device 
*
* This function calls smc8260DevInit, to initialize the serial device
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSerialReset
    ( 
    void 
    )
    {
    int i;

    for (i = 0; i < N_SIO_CHANNELS; i++)
        smc8260DevInit(&ppcChn[i]) ;
    }

#ifdef INCLUDE_SIO_POLL
/*******************************************************************************
*
* sysSerialPollConsoleOut - poll out routine
*
* This function prints by polling.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL int sysSerialPollConsoleOut
    (
    int    arg,
    char * buf,
    int    len
    )
    {
    char c;
    int  bytesOut = 0;

    if ((len <= 0) || (buf == NULL))
        return (0);

    while ((bytesOut < len) && ((c = *buf++) != EOS))
        {
        while (sioPollOutput((SIO_CHAN *) &ppcChn[SIO_POLL_CONSOLE], c)
               == EAGAIN);
        bytesOut++;

        if (c == '\n')
            while (sioPollOutput((SIO_CHAN *) &ppcChn[SIO_POLL_CONSOLE], '\r')
                   == EAGAIN);
        }
    return (bytesOut);
    }
#endif  /* INCLUDE_SIO_POLL */

