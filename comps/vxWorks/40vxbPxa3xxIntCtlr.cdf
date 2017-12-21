/* 40vxbPxa3xxIntCtlr.cdf - pxa3xx interrupt controller */

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
01a,06apr09,d_l  written
*/

Component DRV_INTCTLR_PXA3XX {
    NAME        Marvell pxa3xx Interrupt Controller driver
    SYNOPSIS    pxa3xx Interrupt Controller driver
    MODULES     vxbPxa3xxIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbPxa3xxIntCtlrRegister();
    PROTOTYPE   void vxbPxa3xxIntCtlrRegister (void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}

