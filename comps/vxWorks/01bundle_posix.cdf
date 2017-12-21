/* 01bundle_posix.cdf - VxWorks POSIX component bundles */

/*
 * Copyright (c) 2004-2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,10may06,tfo  removed FOLDER_BUNDLES
01d,09feb06,jlk  removed INCLUDE_POSIX_GENERAL
01c,03feb06,jlk  added INCLUDE_POSIX_GENERAL, fixed up copyright
01b,12jan06,act  add component INCLUDE_POSIX_ADVISORY_FILE_LOCKING
01a,14apr04,job  written
*/

Bundle BUNDLE_POSIX {
    NAME        POSIX functionality
    SYNOPSIS	Kernel components required for full POSIX support
    COMPONENTS  INCLUDE_POSIX_ADVISORY_FILE_LOCKING \
                INCLUDE_POSIX_AIO \
                INCLUDE_POSIX_AIO_SYSDRV \
		INCLUDE_POSIX_CLOCKS \
                INCLUDE_POSIX_FTRUNC \
                INCLUDE_POSIX_MEM \
                INCLUDE_POSIX_MQ \
                INCLUDE_POSIX_PTHREADS \
                INCLUDE_POSIX_SCHED \
                INCLUDE_POSIX_SEM \
                INCLUDE_POSIX_SIGNALS \
                INCLUDE_POSIX_TIMERS \
                INCLUDE_POSIX_DIRLIB
}


