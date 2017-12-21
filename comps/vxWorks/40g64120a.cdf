/* 40g64120a.cdf - Galileo g64120a system controller */

/*
 * Copyright (c) 2006, 2007, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,21aug12,l_z  clean up unsupported component. (WIND00105282)
01c,24may07,h_k  converted g64120aMf to binary release.
01b,30jan07,wap  Update PCI support
01a,07Jul06,tor  created
*/

/**********************************************************************
*
* Galileo g64120a system controller
*
*/

Folder INCLUDE_GT64120A {
	NAME		g64120a
	SYNOPSIS	Galileo GT64120A system controller
	_CHILDREN	FOLDER_DRIVERS
	CHILDREN	INCLUDE_GT64120A_MF \
			INCLUDE_GT64120A_PCI 
        DEFAULTS        INCLUDE_GT64120A_MF \
			INCLUDE_GT64120A_PCI
}

Component INCLUDE_GT64120A_MF {
	NAME		g64120a driver
	SYNOPSIS	Galileo GT64120A system controller driver
	MODULES		g64120aMf.o
	SOURCE		$(WIND_BASE)/target/src/hwif/mf
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS
	INIT_RTN	g64120aMfRegister();
	PROTOTYPE	void g64120aMfRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}

Component INCLUDE_GT64120A_PCI {
	NAME		g64120a PCI bus controller
	SYNOPSIS	Galileo GT64120A PCI bus controller driver
	SOURCE		$(WIND_BASE)/target/src/busCtlr
	MODULES		g64120aPci.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS
	INIT_RTN	g64120aPciRegister();
	PROTOTYPE	void g64120aPciRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}






