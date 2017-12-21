/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,07jan11,pch  CQ242489: exclude INCLUDE_SIO_UTILS
01d,13nov08,jmt  Fix Bootapp build failure
01c,31oct08,y_w  fix the BootApp VTS test failure. (WIND00134573)
01b,03jul08,y_w  Add support to BOOTAPP and BASIC OS profile, code cleanup.
01a,04jul07,y_w  created
*/

Bsp ibm_evb750cl {
	CPU         PPC32
	REQUIRES    INCLUDE_KERNEL   \
		    INCLUDE_MMU_BASIC
	FP          hard
}

Parameter RAM_HIGH_ADRS {
	NAME        Bootrom Copy region
	DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
		    (INCLUDE_BOOT_APP)::(0x00a00000) \
		    0x00600000
}

Parameter RAM_LOW_ADRS {
	NAME        Runtime kernel load address
	DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
		    (INCLUDE_BOOT_APP)::(0x00600000) \
		    0x00200000
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component INCLUDE_BOOT_NET_DEVICES {
	REQUIRES    INCLUDE_TSI108_END
}

Component INCLUDE_TSI108_END {
	REQUIRES    INCLUDE_END
}

/* PROFILE_BOOTAPP support */

Profile PROFILE_BOOTAPP {
	COMPONENTS +=	DRV_TIMER_DEC_PPC \
			INCLUDE_BOOT_ETH_MAC_HANDLER
}

Component INCLUDE_SIO_UTILS {
	REQUIRES    COMPONENT_NOT_SUPPORTED
}
