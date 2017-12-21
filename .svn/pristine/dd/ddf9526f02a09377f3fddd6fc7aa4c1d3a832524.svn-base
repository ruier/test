/* 40vxbTiEdma3.cdf - TI EDMA3 Controller Driver */

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
01a,20jul11,my_  written.
*/

Component DRV_TI_EDMA3 {
    NAME        TI EDMA3 Controller Driver
    SYNOPSIS    TI EDMA3 Controller Driver
    MODULES     vxbTiEdma3.o
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_DMA_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    tiEdma3Register();
    PROTOTYPE   void tiEdma3Register(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
