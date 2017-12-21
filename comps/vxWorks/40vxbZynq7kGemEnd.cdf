/* 40vxbZynq7kGemEnd.cdf - Component configuration file */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,23jun11,rab  written.
*/

Component DRV_VXBEND_ZYNQMAC {
    NAME            Xilinx Zynq-7000 GEM Enhanced Network Driver
    SYNOPSIS        Xilinx Zynq-7000 GEM Enhanced Network Driver
    HDR_FILES       ../src/hwif/h/end/vxbZynq7kGemEnd.h
    _CHILDREN       FOLDER_DRIVERS
    _INIT_ORDER     hardWareInterFaceBusInit
    INIT_RTN        zynq7kGemRegister();
    REQUIRES        INCLUDE_PLB_BUS   \
                    INCLUDE_PARAM_SYS \
                    INCLUDE_MII_BUS
    INIT_AFTER      INCLUDE_PLB_BUS
}
