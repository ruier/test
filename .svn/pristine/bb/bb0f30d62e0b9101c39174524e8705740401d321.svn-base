/* 40vxbQorIQRman.cdf - Freescal QorIQ Rman Component configuration file */

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

Component DRV_RESOURCE_QORIQRMAN {
        NAME            Freescal QorIQ Rman Controller driver
        SYNOPSIS        Freescal QorIQ Rman Controller driver
        _CHILDREN       FOLDER_DRIVERS
        PROTOTYPE       void rmanRegister (void);
        MODULES         vxbQorIQRman.o
        INIT_RTN        rmanRegister ();
        REQUIRES        INCLUDE_PLB_BUS \
                        DRV_SRIO_FSL
        _INIT_ORDER     hardWareInterFaceBusInit
}

