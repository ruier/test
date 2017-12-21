/* 40vxbXlnxUartLiteSio.cdf - Xlnx Lite Uart controller driver */

/*
 * Copyright (c) 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,17nov10,e_d  replace DRV_SIO_LITE_UART with DRV_SIO_UART_LITE
01a,09dec09,e_d  written.
*/

Component DRV_SIO_UART_LITE {
    NAME            UART support for XLNX LITE
    SYNOPSIS        XLNX LITE UART support
    MODULES         vxbXlnxUartLiteSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        xlnxUartLiteRegister();
    PROTOTYPE       void xlnxUartLiteRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    REQUIRES        INCLUDE_VXBUS \
                    INCLUDE_PLB_BUS \
                    INCLUDE_SIO_UTILS
    _CHILDREN       FOLDER_DRIVERS
}