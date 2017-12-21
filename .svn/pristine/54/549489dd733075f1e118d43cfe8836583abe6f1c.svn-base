/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2008-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,15oct10,kab  Rename BSP from rdb8323
01f,15oct09,l_z  Add auxiliary clock support. (WIND00183633)
01e,09jul09,e_d  Add timer and interrupt component for BootApp and 
                 Fix the issue for modification.
01d,08jan09,z_l  Fix RAM_LOW_ADRS definition error. (WIND00150786)
01c,18dec08,z_l  Fix net device problems in BootApp. (WIND00139815)
01b,29sep08,pgh  Fix PROFILE_BOOTAPP Build problems.
01a,24jun08,d_c  Written
*/

Bsp fsl_rdb8323    {
    CPU         PPC32
    REQUIRES    INCLUDE_KERNEL \
                DRV_INTCTLR_PPC \
                DRV_INTCTLR_QUICC   \
                DRV_SIO_NS16550 \
                DRV_TIMER_DEC_PPC \
                DRV_INTCTLR_QE 

    TOOL        sfdiab
    FP          soft
}

/* Define INCLUDE_BOOT_ETH_MAC_HANDLER when BootApp Networking is enabled */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN    INCLUDE_BOOT_NETWORK 
}

/* required network Boot device */

Component       INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_UCC_VXB_END
}

/* Modify INCLUDE_UCC_VXB_END component */

Component       INCLUDE_UCC_VXB_END {
    REQUIRES += INCLUDE_UCC_MDIO \
                INCLUDE_ICP175CPHY \
                INCLUDE_GENERICPHY \
                INCLUDE_QUICC_ENGINE_UTILS
}

/* auxiliary clock support */

Component DRV_TIMER_QUICC_PPC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x02a00000) \
                (INCLUDE_BOOT_APP)::(0x01a00000) \
                0x00e00000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01a00000) \
                (INCLUDE_BOOT_APP)::(0x00e00000) \
                0x00010000
}
