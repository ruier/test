/* 40vxbXlnxDma.cdf - Xlnx DMA controller driver */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01b,17nov10,e_d  replace INCLUDE_VXB_XLNX_DMA with DRV_DMA_XLNX
01a,05jan10,e_d  written

*/

Component DRV_DMA_XLNX {
    NAME            vxbXlnxDma ML507 DMA support for vxBus
    SYNOPSIS        DMA support
    MODULES         vxbXlnxDma.o
    _CHILDREN       FOLDER_DRIVERS
    INIT_RTN        xlnxDmaRegister();
    PROTOTYPE       void xlnxDmaRegister(void);
    _INIT_ORDER     hardWareInterFaceBusInit
    REQUIRES        INCLUDE_VXBUS   \
                    INCLUDE_PLB_BUS
    INIT_AFTER      INCLUDE_PLB_BUS
}