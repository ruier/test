/* 40vxbAltSocGen5Wdt.cdf - VxBus WDT driver for Altera SoC Gen 5 module */

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

Component INCLUDE_ALT_SOC_GEN5_WDT {
    NAME        Altera WDT support
    SYNOPSIS    This component adds support of driver for watchdog timer module in Altera SoC FPGA.
    MODULES     vxbAltSocGen5Wdt.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbAltSocGen5WdtDrvRegister();
    PROTOTYPE   void vxbAltSocGen5WdtDrvRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_SOC_FPGA_TIMER
}
