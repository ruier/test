/**************************************************************************
 *
 *   Copyright (c) 2009 Curtiss-Wright Controls, Inc. All rights
 *   reserved.  This Source Code is the Property of Curtiss-Wright
 *   Controls, Inc. and can only be used in accordance with Source
 *   Code License Agreement(s) of Curtiss-Wright Controls, Inc. or any
 *   of its subsidiaries.
 *
 **************************************************************************/


 Folder FOLDER_PROPHET_BSP_INIT {
    NAME            BSP PROPHET 835 Initialization
    SYNOPSIS        BSP PROPHET 835 Initialization
    CHILDREN        INCLUDE_BSP_INIT
    _CHILDREN       FOLDER_HARDWARE
    _DEFAULTS       FOLDER_PROPHET_DEV_DRIVER 
    _DEFAULTS       INCLUDE_BSP_INIT   
}

Component INCLUDE_BSP_INIT {
    NAME        Include prophet BSP Initialization routine
    SYNOPSIS    Include prophet BSP's last initialization routine.
    PROTOTYPE   void prophetBspInit(void);
    INIT_RTN    prophetBspInit();
}


Folder FOLDER_PROPHET_DEV_DRIVER {
    NAME            BSP PROPHET 835 Device Driver Selection
    SYNOPSIS        Provides the ability to include devices drivers in the kernel. 
    CHILDREN        INCLUDE_PROPHET_DUMMY_MDIO 
    								INCLUDE_PROPHET_M8640D_MDIO
    								DRV_INTCTRL_GPIO
    								DRV_INTCTRL_FPGA_TIMER
    								DRV_INTCTRL_I2C
    								DRV_M8641_GPIO	
    								DRV_M8641_IIC		
    								VXB_M8641_DMA		
    								DRV_M8641_SYN_UART
    								
    DEFAULTS        INCLUDE_PROPHET_DUMMY_MDIO  
    								INCLUDE_PROPHET_M8640D_MDIO            
    _CHILDREN       FOLDER_HARDWARE
    _DEFAULTS       FOLDER_PROPHET_DEV_DRIVER
}

Component DRV_M8641_SYN_UART {
        NAME           Synchronous Uart VxBus driver
        SYNOPSIS       Synchronous Uart VxBus driver provided by prophet
}

Component	VXB_M8641_DMA {
	NAME		MPC8641 DMA  VxBus Driver
	SYNOPSIS	DMA  VxBus Driver
}

Component DRV_M8641_IIC {
   NAME            I2C Control VxBus driver
   SYNOPSIS        I2C Control VxBus driver provided by prophet
}


Component INCLUDE_PROPHET_DUMMY_MDIO 
{
	NAME		prophet DUMMY MDIO driver
	SYNOPSIS	prophet DUMMY MDIO driver
}

Component INCLUDE_PROPHET_M8640D_MDIO {
	NAME		PROPHET M8640D MDIO driver
	SYNOPSIS	PROPHET M8640D MDIO driver
}



Component	DRV_M8641_GPIO {
	NAME		MPC8641 GPIO Device VxBus Driver
	SYNOPSIS	GPIO Device VxBus Driver
}

Component	DRV_INTCTLR_GPIO {
	NAME		MPC8641 GPIO INTCTRL VxBus Driver
	SYNOPSIS	GPIO INTCTRL VxBus Driver
}

Component	DRV_INTCTRL_FPGA_TIMER {
	NAME		fpga timer INTCTRL VxBus Driver
	SYNOPSIS	FPGA_TIMER INTCTRL VxBus Driver
}

Component	DRV_INTCTRL_I2C {
	NAME		I2C INTCTRL VxBus Driver
	SYNOPSIS	I2C INTCTRL VxBus Driver
}