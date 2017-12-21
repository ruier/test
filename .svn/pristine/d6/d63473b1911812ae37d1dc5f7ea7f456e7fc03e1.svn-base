/* 440gxBusErr.s - IBM 440GX chip dependent BESR/BEAR reporting */

/* Copyright 2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,05aug03,jtp  Formal code inspection changes: improved comments.
01a,25apr03,pch  created
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
#include "ppc440gx.h"
#include "ibmEvb440gx.h"

#define POB_NUM_MASTERS		4	/* 4 POB masters */

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
* Return values:
*
*   p0 and p1 contain a representation of the error address.  Typically
*   this will be a physical address, with p0 more significant.
*
*   p2 in some sense identifies the source of the error, or is zero if
*   no error can be identified.  It may be a copy of the BESR if there is
*   only one, but it typically will need to be abstracted to unambiguously
*   identify a single source among those reported in several BESR's.
*   This implementation, for the 440GX, uses:
*   BIT  significance
*    0   1 => address in p0-p1 is virtual (else physical)
*   1-7  0 => no error found
*        1 => error reported by PLB0_BESR, physical addr from PLB0_BEAR[HL]
*        2 => error reported by POB0_BESR[01], physical addr from POB0_BEAR[HL]
*	 3-127 => TBD
*   8-31 For PLB/POB, compressed BESR contents:  PTE and R/W from each
*	 master 0-7.  Lock bits, and the POB's WIRQ, are omitted.
*	 For other reporters, TBD.
*/

FUNC_BEGIN(sysBusErrRegsGet)
	li	p2, 0
        rlwinm.	p1, p0, 0, _PPC_MCSR_BIT_IPLBE, _PPC_MCSR_BIT_DWPLBE
	beqlr					/* MCSR shows no bus error */
	mfdcr	p0, PLB0_BESR
	andis.	p1, p0, PLB0_BESR_CACHE_MASK_U
	bne	usePlbBesr
	mfdcr	p0, POB0_BESR0
	andis.	p1, p0, POB0_BESR0_CACHE_MASK_U
	bne	usePobBesr0
	andi.	p1, p0, POB0_BESR0_CACHE_MASK_L
	bne	usePobBesr0
	mfdcr	p0, PLB0_BESR
	andis.	p1, p0, PLB0_BESR_IO_MASK_U
	bne	usePlbBesr
	andi.	p1, p0, PLB0_BESR_IO_MASK_L
	bne	usePlbBesr
	mfdcr   p0, POB0_BESR0
	andi.	p1, p0, POB0_BESR0_IO_MASK
	bne	usePobBesr0
	mfdcr	p0, POB0_BESR1
	andis.	p1, p0, POB0_BESR1_IO_MASK_U
	bne	usePobBesr1
	andi.	p1, p0, POB0_BESR1_IO_MASK_L
	beqlr					/* no error found */
usePobBesr1:
	/*
	 * p0 contains POB0_BESR1 -- refetch POB0_BESR0 (which has no PTE's
	 * set, but optimizing for that is more trouble than it's worth)
	 */
	mfdcr   p0, POB0_BESR0
usePobBesr0:
	/* p0 contains POB0_BESR0 */
	li	p2, POB_NUM_MASTERS		/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_POB_ID		/* identify POB as source */
	mtdcr   POB0_BESR0, p0			/* write bits back to clear */
pob0Loop:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	pob0Loop

	li	p0, POB_NUM_MASTERS		/* # of masters */
	mtspr	CTR, p0
	mfdcr	p0, POB0_BESR1
	mtdcr   POB0_BESR1, p0			/* write bits back to clear */
pob1Loop:
	rotlwi	p0, p0, 8		/* get next master's bits */
	rlwinm	p1, p0, 27, 29, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	pob1Loop

	mfdcr	p0, POB0_BEARH
	mfdcr	p1, POB0_BEARL
        blr

usePlbBesr:
	/* p0 contains PLB0_BESR */
	li	p2, 8				/* # of masters */
	mtspr	CTR, p2
	li	p2, SYS_BUS_ERR_PLB_ID		/* identify PLB as source */
	mtdcr   PLB0_BESR, p0			/* write bits back to clear */
plbLoop:
	rotlwi	p0, p0, 4		/* get next master's bits */
	rlwinm	p1, p0, 30, 30, 31	/* shift to position, discard rest */
	slwi	p2, p2, 3		/* position p2 for next field */
	or	p2, p2, p1
	bdnz	plbLoop

	mfdcr	p0, PLB0_BEARH
	mfdcr	p1, PLB0_BEARL
	blr

FUNC_END(sysBusErrRegsGet)
