/* 40vxbFslI2c.cdf - freescale I2C controller configuration file */

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
01b,11apr12,y_y add PROTOTYPE to DRV_I2CBUS_FSL component.
01a,03feb12,l_z written
*/

Component   DRV_I2CBUS_FSL {
    NAME        freescale I2C bus controller vxBus driver
    SYNOPSIS    freescale i2c bus controller vxBus driver
    MODULES     vxbFslI2c.o
    _CHILDREN   FOLDER_DRIVERS
    HDR_FILES   ../src/hwif/h/i2c/vxbFslI2c.h
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    fslI2cRegister ();
    PROTOTYPE   void fslI2cRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_I2C_BUS
    INIT_AFTER  INCLUDE_I2C_BUS
}

