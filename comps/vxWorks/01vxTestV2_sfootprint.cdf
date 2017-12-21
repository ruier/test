/* 01vxTestV2_sfootprint.cdf - BSPVTS small footprint VxTestV2 test suite */

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
01b,16jul09,jc0  update some text because this filename is updated
01a,19may09,jc0  Created

DESCRIPTION

This file contains vxTestV2 test component definitions for internal-only 
test modules that test basic BSP features.

To make a test module part of the BSPVTS internal basic test suite,
add it's path to $WIND_BASE/vxtest/src/config/basicInt.prm.

*/

/*
 * Test Init Group
 */
Component           INCLUDE_TM_COMPS {
    NAME            sysComps test module
    SYNOPSIS        This component adds the sysComps Test Module
    CONFIGLETTES    vxtest/usrvxTestV2Comps.c
    REQUIRES        INCLUDE_VXTESTV2
    SOURCE          $(WIND_BASE)/vxtest/src/tests/bsp/comps
    MODULES         tmComps.o
    PROTOTYPE       void tmCompsInit(void);
    INIT_RTN        tmCompsInit();
}

InitGroup           usrVxTestV2SfootprintIntTestsInit {
    INIT_RTN        usrVxTestV2SfootprintIntTestsInit ();
    SYNOPSIS        BSPVTS VxTestV2 internal small footprint tests init sequence
    INIT_ORDER      INCLUDE_TM_COMPS
    _INIT_ORDER     usrVxTestV2Init
}

/*
 * Small Footprint Tests Folder
 */
Folder              FOLDER_VXTESTV2_SFOOTPRINTINT {
    NAME            VxTestV2 internal small footprint test components
    SYNOPSIS        Used to group small footprint test components
    CHILDREN        INCLUDE_TM_COMPS
    DEFAULTS        INCLUDE_TM_SMALLFOOTPRINT
    _CHILDREN       FOLDER_VXTESTV2_TESTS
}

