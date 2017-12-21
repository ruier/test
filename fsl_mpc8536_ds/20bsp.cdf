/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,30oct08,b_m  created.
*/

Bsp fsl_mpc8536_ds {
    CPU         PPC32
    REQUIRES    INCLUDE_KERNEL
    FP          e500v2 soft
    REQUIRES    DRV_INTCTLR_PPC     \
                DRV_INTCTLR_EPIC    \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550
}

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
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

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                (INCLUDE_BOOT_APP)::(0x01800000)        \
                0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                (INCLUDE_BOOT_APP)::(0x01000000)        \
                0x00100000
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}
