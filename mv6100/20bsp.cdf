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
01f,29oct08,y_w  fixed errors when using vxprj to create a project.
01e,07oct08,x_f  Fix VSB build issue. (WIND00135565)
01d,23jun08,d_c  Auto-exclude tests for unsupported components
01c,27apr08,y_w  Add support to BOOTAPP and BASIC OS profile, code cleanup.
01b,26may07,y_w  Updated for vxWorks 6.5.
01a,05dec06,pai  Written.
*/

/*
DESCRIPTION
This file contains descriptions for this BSP and its components.
*/

/* Describe the BSP to the project tools */

Bsp mv6100 {
    NAME            board support package
    REQUIRES        INCLUDE_KERNEL           \
                    INCLUDE_CACHE_L2         \
                    INCLUDE_CACHE_L2_ENABLE  \
                    INCLUDE_AUX_CLK          \
                    INCLUDE_MMU_BASIC        \
                    INCLUDE_PCI              \
                    INCLUDE_PCI_AUTOCONF 
    CPU             PPC32
    FP              hard
}

/* Components to support kernel source build */

Component INCLUDE_CACHE_BSP {
    NAME            BSP cache CPU varient select
    LAYER           1
    REQUIRES        INCLUDE_CACHE_ARCH_PPC60X
    INCLUDE_WHEN    INCLUDE_CACHE_SUPPORT
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_MMU_BSP {
    NAME            BSP MMU CPU varient select
    LAYER           1
    REQUIRES        INCLUDE_MMU_ARCH_PPC60X
    INCLUDE_WHEN    INCLUDE_MMU_GLOBAL_MAP
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_PCI {
    NAME            BSP PCI support
    LAYER           2
    _CHILDREN       FOLDER_BSP_CONFIG
    REQUIRES        INCLUDE_MEM_MGR_BASIC
}

Component INCLUDE_PCI_AUTOCONF {
    NAME            BSP PCI auto config support
    LAYER           2
    _CHILDREN       FOLDER_BSP_CONFIG
    REQUIRES        INCLUDE_PCI
    INCLUDE_WHEN    INCLUDE_PCI
}

Parameter RAM_HIGH_ADRS {
    NAME            Bootrom Copy region
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
                    (INCLUDE_BOOT_APP)::(0x00f00000) \
                    0x00800000
}

Parameter RAM_LOW_ADRS {
    NAME            Runtime kernel load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x01000000) \
                    (INCLUDE_BOOT_APP)::(0x00800000) \
                    0x00100000
}

/* PROFILE_BASIC_OS support */

Profile PROFILE_BASIC_OS {
    COMPONENTS +=   DRV_TIMER_DEC_PPC

}

/* PROFILE_BOOTAPP support */

Profile PROFILE_BOOTAPP {
    COMPONENTS +=   DRV_TIMER_DEC_PPC
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
