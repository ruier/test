/* 40vxbI2cEeprom.cdf - I2C Vitesse VSC3316/3308 component configuration file */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,16jan13,wap  written
*/

Component   DRV_I2C_VSC3316 {
    NAME        I2C Vitesse VSC3316/3308 crossbar switch vxBus driver
    SYNOPSIS    i2c Vitesse VSC3316/3308 crossbar switch vxBus driver
    MODULES     vxbI2cVsc3316.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbI2cVsc3316Register ();
    PROTOTYPE   void vxbI2cVsc3316Register (void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_I2C_BUS 
    INIT_AFTER  INCLUDE_I2C_BUS
}

