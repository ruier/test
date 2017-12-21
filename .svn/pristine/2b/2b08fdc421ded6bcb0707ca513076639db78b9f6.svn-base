/* 01compact_cache_dma.cdf - component defns for cacheDmaMalloc optimization */

/*
 * Copyright (c) 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,09aug10,cww  Moved INCLUDE_CACHE_COMPACT_DMA_SUPPORT init to after
                 INCLUDE_KERNEL_COMMON_HEAP
01b,16apr10,jpb  Updated prototype for cacheDmaPoolAlloc.
01a,03mar09,pch  written

DESCRIPTION

This file defines components for cacheDmaMalloc optimization.
*/

Component INCLUDE_CACHE_COMPACT_DMA_SUPPORT {
	NAME		cacheDmaMalloc optimizations
	SYNOPSIS	Improve memory usage and performance of cacheDmaMalloc
	SOURCE		$(TGT_DIR)/src/os/mm
	MODULES		cacheDmaPool.o
	PROTOTYPE	STATUS cacheDmaPoolAlloc(UINT32, UINT32);
	CFG_PARAMS	CACHE_DMA_POOL_INITIAL_SIZE \
			CACHE_DMA_POOL_INCREMENT_SIZE
	INIT_RTN	cacheDmaPoolAlloc(CACHE_DMA_POOL_INITIAL_SIZE, \
					  CACHE_DMA_POOL_INCREMENT_SIZE);
	REQUIRES	INCLUDE_CACHE_SUPPORT INCLUDE_MEM_MGR_BASIC
	_CHILDREN	FOLDER_MEMORY
}

Parameter CACHE_DMA_POOL_INITIAL_SIZE {
	NAME		Initial size of cache-safe memory pool, in pages
	DEFAULT		0
}

Parameter CACHE_DMA_POOL_INCREMENT_SIZE {
	NAME		no. of pages to add to cache-safe memory pool when exhausted
	DEFAULT		1
}
