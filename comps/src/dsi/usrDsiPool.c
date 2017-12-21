/* usrDsiPool.c - DSI buffer pool config */

/* Copyright 2004-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01f,08aug12,shi  support to use _ALLOC_ALIGN_SIZE alignment when in 64bit-SMP
                 system. Defect: WIND00359746 (depend on WIND00306857)
01e,04jan05,bwa  added dsiPoolInit() prototype;
                 removed 'Net' part of routine names (SPR#103232).
01d,20aug04,bwa  DSI backend now only needs 1 sockaddr_un size cluster.
01c,12aug04,bwa  bumped the number of sizes in DSI's system pool by 1.
01b,25may04,ann  merged from /main/base6_itn5-int/base6_itn5_kernel-dev.ot/1.
01a,17apr04,bwa  written.
*/

extern STATUS dsiPoolInit (NET_POOL_CONFIG_PARAMS * pPools);

#if defined(_WRS_CONFIG_SMP) && defined (_WRS_CONFIG_LP64)
#define DSI_ALLOC_ALIGN_PLUS    _ALLOC_ALIGN_SIZE
#else
#define DSI_ALLOC_ALIGN_PLUS    0
#endif

#define DSI_NUM_PROTOS   1
#define DSI_NUM_DATA    12
#define DSI_NUM_DOMAIN   1

#define DSI_NUM_SYS     (DSI_NUM_PROTOS + 1)

#define DSI_DATA_TOTAL  (DSI_DATA_32  + \
                         DSI_DATA_64  + \
                         DSI_DATA_128 + \
                         DSI_DATA_256 + \
                         DSI_DATA_512 + \
                         DSI_DATA_1K  + \
                         DSI_DATA_2K  + \
                         DSI_DATA_4K  + \
                         DSI_DATA_8K  + \
                         DSI_DATA_16K + \
                         DSI_DATA_32K + \
                         DSI_DATA_64K)


void usrDsiPoolConfig (void)
    {
    NET_POOL_CONFIG_PARAMS pools;

    /*
     * algorithm for choosing the number of mblk - clblk:
     * number of socket * (number of proto sizes + 1 for the size of sockets)
     * + number of domains for addr size blocks * 1 (only one is needed because
     * address allocation-deallocation is always atomic).
     */

    M_CL_CONFIG sysClCfg =
        {
        DSI_NUM_SOCKETS * DSI_NUM_SYS + DSI_NUM_DOMAIN,
        DSI_NUM_SOCKETS * DSI_NUM_SYS + DSI_NUM_DOMAIN,
        NULL,
        (DSI_NUM_SOCKETS * DSI_NUM_SYS + DSI_NUM_DOMAIN) *
                                                (M_BLK_SZ +sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS) +
            (DSI_NUM_SOCKETS * DSI_NUM_SYS + DSI_NUM_DOMAIN) * (CL_BLK_SZ + DSI_ALLOC_ALIGN_PLUS) +DSI_ALLOC_ALIGN_PLUS
        };

    CL_DESC sysClDesc[] =
        {
            {
            0, DSI_NUM_SOCKETS, NULL, 0     /* sockets */
            },

            {
            0, 1,               NULL, 0     /* sockaddr_un */
            },

            {
            0, DSI_NUM_SOCKETS, NULL, 0     /* COMP */
            }
        };

    M_CL_CONFIG dClCfg =
        {
        DSI_DATA_TOTAL, DSI_DATA_TOTAL, NULL,
        DSI_DATA_TOTAL * (M_BLK_SZ + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS) +
            DSI_DATA_TOTAL * (CL_BLK_SZ + DSI_ALLOC_ALIGN_PLUS) + DSI_ALLOC_ALIGN_PLUS
        };

    CL_DESC dClDesc[] =
        {
            {
               32, DSI_DATA_32, NULL,
            DSI_DATA_32 * (   32 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
               64, DSI_DATA_64, NULL,
            DSI_DATA_64 * (   64 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
              128, DSI_DATA_128, NULL,
            DSI_DATA_128 * (  128 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
              256, DSI_DATA_256, NULL,
            DSI_DATA_256 * (  256 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
              512, DSI_DATA_512, NULL,
            DSI_DATA_512 * (  512 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
             1024, DSI_DATA_1K, NULL,
            DSI_DATA_1K * ( 1024 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
             2048, DSI_DATA_2K, NULL,
            DSI_DATA_2K * ( 2048 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
             4096, DSI_DATA_4K, NULL,
            DSI_DATA_4K * ( 4096 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
             8192, DSI_DATA_8K, NULL,
            DSI_DATA_8K * ( 8192 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
            16384, DSI_DATA_16K, NULL,
            DSI_DATA_16K * (16384 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
            32768, DSI_DATA_32K, NULL,
            DSI_DATA_32K * (32768 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            },

            {
            65536, DSI_DATA_64K, NULL,
            DSI_DATA_64K * (65536 + sizeof (ULONG) + DSI_ALLOC_ALIGN_PLUS)
            }
        };

    pools.cfgh.len         = sizeof (NET_POOL_CONFIG_PARAMS);
    pools.sysMblkClBlkConf = &sysClCfg;
    pools.sysClDesc        = sysClDesc;
    pools.sysClDescNum     = DSI_NUM_SYS + DSI_NUM_DOMAIN;
    pools.pktMblkClBlkConf = &dClCfg;
    pools.pktClDesc        = dClDesc;
    pools.pktClDescNum     = DSI_NUM_DATA;

    dsiPoolInit (&pools);
    }

