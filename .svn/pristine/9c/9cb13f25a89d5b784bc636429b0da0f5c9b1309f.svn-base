/* wdbTyco.c - WDB serial communication initialization library */

/* Copyright 1998-2003 Wind River Systems, Inc. */

/*
 * Copyright (c) 1998,2003,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,29apr10,jmp  Updated wdbSlipPktDevInit() call to pass input buffer for the
                 packet device.
01c,03sep03,elg  Check wdbTyCoDevInit() return value.
01b,22may98,dbt  reworked
01a,11mar98,ms   written
*/

/*
DESCRIPTION
For obsolete tyCoDrv style serial drivers.

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
* This routine initializes the serial connection for tyCoDrv style serial
* drivers used by the WDB agent.
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
    static WDB_TYCO_SIO_CHAN	tyCoSioChan;	/* serial I/O device */
    static WDB_SLIP_PKT_DEV	wdbSlipPktDev;	/* SLIP packet device */
    static uint_t	wdbInBuf [WDB_MTU/4];	/* WDB RPC Input array */
						/* (must be 4bytes aligned) */
    static uint_t	wdbOutBuf [WDB_MTU/4];	/* WDB RPC Output array */
						/* (must be 4bytes aligned) */
    static char	slipDevInputBuffer [WDB_MTU];	/* SLIP device input buffer */

    /* update input & output buffer pointers */

    *ppWdbInBuf = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communication interface mtu */

    wdbCommMtu = WDB_MTU;

    /* an old VxWorks serial driver - supports a task agent */

    if (wdbTyCoDevInit (&tyCoSioChan, WDB_TTY_DEV_NAME, WDB_TTY_BAUD) != OK)
    	return ERROR;

#ifdef  INCLUDE_WDB_TTY_TEST
    wdbSioTest ((SIO_CHAN *)&tyCoSioChan, SIO_MODE_INT, 0);
#endif  /* INCLUDE_WDB_TTY_TEST */

    wdbSlipPktDevInit (&wdbSlipPktDev, (SIO_CHAN *)&tyCoSioChan, udpRcv,
		       slipDevInputBuffer, WDB_MTU);

    if (udpCommIfInit (pCommIf, &wdbSlipPktDev.wdbDrvIf) == ERROR)
	return (ERROR);

    return (OK);
    }
