/* 40vxbM85xxTimer.cdf - Component Definition file for m85xx timer */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,13aug08,l_g  add INCLUDE_TIMER_SYS to support BOOTAPP build
01a,11Jul07,vik  written
*/

Component       DRV_TIMER_M85XX {
    NAME        m85xx Timer Driver
    SYNOPSIS    vxBus Driver for m85xx Timer
    MODULES     vxbM85xxTimer.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void m85xxTimerDrvRegister (void);
    INIT_RTN    m85xxTimerDrvRegister();
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
}
