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
01g,23jun08,d_c  Auto-exclude tests for unsupported components
01f,16oct07,y_w  Fix the errors when building VIP with PROFILE_BASIC_OS.
                 (WIND00108118)
01e,12oct07,d_l  WIND00107657 - Fix the errors while compiling with
                 PROFILE_BOOTAPP.(WIND00107657)
01d,23feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01c,05feb07,rlg  Updates for NOR and BootApp conversions
01b,24jan07,rlg  Updates for NOR conversion
01a,16jul06,pee  created

*/

Bsp mv5500 {
	CPU             PPC32
	REQUIRES        INCLUDE_KERNEL
	ENDIAN          big
	FP              hard
}

Component INCLUDE_GEI_END {
        NAME            BSP gei82543 End driver
	SYNOPSIS	gei82543 End
	_CHILDREN	FOLDER_BSP_CONFIG
        REQUIRES        INCLUDE_END
}

/* Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_GEI_END
}

Parameter RAM_HIGH_ADRS {
       NAME         Bootrom Copy region
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
                    (INCLUDE_BOOT_APP)::(0x01000000) \
                    0x00800000
}

Parameter RAM_LOW_ADRS {
       NAME         Runtime kernel load address
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x01000000) \
                    (INCLUDE_BOOT_APP)::(0x00800000) \
                    0x00100000
}

Profile PROFILE_BOOTAPP {
 COMPONENTS += INCLUDE_CACHE_L2 DRV_TIMER_DEC_PPC INCLUDE_BOOT_MMU_BASIC
}

Profile PROFILE_BASIC_OS {
 COMPONENTS += INCLUDE_CACHE_L2 DRV_TIMER_DEC_PPC
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
