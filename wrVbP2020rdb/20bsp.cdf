/* 20bsp.cdf - BSP-specific component descriptor file */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01d,05may11,agf  remove CCSR resource dependency of PCI, Hv does the
                 LAW setup automatically (WIND00267843)
01c,22apr11,agf  add enable parameters for timer groups A & B 
01b,29mar11,agf  add SPE/hardware floating pt support
01a,15feb11,agf  created
*/

/*
DESCRIPTION
This file contains descriptions for this BSP components.
*/

Bsp     wrVbP2020rdb {
	NAME		Freescale P2020rdb guest vxWorks board support package
	CPU		PPC32
	MP_OPTIONS	UP
	FP		e500v2 soft
	GUEST_OS_OPTION REQUIRED
	REQUIRES	INCLUDE_KERNEL  \
			INCLUDE_VXBUS   \
			INCLUDE_SIO_UTILS \
			DRV_INTCTLR_PPC \
			DRV_INTCTLR_VB  \
			DRV_TIMER_VB
}


Component INCLUDE_ETSEC_VXB_END {
	REQUIRES +=	INCLUDE_VXBUS   \
			INCLUDE_TSEC_MDIO \
			INCLUDE_DUMMY_MDIO \
			INCLUDE_VSC82XXPHY
	CFG_PARAMS	ENABLE_ETSEC1 \
			ENABLE_ETSEC2 \
			ENABLE_ETSEC3
}

Parameter ENABLE_ETSEC1  {
	NAME		Enable ETSEC1 interface
	TYPE		BOOL
	DEFAULT		TRUE
}

Parameter ENABLE_ETSEC2 { 
	NAME		Enable ETSEC2 interface
	TYPE		BOOL
	DEFAULT		FALSE
}

Parameter ENABLE_ETSEC3  {
	NAME		Enable ETSEC3 interface
	TYPE		BOOL
	DEFAULT		FALSE
}

Component DRV_TIMER_OPENPIC {
	CFG_PARAMS	+= ENABLE_TIMER_A \
			   ENABLE_TIMER_B
}

Parameter ENABLE_TIMER_A {
	NAME		Enable openPic Timer A
	TYPE		BOOL
	DEFAULT		TRUE
}

Parameter ENABLE_TIMER_B {
	NAME		Enable openPic Timer B
	TYPE		BOOL
	DEFAULT		FALSE
}

/* 
 * The standard DRV_PCIBUS_M85XX in config/comps/vxWorks would require 
 * DRV_RESOURCE_M85XXCCSR. Since there isn't a 'REQUIRES -=' syntax, we
 * will redefine the entire REQUIRES field leaving out the CCSR component.
 */

Component DRV_PCIBUS_M85XX {
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS \
                        INCLUDE_PARAM_SYS \
                        INCLUDE_PCI_BUS \
                        INCLUDE_PCI_BUS_AUTOCONF
}

Component INCLUDE_PCI_BUS {
	REQUIRES	+= DRV_PCIBUS_M85XX
}

Component INCLUDE_AUX_CLK {
	REQUIRES	+= DRV_TIMER_OPENPIC
}

Component INCLUDE_TIMESTAMP {
	REQUIRES	+= DRV_TIMER_OPENPIC
}


Parameter SYSCLK_TIMER_NAME  {
	DEFAULT		"vxbVbTimerDev"
}


Parameter RAM_HIGH_ADRS {
    NAME         Bootrom Copy region
    DEFAULT      0x00800000
}

Parameter RAM_LOW_ADRS {
    NAME         Runtime kernel load address
    DEFAULT      0x00010000
}

Parameter ROM_BASE_ADRS {
    DEFAULT      0xfff00000
}

Parameter ROM_TEXT_ADRS {
    DEFAULT      0xfff00100
}

Parameter ROM_SIZE {
    DEFAULT      0x00080000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
    DEFAULT      0x0
}

Parameter LOCAL_MEM_SIZE {
    DEFAULT      0x01000000
}

Parameter LOCAL_MEM_AUTOSIZE {
    DEFAULT      TRUE
}

Parameter USER_RESERVED_MEM {
    DEFAULT      0x0
}

Parameter HWMEM_POOL_SIZE {
    DEFAULT      100000
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

/* guest OS vxWorks may not access the MMU, it is reserved to
 * the Hypervisor
 */

Component               INCLUDE_TM_PGMGRLIB {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component               INCLUDE_MMU_BASIC {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component               INCLUDE_MMU_GLOBAL_MAP {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

