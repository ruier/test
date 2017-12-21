/* 40vxbPpc405Timer.cdf - PowerPC 405 Timer Driver */

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
01a,28apr08,b_m  written.
*/

Component DRV_TIMER_PPC405 {
    NAME        Timer driver for PowerPC 405
    SYNOPSIS    Timer driver for PowerPC 405
    MODULES     vxbPpc405Timer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    ppc405TimerRegister();
    PROTOTYPE   void ppc405TimerRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
}
