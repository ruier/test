/* 40vxbLpc24xxIntCtlr.cdf - LPC24XX Interrupt Controller driver */

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
01a,22dec08,x_s  written.
*/

Component DRV_INTCTL_LPC24XX {
    NAME        LPC24XX interrupt controller driver
    SYNOPSIS    LPC24XX interrupt controller driver
    MODULES     vxbLpc24xxIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbLpc24xxIntCtlrRegister();
    PROTOTYPE   void vxbLpc24xxIntCtlrRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
