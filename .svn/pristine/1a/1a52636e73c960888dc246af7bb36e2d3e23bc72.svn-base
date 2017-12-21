/* 40vxbOmap2530IntCtlr.cdf - OMAP2530 interrupt controller */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,04Dec08,fao written
*/

Component   DRV_INTCTLR_OMAP2530 {
    NAME        OMAP2530 Interrupt Controller driver
    SYNOPSIS    OMAP2530 Interrupt Controller driver
    MODULES     vxbOmap2530IntCtlr.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE       void vxbOmap2530IntCtlrRegister(void);
    INIT_RTN    vxbOmap2530IntCtlrRegister();
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
}
