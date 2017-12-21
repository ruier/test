/* simSio.c - simulator serial driver */

/*
 * Copyright (c) 2007,2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

/*
modification history
--------------------
01f,04jan11,dbt  Fixed build warning (CQ:WIND00249312).
01e,02jun10,elp  64 bits support.
01d,12jan10,elp  64bits support (host calls).
01c,07oct09,elp  added SIO_DEV_LOCK/SIO_DEV_UNLOCK support (CQ:WIND00183589).
01b,28mar07,elp  fixed doc
01a,02feb07,elp  written from unixSio.c and winSio.c.
*/

/*
DESCRIPTION
This is the driver for the VxSim simulated serial port.

USAGE
A SIM_SIO_CHAN structure is used to describe each channel available.

The BSP's sysHwInit() routine typically calls sysSerial.c:sysSerialHwInit(),
which opens unix tty/pty devices for serial lines & initializes the
UNIX_CHAN u_fd & u_pid fields before calling simSioConsoleDevInit().

The BSP sysSerialHwInit2() calls simSioDevInit2() to enable interrupts.

.CS
i.e.

#include "simSio.h"

SIO_CHAN myChan [NUM_TTY];
SIO_CHAN * sysSioChans[NUM_TTY];

sysSerialHwInit (void)
    {
    ...
    for (ix = 0; ix < NUM_TTY; ix++)
        {        
        if (ix > 0)     // dev 0 is simConsole sdtin/out/err //
	    {
	    UNIX_CHAN * pChan = &myChan[ix];
	    sysSioChans[ix] = (SIO_CHAN *) pChan;
	    pChan->u_fd = ptyXtermOpen (ptyName, &pChan->u_pid, 0);
	    }
        ...
        simSioDevInit (&myChan);
        }
    }

sysSerialHwInit2 (void)
    {
    ...
    for (i = 0; i < NUM_TTY; i++)
        intConnect (myChan[i]->intRcvVec, simSioIntRcv, (int)&myChan[i]);
    ...
    }
.CE


INCLUDE FILES: simSio.h sioLib.h
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <sioLib.h>
#include <intLib.h>
#include <errno.h>
#include <selectLib.h>
#include <sys/times.h>
#include <simSio.h>
#include <vxsimHostLib.h>
#ifdef _WRS_CONFIG_SMP
#include <private/spinLockLibP.h>
#endif /* _WRS_CONFIG_SMP */

/* defines */

#define SIO_RCVBUF_SIZE 1024		/* receive buffer size */

#define SIO_SNDBUF_SIZE	2048		/* transmit buffer size */

#define SIM_SIO_BAUD_MIN    50
#define SIM_SIO_BAUD_MAX    38400

#ifdef  _WRS_CONFIG_SMP
LOCAL BOOL simSioSpinlockFuncReady = FALSE;

/* take sio spinlock allowing reentrancy if already owned */

#define SIM_SIO_SPIN_LOCK_ISR_TAKE_R(pChan,owned)		\
    owned = FALSE;						\
    if (simSioSpinlockFuncReady)				\
        {							\
	if (spinLockIsrHeld(&pChan->spinlockIsr))		\
	    owned = TRUE;					\
	else							\
	    SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr);		\
	}

#define SIM_SIO_SPIN_LOCK_ISR_TAKE(pChan)			\
    if (simSioSpinlockFuncReady)				\
	SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr);
	
#define SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan,owned)		\
    if (simSioSpinlockFuncReady)				\
        {							\
	if (!owned)						\
	    SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr);		\
	}

#define SIM_SIO_SPIN_LOCK_ISR_GIVE(pChan)			\
    if (simSioSpinlockFuncReady)				\
	SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr)

#define SIM_SIO_ISR_SET(pChan)					\
    SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr)
#define SIM_SIO_ISR_CLEAR(pChan)				\
    SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr)
#define SIM_SIO_SPIN_LOCK_READY					\
    simSioSpinlockFuncReady = TRUE

#else /* _WRS_CONFIG_SMP */

#define SIM_SIO_SPIN_LOCK_ISR_TAKE_R(pChan,owned)		\
    owned = FALSE;						\
    SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr)

#define SIM_SIO_SPIN_LOCK_ISR_TAKE(pChan,owned)			\
    SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr)

#define SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan,owned)		\
    SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr)

#define SIM_SIO_SPIN_LOCK_ISR_GIVE(pChan,owned)			\
    SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr)

#define SIM_SIO_SPIN_LOCK_ISR_HELD(pChan)    FALSE
#define SIM_SIO_ISR_SET(pChan)
#define SIM_SIO_ISR_CLEAR(pChan)
#define SIM_SIO_SPIN_LOCK_READY
#endif  /* _WRS_CONFIG_SMP */

/* forward static declarations */

static int	simSioModeSet (SIM_SIO_CHAN * pSioChan, int mode);
static int	simSioTxStartup (SIO_CHAN * pSioChan);
static int	simSioCallbackInstall (SIO_CHAN *pSioChan, int callbackType,
				       STATUS (*callback)(), void *callbackArg);
static int	simSioPollOutput (SIO_CHAN *pSioChan, char outChar);
static int	simSioPollInput (SIO_CHAN *pSioChan, char *thisChar);
static int	simSioIoctl (SIO_CHAN *pSioChan, int request, void *arg);
static STATUS	dummyCallback (void);

/* local variables */

static SIO_DRV_FUNCS simSioDrvFuncs =
    {
    (int (*)(SIO_CHAN *, int, void *)) simSioIoctl,
    (int (*)(SIO_CHAN *)) simSioTxStartup,
    (int (*)()) simSioCallbackInstall,
    (int (*)(SIO_CHAN *, char *)) simSioPollInput,
    (int (*)(SIO_CHAN *, char)) simSioPollOutput
    };

/******************************************************************************
*
* simSioDevInit - initialize a sio device
*
* This routine initializes the driver function pointers and then
* resets to a quiescent state.  The BSP must have already opened all
* the file descriptors in the structure before passing it to this
* routine.
*
* RETURNS: N/A
*/

void simSioDevInit
    (
    SIM_SIO_CHAN * pChan	/* sio channel */
    )
    {
    /* initialize each channel's driver function pointers */

    pChan->sio.pDrvFuncs	= &simSioDrvFuncs;

    /* install dummy driver callbacks */

    pChan->getTxChar		= dummyCallback;
    pChan->putRcvChar		= dummyCallback;
    
    /* setting polled mode is one way to make the device quiet */

    pChan->mode			= 0;	/* not valid */

    pChan->hostSioId = vxsimHostSioDevFind (pChan->desc);
    pChan->intVec = vxsimHostSioIntVecGet (pChan->hostSioId);
    pChan->availModes = SIO_MODE_INT | SIO_MODE_POLL;

    SPIN_LOCK_ISR_INIT (&pChan->spinlockIsr, 0);

    pChan->created		= 1;
    }

/******************************************************************************
*
* simSioDevInit2 - enable console interrupts
*
* If called at sysHwInit2 time, this routine will enable the device to
* operate in interrupt mode.  If this routine is not called, the device
* will be limited to polled mode only.
*
* RETURNS: N/A
*/

void simSioDevInit2
    (
    SIM_SIO_CHAN * pChan	/* sio channel */
    )
    {
    /* MMU is already enabled when it comes here and safe to use spinlock. */

    SIM_SIO_SPIN_LOCK_READY;

    if ((pChan != NULL) && (pChan->created != 1))
	{
	simSioModeSet (pChan, SIO_MODE_INT);
	}
    }
    
/******************************************************************************
*
* simSioIntRcv - handle a channel's receive-character interrupt
*
* This routine is the device receive interrupt service routine.
*
* RETURNS: N/A
*/ 

void simSioIntRcv
    (
    SIM_SIO_CHAN *	pChan	/* sio channel generating the interrupt */
    )
    {
    char buf[SIO_RCVBUF_SIZE];
    int nread;
    int ix;

    /*
     *  Read characters into buffer, then pass them one at a time
     *  to tyIRd.
     */

    while ((nread =
    	    vxsimHostSioRead (pChan->hostSioId, buf, SIO_RCVBUF_SIZE)) > 0)
	{
        for (ix = 0; ix < nread; ix++)
	    {
	    (*pChan->putRcvChar) (pChan->putRcvArg, buf[ix]);
	    }
        }
    }

/******************************************************************************
*
* simSioTxStartup - start the interrupt transmitter
*
* This routine will start the transmitter.  Transmitter interrupts will
* fetch characters until the buffer is empty.
*
* RETURNS: 
* Returns OK on success, ENOSYS if the device is polled-only, or
* EIO on hardware error.
*/

static int simSioTxStartup
    (
    SIO_CHAN * pSioChan		/* sio channel to start */
    )
    {
    SIM_SIO_CHAN *	pChan = (SIM_SIO_CHAN *)pSioChan;
    char		buf [SIO_SNDBUF_SIZE];
    char *		pbuf;
    ssize_t		t;
    size_t		tt;
    int			ix;
    BOOL		spinLockOwn = FALSE;

    if (pChan == NULL)
	return (EIO);

    do
	{
	/* get buffer */
	for (ix = 0; ix < sizeof (buf); ix++)
	    {
	    if ((*pChan->getTxChar) (pChan->getTxArg, &buf[ix]) != OK)
		{
		break;
		}
	    }
	pbuf = buf;
	tt = ix;
	
	if (tt != 0)
	    {
	    /* transmit buffer */
	    
	    SIM_SIO_SPIN_LOCK_ISR_TAKE_R(pChan,spinLockOwn);

	    while ((t = vxsimHostSioWrite (pChan->hostSioId, pbuf, tt)) != tt)
		{
		if (t >= 0)
		    {
		    tt -= t;
		    pbuf += t;
		    }
		else
		    {
		    SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan, spinLockOwn);
		    return (EIO);
		    }
		}
	    SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan, spinLockOwn);
	    }
	}
    while (ix == sizeof (buf));

    return (OK);
    }

/******************************************************************************
*
* simSioCallbackInstall - install ISR callbacks to get/put chars
*
* This driver allows interrupt callbacks for transmitting characters
* and receiving characters. In general, drivers may support other
* types of callbacks too.
*
* RETURNS: OK on success, or ENOSYS for an unsupported callback type.
*/ 

static int simSioCallbackInstall
    (
    SIO_CHAN *	pSioChan,               /* sio channel */
    int		callbackType,           /* type of callback */
    STATUS	(*callback)(),          /* callback */
    void *      callbackArg             /* parameter to callback */
    )
    {
    SIM_SIO_CHAN * pChan = (SIM_SIO_CHAN *) pSioChan;

    switch (callbackType)
	{
	case SIO_CALLBACK_GET_TX_CHAR:
	    pChan->getTxChar	= callback;
	    pChan->getTxArg	= callbackArg;
	    return (OK);
	case SIO_CALLBACK_PUT_RCV_CHAR:
	    pChan->putRcvChar	= callback;
	    pChan->putRcvArg	= callbackArg;
	    return (OK);
	default:
	    return (ENOSYS);
	}

    }

/******************************************************************************
*
* simSioPollOutput - output a character in polled mode
*
* RETURNS: 
* Returns OK if a character arrived, EIO on device error, EAGAIN
* if the output buffer if full. ENOSYS if the device is
* interrupt-only.
*/

static int simSioPollOutput
    (
    SIO_CHAN *	pSioChan,	/* sio channel */
    char	outChar		/* character to send */
    )
    {
    SIM_SIO_CHAN * pChan = (SIM_SIO_CHAN *) pSioChan;
    ssize_t ret = 0;

    ret = vxsimHostSioWrite (pChan->hostSioId, &outChar, 1);

    if (ret == -1)
	return (EIO);
    else if (ret == 0)
	return (EAGAIN);

    return OK;
    }

/******************************************************************************
*
* simSioPollInput - poll the device for input
*
* RETURNS:
* Returns OK if a character arrived, EIO on device error, EAGAIN
* if the input buffer if empty, ENOSYS if the device is
* interrupt-only.
*/

static int simSioPollInput
    (
    SIO_CHAN *	pSioChan,	/* sio channel */
    char *	pChar		/* where to store read character */
    )
    {
#if CPU==SIMNT
    return (ENOSYS);
#else
    SIM_SIO_CHAN * pChan = (SIM_SIO_CHAN *) pSioChan;
    ssize_t ret;

    if ((ret = vxsimHostSioRead (pChan->hostSioId, pChar, 1)) == -1)
	return (EIO);
    else if (ret == 0)
	return (EAGAIN);

    return (OK);
#endif
    }

/******************************************************************************
*
* simSioModeSet - change channel mode setting
*
* This driver supports both polled and interrupt modes and is capable of
* switching between modes dynamically.
*
* RETURNS: 0 on succes, error code (EIO) otherwise
*/

static int simSioModeSet
    (
    SIM_SIO_CHAN * 	pChan,	/* pointer to channel */
    int			mode	/* mode requested */
    )
    {
    int status = OK;
    BOOL spinLockOwn = FALSE;

    if ((mode != SIO_MODE_POLL) && (mode != SIO_MODE_INT)) 
	{
	return (EIO);
	}

    /* test if spinlock is already owned by the current CPU core */

    SIM_SIO_SPIN_LOCK_ISR_TAKE_R(pChan,spinLockOwn);

    if (mode == SIO_MODE_INT)
	{
	if (pChan->mode != SIO_MODE_INT)
	    {
	    if (vxsimHostSioModeSet (pChan->hostSioId, 1) == OK)
		pChan->mode = mode;
	    else
		status = EIO;
	    }
	}
    else 
	{
	if (pChan->mode != SIO_MODE_POLL)
	    {
	    if (vxsimHostSioModeSet (pChan->hostSioId, 0) == OK)
		pChan->mode = mode;
	    else
		status = EIO;
	    }
	}

    SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan, spinLockOwn);
    return status;
    }

/*******************************************************************************
*
* simSioIoctl - special device control
*
* This routine is used to perform special device control operation.
*
* RETURNS: 
* Returns OK on success, ENOSYS on unsupported request, EIO on failed
* request.
*/

static int simSioIoctl
    (
    SIO_CHAN *	pSioChan,		/* device to control */
    int		request,		/* request code */
    void *	someArg			/* some argument */
    )
    {
    SIM_SIO_CHAN *	pChan = (SIM_SIO_CHAN *) pSioChan;
    int	 		arg = (int)(long)someArg;
    STATUS		status = OK;
    BOOL		spinLockOwn = FALSE;

    switch (request)
	{
	case SIO_BAUD_SET:
	    /*
	     * Set the baud rate. Return EIO for an invalid baud rate, or
	     * OK on success.
	     */

	    if (arg < SIM_SIO_BAUD_MIN || arg > SIM_SIO_BAUD_MAX)
	        {
		return (EIO);
	        }

	    SIM_SIO_SPIN_LOCK_ISR_TAKE_R(pChan,spinLockOwn);

	    if (vxsimHostSioBaudRateSet (pChan->hostSioId, arg) == OK)
		pChan->baudRate = arg;

	    SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan,spinLockOwn);
	    return (OK);

	case SIO_BAUD_GET:
	    /* Get the baud rate and return OK */

	    *(int *)someArg = pChan->baudRate;
	    return (OK);

	case SIO_MODE_SET:
	    /*
	     * Set the mode (e.g., to interrupt or polled). Return OK
	     * or EIO for an unknown or unsupported mode.
	     */
	    
	    status = simSioModeSet (pChan, arg);
	    break;

	case SIO_MODE_GET:
	    /* Get the current mode and return OK. */

	    *(int *)someArg = pChan->mode;
	    return (OK);

	case SIO_AVAIL_MODES_GET:
	    /* Get the available modes and return OK. */

	    *(int *)someArg = pChan->availModes;
	    return (OK);

	case SIO_HW_OPTS_SET:
	    /*
	     * Optional command to set the hardware options (as defined
	     * in sioLib.h).
	     * Return OK, or ENOSYS if this command is not implemented.
	     * Note: several hardware options are specified at once.
	     * This routine should set as many as it can and then return
	     * OK. The SIO_HW_OPTS_GET is used to find out which options
	     * were actually set.
	     */

	    return (ENOSYS);

	case SIO_HW_OPTS_GET:
	    /*
	     * Optional command to get the hardware options (as defined
	     * in sioLib.h).
	     * Return OK or ENOSYS if this command is not implemented.
	     * Note: if this command is unimplemented, it will be
	     * assumed that the driver options are CREAD | CS8 (e.g.,
	     * eight data bits, one stop bit, no parity, ints enabled).
	     */

	    return (ENOSYS);

	case SIO_OPEN:
	    SIM_SIO_SPIN_LOCK_ISR_TAKE_R(pChan,spinLockOwn);
	    if (vxsimHostSioOpen (pChan->hostSioId) == ERROR)
		status = EIO;
	    SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan,spinLockOwn);
	    break;

	case SIO_HUP:
	    SIM_SIO_SPIN_LOCK_ISR_TAKE_R(pChan,spinLockOwn);
	    if (vxsimHostSioClose (pChan->hostSioId) == ERROR)
		status = EIO;
	    SIM_SIO_SPIN_LOCK_ISR_GIVE_R(pChan,spinLockOwn);
	    break;

#ifdef _WRS_CONFIG_SMP
	case SIO_DEV_LOCK:
	    SIM_SIO_SPIN_LOCK_ISR_TAKE(pChan);
	    break;

	case SIO_DEV_UNLOCK:
	    SIM_SIO_SPIN_LOCK_ISR_GIVE(pChan);
	    break;
#endif /* _WRS_CONFIG_SMP */

	default:
	    return (ENOSYS);
	}

    return (status);
    }

/*******************************************************************************
*
* dummyCallback - dummy callback routine
*
* RETURNS: Returns ERROR always.
*/

LOCAL STATUS dummyCallback (void)
    {
    return (ERROR);
    }

