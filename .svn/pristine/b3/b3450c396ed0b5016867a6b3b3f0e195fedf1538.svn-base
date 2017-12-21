/* sysL2Cache.s - L2 cache functions */

/* Copyright 2007 Tundra Semiconductor Corp. */
/* Copyright 1984-2002 Wind River Systems, Inc. */
/* Copyright 1996-1998 Motorola, Inc. */

/*
modification history
--------------------
01b,17may07,jjm  Created from sysL2BackCache.s (ibm_evb750cl)
01a,02sep05,avb  Created from sysL2BackCache.s (sp7447)
*/

/* includes */

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <cacheLib.h>
#include "config.h"
#include "sysL2Cache.h"


	/* globals */

	FUNC_EXPORT(sysL2crPut)
	FUNC_EXPORT(sysL2crGet)
	FUNC_EXPORT(sysL2CacheGlobalInv)
	FUNC_EXPORT(sysL2CacheDisable)
	FUNC_EXPORT(sysL2CacheEnable)
	FUNC_EXPORT(sysL2CacheSWFlush)
	FUNC_EXPORT(sysL2CacheInit)
	FUNC_EXPORT(sysL2CacheSetWT)
	FUNC_EXPORT(sysL2CacheSetWT)

	_WRS_TEXT_SEG_START
	
/*******************************************************************************
*
* sysL2crPut - write to L2CR register of Arthur CPU (SPR1017)
*
* This routine will write the contents of r3 to the L2CR (SPR 1017)
* register.
*
* SYNOPSIS
* \ss
* void sysL2crPut
*     (
*     ULONG dataOut
*     )
* \se
*
* RETURNS: N/A
*
* SEE ALSO: sysL2crGet()
*/

FUNC_BEGIN(sysL2crPut)
	mtspr	L2CR_REG,r3
	blr
FUNC_END(sysL2crPut)

/*******************************************************************************
*
* sysL2crGet - read from L2CR register of Arthur CPU (SPR1017)
*
* This routine will read the contents the L2CR (SPR 1017) register.
*
* SYNOPSIS
* \ss
* UINT sysL2crGet
*     (
*     void
*     )
* \se
*
* RETURNS: value of SPR1017 (in r3)
*
* SEE ALSO: sysL2crPut()
*/

FUNC_BEGIN(sysL2crGet)
	mfspr	r3,L2CR_REG
	blr
FUNC_END(sysL2crGet)

/*******************************************************************************
*
* sysL2CacheGlobalInv - globally invalidate L2 backside cache 
*
* This function reads the value of the l2cr register, disables
* the cache, before setting the global invalidate bit.  It waits
* for the L2CR[L2IP] bit is clear before returning to the caller.
* 
* SYNOPSIS
* \ss
* void sysL2CacheGlobalInv
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysL2CacheGlobalInv)

	isync
	sync
	mfspr   r3, L2CR_REG
	andis.  r3, r3, L2CR_DISABLE_MASK_U /* disable L2 cache */
	mtspr   L2CR_REG, r3

	sync

	oris    r3, r3, L2CR_GBL_INV_U	/* set global invalidate command */
	mtspr   L2CR_REG, r3
	sync

	/*
	 * Monitoring the L2CR[L2IP] bit to determine when the global 
	 * invalidation operation is completed.
	 */

invalidate_in_progress:
	mfspr   r3, L2CR_REG
	andi.   r3, r3, L2CR_IP
	bne     invalidate_in_progress

	sync
	mfspr   r3, L2CR_REG
	rlwinm  r3, r3, 0, 11, 9	/* zero out the invalidate bit */
	mtspr   L2CR_REG, r3
	sync
	blr

FUNC_END(sysL2CacheGlobalInv)

/*******************************************************************************
*
* sysL2CacheEnable - set and enable L2 backside cache
*
* This function set and enable L2 backside cache
* 
* SYNOPSIS
* \ss
* void sysL2CacheEnable
*     (
*     CACHE_TYPE cache         /@ type of L1 cache being enable @/
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysL2CacheEnable)
	sync
	cmpwi   r3, _DATA_CACHE
	beq     DCACHE

ICACHE:

#if  defined(USER_I_CACHE_ENABLE) && defined(USER_D_CACHE_ENABLE)
	b       RETOK
#elif defined(USER_I_CACHE_ENABLE)
	mfspr   r4, L2CR_REG

	mfspr	r9, PVR			/* read PVR             */
	rlwinm	r9,r9,16,16,31		/* shift down 16 bits    */
	cmpli	0,0,r9,CPU_TYPE_750FX	/* No L2 Instruction Only in 750FX */
	beq	L2Enable
	cmpli	0,0,r9,CPU_TYPE_750CL	/* No L2 Instruction Only in 750CL */
	beq	L2Enable

	ori     r4, r4, L2IO_PPC750
	mtspr   L2CR_REG, r4
	b       L2Enable
#else
	b       RETOK
#endif

DCACHE: 

#if  defined(USER_I_CACHE_ENABLE) && defined(USER_D_CACHE_ENABLE)
	b	L2Enable
#elif defined(USER_D_CACHE_ENABLE)
	mfspr   r4, L2CR_REG
	oris    r4, r4, L2DO_PPC750
	mtspr   L2CR_REG, r4
	b       L2Enable
#else
	b       RETOK
#endif

L2Enable:
	mfspr   r4, L2CR_REG
	oris    r4, r4, L2CR_EN_U
	mtspr   L2CR_REG, r4 
RETOK: 
	sync
	blr

FUNC_END(sysL2CacheEnable)

/*******************************************************************************
* sysL2CacheDisable - Disable the L2 backside cache
*
* The value of the l2cr register is read, the enable bit is 
* disabled and written back to the l2cr register.
*
* The following problem exists on all versions of the MPC7400 (MAX) processor and is
* not expected to be fixed.  A fix is targeted for revision 1.3 of the MPC7410
* (Nitro) processor.
*
* The following is the response from SPS
* regarding the problem:
*
* Overview:
*
* The MPC7400 can hang if the L2 is disabled during an outstanding instruction
* fetch.
*
* Detailed Description:
*
* The problem centers around the interaction of the instruction cache and the
* L2 cache as the L2 cache is disabled.  The scenario is as follows:
*
*  1. An ifetch misses in the icache and allocates a reload table entry
*  2. As the instructions return from the BIU they are forwarded around the
*     icache and dispatched as well as written to the IRLDQ.
*  3. One of these instruction is a mtspr targeting the L2CR.  This
*     instruction  disables the L2.
*  4. When all beats of data return to the IRLDQ, the IRLT arbitrates to
*     reload the L2.  Since the L2 is now disabled, it does not expect reload
*     requests from the IRLT.
*  5. The unexpected reload request is mishandled by the L2 and passed to the
*     BIU as an ifetch miss.
*
* Workaround:
*  1. Preload the code that disables the L2 into the instruction cache before
*     execution.  This requires the code be structured in such a way that the
*     instruction fetch be completed before the mtspr is executed.
*
* SYNOPSIS
* \ss
* void sysL2CacheDisable
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysL2CacheDisable)
	sync
	mfspr	r3, L2CR_REG		   	/* Read L2 control register */
	rlwinm  r3, r3, 0, 1, 31        	/* Disable L2 backside */
        sync

        b  preFetchL2DisableCode

codeIsPrefetched:
	mtspr   L2CR_REG, r3     
	sync
        isync

        b pastFetchL2DisableCode

preFetchL2DisableCode:
        sync
        isync
        b codeIsPrefetched

pastFetchL2DisableCode:

   /* Return to caller */

        blr
FUNC_END(sysL2CacheDisable)

/**************************************************************************
*
* sysL2CacheSWFlush - Software Flush for L2                     
*
* The software based routine, which loads block of memory to L1, equal 
* to size  of L2 RAM.   
* 
* SYNOPSIS
* \ss
* void sysL2CacheSWFlush
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/
        
FUNC_BEGIN(sysL2CacheSWFlush)
        sync

        lis	r3, HI((L2CACHE_SIZE * 2)/L1CACHE_ALIGN_SIZE)
	ori	r3, r3, LO((L2CACHE_SIZE * 2)/L1CACHE_ALIGN_SIZE)

        mtspr	CTR, r3
        lis	r4, HI(BUFFER_ADDR)
        ori	r4, r4, LO(BUFFER_ADDR)
        subi	r4,r4,(L1CACHE_ALIGN_SIZE)
        andi.	r5, r5, 0x0
        
FlushCache:
        lbzu	r5, L1CACHE_ALIGN_SIZE(r4)
        bdnz	FlushCache
        sync
        blr
FUNC_END(sysL2CacheSWFlush)

/**************************************************************************
*
* sysL2CacheInit - Initialize L2 Back Cache
*
* The routine initialize L2 Back Cache.
* 
* SYNOPSIS
* \ss
* void sysL2CacheInit
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/           
                
FUNC_BEGIN(sysL2CacheInit)
	mfspr   r3, L2CR_REG	    /* check if L2 cache is already enabled */
	andis.  r4, r3, L2CR_EN_U
	bne     l2backenable_done   /* exit if L2 is enabled */
	mflr    r7
	sync
	bl      sysL2CacheGlobalInv
	sync
	mfspr   r3, L2CR_REG
	oris    r3, r3, L2CR_CFG
	mtspr   L2CR_REG, r3
	sync
	isync
	mtlr    r7

l2backenable_done:
	blr

FUNC_END(sysL2CacheInit)

/**************************************************************************
*
* sysL2CacheSetWT - Sets L2 Cache in writeThrough mode.
*
* The value of the l2cr register is read, the writethrough bit is 
* set and written back to the l2cr register.
* 
* SYNOPSIS
* \ss
* void sysL2CacheSetWT
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysL2CacheSetWT)
	isync
	sync
	mfspr	r3, L2CR_REG
	oris	r3, r3, L2CR_WT
	mtspr	L2CR_REG, r3     
	sync
	blr
FUNC_END(sysL2CacheSetWT)        
