/* 10vxbus.cdf - Component configuration file */

/*
 * Copyright (c) 2005-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03j,10jun13,y_y  remove the wrapper of the INCLUDE_VXB_TIMESTAMP64. (WIND00392055)
03i,31may13,y_y  update INCLUDE_VXB_TIMESTAMP64 only for LP64. (WIND00414497)
03h,25apr13,rbc  WIND00414401 - Changes to support Unified BSP.
03g,07apr13,y_c  Add INIT_RTN to component INCLUDE_DMA_SYS. (WIND00411594)
03f,11mar13,y_y  Removed EXCLUDES macro INCLUDE_TIMER_STUB in
                 INCLUDE_TIMER_SYS Component. (WIND00407544)
03e,20sep12,l_z  Excludes INCLUDE_TIMER_STUB for INCLUDE_TIMER_SYS. 
                 (WIND00377568)
                 Add INCLUDE_TIMER_EARLY_VXB_DELAY. (WIND00375640)
03d,18sep12,l_z  Excludes INCLUDE_PCI_CFGSHOW from INCLUDE_PCI_BUS.
                 (WIND00108357)
03c,09jun12,y_y  Add INCLUDE_SPI_BUS in FOLDER_BUS.
03b,11apr12,y_y  Add CONFIGLETTES to INCLUDE_I2C_BUS component.
03a,09apr12,y_y  Add INCLUDE_I2C_BUS in FOLDER_BUS.
02z,11feb12,l_z  Add I2C bus and RTC support
02y,04oct12,lll  Add DRV_VGA_M6845 to InitGroup hardWareInterFaceBusInit
02x,09feb12,cwl  increase HWMEM_POOL_SIZE to 8KB by default. (WIND00310499)
02w,19aug11,dee  WIND00278138 - 64-bit timestamp INCLUDE_VXB_TIMESTAMP64
02v,06jan11,cfm  Add INCLUDE_SM_AND_MIPC and INCLUDE_SM_AND_DSHM to the
                 definition of component INCLUDE_VXBUS_SM_SUPPORT as required.
                 (WIND00236270)
02u,07dec10,ghs  Remove USB host controller initialization functions
                 (WIND00245236)
02t,28oct10,h_k  removed unnecessary INIT_AFTER from
                 INCLUDE_VXBUS_DEVICE_CONNECT component definition. (CQ:238851)
02s,16sep10,j_l  Include DMA32 only when needed (Fix for WIND00230745)
02r,08sep10,ghs  added USB MHCI support
02q,13apr10,cwl  add dependency INCLUDE_PARAM_SYS to INCLUDE_PCI_BUS.
                 (WIND00204365)
02p,17nov09,m_y	 Add synopsys USB host controller support(WIND00123426).
02o,12nov09,h_k  removed hwMemPool for _WRS_CONFIG_DELAYED_HW_INIT option.
02n,04nov09,h_k  added INCLUDE_VXBUS_DISK. (WIND00189354)
02m,21sep09,cww  Moved hardWareInterFaceInit to sysHwInit1 for 64-bit
02l,22sep09,h_k  added dependency for INCLUDE_DMA_SYS and INCLUDE_VXBUS.
02k,26aug09,cww  Added hardWareInterFaceInit to usrRoot for 64-bit
02j,03may09,x_s  added support to minimal kernel.
02i,10feb09,h_k  added a stub file for INCLUDE_SIO_UTILS.
02h,22oct08,h_k  added INCLUDE_TIMER_SYS_SHOW.
02g,06oct08,h_k  moved INCLUDE_VXBUS_SHOW and INCLUDE_PLB_BUS to binary link.
02f,27aug08,h_k  fixed timer init orders.
02e,15aug08,h_k  converted vxbArchAccess.c to library build.
02d,07aug08,h_k  added timer assignment feature. (CQ:121884)
02c,09jul08,tor  convert to src build: vxbPci, vxbPciAccess
02b,01jul08,tor  Convert to src build: hwMemLib, sioChanUtil,
		 vxbVerChk, vxbSmSupport
02a,26jun08,pmr  support library build of timer modules
01z,06may08,tor  update vxBus version
01y,23apr08,h_k  converted to source build for vxbDmaBufLib, vxbDmaLib,
                 vxbParamSys and vxbDevTable.
01x,18apr08,tor  Fix hwMem alloc
01w,14apr08,tor  allow BSP hwMemPool init
01v,28nov07,mdo  CQ:WIND00110062 - fix typo
01u,19sep07,ami  CQ:WIND00102614 Fix (USB Related)
01t,14sep07,pmr  WIND00104575 - fix timestamp and aux clk
01s,31aug07,tor  timer driver configuration
01r,22aug07,h_k  fixed INCLUDE_BUS to INCLUDE_VXBUS.
                 fixed missing folder attributes.
01q,10aug07,kab  CQ:WIND00101159 Fix typo
01p,26jul07,tor  Add R/W semaphor dependency to VxBus
01p,24jul07,jrp  Added INCLUDE_VXBUS_DEV_INIT and INCLUDE_VXBUS_DEVICE_CONNECT
01o,14jun07,h_k  removed pciSlot.c.
01n,21feb07,dtr  Update HWIF folder with PCI bus optional includes.
01m,06feb07,dtr  Add new PCI component info for vxBus.
01m,10jan07,pdg  renamed vxbDelay.c to vxbDelayLib.c
01o,15feb07,pdg  updated vxbTimerLib.h path
01m,10jan07,pdg  renamed vxbDelay.c to vxbDelayLib.c
01l,25aug06,pdg  added support for timer stub
01k,28jul06,pdg  updated dma support
01j,06jun06,pdg  updated timer support
01i,20apr06,pdg  updated dependency of INCLUDE_SIO_UTILS
01h,11jan06,dtr  Add vxbDevTable.
01g,11jan06,mdo  Change INCLUDE_NS16550_SIO to DRV_SIO_NS16550
01f,09dec05,mdo  Add vxBus DMA buffer subsystem
01e,05dec05,mdo  Add parameter subsystem
01d,05oct05,mdo  Remove unsupported buses
01c,15aug05,mdo  Add new access method configlettes
01b,08aug05,mdo  Change WINDBUS to VXBUS
01a,06jul05,tor  written

*/

InitGroup               hardWareInterFaceBusInit {
        INIT_RTN        hardWareInterFaceBusInit ();
        SYNOPSIS        bus-subsystem initialization
        INIT_ORDER	INCLUDE_VXBUS_INIT    \
                        INCLUDE_PLB_BUS         \
                        INCLUDE_PCI_BUS         \
                        DRV_VGA_M6845		  \
                        INCLUDE_I2C_BUS        \
                        INCLUDE_SPI_BUS
}

InitGroup               hardWareInterFaceInit {
        PROTOTYPE       void hardWareInterFaceInit (void)
        SYNOPSIS        Hardware Interface Initialization
        INIT_ORDER      INIT_HWMEMPOOL_GLOBAL \
                        INCLUDE_HWMEM_ALLOC \
                        INCLUDE_TIMER_SYS \
                        INCLUDE_VXBUS \
                        INCLUDE_TIMER_EARLY_VXB_DELAY \
                        INCLUDE_VXBUS_ACTIVATE \
                        INCLUDE_VXB_SYS_CLK \
                        INCLUDE_VXB_AUX_CLK \
                        INCLUDE_VXB_RTC_CLK \
                        INCLUDE_VXB_TIMESTAMP \
                        INCLUDE_TIMER_SYS_DELAY \
                        INCLUDE_TIMER_STUB \
                        INCLUDE_DMA_SYS
}

InitGroup				vxbDevInit {
		PROTOTYPE		STATUS vxbDevInit (void)
		SYNOPSIS		Device initialization post kernel
		INIT_ORDER		INCLUDE_USB_INIT  \
                        INCLUDE_VXBUS_DEV_INIT
}

InitGroup				vxbDevConnect {
		PROTOTYPE		STATUS vxbDevConnect (void)
		SYNOPSIS		vxBus Device Connect post kernel
		INIT_ORDER		INCLUDE_VXBUS_DEVICE_CONNECT
}
		
Folder          FOLDER_HWIF {
        NAME            Hardware Interface Modules
        SYNOPSIS        Hardware Interface Modules
        _CHILDREN       FOLDER_HARDWARE
        CHILDREN        INCLUDE_HWMEM_ALLOC \
			SELECT_HWMEM_INIT \
                        INCLUDE_SIO_UTILS \
			INCLUDE_PARAM_SYS \
                        INCLUDE_DMA_SYS \
			INCLUDE_TIMER_STUB \
			INCLUDE_VXB_SYS_CLK \
			INCLUDE_VXB_AUX_CLK \
			INCLUDE_VXB_RTC_CLK \
			INCLUDE_VXB_TIMESTAMP \
			INCLUDE_VXB_TIMESTAMP64 \
                        INCLUDE_TIMER_SYS \
			INCLUDE_TIMER_SYS_SHOW \
			VXBUS_TABLE_CONFIG
}

#ifndef	_WRS_CONFIG_DELAYED_HW_INIT
Selection       SELECT_HWMEM_INIT {
        SYNOPSIS        Pre-Kernel Memory Allocation Initialization
        NAME            Pre-Kernel Memory Allocation Initialization
        COUNT           1-1
        CHILDREN        INIT_HWMEMPOOL_GLOBAL \
                        INIT_HWMEMPOOL_IN_BSP
        DEFAULTS        INIT_HWMEMPOOL_GLOBAL
}

Component       INIT_HWMEMPOOL_GLOBAL {
        NAME            Init pre-kernel memory allocation globally
        SYNOPSIS        Init pre-kernel memory allocation globally
        INIT_RTN        hwMemLibInit(); hwMemPoolCreate(&hwMemPool[0], HWMEM_POOL_SIZE);
        CFG_PARAMS      HWMEM_POOL_SIZE
	CONFIGLETTES	hwif/hwMemAlloc.uc
}

Component       INIT_HWMEMPOOL_IN_BSP {
        NAME            Init pre-kernel memory allocation in BSP
        SYNOPSIS        Init pre-kernel memory allocation in BSP
}

Component       INCLUDE_HWMEM_ALLOC {
        NAME            Pre-Kernel Memory Allocation
        SYNOPSIS        Pre-Kernel Memory Allocation
	MODULES		hwMemLib.o
        SOURCE	    	$(WIND_BASE)/target/src/hwif/util
	CONFIGLETTES	hwif/hwMemAlloc.uc
        CFG_PARAMS      HWMEM_POOL_SIZE
        REQUIRES        INCLUDE_VXBUS \
                        SELECT_HWMEM_INIT
}
#endif	/* !_WRS_CONFIG_DELAYED_HW_INIT */

Component       INCLUDE_SIO_UTILS {
        NAME            Sio Channel Utilities
        SYNOPSIS        Sio Channel Utilities
	MODULES		sioChanUtil.o
        SOURCE		$(WIND_BASE)/target/src/hwif/util
	CONFIGLETTES	usrVxbSioChanUtil.c
        REQUIRES	INCLUDE_VXBUS
}

Parameter       HWMEM_POOL_SIZE {
        NAME            Pre-Kernel Memory Allocation Pool Size
        SYNOPSIS        Pre-Kernel Memory Allocation Pool Size
        TYPE            uint
#ifndef _WRS_CONFIG_VXBUS_BASIC
        DEFAULT         50000
#else
        DEFAULT         8000
#endif /* !_WRS_CONFIG_VXBUS_BASIC */
}

Folder          FOLDER_BUS {
        NAME            Bus Support
        SYNOPSIS        Bus Subsystem
        _CHILDREN       FOLDER_HARDWARE
        CHILDREN        INCLUDE_VXBUS		\
			INCLUDE_VXBUS_SHOW	\
                        INCLUDE_PLB_BUS         \
                        INCLUDE_PCI_BUS		\
                        INCLUDE_I2C_BUS     \
                        INCLUDE_SPI_BUS         \
			INCLUDE_PCI_BUS_AUTOCONF \
			INCLUDE_PCI_OLD_CONFIG_ROUTINES \
			INCLUDE_PCI_BUS_SHOW
        DEFAULTS        INCLUDE_VXBUS             \
                        INCLUDE_PLB_BUS
}

Folder          FOLDER_DRIVERS {
        NAME            Device Drivers
        SYNOPSIS        Device Drivers
        _CHILDREN       FOLDER_HARDWARE
}

Component       INCLUDE_VXBUS {
        NAME            vxBus subsystem
        SYNOPSIS        vxBus subsystem
        CONFIGLETTES    hwif/vxbVerChk.uc
	SOURCE		{$(TGT_DIR)/src/hwif/vxbus {vxbArchAccess.o vxBus.o}} \
			{$(TGT_DIR)/src/hwif/util vxbDelayLib.o}
	LINK_SYMS       vxbRegMap vxbDelay vxbMsDelay vxbUsDelay
        HDR_FILES       vxBusLib.h \
			vxbTimerLib.h
        INIT_RTN        hardWareInterFaceBusInit();
#ifdef	_WRS_CONFIG_LP64
        REQUIRES        INCLUDE_VXBUS_INIT	\
                        INCLUDE_PLB_BUS         \
                        INCLUDE_VXBUS_ACTIVATE	\
			INCLUDE_VXBUS_DEV_INIT	\
                        INCLUDE_VXBUS_DEVICE_CONNECT \
			INCLUDE_ADR_SPACE_LIB
#else	/* _WRS_CONFIG_LP64 */
        REQUIRES        INCLUDE_VXBUS_INIT	\
                        INCLUDE_PLB_BUS         \
                        INCLUDE_HWMEM_ALLOC     \
                        INCLUDE_VXBUS_ACTIVATE	\
			INCLUDE_VXBUS_DEV_INIT	\
                        INCLUDE_VXBUS_DEVICE_CONNECT
#endif	/* _WRS_CONFIG_LP64 */
}

Component        INCLUDE_VXBUS_INIT {
        NAME            vxBus Library Initialization
        SYNOPSIS        vxBus Library Initialization
        INIT_RTN        vxbLibInit();
        REQUIRES        INCLUDE_VXBUS
	_CHILDREN       FOLDER_NOT_VISIBLE
}

Component        INCLUDE_VXBUS_ACTIVATE {
        NAME            vxBus Library Activation
        SYNOPSIS        vxBus Library Activation
        INIT_RTN        vxbInit();
        REQUIRES        INCLUDE_VXBUS
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component       INCLUDE_VXBUS_SHOW {
        NAME            vxBus subsystem show routines
        SYNOPSIS        vxBus subsystem show routines
	MODULES		vxbShow.o
        REQUIRES        INCLUDE_VXBUS
	LINK_SYMS	vxBusShow
}

Component       INCLUDE_PLB_BUS {
        NAME            Processor Local Bus
        SYNOPSIS        Processor Local Bus
	MODULES		vxbPlb.o vxbPlbAccess.o
        HDR_FILES       hwif/vxbus/vxBus.h hwif/vxbus/vxbPlbLib.h
        INIT_RTN        plbRegister();
	REQUIRES	INCLUDE_VXBUS
}

Component       INCLUDE_PCI_BUS {
        NAME            Peripheral Component Interconnect Bus
        SYNOPSIS        Peripheral Component Interconnect Bus
        CONFIGLETTES    hwif/vxbPci.bc
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_PARAM_SYS
        HDR_FILES       hwif/vxbus/vxBus.h hwif/vxbus/vxbPciLib.h
        INIT_RTN        pciRegister();
        EXCLUDES        INCLUDE_PCI_CFGSHOW
}

Component       INCLUDE_PCI_BUS_SHOW {
        NAME            PCI Bus Show Routines
        SYNOPSIS        PCI Bus Show Routines
        REQUIRES        INCLUDE_PCI_BUS
}

Component       INCLUDE_PCI_BUS_AUTOCONF {
        NAME            PCI Bus Auto Configuration Routines
        SYNOPSIS        PCI Bus Auto Configuration Routines
        REQUIRES        INCLUDE_PCI_BUS
}

Component       INCLUDE_PCI_OLD_CONFIG_ROUTINES {
        NAME            PCI Bus legacy Auto Configuration Routines
        SYNOPSIS        PCI Bus legacy Auto Configuration Routines
        REQUIRES        INCLUDE_PCI_BUS
}

Component       INCLUDE_I2C_BUS {
        NAME            Inter-Integrated Circuit Bus
        SYNOPSIS        Inter-Integrated Circuit Bus
        CONFIGLETTES    hwif/vxbI2c.bc
        MODULES         vxbI2cLib.o
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_PARAM_SYS
        HDR_FILES       hwif/vxbus/vxBus.h hwif/vxbus/vxbI2cLib.h
        INIT_RTN        i2cRegister();
}

Component       INCLUDE_SPI_BUS {
        NAME            Serial Peripheral Interface Bus
        SYNOPSIS        Serial Peripheral Interface Bus
        CONFIGLETTES    hwif/vxbSpi.bc
        MODULES         vxbSpiLib.o
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_PARAM_SYS
        HDR_FILES       hwif/vxbus/vxBus.h hwif/vxbus/vxbSpiLib.h
        INIT_RTN        spiRegister();
}

Component	INCLUDE_PARAM_SYS {
	NAME		vxBus Driver Parameter System
	SYNOPSIS	vxBus Driver Parameter System
	MODULES		vxbParamSys.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	REQUIRES	INCLUDE_VXBUS
}

Component	INCLUDE_DMA_SYS {
	NAME		vxBus Driver DMA System
	SYNOPSIS	vxBus Driver DMA System
	MODULES		vxbDmaLib.o vxbDmaBufLib.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	PROTOTYPE	void vxbDmaLibInit(void);
	INIT_RTN	vxbDmaLibInit();
	REQUIRES	INCLUDE_VXBUS
}

Component	INCLUDE_TIMER_STUB {
	NAME		vxBus Timer Stub Support
	SYNOPSIS	vxBus Timer Stub Support for delay functions
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	MODULES		vxbTimerStub.o
	PROTOTYPE	void vxbTimerStubInit(void);
	INIT_RTN	vxbTimerStubInit();
	REQUIRES	INCLUDE_VXBUS
	HDR_FILES	vxbTimerLib.h
}

Component   INCLUDE_TIMER_EARLY_VXB_DELAY {
	NAME		VxBus Timer early delay Support
	SYNOPSIS	VxBus Timer early delay for delay functions, the sysUsDelay \
used by this component is implemented in BSP, the timer unit used in sysUsDelay \
shouldn't be initialized by VxBus, unless the BSP developer is sure that \
it's free-running that cannot be stopped and keeps working fine after the \
initialization done in VxBus first phase. After the VxBus first phase, the \
VxBus delay routine will switch to use a actual timer, which is provided by \
VxBus timer abstraction layer
	MODULES		vxbTimerEarlyDelay.o
	PROTOTYPE	void vxbTimerEarlyDelayInit (void);
	INIT_RTN	vxbTimerEarlyDelayInit ();
	REQUIRES	INCLUDE_VXBUS
	HDR_FILES	vxbTimerLib.h
}

Component	INCLUDE_VXB_SYS_CLK {
	NAME		vxBus Sys Clk Support
	SYNOPSIS	vxBus Sys Clk Support
        MODULES		vxbSysClkLib.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	PROTOTYPE	STATUS vxbSysClkLibInit (void);
	INIT_RTN	vxbSysClkLibInit();
	INCLUDE_WHEN	INCLUDE_SYSCLK_INIT INCLUDE_TIMER_SYS
	REQUIRES	INCLUDE_SYSCLK_INIT INCLUDE_TIMER_SYS
}

Component	INCLUDE_VXB_AUX_CLK {
	NAME		vxBus Aux Clk Support
	SYNOPSIS	vxBus Aux Clk Support
        MODULES		vxbAuxClkLib.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	PROTOTYPE	STATUS vxbAuxClkLibInit (void);
	INIT_RTN	vxbAuxClkLibInit();
	INCLUDE_WHEN	INCLUDE_AUX_CLK INCLUDE_TIMER_SYS
	REQUIRES	INCLUDE_AUX_CLK INCLUDE_TIMER_SYS
}

Component	INCLUDE_VXB_TIMESTAMP {
	NAME		VxBus Timestamp Support
	SYNOPSIS	VxBus Timestamp Support
        MODULES		vxbTimestampLib.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	PROTOTYPE	STATUS vxbTimestampLibInit (void);
	INIT_RTN	vxbTimestampLibInit();
	INCLUDE_WHEN	INCLUDE_TIMESTAMP INCLUDE_TIMER_SYS
	REQUIRES	INCLUDE_TIMESTAMP INCLUDE_TIMER_SYS
}

Component   INCLUDE_VXB_RTC_CLK {
	NAME		VxBus RTC Support
	SYNOPSIS	VxBus RTC Support
	MODULES		vxbRtcLib.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	PROTOTYPE	STATUS vxbRtcLibInit (void);
	INIT_RTN	vxbRtcLibInit();
	INCLUDE_WHEN	INCLUDE_TIMER_RTC INCLUDE_TIMER_SYS
	REQUIRES	INCLUDE_TIMER_RTC INCLUDE_TIMER_SYS
}

Component	INCLUDE_VXB_TIMESTAMP64 {
	NAME		VxBus 64-bit resolution Timestamp Support
	SYNOPSIS	VxBus 64-bit resolution Timestamp Support
        MODULES		vxbTimestamp64Lib.o
	INCLUDE_WHEN	INCLUDE_TIMESTAMP64 INCLUDE_VXB_TIMESTAMP
	REQUIRES	INCLUDE_TIMESTAMP64 INCLUDE_VXB_TIMESTAMP
}

Component	INCLUDE_TIMER_SYS {
	NAME		vxBus Timer Support
	SYNOPSIS	vxBus Timer Support
	CONFIGLETTES	usrVxbTimerSys.c
	SOURCE		$(WIND_BASE)/target/src/hwif/util
	MODULES		vxbTimerLib.o
#ifndef _WRS_CONFIG_VXBUS_BASIC
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_TIMER_SYS_DELAY
#else
	REQUIRES	INCLUDE_VXBUS 
#endif /* !_WRS_CONFIG_VXBUS_BASIC */
	HDR_FILES	vxbTimerLib.h
	INIT_RTN	usrVxbTimerSysInit();
	CFG_PARAMS      SYSCLK_TIMER_NAME \
			SYSCLK_TIMER_UNIT \
			SYSCLK_TIMER_NUM \
        		AUXCLK_TIMER_NAME \
                        AUXCLK_TIMER_UNIT \
                        AUXCLK_TIMER_NUM \
        		TIMESTAMP_TIMER_NAME \
                        TIMESTAMP_TIMER_UNIT \
                        TIMESTAMP_TIMER_NUM
}

Component	INCLUDE_TIMER_SYS_DELAY {
	NAME		vxBus Delay Timer Support
	SYNOPSIS	vxBus Delay Timer Support
	REQUIRES	INCLUDE_TIMER_SYS
	PROTOTYPE	STATUS vxbDelayLibInit (void);
	INIT_RTN	vxbDelayLibInit();
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component	INCLUDE_TIMER_SYS_SHOW {
	NAME		vxBus Timer show routines
	SYNOPSIS	vxBus Timer show routines
	REQUIRES	INCLUDE_TIMER_SYS
}

Component	INCLUDE_VXBUS_DISK {
	NAME		VxBus disk drive configuration
	SYNOPSIS	VxBus disk drive configuration
	CONFIGLETTES	usrVxbDisk.c
	REQUIRES	INCLUDE_VXBUS
	PROTOTYPE	void usrVxbDiskDrv (void);
	INIT_RTN	usrVxbDiskDrv ();
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Parameter SYSCLK_TIMER_NAME {
        NAME            System Clock Device Name (NULL is auto-assign)
        TYPE            string
        DEFAULT         NULL
}

Parameter SYSCLK_TIMER_UNIT {
        NAME            System Clock Device Unit
        TYPE            int
        DEFAULT         0
}

Parameter SYSCLK_TIMER_NUM {
        NAME            System Clock Timer Number
        TYPE            int
        DEFAULT         0
}

Parameter AUXCLK_TIMER_NAME {
        NAME            Auxiliary Clock Device Name (NULL is auto-assign)
        TYPE            string
        DEFAULT         NULL
}

Parameter AUXCLK_TIMER_UNIT {
        NAME            Auxiliary Clock Device Unit
        TYPE            int
        DEFAULT         0
}

Parameter AUXCLK_TIMER_NUM {
        NAME            Auxiliary Clock Timer Number
        TYPE            int
        DEFAULT         0
}

Parameter TIMESTAMP_TIMER_NAME {
        NAME            Timestamp Device Name (NULL is auto-assign)
        TYPE            string
        DEFAULT         NULL
}

Parameter TIMESTAMP_TIMER_UNIT {
        NAME            Timestamp Device Unit
        TYPE            int
        DEFAULT         0
}

Parameter TIMESTAMP_TIMER_NUM {
        NAME            Timestamp Timer Number
        TYPE            int
        DEFAULT         0
}

Component	VXBUS_TABLE_CONFIG {
	NAME		VxBus Device Table
	SYNOPSIS	VxBus Table-Based Device Enumeration
	MODULES		vxbDevTable.o
	SOURCE		$(WIND_BASE)/target/src/hwif/util
}

Component    INCLUDE_VXBUS_SM_SUPPORT {
        NAME            vxbus shared memory config
        SYNOPSIS        vxbus shared memory config
        CONFIGLETTES    hwif/vxbSmSupport.uc
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceInit
        INIT_RTN        smEndRegister();
        REQUIRES        INCLUDE_PLB_BUS     \
                        INCLUDE_VXBUS       \
                        INCLUDE_SM_AND_MIPC \
                        INCLUDE_SM_AND_DSHM
        INIT_AFTER      INCLUDE_PLB_BUS
        }

Component INCLUDE_VXBUS_DEV_INIT {
	NAME		vxbus device initialization
	SYNOPSIS	vxbus device initialization
	REQUIRES	INCLUDE_VXBUS\
			INCLUDE_VXBUS_DEVICE_CONNECT
	INIT_RTN	return(vxbDevInitInternal ());
	_CHILDREN	FOLDER_NOT_VISIBLE
}

Component INCLUDE_VXBUS_DEVICE_CONNECT {
	NAME		vxbus device connection
	SYNOPSIS	vxbus device connection
	REQUIRES	INCLUDE_VXBUS\
			INCLUDE_VXBUS_DEV_INIT
	INIT_RTN	return (vxbDevConnectInternal ());
	_CHILDREN	FOLDER_NOT_VISIBLE
}
