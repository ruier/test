/* 01vxTestV2_bootapp.cdf - BSPVTS bootapp test suite test components  */

/*
 * Copyright (c) 2006, 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,25feb09,d_c  Reorg in preparation for internal test module
                 componentization
01b,26aug08,jpb  Removed LAYER from components.
01a,24oct06,jmt  Created 


DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'bootapp' test suite ($WIND_BASE/vxtest/src/config/bootapp.prm).

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/

/*
* The following component definition is for the BootApp test module.
*/

Component               INCLUDE_TM_BOOT_APP {
        NAME            vxTestV2 bootApp test module
        SYNOPSIS        This component adds the BootApp Test Module
        REQUIRES        INCLUDE_BOOT_TEST_FUNCS \
                        INCLUDE_BOOT_RAM_IMAGE \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/bsp/boot
        MODULES         tmBootApp.o
	PROTOTYPE	void tmBootAppInit(void);
	INIT_RTN	tmBootAppInit();
}

/*
 * Test Init Group
 */
InitGroup		usrVxTestV2BootappTestsInit {
    INIT_RTN		usrVxTestV2BootappTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 arch tests initialization sequence
    INIT_ORDER		INCLUDE_TM_BOOT_APP 
    _INIT_ORDER 	usrVxTestV2Init
}

/*
 * BSPVTS Tests Folder 
 */
Folder			FOLDER_VXTESTV2_BOOTAPP	{
	NAME            VxTestV2 bootapp test components
	SYNOPSIS	Used to group bootapp test components
	CHILDREN	INCLUDE_TM_BOOT_APP 
	DEFAULTS	INCLUDE_TM_BOOT_APP 
	_CHILDREN	FOLDER_VXTESTV2_TESTS
}

