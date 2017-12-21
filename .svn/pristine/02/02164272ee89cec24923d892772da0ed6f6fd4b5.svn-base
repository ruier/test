/* perfMonEvent.h - Performance Monitor Events */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,23mayo5,efb Initial writing. 
*/


#ifndef __INCperfMonEventh
#define __INCperfMonEventh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* includes */

#include "vxWorks.h"

/* defines */

#define PERFMON_REF_EVENT                      0xFF
#define PERFMON_COUNTER_SPECIFIC_EVENT_OFFSET  64

#define PERFMON_IS_INT_EVENT(event) \
    (((event) > PERF_MON_C8_CARRY_OUTS) && ((event) < PERFMON_NUM_EVENTS))

#define PERFMON_IS_INTERNAL_INT_EVENT(event) \
    (((event) > EPIC_EXT_IRQ11_DS1375_ALARM) && ((event) < PERFMON_NUM_EVENTS))

/* 
 * Convert left-to-right PPC bit indexing to right-to-left indexing.
 * "ppcBit" is the left-to-right PPC bit number. "width" is the number of
 * bits in a bit field. The ppcBit will be the leftmost bit in the bit
 * field. The swap macro will return a bit index number counting from right-
 * to-left to the rightmost bit of the original field.
 */

#define PPC_BIT_INDEX_SWAP(ppcBit, width)  (32 - (ppcBit) - (width))

/* EPIC Performance Monitor Mask Register Defines */

#define EPIC_PERFMON_0_MSK_REG0  0x41350   /* PerfMon 0 Mask 0 */
#define EPIC_PERFMON_0_MSK_REG1  0x41360   /* PerfMon 0 Mask 1 */
#define EPIC_PERFMON_1_MSK_REG0  0x41370   /* PerfMon 1 Mask 0 */
#define EPIC_PERFMON_1_MSK_REG1  0x41380   /* PerfMon 1 Mask 1 */
#define EPIC_PERFMON_2_MSK_REG0  0x41390   /* PerfMon 2 Mask 0 */
#define EPIC_PERFMON_2_MSK_REG1  0x413a0   /* PerfMon 2 Mask 1 */
#define EPIC_PERFMON_3_MSK_REG0  0x413b0   /* PerfMon 3 Mask 0 */
#define EPIC_PERFMON_3_MSK_REG1  0x413c0   /* PerfMon 3 Mask 1 */

/* typedefs */

/* Events available for monitoring, counter (C0-C8), description */

typedef enum events 
    {
    SYSTEM_CYCLES,                  /* C0,  CCB clock cycles */

    DDR_READ_CYCLES,                /* Any, Each data beat returned */
    DDR_READ_WRITE_CYCLES,          /* Any, Each data beat sent or returned */ 
    DDR_PIPE_READ_MISSES,           /* C1,  Row open table read miss */
    DDR_PIPE_READ_WRITE_MISSES,     /* C2,  Row open table read/write miss */
    DDR_NONPIPE_READ_MISSES,        /* C3,  Row open table read miss */
    DDR_NONPIPE_READ_WRITE_MISSES,  /* C4,  Row open table read/write miss */
    DDR_PIPE_READ_HITS,             /* C5,  Row open table read hits */
    DDR_PIPE_READ_WRITE_HITS,       /* C6,  Row open table read/write hits */
    DDR_NONPIPE_READ_HITS,          /* C7,  Row open table read hits */
    DDR_NONPIPE_READ_WRITE_HITS,    /* C8,  Row open table read/write hits */
    DDR_NONRFRSH_FRCD_PAGE_CLOSES,  /* C1,  Non-refresh forced page closings */
    DDR_ROW_OPEN_TABLE_MISSES,      /* C2,  Miss in the row open table */
    DDR_ROW_OPEN_TABLE_HITS,        /* C3,  Hit in the row open table*/
    DDR_FORCED_PAGE_CLOSES,         /* C4,  Forced page closings */
    DDR_ECC_CAUSED_READ_MOD_WRITES, /* C5,  RMWs due to ECC */
    DDR_CLSN_FRCD_PAGE_CLOSES,      /* Any, Bank collision forced page close */
    DDR_CORE_READS_WRITES,          /* Any, Reads or writes from core */
    DDR_TSEC1_READS_WRITES,         /* C3,  Reads or writes from TSEC1 */
    DDR_TSEC2_READS_WRITES,         /* C4,  Reads or writes from TSEC2 */
    DDR_RAPIO_READS_WRITES,         /* C3,  Reads or writes from RapidIO */
    DDR_PCI_READS_WRITES,           /* C4,  Reads or writes from PCI */
    DDR_DMA_READS_WRITES,           /* C5,  Reads or writes from DMA */
    DDR_CORE_ROW_OPEN_TABLE_HITS,   /* Any, Row open table read/write hits */
    DDR_TSEC1_ROW_OPEN_TABLE_HITS,  /* C6,  Row open table read/write hits */
    DDR_TSEC2_ROW_OPEN_TABLE_HITS,  /* C7,  Row open table read/write hits */
    DDR_RAPIO_ROW_OPEN_TABLE_HITS,  /* C6,  Row open table read/write hits */
    DDR_PCI_ROW_OPEN_TABLE_HITS,    /* C7,  Row open table read/write hits */
    DDR_DMA_ROW_OPEN_TABLE_HITS,    /* C8,  Row open table read/write hits */

    MEM_TARGET_Q_ADRS_COLLISONS,    /* C5,  Read/write address collisions */

    RAPIO_MISALIGNED_TRANS,         /* C4,  Misaligned transactions */
    RAPIO_TRANSMIT_RETRIES,         /* C2,  Transmit packet retries */
    RAPIO_FLUSHED_PACKETS,          /* C3,  Flushed packets from error/retry */
    RAPIO_REMOTE_RETRIES_REJECTS,   /* C5,  Remote device retries/rejects */
    RAPIO_MISALIGNED_ENG_PR2_BUSY,  /* C5,  Misaligned engine busy */
    RAPIO_MISALIGNED_ENG_PR1_BUSY,  /* C6,  Misaligned engine busy */
    RAPIO_MISALIGNED_ENG_PR0_BUSY,  /* C7,  Misaligned engine busy */
    RAPIO_ACK_HISTORY_Q_FULL,       /* C4,  Unacked packets outstanding */
    RAPIO_OUTBND_STOPPED_TRAINING,  /* C6,  Training event stopped outbound */ 
    RAPIO_OUTBND_STOPPED_RETRY,     /* C7,  ACK retry event stopped outbound */
    RAPIO_OUTBND_STOPPED_ERROR,     /* C8,  ACK not accepted stopped outbound */
    RAPIO_INBND_PROTO_LVL_RETRY,    /* C4,  Protocol level inbound retry */
    RAPIO_INBND_DOORBELL_RETRY,     /* C6,  Inbound doorbell packet retry */
    RAPIO_OUTBND_ACK_TIMEOUTS,      /* C8,  Outbound ACK timeouts */

    DMA_CHAN_0_READ_REQUESTS,       /* C1,  Channel 0 read request active */
    DMA_CHAN_1_READ_REQUESTS,       /* C2,  Channel 1 read request active */
    DMA_CHAN_2_READ_REQUESTS,       /* C3,  Channel 2 read request active */
    DMA_CHAN_3_READ_REQUESTS,       /* C4,  Channel 3 read request active */
    DMA_CHAN_0_WRITE_REQUESTS,      /* C1,  Channel 0 write request active */
    DMA_CHAN_1_WRITE_REQUESTS,      /* C2,  Channel 1 write request active */
    DMA_CHAN_2_WRITE_REQUESTS,      /* C3,  Channel 2 write request active */
    DMA_CHAN_3_WRITE_REQUESTS,      /* C4,  Channel 3 write request active */
    DMA_CHAN_0_DESC_REQUESTS,       /* C5,  Channel 0 descriptor request */
    DMA_CHAN_1_DESC_REQUESTS,       /* C6,  Channel 1 descriptor request */
    DMA_CHAN_2_DESC_REQUESTS,       /* C7,  Channel 2 descriptor request */
    DMA_CHAN_3_DESC_REQUESTS,       /* C8,  Channel 3 descriptor request */
    DMA_CHAN_0_READ_DW_REQUESTS,    /* C1,  Channel 0 double word read */
    DMA_CHAN_0_READ_DW_REQUESTS_ALT,/* C5,  Channel 0 double word read */
    DMA_CHAN_1_READ_DW_REQUESTS,    /* C2,  Channel 1 double word read */
    DMA_CHAN_1_READ_DW_REQUESTS_ALT,/* C6,  Channel 1 double word read */
    DMA_CHAN_2_READ_DW_REQUESTS,    /* C3,  Channel 2 double word read */
    DMA_CHAN_2_READ_DW_REQUESTS_ALT,/* C7,  Channel 2 double word read */
    DMA_CHAN_3_READ_DW_REQUESTS,    /* C4,  Channel 3 double word read */
    DMA_CHAN_3_READ_DW_REQUESTS_ALT,/* C8,  Channel 3 double word read */
    DMA_CHAN_0_WRITE_DW_REQUESTS,   /* C1,  Channel 0 double word write */
    DMA_CHAN_1_WRITE_DW_REQUESTS,   /* C2,  Channel 1 double word write */
    DMA_CHAN_2_WRITE_DW_REQUESTS,   /* C3,  Channel 2 double word write */
    DMA_CHAN_3_WRITE_DW_REQUESTS,   /* C4,  Channel 3 double word write */

    ECM_REQUEST_WAIT_CORE,          /* C8,  Request wait core */
    ECM_REQUEST_WAIT_TSEC1,         /* C5,  Request wait TSEC1 */
    ECM_REQUEST_WAIT_TSEC2,         /* C6,  Request wait TSEC1 */
    ECM_REQUEST_WAIT_DMA_PCI_RAPIO, /* C4,  Request wait DMA/PCI/RapidIO */
    ECM_DISPATCH,                   /* Any, ECM dispatch events */
    ECM_DISPATCH_FROM_CORE,         /* C1,  ECM dispatch from core */
    ECM_DISPATCH_FROM_TSEC1,        /* C3,  ECM dispatch from TSEC1 */
    ECM_DISPATCH_FROM_TSEC2,        /* C4,  ECM dispatch from TSEC1 */
    ECM_DISPATCH_FROM_RAPIO,        /* C5,  ECM dispatch from RapidIO */
    ECM_DISPATCH_FROM_PCI,          /* C6,  ECM dispatch from PCI */
    ECM_DISPATCH_FROM_DMA,          /* C7,  ECM dispatch from DMA */
    ECM_DISPATCH_FROM_OTHER,        /* C8,  ECM dispatch from other */
    ECM_DISPATCH_TO_DDR,            /* C4,  ECM dispatch to DDR */
    ECM_DISPATCH_TO_L2SRAM,         /* C5,  ECM dispatch to L2/SRAM */
    ECM_DISPATCH_TO_LBC,            /* C6,  ECM dispatch to LBC */
    ECM_DISPATCH_TO_RAPIO,          /* C7,  ECM dispatch to RapidIO */
    ECM_DISPATCH_TO_PCI,            /* C8,  ECM dispatch to PCI */
    ECM_DISPATCH_SNOOPABLE,         /* C3,  ECM dispatch snoopable */
    ECM_DISPATCH_WRITE,             /* C1,  ECM dispatch write */
    ECM_DISPATCH_WRITE_ALLOC,       /* C2,  ECM dispatch write allocate */
    ECM_DISPATCH_WRITE_ALLOC_LOCK,  /* C3,  ECM dispatch write allocate lock */
    ECM_DISPATCH_READ,              /* C4,  ECM dispatch read */
    ECM_DISPATCH_READ_UNLOCK,       /* C5,  ECM dispatch read unlock */
    ECM_DISPATCH_READ_CLR_ATOMIC,   /* C6,  ECM dispatch read clear atomic */
    ECM_DISPATCH_READ_SET_ATOMIC,   /* C7,  ECM dispatch read set atomic */
    ECM_DISPATCH_READ_DEC_ATOMIC,   /* C8,  ECM dispatch read decrmnt atomic */
    ECM_DISPATCH_READ_INC_ATOMIC,   /* C7,  ECM dispatch read incrmnt atomic */
    ECM_DATA_BUS_GRNT_DDR,          /* C1,  ECM data bus grant DDR */
    ECM_DATA_BUS_GRNT_LBC,          /* C2,  ECM data bus grant LBC */
    ECM_DATA_BUS_GRNT_PIC,          /* C1,  ECM data bus grant PIC */
    ECM_DATA_BUS_GRNT_TSEC1,        /* C3,  ECM data bus grant TSEC1 */
    ECM_DATA_BUS_GRNT_TSEC2,        /* C4,  ECM data bus grant TSEC2 */
    ECM_DATA_BUS_WAIT_DDR,          /* C5,  ECM data bus wait DDR */
    ECM_DATA_BUS_WAIT_LBC,          /* C6,  ECM data bus wait LBC */
    ECM_DATA_BUS_WAIT_PIC,          /* C5,  ECM data bus wait PIC */
    ECM_DATA_BUS_WAIT_TSEC1,        /* C7,  ECM data bus wait TSEC1 */
    ECM_DATA_BUS_WAIT_TSEC2,        /* C8,  ECM data bus wait TSEC2 */
    ECM_DATA_BUS_BEAT_GLOBAL,       /* Any, ECM global data bus beat */
    ECM_BUS_BEAT_E500_DIRECT,       /* Any, ECM e500 direct read bus beat */
    ECM_BUS_BEAT_E500_DIRECT_FWD,   /* C2,  ECM e500 direct read bus beat */
    ECM_CANCEL,                     /* Any, ECM cancel */
     
    PIC_TOTAL_INTERRUPTS,           /* Any, Total number of interrupts */
    PIC_INT_WAIT_CYCLES,            /* C8,  Interrupt wait cycles */
    PIC_INT_SERVICE_CYCLES,         /* C2,  Current interrupt service cycles */
    PIC_INT_SELECT_0,               /* C1,  Use interrupt name instead */
    PIC_INT_SELECT_1,               /* C3,  Use interrupt name instead */
    PIC_INT_SELECT_2,               /* C5,  Use interrupt name instead */
    PIC_INT_SELECT_3,               /* C6,  Use interrupt name instead */

    PCI_PCIX_CLOCK_CYCLES,          /* Any, PCI and PCI-X clock cycles */
    PCI_PCIX_INBND_MEM_READS,       /* C1,  PCI/X inbound memory reads */ 
    PCI_PCIX_INBND_MEM_WRITES,      /* C2,  PCI/X inbound memory writes */ 
    PCI_PCIX_INBND_CFG_READS,       /* C3,  PCI/X inbound config reads */ 
    PCI_PCIX_INBND_CFG_WRITES,      /* C4,  PCI/X inbound config writes */ 
    PCI_PCIX_OUTBND_MEM_READS,      /* C5,  PCI/X outbound memory reads */ 
    PCI_PCIX_OUTBND_MEM_WRITES,     /* C6,  PCI/X outbound memory writes */ 
    PCI_PCIX_OUTBND_IO_READS,       /* C3,  PCI/X outbound I/O reads */ 
    PCI_PCIX_OUTBND_IO_WRITES,      /* C4,  PCI/X outbound I/O writes */ 
    PCI_PCIX_OUTBND_CFG_READS,      /* C7,  PCI/X outbound config reads */ 
    PCI_PCIX_OUTBND_CFG_WRITES,     /* C8,  PCI/X outbound config writes */ 
    PCI_PCIX_INBND_READ_BEATS,      /* C5,  PCI/X inbound read data beats */ 
    PCI_PCIX_INBND_WRITE_BEATS,     /* C6,  PCI/X inbound write data beats */ 
    PCI_PCIX_OUTBND_READ_BEATS,     /* C7,  PCI/X outbound read data beats */ 
    PCI_PCIX_OUTBND_WRITE_BEATS,    /* C8,  PCI/X outbound write data beats */ 
    PCI_PCIX_INBND_READ32_BEATS,    /* C1,  PCI/X inbnd read 32b data beats */
    PCI_PCIX_INBND_WRITE32_BEATS,   /* C2,  PCI/X inbnd write 32b data beats */ 
    PCI_PCIX_OUTBND_READ32_BEATS,   /* C3,  PCI/X outbnd read 32b data beats */ 
    PCI_PCIX_OUTBND_WRITE32_BEATS,  /* C4,  PCI/X outbnd write 32b data beats */
    PCI_PCIX_INBND_READ64_BEATS,    /* C5,  PCI/X inbnd read 64b data beats */
    PCI_PCIX_INBND_WRITE64_BEATS,   /* C6,  PCI/X inbnd write 64b data beats */
    PCI_PCIX_OUTBND_READ64_BEATS,   /* C7,  PCI/X outbnd read 64b data beats */
    PCI_PCIX_OUTBND_WRITE64_BEATS,  /* C8,  PCI/X outbnd write 64b data beats */
    PCI_PCIX_TOTAL_TRANS,           /* C7,  PCI/X total transactions */
    PCI_PCIX_64BIT_TRANS,           /* C8,  PCI/X 64-bit transactions */
    PCI_PCIX_INBND_PURG_READS,      /* C2,  PCI/X inbound purgeable reads */
    PCI_PCIX_INBND_PURG_READS_DSCRD,/* C8,  PCI/X inbound purg reads discard */
    PCI_PCIX_IDLE_CYCLES,           /* C1,  PCI/X idles cycles */
    PCI_PCIX_DUAL_ADDRS_CYCLES,     /* C2,  PCI/X dual address cycles */
    PCI_PCIX_INTERNAL_CYCLES,       /* C3,  PCI/X internal cycles */
    PCI_PCIX32_INBND_MEM_READS,     /* C1,  PCI + PCI-X-32b inbnd mem reads */
    PCI_PCIX_ALIAS_INBND_MEM_READS, /* C2,  PCI/X line/alias inbnd mem reads */
    PCI_PCIX_BLOCK_INBND_MEM_READS, /* C3,  PCI/X multi/block inbnd mem reads */
    PCI_PCIX_DWORD_OUTBND_MEM_READS,/* C4,  PCI/PCI-X-DWORD outbnd mem reads */
    PCI_PCIX_BURST_OUTBND_MEM_READS,/* C5,  PCI/PCI-X-burst outbnd mem reads */
    PCI_PCIX_WAIT_INITIAL_WAIT,     /* C1,  PCI/X wait/initial wait */

    PCI_CYCLES_PCI_IRDY_ASSERT,     /* C6,  Cycles ~PCI_IRDY asserted */
    PCI_CYCLES_PCI_TRDY_ASSERT,     /* C7,  Cycles ~PCI_TRDY asserted */
    PCI_CYCLES_PCI_FRAME_ASSERT,    /* C8,  Cycles ~PCI_FRAME asserted */
    PCIX_1_SPLIT_TRANS,             /* C2,  Cycles 1+ outbnd reads waiting */
    PCIX_2_SPLIT_TRANS,             /* C3,  Cycles 2+ outbnd reads waiting */
    PCIX_3_SPLIT_TRANS,             /* C4,  Cycles 3+ outbnd reads waiting */
    PCIX_4_SPLIT_TRANS,             /* C5,  Cycles 4+ outbnd reads waiting */
    PCIX_SPLIT_RESPONSES,           /* C6,  Split responses received */
    PCIX_ADB_DISCONNECTS,           /* C7,  ADB disconnect */
    PCI_PCIX_SNOOPABLE,             /* C1,  Snoopable */
    PCI_PCIX_WRITE_STASH,           /* C2,  Write stash */
    PCI_PCIX_WRITE_STASH_LOCK,      /* C3,  Write stash with lock */
    PCI_PCIX_READ_UNLOCK,           /* C4,  Read unlock */
    PCI_PCIX_BYTE_ENB_TRANS,        /* C1,  Byte enable transactions */
    PCI_PCIX_NONBYTE_ENB_TRANS,     /* C2,  Non-byte enable transactions */
    
    TSEC1_ACCEPTED_FRAMES,          /* Any, Accepted frames */
    TSEC1_INDIVL_HASH_TBL_FRAMES,   /* C7,  Individual hash table frames */
    TSEC1_GROUP_HASH_TBL_FRAMES,    /* C8,  Group hash table accepted frames */
    TSEC1_REJECTED_FRAMES,          /* Any, Reject frames */
    TSEC1_DROPPED_FRAMES,           /* Any, Dropped frames */
    TSEC1_DATA_OVFL_DROPPED_FRAMES, /* C1,  Dropped frames - data overflow */
    TSEC1_STAT_OVFL_DROPPED_FRAMES, /* C2,  Dropped frames - status overflow */
    TSEC1_FIFO_DATA_VALID,          /* C1,  FIFO data valid */
    TSEC1_TX_FRAMES_NO_THRESHLD,    /* C7,  Transmit frames w/o threshold */
    TSEC1_RX_FIFO_ABOVE_1QTR,       /* Any, Receive FIFO above 1/4 */
    TSEC1_RX_FIFO_ABOVE_1Half,      /* Any, Receive FIFO above 1/2 */
    TSEC1_RX_FIFO_ABOVE_3QTR,       /* Any, Receive FIFO above 3/4 */
    TSEC1_DMA_READS,                /* C1,  Descriptor and data reads */
    TSEC1_BD_READS,                 /* C2,  TxBD and RXBD reads */
    TSEC1_RXBD_READS,               /* C3,  RXBD reads */
    TSEC1_DMA_WRITES,               /* C4,  Descriptor and data writes */
    TSEC1_BD_WRITES,                /* C5,  TxBD and RXBD writes */
    TSEC1_RXBD_WRITES,              /* C6,  RXBD writes */
    TSEC1_RXBD_READ_LATENCY,        /* Any, Read latency exceeded threshold */
    TSEC1_TXBD_READ_LATENCY,        /* Any, Read latency exceeded threshold */
    TSEC1_RXBD_WRITE_LATENCY,       /* Any, Write latency exceeded threshold */
    TSEC1_TXBD_WRITE_LATENCY,       /* Any, Write latency exceeded threshold */
    TSEC1_TX_DATA_READ_LATENCY,     /* Any, Data read latency exceeded thresh */
    TSEC1_DATA_BEATS,               /* C7,  Data beats */
    TSEC1_READ_DATA_BEATS,          /* C8,  Read data beats */
    TSEC1_RX_FRAME_INTERRUPTS,      /* C1,  RX frame interrupts */
    TSEC1_TX_FRAME_INTERRUPTS,      /* C3,  TX frame interrupts */
    TSEC1_RX_FRAME_PROCESSING,      /* Any, RX frame proc exceeded threshold */

    TSEC2_ACCEPTED_FRAMES,          /* Any, Accepted frames */
    TSEC2_INDIVL_HASH_TBL_FRAMES,   /* C7,  Individual hash table frames */
    TSEC2_GROUP_HASH_TBL_FRAMES,    /* C8,  Group hash table accepted frames */
    TSEC2_REJECTED_FRAMES,          /* Any, Reject frames */
    TSEC2_DROPPED_FRAMES,           /* Any, Dropped frames */
    TSEC2_DATA_OVFL_DROPPED_FRAMES, /* C1,  Dropped frames - data overflow */
    TSEC2_STAT_OVFL_DROPPED_FRAMES, /* C2,  Dropped frames - status overflow */
    TSEC2_FIFO_DATA_VALID,          /* C1,  FIFO data valid */
    TSEC2_TX_FRAMES_NO_THRESHLD,    /* C7,  Transmit frames w/o threshold */
    TSEC2_RX_FIFO_ABOVE_QUARTER,    /* Any, Receive FIFO above 1/4 */
    TSEC2_RX_FIFO_ABOVE_Half,       /* Any, Receive FIFO above 1/2 */
    TSEC2_RX_FIFO_ABOVE_3QUARTER,   /* Any, Receive FIFO above 3/4 */
    TSEC2_DMA_READS,                /* C1,  Descriptor and data reads */
    TSEC2_BD_READS,                 /* C2,  TxBD and RXBD reads */
    TSEC2_RXBD_READS,               /* C3,  RXBD reads */
    TSEC2_DMA_WRITES,               /* C4,  Descriptor and data writes */
    TSEC2_BD_WRITES,                /* C5,  TxBD and RXBD writes */
    TSEC2_RXBD_WRITES,              /* C6,  RXBD writes */
    TSEC2_RXBD_READ_LATENCY,        /* Any, Read latency exceeded threshold */
    TSEC2_TXBD_READ_LATENCY,        /* Any, Read latency exceeded threshold */
    TSEC2_RXBD_WRITE_LATENCY,       /* Any, Write latency exceeded threshold */
    TSEC2_TXBD_WRITE_LATENCY,       /* Any, Write latency exceeded threshold */
    TSEC2_TX_DATA_READ_LATENCY,     /* Any, Data read latency exceeded thresh */
    TSEC2_DATA_BEATS,               /* C7,  Data beats */
    TSEC2_READ_DATA_BEATS,          /* C8,  Read data beats */
    TSEC2_RX_FRAME_INTERRUPTS,      /* C1,  RX frame interrupts */
    TSEC2_TX_FRAME_INTERRUPTS,      /* C3,  TX frame interrupts */
    TSEC2_RX_FRAME_PROCESSING,      /* Any, RX frame proc exceeded threshold */

    LOCAL_BUS_BANK_1_HITS,          /* C1,  Chip select 1 hits */
    LOCAL_BUS_BANK_2_HITS,          /* C2,  Chip select 2 hits */
    LOCAL_BUS_BANK_3_HITS,          /* C3,  Chip select 3 hits */
    LOCAL_BUS_BANK_4_HITS,          /* C4,  Chip select 4 hits */
    LOCAL_BUS_BANK_5_HITS,          /* C5,  Chip select 5 hits */
    LOCAL_BUS_BANK_6_HITS,          /* C6,  Chip select 6 hits */
    LOCAL_BUS_BANK_7_HITS,          /* C7,  Chip select 7 hits */
    LOCAL_BUS_BANK_8_HITS,          /* C8,  Chip select 8 hits */
    LOCAL_BUS_ECM_REQS_GRANTED,     /* C2,  Requests granted to ECM port */
    LOCAL_BUS_ECM_RES_ENB_CYCLES,   /* C4,  Reservation for ECM port enabled */
    LOCAL_BUS_ECM_RES_TIMEOUTS,     /* C6,  Reservation for ECM port timeouts */
    LOCAL_BUS_GPCM_READ_CYCLES,     /* C1,  Cycles in a GPCM read */
    LOCAL_BUS_UPM_READ_CYCLES,      /* C2,  Cycles in an UPM read */
    LOCAL_BUS_SDRAM_READ_CYCLES,    /* C2,  Cycles in a SDRAM read */
    LOCAL_BUS_GPCM_WRITE_CYCLES,    /* C4,  Cycles in a GPCM write */
    LOCAL_BUS_UPM_WRITE_CYCLES,     /* C5,  Cycles in an UPM write */
    LOCAL_BUS_SDRAM_WRITE_CYCLES,   /* C6,  Cycles in a SDRAM write */
    LOCAL_BUS_SDRAM_BANK_MISSES,    /* C7,  SDRAM bank misses */
    LOCAL_BUS_SDRAM_PAGE_MISSES,    /* C8,  SDRAM page misses */

    L2_CORE_INST_ACCESS_HITS,       /* Any, Core instruction access hits */
    L2_CORE_INST_ACCESS_MISSES,     /* C2,  Core instruction access misses */
    L2_CORE_DATA_ACCESS_HITS,       /* Any, Core data access hits */
    L2_CORE_DATA_ACCESS_MISSES,     /* C4,  Core data access misses */
    L2_NON_CORE_BURST_WRITES,       /* C5,  Non-core burst writes to L2 */
    L2_NON_CORE_NON_BURST_WRITES,   /* C6,  Non-core non-burst writes to L2 */
    L2_NON_CORE_WRITE_MISSES,       /* C7,  Non-core write misses */
    L2_NON_CORE_READ_HITS,          /* Any, Non-core read hits */
    L2_NON_CORE_READ_MISSES,        /* C1,  Non-core read misses */
    L2_ALLOCATES,                   /* Any, L2 allocates from any source */
    L2_WRITE_Q_FULL_RETRIES,        /* C2,  Retries due to write queue full */
    L2_ADRS_CLSN_RETRIES,           /* C3,  Retries due to address collision */
    L2_FAILED_LOCK_ATTEMPTS,        /* C4,  Failed lock attempts, set full */
    L2_VALID_LINE_VICTIMS,          /* C5,  Victimizations of valid lines */
    L2_LINE_INVALIDATIONS,          /* C6,  Invalidations of lines */
    L2_LOCK_CLEARINGS,              /* C7,  Clearing of locks */

    DEBUG_EXTERNAL_EVENTS,          /* C3,  Cycles trig_in is asserted */
    DEBUG_WATCHPOINT_MON_HITS,      /* C2,  Watchpoint monitor hits */
    DEBUG_TRACE_BUFFER_HITS,        /* C1,  Trace buffer hits */

    DUART0_BAUD_RATE_CHANGES,       /* C1, DUART 0 baud rate changes */
    DUART1_BAUD_RATE_CHANGES,       /* C5, DUART 1 baud rate changes */

    PERF_MON_C0_CARRY_OUTS,          /* Any, C0 bit 0, 1 to 0 transitions */ 
    PERF_MON_C1_CARRY_OUTS,          /* Any, C1 bit 0, 1 to 0 transitions */ 
    PERF_MON_C2_CARRY_OUTS,          /* Any, C2 bit 0, 1 to 0 transitions */ 
    PERF_MON_C3_CARRY_OUTS,          /* Any, C3 bit 0, 1 to 0 transitions */ 
    PERF_MON_C4_CARRY_OUTS,          /* Any, C4 bit 0, 1 to 0 transitions */ 
    PERF_MON_C5_CARRY_OUTS,          /* Any, C5 bit 0, 1 to 0 transitions */ 
    PERF_MON_C6_CARRY_OUTS,          /* Any, C6 bit 0, 1 to 0 transitions */ 
    PERF_MON_C7_CARRY_OUTS,          /* Any, C7 bit 0, 1 to 0 transitions */ 
    PERF_MON_C8_CARRY_OUTS,          /* Any, C8 bit 0, 1 to 0 transitions */ 
    
    EPIC_IPI_IRQ0,                   /* Interprocessor interrupt 0 count */
    EPIC_IPI_IRQ1,                   /* Interprocessor interrupt 1 count */
    EPIC_IPI_IRQ2,                   /* Interprocessor interrupt 2 count */
    EPIC_IPI_IRQ3,                   /* Interprocessor interrupt 3 count */
    EPIC_TIMER_IRQ0,                 /* Global timer interrupt 0 count */
    EPIC_TIMER_IRQ1,                 /* Global timer interrupt 1 count */
    EPIC_TIMER_IRQ2,                 /* Global timer interrupt 2 count */
    EPIC_TIMER_IRQ3,                 /* Global timer interrupt 3 count */
    EPIC_MESSAGE_IRQ0,               /* Message interrupt 0 count */
    EPIC_MESSAGE_IRQ1,               /* Message interrupt 1 count */
    EPIC_MESSAGE_IRQ2,               /* Message interrupt 2 count */
    EPIC_MESSAGE_IRQ3,               /* Message interrupt 3 count */
    EPIC_EXT_IRQ0_VME0,              /* External interrupt VME 0 */
    EPIC_EXT_IRQ1_VME1_TIMERS,       /* External interrupt VME 1 & timers */
    EPIC_EXT_IRQ2_VME2_SATA,         /* External interrupt VME 2 & sATA */
    EPIC_EXT_IRQ3_VME3_UARTS,        /* External interrupt VME 3 & UARTs */
    EPIC_EXT_IRQ4_PMCSPAN_PMC_USB,   /* External interrupt PMCspan/PMCs/USB */
    EPIC_EXT_IRQ5_PMCSPAN_PMC,       /* External interrupt PMCspan/PMCs */
    EPIC_EXT_IRQ6_PMCSPAN_PMC,       /* External interrupt PMCspan/PMCs */
    EPIC_EXT_IRQ7_PMCSPAN_PMC,       /* External interrupt PMCspan/PMCs */
    EPIC_EXT_IRQ8_ABORT,             /* External interrupt abort */
    EPIC_EXT_IRQ9_TEMP_SENSOR,       /* External interrupt temperature sensor */
    EPIC_EXT_IRQ10_ETHERNET_PHYS,    /* External interrupt Ethernet PHYs */
    EPIC_EXT_IRQ11_DS1375_ALARM,     /* Enternal interrupt DS1375 alarm */
    EPIC_INT_IRQ0_L2_CACHE,          /* Internal interrupt L2 cache */
    EPIC_INT_IRQ1_ECM,               /* Internal interrupt ECM */
    EPIC_INT_IRQ2_DDR,               /* Internal interrupt DDR */
    EPIC_INT_IRQ3_LBC,               /* Internal interrupt LBC */
    EPIC_INT_IRQ5_DMA0,              /* Internal interrupt DMA channel 0 */
    EPIC_INT_IRQ6_DMA1,              /* Internal interrupt DMA channel 1 */
    EPIC_INT_IRQ7_DMA2,              /* Internal interrupt DMA channel 2 */
    EPIC_INT_IRQ8_DMA3,              /* Internal interrupt DMA channel 3 */
    EPIC_INT_IRQ9_RIO_WRERR,         /* Internal interrupt RIO R/W error */
    EPIC_INT_IRQ10_RIO_DBELL,        /* Internal interrupt RIO doorbell */
    EPIC_INT_IRQ11_RIO_MSGOUT,       /* Internal interrupt RIO message out */
    EPIC_INT_IRQ12_RIO_MSGIN,        /* Internal interrupt RIO message in */
    EPIC_INT_IRQ13_TSEC1_TX,         /* Internal interrupt TSEC1 transmit */
    EPIC_INT_IRQ14_TSEC1_RX,         /* Internal interrupt TSEC1 receive */
    EPIC_INT_IRQ18_TSEC1_ERR,        /* Internal interrupt TSEC1 error */
    EPIC_INT_IRQ19_TSEC2_TX,         /* Internal interrupt TSEC2 transmit */
    EPIC_INT_IRQ20_TSEC2_RX,         /* Internal interrupt TSEC2 receive */
    EPIC_INT_IRQ24_TSEC2_ERR,        /* Internal interrupt TSEC2 error */
    EPIC_INT_IRQ25_FEC,              /* Internal interrupt FEC */
    EPIC_INT_IRQ26_DUART,            /* Internal interrupt DUART */
    EPIC_INT_IRQ27_I2C,              /* Internal interrupt I2C */
    EPIC_INT_IRQ28_PERFMON,          /* Internal interrupt performance mon */

    PERFMON_NUM_EVENTS               /* Total number of events available */
    } PERFMON_EVENT;

/* Event classifications */

typedef enum classes 
    {
    GENERAL,                         /* Platform events */
    DDR_MEMORY_CONTROLLER,           /* Memory controller events */
    MEMORY_TARGET_QUEUE,             /* Memory target queue events */
    RAPIDIO_CONTROLLER,              /* RapidIO controller events */
    DMA_CONTROLLER,                  /* DMA controller events */
    E500_COHERENCY_MODULE,           /* e500 coherency module events */
    INTERRUPT_CONTROLLER,            /* Interrupt controller (PIC) events */
    PCI_PCIX_COMMON,                 /* PCI/PCI-X common events */
    PCI_SPECIFIC,                    /* PCI specific events */
    TSEC,                            /* TSEC1 and TSEC2 events */
    LOCAL_BUS,                       /* Local bus events */
    L2_CACHE_SRAM,                   /* L2 cache and SRAM events */
    DEBUG,                           /* Debug events */
    DUART,                           /* DUART events */
    CHAINING,                        /* Performance monitor changing events */
    
    PERFMON_NUM_CLASSES              /* Total number of event classes */
    } PERFMON_EVENT_CLASSES;

STATUS perfMonEventCounterGet (PERFMON_EVENT, UINT32 *);

STATUS perfMonEventIdGet (PERFMON_EVENT, UINT32 *);

STATUS perfMonEventDescriptionGet (PERFMON_EVENT, const char **, const char**);

STATUS perfMonEventClassGet (PERFMON_EVENT, const char **);

STATUS perfMonEventNameGet (PERFMON_EVENT, const char **);

STATUS perfMonEventIntIdGet (PERFMON_EVENT, UINT32 *);

STATUS perfMonEventIntCounterGet (PERFMON_EVENT, UINT32 *);

STATUS perfMonEventIntMaskSet (PERFMON_EVENT);

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCperfMonEventh */
