/* 40vxbAlteraTimer.cdf - Altera Nevada Timer Driver */

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
01a,21nov08,l_z  created
*/

Component DRV_TIMER_ALTERA {
    NAME        Altera Nevada timer driver
    SYNOPSIS    Altera Nevada timer driver
    MODULES     vxbAlteraTimer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    alteraTimerRegister();
    PROTOTYPE   void alteraTimerRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
