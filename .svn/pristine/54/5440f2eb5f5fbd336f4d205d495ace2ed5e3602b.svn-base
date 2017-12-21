/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,23oct09,j_z  change RAM_HIGH_ADRS, RAM_LOW_ADRS, add INCLUDE_BOOT_MMU_BASIC
                 for profile BOOTAPP(WIND00128640).
01c,05sep08,pee  changing FP order so that VSB get e500v2 by default
01b,22aug08,b_m  update BSP REQUIRES.
01a,28jun07,b_m  created.
*/

Bsp fsl_mds8568 {
    CPU         PPC32
    REQUIRES    INCLUDE_KERNEL
    FP          e500v2 soft
    REQUIRES    DRV_INTCTLR_PPC     \
                DRV_INTCTLR_EPIC    \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550
}

Component       INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_MV88E1X11PHY \
                INCLUDE_MII_BUS
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

Profile PROFILE_BOOTAPP {
    COMPONENTS += INCLUDE_BOOT_MMU_BASIC
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
