/* 40vxbAltSocGen5Timer.cdf - Altera SOC Gen 5 Timer Driver */

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

Component INCLUDE_ALT_SOC_GEN5_TIMER {
    NAME        Altera SoC Gen 5 timer driver
    SYNOPSIS    Altera SoC Gen 5 timer driver
    MODULES     vxbAltSocGen5Timer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbAltSocGen5TimerRegister();
    PROTOTYPE   void vxbAltSocGen5TimerRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
