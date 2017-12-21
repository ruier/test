/* 40vxbUccSio.cdf - Component configuration file */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,15jan10,x_z  written.
*/

Component DRV_SIO_UCC {
    NAME            UART support for Freescale UCC
    SYNOPSIS        Freescale UCC SIO support
    MODULES         vxbUccSio.o
    SOURCE          ../../../src/hwif/sio
    INIT_RTN        uccSioRegister();
    PROTOTYPE       void uccSioRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_AFTER      INCLUDE_PLB_BUS
    REQUIRES        INCLUDE_VXBUS       \
                    INCLUDE_PLB_BUS     \
                    INCLUDE_SIO_UTILS   \
                    INCLUDE_ISR_DEFER   \
                    INCLUDE_QUICC_ENGINE_UTILS
    _CHILDREN       FOLDER_DRIVERS
}
