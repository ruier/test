/* wdbNetwork.c - WDB network communication initialization library */

/*
 * Copyright (c) 1998, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use  of this
 * software may be licensed only pursuant to the terms of an applicable Wind
 * River license agreement.
 */

/*
modification history
--------------------
01c,17mar11,rlp  Modified wdbUdpSockIfInit call to pass an IP address.
01b,22may98,dbt	 reworked
01a,11mar98,ms	 written
*/

/*
DESCRIPTION
Initializes the network connection for the WDB agent.

NOMANUAL
*/

/******************************************************************************
*
* wdbCommDevInit - initialize the network connection
*
* This routine initializes the network connection used by the WDB agent.
*
* RETURNS :
* OK or ERROR if the network connection can not be initialized.
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
    char *		ipAddr = NULL;
    static uint_t	wdbInBuf [WDB_MTU/4];
    static uint_t	wdbOutBuf [WDB_MTU/4];

    /* update input & output buffer pointers */

    *ppWdbInBuf = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communication interface mtu */

    wdbCommMtu = WDB_MTU;

    /* UDP sockets - supports a task agent */

#ifdef	INCLUDE_WDB_SSL
    /*
     * If WDB over SSL is turned on, we initialize the socket used to
     * communicate with remote clients to accept packets from the loopback
     * device driver only.
     */

    ipAddr = "127.0.0.1";
#endif	/* INCLUDE_WDB_SSL */

    if (wdbUdpSockIfInit (pCommIf, ipAddr) == ERROR)
	return (ERROR);
    else
	return (OK);
    }
