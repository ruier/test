/* simSio.h - header file for simulator serial driver */

/*
 * Copyright (c) 2007,2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */


/*
modification history
--------------------
01b,07oct09,elp  added SIO_DEV_LOCK/SIO_DEV_UNLOCK support (CQ:WIND00183589).
01a,05feb07,elp  merged unix and windows
*/

#ifndef __INCsimSioh
#define __INCsimSioh

#include <sioLib.h>
#include <spinLockLib.h>

/* defines */

#define SIM_SIO_DESC_MAX_LEN	30

/* device and channel structures */

typedef struct
    {
    /* always goes first */

    SIO_CHAN		sio;		/* driver functions */

    /* callbacks */

    STATUS		(*getTxChar) ();
    STATUS		(*putRcvChar) ();
    void *		getTxArg;
    void *		putRcvArg;
 
    void *		hostSioId;	/* host sio descriptor */
    int			intVec;		/* interrupt vector */
    
    BOOL		created;	
    int			availModes;	/* available modes */
    int			mode;		/* current mode (interrupt or poll) */
    int			baudRate;	/* input clock frequency */
    
    /* host sio driver description */

    char		desc[SIM_SIO_DESC_MAX_LEN];

    spinlockIsr_t	spinlockIsr;	/* ISR-callable spinlock */
    } SIM_SIO_CHAN;

/* definitions */

/* function prototypes */

extern void 	simSioDevInit (SIM_SIO_CHAN * pChan); 
extern void 	simSioDevInit2 (SIM_SIO_CHAN * pChan); 
extern void	simSioIntRcv (SIM_SIO_CHAN * pChan);
#endif  /* __INCsimSioh */
