/*
modification history
--------------------
01a,15mar2012,hdz  written
*/

				
Component INCLUDE_PROPHET_DUMMY_MDIO 
{
	NAME		DUMMY MDIO driver
	SYNOPSIS	DUMMY MDIO driver
	_CHILDREN	FOLDER_PROPHET_DEV_DRIVER
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE void vxbProphetDummyMdioRegister(void);
	INIT_RTN	vxbProphetDummyMdioRegister();
	REQUIRES	INCLUDE_MII_BUS
	INIT_AFTER	INCLUDE_MII_BUS
}


