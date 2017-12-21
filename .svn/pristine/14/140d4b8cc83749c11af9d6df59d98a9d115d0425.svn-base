/* 40vxbOmap35xxDma.cdf - TI Omap35xx DMA Controller Driver */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,20jul10,hcl  written.
*/

Component DRV_DMA_OMAP35XX {
    NAME        TI Omap35xx DMA Controller Driver
    SYNOPSIS    TI Omap35xx DMA Controller Driver
    MODULES     vxbOmap35xxDma.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    omap35xxDmaRegister();
    PROTOTYPE   void omap35xxDmaRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
