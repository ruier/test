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
01d,23oct08,j_z  change RAM_HIGH_ADRS,RAM_LOW_ADRS add INCLUDE_BOOT_MMU_BASIC
                 for profile BOOTAPP(WIND00128640).
01c,25aug08,dtr  Add support for hardware float build.
01b,17apr08,x_f  add bootApp support
01a,16feb07,dtr  created
*/

Bsp ads8544 {
    NAME            board support package
    CPU             PPC32
    FP              e500v2 soft
    REQUIRES        INCLUDE_KERNEL
}

Component           INCLUDE_ETSEC_VXB_END {
    REQUIRES +=     INCLUDE_GENERICPHY  \
                    INCLUDE_MII_BUS
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component           INCLUDE_BOOT_NET_DEVICES {
    REQUIRES        INCLUDE_ETSEC_VXB_END
}

Parameter RAM_HIGH_ADRS {
    NAME            Bootrom Copy region
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000)  \
                    (INCLUDE_BOOT_APP)::(0x01400000)        \
                    0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME            Runtime kernel load address
    DEFAULT         (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                    (INCLUDE_BOOT_APP)::(0x01000000)        \
                    0x00100000
}

/* PROFILE_BASIC_OS support */

Profile PROFILE_BASIC_OS {
    COMPONENTS +=   DRV_INTCTLR_PPC   \
                    DRV_INTCTLR_EPIC  \
                    DRV_TIMER_OPENPIC \
                    DRV_TIMER_M85XX   \
                    DRV_SIO_NS16550
}

/* PROFILE_BOOTAPP support */

Profile PROFILE_BOOTAPP {
    COMPONENTS +=   DRV_INTCTLR_PPC   \
                    DRV_INTCTLR_EPIC  \
                    DRV_TIMER_OPENPIC \
                    DRV_TIMER_M85XX   \
                    DRV_SIO_NS16550   \
                    INCLUDE_BOOT_MMU_BASIC
}
