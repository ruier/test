/* 01vxTestV2_vxbus.cdf - BSPVTS vxbus test suite VxTestV2 test components  */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,24feb09,d_c  Reorg in preparation for internal test module
                 componentization
01c,23jun08,d_c  Auto-exclude tests for unsupported components
01b,30apr08,mdo  Modified to run under bspvts
01a,18sep07,agf  Initial creation based on 01vxTestV2Tests.cdf


DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'vxbus' test suite ($WIND_BASE/vxtest/src/config/vxbus.prm).

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/

Component               INCLUDE_TM_VXBTEST {
        NAME            vxbus test module
        SYNOPSIS        This component adds the vxBus Test Module
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_VXBUS_SHOW \
                        INCLUDE_SEM_BINARY \
                        SELECT_MEM_ALLOC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTest.o
	LINK_SYMS	vxbTestInit
}

Component               INCLUDE_TM_VXBPCIACCESS {
        NAME            vxbus PCI access test module
        SYNOPSIS        This component adds the vxBus PCI access Test Module
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_VXBUS_SHOW \
                        INCLUDE_PCI_BUS \
                        INCLUDE_SEM_BINARY \
                        SELECT_MEM_ALLOC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestAccessPciDriver.o
        LINK_SYMS       vxbTestAccessPciDriverInit
}

Component               INCLUDE_TM_VXBVIRTACCESS {
        NAME            vxbus virtual device access test module
        SYNOPSIS        This component adds the vxBus virtual device access Test Module
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_VXBUS_SHOW \
                        INCLUDE_PCI_BUS \
                        INCLUDE_SEM_BINARY \
                        SELECT_MEM_ALLOC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestAccessVirtDriver.o
        LINK_SYMS       vxbTestAccessVirtDriverInit
}

Component               INCLUDE_TM_VXBPARAMSYS {
        NAME            vxbus parameter system test module
        SYNOPSIS        This component adds the vxBus Parameter System Test Module
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_VXBUS_SHOW \
                        INCLUDE_PARAM_SYS \
                        INCLUDE_SEM_BINARY \
                        SELECT_MEM_ALLOC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestParamSys.o
        LINK_SYMS       vxbTestParamSysInit
}

Component               INCLUDE_TM_VXBTESTDMABUFFER {
        NAME            vxbus DMA Buffer test module
        SYNOPSIS        This component adds the vxBus DMA Buffer Test Module
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_VXBUS_SHOW \
                        INCLUDE_DMA_SYS \
                        INCLUDE_SEM_BINARY \
                        SELECT_MEM_ALLOC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestDmaBuffer.o
        LINK_SYMS       vxbTestDmaBufferInit
}

Component               INCLUDE_TM_VXBTESTDMALIB {
        NAME            vxbus DMA Library test module
        SYNOPSIS        This component adds the vxBus DMA Library Test Module
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_VXBUS_SHOW \
                        INCLUDE_DMA_SYS \
                        INCLUDE_SEM_BINARY \
                        SELECT_MEM_ALLOC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestDmaLib.o
        LINK_SYMS       vxbTestDmaLibInit
}

Component               INCLUDE_TM_VXBTIMERLIB {
        NAME            vxbus Timer Library test module
        SYNOPSIS        This component adds the vxBus Timer Library Test Module
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_VXBUS_SHOW \
                        INCLUDE_TIMER_SYS \
                        INCLUDE_SEM_BINARY \
                        SELECT_MEM_ALLOC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestTimerLib.o
        LINK_SYMS       vxbTestTimerLibInit
}

Component               INCLUDE_TM_VXBTESTSIO {
        NAME            vxbus SIO test module
        SYNOPSIS        This component adds the vxBus SIO Test Module
        REQUIRES        INCLUDE_VXBUS \
			INCLUDE_SIO_UTILS \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestSio.o
        LINK_SYMS       vxbTestSioInit
}

Component               INCLUDE_TM_VXBTIMERDRIVER {
        NAME            vxBus Timer Driver test module
        SYNOPSIS        This component adds the vxBus Timer Driver Test Module
        REQUIRES        INCLUDE_VXBUS \
			INCLUDE_TIMER_SYS \
			INCLUDE_SYSCLK_INIT \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/vxbus
        MODULES         vxbTestTimerDriver.o
        LINK_SYMS       vxbTestTimerDriverInit
}


/*
 * Test Init Group
 */
InitGroup		usrVxTestV2VxBusTestsInit {
    INIT_RTN		usrVxTestV2VxBusTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 vxbus tests initialization sequence
    INIT_ORDER		INCLUDE_TM_VXBTEST \
			INCLUDE_TM_VXBPCIACCESS \
			INCLUDE_TM_VXBVIRTACCESS \
			INCLUDE_TM_VXBPARAMSYS \
			INCLUDE_TM_VXBTESTDMABUFFER \
			INCLUDE_TM_VXBTESTDMALIB \
			INCLUDE_TM_VXBTIMERLIB \
                        INCLUDE_TM_VXBTIMERDRIVER \
			INCLUDE_TM_VXBTESTSIO
    _INIT_ORDER 	usrVxTestV2Init
}

/*
 * VxBus Tests Folder
 */
Folder			FOLDER_VXTESTV2_VXBUS {
	NAME            VxTestV2 vxbus test components
	SYNOPSIS        Used to group vxbus test components
	CHILDREN        INCLUDE_TM_VXBTEST \
			INCLUDE_TM_VXBPCIACCESS \
			INCLUDE_TM_VXBVIRTACCESS \
			INCLUDE_TM_VXBPARAMSYS \
			INCLUDE_TM_VXBTESTDMABUFFER \
			INCLUDE_TM_VXBDMALIB \
			INCLUDE_TM_VXBTIMERLIB \
                        INCLUDE_TM_VXBTIMERDRIVER \
			INCLUDE_TM_VXBTESTSIO
	DEFAULTS        INCLUDE_TM_VXBUS
	_CHILDREN       FOLDER_VXTESTV2_TESTS
}

