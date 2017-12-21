/* 40vxbGpioIntCtrl.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007-2008 Wind River Systems, Inc. */
                                                                                
/*

*/

Component	DRV_INTCTRL_FPGA_TIMER {
	NAME		fpga timer INTCTRL VxBus Driver
	SYNOPSIS	FPGA_TIMER INTCTRL VxBus Driver
	_INIT_ORDER	hardWareInterFaceBusInit
	PROTOTYPE   void vxbFpgaTimerIntCtlrRegister(void);
	INIT_RTN	vxbFpgaTimerIntCtlrRegister();
	_CHILDREN   FOLDER_PROPHET_DEV_DRIVER
	REQUIRES    INCLUDE_VXBUS \
              INCLUDE_PLB_BUS  \
							INCLUDE_INTCTLR_LIB
							DRV_M8641_GPIO

	INIT_AFTER	INCLUDE_PLB_BUS
}
