/* sysMpc8540PerfMonEvent.c - Performance Monitor Events */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01b,22sep05,pjh  Fix compiler warnings.
01a,31may05,efb  NewFile.
*/


/*

DESCRIPTION

This file contains the relationship of the available monitor
events with their specific counters, event ID codes, and textual
descriptions, including full event names and class associations.
Routines are provided to retrieve this information for a 
specified event. All event information is fixed and static.

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "mpc8540.h"
#include "sysEpic.h"
#include "sysMpc8540PerfMonEvent.h"

/* defines */

#define PERFMON_INT_REG_INTERVAL 0x20

/* typedefs */

/*
 * This structure contains descriptive information about an event.
 * The event is cross referenced to its assigned counter index and its
 * event code. For interrupts the event code is the PPC bit index for
 * interrupt mask in the performance monitor mask registers.
 */

typedef struct eventInfo
    {
    UINT32  class;          /* Event classification string */
    char   *description1;   /* Event description string */
    char   *description2;   /* Event description string */
    UINT32  counterNum;     /* Assigned counter or reference event */
    UINT32  eventCode;      /* Event code */
    } EVENT_INFO;

/* forward declarations */

/*
 * This array contains descriptive full name strings corresponding to
 * each event. The array order maps one-to-one with the PERFMON_EVENT
 * enumeration allowing the enumeration defines to be used as indexes 
 * the textual event names.
 */

LOCAL const char *eventName[PERFMON_NUM_EVENTS] =
    {
    "CCB: System Cycles ",               
    "DDR: Read Cycles ",            
    "DDR: Read/Write Cycles ",     
    "DDR: Pipelined Read Misses ",
    "DDR: Pipelined Read Write Misses ",  
    "DDR: Non-pipelined Read Misses ",   
    "DDR: Non-pipelined Read/Write Misses ",
    "DDR: Pipelined Read Hits ",            
    "DDR: Pipelined Read Write Hits ",     
    "DDR: Non-pipelined Read Hits ",      
    "DDR: Non-pipelined Read/Write Hits ",  
    "DDR: Forced Page Closings Not Caused By Refresh ",
    "DDR: Row Open Table Misses ",   
    "DDR: Row Open Table Hits ",  
    "DDR: Forced_Page Closings ",    
    "DDR: RMW Transactions due to ECC ",
    "DDR: Forced Page Closings Due To Bank/Sub-bank Collisions ",
    "DDR: Core Reads/Writes ",       
    "DDR: TSEC1 Reads/Writes ",     
    "DDR: TSEC2 Reads/Writes ",    
    "DDR: RapidIO Reads/Writes ",   
    "DDR: PCI Reads/Writes ",    
    "DDR: DMA Reads/Writes ",   
    "DDR: Core Read/Write Row Open Table Hits ", 
    "DDR: TSEC1 Read/Write Row Open Table Hits ", 
    "DDR: TSEC2 Read/Write Row Open Table Hits ", 
    "DDR: RapidIO Read/Write Row Open Table Hits ", 
    "DDR: PCI Read/Write Row Open Table Hits ",  
    "DDR: DMA Read/Write Row Open Table Hits ", 
    "Memory Target Queue: Read/Write Address Collisions ",
    "RapidIO: Misaligned Transactions ",
    "RapidIO: Packet Transmission Retries ",
    "RapidIO: Packet Flushes Due To Retries/Error Recovery ",
    "RapidIO: Packets Retried/Not Accepted on Receive ",
    "RapidIO: Misaligned Engine Priority 2 Occupied ",
    "RapidIO: Misaligned Engine Priority 1 Occupied ",
    "RapidIO: Misaligned Engine Priority 0 Occupied ",
    "RapidIO: Acknowledged Packet History Queue Full ",
    "RapidIO: Outbound I/O Stopped For Training Event ",
    "RapidIO: Outbound Port Stopped On Retry ",
    "RapidIO: Outbound Port Stopped On Error ",
    "RapidIO: Inbound Message Packet Protocol Level Retry ",
    "RapidIO: Inbound Doorbell Packet Protocol Level Retry ",
    "RapidIO: Outbound Acknowledge Timeout/Out-of-order Event ",
    "DMA: Channel 0 Read Request ",
    "DMA: Channel 1 Read Request ",
    "DMA: Channel 2 Read Request ",
    "DMA: Channel 3 Read Request ",
    "DMA: Channel 0 Write Request ",
    "DMA: Channel 1 Write Request ",
    "DMA: Channel 2 Write Request ",
    "DMA: Channel 3 Write Request ",
    "DMA: Channel 0 Descriptor Request ",
    "DMA: Channel 1 Descriptor Request ",
    "DMA: Channel 2 Descriptor Request ",
    "DMA: Channel 3 Descriptor Request ",
    "DMA: Channel 0 Read Double Word Or Less ",
    "DMA: Channel 0 Read Double Word Or Less Alternate",
    "DMA: Channel 1 Read Double Word Or Less ",
    "DMA: Channel 1 Read Double Word Or Less Alternate",
    "DMA: Channel 2 Read Double Word Or Less ",
    "DMA: Channel 2 Read Double Word Or Less Alternate",
    "DMA: Channel 3 Read Double Word Or Less ",
    "DMA: Channel 3 Read Double Word Or Less Alternate",
    "DMA: Channel 0 Write Double Word Or Less ",
    "DMA: Channel 1 Write Double Word Or Less ",
    "DMA: Channel 2 Write Double Word Or Less ",
    "DMA: Channel 3 Write Double Word Or Less ",
    "ECM: Request Wait Core ",
    "ECM: Request Wait TSEC1 ",
    "ECM: Request Wait TSEC2 ",
    "ECM: Request Wait PCI/RapidIO/DMA ",
    "ECM: Dispatch ",
    "ECM: Dispatch From Core ",
    "ECM: Dispatch From TSEC1 ",
    "ECM: Dispatch From TSEC2 ",
    "ECM: Dispatch From RapidIO ",
    "ECM: Dispatch From PCI ",
    "ECM: Dispatch From DMA ",
    "ECM: Dispatch From Other ",
    "ECM: Dispatch To DDR ",
    "ECM: Dispatch To L2/SRAM ",
    "ECM: Dispatch To LBC ",
    "ECM: Dispatch To RapidIO ",
    "ECM: Dispatch To PCI ",
    "ECM: Dispatch Snoopable ",
    "ECM: Dispatch Write ",
    "ECM: Dispatch Write Allocate ",
    "ECM: Dispatch Write Allocate Lock ",
    "ECM: Dispatch Read ",
    "ECM: Dispatch Read Unlock ",
    "ECM: Dispatch Read Clear Atomic ",
    "ECM: Dispatch Read Set Atomic ",
    "ECM: Dispatch Read Decrement Atomic ",
    "ECM: Dispatch Read Increment Atomic ",
    "ECM: Data Bus Grant DDR ",
    "ECM: Data Bus Grant LBC ",
    "ECM: Data Bus Grant PIC ",
    "ECM: Data Bus Grant TSEC1 ",
    "ECM: Data Bus Grant TSEC2 ",
    "ECM: Data Bus Wait DDR ",
    "ECM: Data Bus Wait LBC ",
    "ECM: Data Bus Wait PIC ",
    "ECM: Data Bus Wait TSEC1 ",
    "ECM: Data Bus Wait TSEC2 ",
    "ECM: Data Global Data Bus Beat ",
    "ECM: e500 Direct Read Bus Beat ",
    "ECM: e500 Direct Read Bus Beat Forwarded ",
    "ECM: Cancel ",
    "PIC: Total Interrupt Count ",
    "PIC: Interrupt Wait Cycles ",
    "PIC: Interrupt Service Cycles ",
    "PIC: Interrupt Select 0 ",
    "PIC: Interrupt Select 1 ",
    "PIC: Interrupt Select 2 ",
    "PIC: Interrupt Select 3 ",
    "PCI/PCI-X: Clock Cycles ",
    "PCI/PCI-X: Inbound Memory Reads ",
    "PCI/PCI-X: Inbound Memory Writes ",
    "PCI/PCI-X: Inbound Configuration Reads ",
    "PCI/PCI-X: Inbound Configuration Writes ",
    "PCI/PCI-X: Outbound Memory Reads ",
    "PCI/PCI-X: Outbound Memory Writes ",
    "PCI/PCI-X: Outbound I/O Reads ",
    "PCI/PCI-X: Outbound I/O Writes ",
    "PCI/PCI-X: Outbound Configuration Reads ",
    "PCI/PCI-X: Outbound Configuration Writes ",
    "PCI/PCI-X: Inbound Total Read Data Beats ",
    "PCI/PCI-X: Inbound Total Write Data Beats ",
    "PCI/PCI-X: Outbound Total Read Data Beats ",
    "PCI/PCI-X: Outbound Total Write Data Beats ",
    "PCI/PCI-X: Inbound 32-bit Read Data Beats ",
    "PCI/PCI-X: Inbound 32-bit Write Data Beats ",
    "PCI/PCI-X: Outbound 32-bit Read Data Beats ",
    "PCI/PCI-X: Outbound 32-bit Write Data Beats ",
    "PCI/PCI-X: Inbound 64-bit Read Data Beats ",
    "PCI/PCI-X: Inbound 64-bit Write Data Beats ",
    "PCI/PCI-X: Outbound 64-bit Read Data Beats ",
    "PCI/PCI-X: Outbound 64-bit Write Data Beats ",
    "PCI/PCI-X: Total Transactions ",
    "PCI/PCI-X: 64-bit Transactions ",
    "PCI/PCI-X: Inbound Purgeable Reads ",
    "PCI/PCI-X: Inbound Purgeable Reads Discarded ",
    "PCI/PCI-X: Idle Cycles ",
    "PCI/PCI-X: Dual Address Cycles ",
    "PCI/PCI-X: Internal Cycles ",
    "PCI/PCI-X: Inbound Memory Reads ",
    "PCI/PCI-X: Inbound Memory Read Lines ",
    "PCI/PCI-X: Inbound Memory Read Multiple ",
    "PCI/PCI-X: Outbound Memory Reads ",
    "PCI/PCI-X: Outbound Memory Read Lines ",
    "PCI/PCI-X: Waits ",
    "PCI: Cycles ~PCI_IRDY Is Asserted ",
    "PCI: Cycles ~PCI_TRDY Is Asserted ",
    "PCI: Cycles ~PCI_FRAME Is Asserted ",
    "PCI-X: 1 Split Transaction ",
    "PCI-X: 2 Split Transaction ",
    "PCI-X: 3 Split Transaction ",
    "PCI-X: 4 Split Transaction ",
    "PCI-X: Split Responses ",
    "PCI-X: ADB Disconnects ",
    "PCI/PCI-X: Snoopable ",
    "PCI/PCI-X: Write Stash ",
    "PCI/PCI-X: Write Stash With Lock ",
    "PCI/PCI-X: Write Read Unlock ",
    "PCI/PCI-X: Byte Enable Transactions ",
    "PCI/PCI-X: Non-byte Enable Transactions ",
    "TSEC1 Address Data Filtering: Accepted Frames ",
    "TSEC1 Address Data Filtering: Individual Hash Table Accepted Frames ",
    "TSEC1 Address Data Filtering: Group Hash Table Accepted Frames ",
    "TSEC1 Address Data Filtering: Rejected Frames ",
    "TSEC1 Address Data Filtering: Dropped Frames ",
    "TSEC1 Address Data Filtering: Dropped Frames Due To Data Overflow ",
    "TSEC1 Address Data Filtering: Dropped Frames Due To Status Overflow ",
    "TSEC1 FIFO: Receive FIFO Data Valid ",
    "TSEC1 FIFO: Transmit Frames Without Threshold ",
    "TSEC1 FIFO: Receive FIFO above 1/4 ",
    "TSEC1 FIFO: Receive FIFO above 1/2 ",
    "TSEC1 FIFO: Receive FIFO above 3/4 ",
    "TSEC1 FIFO: DMA Reads ",
    "TSEC1 FIFO: Buffer Descriptor Reads ",
    "TSEC1 FIFO: Receive Buffer Descriptor Reads ",
    "TSEC1 FIFO: DMA Writes ",
    "TSEC1 FIFO: Buffer Descriptor Writes ",
    "TSEC1 FIFO: Receive Buffer Descriptor Writes ",
    "TSEC1 FIFO: Receive Buffer Descriptor Read Latency ",
    "TSEC1 FIFO: Transmit Buffer Descriptor Read Latency ",
    "TSEC1 FIFO: Receive Buffer Descriptor Write Latency ",
    "TSEC1 FIFO: Transmit Buffer Descriptor Write Latency ",
    "TSEC1 FIFO: Transmit Data Read Latency ",
    "TSEC1 FIFO: Data Beats ",
    "TSEC1 FIFO: Read Data Beats ",
    "TSEC1 FIFO: Receive Frame Interrupts ",
    "TSEC1 FIFO: Transmit Frame Interrupts ",
    "TSEC1 FIFO: Receive Frame Processing ",
    "TSEC2 Address Data Filtering: Accepted Frames ",
    "TSEC2 Address Data Filtering: Individual Hash Table Accepted Frames ",
    "TSEC2 Address Data Filtering: Group Hash Table Accepted Frames ",
    "TSEC2 Address Data Filtering: Rejected Frames ",
    "TSEC2 Address Data Filtering: Dropped Frames ",
    "TSEC2 Address Data Filtering: Dropped Frames Due To Data Overflow ",
    "TSEC2 Address Data Filtering: Dropped Frames Due To Status Overflow ",
    "TSEC2 FIFO: Receive FIFO Data Valid ",
    "TSEC2 FIFO: Transmit Frames Without Threshold ",
    "TSEC2 FIFO: Receive FIFO above 1/4 ",
    "TSEC2 FIFO: Receive FIFO above 1/2 ",
    "TSEC2 FIFO: Receive FIFO above 3/4 ",
    "TSEC2 FIFO: DMA Reads ",
    "TSEC2 FIFO: Buffer Descriptor Reads ",
    "TSEC2 FIFO: Receive Buffer Descriptor Reads ",
    "TSEC2 FIFO: DMA Writes ",
    "TSEC2 FIFO: Buffer Descriptor Writes ",
    "TSEC2 FIFO: Receive Buffer Descriptor Writes ",
    "TSEC2 FIFO: Receive Buffer Descriptor Read Latency ",
    "TSEC2 FIFO: Transmit Buffer Descriptor Read Latency ",
    "TSEC2 FIFO: Receive Buffer Descriptor Write Latency ",
    "TSEC2 FIFO: Transmit Buffer Descriptor Write Latency ",
    "TSEC2 FIFO: Transmit Data Read Latency ",
    "TSEC2 FIFO: Data Beats ",
    "TSEC2 FIFO: Read Data Beats ",
    "TSEC2 FIFO: Receive Frame Interrupts ",
    "TSEC2 FIFO: Transmit Frame Interrupts ",
    "TSEC2 FIFO: Receive Frame Processing ",
    "Local Bus: Bank 1 Hits ",
    "Local Bus: Bank 2 Hits ",
    "Local Bus: Bank 3 Hits ",
    "Local Bus: Bank 4 Hits ",
    "Local Bus: Bank 5 Hits ",
    "Local Bus: Bank 6 Hits ",
    "Local Bus: Bank 7 Hits ",
    "Local Bus: Bank 8 Hits ",
    "Local Bus: Requests Granted To ECM Port ",
    "Local Bus: Cycles ECM Port's Atomic Reservation Is Enabled ",
    "Local Bus: ECM Port Atomic Reservation Timouts ",
    "Local Bus: Cycles Per GPCM Read ",
    "Local Bus: Cycles Per UPM Read ",
    "Local Bus: Cycles Per SDRAM Read ",
    "Local Bus: Cycles Per GPCM Write ",
    "Local Bus: Cycles Per UPM Write ",
    "Local Bus: Cycles Per SDRAM Write ",
    "Local Bus: SDRAM Bank Misses ",
    "Local Bus: SDRAM Page Misses ",
    "L2 Cache/SRAM: Core Instruction L2 Hits ",
    "L2 Cache/SRAM: Core Instruction L2 Misses ",
    "L2 Cache/SRAM: Core Data L2 Hits ",
    "L2 Cache/SRAM: Core Data L2 Misses ",
    "L2 Cache/SRAM: Non-core Burst L2 Writes ",
    "L2 Cache/SRAM: Non-core, Non-burst L2 Writes ",
    "L2 Cache/SRAM: Non-core write misses ",
    "L2 Cache/SRAM: Non-core Read L2 Hits ",
    "L2 Cache/SRAM: Non-core Read L2 Misses ",
    "L2 Cache/SRAM: Allocates ",
    "L2 Cache/SRAM: Write Queue Full Caused Retries ",
    "L2 Cache/SRAM: Address Collision Caused Retries ",
    "L2 Cache/SRAM: Full Set Caused Lock Failures ",
    "L2 Cache/SRAM: Valid Line Victimizations ",
    "L2 Cache/SRAM: Invalidation Of Lines ",
    "L2 Cache/SRAM: Clearing Of Locks ",
    "Debug: External ",
    "Debug: Watchpoint Monitor Hits ",
    "Debug: Trace Buffer Hits ",
    "DUART: UART0 Baud Rate ",
    "DUART: UART1 Baud Rate ",
    "Chaining: Performance Monitor Counter 0 Carry Out ",
    "Chaining: Performance Monitor Counter 1 Carry Out ",
    "Chaining: Performance Monitor Counter 2 Carry Out ",
    "Chaining: Performance Monitor Counter 3 Carry Out ",
    "Chaining: Performance Monitor Counter 4 Carry Out ",
    "Chaining: Performance Monitor Counter 5 Carry Out ",
    "Chaining: Performance Monitor Counter 6 Carry Out ",
    "Chaining: Performance Monitor Counter 7 Carry Out ",
    "Chaining: Performance Monitor Counter 8 Carry Out"
    "EPIC: Interprocessor Interrupt IRQ0 ",
    "EPIC: Interprocessor Interrupt IRQ1 ",
    "EPIC: Interprocessor Interrupt IRQ2 ",
    "EPIC: Interprocessor Interrupt IRQ3 ",
    "EPIC: Timer Interrupt IRQ0 ",
    "EPIC: Timer Interrupt IRQ1 ",
    "EPIC: Timer Interrupt IRQ2 ",
    "EPIC: Timer Interrupt IRQ3 ",
    "EPIC: Message Interrupt IRQ0 ",
    "EPIC: Message Interrupt IRQ1 ",
    "EPIC: Message Interrupt IRQ2 ",
    "EPIC: Message Interrupt IRQ3 ",
    "EPIC: External Interrupt IRQ0 ",
    "EPIC: External Interrupt IRQ1 ",
    "EPIC: External Interrupt IRQ2 ",
    "EPIC: External Interrupt IRQ3 ",
    "EPIC: External Interrupt IRQ4 ",
    "EPIC: External Interrupt IRQ5 ",
    "EPIC: External Interrupt IRQ6 ",
    "EPIC: External Interrupt IRQ7 ",
    "EPIC: External Interrupt IRQ8 ",
    "EPIC: External Interrupt IRQ9 ",
    "EPIC: External Interrupt IRQ10 ",
    "EPIC: External Interrupt IRQ11 ",
    "EPIC: Internal Interrupt IRQ1 ",
    "EPIC: Internal Interrupt IRQ2 ",
    "EPIC: Internal Interrupt IRQ3 ",
    "EPIC: Internal Interrupt IRQ5 ",
    "EPIC: Internal Interrupt IRQ6 ",
    "EPIC: Internal Interrupt IRQ7 ",
    "EPIC: Internal Interrupt IRQ8 ",
    "EPIC: Internal Interrupt IRQ9 ",
    "EPIC: Internal Interrupt IRQ10 ",
    "EPIC: Internal Interrupt IRQ11 ",
    "EPIC: Internal Interrupt IRQ12 ",
    "EPIC: Internal Interrupt IRQ13 ",
    "EPIC: Internal Interrupt IRQ14 ",
    "EPIC: Internal Interrupt IRQ18 ",
    "EPIC: Internal Interrupt IRQ19 ",
    "EPIC: Internal Interrupt IRQ20 ",
    "EPIC: Internal Interrupt IRQ24 ",
    "EPIC: Internal Interrupt IRQ25 ",
    "EPIC: Internal Interrupt IRQ26 ",
    "EPIC: Internal Interrupt IRQ27 ",
    "EPIC: Internal Interrupt IRQ28 ",
    };

/*
 * This array contains descriptive full name strings corresponding to
 * each event class. The array order maps one-to-one with the
 * PERFMON_EVENT_CLASSES  enumeration allowing the enumeration defines to 
 * be used as indexes the textual class names.
 */

LOCAL const char *className[] =
    {
    "General ",
    "DDR Memory Controller ",
    "Memory Target Queue ",
    "RapidIO Controller ",
    "DMA Controller ",
    "e500 Coherency Module ",
    "Programmable Interrupt Controller (PIC) ",
    "PCI/PCI-X Common ",
    "PCI Specific ",
    "Three-speed Ethernet Controller (TSEC) ",
    "Local Bus ",
    "L2 Cache/SRAM ",
    "Debug ",
    "DUART ",
    "Performance Monitor Counter Chaining"
    };

/*
 * This array contains the event data records that fully describe
 * each available event that can be monitored. The record order
 * corresponds one-to-one with the PERFMON_EVENT enumeration such
 * the enumeration defines can be used as indexes to retrieve the
 * corresponding data record. This array is private and can be
 * accessed only through the routines provided below.
 */

LOCAL const EVENT_INFO events[PERFMON_NUM_EVENTS] =
    {
        { 
        GENERAL,
        "CCB (platform) clock cycles", 
        "",
        0, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "each data beat that a read is returning on the SDRAM interface",
        "",
        PERFMON_REF_EVENT, 10
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "each data beat that is transferred to or from the SDRAM",
        "",
        PERFMON_REF_EVENT, 11
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read misses issued when a read is outstanding",
        "",
        1, 57
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read/write misses issued when a read/write is ",
        "outstanding ",
        2, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read misses issued when a no reads are ",
        "outstanding",
        3, 60
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read/write misses issued when no reads/writes ",
        "are outstanding ",
        4, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read hits issued when a read is outstanding",
        "",
        5, 56
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read/write hits issued when a read/write is",
        "outstanding ",
        6, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read hits issued when no read is outstanding",
        "",
        7, 57
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "row open table read/write hits issued when no reads/writes ",
        "are outstanding",
        8, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "precharges issued to the DDR SDRAM for any reason except refresh",
        "",
        1, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "transactions that miss in the row open table",
        "",
        2, 1
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "transactions that hit in the row open table",
        "",
        3, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "forced page closings including those due to refreshes",
        "",
        4, 1
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "read-modify-write transactions due to ECC being enabled",
        "",
        5, 0
        },

        { 
        DDR_MEMORY_CONTROLLER,
        "forced page closings due to collision with bank and sub-bank",
        "",
        PERFMON_REF_EVENT, 12
        },

        {
        DDR_MEMORY_CONTROLLER,
        "reads or writes from core",
        "",
        PERFMON_REF_EVENT, 13
        },

        {
        DDR_MEMORY_CONTROLLER,
        "reads or writes from TSEC 1",
        "",
        3, 1
        },

        {
        DDR_MEMORY_CONTROLLER,
        "reads or writes from TSEC 2",
        "",
        4, 2
        },

        {
        DDR_MEMORY_CONTROLLER,
        "reads or writes from RapidIO",
        "",
        3, 2
        },

        {
        DDR_MEMORY_CONTROLLER,
        "reads or writes from PCI",
        "",
        4, 3
        },

        {
        DDR_MEMORY_CONTROLLER,
        "reads or writes from DMA",
        "",
        5, 2
        },

        {
        DDR_MEMORY_CONTROLLER,
        "row open table hits for reads or writes from core",
        "",
        PERFMON_REF_EVENT, 14
        },

        {
        DDR_MEMORY_CONTROLLER,
        "row open table hits for reads or writes from TSEC 1",
        "",
        6, 1
        },

        {
        DDR_MEMORY_CONTROLLER,
        "row open table hits for reads or writes from TSEC 2",
        "",
        7, 0
        },

        {
        DDR_MEMORY_CONTROLLER,
        "row open table hits for reads or writes from RapidIO",
        "",
        6, 2
        },

        {
        DDR_MEMORY_CONTROLLER,
        "row open table hits for reads or writes from PCI",
        "",
        7, 1
        },

        {
        DDR_MEMORY_CONTROLLER,
        "row open table hits for reads or writes from DMA",
        "",
        8, 2
        },

        {
        MEMORY_TARGET_QUEUE,
        "memory target queue read/write address collisions",
        "",
        5, 5
        },

        {
        RAPIDIO_CONTROLLER,
        "new transactions loaded into the misaligned engine",
        "",
        4, 11
        },

        {
        RAPIDIO_CONTROLLER,
        "only packets that caused a retry due to resource limitations, ",
        "not ignored packets that must be re-sent",
        2, 62
        },

        {
        RAPIDIO_CONTROLLER,
        "flushed packets due to prior retries or error recovery ", 
        "on RapidIO transmit ",
        3, 10
        },

        {
        RAPIDIO_CONTROLLER,
        "retried or not accepted packets from a remote device ",
        "on RapidIO receive, not ignored packets that must be re-sent",
        5, 7
        },

        {
        RAPIDIO_CONTROLLER,
        "misaligned engine busies for priority 2 transactions",
        "",
        5, 12
        },

        {
        RAPIDIO_CONTROLLER,
        "misaligned engine busies for priority 1 transactions",
        "",
        6, 12
        },

        {
        RAPIDIO_CONTROLLER,
        "misaligned engine busies for priority 0 transactions",
        "",
        7, 10
        },

        {
        RAPIDIO_CONTROLLER,
        "outstanding un-ACKed packets",
        "",
        4, 61
        },

        {
        RAPIDIO_CONTROLLER,
        "events asserted during cycles when packet transmission is ",
        "stopped for training",
        6, 13
        },

        {
        RAPIDIO_CONTROLLER,
        "events asserted during cycles when packet transmission is ",
        "stopped due to an inbound ACK retry",
        7, 11
        },

        {
        RAPIDIO_CONTROLLER,
        "events asserted during cycles when packet transmission is ",
        "stopped due to an inbound ACK not accepted",
        8, 11
        },

        {
        RAPIDIO_CONTROLLER,
        "protocol level retries for inbound message packets",
        "",
        4, 18
        },

        {
        RAPIDIO_CONTROLLER,
        "protocol level retries for inbound doorbell packets",
        "",
        6, 14
        },

        {
        RAPIDIO_CONTROLLER,
        "outbound ACK timeouts or out-of-sequence ACKs detected by ",
        "the ACK history queue",
        8, 12
        },

        {
        DMA_CONTROLLER,
        "channel 0 read requests active in the system",
        "",
        1, 2
        },

        {
        DMA_CONTROLLER,
        "channel 1 read requests active in the system",
        "",
        2, 5
        },

        {
        DMA_CONTROLLER,
        "channel 2 read requests active in the system",
        "",
        3, 4
        },

        {
        DMA_CONTROLLER,
        "channel 3 read requests active in the system",
        "",
        4, 6
        },

        {
        DMA_CONTROLLER,
        "channel 0 write requests active in the system",
        "",
        1, 3
        },

        {
        DMA_CONTROLLER,
        "channel 1 write requests active in the system",
        "",
        2, 6
        },

        {
        DMA_CONTROLLER,
        "channel 2 write requests active in the system",
        "",
        3, 5
        },

        {
        DMA_CONTROLLER,
        "channel 3 write requests active in the system",
        "",
        4, 7
        },

        {
        DMA_CONTROLLER,
        "channel 0 descriptor requests active in the system",
        "",
        5, 41
        },

        {
        DMA_CONTROLLER,
        "channel 1 descriptor requests active in the system",
        "",
        6, 44
        },

        {
        DMA_CONTROLLER,
        "channel 2 descriptor requests active in the system",
        "",
        7, 41
        },

        {
        DMA_CONTROLLER,
        "channel 3 descriptor requests active in the system",
        "",
        8, 41
        },

        {
        DMA_CONTROLLER,
        "channel 0 valid double word (or less) reads",
        "",
        1, 4
        },

        {
        DMA_CONTROLLER,
        "channel 0 valid double word (or less) reads (alternate)",
        "",
        5, 53
        },

        {
        DMA_CONTROLLER,
        "channel 1 valid double word (or less) reads",
        "",
        2, 7
        },

        {
        DMA_CONTROLLER,
        "channel 1 valid double word (or less) reads (alternate)",
        "",
        6, 58
        },

        {
        DMA_CONTROLLER,
        "channel 2 valid double word (or less) reads",
        "",
        3, 6
        },

        {
        DMA_CONTROLLER,
        "channel 2 valid double word (or less) reads (alternate)",
        "",
        7, 54
        },

        {
        DMA_CONTROLLER,
        "channel 3 valid double word (or less) reads",
        "",
        4, 8
        },

        {
        DMA_CONTROLLER,
        "channel 3 valid double word (or less) reads (alternate)",
        "",
        8, 52
        },

        {
        DMA_CONTROLLER,
        "channel 0 valid double word (or less) writes",
        "",
        1, 5
        },

        {
        DMA_CONTROLLER,
        "channel 1 valid double word (or less) writes",
        "",
        2, 8
        },

        {
        DMA_CONTROLLER,
        "channel 2 valid double word (or less) writes",
        "",
        3, 7
        },

        {
        DMA_CONTROLLER,
        "channel 3 valid double word (or less) writes",
        "",
        4, 9
        },

        {
        E500_COHERENCY_MODULE,
        "requests for core wait", 
        "",
        8, 13
        },

        {
        E500_COHERENCY_MODULE,
        "requests for TSEC 1 wait", 
        "",
        5, 16
        },

        {
        E500_COHERENCY_MODULE,
        "requests for TSEC 2 wait", 
        "",
        6, 16
        },

        {
        E500_COHERENCY_MODULE,
        "requests for PCI/RapidIO/DMA wait",
        "",
        4, 20
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches including address only's",
        "",
        PERFMON_REF_EVENT, 15
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches from core including address only's",
        "",
        1, 16
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches from TSEC 1",
        "",
        3, 19
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches from TSEC 2",
        "",
        4, 21
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches from RapidIO",
        "",
        5, 17
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches from PCI",
        "",
        6, 17
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches from DMA",
        "",
        7, 14
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches from other",
        "",
        8, 14
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches to DDR",
        "",
        4, 22
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches to L2/SRAM",
        "",
        5, 18
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches to LBC",
        "",
        6, 18
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches to RapidIO",
        "",
        7, 15
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM dispatches to PCI",
        "",
        8, 15
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM snoopable dispatches",
        "",
        3, 20
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM write dispatches",
        "",
        1, 17
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM write allocate dispatches",
        "",
        2, 21
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM write allocate/lock dispatches",
        "",
        3, 21
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM read dispatches",
        "",
        4, 23
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM read unlock dispatches",
        "",
        5, 19
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM read clear atomic dispatches",
        "",
        6, 19
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM read set atomic dispatches",
        "",
        7, 16
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM read decrement atomic dispatches",
        "",
        8, 16
        },

        {
        E500_COHERENCY_MODULE,
        "committed ECM read increment atomic dispatches",
        "",
        7, 17
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus grants for DDR",
        "",
        1, 18
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus grants for LBC",
        "",
        2, 22
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus grants for PIC",
        "",
        1, 19
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus grants for TSEC 1",
        "",
        3, 23
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus grants for TSEC 2",
        "",
        4, 25
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus waits for DDR",
        "",
        5, 20
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus waits for LBC",
        "",
        6, 20
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus waits for PIC",
        "",
        5, 21
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus waits for TSEC 1",
        "",
        7, 19
        },

        {
        E500_COHERENCY_MODULE,
        "ECM data bus waits for TSEC 2",
        "",
        8, 18
        },

        {
        E500_COHERENCY_MODULE,
        "ECM global data bus beats",
        "",
        PERFMON_REF_EVENT, 16
        },

        {
        E500_COHERENCY_MODULE,
        "ECM e500 direct read bus beats",
        "",
        PERFMON_REF_EVENT, 17
        },

        {
        E500_COHERENCY_MODULE,
        "ECM e500 direct read bus beats forwarded directly to the ",
        "e500 R1 data bus",
        2, 24
        },

        {
        E500_COHERENCY_MODULE,
        "ECM cancels",
        "",
        PERFMON_REF_EVENT, 18
        },

        {
        INTERRUPT_CONTROLLER,
        "total number of interrupts serviced",
        "",
        PERFMON_REF_EVENT, 26
        },

        {
        INTERRUPT_CONTROLLER,
        "cycles an interrupt waits to be acknowledged",
        "",
        8, 62
        },

        {
        INTERRUPT_CONTROLLER,
        "cycles for an interrupt currently being serviced",
        "",
        2, 19
        },

        {
        INTERRUPT_CONTROLLER,
        "for selected interrupt, the interrupt count over threshold",
        "",
        1, 56
        },

        {
        INTERRUPT_CONTROLLER,
        "for selected interrupt, the interrupt count over threshold",
        "",
        3, 59
        },

        {
        INTERRUPT_CONTROLLER,
        "for selected interrupt, the interrupt count over threshold",
        "",
        5, 55
        },

        {
        INTERRUPT_CONTROLLER,
        "for selected interrupt, the interrupt count over threshold",
        "",
        6, 60
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X clock cycles",
        "",
        PERFMON_REF_EVENT, 28
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound memory reads of all types",
        "",
        1, 62
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound memory writes",
        "",
        2, 37
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound configuration reads",
        "",
        3, 63
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound configuration writes",
        "",
        4, 37
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound memory read of all types",
        "",
        5, 30
        },

        {
        PCI_PCIX_COMMON,
        "PCI outbound memory writes or PCI-X outbound memory writes ", 
        "attempted",
        6, 32
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound I/O reads", 
        "",
        3, 37
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound I/O writes", 
        "",
        4, 38
        },

        {
        PCI_PCIX_COMMON,
        "PCI outbound configuration reads or PCI-X outbound configuration", 
        "writes attempted",
        7, 26
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound configuration writes", 
        "",
        8, 26
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound total read data beats for both 32 bit ",
        "and 64 bit transactions",
        5, 32
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound total write data beats for both 32 bit ",
        "and 64 bit transactions",
        6, 34
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound total read data beats for both 32 bit ",
        "and 64 bit transactions",
        7, 28
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound total write data beats for both 32 bit ",
        "and 64 bit transactions",
        8, 28
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound 32 bit read data beats",
        "",
        1, 30
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound 32 bit write data beats",
        "",
        2, 38
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound 32 bit read data beats",
        "",
        3, 38
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound 32 bit write data beats",
        "",
        4, 39
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound 64 bit read data beats",
        "",
        5, 31
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound 64 bit write data beats",
        "",
        6, 33
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound 64 bit read data beats",
        "",
        7, 27
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X outbound 64 bit write data beats",
        "",
        8, 27
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X total 32 bit and 64 bit transactions",
        "",
        7, 29
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X total 64 bit transactions",
        "",
        8, 29
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound purgeable reads",
        "",
        2, 2
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X inbound (speculative) purgeable reads discarded",
        "",
        8, 63
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X idle cycles",
        "",
        1, 31
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X dual address cycles",
        "",
        2, 40
        },

        {
        PCI_PCIX_COMMON,
        "PCI and PCI-X internal cycles",
        "",
        3, 39
        },

        {
        PCI_PCIX_COMMON,
        "PCI inbound memory reads; PCI-X inbound memory 32 bit reads",
        "",
        1, 34
        },

        {
        PCI_PCIX_COMMON,
        "PCI inbound memory read lines; PCI-X inbound memory alias reads",
        "",
        2, 44
        },

        {
        PCI_PCIX_COMMON,
        "PCI inbound memory read multiples; PCI-X inbound memory ",
        "block reads",
        3, 42
        },

        {
        PCI_PCIX_COMMON,
        "PCI outbound memory reads; PCI-X inbound memory double ",
        "word reads attempted",
        4, 43
        },

        {
        PCI_PCIX_COMMON,
        "PCI outbound memory read lines; PCI-X outbound memory ",
        "burst reads attempted",
        5, 36
        },

        {
        PCI_PCIX_COMMON,
        "PCI waits; PCI-X initial waits; (~PCI_IRDY and ~PCI_TRDY not ",
        "both asserted)",
        1, 35
        },

        {
        PCI_SPECIFIC,
        "PCI cycles that ~PCI_IRDY is asserted",
        "",
        6, 36
        },

        {
        PCI_SPECIFIC,
        "PCI cycles that ~PCI_TRDY is asserted",
        "",
        7, 31
        },

        {
        PCI_SPECIFIC,
        "PCI cycles that ~PCI_FRAME is asserted",
        "",
        8, 31
        },

        {
        PCI_SPECIFIC,
        "PCI-X cycles that there are one or more outbound read ",
        "transactions awaiting completion",
        2, 41
        },

        {
        PCI_SPECIFIC,
        "PCI-X cycles that there are two or more outbound read ",
        "transactions awaiting completion",
        3, 40
        },

        {
        PCI_SPECIFIC,
        "PCI-X cycles that there are three or more outbound read ",
        "transactions awaiting completion",
        4, 41
        },

        {
        PCI_SPECIFIC,
        "PCI-X cycles that there are four or more outbound read ",
        "transactions awaiting completion",
        5, 34
        },

        {
        PCI_SPECIFIC,
        "PCI-X split responses received for an outbound transaction",
        "",
        6, 37
        },

        {
        PCI_SPECIFIC,
        "PCI-X ADB disconnects",
        "",
        7, 32
        },

        {
        PCI_SPECIFIC,
        "PCI and PCI-X snoopables",
        "",
        1, 32
        },

        {
        PCI_SPECIFIC,
        "PCI and PCI-X write stashes",
        "",
        2, 42
        },

        {
        PCI_SPECIFIC,
        "PCI and PCI-X write stashes with lock",
        "",
        3, 41
        },

        {
        PCI_SPECIFIC,
        "PCI and PCI-X read unlocks",
        "",
        4, 42
        },

        {
        PCI_SPECIFIC,
        "PCI and PCI-X byte enable transactions",
        "",
        1, 33
        },

        {
        PCI_SPECIFIC,
        "PCI and PCI-X non-byte enable transactions",
        "",
        2, 43
        },

        {
        TSEC,
        "accepted frames",
        "",
        PERFMON_REF_EVENT, 36
        },

        {
        TSEC,
        "individual hash table accepted frames",
        "",
        7, 35
        },

        {
        TSEC,
        "group hash table accepted frames",
        "",
        8, 35
        },

        {
        TSEC,
        "rejected frames",
        "",
        PERFMON_REF_EVENT, 39
        },

        {
        TSEC,
        "dropped frames that could have accepted", 
        "",
        PERFMON_REF_EVENT, 38
        },

        {
        TSEC,
        "dropped frames due to FIFO overflow or ", 
        "256 byte buffer overflow",
        1, 41
        },

        {
        TSEC,
        "dropped frames due to the inability to ",
        "write status of one frame before a new frame starts",
        2, 50
        },

        {
        TSEC,
        "number of times receive FIFO data is valid",
        "",
        1, 45
        },

        {
        TSEC,
        "number of transmit frames that do not hit the transmit ",
        "FIFO threshold",
        7, 44
        },

        {
        TSEC,
        "receive FIFO above 1/4 full",
        "",
        PERFMON_REF_EVENT, 47
        },

        {
        TSEC,
        "receive FIFO above 1/2 full",
        "",
        PERFMON_REF_EVENT, 48
        },

        {
        TSEC,
        "receive FIFO above 3/4 full",
        "",
        PERFMON_REF_EVENT, 49
        },

        {
        TSEC,
        "descriptor and data reads",
        "",
        1, 47
        },

        {
        TSEC,
        "transmit and receive buffer descriptor reads",
        "",
        2, 54
        },

        {
        TSEC,
        "receive buffer descriptor reads",
        "",
        3, 51
        },

        {
        TSEC,
        "descriptor and data writes",
        "",
        4, 52
        },

        {
        TSEC,
        "descriptor and data closings (writes)",
        "",
        5, 46
        },

        {
        TSEC,
        "receive buffer descriptor closings (writes)",
        "",
        6, 49
        },

        {
        TSEC,
        "DMA: number of times receive buffer descriptor read latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 41
        },

        {
        TSEC,
        "number of times transmit buffer descriptor read latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 42
        },

        {
        TSEC,
        "number of times receive buffer descriptor write latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 43
        },

        {
        TSEC,
        "number of times transmit buffer descriptor write latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 44
        },

        {
        TSEC,
        "number of times transmit data read response latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 45
        },

        {
        TSEC,
        "data beats",
        "",
        7, 48
        },

        {
        TSEC,
        "read data beats",
        "",
        8, 46
        },

        {
        TSEC,
        "receive frame interrupts",
        "",
        1, 49
        },

        {
        TSEC,
        "transmit frame interrupts",
        "",
        3, 53
        },

        {
        TSEC,
        "number of times receive frame processing exceeds ",
        "duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 46
        },

        {
        TSEC,
        "accepted frames",
        "",
        PERFMON_REF_EVENT, 50
        },

        {
        TSEC,
        "individual hash table accepted frames",
        "",
        7, 36
        },

        {
        TSEC,
        "group hash table accepted frames",
        "",
        8, 36
        },

        {
        TSEC,
        "rejected frames",
        "",
        PERFMON_REF_EVENT, 53
        },

        {
        TSEC,
        "dropped frames that could have accepted", 
        "",
        PERFMON_REF_EVENT, 52
        },

        {
        TSEC,
        "dropped frames due to FIFO overflow or 256 byte buffer overflow",
        "",
        1, 42
        },

        {
        TSEC,
        "dropped frames due to the inability to write status of one ",
        "frame before a new frame starts",
        2, 51
        },

        {
        TSEC,
        "number of times receive FIFO data is valid",
        "",
        1, 46
        },

        {
        TSEC,
        "number of transmit frames that do not hit the transmit ",
        "FIFO threshold",
        7, 45
        },

        {
        TSEC,
        "receive FIFO above 1/4 full",
        "",
        PERFMON_REF_EVENT, 61
        },

        {
        TSEC,
        "receive FIFO above 1/2 full",
        "",
        PERFMON_REF_EVENT, 62
        },

        {
        TSEC,
        "receive FIFO above 3/4 full",
        "",
        PERFMON_REF_EVENT, 63
        },

        {
        TSEC,
        "descriptor and data reads",
        "",
        1, 48
        },

        {
        TSEC,
        "transmit and receive buffer descriptor reads",
        "",
        2, 55
        },

        {
        TSEC,
        "receive buffer descriptor reads",
        "",
        3, 52
        },

        {
        TSEC,
        "descriptor and data writes",
        "",
        4, 53
        },

        {
        TSEC,
        "descriptor and data closings (writes)",
        "",
        5, 47
        },

        {
        TSEC,
        "receive buffer descriptor closings (writes)",
        "",
        6, 50
        },

        {
        TSEC,
        "number of times receive buffer descriptor read latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 55
        },

        {
        TSEC,
        "number of times transmit buffer descriptor read latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 56
        },

        {
        TSEC,
        "number of times receive buffer descriptor write latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 57
        },

        {
        TSEC,
        "number of times transmit buffer descriptor write latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 58
        },

        {
        TSEC,
        "number of times transmit data read response latency ",
        "exceeds duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 59
        },

        {
        TSEC,
        "data beats",
        "",
        7, 49
        },

        {
        TSEC,
        "read data beats",
        "",
        8, 47
        },

        {
        TSEC,
        "receive frame interrupts",
        "",
        1, 50
        },

        {
        TSEC,
        "transmit frame interrupts",
        "",
        3, 54
        },

        {
        TSEC,
        "number of times receive frame processing exceeds ",
        "duration threshold causing a threshold event",
        PERFMON_REF_EVENT, 60
        },

        {
        LOCAL_BUS,
        "bank 1 hits (chip select)",
        "",
        1, 51
        },

        {
        LOCAL_BUS,
        "bank 2 hits (chip select)",
        "",
        2, 56
        },

        {
        LOCAL_BUS,
        "bank 3 hits (chip select)",
        "",
        3, 55
        },

        {
        LOCAL_BUS,
        "bank 4 hits (chip select)",
        "",
        4, 54
        },

        {
        LOCAL_BUS,
        "bank 5 hits (chip select)",
        "",
        5, 48
        },

        {
        LOCAL_BUS,
        "bank 6 hits (chip select)",
        "",
        6, 53
        },

        {
        LOCAL_BUS,
        "bank 7 hits (chip select)",
        "",
        7, 50
        },

        {
        LOCAL_BUS,
        "bank 8 hits (chip select)",
        "",
        8, 50
        },

        {
        LOCAL_BUS,
        "requests granted to ECM port",
        "",
        2, 57
        },

        {
        LOCAL_BUS,
        "number of cycles atomic reservation for ECM port is enabled",
        "",
        4, 55
        },

        {
        LOCAL_BUS,
        "atomic reservation time-outs for ECM port",
        "",
        6, 54
        },

        {
        LOCAL_BUS,
        "number of cycles a read is taking in GPMC",
        "",
        1, 53
        },

        {
        LOCAL_BUS,
        "number of cycles a read is taking in UPM",
        "",
        2, 58
        },

        {
        LOCAL_BUS,
        "number of cycles a read is taking in SDRAM",
        "",
        3, 57
        },

        {
        LOCAL_BUS,
        "number of cycles a write is taking in GPMC",
        "",
        4, 56
        },

        {
        LOCAL_BUS,
        "number of cycles a write is taking in UPM",
        "",
        5, 50
        },

        {
        LOCAL_BUS,
        "number of cycles a write is taking in SDRAM",
        "",
        6, 55
        },

        {
        LOCAL_BUS,
        "SDRAM bank misses",
        "",
        7, 51
        },

        {
        LOCAL_BUS,
        "SDRAM page misses",
        "",
        8, 51
        },

        {
        L2_CACHE_SRAM,
        "core instruction accesses to L2 that are hits",
        "",
        PERFMON_REF_EVENT, 22
        },

        {
        L2_CACHE_SRAM,
        "core instruction accesses to L2 that are misses",
        "",
        2, 59
        },

        {
        L2_CACHE_SRAM,
        "core data accesses to L2 that are hits",
        "",
        PERFMON_REF_EVENT, 23
        },

        {
        L2_CACHE_SRAM,
        "core data accesses to L2 that are misses",
        "",
        4, 57
        },

        {
        L2_CACHE_SRAM,
        "non-core external burst writes to L2 or SRAM", 
        "",
        5, 51
        },

        {
        L2_CACHE_SRAM,
        "non-core non-burst writes to L2", 
        "",
        6, 56
        },

        {
        L2_CACHE_SRAM,
        "non-core writes that miss the L2 external write window and ",
        "SRAM memory range",
        7, 52
        },

        {
        L2_CACHE_SRAM,
        "non-core read hits in L2",
        "",
        PERFMON_REF_EVENT, 24
        },

        {
        L2_CACHE_SRAM,
        "non-core read misses in L2",
        "",
        1, 54
        },

        {
        L2_CACHE_SRAM,
        "L2 allocations from any source",
        "",
        PERFMON_REF_EVENT, 25
        },

        {
        L2_CACHE_SRAM,
        "L2 retries due to full write queue",
        "",
        2, 60
        },

        {
        L2_CACHE_SRAM,
        "L2 retries due to address collisions",
        "",
        3, 58
        },

        {
        L2_CACHE_SRAM,
        "L2 failed lock attempts due to full set",
        "",
        4, 58
        },

        {
        L2_CACHE_SRAM,
        "L2 victimization of valid cache lines",
        "",
        5, 52
        },

        {
        L2_CACHE_SRAM,
        "L2 invalidations of cache lines",
        "",
        6, 57
        },

        {
        L2_CACHE_SRAM,
        "L2 clearing of locks",
        "",
        7, 53
        },

        {
        DEBUG,
        "number of cycles trig_in pin is asserted by an external event",
        "",
        3, 61
        },

        {
        DEBUG,
        "watchpoint monitor hits",
        "",
        2, 61
        },

        {
        DEBUG,
        "trace buffer hits",
        "",
        1, 58
        },

        {
        DUART,
        "UART0 baud rate changes",
        "",
        1, 63
        },

        {
        DUART,
        "UART1 baud rate changes",
        "",
        5, 63
        },

        {
        CHAINING,
        "counter 0 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 1
        },

        {
        CHAINING,
        "counter 1 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 2
        },

        {
        CHAINING,
        "counter 2 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 3
        },

        {
        CHAINING,
        "counter 3 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 4
        },

        {
        CHAINING,
        "counter 4 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 5
        },

        {
        CHAINING,
        "counter 5 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 6
        },

        {
        CHAINING,
        "counter 6 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 7
        },

        {
        CHAINING,
        "counter 7 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 8
        },

        {
        CHAINING,
        "counter 8 carry-outs, msb 1 to 0 transitions",
        "",
        PERFMON_REF_EVENT, 9
        },

        {
        INTERRUPT_CONTROLLER,
        "interprocessor 0 interrupts",
        "",
        PIC_INT_SELECT_0, 8
        },

        {
        INTERRUPT_CONTROLLER,
        "interprocessor 1 interrupts",
        "",
        PIC_INT_SELECT_1, 9
        },

        {
        INTERRUPT_CONTROLLER,
        "interprocessor 2 interrupts",
        "",
        PIC_INT_SELECT_2, 10
        },

        {
        INTERRUPT_CONTROLLER,
        "interprocessor 3 interrupts",
        "",
        PIC_INT_SELECT_3, 11
        },

        {
        INTERRUPT_CONTROLLER,
        "global timer 0 interrupts",
        "",
        PIC_INT_SELECT_0, 12
        },

        {
        INTERRUPT_CONTROLLER,
        "global timer 1 interrupts",
        "",
        PIC_INT_SELECT_1, 13
        },

        {
        INTERRUPT_CONTROLLER,
        "global timer 2 interrupts",
        "",
        PIC_INT_SELECT_2, 14
        },

        {
        INTERRUPT_CONTROLLER,
        "global timer 3 interrupts",
        "",
        PIC_INT_SELECT_3, 15
        },

        {
        INTERRUPT_CONTROLLER,
        "message 0 interrupts",
        "",
        PIC_INT_SELECT_0, 16
        },

        {
        INTERRUPT_CONTROLLER,
        "message 1 interrupts",
        "",
        PIC_INT_SELECT_1, 17
        },

        {
        INTERRUPT_CONTROLLER,
        "message 2 interrupts",
        "",
        PIC_INT_SELECT_2, 18
        },

        {
        INTERRUPT_CONTROLLER,
        "message 3 interrupts",
        "",
        PIC_INT_SELECT_3, 19
        },

        {
        INTERRUPT_CONTROLLER,
        "external VME 0 interrupts",
        "",
        PIC_INT_SELECT_0, 20
        },

        {
        INTERRUPT_CONTROLLER,
        "external VME 1 and timer interrupts",
        "",
        PIC_INT_SELECT_1, 21
        },

        {
        INTERRUPT_CONTROLLER,
        "external VME 2 and sATA interrupts",
        "",
        PIC_INT_SELECT_2, 22
        },

        {
        INTERRUPT_CONTROLLER,
        "external VME 3 and UARTs interrupts",
        "",
        PIC_INT_SELECT_3, 23
        },

        {
        INTERRUPT_CONTROLLER,
        "external PMCspan/PMC/USB interrupts",
        "",
        PIC_INT_SELECT_0, 24
        },

        {
        INTERRUPT_CONTROLLER,
        "external PMCspan/PMC interrupts",
        "",
        PIC_INT_SELECT_1, 25
        },

        {
        INTERRUPT_CONTROLLER,
        "external PMCspan/PMC interrupts",
        "",
        PIC_INT_SELECT_2, 26
        },

        {
        INTERRUPT_CONTROLLER,
        "external PMCspan/PMC interrupts",
        "",
        PIC_INT_SELECT_3, 27
        },

        {
        INTERRUPT_CONTROLLER,
        "external abort interrupts",
        "",
        PIC_INT_SELECT_0, 28
        },

        {
        INTERRUPT_CONTROLLER,
        "external temperature sensor interrupts",
        "",
        PIC_INT_SELECT_1, 29
        },

        {
        INTERRUPT_CONTROLLER,
        "external Ethernet PHY interrupts",
        "",
        PIC_INT_SELECT_2, 30
        },

        {
        INTERRUPT_CONTROLLER,
        "external DS1375 alarm interrupts",
        "",
        PIC_INT_SELECT_3, 31
        },

        {
        INTERRUPT_CONTROLLER,
        "internal L2 cache interrupts",
        "",
        PIC_INT_SELECT_0, 0
        },

        {
        INTERRUPT_CONTROLLER,
        "internal ECM interrupts",
        "",
        PIC_INT_SELECT_1, 1
        },

        {
        INTERRUPT_CONTROLLER,
        "internal DDR interrupts",
        "",
        PIC_INT_SELECT_2, 2
        },

        {
        INTERRUPT_CONTROLLER,
        "internal LBC interrupts",
        "",
        PIC_INT_SELECT_3, 3
        },

        {
        INTERRUPT_CONTROLLER,
        "internal DMA channel 0 interrupts",
        "",
        PIC_INT_SELECT_0, 5
        },

        {
        INTERRUPT_CONTROLLER,
        "internal DMA channel 1 interrupts",
        "",
        PIC_INT_SELECT_1, 6
        },

        {
        INTERRUPT_CONTROLLER,
        "internal DMA channel 2 interrupts",
        "",
        PIC_INT_SELECT_2, 7
        },

        {
        INTERRUPT_CONTROLLER,
        "internal DMA channel 3 interrupts",
        "",
        PIC_INT_SELECT_3, 8
        },

        {
        INTERRUPT_CONTROLLER,
        "internal RapidIO read/write error interrupts",
        "",
        PIC_INT_SELECT_0, 9
        },

        {
        INTERRUPT_CONTROLLER,
        "internal RapidIO doorbell interrupts",
        "",
        PIC_INT_SELECT_1, 10
        },

        {
        INTERRUPT_CONTROLLER,
        "internal RapidIO message out interrupts",
        "",
        PIC_INT_SELECT_2, 11
        },

        {
        INTERRUPT_CONTROLLER,
        "internal RapidIO message in interrupts",
        "",
        PIC_INT_SELECT_3, 12
        },

        {
        INTERRUPT_CONTROLLER,
        "internal TSEC 1 transmit interrupts",
        "",
        PIC_INT_SELECT_0, 13
        },

        {
        INTERRUPT_CONTROLLER,
        "internal TSEC 1 receive interrupts",
        "",
        PIC_INT_SELECT_1, 14
        },

        {
        INTERRUPT_CONTROLLER,
        "internal TSEC 1 error interrupts",
        "",
        PIC_INT_SELECT_2, 18
        },

        {
        INTERRUPT_CONTROLLER,
        "internal TSEC 2 transmit interrupts",
        "",
        PIC_INT_SELECT_3, 19
        },

        {
        INTERRUPT_CONTROLLER,
        "internal TSEC 2 receive interrupts",
        "",
        PIC_INT_SELECT_0, 20
        },

        {
        INTERRUPT_CONTROLLER,
        "internal TSEC 2 error interrupts",
        "",
        PIC_INT_SELECT_1, 24
        },

        {
        INTERRUPT_CONTROLLER,
        "internal FEC interrupts",
        "",
        PIC_INT_SELECT_2, 25
        },

        {
        INTERRUPT_CONTROLLER,
        "internal DUART interrupts",
        "",
        PIC_INT_SELECT_3, 26
        },

        {
        INTERRUPT_CONTROLLER,
        "internal I2C interrupts",
        "",
        PIC_INT_SELECT_0, 27
        },

        {
        INTERRUPT_CONTROLLER,
        "internal performance monitor interrupts",
        "",
        PIC_INT_SELECT_1, 28
        }
   };

/* external references */


/***************************************************************************    
*
* perfMonEventCounterGet - get an event's fixed associated counter if any
*
* This routine uses the given event to lookup its associated counter
* index. Certain events can only be counted by a particular counter. If
* the event specified can be counted by any counter, PERFMON_REF_EVENT is
* passed back.
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventCounterGet 
    (
    PERFMON_EVENT  theEvent,       /* The event to lookup */
    UINT32        *counterIndex    /* Associated counter 0-9, or Ref */
    )
    {
    STATUS status = ERROR;

    if (theEvent < PERFMON_NUM_EVENTS)
        {
        *counterIndex = events[theEvent].counterNum;
        status = OK;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventIdGet - get an event's encoded ID 
*
* This routine gets the event specific ID value that is used to
* program counter control to monitor the associated event.
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventIdGet 
    (
    PERFMON_EVENT  theEvent,     /* The event to lookup */
    UINT32        *id            /* Unique ID for given event */
    )
    {
    STATUS status = ERROR;

    if (theEvent < PERFMON_NUM_EVENTS)
        {
        *id = events[theEvent].eventCode;

        if (events[theEvent].counterNum != PERFMON_REF_EVENT)
             {
             *id += PERFMON_COUNTER_SPECIFIC_EVENT_OFFSET;
             }

        status = OK;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventDescriptionGet - get the event's textual description
*
* This routine gets two extended descriptive strings associated with 
* the specified event. The character strings can be displayed in an event 
* report. If no description is available a null string is passed back.
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventDescriptionGet 
    (
    PERFMON_EVENT   theEvent,      /* The event to lookup */
    const char    **description1,  /* Full text description, part 1 */
    const char    **description2   /* Text description, part 2 */
    )
    {
    STATUS status = ERROR;

    if (theEvent < PERFMON_NUM_EVENTS)
        {
        *description1 = events[theEvent].description1;
        *description2 = events[theEvent].description2;
        status = OK;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventClassGet - get the event's class name
*
* This routine gets a character string containing the class name of
* the specified event. The name string can be displayed in an event report.
* An event class generally corresponds to a particular grouping of events
* related to the same hardware subsystem (e.g. memory, Ethernet, RapidIO,
* etc.)
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventClassGet 
    (
    PERFMON_EVENT   theEvent,     /* The event to lookup */
    const char    **name          /* Class name association for the event */
    )
    {
    STATUS status = ERROR;

    if (theEvent < PERFMON_NUM_EVENTS)
        {
        *name = className[events[theEvent].class];
        status = OK;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventNameGet - get the event's name
*
* This routine gets a character string similar to the 
* enumerated name of the event. The name string can be displayed in an 
* event report.
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventNameGet 
    (
    PERFMON_EVENT   theEvent,   /* The event to lookup */
    const char    **name        /* Full textual name of the event */
    )
    {
    STATUS status = ERROR;

    if (theEvent < PERFMON_NUM_EVENTS)
        {
        *name = eventName[theEvent];
        status = OK;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventIntIdGet - get the interrupt's event ID
*
* This routine gets the event ID for given PIC interrupt event.
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventIntIdGet 
    (
    PERFMON_EVENT   intEvent,   /* The event to lookup */
    UINT32         *id          /* The interrupt ID */
    )
    {
    STATUS status = ERROR;
    UINT32 index = 0;

    if (intEvent < PERFMON_NUM_EVENTS)
        {
        index = events[intEvent].counterNum;
        *id = events[index].eventCode;

        if (events[index].counterNum != PERFMON_REF_EVENT)
             {
             *id += PERFMON_COUNTER_SPECIFIC_EVENT_OFFSET;
             }

        status = OK;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventIntCounterGet - get the interrupt's counter number
*
* This routine gets the counter for given PIC interrupt event.
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventIntCounterGet 
    (
    PERFMON_EVENT  intEvent,      /* The event to lookup */
    UINT32        *counterIndex   /* The counter number */
    )
    {
    STATUS status = ERROR;
    UINT32 index = 0;

    if (intEvent < PERFMON_NUM_EVENTS)
        {
        index = events[intEvent].counterNum;
        *counterIndex = events[index].counterNum;

        status = OK;
        }

    return (status);
    }

/***************************************************************************    
*
* perfMonEventIntMaskSet - set event enable mask for the interrupt
*
* This routine sets the event enable mask for the given interrupt event 
* so that it can be counted when it is attached to an event monitor counter.
*
* RETURNS: OK upon success.
* ERROR if theEvent is out of range.
*/

STATUS perfMonEventIntMaskSet 
    (
    PERFMON_EVENT  intEvent   /* The event to lookup */
    )
    {
    STATUS status = ERROR;
    UINT32 bit = 0;
    UINT32 mask = 0;
    UINT32 maskReg = 0;

    if (intEvent < PERFMON_NUM_EVENTS)
        {
        bit = events[intEvent].eventCode;
        mask = 0xFFFFFFFF & ~(0x1 << PPC_BIT_INDEX_SWAP (bit, 1));

        maskReg = ((events[intEvent].counterNum - PIC_INT_SELECT_0) *
                   PERFMON_INT_REG_INTERVAL) + EPIC_PERFMON_0_MSK_REG0;
     
        if (PERFMON_IS_INTERNAL_INT_EVENT(intEvent))
            {
            maskReg += PERFMON_INT_REG_INTERVAL / 2;
            }
        else
            {
            mask &= 0x00FFFFFF;
            }

        CCSR_WRITE32 (CCSBAR, maskReg, mask);
  
        status = OK;
        }

    return (status);
    }
