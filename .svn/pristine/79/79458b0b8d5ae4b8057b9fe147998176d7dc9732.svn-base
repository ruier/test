/* usbMpc8349.h - Definitions for MPC 8349 USB target controller */

/* Copyright 2007, 2009 Wind River Systems, Inc. */

/*
Modification history
--------------------
01c,09sep09,s_z  Integrate the USB TCD support by INCLUDE_FSL_TCD(WIND00180575)
01b,08jun07,j_l  Fix DMA
01a,27mar07,j_l  Created
*/

/*
DESCRIPTION

Defines constants related to the MPC 8349 USB device (target) IC.
*/

#ifndef __INCusbMpc8349h
#define __INCusbMpc8349h

#ifdef  __cplusplus
extern "C" {
#endif

/* includes */

#include "ads834x.h"

/* defines */


/* Base Address of Memory Mapped Registers */
#define MPC8349_DR_CCSBAR                   M834X_DR_CCSBAR

/* USB register memory map */
#define MPC8349_DR_USB_BASE                 (MPC8349_DR_CCSBAR | 0x23000)

/* Dual Role (DR) Interrupt */
#define MPC8349_DR_IRQ                      INUM_USB2_DR

/* Board Status and Control Registers (BCSR) - unique to ADS */
#define MPC8349_DR_BCSR5                    BCSR5 

/* BCSR Bits */
#define MPC8349_DR_BCSR5_INT_USB            0x02


/* MPC 8349 Endpoint Index */
#define MPC8349_DR_ENDPT_0                  0x0  /* Control Endpoint  */
#define MPC8349_DR_ENDPT_1                  0x1  /* Endpoint 1 */
#define MPC8349_DR_ENDPT_2                  0x2  /* Endpoint 2 */
#define MPC8349_DR_ENDPT_3                  0x3  /* Endpoint 3 */
#define MPC8349_DR_ENDPT_4                  0x4  /* Endpoint 4 */
#define MPC8349_DR_ENDPT_5                  0x5  /* Endpoint 5 */

#define MPC8349_DR_NUM_ENDPOINTS_RX         0x6
#define MPC8349_DR_NUM_ENDPOINTS_TX         0x6

/* number of MPC 8349 endpoints */
#define MPC8349_DR_NUM_ENDPOINTS            (MPC8349_DR_NUM_ENDPOINTS_RX+MPC8349_DR_NUM_ENDPOINTS_TX)

/* MPC 8349 I/O registers */


/* System Configuration Control */
#define MPC8349_DR_SICRL(base)               M83XX_SICRL(base)
#define MPC8349_DR_SICRH(base)               M83XX_SICRH(base)

/* SICRL Bits */
#define MPC8349_DR_SICRL_USB_PORT_0          0x40000000
#define MPC8349_DR_SICRL_USB_PORT_1          0x20000000

/*SICRH Bits */
#define MPC8349_DR_SICRH_TSEC2_F             0x00020000

/* Clock Module */
#define MPC8349_DR_SCCR(base)                M83XX_SCCR(base)

/* SCCR Bits */

#define MPC8349_DR_SCCR_USB_MPHCM_DRCM_MASK  0x00F00000
#define MPC8349_DR_SCCR_USB_MPHCM_DRCM_DIS   0x00000000
#define MPC8349_DR_SCCR_USB_MPHCM_DRCM_1_1   0x00500000
#define MPC8349_DR_SCCR_USB_MPHCM_DRCM_1_2   0x00A00000
#define MPC8349_DR_SCCR_USB_MPHCM_DRCM_1_3   0x00F00000

/* USB DR registers */

/* These are little-endian */
#define MPC8349_DR_CAPLENGTH(base)           (CAST(VUINT8  *)((base) | 0x23100))
#define MPC8349_DR_HCIVERSION(base)          (CAST(VUINT16 *)((base) | 0x23102))
#define MPC8349_DR_HCSPARAMS_L(base)         (CAST(VUINT32 *)((base) | 0x23104))
#define MPC8349_DR_HCSPARAMS_H(base)         (CAST(VUINT32 *)((base) | 0x23108))
#define MPC8349_DR_DCIVERSION(base)          (CAST(VUINT32 *)((base) | 0x23120))
#define MPC8349_DR_DCCPARAMS(base)           (CAST(VUINT32 *)((base) | 0x23124))

#define MPC8349_DR_USBCMD(base)              (CAST(VUINT32 *)((base) | 0x23140))
#define MPC8349_DR_USBSTS(base)              (CAST(VUINT32 *)((base) | 0x23144))
#define MPC8349_DR_USBINTR(base)             (CAST(VUINT32 *)((base) | 0x23148))
#define MPC8349_DR_FRINDEX(base)             (CAST(VUINT32 *)((base) | 0x2314c))
#define MPC8349_DR_DEVICEADDR(base)          (CAST(VUINT32 *)((base) | 0x23154))
#define MPC8349_DR_ENDPOINTLISTADDR(base)    (CAST(VUINT32 *)((base) | 0x23158))
#define MPC8349_DR_BURSTSIZE(base)           (CAST(VUINT32 *)((base) | 0x23160))
#define MPC8349_DR_TXTTFILLTUNING(base)      (CAST(VUINT32 *)((base) | 0x23164))
#define MPC8349_DR_CONFIGFLAG(base)          (CAST(VUINT32 *)((base) | 0x23180))
#define MPC8349_DR_PORTSC1(base)             (CAST(VUINT32 *)((base) | 0x23184))
#define MPC8349_DR_OTGSC(base)               (CAST(VUINT32 *)((base) | 0x231a4))
#define MPC8349_DR_USBMODE(base)             (CAST(VUINT32 *)((base) | 0x231a8))
#define MPC8349_DR_ENDPTSETUPSTAT(base)      (CAST(VUINT32 *)((base) | 0x231ac))
#define MPC8349_DR_ENDPTPRIME(base)          (CAST(VUINT32 *)((base) | 0x231b0))
#define MPC8349_DR_ENDPTFLUSH(base)          (CAST(VUINT32 *)((base) | 0x231b4))
#define MPC8349_DR_ENDPTSTATUS(base)         (CAST(VUINT32 *)((base) | 0x231b8))
#define MPC8349_DR_ENDPTCOMPLETE(base)       (CAST(VUINT32 *)((base) | 0x231bc))
#define MPC8349_DR_ENDPTCTRL(base,endpoint)  (CAST(VUINT32 *)((base) | (0x231c0+(sizeof(UINT32)*endpoint))))
#define MPC8349_DR_ENDPTCTRL0(base)          (CAST(VUINT32 *)((base) | 0x231c0))
#define MPC8349_DR_ENDPTCTRL1(base)          (CAST(VUINT32 *)((base) | 0x231c4))
#define MPC8349_DR_ENDPTCTRL2(base)          (CAST(VUINT32 *)((base) | 0x231c8))
#define MPC8349_DR_ENDPTCTRL3(base)          (CAST(VUINT32 *)((base) | 0x231ca))
#define MPC8349_DR_ENDPTCTRL4(base)          (CAST(VUINT32 *)((base) | 0x231d0))
#define MPC8349_DR_ENDPTCTRL5(base)          (CAST(VUINT32 *)((base) | 0x231d4))

#define MPC8349_DR_USBCMD_RS                 0x00000001 /* Run/Stop */
#define MPC8349_DR_USBCMD_RST                0x00000002 /* Controller Reset */
#define MPC8349_DR_USBCMD_SUTW               0x00002000 /* Setup tripwire */

#define MPC8349_DR_USBSTS_UI                 0x00000001 /* USB interrupt */
#define MPC8349_DR_USBSTS_UEI                0x00000002 /* USB error interrupt */
#define MPC8349_DR_USBSTS_PCI                0x00000004 /* port change interrupt */
#define MPC8349_DR_USBSTS_SEI                0x00000010 /* system error interrupt */
#define MPC8349_DR_USBSTS_URI                0x00000040 /* USB reset interrupt */
#define MPC8349_DR_USBSTS_SRI                0x00000080 /* SOF received interrupt */
#define MPC8349_DR_USBSTS_SLI                0x00000100 /* DCSuspend interrupt */
#define MPC8349_DR_USBSTS_ULPII              0x00000400 /* ULPI interrupt */

#define MPC8349_DR_USBINTR_UE                0x00000001 /* USB interrupt enable */
#define MPC8349_DR_USBINTR_UEE               0x00000002 /* USB error interrupt enable */
#define MPC8349_DR_USBINTR_PCE               0x00000004 /* port change detect interrupt enable */
#define MPC8349_DR_USBINTR_SEE               0x00000010 /* system error interrupt enable */
#define MPC8349_DR_USBINTR_URE               0x00000040 /* USB reset interrupt enable */
#define MPC8349_DR_USBINTR_SRE               0x00000080 /* SOF receive interrupt enable */
#define MPC8349_DR_USBINTR_SLE               0x00000100 /* sleep interrupt enable */
#define MPC8349_DR_USBINTR_ULPIE             0x00000400 /* ULPI interrupt enable */
#define MPC8349_DR_USBINTR_IRQS            ( \
                                             MPC8349_DR_USBINTR_UEE   \
                                        /* | MPC8349_DR_USBINTR_UE    *//* enabled separately */ \
                                           | MPC8349_DR_USBINTR_PCE   \
                                           | MPC8349_DR_USBINTR_SEE   \
                                           | MPC8349_DR_USBINTR_URE   \
                                        /* | MPC8349_DR_USBINTR_SRE   */ \
                                           | MPC8349_DR_USBINTR_SLE   \
                                        /* | MPC8349_DR_USBINTR_ULPIE */ \
                                           )

#define MPC8349_DR_FRINDEX_MASK              0x00003fff /* FRINDEX mask */

#define MPC8349_DR_DEVICEADDR_USBADR_SHIFT   25         /* Device address starts at bit 25 */

#define MPC8349_DR_PORTSC1_PTS_MASK          0xc0000000 /* Port tranceiver select */
#define MPC8349_DR_PORTSC1_PTS_UTMI          0x00000000 /* Port tranceiver select - UTMI */
#define MPC8349_DR_PORTSC1_PTS_RESERVED      0x40000000 /* Port tranceiver select - reserved */
#define MPC8349_DR_PORTSC1_PTS_ULPI          0x80000000 /* Port tranceiver select - ULPI */
#define MPC8349_DR_PORTSC1_PTS_SERIAL        0xc0000000 /* Port tranceiver select - FS/LS serial */
#define MPC8349_DR_PORTSC1_PSPD_MASK         0x0c000000 /* Port speed */
#define MPC8349_DR_PORTSC1_PSPD_FS           0x00000000 /* Port speed - full speed */
#define MPC8349_DR_PORTSC1_PSPD_LS           0x04000000 /* Port speed - low speed */
#define MPC8349_DR_PORTSC1_PSPD_HS           0x08000000 /* Port speed - high speed */
#define MPC8349_DR_PORTSC1_PTC_MASK          0x000f0000 /* Port test control - mask */
#define MPC8349_DR_PORTSC1_PTC_DISABLED      0x00000000 /* Port test control - disable */
#define MPC8349_DR_PORTSC1_PTC_J_STATE       0x00010000 /* Port test control - J_STATE */
#define MPC8349_DR_PORTSC1_PTC_K_STATE       0x00020000 /* Port test control - K_STATE */
#define MPC8349_DR_PORTSC1_PTC_SEQ_NAK       0x00030000 /* Port test control - SEQ_NAK */
#define MPC8349_DR_PORTSC1_PTC_PACKET        0x00040000 /* Port test control - PACKET  */
#define MPC8349_DR_PORTSC1_PTC_FORCE_ENABLE  0x00050000 /* Port test control - FORCE_ENABLE */
#define MPC8349_DR_PORTSC1_PR                0x00000100 /* Port reset */
#define MPC8349_DR_PORTSC1_FPR               0x00000040 /* Force port resume */

#define MPC8349_DR_OTGSC_VC                  0x00000002 /* VBUS charge */

#define MPC8349_DR_USBMODE_CM_IDLE           0x00000000 /* Controller mode - idle */
#define MPC8349_DR_USBMODE_CM_RESERVED       0x00000001 /* Controller mode - reserved */
#define MPC8349_DR_USBMODE_CM_DEVICE         0x00000002 /* Controller mode - device controller */
#define MPC8349_DR_USBMODE_CM_HOST           0x00000003 /* Controller mode - host controller */
#define MPC8349_DR_USBMODE_CM_SLOM           0x00000008 /* Setup lockout mode */

#define MPC8349_DR_ENDPTPRIME_TX_SHIFT       16         /* TX bit position */

#define MPC8349_DR_ENDPTFLUSH_TX_SHIFT       16         /* TX bit position */

#define MPC8349_DR_ENDPTCOMPLETE_ERCE_MASK   0x0000003f /* endpoint receive complete event */
#define MPC8349_DR_ENDPTCOMPLETE_ETCE_MASK   0x003f0000 /* endpoint transmit complete event */
#define MPC8349_DR_ENDPTCOMPLETE_TX_SHIFT    16         /* TX bit position */

#define MPC8349_DR_ENDPTCTRL_RXS             0x00000001 /* RX endpoint stall */
#define MPC8349_DR_ENDPTCTRL_RXD             0x00000002 /* RX endpoint data sink */
#define MPC8349_DR_ENDPTCTRL_RXT_MASK        0x0000000c /* RX endpoint type */
#define MPC8349_DR_ENDPTCTRL_RXT_CTRL        0x00000000 /* RX endpoint type - control */
#define MPC8349_DR_ENDPTCTRL_RXT_ISOCH       0x00000004 /* RX endpoint type - isochronous */
#define MPC8349_DR_ENDPTCTRL_RXT_BULK        0x00000008 /* RX endpoint type - bulk */
#define MPC8349_DR_ENDPTCTRL_RXT_INT         0x0000000c /* RX endpoint type - interrupt */
#define MPC8349_DR_ENDPTCTRL_RXI             0x00000020 /* RX data toggle inhibit */
#define MPC8349_DR_ENDPTCTRL_RXR             0x00000040 /* RX data toggle reset */
#define MPC8349_DR_ENDPTCTRL_RXE             0x00000080 /* RX endpoint enable */
#define MPC8349_DR_ENDPTCTRL_RX_MASK         0x000000ff /* RX mask */
#define MPC8349_DR_ENDPTCTRL_TXS             0x00010000 /* TX endpoint stall */
#define MPC8349_DR_ENDPTCTRL_TXD             0x00020000 /* TX endpoint data sink */
#define MPC8349_DR_ENDPTCTRL_TXT_MASK        0x000c0000 /* TX endpoint type */
#define MPC8349_DR_ENDPTCTRL_TXT_CTRL        0x00000000 /* TX endpoint type - control */
#define MPC8349_DR_ENDPTCTRL_TXT_ISOCH       0x00040000 /* TX endpoint type - isochronous */
#define MPC8349_DR_ENDPTCTRL_TXT_BULK        0x00080000 /* TX endpoint type - bulk */
#define MPC8349_DR_ENDPTCTRL_TXT_INT         0x000c0000 /* TX endpoint type - interrupt */
#define MPC8349_DR_ENDPTCTRL_TXI             0x00200000 /* TX data toggle inhibit */
#define MPC8349_DR_ENDPTCTRL_TXR             0x00400000 /* TX data toggle reset */
#define MPC8349_DR_ENDPTCTRL_TXE             0x00800000 /* TX endpoint enable */
#define MPC8349_DR_ENDPTCTRL_TX_MASK         0x00ff0000 /* TX mask */
#define MPC8349_DR_ENDPTCTRL_TX_SHIFT        16         /* TX bit position */

/* These are big-endian */
#define MPC8349_DR_SNOOP1(base)              (CAST(VUINT32 *)((base) | 0x23400))
#define MPC8349_DR_SNOOP2(base)              (CAST(VUINT32 *)((base) | 0x23404))
#define MPC8349_DR_AGE_CNT_THRESH(base)      (CAST(VUINT32 *)((base) | 0x23408))
#define MPC8349_DR_PRI_CTRL(base)            (CAST(VUINT32 *)((base) | 0x2340c))
#define MPC8349_DR_SI_CTRL(base)             (CAST(VUINT32 *)((base) | 0x23410))
#define MPC8349_DR_CONTROL(base)             (CAST(VUINT32 *)((base) | 0x23500))

#define MPC8349_DR_SNOOP_ADDRESS_MASK        0xfffff000
#define MPC8349_DR_SNOOP_DISABLE             0x00000000
#define MPC8349_DR_SNOOP_4KB                 0x0000000b
#define MPC8349_DR_SNOOP_8KB                 0x0000000c
#define MPC8349_DR_SNOOP_16KB                0x0000000d
#define MPC8349_DR_SNOOP_32KB                0x0000000e
#define MPC8349_DR_SNOOP_64KB                0x0000000f
#define MPC8349_DR_SNOOP_128KB               0x00000010
#define MPC8349_DR_SNOOP_256KB               0x00000011
#define MPC8349_DR_SNOOP_512KB               0x00000012
#define MPC8349_DR_SNOOP_1MB                 0x00000013
#define MPC8349_DR_SNOOP_2MB                 0x00000014
#define MPC8349_DR_SNOOP_4MB                 0x00000015
#define MPC8349_DR_SNOOP_8MB                 0x00000016
#define MPC8349_DR_SNOOP_16MB                0x00000017
#define MPC8349_DR_SNOOP_32MB                0x00000018
#define MPC8349_DR_SNOOP_64MB                0x00000019
#define MPC8349_DR_SNOOP_31MB                0x0000001a
#define MPC8349_DR_SNOOP_256MB               0x0000001b
#define MPC8349_DR_SNOOP_512MB               0x0000001c
#define MPC8349_DR_SNOOP_1GB                 0x0000001d
#define MPC8349_DR_SNOOP_2GB                 0x0000001e

#define MPC8349_DR_CONTROL_USB_EN            0x00000004 /* Enable USB interface */


/* Endpoint queue head - capabilities and characteristics */
#define MPC8349_DR_DQH_MULT_MASK             0xc0000000 /* mult */
#define MPC8349_DR_DQH_MULT_0                0x00000000 /* mult 0 */
#define MPC8349_DR_DQH_MULT_1                0x40000000 /* mult 1 */
#define MPC8349_DR_DQH_MULT_2                0x80000000 /* mult 2 */
#define MPC8349_DR_DQH_MULT_3                0xc0000000 /* mult 3 */
#define MPC8349_DR_DQH_MULT_SHIFT            30         /* mult bit position */
#define MPC8349_DR_DQH_ZLT_MASK              0x20000000 /* zero length termination select */
#define MPC8349_DR_DQH_ZLT_ENABLE            0x00000000 /* zero length termination select */
#define MPC8349_DR_DQH_ZLT_DISABLE           0x20000000 /* zero length termination select */
#define MPC8349_DR_DQH_MAXPACKETLENGTH_MASK  0x07ff0000 /* maximum packet length */
#define MPC8349_DR_DQH_MAXPACKETLENGTH_SHIFT 16         /* maximum packet length */
#define MPC8349_DR_DQH_IOS_MASK              0x00008000 /* interrupt on setup */
#define MPC8349_DR_DQH_IOS_ENABLE            0x00008000 /* interrupt on setup */
#define MPC8349_DR_DQH_IOS_DISABLE           0x00000000 /* interrupt on setup */

#define MPC8349_DR_DTD_TERMINATE             0x00000001 /* terminate */
#define MPC8349_DR_DTD_STATUS_MASK           0x000000ff /* status */
#define MPC8349_DR_DTD_STATUS_TRANS_ERROR    0x00000008 /* status - transaction error */
#define MPC8349_DR_DTD_STATUS_BUFFER_ERROR   0x00000020 /* status - data buffer error */
#define MPC8349_DR_DTD_STATUS_HALTED         0x00000040 /* status - halted */
#define MPC8349_DR_DTD_STATUS_ACTIVE         0x00000080 /* status - active */
#define MPC8349_DR_DTD_IOC                   0x00008000 /* interrupt on complete */
#define MPC8349_DR_DTD_TOTALBYTES_MASK       0x3fff0000 /* total bytes bit position */
#define MPC8349_DR_DTD_TOTALBYTES_SHIFT      16         /* total bytes bit position */

/* Definitions for Max Packet Size */
#define MPC8349_DR_MAXPSIZE_8                0x0008   /* Max Packet Size - 8 */
#define MPC8349_DR_MAXPSIZE_16               0x0010   /* Max Packet Size - 16 */
#define MPC8349_DR_MAXPSIZE_32               0x0020   /* Max Packet Size - 32 */
#define MPC8349_DR_MAXPSIZE_64               0x0040   /* Max Packet Size - 64 */
#define MPC8349_DR_MAXPSIZE_128              0x0080   /* Max Packet Size - 128 */
#define MPC8349_DR_MAXPSIZE_256              0x0100   /* Max Packet Size - 256 */
#define MPC8349_DR_MAXPSIZE_512              0x0200   /* Max Packet Size - 512 */
#define MPC8349_DR_MAXPSIZE_3024             0x0400   /* Max Packet Size - 3024 */

#define MPC8349_DR_MAXPSIZE_ENDPT_0          MPC8349_DR_MAXPSIZE_64

/* Alignment */
#define MPC8349_DR_DQH_MEM_ALIGN             0x1000
#define MPC8349_DR_DTD_MEM_ALIGN             0x20
#define MPC8349_DR_DTD_BUFFER_MEM_ALIGN      0x1000

#define MPC8349_DR_DTD_BUFFERS               5
#define MPC8349_DR_DTD_BUFFER_SIZE           0x1000
#define MPC8349_DR_DTD_MAX_XFER_SIZE         0x4000
#define MPC8349_DR_DTD_SETUP_BUFFERS         2

#ifdef MPC8349_DR_DMA_SUPPORTED
/* DMA */
#define MPC8349_DMA_IRQ                      INUM_DMA
#define MPC8349_MSG_IRQ                      INUM_MU
#define MPC8349_DR_DMA_CHANNELS              4
#endif /* MPC8349_DR_DMA_SUPPORTED */

#ifdef  __cplusplus
}
#endif

#endif  /* __INCusbMpc8349h */
