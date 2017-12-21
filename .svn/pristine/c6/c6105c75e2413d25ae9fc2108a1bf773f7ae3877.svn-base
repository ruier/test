
/* Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Component	DRV_GPIO_OMAP35XX {
        NAME            OMAP35xx GPIO Driver
        SYNOPSIS        OMAP35xx GPIO Driver
        MODULES         vxbOmap35xxGpio.o
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        vxbOmap35xxGpioDrvRegister();
        PROTOTYPE       void vxbOmap35xxGpioDrvRegister (void);
        REQUIRES        INCLUDE_PLB_BUS  
}

