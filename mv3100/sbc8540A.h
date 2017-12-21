/* sbc8540A.h - Motorola sbc8540 board header file */

/*
 * Copyright (c) 2004-2005, 2008 Wind River Systems, Inc
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2004-2005, Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
02g,07apr09,syt Added a macro for Quad UART's reference crystal frequency 
                (1.8432MHz) (WIND00160039) 
02f,15oct08,y_w Move TFFS related componets to config.h
02e,21aug08,y_w Change NVRAM storage to a 64KB EEPROM which could specifically
                be used by user (WIND00131666)
02d,25apr08,x_f Modified AUX clock rate for vxBus FIT timer driver.
02c,24jan08,y_w Removed sysApplInit() related lines to fix compiler warnings
                Updated TFFS support for vxWorks 6.6; fixed mod history numbering
02b,21oct05,cak Fixed the PCI bridge interrupt assignments for bridge number
                1 to PMC1 and PMC2.
02a,21sep05,pjh Reduced AUX_CLK_RATE_MAX to 600000 because the F02 version
                has a slower processor speed.
01z,07sep05,cak Modifications to make the file more Tornado Project friendly.
01y,29aug05,efb Added USB defines and corrected PCI bridge interrupt numbers.
01x,27aug05,cak TFFS support. 
01w,15aug05,efb Completed board register bit definitions for show routines.
01v,12aug05,pjh Support for global and external tick timers.
01u,02aug05,cak Fixed the board register definitions, adjusted the INT_LINES
                macro for PCI interrupt assignment to include entries for when 
                the PCI bus is running PCI-X as well as PCI, and added a 
                definition for the Tsi148's (Tempe's) PCI vendor and device ID.
01t,18jul05,cak Adjusted local access window programming (DRAM to window 1)
                to match MotLoad which is necessary for firmware RAM booting.
01s,06jul05,cak Added BIT and MASK definitions for the user LEDs.
01r,24jun05,efb Added bit define for PHY int mask in sys control register.
01q,21jun05,cak DS1375 RTC with Alarm support.
01p,02jun05,pjh Added Quad UART EXAR ST16C554D support. 
01o,15may05,efb Added sATA support.
01n,09may05,efb Added defines for FEC support
01m,03may05,cak Fixed local bus controller chip select assignments.
01l,26apr05,cak DS1621 Temperature Sensor support.
01k,05apr05,cak Modified Local Access Window configuration to match the
                PCI outbound window configuration.
01j,10mar05,cak Added defines for TSEC support.
01i,21feb05,cak Enable INCLUDE_ECC by default and adjust hard-coded SMC
                timing parameters to those recommended by hardware team.
01h,07feb05,cak L2 Cache/SRAM support. 
01g,31jan05,cak VPD Support.
01f,06jan05,cak Adjusted the oscillator frequency, the flash bank
                parameters and the chip select settings - MV3100
                board bring-up. 
01e,17dec04,cak Added Tempe support definitions.
01d,10dec04,cak PCI/PCI-X Bus Interface support.
01c,24nov04,cak Memory controller support.
01b,09nov04,scb I2c support.
01a,11oct04,scb Initial writing.
*/

#ifndef __INCsbc8540Ah
#define __INCsbc8540Ah

/*
 * This file contains macros and defines for I/O addresses, chip
 * selects etc. for the SBC (single board computer).
 */

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* 
 * Define board specific addresses and field programming values.
 * There are three different register domains which much be programmed
 * in order to access an area such a flash or IO space.  These
 * domains are:
 * 
 * MMU access -   This will be facilitated by programming TLB1 entries
 *                to cover the appropriate address range.  TLB1 entries
 *                are programmed initially inside of "romInit.s" and 
 *                again after the kernel comes up, using the "sysLib.c"
 *                resident table sysStaticTlbDesc[].
 * 
 * Local access - This is covered programming of the appropriate LAWBARn
 *                LAWARn register set.  Along with an address range, these
 *                registers specify the associated target interface.  This
 *                programming is performed inside of "romInit.s"
 * 
 * Chip select  - This is governed by BRn and ORn register programming.
 *                This programming is also done inside of "romInit.s".
 * 
 * All three of these register domains must be set in harmony with each
 * other to properly access flash or IO space.  The assumptions concerning 
 * flash are:
 * 
 * 1.  There may be up to two different banks of flash, the 'boot' flash
 *     and the 'alternate' flash.  Two flash banks are on the SBC
 *     (Single board computer) version of Big Easy.  
 * 
 * 2.  The boot and alternate flash banks occupy one contiguous region
 *     of memory and can thus be covered by a single TLB1 address range,
 *     and local access register set.  Separate flash banks require
 *     separate chip selects (and thus two sets of BRn/ORn registers
 *     must be programmed).  Note the we may actually have two separate
 *     TLB1 entries covering the same address range, one with a TS
 *     (Translation Space) bit set to zero, and one with TS set to 1.
 *     This will allow both the bootrom (TS = 0) and the kernel (TS = 1)
 *     to access this space.
 *
 * Note: FLASH_WINDOW_SIZE is used by sysTffs.c
 */

#define FLASH_BASE_ADRS                 0xf0000000
#define FLASH_MMU_TLB_SZ                _MMU_TLB_SZ_256M
#define FLASH_LAWAR_SZ                  MPC8540_PCI_SIZE_ENCODE(FLASH_WINDOW_SIZE)    
#define FLASH_WINDOW_SIZE               0x10000000
#define FLASH_BOOT_ADRS                 0xf8000000
#define FLASH_BOOT_SZ                   0x10000000
#define FLASH_ALT_ADRS                  0xf0000000
#define FLASH_ALT_SZ                    0x10000000

#define LOCAL_BOARD_ACCESS_ADRS         0xe2000000
#define LOCAL_BOARD_ACCESS_SIZE         0x00040000
#define LOCAL_BOARD_ACCESS_MMU_TBL_SZ   _MMU_TLB_SZ_256K
#define LOCAL_BOARD_ACCESS_LAWAR_SZ	\
        MPC8540_PCI_SIZE_ENCODE(LOCAL_BOARD_ACCESS_SIZE)    

/* The following describe the inside of the "local board access" space */

#define SBC_CTRL_REG_ADRS               (LOCAL_BOARD_ACCESS_ADRS)
#define SBC_CTRL_REG_SZ                  0x10000
#define SBC_QUAD_UART_ADRS              (SBC_CTRL_REG_ADRS + 0x10000)
#define SBC_QUAD_UART_SZ                0x10000
#define SBC_32BIT_TIMER_ADRS            (SBC_CTRL_REG_ADRS + 0x20000)
#define SBC_32BIT_TIMER_SZ              0x10000

/*
 * Define programming values for the 8 sets of local access window
 *  registers (LAWBARn/LAWARn)
 */

/* Access to flash (covers chip selects 0 and 1 (BR0/OR0 and BR1/OR1)) */

#define CCSR_LAWBAR0_PROG (CCSR_LAWBAR_BASE_ADDR_ENCODE(FLASH_BASE_ADRS))
#define CCSR_LAWAR0_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_LBC | \
                           FLASH_LAWAR_SZ)

/* Access to local DRAM */

#define CCSR_LAWBAR1_PROG  \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(LOCAL_MEM_LOCAL_ADRS))
#define CCSR_LAWAR1_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_DDRSDRAM | \
                           MPC8540_PCI_SIZE_ENCODE(0x40000000))

/*
 * Access to local board devices (covers chip selects 2, 3 and 4)
 * (BR2/OR2, BR3/OR3 and BR4/OR4))
 */

#define CCSR_LAWBAR2_PROG  \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(LOCAL_BOARD_ACCESS_ADRS))
#define CCSR_LAWAR2_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_LBC | \
                           LOCAL_BOARD_ACCESS_LAWAR_SZ)

/*
 * Now let's cover PCI space.  We must configure one local access 
 * window for each of the configured outbound PCI windows.  There
 * will be a total of four windows configured for PCI space:
 * 
 * PCI_MSTR_MEMIO_LOCAL
 * PCI_MSTR_MEM_LOCAL
 * ISA_MSTR_IO_LOCAL/PCI_MSTR_IO_LOCAL
 * VME_MEM_LOCAL_START
 */

#define CCSR_LAWBAR3_PROG  \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI_MSTR_MEMIO_LOCAL))
#define CCSR_LAWAR3_PROG  \
        (CCSR_LAWAR_EN_VAL_ENABLED | CCSR_LAWAR_TGTIF_VAL_PCIX | \
         MPC8540_PCI_SIZE_ENCODE(PCI_MSTR_MEMIO_SIZE))

#define CCSR_LAWBAR4_PROG \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI_MSTR_MEM_LOCAL))
#define CCSR_LAWAR4_PROG (CCSR_LAWAR_EN_VAL_ENABLED | \
                          CCSR_LAWAR_TGTIF_VAL_PCIX | \
                          MPC8540_PCI_SIZE_ENCODE(PCI_MSTR_MEM_SIZE))

#define CCSR_LAWBAR5_PROG (CCSR_LAWBAR_BASE_ADDR_ENCODE(ISA_MSTR_IO_LOCAL))
#define CCSR_LAWAR5_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIX | \
                           MPC8540_PCI_SIZE_ENCODE(PCI_IO_SIZE))

#define CCSR_LAWBAR6_PROG (CCSR_LAWBAR_BASE_ADDR_ENCODE(VME_MEM_LOCAL_START))
#define CCSR_LAWAR6_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIX | \
                           MPC8540_PCI_SIZE_ENCODE(VME_MEM_SIZE))

/* No more access windows required so we set the last one to 'invalid' */

#define CCSR_LAWBAR7_PROG  CCSR_LAWBARn_INVALID
#define CCSR_LAWAR7_PROG   CCSR_LAWARn_INVALID

/* Define information for chip select programming (BRn/ORn) */

/*
 * ----- Chip select 0
 *
 * BR0 to be set as follows (flash boot access):
 *   bit 0       = 1    ->  V: valid
 *   bit 2:3     = 00   ->  ATOM: Not used for atomic operations
 *   bit 5:7     = 000  ->  MSEL: Machine select = GPCM
 *   bit 8       = 0    ->  WP: Read and write accesses allowed
 *   bit 10:9    = 00   ->  DECC: Err check disable, parity generated
 *   bit 12:11   = 11   ->  PS: Port size = 32-bit
 *   bit 31:16   = xx   ->  BA: FLASH_BOOT_ADRS (base address)
 */

#define CCSR_BR0_PROG    (FLASH_BOOT_ADRS | 0x1801)

/*
 * OR0 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing 
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup 
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle 
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier 
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to 
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 *
 */

#define CCSR_OR0_PROG    (CCSR_ORn_AM_VAL(FLASH_BOOT_SZ) | 0x6FF7)

/*
 * ----- Chip select 1
 *
 * BR1 to be set as follows (Optional flash bank):
 *   bit 0       = 1    ->  V: valid
 *   bit 2:3     = 00   ->  ATOM: Not used for atomic operations
 *   bit 5:7     = 000  ->  MSEL: Machine select = GPCM
 *   bit 8       = 0    ->  WP: Read and write accesses allowed
 *   bit 10:9    = 00   ->  DECC: Err check disable, parity generated
 *   bit 12:11   = 11   ->  PS: Port size = 32-bit
 *   bit 31:16   = xx   ->  BA: FLASH_BASE_ADRS (base address)
 */

#define CCSR_BR1_PROG    (FLASH_ALT_ADRS | 0x1801)
 
/*
 * OR1 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 */

#define CCSR_OR1_PROG    (CCSR_ORn_AM_VAL(FLASH_ALT_SZ) | 0x6FF7)

/*
 * ----- Chip select 2
 *
 * BR2 to be set as follows (on board peripherals, flash ctl reg):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 11b  ->  PS: Port size = 32-bit
 * bits 31:16 = xx   ->  BA: LOCAL_BOARD_ACCESS_ADRS_2 (base address)
 */

#define CCSR_BR2_PROG   (SBC_CTRL_REG_ADRS | 0x1801)

/*
 * OR2 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 */

#define CCSR_OR2_PROG   (CCSR_ORn_AM_VAL(SBC_CTRL_REG_SZ) | 0x0FF7)

/*
 * ----- Chip select 3
 *
 * BR3 to be set as follows (Quad UARTs):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 01b  ->  PS: Port size = 8-bit
 * bits 31:16 = xx   ->  BA: SBC_QUAD_UART_ADRS (base address)
 */

#define CCSR_BR3_PROG   (SBC_QUAD_UART_ADRS | 0x0801)

/*
 * OR3 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 *
 */

#define CCSR_OR3_PROG  (CCSR_ORn_AM_VAL(SBC_QUAD_UART_SZ) | 0x0FF7)

/*
 * ----- Chip select 4
 *
 * BR4 to be set as follows (32-bit timers):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 11b  ->  PS: Port size = 32-bit
 * bits 31:16 = xx   ->  BA: SBC_32BIT_TIMER_ADRS (base address)
 */

#define CCSR_BR4_PROG   (SBC_32BIT_TIMER_ADRS | 0x1801)

/*
 * OR4 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 1    -> EHTR: 8 idle clock cycles are inserted.
 *   bit 2       = 2    -> TRLX: relaxed timing
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1111 -> SCY: 15 bus clock cycle wait states.
 *   bit 8       = 1    -> XACS: Address to chip select setup
 *                           is extended.
 *   bit 10:9    = 11   -> ACS: LCSn# is output a half bus clock cycle
 *                           after the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 */

#define CCSR_OR4_PROG   (CCSR_ORn_AM_VAL(SBC_32BIT_TIMER_SZ) | 0x0FF7)

/* The rest are invalid */

#define CCSR_BR5_PROG   (CCSR_BRn_INVALID)
#define CCSR_OR5_PROG   (CCSR_ORn_INVALID)

#define CCSR_BR6_PROG   (CCSR_BRn_INVALID)
#define CCSR_OR6_PROG   (CCSR_ORn_INVALID)

#define CCSR_BR7_PROG   (CCSR_BRn_INVALID)
#define CCSR_OR7_PROG   (CCSR_ORn_INVALID)

/* Now for some board related registers */

#define BRD_SYSTEM_STATUS_REG             (LOCAL_BOARD_ACCESS_ADRS + 0x0)
#define BRD_SYSTEM_CONTROL_REG            (LOCAL_BOARD_ACCESS_ADRS + 0x1)
#define BRD_SYSTEM_STATUS_INDICATOR_REG   (LOCAL_BOARD_ACCESS_ADRS + 0x2)
#define BRD_FLASH_CONTROL_REG             (LOCAL_BOARD_ACCESS_ADRS + 0x3)
#define BRD_PCI_BUS_A_STAT_REG            (LOCAL_BOARD_ACCESS_ADRS + 0x4)
#define BRD_PCI_BUS_B_STAT_REG            (LOCAL_BOARD_ACCESS_ADRS + 0x5)
#define BRD_PCI_BUS_C_STAT_REG            (LOCAL_BOARD_ACCESS_ADRS + 0x6)
#define BRD_INTERRUPT_DETECT_REG          (LOCAL_BOARD_ACCESS_ADRS + 0x7)
#define BRD_PRESENCE_DETECT_REG           (LOCAL_BOARD_ACCESS_ADRS + 0x8)
#define BRD_PLD_REVISION_REG              (LOCAL_BOARD_ACCESS_ADRS + 0x9)
#define BRD_PLD_DATE_CODE_REG             (LOCAL_BOARD_ACCESS_ADRS + 0xC)
#define BRD_TEST_1_REG                    (LOCAL_BOARD_ACCESS_ADRS + 0x10)
#define BRD_TEST_2_REG                    (LOCAL_BOARD_ACCESS_ADRS + 0x14)

#define BRD_SYSTEM_CONTROL_REG_TSTAT_MASK    0x01
#define BRD_PCI_BUS_B_STAT_REG_ERDY2_MASK    0x20
#define BRD_PCI_BUS_B_STAT_REG_ERDY1_MASK    0x10

/* MPC8540 DUART Support */

#define COM0_BASE_ADRS             ((ULONG) CCSBAR + (ULONG) 0x4500)
#define UART_REG_INTVL             1
#define N_DUART_CHANNELS           1                 /* 1 serial ports  */
#define N_DUART_SIO_CHANNELS       N_DUART_CHANNELS  /* 1 serial I/O chnl */
#define DUART_BAUD                 9600              /* default baud rate */
#define DUART_REG_ADDR_INTERVAL    1                 /* duart vector reg distance */

#define UART_REG_ADDR_INTERVAL     1                 /* uart vector reg distance */

/* Rear Transition Module Quad UART Support */

#undef NUM_TTY

#ifdef INCLUDE_QUAD_UART
#  define NUM_TTY 5
#else
#  define NUM_TTY 1
#endif  /* INCLUDE_QUAD_UART */

#define QUART_REF_FREQ       1843200

#define COM1_BASE_ADRS       (SBC_QUAD_UART_ADRS + 0x1000)
#define COM2_BASE_ADRS       (SBC_QUAD_UART_ADRS + 0x2000)
#define COM3_BASE_ADRS       (SBC_QUAD_UART_ADRS + 0x3000)
#define COM4_BASE_ADRS       (SBC_QUAD_UART_ADRS + 0x4000)

#define COM1_INT_LVL         EPIC_VEC_EXT_IRQ3
#define COM2_INT_LVL         EPIC_VEC_EXT_IRQ3
#define COM3_INT_LVL         EPIC_VEC_EXT_IRQ3
#define COM4_INT_LVL         EPIC_VEC_EXT_IRQ3

#define COM1_INT_VEC         (COM1_INT_LVL + 0x00)
#define COM2_INT_VEC         (COM2_INT_LVL + 0x00)
#define COM3_INT_VEC         (COM3_INT_LVL + 0x00)
#define COM4_INT_VEC         (COM4_INT_LVL + 0x00)

#define N_UART_CHANNELS      4    /* No. serial I/O channels */

/* External Tick timers */

#define BRD_PRESCALER_REG            (LOCAL_BOARD_ACCESS_ADRS + 0x20000)

#define BRD_TICK_TIMER_1_CTRL_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20010)
#define BRD_TICK_TIMER_2_CTRL_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20020)
#define BRD_TICK_TIMER_3_CTRL_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20030)
#define BRD_TICK_TIMER_4_CTRL_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20040)
#define BRD_TICK_TIMER_1_CMPR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20014)
#define BRD_TICK_TIMER_2_CMPR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20024)
#define BRD_TICK_TIMER_3_CMPR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20034)
#define BRD_TICK_TIMER_4_CMPR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20044)
#define BRD_TICK_TIMER_1_CNTR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20018)
#define BRD_TICK_TIMER_2_CNTR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20028)
#define BRD_TICK_TIMER_3_CNTR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20038)
#define BRD_TICK_TIMER_4_CNTR_REG    (LOCAL_BOARD_ACCESS_ADRS + 0x20048)

#define TMR_CNTR0_INT_LVL            EPIC_VEC_EXT_IRQ1
#define TMR_CNTR0_INT_VEC            (TMR_CNTR0_INT_LVL + 0x00)

/* OK, here are some bit and mask definitions for the board registers */

#define BRD_SYSTEM_CONTROL_TSTAT_BIT      (0)
#define BRD_SYSTEM_CONTROL_TSTAT_MASK     (1 << BRD_SYSTEM_CONTROL_TSTAT_BIT)
                                          
#define BRD_SYSTEM_CONTROL_EEPROMWP_BIT   (1)
#define BRD_SYSTEM_CONTROL_EEPROMWP_MASK \
        (1 << BRD_SYSTEM_CONTROL_EEPROMWP_BIT)
                                          
#define BRD_SYSTEM_CONTROL_FPHY_MASK_BIT  (2)
#define BRD_SYSTEM_CONTROL_FPHY_MASK_MASK \
        (1 << BRD_SYSTEM_CONTROL_FPHY_MASK_BIT)

#define BRD_SYSTEM_CONTROL_RESET_BIT      (5)
#define BRD_SYSTEM_CONTROL_RESET_MASK     (7 << BRD_SYSTEM_CONTROL_RESET_BIT)
#define BRD_SYSTEM_CONTROL_RESET_VAL      0xA0
                                          
#define BRD_SYSTEM_STATUS_BRD_TYPE_BIT    (0)
#define BRD_SYSTEM_STATUS_BRD_TYPE_MASK  \
        (0x03 << BRD_SYSTEM_STATUS_BRD_TYPE_BIT)
#define BRD_SYSTEM_STATUS_VME_BRD_TYPE    0
#define BRD_SYSTEM_STATUS_PRPMC_BRD_TYPE  1

#define BRD_SYSTEM_STATUS_SAFE_START_BIT  (4)
#define BRD_SYSTEM_STATUS_SAFE_START_MASK  \
        (1 << BRD_SYSTEM_STATUS_SAFE_START_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_BRD_FAIL_BIT (0)
#define BRD_SYSTEM_STATUS_INDICATOR_BRD_FAIL_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_BRD_FAIL_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR1_LED_BIT (1)
#define BRD_SYSTEM_STATUS_INDICATOR_USR1_LED_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_USR1_LED_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_BIT (2)
#define BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_USR2_LED_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_BIT (3)
#define BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_MASK \
        (1 << BRD_SYSTEM_STATUS_INDICATOR_USR3_LED_BIT)

#define BRD_SYSTEM_STATUS_INDICATOR_USR_LED_MASK  0x0E

#define BRD_FLASH_CONTROL_FLASH_RDY_BIT       (0)
#define BRD_FLASH_CONTROL_FLASH_RDY_MASK  \
        (1 << BRD_FLASH_CONTROL_FLASH_RDY_BIT)

#define BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_BIT  (1)
#define BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_MASK  \
        (1 << BRD_FLASH_CONTROL_BOOT_BLOCK_SEL_BIT)

#define BRD_FLASH_CONTROL_HARDWARE_WP_BIT     (2)
#define BRD_FLASH_CONTROL_HARDWARE_WP_MASK  \
        (1 << BRD_FLASH_CONTROL_HARDWARE_WP_BIT)

#define BRD_FLASH_CONTROL_SOFTWARE_WP_BIT     (3)
#define BRD_FLASH_CONTROL_SOFTWARE_WP_MASK  \
        (1 << BRD_FLASH_CONTROL_SOFTWARE_WP_BIT)

#define BRD_FLASH_CONTROL_MAP_SEL_BIT         (4)
#define BRD_FLASH_CONTROL_MAP_SEL_MASK  \
        (1 << BRD_FLASH_CONTROL_MAP_SEL_BIT)

#define BRD_PCI_BUS_A_STAT_SPD_BIT    (0)
#define BRD_PCI_BUS_A_STAT_SPD_MASK   (0x03 << BRD_PCI_BUS_A_STAT_SPD_BIT)

#define BRD_PCI_BUS_A_STAT_PCIX_BIT   (2)
#define BRD_PCI_BUS_A_STAT_PCIX_MASK  (1 << BRD_PCI_BUS_A_STAT_PCIX_BIT)

#define BRD_PCI_BUS_A_STAT_64B_BIT    (3)
#define BRD_PCI_BUS_A_STAT_64B_MASK   (1 << BRD_PCI_BUS_A_STAT_64B_BIT)

#define BRD_PCI_BUS_B_STAT_SPD_BIT    (0)
#define BRD_PCI_BUS_B_STAT_SPD_MASK   (0x03 << BRD_PCI_BUS_B_STAT_SPD_BIT)

#define BRD_PCI_BUS_B_STAT_PCIX_BIT   (2)
#define BRD_PCI_BUS_B_STAT_PCIX_MASK  (1 << BRD_PCI_BUS_B_STAT_PCIX_BIT)

#define BRD_PCI_BUS_B_STAT_64B_BIT    (3)
#define BRD_PCI_BUS_B_STAT_64B_MASK   (1 << BRD_PCI_BUS_B_STAT_64B_BIT)

#define BRD_PCI_BUS_B_STAT_ERDY1_BIT  (4)
#define BRD_PCI_BUS_B_STAT_ERDY1_MASK (1 << BRD_PCI_BUS_B_STAT_ERDY1_BIT)

#define BRD_PCI_BUS_B_STAT_ERDY2_BIT  (5)
#define BRD_PCI_BUS_B_STAT_ERDY2_MASK (1 << BRD_PCI_BUS_B_STAT_ERDY2_BIT)

#define BRD_PCI_BUS_B_STAT_5VIO_BIT   (6)
#define BRD_PCI_BUS_B_STAT_5VIO_MASK  (1 << BRD_PCI_BUS_B_STAT_5VIO_BIT)

#define BRD_PCI_BUS_B_STAT_3VIO_BIT   (7)
#define BRD_PCI_BUS_B_STAT_3VIO_MASK  (1 << BRD_PCI_BUS_B_STAT_3VIO_BIT)

#define BRD_PCI_BUS_C_STAT_SPD_BIT    (0)
#define BRD_PCI_BUS_C_STAT_SPD_MASK   (0x03 << BRD_PCI_BUS_C_STAT_SPD_BIT)

#define BRD_PCI_BUS_C_STAT_PCIX_BIT   (2)
#define BRD_PCI_BUS_C_STAT_PCIX_MASK  (1 << BRD_PCI_BUS_C_STAT_PCIX_BIT)

#define BRD_PCI_BUS_C_STAT_64B_BIT    (3)
#define BRD_PCI_BUS_C_STAT_64B_MASK   (1 << BRD_PCI_BUS_C_STAT_64B_BIT)

#define BRD_PRESENCE_DETECT_PMC1_BIT  (0)
#define BRD_PRESENCE_DETECT_PMC1_MASK (1 << BRD_PRESENCE_DETECT_PMC1_BIT)

#define BRD_PRESENCE_DETECT_PMC2_BIT  (1)
#define BRD_PRESENCE_DETECT_PMC2_MASK (1 << BRD_PRESENCE_DETECT_PMC2_BIT)

#define BRD_PRESENCE_DETECT_PMC_SPAN_BIT  (2)
#define BRD_PRESENCE_DETECT_PMC_SPAN_MASK  \
        (1 << BRD_PRESENCE_DETECT_PMC_SPAN_BIT)

/* TFFS Support */

#define NUM_TFFS_VOLS 1 

/* I2C device address etc. */

#define DS1621_TEMP_SENSOR_I2C_ADDR    (0x90)
#define SPD_EEPROM_I2C_ADDR            (0xa0)
#define USR_BRD_EEPROM1_I2C_ADDR       (0xa4)
#define USR_BRD_EEPROM2_I2C_ADDR       (0xa6)
#define VPD_BRD_EEPROM_I2C_ADDR        (0xa8)
#define VPD_RTM_EEPROM_I2C_ADDR        (0xaa)
#define DS1375_RTC_I2C_ADDR            (0xd0)

#define VPD_BYTE_ADDR_SZ               2

#define VALID_I2C_WRT_ADDR    USR_BRD_EEPROM1_I2C_ADDR, \
                              USR_BRD_EEPROM2_I2C_ADDR, \
                              VPD_BRD_EEPROM_I2C_ADDR, \
                              DS1621_TEMP_SENSOR_I2C_ADDR, \
                              DS1375_RTC_I2C_ADDR, \
                              0xffffffff

#define I2C_WRITE_ENABLE \
    { \
    UINT8 reg08; \
    reg08 = *(VUINT8 *)BRD_SYSTEM_CONTROL_REG; \
    reg08 &= ~BRD_SYSTEM_CONTROL_EEPROMWP_MASK; \
    *(VUINT8 *)(BRD_SYSTEM_CONTROL_REG) = reg08; \
    reg08 = *(VUINT8 *)BRD_SYSTEM_CONTROL_REG; \
    }

/*
 * Non-volatile (NVRAM) defines. 
 *
 * The MVME3100 board does not have a true NVRAM chip.  This 
 * functionality is implemented via an 64KB EEPROM.  This serial EEPROM is
 * specifically for user configuration data storage.
 *
 * Of the storage area available to VxWorks, the first 0x100 (256 bytes)
 * are used to store the VxWorks boot parameters. 
 */

#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET          0x0         /* From start */
#define NV_RAM_SIZE             0x10000     /* 64KB Total */
#define NV_RAM_INTRVL           1
#define NV_RAM_I2C_ADDR         USR_BRD_EEPROM1_I2C_ADDR
#define NV_RAM_READ(x)          sysNvRead  (x,NV_RAM_I2C_ADDR)
#define NV_RAM_WRITE(x,y)       sysNvWrite (x,y,NV_RAM_I2C_ADDR)

/* DDR SDRAM Memory Controller Configuration Parameters */

#define DDR_SDRAM_CS_0_MODE                DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_1_MODE                DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_2_MODE                DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_3_MODE                DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_BSTOPRE_VAL              0x100
#define MPC8540_DDR_SDRAM_WRREC_VAL        3
#define MPC8540_DDR_SDRAM_WRTORD_VAL       1
#define DDR_SDRAM_WR_DATA_DELAY_VAL        0x3
#define DDR_SDRAM_ACSM_VAL                 0
#define DDR_SDRAM_CPO_VAL                  0x0
#define DDR_SDRAM_SDMODE_VAL               0x0062
#define DDR_SDRAM_ESDMODE_VAL              0x0000
#define DDR_SDRAM_2T_EN_VAL                1
#define DDR_SDRAM_NCAP_VAL                 0
#define DDR_SDRAM_DYN_PWR_VAL              0
#define DDR_SDRAM_SREN_VAL                 1
#define DDR_SDRAM_SBET_VAL                 0xFF
#define    INCLUDE_ECC
#define    LOCAL_MEM_AUTOSIZE
#define LOCAL_MEM_SIZE                     0x01000000

/* External Interrupt Sources */

#define TEMPE_INT_LVL           (EPIC_VEC_EXT_IRQ0)
#define PCI_SATA_INT            (EPIC_VEC_EXT_IRQ2)
#define PCI_VME_LINT0           (EPIC_VEC_EXT_IRQ0)
#define PCI_VME_LINT1           (EPIC_VEC_EXT_IRQ1)
#define PCI_VME_LINT2           (EPIC_VEC_EXT_IRQ2)
#define PCI_VME_LINT3           (EPIC_VEC_EXT_IRQ3)
#define PCI_BR1_INT0            (EPIC_VEC_EXT_IRQ4)
#define PCI_BR1_INT1            (EPIC_VEC_EXT_IRQ5)
#define PCI_BR1_INT2            (EPIC_VEC_EXT_IRQ6)
#define PCI_BR1_INT3            (EPIC_VEC_EXT_IRQ7)
#define PCI_BR2_INT0            (EPIC_VEC_EXT_IRQ4)
#define PCI_BR2_INT1            (EPIC_VEC_EXT_IRQ5)
#define PCI_BR2_INT2            (EPIC_VEC_EXT_IRQ6)
#define PCI_BR2_INT3            (EPIC_VEC_EXT_IRQ7)
#define DS1621_INT_VEC          (EPIC_VEC_EXT_IRQ9)
#define DS1375_INT_VEC          (EPIC_VEC_EXT_IRQ11)

/* Tempe Support */

#define TEMPE_PCI_BUS           0         /* PCI bus of Tempe PCI-VME bridge */
#define TEMPE_PCI_DEV           0x1       /* Device number of Tempe */
#define TEMPE_PCI_FCN           0         /* Function number of Tempe */

/* 
 * PCI interrupt assignments 
 *
 * The following table contains the interrupt assignments for the PCI
 * devices.  Note that there are two entries for each device on PCI bus
 * 0 - the bus directly connected to the MPC8540.  One entry is used when
 * bus 0 is running in PCI-X mode, the other entry is used when bus 0 is
 * running in PCI mode.  Tables 16-55 and 16-63 in the MPC8540 reference
 * manual show the IDSEL to PCI device number mappings for PCI and PCI-X
 * mode.
 */

#define INT_LINES \
    { \
        { 0xff,0xff,0xff,0xff }, \
        { PCI_VME_LINT0, PCI_VME_LINT1, PCI_VME_LINT2, PCI_VME_LINT3 }, \
        { PCI_BR1_INT0,  PCI_BR1_INT1,  PCI_BR1_INT2,  PCI_BR1_INT3 },  \
        { PCI_BR2_INT0,  PCI_BR2_INT1,  PCI_BR2_INT2,  PCI_BR2_INT3 },  \
        { PCI_SATA_INT,  0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { PCI_VME_LINT0, PCI_VME_LINT1, PCI_VME_LINT2, PCI_VME_LINT3 }, \
        { PCI_BR1_INT0,  PCI_BR1_INT1,  PCI_BR1_INT2,  PCI_BR1_INT3 },  \
        { PCI_BR2_INT0,  PCI_BR2_INT1,  PCI_BR2_INT2,  PCI_BR2_INT3 },  \
        { PCI_SATA_INT,  0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff },          \
        { 0xff,          0xff,          0xff,          0xff }           \
    }

#define EEBACR_STREAM_DIS           0x0000000b
#define EEBPCR_CPU_ENB_HIGHEST      0x01000002

/* Fixed Ethernet parameters */

#define PHY_ADDR_ENET_PORT0         1
#define PHY_ADDR_ENET_PORT1         2
#define PHY_ADDR_ENET_PORT2         3

#define MAX_ENET_PORTS              3

/* Transmit descriptors per queue per port */

#define ETHER_TX_DESC_NUM           0x80

/* Receive descriptors per queue per port */

#define ETHER_RX_DESC_NUM           0x80

/* SATA support in IDE legacy mode */

#ifdef INCLUDE_ATA
#ifndef INCLUDE_DOSFS
#   define INCLUDE_DOSFS           /* file system to be used */
#endif  /* INCLUDE_DOSFS */
#endif  /* INCLUDE_ATA */

/* SATA PCI IDs */

#define PCI_ID_PCI31244 0x32008086      /* Intel 31244 sATA Controller */
#define PCI_ID_SATA     PCI_ID_PCI31244

#define SATA_DPA_MODE   0x06            /* PCI subclass value for DPA mode */
#define SATA_IDE_MODE   0x01            /* PCI subclass value for legacy IDE */

/* Tsi148 (Tempe) PCI ID */

#define PCI_ID_TEMPE    0x014810e3    /* Id for Tempe VME-PCI Brdg */

/* USB */

#define PCI_ID_USB      0x00351033   /* USB controller device/vendor ID */
#define PCI_CONFIG_EXT2_OFFSET   0xE4
#define HIGH_CLOCK_SPD_SEL       0x20

#ifdef INCLUDE_USB
#   define INCLUDE_OHCI
#   define INCLUDE_EHCI

#   ifndef INCLUDE_USBTOOL
#      define INCLUDE_USB_INIT
#      define INCLUDE_OHCI_INIT
#      define INCLUDE_EHCI_INIT

#      ifdef INCLUDE_USB_MOUSE
#          define INCLUDE_USB_MOUSE_INIT
#      endif /* INCLUDE_USB_MOUSE */

#      ifdef INCLUDE_USB_KEYBOARD
#          define INCLUDE_USB_KEYBOARD_INIT
#      endif /* INCLUDE_USB_KEYBOARD */

#      ifdef INCLUDE_USB_MS_BULKONLY
#          define INCLUDE_USB_MS_BULKONLY_INIT
#          define BULK_MAX_DEVS     8

#          define INCLUDE_USB_MS_BULKONLY_INIT
#          define INCLUDE_DOSFS           /* dosFs file system */
#          define INCLUDE_XBD
#          define INCLUDE_DOSFS
#          define INCLUDE_USB_INIT
#          define INCLUDE_CBIO            /* Cached Block I/O support, cbioLib */
#          define INCLUDE_DOSFS_CHKDSK    /* DOS file system consistency checker */
#          define INCLUDE_DOSFS_FAT       /* DOS file system FAT12/16/32 handler */
#          define INCLUDE_DOSFS_FMT       /* DOS file system volume formatter */
#          define INCLUDE_DOSFS_MAIN      /* DOSFS2 file system primary module */
#          define INCLUDE_DISK_CACHE      /* Disk cache handler */
#          define INCLUDE_DISK_PART       /* Disk partition handler */
#          define INCLUDE_TAR             /* File system backup and archival */
#          define INCLUDE_DISK_UTIL          /* File system and disk utilities */
#          define INCLUDE_DOSFS_DIR_FIXED /* DOS File system old directory format handler */
#          define INCLUDE_DOSFS_DIR_VFAT  /* DOS file system VFAT directory handler */
#      endif /* INCLUDE_USB_MS_BULKONLY */

#      ifdef INCLUDE_USB_MS_CBI
#          define INCLUDE_USB_MS_CBI_INIT
#      endif /* INCLUDE_USB_MS_CBI */

#      ifdef INCLUDE_USB_PRINTER
#          define INCLUDE_USB_PRINTER_INIT
#      endif /* INCLUDE_USB_PRINTER */

#      ifdef INCLUDE_USB_SPEAKER
#          define INCLUDE_USB_SPEAKER_INIT
#      endif /* INCLUDE_USB_SPEAKER */

#      ifdef INCLUDE_USB_PEGASUS_END
#          define INCLUDE_USB_PEGASUS_END_INIT
#      endif /* INCLUDE_USB_PEGASUS_END */

#   endif /* INCLUDE_USBTOOL */
#endif /* INCLUDE_USB */

/* VxWorks AuxClock Support */

#ifdef INCLUDE_AUX_CLK                  /* auxClk depends on global timers */
#   define INCLUDE_MPC8540_TIMERS
#endif /* INCLUDE_AUX_CLK */

/* clock rates */

#define SYS_CLK_RATE_MIN        1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        8000    /* maximum system clock rate */
#define AUX_CLK_RATE            (FREQ_33_MHZ / (1 << 18))
#define AUX_CLK_RATE_MIN        (FREQ_33_MHZ / (1 << 21))
#define AUX_CLK_RATE_MAX        (FREQ_33_MHZ / (1 << 10))

/* Boot Image Programming Support */

#ifdef INCLUDE_BOOT_IMAGE_PROGRAM
#   define STANDALONE_NET
#endif /* INCLUDE_BOOT_IMAGE_PROGRAM */

/*
 * Error handling support
 *
 * If error handling support is included, we must also include support
 * for the real-time clock in order to provide time tags for the errors
 * which are logged.
 */

#ifdef INCLUDE_ERROR_HANDLING
#   define INCLUDE_RTC
#endif

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCsbc8540Ah */
