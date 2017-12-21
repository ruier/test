/* 40vxbOmap35xDpll.cdf - Component configuration file */

/* Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,30nov10,rec  written
*/

Component DRV_OMAP35XDPLL {
    NAME        Power Management DPLL Driver
    SYNOPSIS    VXBUS Frequency Controller driver
    MODULES     vxbOmap35xDpll.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbOmap35xDpllDrvRegister();
    PROTOTYPE   void vxbOmap35xDpllDrvRegister (void);
    REQUIRES    INCLUDE_PLB_BUS   
}

