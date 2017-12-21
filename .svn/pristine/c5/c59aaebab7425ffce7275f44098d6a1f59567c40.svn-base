/* usrTipcPool.c - TIPC buffer pool config */

/*
 * Copyright (c) 2005-2006, 2008 Wind River Systems, Inc.
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
01k,23oct09,h_x  WIND00183237
01j,08oct08,als  Update to TIPC 1.7.6 (Defect WIND00127984)
01i,09mar06,ebh  changes to comments as per code review
01h,21feb06,ebh  Revert maximum cluster size back to 65536 after problems found
01g,20feb06,mze  removing 32 byte buffers and increasing 65536 to 66076.
01f,13feb06,bwa  scaled down system pool to only support socket-sized buffers.
01e,29sep05,dsk  tuned numbers of system clusters
01d,15sep05,ebh  extra buffers required by drivers for TIPC only builds
01c,26aug05,ebh  temporary fix to point _pNetDpool to _pTipcDpool if TIPC_ONLY
01b,20jul05,dsk  fixed configlette issue, also mods as per code inspection
01a,07jun05,dsk  ported from target/config/comps/src/dsi/usrDsiPool.c.
*/

extern STATUS tipcPoolInit (NET_POOL_CONFIG_PARAMS * pPools);
IMPORT NET_POOL_ID _pNetDpool;		/* networking pool reference */
IMPORT NET_POOL_ID _pTipcDpool;		/* Tipc Data pool reference */

#define TIPC_NUM_DATA    11
#define TIPC_DATA_TOTAL (TIPC_DATA_00064 + \
                         TIPC_DATA_00128 + \
                         TIPC_DATA_00256 + \
                         TIPC_DATA_00512 + \
                         TIPC_DATA_01024 + \
                         TIPC_DATA_02048 + \
                         TIPC_DATA_04096 + \
                         TIPC_DATA_08192 + \
                         TIPC_DATA_16384 + \
                         TIPC_DATA_32768 + \
                         TIPC_DATA_65536)

/* TIPC_NUM_SOCKETS must be declared for the calculations that follow */

#ifndef TIPC_NUM_SOCKETS
#define TIPC_NUM_SOCKETS 1
#endif

/*******************************************************************************
*
* usrTipcPoolConfig - creates a TIPC pool of system and data buffers
*
* This routine creates the TIPC pool of system and data buffers. 
* Number of system buffers depends on number of TIPC sockets configured 
*   (parameter TIPC_NUM_SOCKETS defined for the INCLUDE_TIPC_MEMPOOL component)
* Number of data buffers depends on number various sized buffers configured   
*   (parameter TIPC_DATA_00064, TIPC_DATA_00128 etc. defined for the 
*   INCLUDE_TIPC_MEMPOOL component)
*
* INTERNAL
* This routine cannot use the TIPC output macros since it is called
* before the debugging subsystem is initialized.
*
* RETURNS: N/A
*/

void usrTipcPoolConfig (void)
{
    NET_POOL_CONFIG_PARAMS pools;
    STATUS status;

    /*
     * The System Pool is comprised of only one size (socket size) 
     *
     * Fields include: number of M-blocks, number of cluster blocks
     * pointer to pre-allocated space, and total memory size.
     */

    M_CL_CONFIG sysClCfg =
        {
        TIPC_NUM_SOCKETS,
        TIPC_NUM_SOCKETS,
        NULL,
        TIPC_NUM_SOCKETS * (M_BLK_SZ +sizeof (ULONG)) +
            TIPC_NUM_SOCKETS * CL_BLK_SZ
        };

    CL_DESC sysClDesc[] =
        {
            {
            0, TIPC_NUM_SOCKETS, NULL, 0     /* sockets */
            }
        };

    M_CL_CONFIG dClCfg =
        {
        2*TIPC_DATA_TOTAL, TIPC_DATA_TOTAL, NULL,
        2*TIPC_DATA_TOTAL * (M_BLK_SZ + sizeof (ULONG)) +
            TIPC_DATA_TOTAL * CL_BLK_SZ
        };



    /*
     * The Data Pool is comprised of many sizes
     *
     * Fields include: cluster type, number of clusters,
     * pointer to pre-allocated space, and total memory size
     */

    CL_DESC dClDesc[] =
        {
            {
               64, TIPC_DATA_00064, NULL,
            TIPC_DATA_00064 * (   64 + sizeof (ULONG))
            },

            {
              128, TIPC_DATA_00128, NULL,
            TIPC_DATA_00128 * (  128 + sizeof (ULONG))
            },

            {
              256, TIPC_DATA_00256, NULL,
            TIPC_DATA_00256 * (  256 + sizeof (ULONG))
            },

            {
              512, TIPC_DATA_00512, NULL,
            TIPC_DATA_00512 * (  512 + sizeof (ULONG))
            },

            {
             1024, TIPC_DATA_01024, NULL,
            TIPC_DATA_01024 * ( 1024 + sizeof (ULONG))
            },

            {
             2048, TIPC_DATA_02048, NULL,
            TIPC_DATA_02048 * ( 2048 + sizeof (ULONG))
            },

            {
             4096, TIPC_DATA_04096, NULL,
            TIPC_DATA_04096 * ( 4096 + sizeof (ULONG))
            },

            {
             8192, TIPC_DATA_08192, NULL,
            TIPC_DATA_08192 * ( 8192 + sizeof (ULONG))
            },

            {
            16384, TIPC_DATA_16384, NULL,
            TIPC_DATA_16384 * (16384 + sizeof (ULONG))
            },

            {
            32768, TIPC_DATA_32768, NULL,
            TIPC_DATA_32768 * (32768 + sizeof (ULONG))
            },

            {
            65536, TIPC_DATA_65536, NULL,
            TIPC_DATA_65536 * (65536 + sizeof (ULONG))
            }
        };

    pools.cfgh.len         = sizeof (NET_POOL_CONFIG_PARAMS);
    pools.sysMblkClBlkConf = &sysClCfg;
    pools.sysClDesc        = sysClDesc;
    pools.sysClDescNum     = 1;	/* sockets only */
    pools.pktMblkClBlkConf = &dClCfg;
    pools.pktClDesc        = dClDesc;
    pools.pktClDescNum     = TIPC_NUM_DATA;

    status = tipcPoolInit (&pools);
    if (status != OK) 
    {
        printf("\n usrTipcPoolConfig: Failed to initialize TIPC buffers \n");
        /* TBD should we use _func_printErr at this early stage, or is 
            printf already available; it seems to be */
        if (_func_printErr)
            (*_func_printErr) ("usrTipcPoolConfig: failed tipcPoolInit \n");
    }

#ifdef INCLUDE_TIPC_ONLY
    /* Temporary fix to initialize the networking buffers */
    _pNetDpool = _pTipcDpool;
#endif
}

