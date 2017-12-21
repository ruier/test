/* usrMipcSocket - AF_MIPC sockets init */

/*
 * Copyright (c)    2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,02jun10,als  Updated to MIPC 2.1
01b,10feb10,h_w  updated after code inspection
01a,04jan10,h_w  ported from target/config/comps/src/tipc/usrTipcSocket.c.
*/

#include <sockLib.h>

/*
 * Generate compile error if AF_MIPC CDF settings are invalid
 */
 
#if (AF_MIPC_NUM_SOCKETS < -1) || (AF_MIPC_NUM_SOCKETS > 1048576)
    #error "AF_MIPC_NUM_SOCKETS must be between -1 and 1048576\n"
#endif

/* external declarations */
extern STATUS afMipcPoolInit (int afMipcMaxSockets);
extern SOCK_FUNC * afMipcSockLibInit (void);

/*******************************************************************************
*
* usrMipcSocket.c - AF_MIPC socket configuration
*
* This routine initializes the AF_MIPC component and registers AF_MIPC socket
* backend to the VxWorks networks infrastructure.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void usrMipcSocket (void)
    {
    if (afMipcPoolInit (AF_MIPC_NUM_SOCKETS) == ERROR)
        {
        if (_func_printErr)
            (*_func_printErr) ("usrMipcSocket: afMipcPoolInit failed\n");
        return;
        }

    if (sockLibAdd (afMipcSockLibInit, AF_MIPC, AF_MIPC) == ERROR)
        {
        if (_func_printErr)
            (*_func_printErr) ("usrMipcSocket: sockLibAdd for AF_MIPC failed\n");
        }
    }

