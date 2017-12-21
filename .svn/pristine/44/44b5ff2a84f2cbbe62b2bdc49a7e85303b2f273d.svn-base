/* fsl_mpc8360_rdk.h - Freescale MPC8360 RDK board header */

/*
 * Copyright (c) 2005-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,23sep09,x_z  remove QE_MURAM_POOL1_SIZE and QE_MURAM_POOL2_SIZE.
01g,15jul09,b_m  add QE MURAM defines.
01f,16jun08,y_w  Add Parallel I/O Ports macros
01e,07jan08,z_l  Add UART support macros.
01d,05sep07,dtr  WIND0045894 - Change INUM_CPIO1 to INUM_GPIO1.
01c,06jun07,dtr  Support vxBus Interrupt controller drivers.
01b,07jun06,dtr  Add CMXUPCR reg definitions.
01a,16mar05,dtr  adapted from ads834x.h
*/

/*
DESCRIPTION
This file contains I/O addresses and related constants for the
Freescale MPC8360 RDK board.
*/

#ifndef INCfslmpc8360rdkh
#define INCfslmpc8360rdkh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef  _ASMLANGUAGE
#   if (defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
    /* nothing needed */
#   else  /* _WRS_VXWORKS_MAJOR */
        typedef void * VIRT_ADDR;
        typedef void * PHYS_ADDR;
#   endif  /* _WRS_VXWORKS_MAJOR */
#endif  /* _ASMLANGUAGE */

#ifndef M8260ABBREVIATIONS
#define M8260ABBREVIATIONS

#ifdef  _ASMLANGUAGE
#define CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile UCHAR  VCHAR;      /* shorthand for volatile UCHAR */
typedef volatile INT32  VINT32;     /* volatile unsigned word */
typedef volatile INT16  VINT16;     /* volatile unsigned halfword */
typedef volatile INT8   VINT8;      /* volatile unsigned byte */
typedef volatile UINT32 VUINT32;    /* volatile unsigned word */
typedef volatile UINT16 VUINT16;    /* volatile unsigned halfword */
typedef volatile UINT8  VUINT8;     /* volatile unsigned byte */
#define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

#endif /* M8260ABBREVIATIONS */

#define N_SIO_CHANNELS      2       /* No. serial I/O channels */

/* define the decrementer input clock frequency */

#define SPD_DATA_SIZE         128
#undef  BOOT_LINE_SIZE
#define BOOT_LINE_SIZE        256
#define NV_RAM_SIZE_WRITEABLE NV_RAM_SIZE  /* force VTS to only use 128 bytes */
#define MAX_MAC_ADRS 2
#define MAC_ADRS_LEN 6

#define WR_ENET0        0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1        0xA0
#define WR_ENET2        0x1E

#define CUST_ENET3_0    0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1    0xA1
#define CUST_ENET4      0xAA
#define CUST_ENET5      0xA0

/* Base Address of Memory Mapped Registers */

#define CCSBAR_INIT     0xFF400000
#define CCSBAR          0xE0000000
#define CCSBAR_SIZE     0x400000 /* 1MB */

/* Defines used for driver features*/

#define PQ2PRO_QE
#define QUICC_ENGINE
#undef  USE_MUX_LAYER

/* Register Set Base addresses */

#define SCCFG_REG_BA        0x00100 
#define WDT_REG_BA          0x00200
#define QUICC_RTC_BASE      0x00300
#define QUICC_PIT_BASE      0x00400
#define QUICC_GTM1_BASE     0x00500
#define QUICC_GTM2_BASE     0x00600
#define SIC_REG_BA          0x00700
#define ARBITER_REG_BA      0x00800
#define RESETM_REG_BA       0x00900

#define CLKM_REG_BA         0x00A00

#define QE_PORT_INTERRUPTS  0x00C00

#define QE_IO_PORTS         0x01400
#define QE_SEC_BA_WINDOWS   0x01800

#define DDR_REG_BA          0x02000
#define LBC_REG_BA          0x05000
#define DMA_REG_BA          0x08100
#define PCICFG_REG_BA       0x08300
#define PCI2CFG_REG_BA      0x08380
#define SEC_REG_BA          0x101B4
#define QUICC_ENGINE_BA     0x100000
#define QE_MURAM_BA         0x010000
#define QE_MURAM_SIZE       0xC000
#define DMA_NUM 4

#define UCC1_BASE           0x2000
#define UCC2_BASE           0x3000
#define UCC3_BASE           0x2200
#define UCC4_BASE           0x3200
#define UCC5_BASE           0x2400
#define UCC6_BASE           0x3400
#define UCC7_BASE           0x2600
#define UCC8_BASE           0x3600

#define QE_MUX (QUICC_ENGINE_BA + 0x400)

/* CPM mux FCC clock route register */

#define QE_CMXUCR1(base)   (CAST(VUINT32 *)((base) + QE_MUX + 0x10))
#define QE_CMXUCR2(base)   (CAST(VUINT32 *)((base) + QE_MUX + 0x14))
#define QE_CMXUCR3(base)   (CAST(VUINT32 *)((base) + QE_MUX + 0x18))
#define QE_CMXUCR4(base)   (CAST(VUINT32 *)((base) + QE_MUX + 0x1C))
#define QE_CMXUPCR(base)   (CAST(VUINT32 *)((base) + QE_MUX + 0x22))

#define QE_SDMA (QUICC_ENGINE_BA + 0x4000)

/* CPM mux FCC clock route register */

#define QE_SDSR(base)    (CAST(VUINT32 *)((base) + QE_SDMA + 0x00))
#define QE_SDMR(base)    (CAST(VUINT32 *)((base) + QE_SDMA + 0x04))
#define QE_SDTR1(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x08))
#define QE_SDTR2(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x0C))
#define QE_SDHY1(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x10))
#define QE_SDHY2(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x14))
#define QE_SDTA1(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x18)) /* read only */
#define QE_SDTA2(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x1C)) /* read only */
#define QE_SDTM1(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x20)) /* read only */
#define QE_SDTM2(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x24)) /* read only */

#define QE_SDAQR(base)   (CAST(VUINT32 *)((base) + QE_SDMA + 0x38))
#define QE_SDAQMR(base)  (CAST(VUINT32 *)((base) + QE_SDMA + 0x3C))

#define QE_SDEBCR(base)  (CAST(VUINT32 *)((base) + QE_SDMA + 0x44))

/* 
 * Define the register definition and interrupt numbers based on 
 * cpu varient 
 */

#include <hwif/intCtlr/vxbQuiccIntCtlr.h>
#include <hwif/intCtlr/vxbQeIntCtlr.h>
#include <drv/mem/quiccLbc.h>

/* System Configuration Control */

#define M83XX_SGPRL(base)  (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x00 ))
#define M83XX_SGPRH(base)  (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x04 ))
#define M83XX_SPRIDR(base) (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x08 ))


#define M83XX_SPCR(base)   (CAST(VUINT32 *)((base) +  SCCFG_REG_BA + 0x10 ))
#define M83XX_SPCR_OPT     0x00800000
#define M83XX_SPCR_TBEN    0x00400000

#define M83XX_SICRL(base)  (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x14 ))
#define M83XX_SICRH(base)  (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x18 ))

/* Watch Dog Registers */

#define M83XX_SWCRR(base)  (CAST(VUINT32 *)((base) + WDT_REG_BA + 0x04 ))
#define M83XX_SWCNR(base)  (CAST(VUINT32 *)((base) + WDT_REG_BA + 0x08 ))
#define M83XX_SWSRR(base)  (CAST(VUINT16 *)((base) + WDT_REG_BA + 0x0E ))

/* Clock Module */

#define M83XX_SPMR(base)   (CAST(VUINT32 *)((base) + CLKM_REG_BA + 0x00 ))
#define M83XX_SPMR_LBIU_VAL(spmrVal)    (spmrVal >> 31);
#define M83XX_SPMR_DDR_VAL(spmrVal)     (spmrVal >> 30) & 0x1;
#define M83XX_SPMR_SPMF_VAL(spmrVal)    (spmrVal >> 24) & 0xf;
#define M83XX_SPMR_CLK_DIV(spmrVal)     (spmrVal >> 23) & 0x1;
#define M83XX_SPMR_CLK_PLL(spmrVal)     (spmrVal >> 16) & 0x7f;

#define M83XX_OCCR(base)   (CAST(VUINT32 *)((base) + CLKM_REG_BA + 0x04 ))
#define M83XX_SCCR(base)   (CAST(VUINT32 *)((base) + CLKM_REG_BA + 0x08 ))

#define M83XX_SCCR_RES_MSK      0xf3f10000      /* reserved field Mask */

/* Arbiter registers*/

#define M83XX_ACR(base)     (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x00))
#define M83XX_ATR(base)     (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x04))
#define M83XX_AER(base)     (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x0c))
#define M83XX_AIDR(base)    (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x10))
#define M83XX_AMR(base)     (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x14))
#define M83XX_AEATR(base)   (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x18))
#define M83XX_AEADR(base)   (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x1c))
#define M83XX_AERR(base)    (CAST(VUINT32 *)((base) + ARBITER_REG_BA + 0x20))

/* Reset Configuration Module */

#define M83XX_RCWLR(base)   (CAST(VUINT32 *)((base) + RESETM_REG_BA + 0x00))
#define M83XX_RCWHR(base)   (CAST(VUINT32 *)((base) + RESETM_REG_BA + 0x04))
#define M83XX_RSR(base)     (CAST(VUINT32 *)((base) + RESETM_REG_BA + 0x10))
#define M83XX_RMR(base)     (CAST(VUINT32 *)((base) + RESETM_REG_BA + 0x14))
#define M83XX_RPR(base)     (CAST(VUINT32 *)((base) + RESETM_REG_BA + 0x18))
#define M83XX_RCR(base)     (CAST(VUINT32 *)((base) + RESETM_REG_BA + 0x1c))
#define M83XX_RCER(base)    (CAST(VUINT32 *)((base) + RESETM_REG_BA + 0x20))

/* QE Parallel I/O Port A */

#define M83XX_CPODRA(base)  (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x00))
#define M83XX_CPDATA(base)  (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x04))
#define M83XX_CPDIR1A(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x08))
#define M83XX_CPDIR2A(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x0c))
#define M83XX_CPPAR1A(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x10))
#define M83XX_CPPAR2A(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x14))

/* QE Parallel I/O Port B */

#define M83XX_CPODRB(base)  (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x18))
#define M83XX_CPDATB(base)  (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x1c))
#define M83XX_CPDIR1B(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x20))
#define M83XX_CPDIR2B(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x24))
#define M83XX_CPPAR1B(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x28))
#define M83XX_CPPAR2B(base) (CAST(VUINT32 *)((base) + QE_IO_PORTS + 0x2c))

/* I2C1 Controller */

#define  M834X_I2C1_BASE    0x03000

#define  I2C1ADR            0x03000
#define  I2C1FDR            0x03004
#define  I2C1ICR            0x03008
#define  I2C1ISR            0x0300c
#define  I2C1IDR            0x03010
#define  I2C1DFSRR          0x03014

#define  M834X_I2C1ADR(base)        (CAST(VUINT8 *)((base) | I2C1ADR))
#define  M834X_I2C1FDR(base)        (CAST(VUINT8 *)((base) | I2C1FDR))
#define  M834X_I2C1ICR(base)        (CAST(VUINT8 *)((base) | I2C1ICR))
#define  M834X_I2C1ISR(base)        (CAST(VUINT8 *)((base) | I2C1ISR))
#define  M834X_I2C1IDR(base)        (CAST(VUINT8 *)((base) | I2C1IDR))
#define  M834X_I2C1DFSRR(base)      (CAST(VUINT32 *)((base) | I2C1DFSRR))

/* I2C2 Controller */

#define  M834X_I2C2_BASE    0x03100

#define  I2C2ADR            0x03100
#define  I2C2FDR            0x03104
#define  I2C2ICR            0x03108
#define  I2C2ISR            0x0310c
#define  I2C2IDR            0x03110
#define  I2C2DFSRR          0x03114

#define  M834X_I2C2ADR(base)        (CAST(VUINT8 *)((base) | I2C2ADR))
#define  M834X_I2C2FDR(base)        (CAST(VUINT8 *)((base) | I2C2FDR))
#define  M834X_I2C2ICR(base)        (CAST(VUINT8 *)((base) | I2C2ICR))
#define  M834X_I2C2ISR(base)        (CAST(VUINT8 *)((base) | I2C2ISR))
#define  M834X_I2C2IDR(base)        (CAST(VUINT8 *)((base) | I2C2IDR))
#define  M834X_I2C2DFSRR(base)      (CAST(VUINT32 *)((base) | I2C2DFSRR))

/* Security IMMR registers */

#define QUICC_SECBR(base) (CAST(VUINT32 *)((base) + SEC_REG_BA + 0x0)) 
#define QUICC_SECMR(base) (CAST(VUINT32 *)((base) + SEC_REG_BA + 0x4))

#define SEC_ENG_BASE_ADRS (CCSBAR + 0x40000)
#define SEC_ENG_SIZE      0x20000
#define SEC_ENG_SIZE_MASK 0xfffe0000

/* These are little-endian */
#define  M83XX_CAPLENGTH(base)           (CAST(VUINT8  *)((base) | 0x22100))
#define  M83XX_HCIVERSION(base)          (CAST(VUINT16 *)((base) | 0x22102))
#define  M83XX_HCSPARAMS_L(base)         (CAST(VUINT32 *)((base) | 0x22104))
#define  M83XX_HCSPARAMS_H(base)         (CAST(VUINT32 *)((base) | 0x22108))

#define  M83XX_USBCMD(base)              (CAST(VUINT32 *)((base) | 0x22140))
#define  M83XX_USBSTS(base)              (CAST(VUINT32 *)((base) | 0x22144))
#define  M83XX_USBINTR(base)             (CAST(VUINT32 *)((base) | 0x22148))
#define  M83XX_FRINDEX(base)             (CAST(VUINT32 *)((base) | 0x2214c))
#define  M83XX_PERIODICLISTBASE(base)    (CAST(VUINT32 *)((base) | 0x22154))
#define  M83XX_ASYNCLISTADDR(base)       (CAST(VUINT32 *)((base) | 0x22158))
#define  M83XX_ASYNCTTSTS(base)          (CAST(VUINT32 *)((base) | 0x2215c))
#define  M83XX_BURSTSIZE(base)           (CAST(VUINT32 *)((base) | 0x22160))
#define  M83XX_TXTTFILLTUNING_L(base)    (CAST(VUINT32 *)((base) | 0x22164))
#define  M83XX_TXTTFILLTUNING_H(base)    (CAST(VUINT32 *)((base) | 0x22168))
#define  M83XX_CONFIGFLAG(base)          (CAST(VUINT32 *)((base) | 0x22180))
#define  M83XX_PORTSC1(base)             (CAST(VUINT32 *)((base) | 0x22184))
#define  M83XX_PORTSC2(base)             (CAST(VUINT32 *)((base) | 0x22188))
#define  M83XX_USBMODE(base)             (CAST(VUINT32 *)((base) | 0x221a8))

/* These are big-endian */
#define  M83XX_SNOOP1(base)              (CAST(VUINT32 *)((base) | 0x22400))
#define  M83XX_SNOOP2(base)              (CAST(VUINT32 *)((base) | 0x22404))
#define  M83XX_AGE_CNT_THRESH(base)      (CAST(VUINT32 *)((base) | 0x22408))
#define  M83XX_PRI_CTRL(base)            (CAST(VUINT32 *)((base) | 0x2240c))
#define  M83XX_SI_CTRL(base)             (CAST(VUINT32 *)((base) | 0x22410))
#define  M83XX_CONTROL(base)             (CAST(VUINT32 *)((base) | 0x22500))

/* Local address windows */

#define M83XX_LBLAWBARn(base,n)     (CAST(VUINT32 *)((base) + 0x20 + (n*0x8)))
#define M83XX_LBLAWARn(base,n)      (CAST(VUINT32 *)((base) + 0x24 + (n*0x8)))
#define M83XX_PCILAWBARn(base,n)    (CAST(VUINT32 *)((base) + 0x60 + (n*0x8)))
#define M83XX_PCILAWARn(base,n)     (CAST(VUINT32 *)((base) + 0x64 + (n*0x8)))
#define M83XX_DDRLAWBARn(base,n)    (CAST(VUINT32 *)((base) + 0xA0 + (n*0x8)))
#define M83XX_DDRLAWARn(base,n)     (CAST(VUINT32 *)((base) + 0xA4 + (n*0x8)))

#define LAWBAR_ADRS_SHIFT  0

#define  LAWAR_ENABLE       0x80000000

/* LAWAR SIZE Settings */

#define  LAWAR_SIZE_4KB     0x0000000B
#define  LAWAR_SIZE_8KB     0x0000000C
#define  LAWAR_SIZE_16KB    0x0000000D
#define  LAWAR_SIZE_32KB    0x0000000E
#define  LAWAR_SIZE_64KB    0x0000000F
#define  LAWAR_SIZE_128KB   0x00000010
#define  LAWAR_SIZE_256KB   0x00000011
#define  LAWAR_SIZE_512KB   0x00000012
#define  LAWAR_SIZE_1MB     0x00000013
#define  LAWAR_SIZE_2MB     0x00000014
#define  LAWAR_SIZE_4MB     0x00000015
#define  LAWAR_SIZE_8MB     0x00000016
#define  LAWAR_SIZE_16MB    0x00000017
#define  LAWAR_SIZE_32MB    0x00000018
#define  LAWAR_SIZE_64MB    0x00000019
#define  LAWAR_SIZE_128MB   0x0000001A
#define  LAWAR_SIZE_256MB   0x0000001B
#define  LAWAR_SIZE_512MB   0x0000001C
#define  LAWAR_SIZE_1GB     0x0000001D
#define  LAWAR_SIZE_2GB     0x0000001E

/* PCI Window SIZE  */

#define  PCI_SIZE_4KB       0x0000000B
#define  PCI_SIZE_8KB       0x0000000C
#define  PCI_SIZE_16KB      0x0000000D
#define  PCI_SIZE_32KB      0x0000000E
#define  PCI_SIZE_64KB      0x0000000F
#define  PCI_SIZE_128KB     0x00000010
#define  PCI_SIZE_256KB     0x00000011
#define  PCI_SIZE_512KB     0x00000012
#define  PCI_SIZE_1MB       0x00000013
#define  PCI_SIZE_2MB       0x00000014
#define  PCI_SIZE_4MB       0x00000015
#define  PCI_SIZE_8MB       0x00000016
#define  PCI_SIZE_16MB      0x00000017
#define  PCI_SIZE_32MB      0x00000018
#define  PCI_SIZE_64MB      0x00000019
#define  PCI_SIZE_128MB     0x0000001A
#define  PCI_SIZE_256MB     0x0000001B
#define  PCI_SIZE_512MB     0x0000001C
#define  PCI_SIZE_1GB       0x0000001D
#define  PCI_SIZE_2GB       0x0000001E

/* Offsets for DDR registers */

#define DDRBA               (CCSBAR | DDR_REG_BA)
#define CS0_BNDS            0x000
#define CS1_BNDS            0x008
#define CS2_BNDS            0x010
#define CS3_BNDS            0x018
#define CS0_CONFIG          0x080
#define CS1_CONFIG          0x084
#define CS2_CONFIG          0x088
#define CS3_CONFIG          0x08C

#define TIMING_CFG_1            0x108
#define TIMING_CFG_2            0x10C
#define DDR_SDRAM_CFG           0x110
#define DDR_SDRAM_MODE_CFG      0x118
#define DDR_SDRAM_INTERVAL      0x124
#define DDR_SDRAM_CLK_CNTRL     0x130

#define DDR_DATA_ERR_INJECT_HI  0xe00
#define DDR_DATA_ERR_INJECT_LO  0xe04
#define DDR_ECC_ERR_INJECT      0xe08
#define DDR_CAPTURE_DATA_HI     0xe20
#define DDR_CAPTURE_DATA_LO     0xe24
#define DDR_CAPTURE_ECC         0xe28
#define DDR_ERR_DETECT          0xe40
#define DDR_ERR_DISABLE         0xe44
#define DDR_ERR_INT_EN          0xe48
#define DDR_CAPTURE_ATTRIBUTES  0xe4c
#define DDR_CAPTURE_ADDRESS     0xe50
#define DDR_ERR_SBE             0xe58

/* add PCI access macros */

#define PCI_MEMIO2LOCAL(x)  (((UINT32)x  - PCI_MEMIO_ADRS) + CPU_PCI_MEMIO_ADRS)

/* PCI IO memory adrs to CPU (60x bus) adrs */

#define PCI_IO2LOCAL(x)     (((UINT32)x  - PCI_IO_ADRS)    + CPU_PCI_IO_ADRS)

#define PCI_MEM2LOCAL(x)    (((UINT32)x  - PCI_MEM_ADRS)   + CPU_PCI_MEM_ADRS)

/* 60x bus adrs to PCI (non-prefetchable) memory address */

#define LOCAL2PCI_MEMIO(x)  ((int)(x) + PCI_MSTR_MEM_BUS)

/* PCI defines begin */
/* configuration space reg and int ack */

#define PCI_CFG_ADR_REG     (CCSBAR + PCICFG_REG_BA + 0x00)
#define PCI_CFG_DATA_REG    (CCSBAR + PCICFG_REG_BA + 0x04)
#define PCI_INT_ACK         (CCSBAR + PCICFG_REG_BA + 0x08)

#define PCI2_CFG_ADR_REG    (CCSBAR + PCI2CFG_REG_BA + 0x00)
#define PCI2_CFG_DATA_REG   (CCSBAR + PCI2CFG_REG_BA + 0x04)
#define PCI2_INT_ACK        (CCSBAR + PCI2CFG_REG_BA + 0x08)

#define PCI_AUTO_CONFIG_ADRS    0x4c00

#define PPCACR_PRKM_MASK        0xf0
#define PCI_REQUEST_LEVEL       0x3

#define CLASS_OFFSET            0xb
#define CLASS_WIDTH             0x1
#define BRIDGE_CLASS_TYPE       0x6

#define PCI_2_BUS               2
#define PCI_1_BUS               1

#define PCICMD_ADRS       (PCI_CFG_BASE + 0x04)  /* PCI cmd reg */
#define PCICMD_VAL        0x00000006             /* PCI COMMAND Default value */
#define PCISTAT_ADRS      (PCI_CFG_BASE + 0x06)  /* PCI status reg */

#define PCI_LAT_TIMER     0x40        /* latency timer value, 64 PCI clocks */

#define PCI1_DEV_ID       0x826010E3
#define PCI2_DEV_ID       0x826110E3
#define PCI3_DEV_ID       0x826210E3
#define PCI_DEV_ID_82XX   0x00031057  /* Id for MPC8266ADS-PCI board - Rev1 */
#define PCI_DEV_ID_85XX   0x00091057  /* Id for MPC85xxADS-PCI board - Rev2 */
#define PCI_ID_I82559     0x12298086  /* Id for Intel 82559 */
#define PCI_ID_I82559ER   0x12098086  /* Id for Intel 82559 ER */
#define PCI_ID_I82559_IB  0x10308086

#define PIB_PCI_IRQ       INUM_IRQ4

#define PCI_INTA_IRQ      PIB_PCI_IRQ
#define PCI_INTB_IRQ      PIB_PCI_IRQ
#define PCI_INTC_IRQ      PIB_PCI_IRQ
#define PCI_INTD_IRQ      PIB_PCI_IRQ

#define DELTA(a,b)        (sysAbs((int)a - (int)b))

#define BUS 0             /* bus-less board */

/* Board Status and Control Registers - unique to ADS */

#define BCSR_BASE_ADRS  0xF8000000          /* BCSR base address */
#define BCSRS_SIZE      0x00008000          /* 32K of address space */
#define BCSRS_MASK      ~(BCSRS_SIZE - 1)   /* set 32K mask for BCSRs */

#ifdef _ASMLANGUAGE

#define BCSR0       BCSR_BASE_ADRS          /* Register 0 */
#define BCSR1       BCSR_BASE_ADRS + 0x01   /* Register 1 */
#define BCSR2       BCSR_BASE_ADRS + 0x02   /* Register 2 */
#define BCSR3       BCSR_BASE_ADRS + 0x03   /* Register 3 */
#define BCSR4       BCSR_BASE_ADRS + 0x04   /* Register 4 */
#define BCSR5       BCSR_BASE_ADRS + 0x05   /* Register 5 */
#define BCSR6       BCSR_BASE_ADRS + 0x06   /* Register 6 */
#define BCSR7       BCSR_BASE_ADRS + 0x07   /* Register 7 */
#define BCSR8       BCSR_BASE_ADRS + 0x08   /* Register 8 */
#define BCSR9       BCSR_BASE_ADRS + 0x09   /* Register 9 */
#define BCSR10      BCSR_BASE_ADRS + 0x0a   /* Register 10 */
#define BCSR11      BCSR_BASE_ADRS + 0x0b   /* Register 11 */

#else

#define BCSR0   ((volatile uint8_t *)BCSR_BASE_ADRS)
#define BCSR1   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x01)
#define BCSR2   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x02)
#define BCSR3   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x03)
#define BCSR4   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x04)
#define BCSR5   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x05)
#define BCSR6   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x06)
#define BCSR7   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x07)
#define BCSR8   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x08)
#define BCSR9   (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x09)
#define BCSR10  (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x0a)
#define BCSR11  (((volatile uint8_t *)BCSR_BASE_ADRS) + 0x0b)

typedef union mds_bcsr 
    {
    UINT32 bcsr[12];
    } BCSR_REG;

#endif  /* _ASMLANGUAGE */

#define BCSR0_GETH0_EN    0x80
#define BCSR0_GETH1_EN    0x40
#define BCSR0_GETH_RST    0x20
#define BCSR0_RS232_EN    0x10
#define BCSR0_BOOTWP      0x08 /* protect is low */
#define BCSR0_SIGNAL0     0x04
#define BCSR0_SIGNAL1     0x02
#define BCSR0_SPARE       0x01

#define BCSR1_FLASH_PRT   0x01
#define BCSR0_LED_ON      0
#define BCSR0_LED_OFF     1

/* CPU type in the PVR */

#define CPU_TYPE_8260                   0xAAAA      /* value for PPC8260 */
#define CPU_TYPE_8266                   0xBBBB      /* value for PPC8266 */
#define CPU_REV_A1_MASK_NUM             0x0010      /* revision mask num */
#define HIP4_ID                         0x80810000  /* device ID via PVR */
#define HIP4_MASK                       0xFFFF0000  /* mask upper word   */
#define HIP7_ID                         0x80820000

/* Default Values */

#define SPCR_DEFAULT_VAL                0x13020000
#define SICRL_DEFAULT_VAL               0x80000000
#define SICRH_DEFAULT_VAL               0x00000003

/* 66MHz PCI */
/* Must also flip CLKIN_DIV dip switch on board to 0 */

#define OCCR_DEFAULT_VAL                0xff000000
#define ATR_DEFAULT_VAL                 0x00ff00ff
#define AIDR_DEFAULT_VAL                0x0000003f
#define AERR_DEFAULT_VAL                0x00000000
#define AMR_DEFAULT_VAL                 0x00000007
#define ACR_DEFAULT_VAL                 0x00100000
#define AER_DEFAULT_VAL                 0x0000003f

/* romInit initialization values... */
/* Values for local bus windows before flash is remapped at powerup. */

#define LBLAWBAR0_BOOT_VAL              0x00000000

/* 2 GB */

#define LBLAWAR0_BOOT_VAL               (LAWAR_ENABLE|LAWAR_SZ_2GB)

#define LBLAWBAR1_BOOT_VAL              0x80000000

/* 2 GB */

#define LBLAWAR1_BOOT_VAL               (LAWAR_ENABLE|LAWAR_SZ_2GB)

#define LBLAWBAR2_BOOT_VAL              0x00000000
#define LBLAWAR2_BOOT_VAL               0x00000000

#define LBLAWBAR3_BOOT_VAL              0x00000000
#define LBLAWAR3_BOOT_VAL               0x00000000

#define BR0_DEFAULT_VAL                 (ROM_BASE_ADRS|0x1001)

#define OR0_DEFAULT_VAL                 0xff806ff7

#define LBLAWBAR0_DEFAULT_VAL           ROM_BASE_ADRS

/* 8MB */

#define LBLAWAR0_DEFAULT_VAL            (LAWAR_ENABLE|LAWAR_SZ_8MB)

#define LBLAWBAR1_DEFAULT_VAL           BCSR_BASE_ADRS 
#define LBLAWAR1_DEFAULT_VAL            (LAWAR_ENABLE|LAWAR_SZ_8KB)

#define PCILAWBAR0_DEFAULT_VAL          0x80000000

/* 256 MB */

#define PCILAWAR0_DEFAULT_VAL           0x8000001b

#define PCILAWBAR1_DEFAULT_VAL          0x90000000

/* 256 MB */

#define PCILAWAR1_DEFAULT_VAL           0x8000001b

#define BR1_DEFAULT_VAL                 (BCSR_BASE_ADRS|0x801)
#define OR1_DEFAULT_VAL                 0xffffe8f0

/* Hard Reset Configuration Words */
/* Low Word */
/* Byte 0 */

#define HRCW_LOW_LBIU_DIV2              0x80
#define HRCW_LOW_DDR_DIV2               0x40
#define HRCW_LOW_SPMF                   0x0f

/* Byte 1 */

#define HRCW_LOW_CORE_PLL               0xFF

/* byte 2 must be cleared */

/* byte 3 */

#define HRCW_LOW_CEVCOD_DIV4            0x00
#define HRCW_LOW_CEVCOD_DIV8            0x40
#define HRCW_LOW_CEVCOD_DIV2            0x80
#define HRCW_LOW_CEPDF                  0x20
#define HRCW_LOW_CEPMF_MASK             0x1f
#define HRCW_LOW_CEPMF_6                0x06

/* High Word */
/* Byte 0 */

#define HRCW_HIGH_PCI_HOST              0x80
#define HRCW_HIGH_PCI64                 0x40
#define HRCW_HIGH_PCI1_ARB              0x20
#define HRCW_HIGH_PCI_CLK_OUT           0x10
#define HRCW_HIGH_CORE_DIS              0x08
#define HRCW_HIGH_BMS_HIGH              0x04
#define HRCW_HIGH_BOOT_SEQ_I2C          0x01
#define HRCW_HIGH_BOOT_SEQ_EXT_I2C      0x02

/*Byte 1*/

#define HRCW_HIGH_SWEN                  0x80
#define HRCW_HIGH_ROMLOC                0x70
#define ROMLOC_DDR                      0x00
#define ROMLOC_PCI1                     0x10
#define ROMLOC_PCI2                     0x20
#define ROMLOC_GPCM_8BIT                0x50
#define ROMLOC_GPCM_16BIT               0x60
#define ROMLOC_GPCM_32BIT               0x70


/*Byte 2*/
#define HRCW_HIGH_TSEC1M                0xc0
#define HRCW_HIGH_TSEC2M                0x30

#define HRCW_HIGH_TSEC12M_GMII          0xa0

/*Byte 3*/
#define HRCW_HIGH_TLE                   0x40

#ifdef PQ2PRO
#   define INUM_UART1        9
#   define INUM_UART2        10
#   define INUM_SEC          11
#   define INUM_I2C1         14
#   define INUM_I2C2         15
#   define INUM_SPI          16
#   define INUM_IRQ1         17
#   define INUM_IRQ2         18
#   define INUM_IRQ3         19
#   define INUM_IRQ4         20
#   define INUM_IRQ5         21
#   define INUM_IRQ6         22
#   define INUM_IRQ7         23
#   define INUM_TSEC1_TX     32
#   define INUM_TSEC1_RX     33
#   define INUM_TSEC1_ERR    34
#   define INUM_TSEC2_TX     35
#   define INUM_TSEC2_RX     36
#   define INUM_TSEC2_ERR    37
#   define INUM_USB2_DR      38
#   define INUM_USB2_MPH     39
#   define INUM_IRQ0         48
#   define INUM_RTC_SEC      64
#   define INUM_PIT          65
#   define INUM_PCI1         66
#   define INUM_PCI2         67
#   define INUM_RTC_ALR      68 
#   define INUM_MU           69
#   define INUM_SBA          70
#   define INUM_DMA          71
#   define INUM_GTM4         72
#   define INUM_GTM8         73
#   define INUM_GPIO1        74
#   define INUM_GPIO2        75
#   define INUM_DDR          76
#   define INUM_LBC          77
#   define INUM_GTM2         78
#   define INUM_GTM6         79
#   define INUM_PMC          80
#   define INUM_GTM3         84
#   define INUM_GTM7         85
#   define INUM_GTM1         90
#   define INUM_GTM5         91
#endif /* PQ2PRO */

#ifdef PQ2PRO_QE
#   define INUM_UART1        9
#   define INUM_UART2        10
#   define INUM_SEC          11
#   define INUM_I2C1         14
#   define INUM_I2C2         15
#   define INUM_IRQ1         17
#   define INUM_IRQ2         18
#   define INUM_IRQ3         19
#   define INUM_IRQ4         20
#   define INUM_IRQ5         21
#   define INUM_IRQ6         22
#   define INUM_IRQ7         23
#   define INUM_QE_HIGH      32
#   define INUM_QE_LOW       33
#   define INUM_IRQ0         48
#   define INUM_RTC_SEC      64
#   define INUM_PIT          65
#   define INUM_PCI1         66
#   define INUM_RTC_ALR      68
#   define INUM_MU           69
#   define INUM_SBA          70
#   define INUM_DMA          71
#   define INUM_GTM4         72
#   define INUM_GTM8         73
#   define INUM_QE_PORTS     74
#   define INUM_SDDR         75
#   define INUM_DDR          76
#   define INUM_LBC          77
#   define INUM_GTM2         78
#   define INUM_GTM6         79
#   define INUM_PMC          80
#   define INUM_GTM3         84
#   define INUM_GTM7         85
#   define INUM_GTM1         90
#   define INUM_GTM5         91
#endif /* PQ2PRO_QE */

#define QE_INUM_ERROR        0
#define QE_INUM_SPI2         1
#define QE_INUM_SPI1         2
#define QE_INUM_RTT          3
#define QE_INUM_SDMA         10
#define QE_INUM_USB          11
#define QE_INUM_TIMER1       12
#define QE_INUM_TIMER2       13
#define QE_INUM_TIMER3       14
#define QE_INUM_TIMER4       15
#define QE_INUM_VT           20
#define QE_INUM_EXT1         25
#define QE_INUM_EXT2         26
#define QE_INUM_EXT3         27
#define QE_INUM_EXT4         28
#define QE_INUM_UCC1         32
#define QE_INUM_UCC2         33
#define QE_INUM_UCC3         34
#define QE_INUM_UCC4         35
#define QE_INUM_MCC1         36
#define QE_INUM_UCC5         40
#define QE_INUM_UCC6         41
#define QE_INUM_UCC7         42
#define QE_INUM_UCC8         43

#ifndef _ASMLANGUAGE

#define COM1_LOCAL_MEM_BASE     0x4500    /* com1 local mem offset */
#define COM2_LOCAL_MEM_BASE     0x4600    /* com2 local mem offset */
#define COM1_ADR       ((ULONG) CCSBAR + (ULONG) COM1_LOCAL_MEM_BASE)
#define COM2_ADR       ((ULONG) CCSBAR + (ULONG) COM2_LOCAL_MEM_BASE)
#define DUART_REG_ADDR_INTERVAL 1         /* duart vector register distance */

/* function declarations */
extern uint32_t sysBaudClkFreq(void);
extern void     sysMsDelay(uint32_t);
extern void     sysDelay(void);
extern uint32_t sysAbs(int);
extern uint32_t sysDecGet(void);
extern UINT32   vxHid2Get();
extern void     vxHid2Set(UINT32);
extern UINT32   vxSvrGet();
#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INCfslmpc8360rdkh */
