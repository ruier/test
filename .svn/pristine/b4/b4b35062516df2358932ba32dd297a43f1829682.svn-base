/* 40vxbLpc24xxTimer.cdf - LPC24XX Timer Driver */

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

Component DRV_TIMER_LPC24XX {
    NAME        LPC24XX timer driver
    SYNOPSIS    LPC24XX timer driver
    MODULES     vxbLpc24xxTimer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbLpc24xxTimerDrvRegister();
    PROTOTYPE   void vxbLpc24xxTimerDrvRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
