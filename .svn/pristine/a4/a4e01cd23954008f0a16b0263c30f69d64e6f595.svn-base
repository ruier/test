#ifdef _WRS_CONFIG_LP64
#ifndef _WRS_CONFIG_SMP
Component INCLUDE_TASK_VARS_TEST {
        NAME            task variables for 64bit
        SYNOPSIS        allow global variables to be made private to a task
        SOURCE          $(TGT_DIR)/src/os
        MODULES         taskVarLib.o
        LINK_SYMS       __taskVarAdd
        HDR_FILES       taskVarLib.h
        REQUIRES        INCLUDE_KERNEL            \
                        INCLUDE_SEM_DELETE        \
                        INCLUDE_SEM_MUTEX         \
                        INCLUDE_TASK_CREATE_HOOKS \
                        INCLUDE_TASK_SWITCH_HOOKS \
                        INCLUDE_MEM_MGR_BASIC
}
#endif
#endif

