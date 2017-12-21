/* 40vxbImxSdma.cdf - Freescale IMX SDMA Controller Driver */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,25apr11,hcl  written.
*/

Component DRV_SDMA_IMX {
    NAME        Freescale IMX SDMA Controller Driver
    SYNOPSIS    Freescale IMX SDMA Controller Driver
    MODULES     vxbImxSdma.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    imxSdmaRegister();
    PROTOTYPE   void imxSdmaRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
