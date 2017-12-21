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
01h,03dec08,syt  added component INCLUDE_GENERICPHY related with 
                 INCLUDE_TSEC_VXB_END (WIND00139819)
01g,23jun08,d_c  Auto-exclude tests for unsupported components
01f,13jun08,wap  Switch to new VxBus TSEC driver
01e,20mar07,jmt  Update to work with PROFILE_BOOTAPP
01d,23feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01c,11jan07,rec  add SELECT_BOOT_NET_DEVICES
01b,08dec06,rec  vxWorks 6.5 update
01a,16jul06,pee  created

*/

Bsp wrSbc834x {
        NAME        board support package
        REQUIRES    INCLUDE_KERNEL
        CPU         PPC32
        FP          hard
}

/* Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component INCLUDE_BOOT_NET_DEVICES {
	REQUIRES    INCLUDE_TSEC_VXB_END \
                INCLUDE_GENERICPHY
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */
Component INCLUDE_BOOT_ETH_MAC_HANDLER {
        INCLUDE_WHEN    INCLUDE_BOOT_NETWORK 
}

Parameter RAM_HIGH_ADRS {
        NAME            Bootrom Copy region
        DEFAULT         (INCLUDE_BOOT_APP)::(0x01000000) \
                        0x00c00000
}

Parameter RAM_LOW_ADRS {
        NAME            Runtime kernel load address
        DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x00E00000) \
                        (INCLUDE_BOOT_APP)::(0x00c00000) \
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
