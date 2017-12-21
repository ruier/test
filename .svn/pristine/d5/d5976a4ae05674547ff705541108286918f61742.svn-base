/* 40vxbAt91sam9260IntCtlr.cdf - At91 interrupt controller configuration file */

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
01b,15oct08,b_m  fix macro typos.
01a,25apr08,x_s written
*/

Component DRV_INTCTLR_AT91 {
    NAME        at91 Interrupt Controller driver
    SYNOPSIS    at91 Interrupt Controller driver
    MODULES     vxbAt91sam9260IntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbAt91IntCtlrRegister();
    PROTOTYPE   void vxbAt91IntCtlrRegister (void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}


