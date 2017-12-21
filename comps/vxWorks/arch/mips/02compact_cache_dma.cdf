/* mips/02compact_cache_dma.cdf - cacheDmaMalloc optimization, MIPS settings */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,06mar09,pch  written

DESCRIPTION

This file provides MIPS-specific adjustments for cacheDmaMalloc optimization.
*/

/*
 * Few if any currently-supported MIPS systems are fully coherent,
 * so cacheDmaMalloc is not just malloc and its optimizer is useful.
 * This can be wrapped in a #ifndef when/if support for a fully
 * coherent MIPS part is added; see the PPC instance for an example.
 */

Component INCLUDE_CACHE_COMPACT_DMA_SUPPORT {
	INCLUDE_WHEN	INCLUDE_CACHE_SUPPORT
}
