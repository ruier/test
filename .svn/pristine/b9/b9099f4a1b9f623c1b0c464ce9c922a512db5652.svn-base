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
01g,13jan11,c_p  added component INCLUDE_GENERICPHY (WIND00244693)
01f,21oct08,j_z  change boot device to INCLUDE_TSEC_VXB_END(WIND00128640) 
01e,24jun08,d_c  Auto-exclude tests for unsupported components
01d,23feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01c,10jan07,rec	 add INCLUDE_SIO_UTILS and network devices
01b,30nov06,rec	 vxWorks 6.5 changes
01a,16jul06,pee	 created

*/

Bsp wrSbc8560 {
	NAME	    board support package
	REQUIRES    INCLUDE_KERNEL \
		    DRV_SIO_NS16550 
	CPU	    PPC32
	FP	    soft
}

Component INCLUDE_MOT_FCC_END {
        NAME            BSP motFcc2End End driver
        REQUIRES        INCLUDE_END \
                        INCLUDE_DMA_SYS \
                        INCLUDE_PARAM_SYS
}

Component INCLUDE_PRIMARY_FCC_END {
        NAME            BSP primary motFcc2End End driver
        REQUIRES        INCLUDE_MOT_FCC_END
}

Component INCLUDE_SECONDARY_FCC_END {
        NAME            BSP secondary motFcc2End End driver
        REQUIRES        INCLUDE_MOT_FCC_END
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
 
Component       INCLUDE_BOOT_NET_DEVICES {
        REQUIRES    INCLUDE_TSEC_VXB_END	\
		    INCLUDE_GENERICPHY
}

Parameter RAM_HIGH_ADRS {
	NAME	    Bootrom Copy region
	DEFAULT	    (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
	            (INCLUDE_BOOT_APP)::(0x01400000) \
		    0x00C00000
}

Parameter RAM_LOW_ADRS {
	NAME	    Runtime kernel load address
	DEFAULT	    (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
		    (INCLUDE_BOOT_APP)::(0x00C00000) \
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
