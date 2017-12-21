/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2006-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,22sep09,syt  Fix bootApp issue.(WIND00128808)
01f,04sep08,l_z  Add support for bootApp
01e,23jun08,d_c  Auto-exclude tests for unsupported components
01d,21mar08,wap  Switch to new VxBus EMAC driver
01c,11jan07,rec  add SELECT_BOOT_NET_DEVICES
01b,31oct06,rec  vxWorks 6.5 update
01a,16jul06,pee  created

*/

Bsp amcc440ep {
    NAME        board support package
    REQUIRES    INCLUDE_KERNEL                 \
                INCLUDE_440X5_DCACHE_RECOVERY  \
                INCLUDE_440X5_TLB_RECOVERY     \
                INCLUDE_440X5_TLB_RECOVERY_MAX \
                INCLUDE_440X5_PARITY_RECOVERY  \
                INCLUDE_440X5_MCH_LOGGER       \
                INCLUDE_MMU_BASIC
    CPU         PPC32
    FP          hard
}

Component INCLUDE_EMAC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY
}
 
/* 
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_EMAC_VXB_END
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */
Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK 
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
                (INCLUDE_BOOT_APP)::(0x01400000) \
                0x00C00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01400000) \
                (INCLUDE_BOOT_APP)::(0x01000000) \
                0x00010000
}

/*
 * Force unsupported components to be unavailable.
 *
 * The following component definition(s) forces the named component(s)
 * to be "unavailable" as far as the project build facility (vxprj) is
 * concerned. The required component (COMPONENT_NOT_SUPPORTED) should
 * never be defined, and hence the named component(s) will never be
 * available. This little trick is used by the BSPVTS build scripts
 * (suiteBuild, bspBuildTest.tcl) to automatically exclude test modules
 * that are not applicable to a BSP because the BSP does not support a
 * given component and/or test module. If and when support is added,
 * the following definition(s) should be removed.
 */

Component               INCLUDE_SIO_UTILS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component               INCLUDE_TIMER_SYS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component               INCLUDE_PCI_BUS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}
