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
01f,23jun08,d_c  Auto-exclude tests for unsupported components
01e,24apr08,l_g  Add vxbus support
01d,23mar07,rec  WIND00090905 - bootApp requires MMU
01c,22feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01b,16feb07,rec  vxWorks 6.5 update
01a,16jul06,pee  created

*/

Bsp ads827x {
    NAME        board support package
    REQUIRES    INCLUDE_KERNEL
    CPU         PPC32
    FP          hard
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component  INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_FCC_VXB_END
}

/* BootApp Configuration */

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component   INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK
}

Parameter RAM_HIGH_ADRS {
    NAME            Bootrom Copy region
    DEFAULT         (INCLUDE_BOOT_APP)::(0x01F00000) \
                    0x01D00000
}

Parameter RAM_LOW_ADRS {
    NAME            Runtime kernel load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x01B00000) \
                    (INCLUDE_BOOT_APP)::(0x01D00000) \
                    0x00100000
}

/* add INCLUDE_MMU_BASIC to BootApp */

Component INCLUDE_BOOT_APP {
    REQUIRES +=     INCLUDE_MMU_BASIC
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
