/* 22comp_mipc.cdf - configuration for MCB/MSD/MIPC */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,17may12,d_l  created from fsl_p1020_rdb/01b.
*/

Folder FOLDER_MIPC {
    _CHILDREN   FOLDER_MULTIOS
}

Component INCLUDE_MIPC_UNSUPPORTED {
    _CHILDREN   FOLDER_NOT_VISIBLE
}

Parameter MIPC_SM_SYSTEM_POOL_BASE {
    SYNOPSIS    The base address is at 0x20010000. \
                MIPC, SM, and DSHM must share that space if included. \
                Care must be taken so that it is not overflown, \
                since this will corrupt kernel data and/or text.
    DEFAULT     0x20010000
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
    DEFAULT     0x80000
}

Parameter MIPC_SM_NODE_IRQ {
    NAME        Interrupt request line
    SYNOPSIS    Interrupt request line.
    TYPE        INT
    DEFAULT     0
}

Parameter MIPC_SM_NODES {
    NAME        Maximum nodes per bus
    SYNOPSIS    Specifies the maximum number of nodes a bus can support, \
                unless overridden by the bus configuration string.
    TYPE        UINT
    DEFAULT     2
}

Parameter MSD_NUM_DEVS {
    DEFAULT     1
} 
