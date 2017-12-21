/* 40vxbGpioDrv.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007-2008 Wind River Systems, Inc. */
                                                                                
/*

*/

Component	DRV_M8641_GPIO {
	NAME		MPC8641 GPIO Device VxBus Driver
	SYNOPSIS	GPIO Device VxBus Driver
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE   void vxbGpioDrvRegister(void);
	INIT_RTN	vxbGpioDrvRegister();
	_CHILDREN  FOLDER_PROPHET_DEV_DRIVER
	REQUIRES    INCLUDE_VXBUS \
              INCLUDE_PLB_BUS 

	INIT_AFTER	INCLUDE_PLB_BUS
}
