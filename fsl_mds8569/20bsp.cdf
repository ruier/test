/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,29sep11,y_c  add DRV_SIO_NS16550. (WIND00304487)
01b,30dec09,x_z  remove DRV_SIO_NS16550.
01a,10jul09,x_z  derived from version 01d of fsl_mmds8568/20bsp.cdf.
*/

Bsp fsl_mds8569 {
    CPU         PPC32
    REQUIRES    INCLUDE_KERNEL
    FP          e500v2 soft
    REQUIRES    DRV_INTCTLR_PPC     \
                DRV_INTCTLR_EPIC    \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550
}

Component       INCLUDE_UCC_VXB_END {
    REQUIRES += INCLUDE_MV88E1X11PHY \
                INCLUDE_MII_BUS      \
                DRV_INTCTLR_QE       \
                INCLUDE_QUICC_ENGINE_UTILS
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_UCC_VXB_END
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                (INCLUDE_BOOT_APP)::(0x01400000)        \
                0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                (INCLUDE_BOOT_APP)::(0x01000000)        \
                0x00100000
}
