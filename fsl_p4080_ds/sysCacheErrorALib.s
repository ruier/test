/* sysCacheErrorALib.s - P4080 cache error handler assembly routines */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,22apr10,pmr  Created.
*/
	FUNC_EXPORT(sysCacheErrorHandler)
	FUNC_EXPORT(sysCacheErrorHandlerEnd)
#ifdef INCLUDE_L2_CACHE_ERROR_RECOVERY
	DATA_IMPORT(sysL2CacheErrorMsg)
#endif /* INCLUDE_L2_CACHE_ERROR_RECOVERY */
#ifdef INCLUDE_L3_CACHE_ERROR_RECOVERY
	DATA_IMPORT(sysL3CacheErrorMsg1)
	DATA_IMPORT(sysL3CacheErrorMsg2)
#endif /* INCLUDE_L3_CACHE_ERROR_RECOVERY */
#ifdef INCLUDE_SHOW_ROUTINES 
	DATA_IMPORT(sysL1CacheErrorCount)
#endif /* INCLUDE_SHOW_ROUTINES */

	_WRS_TEXT_SEG_START

	.balign 64      /* align to cache line boundary */

/*********************************************************************
 *
 * sysCacheErrorHandler - check for and handle cache errors
 *
 * This routine replaces the standard machine check handler.  It detects
 * L1, L2, and L3 cache errors, and handles them appropriately.  
 *
 * The standard e500 machine check handler is called if there are any 
 * machine-check conditions in addition to cache errors.
 */ 

FUNC_BEGIN(sysCacheErrorHandler)

	/* Save registers used */

	mtspr   SPRG4_W, p7
	mfcr	p7
	mtspr	SPRG5_W, p7

#ifdef L1_CACHE_ERROR_INJECTION
	mfspr   p7, L1CSR1
	rlwinm  p7, p7, 0, 17, 15
	sync
	isync
	mtspr   L1CSR1, p7
	isync

	mfspr   p7, L1CSR0
	rlwinm  p7, p7, 0, 17, 15
	sync
	isync
	mtspr   L1CSR0, p7
	isync
	mfspr	p7, L1CSR0
#endif /* L1_CACHE_ERROR_INJECTION */

#ifdef L2_CACHE_ERROR_INJECTION
	xor	p7, p7, p7
	mtspr   L2ERRINJCTL, p7
	sync
	isync
#endif /* L2_CACHE_ERROR_INJECTION */


#ifdef INCLUDE_L3_CACHE_ERROR_RECOVERY

	/*
	 * Check for L3 cache errors.  If any exist, do what is necessary
	 * to quiesce the L3 cache, log the error, and jump to the boot
	 * monitor.  It is assumed that L3 cache errors are not recoverable.
	 */

	/* L3 recovery requires an additional register */
	mtspr	SPRG6_W, r11

	lis	r11, HIADJ(CCSBAR+CPC1_BASE_ADDR)
	addi	r11, r11, LO(CCSBAR+CPC1_BASE_ADDR)
	b	32f

31: /* loop 2 for second CPC */
	mr	p7, r11
	lis	r11, HIADJ(CCSBAR+CPC2_BASE_ADDR)
	addi	r11, r11, LO(CCSBAR+CPC2_BASE_ADDR)
	cmp	p7, r11
	beq	34f

32: /* main branch */

#ifdef L3_CACHE_ERROR_INJECTION
	xor	p7, p7, p7
	stw	p7, CPCERRINJCTL(0)(r11)
	sync
	isync
#endif /* L3_CACHE_ERROR_INJECTION */

	lwz	p7, CPCERRDET(0)(r11)
	cmpwi	p7, 0
	beq	31b

	mr	p2, p7
	
	/* clear error detect (w1c) */
	stw	p2, CPCERRDET(0)(r11)
	
	/* get more information */
	lwz	p3, CPCERRADDR(0)(r11)
	lwz	p4, CPCERRATTR(0)(r11)
	lwz	p5, CPCCAPTDATAHI(0)(r11)
	lwz	p6, CPCCAPTDATALOW(0)(r11)
	lwz	p7, CPCCAPTECC(0)(r11)

	/* clear error attribute register */
	xor 	p0, p0, p0
	stw	p0, CPCERRATTR(0)(r11)

	/* all L3 errors are handled the same: disable/invalidate */
	/* disable */
	sync
	isync
	stw	p0, CPCCSR0(0)(r11)
	isync

	/* invalidate */
	lis	p0, HI(1 << (31 - CPCCSR0_CPCFI_BIT))
	sync
	isync
	stw	p0, CPCCSR0(0)(r11)
	isync

33: /* invalidate loop */	
	lwz	p0, CPCCSR0(0)(r11)
	isync
	andis.	p0, p0, HI(1 << (31 - CPCCSR0_CPCFI_BIT))	
	bne	33b

	/* convert value in r11 to CPC1 / 2 differentiator */
	rlwinm.	r11, r11, 20, 31, 31
	
	/* log the error */
	bne	331f
	lis	p1, HIADJ(sysL3CacheErrorMsg1)
	lwz	p1, LO(sysL3CacheErrorMsg1)(p1)
	b	23f
331:
	lis	p1, HIADJ(sysL3CacheErrorMsg2)
	lwz	p1, LO(sysL3CacheErrorMsg2)(p1)
	b	23f
	
34:	/* no errors were caught, restore r11 and continue with other caches */
	mfspr	r11, SPRG6_R

2:
#endif /* INCLUDE_L3_CACHE_ERROR_RECOVERY */

#ifdef INCLUDE_L2_CACHE_ERROR_RECOVERY

	/*
	 * Check for L2 cache errors.  If any exist, do what is necessary
	 * to quiesce the L2 cache, log the error, and jump to the boot
	 * monitor.  It is assumed that L2 cache errors are not recoverable.
	 */
	mfspr	p7, MCSR
	rlwinm. p7, p7, 0, _PPC_MCSR_BIT_BSL2_ERR, _PPC_MCSR_BIT_BSL2_ERR
	beq	1f

	mtspr	MCSR, p7

	mfspr	p2, L2ERRDET

	/* clear error detect (w1c) */
	mtspr	L2ERRDET, p2
	
	/* get more information */
	mfspr	p3, L2ERRADDR
	mfspr	p4, L2ERRATTR
	mfspr	p5, L2CAPTDATAHI
	mfspr	p6, L2CAPTDATALO
	mfspr	p7, L2CAPTECC

	/* clear error attribute register */
	xor 	p0, p0, p0
	mtspr	L2ERRATTR, p0

	/* all L2 errors are handled the same: disable/invalidate */
	/* disable */
	sync
	isync
	mtspr	L2CSR0, p0
	isync

	/* invalidate */
	lis	p0, HI(1 << (31 - P4080_L2CSR0_L2FI_BIT))
	sync
	isync
	mtspr	L2CSR0, p0
	isync

21:	mfspr	p0, L2CSR0
	isync
	andis.	p0, p0, HI(1 << (31 - P4080_L2CSR0_L2FI_BIT))	
	bne	21b

	/* log the error */
	lis	p1, HIADJ(sysL2CacheErrorMsg)
	lwz	p1, LO(sysL2CacheErrorMsg)(p1)
#endif /* INCLUDE_L2_CACHE_ERROR_RECOVERY */

#if defined(INCLUDE_L2_CACHE_ERROR_RECOVERY) || \
    defined(INCLUDE_L3_CACHE_ERROR_RECOVERY)
	/* common exit for L2 and L3 error handlers */
	/* p1 = format string, p2-p7 = values */
23:
        /* can't use absolute branches with LMLA!=0 (AMP) */
	lis	p0, HI(sprintf)
	ori	p0, p0, LO(sprintf)
	mtctr	p0
	lis	p0, HIADJ(sysExcMsg)
	lwz	p0, LO(sysExcMsg)(p0)
	bctrl	/* sprintf */
	lis	p2, HIADJ(sysExcMsg)
	lwz	p1, LO(sysExcMsg)(p2)
	add	p1, p1, p0
	stw	p1, LO(sysExcMsg)(p2)

	/* jump to the boot monitor */
        /* can't use absolute branches with LMLA!=0 (AMP) */
	lis	p0, HI(sysToMonitor)
	ori	p0, p0, LO(sysToMonitor)
	mtctr	p0

	li	p0, BOOT_NORMAL
	bctr	/* sysToMonitor */
1:
#endif /* INCLUDE_L2_CACHE_ERROR_RECOVERY||INCLUDE_L3_CACHE_ERROR_RECOVERY */

#ifdef INCLUDE_L1_CACHE_ERROR_RECOVERY
	/* Check for L1 cache errors. */

	mfspr	p7, MCSR

	/* In e500mc, MCSR is w1c, so mask off all bits of MCSR except
	 * ICPERR and DCPERR, and write these bits back to clear them
	 * if they are set.
	 */
	rlwinm. p7, p7, 0, _PPC_MCSR_BIT_ICPERR, _PPC_MCSR_BIT_DCPERR
	beq	11f
	mtspr	MCSR, p7

#ifdef INCLUDE_SHOW_ROUTINES
	lwz	p7, SDA21(sysL1CacheErrorCount)
	addi	p7, p7, 1
	stw	p7, SDA21(sysL1CacheErrorCount)
#endif /* INCLUDE_SHOW_ROUTINES */

	/*
	 * Any remaining MCSR bits represent non-recoverable conditions
	 */
11:
	mfspr	p7, MCSR
	/* In e500mc, some bits of MCSR are informational only, and do not
	 * indicate an error independently of other bits.  Avoid vectoring
	 * to the machine check handler if only these bits are set.
	 */
	rlwinm.	p7, p7, 0, _PPC_MCSR_BIT_LDG + 1, _PPC_MCSR_BIT_MAV - 1	
	bne     0f		/* normal machine check */
	mfspr   p7, SPRG5_R
	mtcr	p7
	mfspr   p7, SPRG4_R
	isync

	rfmci   
#endif /* INCLUDE_L1_CACHE_ERROR_RECOVERY */
/* normal machine check handler */
0:     
	mfspr   p7, SPRG5_R
	mtcr	p7
	mfspr   p7, SPRG4_R
	ba	0x0			/* will be replaced by install */
FUNC_END(sysCacheErrorHandler)
FUNC_LABEL(sysCacheErrorHandlerEnd)
