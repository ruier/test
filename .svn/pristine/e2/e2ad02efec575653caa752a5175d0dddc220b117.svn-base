/* 40vxbAt91I2c.cdf - Ateml TWI controller configuration file */

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
01a,12apr12,e_d written
*/

Component   DRV_I2CBUS_AT91 {
    NAME        atmel TWI bus controller vxBus driver
    SYNOPSIS    atmel TWI bus controller vxBus driver
    MODULES     vxbAt91I2c.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    at91I2cRegister ();
    PROTOTYPE   void at91I2cRegister (void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_I2C_BUS
    INIT_AFTER  INCLUDE_I2C_BUS
}

