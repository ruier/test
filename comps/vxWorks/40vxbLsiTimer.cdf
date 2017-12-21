/* 40vxbLsiTimer.cdf - Component configuration file */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,04may10,x_z  written
*/

Component DRV_TIMER_LSI {
        NAME            LSI ACP34XX Timer
        SYNOPSIS        LSI ACP34XX Timer for vxBus
        CHILDREN        FOLDER_DRIVERS
        MODULES         vxbLsiTimer.o
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_TIMER_SYS
        INIT_RTN        vxbLsiTimerRegister();
        PROTOTYPE       void vxbLsiTimerRegister (void);
        _INIT_ORDER     hardWareInterFaceBusInit
        _CHILDREN       FOLDER_DRIVERS
        _DEFAULTS       += FOLDER_DRIVERS
}
