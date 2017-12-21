/* usrTipcSocket.c - TIPC sockets init */

/*
 * Copyright (c) 2005-2008 Wind River Systems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
modification history
--------------------
01i,01jun09,dlk  LP64 support & POSIXification of socket APIs
01i,08oct08,als  Update to TIPC 1.7.6 (Defect WIND00127984)
01h,24jan07,mze  removed extern of tipc_sock_rxq_limit
01g,01dec06,mze  added INCLUDE_CONFIG_TIPC_SOCKET_API for footprint reduction
01f,13mar06,ebh  added tipc_sock_rxq_limit initialization
01e,17feb06,bwa  removed call to addDomain().
01d,19aug05,ebh  eliminate unused parameters and functions
01c,29jul05,dsk  code inspection changes
01b,20jul05,dsk  fixed configlette issue, also mods as per code inspection
01a,07jun05,dsk  ported from target/config/comps/src/dsi/usrDsiSocket.c.
*/


/* includes */

/* external declarations */
struct sockFunc;

extern struct sockFunc *  tipcSockLibInit (void);
#ifdef INCLUDE_CONFIG_TIPC_SOCKET_API

extern STATUS       sockLibAdd (struct sockFunc * (*sockLibInitRtn) (void),
				int domainMap, int domainReal);

/* defines */

#ifndef TIPC_SOCK_RXQ_LIMIT
#define TIPC_SOCK_RXQ_LIMIT 2500
#endif


/* globals */

unsigned int tipc_sock_rxq_limit = TIPC_SOCK_RXQ_LIMIT;

#endif


/*******************************************************************************
*
* usrTipcSocket.c - TIPC socket configuration
*
* This routine adds TIPC socket backend for AF_TIPC.
*
* RETURNS: N/A
*
* NOMANUAL
*/
                    
void usrTipcSocket (void)
    {
#ifdef INCLUDE_CONFIG_TIPC_SOCKET_API
    if (sockLibAdd (tipcSockLibInit, AF_TIPC, AF_TIPC) == ERROR)
        {
        if (_func_printErr)
            (*_func_printErr) ("usrTipcSocket: failed sockLibAdd for AF_TIPC\n");
        }
#endif
    }

