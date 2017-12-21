/* usrvxTestV2AutoRun.c - Launch vxTestV2 on bootup */

/* Copyright (c) 2006 Wind River Systems, Inc. 
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01b,03jul06,jmt  Add Parameters to INCLUDE_VXTESTV2_AUTORUN
01a,28jun06,jmt  written 
*/

/*
DESCRIPTION
This file is used to launch a vxTestV2 set of tests on powerup.
The image must be built with the tests and itc file embedded.
*/ 

/* includes */

#ifdef INCLUDE_TASK_CREATE_DELETE
#include <taskLib.h>
#endif

/* externals */


/******************************************************************************
*
* usrvxTestV2AutoRun - Launch a vxTestV2 set of tests.
*
* This routine launches the vxTestV2 driver in embedded mode
* 
* RETURNS: N/A.
*
* ERRNO:   N/A.
*
* SEE ALSO: 
* .pG ""
*
*/

void usrvxTestV2AutoRun(void)
    {
    /* if we can spawn, do so.  Otherwise just run it */

#ifdef INCLUDE_TASK_CREATE_DELETE
    /* spawn off a task to run the tests */

    taskSpawn("vxTestV2",
              VXTESTV2_AUTORUN_TASK_PRIORITY,
              VXTESTV2_AUTORUN_TASK_OPTIONS,
              VXTESTV2_AUTORUN_TASK_STACK_SIZE,
              (FUNCPTR) vxTestV2,
              (int) "-em -v 4", /* arg1 */
              0,0,0,0,0,0,0,0,0 /* args 2-10 */
             );

#else
    /* run the command directly */

    vxTestV2("-em -v 4"); 
#endif
    }
