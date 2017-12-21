/* 40vxbPpcQuiccTimer64.cdf - Component configuration file */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,07jun13,y_y  written
*/

Component DRV_TIMER64_QUICC_PPC {
        NAME            PPC Quicc 64-bit Timer(GTM)
        SYNOPSIS        PPC Quicc 64-bit Timer(GTM) for vxBus
        _CHILDREN       FOLDER_DRIVERS
        MODULES         vxbPpcQuiccTimer64.o
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_TIMER_SYS
        INIT_RTN        quiccTimer64DrvRegister();
        PROTOTYPE       void quiccTimer64DrvRegister (void);
        _INIT_ORDER     hardWareInterFaceBusInit
        _CHILDREN       FOLDER_DRIVERS
        _DEFAULTS       += FOLDER_DRIVERS
}