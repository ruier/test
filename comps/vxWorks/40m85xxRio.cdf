/* 40m85xxRio.cdf - m85xx RapidIO Component configuration file */

/* Copyright (c) 2006-2007 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,07aug07,dtr  Driver build out of BSP.
01c,23aug06,pmr  use CCSR driver rather than LAWBAR
01b,16feb06,dtr  SPR#117948 - Move initialization of RIO bus to before CPU.
01a,05jan06,dtr  created
*/

Component	INCLUDE_M85XX_RAPIDIO {
	NAME		m85xx RapidIO bus controller device driver
	SYNOPSIS	m85xx RapidIO bus controller device driver
	_CHILDREN	FOLDER_DRIVERS
	PROTOTYPE	void m85xxRioRegister (void);
	MODULES		m85xxRio.o
	INIT_RTN	m85xxRioRegister();
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_RAPIDIO_BUS		\
			INCLUDE_M85XX_CPU	\
			DRV_RESOURCE_M85XXCCSR
	_INIT_ORDER     hardWareInterFaceBusInit
	INIT_BEFORE     INCLUDE_M85XX_CPU
}

Component	INCLUDE_M85XX_RIO_SM_CFG {
	NAME		m85xx rio shared memory config
	SYNOPSIS	m85xx rio shared memory config
	_CHILDREN	FOLDER_DRIVERS
	REQUIRES	INCLUDE_PLB_BUS \
			INCLUDE_RAPIDIO_BUS		\
			INCLUDE_M85XX_CPU \
			INCLUDE_VXBUS_SM_SUPPORT	
	INIT_AFTER	INCLUDE_PLB_BUS
}
