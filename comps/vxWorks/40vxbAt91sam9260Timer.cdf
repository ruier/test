/* 40vxbAt91sam9260Timer.cdf - At91 timer configuration file */

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
01a,25apr08,x_s written
*/

Component DRV_TIMER_AT91 {
    NAME            AT91 timer Driver
    SYNOPSIS        AT91 timer Driver
    MODULES         vxbAt91sam9260Timer.o
    _CHILDREN       FOLDER_DRIVERS
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_RTN        vxbAt91TimerDrvRegister();
    PROTOTYPE       void vxbAt91TimerDrvRegister (void);
    REQUIRES        INCLUDE_PLB_BUS \
                    INCLUDE_PARAM_SYS \
                    INCLUDE_TIMER_SYS
}

