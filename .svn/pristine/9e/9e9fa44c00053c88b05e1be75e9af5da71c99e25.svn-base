/* walnut.h - IBM Walnut/Sycamore 405GP/405GPr eval board header */

/*
 * Copyright (c) 1999-2002, 2004, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,28aug08,d_l  break default mac(ENET_DEFAULT) to three bytes.(WIND00121398)
01j,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
01i,20aug02,pch  405GP / 405GPr interoperability
01h,18jun02,mcg  add 405GPr Sycamore board FPGA support, fix UIC_INTR_TRIGGER
                 and UIC_INTR_CRITICAL.
01g,12jun02,pch  SPR 74987: C++
01f,09jan02,pch  Change ENET references to ENET0 for consistency with ebony
01e,16sep01,dat  Use of WRS_ASM macro
01d,27jul01,kab  Changed INCLUDE_PCI_NETWORK to INCLUDE_LN97XEND; reformat to
                 WR convention
01c,25may00,mcg  added system information structure
01b,11may00,mcg  added PCI to local memory translation macros and PCI
                 memory access function definitions
01a,08sep99,mcg  created
*/

/*
This file contains I/O addresses and related constants for the
IBM Walnut/Sycamore BSP.  Walnut and Sycamore are the evaluation boards
for the PowerPC 405GP and 405GPr embedded controllers, respectively.
*/

#ifndef INCwalnuth
#define INCwalnuth

#ifdef __cplusplus
    extern "C" {
#endif

/* generic macros */

#ifndef EIEIO_SYNC
# define EIEIO_SYNC  WRS_ASM (" eieio; sync")
#endif  /* EIEIO_SYNC */

#ifndef EIEIO
# define EIEIO  WRS_ASM (" eieio")
#endif  /* EIEIO */

#ifndef SYNC
# define SYNC  WRS_ASM (" sync")
#endif  /* SYNC */


#define BUS             NONE                 /* no off-board bus interface    */
#define N_SIO_CHANNELS  2                    /* Number of serial I/O channels */

/*
 * Minimum and maximum system clock rates
 */
#define SYS_CLK_RATE_MIN        3            /* minimum system clock rate     */
#define SYS_CLK_RATE_MAX        5000         /* maximum system clock rate     */

/*
 * clock frequencies
 */
#define ONE_BILLION        1000000000
#define SYS_CLK_FREQ         33333333           /* 33.333333 MHz from MPC972  */
#define EXT_TIMER_CLK_FREQ  (SYS_CLK_FREQ / 4)  /* FPGA divides sysclk by 4   */
#define EXT_SER_CLK_FREQ     11059200           /* 11.0592 MHz dedicated clk  */


/*
 * Exception Vector Prefix Register value.  Exception vectors will be located
 * at the start of SDRAM.
 */

#define WALNUT_EVPR_VAL    0x00000000

/*
 * Initial values for UIC interrupt controller polarity, triggering, and
 * critical vs. non-critical.
 * All interrupts are level triggered except the external master.
 * All interrupts are non-critical except for the pushbutton switch attached
 * to IRQ1.
 */

#define UIC_INTR_POLARITY  0xFFFFFFE0
#define UIC_INTR_TRIGGER   0x10000000
#define UIC_INTR_CRITICAL  0x00000020

/*
 * PCI
 */

#define WALNUT_NUM_PCI_SLOTS   4

#ifdef INCLUDE_PCI
    /* Translate PCI addresses to virtual addresses (master windows) */

#ifndef PCI_MEM2LOCAL
#define PCI_MEM2LOCAL(x) \
        ((void *)((UINT)(x) - PCI_MSTR_MEM_BUS + PCI_MSTR_MEM_LOCAL))
#endif

#ifndef PCI_IO2LOCAL
#define PCI_IO2LOCAL(x) \
        ((void *)((UINT)(x) - PCI_MSTR_IO_BUS + PCI_MSTR_IO_LOCAL))
#endif

#ifndef PCI_MEMIO2LOCAL
#define PCI_MEMIO2LOCAL(x) \
        ((void *)((UINT)(x) - PCI_MSTR_MEMIO_BUS + PCI_MSTR_MEMIO_LOCAL))
#endif

    /* Translate local memory address to PCI address (slave window) */

#ifndef LOCAL2PCI_MEM
#define LOCAL2PCI_MEM(x) \
        ((void *)((UINT)(x) - PCI_SLV_MEM_LOCAL + PCI_SLV_MEM_BUS))
#endif

/*
 * Routines for reading or writing PCI I/O or Memory space. These routines are
 * written in assembler and use byte swapping load/store instructions.
 */

#ifndef PCI_IN_BYTE
#define PCI_IN_BYTE(x)         sysPciInByte (x)
#endif
#ifndef PCI_IN_WORD
#define PCI_IN_WORD(x)         sysPciInWord (x)
#endif
#ifndef PCI_IN_LONG
#define PCI_IN_LONG(x)         sysPciInLong (x)
#endif
#ifndef PCI_OUT_BYTE
#define PCI_OUT_BYTE(x,y)      sysPciOutByte (x,y)
#endif
#ifndef PCI_OUT_WORD
#define PCI_OUT_WORD(x,y)      sysPciOutWord (x,y)
#endif
#ifndef PCI_OUT_LONG
#define PCI_OUT_LONG(x,y)      sysPciOutLong (x,y)
#endif

#endif /* INCLUDE_PCI */

/*
 * The Allied Telesyn AT-2700TX card contains the AMD Am79C972 controller
 * (PCnet-FAST+).  Below is the information for the PCI vendor and device
 * IDs for the AMD Am79C97x family and registers where the MAC address can be
 * found after it is loaded from the serial EPROM on the PCI card.
 */
#ifdef INCLUDE_END
# ifdef INCLUDE_LN97XEND
#   define PCI_VENDOR_ID_AMD     0x1022
#   define PCI_DEVICE_ID_79C97X  0x2000

#   define APROM01               0           /* EEPROM registers */
#   define APROM23               2
#   define APROM45               4
#   define APROM_SIZE            16
# endif /* INCLUDE_LN97XEND */
#endif /* INCLUDE_END */

/*
 * Dallas Semiconductor DS1743P 8KB NVRAM with Real-Time Clock definitions
 * The first 1KB of the NVRAM is reserved for IBM Eval kit software use.
 * The last three bytes of the EMAC Ethernet hardware address are stored
 * after the boot information (offset 1KB + 256).  The first three bytes of
 * this address are assumed to be 0x0004ac.
 */

#define NV_RAM_ADRS		0xF0000000
#define NV_RAM_INTRVL		1
#define NV_RAM_SIZE		(8*1024) - 8	/* 8KB - 8 bytes for the RTC */
#define NV_RAM_SIZE_ALIGNED	(8*1024)	/* page aligned size for MMU */
#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET		0x400		/* Offset of 1KB             */
#define NV_ENET_OFFSET_0	0x100		/* Offset of Ethernet HW     */
						/* addr from the boot offset */

#define RTC_BASE_ADRS		0xF0001FF8	/* real-time clock base addr */


/*
 * The Ethernet hardware address that is used with EMAC is
 *
 *       0x0004ACnnnnnn
 *
 * where the first three bytes are defined below, and last three bytes
 * are stored in the NVRAM and can be modified by using the function
 * sysLanIbmEmacEnetAddrSet0()
 */

#define ENET_DEFAULT0   0x00
#define ENET_DEFAULT1   0x04
#define ENET_DEFAULT2   0xAC

/*
 * The address of the PHY attached to the MII interface of EMAC.
 */

#define EMAC_PHY_ADRS   1

/*
 * FPGA registers and bit definitions
 * The FPGA acts as a secondary interrupt controller for the keyboard, mouse,
 * and irDA controllers on the board.
 */

#define FPGA_BASE_ADRS       0xF0300000
#define FPGA_INT_STATUS      0xF0300000       /* R/O Interrupt status         */
#define FPGA_INT_ENABLE      0xF0300001       /* R/W Interrupt enable         */
#define FPGA_INT_POLARITIES  0xF0300002       /* R/W Interrupt polarity.      */
#define FPGA_INT_TRIGGERS    0xF0300003       /* R/W Interrupt type.          */
#define FPGA_BRDC            0xF0300004       /* R/W Board controls.          */
#define FPGA_BRDS1           0xF0300005       /* R/O Board status.            */
#define FPGA_BRDS2           0xF0300006       /* R/O Board status.            */
#define FPGA_SPARE1          0xF030000E       /* R/O Spare inputs.            */
#define FPGA_SPARE2          0xF030000F       /* R/W Spare outputs.           */

/* FPGA bits */
#define SW_CLK_SRC1          0x40         /* if async pci, ext or int clk     */
#define FSEL_A               0x03         /* use for mask                     */
#define FSEL_INVALID1        0x00         /* invalid FSEL_A0, FSEL_A1 settings*/
#define FSEL_INVALID2        0x02         /* invalid FSEL_A0, FSEL_A1 settings*/
#define FSEL_66              0x01         /* select 66Mhz async int PCI       */
#define FSEL_33              0x03         /* select 33Mhz async int PCI       */
#define FPGA_BRDC_DEF_UART1  0x01         /* default UART1 control mode       */

/* The following bits are in the 405GPr Sycamore FPGA_BRDS1 register only */
#define PCI_ASYNC_MODE_EN    0x80
#define PCI_CLK_SEL          0x40
#define MASTER_CLK_SEL       0x20
#define PCI_ARBITER_EN       0x10
#define PCI_M66EN            0x08
#define PCI_FORCE_33         0x04

/*
 * There is 512KB of SRAM on the board.  It can reside in one of two
 * locations depending on the setting of switch 6 on bank U79.
 */
#define SRAM_START_ADRS  0xFFF00000
#define SRAM_SIZE        (512*1024)


/*
 * The board has one socket for a 168 pin SDRAM DIMM.
 * The following are offsets in the SDRAM DIMM's presence-detect serial EEPROM.
 * Values at these offsets are are accessed via the 405GP I2C interface, and
 * are used to help configure the SDRAM controller correctly for the
 * properties of the DIMM.
 */
#define SDRAM_NUM_ROWS     3
#define SDRAM_NUM_COLS     4
#define SDRAM_NUM_BANKS    5
#define SDRAM_WIDTH        6
#define SDRAM_NUM_INTBANKS 17
#define SDRAM_MOD_DENSITY  31

/*
 * WALNUT_ICCR_VAL and WALNUT_DCCR_VAL define the cachability
 * state of the total effective adress space at a 128 MB granularity
 * for instruction and data.
 * By default memory spaces 0x00000000-0x07ffffff and 0xf8000000-0xfffffffff
 * are set as cached. The rest of the effective adress space is programmed
 * as non-cached.
 * this can be changed here, according to the memory board mapping.
 */

#define WALNUT_ICCR_VAL            _PPC405_ICCR_DEFAULT_VAL
#define WALNUT_DCCR_VAL            _PPC405_DCCR_DEFAULT_VAL

/*
 * External IRQ assignments, or how the UIC external interrupt pins
 * are actually used on the board.
 */
#define INT_VEC_PCI_SLOT3  INT_VEC_EXT_IRQ_3        /* J25 */
#define INT_VEC_PCI_SLOT2  INT_VEC_EXT_IRQ_4        /* J26 */
#define INT_VEC_PCI_SLOT1  INT_VEC_EXT_IRQ_5        /* J29 */
#define INT_VEC_PCI_SLOT0  INT_VEC_EXT_IRQ_6        /* J31 */

#define INT_LVL_PCI_SLOT3  INT_LVL_EXT_IRQ_3        /* J25 */
#define INT_LVL_PCI_SLOT2  INT_LVL_EXT_IRQ_4        /* J26 */
#define INT_LVL_PCI_SLOT1  INT_LVL_EXT_IRQ_5        /* J29 */
#define INT_LVL_PCI_SLOT0  INT_LVL_EXT_IRQ_6        /* J31 */


#ifndef _ASMLANGUAGE

/*
 * System information structure.  It is used to get information about the
 * processor PLL and operating frequencies.
 */

typedef struct
    {
    UINT32  pllFwdDiv;              /* FwdDiv for 405GP, FwdDivA for 405GPr */
    UINT32  pllFwdDivB;             /* 405GPr only */
    UINT32  pllFbkDiv;
    UINT32  pllPlbDiv;
    UINT32  pllPciDiv;
    UINT32  pllExtBusDiv;
    UINT32  pllOpbDiv;
    UINT32  freqVCOMhz;             /* in MHz                          */
    UINT32  freqProcessor;
    UINT32  freqPLB;
    UINT32  freqPCI;
    UINT32  pciIntArbEn;            /* Internal PCI arbiter is enabled */
    UINT32  pciClkSync;             /* PCI clock is synchronous        */
    BOOL    isGpr;                  /* TRUE => 405GPr (else 405GP)     */
    char    sysModelStr[40];        /* sprintf target in sysModelGet   */
    } SYS_INFO;

#endif

#ifdef __cplusplus
    }
#endif

#endif  /* INCwalnuth */
