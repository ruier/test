/* 40vxbI2cRtc.cdf - I2C RTC component configuration file */

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
01b,11apr12,y_y add PROTOTYPE to DRV_I2C_RTC component.
01a,03feb12,l_z written
*/

Component   DRV_I2C_RTC {
    NAME        I2C RTC vxBus driver
    SYNOPSIS    i2c RTC vxBus driver
    MODULES     vxbI2cRtc.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbI2cRtcRegister ();
    PROTOTYPE   void vxbI2cRtcRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_I2C_BUS
    INIT_AFTER  INCLUDE_I2C_BUS
}

