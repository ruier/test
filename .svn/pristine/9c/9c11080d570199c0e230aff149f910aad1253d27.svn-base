/* 40vxbOmap2530Timer.cdf - Component Definition file for Omap2530 timer */
                                                                                
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
01a,04Dec08,fao written
*/

Component   DRV_TIMER_OMAP2530 {
    NAME        OMAP2530 Timer Driver
    SYNOPSIS    vxBus Driver for OMAP2530 Timer
    MODULES     vxbOmap2530Timer.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE       void vxbOmap2530TimerDrvRegister(void);
    INIT_RTN    vxbOmap2530TimerDrvRegister();
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_TIMER_SYS \
                INCLUDE_PLB_BUS
}
