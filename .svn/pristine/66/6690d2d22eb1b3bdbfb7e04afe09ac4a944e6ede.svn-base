/* ppc/02compact_cache_dma.cdf - cacheDmaMalloc optimization, PPC settings */

/*
 * Copyright (c) 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,30apr11,syt  added PPC465 supported
01a,06mar09,pch  written

DESCRIPTION

This file provides PPC-specific adjustments for cacheDmaMalloc optimization.
*/

/*
 * Most PPC6xx (including 7xx/74xx/86xx) and 85xx are fully coherent, so
 * cacheDmaMalloc is just malloc and its optimizer should be omitted (by
 * default) to save space.  PPC403 is not coherent, but its cacheDmaMalloc
 * uses address aliases (due to absence of a supported MMU) so the optimizer
 * is not likely to be needed there either.  Thus the optimizer is included
 * by default only for PPC405, PPC440, and PPC860.
 */

#if defined(PPC405) || defined(PPC440) || defined(PPC860) || defined(PPC465) || defined(_ppc405) \
 || defined(_ppc440) || defined(_ppc440_x5) || defined(_ppc860) || defined(_ppc465)
Component INCLUDE_CACHE_COMPACT_DMA_SUPPORT {
	INCLUDE_WHEN	INCLUDE_CACHE_SUPPORT
}
#endif	/* PPC405 || PPC440 || PPC860 || PPC465 */
