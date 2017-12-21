/* 02init_dsi.cdf - Component initialization file */

/* Copyright 2004-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,06jan05,bwa  removed xxSOCKLIB and NETBUFxxx from init group (SPR #104200).
01b,11nov04,job  Removed INCLUDE_SC_SOCKLIB
01a,20sep04,bwa  written.
*/

InitGroup usrDsiInit {
        INIT_RTN        usrDsiInit ();
        SYNOPSIS        DSI backend init group
        INIT_ORDER      INCLUDE_DSI_SOCKET      \
                        INCLUDE_DSI_POOL        \
                        INCLUDE_UN              \
                        INCLUDE_UN_COMP         \
                        INCLUDE_UN_SHOW         \
                        INCLUDE_UN_COMP_SHOW
}

