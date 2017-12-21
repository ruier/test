/* 40vxbDavinciTimer.cdf - Davinci timer configuration file */
                                                                                
/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,18jun08,d_l fix typo about module name.
01a,25feb08,d_l written
*/

Component   DRV_TIMER_DAVINCI {
    NAME        DAVINCI Timer Driver
    SYNOPSIS    DAVINCI Timer Driver
    MODULES     vxbDavinciTimer.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbDavinciTimerDrvRegister();
    PROTOTYPE   void vxbDavinciTimerDrvRegister (void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_PARAM_SYS \
                INCLUDE_TIMER_SYS
}

