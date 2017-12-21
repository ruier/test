/* 40vxbFslDma.cdf - Freescale DMA Controller Driver */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,15jun09,syt  written.
*/

Component DRV_DMA_FSL {
    NAME        Freescale DMA Controller Driver
    SYNOPSIS    Freescale DMA Controller Driver
    MODULES     vxbFslDma.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbFslDmaRegister();
    PROTOTYPE   void vxbFslDmaRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
