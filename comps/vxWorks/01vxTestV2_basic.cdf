/* 01vxTestV2_basic.cdf - BSPVTS basic test suite test components */

/*
 * Copyright (c) 2006-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,24feb09,d_c  Reorg in preparation for internal test module
                 componentization
01k,04sep08,kab  Add BSP API component
01j,26aug08,jpb  Removed LAYER from components.
01i,09apr08,kab  Move OS tests
01h,01apr08,mdo  Adding more test components
01g,24mar08,mdo  Add tmAnsiSetjmp
01g,06mar08,d_c  Add tmTaskLib
01f,10oct07,pgh  Fix WIND00107603
01e,29aug07,jmt  Updated INCLUDE_TM_PINGLIB to match changes in test
01d,09mar07,mdo  Fix spelling of SYNOPSIS for vxtest V2
01c,28jun06,jmt  Fix definition of INCLUDE_TM_INTARCHLIB
01b,05jun06,jmt  Add BspVts folder and IntArchLib test
01a,30may06,jmt  Created from 01vxTestV2Test.cdf (01m,29mar06,slk)


DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'basic' test suite ($WIND_BASE/vxtest/src/config/basic.prm).
The 'basic' test suite tests basic BSP functionality, and consists mainly
of the test cases that comprised the pre-vxWorks 6.7 BSPVTS.

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/

Component               INCLUDE_TM_BSPAPI {
        NAME            BSP API conformance test module
        SYNOPSIS        This component verifies presence of required APIs
	PROTOTYPE	void tmBspApiInit(void);
	INIT_RTN	tmBspApiInit();
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/bsp/api
        MODULES         tmBspApi.o
}

Component               INCLUDE_TM_MODEL {
        NAME            sysModel test module
        SYNOPSIS        This component adds the sysModel() API Test Module
	PROTOTYPE	void tmModelInit(char * expSysModel);
	INIT_RTN	tmModelInit(SYS_MODEL);
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/bsp/model
        MODULES         tmModel.o
	CFG_PARAMS      SYS_MODEL
}

/* 
 * NOTE: The following default for SYS_MODEL is coupled to the
 * tmModel:sysModelTest test case.  It is used to detect that a BSP
 * has not defined SYS_MODEL.
 */
Parameter		SYS_MODEL {
	DEFAULT		"SYS_MODEL not defined"
}

/*
* The following component definition is for the system clock test module.
*/

Component 		INCLUDE_TM_SYSCLOCK {
	NAME		System clock test module
	SYNOPSIS	This component adds the system clock test module
	REQUIRES	INCLUDE_VXTESTV2 \
			INCLUDE_SYSCLK_INIT
	SOURCE		$(WIND_BASE)/vxtest/src/tests/bsp/sysClock
	MODULES		tmSysClock.o
	PROTOTYPE	void tmSysClockInit(int, int, int);
	INIT_RTN	tmSysClockInit(SYS_CLK_RATE_MIN, SYS_CLK_RATE_MAX, \
			SYS_CLK_RATE);
    	CFG_PARAMS  	SYS_CLK_RATE SYS_CLK_RATE_MIN SYS_CLK_RATE_MAX
}


/*
* The following component definition is for the auxiliary clock test module.
*/

Component 		INCLUDE_TM_AUXCLOCK {
	NAME		Auxiliary clock test module
	SYNOPSIS	This component adds the auxiliary clock test module
	REQUIRES	INCLUDE_VXTESTV2 \
                	INCLUDE_AUX_CLK
	SOURCE		$(WIND_BASE)/vxtest/src/tests/bsp/auxClock
	MODULES		tmAuxClock.o
	PROTOTYPE	void tmAuxClockInit(int, int, int);
	INIT_RTN	tmAuxClockInit(AUX_CLK_RATE_MIN, AUX_CLK_RATE_MAX, \
			AUX_CLK_RATE);
	CFG_PARAMS  	AUX_CLK_RATE AUX_CLK_RATE_MIN AUX_CLK_RATE_MAX
}

/*
* The following component definition is for the NvRam test module.
*/

Component 		INCLUDE_TM_NVRAM {
	NAME		Nvram test module
	SYNOPSIS	This component adds the nvram test module
	REQUIRES	INCLUDE_VXTESTV2 \
                	INCLUDE_BLIB \
                        SELECT_MEM_ALLOC
	SOURCE		$(WIND_BASE)/vxtest/src/tests/bsp/nvRam
	MODULES		tmNvRam.o
	CFG_PARAMS  	NV_RAM_SIZE NV_RAM_SIZE_WRITEABLE NV_BOOT_OFFSET
	PROTOTYPE	void tmNvRamInit(int, int, int);
	INIT_RTN	tmNvRamInit(NV_RAM_SIZE, NV_RAM_SIZE_WRITEABLE, \
                        NV_BOOT_OFFSET);
}

/*
* The following component definition is for the timeStamp test module.
*/

Component               INCLUDE_TM_TIMESTAMP {
        NAME            Timestamp test module
        SYNOPSIS        This component adds the Timestamp Test Module
        REQUIRES        INCLUDE_TIMESTAMP \
                        INCLUDE_SEM_BINARY \
                        INCLUDE_WATCHDOGS \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/bsp/timeStamp
        MODULES         tmTimeStamp.o
	CFG_PARAMS  	TIMESTAMP_API_STRICT TIMESTAMP_MAX_ERROR
	PROTOTYPE	void tmTimeStampInit(BOOL, UINT);
	INIT_RTN	tmTimeStampInit(TIMESTAMP_API_STRICT, TIMESTAMP_MAX_ERROR);
}

/* 
 * The following default for TIMESTAMP_API_STRICT is coupled
 * to the timeStamp test case.  This parameter is defined as
 * FALSE meaning use loose API rules for timestamp APIs.
 */
Parameter 		TIMESTAMP_API_STRICT {
	TYPE		BOOL
	DEFAULT		FALSE
}

/* 
 * The following default for TIMESTAMP_MAX_ERROR is coupled
 * to the timeStamp test case.  This parameter defines the
 * maximum amount of error that is tolerated before declaring
 * a test case as having failed.  This value is compared against
 * timeStamp counter values.
 */
Parameter 		TIMESTAMP_MAX_ERROR {
	TYPE		UINT
	DEFAULT		6000
}


/*
 * Test Init Group
 */
InitGroup		usrVxTestV2BasicTestsInit {
    INIT_RTN		usrVxTestV2BasicTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 basic tests initialization sequence
    INIT_ORDER		INCLUDE_TM_MODEL \
			INCLUDE_TM_BSPAPI \
			INCLUDE_TM_SYSCLOCK \
			INCLUDE_TM_AUXCLOCK \
			INCLUDE_TM_NVRAM \
			INCLUDE_TM_TIMESTAMP
    _INIT_ORDER 	usrVxTestV2Init
}

/*
 * BSPVTS Tests Folder
 */
Folder			FOLDER_VXTESTV2_BASIC {
	NAME		VxTestV2 basic test components
	SYNOPSIS	Used to group basic test components
	CHILDREN	INCLUDE_TM_MODEL \
			INCLUDE_TM_BSPAPI \
			INCLUDE_TM_SYSCLOCK \
			INCLUDE_TM_AUXCLOCK \
			INCLUDE_TM_NVRAM \
			INCLUDE_TM_TIMESTAMP
	DEFAULTS	INCLUDE_TM_MODEL \
			INCLUDE_TM_BSPAPI \
			INCLUDE_TM_SYSCLOCK \
			INCLUDE_TM_AUXCLOCK \
			INCLUDE_TM_NVRAM \
			INCLUDE_TM_TIMESTAMP
	_CHILDREN	FOLDER_VXTESTV2_TESTS
}

