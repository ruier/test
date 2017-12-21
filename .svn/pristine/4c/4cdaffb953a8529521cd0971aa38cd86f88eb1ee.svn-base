/* sysMpc8540Error.h -  MPC8540 Error Handling defines */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01e,18aug05,efb  Added support for handling SMC and PCI/X errors.
01d,22jul05,efb  Added support for handling LBC errors.
01c,19jul05,efb  Added support for handling ECM errors.
01b,24mar05,cak  Added support for handling DMA controller errors.
01a,14feb05,cak  Created
*/

#ifndef __INCsysMpc8540Errorh
#define __INCsysMpc8540Errorh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define MPC8540_ERROR_TYPE	ERROR_CPU_TYPE

/* L2 Cache/SRAM Error Types */

#define L2CACHE_ERR_TPARERR     (MPC8540_ERROR_TYPE | CCSR_L2ERRDET | \
                                 (2 << 20))
#define L2CACHE_ERR_MBECCERR    (MPC8540_ERROR_TYPE | CCSR_L2ERRDET | \
                                 (3 << 20))
#define L2CACHE_ERR_SBECCERR    (MPC8540_ERROR_TYPE | CCSR_L2ERRDET | \
                                 (4 << 20))
#define L2CACHE_ERR_L2CFGERR    (MPC8540_ERROR_TYPE | CCSR_L2ERRDET | \
                                 (5 << 20))
#define L2ERR_STR		" L2ERRDET(@2_0E40)=%08x"

/* DMA Controller Error Types */

#define	DMA_ERR			(MPC8540_ERROR_TYPE | CCSR_DGSR) 
#define	DMAERR_STR		" DGSR (@2_1300)=%08x"				 
/* ECM Error Types */

#define ECM_ERR_LAE              (MPC8540_ERROR_TYPE | CCSR_EEDR | \
                                 (2 << 20))
#define ECM_ERR_MULTI_LAE        (MPC8540_ERROR_TYPE | CCSR_EEDR | \
                                 (3 << 20))
#define ECMERR_STR               " EEDR (@0_1E00)=%08x"

/* LBC Error Types */

#define LBC_ERR_BMTOERR          (MPC8540_ERROR_TYPE | CCSR_LTESR | \
                                 (2 << 20))
#define LBC_ERR_PARERR           (MPC8540_ERROR_TYPE | CCSR_LTESR | \
                                 (3 << 20))
#define LBC_ERR_WPERR            (MPC8540_ERROR_TYPE | CCSR_LTESR | \
                                 (4 << 20))
#define LBC_ERR_ATMWERR          (MPC8540_ERROR_TYPE | CCSR_LTESR | \
                                 (5 << 20))
#define LBC_ERR_ATMRERR          (MPC8540_ERROR_TYPE | CCSR_LTESR | \
                                 (6 << 20))
#define LBC_ERR_CSERR            (MPC8540_ERROR_TYPE | CCSR_LTESR | \
                                 (7 << 20))

#define LBCERR_STR               " LTESR (@0_50B0)=%08x"

/* DDR Memory Controller (SMC) Error Types */

#define SMC_ERR_MULTI_MEMERR     (MPC8540_ERROR_TYPE | CCSR_ERR_DETECT | \
                                  (2 << 20))
#define SMC_ERR_MULTI_BITERR     (MPC8540_ERROR_TYPE | CCSR_ERR_DETECT | \
                                  (3 << 20))
#define SMC_ERR_MEM_SELERR       (MPC8540_ERROR_TYPE | CCSR_ERR_DETECT | \
                                  (4 << 20))
#define SMC_ERR_SINGLE_BITERR    (MPC8540_ERROR_TYPE | CCSR_ERR_DETECT | \
                                  (5 << 20))
#define SMCERR_STR \
"\n\r               ERR_DETECT (@0_2E40)=%08x, CAPT_ATTRIB (@0_2E4C)=%08x\n\r"\
"               CAPT_ADDR (@0_2E50)=%08x,  ERR_SBE (@0_2E58)=%08x"\

/* PCI/PCI-X Interface Error Types */

#define PCI_ERR_MULTI_ERR        (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (2 << 20))
#define PCI_ERR_ADDR_PARITYERR   (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (3 << 20))
#define PCI_ERR_RCVD_SERR        (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (4 << 20))
#define PCI_ERR_MSTR_PERR        (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (5 << 20))
#define PCI_ERR_TRGT_PERR        (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (6 << 20))
#define PCI_ERR_MSTR_ABORTERR    (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (7 << 20))
#define PCI_ERR_TRGT_ABORTERR    (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (8 << 20))
#define PCI_ERR_OWMSVERR         (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (9 << 20))
#define PCI_ERR_ORMSVERR         (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (10 << 20))
#define PCI_ERR_IRMSVERR         (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (11 << 20))
#define PCI_ERR_SCMERR           (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (12 << 20))
#define PCI_ERR_TOEERR           (MPC8540_ERROR_TYPE | CCSR_ERR_DR | \
                                  (13 << 20))

#define PCIERR_STR \
"\n\r               ERR_DR (@0_8E00)=%08x,   ERR_ATTRIB (@0_8E0C)=%08x\n\r"\
"               ERR_ADDR (@0_8E10)=%08x, ERR_EXT_ADDR (@0_8E14)=%08x\n\r"\
"               ERR_DL (@0_8E18)=%08x,   ERR_DH (@0_8E1C)=%08x"

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCsysMpc8540Errorh */
