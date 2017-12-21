/* 00folder_ssi.cdf - SSI folder description file */

/* Copyright 2006, 2011 Wind River Systems, Inc. */

/* 
modification history 
--------------------
01c,20dec11,shi  fix for WIND00317123, disable SSI for LP64. (currently doesn't support 64bit system)
01b,30jan06,xli  move CNS to this folder
01a,02dec06,tkf  written.
*/

/*

DESCRIPTION
  This file contains descriptions for the SSI folder.

*/

#if (!defined _WRS_CONFIG_LP64)
Folder FOLDER_SSI
	{
	NAME		Startup Sequence and Initialization Components
	SYNOPSIS	Startup Sequence and Initialization Components
	CHILDREN	INCLUDE_SSI \
                        INCLUDE_CNS
	DEFAULTS	INCLUDE_SSI \
                        INCLUDE_CNS
	}
#endif

