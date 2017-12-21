/* 40vxbM512xDma.cdf - Freescale MPC512x DMA Controller Driver */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,20aug08,b_m  convert to library build.
01a,30oct07,b_m  written.
*/

Component DRV_DMA_M512X {
    NAME        Freescale MPC512x DMA Controller Driver
    SYNOPSIS    Freescale MPC512x DMA Controller Driver
    MODULES     vxbM512xDma.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    m512xDmaRegister();
    PROTOTYPE   void m512xDmaRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
