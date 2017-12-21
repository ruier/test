/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2006-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,24nov10,mze  add new VIP_BOOT parameter
01i,02jun10,elp  64 bits support.
01h,18feb10,mze  mod to MP_OPTIONS
01g,03jun09,elp  moved INCLUDE_VXSIM_IFCONFIG component after usrNetworkInit
                 move.
01f,29oct08,dcc  removed AMP from MP_OPTIONS
01e,16oct07,dbt  Added FP attributes (CQ:WIND107552).
01d,29aug07,jmg  Added VX_SMP_NUM_CPUS
01c,19jul07,ebh  Add AMP advertising option
01b,19jul07,pee  added MP_OPTIONS
01a,16jul06,pee  created

*/

Bsp simpc {
	CPU		SIMNT
	DATA_MODEL	ILP32 LP64
	MP_OPTIONS	UP SMP
	VIP_BOOT        REQUIRED
	FP		hard
}

/*
 * VX_SMP_NUM_CPUS is a SMP parameter only and only available for SMP 
 * builds. Due to a limitation of the project tool at the time this 
 * parameter is created where the tool can not recognize the ifdef SMP 
 * selection, this parameter is set up such that _CFG_PARAMS is not 
 * specified here. In the 00vxWorks.cdf file, where the parameter 
 * VX_SMP_NUM_CPUS is defined, the _CFG_PARAMS is specified only for 
 * VxWorks SMP. Hence the redefining of VX_SMP_NUM_CPUS here should only 
 * override the value and not the rest of the properties. And for UP, the 
 * parameter is ignored since the parameter is not tied to any component 
 * (_CFG_PARAMS is not specified).
 */
 
Parameter VX_SMP_NUM_CPUS {
	NAME		Number of CPUS to be enabled for SMP
	TYPE		UINT
	DEFAULT		2
}

/* BSP specific component to initialize devices specified through -ni option */

Component INCLUDE_VXSIM_IFCONFIG {
	NAME            initialize VxSim additional network interfaces
	_INIT_ORDER     usrNetworkInit
	INIT_AFTER      INCLUDE_IPCOM
	INIT_BEFORE     usrNetAppInit
	REQUIRES        INCLUDE_IPWRAP_IFCONFIG \
			INCLUDE_IPWRAP_IPPROTO \
			INCLUDE_IPCOM
	_REQUIRES       INCLUDE_END
	INIT_RTN        sysNetIfConfig ();
}
