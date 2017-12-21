/* 40vxbPpc440Timer.cdf - PowerPC 440 Timer Driver */

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

Component DRV_TIMER_PPC440 {
    NAME        Timer driver for PowerPC 440
    SYNOPSIS    Timer driver for PowerPC 440
    MODULES     vxbPpc440Timer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    ppc440TimerRegister();
    PROTOTYPE   void ppc440TimerRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
}
