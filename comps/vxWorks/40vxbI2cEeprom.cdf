/* 40vxbI2cEeprom.cdf - I2c EEPROM component configuration file */

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
01b,11apr12,y_y add PROTOTYPE to DRV_I2C_EEPROM component.
01a,03feb12,l_z written
*/

Component   DRV_I2C_EEPROM {
    NAME        I2C EEPROM vxBus driver
    SYNOPSIS    i2c EEPROM vxBus driver
    MODULES     vxbI2cEeprom.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbI2cEepromRegister ();
    PROTOTYPE   void vxbI2cEepromRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_I2C_BUS 
    INIT_AFTER  INCLUDE_I2C_BUS
}

