/* 22comp_mipc.cdf - configuration for MCB/MSD/MIPC */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,19nov11,x_s  written.
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
    NAME        Base address of shared memory pool used by MCB
    SYNOPSIS    The base address on the apm_serengeti is at 0x20010000. \
                The shared memory extends up to 0x20100000. MCB, SM and DSHM \
                must share that space if included.  Care must be taken so that \
                it is not overflown, since this will corrupt kernel data \
                and/or text.  Besides, the address must be visible for all the \
                cores.
    _CFG_PARAMS INCLUDE_MIPC_SM
    DEFAULT     (0x20010000)
}

Parameter MIPC_SM_SYSTEM_POOL_SIZE {
    DEFAULT     0x80000
}

/*
 * use the IPI reserved for MIPC instead of 0,
 */

Parameter MIPC_SM_NODE_IRQ {
    NAME        Interrupt request line
    SYNOPSIS    Interrupt request line. 0 will give better performance for \
                UP AMP node, and 3 will work too. For an SMP AMP node, 3 \
                should be used since 0 is for CPC and 3 is most probably be an \
                SMP node.
    TYPE        INT
    DEFAULT     3
}

Parameter MIPC_SM_NODES {
    NAME        Maximum nodes per bus
    SYNOPSIS    Specifies the maximum number of nodes a bus can support, \
                unless overridden by the bus configuration string.
    TYPE        UINT
    DEFAULT     2
}

Parameter MSD_NUM_DEVS {
    DEFAULT     (INCLUDE_AMP_CPU_00)::(2) \
                (1)
}

