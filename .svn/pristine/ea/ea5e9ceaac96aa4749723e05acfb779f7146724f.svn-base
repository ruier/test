/* 02mm.cdf - Memory Management component description file */

/*
 * Copyright (c) 2003-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01x,19jul13,pcs  Change the prototype of memPartCacheLibInit
01w,11jul13,pcs  Add Components INCLUDE_MEM_PART_CACHE and 
                 INCLUDE_MEM_PART_CACHE_SHOW
01v,05may13,pcs  Add support for SMP-optimized memory allocator.
01u,08sep10,pcs  Add Parameter GLOBAL_VIRT_MEM_REGION_SIZE to represent the
                 size of the virtual memory region to be reserved by the
                 addressSpace allocator for usage as global virtual memory.
01v,16sep10,s_s  parameter KERNEL_HEAP_SIZE now uses FREE_MEM_START_ADRS to
                 get its value (CQ: WIND00214548)
01u,09sep10,j_l  Parameterize DMA32_RESERVED_MEM (Fix for WIND00230744)
                 Rename DMA32_RESERVED_MEM to DMA32_HEAP_SIZE
01t,26aug10,gls  adapted vmInvTblLib for LP64
01s,09jun10,pcs  Add devMemFsLib.o to Component INCLUDE_DEVMEM.
01r,18may10,pcs  Add support for device memory object.
01q,12mar10,jpb  Temporary removal of INCLUDE_VM_INVTBL for lp64.
01p,19feb10,zl   added INCLUDE_POSIX_SHM_API
01o,08jan10,zl   added INCLUDE_MMAP. Moved init order specifications 
                 to 00vxWorks.cdf.
01n,08dec09,cww  Added INCLUDE_TASK_STACK_ALLOC
01m,16sep09,h_k  added INCLUDE_CACHE_DMA32_LIB.
01l,11aug09,cww  Delayed pgMgrLibInit
01k,29jun09,zl   support for address space manager without RTPs. Added
                 32-bit and 64-bit specific modules.
01j,23apr08,jpb  Defect #108859.  Changed ordering of shared memory.
01i,29mar07,pcm  io footprint reduction work
01h,01jan06,zl   added INCLUDE_MMAP_SHOW
01g,25oct05,zl   added INCLUDE_POSIX_SHM components.
01f,22nov04,zl   removed module definition for KH components
01e,20sep04,tam  added INCLUDE_PROTECT_TASK_STACK &
                 INCLUDE_TASK_STACK_NO_EXEC
01d,13sep04,zl   private header files changes.
01c,09jun04,zl   added INCLUDE_VM_RTP; added _CHILDREN definitions.
01b,26feb04,tam  added missing defintions to the SHOW components
01a,23oct03,tam  written
*/

/*
DESCRIPTION
This file contains descriptions for Memory Management components.
*/


/* address space allocator components */

Component INCLUDE_ADR_SPACE_LIB {
	NAME		System Address Space Allocator 
	SYNOPSIS	System Address Space Allocator
	_CHILDREN	FOLDER_OS
#ifdef _WRS_CONFIG_LP64
	MODULES		adrSpaceLib.o			\
			adrSpaceLib64.o
#else
	MODULES		adrSpaceLib.o			\
			adrSpaceLib32.o
	CFG_PARAMS	KERNEL_HEAP_SIZE                \
                        GLOBAL_VIRT_MEM_REGION_SIZE
#endif
	REQUIRES	INCLUDE_MMU_BASIC		\
			INCLUDE_PG_POOL_LIB		\
			INCLUDE_PG_POOL_VIRT_LIB	\
			INCLUDE_PG_POOL_PHYS_LIB
	HDR_FILES	private/adrSpaceLibP.h
}

#ifndef _WRS_CONFIG_LP64
Parameter KERNEL_HEAP_SIZE {
        NAME            Size of the kernel heap in bytes
        SYNOPSIS        Size of the kernel heap in bytes. Used only if RTP \
                        support is included.
        TYPE 		uint
        DEFAULT         ((((unsigned int) (sysMemTop() - FREE_MEM_START_ADRS) / 3) * 2))
}
Parameter GLOBAL_VIRT_MEM_REGION_SIZE {
        NAME            Size of global virtual memory region
        SYNOPSIS        Represents the size of the virtual memory to be \
                        reserved for usage as global virtual memory.
        TYPE 		uint
        DEFAULT         (0x800000)
}
#endif

Component INCLUDE_ADR_SPACE_SHOW {
        NAME            Address Space Allocator Show Routines
        SYNOPSIS        Address Space Allocator Show Routines: display \
			information on the system address space allocator.
	_CHILDREN	FOLDER_SHOW_ROUTINES
	REQUIRES        INCLUDE_ADR_SPACE_LIB
	INIT_RTN	adrSpaceShowInit();
        MODULES         adrSpaceShow.o
	INCLUDE_WHEN	INCLUDE_RTP_SHOW
	_INIT_ORDER	usrShowInit
	INIT_BEFORE	INCLUDE_RTP_SHOW
}

Component INCLUDE_MMAP {
	NAME		Memory mapping
	SYNOPSIS	Memory mapping and unmapping routines
	MODULES		mmanLib.o
	PROTOTYPE	void mmanLibInit (void);
	INIT_RTN	mmanLibInit ();
	REQUIRES	INCLUDE_BASE_KERNEL \
			INCLUDE_PG_MGR_BASIC
	_CHILDREN	FOLDER_OS
}

/* page manager components */

Component INCLUDE_PG_MGR_BASIC {
	NAME		Basic Page Manager
	SYNOPSIS	Basic Page Manager Library
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_ADR_SPACE_LIB
	INIT_RTN	pgMgrBaseLibInit();
	MODULES		pgMgrBaseLib.o
	HDR_FILES	private/pgMgrLibP.h
}

Component INCLUDE_PG_MGR_LIB {
	NAME		Page Manager Library
	SYNOPSIS	Page Manager Library
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES	INCLUDE_PG_MGR_BASIC \
			INCLUDE_VM_RTP
	_REQUIRES	INCLUDE_RTP
	INIT_RTN	pgMgrLibInit();
	MODULES		pgMgrLib.o
	HDR_FILES	private/pgMgrLibP.h
}

Component INCLUDE_PG_MGR_SHOW {
        NAME            Page Manager Show Routines
        SYNOPSIS        Page manager show routines: display information on \
			page managers.
	_CHILDREN	FOLDER_NOT_VISIBLE
	REQUIRES        INCLUDE_PG_MGR_LIB
	INIT_RTN	pgMgrShowInit();
        MODULES         pgMgrShow.o
	INCLUDE_WHEN	INCLUDE_RTP_SHOW
	_INIT_ORDER	usrShowInit
	INIT_BEFORE	INCLUDE_RTP_SHOW
}


/* page pool components */

Component INCLUDE_PG_POOL_LIB {
	NAME		Generic Page Pool Allocator
	SYNOPSIS	Generic Page Pool Allocator
	_CHILDREN	FOLDER_NOT_VISIBLE
	INIT_RTN	pgPoolLibInit();
	MODULES		pgPoolLib.o
	HDR_FILES	private/pgPoolLibP.h
}

Component INCLUDE_PG_POOL_VIRT_LIB {
	NAME		Page Pool Allocator for Virtual Space
	SYNOPSIS	Page Pool Allocator for Virtual Space
	_CHILDREN	FOLDER_NOT_VISIBLE
	INIT_RTN	pgPoolVirtLibInit();
	MODULES		pgPoolVirtLib.o
	HDR_FILES	private/pgPoolVirtLibP.h
}

Component INCLUDE_PG_POOL_PHYS_LIB {
	NAME		Page Pool Allocator for Physical Space
	SYNOPSIS	Page Pool Allocator for Physical Space
	_CHILDREN	FOLDER_NOT_VISIBLE
	INIT_RTN	pgPoolPhysLibInit();
	MODULES		pgPoolPhysLib.o
	HDR_FILES	private/pgPoolPhysLibP.h
}

Component INCLUDE_PG_POOL_SHOW {
        NAME            Generic Page Pool Allocator Show Routines
        SYNOPSIS        Generic page pool allocator show routines: display \
			information on page pools.
	_CHILDREN	FOLDER_NOT_VISIBLE
        INIT_RTN        pgPoolShowInit();
	REQUIRES        INCLUDE_PG_POOL_LIB
        MODULES         pgPoolShow.o
	INCLUDE_WHEN	INCLUDE_RTP_SHOW
	_INIT_ORDER	usrShowInit
	INIT_BEFORE	INCLUDE_RTP_SHOW
}

Component INCLUDE_TASK_STACK_ALLOC {
	NAME		taskStackAlloc and taskStackFree routines
	SYNOPSIS	taskStackAlloc and taskStackFree routines
	_CHILDREN	FOLDER_NOT_VISIBLE
	INIT_RTN	taskMemLibInit ();
	REQUIRES	INCLUDE_MMU_BASIC
	HDR_FILES	private/taskMemLibP.h
}

Component INCLUDE_PROTECT_TASK_STACK {
	NAME		enable guard pages for kernel task stacks
	SYNOPSIS	insert underflow and overflow guard pages to kernel task stacks
	_CHILDREN	FOLDER_KERNEL
	INIT_RTN	taskStackGuardPageEnable();
	REQUIRES        INCLUDE_TASK_STACK_ALLOC
	HDR_FILES	private/taskMemLibP.h
}

Component INCLUDE_TASK_STACK_NO_EXEC {
	NAME		enable non-executable kernel task stacks
	SYNOPSIS	enable non-executable kernel task stacks
	_CHILDREN	FOLDER_KERNEL
	INIT_RTN	taskStackNoExecEnable();
	REQUIRES        INCLUDE_TASK_STACK_ALLOC
	HDR_FILES	private/taskMemLibP.h
}

Component INCLUDE_VM_RTP {
	NAME		VM support for RTPs
	_CHILDREN	FOLDER_NOT_VISIBLE
	MODULES		vmLib.o
	REQUIRES        INCLUDE_MMU_BASIC
	HDR_FILES	vmLib.h
}

Component INCLUDE_VM_INVTBL {
	NAME		VM inverse page table support
	_CHILDREN	FOLDER_NOT_VISIBLE
	_REQUIRES	INCLUDE_SHARED_DATA
	MODULES		vmInvTblLib.o
	REQUIRES        INCLUDE_MMU_BASIC
}

Component INCLUDE_POSIX_MAPPED_FILES {
        NAME            POSIX Memory Mapped Files
        SYNOPSIS        POSIX Memory Mapped Files
	_CHILDREN	FOLDER_OS
	REQUIRES        INCLUDE_MMAP
	PROTOTYPE	extern STATUS mmanFdLibInit (void);
	INIT_RTN	mmanFdLibInit();
        MODULES         mmanFdLib.o
}

Component INCLUDE_POSIX_SHM {
        NAME            POSIX Shared Memory Objects
        SYNOPSIS        POSIX Shared Memory Objects. This component has to \
			be included if POSIX shared memory objects are used \
			in either user space applcations or kernel applications
	_CHILDREN	FOLDER_OS
	REQUIRES        INCLUDE_POSIX_MAPPED_FILES
	PROTOTYPE	extern STATUS shmFsLibInit (const char *);
	INIT_RTN	shmFsLibInit (SHM_DEV_NAME);
        MODULES         shmFsLib.o
	CFG_PARAMS	SHM_DEV_NAME
}

Parameter SHM_DEV_NAME {
        NAME            SHM device name
        SYNOPSIS        Name of POSIX shared memory object device
        TYPE 		string
        DEFAULT         "/shm"
}

Component INCLUDE_POSIX_SHM_API {
        NAME            POSIX Shared Memory Object API
        SYNOPSIS        POSIX Shared Memory Object API (shm_open and \
			shm_unlink) in kernel space
	_CHILDREN	FOLDER_OS
	REQUIRES        INCLUDE_POSIX_SHM
	PROTOTYPE	extern STATUS shmLibInit (void);
	INIT_RTN	shmLibInit();
        MODULES         shmLib.o
}

Component INCLUDE_DEVMEM {
        NAME            device Memory Objects
        SYNOPSIS        device Memory Objects. This component has to be \
                        included if device memory objects are used \
                        in either user space applcations or kernel applications
        _CHILDREN       FOLDER_OS
        REQUIRES        INCLUDE_POSIX_MAPPED_FILES
        PROTOTYPE       extern STATUS devMemLibInit (const char *);
        INIT_RTN        devMemLibInit (DEVMEM_DEV_NAME);
        MODULES         devMemLib.o devMemFsLib.o
        CFG_PARAMS      DEVMEM_DEV_NAME
}

Parameter DEVMEM_DEV_NAME {
        NAME            DEVMEM device name
        SYNOPSIS        Name of device memory object device
        TYPE            string
        DEFAULT         "/devm"
}


#ifdef	_WRS_CONFIG_LP64
Component INCLUDE_CACHE_DMA32_LIB {
	NAME		cache DMA partition manager for 32-bit physical memory
	SYNOPSIS	cache DMA partition manager for 32-bit physical memory
	_CHILDREN	FOLDER_MEMORY
	REQUIRES	INCLUDE_ADR_SPACE_LIB \
			INCLUDE_MMU_BASIC \
			INCLUDE_MEM_MGR_BASIC
	PROTOTYPE	extern STATUS cacheDma32PartInit (void);
	INIT_RTN	cacheDma32PartInit ();
	MODULES		cacheDma32Lib.o
	CFG_PARAMS	DMA32_HEAP_SIZE
	_INIT_ORDER	usrRoot
	INIT_BEFORE	INCLUDE_SYSCLK_INIT
	INIT_AFTER	INCLUDE_KERNEL_SYSTEM_HEAP
}

Parameter DMA32_HEAP_SIZE {
        NAME            DMA32 heap reserved Memory
        SYNOPSIS        DMA32 heap reserved memory size ( bytes)
        DEFAULT         0x04000000
}
#endif	/* _WRS_CONFIG_LP64 */

Component INCLUDE_MAPPED_FILES_SHOW {
	NAME		Show routines for memory mapped objects
        SYNOPSIS        Show routines for memory mapped objects
	_CHILDREN	FOLDER_SHOW_ROUTINES
	REQUIRES        INCLUDE_POSIX_MAPPED_FILES
	PROTOTYPE	extern STATUS mmanShowInit ();
	INIT_RTN	mmanShowInit();
        MODULES         mmanShow.o
	_INIT_ORDER	usrShowInit
	INIT_AFTER	INCLUDE_RTP_SHOW
}

Component INCLUDE_MEM_PART_CACHE {
        NAME            memory caching component
        SYNOPSIS        This component has to be included if task level memory \
                        caching of the system heap is desired in the kernel.
        _CHILDREN       FOLDER_OS
        MODULES         memPartCacheLib.o
        PROTOTYPE       extern STATUS memPartCacheLibInit (BOOL);
        REQUIRES        INCLUDE_MEM_MGR_BASIC
        INIT_RTN        memPartCacheLibInit (MEM_PART_CACHE_GLOBAL_ENABLE);
        CFG_PARAMS      MEM_PART_CACHE_GLOBAL_ENABLE
}

Component INCLUDE_MEM_PART_CACHE_SHOW {
        NAME            Memory Partition Cache Show Routines
        SYNOPSIS        Memory Partition Cache Show Routines
        MODULES         memPartCacheShow.o
        INIT_RTN        memPartCacheShowInit();
        _CHILDREN       FOLDER_SHOW_ROUTINES
        LINK_SYMS       memPartCacheShow
        REQUIRES        INCLUDE_MEM_PART_CACHE
        _INIT_ORDER     usrShowInit
}

Parameter MEM_PART_CACHE_GLOBAL_ENABLE
        {
        NAME            Enable memory caching
        SYNOPSIS        Enable's task level memory caching globally for all \
                        tasks.
        TYPE            BOOL
        DEFAULT         FALSE
        }

