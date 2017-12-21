/* 40mii.cdf - Component configuration file */
                                                                                
/*
 * Copyright (c) 2005-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */         
                                                                       

/*
modification history
--------------------
01q,15aug12,wap  Add T4240 MEMAC MDIO support
01r,23apr12,d_l  Add HDR_FILES for vxbQcomAr8021Phy
01q,11apr12,x_z  Add Qualcomm AR8021 PHY driver.
01p,26oct10,wap  Add I2C MDIO
01o,11jun10,wap  Add TGEC MDIO driver
01n,29jul09,wap  Add dTSEC MDIO
01n,22may09,wap  Add RMI MDIO driver
01m,21jul08,z_l  Add DUMMY_MDIO driver
01l,17jun08,dlk  Make INCLUDE_MII_BUS require INCLUDE_END_COMMON instead of
                 INCLUDE_END.
01k,21may08,jmt  Add requirement for INCLUDE_END component to INCLUDE_MII_BUS
01k,13may08,wap  Add DTE 10Gbps PHY driver
01j,18mar08,wap  Add uccMdio and ip175cPhy drivers
01i,03dec07,mdo  CQ: WIND00110287 - Fix supported driver in workbench
01h,16jul07,wap  Add generic TBI driver
01g,03jul07,wap  Add TSEC_MDIO driver
01f,09mar07,wap  Add GEI TBI pseudo driver
01e,04jan07,wap  Correct component name for DM9161PHY driver, add Marvell
                 88E1113 driver
01d,21aug06,wap  Add MDIO driver
01c,02jun06,wap  Add RTL8201 driver
01b,08feb06,wap  Correct typo in name of Marvell PHY driver header file
01a,19dec05,wap  written
*/

Component INCLUDE_MII_BUS {
	NAME		MII bus controller module
	SYNOPSIS	MII bus controller for ethernet transceivers
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/miiBus.h
	INIT_RTN	miiBusRegister();
	REQUIRES	INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS \
                        INCLUDE_END_COMMON
	INIT_AFTER	INCLUDE_PLB_BUS
}

Component INCLUDE_GENERICPHY {
	NAME		Generic PHY driver
	SYNOPSIS	Generic 10/100/1000 ethernet copper PHY driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/genericPhy.h
	INIT_RTN	genPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_GENERICTBIPHY {
	NAME		Generic TBI driver
	SYNOPSIS	Generic 1000Mbps TBI fiber ethernet PHY driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/genericTbiPhy.h
	INIT_RTN	genTbiRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_BCM54XXPHY {
	NAME		Broadcom 54xx driver
	SYNOPSIS	Broadcom 54xx 10/100/1000 ethernet over copper PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/bcm54xxPhy.h
	INIT_RTN	brgPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_BCM52XXPHY {
	NAME		Broadcom 52xx driver
	SYNOPSIS	Broadcom 52xx 10/100 ethernet PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/bcm52xxPhy.h
	INIT_RTN	bmtPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_DM9161PHY {
	NAME		Davicom DM9161 driver
	SYNOPSIS	Davicom DM9161 10/100 ethernet PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/dm9161Phy.h
	INIT_RTN	dmPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_GEITBIPHY {
	NAME		Intel PRO/1000 TBI/SERDES pseudo driver
	SYNOPSIS	Intel PRO/1000 TBI/SERDES pseudo driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/geiTbiPhy.h
	INIT_RTN	geiTbiPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_ICP175CPHY {
	NAME		ICplus IP175C switch controller driver
	SYNOPSIS	ICplus IC175C 5 port switch controller
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/icp175cPhy.h
	INIT_RTN	icpRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_LXT972PHY {
	NAME		Level1/Intel LXT972 driver
	SYNOPSIS	Level1/Intel LXT972 10/100 ethernet PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/lxt972Phy.h
	INIT_RTN	lxtPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_MV88E1X11PHY {
	NAME		Marvell 88E1x11 driver
	SYNOPSIS	Marvell 88E1x11 10/100/1000 ethernet copper PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/mv88E1x11Phy.h
	INIT_RTN	mvPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_MV88E1113PHY {
	NAME		Marvell 88E1113 driver
	SYNOPSIS	Marvell 88E1113 100/1000 ethernet fiber PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/mv88E1113Phy.h
	INIT_RTN	mvfPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_RTL8201PHY {
	NAME		RealTek RTL8201 driver
	SYNOPSIS	RealTek RTL8201 10/100 ethernet PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/rtl8201Phy.h
	INIT_RTN	rtlPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_RTL8169PHY {
	NAME		RealTek RTL8169 driver
	SYNOPSIS	RealTek RTL8169 C+ 10/100 ethernet PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/rtl8169Phy.h
	INIT_RTN	rtgPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_VSC82XXPHY {
	NAME		Vitess/Cicada 82XX driver
	SYNOPSIS	Vitesse/Xicada 10/100/1000 ethernet copper PHY
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vsc82xxPhy.h
	INIT_RTN	vigPhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_QCOM_AR8021PHY {
	NAME		Qualcomm AR8021 GbE PHY driver
	SYNOPSIS	Qualcomm AR8021 GbE PHY driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbQcomAr8021Phy.h
	INIT_RTN	qcomAr8021PhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_MDIO {
	NAME		Generic MDIO bit-bang driver
	SYNOPSIS	Generic MDIO bit-bang driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/mdio.h
	INIT_RTN	mdioRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component DRV_MII_I2C_MDIO {
	NAME		DTSEC I2C driver
	SYNOPSIS	DTSEC I2C driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbI2cMdio.h
	INIT_RTN	imRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component DRV_MII_DTSEC_MDIO {
	NAME		DTSEC MDIO driver
	SYNOPSIS	DTSEC MDIO driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbDtsecMdio.h
	INIT_RTN	dtmRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component DRV_MII_TGEC_MDIO {
	NAME		TGEC MDIO driver
	SYNOPSIS	TGEC MDIO driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbTgecMdio.h
	INIT_RTN	tgmRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component DRV_MII_QORIQMEMAC_MDIO {
	NAME		Freescale QorIQ MEMAC MDIO driver
	SYNOPSIS	Freescale QorIQ MEMAC MDIO driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbQorIQMemacMdio.h
	MODULES		vxbQorIQMemacMdio.o
	INIT_RTN	qmmRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_RMI_MDIO {
	NAME		RMI MDIO driver
	SYNOPSIS	RMI MDIO driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbRmiMdio.h
	INIT_RTN	rmRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_TSEC_MDIO {
	NAME		TSEC MDIO driver
	SYNOPSIS	TSEC MDIO driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/tsecMdio.h
	INIT_RTN	tmRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_UCC_MDIO {
	NAME		UCC MDIO driver
	SYNOPSIS	UCC MDIO driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/uccMdio.h
	INIT_RTN	umRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component DRV_MII_DTEPHY {
	NAME		DTE PMD 10Gbps pseudo driver
	SYNOPSIS	DTE PMD 10Gbps pseudo driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbDtePhy.h
	MODULES		vxbDtePhy.o
	INIT_RTN	dtePhyRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

Component INCLUDE_DUMMY_MDIO {
	NAME		DUMMY MDIO driver
	SYNOPSIS	DUMMY MDIO driver
	_CHILDREN	FOLDER_DRIVERS
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/mii/vxbDummyMdio.h
	INIT_RTN	dummyMdioRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

