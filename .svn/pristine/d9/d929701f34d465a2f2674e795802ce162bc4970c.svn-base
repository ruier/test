/* 01bsp.cdf - BSP component description file for MVME3100 BSP */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,07sep05,cak  Initial writing.
*/

/*
DESCRIPTION
This file describes components that are specific to the MVME3100
BSP.

The configuration tool searches for .cdf files in four
places: target/config/comps/vxWorks, arch/<arch>, config/<bsp>, 
and the project directory.  These .cdf files are used to create
and configure the vxWorks "tree" structure you see in Project in
your workspace.  Once this has been created the config.h for the
BSP is parsed and the items in the "tree" are configured as per
config.h.
*/

/* 
 * Signal Processing Engine (SPE) Support 
 *
 * define/undef INCLUDE_SPE 
 */

Component INCLUDE_SPE {
	NAME		signal processing engine support
	SYNOPSIS	Signal Processing Engine Support
	_CHILDREN	FOLDER_BSP_CONFIG
}

/*
 * Flash Boot Image Programming
 *
 * This allows the boot image programming support to be enabled/disabled.
 *
 * define/undef INCLUDE_BOOT_IMAGE_PROGRAM
 */

Component INCLUDE_BOOT_IMAGE_PROGRAM {
	NAME		flash boot image programming support
	SYNOPSIS	Flash Boot Image Programming Support
	_CHILDREN	FOLDER_BSP_CONFIG
}

/* 
 * Error Handling Suite 
 *
 * Allows the error handling suite support to be enabled/disabled.
 *
 * define/undef INCLUDE_ERROR_HANDLING
 */

Component INCLUDE_ERROR_HANDLING {
	NAME		error handling suite
	SYNOPSIS	enables error handling suite
	_CHILDREN	FOLDER_BSP_CONFIG
}

/* 
 * AuxClock Timer/Counter Selection 
 *
 * This allows the user to select which of the mpc8540's 
 * timer/counters to use for the vxWorks Aux Clock.
 *
 * MPC8540_AUXCLK_TIMER = {0, 1, 2, 3}
 */

Parameter MPC8540_AUXCLK_TIMER {
	NAME		mpc8540 aux clock timer
	_CFG_PARAMS	INCLUDE_AUX_CLK
	TYPE		uint
	DEFAULT		0
}

/* 
 * MPC8540 DMA Support 
 *
 * Allows the MPC8540 DMA support to be enabled/disabled.
 *
 * define/undef INCLUDE_MPC8540_DMA 
 */

Component INCLUDE_MPC8540_DMA {
	NAME		mpc8540 dma support
	SYNOPSIS	MPC8540 DMA Support
	_CHILDREN	FOLDER_BSP_CONFIG
}

/*
 * Maxim DS1621 Temp Sensor Support
 *
 * This allows the support for the Maxim DS1621 Temperature sensor
 * to be enabled/disabled.
 *
 * define/undef INCLUDE_DS1621
 */

Component INCLUDE_DS1621 {
	NAME		maxim ds1621 temperature sensor support
	SYNOPSIS	enable support for the maxim ds1621 temp sensor
	_CHILDREN	FOLDER_BSP_CONFIG
}

/* 
 * Maxim DS1375 Real-Time Clock 
 *
 * Allows the real-time clock support to be enabled/disabled.
 *
 * define/undef INCLUDE_RTC
 */

Component INCLUDE_RTC {
	NAME		maxim ds1375 real-time clock
	SYNOPSIS	enables real-time clock of maxim ds1375
	_CHILDREN	FOLDER_BSP_CONFIG
	INCLUDE_WHEN	INCLUDE_ERROR_HANDLING
}

/*
 * MPC8540 Watchdog Timer Support
 *
 * This allows the mpc8540's watchdog timer support to be enabled/disabled.
 *
 * define/undef INCLUDE_WDOG_TIMER 
 */

Component INCLUDE_WDOG_TIMER {
	NAME		mpc8540 watchdog timer support
	SYNOPSIS	enable support for the mpc8540s watchdog timer
	_CHILDREN	FOLDER_BSP_CONFIG
}

/*
 * MPC8540 Timer Support 
 *
 * This allows the support for the MPC8540s timers to be enabled/disabled.
 *
 * define/undef INCLUDE_MPC8540_TIMERS 
 */

Component INCLUDE_MPC8540_TIMERS {
	NAME		mpc8540 timer support
	SYNOPSIS	MPC8540 Timer Support
	_CHILDREN	FOLDER_BSP_CONFIG
	INCLUDE_WHEN	INCLUDE_AUX_CLK
}

/*
 * Tick Timer Support 
 *
 * This allows the support for the tick timers to be enabled/disabled.
 *
 * define/undef INCLUDE_TICK_TIMERS 
 */

Component INCLUDE_TICK_TIMERS {
	NAME		tick timer support
	SYNOPSIS	Tick Timer Support
	_CHILDREN	FOLDER_BSP_CONFIG
}
