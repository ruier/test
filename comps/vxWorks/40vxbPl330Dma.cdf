/* 40vxbPl330Dma.cdf - ARM Pl330 DMA Controller Driver */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,20aug20,fao  written.
*/

Component DRV_DMA_PL330 {
    NAME        ARM Pl330 DMA Controller Driver
    SYNOPSIS    ARM Pl330 DMA Controller Driver
    MODULES     vxbPl330Dma.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    pl330DmaRegister();
    PROTOTYPE   void pl330DmaRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
