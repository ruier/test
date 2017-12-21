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
01e,18dec08,z_l Fix net device problems in BootApp. (WIND00139815)
01d,13nov08,jmt Add INCLUDE_MOT_TSEC_END component
01c,04jul08,l_g remove the description of EN29LV640
01b,21mar08,z_l added DRV_TIMER_DEC_PPC component for PROFILE_BASIC_OS
01a,26jun07,l_g created
*/

Bsp fsl_rdb8313 {
    NAME        board support package
    CPU         PPC32
    ENDIAN      big
    REQUIRES    INCLUDE_KERNEL
    FP          hard
}

Component INCLUDE_SECURITY_ENGINE {
    NAME        on-chip security engine drivers
    SYNOPSIS    security support
    _CHILDREN   FOLDER_DRIVERS
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END \
                INCLUDE_MV88E1X11PHY \
                INCLUDE_GENERICPHY \
                INCLUDE_TSEC_MDIO \
                INCLUDE_DUMMY_MDIO
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK 
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x1d00000) \
                (INCLUDE_BOOT_APP)::(0x1500000) \
                0xC00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x1a00000) \
                (INCLUDE_BOOT_APP)::(0xF00000) \
                0x10000
}

Profile PROFILE_BASIC_OS {
    COMPONENTS += DRV_INTCTLR_PPC \
                  DRV_INTCTLR_QUICC   \
                  DRV_SIO_NS16550 \
                  DRV_TIMER_DEC_PPC
}

Profile PROFILE_BOOTAPP {
    COMPONENTS += DRV_INTCTLR_PPC \
                  DRV_INTCTLR_QUICC   \
                  DRV_SIO_NS16550 \
                  DRV_TIMER_DEC_PPC
}

