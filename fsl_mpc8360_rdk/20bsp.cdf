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
01h,21oct08,z_l  Modify INCLUDE_BOOT_RAM_IMAGE to support BootApp VTS (WIND00135163)
01g,09jul08,y_w  Fixed BSP name and switch from hEnd qeFcc driver to UCC driver
01f,08jan08,z_l  Add timer for PROFILE_BASIC_OS
01e,22mar07,jmt  Defect 90991 - Fix problem with bootapp profile build
01d,23feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01c,14feb07,jmt  Defect 87981- Wrong floating point type specified
01b,19dec06,rlg  Updates for NOR
01a,16jul06,pee  created

*/

Bsp fsl_mpc8360_rdk {
    NAME        board support package
    CPU         PPC32
    REQUIRES    INCLUDE_KERNEL \
                DRV_SIO_NS16550 \
                INCLUDE_TIMER_SYS
    FP          hard
}

/* 
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_UCC_VXB_END
}

/* Modify INCLUDE_UCC_VXB_END component */

Component       INCLUDE_UCC_VXB_END {
    REQUIRES += INCLUDE_MII_BUS \
                INCLUDE_BCM54XXPHY \
                INCLUDE_GENERICPHY
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02000000) \
                (INCLUDE_BOOT_APP)::(0x01000000) \
                0x00e00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000) \
                (INCLUDE_BOOT_APP)::(0x00e00000) \
                0x00010000
}

/* PROFILE_BASIC_OS support */

Profile PROFILE_BASIC_OS {
    COMPONENTS +=   DRV_INTCTLR_PPC \
                    DRV_INTCTLR_QE \
                    DRV_INTCTLR_QUICC \
                    DRV_TIMER_DEC_PPC \
                    DRV_TIMER_QUICC_PPC
}

/* PROFILE_BOOTAPP support */

Profile PROFILE_BOOTAPP {
    COMPONENTS +=   DRV_INTCTLR_PPC \
                    DRV_INTCTLR_QE \
                    DRV_INTCTLR_QUICC \
                    DRV_TIMER_DEC_PPC \
                    DRV_TIMER_QUICC_PPC \
                    INCLUDE_QUICC_ENGINE_UTILS
}

