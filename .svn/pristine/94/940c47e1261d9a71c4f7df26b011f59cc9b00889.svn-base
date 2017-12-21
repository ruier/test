/* 40vxbLsiSRio.cdf - LSI SRIO Component configuration file */

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
01a,22oct12,j_z  created
*/

Component DRV_SRIO_LSI {
        NAME            LSI Serial RapidIO controller driver
        SYNOPSIS        LSI Serial RapidIO controller driver
        _CHILDREN       FOLDER_DRIVERS
        PROTOTYPE       void lsiRioRegister (void);
        MODULES         vxbLsiSRio.o
        INIT_RTN        lsiRioRegister ();
        REQUIRES        INCLUDE_PLB_BUS \
                        INCLUDE_RAPIDIO_BUS \
                        DRV_SRIO_CPU_EP
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_AFTER      INCLUDE_RAPIDIO_BUS
}

