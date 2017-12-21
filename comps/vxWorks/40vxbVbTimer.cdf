/* 40vxbVbTimer.cdf - Virtual board interrupt controller */

/*
* Copyright (c) 2009 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River License agreement.
*/

/*
modification history
--------------------
01a,20Feb09,rec  created
*/

/********************************************************************************
* virtual timer for vxBus
*
*/
Component DRV_TIMER_VB {
    NAME            virtual timer driver for vxBus
    SYNOPSIS        Timer Driver support
    MODULES         vxbVbTimer.o
    INIT_RTN        vxbVbTimerDrvRegister();
    PROTOTYPE       void vxbVbTimerDrvRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_TIMER_SYS
    _CHILDREN       FOLDER_DRIVERS
    _DEFAULTS       += FOLDER_DRIVERS
}

