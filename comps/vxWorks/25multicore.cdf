/* 01multicore.cdf - VxWorks Multicore AMP folder and components */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,04oct07,mze  fix for WIND00104829
01a,14aug07,mze  moved here from config/hpcNet8641
*/

#ifdef _WRS_VX_AMP


/********************************************************************************
* Asynchronous Multiprocessor mode Options Folder
*
*/
Folder FOLDER_AMP {
    NAME           AMP
    SYNOPSIS       Asynchronous Multiprocessor mode build options
    CHILDREN       INCLUDE_AMP \
                   INCLUDE_AMP_SM_END_AUTO
    _CHILDREN      FOLDER_HARDWARE
}
                        
/********************************************************************************
* Asynchronous Multiprocessor mode - Two vxWorks kernels boot one for each core
*
*/	
Component INCLUDE_AMP {
    NAME            Asynchronous Multiprocessor mode
    SYNOPSIS        vxWorks images running each processor 
    }


/********************************************************************************
* Asynchronous Multiprocessor mode with Shared memory END 
* Two vxWorks kernels boot one for each core commincating via shared 
* memory network.
*
*/
Component INCLUDE_AMP_SM_END_AUTO {
    NAME            Asynchronous Multiprocessor mode with shared memory END
    SYNOPSIS        vxWorks images running each processor with SM network
    EXCLUDES        INCLUDE_WDB_COMM_END
    REQUIRES        INCLUDE_AMP \
                    INCLUDE_SM_COMMON \
                    INCLUDE_SMEND \
                    INCLUDE_SM_NET \
                    INCLUDE_SM_NET_SHOW \
                    INCLUDE_NET_DRV \
                    INCLUDE_WDB_COMM_NETWORK
}

#endif
