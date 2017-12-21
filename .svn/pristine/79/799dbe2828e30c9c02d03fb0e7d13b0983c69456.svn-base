
/* Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Component	DRV_I2C_OMAP35XX {
        NAME            OMAP35xx I2C Driver
        SYNOPSIS        OMAP35xx I2C Driver
        MODULES         vxbOmap35xxI2C.o
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        vxbOmap35xxI2cDrvRegister();
        PROTOTYPE       void vxbOmap35xxI2cDrvRegister (void);
        REQUIRES        INCLUDE_PLB_BUS  
}

