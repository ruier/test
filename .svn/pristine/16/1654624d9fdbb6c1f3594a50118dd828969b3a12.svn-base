/* 40vxbComcertoIntCtlr.cdf - Mindspeed Comcerto 100 Interrupt Controller driver */

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
01a,22may08,b_m  written.
*/

Component DRV_INTCTLR_COMCERTO {
    NAME        Mindspeed Comcerto 100 interrupt controller driver
    SYNOPSIS    Mindspeed Comcerto 100 interrupt controller driver
    MODULES     vxbComcertoIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    comcertoIntCtlrRegister();
    PROTOTYPE   void comcertoIntCtlrRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
