/* 02ssi_dsi.cdf - SSI config file for DSI component */

/* Copyright 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01e,04mar06,xli correct the cdf syntax errot
01d,18feb06,mze change to SELECT_DSI_INIT and  added  INCLUDE_DSI_POOL as a REQUIRES
01c,30jan06,xli use selection clause
01b,19jan06,xli Added INIT_RTN clause
01a,04jan06,tkf  written
*/

/* 
DESCRIPTION   This file contains the SSI configuration file for the DSI component. The
purpose of the file is to allow the SSI system co-exist with the legacy
VxWorks initialization system. NAME            DSI Initialization Selection
*/

Selection SELECT_DSI_INIT {
        NAME            DSI Initialization
        SYNOPSIS        Initialize DSI Component
        COUNT           1-1
        CHILDREN        INCLUDE_DSI_DYNAMIC_INIT \
                        INCLUDE_DSI_STATIC_INIT
        _CHILDREN       FOLDER_DSI_BACKEND                        
        DEFAULTS        INCLUDE_DSI_STATIC_INIT
}
        
Component INCLUDE_DSI_DYNAMIC_INIT {
        NAME         DSI Dynamic Initialization
        SYNOPSIS     Initialize DSI Dynamically
        HDR_FILES    ssi/ssm.h
        MODULES      ssm.o
        CFG_PARAMS   SSI_DSI_NAME SSI_DSI_DEPENDON \
                     SSI_DSI_INIT_ROUTINE \
                     SSI_DSI_ARGS \
                     SSI_DSI_OPTIONS
        INIT_RTN     ssmCompWait (SSI_DSI_NAME);             
        REQUIRES     INCLUDE_SSI \
                     INCLUDE_CNS  \
                     INCLUDE_DSI_POOL
        EXCLUDES     INCLUDE_DSI_STATIC_INIT
}

Component INCLUDE_DSI_STATIC_INIT {
    NAME                DSI Static Initialization
    SYNOPSIS            Initialize DSI Statically
    CONFIGLETTES        ssi/ssiDsi.c
    INIT_RTN            usrDsiInit2 ();
    REQUIRES            INCLUDE_DSI_POOL
    EXCLUDES            INCLUDE_DSI_DYNAMIC_INIT
}

Parameter SSI_DSI_NAME {
        NAME		DSI component name
	SYNOPSIS	DSI component name
	TYPE		string
	DEFAULT		"dsi"
}

Parameter SSI_DSI_DEPENDON {
        NAME		DSI depend on:
	SYNOPSIS	DSI depend on list
	TYPE		string
        DEFAULT         NULL
                 
}


Parameter SSI_DSI_INIT_ROUTINE {
        NAME		DSI init routine
	SYNOPSIS	DSI init routine name
	TYPE		long
	DEFAULT		usrSsiDsiInit
}

Parameter SSI_DSI_ARGS {
        NAME		DSI init routine arguments
	SYNOPSIS	DSI init routine arguments
	TYPE		string
        DEFAULT         NULL
			
}

Parameter SSI_DSI_OPTIONS {
        NAME		DSI init options
	SYNOPSIS	DSI component init options
	TYPE		string
        DEFAULT         NULL    
			
}
 

