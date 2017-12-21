/* sdramDcr.h - IBM SDRAM controller DCR access assembly routines */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01e,03sep04,mdo  Documentation fixes for apigen
01d,12jun02,pch  SPR 74987: C++
01c,19nov01,pch  cleanup
01b,15may00,mcg  register name updates to match 405GP User Manual
01a,17jan00,mcg  created
*/

/*
This file contains the definitions of the SDRAM controller DCR register access
functions.
*/

#ifndef INCsdramDcrh
#define INCsdramDcrh

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * SDRAM controller register definitions.  Two DCRs are used (address/data) to
 * indirectly access the config registers of this controller.
 */

#define SDRAM_CFGADDR  (SDRAM_DCR_BASE+0x0)   /* SDRAM configuration adrs reg */
#define SDRAM_CFGDATA  (SDRAM_DCR_BASE+0x1)   /* SDRAM configuration data reg */

/*
* Values for SDRAM_CFGADDR register - indirect addressing of these regs
*/
#define SDRAM_BESR0   0x00    /* bus error syndrome reg 0            */
#define SDRAM_BESR1   0x08    /* bus error syndrome reg 1            */
#define SDRAM_BEAR    0x10    /* bus error address reg               */
#define SDRAM_CFG     0x20    /* memory controller config options    */
#define SDRAM_RTR     0x30    /* refresh timer reg                   */
#define SDRAM_PMIT    0x34    /* power management idle timer         */
#define SDRAM_B0CR    0x40    /* memory bank 0 configuration         */
#define SDRAM_B1CR    0x44    /* memory bank 1 configuration         */
#define SDRAM_B2CR    0x48    /* memory bank 2 configuration         */
#define SDRAM_B3CR    0x4c    /* memory bank 3 configuration         */
#define SDRAM_TR      0x80    /* timing reg 1                        */
#define SDRAM_ECCCFG  0x94    /* ECC configuration                   */
#define SDRAM_ECCESR  0x98    /* ECC error status                    */


/*
 *  SDRAM - Memory Controller Config Options (SDRAM_CFG) Reg
 */

#define SDRAM_CFG_DC_EN       0x80000000  /* SDRAM Controller Enable          */
#define SDRAM_CFG_SLFREFEN    0x40000000  /* Self-refresh enable              */
#define SDRAM_CFG_PM_EN       0x20000000  /* Power-management enable          */
#define SDRAM_CFG_MEMCHK      0x18000000  /* Memory data error checking mask  */
#define SDRAM_CFG_MEMCHK_ECC  0x10000000  /* ECC                              */
#define SDRAM_CFG_DRAMWDTH    0x06000000  /* DRAM width mask                  */
#define SDRAM_CFG_DRAMWDTH_32 0x00000000  /* 32 bits                          */
#define SDRAM_CFG_DRAMWDTH_64 0x02000000  /* 64 bits                          */
#define SDRAM_CFG_BRD_PRF     0x01800000  /* PLB Burst prefetch granular mask */
#define SDRAM_CFG_BRD_PRF_8   0x00000000  /* 8 bytes                          */
#define SDRAM_CFG_BRD_PRF_16  0x00800000  /* 16 bytes                         */
#define SDRAM_CFG_BRD_PRF_32  0x01000000  /* 32 bytes                         */

/*
 *  SDRAM - Memory Bank 0-3 configuration (for SDRAM_BxCR registers)
 */

#define SDRAM_BXCR_SDX_BA      0xffc00000          /* Base address            */
#define SDRAM_BXCR_SDX_SZ      0x000e0000          /* Size                    */
#define SDRAM_BXCR_SDX_SZ_4    0x00000000          /*   4M                    */
#define SDRAM_BXCR_SDX_SZ_8    0x00020000          /*   8M                    */
#define SDRAM_BXCR_SDX_SZ_16   0x00040000          /*  16M                    */
#define SDRAM_BXCR_SDX_SZ_32   0x00060000          /*  32M                    */
#define SDRAM_BXCR_SDX_SZ_64   0x00080000          /*  64M                    */
#define SDRAM_BXCR_SDX_SZ_128  0x000a0000          /* 128M                    */
#define SDRAM_BXCR_SDX_SZ_256  0x000c0000          /* 256M                    */
#define SDRAM_BXCR_SDX_SZ_512  0x000e0000          /* 512M                    */
#define SDRAM_BXCR_SDX_AM      0x0000e000          /* Addressing mode         */
#define SDRAM_BXCR_SDX_AM_1    0x00000000          /*   Mode 1                */
#define SDRAM_BXCR_SDX_AM_2    0x00002000          /*   Mode 2                */
#define SDRAM_BXCR_SDX_AM_3    0x00004000          /*   Mode 3                */
#define SDRAM_BXCR_SDX_AM_4    0x00006000          /*   Mode 4                */
#define SDRAM_BXCR_SDX_AM_5    0x00008000          /*   Mode 5                */
#define SDRAM_BXCR_SDX_AM_6    0x0000a000          /*   Mode 6                */
#define SDRAM_BXCR_SDX_AM_7    0x0000c000          /*   Mode 7                */
#define SDRAM_BXCR_SDX_BE      0x00000001          /* Memory Bank Enable      */

#ifndef _ASMLANGUAGE

/*
 *  SDRAM Controller DCR access function (read only)
 *  To access SDRAM controller registers, a DCR register pair (address + data)
 *  is used.
 */

UINT32  sysDcrSdramGet(UINT32 sdramReg);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif  /* INCsdramDcrh */
