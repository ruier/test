/* 22comp_mipc.cdf - configuration for MCB/MSD/MIPC */

/*
 * Copyright (c) 2010-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,05jul11,zhw  Removed unnecessary MSD_CFG_STR parameter (WIND00279265)
01a,12feb10,syt  written.
*/

Folder FOLDER_MIPC {
    _CHILDREN   FOLDER_MULTIOS
}

Component INCLUDE_MIPC_UNSUPPORTED {
    _CHILDREN   FOLDER_NOT_VISIBLE
}

/*
 * MIPC_SM_SYSTEM_POOL_BASE, MIPC_SM_SYSTEM_POOL_SIZE, and MIPC_SM_NODE_IRQ 
 * are only used in unsupervised systems and these parameters are ignored in 
 * a supervised system
 */

Parameter MIPC_SM_SYSTEM_POOL_BASE {
    SYNOPSIS    The base address on the fsl_p2020_rdb is at 0x20010000. \
                The shared memory extends up to 0x20100000. MIPC, SM, and DSHM \
                must share that space if included. Care must be taken so that \
                it is not overflown, since this will corrupt kernel data \
                and/or text.
    DEFAULT     (SM_BASE_ADRS + 0x10000)
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
    DEFAULT     0x78000
}

Parameter MIPC_SM_NODE_IRQ {
    DEFAULT     0
}
