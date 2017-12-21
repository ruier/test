/* usrWdbProxy.c - configuration file for the WDB Proxy Agent */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.  
 *
 * The right to copy, distribute, modify or otherwise make use  of this 
 * software may be licensed only pursuant to the terms of an applicable Wind 
 * River license agreement.
 */

/*
modification history
--------------------
01d,20dec11,rlp  CQID: Fix WIND00323886 - Added VX_FP_TASK option for e500v2
                 SSL support.
01c,11may11,rlp  Added initialization of SSL shell commands.
01b,25mar11,rlp  Added SSL support.
01a,17mar11,rlp  Written.
*/

/*
DESCRIPTION

This library configures and initializes the WDB Proxy Agent.
*/

#include <vsbConfig.h>

/******************************************************************************
*
* usrWdbProxyCfgInit - configure and initialize the WDB Proxy Agent.
*
*/

STATUS usrWdbProxyCfgInit
    (
    UINT16	portNumber,		/* proxy port number */
    char *	initString,		/* proxy initialization string */
    BOOL	hasControlPort,		/* proxy has control port enabled? */
    UINT32	priority,		/* proxy task priority */
    UINT32	stackSize,		/* proxy task stack size */
    UINT32	options			/* proxy task options */
    )
    {
    BOOL	useSsl = FALSE;

#ifdef	INCLUDE_WDB_PROXY_SSL
    useSsl = TRUE;

    if (wrProxySslLibInit (WDB_PROXY_SSL_REPOSITORY) == ERROR)
    	return ERROR;

#ifdef	INCLUDE_SHELL_INTERP_CMD
    if (agentSslShellCmdInit () == ERROR)
    	return ERROR;
#endif	/* INCLUDE_SHELL_INTERP_CMD */

#ifdef	INCLUDE_SPE
    /* e500v2 SSL support uses SPE instructions. */

    options |= VX_FP_TASK;
#endif	/* INCLUDE_SPE */
#endif	/* INCLUDE_WDB_PROXY_SSL */

    return (wrProxyLibInit (portNumber, initString, useSsl,
    				hasControlPort, priority, stackSize, options));
    }

