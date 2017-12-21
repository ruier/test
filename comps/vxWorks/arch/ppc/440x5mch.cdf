/* 440x5mch.cdf - PPC440x5 cpu components */

/* Copyright 2003-2004, Wind River Systems, Inc. */

/*
modification history
--------------------
01d,09mar05,pch  adjust for PPC32 builds
01c,24sep04,dtr  added mod history and removed logging dependency.
01b,29jul03,pch  added FOLDER_440X5_MACHINE_CHECK
01a,19jun03,pch  written
*/

/*
This cdf file defines the PPC440x5 cpu specific component units.
*/

#if ( defined _ppc440_x5 || ( defined PPC440 && defined _x5 ) )
Folder FOLDER_440X5_MACHINE_CHECK {
	NAME		440x5 Machine Check Handler
	SYNOPSIS	Machine Check Handling for 440x5 core
	_CHILDREN	FOLDER_MEMORY
	CHILDREN	INCLUDE_440X5_DCACHE_RECOVERY \
			INCLUDE_440X5_TLB_RECOVERY \
			INCLUDE_440X5_TLB_RECOVERY_MAX \
			INCLUDE_440X5_PARITY_RECOVERY \
			INCLUDE_440X5_MCH_LOGGER
	DEFAULTS	INCLUDE_440X5_DCACHE_RECOVERY \
			INCLUDE_440X5_TLB_RECOVERY \
			INCLUDE_440X5_TLB_RECOVERY_MAX \
			INCLUDE_440X5_PARITY_RECOVERY \
			INCLUDE_440X5_MCH_LOGGER
}

Component INCLUDE_440X5_DCACHE_RECOVERY {
	NAME		Data cache recovery
	SYNOPSIS	Make data cache parity errors recoverable
	REQUIRES	INCLUDE_440X5_PARITY_RECOVERY
}

Component INCLUDE_440X5_TLB_RECOVERY {
	NAME		TLB recovery
	SYNOPSIS	Make TLB parity errors recoverable
	REQUIRES	INCLUDE_440X5_PARITY_RECOVERY INCLUDE_MMU_BASIC
}

Component INCLUDE_440X5_TLB_RECOVERY_MAX {
	NAME		Maximum TLB recovery
	SYNOPSIS	Maximize TLB parity error recovery
	REQUIRES	INCLUDE_440X5_TLB_RECOVERY
}

Component INCLUDE_440X5_PARITY_RECOVERY {
	NAME		440x5 parity recovery enable
	SYNOPSIS	Sets the PRE bit in CCR0
	REQUIRES	INCLUDE_EXC_HANDLING
}

Component INCLUDE_440X5_MCH_LOGGER {
	NAME		440x5 Machine Check Logger
	SYNOPSIS	Causes the Machine Check handler to log recovered events
	CFG_PARAMS	_PPC_440X5_MCH_LOG_RTN
	REQUIRES	INCLUDE_EXC_HANDLING
}

Parameter _PPC_440X5_MCH_LOG_RTN {
	NAME		440x5 Machine Check logging function
	SYNOPSIS	Function to call, leave default value 1 for logMsg
	TYPE		FUNCPTR
	DEFAULT		(FUNCPTR)1
}

#endif	/* 440_x5 */
