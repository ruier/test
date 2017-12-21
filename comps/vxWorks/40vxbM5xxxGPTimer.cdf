/* 40vxbM5xxxGPTimer.cdf - Component configuration file */

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
01b,16apr08,b_m  changed to library build.
01a,28sep07,b_m  written.
*/

/*******************************************************************************
*
* General Purpose Timer for MPC5xxx
*
*/

Component DRV_TIMER_M5XXX_GPT {
    NAME            Freescale MPC5xxx General Purpose Timer 
    SYNOPSIS        Freescale MPC5xxx General Purpose Timer
    MODULES         vxbM5xxxGPTimer.o
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_TIMER_SYS
    INIT_RTN        m5xxxGPTimerRegister();
    _INIT_ORDER     hardWareInterFaceBusInit
    PROTOTYPE       void m5xxxGPTimerRegister (void);
    _CHILDREN       FOLDER_DRIVERS
}
