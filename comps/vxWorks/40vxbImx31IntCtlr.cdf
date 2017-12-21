/* 40vxbImx31IntCtlr.cdf - i.MX31 interrupt controller */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,30apr08,l_z  written
*/

Component DRV_INTCTLR_IMX31 {
    NAME        i.MX31 Interrupt Controller driver
    SYNOPSIS    i.MX31 Interrupt Controller driver
    MODULES     vxbImx31IntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbImx31IntCtlrRegister();
    PROTOTYPE   void vxbImx31IntCtlrRegister (void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
