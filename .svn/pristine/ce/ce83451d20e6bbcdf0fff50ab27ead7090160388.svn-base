/* 40vxbI2cPcfRtc.cdf - I2C RTC component configuration file */

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
01a,13jun12,fao written
*/

Component   DRV_I2C_PCFRTC {
    NAME        PCF8564 I2C RTC vxBus driver
    SYNOPSIS    PCF8654 I2C RTC vxBus driver
    MODULES     vxbI2cPcfRtc.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_RTN    vxbI2cPcfRtcRegister ();
    PROTOTYPE   void vxbI2cPcfRtcRegister(void);
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_I2C_BUS
    INIT_AFTER  INCLUDE_I2C_BUS
}
