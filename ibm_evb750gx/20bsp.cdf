/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2008, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,20mar12,l_z  exclude INCLUDE_SIO_UTILS. (WIND00338917)
01c,31oct08,y_w  fix the BootApp VTS test failure. (WIND00134573)
01b,03jul08,y_w  Add support to BOOTAPP and BASIC OS profile, code cleanup.
01a,12nov07,y_w  created
*/

Bsp ibm_evb750gx {
    CPU PPC32
    REQUIRES        INCLUDE_KERNEL   \
                    INCLUDE_MMU_BASIC
    FP              hard
}

Parameter RAM_HIGH_ADRS {
    NAME            Bootrom Copy region
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
                    (INCLUDE_BOOT_APP)::(0x00a00000) \
                    0x00600000
}

Parameter RAM_LOW_ADRS {
    NAME            Runtime kernel load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
                    (INCLUDE_BOOT_APP)::(0x00600000) \
                    0x00200000
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

Component INCLUDE_SIO_UTILS {
    REQUIRES        COMPONENT_NOT_SUPPORTED
}

