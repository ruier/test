/* 40vxbImx51IntCtlr.cdf - i.MX51 TrustZone Interrupt Controller */

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
01a,31aug09,d_l  written
*/

Component DRV_INTCTLR_IMX51 {
    NAME        i.MX51 TrustZone Interrupt Controller driver
    SYNOPSIS    i.MX51 TrustZone Interrupt Controller driver
    MODULES     vxbImx51IntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbImx51IntCtlrRegister();
    PROTOTYPE   void vxbImx51IntCtlrRegister (void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
