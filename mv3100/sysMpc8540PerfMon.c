/* sysMpc8540PerfMon.c - Performance Monitor */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
o1b,22sep05,pjh  Fix compiler warnings.
01a,31may05,efb  NewFile.
*/


/*

DESCRIPTION

The performance monitor allows various cyclical system functions to
be counted. Up to nine events can be counted at one time using
configurable counter registers. The performance monitor driver and API
provides a convenient way to manage event monitoring through the use
of custom event monitors that are created for each desired event
needing monitoring.

Because there are approximately 300 events that can be chosen for
monitoring with only 9 available counters many events are mutually
exclusive for monitoring. Many events are specifically assigned to
particular counters when in use and cannot be changed. Other events
can be assigned to any available counter resource. The performance
monitor driver manages this resource contention and allocation.

New monitor objects are created via the "create" function in an
initially suspended state. Monitors can be created using any number
of free counters to increase the available counting bit width. Using
more counters to cover a particular event, reduces the number of
events that can be monitor simultaneously. As many event monitors as
desired can be created, one monitor per event.

While suspended, no counter resources are reserved or assigned to
the monitor. A monitor can be suspended as required by the application
at any time. A suspended monitor can be further configured with
burst characteristics, threshold, scaling, interrupt enable, etc.
When a monitor becomes suspended, its last count value is saved and
can be used as the starting value when monitor activity is resumed.

To reserve and assign counter register resources, the monitor must be
made active. The number of active monitors is limited by the
available resources up to a maximum of 9. The "activate"
routine places the monitor into the active state with all needed
resources reserved and configured. If the required resources are
in use by other monitors, specifying "override" will automatically
suspend those monitors to free their resources for use by the newly
activately monitor. Monitors can lock resources to prevent automatic
suspension. Locked monitors must be explicity suspended by their
creating tasks to free their resources. If no resources are available
due to being locked or no desire to override, an error condition is
returned, and the monitor remains suspended and unchanged.

Actual monitoring is started after successful activation by
explicitly calling the "go" function. Monitoring can be stopped
by calling the "stop" function. An active stopped monitor still
has its resources allocated, but can be reconfigured safely.
A started monitor will accumulate a count of its assigned event
as it occurs according to its configuration parameters.

Event monitoring for a particular event can be triggered to start
and/or stop when linked trigger event monitors change count or
overflow. To create a triggered event monitor, first create the
event monitor that will be triggered. Then create a start and/or
stop event monitor for the events that will monitored and cause
the start and/or stop trigger for the first monitor. Set the
initial values of all monitors using the "reset" function, if
necessary. Then use the "trigger set" function to associate the
triggering monitors to the triggered monitor. To conveniently start
a triggered monitor and its associated monitors use the "trigger go"
function on the triggered monitor.

Once created, an event monitor can be configured to count event
bursts. Burst size, granularity, and burst group spacing can
be specified. A threshold can also be specified that must be
exceeded before an event is counted. The threshold and burst
spacing can be scaled by specifying a multiplier. Functions are
provided to easily set or modify these values for any event monitor.
These properties do not necessarily apply to all events at all
times. The application must determine when their usage makes sense 
for a particular event.

An interrupt service routine may be optionally provided using the
interrupt "connect" routine. The service routine is invoked whenever
an interrupt enabled event monitor overflows. The routine can be
connected with the option of freezing all event counters at the
time of interrupt. The interrupting monitors are cleared after the user
handler returns and before the service routine completes. If counters
were frozen, the user routine or application must unfreeze the counters
to resume monitoring.

To ensure the best allocation of counter resources, events that can
be monitored/counted by "any" counter (per the enumeration list), should
be activated after all fixed counter event monitors are activated.

*/

/* includes */

#include "vxWorks.h"
#include "semLib.h"
#include "stdlib.h"
#include "stdio.h"
#include "intLib.h"
#include "config.h"
#include "mpc8540.h"
#include "sysEpic.h"
#include "sysMpc8540PerfMon.h"


/* defines */

#define PERFMON_NOT_CHAINED     0xffff
#define PERFMON_NOT_USED        0xff
#define PERFMON_REG_INTRVL      0x10    /* Register address interval */
#define PERFMON_MIN_BIT_WIDTH   32      
#define PERFMON_MAX_BIT_WIDTH   (PERFMON_MIN_BIT_WIDTH *      \
                                 (PERFMON_NUM_COUNTERS - 1))

#define PERFMON_CNTR0_MIN_BIT_WIDTH 64
#define PERFMON_CNTR0_MAX_BIT_WIDTH (PERFMON_MAX_BIT_WIDTH + \
                                     PERFMON_CNTR0_MIN_BIT_WIDTH)

#define PERFMON_BASE_CNTR_INDEX     0      

/* Permformance monitor bit definitions and masks */

#define PERFMON_GLOBAL_FAC_BIT       0
#define PERFMON_GLOBAL_FAC_MASK      (0x80000000UL >> PERFMON_GLOBAL_FAC_BIT)

#define PERFMON_GLOBAL_PMIE_BIT      1
#define PERFMON_GLOBAL_PMIE_MASK     (0x80000000UL >> PERFMON_GLOBAL_PMIE_BIT)

#define PERFMON_GLOBAL_FCECE_BIT     2
#define PERFMON_GLOBAL_FCECE_MASK    (0x80000000UL >> PERFMON_GLOBAL_FCECE_BIT)

#define PERFMON_LOCAL_A0_FC_BIT      0
#define PERFMON_LOCAL_A0_FC_MASK     (0x80000000UL >> PERFMON_LOCAL_A0_FC_BIT)

#define PERFMON_LOCAL_A0_CE_BIT      5
#define PERFMON_LOCAL_A0_CE_MASK     (0x80000000UL >> PERFMON_LOCAL_A0_CE_BIT)

#define PERFMON_LOCAL_A1_FC_BIT      0
#define PERFMON_LOCAL_A1_FC_MASK     (0x80000000UL >> PERFMON_LOCAL_A1_FC_BIT)

#define PERFMON_LOCAL_A1_CE_BIT      5
#define PERFMON_LOCAL_A1_CE_MASK     (0x80000000UL >> PERFMON_LOCAL_A1_CE_BIT)

#define PERFMON_LOCAL_A1_EVENT_BIT   9
#define PERFMON_LOCAL_A1_EVENT_MASK  (0xFE000000UL >> \
                                      PERFMON_LOCAL_A1_EVENT_BIT)
#define PERFMON_LOCAL_A1_EVENT_WIDTH 7

#define PERFMON_LOCAL_A1_BSIZE_BIT   16
#define PERFMON_LOCAL_A1_BSIZE_MASK  (0xF8000000UL >> \
                                      PERFMON_LOCAL_A1_BSIZE_BIT)
#define PERFMON_LOCAL_A1_BSIZE_WIDTH 5

#define PERFMON_LOCAL_A1_BGRAN_BIT   21
#define PERFMON_LOCAL_A1_BGRAN_MASK  (0xF8000000UL >> \
                                      PERFMON_LOCAL_A1_BGRAN_BIT)
#define PERFMON_LOCAL_A1_BGRAN_WIDTH 5

#define PERFMON_LOCAL_A1_BDIST_BIT   26
#define PERFMON_LOCAL_A1_BDIST_MASK  (0xFC000000UL >> \
                                      PERFMON_LOCAL_A1_BDIST_BIT)
#define PERFMON_LOCAL_A1_BDIST_WIDTH 6

#define PERFMON_LOCAL_B0_TRIGONSEL_BIT     2
#define PERFMON_LOCAL_B0_TRIGONSEL_MASK    (0xF0000000UL >> \
                                            PERFMON_LOCAL_B0_TRIGONSEL_BIT)
#define PERFMON_LOCAL_B0_TRIGONSEL_WIDTH   4

#define PERFMON_LOCAL_B0_TRIGOFFSEL_BIT    8
#define PERFMON_LOCAL_B0_TRIGOFFSEL_MASK   (0xF0000000UL >> \
                                            PERFMON_LOCAL_B0_TRIGOFFSEL_BIT)
#define PERFMON_LOCAL_B0_TRIGOFFSEL_WIDTH  4
 
#define PERFMON_LOCAL_B0_TRIGONCNTL_BIT    12
#define PERFMON_LOCAL_B0_TRIGONCNTL_MASK   (0xC0000000UL >> \
                                            PERFMON_LOCAL_B0_TRIGONCNTL_BIT)
#define PERFMON_LOCAL_B0_TRIGONCNTL_WIDTH  2

#define PERFMON_LOCAL_B0_TRIGOFFCNTL_BIT   14
#define PERFMON_LOCAL_B0_TRIGOFFCNTL_MASK  (0xC0000000UL >> \
                                            PERFMON_LOCAL_B0_TRIGOFFCNTL_BIT)
#define PERFMON_LOCAL_B0_TRIGOFFCNTL_WIDTH 2

#define PERFMON_LOCAL_B1_TRIGONSEL_BIT     2
#define PERFMON_LOCAL_B1_TRIGONSEL_MASK    (0xF0000000UL >> \
                                            PERFMON_LOCAL_B1_TRIGONSEL_BIT)
#define PERFMON_LOCAL_B1_TRIGONSEL_WIDTH   4

#define PERFMON_LOCAL_B1_TRIGOFFSEL_BIT    8
#define PERFMON_LOCAL_B1_TRIGOFFSEL_MASK   (0xF0000000UL >> \
                                            PERFMON_LOCAL_B1_TRIGOFFSEL_BIT)
#define PERFMON_LOCAL_B1_TRIGOFFSEL_WIDTH  4

#define PERFMON_LOCAL_B1_TRIGONCNTL_BIT    12
#define PERFMON_LOCAL_B1_TRIGONCNTL_MASK   (0xC0000000UL >> \
                                            PERFMON_LOCAL_B1_TRIGONCNTL_BIT)
#define PERFMON_LOCAL_B1_TRIGONCNTL_WIDTH  2

#define PERFMON_LOCAL_B1_TRIGOFFCNTL_BIT   14
#define PERFMON_LOCAL_B1_TRIGOFFCNTL_MASK  (0xC0000000UL >> \
                                           PERFMON_LOCAL_B1_TRIGOFFCNTL_BIT)
#define PERFMON_LOCAL_B1_TRIGOFFCNTL_WIDTH 2

#define PERFMON_LOCAL_B1_TBMULT_BIT        21
#define PERFMON_LOCAL_B1_TBMULT_MASK       (0xE0000000UL >> \
                                            PERFMON_LOCAL_B1_TBMULT_BIT)
#define PERFMON_LOCAL_B1_TBMULT_WIDTH      3
 
#define PERFMON_LOCAL_B1_THRESHOLD_BIT     26
#define PERFMON_LOCAL_B1_THRESHOLD_MASK    (0xFC000000UL >> \
                                            PERFMON_LOCAL_B1_THRESHOLD_BIT)
#define PERFMON_LOCAL_B1_THRESHOLD_WIDTH   6

/* typedefs */

typedef struct counterResource
    {
    EVENT_MONITOR *activeMonitor;  /* The event monitor using the counter */
    UINT32         localCtrlRegA;  /* Offset to counter's control A reg */
    UINT32         localCtrlRegB;  /* Offset to counter's control B reg */
    UINT32         counterReg;     /* Offset to counter register */
    UINT32         cntrUpperReg;   /* Offset to counter zero's MSW register */
    BOOL           isLocked;       /* True when no override allowed */
    } COUNTER_RESOURCE;
    
/* forward declarations */

LOCAL COUNTER_RESOURCE eventCounters[PERFMON_NUM_COUNTERS];

LOCAL EVENT_MONITOR *activeListHead = NULL;
LOCAL EVENT_MONITOR *suspendListHead = NULL;

LOCAL SEM_ID  semProt = NULL;           /* Perf monitor protection semaphore */
LOCAL BOOL    pmIsInitialized = FALSE;  /* Allow initialization only once */

LOCAL void (*userHandler) (int) = NULL;  /* User-supplied interrupt handler */

/* external references */

/**************************************************************************    
*
* eventSwapToOtherList - move the event monitor to the other list
*
* This routine moves the given event monitor from the active list to the
* suspend list or from the suspend list to the active list depending on
* to which list the monitor is aleady linked. If deleteOnly is specified
* as true, the monitor is removed from whichever list only.
*
* RETURNS: N/A
*/

LOCAL void eventSwapToOtherList
    (
    EVENT_MONITOR *eventMonitor,   /* Move this event monitor */
    BOOL           deleteOnly      /* Delete from current list only */
    )
    {
    EVENT_MONITOR **destList;  

    /* Remove from current list */

    if (eventMonitor->isActive)
        {
        if (activeListHead == eventMonitor)
            {
            activeListHead = eventMonitor->nextEvent;
            }

        destList = &suspendListHead;
        }
    else
        {
        if (suspendListHead == eventMonitor)
            {
            suspendListHead = eventMonitor->nextEvent;
            }

        destList = &activeListHead;
        }

    if (eventMonitor->nextEvent != NULL)
        {
        eventMonitor->nextEvent->prevEvent = eventMonitor->prevEvent;
        } 

    if (eventMonitor->prevEvent != NULL)
        {
        eventMonitor->prevEvent->nextEvent = eventMonitor->nextEvent;
        }
    
    if (!deleteOnly)
        {
        if (*destList != NULL)
            {
            (*destList)->prevEvent = eventMonitor;
            }

        eventMonitor->nextEvent = *destList;
        eventMonitor->prevEvent = NULL;

        *destList = eventMonitor;
        eventMonitor->isActive = !eventMonitor->isActive;
        }
    else
        {
         eventMonitor->isActive = FALSE;
        }
    }

/**************************************************************************    
*
* eventOverflowIntHandle - interrupt handler and wrapper function
*
* This routine invokes any user supplied interrupt handler function. 
* An interrupt occurs when counter is so enabled and overflows
* (i.e. its msb becomes set). If all counters were frozen on interrupt,
* it is up to the user routine or application to invoke perfMonUnfreezaAll
* to resume monitoring. All interrupting and overflowing monitors are 
* cleared to zero after returning from the user routine.
*
* RETURNS: N/A
*/

LOCAL void eventOverflowIntHandle
    (
    int parameter    /* Parameter to be passed onto user routine */
    )
    {
    EVENT_MONITOR *eventMonitor = activeListHead;

    if (userHandler != NULL)
        {
        (*userHandler) (parameter);
        }

    while (eventMonitor != NULL)
        {
        if (eventMonitor->isIntEnabled)
            {
            if (perfMonEventMonitorHasOverflowed (eventMonitor))
                {
                perfMonEventMonitorReset (eventMonitor, NULL);
                }
            }

        eventMonitor = eventMonitor->nextEvent;
        }
    }

/**************************************************************************    
*
* eventTriggerSet - set trigger counters
*
* This routine sets the start/stop triggers for the given event monitor. 
* The start and stop triggers are other event monitors that will, upon 
* overflow or count change, trigger the given event monitor to start/stop
* counting. The trigger control specifies no trigger, count change, or 
* overflow. 
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null.
*/

LOCAL void eventTriggerSet
    (
    EVENT_MONITOR *eventMonitor  /* Set triggers for this monitor */
    )
    {
    EVENT_MONITOR *trigEvent;
    UINT32         regVal = 0;
    UINT32         counter = 0;
    UINT32         trigger = 0;

    counter = eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX];
    regVal = CCSR_READ32(CCSBAR, eventCounters[counter].localCtrlRegB);
    regVal &= ~PERFMON_LOCAL_B0_TRIGONCNTL_MASK;
    regVal &= ~PERFMON_LOCAL_B0_TRIGOFFCNTL_MASK;

    if (eventMonitor->trigOnEvent != NULL && eventMonitor->trigOnCtrl !=
        PERFMON_TRIGCNTL_DISAB)
        {
 
        /* Get the most significant counter register */

        trigEvent = eventMonitor->trigOnEvent;
        trigger = trigEvent->countersInUse[trigEvent->numCounters - 1];
 
        regVal &= ~PERFMON_LOCAL_B0_TRIGONSEL_MASK;
        regVal |= trigger <<  
                  PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_B0_TRIGONSEL_BIT,
                                      PERFMON_LOCAL_B0_TRIGONSEL_WIDTH);

        regVal |= eventMonitor->trigOnCtrl <<  
                  PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_B0_TRIGONCNTL_BIT,
                                      PERFMON_LOCAL_B0_TRIGONCNTL_WIDTH);
        }
  
    if (eventMonitor->trigOffEvent != NULL && eventMonitor->trigOffCtrl !=
        PERFMON_TRIGCNTL_DISAB)
        {
 
        /* Get the most significant counter register */

        trigEvent = eventMonitor->trigOffEvent;
        trigger = trigEvent->countersInUse[trigEvent->numCounters - 1];
 
        regVal &= ~PERFMON_LOCAL_B0_TRIGOFFSEL_MASK;
        regVal |= trigger <<  
                  PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_B0_TRIGOFFSEL_BIT,
                                      PERFMON_LOCAL_B0_TRIGOFFSEL_WIDTH);

        regVal |= eventMonitor->trigOffCtrl <<  
                  PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_B0_TRIGOFFCNTL_BIT,
                                      PERFMON_LOCAL_B0_TRIGOFFCNTL_WIDTH);
        }
  
    CCSR_WRITE32(CCSBAR, eventCounters[counter].localCtrlRegB, regVal);
    }

/**************************************************************************    
*
* eventCounterConfig - program the counters for the given monitor
*
* This routine programs the counter configuration registers for the
* given event monitor from the event monitor's attributes using its assigned
* counters. If resume is true, initialVal is ignored.
*
* RETURNS: N/A
*/

LOCAL void eventCounterConfig
    (
    EVENT_MONITOR *eventMonitor,   /* Configure counters for this monitor */
    COUNTER_VALUE  initialVal,     /* Initial counter value */
    BOOL           resume          /* Resume from saved count */
    )
    {
    UINT32 valIndex = 0;
    UINT32 index = 0;
    UINT32 eventId = 0;
    UINT32 counter = 0;
    UINT32 regVal = 0;
 
    /* Configure the base counter */

    perfMonEventMonitorStop (eventMonitor);
    perfMonEventMonitorReset (eventMonitor, initialVal);

    perfMonEventMonitorThresholdSet (eventMonitor, eventMonitor->threshold,
                                     eventMonitor->scaleFactor); 

    perfMonEventMonitorBurstSet (eventMonitor, eventMonitor->burstSize,
                                 eventMonitor->burstGran, 
                                 eventMonitor->burstDist,  
                                 eventMonitor->scaleFactor);

    eventTriggerSet (eventMonitor);

    /* Restore saved count values if resuming count */

    if (resume)
        {
        for (index = 0; index < eventMonitor->numCounters; ++index)
            {
            counter = eventMonitor->countersInUse[index];

            CCSR_WRITE32 (CCSBAR, eventCounters[counter].counterReg, 
                          eventMonitor->savedCount[valIndex]);

            if (counter == 0)               
                {

                /* Counter 0, if used, is 64 bits, always index 0. */
           
                ++valIndex;
                CCSR_WRITE32 (CCSBAR, eventCounters[counter].cntrUpperReg, 
                              eventMonitor->savedCount[valIndex]);
                }

            ++valIndex;
            }
        }
    
    /* Program main event in base counter, if not counter 0 */

    counter = eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX];

    regVal = CCSR_READ32 (CCSBAR, eventCounters[counter].localCtrlRegA); 
    regVal &= ~PERFMON_LOCAL_A0_CE_MASK;

    if (counter != 0)
        {
        regVal &= ~PERFMON_LOCAL_A1_EVENT_MASK;

        if (PERFMON_IS_INT_EVENT (eventMonitor->theEvent))
            {
            perfMonEventIntIdGet (eventMonitor->theEvent, &eventId);
            perfMonEventIntMaskSet (eventMonitor->theEvent);
            }
        else
            { 
            perfMonEventIdGet (eventMonitor->theEvent, &eventId);
            }

        regVal |= eventId << PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_A1_EVENT_BIT,
                                                 PERFMON_LOCAL_A1_EVENT_WIDTH);
        }

    CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegA, regVal); 

    /* Set up chained counters, if any, (numCounters > 1) */

    index = 1;
    while (index < eventMonitor->numCounters)
        {
        counter = eventMonitor->countersInUse[index - 1];
        perfMonEventIdGet (PERF_MON_C0_CARRY_OUTS + counter, &eventId);

        counter = eventMonitor->countersInUse[index];
        regVal = eventId << PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_A1_EVENT_BIT,
                                                PERFMON_LOCAL_A1_EVENT_WIDTH);

        CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegA, regVal); 
        CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegB, 0x00); 

        ++index;
        }

    /* Set condition enable bit in the most significant counter */

    counter = eventMonitor->countersInUse[eventMonitor->numCounters - 1];
    regVal = CCSR_READ32 (CCSBAR, eventCounters[counter].localCtrlRegA); 
    regVal &= ~PERFMON_LOCAL_A0_CE_MASK;

    if (eventMonitor->isIntEnabled)
        {
        regVal |= PERFMON_LOCAL_A0_CE_MASK;
        }

    CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegA, regVal); 
    }

/***************************************************************************    
*
* numCountersGet - determine the number of counters from given bit width
*
* RETURNS: The number counters needed to cover the given bit width.
*/

LOCAL UINT32 numCountersGet
    (
    PERFMON_EVENT  theEvent,       /* The event */
    UINT32         countBitWidth   /* Number of bits needed for counting */
    )
    {
    UINT32 minBitWidth = 0;
    UINT32 maxBitWidth = 0;
    UINT32 numCounters = 0;

    /* Adjust bit width limits for counter 0 events */

    if (theEvent == SYSTEM_CYCLES)
        {
        minBitWidth = PERFMON_CNTR0_MIN_BIT_WIDTH;
        maxBitWidth = PERFMON_CNTR0_MAX_BIT_WIDTH;
        }
    else
        {
        minBitWidth = PERFMON_MIN_BIT_WIDTH;
        maxBitWidth = PERFMON_MAX_BIT_WIDTH;
        }
           
    /* Determine actual bit width from given bit width */

    if (countBitWidth < minBitWidth)
        {
        countBitWidth = minBitWidth;
        }
    else if (countBitWidth > maxBitWidth)
        {
        countBitWidth = maxBitWidth;
        }

    numCounters = (countBitWidth + PERFMON_MIN_BIT_WIDTH - 1) / 
                  PERFMON_MIN_BIT_WIDTH;

    if (theEvent == SYSTEM_CYCLES)
        {
        --numCounters;
        }

    return (numCounters);
    }

/***************************************************************************    
*
* sysPerfMonInit - initializes the performance monitor for use
*
* This routine initializes all counters to zero and frozen. Interrupts are
* disabled. Internal data structures are allocated and initialized. The 
* routine must be called before any other performance monitor routine.
*
* RETURNS: OK upon success.
* ERROR if internal data structures could not be initialized.
*/

STATUS sysPerfMonInit (void)
    {
    STATUS status = OK;
    UINT32 counter = 0;
    COUNTER_RESOURCE *theCounter = NULL;

    if (!pmIsInitialized)
        {

        /* Create general protection semaphore */

        semProt = semMCreate (SEM_Q_FIFO);

        if (semProt == NULL)
            {
            status = ERROR;
            }
        
        /* Freeze the performance monitor */
             
        CCSR_WRITE32 (CCSBAR, CCSR_PMGC0, PERFMON_GLOBAL_FAC_MASK); 

        /* Initialize each counter info block */

        for (counter = 1; counter < PERFMON_NUM_COUNTERS; ++counter)
            {
            theCounter = &eventCounters[counter];
            theCounter->activeMonitor = NULL;

            theCounter->localCtrlRegA = ((counter - 1) * 
                                         PERFMON_REG_INTRVL) + CCSR_PMLCA1;
            theCounter->localCtrlRegB = ((counter - 1) * 
                                         PERFMON_REG_INTRVL) + CCSR_PMLCB1;
            theCounter->counterReg = ((counter - 1) * 
                                       PERFMON_REG_INTRVL) + CCSR_PMC1;
            theCounter->cntrUpperReg = theCounter->counterReg;
        
            /* Initialize counter to zero and frozen with no features */

            CCSR_WRITE32 (CCSBAR, theCounter->localCtrlRegA, 
                          PERFMON_LOCAL_A1_FC_MASK);
            CCSR_WRITE32 (CCSBAR, theCounter->localCtrlRegB, 0x00000000UL); 
            CCSR_WRITE32 (CCSBAR, theCounter->counterReg, 0x00000000UL);
            }

        /*
         * Counter zero is unique with 64 bits dedicated to counting
         * clock cycle events only. All other counters are 32-bits.
         */

        theCounter = &eventCounters[0];
        theCounter->activeMonitor = NULL;

        theCounter->localCtrlRegA = CCSR_PMLCA0;
        theCounter->localCtrlRegB = CCSR_PMLCB0;
        theCounter->counterReg = CCSR_PMC0U;
        theCounter->cntrUpperReg = CCSR_PMC0L;
    
        /* Initialize counter to zero and frozen with no features */

        CCSR_WRITE32 (CCSBAR, theCounter->localCtrlRegA, 
                      PERFMON_LOCAL_A0_FC_MASK);
        CCSR_WRITE32 (CCSBAR, theCounter->localCtrlRegB, 0x00000000UL); 
        CCSR_WRITE32 (CCSBAR, theCounter->counterReg, 0x00000000UL);
        CCSR_WRITE32 (CCSBAR, theCounter->cntrUpperReg, 0x00000000UL);

        /* Mask all interrupt events */

        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_0_MSK_REG0, 0x00FFFFFF); 
        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_0_MSK_REG1, 0xFFFFFFFF); 
        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_1_MSK_REG0, 0x00FFFFFF); 
        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_1_MSK_REG1, 0xFFFFFFFF); 
        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_2_MSK_REG0, 0x00FFFFFF); 
        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_2_MSK_REG1, 0xFFFFFFFF); 
        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_3_MSK_REG0, 0x00FFFFFF); 
        CCSR_WRITE32 (CCSBAR, EPIC_PERFMON_3_MSK_REG1, 0xFFFFFFFF); 

        /* Unfreeze the performance monitor */
             
        CCSR_WRITE32 (CCSBAR, CCSR_PMGC0, 0x00);
        pmIsInitialized = (status == OK);
        }

    return (status);
    }

/*************************************************************************    
*
* perfMonIntConnect - connect an interrupt service routine
*
* This routine connects a user interrupt service routine to the performance
* monitor. The handler is invoked when any active event monitor 
* reaches an enabled overflow condition. All interrupting and overflowing
* event monitors are reset to zero after returning from the user routine,
* before the handler completes.
*
* The user routine must NOT create/delete or activate/suspend any event 
* monitors. If all monitors were frozen on interrupt, the user routine or 
* application must invoke perfMonUnfreezeAll to resume monitoring. 
*
* RETURNS: OK upon success.
* ERROR if the handler is null.
*/

STATUS perfMonIntConnect
    (
    VOIDFUNCPTR handler,     /* User handler routine to connect */
    int         parameter    /* Parameter for handler routine */
    )
    {
    STATUS status = OK;

    semTake (semProt, WAIT_FOREVER);

    if (handler != NULL)
        {
        userHandler = handler;
        status = intConnect (INUM_TO_IVEC (EPIC_PERFMON_INT_VEC),
                             (VOIDFUNCPTR)eventOverflowIntHandle, parameter);
        }
    else
        {

        /* Invalid handler */

        status = ERROR;
        }

    semGive (semProt);

    return (status);
    }

/*************************************************************************    
*
* perfMonIntEnable - enables performance monitor interrupts
*
* This routine enables interrupts for the entire performance monitor.
* If freezeAll is given as true, all counters will be frozen on interrupt.
*
* RETURNS: N/A.
*/

void perfMonIntEnable
    (
    BOOL freezeAll  /* All monitors are frozen on interrupt */
    )
    {
    UINT32 regVal = 0;

    semTake (semProt, WAIT_FOREVER);

    regVal = CCSR_READ32 (CCSBAR, CCSR_PMGC0);
    regVal &= ~PERFMON_GLOBAL_FCECE_MASK;

    if (freezeAll)
        {
        regVal |= PERFMON_GLOBAL_FCECE_MASK;
        }

    regVal |= PERFMON_GLOBAL_PMIE_MASK;

    CCSR_WRITE32 (CCSBAR, CCSR_PMGC0, regVal);

    intEnable (EPIC_PERFMON_INT_VEC);

    semGive (semProt);
    }

/*************************************************************************    
*
* perfMonIntDisable - disables performance monitor interrupts
*
* This routine disables interrupts for the entire performance monitor.
* Freezing of counters on interrupt will be cleared.
*
* RETURNS: N/A.
*/

void perfMonIntDisable (void)
    {
    UINT32 regVal = 0;

    semTake (semProt, WAIT_FOREVER);

    intDisable (EPIC_PERFMON_INT_VEC);

    regVal = CCSR_READ32 (CCSBAR, CCSR_PMGC0);
    regVal &= ~PERFMON_GLOBAL_FCECE_MASK;
    regVal &= ~PERFMON_GLOBAL_PMIE_MASK;

    CCSR_WRITE32 (CCSBAR, CCSR_PMGC0, regVal);

    semGive (semProt);
    }

/***************************************************************************    
*
* perfMonFreezeAll - unconditionally freeze all event counters
*
* This routine freezes all event counters regardless of task ownership
* or lock status. No other states are changed. Call perfMonUnfreezeAll
* to allow counting to continue. Frozen event monitors behave as
* if no events are occuring, so all operations are normally available.
*
* RETURNS:  N/A
*/

void perfMonFreezeAll (void)
    {
    UINT32 regVal;

    semTake (semProt, WAIT_FOREVER);

    regVal = CCSR_READ32(CCSBAR, CCSR_PMGC0);
    CCSR_WRITE32 (CCSBAR, CCSR_PMGC0, regVal | PERFMON_GLOBAL_FAC_MASK); 

    semGive (semProt);
    }

/***************************************************************************    
*
* perfMonUnfreezeAll - unconditionally allow all event counters to continue
*
* This routine allows event counters to continue counting. No other 
* states are changed. Must be called to continue event counting after a 
* perfMonFreezeAll or when an interrupt with freeze occurs. Previously 
* active event monitors will count their events as they occur.
*
* RETURNS: N/A
*/

void perfMonUnfreezeAll (void)
    {
    UINT32 regVal;

    semTake (semProt, WAIT_FOREVER);

    regVal = CCSR_READ32 (CCSBAR, CCSR_PMGC0);
    CCSR_WRITE32 (CCSBAR, CCSR_PMGC0, regVal & ~PERFMON_GLOBAL_FAC_MASK); 

    semGive (semProt);
    }

/***************************************************************************    
*
* perfMonEventMonitorCreate - create an event specific monitor object.
*
* This routine creates an event monitor object that is used for status
* and control of the specified event. It is created in suspended mode
* pending further optional configuration. The counterBitWidth parameter 
* specifies the number of bits needed to count the event. The bit count is 
* rounded up to next 32-bit boundary. Minimum bit width is 32. Maximum bit 
* width is 256. Bit widths outside the bounds are adjusted to the 
* respective limit internally. Each additional 32-bit block reduces by 
* one, the number of event monitors that can be concurrently active. 
* The event monitor is created with its interrupt disabled by default.
*
* If an event monitor exists for the given event already (it will be either
* in the active or suspend list), it will be suspended if active and its
* bit width will be adjusted to the value now given. All trigger, burst, and
* threshold properties are also cleared/reset. Only one event monitor
* can exist for a particular event.
*
* RETURNS: OK with a valid eventMontor object reference.
* ERROR when creation fails or a parameter is invalid.
*/

STATUS perfMonEventMonitorCreate
    (
    EVENT_MONITOR  **eventMonitor,   /* The new event monitor object */ 
    PERFMON_EVENT    theEvent,       /* The event to monitor */
    UINT32           countBitWidth   /* Number of bits needed for counting */
    )
    {
    EVENT_MONITOR *nextMon = NULL;
    EVENT_MONITOR *monObj = NULL;
    BOOL           found  = FALSE;
    STATUS         status = OK;
    UINT32         index = 0;

    if (eventMonitor != NULL)
        {
        semTake (semProt, WAIT_FOREVER);

        /* Check if event monitor already exits in active list */

        monObj = activeListHead;
        while (nextMon != NULL && !found)
            {
            found = (nextMon->theEvent == theEvent);
            monObj = nextMon;
            nextMon = nextMon->nextEvent;
            }
        
        if (found)
            {

            /* Found in the active list, so suspend the monitor */

            perfMonEventMonitorSuspend (monObj);
            }
        else
            { 

            /* If not found in the active list, check the suspend list */

            nextMon = suspendListHead;
            while (nextMon != NULL && !found)
                {
                found = (nextMon->theEvent == theEvent);
                monObj = nextMon;
                nextMon = nextMon->nextEvent;
                }
            } 

        if (!found)
            {

            /* Allocate a new monitor object since not found */

            monObj = (EVENT_MONITOR *)calloc (1, sizeof (EVENT_MONITOR));
            }
        
        if (monObj != NULL)
            {
            monObj->numCounters = numCountersGet (theEvent, countBitWidth);

            /* No counters are used until activation */

            for (index = 0; index < PERFMON_NUM_RESOURCES; ++index)
                {
                monObj->countersInUse[index] = PERFMON_NOT_USED;
                monObj->savedCount[index] = 0;
                }

            monObj->theEvent = theEvent;
            monObj->isActive = FALSE;
            monObj->isFrozen = TRUE;
            monObj->isIntEnabled = FALSE;
            monObj->isLocked = FALSE;
            monObj->threshold = 0;
            monObj->scaleFactor = 0;
            monObj->burstSize = 0;
            monObj->burstGran = 0;
            monObj->burstDist = 0;
            monObj->trigOnEvent = NULL;
            monObj->trigOffEvent = NULL;
            monObj->trigOnCtrl = PERFMON_TRIGCNTL_DISAB;
            monObj->trigOffCtrl = PERFMON_TRIGCNTL_DISAB;
        
            /* Was allocated new so add to suspend list */

            if (!found)
                {
                if (suspendListHead != NULL)
                    {
                    suspendListHead->prevEvent = monObj;
                    }

                monObj->nextEvent = suspendListHead;
                monObj->prevEvent = NULL;

                suspendListHead = monObj;
                }

            /* Pass back the new monitor */

            *eventMonitor = monObj;
            }
         else
            {
           
            /* Event monitor object allocation failed */
   
            status = ERROR;
            *eventMonitor = NULL;
            }

       semGive (semProt);
       }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventMonitorActivate - start an event monitor counting.
*
* This routine activates the given event monitor with its existing 
* configuration. It should be called AFTER configuring threshold, 
* burstiness, interrupts, and trigger events. If override is specified 
* as TRUE, then any other active event monitors using the same counter 
* resources that are not locked are suspended so this event can be actively 
* monitored. If resume is TRUE, then counting starts at the last count, 
* otherwise the count is reset to zero. If lock is TRUE, then the 
* activated monitor is locked and cannot be overriden by another
* activation without explicitly being suspended first. This is useful to 
* prevent tasks from overriding monitors in other tasks. 
*
* If there are not enough resources available, even with override 
* specified, ERROR is returned and the event monitor is left suspended. If 
* override was specified, previously active monitors may have been suspended 
* even if ERROR is returned.
*
* If OK is returned the monitor is ready (but stopped) with all resources 
* needed assigned and configured.  A perfMonEventMonitorGo command must be 
* issued to the event monitor to start it counting. 
*
* RETURNS: OK upon successful activation.
* ERROR if the event monitor is null, or override is FALSE and counter
* resources for this event are already in use or locked.
*/

STATUS perfMonEventMonitorActivate 
    (
    EVENT_MONITOR *eventMonitor,  /* The event monitor object to activate */ 
    COUNTER_VALUE  initialValue,  /* Starting value */
    BOOL           override,      /* Suspend unlocked event monitors */
    BOOL           resume,        /* Resume from previous count */
    BOOL           lock           /* Lock the monitor to prevent override */
    )
    {
    STATUS status = OK;
    UINT32 usedIndex = PERFMON_BASE_CNTR_INDEX;
    UINT32 counter = 0;
    UINT32 countersNeeded = 0;
    UINT32 available = 0;

    if (eventMonitor != NULL)
        {
        semTake (semProt, WAIT_FOREVER);

        if (!eventMonitor->isActive)
            {

            /* Check for enough free counters */

            for (counter = 0; counter < PERFMON_NUM_COUNTERS; ++counter)
                {
                if (eventCounters[counter].activeMonitor == NULL || 
                    (!eventCounters[counter].isLocked && override)) 
                    {
                    ++available;
                    }
                }
   
            countersNeeded = eventMonitor->numCounters;

            if (available >= countersNeeded)
                {

                /* Check that the specific counter base needed is available */

                if (PERFMON_IS_INT_EVENT (eventMonitor->theEvent))
                    {
                    perfMonEventIntCounterGet (eventMonitor->theEvent, 
                                               &counter);
                    }
                else
                    { 
                    perfMonEventCounterGet (eventMonitor->theEvent, &counter);
                    }

                if (counter != PERFMON_REF_EVENT)
                   {
                   if (eventCounters[counter].activeMonitor != NULL)
                       { 
                       if (!eventCounters[counter].isLocked)
                           {
                           if (override)
                               {
                               status = 
                                   perfMonEventMonitorSuspend (
                                       eventCounters[counter].activeMonitor);
                               }
                           else
                               {
                               
                               /* Choose not to suspend, so error */
  
                               status = ERROR;
                               }
                           }
                       else
                           {
  
                           /* Counter is locked, so cannot suspend */
  
                           status = ERROR;
                           }
                       } 
 
                       /* If okay, enough counter resources are available */

                       if (status == OK)
                          {
                           
                          /* Reserve specific base counter */ 
                         
                          eventMonitor->countersInUse[usedIndex] = counter;
                          eventCounters[counter].activeMonitor = eventMonitor;
                          eventCounters[counter].isLocked = lock;

                          ++usedIndex;
                          --countersNeeded;
                          }
                   }    
                  
                /* Reserve the remaining needed counters */

                if (status == OK)
                    {

                    /* Scan for completely free available counters */

                    counter = 1;
                    while (counter < PERFMON_NUM_COUNTERS && 
                        countersNeeded > 0)
                        {
                        if (eventCounters[counter].activeMonitor == NULL)
                            {
                            
                            /* Counter is open so reserve it */ 

                            eventMonitor->countersInUse[usedIndex] = counter;
                            eventCounters[counter].activeMonitor = eventMonitor;
                            eventCounters[counter].isLocked = lock;

                            ++usedIndex;
                            --countersNeeded;
                            }

                        ++counter;
                        }
                    
                    /* Scan for suspendable counters if specified */

                    counter = 1;
                    while (override && counter < PERFMON_NUM_COUNTERS && 
                        countersNeeded > 0)
                        {
                        if (eventCounters[counter].activeMonitor != 
                            eventMonitor && !eventCounters[counter].isLocked)
                            {
                            status = perfMonEventMonitorSuspend (
                                         eventCounters[counter].activeMonitor);
                            
                            if (status == OK)
                                {
                                eventMonitor->countersInUse[usedIndex] = 
                                    counter;
                                eventCounters[counter].activeMonitor = 
                                    eventMonitor;
                                eventCounters[counter].isLocked = lock;

                                ++usedIndex;
                                --countersNeeded;
                                }
                            }

                        ++counter;
                        } 

                    if (status == OK && countersNeeded == 0)
                        {
                        
                        /* Remove from suspend list */

                        eventSwapToOtherList (eventMonitor, FALSE);

                        eventMonitor->isFrozen = TRUE;
                        eventMonitor->isLocked = lock;

                        /* Configure all the assigned counter registers */

                        eventCounterConfig (eventMonitor, initialValue, 
                                            resume);
                        }
                    else 
                        {

                        /* 
                         * Not enough counters available to allow activation.
                         * Free up any counters that may have been assigned.
                         * Monitor remains suspended.
                         */
                       
                        for (usedIndex = 0; 
                             usedIndex < eventMonitor->numCounters; ++usedIndex)
                            {
                            counter = eventMonitor->countersInUse[usedIndex];

                            if (counter != PERFMON_NOT_USED)
                                {
                                eventCounters[counter].activeMonitor = NULL;
                                eventCounters[counter].isLocked = FALSE;
                                eventMonitor->countersInUse[usedIndex] =
                                    PERFMON_NOT_USED;
                                }
                            }    
                    
                        status = ERROR;
                        }
                    }   
                }
            else 
                {
                
                /* Not enough counters available to allow activation */

                status = ERROR;
                }
            }

        /* Already active */

        semGive (semProt);
        }
    else
        {        
         
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventMonitorTrigGo - start a triggered event monitor.
*
* This routine activates and starts all event monitors associated with
* triggering the given event monitor. The event monitor must have its
* trigger events previously setup using perfMonEventMonitorTrigSet.
* Each associated event monitor is activated according to the rules described 
* above in perfMonEventMonitorActivate. If any event monitor cannot be 
* activated and started successfully ERROR will be returned and all 
* associated monitors will remain suspended. No additional "go" commands 
* are required. An OK return means all trigger monitors are enabled for 
* counting their assigned events. Monitors resume from previous values, so
* the perfMonEventMonitorReset function should be used to set any
* starting values prior to calling this function.
*
* RETURNS: OK upon successful activation.
* ERROR if the event monitor is null, or override is FALSE and counter
* resources for this event are already in use or locked.
*/

STATUS perfMonEventMonitorTrigGo 
    (
    EVENT_MONITOR *eventMonitor,  /* The event monitor object to activate */ 
    BOOL           override,      /* Suspend unlocked event monitors */
    BOOL           lock           /* Lock the monitor to prevent override */
    )
    {
    STATUS status = OK;

    if (eventMonitor != NULL)
        {
        if (eventMonitor->trigOffEvent != NULL)
            {
            status = perfMonEventMonitorActivate (
                         eventMonitor->trigOffEvent, NULL, override, 
                         TRUE, lock);
             }

        if (status == OK)
            {
            if (eventMonitor->trigOnEvent != NULL)
                {
                status = perfMonEventMonitorActivate (
                             eventMonitor->trigOnEvent, NULL, override, 
                             TRUE, lock);
                }

            if (status == OK)
                {
                status = perfMonEventMonitorActivate (eventMonitor, NULL, 
                                                          override, TRUE, lock);
                if (status == OK)
                    {
                    perfMonEventMonitorGo (eventMonitor); 

                    if (eventMonitor->trigOffEvent != NULL)
                        {
                        perfMonEventMonitorGo (eventMonitor->trigOffEvent); 
                        }

                    if (eventMonitor->trigOnEvent != NULL)
                        {
                        perfMonEventMonitorGo (eventMonitor->trigOnEvent); 
                        }
                    }
                else
                    {

                    /* Triggered monitor could not be activated */

                    perfMonEventMonitorSuspend (eventMonitor->trigOffEvent);
                    perfMonEventMonitorSuspend (eventMonitor->trigOnEvent);
                    }
                }
            else
                {

                /* Trigger-on monitor could not be activated */
          
                perfMonEventMonitorSuspend (eventMonitor->trigOffEvent);
                }
            }
        }
    else
        {
 
        /* Invalid parameter */
        
        status = ERROR;
        }
             
    return (status);
    }

/***************************************************************************    
*
* perfMonEventMonitorSuspend - suspends an event monitor.
*
* This routine suspends the given event monitor freeing the counter
* resources for use by other event monitors. If the monitor has locked 
* resources, the resources are unlocked. The count at the time of
* suspension is preserved as well as the event monitor's configuration. The
* event monitor can be resumed using the activate function. Suspended
* event monitors can still be read, displayed, etc. Monitors using locked 
* resources must be suspended before another monitor can use its counter 
* resource regardless of override. 
*
* RETURNS: OK upon successful activation.
* ERROR if the event monitor is null.
*/

STATUS perfMonEventMonitorSuspend
    (
    EVENT_MONITOR *eventMonitor  /* The event monitor object to suspend */
    )
    {
    STATUS status = OK;
    UINT32 index = 0;
    UINT32 saveIndex = 0;
    UINT32 counter = 0;

    if (eventMonitor != NULL)
        {
        semTake (semProt, WAIT_FOREVER);

        if (eventMonitor->isActive)
            {
            if (!eventMonitor->isFrozen)
                {
                perfMonEventMonitorStop (eventMonitor);
                }
            
            /* Zero the entire saved count array space */

            for (index = 0; index < PERFMON_NUM_RESOURCES; ++index)
                {
                eventMonitor->savedCount[index] = 0x00;
                }

            /* Save current count and free all counters */

            for (index = 0; index < eventMonitor->numCounters; ++index)
                {
                counter = eventMonitor->countersInUse[index];
                    
                eventMonitor->savedCount[saveIndex] = 
                    CCSR_READ32 (CCSBAR, eventCounters[counter].counterReg);

                if (counter == 0)               
                    {

                    /* Counter 0, if used, is 64 bits, always index 0. */
                   
                    ++saveIndex;
                    eventMonitor->savedCount[saveIndex] =
                        CCSR_READ32 (CCSBAR, 
                                     eventCounters[counter].cntrUpperReg);
                    }
                    
                ++saveIndex;
           
                /* Reset counter registers to safe initial values */

                CCSR_WRITE32 (CCSBAR, eventCounters[counter].counterReg, 0x00);
                CCSR_WRITE32 (CCSBAR, eventCounters[counter].cntrUpperReg, 
                              0x00);
                CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegA, 
                              PERFMON_LOCAL_A0_FC_MASK);
                CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegB, 
                              0x00);
                eventCounters[counter].activeMonitor = NULL;
                eventCounters[counter].isLocked = FALSE;

                eventMonitor->countersInUse[index] = PERFMON_NOT_USED;
                }

            /* Remove from active list */

            eventSwapToOtherList (eventMonitor, FALSE);
 
            /* Set attributes to reflect suspension */
       
            eventMonitor->isFrozen = TRUE;
            eventMonitor->isIntEnabled = FALSE;
            eventMonitor->isLocked = FALSE;
            }

        semGive (semProt);
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventMonitorReset - resets an event monitor count to a value.
*
* This routine resets an event monitor to the given value. If the event 
* monitor is active, the count resource is immediately set. If the monitor 
* is suspended, its last count value is set. No resources are freed and
* the monitor remains active or suspended.
*
* If the event monitor is using multiple counters to extend the bit width,
* the counter is halted before reading and resumed after (if actively 
* counting when reset is called). This prevents incomplete clearing
* caused  by individual sequential writing of the assigned counter registers
* while the count is potentially incrementing.
*
* If initialValue is NULL, the monitor is reset to zero.
*
* RETURNS: OK upon successful reset.
* ERROR if the event monitor is null.
*/

STATUS perfMonEventMonitorReset
    (
    EVENT_MONITOR *eventMonitor,  /* The event monitor object to reset */
    COUNTER_VALUE  initialValue   /* Initial counter value */
    )
    {
    STATUS  status = OK;
    UINT32  index = 0;
    UINT32  valIndex = 0;
    UINT32  counter = 0;
    BOOL    needResume = FALSE;
    UINT32 *value = 0;
    COUNTER_VALUE zeroCount = {0};

    if (eventMonitor != NULL)
        {
        value = initialValue != NULL ? initialValue : zeroCount;

        /* Zero the entire saved count array space */

        for (index = 0; index < PERFMON_NUM_RESOURCES; ++index)
            {
            eventMonitor->savedCount[index] = 0x00;
            }

        /* If using multi-word counters, stop the monitor first */

        if ((eventMonitor->numCounters > 1 || 
             eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX] == 0) && 
            !eventMonitor->isFrozen)
            {
            needResume = !eventMonitor->isFrozen;
            perfMonEventMonitorStop (eventMonitor);
            }
                 
        /* Set counter registers directly if active. Updated saved count. */
       
        for (index = 0; index < eventMonitor->numCounters; ++index)
            {
            counter = eventMonitor->countersInUse[index];

            if (eventMonitor->isActive)
                {
                CCSR_WRITE32 (CCSBAR, eventCounters[counter].counterReg, 
                              value[valIndex]);
                }
            eventMonitor->savedCount[valIndex] = value[valIndex];

            if (counter == 0)               
                {

                /* Counter 0, if used, is 64 bits, always index 0. */
             
                ++valIndex; 

                if (eventMonitor->isActive)
                    {
                    CCSR_WRITE32 (CCSBAR, eventCounters[counter].cntrUpperReg, 
                                  value[valIndex]);
                    }

                eventMonitor->savedCount[valIndex] = value[valIndex];
                }

            ++valIndex;
            }

        /* Resume counting if this routine stopped the counter */

        if (needResume)
            {
            perfMonEventMonitorGo (eventMonitor);
            }
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventMonitorDelete - delete an event monitor.
*
* This routine deletes an event monitor and immdediately frees any counter
* resources that it may have been using. To monitor the event again, the
* create routine must be called. Upon return references to the given event
* monitor are no longer valid.
*
* RETURNS: OK upon successful deletion.
* ERROR if the event monitor is null.
*/

STATUS perfMonEventMonitorDelete
    (
    EVENT_MONITOR *eventMonitor  /* The event monitor object to delete */
    )
    {
    STATUS  status = OK;

    if (eventMonitor != NULL)
        {
        semTake (semProt, WAIT_FOREVER);

        if (eventMonitor->isActive)
            {
            perfMonEventMonitorSuspend (eventMonitor);
            }
        
        /* Remove from list */

        eventSwapToOtherList (eventMonitor, TRUE);
        free (eventMonitor);

        semGive (semProt);
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventMonitorRead - read an event monitor count.
*
* This routine returns the current value of the given event monitor.
* Because a single event monitor can be many 32-bit words wide a multi-word
* value is returned. The number of significant words depends on the 
* bit width specified at creation. The least significant word is at index
* zero. 
*
* If the event monitor is using multiple counters to extend the bit width,
* the counter is halted before reading and resumed after (if actively 
* counting when Read is called), to prevent an erroneous value return caused
* by individual sequential reading of the assigned counter registers while 
* the count is potentially incrementing.
*
* RETURNS: OK upon successful read.
* ERROR if the event monitor is null.
*/

STATUS perfMonEventMonitorRead
    (
    EVENT_MONITOR *eventMonitor, /* The event monitor to read */
    COUNTER_VALUE  value         /* The event count */
    )
    {
    STATUS status = OK;
    UINT32 index = 0;
    UINT32 valIndex = 0;
    UINT32 counter = PERFMON_NOT_USED;
    BOOL   needResume = FALSE;

    if (eventMonitor != NULL)
        {
        
        /* Initialize entire return value to zero */

        for (index = 0; index < PERFMON_NUM_RESOURCES; ++index)
            {
            value[index] = 0x00;
            }

        if (eventMonitor->isActive)
            {
            if ((eventMonitor->numCounters > 1 || 
                 eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX] == 0) && 
                !eventMonitor->isFrozen)
                {
                needResume = !eventMonitor->isFrozen;
                perfMonEventMonitorStop (eventMonitor);
                }

            /* Read counter values directly from registers. */
           
            for (index = 0; index < eventMonitor->numCounters; ++index)
                {
                counter = eventMonitor->countersInUse[index];
                    
                value[valIndex] = 
                    CCSR_READ32 (CCSBAR, eventCounters[counter].counterReg);

                if (counter == 0)               
                    {

                    /* Counter 0, if used, is 64 bits, always index 0. */
                   
                    ++valIndex;
                    value[valIndex] =
                        CCSR_READ32 (CCSBAR, 
                                     eventCounters[counter].cntrUpperReg);
                    }
                    
                ++valIndex;
                }

            if (needResume)
                {
                perfMonEventMonitorGo (eventMonitor);
                }
            }
        else
            {

            /* Suspended, so return counter value at time of suspension */

            for (index = 0; index < PERFMON_NUM_RESOURCES; ++index)
                {
                value[index] = eventMonitor->savedCount[index];
                }
            }
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventMonitorIsActive - get the active status of an event monitor.
*
* This routine returns the active status of the event monitor. An active
* monitor is still considered active even when all counters have been
* frozen. An active monitor is always in the active list herein.
*
* RETURNS: TRUE if the event monitor is active and counting, FALSE if
* suspended or eventMonitor is null.
*/

BOOL perfMonEventMonitorIsActive
    (
    EVENT_MONITOR *eventMonitor  /* The event monitor object to check */
    )
    {
    return (eventMonitor != NULL ? eventMonitor->isActive : FALSE); 
    }

/***************************************************************************    
*
* perfMonEventMonitorActiveGet - get the next active event monitor.
*
* This routine returns the next active event monitor. If the given
* currentEventMonitor is null, then the first active event monitor is 
* returned in nextEventMonitor. If the currentEvent is not active, then
* nextEventMonitor will be null. If nextEventMonitor is null, then all 
* active event monitors have been retrieved, or there are no active
* event monitors. To retrieve all active event monitors,
* the nextEventMonitor returned should be used in the subsequent call to 
* this routine as currentEventMonitor iterating until nextEventMonitor 
* returns null. 
* 
* RETURNS: N/A
*/

void perfMonEventMonitorActiveGet
    (
    EVENT_MONITOR  *currentEventMonitor, /* An active event monitor or null */
    EVENT_MONITOR **nextEventMonitor     /* An active event monitor */
    )
    {
    if (currentEventMonitor == NULL)
        {
        *nextEventMonitor = activeListHead;
        }
    else
        {
        *nextEventMonitor = currentEventMonitor->isActive ?  
                            currentEventMonitor->nextEvent : NULL;
        }
    }

/***************************************************************************    
*
* perfMonEventMonitorSuspendedGet - get the next suspended event monitor.
*
* This routine returns the next suspended event monitor. If the given
* currentEventMonitor is null, then the first suspended event monitor is 
* returned in nextEventMonitor. If the currentEvent is not suspended then,
* nextEventMonitor will be null. If nextEventMonitor is null, then all 
* suspended event monitors have been retrieved, or there are no suspended
* event monitors. To retrieve all suspended event monitors,
* the nextEventMonitor returned should be used in the subsequent call to 
* this routine as currentEventMonitor iterating until nextEventMonitor 
* returns null. 
* 
* RETURNS: N/A
*/

void perfMonEventMonitorSuspendedGet
    (
    EVENT_MONITOR  *currentEventMonitor, /* A suspended monitor or null */
    EVENT_MONITOR **nextEventMonitor     /* A suspended event monitor */
    )
    {
    if (currentEventMonitor == NULL)
        {
        *nextEventMonitor = suspendListHead;
        }
    else
        {
        *nextEventMonitor = currentEventMonitor->isActive ?  
                            NULL : currentEventMonitor->nextEvent;
        }
    }

/***************************************************************************    
*
* perfMonEventMonitorGetEvent - get the event type for this monitor
*
* This routine returns the event type that is assigned to this monitor.
* 
* RETURNS: N/A
*/

PERFMON_EVENT perfMonEventMonitorGetEvent
    (
    EVENT_MONITOR  *eventMonitor /* Get event type from this monitor */
    )
    {
    return (eventMonitor->theEvent);
    }
    
/**************************************************************************    
*
* perfMonEventMonitorIsAvailable - check if event resource is free.
*
* This routine returns TRUE if the resources for a given type are free
* to be used for monitoring. This can be called before creating or 
* activating a particular event monitor to help manage previously created 
* monitors that may need suspension. If FALSE is returned, the counter 
* resource is being used by another active event monitor. If the bit width 
* specified is greater than 32, then multiple event monitors may be using 
* the needed resources. 
* 
* If the resource is locked (rsrcIsLocked returns TRUE), the function 
* returns FALSE and otherUser is NULL since the resource maybe in use by 
* another task.  If the function returns FALSE but the resource is not 
* locked, it can be overridden forcing the event monitor using the resource 
* to be suspended so the resource can be re-used. Override is specified when 
* calling the event monitor activate function.
*
* RETURNS: OK upon success.
* ERROR theEvent is out of range.
*/

BOOL perfMonEventMonitorIsAvailable
    (
    PERFMON_EVENT   theEvent,      /* Resource check this event */
    UINT32          countBitWidth, /* bit width needed */
    BOOL           *rsrcIsLocked,  /* TRUE when resources are locked */
    EVENT_MONITOR **otherUser      /* Event monitor using the resources */
    )
    {
    UINT32 counter = 0;
    UINT32 availCounters = 0;
    BOOL   isAvailable = FALSE;

    /* Check for enough free counters */

    *rsrcIsLocked = FALSE;
    *otherUser = NULL;

    for (counter = 0; counter < PERFMON_NUM_COUNTERS; ++counter)
        {
        if (eventCounters[counter].activeMonitor == NULL) 
            {
            ++availCounters;
            }
        }

    if (availCounters >= numCountersGet (theEvent, countBitWidth)); 
        {

        /* Check that the specific counter needed is available */

        if (PERFMON_IS_INT_EVENT (theEvent))
            {
            perfMonEventIntCounterGet (theEvent, &counter);
            }
        else
            { 
            perfMonEventCounterGet (theEvent, &counter);
            }


        if (counter != PERFMON_REF_EVENT)
            {
            *otherUser = eventCounters[counter].activeMonitor;
            *rsrcIsLocked = eventCounters[counter].isLocked;
            }

        isAvailable = (*otherUser == NULL);
        }

    if (*rsrcIsLocked)
        {
        *otherUser = NULL;
        }

    return (isAvailable);
    }

/**************************************************************************    
*
* perfMonEventMonitorIntEnable - enable an event monitors interrupt
*
* This routine enables at event monitor's interrupt such that an
* overflow interrupt occurs when the event monitor's increment sets the
* MSB of the counter resource. An event monitor is created with its
* interrupt disabled. If freezeAll is true, all counters will be
* unconditionally frozen when the interrupt occurs. 
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null.
*/

STATUS perfMonEventMonitorIntEnable
    (
    EVENT_MONITOR *eventMonitor /* Enable interrupts for this monitor */
    )
    {
    STATUS status = OK;
    UINT32 regVal = 0;
    UINT32 counter;

    if (eventMonitor != NULL)
        {
        eventMonitor->isIntEnabled = TRUE;

        /* If active, change the register bit immediately */

        if (eventMonitor->isActive)
            {

            /* Get the most significant counter register */

            counter = 
                eventMonitor->countersInUse[eventMonitor->numCounters - 1];

            /* Set the condition enable to allow the counter to interrupt */

            regVal = CCSR_READ32 (CCSBAR, eventCounters[counter].localCtrlRegA);
            regVal |= PERFMON_LOCAL_A0_CE_MASK;

            CCSR_WRITE32 (CCSBAR, 
                          eventCounters[counter].localCtrlRegA, regVal);
            }
        }
    else
        {

        /* Invalid parameter */

        status = ERROR;
        }
 
    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorIntDisable - disable an event monitors interrupt
*
* This routine disables an event monitor's interrupt such that an overflow
* will not cause an interrupt.
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null.
*/

STATUS perfMonEventMonitorIntDisable
    (
    EVENT_MONITOR *eventMonitor  /* Disable interrupts for this monitor */
    )
    {
    STATUS status = OK;
    UINT32 regVal = 0;
    UINT32 counter;

    if (eventMonitor != NULL)
        {
        eventMonitor->isIntEnabled = FALSE;

        if (eventMonitor->isActive)
            {

            /* Get the most significant counter register */

            counter = 
                eventMonitor->countersInUse[eventMonitor->numCounters - 1];

            /* Set the condition enable to allow the counter to interrupt */

            regVal = CCSR_READ32 (CCSBAR, eventCounters[counter].localCtrlRegA);
            regVal &= ~PERFMON_LOCAL_A0_CE_MASK;

            CCSR_WRITE32 (CCSBAR, 
                          eventCounters[counter].localCtrlRegA, regVal);
            }
        }
    else
        {

        /* Invalid parameter */

        status = ERROR;
        }
 
    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorThresholdSet - set a threshold to start event couting
*
* The routine sets a threshold which must be exceeded by event occurrences
* before actual event counting begins. The scale factor multiplies 
* the threshold and can be varied using the scale change routine.
*
* Scale cannot be set for event SYSTEM_CYCLES (counter 0).
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null, or event is SYSTEM_CYCLES.
*/

STATUS perfMonEventMonitorThresholdSet
    (
    EVENT_MONITOR *eventMonitor, /* Set threshold for this event monitor */
    UINT32         threshold,    /* Threshold value */
    UINT32         scaleFactor   /* Scale multiplier for threshold */
    )
    {
    STATUS status = OK;
    UINT32 regVal;
    UINT32 counter;

    if (eventMonitor != NULL && threshold <= PERFMON_MAX_THRESHOLD &&
        scaleFactor <= PERFMON_TBMULT_X128)
        {
        counter = eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX];

        if (counter != 0)
            {
            eventMonitor->threshold = threshold;

            if (eventMonitor->isActive)
                {
                regVal = CCSR_READ32 (CCSBAR, 
                                      eventCounters[counter].localCtrlRegB);

                regVal &= ~PERFMON_LOCAL_B1_THRESHOLD_MASK;
                regVal |= (threshold << 
                           PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_B1_THRESHOLD_BIT,
                               PERFMON_LOCAL_B1_THRESHOLD_WIDTH));

                CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegB, 
                              regVal);
                }

            perfMonEventMonitorScaleChange (eventMonitor, scaleFactor);
            }
        else
            {
          
            /* Not available for counter 0 */

            status = ERROR;
            }
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorScaleChange - modify threshold/burst distance scaling
*
* This routine changes the scale factor that is multiplied with the 
* threshold value or burst distance value to modifiy the effective 
* threshold or burst distance values. If the monitor is active, the change
* is effective immdediately.
*
* Scale cannot be set for event SYSTEM_CYCLES (counter 0).
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null, or event is SYSTEM_CYCLES.
*/

STATUS perfMonEventMonitorScaleChange
    (
    EVENT_MONITOR *eventMonitor, /* Set the scaling for this monitor */
    UINT32         scaleFactor   /* Scale multiplier for threshold */
    )
    {
    STATUS status = OK;
    UINT32 regVal;
    UINT32 counter;

    if (eventMonitor != NULL && scaleFactor <= PERFMON_TBMULT_X128)
        {
        counter = eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX];

        if (counter != 0)
            {
            eventMonitor->scaleFactor = scaleFactor;

            if (eventMonitor->isActive)
                {
                regVal = CCSR_READ32 (CCSBAR, 
                                      eventCounters[counter].localCtrlRegB);

                regVal &= ~PERFMON_LOCAL_B1_TBMULT_MASK;
                regVal |= (scaleFactor << 
                           PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_B1_TBMULT_BIT,
                                               PERFMON_LOCAL_B1_TBMULT_WIDTH));

                CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegB, 
                              regVal);
                }
            }
        else
            {
          
            /* Not available for counter 0 */

            status = ERROR;
            }
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorBurstSet - set burst characteristics
*
* This routine sets the burst characteristics for the event monitor. The
* burst size specifies the number of events per burst that must occur 
* before the burst is counted. The burst granularity specifies the 
* maximum clocks to occur between events in a particular burst. The
* burst distance is the number of clocks that must occur between bursts. 
* The burst distance be must a value larger than the burst size. The scale 
* factor is multiplied by the burst distance to obtain the effective
* burst distance. The scale factor can be modified independently via the
* scale change routine. 
*
* Burst properties cannot be set for event SYSTEM_CYCLES (counter 0).
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null, or if event monitor is SYSTEM_CYCLES.
*/

STATUS perfMonEventMonitorBurstSet
    (
    EVENT_MONITOR *eventMonitor, /* Set the scaling for this event monitor */
    UINT32         burstSize,    /* Number of events per burst */
    UINT32         burstGran,    /* Max clocks between events in burst */
    UINT32         burstDist,    /* Num of clocks between bursts */
    UINT32         scaleFactor   /* Scale multiplier for burst distance */
    )
    {
    STATUS status = OK;
    UINT32 regVal;
    UINT32 counter;

    if (eventMonitor != NULL && burstSize <= PERFMON_MAX_BURST_SIZE &&
        burstGran <= PERFMON_MAX_BURST_GRAN && 
        burstDist <= PERFMON_MAX_BURST_DIST && 
        scaleFactor <= PERFMON_TBMULT_X128)
        {
        counter = eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX];

        if (counter != 0)
            {
            eventMonitor->burstSize = burstSize;
            eventMonitor->burstGran = burstGran;
            eventMonitor->burstDist = burstDist;

            if (eventMonitor->isActive)
                {
                regVal = CCSR_READ32 (CCSBAR, 
                                      eventCounters[counter].localCtrlRegA);

                regVal &= ~(PERFMON_LOCAL_A1_BSIZE_MASK | 
                            PERFMON_LOCAL_A1_BGRAN_MASK |
                            PERFMON_LOCAL_A1_BDIST_MASK);

                regVal |= (burstSize << 
                           PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_A1_BSIZE_BIT,
                                               PERFMON_LOCAL_A1_BSIZE_WIDTH)) |
                          (burstGran <<
                           PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_A1_BGRAN_BIT,
                                               PERFMON_LOCAL_A1_BGRAN_WIDTH)) |
                          (burstDist <<
                           PPC_BIT_INDEX_SWAP (PERFMON_LOCAL_A1_BDIST_BIT,
                                               PERFMON_LOCAL_A1_BDIST_WIDTH));

                CCSR_WRITE32 (CCSBAR, eventCounters[counter].localCtrlRegA, 
                              regVal);
                }

            perfMonEventMonitorScaleChange (eventMonitor, scaleFactor); 
            }
        else
            {
          
            /* Not available for counter 0 */

            status = ERROR;
            }
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorTrigSet - set start/stop trigger events
*
* This routine sets the start and stop trigger event for the given event 
* monitor. The start and/or stop trigger is another event monitor that 
* will, upon overflow or count change, trigger the given event monitor to 
* start counting. The trigger type specifies no trigger, count change, or 
* overflow. The triggering event monitors must not have their interrupt 
* enabled for proper triggering to occur. This routine should be called 
* before activating any of event monitors involved. Activating an event 
* monitor with triggering enabled will not automatically activate 
* the start and/or stop trigger event monitors as well. Each trigger
* monitor should be activated individually after activating the monitor
* to be triggered. Monitors should then be issued Go commands first to
* the triggered monitor, then the stop trigger, then the start trigger.
*
* Care should be taken that there are adequate counter resources available
* for the event monitor and all triggering event monitors. Triggering
* events will have interrupts disabled necessarily by this routine.
*
* The event monitor being assigned triggers should be suspended prior to 
* calling this function, otherwises changes made will take effect only 
* following a suspension and re-activation.
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null.
*/

STATUS perfMonEventMonitorTrigSet
    (
    EVENT_MONITOR *eventMonitor, /* Set start trigger for this monitor */
    EVENT_MONITOR *startTrigger, /* Event monitor that triggers start */
    EVENT_MONITOR *stopTrigger,  /* Event monitor that triggers start */
    UINT32         trigOnType,   /* Off, on change, on overflow */
    UINT32         trigOffType   /* Off, on change, on overflow */
    )
    {
    STATUS status = OK;

    if (eventMonitor != NULL && trigOnType <= PERFMON_TRIGCNTL_OVERFLOW &&
        trigOffType <= PERFMON_TRIGCNTL_OVERFLOW) 
        {
        if (startTrigger == NULL || trigOnType == PERFMON_TRIGCNTL_DISAB)
            {
            eventMonitor->trigOnEvent = NULL;
            eventMonitor->trigOnCtrl = PERFMON_TRIGCNTL_DISAB;
            }
        else
            {
            eventMonitor->trigOnEvent = startTrigger;
            eventMonitor->trigOnCtrl = trigOnType;

            perfMonEventMonitorIntDisable (startTrigger);
            }
 
        if (stopTrigger == NULL || trigOffType == PERFMON_TRIGCNTL_DISAB)
            {
            eventMonitor->trigOffEvent = NULL;
            eventMonitor->trigOffCtrl = PERFMON_TRIGCNTL_DISAB;
            }
        else
            {
            eventMonitor->trigOffEvent = stopTrigger;
            eventMonitor->trigOffCtrl = trigOffType;

            perfMonEventMonitorIntDisable (stopTrigger);
            }
        }
    else
        {
 
        /* Invalid parameter */

        status = ERROR;
        }
  
    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorHasOverflowed - check for overflow
*
* This routine returns true if the event monitor has overflowed. The 
* event monitor must be active for the return result to be meaningful.
*
* RETURNS: TRUE if count has overflowed, FALSE otherwise.
*/

BOOL perfMonEventMonitorHasOverflowed
    (
    EVENT_MONITOR *eventMonitor /* Check this event monitor */
    )
    {
    UINT32 regVal = 0;
    UINT32 counter = 0;
    BOOL   overflow = FALSE;

    if (eventMonitor != NULL)
        {
        if (eventMonitor->isActive)
            {

            /* Get the most significant counter register */

            counter = 
                eventMonitor->countersInUse[eventMonitor->numCounters - 1];

            /* Check the counter register's msb. If set, overflow occurred. */

            regVal = CCSR_READ32(CCSBAR, eventCounters[counter].counterReg);
        
            overflow = ((regVal & PERFMON_COUNTER_OVERFLOW_MASK) == 
                        PERFMON_COUNTER_OVERFLOW_MASK);
            }
        }

    return (overflow);
    }

/**************************************************************************    
*
* perfMonEventMonitorGo - unfreeze an active but stopped event monitor
*
* This routine starts an active event monitor that has been stopped. No
* resources are changed or taken. The specified event monitor must already 
* be active and configured.
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null or the event monitor is not active.
*/

STATUS perfMonEventMonitorGo
    (
    EVENT_MONITOR *eventMonitor /* Unfreeze this event monitor */
    )
    {
    STATUS status = OK;
    UINT32 regVal;
    UINT32 counter;

    if (eventMonitor != NULL)
        {
        if (eventMonitor->isActive && eventMonitor->isFrozen)
            {
            counter = eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX];
            regVal = CCSR_READ32(CCSBAR, eventCounters[counter].localCtrlRegA);

            regVal &= ~PERFMON_LOCAL_A0_FC_MASK;
            CCSR_WRITE32(CCSBAR, eventCounters[counter].localCtrlRegA, regVal);

            eventMonitor->isFrozen = FALSE;
            }
        else
            {
  
            /* Monitor is not in active list */

            status = ERROR;
            }
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorStop - freeze an active event monitor
*
* This routine stops an active event monitor without freeing any resources. 
* No resources are changed or freed. The specified event monitor must 
* already be active and configured. It remains active (in the active list), 
* and can be started using perfMonEventMonitorGo.
*
* RETURNS: OK upon success.
* ERROR if the eventMonitor is null or the event monitor is not active.
*/

STATUS perfMonEventMonitorStop
    (
    EVENT_MONITOR *eventMonitor /* Stop this event monitor */
    )
    {
    STATUS status = OK;
    UINT32 regVal;
    UINT32 counter;

    if (eventMonitor != NULL)
        {
        if (eventMonitor->isActive && !eventMonitor->isFrozen)
            {
            counter = eventMonitor->countersInUse[PERFMON_BASE_CNTR_INDEX];
            regVal = CCSR_READ32(CCSBAR, eventCounters[counter].localCtrlRegA);

            regVal |= PERFMON_LOCAL_A0_FC_MASK;
            CCSR_WRITE32(CCSBAR, eventCounters[counter].localCtrlRegA, regVal);

            eventMonitor->isFrozen = TRUE;
            }
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

#ifdef INCLUDE_SHOW_ROUTINES

/**************************************************************************    
*
* perfMonEventMonitorShow - display event monitor details.
*
* This routine prints detailed event monitor information including 
* name, current count, activation status, locked status, counter resources
* used (bitwidth), interrupt enable status, etc, in a readable format for
* the given event monitor.
* 
* RETURNS: OK upon success.
* ERROR if theEvent is null.
*/

STATUS perfMonEventMonitorShow
    (
    EVENT_MONITOR *eventMonitor /* Set the scaling for this event monitor */
    )
    {
    STATUS         status = OK;
    COUNTER_VALUE  value = {0};
    PERFMON_EVENT  theEvent;
    const char    *str1;
    const char    *str2;
    UINT32         id;
    UINT32         counter;
    UINT32         index;
    UINT32         bits;

    if (eventMonitor != NULL)
        {
        theEvent = eventMonitor->theEvent;
  
        perfMonEventClassGet (eventMonitor->theEvent, &str1);
        perfMonEventNameGet (eventMonitor->theEvent, &str2);
        perfMonEventIdGet (eventMonitor->theEvent, &id);
        perfMonEventCounterGet (eventMonitor->theEvent, &counter);

        if (PERFMON_IS_INT_EVENT(eventMonitor->theEvent) ||
            (counter != PERFMON_REF_EVENT))
            {
            id -=PERFMON_COUNTER_SPECIFIC_EVENT_OFFSET;
            } 

        printf ("%s: \r\n       %s (%d) \r\n", str1, str2, id);
   
        perfMonEventMonitorRead (eventMonitor, value);
 
        perfMonEventDescriptionGet (eventMonitor->theEvent, &str1, &str2);
        printf ("       Counts %s\r\n", str1);
      
        if (strlen(str2) > 0)
            {
            printf ("              %s\r\n", str2);
            }
        else
            {
            printf ("\r\n");
            }
   
        printf ("       Current Count: 0x");
    
        index = eventMonitor->numCounters;

        /* Add one more for 64 bit counter zero */

        if (eventMonitor->theEvent == SYSTEM_CYCLES)
            {
            ++index;
            }

        bits = index * PERFMON_MIN_BIT_WIDTH;

        /* Print the value in words from left to right */

        while (index > 0)
            {
            printf("%08x", value[index - 1]);
            --index;
            }

        printf ("\r\n");
        printf ("\r\n");
    
        printf ("       Properties: ");

        if (eventMonitor->isFrozen)
            {
            printf ("*Stopped* ");
            }
        else
            {
            printf ("*Monitoring* ");
            }
       
        if (eventMonitor->isIntEnabled)
            {
            printf ("*Interrupt* ");
            }
       
        if (eventMonitor->isLocked)
            {
            printf ("*Locked* ");
            }
       
        printf ("\r\n");
        printf ("           Number Of Counters: %d, (%d bits)\r\n", 
                eventMonitor->numCounters, bits);

        if (eventMonitor->isActive)
            {
            printf ("          Which Counters Used: "); 

            for (index = 0; index < eventMonitor->numCounters; ++index)
                {
                printf ("%d  ", eventMonitor->countersInUse[index]);
                }
                printf ("\r\n");
            }

        /* Burst, Threshold, and Scaling do not apply to counter 0 */

        if (eventMonitor->theEvent != SYSTEM_CYCLES)
            {
            printf ("              Count Threshold: %d\r\n", 
                    eventMonitor->threshold);
            printf ("                 Scale Facter: %d\r\n", 
                    eventMonitor->scaleFactor);
            printf ("                   Burst Size: %d\r\n", 
                    eventMonitor->burstSize);
            printf ("            Burst Granularity: %d\r\n", 
                    eventMonitor->burstGran);
            printf ("               Burst Distance: %d\r\n", 
                    eventMonitor->burstDist);
            }

        if (eventMonitor->trigOnEvent != NULL)
            {
            str2 = "Disabled";

            if (eventMonitor->trigOnCtrl == PERFMON_TRIGCNTL_CHANGE)
                {
                str2 = "on Count Change";
                }
            else if (eventMonitor->trigOnCtrl == PERFMON_TRIGCNTL_OVERFLOW)
                {
                str2 = "on Count Overflow";
                }
        
           perfMonEventNameGet (eventMonitor->trigOnEvent->theEvent, &str1);
           printf ("             Trigger On Event: %s \r\n", str1);
           printf ("                               %s \r\n", str2);
           }

        if (eventMonitor->trigOffEvent != NULL)
            {
            str2 = "Disabled";

            if (eventMonitor->trigOffCtrl == PERFMON_TRIGCNTL_CHANGE)
                {
                str2 = "on Count Change";
                }
            else if (eventMonitor->trigOffCtrl == PERFMON_TRIGCNTL_OVERFLOW)
                {
                str2 = "on Count Overflow";
                }
        
            perfMonEventNameGet (eventMonitor->trigOffEvent->theEvent, &str1);
            printf ("            Trigger Off Event: %s \r\n", str1);
            printf ("                               %s \r\n", str2);
            }

        printf ("    -------------------------------------------------\r\n");
        printf ("\r\n");
        }
    else
        {
  
        /* Invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/**************************************************************************    
*
* perfMonEventMonitorShowAll - display event monitor details.
*
* This routine prints detailed event monitor information including 
* name, current count, activation status, locked status, counter resources
* used (bitwidth), interrupt enable status, etc, in a readable format for
* all of the existing event monitors. If activeOnly is TRUE, only the 
* currently active monitors are displayed. 
* 
* RETURNS: N/A
*/

void perfMonEventMonitorShowAll
    (
    BOOL activeOnly      /* Show only the active event monitors */
    )
    {
    UINT32 index = 1;
    EVENT_MONITOR *eventMonitor;

    printf ("\r\n");
    printf ("Actively Monitored Events\r\n");
    printf ("=========================\r\n");

    eventMonitor = activeListHead;

    if (eventMonitor != NULL)
        {
        while (eventMonitor != NULL)
            { 
            printf ("%4d.  ", index);
            perfMonEventMonitorShow (eventMonitor);
            eventMonitor = eventMonitor->nextEvent;
            ++index;
            }
        } 
    else
        {
        printf ("...None\r\n\r\n");
        }
 
    if (!activeOnly)
        { 
        printf ("Suspended Monitored Events\r\n");
        printf ("==========================\r\n");

        index = 1;
        eventMonitor = suspendListHead;

        if (eventMonitor != NULL)
            {
            while (eventMonitor != NULL)
                {
                printf ("%4d)  ", index);
                perfMonEventMonitorShow (eventMonitor);
                eventMonitor = eventMonitor->nextEvent;
                ++index;
                }
            }
        else
            {
            printf ("...None\r\n\r\n");
            }
        }
 
    }

#endif /* INCLUDE_SHOW_ROUTINES */
