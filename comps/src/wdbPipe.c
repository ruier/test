/* wdbPipe.c - WDB pipe communication initialization library */

/* Copyright 1998-2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,03sep03,elg  Add control on MTU limits.
01a,18may98,dbt   written based on usrWdb.c ver 02t
*/

/*
DESCRIPTION
Initializes the pipe connection for the WDB agent (VxSim only).

NOMANUAL
*/

/* defines */

#if	WDB_MTU > WDB_PIPE_PKT_MTU
#undef	WDB_MTU
#define	WDB_MTU	WDB_PIPE_PKT_MTU
#endif	/* WDB_MTU > WDB_PIPE_PKT_MTU */

/******************************************************************************
*
* wdbCommDevInit - initialize the pipe connection
*
* This routine initializes the pipe connection used by the WDB agent.
*
* RETURNS :
* OK or ERROR if the pipe connection can not be initialized.
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
    static WDB_PIPE_PKT_DEV	wdbPipePktDev;	/* Pipe packet device */
    static uint_t		wdbInBuf [WDB_MTU/4];
    static uint_t		wdbOutBuf [WDB_MTU/4];

    /* update input & output buffer pointers */

    *ppWdbInBuf = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communication interface mtu */

    wdbCommMtu = WDB_MTU;

    /* pipe packet driver - supports task or external agent */

    if (wdbPipePktDevInit(&wdbPipePktDev, udpRcv) == ERROR)
	return (ERROR);

    if (udpCommIfInit(pCommIf, &wdbPipePktDev.wdbDrvIf) == ERROR)
	return (ERROR);

    return (OK);
    }
