/* 40vxbAm38xxI2c.cdf - AM38xx I2C controller configuration file */

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

Component	DRV_I2C_AM38XX {
        NAME            AM38XX I2C Driver
        SYNOPSIS        AM38XX I2C Driver
        MODULES         vxbAm38xxI2c.o
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        am38xxI2cRegister();
        PROTOTYPE       void am38xxI2cRegister(void);
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_I2C_BUS
        INIT_AFTER      INCLUDE_I2C_BUS
}
