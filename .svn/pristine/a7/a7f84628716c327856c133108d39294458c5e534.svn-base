/* 40vxbTei82598End.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2009-2010 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01d,04aug10,dlk  Fix ambiguous parentage of the driver components.
01c,03jun10,dlk  Added DRV_VXBEND2_TEI8259X and SELECT_TEI8259X_VXB_END
01b,15jul10,wap  Make sure the DTSEC driver shows up in the drivers folder
01b,15jul10,wap  Make sure the 8259x driver shows up in the drivers folder
01a,26feb09,wap  written

*/

Selection	SELECT_TEI8259X_VXB_END {
	NAME		Select END or END2 version of VxBus Intel 10GbE driver
	SYNOPSIS	Select original or IPNET-Native version of \
                        VxBus Intel 10GbE driver
	COUNT		1-1
	_CHILDREN	FOLDER_DRIVERS
	CHILDREN	DRV_VXBEND_TEI82598 \
                        DRV_VXBEND2_TEI8259X
	DEFAULTS	DRV_VXBEND_TEI82598
}

Component	DRV_VXBEND_TEI82598 {
	NAME		Intel 10GbE VxBus Enhanced Network Driver
	SYNOPSIS	Intel 10GbE VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/vxbTei82598End.h
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	teiRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PCI_BUS
}

Component	DRV_VXBEND2_TEI8259X {
	NAME		Intel 10GbE VxBus IPNET-Native Network Driver
	SYNOPSIS	Intel 10GbE VxBus IPNET-Native Network Driver
	HDR_FILES	../src/hwif/h/end2/vxbTei8259xEnd2.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	teiEnd2Register();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS \
                        INCLUDE_END2
	INIT_AFTER	INCLUDE_PCI_BUS
}
