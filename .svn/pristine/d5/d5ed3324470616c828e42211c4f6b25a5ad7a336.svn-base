/* 40vxbAlteraIntCtlr.cdf - Altera Nevada Interrupt Controller driver */

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

Component DRV_INTCTLR_ALTERA {
    NAME        Altera Nevada interrupt controller driver
    SYNOPSIS    Altera Nevada interrupt controller driver
    MODULES     vxbAlteraIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbAlteraIntCtlrRegister ();
    PROTOTYPE   void vxbAlteraIntCtlrRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
