/* 25bundle_amp.cdf - VxWorks AMP OS bundles */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,08oct08,ebh  Fix name of INCLUDE_WRLOAD_IMAGE_BUILD
01b,02oct08,kab  WIND00137000 - remove _WRS_VX_AMP
01a,26aug08,mak  created
*/


Bundle BUNDLE_AMP_PRI {
    NAME	AMP Primary OS bundle
    SYNOPSIS	Kernel components required to build an AMP primary OS that can load and interact with AMP secondary OSs in a multicore processor used in an AMP configuration.  The primary OS provides wrload to load secondary OSs and uses MIPC to communciate with the secondary OSs.  It also provides WDB proxy support so that Workbench tools can connect through the primary OS to the secondary OSs.  In addition, MIPC serial devices are provided to enable the tip command to be used to connect to the secondary OSs.
    COMPONENTS  INCLUDE_MSD \
                INCLUDE_WDB_PROXY_MIPC \
                INCLUDE_WRLOAD
}

Bundle BUNDLE_AMP_SEC {
    NAME	AMP Secondary OS bundle
    SYNOPSIS	Kernel components required to build an AMP secondary OS that can be loaded via wrload from a primary OS in a multicore AMP configuration.  The resulting secondary OS image uses MIPC to interact with the primary OS and includes WDB over MIPC support and MIPC serial device support.
    COMPONENTS  INCLUDE_MSD \
                INCLUDE_WDB_COMM_MIPC \
                INCLUDE_WRLOAD_IMAGE_BUILD
}

