/*
modification history
--------------------
01a,15mar2012,hdz  written
*/

				
Component INCLUDE_PROPHET_M8640D_MDIO {
	NAME		PROPHET M8640D MDIO driver
	SYNOPSIS	PROPHET M8640D MDIO driver
	_CHILDREN	FOLDER_PROPHET_DEV_DRIVER
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE       void vxbProphetM8640dMdioRegister(void);
	INIT_RTN	vxbProphetM8640dMdioRegister();		
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}

