/* fsl_rdb8313.h - Freescale MPC8313ERDB board header */

/*
 * Copyright (c) 2007-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,04aug10,liu  clear mot83xxPci.h. (WIND00212294)
01e,09sep09,s_z  Integrate the USB TCD support by INCLUDE_FSL_TCD(WIND00180575)
01d,07may09,wap  Add corrected eTSEC interrupt mappings for rev 2 silicon
01c,03mar08,z_l  updated to vxWorks6.6
01b,18sep07,l_g  add support of nand boot
01a,26jun07,l_g  initial creation, based on ads834x
*/

/*
DESCRIPTION
This file contains I/O addresses and related constants for the
Freescale MPC8313ERDB board.
*/

#ifndef __INCfsl_rdb8313h
#define __INCfsl_rdb8313h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef  _ASMLANGUAGE
#   if (defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))

    /* nothing needed */

#   else  /* _WRS_VXWORKS_MAJOR && ... */
    typedef void * VIRT_ADDR;
    typedef void * PHYS_ADDR;
#   endif  /* _WRS_VXWORKS_MAJOR && ...*/
#endif  /* !_ASMLANGUAGE */

#ifndef M8260ABBREVIATIONS
#   define M8260ABBREVIATIONS

#   ifdef  _ASMLANGUAGE
#       define CAST(x)
#   else /* _ASMLANGUAGE */
        typedef volatile UCHAR  VCHAR;      /* shorthand for volatile UCHAR */
        typedef volatile INT32  VINT32;     /* volatile unsigned word */
        typedef volatile INT16  VINT16;     /* volatile unsigned halfword */
        typedef volatile INT8   VINT8;      /* volatile unsigned byte */
        typedef volatile UINT32 VUINT32;    /* volatile unsigned word */
        typedef volatile UINT16 VUINT16;    /* volatile unsigned halfword */
        typedef volatile UINT8  VUINT8;     /* volatile unsigned byte */
#       define CAST(x) (x)
#   endif  /* _ASMLANGUAGE */

#endif /* !M8260ABBREVIATIONS */

#define N_SIO_CHANNELS      2       /* No. serial I/O channels */

#undef  BOOT_LINE_SIZE
#define BOOT_LINE_SIZE      256
#define NV_RAM_SIZE_WRITEABLE NV_RAM_SIZE  
#define MAX_MAC_ADRS        2
#define MAC_ADRS_LEN        6

/* WR specific portion of MAC (MSB->LSB) */

#define WR_ENET0            0x00  
#define WR_ENET1            0xA0
#define WR_ENET2            0x1E

#define CUST_ENET3_0        0xA0    /* Customer portion of MAC address */
#define CUST_ENET3_1        0xA1
#define CUST_ENET4          0xAA
#define CUST_ENET5          0xA0

/* Base Address of Memory Mapped Registers */

#define CCSBAR_INIT         0xFF400000
#define CCSBAR              0xE0000000
#define CCSBAR_SIZE         0x100000    /* 1MB */

/* Register Set Base addresses */

#define PQ2PRO

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
#define DDR_REG_BA          0x02000
#define LBC_REG_BA          0x05000
#define DMA_REG_BA          0x08100
#define PCICFG_REG_BA       0x08300
#define PCI2CFG_REG_BA      0x08380
#define SEC_REG_BA          0x101B4

#define DMA_NUM 4

#include <hwif/intCtlr/vxbQuiccIntCtlr.h>
#include <drv/mem/quiccLbc.h>

/* System Configuration Control */

#define M83XX_SGPRL(base)   (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x00 ))
#define M83XX_SGPRH(base)   (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x04 ))
#define M83XX_SPRIDR(base)  (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x08 ))
                            
#define M83XX_SPCR(base)    (CAST(VUINT32 *)((base) +  SCCFG_REG_BA + 0x10 ))
#define M83XX_SPCR_OPT      0x00800000
#define M83XX_SPCR_TBEN     0x00400000
                            
#define M83XX_SICRL(base)   (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x14 ))
#define M83XX_SICRH(base)   (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x18 ))
                            
#define M83XX_PECR1(base)   (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x40 ))
#define M83XX_PECR2(base)   (CAST(VUINT32 *)((base) + SCCFG_REG_BA + 0x44 ))
                            
/* Watch Dog Registers */   
                            
#define M83XX_SWCRR(base)   (CAST(VUINT32 *)((base) + WDT_REG_BA + 0x04 ))
#define M83XX_SWCNR(base)   (CAST(VUINT32 *)((base) + WDT_REG_BA + 0x08 ))
#define M83XX_SWSRR(base)   (CAST(VUINT16 *)((base) + WDT_REG_BA + 0x0E ))

/* Clock Module */

#define M83XX_SPMR(base)    (CAST(VUINT32 *)((base) + CLKM_REG_BA + 0x00 ))
#define M83XX_SPMR_LBIU_VAL(spmrVal)  (spmrVal >> 31)
#define M83XX_SPMR_DDR_VAL(spmrVal)  ((spmrVal >> 30) & 0x1)
#define M83XX_SPMR_SPMF_VAL(spmrVal) ((spmrVal >> 24) & 0xf)
#define M83XX_SPMR_CLK_DIV(spmrVal)  ((spmrVal >> 23) & 0x1)
#define M83XX_SPMR_CLK_PLL(spmrVal)  ((spmrVal >> 16) & 0x7f)

#define M83XX_OCCR(base)    (CAST(VUINT32 *)((base) + CLKM_REG_BA + 0x04 ))
#define M83XX_SCCR(base)    (CAST(VUINT32 *)((base) + CLKM_REG_BA + 0x08 ))

#define M83XX_SCCR_RES_MSK  0xf3f10000      /* reserved field Mask */

/* Arbiter registers */

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

/* I2C1 Controller */

#define  M8313_I2C1_BASE    0x03000 

#define  I2C1ADR            0x03000
#define  I2C1FDR            0x03004
#define  I2C1ICR            0x03008
#define  I2C1ISR            0x0300c
#define  I2C1IDR            0x03010
#define  I2C1DFSRR          0x03014

/* I2C2 Controller */

#define  M8313_I2C2_BASE    0x03100 

#define  I2C2ADR            0x03100
#define  I2C2FDR            0x03104
#define  I2C2ICR            0x03108
#define  I2C2ISR            0x0310c
#define  I2C2IDR            0x03110
#define  I2C2DFSRR          0x03114


/* Base Address of Memory Mapped Registers */
#define M83XX_DR_CCSBAR                 (CCSBAR)

/* USB register memory map */
#define M83XX_DR_USB_BASE               (CCSBAR | 0x23000)


/* USB DR (Host) registers */

/* These are little-endian */

#define  M83XX_CAPLENGTH(base)           (CAST(VUINT8  *)((base) | 0x23100))
#define  M83XX_HCIVERSION(base)          (CAST(VUINT16 *)((base) | 0x23102))
#define  M83XX_HCSPARAMS_L(base)         (CAST(VUINT32 *)((base) | 0x23104))
#define  M83XX_HCSPARAMS_H(base)         (CAST(VUINT32 *)((base) | 0x23108))

#define  M83XX_USBCMD(base)              (CAST(VUINT32 *)((base) | 0x23140))
#define  M83XX_USBSTS(base)              (CAST(VUINT32 *)((base) | 0x23144))
#define  M83XX_USBINTR(base)             (CAST(VUINT32 *)((base) | 0x23148))
#define  M83XX_FRINDEX(base)             (CAST(VUINT32 *)((base) | 0x2314c))
#define  M83XX_PERIODICLISTBASE(base)    (CAST(VUINT32 *)((base) | 0x23154))
#define  M83XX_ASYNCLISTADDR(base)       (CAST(VUINT32 *)((base) | 0x23158))
#define  M83XX_ASYNCTTSTS(base)          (CAST(VUINT32 *)((base) | 0x2315c))
#define  M83XX_BURSTSIZE(base)           (CAST(VUINT32 *)((base) | 0x23160))
#define  M83XX_TXTTFILLTUNING_L(base)    (CAST(VUINT32 *)((base) | 0x23164))
#define  M83XX_TXTTFILLTUNING_H(base)    (CAST(VUINT32 *)((base) | 0x23168))
#define  M83XX_CONFIGFLAG(base)          (CAST(VUINT32 *)((base) | 0x23180))
#define  M83XX_PORTSC1(base)             (CAST(VUINT32 *)((base) | 0x23184))
#define  M83XX_PORTSC2(base)             (CAST(VUINT32 *)((base) | 0x23188))
#define  M83XX_USBMODE(base)             (CAST(VUINT32 *)((base) | 0x231a8))

/* These are big-endian */

#define  M83XX_SNOOP1(base)              (CAST(VUINT32 *)((base) | 0x23400))
#define  M83XX_SNOOP2(base)              (CAST(VUINT32 *)((base) | 0x23404))
#define  M83XX_AGE_CNT_THRESH(base)      (CAST(VUINT32 *)((base) | 0x23408))
#define  M83XX_PRI_CTRL(base)            (CAST(VUINT32 *)((base) | 0x2340c))
#define  M83XX_SI_CTRL(base)             (CAST(VUINT32 *)((base) | 0x23410))
#define  M83XX_CONTROL(base)             (CAST(VUINT32 *)((base) | 0x23500))

/* Local address windows */

#define M83XX_LBLAWBARn(base,n)     (CAST(VUINT32 *)((base) + 0x20 + (n*0x8)))
#define M83XX_LBLAWARn(base,n)      (CAST(VUINT32 *)((base) + 0x24 + (n*0x8)))
#define M83XX_PCILAWBARn(base,n)    (CAST(VUINT32 *)((base) + 0x60 + (n*0x8)))
#define M83XX_PCILAWARn(base,n)     (CAST(VUINT32 *)((base) + 0x64 + (n*0x8)))
#define M83XX_DDRLAWBARn(base,n)    (CAST(VUINT32 *)((base) + 0xA0 + (n*0x8)))
#define M83XX_DDRLAWARn(base,n)     (CAST(VUINT32 *)((base) + 0xA4 + (n*0x8)))
#define M8313_DDRCDR(base)          (CAST(VUINT32 *)((base) + 0x128))

#define LAWBAR_ADRS_SHIFT       0
#define LAWAR_ENABLE            0x80000000

/* LAWAR SIZE Settings */

#define LAWAR_SIZE_4KB          0x0000000B
#define LAWAR_SIZE_8KB          0x0000000C
#define LAWAR_SIZE_16KB         0x0000000D
#define LAWAR_SIZE_32KB         0x0000000E
#define LAWAR_SIZE_64KB         0x0000000F
#define LAWAR_SIZE_128KB        0x00000010
#define LAWAR_SIZE_256KB        0x00000011
#define LAWAR_SIZE_512KB        0x00000012
#define LAWAR_SIZE_1MB          0x00000013
#define LAWAR_SIZE_2MB          0x00000014
#define LAWAR_SIZE_4MB          0x00000015
#define LAWAR_SIZE_8MB          0x00000016
#define LAWAR_SIZE_16MB         0x00000017
#define LAWAR_SIZE_32MB         0x00000018
#define LAWAR_SIZE_64MB         0x00000019
#define LAWAR_SIZE_128MB        0x0000001A
#define LAWAR_SIZE_256MB        0x0000001B
#define LAWAR_SIZE_512MB        0x0000001C
#define LAWAR_SIZE_1GB          0x0000001D
#define LAWAR_SIZE_2GB          0x0000001E

/* PCI Window SIZE  */ 

#define PCI_SIZE_4KB            0x0000000B
#define PCI_SIZE_8KB            0x0000000C
#define PCI_SIZE_16KB           0x0000000D
#define PCI_SIZE_32KB           0x0000000E
#define PCI_SIZE_64KB           0x0000000F
#define PCI_SIZE_128KB          0x00000010
#define PCI_SIZE_256KB          0x00000011
#define PCI_SIZE_512KB          0x00000012
#define PCI_SIZE_1MB            0x00000013
#define PCI_SIZE_2MB            0x00000014
#define PCI_SIZE_4MB            0x00000015
#define PCI_SIZE_8MB            0x00000016
#define PCI_SIZE_16MB           0x00000017
#define PCI_SIZE_32MB           0x00000018
#define PCI_SIZE_64MB           0x00000019
#define PCI_SIZE_128MB          0x0000001A
#define PCI_SIZE_256MB          0x0000001B
#define PCI_SIZE_512MB          0x0000001C
#define PCI_SIZE_1GB            0x0000001D
#define PCI_SIZE_2GB            0x0000001E

/* Offsets for DDR registers */

#define DDRBA                   (CCSBAR + DDR_REG_BA)
#define CS0_BNDS                0x000
#define CS1_BNDS                0x008
#define CS2_BNDS                0x010
#define CS3_BNDS                0x018
#define CS0_CONFIG              0x080
#define CS1_CONFIG              0x084
#define CS2_CONFIG              0x088
#define CS3_CONFIG              0x08C
                                
#define TIMING_CFG_0            0x104
#define TIMING_CFG_1            0x108
#define TIMING_CFG_2            0x10C
#define TIMING_CFG_3            0x100

#define DDR_SDRAM_CFG           0x110
#define DDR_SDRAM_CFG_2         0x114
#define DDR_SDRAM_MODE_CFG      0x118
#define DDR_SDRAM_MODE_CFG_2    0x11C
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
          
#define LOCAL2PCI_MEMIO(x)  ((int)(x)    + PCI_MSTR_MEM_BUS)

/* PCI defines begin */

/* configuration space reg and int ack */

#define PCI_REG_BASE            0x8000

#define PCI_CFG_ADR_REG         (CCSBAR + PCICFG_REG_BA + 0x00)
#define PCI_CFG_DATA_REG        (CCSBAR + PCICFG_REG_BA + 0x04)
#define PCI_INT_ACK             (CCSBAR + PCICFG_REG_BA + 0x08)
                                
#define PCI2_CFG_ADR_REG        (CCSBAR + PCI2CFG_REG_BA + 0x00)
#define PCI2_CFG_DATA_REG       (CCSBAR + PCI2CFG_REG_BA + 0x04)
#define PCI2_INT_ACK            (CCSBAR + PCI2CFG_REG_BA + 0x08)
                                
#define PCI_AUTO_CONFIG_ADRS    0x4c00

#define PPCACR_PRKM_MASK        0XF0
#define PCI_REQUEST_LEVEL       0x3
                                
#define CLASS_OFFSET            0xB
#define CLASS_WIDTH             0x1
#define BRIDGE_CLASS_TYPE       0x6

#define PCI_2_BUS               2
#define PCI_1_BUS               1

#define NUM_PCI_SLOTS           0x1     /* 3 PCI slots: 0 to 2 */

#define PCI_XINT1_LVL           0x0     /* PCI XINT1 routed to IRQ0  */
#define PCI_XINT2_LVL           0x1     /* PCI XINT2 routed to IRQ1 */
#define PCI_XINT3_LVL           0x2     /* PCI XINT3 routed to IRQ2 */
#define PCI_XINT4_LVL           0x3     /* PCI XINT3 routed to IRQ2 */

#define PCI_SLOT1_DEVNO         0x10    /* PCI SLOT 1 Device no */

#define PCI_LAT_TIMER           0x40    /* latency timer value, 64 PCI clocks */

#define PCI1_DEV_ID             0x826010E3
#define PCI2_DEV_ID             0x826110E3
#define PCI3_DEV_ID             0x826210E3
#define PCI_ID_I82559           0x12298086      /* Id for Intel 82559 */
#define PCI_ID_I82559ER         0x12098086      /* Id for Intel 82559 ER */
#define PCI_ID_I82559_IB        0x10308086

#define PIB_PCI_IRQ             INUM_IRQ4

#define PCI_INTA_IRQ            PIB_PCI_IRQ
#define PCI_INTB_IRQ            PIB_PCI_IRQ
#define PCI_INTC_IRQ            PIB_PCI_IRQ
#define PCI_INTD_IRQ            PIB_PCI_IRQ

#define PCI_LAWAR_ENABLE        0x80000000
#define PCI_ATTR_IO_BIT         0x40000000

#define PCI_IN_ATTR_WS_256M     0x0000001b

#define PCI_OUT_ATTR_WS_128M    0x000F8000
#define PCI_OUT_ATTR_WS_64M     0x000FC000

#define PCI_LAWAR_SIZE_256MB    0x0000001B

#define DELTA(a,b)              (sysAbs((int)a - (int)b))

#define BUS                     0               /* bus-less board */

/* Enhanced Local Bus Register */

#define M8313_LSOR(base)   (CAST(volatile unsigned long *)((base) | 0x5090))
#define M8313_LTESR(base)  (CAST(volatile unsigned long *)((base) | 0x50b0))
#define M8313_LTEDR(base)  (CAST(volatile unsigned long *)((base) | 0x50b4))
#define M8313_LTEIR(base)  (CAST(volatile unsigned long *)((base) | 0x50b8))
#define M8313_LTEATR(base) (CAST(volatile unsigned long *)((base) | 0x50bc))
#define M8313_LTEAR(base)  (CAST(volatile unsigned long *)((base) | 0x50c0))

#define FCM_COMMAND_COMPLETE   0x00000001
#define FCM_ECC_ERROR          0x20000000

/* Flash Control Machine Register */

#define M8313_FMR(base)    (CAST(volatile unsigned long *)((base) | 0x50e0))
#define M8313_FIR(base)    (CAST(volatile unsigned long *)((base) | 0x50e4))
#define M8313_FCR(base)    (CAST(volatile unsigned long *)((base) | 0x50e8))
#define M8313_FBAR(base)   (CAST(volatile unsigned long *)((base) | 0x50ec))
#define M8313_FPAR(base)   (CAST(volatile unsigned long *)((base) | 0x50f0))
#define M8313_FBCR(base)   (CAST(volatile unsigned long *)((base) | 0x50f4))

/* Define CS */

/* Chip Select 3 for 8313rdb board, used for LED */

#define CS_LED                  0xFA000000

/* Chip Select 2 for 8313rdb board, used for VSC7385 */

#define CS_VSC7385              0xF0000000
#define CS_VSC7385_SIZE         0x20000

/* DS1339 RTC addr */

#define I2C_RTC_ADDR            0x68    /* at address 0x68 */

#ifdef PQ2PRO
#define INUM_UART1               9
#define INUM_UART2              10
#define INUM_SEC                11
#define INUM_I2C1               14
#define INUM_I2C2               15
#define INUM_SPI                16
#define INUM_IRQ1               17
#define INUM_IRQ2               18
#define INUM_IRQ3               19
#define INUM_IRQ4               20
#define INUM_IRQ5               21
#define INUM_IRQ6               22
#define INUM_IRQ7               23
#define INUM_TSEC2_ERR          32
#define INUM_TSEC2_RX           33
#define INUM_TSEC2_TX           34
#define INUM_TSEC1_ERR          35
#define INUM_TSEC1_RX           36
#define INUM_TSEC1_TX           37
#define INUM_REV2_TSEC1_TX      32
#define INUM_REV2_TSEC1_RX      33
#define INUM_REV2_TSEC1_ERR     34
#define INUM_REV2_TSEC2_TX      35
#define INUM_REV2_TSEC2_RX      36
#define INUM_REV2_TSEC2_ERR     37
#define INUM_USB2_DR            38
#define INUM_USB2_MPH           39
#define INUM_IRQ0               48
#define INUM_RTC_SEC            64
#define INUM_PIT                65
#define INUM_PCI1               66
#define INUM_PCI2               67
#define INUM_RTC_ALR            68 
#define INUM_MU                 69
#define INUM_SBA                70
#define INUM_DMA                71
#define INUM_GTM4               72
#define INUM_GTM8               73
#define INUM_GPIO1              74
#define INUM_GPIO2              75
#define INUM_DDR                76
#define INUM_LBC                77
#define INUM_GTM2               78
#define INUM_GTM6               79
#define INUM_PMC                80
#define INUM_GTM3               84
#define INUM_GTM7               85
#define INUM_GTM1               90
#define INUM_GTM5               91
#endif

#define COM1_LOCAL_MEM_BASE     0x4500    /* com1 local mem offset */
#define COM2_LOCAL_MEM_BASE     0x4600    /* com2 local mem offset */
#define COM1_ADR                ((ULONG) CCSBAR + (ULONG) COM1_LOCAL_MEM_BASE)
#define COM2_ADR                ((ULONG) CCSBAR + (ULONG) COM2_LOCAL_MEM_BASE)
#define DUART_REG_ADDR_INTERVAL 1         /* duart vector register distance */

#define PVR_E300C3              0x80850000
#define SVR_MPC8313E_0          0x80B00000
#define SVR_MPC8313E_1          0x80B10000

/* Outbound/Inbound Comparison mask register defines */
#define PCI_WINDOW_ENABLE_BIT           0x80000000
#define PCI_PREFETCHABLE                0x20000000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP   0x00050000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP  0x00005000


#ifndef _ASMLANGUAGE
/* function declarations */

    extern void     sysMsDelay  (uint32_t);
    extern void     sysDelay    (void);
    extern uint32_t sysAbs      (int);
    extern uint32_t sysDecGet   (void);
    extern UINT32   vxHid2Get   (void);
    extern void     vxHid2Set   (UINT32);
    extern UINT32   vxSvrGet    (void);
#endif /* !_ASMLANGUAGE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCfsl_rdb8313h */

