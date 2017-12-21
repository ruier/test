/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,05aug09,e_d  removed INCLUDE_ATA. (WIND00171937)
01g,24oct08,pgh  Fix bootapp issue
01f,06oct08,b_m  remove legacy fei/gei driver support.
01e,26aug08,l_g  include DRV_TIMER_M5XXX_GPT when INCLUDE_AUX_CLK
                 is included.(WIND00128798)
                 Correct typo of DRV_SIO_M5XXX_PSC.(WIND00128773)
01d,16apr08,b_m  add PROFILE_BASIC_OS & PROFILE_BOOTAPP support.
01c,12oct07,d_l  add component INCLUDE_FALSH for PROFILE_BOOTAPP
01b,19sep07,b_m  change to use INCLUDE_MTD_USR macro.
01a,18jan07,b_m  Created based on lite5200 bsp.
*/

Bsp lite5200b {
    NAME            board support package
    CPU             PPC32
    FP              hard
    REQUIRES        INCLUDE_KERNEL \
                    DRV_TIMER_DEC_PPC
}

/* Specify boot rom console device for this BSP */
Component       INCLUDE_BOOT_SHELL {
    REQUIRES += DRV_SIO_M5XXX_PSC
}

/* include DRV_TIMER_M5XXX_GPT when INCLUDE_AUX_CLK is included */

Component DRV_TIMER_M5XXX_GPT {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

Parameter RAM_HIGH_ADRS {
   NAME         Bootrom Copy region
   DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x02100000)  \
                (INCLUDE_BOOT_APP)::(0x01F00000)        \
                0x01D00000
}

Parameter RAM_LOW_ADRS {
   NAME         Runtime kernel load address
   DEFAULT      (INCLUDE_BOOT_RAM_IMAGE)::(0x01f00000)  \
                (INCLUDE_BOOT_APP)::(0x01D00000)        \
                0x00100000
}
