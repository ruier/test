/* 40vxbPxa3xxTimer.cdf - pxa3xx timer configuration file */
                                                                                
/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,06apr09,d_l  written
*/

Component   DRV_TIMER_PXA3XX {
    NAME        PXA3XX Timer Driver
    SYNOPSIS    PXA3XX Timer Driver
    MODULES     vxbPxa3xxTimer.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbPxa3xxTimerRegister();
    PROTOTYPE   void vxbPxa3xxTimerRegister (void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_PARAM_SYS \
                INCLUDE_TIMER_SYS
}

