/* wdbMipc.c - WDB MIPC communication initialization library */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.  
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
Initializes the MIPC connection for the WDB agent.

NOMANUAL
*/

/******************************************************************************
*
* wdbCommDevInit - initialize the MIPC connection
*
* This routine initializes the MIPC connection used by the WDB agent.
*
* RETURNS :
* OK or ERROR if the MIPC connection can not be initialized.
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
    /*
     * Declare intput and output buffers as integer to make sure they are
     * aligned on a 32 bits address.
     */

    static uint_t	wdbInBuf [WDB_MTU/4];
    static uint_t	wdbOutBuf [WDB_MTU/4];

    /* update input & output buffer pointers */

    *ppWdbInBuf = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communication interface mtu */

    wdbCommMtu = WDB_MTU;

    /* MIPC sockets - supports a task agent */

    if (wdbMipcSockIfInit (pCommIf, WDB_MIPC_BUS_NAME, WDB_MIPC_PORT) == ERROR)
	return (ERROR);
    else
	return (OK);
    }
