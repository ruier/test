/* 40vxbI2cGenericDev.cdf - I2C Generic device configuration file */

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
01b,11apr12,y_y  add PROTOTYPE to DRV_I2C_GENERIC_DEV component.
01a,09mar12,y_y  written
*/

Component   DRV_I2C_GENERIC_DEV {
    NAME        I2C generic device vxBus driver
    SYNOPSIS    I2C generic device vxBus driver
    MODULES     vxbI2cGenericDev.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbI2cGenericDevRegister ();
    PROTOTYPE   void vxbI2cGenericDevRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_I2C_BUS 
    INIT_AFTER  INCLUDE_I2C_BUS
}
