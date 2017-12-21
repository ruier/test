/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,13nov08,jmt  Add INCLUDE_PRIMARY_TSEC_END component
01c,13aug08,l_g  add BOOTAPP and BASIC_OS support
01b,26jul07,agf  rename BSP from mpc8540 to mv3100
01a,20may07,y_w  created
*/

Bsp mv3100 {
	CPU PPC32
        REQUIRES        INCLUDE_KERNEL   \
                        INCLUDE_MMU_BASIC
	FP  soft
}

/* Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */
Component       INCLUDE_BOOT_NET_DEVICES {
       REQUIRES    INCLUDE_PRIMARY_TSEC_END
}

Component       INCLUDE_PRIMARY_TSEC_END {
       REQUIRES    INCLUDE_END
}

Parameter RAM_HIGH_ADRS {
       NAME         Bootrom Copy region
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
		            (INCLUDE_BOOT_APP)::(0x00f00000) \
                    0x00d00000
}

Parameter RAM_LOW_ADRS {
       NAME         Runtime kernel load address
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x01000000) \
                    (INCLUDE_BOOT_APP)::(0x00d00000) \
                    0x00100000
}

/* PROFILE_BASIC_OS support */

Profile PROFILE_BASIC_OS {
    COMPONENTS +=   DRV_TIMER_M85XX  \
                    DRV_SIO_NS16550
}


/* PROFILE_BOOTAPP support */

Profile PROFILE_BOOTAPP {
    COMPONENTS +=   DRV_TIMER_M85XX  \
                    DRV_SIO_NS16550
}
