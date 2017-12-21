/* 40pcConsole.cdf - Component configuration file */
                                                                                
/* Copyright (c) 2007, 2011, 2012 Wind River Systems, Inc. */
                                                                                
/*
modification history
--------------------
01f,05jun12,g_x  remove DRV_KBD_NONE and hard dependence of INCLUDE_PC_CONSOLE 
                 on SELECT_PC_CONSOLE_KBD (WIND00347234)
01e,21nov11,wyt  add a null driver as one of the children of 
                 SELECT_PC_CONSOLE_KBD. 
01d,11sep07,pmr  add prototypes for registration routines.
01c,17aug07,pmr  fix selection
01b,08mar07,pmr  add PC Console component override and kbd/vga selections
01a,02mar07,pmr  written
*/

/* initially defined in 00bsp.cdf */
Component INCLUDE_PC_CONSOLE {
	HDR_FILES       ../src/hwif/h/console/pcConsole.h \
			stdio.h \
			sysLib.h
	REQUIRES	SELECT_PC_CONSOLE_VGA
}

Component DRV_KBD_I8042 {
	NAME		Intel 8042 keyboard driver
	SYNOPSIS	driver for commonly found keyboard controllers
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/console/vxbI8042Kbd.h
	INIT_RTN	i8042vxbRegister();
	PROTOTYPE	void i8042vxbRegister (void);
	REQUIRES	INCLUDE_VXBUS INCLUDE_PC_CONSOLE
	INCLUDE_WHEN    INCLUDE_PC_CONSOLE
	INIT_AFTER	INCLUDE_PLB_BUS
}

Component DRV_VGA_M6845 {
	NAME		Motorola 6845 VGA driver
	SYNOPSIS	driver for commonly found VGA adapters
	_INIT_ORDER	hardWareInterFaceBusInit
	HDR_FILES	../src/hwif/h/console/vxbM6845Vga.h
	INIT_RTN	m6845vxbRegister();
	PROTOTYPE	void m6845vxbRegister (void);
	REQUIRES	INCLUDE_VXBUS INCLUDE_PC_CONSOLE
	INIT_AFTER	INCLUDE_PLB_BUS
}

Selection SELECT_PC_CONSOLE_KBD {
        NAME            PC Console keyboard driver
        SYNOPSIS        Keyboard drivers compatible with PC Console
        COUNT           1-1
	_CHILDREN	FOLDER_DRIVERS
        CHILDREN        DRV_KBD_I8042
        DEFAULTS        DRV_KBD_I8042
}

Selection SELECT_PC_CONSOLE_VGA {
        NAME            PC Console video driver
        SYNOPSIS        Video drivers compatible with PC Console
        COUNT           1-
	_CHILDREN	FOLDER_DRIVERS
        CHILDREN        DRV_VGA_M6845
        DEFAULTS        DRV_VGA_M6845
}
