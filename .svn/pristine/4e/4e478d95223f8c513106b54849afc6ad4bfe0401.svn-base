/* 40vxbRioCpuEp.cdf - generic RapidIO CPU EP configuration file */

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

Component DRV_SRIO_CPU_EP {
        NAME            generic RapidIO CPU EP device driver
        SYNOPSIS        generic RapidIO CPU EP device driver
        _CHILDREN       FOLDER_DRIVERS
        PROTOTYPE       void rioCpuRegister (void);
        MODULES         vxbRioCpuEp.o
        INIT_RTN        rioCpuRegister ();
        REQUIRES        INCLUDE_PLB_BUS \
                        INCLUDE_RAPIDIO_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_AFTER      INCLUDE_RAPIDIO_BUS
}
