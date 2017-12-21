/* 40m85xxCpu.cdf - Remote m85xx processor Component configuration file */

/* Copyright (c) 2006-2007 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,07aug07,dtr  Compile outside BSP.
01a,05jan06,dtr  created
*/

Component	INCLUDE_M85XX_CPU {
	NAME		m85xx CPU 
	SYNOPSIS	Device Driver for m85xx Cpu, communicating over RapidIO bus type
	PROTOTYPE	void m85xxCpuRegister (void);
	MODULES		m85xxCpu.o
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	INIT_RTN	m85xxCpuRegister();
	REQUIRES	INCLUDE_RAPIDIO_BUS
	INIT_AFTER	INCLUDE_PLB_BUS
}

