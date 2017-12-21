/* sysCache.h -  L2/L3 cache defines */

/*
 * Copyright (c) 2008-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,20feb12,x_s  WIND00247656: add L3_CACHE_SIZE definition.
01g,03nov11,ers  WIND00297094: cache fix for vmPageLock/Unlock.
01f,08dec10,agf  clean-up compiler warnings
01e,08sep10,agf  Fix compiler warning
01d,25mar10,pmr  add L2 error support
01c,26aug09,pmr  renamed sysCache.h, added L3 defines
01b,19aug09,pmr  cleanup
01a,01dec08,pmr  Created from ads8544/01a
*/

/*
DESCRIPTION
This header provides definitions pertaining to the L2 backside cache.

This header provides definitions pertaining to the CoreNet Platform
Cache (CPC), also known as the L3 cache.
*/

#ifndef INCsyscacheh
#define INCsyscacheh

/* L2 Cache defines */

/* L2CSR0 BIT MASKS and BIT SHIFTS */
#define P4080_L2CSR0_L2E_BIT      0		/* L2 Enable */
#define P4080_L2CSR0_L2PE_BIT     1		/* L2 Parity Enable */
#define P4080_L2CSR0_L2WP_BIT     3 		/* L2 Way Partitioning (3) */
#define P4080_L2CSR0_L2CM_BIT     6		/* L2 Coherency Mode (2) */
#define P4080_L2CSR0_L2FI_BIT     10		/* L2 Flash Invalidate */
#define P4080_L2CSR0_L2IO_BIT     11		/* L2 Instruction Only */
#define P4080_L2CSR0_L2DO_BIT     15		/* L2 Data Only */
#define P4080_L2CSR0_L2REP_BIT    18		/* L2 Line Replcmnt Algo. (2) */
#define P4080_L2CSR0_L2FL_BIT     20		/* L2 Flush */
#define P4080_L2CSR0_L2LFC_BIT    21		/* L2 Lock Flush Clear */
#define P4080_L2CSR0_L2LOA_BIT    24		/* L2 Lock Overflow Allocate */
#define P4080_L2CSR0_L2LO_BIT     25		/* L2 Lock Overflow */

#define P4080_L2CSR0_L2E_MSK      (1U << (31 - P4080_L2CSR0_L2E_BIT))
#define P4080_L2CSR0_L2PE_MSK     (1U << (31 - P4080_L2CSR0_L2PE_BIT))
#define P4080_L2CSR0_L2WP_MSK     (7U << (31 - P4080_L2CSR0_L2WP_BIT - 2))
#define P4080_L2CSR0_L2CM_MSK     (3U << (31 - P4080_L2CSR0_L2CM_BIT - 1))
#define P4080_L2CSR0_L2FI_MSK	  (1U << (31 - P4080_L2CSR0_L2FI_BIT))
#define P4080_L2CSR0_L2IO_MSK     (1U << (31 - P4080_L2CSR0_L2IO_BIT))
#define P4080_L2CSR0_L2DO_MSK     (1U << (31 - P4080_L2CSR0_L2DO_BIT))
#define P4080_L2CSR0_L2REP_MSK    (3U << (31 - P4080_L2CSR0_L2REP_BIT))
#define P4080_L2CSR0_L2FL_MSK     (1U << (31 - P4080_L2CSR0_L2FL_BIT))
#define P4080_L2CSR0_L2LFC_MSK    (1U << (31 - P4080_L2CSR0_L2LFC_BIT))
#define P4080_L2CSR0_L2LOA_MSK    (1U << (31 - P4080_L2CSR0_L2LOA_BIT))
#define P4080_L2CSR0_L2LO_MSK     (1U << (31 - P4080_L2CSR0_L2LO_BIT))

#define P4080_L2ERRDET_MULL2ERR_BIT	0

/* the following 5 bits are common to L2ERRDIS, L2ERRDET, and L2ERRINTEN */
#define P4080_L2ERR_TMHIT_BIT		24
#define P4080_L2ERR_TPAR_BIT		27
#define P4080_L2ERR_MBECC_BIT		28
#define P4080_L2ERR_SBECC_BIT		29
#define P4080_L2ERR_PAR_BIT		30

#define P4080_L2ERRDIS_L2CFGDIS_BIT	31

#define P4080_L2ERRINJCTL_TERRIEN_BIT	15
#define P4080_L2ERRINJCTL_ECCMB_BIT	22
#define P4080_L2ERRINJCTL_DERRIEN_BIT	23
#define P4080_L2ERRINJCTL_ECCERRIM_MASK	(0x000000ff)U

/* L3 Cache defines */

#define NUM_L3_CACHES       2  /* One per CCM, (per DDR controller) */
#define L3_CACHE_SIZE       0x100000 /* Every L3 cache size is 1M */

#define CPCCSR0(cpcNum)   ((volatile UINT32 *) (CCSBAR + 0x010000  \
                                              +(cpcNum * 0x001000)))

#define CPCPAR(cpcNum,n)  ((volatile UINT32 *) (CCSBAR + 0x010000  \
                                              +(cpcNum * 0x001000) \
                                              +          0x000208  + n*0x10 ))

#define CPCEWCR(cpcNum,n) ((volatile UINT32 *) (CCSBAR + 0x010000  \
                                              +(cpcNum * 0x001000) \
                                              +          0x000010  + n*0x10 ))

#define CPCEWBAR(cpcNum,n)((volatile UINT32 *) (CCSBAR + 0x010000  \
                                              +(cpcNum * 0x001000) \
                                              +          0x000014  + n*0x10 ))

#define CPCHDBCR(cpcNum)  ((volatile UINT32 *) (CCSBAR + 0x010000  \
                                              +(cpcNum * 0x001000) \
                                              +          0x000F00  ))

#define CPC_ENABLE          0x80000000  /* CPCCSR0_CPCE */
#define CPC_FLUSH           0x00000800  /* CPCCSR0_CPCFL */
#define CPC_INVALIDATE      0x00200400  /* CPCCSR0_CPCFI | CPCCSR0_CPCLFC */
#define CPC_CLEAR_LOCKS     0x00000400  /* CPCCSR0_CPCLFC */
#define CPC_ERRATA_A00N     0x01400000  /* CPC-A002, CPC-A003 */

#if (L3_WRITE_THRU_MODE != 0)
#define CPC_WT_MODE         0x00080000  /* CPCCSR0_CPCWT */
#else
#define CPC_WT_MODE         0
#endif

#define CPC_ENABLE_W_MODE (CPC_ENABLE|CPC_WT_MODE)

/* Per sect 8.2.9  (sect. 2.7.3 set reserved = 0) */

#define CPCPAR_RESET_VALUE 0x000003fb


#define CPC1_BASE_ADDR		0x010000
#define CPC2_BASE_ADDR		0x011000

#define CPCEWCR0(x)		(x + 0x010)
#define CPCEWBAR0(x)		(x + 0x014)
#define CPCEWCR1(x)		(x + 0x020)
#define CPCEWBAR1(x)		(x + 0x024)
#define CPCSRCR1(x)		(x + 0x100)
#define CPCSRCR0(x)		(x + 0x104)
#define CPCPIR(x,y)		(x + 0x200 + y * 0x10)
#define CPCPWR(x,y)		(x + 0x20C + y * 0x10)
#define CPCERRINJHI(x)		(x + 0xE00)
#define CPCERRINJLO(x)		(x + 0xE04)
#define CPCERRINJCTL(x)		(x + 0xE08)
#define CPCCAPTDATAHI(x)	(x + 0xE20)
#define CPCCAPTDATALOW(x)	(x + 0xE24)
#define CPCCAPTECC(x)		(x + 0xE28)
#define CPCERRDET(x)		(x + 0xE40)
#define CPCERRDIS(x)		(x + 0xE44)
#define CPCERRINTEN(x)		(x + 0xE48)
#define CPCERRATTR(x)		(x + 0xE4C)
#define CPCERREADDR(x)		(x + 0xE50)
#define CPCERRADDR(x)		(x + 0xE54)
#define CPCERRCTL(x)		(x + 0xE58)

/* CPCCSR0 field definitions */

#ifndef _ASMLANGUAGE
typedef union cpc_cpccsr0 {
    struct {
        UINT32 cpce:1;			/* CPC enable */
        UINT32 cpcpe:1;			/* CPC parity/ECC enable */
        UINT32 unused0:8;		/* unused */
        UINT32 cpcfi:1;			/* CPC flash invalidate */
        UINT32 unused1:1;		/* unused */
        UINT32 cpcwt:1;			/* CPC write-through mode */
        UINT32 unused2:5;		/* unused */
        UINT32 cpcrep:2;		/* CPC line replacement algorithm */
        UINT32 cpcfl:1;			/* CPC flush */
        UINT32 cpclfc:1;		/* CPC lock flash clear */
        UINT32 unused3:2;		/* unused */
        UINT32 cpcloa:1;		/* CPC lock overflow allocate */
        UINT32 unused4:1;		/* unused */
        UINT32 cpclo:1;			/* CPC lock overflow */
        UINT32 unused5:5;		/* unused */
    } field;
    UINT32 value;
} CPC_CPCCSR0;

typedef union cpc_cpcewcr {
    struct {
        UINT32 e:1;			/* Enable */
        UINT32 lock:1;			/* Lock */
        UINT32 unused0:8;		/* unused */
        UINT32 size:6;			/* Size */
	UINT32 eaddrbase:16;		/* Extended Base Address */
    } field;
    UINT32 value;
} CPC_CPCEWCR;
#endif /* _ASMLANGUAGE */

#define CPCCSR0_CPCE_BIT	0 	/* CPC enable */
#define CPCCSR0_CPCPE_BIT	1 	/* CPC parity/ECC enable */
#define CPCCSR0_CPCFI_BIT	10 	/* CPC flash invalidate */
#define CPCCSR0_CPCWT_BIT	12 	/* CPC write-through mode */
#define CPCCSR0_CPCREP_BIT	18 	/* CPC line replacement algorithm (2) */
#define CPCCSR0_CPCFL_BIT	20 	/* CPC flush */
#define CPCCSR0_CPCLFC_BIT	21 	/* CPC lock flash clear */
#define CPCCSR0_CPCLOA_BIT	24 	/* CPC lock overflow allocate */
#define CPCCSR0_CPCLO_BIT	26 	/* CPC lock overflow */

#define CPCCSR0_CPCE_MSK	(1U << (31 - CPCCSR0_CPCE_BIT))
#define CPCCSR0_CPCPE_MSK	(1U << (31 - CPCCSR0_CPCPE_BIT))
#define CPCCSR0_CPCFI_MSK	(1U << (31 - CPCCSR0_CPCFI_BIT))
#define CPCCSR0_CPCWT_MSK	(1U << (31 - CPCCSR0_CPCWT_BIT))
#define CPCCSR0_CPCREP_MSK	(3U << (31 - CPCCSR0_CPCREP_BIT - 1))
#define CPCCSR0_CPCFL_MSK	(1U << (31 - CPCCSR0_CPCFL_BIT))
#define CPCCSR0_CPCLFC_MSK	(1U << (31 - CPCCSR0_CPCLFC_BIT))
#define CPCCSR0_CPCLOA_MSK	(1U << (31 - CPCCSR0_CPCLOA_BIT))
#define CPCCSR0_CPCLO_MSK	(1U << (31 - CPCCSR0_CPCLO_BIT))

/* CPCEWCRn field definitions */

#define CPCEWCRN_E_BIT		0	/* Enable */
#define CPCEWCRN_LOCK_BIT	1	/* Lock */
#define CPCEWCRN_SIZE_BIT	10	/* Size (6) 2^(size + 6) */
#define CPCEWCRN_EADDRBASE_BIT	16	/* Extended base address (16) */

#endif /* INCsyscacheh */
