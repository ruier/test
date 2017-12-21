/* smc8260Sio.h - Motorola PPC8260 Serial I/O header */

/* Copyright 1984-1999 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,07may01,g_h  rename to smc8260Sio.h
01a,05mar99,elk	 created from ppc860Sio.h (ver 01e).
*/

/*
 * This file contains constants for the Motorola MPC8260 PowerPC
 * microcontroller chip.
 */

#ifndef __INCsmc8260Sioh
#define __INCsmc8260Sioh

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	_ASMLANGUAGE

#include "sioLib.h"

	#ifndef M8260ABBREVIATIONS
#define M8260ABBREVIATIONS

#ifdef  _ASMLANGUAGE
#define CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile UCHAR VCHAR;   /* shorthand for volatile UCHAR */
typedef volatile INT32 VINT32; /* volatile unsigned word */
typedef volatile INT16 VINT16; /* volatile unsigned halfword */
typedef volatile INT8 VINT8;   /* volatile unsigned byte */
typedef volatile UINT32 VUINT32; /* volatile unsigned word */
typedef volatile UINT16 VUINT16; /* volatile unsigned halfword */
typedef volatile UINT8 VUINT8;   /* volatile unsigned byte */
#define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

#endif /* M8260ABBREVIATIONS */

/* device and channel structures */

/* 
 * The PPC8260SMC_CHAN structure defines a serial I/O channel which
 * describes the (TBD) registers for a given channel. 
 * Also the various SIO driver functions ( ioctl, txStarup, callbackInstall,
 * pollInput, polloutput ) that this channel could utilize are declared 
 * here.
 */

/* MPC8260 Dual Ported Ram addresses */

#define PPC8260_DPR_SMC1(dprbase)     ((VINT16 *) (dprbase + 0x87FC))
#define PPC8260_DPR_SMC2(dprbase)     ((VINT16 *) (dprbase + 0x88FC))

/* CPM Mux */

#define	CMXSMR(base)	(CAST(VUINT8  *)(base + 0x11B0C)) /* mux SMC clk rte */

/* Baud Rate Generation - BRGs 1-4 */

#define	BRGC1(base)	(CAST(VUINT32 *)(base + 0x119F0)) /* BRG1 Config Reg */
#define	BRGC2(base)	(CAST(VUINT32 *)(base + 0x119F4)) /* BRG2 Config Reg */
#define	BRGC3(base)	(CAST(VUINT32 *)(base + 0x119F8)) /* BRG3 Config Reg */
#define	BRGC4(base)	(CAST(VUINT32 *)(base + 0x119FC)) /* BRG4 Config Reg */

#define BRGC_CD_MSK             0x01FFE         /* clock divider 12 bit value */

/* SMC UART Event Register bit definition (SMCE - 0x11A86,0x11A96) */ 

#define SMCE_BRK                0x10    /* Break character received */
#define SMCE_BSY                0x04    /* Busy Condition */
#define SMCE_TX                 0x02    /* Transmit Buffer */
#define SMCE_RX                 0x01    /* Receive Buffer */
#define SMCE_ALL_EVENTS         (SMCE_BRK | SMCE_BSY | SMCE_TX | SMCE_RX)

/* SMC UART Mask Register bit definition (SMCM - 0x11A8A,0x11A9A) */ 

#define SMCM_BRK_MSK            0x10    /* Break character received Mask */
#define SMCM_BSY_MSK            0x04    /* Busy Condition Mask */
#define SMCM_TX_MSK             0x02    /* Transmit Buffer Mask */
#define SMCM_RX_MSK             0x01    /* Receive Buffer Mask */

/* SMC UART Mode Register bit definition (SMCMR - 0x0A82) */

#define	SMCMR_CLEN_MSK		0x7800	/* Character Length Mask */
#define SMCMR_CLEN_0_BIT	0x0000	/* Character Length: 0 bit */
#define SMCMR_CLEN_1_BIT	0x0800	/* Character Length: 1 bit */
#define SMCMR_CLEN_2_BITS	0x1000	/* Character Length: 2 bits */
#define SMCMR_CLEN_3_BITS	0x1800	/* Character Length: 3 bits */
#define SMCMR_CLEN_4_BITS	0x2000	/* Character Length: 4 bits */
#define SMCMR_CLEN_5_BITS	0x2800	/* Character Length: 5 bits */
#define SMCMR_CLEN_6_BITS	0x3000	/* Character Length: 6 bits */
#define SMCMR_CLEN_7_BITS	0x3800	/* Character Length: 7 bits */
#define SMCMR_CLEN_8_BITS	0x4000	/* Character Length: 8 bits */
#define SMCMR_CLEN_9_BITS	0x4800	/* Character Length: 9 bits */
#define SMCMR_CLEN_10_BITS	0x5000	/* Character Length: 10 bits */
#define SMCMR_CLEN_11_BITS	0x5800	/* Character Length: 11 bits */
#define SMCMR_CLEN_12_BITS	0x6000	/* Character Length: 12 bits */
#define SMCMR_CLEN_13_BITS	0x6800	/* Character Length: 13 bits */
#define SMCMR_CLEN_14_BITS	0x7000	/* Character Length: 14 bits */
#define SMCMR_CLEN_15_BITS	0x7800	/* Character Length: 15 bits */
#define SMCMR_SL		0x0400	/* Stop Length */		
#define SMCMR_SL_1_BIT		0x0000	/* Stop Length: 1 bit */		
#define SMCMR_SL_2_BITS		0x0400	/* Stop Length: 2 bits */		
#define SMCMR_PEN		0x0200	/* Parity Enable */
#define SMCMR_PM		0x0100	/* Parity Mode */
#define SMCMR_PM_ODD		0x0000	/* Parity Mode: Odd parity */
#define SMCMR_PM_EVEN		0x0100	/* Parity Mode: Even parity */
#define SMCMR_SM_MSK		0x0030	/* SMC Mode Mask */
#define SMCMR_SM_GCI_SCIT	0x0000	/* SMC Mode: GCI or SCIT support */
#define SMCMR_SM_UART		0x0020	/* SMC Mode: GCI or SCIT support */
#define SMCMR_SM_TRANSPARENT	0x0030	/* SMC Mode: Totally transparent */
#define SMCMR_DM_MSK		0x000c	/* Diagnostic Mode Mask */
#define SMCMR_DM_NORMAL		0x0000	/* Diagnostic Mode: Normal */
#define SMCMR_DM_LCLB		0x0004	/* Diagnostic Mode: Local Loopback */
#define SMCMR_DM_ECHO		0x0008	/* Diagnostic Mode: Echo */
#define SMCMR_TEN		0x0002	/* SMC Transmit Enable */
#define SMCMR_REN		0x0001	/* SMC Receive Enable */

#define SMCMR_STD_MODE  	(SMCMR_CLEN_9_BITS | \
				 SMCMR_SL_1_BIT    | \
				 SMCMR_SM_UART	   | \
				 SMCMR_DM_NORMAL)


#define SMCMR_STD_MODE_ENABLED	(SMCMR_STD_MODE	| \
				 SMCMR_TEN	| \
				 SMCMR_REN)

/*
 * dual-ported parameter RAM SMC offsets                     
 * access these with SMC_RBASE(base_dpram_addr_of_this_SMC1) 
 * base_dpram_addr_of_this_SMC1 == PPC8260_DPR_SMC1(dprbase)  
 * or == PPC8260_DPR_SMC2(dprbase)                            
 */
#define SMC_RBASE(base)   ((VINT16 *) (base + 0x00) /* Rx Buff Descr Base */
#define SMC_TBASE(base)   ((VINT16 *) (base + 0x02) /* Tx Buff Descr Base */
#define SMC_RFCR(base)    ((VINT8  *) (base + 0x04) /* Rx Function Code */
#define SMC_TFCR(base)    ((VINT8  *) (base + 0x05) /* Tx Function Code */
#define SMC_MRBLR(base)   ((VINT16 *) (base + 0x06) /* Max Rcv Buff Length */
#define SMC_RSTATE(base)  ((VINT32 *) (base + 0x08) /* Rx Internal State */
#define SMC_RBPTR(base)   ((VINT16 *) (base + 0x10) /* Rx Buffer Pointer */
#define SMC_TSTATE(base)  ((VINT32 *) (base + 0x18) /* Tx Internal State */
#define SMC_TBPTR(base)   ((VINT16 *) (base + 0x20) /* Tx Buffer Pointer */
#define SMC_MAX_IDL(base) ((VINT16 *) (base + 0x28) /* Max Idle characters */
#define SMC_IDLC(base)    ((VINT16 *) (base + 0x2A) /* Temp Idle counter */
#define SMC_BRKLN(base)   ((VINT16 *) (base + 0x2C) /* Last Rcv Break length */
#define SMC_BRKEC(base)   ((VINT16 *) (base + 0x2E) /* Recv Brk Cond counter */
#define SMC_BRKCR(base)   ((VINT16 *) (base + 0x30) /* Break Count reg (Tx) */
#define SMC_RMASK(base)   ((VINT16 *) (base + 0x32) /* Temp bit mask */

/* SMC 1 register set */

#define	SMCMR1(base)(CAST(VUINT16 *)(base + 0x11A82)) /* SMC1 Mode Reg */
#define	SMCE1(base)	(CAST(VUINT8  *)(base + 0x11A86)) /* SMC1 Event Reg */
#define	SMCM1(base)	(CAST(VUINT8  *)(base + 0x11A8A)) /* SMC1 Mask Reg */
/* 11A8B-11A91 RESERVED */

/* SMC 2 register set */

#define	SMCMR2(base)(CAST(VUINT16 *)(base + 0x11A92)) /* SMC2 Mode Reg */
#define	SMCE2(base)	(CAST(VUINT8  *)(base + 0x11A96)) /* SMC2 Event Reg */
#define	SMCM2(base)	(CAST(VUINT8  *)(base + 0x11A9A)) /* SMC2 Mask Reg */
/* 11A9B-11A9F RESERVED */

typedef struct          /* SMC_BUF */
    {
    VUINT16	statusMode;             /* status and control */
    VINT16      dataLength;         /* length of data buffer in bytes */
    u_char *    dataPointer;        /* points to data buffer */
    } SMC_BUF;

typedef struct          /* SMC_PARAM */
    {	                        /* offset description*/
    VINT16      rbase;          /* 00 Rx buffer descriptor base address */
    VINT16      tbase;          /* 02 Tx buffer descriptor base address */
    VINT8       rfcr;           /* 04 Rx function code */
    VINT8       tfcr;           /* 05 Tx function code */
    VINT16      mrblr;          /* 06 maximum receive buffer length */
    VINT32      rstate;         /* 08 Rx internal state */
    VINT32      res1;           /* 0C Rx internal data pointer */
    VINT16      rbptr;          /* 10 Rx buffer descriptor pointer */
    VINT16      res2;           /* 12 reserved/internal */
    VINT32      res3;           /* 14 reserved/internal */
    VINT32      tstate;         /* 18 Tx internal state */
    VINT32      res4;           /* 1C reserved/internal */
    VINT16      tbptr;          /* 20 Tx buffer descriptor pointer */
    VINT16      res5;           /* 22 reserved/internal */
    VINT32      res6;           /* 24 reserved/internal */
    VINT16      maxidl;         /* 28 Maximum idle characters */
    VINT16      idlc;           /* 2A temporary idle counter */
    VINT16      brkln;          /* 2C last recv break length */
    VINT16      brkec;          /* 2E recv break condition counter */
    VINT16      brkcr;          /* 30 xmit break count register */
    VINT16      r_mask;         /* 32 temporary bit mask */
    } SMC_PARAM;

typedef struct          /* SMC */
    {
    SMC_PARAM   param;                  /* SMC parameters */
    } SMC;

typedef struct          /* SMC_REG */
    {
    VINT16      smcmr;                  /* SMC Mode register */
    VINT8	res1[2];		/* reserved */
    VINT8       smce;                   /* SMC Event register */
    VINT8	res2[3];		/* reserved */
    VINT8       smcm;                   /* SMC Mask register */
    } SMC_REG;

/* SMC device descriptor */

typedef struct          /* SMC_DEV */
    {
    int                 smcNum;         /* number of SMC device (1 or 2)*/
    int                 txBdNum;        /* number of transmit buf descriptors */
    int                 rxBdNum;        /* number of receive buf descriptors */
    SMC_BUF *           txBdBase;       /* transmit BD base address */
    SMC_BUF *           rxBdBase;       /* receive BD base address */
    u_char *            txBufBase;      /* transmit buffer base address */
    u_char *            rxBufBase;      /* receive buffer base address */
    VINT32              txBufSize;      /* transmit buffer size */
    VINT32              rxBufSize;      /* receive buffer size */
    int                 txBdNext;       /* next transmit BD to fill */
    int                 rxBdNext;       /* next receive BD to read */
    volatile SMC *      pSmc;           /* SMC parameter RAM. Must point */
					                    /* at DPRAM area for SMC1 or SMC2 */
    volatile SMC_REG *  pSmcReg;        /* SMC registers must point at */
					                    /* SMCMR1 or SMCMR2 */
    VINT32              intMask;        /* interrupt acknowledge mask */
    } SMC_DEV;

/* standard dual-mode serial driver header structure follows      */

typedef struct ppc8260smc_chan	/* PPC8260SMC_CHAN */
    {
    /* always goes first */
    SIO_DRV_FUNCS	*pDrvFuncs;	/* driver functions */
    
    /* callbacks */

    STATUS	(*getTxChar)();	/* pointer to a xmitr function */
    STATUS	(*putRcvChar)();/* pointer to a recvr function */
    void *	getTxArg;
    void *	putRcvArg;

    VINT16              int_vec;        /* interrupt vector number */
    VINT16              channelMode;    /* SIO_MODE                */
    int                 baudRate;
    int                 clockRate;      /* CPU clock frequency (Hz) */
    int                 bgrNum;         /* number of BRG being used */
    VINT32 *            pBaud;          /* BRG registers */
    VINT32              regBase;        /* register/DPR base address */
    SMC_DEV             uart;           /* UART SCC device */
    } PPC8260SMC_CHAN;


/* serial procedures */
IMPORT	void	smc8260DevInit	(PPC8260SMC_CHAN *);
IMPORT	void	smc8260Int		(PPC8260SMC_CHAN *);

/* Baud rate */

#define SMC_MAX_BAUD 115200
#define SMC_MIN_BAUD 50

/* channels */

#define PPC8260_CHANNEL_A	0
#define PPC8260_CHANNEL_B	1

#define PPC8260SMC_N_CHANS    2	/* number of serial channels on chip */

/* SIO -- mpc8260 serial channel chip -- register definitions */

/* Buffer Descriptor Pointer Definitions */

/* Receive BD status bits 16-bit value */

#define BD_RX_EMPTY_BIT		0x8000	/* buffer is empty */
#define BD_RX_WRAP_BIT		0x2000	/* last BD in chain */
#define BD_RX_INTERRUPT_BIT	0x1000	/* set interrupt when filled */
#define BD_RX_CON_MODE_BIT	0x0200	/* Continuous Mode bit */
#define BD_RX_IDLE_CLOSE_BIT	0x0100	/* Close on IDLE recv bit */
#define BD_RX_BREAK_CLOSE_BIT	0x0020	/* Close on break recv bit */
#define BD_RX_FRAME_CLOSE_BIT	0x0010	/* Close on frame error bit */
#define BD_RX_PARITY_ERROR_BIT	0x0008	/* Parity error in last byte */
#define BD_RX_OVERRUN_ERROR_BIT	0x0002	/* Overrun occurred */

/* Transmit BD status bits 16-bit value */

#define BD_TX_READY_BIT		0x8000	/* Transmit ready/busy bit */
#define BD_TX_WRAP_BIT		0x2000	/* last BD in chain */
#define BD_TX_INTERRUPT_BIT	0x1000	/* set interrupt when emptied */
#define BD_TX_CON_MODE_BIT	0x0200	/* Continuous Mode bit */
#define BD_TX_PREAMBLE_BIT	0x0100	/* send preamble sequence */

#define BD_STATUS_OFFSET	0x00	/* two bytes */
#define BD_DATA_LENGTH_OFFSET	0x02	/* two bytes */
#define	BD_BUF_POINTER_OFFSET	0x04	/* four bytes */

/*
 * MPC8260 internal register/memory map (section 17 of prelim. spec)
 * note that these are offsets from the value stored in the IMMR
 * register, which is in the PowerPC special register address space
 * at register number 638
 */

/**** SIU module configuration ***/
#define MPC8260_SIUMCR(base)	((VINT32 *) (base + 0x10000))
/**** System Protection Control ***/
#define	MPC8260_SYPCR(base)	((VINT32 *) (base + 0x10004))
/*** Software service reg ***/
#define	MPC8260_SWSR(base)	((VINT16 *) (base + 0x1000E))
/*** Interrupt pending reg ***/
#define	MPC8260_SIVEC(base)	((VINT32 *) (base + 0x10C04))


/*** Data Param RAM base addr ***/
#define	MPC8260_DPRAM1_BASE(base)	(base + 0x0000) /* 16 Kbytes */
#define	MPC8260_DPRAM2_BASE(base)	(base + 0x8000) /*  4 Kbytes */
#define	MPC8260_DPRAM3_BASE(base)	(base + 0xb000) /*  4 Kbytes */

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCppc8260Sioh */
