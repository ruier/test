/* wdbTipc.c - WDB tipc communication initialization library */

/*
 * Copyright (c) 2005 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01a,30may05,dbt	 written
*/

/*
DESCRIPTION
Initializes the TIPC connection for the WDB agent.

NOMANUAL
*/

/******************************************************************************
*
* wdbCommDevInit - initialize the TIPC connection
*
* This routine initializes the TIPC connection used by the WDB agent.
*
* RETURNS :
* OK or ERROR if the TIPC connection can not be initialized.
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
    static uint_t	wdbInBuf [WDB_MTU/4];
    static uint_t	wdbOutBuf [WDB_MTU/4];

    /* update input & output buffer pointers */

    *ppWdbInBuf = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communication interface mtu */

    wdbCommMtu = WDB_MTU;

    /* TIPC sockets - supports a task agent */

    if (wdbTipcSockIfInit (pCommIf, WDB_TIPC_PORT_TYPE,
    				WDB_TIPC_PORT_INSTANCE) == ERROR)
	return (ERROR);
    else
	return (OK);
    }
