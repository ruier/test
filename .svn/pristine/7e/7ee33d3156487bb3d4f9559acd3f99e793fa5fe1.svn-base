/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007, 2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,07jan11,swu  Fix VIP build error with BOOTAPP profile. (WIND00249429)
01e,08oct08,syt  modified RAM_HIGH_ADRS and RAM_LOW_ADRS parameters (WIND00135772)
01d,12aug08,x_s  cleaned up code and added INCLUDE_MII_BUS & INCLUDE_MV88E1X11PHY.
01c,20feb08,x_s  replaced INCLUDE_MOT_ETSEC_HEND with INCLUDE_ETSEC_VXB_END.
01b,20oct07,x_s  modified for mds837x.
01a,31jul07,dtr  created
*/

Bsp mds837x {
    NAME        board support package
    REQUIRES    INCLUDE_KERNEL
    CPU         PPC32
    FP          hard
}

Component   INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_MV88E1X11PHY \
                INCLUDE_MII_BUS
}

/* 
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component   INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

/*******************************************************************************
* BootApp Configuration 
*
*/

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component   INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK 
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_APP)::(0x01400000) \
                0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
                (INCLUDE_BOOT_APP)::(0x01000000) \
                0x00010000
}

/* PROFILE_BASIC_OS support */

Profile PROFILE_BASIC_OS {
    COMPONENTS +=   DRV_INTCTLR_PPC     \
                    DRV_INTCTLR_QUICC   \
                    DRV_SIO_NS16550   
}

/* PROFILE_BOOTAPP support */

Profile PROFILE_BOOTAPP {
    COMPONENTS +=   DRV_INTCTLR_PPC     \
                    DRV_INTCTLR_QUICC   \
                    DRV_SIO_NS16550     \
                    INCLUDE_MMU_BASIC	\
                    DRV_TIMER_DEC_PPC
}
