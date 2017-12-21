/* 40vxZynq7kI2c.cdf - ZYNQ7K I2C controller configuration file */

/* Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,29may12,fao  written
*/

Component	DRV_I2CBUS_ZYNQ7K {
        NAME            XILINX ZYNQ7K I2C Driver
        SYNOPSIS        XILINX ZYNQ7K I2C Driver
        MODULES         vxbZynq7kI2c.o
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        zynq7kI2cRegister();
        PROTOTYPE       void zynq7kI2cRegister(void);
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_I2C_BUS
        INIT_AFTER      INCLUDE_I2C_BUS
}
