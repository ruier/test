/* 40geiVxbEnd.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
                                                                                
/*
modification history
--------------------
01d,04nov09,h_k  removed INCLUDE_DMA_SYS from REQUIRES attribute for Intel
                 arch.
01c,04may08,dlk  Added INCLUDE_GEI825XX_VXB_END2 and
                 SELECT_GEI825XX_VXB_END.
01b,07mar08,dlk  Exclude INCLUDE_GEI825XX_VXB_END2.
01a,01mar07,wap  written

*/

Selection	SELECT_GEI825XX_VXB_END {
	NAME		Select END or END2 version of VxBus Intel PRO/1000 driver
	SYNOPSIS	Select END or END2 version of VxBus Intel PRO/1000 driver
	COUNT		1-1
	_CHILDREN	FOLDER_DRIVERS
	CHILDREN	INCLUDE_GEI825XX_VXB_END \
			INCLUDE_GEI825XX_VXB_END2
	DEFAULTS	INCLUDE_GEI825XX_VXB_END
}

Component	INCLUDE_GEI825XX_VXB_END {
	NAME		Intel PRO/1000 VxBus Enhanced Network Driver
	SYNOPSIS	Intel PRO/1000 VxBus Enhanced Network Driver
	HDR_FILES	../src/hwif/h/end/gei825xxVxbEnd.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	geiRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_END
	INIT_AFTER	INCLUDE_PCI_BUS
}

Component	INCLUDE_GEI825XX_VXB_END2 {
	NAME		Intel PRO/1000 VxBus END2 driver
	SYNOPSIS	Intel PRO/1000 VxBus END2 driver
	HDR_FILES	../src/hwif/h/end2/vxbGei825xxEnd2.h
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	geiEnd2Register();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_PCI_BUS \
			INCLUDE_GENERICPHY \
			INCLUDE_PARAM_SYS \
			INCLUDE_END2
	INIT_AFTER	INCLUDE_PCI_BUS
}

