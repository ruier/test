/* 440epBusErrOriginShow.c - decode the "besr" value from sysBusErrRegsGet */

/* 
 * Copyright (c) 2003-2005 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */ 

/*
*****************************************************************************
   COPYRIGHT   Applied Micro Circuits Corporation (AMCC) 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF AMCC
   All rights reserved
   AMCC Proprietary

******************************************************************************
\NOMANUAL
*/

/*
modification history
--------------------
01e,27jun05,yvp sysBusErrOriginShow takes a VOIDFUNCPTR arg.
01d,07mar05,dr  merge with AMCC version
01c,26jan05,tr  Add support for PPC440EP 2.0
01b,02nov04,dr  Documentation changes for apigen
01a,24oct03,tr  created from 440gxBusErrOriginShow.c version 01b
*/

/* includes */

#include <vxWorks.h>
#include <ppc440EP.h>
#include <amcc440ep.h>

/* globals */

/*
 * macros to aid in extracting fields from the BESR value passed in. The
 * top byte contains the reporter ID. The remaining bits are in 8 groups
 * of 3. Each group refers to the bus master sourcing the cycle.  The
 * most significant two bits are the PTE code, and the least significant
 * bit is the direction.
 */

#define BESR_REPORTER(val)    ((val) >> 24)
#define BESR_SRC_EXTRACT(val,ix)  (((val) >> (21 - 3 * (ix))) & 7)
#define BESR_SRC_EXISTS(val,ix)   (BESR_SRC_EXTRACT(val, ix) != 0)
#define BESR_SRC_PTE_CODE(val,ix) (BESR_SRC_EXTRACT(val, ix) >> 1)
#define BESR_SRC_DIRECTION(val,ix)  (BESR_SRC_EXTRACT(val, ix) & 1)

static char * plb4MastName[] =
    {
    "I-cache", "D-cache fill", "D-cache castout",
    "DMA to PLB4", "PLB3 to PLB4 Bridge","Not used","Not used","Not used"
    };
static char * plb3MastName[] =
    {
    "PLB4 to PLB3 Bridge", "MADMAL (Emac)",
    "PCI", "DMA", "OPB to PLB3", "EBC Master","Not used","Not used"
    };

static char * pteDesc[] =
    {
    "no", "timeout", "slave", "reserved"
    };

static char * dirDesc[] =
    {
     "write","read"
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
*   printFcn  Function used to print report.
*   It is actually printExc, whose prototype is in fioLib.h.
*
*   besrVal The value returned in p2 by sysBusErrRegsGet().  The format
*   is described with sysBusErrRegsGet(), in 440epBusErr.s.
*
*   for the 440EP 1.0:
*   BIT  significance
*    0    1 => address in p0-p1 is virtual (else physical)
*    1-7  0 => no error found
*         1 => error reported by PLB4_BESR, physical addr from PLB4_BEAR[HL]
*         2 => error reported by P4OB_BESR[01], physical addr from P4OB_BEAR[HL]
*         3 => error reported by P4P3_ESR0, physical addr from P4P3_EADR[HL]
*         4 => error reported by PLB3_BESR, physical addr from PLB3_BEAR
*         5 => error reported by P3OB_BESR[01], physical addr from P3OB_BEAR[HL]
*         6-127 => TBD
*         8-31 For PLB/POB, compressed BESR contents:
*         PTE and R/W from each master 0-7.
*         Lock bits, and the POB's and P4P3B's WIRQ, are omitted.
*         For other reporters, TBD.
*
*   for the 440EP 2.0:
*   BIT  significance
*    0   1 => address in p0-p1 is virtual (else physical)
*    1-7 0 => no error found
*        1 => error reported by PLB0_BESRL, physical addr from PLB0_BEAR[HL]
*        2 => error reported by PLB1_BESRL, physical addr from PLB1_BEAR[HL]
*        3 => error reported by P4OB_BESR[01], physical addr from P4OB_BEAR[HL]
*        4 => error reported by P4P3_ESR0, physical addr from P4P3_EADR[HL]
*        5 => error reported by PLB3_BESR, physical addr from PLB3_BEAR
*        6 => error reported by P3OB_BESR[01], physical addr from P3OB_BEAR[HL]
*        7-127 => TBD
*        8-31 - For PLB0_BESRL/PLB1_BESRL/POB, compressed BESR contents:
*        PTE and R/W from each master 0-7.
*        Lock bits, and the POB's and P4P3B's WIRQ, are omitted.
*        For other reporters, TBD.
*/

void sysBusErrOriginShow
    (
    VOIDFUNCPTR	printFcn,	/* print function */
    UINT32	besrVal		/* value returned in p2 by sysBusErrRegsGet() */
    )
    {
    char * reporter;
    char * mastName [8];
    int    i;

    printFcn("    Bus Error Address is %s\n",
	     (int)((besrVal & 0x80000000) ? "virtual" : "physical"),
	     0, 0, 0, 0, 0);

    switch (BESR_REPORTER(besrVal))
  {
  #ifdef PPC440EP_REVA /* PPC440EP 1.0 */

  case SYS_BUS_ERR_PLB4_ID:   reporter = "PLB4";   for (i=0;i<8;i++) mastName[i] = plb4MastName[i] ;break;

  #else /* PPC440EP 2.0 */

  case SYS_BUS_ERR_PLB0L_ID:    reporter = "PLB0L";   for (i=0;i<8;i++) mastName[i] = plb4MastName[i] ;break;
  case SYS_BUS_ERR_PLB1L_ID:    reporter = "PLB1L";   for (i=0;i<8;i++) mastName[i] = plb4MastName[i] ;break;

  #endif /* PPC440EP 1.0 */

  case SYS_BUS_ERR_P4OB_ID:    reporter = "P4OB";  for (i=0;i<8;i++) mastName[i] = plb4MastName[i] ;break;
  case SYS_BUS_ERR_P4P3B_ID:   reporter = "P4P3B"; for (i=0;i<8;i++) mastName[i] = plb4MastName[i] ;break;
  case SYS_BUS_ERR_PLB3_ID:    reporter = "PLB3";  for (i=0;i<8;i++) mastName[i] = plb3MastName[i] ;break;
  case SYS_BUS_ERR_P3OB_ID:    reporter = "P3OB";  for (i=0;i<8;i++) mastName[i] = plb3MastName[i] ;break;
  default:          return;
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
