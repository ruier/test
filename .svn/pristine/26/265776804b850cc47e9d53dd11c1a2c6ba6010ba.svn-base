/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,23oct08,j_z  add boot device and RAM_LOW_ADRS,RAM_HIGH_ADRS(WIND00128640).
01c,17sep08,pmr  CPU PPC32
01b,23jun08,d_c  Auto-exclude tests for unsupported components
01a,01may08,mze  created

*/

Bsp ads8540 {
	CPU PPC32
	FP soft
        ENDIAN big
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
        REQUIRES    INCLUDE_TSEC_VXB_END
}

Parameter RAM_HIGH_ADRS {
	NAME	    Bootrom Copy region
	DEFAULT	    (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
	            (INCLUDE_BOOT_APP)::(0x01400000) \
		    0x01000000
}

Parameter RAM_LOW_ADRS {
	NAME	    Runtime kernel load address
	DEFAULT	    (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
		    (INCLUDE_BOOT_APP)::(0x01000000) \
		    0x00100000
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
