/* 01bundle_rtp.cdf - VxWorks RTP component bundles */

/*
 * Copyright (c) 2004, 2006-2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,19nov07,rlp  Added INCLUDE_WDB_TSFS component to the RTP_DEVELOP bundle
                 (CQ:WIND0037020).
01d,10may06,tfo  removed FOLDER_BUNDLES
01c,23sep04,kk   added INCLUDE_RTP_SHOW_SHELL_CMD
01b,21jun04,kk   fix SPR# 98379 - renamed RTP_APPL to RTP_APPL_USER,
                 added SHL components * SHARED_DATA_SHOW component
01a,14apr04,job  written
*/

Bundle BUNDLE_RTP_DEVELOP {
    NAME	Real Time Process Development
    SYNOPSIS	Kernel components required for RTP development
    COMPONENTS	INCLUDE_RTP \
		INCLUDE_RTP_APPL_USER \
		INCLUDE_RTP_SHOW \
		INCLUDE_RTP_HOOKS \
		INCLUDE_SHARED_DATA \
		INCLUDE_SHARED_DATA_SHOW \
		INCLUDE_SHL \
		INCLUDE_SHL_SHOW \
		INCLUDE_RTP_SHELL_CMD \
		INCLUDE_RTP_SHOW_SHELL_CMD \
		INCLUDE_SHL_SHELL_CMD \
		INCLUDE_WDB_TSFS
}

Bundle BUNDLE_RTP_DEPLOY {
    NAME	Real Time Process Deployment
    SYNOPSIS	Kernel components required for RTP deployment
    COMPONENTS	INCLUDE_RTP \
		INCLUDE_RTP_APPL_USER \
		INCLUDE_RTP_HOOKS \
		INCLUDE_SHARED_DATA \
		INCLUDE_SHL
}

