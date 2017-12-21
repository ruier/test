/* usrCustomScheduler.c - user-specified scheduler configuration */
                                                                                
/*
 * Copyright (c) 2005, 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,02jul10,cww  LP64 and general content update
01f,02oct09,zl   replaced qLib.h with qLibP.h
01e,29sep05,aeg  updated comments
01d,28sep05.kk   updated comments
01c,17sep05,jln  updated comments 
01b,16aug05,jln  more comments how to configure custom scheduler
01a,05aug05,jln  created
*/

/*
DESCRIPTION
This configlette configures and initializes a custom VxWorks task scheduler.
The custom task scheduler will be used to manage the set of tasks that are
in the 'READY' state, i.e. are eligible for execution.

A custom task scheduler consists of a multi-way queue class (Q_CLASS) to
manage the set of 'READY' tasks, and/or a hook routine executed during every
system clock tick. See the VxWorks Kernel Programmer's Guide for a
complete description of the VxWorks multi-way queue class, and how the
VxWorks kernel interacts with a queue class w.r.t. managing the set of
'READY' tasks.

CUSTOM Q_CLASS
A custom multi-way queue class can be specified to manage the set of 'READY'
tasks. A set of functions required by the Q_CLASS structure must be
implemented.  See the VxWorks Kernel Programmer Guide for more details
about the VxWorks kernel Q_CLASS structure and the requirements for managing
the set of 'READY' tasks.

There are several ways to link the definition and implementation of 
Q_NODE, Q_HEAD, and Q_CLASS structures to the VxWorks image. For example, 
this configlette file may be a placeholder for the Q_NODE/Q_HEAD type 
definitions and the custom Q_CLASS implementation. 

Another way is to create a new header file for the Q_NODE/Q_HEAD definition 
and a new source file for the Q_CLASS implementation. This file can then be 
added to the VxWorks image via the project facility as follows: 

\cs
	%cd <prjDir>
 	%vxprj file add qCustomLib.c
\ce
where <prjDir> is the project directory.

Alternatively, update the Makefile in the project to add the object directly:

\cs
 	EXTRA_MODULES = qCustomLib.o
\ce

INCLUDE_CUSTOM_SCHEDULER Component
The component INCLUDE_CUSTOM_SCHEDULER must be configured in order to utilize
a custom VxWorks task scheduler.  The INCLUDE_CUSTOM_SCHEDULER component
can be configured via the project facility tools, for example:
\cs
        %cd <prjDir>
        %vxprj component add INCLUDE_CUSTOM_SCHEDULER
\ce
where <prjDir> is the project directory.

When the component INCLUDE_CUSTOM_SCHEDULER is configured, this file will be 
pulled into the system image. Modification must be made to the 
usrCustomSchedulerInit() routine below to configure and initialize the custom
queuing library and any hook routine to be executed at each system clock tick 
interrupt. The following is an example of the usrCustomSchedulerInit() routine:

\cs
void usrCustomSchedulerInit (void)
    {
    /@ 
     @ Initialize readyQHead with the custom queue initialization routine.
     @ Typically this involves initializing a Q_HEAD structure with its Q_CLASS
     @ pointer, and possibly some internal structures.
     @/

    qCustomInit (&readyQHead, &qCustomClass);

    tickAnnounceHookAdd ((FUNCPTR) usrTickHook);
    kernelRoundRobinInstall();
    }
\ce

The readyQHead variable is a Q_HEAD structure used by VxWorks to access the 
set of 'READY' tasks. For custom schedulers, this variable can be initialized
as a custom Q_CLASS in usrCustomSchedulerInit() to customize the behaviour
of the VxWorks task scheduling algorithm.  The Q_CLASS structure definition
resides in $WIND_BASE/target/h/private/qLibP.h. It consists of pointers to
functions that manage the 'READY' tasks in the system, such as inserting,
removing, or resorting a task. Refer to the Custom Scheduler section of
the VxWorks Kernel Programmer's Guide for more details.

CLASS SPECIFIC Q_HEAD And Q_NODE
A custom multi-way queue class used to manage the set of READY tasks may need
class specific Q_NODE and Q_HEAD structures. The size of these structures
must not be more than 4 native words, ie. 16 bytes on 32-bit architectures and
32 bytes on 64-bit architectures. The following is an example of class specific
Q_NODE and Q_HEAD definitions:

\cs 
/@
 @ The first element of a DL_LIST structure is the 'head' of the doubly
 @ linked list. This is equivalent to the 'pFirstNode' field of the
 @ generic Q_HEAD structure.  Note that the DL structures are 2 words in size.
 @/

typedef struct     /@ Q_PRI_INV_HEAD @/
    {
    DL_LIST   dlList;  /@ 0x00/0x00 : doubly linked list @/
    ULONG     unused;  /@ 0x08/0x16 : unused field @/
    Q_CLASS * pQClass; /@ 0x0c/0x24 : pointer to queue class struct @/
    } Q_PRI_INV_HEAD;
\ce

\cs
typedef struct		/@ Q_PRI_INV_NODE @/
    {
    DL_NODE	node;	/@ 0x00/0x00 : priority doubly linked node @/
    ULONG	key;	/@ 0x08/0x10 : insertion key (i.e. priority) @/
    } Q_PRI_INV_NODE;	/@ sizeof (Q_PRI_INV_NODE) < sizeof (Q_NODE) @/
\ce

IMPLEMENTATION SPECIFIC DATA IN TASK CONTROL BLOCK
The <pSchedInfo> field in the task control block (TCB) is available for custom
schedulers that need to manage per-task implementation specific data.  This
field is of type 'void *'. There are two ways to access the <pSchedInfo> field:

\is
\i If a Q_NODE pointer is provided, the macro TASK_QNODE_TO_PSCHEDINFO(qNode)
may be used to get the address of <pSchedInfo>. The file 
$WIND_BASE/target/h/taskLib.h contains the definition of this macro. This macro
is typically used in the custom queue management functions. For example:

\cs   
void customQPut 
    ( 
    CUSTOM_Q_HEAD * pQHead, 
    CUSTOM_Q_NODE * pQNode, 
    ULONG           key
    )
    {
    void **	    ppSchedInfo;

    /@ get the address to the pSchedInfo @/

    ppSchedInfo = (void **) TASK_QNODE_TO_PSCHEDINFO (pQNode);
    }                                                                       
\ce

\i If a task ID <tid> is provided, the TASK_SCHED_INFO_SET(tid, pSchedInfo) 
macro can be used to set the <pSchedInfo> field in the TCB; and the macro 
TASK_SCHED_INFO_GET(tid) can be used to get the value of <pSchedInfo>. 
Both macros are defined in $WIND_BASE/target/h/taskUtilLib.h.
\ie

A custom scheduler implementation may use <pSchedInfo> as the pointer to an
implementation specific data structure. A custom scheduler implementation
is responsible for allocating the storage for the data structure. Typically,
the allocation is performed by using taskStackAllot() in a task create hook.
See the taskCreateHookAdd() reference entry regarding installing task create
hooks. If taskStackAllot() is used to allocate memory, then an explicit
deallocation is not required when the task terminates. However, if some
other API is utilized to allocate memory, then a task delete hook may be
required to deallocate the memory.

TICK HOOK ROUTINE
For custom schedulers that require operations to be performed at each
system clock tick, the tickAnnounceHookAdd() API is provided to register a hook
that will be invoked by the kernel's system clock tick handler. The hook
routine will be called in an ISR context, and thus any VxWorks kernel
APIs that are prohibited for ISRs, are also prohibited for tick announce hooks.
Refer to the VxWorks Kernel Programmer's Guide regarding restrictions for
ISRs. Below is a sample on the usage of the hook routine:

\cs
void usrTickHook 
    (
    TASK_ID tid
    )
    {
    /@ 
     @ update statistics information if needed;
     @ update interrupted task's time slice if needed;
     @ resort the interrupted task location in the ready queue if needed
     @/
    }
\ce

VxWorks provides a round robin policy implementation via the hook facility
kernelRoundRobinHook(). This hook checks that (1) the interrupted task has not 
locked preemption, (2) the interrupted task is still in the READY state, and 
(3) the interrupted task has consumed its allowed time slice. If all 3
conditions are true, then the interrupted task's time slice is reset, and  
a Q_REMOVE() operation followed by a Q_PUT() operation are performed to remove
the interrupted task from the task list and insert it back again at the same
priority level. This results in putting the interrupted task to the tail
of the task list with its priority level if the VxWorks native scheduler
is used.

Another VxWorks routine, kernelTimeSlice(int ticks), is used to dynamically
enable or disable round robin scheduling and to adjust the system time
slice. 

A custom scheduler may choose to use the kernelRoundRobinHook() routine to 
perform round robin scheduling if the above described behaviour is 
appropriate. To take advantage of the VxWorks's implementation of round robin 
scheduling, the function kernelRoundRobinInstall() must be called in 
usrCustomSchedulerInit() to install the kernelRoundRobinHook(). The routine
_func_kernelRoundRobinHook() must then be called within the tick hook
for the round robin policy to take effect. The _func_kernelRoundRobinHook() 
takes a <tid>, the task id of the interrupted task, as the argument. Finally,
kernelTimeSlice should be called to initialize the system time slice and enable
round robin scheduling.  Here is example code showing how to take advantage of
the VxWorks round robin scheduling scheme:

\cs
void usrTickHook
    (
    TASK_ID tid 
    )
    {
    /@ statistic information @/

    userTick++;

    /@ call kernelRoundRobinHook @/

    if (_func_kernelRoundRobinHook != NULL)
        _func_kernelRoundRobinHook (tid);

    /@ other work @/
    ...
    }
\ce

If the custom scheduler does not implement round robin policy nor use 
the kernelRoundRobinHook() facility for the round robin implementation, the 
routine kernelTimeSlice() MUST NOT be used to adjust system time slice nor 
enable or disable round robin policy.

CAVEATS
The scheduler is fundamental to a system's behavior. The system behavior is
not guaranteed if a custom scheduler is configured. Users shall take all 
responsibility to ensure all VxWorks components behave as expected when the 
custom scheduler is deployed.  Moreover, as a fundamental subsystem of the
VxWorks kernel, the scheduler interface is subject to change at any time.
Users should make no assumptions regarding compatibility of the custom
scheduler interface across different versions of VxWorks.

Custom schedulers are not supported on VxWorks SMP.
   
SEE ALSO: kernelRoundRobinInstall(), kernelTimeSlice(), tickAnnounceHookAdd(),
The VxWorks Programmer's Guide
*/

#include <vxWorks.h>
#include <kernelLib.h>
#include <private/qLibP.h>
#include <private/readyQLibP.h>

/*******************************************************************************
*
* usrCustomSchedulerInit - configure and initialize a custom task scheduler
*
* This function configures and initializes a custom task scheduler. 
* This routine will only take effect when the INCLUDE_CUSTOM_SCHEDULER
* is configured.
*
* A hook may be added to perform additional operations at each tick interrupt.
* 
* RETURNS: N/A
*
* \NOMANUAL
*/

void usrCustomSchedulerInit (void)
    {
#error INCLUDE_CUSTOM_SCHEDULER requires the initalization of readyQHead!
    }
