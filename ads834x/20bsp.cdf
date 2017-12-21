/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,25jul08,z_l  VxWorks6.7 update
01c,22feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01b,31jan07,rec  vxWorks 6.5 updates
01a,16jul06,pee  created
*/

Bsp ads834x {
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

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
INCLUDE_WHEN    INCLUDE_BOOT_NETWORK 
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_TSEC_VXB_END
}

/* Modify INCLUDE_TSEC_VXB_END component */

Component       INCLUDE_TSEC_VXB_END {
    REQUIRES += INCLUDE_MII_BUS \
                INCLUDE_MV88E1X11PHY
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x1d00000) \
                (INCLUDE_BOOT_APP)::(0x1800000) \
                0xC00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01a00000) \
                (INCLUDE_BOOT_APP)::(0x01200000) \
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
