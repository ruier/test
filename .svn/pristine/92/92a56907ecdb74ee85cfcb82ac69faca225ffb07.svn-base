/* 40vxbOmap35xxIntCtlr.cdf - OMAP 35xx interrupt controller */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,13jul08,m_h  created
*/

Component DRV_INTCTLR_OMAP35XX {
        NAME            OMAP 35xx Interrupt Controller driver
        SYNOPSIS        OMAP 35xx Interrupt Controller driver
        MODULES         vxbOmap35xxIntCtlr.o
        REQUIRES        INCLUDE_VXBUS       \
                        INCLUDE_PLB_BUS     \
                        INCLUDE_INTCTLR_LIB
        _CHILDREN       FOLDER_DRIVERS
        INIT_RTN        vxbOmap35xxIntCtlrRegister();
        PROTOTYPE       void vxbOmap35xxIntCtlrRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}
