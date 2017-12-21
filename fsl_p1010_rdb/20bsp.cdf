/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,05mar12,y_y  remove I2C related component for vxBus I2C.
01a,22octl1,fao  derived from version 01a of fsl_p2020_rdb/20bsp.cdf
*/

Bsp fsl_p1010_rdb {
    CPU         PPC32
    ENDIAN      big
    FP          e500v2 soft 
    REQUIRES    INCLUDE_KERNEL      \
                DRV_INTCTLR_EPIC    \
                DRV_INTCTLR_PPC     \
                DRV_TIMER_M85XX     \
                DRV_SIO_NS16550
}

Component INCLUDE_ETSEC_VXB_END {
    REQUIRES += INCLUDE_GENERICPHY  \
                INCLUDE_TSEC_MDIO   \
                INCLUDE_VSC82XXPHY  \
                INCLUDE_DMA_SYS     \
                INCLUDE_MII_BUS
}

/* network boot devices for bootapp */

Component INCLUDE_BOOT_NET_DEVICES {
    REQUIRES    INCLUDE_ETSEC_VXB_END
}

/* enable M command in bootapp */

Component INCLUDE_BOOT_ETH_MAC_HANDLER {
    INCLUDE_WHEN += INCLUDE_BOOT_NETWORK
}

Parameter RAM_HIGH_ADRS {
    NAME        Bootrom Copy region
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01800000)  \
                (INCLUDE_BOOT_APP)::(0x01400000)        \
                0x01000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_RAM_IMAGE)::(0x01400000)  \
                (INCLUDE_BOOT_APP)::(0x01000000)        \
                0x00100000
}

/* use openPicTimer as the auxClk */

Component DRV_TIMER_OPENPIC {
    INCLUDE_WHEN += INCLUDE_AUX_CLK
}

Component INCLUDE_PCI_BUS {
        REQUIRES        INCLUDE_PCI_BUS_AUTOCONF \
                        DRV_PCIBUS_QORIQ_PCIEX
}

/* Freescale eSPI Controller Driver */

Component INCLUDE_FSL_SPI {
        NAME            SPI Bus Controller Driver
        SYNOPSIS        Freescale eSPI Controller Driver
        _CHILDREN       FOLDER_DRIVERS
}
