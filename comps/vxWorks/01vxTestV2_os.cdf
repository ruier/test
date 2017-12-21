/* 01vxTestV2_os.cdf - BSPVTS os test suite test components  */

/*
 * Copyright (c) 2008-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01u,21jun13,hao  removed INCLUDE_BSD_SOCKET from INCLUDE_TM_SOCKLIB
01t,23feb10,zl   INCLUDE_TM_SHMLIB requires INCLUDE_POSIX_SHM_API
01s,16dec09,d_w  Merge from 17 to correct the properties of these components
01r,13nov09,d_w  Update INCLUDE_TM_MSGQLIB define
01q,04nov09,f_z  Add vxTestV2 case for CQ:WIND00172700
01p,16jun09,d_w  Correcting path locations for salStressLib
01o,13may09,f_z  Add component INCLUDE_PG_POOL_LIB for tmPgPoolPhysLib
                 defect: WIND00165425
01n,24feb09,d_c  Reorg in preparation for internal test module
                 componentization
01m,22feb09,d_p  Add component INCLUDE_AUX_CLK for tmSemShow: WIND00153502
01l,09dec08,d_c  Add tmDbgLib component
01k,11sep08,d_c  Fix tmIsrLib failures: WIND00134121
01j,03sep08,d_c  Fix required comps for tmSemMLib
01i,02sep08,jpb  Removed LAYER attribute.
01h,17jul08,kab  add components
01g,10jul08,ska  added components for OS tests
01f,09jul08,goy  added component for OS tests
01e,25jun08,kab  Fix typos - missing backslash (continuation of list)
		 Remove INCLUDE_TM_MMAN component (RTP only)
01e,21may08,ska  added component for OS tests
01d,13may08,raj  Merged hookLib, workQLib, mmanLib OS tests
01c,09may08,raj  Merged memPartLib, poolLib, taskHookLib OS tests
01b,09apr08,kab  Merged other OS tests
01a,13mar08,kab  Created components for OS tests


DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'os' test suite ($WIND_BASE/vxtest/src/config/os.prm).
The 'os' test suite test core OS features.

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/

Component               INCLUDE_TM_IOBASICLIB {
        NAME            IoBasic Library Test Module
        SYNOPSIS        This component adds the IoBasic Library Test Module
	REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ioBasicLib
        MODULES         tmIoBasicLib.o
        PROTOTYPE       void tmIoBasicLibInit(void);
        INIT_RTN        tmIoBasicLibInit();
}

Component               INCLUDE_TM_LOGINLIB {
        NAME            Login Library Test Module
        SYNOPSIS        This component adds the Login Library Test Module
        REQUIRES        INCLUDE_SECURITY \
                        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/loginLib
        MODULES         tmLoginLib.o
        PROTOTYPE       void tmLoginLibInit(void);
        INIT_RTN        tmLoginLibInit();
}

Component               INCLUDE_TM_ENVLIB {
        NAME            Env Library Test Module
        SYNOPSIS        This component adds the Env Library Test Module
	REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/envLib
        MODULES         tmEnvLib.o
        PROTOTYPE       void tmEnvLibInit(void);
        INIT_RTN        tmEnvLibInit();
}

Component               INCLUDE_TM_HASHLIB {
        NAME            Hash Library Test Module
        SYNOPSIS        This component adds the Hash Library Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/hashLib
        MODULES         tmHashLib.o
        PROTOTYPE       void tmHashLibInit(void);
        INIT_RTN        tmHashLibInit();
}

Component               INCLUDE_TM_ANSICTYPE {
        NAME            ansiCtype Library Test Module
        SYNOPSIS        This component adds the ansiCtype Library Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ansiCtype
        MODULES         tmAnsiCtype.o
        PROTOTYPE       void tmAnsiCtypeInit(void);
        INIT_RTN        tmAnsiCtypeInit();
}

Component               INCLUDE_TM_ANSISETJMP {
        NAME            ansiSetjmp Library Test Module
        SYNOPSIS        This component adds the ansiSetjmp Library Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ansiSetjmp
        MODULES         tmAnsiSetjmp.o
        PROTOTYPE       void tmAnsiSetjmpInit(void);
        INIT_RTN        tmAnsiSetjmpInit();
}

Component               INCLUDE_TM_ANSISTDARG {
        NAME            Ansi Standard Arg Library Test Module
        SYNOPSIS        This component adds the ansiStdarg Library Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ansiStdarg
        MODULES         tmAnsiStdarg.o
        PROTOTYPE       void tmAnsiStdargInit(void);
        INIT_RTN        tmAnsiStdargInit();
}

Component               INCLUDE_TM_ANSISTDLIB {
        NAME            Ansi Standard Library Test Module
        SYNOPSIS        This component adds the ansiStdlib Library Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ansiStdlib
        MODULES         tmAnsiStdlib.o
        PROTOTYPE       void tmAnsiStdlibInit(void);
        INIT_RTN        tmAnsiStdlibInit();
}

Component               INCLUDE_TM_BLIB {
        NAME            bLib Test Module
        SYNOPSIS        This component adds the bLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_BLIB
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/bLib
        MODULES         tmBLib.o
        PROTOTYPE       void tmBLibInit(void);
	INIT_RTN	tmBLibInit();
}

Component               INCLUDE_TM_CLOCKLIB {
        NAME            clockLib Test Module
        SYNOPSIS        This component adds the clockLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/clockLib
        MODULES         tmClockLib.o
        PROTOTYPE       void tmClockLibInit(void);
	INIT_RTN	tmClockLibInit();
}

Component               INCLUDE_TM_MEMLIB {
        NAME            memLib Test Module
        SYNOPSIS        This component adds the memLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/memLib
        MODULES         tmMemLib.o
        PROTOTYPE       void tmMemLibInit(void);
	INIT_RTN	tmMemLibInit();
}

Component               INCLUDE_TM_MEMDRVLIB {
        NAME            memLib Test Module for MEMDRV parts
        SYNOPSIS        This component adds the memLib MEMDRV Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_MEMDRV \
			INCLUDE_IO_SYSTEM
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/memLib
        MODULES         tmMemDrvLib.o
        PROTOTYPE       void tmMemDrvLibInit(void);
	INIT_RTN	tmMemDrvLibInit();
}

Component               INCLUDE_TM_MSGQLIB {
        NAME            msgQLib Test Module
        SYNOPSIS        This component adds the msqQLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_MEMDRV \
			INCLUDE_MSG_Q
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/msgQLib
        PROTOTYPE       void tmMsgQLibInit(void);
	INIT_RTN	tmMsgQLibInit();
}

Component               INCLUDE_TM_SEMSHOW {
        NAME            semShow Test Module
        SYNOPSIS        This component adds the semShow Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_AUX_CLK \
                        INCLUDE_SEM_SHOW
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/semShow
        MODULES         tmSemShow.o
        PROTOTYPE       void tmSemShowInit(void);
        INIT_RTN        tmSemShowInit();
}

Component               INCLUDE_TM_SIGLIB {
        NAME            sigLib Test Module
        SYNOPSIS        This component adds the sigLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_AUX_CLK
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/sigLib
        MODULES         tmSigLib.o
        PROTOTYPE       void tmSigLibInit(void);
        INIT_RTN        tmSigLibInit();
}

Component               INCLUDE_TM_SYMLIB {
        NAME            symLib Test Module
        SYNOPSIS        This component adds the symLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/symLib
        MODULES         tmSymLib.o
	LINK_SYMS	symTblCreateTest1
}

Component               INCLUDE_TM_PGPOOLPHYSLIB {
        NAME            pgPoolPhysLib Test Module
        SYNOPSIS        This component adds the pgPoolPhysLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_PG_POOL_LIB
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pgPoolPhysLib
        MODULES         tmPgPoolPhysLib.o
	LINK_SYMS	pgPoolPhysAddToPoolTest
}

Component               INCLUDE_TM_PGPOOLVIRTLIB {
        NAME            pgPoolVirtLib Test Module
        SYNOPSIS        This component adds the pgPoolVirtLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_PG_POOL_LIB
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pgPoolVirtLib
        MODULES         tmPgPoolVirtLib.o
	LINK_SYMS	pgPoolVirtAddToPoolTest
}

Component               INCLUDE_TM_MQPXLIB {
        NAME            mqPxLib Test Module
        SYNOPSIS        This component adds the mqPxLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/mqPxLib
        MODULES         tmMqPxLib.o
	LINK_SYMS	mqPxTest1
}

Component               INCLUDE_TM_NETBUFLIB {
        NAME            netBufLib Test Module
        SYNOPSIS        This component adds the netBufLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/netBufLib
        MODULES         tmNetBufLibTest.o
	LINK_SYMS	netBuf60TestSuite
}

Component               INCLUDE_TM_OBJLIB {
        NAME            objLib Test Module
        SYNOPSIS        This component adds the objLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/objLib
        MODULES         tmObjLib.o
	LINK_SYMS	objHandleTest
}

Component               INCLUDE_TM_PGMGRLIB {
        NAME            pgMgrLib Test Module
        SYNOPSIS        This component adds the pgMgrLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_SEM_LIB
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pgMgrLib
        MODULES         tmPgMgrLib.o
        PROTOTYPE       void tmPgMgrLibInit(void);
	INIT_RTN	tmPgMgrLibInit();
}

Component               INCLUDE_TM_PRIINHERIT {
        NAME            priInherit Test Module
        SYNOPSIS        This component adds the priInherit Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_SEM_MUTEX
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/priInherit
        MODULES         tmPriInherit.o
        PROTOTYPE       void tmPriInheritInit(void);
        INIT_RTN        tmPriInheritInit();
}

Component               INCLUDE_TM_SALLIB {
        NAME            salLib Test Module
        SYNOPSIS        This component adds the salLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_SAL_COMMON \
			INCLUDE_RTP
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/salLib
        MODULES         tmSalLib.o
	LINK_SYMS	salCallFromKernelTest
}

Component               INCLUDE_TM_SALSTRESSLIB {
        NAME            salStressLib Test Module
        SYNOPSIS        This component adds the salStressLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_SAL_COMMON \
			INCLUDE_RTP
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/salLib
        MODULES         tmSalStressLib.o
	LINK_SYMS	salStressTest
}

Component               INCLUDE_TM_TASKOPEN {
        NAME            taskOpen Test Module
        SYNOPSIS        This component adds the taskOpen Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskOpen
        MODULES         tmTaskOpen.o
        PROTOTYPE       void tmTaskOpenInit(void);
        INIT_RTN        tmTaskOpenInit();
}

Component               INCLUDE_TM_MEMPARTLIB {
        NAME            memPartLib Test Module
        SYNOPSIS        This component adds the tmMemPartLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
			INCLUDE_MEMDRV \
			INCLUDE_VM_RTP
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/memPartLib
        MODULES         tmMemPartLib.o
        PROTOTYPE       void tmMemPartLibInit(void);
        INIT_RTN        tmMemPartLibInit();
}

Component               INCLUDE_TM_POOLLIB {
        NAME            poolLib Test Module
        SYNOPSIS        This component adds the tmPoolLib Test Module
        REQUIRES        INCLUDE_VXTESTV2 \
                        INCLUDE_POOL
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/poolLib
        MODULES         tmPoolLib.o
        PROTOTYPE       void tmPoolLibInit(void);
        INIT_RTN        tmPoolLibInit();
}

Component               INCLUDE_TM_PXTRACEEVENTID {
        NAME            pxTraceEventId Test module
        SYNOPSIS        This component adds the tmPxTraceEventId Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pxTraceEventId
        MODULES         tmPxTraceEventId.o
        PROTOTYPE       void tmPxTraceEventIdInit(void);
        INIT_RTN        tmPxTraceEventIdInit();
}

Component               INCLUDE_TM_HOOKLIB {
        NAME            hookLib Test module
        SYNOPSIS        This component adds the tmHookLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/hookLib
        MODULES         tmHookLib.o
        PROTOTYPE       void tmHookLibInit(void);
        INIT_RTN        tmHookLibInit();
}

Component               INCLUDE_TM_WORKQLIB {
        NAME            workQLib Test module
        SYNOPSIS        This component adds the tmWorkQLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/workQLib
        MODULES         tmWorkQLib.o
        PROTOTYPE       void tmWorkQLibInit(void);
        INIT_RTN        tmWorkQLibInit();
}

Component               INCLUDE_TM_WDCREATELIB {
       NAME             wdCreateLib Test module
       SYNOPSIS         This component adds the tmWdCreateLib Module
       REQUIRES         INCLUDE_VXTESTV2
       SOURCE           $(WIND_BASE)/vxtest/src/tests/int/wdCreateLib
       MODULES          tmWdCreateLib.o
       PROTOTYPE        void tmWdCreateLibInit(void);
       INIT_RTN         tmWdCreateLibInit();
}

Component	      INCLUDE_TM_ANSITIME {
      NAME            ansiTime Test module
      SYNOPSIS        This component adds the tmAnsiTime Module
      REQUIRES        INCLUDE_ANSI_TIME \
                      INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/ansiTime
      MODULES         tmAnsiTime.o
      PROTOTYPE       void tmAnsiTimeInit(void);
      INIT_RTN        tmAnsiTimeInit();
}

Component             INCLUDE_TM_TASKSHOW {
      NAME            taskShow Test module
      SYNOPSIS        This component adds the tmTaskShow Module
      REQUIRES        INCLUDE_RTP \
                      INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskShow
      MODULES         tmTaskShow.o
      PROTOTYPE       void tmTaskShowInit(void);
      INIT_RTN        tmTaskShowInit();
}

Component             INCLUDE_TM_PIPEDRV {
      NAME            pipeDrv Test module
      SYNOPSIS        This component adds the tmPipeDrv Module
      REQUIRES        INCLUDE_PIPES \
      	              INCLUDE_VXTESTV2 \
		      INCLUDE_RTP
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pipeDrv
      MODULES         tmPipeDrv.o
      PROTOTYPE       void tmPipeDrvInit(void);
      INIT_RTN        tmPipeDrvInit();
}

Component             INCLUDE_TM_PIPEDRV2 {
      NAME            pipeDrv2 Test module
      SYNOPSIS        This component adds the tmPipeDrv2 Module
      REQUIRES        INCLUDE_PIPES \
                      INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pipeDrv2
      MODULES         tmPipeDrv2.o
      PROTOTYPE       void tmPipeDrv2Init(void);
      INIT_RTN        tmPipeDrv2Init();
}

Component             INCLUDE_TM_TASKVARLIB {
      NAME	      taskVarLib Test module
      SYNOPSIS        This component adds the tmTaskVarLib Module
      REQUIRES        INCLUDE_TASK_VARS \
      		      INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskVarLib
      MODULES         tmTaskVarLib.o
      PROTOTYPE       void tmTaskVarLibInit(void);
      INIT_RTN        tmTaskVarLibInit();
}

Component             INCLUDE_TM_PMLIB {
      NAME            edrLib Test module
      SYNOPSIS        This component adds the edrLib() API Test Module
      REQUIRES        INCLUDE_VXTESTV2 \
      		      INCLUDE_EDR_ERRLOG
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/edrLib
      PROTOTYPE       void tmPmLibInit(void);
      MODULES         tmPmLib.o
      INIT_RTN        tmPmLibInit();
}

Component             INCLUDE_TM_TASKINFO {
      NAME            taskInfo Test module
      SYNOPSIS        This component adds the tmTaskInfo Module
      REQUIRES        INCLUDE_VXTESTV2 \
      		      INCLUDE_RTP
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/taskInfo
      MODULES         tmTaskInfo.o
      PROTOTYPE       void tmTaskInfoInit(void);
      INIT_RTN        tmTaskInfoInit();
}

Component 	      INCLUDE_TM_SOCKLIB {
      NAME            sockLib test module
      SYNOPSIS        This component adds the tmSockLib test module
      REQUIRES        INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/sockLib
      MODULES         tmSockLib.o
      PROTOTYPE       void tmSockLibInit(void);
      INIT_RTN        tmSockLibInit();
}

Component             INCLUDE_TM_MEMPARTFUNC {
      NAME            memPartFunc Test module
      SYNOPSIS        This component adds the tmMemPartFunc Module
      REQUIRES        INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/comp/memMgmt
      MODULES         tmMemPartFunc.o
      PROTOTYPE       void tmMemPartFuncInit(void);
      INIT_RTN        tmMemPartFuncInit();
}

Component             INCLUDE_TM_PTHREADLIB {
      NAME            pthreadLib  test module
      REQUIRES        INCLUDE_POSIX_PTHREADS \
                      INCLUDE_VXTESTV2 \
                      INCLUDE_RTP
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pthreadLib
      MODULES         tmPthreadLib.o
      PROTOTYPE       void tmPthreadLibInit(void);
      INIT_RTN        tmPthreadLibInit();
}

Component             INCLUDE_TM_SHMLIB {
      NAME            shmLib test module
      SYNOPSIS        This component adds the tmShmLib module
      REQUIRES        INCLUDE_VXTESTV2 \
		      INCLUDE_POSIX_SHM_API
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/shmLib
      MODULES         tmShmLib.o
      PROTOTYPE       void tmShmLibInit(void);
      INIT_RTN        tmShmLibInit();
} 						

Component             INCLUDE_TM_WINDSTRESSTEST {
      NAME            windStress test module
      SYNOPSIS        This component adds the tmWindStressTest module
      REQUIRES        INCLUDE_VXTESTV2 \
                      INCLUDE_RTP
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/windTests/Stress
      MODULES         tmWindStressTest.o
      PROTOTYPE       void tmWindStressTestInit(void);
      INIT_RTN        tmWindStressTestInit();
}

Component             INCLUDE_TM_HEAPSTRESSTEST {
      NAME            heapStress test module
      SYNOPSIS        This component adds the tmHeapStressTest module
      REQUIRES        INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/sys/mm
      MODULES         tmHeapStressTest.o
      PROTOTYPE       void tmHeapStressTestInit(void);
      INIT_RTN        tmHeapStressTestInit();
}

Component             INCLUDE_TM_WINDKERNELLIB {
      NAME            windKernelTests test module
      SYNOPSIS        This component adds the tmWindKernelTest module
      REQUIRES        INCLUDE_VXTESTV2
      SOURCE          $(WIND_BASE)/vxtest/src/tests/int/windKernelTests
      MODULES         tmWindKernelTest.o
      PROTOTYPE       void tmWindKernelTestInit(void);
      INIT_RTN        tmWindKernelTestInit();
}

Component               INCLUDE_TM_MACROTEST {
        NAME            MACRO test module
        SYNOPSIS        This component adds the MACRO Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/comp/macros
        MODULES         tmMacroTest.o
        PROTOTYPE       void tmMacroTestInit(void);
        INIT_RTN        tmMacroTestInit();
}

Component               INCLUDE_TM_PINGLIB {
        NAME            pingLib test module
        SYNOPSIS        This component adds the pingLib Test Module
        REQUIRES        INCLUDE_PING \
                        INCLUDE_VXTESTV2
	PROTOTYPE	void tmPingLibInit();
	INIT_RTN	tmPingLibInit();
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/pingLib
        MODULES         tmPingLib.o
}

Component               INCLUDE_TM_ERRNOLIB_SMP {
        NAME            ErrnoLib test module for UP and SMP
        SYNOPSIS        Adds the UP and SMP ErrnoLib Test Module
        REQUIRES        INCLUDE_VXTESTV2
        SOURCE          $(WIND_BASE)/vxtest/src/tests/int/errnoLib
        MODULES         tmErrnoLib.o
        LINK_SYMS       errnoGetTest1
}

/*
 * Test Init Group
 */
InitGroup		usrVxTestV2OsTestsInit {
    INIT_RTN		usrVxTestV2OsTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 OS tests initialization sequence
    INIT_ORDER		INCLUDE_TM_BLIB \
			INCLUDE_TM_CLOCKLIB \
			INCLUDE_TM_MSGQLIB \
                        INCLUDE_TM_ANSICTYPE \
                        INCLUDE_TM_ANSISETJMP \
                        INCLUDE_TM_ANSISTDLIB \
                        INCLUDE_TM_IOBASICLIB \
                        INCLUDE_TM_LOGINLIB \
                        INCLUDE_TM_ENVLIB \
                        INCLUDE_TM_HASHLIB \
                        INCLUDE_TM_SEMSHOW \
                        INCLUDE_TM_SIGLIB \
                        INCLUDE_TM_PGMGRLIB \
                        INCLUDE_TM_MEMLIB \
                        INCLUDE_TM_MEMDRVLIB \
                        INCLUDE_TM_MEMPARTLIB \
                        INCLUDE_TM_POOLLIB \
                        INCLUDE_TM_PXTRACEEVENTID \
                        INCLUDE_TM_HOOKLIB \
                        INCLUDE_TM_WORKQLIB \
			INCLUDE_TM_WDCREATELIB \
			INCLUDE_TM_ANSITIME \
			INCLUDE_TM_TASKSHOW \
			INCLUDE_TM_PIPEDRV \
			INCLUDE_TM_PIPEDRV2 \
			INCLUDE_TM_TASKVARLIB \
			INCLUDE_TM_TASKINFO \
                        INCLUDE_TM_PMLIB \
			INCLUDE_TM_PRIINHERIT \
			INCLUDE_TM_SOCKLIB \
                        INCLUDE_TM_MEMPARTFUNC \
			INCLUDE_TM_PTHREADLIB \
                        INCLUDE_TM_SHMLIB \
                        INCLUDE_TM_WINDSTRESSTEST \
                        INCLUDE_TM_WINDKERNELLIB
                        INCLUDE_TM_HEAPSTRESSTEST \
                        INCLUDE_TM_MACROTEST \
                        INCLUDE_TM_PINGLIB \
                        INCLUDE_TM_ERRNOLIB_SMP
     _INIT_ORDER 	usrVxTestV2Init
}

/*
 * OS Tests Folder
 */
Folder			FOLDER_VXTESTV2_OS {
	NAME		VxTestV2 OS test components
	SYNOPSIS	Used to group OS test components
	CHILDREN	INCLUDE_TM_BLIB \
			INCLUDE_TM_CLOCKLIB \
			INCLUDE_TM_MSGQLIB \
                        INCLUDE_TM_ANSICTYPE \
                        INCLUDE_TM_ANSISETJMP \
                        INCLUDE_TM_ANSISTDLIB \
                        INCLUDE_TM_IOBASICLIB \
                        INCLUDE_TM_LOGINLIB \
                        INCLUDE_TM_ENVLIB \
                        INCLUDE_TM_HASHLIB \
                        INCLUDE_TM_SEMSHOW \
                        INCLUDE_TM_SIGLIB \
                        INCLUDE_TM_PGMGRLIB \
                        INCLUDE_TM_TASKOPEN \
                        INCLUDE_TM_MEMLIB \
                        INCLUDE_TM_MEMDRVLIB \
                        INCLUDE_TM_MEMPARTLIB \
                        INCLUDE_TM_POOLLIB \
                        INCLUDE_TM_PXTRACEEVENTID \
                        INCLUDE_TM_HOOKLIB \
                        INCLUDE_TM_WORKQLIB \
			INCLUDE_TM_WDCREATELIB \
			INCLUDE_TM_ANSITIME \
			INCLUDE_TM_TASKSHOW \
			INCLUDE_TM_PIPEDRV \
			INCLUDE_TM_PIPEDRV2 \
			INCLUDE_TM_TASKVARLIB \
			INCLUDE_TM_TASKINFO \
                        INCLUDE_TM_PMLIB \
			INCLUDE_TM_PRIINHERIT \
			INCLUDE_TM_SOCKLIB \
                        INCLUDE_TM_MEMPARTFUNC \
                        INCLUDE_TM_PTHREADLIB \
                        INCLUDE_TM_SHMLIB \
 		        INCLUDE_TM_WINDSTRESSTEST \
                        INCLUDE_TM_WINDKERNELLIB
                        INCLUDE_TM_HEAPSTRESSTEST \
                        INCLUDE_TM_MACROTEST \
                        INCLUDE_TM_PINGLIB \
                        INCLUDE_TM_ERRNOLIB_SMP
	DEFAULTS	INCLUDE_TM_BLIB \
			INCLUDE_TM_CLOCKLIB \
			INCLUDE_TM_MSGQLIB \
                        INCLUDE_TM_ANSICTYPE \
                        INCLUDE_TM_ANSISETJMP \
                        INCLUDE_TM_ANSISTDLIB \
                        INCLUDE_TM_IOBASICLIB \
                        INCLUDE_TM_LOGINLIB \
                        INCLUDE_TM_ENVLIB \
                        INCLUDE_TM_HASHLIB \
                        INCLUDE_TM_SEMSHOW \
                        INCLUDE_TM_SIGLIB \
                        INCLUDE_TM_PGMGRLIB \
                        INCLUDE_TM_TASKOPEN \
                        INCLUDE_TM_MEMLIB \
                        INCLUDE_TM_MEMDRVLIB \
                        INCLUDE_TM_MEMPARTLIB \
                        INCLUDE_TM_POOLLIB \
                        INCLUDE_TM_PXTRACEEVENTID \
                        INCLUDE_TM_HOOKLIB \
                        INCLUDE_TM_WORKQLIB \
			INCLUDE_TM_WDCREATELIB \
			INCLUDE_TM_ANSITIME \
			INCLUDE_TM_TASKSHOW \
			INCLUDE_TM_PIPEDRV \
			INCLUDE_TM_PIPEDRV2 \
			INCLUDE_TM_TASKVARLIB \
			INCLUDE_TM_TASKINFO \
                        INCLUDE_TM_PMLIB \
			INCLUDE_TM_PRIINHERIT \
			INCLUDE_TM_SOCKLIB \
                        INCLUDE_TM_MEMPARTFUNC \
                        INCLUDE_TM_PTHREADLIB \
                        INCLUDE_TM_SHMLIB \
			INCLUDE_TM_WINDSTRESSTEST \
                        INCLUDE_TM_WINDKERNELLIB
                        INCLUDE_TM_HEAPSTRESSTEST \
                        INCLUDE_TM_MACROTEST \
                        INCLUDE_TM_PINGLIB \
                        INCLUDE_TM_ERRNOLIB_SMP
       _CHILDREN	FOLDER_VXTESTV2_TESTS
}
