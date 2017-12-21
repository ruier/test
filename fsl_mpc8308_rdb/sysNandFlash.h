/* sysNandFlash.h - Hearder File of Nand Flash Driver for MPC8313ERDB */

/*
 * Copyright (c) 2008,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,19jan10,syt add PVR and SVR macros.
01b,05mar08,z_l modify for code convention.
01a,18Jan08,l_g initial creation
*/

#ifndef __INCsysNandFlashh
#define __INCsysNandFlashh

/* Read/Write Register */

#define FCM_REG_WRITE_ULONG(addr,value)  \
          *(volatile unsigned long *)(addr) = (value)
          
#define FCM_REG_READ_ULONG(addr)         \
          *(volatile unsigned long *)(addr)

#define FCM_REG_WRITE_USHORT(addr,value) \
          *(volatile unsigned short *)(addr) = (value)
          
#define FCM_REG_READ_USHORT(addr)        \
          *(volatile unsigned short *)(addr)

#define FCM_REG_WRITE_CHAR(addr,value)   \
          *(volatile unsigned char *)(addr) = (value)
          
#define FCM_REG_READ_CHAR(addr)          \
         *(volatile unsigned char *)(addr)

/* SOC PVR and SVR */

#define PPC_PVR_85XX    0x80200000
#define PPC_PVR_E500V2  0x80210000
#define PPC_PVR_MASK    0xFFFF0000

#define PPC_SVR_P2020   0x80E00000
#define PPC_SVR_MASK    0xFFF00000

/* Register define */

#define SPECIAL_OP_INIT_REG              (CCSBAR + 0x5090) 
#define TRANSFER_ERROR_ATT_REG           (CCSBAR + 0x50BC)
#define TRANSFER_ERROR_STU_REG           (CCSBAR + 0x50B0)
#define FCM_DATA_REG                     (CCSBAR + 0x5088)
#define FLASH_MODE_REG                   (CCSBAR + 0x50E0)
#define FLASH_INSTRUCTION_REG            (CCSBAR + 0x50E4)
#define FLASH_COMMAND_REG                (CCSBAR + 0x50E8)
#define FLASH_BLOCKADDR_REG              (CCSBAR + 0x50EC)
#define FLASH_PAGEADDR_REG               (CCSBAR + 0x50F0)
#define FLASH_BYTECOUNT_REG              (CCSBAR + 0x50F4) 

#define LTESR_BM        0x80000000
#define LTESR_FCT       0x40000000
#define LTESR_PAR       0x20000000
#define LTESR_WP        0x04000000
#define LTESR_ATMW      0x00800000
#define LTESR_ATMR      0x00400000
#define LTESR_CS        0x00080000
#define LTESR_CC        0x00000001

#define FMR_1M_LCLK     0x0000C000

#define FIR_OP0_SHIFT   28
#define FIR_OP1_SHIFT   24
#define FIR_OP2_SHIFT   20
#define FIR_OP3_SHIFT   16
#define FIR_OP4_SHIFT   12
#define FIR_OP5_SHIFT   8
#define FIR_OP6_SHIFT   4
#define FIR_OP7_SHIFT   0

#define FIR_NOP         0x0
#define FIR_CA          0x1
#define FIR_PA          0x2
#define FIR_UA          0x3
#define FIR_CM0         0x4
#define FIR_CM1         0x5
#define FIR_CM2         0x6
#define FIR_CM3         0x7
#define FIR_WB          0x8
#define FIR_WS          0x9
#define FIR_RB          0xA
#define FIR_RS          0xB
#define FIR_CW0         0xC
#define FIR_CW1         0xD
#define FIR_RBW         0xE
#define FIR_RSW         0xF

#define FIR_OP0_CW0    0xC0000000
#define FIR_OP0_WS     0x90000000
#define FIR_OP0_CM2    0x60000000
#define FIR_OP0_CA     0x10000000
#define FIR_OP0_RSW    0xF0000000
#define FIR_OP0_WB     0x80000000

#define FIR_OP1_UA     0x03000000
#define FIR_OP1_CA     0x01000000
#define FIR_OP1_PA     0x02000000
#define FIR_OP1_RSW    0x0F000000

#define FIR_OP2_RBW    0x00E00000
#define FIR_OP2_PA     0x00200000
#define FIR_OP2_CA     0x00100000
#define FIR_OP2_UA     0x00300000
#define FIR_OP2_CM1    0x00500000
#define FIR_OP2_CW1    0x00D00000

#define FIR_OP3_RBW    0x000E0000
#define FIR_OP3_WB     0x00080000

#define FIR_OP4_CW1    0x0000D000

#define FCR_CMD0_WS_NOP  FIR_OP0_WS
#define FCR_CMD0_CM2_CW1 (FIR_OP0_CM2 | FIR_OP2_CW1)
#define FCR_CMD0_RSW_RSW (FIR_OP0_RSW | FIR_OP1_RSW)
#define FCR_CMD0_WB_NOP  FIR_OP0_WB
#define FCR_CMD1_CA_NOP  FIR_OP2_CA
#define FCR_CMD1_UA_NOP  FIR_OP2_UA


#endif    /* __INCsysNandFlashh */


