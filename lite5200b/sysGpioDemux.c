/* sysGpioWakeupDemux.c - GPIO Wakeup interrupt demultiplexer */

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
01a,12feb04,pdr  written.
*/

/*
DESCRIPTION
This library provides the GPIO Wakeup interrupt demultiplexer.
 
INCLUDE FILES:
*/

/* includes */
#include "vxWorks.h"
#include "logLib.h"
#include "sysGpioDemux.h"


/* defines */
#define GPIO_WKUP_INT_ANY			0	/* 00 - interrupt at any transition */
#define GPIO_WKUP_INT_RISING_EDGE	1	/* 01 - interrupt on rising edge */
#define GPIO_WKUP_INT_FALLING_EDGE	2	/* 10 - interrupt on falling edge */
#define GPIO_WKUP_INT_PULSE			3	/* 11 - interrupt on pulse (any 2 transitions) */

#define GPIO_WKUP_INT_MASK			3	/* 11 */

#define GPIO_WKUP_MASTER_ENABLE		0x01000000	/* Wakeup GPIO master enable pin */

/* types */

typedef struct _gpioWakeupIntSource
	{
	VOIDFUNCPTR		func;   	/* function to call */
	int				arg;		/* argument to pass when the function is called */
	UINT32          mask;		/* bit mask */
	int				nInt;		/* number of interrupts */
	int				nIntNull;	/* number of unhandled interrupts */
	BOOL			enabled;	/* interrupt is enabled */
	} GPIO_WKUP_INT_SOURCE;

/* locals */

/* default initialisation for the interrupts */
LOCAL GPIO_WKUP_INT_SOURCE gpioWkupIntTbl[] = {
	{ NULL, 0, 0x80000000, 0, 0, FALSE },  	/* GPIO_WKUP_7 	*/ 
	{ NULL, 0, 0x40000000, 0, 0, FALSE },  	/* GPIO_WKUP_6 	*/ 
	{ NULL, 0, 0x20000000, 0, 0, FALSE },  	/* GPIO_WKUP_5	*/  
	{ NULL, 0, 0x10000000, 0, 0, FALSE },  	/* GPIO_WKUP_4  */   
	{ NULL, 0, 0x08000000, 0, 0, FALSE },  	/* GPIO_WKUP_3  */   
	{ NULL, 0, 0x04000000, 0, 0, FALSE },  	/* GPIO_WKUP_2	*/  
	{ NULL, 0, 0x02000000, 0, 0, FALSE },  	/* GPIO_WKUP_1	*/  
	{ NULL, 0, 0x01000000, 0, 0, FALSE }	/* GPIO_WKUP_0  */   
};

/* locals */


/* forward declarations */


/***************************************************************************
*
* sysAsicIntHandler - external interrupt handler
*
* This routine calls the appropriate function when an Wakeup
* interrupts occurs.
*
* RETURNS: N/A
*/

LOCAL void sysGpioWkupIntHandler
	(
	int param	/* not used */
	)
	{
	int 					intId;
	UINT32 					wkupStatus;
	GPIO_WKUP_INT_SOURCE *	pGpioWkup;

	wkupStatus = *GPW_SR; /* get status */

	for (intId = 0;intId < NELEMENTS(gpioWkupIntTbl); intId++)
		{
		pGpioWkup = &gpioWkupIntTbl[intId];
		
		/* interrupts occurs ? */
		if ((wkupStatus & pGpioWkup->mask) == pGpioWkup->mask)
			{
			/* acknowledge interrupt */
			
			*GPW_SR = pGpioWkup->mask;

			if (pGpioWkup->func != NULL)
				{
				pGpioWkup->nInt++;
				(*pGpioWkup->func)(pGpioWkup->arg);
				}
			else
				{
				pGpioWkup->nIntNull++;
				logMsg("Unhandled WKUP interrupts %i\n", intId, 0,0,0,0,0);
				}
			}
		
		} /* for */
	}



/*******************************************************************************
*
* sysGpioWkupDemuxInit - initialise
*
* This routine registers the GPIO wakeup interrupt handler
*
* RETURNS: N/A
*/

STATUS sysGpioWkupDemuxInit
	(
    void
	)
    {
	return(intConnect (INUM_TO_IVEC (INUM_GPIO_wkup), sysGpioWkupIntHandler, 0));
	}

/***************************************************************************
*
* sysGpioWkupIntConnect - connect a function to an GPIO Wakeup interrupt
*
* This routine connects the <func> routine to the GPIO Wakeup interrupt
* <intId>.
*
* RETURNS: OK or ERROR if failed
*/

STATUS sysGpioWkupIntConnect
	(
	int 		intId,	/* external interrupt ID */
	VOIDFUNCPTR func,	/* function to call */
	int 		arg	   	/* argument to pass */
	)
	{
	int 					level;
	UINT32 					mask; 	/* For GPIO interrupt type */
	GPIO_WKUP_INT_SOURCE *	pGpioWkup;

	if (intId < 0 || intId >= NELEMENTS(gpioWkupIntTbl) || func == NULL)
		{
		return(ERROR);
		}

	pGpioWkup = &gpioWkupIntTbl[intId];
	
	mask = (GPIO_WKUP_INT_MASK << (30-(2*intId))); /* mask on corresponding ltyp element */

	level = intLock(); /* LOCK INTERRUPT */

	*GPW_ME = 0;
	
	*GPW_WE |= pGpioWkup->mask;		/* 1=pin for use as GPIO (enabled) */
	*GPW_DD &= ~pGpioWkup->mask;	/* 0=pin is input */

	/* set interrupt type */
	*GPW_IT &= ~mask;
	*GPW_IT |= (GPIO_WKUP_INT_RISING_EDGE << (30-(2*intId)));
	
	/* set function to call */
	pGpioWkup->func = func;
	pGpioWkup->arg  = arg;
	
	*GPW_ME = GPIO_WKUP_MASTER_ENABLE;

	intUnlock(level);	/* UNLOCK INTERRUPT */

	return (OK);
	}


/***************************************************************************
*
* sysGpioWkupIntEnable - enable an GPIO wakeup interrupt
*
* This routine enables the GPIO wakeup interrupt <intId>.
*
* RETURNS: OK or ERROR if failed
*/

STATUS sysGpioWkupIntEnable
	(
	int intId	/* interrupt to enable */
	)
	{
	int level;
	GPIO_WKUP_INT_SOURCE *pGpioWkup;

	if (intId < 0 || intId >= NELEMENTS(gpioWkupIntTbl))
		{
		return(ERROR);
		}

	pGpioWkup = &gpioWkupIntTbl[intId];

	level = intLock(); /* LOCK INTERRUPT */

	*GPW_ME = 0;

	*GPW_WE |= pGpioWkup->mask;		/* 1=pin for use as GPIO (enabled) */
	
	/* enable interrupt each mode */
	*GPW_IE	 |= pGpioWkup->mask;
	*GPW_SIE |= pGpioWkup->mask;

	*GPW_ME = GPIO_WKUP_MASTER_ENABLE;
	
	pGpioWkup->enabled = TRUE;

	intEnable(INUM_GPIO_wkup);
	
	intUnlock(level);	/* UNLOCK INTERRUPT */

	return (OK);
	}

/***************************************************************************
*
* sysGpioWkupIntDisable - disable an GPIO wakeup interrupt
*
* This routine disables the GPIO wakeup interrupt <intId>.
*
* RETURNS: OK or ERROR if failed
*/

STATUS sysGpioWkupIntDisable
	(
	int intId	/* interrupt to disable */
	)
	{
	int 					level;
	GPIO_WKUP_INT_SOURCE *	pGpioWkup;
	int						i;
	BOOL					found = FALSE;

	if (intId < 0 || intId >= NELEMENTS(gpioWkupIntTbl))
		{
		return(ERROR);
		}

	pGpioWkup = &gpioWkupIntTbl[intId];

	level = intLock(); /* LOCK INTERRUPT */

	*GPW_ME = 0;

	*GPW_WE &= ~pGpioWkup->mask;		/* 0=pin not enabled */
	
	/* disable interrupt each mode */
	*GPW_IE	 &= ~pGpioWkup->mask;
	*GPW_SIE &= ~pGpioWkup->mask;

	*GPW_ME = GPIO_WKUP_MASTER_ENABLE;

	pGpioWkup->enabled = FALSE;

	for (i = 0; i < NELEMENTS(gpioWkupIntTbl); i++)
		{
		pGpioWkup = &gpioWkupIntTbl[i];
		if (pGpioWkup->enabled == TRUE)
			{
			found = TRUE;
			break;
			}
		}

	if (!found)
		{
		intDisable(INUM_GPIO_wkup);
		}

	intUnlock(level);	/* UNLOCK INTERRUPT */

	return (OK);
	}

