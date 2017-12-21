/* 01vxTestV2_filesystem.cdf - BSPVTS filesystem test suite test components */

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
01c,24feb09,d_c  Reorg in preparation for internal test module
                 componentization
01b,27aug08,mdo  Remove extra backslash
01a,25aug08,mdo   Created components for FileSystem tests


DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'filesystem' test suite 
($WIND_BASE/vxtest/src/config/filesystem.prm).

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/

Component             INCLUDE_TM_FFSLIB {
      NAME            ffsLib test module
      SYNOPSIS        This component adds the tmFfsLib test module
      LAYER           2
      REQUIRES        INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ffsLib
      MODULES         tmFfsLib.o
      PROTOTYPE       void tmFfsLibInit(void);
      INIT_RTN        tmFfsLibInit();
}

Component             INCLUDE_TM_FSINITTESTS {
      NAME            fsInitTests Test module
      SYNOPSIS        This component adds the tmFsInitTests Module
      LAYER           2
      REQUIRES        INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/comp/fileSystemInit
      MODULES         tmFsInitTests.o
      PROTOTYPE       void tmFsInitTestsInit(void);
      INIT_RTN        tmFsInitTestsInit();
}

Component             INCLUDE_TM_FSTESTS {
      NAME            fsTests Test module
      SYNOPSIS        This component adds the tmFsTests Module
      LAYER           2
      REQUIRES        INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/comp/fileSystem
      MODULES         tmFsTests.o
      PROTOTYPE       void tmFsTestsInit(void);
      INIT_RTN        tmFsTestsInit();
}


/*
 * Test Init Group
 */
InitGroup		usrVxTestV2FileSystemTestsInit {
    INIT_RTN		usrVxTestV2FileSystemTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 file system tests initialization sequence
    INIT_ORDER		INCLUDE_TM_FFSLIB \
                        INCLUDE_TM_FSINITTESTS \
                        INCLUDE_TM_FSTESTS
     _INIT_ORDER 	usrVxTestV2Init
}

/*
 * FSVTS Tests Folder
 */
Folder			FOLDER_VXTESTV2_FILESYSTEM {
	NAME		VxTestV2 file system test components
	SYNOPSIS	Used to group file system test components
	CHILDREN	INCLUDE_TM_FFSLIB \
                        INCLUDE_TM_FSINITTESTS \
                        INCLUDE_TM_FSTESTS \
	DEFAULTS	INCLUDE_TM_FFSLIB \
                        INCLUDE_TM_FSINITTESTS \
                        INCLUDE_TM_FSTESTS
       _CHILDREN	FOLDER_VXTESTV2_TESTS
}
