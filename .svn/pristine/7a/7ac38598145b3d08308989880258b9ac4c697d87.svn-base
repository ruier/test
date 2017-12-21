/* 40ixp4xxPci.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2006-2007 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01e,15may08,dlk  Added END2 support (INCLUDE_VXB_DMA_END2_IXP4XX_PCI).
01d,31may07,h_k  changed to library build.
01c,16feb07,dtr  Change component name.
01b,23jul06,wap  Fix project builds
01a,22jul06,wap  written

*/

Component	DRV_PCIBUS_IXP4XX {
	NAME		IXP4xx PCI bus controller Driver
	SYNOPSIS	IXP4xx PCI bus controller Driver
	MODULES		ixp4xxPci.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	ixPciRegister();
	PROTOTYPE	void ixPciRegister (void);
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_PARAM_SYS \
			INCLUDE_DMA_SYS
	INIT_AFTER	INCLUDE_PCI_BUS
}

Component	INCLUDE_VXB_DMA_END2_IXP4XX_PCI {
	NAME		END2 DMA buffer support for IXP4xx PCI
	SYNOPSIS	END2 DMA buffer support for IXP4xx PCI
	MODULES		vxbDmaEnd2BufIxp4xxPci.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	vxbDmaEnd2BufIxp4xxPciInit();
	PROTOTYPE	void vxbDmaEnd2BufIxp4xxPciInit (void);
	REQUIRES	DRV_PCIBUS_IXP4XX
	INIT_AFTER	INCLUDE_PCI_BUS
	INCLUDE_WHEN	DRV_PCIBUS_IXP4XX INCLUDE_END2
}

