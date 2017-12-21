/* 40vxbComcertoTimer.cdf - Mindspeed Comcerto 100 Timer Driver */

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
01a,18may08,b_m  written.
*/

Component DRV_TIMER_COMCERTO {
    NAME        Mindspeed Comcerto 100 timer driver 
    SYNOPSIS    Mindspeed Comcerto 100 timer driver
    MODULES     vxbComcertoTimer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    comcertoTimerRegister();
    PROTOTYPE   void comcertoTimerRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
