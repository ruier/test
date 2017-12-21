/* 01vxTestV2_io.cdf - BSPVTS io test suite test components  */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,24feb09,d_c  Reorg in preparation for internal test module
                 componentization
01a,26aug08,mdo   Created components for I/O tests


DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'io' test suite ($WIND_BASE/vxtest/src/config/io.prm).
The 'io' test suite tests I/O features of the OS.

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/

Component               INCLUDE_TM_ATADRV {
        NAME            ATA Test Module
        SYNOPSIS        This component adds the AtaDrv Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_ATA
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ataDrv
        MODULES         tmAtaDrv.o
        LAYER           2
        PROTOTYPE       void tmAtaDrvInit(void);
        INIT_RTN        tmAtaDrvInit();
}


/*
 * Test Init Group
 */
InitGroup		usrVxTestV2IoTestsInit {
    INIT_RTN	        usrVxTestV2IoTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 I/O tests initialization sequence
    INIT_ORDER		INCLUDE_TM_ATADRV
    _INIT_ORDER 	usrVxTestV2Init
}

/*
 * IOVTS Tests Folder
 */
Folder			FOLDER_VXTESTV2_IO {
	NAME		VxTestV2 I/O test components
	SYNOPSIS	Used to group I/O test components
	CHILDREN	INCLUDE_TM_ATADRV
	DEFAULTS	INCLUDE_TM_ATADRV
       _CHILDREN	FOLDER_VXTESTV2_TESTS
}
