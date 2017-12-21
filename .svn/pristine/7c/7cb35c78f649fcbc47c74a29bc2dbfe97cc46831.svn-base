/* 40vxbOmapl137IntCtlr.cdf - OMAP-L137 interrupt controller */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,24jan09,fao  written
*/

Component   DRV_INTCTLR_OMAPL137 {
    NAME        OMAP-L137 Interrupt Controller driver
    SYNOPSIS    OMAP-L137 Interrupt Controller driver
    MODULES     vxbOmapL137IntCtlr.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void vxbOmapL137IntCtlrRegister(void);
    INIT_RTN    vxbOmapL137IntCtlrRegister();
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
}
