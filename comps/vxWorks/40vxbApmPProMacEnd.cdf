/* 40vxbApmPProMacEnd.cdf - APM PacketPro MAC END Driver */

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
01a,12jul11,x_z  written.
*/

Component DRV_VXBEND_APM_PPRO_MAC {
    NAME        APM PacketPro MAC VxBus Enhanced Network Driver
    SYNOPSIS    APM PacketPro MAC VxBus Enhanced Network Driver
    MODULES     vxbApmPProMacEnd.o
    REQUIRES    INCLUDE_PLB_BUS     \
                INCLUDE_PARAM_SYS   \
                INCLUDE_DMA_SYS     \
                INCLUDE_END         \
                DRV_APM_PPRO_QMAN
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbApmPProMacEndRegister();
    PROTOTYPE   void vxbApmPProMacEndRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  DRV_APM_PPRO_QMAN
}
