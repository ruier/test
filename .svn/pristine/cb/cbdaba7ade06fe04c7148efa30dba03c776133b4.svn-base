/* 40vxbPpc4xxUicIntCtlr.cdf - PPC4xx Universal Interrupt Controller driver */

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
01a,31mar08,b_m  written.
*/

Component DRV_INTCTLR_UIC {
    NAME        Universal Interrupt Controller driver for PowerPC 4xx
    SYNOPSIS    Universal Interrupt Controller driver for PowerPC 4xx
    MODULES     vxbPpc4xxUicIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    uicIntCtlrRegister();
    PROTOTYPE   void uicIntCtlrRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
