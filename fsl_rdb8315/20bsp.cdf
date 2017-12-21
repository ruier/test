/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2008-2009, 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,16mar12,y_c remove the dependency between INCLUDE_PCI_BUS_AUTOCONF and the
                PCI component as they've been defined in 40m83xxPci.cdf and
                40m83xxPciEx.cdf. (WIND00252646)
01c,11oct11,wyt Define INCLUDE_PCI_BUS_AUTOCONF when PCI or PCIe Bus is enabled
01b,14dec09,x_z Code Clean.
01a,05may08,z_l Created.
*/

Bsp fsl_rdb8315 {
    NAME        board support package
    CPU         PPC32
    ENDIAN      big
    REQUIRES    INCLUDE_KERNEL      \
                DRV_INTCTLR_PPC     \
                DRV_INTCTLR_QUICC   \
                DRV_SIO_NS16550     \
                DRV_TIMER_DEC_PPC
    FP          hard
}

Component       INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_MII_BUS     \
                INCLUDE_TSEC_MDIO   \
                INCLUDE_GENERICPHY
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK
} 

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x1d00000) \
                (INCLUDE_BOOT_APP)::(0x1800000) \
                0xC00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x1a00000) \
                (INCLUDE_BOOT_APP)::(0x1200000) \
                0x10000
}

Profile PROFILE_BASIC_OS {
    COMPONENTS += DRV_INTCTLR_PPC \
                  DRV_INTCTLR_QUICC   \
                  DRV_SIO_NS16550 \
                  DRV_TIMER_DEC_PPC
}

