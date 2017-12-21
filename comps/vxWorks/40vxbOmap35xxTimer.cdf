/* 40Omap35xxTimer.cdf - Component configuration file */

/* Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,12aug08,m_h  written
*/

Component	DRV_TIMER_OMAP35XX {
        NAME            OMAP35xx timer Driver
        SYNOPSIS        OMAP35xx timer Driver
        MODULES         vxbOmap35xxTimer.o
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        vxbOmap35xxTimerDrvRegister();
        PROTOTYPE       void vxbOmap35xxTimerDrvRegister (void);
        REQUIRES        INCLUDE_PLB_BUS   \
                        INCLUDE_PARAM_SYS \
                        INCLUDE_TIMER_SYS
}

