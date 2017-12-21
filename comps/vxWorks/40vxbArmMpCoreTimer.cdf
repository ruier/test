/* 40vxbArmMpCoreTimer.cdf - ARM MPCore Timer driver */

/*
 * Copyright (c) 2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,21may13,c_l  Modify incorrect description. (WIND00416481)
01a,20dec10,fao  written.
*/

Component DRV_ARM_MPCORE_TIMER {
    NAME        ARM MPCore timer driver
    SYNOPSIS    ARM MPCore timer driver
    MODULES     vxbArmMpCoreTimer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    armMpCoreTimerDrvRegister();
    PROTOTYPE   void armMpCoreTimerDrvRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
