/* 40vxbArmGenIntCtlr.cdf - ARM GIC interrupt controller */
 
/*
* Copyright (c) 2008 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River License agreement.
*/
 
/*
modification history
--------------------
01a,12nov08,j_b  created
*/
 
Component DRV_ARM_GIC {
        NAME            ARM Generic Interrupt Controller driver
        SYNOPSIS        ARM Generic Interrupt Controller driver
        MODULES         vxbArmGenIntCtlr.o
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS \
                        INCLUDE_INTCTLR_LIB
        _CHILDREN       FOLDER_DRIVERS
        INIT_RTN        vxbArmGenIntCtlrRegister();
        PROTOTYPE       void vxbArmGenIntCtlrRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}

