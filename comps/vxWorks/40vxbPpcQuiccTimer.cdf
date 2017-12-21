/* 40vxbPpcQuiccTimer.cdf - Component configuration file */

/*
 * Copyright (c) 2007, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,10jun13,y_y  add some comments.
01c,17jul07,pee  WIND00098714. Missing closing }
01b,13jun07,h_k  changed to library build.
01a,14may07,sup  written
*/

/**********************************************************************
*
* Generic PowerPC Quicc Timer
*
*/

Component DRV_TIMER_QUICC_PPC {
        NAME            PPC Quicc 32-bit Timer(GTM)
        SYNOPSIS        PPC Quicc 32-bit Timer(GTM) for vxBus
        _CHILDREN       FOLDER_DRIVERS
	MODULES		vxbPpcQuiccTimer.o
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_TIMER_SYS
        INIT_RTN        quiccTimerDrvRegister();
	PROTOTYPE	void quiccTimerDrvRegister (void);
        _INIT_ORDER     hardWareInterFaceBusInit
        _CHILDREN       FOLDER_DRIVERS
        _DEFAULTS       += FOLDER_DRIVERS
}
