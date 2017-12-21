/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,29dec10,g_x  add INCLUDE_I2C when INCLUDE_WINDML configured. (WIND00243445)
01i,05aug09,e_d  removed INCLUDE_ATA. (WIND00171937)
01h,22apr09,x_f  modified RAM_LOW_ADRS from 0x100000 to 0x200000
01g,16mar09,b_m  add FIFOC requirement.
01f,14jan09,x_f  fix PROFILE_BOOTAPP problem with PCB rev 4 hardware.
01e,16sep08,b_m  add auxiliary clock support.
01d,15aug08,x_f  replace DRV_SIO_M5XXXPSC with DRV_SIO_M5XXX_PSC
01c,01feb08,b_m  add PROFILE_BASIC_OS & PROFILE_BOOTAPP support.
01b,12nov07,b_m  add ATA support.
01a,26oct07,b_m  created.
*/

Bsp fsl_ads5121e {
    NAME        board support package
    CPU         PPC32
    FP          hard
    REQUIRES    INCLUDE_KERNEL      \
                DRV_INTCTLR_PPC     \
                DRV_INTCTLR_QUICC   \
                DRV_TIMER_DEC_PPC   \
                DRV_SIO_M5XXX_PSC   \
                DRV_MF_M512X_FIFOC
}

Component       INCLUDE_M512XFEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
                INCLUDE_MII_BUS
}

/*
 * Network Boot Devices for a BSP
 * The REQUIRES line should be modified for a BSP.
 */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_M512XFEC_VXB_END
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02100000)  \
                (INCLUDE_BOOT_APP)::(0x01F00000)        \
                0x01D00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01F00000)  \
                (INCLUDE_BOOT_APP)::(0x01D00000)        \
                0x00200000
}

/* auxiliary clock support */

Component DRV_TIMER_M5XXX_GPT {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

/* PROFILE_BOOTAPP support */

Profile PROFILE_BOOTAPP {
    COMPONENTS +=   INCLUDE_BOOT_MMU_BASIC
}

/*
 * Define I2C Component, which is included automatically when WINDML is 
 * configured.
 */

Component INCLUDE_I2C {
    INCLUDE_WHEN += INCLUDE_WINDML
}

/* WindML support */

Component       INCLUDE_WINDML {
    REQUIRES += DRV_DMA_M512X
}

