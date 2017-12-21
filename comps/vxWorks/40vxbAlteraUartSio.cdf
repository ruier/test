/* 40vxbAlteraUartSio.cdf - Altera Avalon UART SIO Driver */

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
01a,18jun09,x_f  created
*/

Component DRV_SIO_ALTERA_UART {
    NAME        Altera Avalon UART SIO Driver
    SYNOPSIS    Altera Avalon UART SIO Driver
    MODULES     vxbAlteraUartSio.o
    REQUIRES    INCLUDE_VXBUS   \
                INCLUDE_PLB_BUS \
                INCLUDE_SIO_UTILS \
                INCLUDE_ISR_DEFER
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    altUartSioRegister();
    PROTOTYPE   void altUartSioRegister (void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
