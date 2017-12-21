/* 01bundle_rtp_posix_pse52.cdf - VxWorks POSIX PSE52 RTP component bundles */

/* Copyright 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01e,09jul06,tcr  add INCLUDE_POSIX_TRACE
01d,08jul06,jln  added INCLUDE_PX_SCHED_SPORADIC_POLICY
01c,10may06,tfo  removed FOLDER_BUNDLES
01b,14feb06,jln  added INCLUDE_POSIX_THREAD_CPUTIME
01a,18jan06,pad   written
*/

/*
DESCRIPTION
This bundle brings all the kernel components that are required to support the
POSIX PSE52 profile in RTPs.
*/

Bundle BUNDLE_RTP_POSIX_PSE52 {
    NAME        POSIX PSE52 functionality for RTPs
    SYNOPSIS	Kernel components required for POSIX PSE52 support in RTPs
    COMPONENTS  INCLUDE_RTP_POSIX_PSE52 \
		INCLUDE_POSIX_ADVISORY_FILE_LOCKING \
		INCLUDE_POSIX_CLOCKS \
                INCLUDE_POSIX_TIMERS \
                INCLUDE_POSIX_MQ \
                INCLUDE_POSIX_SEM \
                INCLUDE_POSIX_PTHREAD_SCHEDULER \
		INCLUDE_PX_SCHED_SPORADIC_POLICY \
		INCLUDE_POSIX_THREAD_CPUTIME \
		INCLUDE_POSIX_SHM \
		INCLUDE_POSIX_MAPPED_FILES \
                INCLUDE_POSIX_TRACE \
		INCLUDE_VRFS \
		INCLUDE_HRFS \
		INCLUDE_HRFS_ACCESS_TIMESTAMP \
		INCLUDE_HRFS_FORMAT
}


