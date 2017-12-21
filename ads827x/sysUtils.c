/* sysUtils.c - Motorola ads826x common routines */

/*
 * Copyright (c) 2003-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

#include "copyright_wrs.h"

/*
modification history
--------------------
01e,25mar05,mdo  Documentation fixes for apigen
01d,24dec03,dtr  Mod to clk gen for 827x.
01c,04oct03,dee  fix typos
01b,02oct03,dee  fix typo in header
01a,01oct03,dee common 'C' routines for bootrom and vxWorks
*/

/*
DESCRIPTION
This file contains common utilities that need to be shared between
bootrom code and vxWorks code

INCLUDE FILES:
*/

#include "vxWorks.h"
#include "config.h"

/* forwards */

uint32_t sysBaudClkFreq(void);
uint32_t sysCpmFreqGet(void);
uint32_t sysVcoFreqGet(void);
uint32_t sysBusFreqGet(void);
uint32_t sysSccFreqGet(void);
void     romMsDelay(uint32_t);

/* externals */
extern void	 romDecSet(uint32_t);
extern uint32_t romDecGet(void);


/******************************************************************************
*
* sysBaudClkFreq - Obtains frequency of the BRG_CLK in Hz
*
* From page 9-5 in Rev. 0 MPC8260 PowerQUICC II User's Manual
*
* Baud clock = 2*CPM/2^(2*(sccrDfBrg + 1)
*  simplifying yields:
*          Baud clock = CPM/(2^(2*sccrDfbrg + 1))
*
* RETURNS: frequency of the BRG_CLK in Hz
*/

uint32_t sysBaudClkFreq (void)
    {
    uint32_t vcoFreq, sccrDfbrg;
	uint16_t divisor;

	/* Get the physical location of the Internal Mem Map */
	sccrDfbrg  = *(UINT32 *)(SCCR);
	sccrDfbrg &= SCMR_DFBRG_MASK;    /* only want lower bits */
	divisor = 1 << (2*(sccrDfbrg + 1));
	vcoFreq = sysVcoFreqGet();
	return ((vcoFreq)/divisor);
    }

/******************************************************************************
*
* sysCpmFreqGet - Determines the CPM Operating Frequency
*
* From page 9.5 Rev. 0  MPC8260  PowerQUICC II User's Manual
*  Calculation based on Figure 9-1.
*
* CPM_FREQ = VCO_OUT/(CPMDF + 1)
*
* RETURNS: CPM clock frequency for the current configuration settings
*
* ERRNO
*/

uint32_t sysCpmFreqGet (void)
	{
	uint32_t	scmr;				/* value for system clock mode register */ 		
	uint8_t		cpmdf;
	uint32_t	vcoFreq;			/* vco output frequency */

    /* get SCMR (system clock mode reg) */
	scmr =	*(uint32_t *)(SCMR);	

	cpmdf = SCMR_CPMDF(scmr);  	/* get value of cpmdf */

	vcoFreq = sysVcoFreqGet();
	return (vcoFreq / (cpmdf + 1));		/* return the cpm frequency */
	}


/******************************************************************************
*
* sysBusFreqGet - Determines the Bus Operating Frequency
*
* From page 9.5 Rev. 0  MPC8260  PowerQUICC II User's Manual
*  Calculation based on Figure 9-1.
*
* BUS_FREQ = vcoFreq/(BUSDF + 1)
* RETURNS: Bus clock frequency for the current configuration settings
*
* ERRNO
*/

uint32_t sysBusFreqGet (void)
	{
	uint32_t	scmr;				/* value for system clock mode register */ 		
	uint8_t		busdf;
	uint32_t	vcoFreq;			/* vco output frequency */

    /* get SCMR (system clock mode reg) */
	scmr =	*(uint32_t *)(SCMR);	

	busdf = SCMR_BUSDF(scmr);  		/* get value of busdf */

	vcoFreq = sysVcoFreqGet();
	return (vcoFreq / (busdf + 1));		/* return the bus clock frequency */
	}

/******************************************************************************
*
* sysSccFreqGet - Determines the SCC Frequency
*
* From page 9.5 Rev. 0  MPC8260  PowerQUICC II User's Manual
*  Calculation based on Figure 9-1.
*
* SCC_FREQ = vcoFreq/4
* RETURNS: SCC clock frequency for the current configuration settings
*
* ERRNO
*/

uint32_t sysSccFreqGet (void)
	{
	return (sysVcoFreqGet() / 4);		/* return the scc frequency */
	}

/******************************************************************************
*
* sysVcoFreqGet - Determines the VCO Frequency
*
* From page 9.5 Rev. 0  MPC8260  PowerQUICC II User's Manual
*  Calculation based on Figure 9-1.
*
* VCO_OUT = CLKIN/(PLLDF+1)  times  2*(PLLMF+1)
*
* PLLDF, PLLMF are read from System Clock Mode Register (SCMR)
*   MODCK and MODCK_H determine those values in SCMR
*
* RETURNS: VCO clock frequency for the current configuration settings
*
* ERRNO
*/

uint32_t sysVcoFreqGet (void)
	{
	uint32_t	scmr;				/* value for system clock mode register */ 		
	uint16_t	pllmf;				/* pll divide/multiply factors */
	uint32_t	vcoFreq;			/* vco output frequency */

    /* get SCMR (system clock mode reg) */
	scmr =	*(uint32_t *)(SCMR);	

	pllmf = SCMR_PLLMF(scmr);	/* get value of pllmf, lower 4 bits of scmr */

	vcoFreq = (OSCILLATOR_FREQ * (pllmf + 1));
	return (vcoFreq);				/* return the cpm frequency */
	}

/******************************************************************************
*
* sysUsDelay - delay at least the specified amount of time (in microseconds)
*
* This routine will delay for at least the specified amount of time using the
* lower 32 bit "word" of the Time Base register as the timer.  The accuracy of
* the delay increases as the requested delay increases due to a certain amount
* of overhead.  As an example, a requested delay of 10 microseconds is
* accurate within approximately twenty percent, and a requested delay of 100
* microseconds is accurate within approximately two percent.
*
* NOTE:  This routine will not relinquish the CPU; it is meant to perform a
* busy loop delay.  The minimum delay that this routine will provide is
* approximately 10 microseconds.  The maximum delay is approximately the
* size of UINT32; however, there is no roll-over compensation for the total
* delay time, so it is necessary to back off two times the system tick rate
* from the maximum.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysUsDelay
    (
    uint32_t    delay        /* length of time in microsec to delay */
    )
    {
    volatile int loop;
    volatile uint32_t decValue;

	for(loop=0;loop<(delay/10);loop++)
		decValue = sysDecGet();
    }

/*********************************************************************
*
* sysMsDelay - calculate time elapsed
*
* This routine uses the decrementer to calculate time elapsed.
*
* RETURN: N/A
*
* ERRNO
*/

void sysMsDelay
    (
    uint32_t    delay                   /* length of time in MS to delay */
    )
    {
    register uint32_t oldval;               /* decrementer value */
    register uint32_t newval;               /* decrementer value */
    register uint32_t totalDelta;           /* Dec. delta for entire delay period */
    register uint32_t decElapsed;           /* cumulative decrementer ticks */

    /*
     * Calculate delta of decrementer ticks for desired elapsed time.
     * The macro DEC_CLOCK_FREQ MUST REFLECT THE PROPER 6xx BUS SPEED.
     */

    totalDelta = ((DEC_CLOCK_FREQ / 4) / 1000) * delay;

    /*
     * Now keep grabbing decrementer value and incrementing "decElapsed" until
     * we hit the desired delay value.  Compensate for the fact that we may
     * read the decrementer at 0xffffffff before the interrupt service
     * routine has a chance to set in the rollover value.
     */

    decElapsed = 0;

    oldval = sysDecGet ();

    while (decElapsed < totalDelta)
        {
        newval = sysDecGet ();

        if ( DELTA(oldval,newval) < 1000 )
            decElapsed += DELTA(oldval,newval);  /* no rollover */
        else
            if (newval > oldval)
                decElapsed += sysAbs((int)oldval);  /* rollover */

        oldval = newval;
        }
    }


/*********************************************************************
*
* sysDelay - fixed 1ms delay 
*
* This routine just calls sysMsDelay.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }


#if DEBUG
/*********************************************************************
*
* sysMsDelay - calculate time elapsed
*     
* This routine uses the decrementer to calculate time elapsed.
*
* RETURNS: N/A
*
* NOTE:  This should only be called from bootup code - before decrementer
*        interrupts are enabled, since it adjusts the decrementer value.
*
* ERRNO
*/

void romMsDelay
    (
    uint32_t  delay                   /* length of time in MS to delay */
    )
    {
	uint32_t oneMS;
	uint32_t i;

    oneMS = ((DEC_CLOCK_FREQ / 4) / 1000); /* decrementer ticks per ms */

	for (i = 0; i < delay; i++)
		{ /* count milliseconds */
		romDecSet(oneMS);	/* set decrementer to 1 millisecond */
		while ((romDecGet() & 0x80000000) == 0)
			{ /* wait for decrementer to go negative */
				continue;
			}
		}
	}


/*********************************************************************
*
* flashRedLed - light the Red LED a specified number of times
*
* This routine illuminates the Red LED a specified number of times.  
*
* RETURNS : NONE
*
* NOTE:  This should only be called from bootup code - before decrementer
*        interrupts are enabled, since it calls romMsDelay.
*
* ERRNO
*/
void flashRedLed
	(
	uint32_t times         /* how many times to light */
	)
	{
	uint32_t i;

	for (i = 0; i < times; i++)
		{
		*BCSR0 &= ~BCSR0_LED_RED; /* turn on by clearing bit */
		romMsDelay(1000);
		*BCSR0 |= BCSR0_LED_RED;   /* turn off by setting bit */
		romMsDelay(1000);
		}
	romMsDelay (2000);	/* delay 2 seconds after sequence */
	}

/*********************************************************************
*
* flashGreenLed - light the Green LED a specified number of times
*
* This routine illuminates the Green LED a specified number of times.  
*
* RETURNS: N/A
*
* NOTE:  This should only be called from bootup code - before decrementer
*        interrupts are enabled, since it calls romMsDelay.
*
* ERRNO
*/
void flashGreenLed
	(
	uint32_t times          /* how many times to light */
	)
	{
	uint32_t i;

	for (i = 0; i < times; i++)
		{
		*BCSR0 &= ~BCSR0_LED_GREEN; /* turn on by clearing bit */
		romMsDelay(1000);
		*BCSR0 |= BCSR0_LED_GREEN;	 /* turn off by setting bit */
		romMsDelay(1000);
		}
	romMsDelay (2000);	/* delay 2 seconds after sequence */
	}

#endif

