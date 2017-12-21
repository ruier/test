/* 40vxbAds5121eCpldIntCtlr.cdf - Freescale ADS5121E CPLD Interrupt Controller Driver */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,16dec,x_f  written.
*/

Component DRV_INTCTLR_ADS5121E_CPLD {
    NAME        Freescale ADS5121E CPLD Interrupt Controller Driver
    SYNOPSIS    Freescale ADS5121E CPLD Interrupt Controller Driver
    MODULES     vxbAds5121eCpldIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    ads5121eCpldIntCtlrRegister();
    PROTOTYPE   void ads5121eCpldIntCtlrRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}
