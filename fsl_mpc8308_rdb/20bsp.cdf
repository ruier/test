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
01a,16jun10,syt  derived from version 01e of fsl_rdb8313/20bsp.cdf
*/

Bsp fsl_mpc8308_rdb {
    NAME        board support package
    CPU         PPC32
    ENDIAN      big
    REQUIRES    INCLUDE_KERNEL      \
                INCLUDE_TIMER_SYS   \
                DRV_INTCTLR_PPC     \
                DRV_INTCTLR_QUICC   \
                INCLUDE_TIMER_SYS   \
                DRV_TIMER_DEC_PPC   \
                DRV_SIO_NS16550
    FP          hard
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
                INCLUDE_DUMMY_MDIO  \
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

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x2000000)   \
                (INCLUDE_BOOT_APP)::(0x1a00000)         \
                0xC00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x1c00000)   \
                (INCLUDE_BOOT_APP)::(0x1400000)         \
                0x10000
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_QUICC_PPC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}
