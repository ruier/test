/* 40vxbAltSocGen5DwI2c.cdf - VxBus I2C driver for DesignWare I2C Controller */

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
01a,13mar13,swu  created
*/

Component INCLUDE_ALT_SOC_GEN5_DW_I2C {
        NAME            Altera SoC Gen 5 DesignWare I2C support
        SYNOPSIS        Altera SoC Gen 5 DesignWare I2C
        MODULES         vxbAltSocGen5DwI2c.o
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        vxbAltSocGen5DwI2cRegister();
        PROTOTYPE       void vxbAltSocGen5DwI2cRegister(void);
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_I2C_BUS
        INIT_AFTER      INCLUDE_I2C_BUS
}
