/* 40wrsample.cdf - 3RD-Party Sample Driver Component configuration file */ 

/*
 * Copyright (C) 2007-2008 Wind River Systems, Inc. All rights are reserved.
 * 
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history 
--------------------
01c,13oct08,h_k  fixed Copyright format.
01b,02sep08,h_k  added missing MODULES and PROTOTYPE attributes.
                 added DRV_DEMO_WRSAMPLE_DEBUG for debug tip.
01a,21aug07,tor  created 
*/

/**********************************************************************
*
* sample 3rd-party VxBus driver provided by Wind River
*
*/
/*  MODULES 				vxbm8641I2c.o
				SOURCE 					$(WIND_BASE)/target/config/petSbc8640d
				*/
				
Component DRV_M8641_IIC {
        NAME            I2C Control VxBus driver
        SYNOPSIS        I2C Control VxBus driver provided by prophet
      
        PROTOTYPE       void m8641I2cDrvRegister(void);
        INIT_RTN        m8641I2cDrvRegister();
         _CHILDREN       FOLDER_PROPHET_DEV_DRIVER
        _INIT_ORDER     hardWareInterFaceBusInit
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS
         INIT_AFTER      INCLUDE_PLB_BUS
}


