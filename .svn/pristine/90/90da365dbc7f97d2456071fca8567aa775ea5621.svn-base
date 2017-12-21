/* 40vxbIPiix4.cdf - iPIIX4 pci device driver */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01a,05Jul06,pdg  created
*/

Component	INCLUDE_IPIIX4_MF {
	NAME		iPIIX4 PCI driver
	SYNOPSIS	iPIIX4 multifunction PCI driver
	_CHILDREN	FOLDER_DRIVERS
	MODULES		vxbIPiix4Mf.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS
	INIT_RTN	vxbIPiix4MfRegister();
	PROTOTYPE	void vxbIPiix4MfRegister (void);
        INIT_AFTER      INCLUDE_PCI_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}