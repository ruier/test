/* 40vxbXlnxIntCtlr.cdf - Xilinx Interrupt Controller driver */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,05jan10,e_d  written
*/

Component DRV_INTCTLR_XLNX {
    NAME        Interrupt Controller driver for Xilinx ML507 
    SYNOPSIS    Interrupt Controller driver for Xilinx ML507 
    MODULES     vxbXlnxIntCtlr.o
    REQUIRES    INCLUDE_VXBUS \
                INCLUDE_PLB_BUS \
                INCLUDE_INTCTLR_LIB
    _CHILDREN   FOLDER_DRIVERS
    INIT_RTN    xlnxIntCtlrRegister();
    PROTOTYPE   void xlnxIntCtlrRegister(void);
    INIT_AFTER  INCLUDE_PLB_BUS
    _INIT_ORDER hardWareInterFaceBusInit
}