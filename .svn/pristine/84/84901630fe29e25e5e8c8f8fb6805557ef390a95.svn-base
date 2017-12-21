/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,12feb10,syt  derived from version 01a of fsl_p2020_rdb/20bsp.cdf.
*/

Bsp fsl_p2020_rdb_AMP {
    CPU         PPC32
    ENDIAN      big
    FP          e500v2 soft
    REQUIRES    INCLUDE_KERNEL      \
                DRV_INTCTLR_EPIC    \
                DRV_INTCTLR_PPC     \
                DRV_TIMER_M85XX     \
                INCLUDE_TIMER_SYS   \
                DRV_SIO_NS16550     \
                INCLUDE_VXIPI
}

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
                INCLUDE_DUMMY_MDIO  \
                INCLUDE_TSEC_MDIO   \
                INCLUDE_VSC82XXPHY  \
                INCLUDE_DMA_SYS     \
                INCLUDE_MII_BUS
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
}

/* Required component for AMP */

Component INCLUDE_AMP_CPU {
    NAME        AMP CPU Support
    SYNOPSIS    Per-cpu configuration for AMP
    REQUIRES    SELECT_AMP_CPU_NUMBER
    _CHILDREN   FOLDER_BSP_CONFIG
}

/* Override the default (empty) component definitions with custom options */

Component INCLUDE_AMP_CPU_01  {
    NAME        AMP kernel configured for core 01
}

Component INCLUDE_AMP_CPU_00  {
    NAME        AMP kernel configured for core 00
}

/* Memory definitions */

Parameter RAM_HIGH_ADRS {
        NAME     Bootrom Copy region
        DEFAULT     (INCLUDE_AMP_CPU_00)::(0x01000000) \
                    (INCLUDE_AMP_CPU_01)::(0x22000000) \
                    (0x01000000)
}

Parameter RAM_LOW_ADRS {
    NAME     Runtime kernel load address
    DEFAULT     (INCLUDE_AMP_CPU_00)::(0x00100000) \
                (INCLUDE_AMP_CPU_01)::(0x21000000) \
                (0x00100000)
}

Parameter LOCAL_MEM_LOCAL_ADRS {
        NAME     system memory start address
        DEFAULT     (INCLUDE_AMP_CPU_00)::(0x00000000) \
                    (INCLUDE_AMP_CPU_01)::(0x20000000) \
                    (0x00000000)
}

Parameter LOCAL_MEM_SIZE {
        NAME     system memory size
        DEFAULT     (INCLUDE_AMP_CPU_00)::(0x20000000) \
                    (INCLUDE_AMP_CPU_01)::(0x20000000) \
                    (0x20000000)
}

Parameter  RAM_LOW_ADRS_CPU0 {
    NAME        Ram low address specifically for cpu0
    DEFAULT     0x00100000
}

Parameter TM_ANCHOR_ADRS {
    NAME     MDIO anchor adrs
    DEFAULT  (0x4400 + 0x20000000)
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}
