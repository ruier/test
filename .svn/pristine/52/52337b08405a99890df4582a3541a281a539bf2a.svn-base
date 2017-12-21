/* 40vxbMpicIntCtlr.cdf - Multicore Programmable Interrupt Controller driver */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,20jul10,x_z  written.
*/

Component DRV_INTCTLR_MPIC {
    NAME        Multicore Programmable Interrupt Controller driver
    SYNOPSIS    Multicore Programmable Interrupt Controller driver
    MODULES     vxbMpicIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    mpicIntCtlrRegister ();
    PROTOTYPE   void mpicIntCtlrRegister (void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
