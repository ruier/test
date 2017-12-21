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
01h,26mar12,c_t  disable TAH by default. (WIND00272618)
01g,16dec08,b_m  add vxbus timer & uic support.
01f,28oct08,pgh  Fix bootapp issue
01e,04sep08,l_z  Add support for bootApp
01d,20jue08,x_s  Updated for new VxBus EMAC driver.
01c,25nov07,x_s  Deleted component INCLUDE_MTD_S29GL.
01b,20may07,x_s  Updates for BootApp conversions.
01a,18mar07,x_s  Created based on ocotea bsp.
*/

Bsp amcc_taishan {
    NAME        board support package
    CPU         PPC32
    REQUIRES    INCLUDE_KERNEL                 \
                INCLUDE_440X5_DCACHE_RECOVERY  \
                INCLUDE_440X5_TLB_RECOVERY     \
                INCLUDE_440X5_TLB_RECOVERY_MAX \
                INCLUDE_440X5_PARITY_RECOVERY  \
                INCLUDE_440X5_MCH_LOGGER       \
                INCLUDE_MMU_BASIC              \
                DRV_TIMER_PPC440               \
		DRV_INTCTLR_PPC                \
		DRV_INTCTLR_UIC
    FP          soft
}

Component INCLUDE_EMAC_VXB_END {
    REQUIRES += INCLUDE_VXB_IBM_RGMII \
                INCLUDE_GENERICPHY
}

/* 
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component  INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_EMAC_VXB_END
}

/* Specify boot rom console device for this BSP */
Component       INCLUDE_BOOT_SHELL {
    REQUIRES += DRV_SIO_NS16550
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01400000) \
                (INCLUDE_BOOT_APP)::(0x01000000) \
                0x00e00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01200000) \
                (INCLUDE_BOOT_APP)::(0x00e00000) \
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

