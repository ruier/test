/* 40vxbTiAm335xSpi.cdf - Ti Multichannel SPI controller Driver */

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
01a,28nov12,mpc  written.
*/

Component       DRV_TI_AM335X_SPI {
    NAME        Ti AM335x SPI Controller driver
    SYNOPSIS    Ti AM335x SPI Controller driver
    MODULES     vxbTiAm335xSpi.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_SPI_BUS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    tiAm335xSpiRegister();
    PROTOTYPE   void tiAm335xSpiRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_SPI_BUS
}
