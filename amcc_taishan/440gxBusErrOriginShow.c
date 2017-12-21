/* 440gxBusErrOriginShow.c - decode the "besr" value from sysBusErrRegsGet */

/* 
 * Copyright (c) 2004-2005 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */ 

/*
modification history
--------------------
01d,27jun05,yvp  sysBusErrOriginShow takes a VOIDFUNCPTR arg. Copyright update.
01c,27oct04,mdo  Documentation fixes for apigen
01b,05aug03,jtp  Formal code inspection changes. Added descriptive macros.
01a,25apr03,pch  created
*/

/* includes */

#include <vxWorks.h>
#include <ppc440gx.h>
#include <ibmEvb440gx.h>

/* globals */

/* 
DESCRIPTION

 macros to aid in extracting fields from the BESR value passed in. The
 top byte contains the reporter ID. The remaining bits are in 8 groups
 of 3. Each group refers to the bus master sourcing the cycle.  The
 most significant two bits are the PTE code, and the least significant
 bit is the direction.

INCLUDE FILES:
 
*/

#define BESR_REPORTER(val)		((val) >> 24)
#define BESR_SRC_EXTRACT(val,ix)	(((val) >> (21 - 3 * (ix))) & 7)
#define BESR_SRC_EXISTS(val,ix)		(BESR_SRC_EXTRACT(val, ix) != 0)
#define BESR_SRC_PTE_CODE(val,ix)	(BESR_SRC_EXTRACT(val, ix) >> 1)
#define BESR_SRC_DIRECTION(val,ix)	(BESR_SRC_EXTRACT(val, ix) & 1)

static char * mastName[] =
    {
    "I-cache", "D-cache fill", "D-cache castout",
    "PCIX", "I2O", "MAL (Emac)", "DMA", "OPB2PLB"
    };

static char * pteDesc[] =
    {
    "no", "timeout", "slave", "reserved"
    };

static char * dirDesc[] =
    {
    "read", "write"
    };


/*******************************************************************************
*
* sysBusErrOriginShow - Report origin of bus error that caused machine check
*
* This function will be called from excArchShow(), when reporting on a
* machine check for which sysBusErrRegsGet() previously returned p2 != 0.
*
* Parameters
*
*   printFcn	Function used to print report.
*		It is actually printExc, whose prototype is in fioLib.h.
*
*   besrVal	The value returned in p2 by sysBusErrRegsGet().  The format
*		is described with sysBusErrRegsGet(), in 440gxBusErr.s.
*
*   BIT  significance
*    0   1 => address in p0-p1 is virtual (else physical)
*   1-7  0 => no error found
*        1 => error reported by PLB0_BESR, physical addr from PLB0_BEAR[HL]
*        2 => error reported by POB0_BESR[01], physical addr from POB0_BEAR[HL]
*	 3-127 => TBD
*   8-31 For PLB/POB, compressed BESR contents:  PTE and R/W from each
*	 master 0-7.  Lock bits, and the POB's WIRQ, are omitted.
*	 For other reporters, TBD.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysBusErrOriginShow
    (
    VOIDFUNCPTR	printFcn,	/* print function */
    UINT32	besrVal		/* value returned in p2 by sysBusErrRegsGet() */
    )
    {
    char * reporter;
    int    i;

    printFcn("    Bus Error Address is %s\n",
		(int)((besrVal & 0x80000000) ? "virtual" : "physical"),
	     0, 0, 0, 0, 0);

    switch (BESR_REPORTER(besrVal))
        {
        case SYS_BUS_ERR_PLB_ID:    reporter = "PLB";   break;
        case SYS_BUS_ERR_POB_ID:    reporter = "POB";   break;
        default:                    return;
        }

    for ( i = 0 ; i < 8 ; ++i )
        {
        if (BESR_SRC_EXISTS(besrVal, i) != 0)
            printFcn("    %s master %d [%s] %s error on %s\n",
                     (int)reporter, i, (int)mastName[i],
                     (int)pteDesc[BESR_SRC_PTE_CODE(besrVal, i)],
                     (int)dirDesc[BESR_SRC_DIRECTION(besrVal, i)], 0 );
        }
    }
