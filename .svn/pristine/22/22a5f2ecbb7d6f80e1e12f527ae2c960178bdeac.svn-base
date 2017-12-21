/* vxbOmap35xSR.cdf - Component configuration file */

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

Component DRV_OMAP35XSR {
    NAME        Power Management Smart Reflex Driver
    SYNOPSIS    VXBUS Smart Reflex driver
    MODULES     vxbOmap35xSR.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbOmap35xSRDrvRegister();
    PROTOTYPE   void vxbOmap35xSRDrvRegister (void);
    REQUIRES    INCLUDE_PLB_BUS   
}

