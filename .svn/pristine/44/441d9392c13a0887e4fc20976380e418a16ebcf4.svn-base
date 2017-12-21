/* 40vxbIbmMalDma.cdf - Component configuration file */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01a,01may07,rcs  written

*/

Component INCLUDE_VXB_IBM_MAL {
    NAME            vxbIbmMalDma PPC440GX DMA support for vxBus
    SYNOPSIS        DMA support
    MODULES         vxbIbmMalDma.o
    _CHILDREN       FOLDER_DRIVERS
    INIT_RTN        vxbMalRegister();
    PROTOTYPE       void vxbMalRegister (void);
    _INIT_ORDER     hardWareInterFaceBusInit
    REQUIRES        INCLUDE_VXBUS   \
                    INCLUDE_PLB_BUS
    INIT_AFTER      INCLUDE_PLB_BUS
}
