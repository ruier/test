/* 40vxbPxa3xxGpioIntCtlr.cdf - pxa3xx gpio interrupt controller */

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

Component DRV_INTCTLR_PXA3XXGPIO {
    NAME        Marvell pxa3xx GPIO Interrupt Controller driver
    SYNOPSIS    pxa3xx GPIO Interrupt Controller driver
    MODULES     vxbPxa3xxGpioIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbPxa3xxGpioIntCtlrRegister();
    PROTOTYPE   void vxbPxa3xxGpioIntCtlrRegister (void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}

