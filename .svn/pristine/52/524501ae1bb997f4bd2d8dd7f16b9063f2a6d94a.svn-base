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
01h,11jan11,y_y  Fix the network of BootApp does not work. (WIND00245996)
01g,18dec08,z_l  Fix net device problems in BootApp. (WIND00139815)
01f,30jul08,z_l  Support PROFILE_BASIC_OS and PROFILE_BOOTAPP
01e,22mar07,jmt  Defect 90991 - Fix problem with bootapp profile build
01d,23feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01c,14feb07,jmt  Defect 87981- Wrong floating point type specified
01b,19dec06,rlg  Updates for NOR
01a,16jul06,pee  created
*/

Bsp mds8360 {
    NAME        board support package
    CPU         PPC32
    ENDIAN      big
    REQUIRES    INCLUDE_KERNEL \
                DRV_INTCTLR_PPC \
                DRV_INTCTLR_QUICC \
                DRV_SIO_NS16550 \
                DRV_TIMER_DEC_PPC \
                DRV_INTCTLR_QE 
    FP          hard
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_UCC_VXB_END
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK 
}

/* Modify INCLUDE_UCC_VXB_END component */

Component       INCLUDE_UCC_VXB_END {
    REQUIRES += INCLUDE_MII_BUS \
                INCLUDE_MV88E1X11PHY \
                INCLUDE_QUICC_ENGINE_UTILS
}

Parameter RAM_HIGH_ADRS {
       NAME         Bootrom Copy region
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
                    (INCLUDE_BOOT_APP)::(0x01a00000) \
                    0x00e00000
}

Parameter RAM_LOW_ADRS {
       NAME         Runtime kernel load address
       DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x01c00000) \
                    (INCLUDE_BOOT_APP)::(0x01400000) \
                    0x00010000
}
