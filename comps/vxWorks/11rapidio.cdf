/* 11rapidio.cdf - Component configuration file */

/* Copyright (c) 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,30jun08,pmr  library build
01b,23aug06,pmr  cleanup and header fix
01a,06jan06,dtr  written
*/

Folder		FOLDER_RAPIDIO_BUS {
	NAME		RapidIO Bus Support
	SYNOPSIS	RapidIO Bus Support
	_CHILDREN	FOLDER_BUS
	CHILDREN	INCLUDE_RAPIDIO_BUS \
			SELECT_RAPIDIO_CONFIG
}

Selection	SELECT_RAPIDIO_CONFIG {
	NAME		RapidIO Initialization Algorithm
	COUNT		1-1
	CHILDREN	RAPIDIO_BUS_AUTOCONFIG \
			RAPIDIO_BUS_STATIC_PROBE \
			RAPIDIO_BUS_STATIC_TABLE
	DEFAULTS	RAPIDIO_BUS_STATIC_TABLE
}

Component	RAPIDIO_BUS_AUTOCONFIG {
	NAME		RapidIO AutoConfig
	SYNOPSIS	RapidIO Dynamic Configuration with Device Probe
	REQUIRES	INCLUDE_RAPIDIO_BUS
	CONFIGLETTES	$(WIND_BASE)/target/src/hwif/vxbus/vxbRapidIOCfgAuto.c
}

Component	RAPIDIO_BUS_STATIC_PROBE {
	NAME		RapidIO Device Probe
	SYNOPSIS	RapidIO External/Static Configuration with Device Probe.  RapidIO switches must be configured externally.
	REQUIRES	INCLUDE_RAPIDIO_BUS
	CONFIGLETTES	$(WIND_BASE)/target/src/hwif/vxbus/vxbRapidIOCfgProbe.c
}

Component	RAPIDIO_BUS_STATIC_TABLE {
	NAME		RapidIO Table-Based Enumeration
	SYNOPSIS	RapidIO External/Static Configuration with Table of Devices.  RapidIO switches must be configured externally.
	REQUIRES	VXBUS_TABLE_CONFIG INCLUDE_RAPIDIO_BUS
	MODULES		vxbRapidIOCfgTable.o
	SOURCE		$(WIND_BASE)/target/src/hwif/vxbus
	LINK_SYMS	vxbRapidIOCfg
}

Component	INCLUDE_RAPIDIO_BUS {
	NAME    	RapidIO Bus
	SYNOPSIS	RapidIO Bus Type Support
	MODULES		vxbRapidIO.o
	SOURCE		$(WIND_BASE)/target/src/hwif/vxbus
	REQUIRES	INCLUDE_PLB_BUS SELECT_RAPIDIO_CONFIG
	HDR_FILES       hwif/vxbus/vxBus.h
	PROTOTYPE	STATUS rapidIoRegister(void);
	INIT_RTN	rapidIoRegister();
	_INIT_ORDER	hardWareInterFaceBusInit
}

