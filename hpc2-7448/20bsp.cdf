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
01g,25jan11,pch  cq249544: make BOOT_ETH_MAC_HANDLER unavailable
01f,13nov08,jmt  Add INCLUDE_TSI108_END component
01e,23jun08,d_c  Auto-exclude tests for unsupported components
01d,22apr08,z_l  Add DRV_TIMER_DEC_PPC component.
01c,22feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
		 defines
01b,24jan07,rlg  Updates for NOR conversion
01a,16jul06,pee  created

*/

Bsp hpc2-7448 {
	CPU		PPC32
	REQUIRES	INCLUDE_KERNEL
	ENDIAN		big
	FP		hard
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component		INCLUDE_BOOT_NET_DEVICES {
	REQUIRES	INCLUDE_TSI108_END
}

Parameter RAM_HIGH_ADRS {
	NAME		Bootrom Copy region
	DEFAULT		(INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
			(INCLUDE_BOOT_APP)::(0x01000000) \
			0x00400000
}

Parameter RAM_LOW_ADRS {
	NAME		Runtime kernel load address
	DEFAULT		(INCLUDE_BOOT_RAM_IMAGE)::(0x01000000) \
			(INCLUDE_BOOT_APP)::(0x00800000) \
			0x00010000
}

Component INCLUDE_TSI108_END {
	REQUIRES	INCLUDE_END
}

Profile PROFILE_BASIC_OS {
	COMPONENTS += DRV_TIMER_DEC_PPC
}

Profile PROFILE_BOOTAPP {
	COMPONENTS += DRV_TIMER_DEC_PPC
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

Component		INCLUDE_AUX_CLK {
	REQUIRES	COMPONENT_NOT_SUPPORTED
}

Component		INCLUDE_SIO_UTILS {
	REQUIRES	COMPONENT_NOT_SUPPORTED
}

Component		INCLUDE_BOOT_ETH_MAC_HANDLER {
	REQUIRES	COMPONENT_NOT_SUPPORTED
}
