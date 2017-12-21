/* 40vxbOmapl13xI2c.cdf - Omapl13x I2C controller configuration file */

/* Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,01dec11,hcl  written
*/

Component	DRV_I2C_OMAPL13X {
        NAME            TI Omapl13x I2C Driver
        SYNOPSIS        TI Omapl13x I2C Driver
        MODULES         vxbOmapl13xI2c.o
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        omapl13xI2cRegister();
        PROTOTYPE       void omapl13xI2cRegister(void);
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_I2C_BUS
        INIT_AFTER      INCLUDE_I2C_BUS
}
