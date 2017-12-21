/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2006-2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,04jan11,pch  CQ248235: add MAC set/get support
		 (from wrSbc405gp vn 01b,15dec06,rec)
01g,17oct08,pch  CQ135132: fix RAM_{HIGH,LOW}_ADRS definitions,
		 all cases must be below LOCAL_MEM_SIZE
01f,04sep08,l_z  code cleanup
01e,24jun08,d_c  Auto-exclude tests for unsupported components
01d,28apr08,fao  add ns16550 component
01c,23mar07,rec  WIND00090905 - add MMU component to bootApp
01b,09nov06,rec  vxWorks 6.5 update
01a,16jul06,pee  created

*/

Bsp walnut {
    NAME        board support package
    REQUIRES    INCLUDE_KERNEL   \
		DRV_SIO_NS16550  \
		INCLUDE_MMU_BASIC
    CPU         PPC32
    FP          soft
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */
Component   INCLUDE_BOOT_ETH_MAC_HANDLER {
	INCLUDE_WHEN    INCLUDE_BOOT_NETWORK
	REQUIRES	INCLUDE_EMAC_VXB_END
}

Parameter RAM_HIGH_ADRS {
    NAME       Bootrom Copy region
    DEFAULT    (INCLUDE_BOOT_RAM_IMAGE)::(0x00d00000) \
	       (INCLUDE_BOOT_APP)::(0x00f00000) \
	       0x00d00000
}

Parameter RAM_LOW_ADRS {
    NAME       Runtime kernel load address
    DEFAULT    (INCLUDE_BOOT_RAM_IMAGE)::(0x00b00000) \
	       (INCLUDE_BOOT_APP)::(0x00d00000) \
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

Component               INCLUDE_TIMER_SYS {
	REQUIRES        COMPONENT_NOT_SUPPORTED
}
