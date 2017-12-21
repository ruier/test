/* 40vxbAlteraJtagSio.cdf - Altera Nevada JTAG SIO Driver */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,25jun09,x_f  fix typo
01a,21nov08,l_z  created
*/

Component DRV_SIO_ALTERA_JTAG {
    NAME        Altera Nevada JTAG SIO Driver
    SYNOPSIS    Altera Nevada JTAG SIO Driver
    MODULES     vxbAlteraJtagSio.o
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_SIO_UTILS \
                INCLUDE_ISR_DEFER
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    alteraJtagSioRegister();
    PROTOTYPE   void alteraJtagSioRegister (void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
