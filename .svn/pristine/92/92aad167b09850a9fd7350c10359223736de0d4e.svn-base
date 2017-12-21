/* 01vxTestV2_arch.cdf - BSPVTS arch test suite test components */

/*
 * Copyright (c) 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,26apr10,rlg  update for mips disassembly library addition
01h,13jan10,jzl  Modify INCLUDE_TM_SPINLOCK_ISR_ND_LIB and 
		 INCLUDE_TM_VMTEST components for BSPVTS
01g,16jun09,d_w  Adding component  INCLUDE_TM_SPINLOCK_ISR_ND_LIB
01f,24feb09,d_c  Created

DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'arch' test suite ($WIND_BASE/vxtest/src/config/arch.prm).

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/

Component               INCLUDE_TM_CPCLIB {
        NAME            CPC lib test module
        SYNOPSIS        Adds the Cross Processor Communication Lib Test Module
        REQUIRES        INCLUDE_CPC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/cpcLib
        MODULES         tmCpcLib.o
        LINK_SYMS       tmCpcLibInit
}

Component               INCLUDE_TM_SPINLOCKLIB {
        NAME            Spinlock lib test module
        SYNOPSIS        This component adds the SpinLock Lib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/spinLockLib
        MODULES         tmSpinLockLib.o
        PROTOTYPE       void tmSpinLockLibInit(void);
        INIT_RTN        tmSpinLockLibInit();
}

Component               INCLUDE_TM_SPINLOCK_ISR_ND_LIB {
        NAME            spinlockIsrNd lib test module
        SYNOPSIS        This component adds the spinLockIsrNd Lib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_SPINLOCK_ISR_ND \
                        INCLUDE_SYS_TIMESTAMP
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/spinLockLib
        MODULES         tmSpinlockIsrNdLib.o
        PROTOTYPE       void tmSpinLockIsrNdInit(void);
        INIT_RTN        tmSpinLockIsrNdInit();
}


Component               INCLUDE_TM_VXATOMICLIB {
        NAME            vxAtomic lib test module
        SYNOPSIS        This component adds the Atomic Lib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/vxAtomicLib
        MODULES         tmVxAtomicLib.o
        PROTOTYPE       void tmVxAtomicLibInit(void);
        INIT_RTN        tmVxAtomicLibInit();
}

Component               INCLUDE_TM_CACHELIB {
        NAME            Cache lib enable/disable test module
        SYNOPSIS        This component adds the Cache Lib Test Module
        REQUIRES        INCLUDE_CACHE_SUPPORT \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/cacheLib
        MODULES         tmCacheLib.o
        LINK_SYMS       cacheEnableTest1
}

Component               INCLUDE_TM_CACHELIB_MALLOC {
        NAME            Cache lib malloc/free test module
        SYNOPSIS        This component adds the Cache Lib Test Module
        REQUIRES        INCLUDE_MEM_MGR_BASIC \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/cacheLib
        MODULES         tmCacheDmaMallocLib.o
        LINK_SYMS       cacheDmaMallocTest1
}

Component               INCLUDE_TM_INTARCHLIB {
        NAME            intArchLib test module
        SYNOPSIS        This component adds the IntArchLib Test Module
        REQUIRES        INCLUDE_WATCHDOGS \
                        INCLUDE_TASK_CREATE_DELETE \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/intArchLib
        MODULES         tmIntArchLib.o
	PROTOTYPE	void tmIntArchLibInit(void);
	INIT_RTN	tmIntArchLibInit();
}

Component               INCLUDE_TM_TASKLIB {
        NAME            Task lib test module
        SYNOPSIS        This component adds the Task Lib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_OBJ_OPEN \
                        INCLUDE_TASK_OPEN \
                        INCLUDE_MAPPED_KERNEL \
                        INCLUDE_TASK_HOOKS \
                        INCLUDE_SIGNALS \
                        INCLUDE_TASK_CREATE_DELETE \
                        INCLUDE_TASK_INFO \
                        INCLUDE_OBJ_LIB \
                        INCLUDE_TASK_RESTART 
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskLib
        MODULES         tmTaskLib.o
        LINK_SYMS       taskActivateTest2
}

Component               INCLUDE_TM_ADRSPACELIB {
        NAME            Address Space Library Test Module
        SYNOPSIS        This component adds the AdrSpace Library Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/adrSpaceLib
        MODULES         tmAdrSpaceLib.o
        PROTOTYPE       void tmAdrSpaceLibInit(void);
        INIT_RTN        tmAdrSpaceLibInit();
}

Component               INCLUDE_TM_DBGLIB {
        NAME            Shell Debug Library Test Module
        SYNOPSIS        This component adds the Shell Debug Library Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_DEBUG
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/dbgLib
        MODULES         tmDbgLib.o
        PROTOTYPE       void tmDbgLibInit(void);
        INIT_RTN        tmDbgLibInit();
}

Component               INCLUDE_TM_ISRLIB {
        NAME            isrLib Test Module
        SYNOPSIS        This component adds the isrLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_AUX_CLK \
	                INCLUDE_ISR_OBJECTS
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/isrLib
        MODULES         tmIsrLib.o
        PROTOTYPE       void tmIsrLibInit(void);
        INIT_RTN        tmIsrLibInit();
}

Component               INCLUDE_TM_KERNELLIB {
        NAME            Kernel Library Test Module
        SYNOPSIS        This component adds the Kernel Library Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_KERNEL
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/kernelLib
        MODULES         tmKernelLib.o
        PROTOTYPE       void tmKernelLibInit(void);
        INIT_RTN        tmKernelLibInit();
}

Component               INCLUDE_TM_SEMBLIB {
        NAME            semBLib Test Module
        SYNOPSIS        This component adds the semBLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/semBLib
        MODULES         tmSemBLib.o
        PROTOTYPE       void tmSemBLibInit(void);
	INIT_RTN	tmSemBLibInit();
}

Component               INCLUDE_TM_SEMCLIB {
        NAME            semCLib Test Module
        SYNOPSIS        This component adds the semCLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/semCLib
        MODULES         tmSemCLib.o
        PROTOTYPE       void tmSemCLibInit(void);
	INIT_RTN	tmSemCLibInit();
}

Component               INCLUDE_TM_SEMLIBINLINE {
        NAME            semMLibInline Test Module
        SYNOPSIS        This component adds the semMLibInline Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_AUX_CLK
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/semMLib
        MODULES         tmSemMLibInline.o
        PROTOTYPE       void tmSemMLibInlineInit(void);
	INIT_RTN	tmSemMLibInlineInit();
}

Component               INCLUDE_TM_SEMMLIB {
        NAME            semMLib Test Module
        SYNOPSIS        This component adds the semMLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_AUX_CLK
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/semMLib
        MODULES         tmSemMLib.o
        PROTOTYPE       void tmSemMLibInit(void);
	INIT_RTN	tmSemMLibInit();
}

Component             INCLUDE_TM_SEMRWLIB {
      NAME            semRWLib test module
      SYNOPSIS        This component adds the tmSemRWLib module
      REQUIRES        INCLUDE_VXTESTV2 \
                      INCLUDE_RTP
     SOURCE           $(WIND_BASE)/vxtest/src/tests/int/semRWLib
     MODULES          tmSemRWLib.o
     PROTOTYPE        void tmSemRWLibInit(void);
     INIT_RTN         tmSemRWLibInit();
}

Component             INCLUDE_TM_SYSCALLLIB {
      NAME            syscallLib Test module
      SYNOPSIS        This component adds the tmSyscallLib Module
      REQUIRES        INCLUDE_VXTESTV2 \
                      INCLUDE_SEM_MUTEX
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/syscallLib
      MODULES         tmSyscallLib.o
      PROTOTYPE       void tmSyscallLibInit(void);
      INIT_RTN        tmSyscallLibInit();
}

Component             INCLUDE_TM_TASKCREATELIB {
      NAME            taskCreate Test module
      SYNOPSIS        This component adds the tmTaskCreateLib Module
      REQUIRES	      INCLUDE_VXTESTV2 \
		      INCLUDE_RTP
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskCreateLib
      MODULES         tmTaskCreateLib.o
      PROTOTYPE       void tmTaskCreateLibInit(void);
      INIT_RTN        tmTaskCreateLibInit();
}

Component               INCLUDE_TM_TASKHOOKLIB {
        NAME            taskHookLib Test module
        SYNOPSIS        This component adds the tmTaskHookLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskLib
        MODULES         tmTaskHookLib.o
        PROTOTYPE       void tmTaskHookLibInit(void);
        INIT_RTN        tmTaskHookLibInit();
}

Component             INCLUDE_TM_TASKRESTARTLIB {
      NAME            taskRestart Test module
      SYNOPSIS        This component adds the tmTaskRestartLib Module
      REQUIRES        INCLUDE_RTP \
                      INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskRestartLib
      MODULES         tmTaskRestartLib.o
      PROTOTYPE       void tmTaskRestartLibInit(void);
      INIT_RTN        tmTaskRestartLibInit();
}

Component             INCLUDE_TM_VMTEST {
      NAME            vmTest test module
      SYNOPSIS        This component adds the tmVmTest test module
      REQUIRES        INCLUDE_VXTESTV2 \
		      INCLUDE_MEMORY_CONFIG
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/vmLib
      MODULES         tmVmTest.o
      PROTOTYPE       void tmVmTestInit(void);
      INIT_RTN        tmVmTestInit();
      }

Component               INCLUDE_TM_VXCPULIB {
        NAME            CPU lib test module
        SYNOPSIS        This component adds the CPU Lib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/vxCpuLib
        MODULES         tmVxCpuLib.o
        LINK_SYMS       vxCpuIndexGetTest1
}

Component		INCLUDE_TM_MIPSDSMLIB {
	NAME            MIPS Disassembly Test
	SYNOPSIS        This component adds the tmMipsDsmLib Test Module
	REQUIRES        INCLUDE_VXTESTV2
	SOURCE          $(WIND_BASE)/vxtest/src/tests/arch/mips/dsmLib
	MODULES         tmMipsDsmLib.o
	PROTOTYPE       void tmMipsDsmLibInit (void);
	INIT_RTN        tmMipsDsmLibInit ();
}


/*
 * Test Init Group
 */
InitGroup		usrVxTestV2ArchTestsInit {
    INIT_RTN		usrVxTestV2ArchTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 arch tests initialization sequence
    INIT_ORDER		INCLUDE_TM_CPCLIB \
                        INCLUDE_TM_CPUSET \
                        INCLUDE_TM_SPINLOCKLIB \
                        INCLUDE_TM_VXATOMICLIB \
			INCLUDE_TM_CACHELIB \
			INCLUDE_TM_CACHELIB_MALLOC \
			INCLUDE_TM_INTARCHLIB \
                        INCLUDE_TM_TASKLIB \
                        INCLUDE_TM_ADRSPACELIB \
                        INCLUDE_TM_DBGLIB \
                        INCLUDE_TM_ISRLIB  \
                        INCLUDE_TM_KERNELLIB \
                        INCLUDE_TM_SEMBLIB \
                        INCLUDE_TM_SEMCLIB \
                        INCLUDE_TM_SEMLIBINLINE \
                        INCLUDE_TM_SEMMLIB \
                        INCLUDE_TM_SEMRWLIB \
			INCLUDE_TM_SYSCALLLIB \
			INCLUDE_TM_TASKCREATELIB \
                        INCLUDE_TM_TASKHOOKLIB \
                        INCLUDE_TM_TASKRESTARTLIB \
                        INCLUDE_TM_VMTEST \
                        INCLUDE_TM_VXCPULIB \
			INCLUDE_TM_MIPSDSMLIB
    _INIT_ORDER 	usrVxTestV2Init
}

/*
 * BSPVTS Tests Folder
 */
Folder			FOLDER_VXTESTV2_ARCH {
	NAME		VxTestV2 arch test components
	SYNOPSIS	Used to group arch test components
	CHILDREN	INCLUDE_TM_CPCLIB \
                        INCLUDE_TM_CPUSET \
                        INCLUDE_TM_SPINLOCKLIB \
                        INCLUDE_TM_VXATOMICLIB \
			INCLUDE_TM_CACHELIB \
			INCLUDE_TM_CACHELIB_MALLOC \
			INCLUDE_TM_INTARCHLIB \
                        INCLUDE_TM_TASKLIB \
                        INCLUDE_TM_ADRSPACELIB \
                        INCLUDE_TM_DBGLIB \
                        INCLUDE_TM_ISRLIB  \
                        INCLUDE_TM_KERNELLIB \
                        INCLUDE_TM_SEMBLIB \
                        INCLUDE_TM_SEMCLIB \
                        INCLUDE_TM_SEMLIBINLINE \
                        INCLUDE_TM_SEMMLIB \
                        INCLUDE_TM_SEMRWLIB \
			INCLUDE_TM_SYSCALLLIB \
			INCLUDE_TM_TASKCREATELIB \
                        INCLUDE_TM_TASKHOOKLIB \
                        INCLUDE_TM_TASKRESTARTLIB \
                        INCLUDE_TM_VMTEST \
                        INCLUDE_TM_VXCPULIB \
			INCLUDE_TM_MIPSDSMLIB
	DEFAULTS	INCLUDE_TM_CPCLIB \
                        INCLUDE_TM_CPUSET \
                        INCLUDE_TM_SPINLOCKLIB \
                        INCLUDE_TM_VXATOMICLIB \
			INCLUDE_TM_CACHELIB \
			INCLUDE_TM_CACHELIB_MALLOC \
			INCLUDE_TM_INTARCHLIB \
                        INCLUDE_TM_TASKLIB \
                        INCLUDE_TM_ADRSPACELIB \
                        INCLUDE_TM_DBGLIB \
                        INCLUDE_TM_ISRLIB  \
                        INCLUDE_TM_KERNELLIB \
                        INCLUDE_TM_SEMBLIB \
                        INCLUDE_TM_SEMCLIB \
                        INCLUDE_TM_SEMLIBINLINE \
                        INCLUDE_TM_SEMMLIB \
                        INCLUDE_TM_SEMRWLIB \
			INCLUDE_TM_SYSCALLLIB \
			INCLUDE_TM_TASKCREATELIB \
                        INCLUDE_TM_TASKHOOKLIB \
                        INCLUDE_TM_TASKRESTARTLIB \
                        INCLUDE_TM_VMTEST \
                        INCLUDE_TM_VXCPULIB \
			INCLUDE_TM_MIPSDSMLIB
	_CHILDREN	FOLDER_VXTESTV2_TESTS
}

