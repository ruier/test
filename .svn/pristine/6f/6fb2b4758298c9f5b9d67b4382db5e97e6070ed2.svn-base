/* 440epBusErr.s - IBM 440EP chip dependent BESR/BEAR reporting */

/* Copyright 2003-2005 Wind River Systems, Inc. */

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
01d,07mar05,dr  Merging with latest version from AMCC
01c,26jan05,tr  Add support for PPC440EP 2.0
01b,02nov04,dr  Documentation changes for apigen.
01a,15oct03,tr  created from 440gxBusErr.s version 01b
*/

/*
DESCRIPTION

This module supplies a function for identifying and storing the
causes of a bus error.  This function is implemented in assembly
language to limit the side effects of accessing memory, and because
it requires the evaluation of several chip-specific registers.

*/

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"
#include "config.h"


#define PLB_NUM_MASTERS		8	/*  Max PLB masters number */

        /* globals */

        FUNC_EXPORT(sysBusErrRegsGet)

        _WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysBusErrRegsGet - Report details of bus error that caused machine check
*
* Since processors based on the 440x5 core may have multiple BEAR and BESR
* registers, and the details may vary across implementations, the exception
* reporting mechanism delegates the diagnosis to this BSP function.
*
* This function may be called after a machine check not caused by a bus
* error, in which case it should return with p2 == 0; or when no machine
* check has occurred, in which case its return values do not matter.
*
* Entry conditions:
*
*   sp points to the ESF.  All fields except BESR, BEAR_H, and BEAR_L
*   have been filled in, and all registers have been saved.
*
*   If a machine check has occurred, p0 contains the MCSR.  Bits corresponding
*   to any recoverable conditions which have already been handled will have
*   been cleared.  If no machine check has occurred, p0 contains garbage.
*
* RETURNS:
*
*   p0 and p1 contain a representation of the error address.  Typically
*   this will be a physical address, with p0 more significant.
*
*   p2 in some sense identifies the source of the error, or is zero if
*   no error can be identified.  It may be a copy of the BESR if there is
*   only one, but it typically will need to be abstracted to unambiguously
*   identify a single source among those reported in several BESR's.
*   This implementation, for the 440EP 1.0, uses:
*   BIT  significance
*    0   1 => address in p0-p1 is virtual (else physical)
*   1-7  0 => no error found
*        1 => error reported by PLB4_BESR, physical addr from PLB4_BEAR[HL]
*        2 => error reported by P4OB_BESR[01], physical addr from P4OB_BEAR[HL]
*	 3 => error reported by P4P3_ESR0, physical addr from P4P3_EADR[HL]
*	 4 => error reported by PLB3_BESR, physical addr from PLB3_BEAR
*        5 => error reported by P3OB_BESR[01], physical addr from P3OB_BEAR[HL]
*	 6-127 => TBD
*   8-31 For PLB/POB, compressed BESR contents:  PTE and R/W from each
*	 master 0-7.  Lock bits, and the POB's and P4P3B WIRQ, are omitted.
*	 For other reporters, TBD.
*
*   This implementation, for the 440EP 2.0, uses:
*   BIT  significance
*    0   1 => address in p0-p1 is virtual (else physical)
*   1-7  0 => no error found
*        1 => error reported by PLB0_BESRL, physical addr from PLB0_BEAR[HL]
*        2 => error reported by PLB1_BESRL, physical addr from PLB1_BEAR[HL]
*        3 => error reported by P4OB_BESR[01], physical addr from P4OB_BEAR[HL]
*	 4 => error reported by P4P3_ESR0, physical addr from P4P3_EADR[HL]
*	 5 => error reported by PLB3_BESR, physical addr from PLB3_BEAR
*        6 => error reported by P3OB_BESR[01], physical addr from P3OB_BEAR[HL]
*	 7-127 => TBD
*   8-31 - For PLB0_BESRL/PLB1_BESRL/POB, compressed BESR contents:  
*        PTE and R/W from each master 0-7.
*        Lock bits, and the POB's and P4P3B WIRQ, are omitted.
*	 For other reporters, TBD.
*/

FUNC_BEGIN(sysBusErrRegsGet)
	li	p2, 0
        rlwinm.	p1, p0, 0, _PPC_MCSR_BIT_IPLBE, _PPC_MCSR_BIT_DWPLBE
	beqlr					/* MCSR shows no bus error */
	/* Check errors from CPU core Masters (Cache units) */
	/* Check errors on PLB4 */
#ifdef PPC440EP_REVA /* PPC440EP 1.0 */

	/* Check PLB4 arbiter errors */
	mfdcr	p0, PLB4_BESR
	andis.	p1, p0, PLB4_BESR_CACHE_MASK_U
	bne	usePlb4Besr
	
#else /* PPC440EP 2.0 */
	mfdcr	p0, PLB0_BESRL
	andis.	p1, p0, PLB_BESRL_CACHE_MASK_U
	bne	usePlb0lBesr 
	mfdcr	p0, PLB1_BESRL
	andis.	p1, p0, PLB_BESRL_CACHE_MASK_U
	bne	usePlb1lBesr
#endif /* PPC440EP 1.0 */

	/* Check PLB4 to OPB bridge errors */
	mfdcr	p0, P4OB_BESR0
	andis.	p1, p0, P4OB_BESR0_CACHE_MASK_U
	bne	useP4obBesr0
	andi.	p1, p0, P4OB_BESR0_CACHE_MASK_L
	bne	useP4obBesr0
	/* Check PLB4 to PLB3 bridge errors */
	mfdcr	p0, P4P3_ESR0
	andis.	p1, p0, P4P3_ESR0_CACHE_MASK_U
	bne	useP4p3bEsr0
	andi.	p1, p0, P4P3_ESR0_CACHE_MASK_L
	bne	useP4p3bEsr0	
	
checkPlb3:
        /* Check PLB3 arbiter errors envolving PLB4 masters */
	mfdcr	p0, PLB3_BESR
	andi.	p1, p0, PLB3_BESR_P4P3B_MASK_U
	bne	usePlb3besr
	
checkPlb3opb:
        /* Check PLB3 to OPB bridge errors */
	mfdcr	p0, P3OB_BESR0
	andi.	p1, p0, P3OB_BESR0_P4P3B_MASK_U
	bne	useP3obBesr0
	      
	/* Check errors from other Masters (IO units) */
	/* Check errors on PLB4 */
	
#ifdef PPC440EP_REVA /* PPC440EP 1.0 */

	/* Check PLB4 arbiter errors */
	mfdcr	p0, PLB4_BESR
	andis.	p1, p0, PLB4_BESR_IO_MASK_U
	bne	usePlb4Besr
	andi.	p1, p0, PLB4_BESR_IO_MASK_L
	bne	usePlb4Besr
	
#else /* PPC440EP 2.0 */
	mfdcr	p0, PLB0_BESRL
	andis.	p1, p0, PLB_BESRL_IO_MASK_U
	bne	usePlb0lBesr
	andi.	p1, p0, PLB_BESRL_IO_MASK_L
	bne	usePlb0lBesr
	mfdcr	p0, PLB1_BESRL
	andis.	p1, p0, PLB_BESRL_IO_MASK_U
	bne	usePlb1lBesr
	andi.	p1, p0, PLB_BESRL_IO_MASK_L
	bne	usePlb1lBesr
#endif /* PPC440EP 1.0 */
	
	/* Check PLB4 to OPB bridge errors */
	mfdcr   p0, P4OB_BESR0
	andi.	p1, p0, P4OB_BESR0_IO_MASK_L
	bne	useP4obBesr0
	mfdcr	p0, P4OB_BESR1
	andis.	p1, p0, P4OB_BESR1_IO_MASK_U
	bne	useP4obBesr1
	/* Check PLB4 to PLB3 bridge errors */
	mfdcr	p0, P4P3_ESR0
	andi.	p1, p0, P4P3_ESR0_IO_MASK_L
	bne	useP4p3bEsr1Io	
	/* Check PLB3 arbiter errors */
checkPlb3Io:
        /* Check PLB3 arbiter errors envolving PLB4 masters */
	mfdcr	p0, PLB3_BESR
	andis.	p1, p0, PLB3_BESR_P4P3B_MASK_U
	bne	usePlb3besrIo
	/* Check PLB3 to OPB bridge errors */
	
checkPlb3opbIo:
        /* Check PLB3 to OPB bridge errors envolving PLB4 masters */
	mfdcr	p0, P3OB_BESR0
	andis.	p1, p0, P3OB_BESR0_P4P3B_MASK_U
	bne	useP3obBesr0		
	beqlr					/* no error found */

useP3obBesr1:
	/*
	 * p0 contains P3OB_BESR1 -- refetch P3OB_BESR0 (which has no PTE's
	 * set, but optimizing for that is more trouble than it's worth)
	 */
	mfdcr   p0, P3OB_BESR0
useP3obBesr0:
	/* p0 contains P3OB_BESR0 */
	li	p2, (PLB_NUM_MASTERS/2)	/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_P3OB_ID		/* identify P3OB as source */
	mtdcr   P3OB_BESR0, p0			/* write bits back to clear */
p3ob0Loop:
	rotlwi	p0, p0, 5		/* get next master's bits */
	rlwinm	p1, p0, 30, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p3ob0Loop

	li	p0, (PLB_NUM_MASTERS/2)   /* # of masters */
	mtspr	CTR, p0
	mfdcr	p0, P3OB_BESR1
	mtdcr   P3OB_BESR1, p0			/* write bits back to clear */
p3ob1Loop:
	rotlwi	p0, p0, 5		/* get next master's bits */
	rlwinm	p1, p0, 30, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p3ob1Loop
	/* Only 32 bits address */
	mfdcr	p0, 0
	mfdcr	p1, P3OB_BEAR
        blr

useP4p3bEsr0:
	/* p0 contains P4P3_ESR0 */
	li	p2, (PLB_NUM_MASTERS/2)	/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_P4P3B_ID	/* identify P4P3B as source */
	mtdcr   P4P3_ESR0, p0		/* write bits back to clear */
p4p3Loop0:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p4p3Loop0

	li	p0, (PLB_NUM_MASTERS/2)   /* # of masters */
	mtspr	CTR, p0
	mfdcr	p0, P4P3_ESR1
	mtdcr   P4P3_ESR1, p0			/* write bits back to clear */
p4p3Loop1:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p4p3Loop1

	mfdcr	p0, P4P3_EUADR
	mfdcr	p1, P4P3_EADR
        b checkPlb3

useP4p3bEsr1Io:
	/*
	 * p0 contains P4P3_ESR1 -- refetch P4P3_ESR0 (which has no PTE's
	 * set, but optimizing for that is more trouble than it's worth)
	 */
	mfdcr   p0, P4P3_ESR0
useP4p3bEsr0Io:
	/* p0 contains P4P3_ESR0 */
	li	p2, (PLB_NUM_MASTERS/2)	/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_P4P3B_ID	/* identify P4P3B as source */
	mtdcr   P4P3_ESR0, p0		/* write bits back to clear */
p4p3Loop0Io:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p4p3Loop0Io

	li	p0, (PLB_NUM_MASTERS/2)   /* # of masters */
	mtspr	CTR, p0
	mfdcr	p0, P4P3_ESR1
	mtdcr   P4P3_ESR1, p0			/* write bits back to clear */
p4p3Loop1Io:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p4p3Loop1Io

	mfdcr	p0, P4P3_EUADR
	mfdcr	p1, P4P3_EADR
        b checkPlb3Io

        
usePlb3besr:
	/* p0 contains PLB3_BESR */
	li	p2, PLB_NUM_MASTERS		/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_PLB3_ID		/* identify PLB3 as source */
	mtdcr   PLB3_BESR, p0			/* write bits back to clear */
plb3Loop:
	rotlwi	p0, p0, 4		/* get next master's bits */
	rlwinm	p1, p0, 30, 30, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	plb3Loop
	/* Only 32 bits address */
	mfdcr	p0, 0
	mfdcr	p1, PLB3_BEAR
	b checkPlb3opb 	

usePlb3besrIo:
	/* p0 contains PLB3_BESR */
	li	p2, PLB_NUM_MASTERS		/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_PLB3_ID		/* identify PLB3 as source */
	mtdcr   PLB3_BESR, p0			/* write bits back to clear */
plb3LoopIo:
	rotlwi	p0, p0, 4		/* get next master's bits */
	rlwinm	p1, p0, 30, 30, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	plb3Loop
	/* Only 32 bits address */
	mfdcr	p0, 0
	mfdcr	p1, PLB3_BEAR
	b checkPlb3opbIo 

	
useP4obBesr1:
	/*
	 * p0 contains P4OB_BESR1 -- refetch P4OB_BESR0 (which has no PTE's
	 * set, but optimizing for that is more trouble than it's worth)
	 */
	mfdcr   p0, P4OB_BESR0
useP4obBesr0:
	/* p0 contains P4OB_BESR0 */
	li	p2, (PLB_NUM_MASTERS/2)	/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_P4OB_ID		/* identify P4OB as source */
	mtdcr   P4OB_BESR0, p0			/* write bits back to clear */
p4ob0Loop:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p4ob0Loop

	li	p0, (PLB_NUM_MASTERS/2)   /* # of masters */
	mtspr	CTR, p0
	mfdcr	p0, P4OB_BESR1
	mtdcr   P4OB_BESR1, p0			/* write bits back to clear */
p4ob1Loop:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	p4ob1Loop

	mfdcr	p0, P4OB_BEARH
	mfdcr	p1, P4OB_BEARL
        blr
#ifdef PPC440EP_REVA /* PPC440EP 1.0 */
usePlb4Besr:
	/* p0 contains PLB4_BESR */
	li	p2, PLB_NUM_MASTERS	/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_PLB4_ID		/* identify PLB4 as source */
	mtdcr   PLB4_BESR, p0			/* write bits back to clear */
plb4Loop:
	rotlwi	p0, p0, 4		/* get next master's bits */
	rlwinm	p1, p0, 30, 30, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	plb4Loop

	mfdcr	p0, PLB4_BEARH
	mfdcr	p1, PLB4_BEARL
	blr
#else /* PPC440EP 2.0 */
usePlb0lBesr:
	/* p0 contains PLB0_BESRL */
	li	p2, PLB_NUM_MASTERS	/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_PLB0L_ID /* identify PLB0L as source */
	mtdcr   PLB0_BESRL, p0		/* write bits back to clear */
plb0lLoop:
	rotlwi	p0, p0, 4		/* get next master's bits */
	rlwinm	p1, p0, 30, 30, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	plb0lLoop
	
	mfdcr	p0, PLB0_BEARH
	mfdcr	p1, PLB0_BEARL
	blr
	
usePlb1lBesr:
	/* p0 contains PLB1_BESRL */
	li	p2, PLB_NUM_MASTERS	/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_PLB1L_ID /* identify PLB as source */
	mtdcr   PLB1_BESRL, p0		/* write bits back to clear */
plbl1Loop:
	rotlwi	p0, p0, 4		/* get next master's bits */
	rlwinm	p1, p0, 30, 30, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	plbl1Loop

	mfdcr	p0, PLB1_BEARH
	mfdcr	p1, PLB1_BEARL
	blr	
#endif /* PPC440EP 1.0 */

FUNC_END(sysBusErrRegsGet)
