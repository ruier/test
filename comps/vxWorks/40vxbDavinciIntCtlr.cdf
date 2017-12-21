/* 40vxbDavinciIntCtlr.cdf - davinci interrupt controller */

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
01b,03jul08,d_l  change component name.
01a,16jun08,d_l  written
*/

Component DRV_INTCTLR_DAVINCI {
    NAME        TI Davinci Interrupt Controller driver
    SYNOPSIS    Davinci Interrupt Controller driver
    MODULES     vxbDavinciIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbDavinciIntCtlrRegister();
    PROTOTYPE   void vxbDavinciIntCtlrRegister (void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}

