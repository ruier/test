/* 20bsp.cdf - BSP-specific component descriptor file */

/*
 * Copyright (c) 2009-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,15aug12,swu  Add INCLUDE_SIO_UTILS (WIND00366387)
01g,17jun11,agf  Make CONTROL_PLANE_CORE param of DRV_RESOURCE_QORIQFMAN
01f,02jun11,agf  Add enable params for openPic timer groups
01e,03dec10,kab  Remove DRV_SIO_NS16550 from require comp's
01d,23jul10,wap  Add parameter to select control plane core
01c,12jun10,rab  Add smp guest support
01b,26feb10,wap  Add INCLUDE_VXCPULIB component
01a,14dec09,to   created
*/

/*
DESCRIPTION
This file contains descriptions for this BSP components.  It
adds definitions for the power management modes supported by
the BSP.
*/

Bsp	wrVbP4080 {
	NAME		Wind River Virtual Board P4080 board support package
	CPU		PPC32
	MP_OPTIONS      SMP
	ENDIAN		big
	FP		soft
	GUEST_OS_OPTION	REQUIRED
	REQUIRES	INCLUDE_KERNEL \
			INCLUDE_VXCPULIB \
                        INCLUDE_SIO_UTILS \
			INCLUDE_VXBUS \
			DRV_INTCTLR_PPC \
			DRV_INTCTLR_VB \
			DRV_TIMER_VB
}

Parameter VX_SMP_NUM_CPUS {
        Name		Number of CPUs for SMP
	TYPE		UINT
	DEFAULT		4
}

Component DRV_TIMER_OPENPIC {
	CFG_PARAMS	+=  ENABLE_TIMER_A \
			    ENABLE_TIMER_B
}

Parameter ENABLE_TIMER_A {
	NAME		Enable openPic Timer A
	TYPE		BOOL
	DEFAULT		TRUE
}

Parameter ENABLE_TIMER_B {
	NAME		Enable openPic Timer B
	TYPE		BOOL
	DEFAULT		FALSE
}

Component INCLUDE_AUX_CLK {
	REQUIRES += DRV_TIMER_OPENPIC
}

Component INCLUDE_TIMESTAMP {
	REQUIRES += DRV_TIMER_OPENPIC
}

Parameter RAM_HIGH_ADRS {
	NAME		vxWorks bootrom load address
	DEFAULT		0x01000000
}

Parameter RAM_LOW_ADRS {
	NAME		Runtime kernel load address
	DEFAULT		0x00010000
}

Parameter LOCAL_MEM_LOCAL_ADRS {
	NAME		Runtime kernel lower memory address
	DEFAULT		0x00000000
}

Component DRV_RESOURCE_QORIQFMAN {
	CFG_PARAMS	+=  CONTROL_PLANE_CORE
}

Parameter CONTROL_PLANE_CORE {
        Name		Defines which guest core initializes the FMANs
	TYPE		INT
	DEFAULT		-1
}
