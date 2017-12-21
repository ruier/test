/* 40vxbImxEpitTimer.cdf - Component configuration file */

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
01b,08may08,l_z  change comment
01a,13mar08,l_z  written
*/

Component    DRV_TIMER_IMX31EPIT {
    NAME         i.MX31 EPIT timer driver
    SYNOPSIS     i.MX31 EPIT timer driver
    MODULES      vxbImx31EpitTimer.o
    PROTOTYPE    void vxbImx31EpitTimerDrvRegister(void);
    _CHILDREN    FOLDER_DRIVERS
    _INIT_ORDER  hardWareInterFaceBusInit
    INIT_RTN     vxbImx31EpitTimerDrvRegister();
    REQUIRES     INCLUDE_PLB_BUS   \
                 INCLUDE_PARAM_SYS \
                 INCLUDE_TIMER_SYS
}

