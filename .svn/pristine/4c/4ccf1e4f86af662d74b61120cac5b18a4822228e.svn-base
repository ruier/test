/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2003-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01u,13jan10,cwl  Remove the dummy function aimMmuShowInit() from the component
                 INCLUDE_AIM_MMU_SHOW. (WIND00184839)
01t,12aug09,cww  Updated aimMmuShowInit location in usrRoot
01s,04sep08,gls  divided task hook component
01r,26aug08,jpb  Removed LAYER from components.
01q,29nov07,dtr  Change default DATA_SEG_AKLIGN.
01p,26apr06,ajc	 Added INCLUDE_ANSI_STRCAT and INCLUDE_ANSI_STRLEN for
		 mmuPpcLib.c (SPR #119277)
01o,23feb06,h_k  removed INCLUDE_MMU_ARCH. (SPR #118027)
01n,13oct05,yvp  INCLUDE_HW_SP gets pulled in for ANSI_STRTOD and ANSI_ATOF.
01m,06oct05,h_k  changed INCLUDE_WHEN to _REQUIRES attribute for HW_FP. (SPR
                 #113387) also added PPC60X specific POOL component. (SPR
                 #112394) added FP_TYPE as a _REQUIRES comp for HW_FP.
                 (SPR #113210)
01l,19sep05,mil  Added INCLUDE_MMU_ARCH_COMMON_ALIB.
                 Removed INCLUDE_AIM_CACHE_PPC60X.
01k,14sep05,h_k  updated HW_FP. (SPR #112320) and COPROCESSOR.
01j,26aug05,mil  Added source build support for MMU and cache.
01i,22apr05,dr   SPR108100 - adding aimFppLib.o and fppArchLib.o to INCLUDE_HW_FP
01h,09mar05,pch  adjust for PPC32 builds
01g,17dec04,to   PPC970 does not use AIM MMU.
01f,10nov04,dtr  SPR 103997.
01e,30sep04,rlg  fix for SPR 100750 change INCLUDE_WHEN to REQUIRES in
                 INCLUDE_COPROCESSOR, for future scaling issues.
01d,29sep04,dtr  Add AIM MMU show routines.SPR 101412
01c,18aug04,dtr  Pull in conditional AIM folder.
01b,03sep03,dbt  Added _INIT_ORDER directory to INCLUDE_COPROCESSOR component.
01a,07jul03,pch  separated from 20vxWorks.cdf vn 01a

DESCRIPTION

This file contains updated descriptions for some vxWorks components
defined in 00bsp.cdf. It updates those definitions with new ones needed
to support the coprocessor abstraction.
*/

Component INCLUDE_COPROCESSOR {
	NAME		coprocessor
	SYNOPSIS	generalized coprocessor support
	SOURCE	        $(TGT_DIR)/src/os
	MODULES		coprocLib.o
	INIT_RTN	coprocLibInit();
	CONFIGLETTES	usrCoprocs.c
	_CHILDREN	FOLDER_PERIPHERALS
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_TASK_CREATE_HOOKS \
			INCLUDE_TASK_SWAP_HOOKS   \
			INCLUDE_MEM_MGR_FULL	/* for calloc */
	_INIT_ORDER	usrIosCoreInit
}

Component INCLUDE_HW_FP {
	NAME		hardware fpp support
	SYNOPSIS	Architecture common modules for hardware floating point
	SOURCE		{$(TGT_DIR)/src/aim aimFppLib.o} \
			{$(TGT_DIR)/src/arch/ppc {fppALib.o fppArchLib.o}}
	INIT_RTN	
	HDR_FILES	
	REQUIRES	INCLUDE_COPROCESSOR
#if (defined diab) || (defined gnu)
	_REQUIRES	INCLUDE_ANSI_TIME   \
			INCLUDE_ANSI_ATOF   \
			INCLUDE_ANSI_STRTOD \
			INCLUDE_FP_TYPE
#endif	/* diab || gnu */
}

#if ( !defined PPC603 && !defined PPC604 && !defined PPC970 \
   && !defined _ppc603 && !defined _ppc604 && !defined _ppc970 \
   && !defined _ppc603_83xx && !defined _ppc603_g2le && !defined _ppc604_745x)
Folder  FOLDER_AIM_CONFIG {
	NAME		AIM MMU configuration 
	CHILDREN	INCLUDE_AIM_MMU_MEM_POOL_CONFIG \
			INCLUDE_AIM_MMU_PT_PROTECTION \
			FOLDER_MMU_OPTIMIZE	
}

Component INCLUDE_AIM_MMU_SHOW {
            NAME            AIM MMU Show Routines
            MODULES         aimMmuShow.o
            _CHILDREN       FOLDER_SHOW_ROUTINES
            REQUIRES        INCLUDE_MMU_BASIC
            LINK_SYMS       aimMmuMapShow
}

Parameter DATA_SEG_ALIGN {
	NAME		kernel data segment alignment
	_CFG_PARAMS	INCLUDE_MEMORY_CONFIG
	DEFAULT		(INCLUDE_LOCK_TEXT_SECTION)::(0x00040000) \
			1
}
#endif 	/* !60x && !970 */

Component INCLUDE_AIM_CACHE {
        NAME            AIM Cache Library
	SYNOPSIS	PPC common AIM cache library
        SOURCE          $(TGT_DIR)/src/aim
	MODULES		aimCacheLib.o
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_CACHE_ARCH {
	NAME		PPC common cache library
	SYNOPSIS	PPC common cache library
	SOURCE          $(TGT_DIR)/src/arch/ppc
	MODULES		cacheArchSysctl.o
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_CACHE_ARCH_PPC60X {
        NAME            PPC60x cache function libraries
        SYNOPSIS        PPC60x cache function libraries
        SOURCE          $(TGT_DIR)/src/arch/ppc
	MODULES		cacheAim60xLib.o cacheAim60xALib.o
        REQUIRES        INCLUDE_CACHE_ARCH        \
                        INCLUDE_AIM_CACHE         \
                        INCLUDE_ANSI_MEMSET
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_AIM_MMU_BASIC {
        NAME            AIM MMU basic configuration
        SYNOPSIS        AIM MMU basic configuration
        SOURCE          $(TGT_DIR)/src/aim
	MODULES		aimMmuBaseLib.o
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_MMU_ARCH_COMMON_ALIB {
        NAME            PPC MMU common assembly function library
        SYNOPSIS        PPC MMU common assembly function library
        SOURCE          $(TGT_DIR)/src/arch/ppc
	MODULES		mmuPpcALib.o
        _CHILDREN       FOLDER_NOT_VISIBLE
}                                                           

Component INCLUDE_MMU_ARCH_PPC60X {
        NAME            PPC60x MMU function libraries
        SYNOPSIS        PPC60x MMU function libraries
        SOURCE          $(TGT_DIR)/src/arch/ppc
	MODULES		mmuPpcLib.o
        REQUIRES        INCLUDE_MMU_ARCH_COMMON_ALIB \
                        INCLUDE_AIM_MMU_BASIC \
                        SELECT_MEM_ALLOC      \
                        INCLUDE_ANSI_MEMCPY   \
                        INCLUDE_BMPLIB	\
			INCLUDE_ANSI_STRCAT \
			INCLUDE_ANSI_STRLEN
        _CHILDREN       FOLDER_NOT_VISIBLE
}                                                         

Component INCLUDE_POOL_ARCH_PPC60X {
	NAME		PPC60x pool allocation abstraction
	SYNOPSIS	hidden component to drag poolLib for PPC60x MMU
	REQUIRES	INCLUDE_POOL		\
			INCLUDE_MMU_ARCH_PPC60X	\
			INCLUDE_MEM_MGR_BASIC
	INCLUDE_WHEN	INCLUDE_MMU_ARCH_PPC60X	\
			INCLUDE_MEM_MGR_BASIC
	_CHILDREN	FOLDER_NOT_VISIBLE
}
