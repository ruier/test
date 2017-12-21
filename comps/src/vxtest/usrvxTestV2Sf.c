/* usrvxTestV2Sf.c - VxTestV2 small footprint driver configlette */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,11sep09,d_c  Defect WIND00179238 Fix WIND00181447
                 Run sf driver in a task.
01b,21apr09,d_c  Change entry/exit messages to INFO messages
01a,01apr09,d_c  Written
*/

/*
DESCRIPTION
  
This configlette makes calls into the small footprint driver module
(vxTestV2Sf.c) to emulate the actions of the normal, large footprint
driver (vxTestV2Int.c). This configlette works in conjunction with the
mkSfDriverCalls.tcl script, which generates the include file
(sfDriverCalls.c) that contains all the externs and calls to
run the given test suite.
  
INCLUDE FILES: 
  
SEE ALSO: mkSfDriverCalls.tcl, vxTestV2Sf.c, 01vxTestV2.cdf
*/

/* Disable LDA for this module. Test case functions are externed
 * as weak symbols. This conflicts with enabling SDA code generation,
 * which in turn produces a warning from the compiler. Disabling LDA
 * supresses the warning.
 */
#ifdef _DIAB_TOOL
#pragma option -Xlocal-data-area=0
#endif
   
/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include <vxTestV2Sf.h>

/* defines */
   
#define VXTESTV2SF_TASK_PRIORITY    100
#define VXTESTV2SF_TASK_OPTIONS     0
#define VXTESTV2SF_TASK_STACK_SIZE  4096
   
/* locals */
   
LOCAL void vxTestV2SfTask (void);
VX_TASK(vxTestV2SfTask, VXTESTV2SF_TASK_STACK_SIZE);

/***************************************************************************
* usrvxTestV2SfRun - initialize driver and run a small footprint test suite
*
* This function initializes the small footprint driver and spawns
* the small footprint dirver task.
*
* This function is specified as the INIT_RTN for the
* INCLUDE_VXTESTV2_SF_DRIVER component. It is normally called by
* usrRoot in the context of the user root task.
* 
* RETURNS: N/A
*
* \NOMANUAL
*/

void usrvxTestV2SfRun
    (
    PUTCHAR_PTR pPutChar,       /* ptr to character output fcn */
    int         putCharIsRaw    /* true if function does not add \r to \n */
    )
    {
    int vxTestV2SfTaskId;
    
    sfDriverInit (pPutChar, putCharIsRaw);

    vxTestV2SfTaskId = VX_TASK_INSTANTIATE(vxTestV2SfTask, \
					   VXTESTV2SF_TASK_PRIORITY, \
					   VXTESTV2SF_TASK_OPTIONS, \
					   VXTESTV2SF_TASK_STACK_SIZE, \
					   vxTestV2SfTask, \
					   0,1,2,3,4,5,6,7,8,9);

    if (vxTestV2SfTaskId == ERROR)
	sfPrintf ("HEXC vxTestV2SfRun(): "
		  "Creation of vxTestV2 small footprint task failed\n");
    }


/***************************************************************************
* vxTestV2SfTask - small footprint driver task
*
* This task calls all the test cases that have been built into the
* small footprint test image by bspBuildTest.tcl and mkSfDriverCalls.tcl.
* 
* RETURNS: N/A
*
* \NOMANUAL
*/
LOCAL void vxTestV2SfTask (void)
    {
    sfPrintf ("INFO vxTestV2SfTask(): "
	      "Entering vxTestV2 small footprint driver\n");
    
    /* The following code is coupled to bspBuildTest.tcl.
     * bspBuildTest.tcl generates the sfDriverCalls.c include file in
     * the project directory by invoking mkSfDriverCalls.tcl, and adds
     * the INCLUDE_VXTESTV2_SF_DRIVER_CALLS component to enable the
     * inclusion of the file.
     */
    
#ifdef INCLUDE_VXTESTV2_SF_DRIVER_CALLS
#include "sfDriverCalls.c"
#endif
    
    sfPrintf ("INFO usrvxTestV2SfRun(): "
	      "Exiting vxTestV2 small footprint driver\n");
    
    }



