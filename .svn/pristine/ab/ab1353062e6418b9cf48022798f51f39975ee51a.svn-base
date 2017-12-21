/* sysGpioWakeupDemux.h - GPIO Wakeup interrupt controller */

/*
 * Copyright (c) 2004 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01a,12feb04,pdr	 written.
*/

#ifndef	__INCsysGpioWakeupDemux
#define	__INCsysGpioWakeupDemux

#ifdef __cplusplus
extern "C" {
#endif


/* defines */

/* Interrupt source ID's */
#define GPIO_WKUP_7			0
#define GPIO_WKUP_6			1
#define GPIO_WKUP_5			2
#define GPIO_WKUP_4			3
#define GPIO_WKUP_3			4
#define GPIO_WKUP_2			5
#define GPIO_WKUP_1			6
#define GPIO_WKUP_0			7



#ifndef _ASMLANGUAGE

#if defined(__STDC__) || defined(__cplusplus)

IMPORT STATUS sysGpioWkupDemuxInit	( void );
IMPORT STATUS sysGpioWkupIntConnect	( int intId, VOIDFUNCPTR func, int arg );
IMPORT STATUS sysGpioWkupIntEnable 	( int intId );
IMPORT STATUS sysGpioWkupIntDisable ( int intId );

#else

IMPORT STATUS sysGpioWkupDemuxInit	( );
IMPORT STATUS sysGpioWkupIntConnect	( );
IMPORT STATUS sysGpioWkupIntEnable 	( );
IMPORT STATUS sysGpioWkupIntDisable ( );

#endif  /* __STDC__ */

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif	/* __INCsysGpioWakeupDemux */
