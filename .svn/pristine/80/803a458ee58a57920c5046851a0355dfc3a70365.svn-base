/* sysNandFlash.h - Hearder File of Nand Flash Driver for MPC830x mixt */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,12ocr11,e_d add eieio instruction to write register. (WIND00306790) 
01a,30jun11,e_d initial creation
*/

#ifndef __INCsysNandFlashh
#define __INCsysNandFlashh

/* Read/Write Register */

#define FCM_REG_WRITE_ULONG(addr,value)  \
        do                               \
          {                              \
          *(volatile unsigned long *)(addr) = (value); \
          WRS_ASM("eieio");              \
          }while(0)                     

#define FCM_REG_READ_ULONG(addr)         \
          *(volatile unsigned long *)(addr)
   
         
#define FCM_REG_WRITE_USHORT(addr,value) \
        do                               \
          {                              \
          *(volatile unsigned short *)(addr) = (value); \
          WRS_ASM("eieio");              \
          }while(0)

#define FCM_REG_READ_USHORT(addr)        \
          *(volatile unsigned short *)(addr)

#define FCM_REG_WRITE_CHAR(addr,value)   \
        do                               \
          {                              \
          *(volatile unsigned char *)(addr) = (value);  \
          WRS_ASM("eieio");              \
          }while(0)       

#define FCM_REG_READ_CHAR(addr)          \
         *(volatile unsigned char *)(addr)

/* LTESR bit */

#define LTESR_BM        0x80000000
#define LTESR_FCT       0x40000000
#define LTESR_PAR       0x20000000
#define LTESR_WP        0x04000000
#define LTESR_ATMW      0x00800000
#define LTESR_ATMR      0x00400000
#define LTESR_CS        0x00080000
#define LTESR_CC        0x00000001

/* FMR bit */

#define FMR_CWTO        16
#define FMR_BOOT        20
#define FMR_ECCM        23
#define FMR_AL          26
#define FMR_OP          30

#define FMR_1M_LCLK     0x0000C000

#define FMR_AL_2BYTE    0x00000000
#define FMR_AL_3BYTE    0x00000010
#define FMR_AL_4BYTE    0x00000020

#define FMR_OP_NORMAL   0x00000000
#define FMR_OP_SIMBOOT  0x00000001
#define FMR_OP_EXEINPOT 0x00000002
#define FMR_OP_EXENOPOT 0x00000003

/* FIR bit */

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

#define FIR_NOP_OPX(x) (FIR_NOP << x)
#define FIR_CA_OPX(x)  (FIR_CA << x)
#define FIR_PA_OPX(x)  (FIR_PA << x)
#define FIR_UA_OPX(x)  (FIR_UA << x)
#define FIR_CM0_OPX(x) (FIR_CM0 << x)
#define FIR_CM1_OPX(x) (FIR_CM1 << x)
#define FIR_CM2_OPX(x) (FIR_CM2 << x)
#define FIR_CM3_OPX(x) (FIR_CM3 << x)
#define FIR_WB_OPX(x)  (FIR_WB << x)
#define FIR_WS_OPX(x)  (FIR_WS << x)
#define FIR_RB_OPX(x)  (FIR_RB << x)
#define FIR_RS_OPX(x)  (FIR_RS << x)
#define FIR_CW0_OPX(x) (FIR_CW0 << x)
#define FIR_CW1_OPX(x) (FIR_CW1 << x)
#define FIR_RBW_OPX(x) (FIR_RBW << x)
#define FIR_RSW_OPX(x) (FIR_RSW << x)

/* MDR bit */

#define MDR_AS3(x)     (x << (31 - 7))
#define MDR_AS2(x)     (x << (31 - 15))
#define MDR_AS1(x)     (x << (31 - 23))
#define MDR_AS0(x)     x

/* FCR bit */

#define FCR_CMD0(x)    (x << (31 - 7))
#define FCR_CMD1(x)    (x << (31 - 15))
#define FCR_CMD2(x)    (x << (31 - 23))
#define FCR_CMD3(x)    x

/* FBAR bit */

#define FBAR_BLK       8

/* FPAR bit in small mode */

#define NAND_LARGE_MODE

#ifndef NAND_LARGE_MODE

#define FPAR_PI        21
#define FPAR_MS        22
#define FPAR_CI        31

/* FPAR bit in Large mode */

#else /* NAND_LARGE_MODE */
#define FPAR_PI        19
#define FPAR_MS        20
#endif /* NAND_LARGE_MODE */

/* For large mode, only buf0 and buf1 can be used */

#define FPAR_PI_FBUF0  (0 << (31 - FPAR_PI))
#define FPAR_PI_FBUF1  (1 << (31 - FPAR_PI))
#define FPAR_PI_FBUF2  (2 << (31 - FPAR_PI))
#define FPAR_PI_FBUF3  (3 << (31 - FPAR_PI))
#define FPAR_PI_FBUF4  (4 << (31 - FPAR_PI))
#define FPAR_PI_FBUF5  (5 << (31 - FPAR_PI))
#define FPAR_PI_FBUF6  (6 << (31 - FPAR_PI))
#define FPAR_PI_FBUF7  (7 << (31 - FPAR_PI))

#define FPAR_MS_MAIN   (0 << (31 - FPAR_MS))
#define FPAR_MS_SPARE  (1 << (31 - FPAR_MS))

/* Nand flash command define */

#define MICRO_NAND_RESET       0xFF
#define MICRO_NAND_READID      0x90
#define MICRO_NAND_GETFEA      0xEE
#define MICRO_NAND_SETFEA      0xEF
#define MICRO_NAND_READSTS     0x70
#define MICRO_NAND_READFIRST   0x00
#define MICRO_NAND_READSEC     0x30
#define MICRO_NAND_WRITEFIRST  0x80
#define MICRO_NAND_WRITESEC    0x10
#define MICRO_NAND_ERASEFIRST  0x60
#define MICRO_NAND_ERASESEC    0xd0

#define MICRO_NAND_PAGEMASK    0xFFFFF800
#define MICRO_NAND_PAGESIZE    2048
#define MICRO_NAND_BLOCKMASK   0xFFF20000
#define MICRO_NAND_BLOCKSIZE   2048 * 64
#define MICRO_NAND_PAGESET(x)  (((x) << 12 ) & 0x0003F000)
#define MICRO_NAND_BLOCKSET(x) ( x & 0x00FFFFFF)
#define MICRO_NAND_PAGECAL(x)  (((x) & MICRO_NAND_PAGEMASK) / MICRO_NAND_PAGESIZE)
#define MICRO_NAND_BLOCKCAL(x) (((x) & MICRO_NAND_BLOCKMASK) / MICRO_NAND_BLOCKSIZE)

#define TIMEOUT                0x100000

#endif    /* __INCsysNandFlashh */


