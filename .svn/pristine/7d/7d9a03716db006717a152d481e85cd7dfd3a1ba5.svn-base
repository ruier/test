/* 40vxbZynq7kTimer.cd - XILINX ZYNQ7K Timer driver */

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
01a,20sep12,fao  written.
*/

Component DRV_TIMER_ZYNQ7K {
    NAME        XILINX ZYNQ7K timer driver
    SYNOPSIS    XILINX ZYNQ7K timer driver
    MODULES     vxbZynq7kTimer.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    vxbZynqTimerDrvRegister();
    PROTOTYPE   void vxbZynqTimerDrvRegister(void);
    _INIT_ORDER hardWareInterFaceBusInit
    INIT_AFTER  INCLUDE_PLB_BUS
}
