/* 40vxbFslEspi.cdf - Freescale eSPI controller Driver */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,28jul12,y_y  written.
*/

Component       DRV_ESPI_FSL {
    NAME        Freescale Enhancement SPI Controller driver
    SYNOPSIS    Freescale Enhancement SPI Controller driver
    MODULES     vxbFslEspi.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_SPI_BUS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    fslEspiRegister();
    PROTOTYPE   void fslEspiRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_SPI_BUS
}