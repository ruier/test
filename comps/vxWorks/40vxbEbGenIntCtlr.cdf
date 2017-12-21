/* 40vxbEbGenIntCtlr.cdf - ARM EB GIC interrupt controller */
 
/*
* Copyright (c) 2007-2008 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River License agreement.
*/
 
/*
modification history
--------------------
01c,14feb08,mdo Correct module name
01b,16Jul07,jb   Fix missing driver
01a,12Jul07,jb3  created
*/
 
Component DRV_EB_GIC {
        NAME            EB Generic Interrupt Controller driver
        SYNOPSIS        EB Generic Interrupt Controller driver
        MODULES         vxbEbGenIntrCtl.o
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS \
                        INCLUDE_INTCTLR_LIB
        _CHILDREN       FOLDER_DRIVERS
        INIT_RTN        vxbEbGenIntrCtlRegister();
        PROTOTYPE       void vxbEbGenIntrCtlRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}

