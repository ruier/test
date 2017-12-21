/* 40vxbM8641Dma.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007-2008 Wind River Systems, Inc. */
                                                                                
/*

*/

Component	VXB_M8641_DMA {
	NAME		MPC8641 DMA  VxBus Driver
	SYNOPSIS	DMA  VxBus Driver
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE   void m8641DmaDrvRegister(void);
	INIT_RTN	m8641DmaDrvRegister();
	_CHILDREN   FOLDER_PROPHET_DEV_DRIVER
	REQUIRES    INCLUDE_VXBUS \
              INCLUDE_PLB_BUS  
	INIT_AFTER	INCLUDE_PLB_BUS
}
