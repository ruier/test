/* wdbSerial.c - WDB serial communication initialization library */

/*
 * Copyright (c) 1998-2005,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,29apr10,jmp  Updated to run wdbSioTest in SIO_MODE_POLL in small footprint
		 configuration while SIO driver does not support interrupt mode
		 (CQ:WIND00204027).
01e,28apr10,jmp  Updated wdbSlipPktDevInit() call to pass input buffer for the
		 packet device.
01d,07sep05,bpn  Fixed build warning (SPR#109608).
01c,13sep01,jhw  Fixed sysSerialChanGet return value test (SPR 70225).
01b,22may98,dbt  reworked
01a,11mar98,ms   written
*/

/*
DESCRIPTION
Initializes the serial connection for the WDB agent.

NOMANUAL
*/

/* lower WDB_MTU to SLMTU bytes for serial connection */

#if     WDB_MTU > SLMTU
#undef  WDB_MTU
#define WDB_MTU SLMTU
#endif  /* WDB_MTU > SLMTU */

/******************************************************************************
*
* wdbCommDevInit - initialize the serial connection
*
* This routine initializes the serial connection used by the WDB agent.
*
* RETURNS :
* OK or ERROR if the serial connection can not be initialized.
*
* NOMANUAL
*/

STATUS wdbCommDevInit
    (
    WDB_COMM_IF *	pCommIf,
    char ** 		ppWdbInBuf,
    char **		ppWdbOutBuf
    )
    {
    SIO_CHAN *		pSioChan;		/* serial I/O channel */
    static WDB_SLIP_PKT_DEV wdbSlipPktDev;	/* SLIP packet device */
    static uint_t	wdbInBuf [WDB_MTU/4];	/* WDB RPC Input array */
						/* (must be 4bytes aligned) */
    static uint_t	wdbOutBuf [WDB_MTU/4];	/* WDB RPC Output array */
    						/* (must be 4bytes aligned) */
    static char	slipDevInputBuffer [WDB_MTU];	/* SLIP device input buffer */

    /* update input & output buffer pointers */

    *ppWdbInBuf = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communcation interface mtu */

    wdbCommMtu = WDB_MTU;

    /* a raw serial channel supports task or external agent */

    pSioChan = sysSerialChanGet (WDB_TTY_CHANNEL);

    if (pSioChan == (SIO_CHAN *) ERROR)
	{
	if (_func_printErr != NULL)
	    _func_printErr ("wdbSerialInit error: bad serial channel %d\n",
		WDB_TTY_CHANNEL);
	return (ERROR);
	}

    sioIoctl (pSioChan, SIO_BAUD_SET, (void *)WDB_TTY_BAUD);

#ifdef  INCLUDE_WDB_TTY_TEST
    {
#if WDB_TTY_ECHO == TRUE
    int waitChar = 0333;
#else	/* WDB_TTY_ECHO == FALSE */
    int	waitChar = 0;
#endif	/* WDB_TTY_ECHO == TRUE */

#ifdef	INCLUDE_KERNEL
#ifdef	_WRS_CONFIG_PROFILE_SMALL_FOOTPRINT
	/*
	 * XXX JMP - For now, in small footprint configuration the SIO drivers
	 * do not support interrupt mode (See CQ:WIND00204027): wdbSioTest must
	 * be peformed in SIO_MODE_POLL mode.
	 */

	wdbSioTest (pSioChan, SIO_MODE_POLL, waitChar);
#else	/* _WRS_CONFIG_PROFILE_SMALL_FOOTPRINT */
    /* test in polled mode if the kernel hasn't started */

    if (taskIdCurrent == 0)
	wdbSioTest (pSioChan, SIO_MODE_POLL, waitChar);
    else
	wdbSioTest (pSioChan, SIO_MODE_INT, waitChar);
#endif	/* _WRS_CONFIG_PROFILE_SMALL_FOOTPRINT */
#else	/* INCLUDE_KERNEL */
    wdbSioTest (pSioChan, SIO_MODE_POLL, waitChar);
#endif	/* INCLUDE_KERNEL */
    }
#endif  /* INCLUDE_WDB_TTY_TEST */

    wdbSlipPktDevInit (&wdbSlipPktDev, pSioChan, udpRcv,
		       slipDevInputBuffer, WDB_MTU);

    if (udpCommIfInit (pCommIf, &wdbSlipPktDev.wdbDrvIf) == ERROR)
	return (ERROR);

    return (OK);
    }
