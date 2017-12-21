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
01f,29sep04,mdo  Documentation fixes for apigen
01e,13aug03,jtp  Formal code inspection changes: add macros for bank & mem size.
01d,12jul02,jtp  SPR 75726 SDRAM autoconfiguration & ECC support
01c,12jun02,pch  SPR 74987: C++
01b,04dec01,mcg  changed SDRAM0_WRDTR to SDRAM0_WDDCTR, TR1 and TR2 to
                 TR0 and TR1, CFG1 and CFG2 to CFG0 and CFG1, removed
                 sysDcrSdramGet function prototype.
01a,24oct01,jtp  created from walnut vn 01b
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
#define SDRAM0_CFGADDR	(SDRAM0_DCR_BASE+0x0)	/* Memory configuration address*/
#define SDRAM0_CFGDATA	(SDRAM0_DCR_BASE+0x1)	/* Memory configuration data   */

/*
 * Values for SDRAM_CFGADDR register - indirect addressing of these regs
 */
#define SDRAM0_BESR0   0x00    /* plb bus error syndrome reg a		    */
#define SDRAM0_BESRS0  0x04    /* plb bus error syndrome reg set a	    */
#define SDRAM0_BESR1   0x08    /* plb bus error syndrome reg b		    */
#define SDRAM0_BESRS1  0x0c    /* plb bus error syndrome reg set b	    */
#define SDRAM0_BEAR    0x10    /* plb bus error address reg		    */
#define SDRAM0_MIRQ    0x11    /* plb master write interrupt		    */
#define SDRAM0_MIRQS   0x12    /* plb master write interrupt set	    */
#define SDRAM0_SLIO    0x18    /* plb slave interface options		    */
#define SDRAM0_CFG0    0x20    /* memory controller options 1		    */
#define SDRAM0_CFG1    0x21    /* memory controller options 2		    */
#define SDRAM0_DEVOPT  0x22    /* DDR SDRAM device options		    */
#define SDRAM0_MCSTS   0x24    /* memory controller status register	    */
#define SDRAM0_RTR     0x30    /* refresh timer reg			    */
#define SDRAM0_PMIT    0x34    /* power management idle timer		    */
#define SDRAM0_UABBA   0x38    /* PLB UABus Base Address reg		    */
#define SDRAM0_B0CR    0x40    /* memory bank 0 configuration		    */
#define SDRAM0_B1CR    0x44    /* memory bank 1 configuration		    */
#define SDRAM0_B2CR    0x48    /* memory bank 2 configuration		    */
#define SDRAM0_B3CR    0x4c    /* memory bank 3 configuration		    */
#define SDRAM0_TR0     0x80    /* DDR SDRAM timing reg 1		    */
#define SDRAM0_TR1     0x81    /* DDR SDRAM timing reg 2		    */
#define SDRAM0_CLKTR   0x82    /* DDR clock timing register		    */
#define SDRAM0_WDDCTR  0x83    /* write data/DM/DQS clock timing reg	    */
#define SDRAM0_DLYCAL  0x84    /* delay line calibration		    */
#define SDRAM0_ECCESR  0x98    /* ECC error status			    */
#define SDRAM0_CID     0xA4    /* Core ID register			    */
#define SDRAM0_RID     0xA8    /* Revision ID register			    */


/*
 *  SDRAM - Memory Controller Config Options (SDRAM_CFG0) Reg
 */

#define SDRAM_CFG0_DCEN           0x80000000  /* SDRAM Controller Enable      */
#define SDRAM_CFG0_MCHK_MASK      0x30000000  /* Memory data error check mask*/
#define SDRAM_CFG0_MCHK_NON       0x00000000  /* No ECC generation            */
#define SDRAM_CFG0_MCHK_GEN       0x20000000  /* ECC generation               */
#define SDRAM_CFG0_MCHK_CHK       0x30000000  /* ECC generation and checking  */
#define SDRAM_CFG0_RDEN           0x08000000  /* Registered DIMM enable       */
#define SDRAM_CFG0_PMUD           0x04000000  /* Page management unit         */
#define SDRAM_CFG0_DMWD_MASK      0x02000000  /* DRAM width mask              */
#define SDRAM_CFG0_DMWD_32        0x00000000  /* 32 bits                      */
#define SDRAM_CFG0_DMWD_64        0x02000000  /* 64 bits                      */
#define SDRAM_CFG0_UIOS_MASK      0x00C00000  /* Unused IO State              */
#define SDRAM_CFG0_PDP            0x00200000  /* Page deallocation policy     */

/*
 *  SDRAM - Memory Controller Config Options (SDRAM_CFG1) Reg
 */

#define SDRAM_CFG1_SRE            0x80000000  /* Self-Refresh Entry          */
#define SDRAM_CFG1_PMEN           0x40000000  /* Power Management Enable     */

/*
 *  SDRAM - Memory Bank 0-3 configuration (for SDRAM_BxCR registers)
 */

#define SDRAM_BXCR_SDBA_MASK      0xff800000      /* Base address             */
#define SDRAM_BXCR_SDSZ_MASK      0x000e0000      /* Size                     */
#define SDRAM_BXCR_SDSZ_8         0x00020000      /*   8M                     */
#define SDRAM_BXCR_SDSZ_16        0x00040000      /*  16M                     */
#define SDRAM_BXCR_SDSZ_32        0x00060000      /*  32M                     */
#define SDRAM_BXCR_SDSZ_64        0x00080000      /*  64M                     */
#define SDRAM_BXCR_SDSZ_128       0x000a0000      /* 128M                     */
#define SDRAM_BXCR_SDSZ_256       0x000c0000      /* 256M                     */
#define SDRAM_BXCR_SDSZ_512       0x000e0000      /* 512M                     */
#define SDRAM_BXCR_SDAM_MASK      0x0000e000      /* Addressing mode          */
#define SDRAM_BXCR_SDAM_1         0x00000000      /*   Mode 1                 */
#define SDRAM_BXCR_SDAM_2         0x00002000      /*   Mode 2                 */
#define SDRAM_BXCR_SDAM_3         0x00004000      /*   Mode 3                 */
#define SDRAM_BXCR_SDAM_4         0x00006000      /*   Mode 4                 */
#define SDRAM_BXCR_SDBE           0x00000001      /* Memory Bank Enable       */

#define SDRAM_BXCR_NUM_REGS	  4	      	 /* 4 BXCR regs on chip */
#define SDRAM_BXCR_MAX_SIZE	  0x20000000     /* 512MB per bank */

/*
 * maximum size of memory SDRAM controller can support
 */

#define SDRAM_MAX_SIZE	    (SDRAM_BXCR_NUM_REGS * (UINT32)SDRAM_BXCR_MAX_SIZE)

/*
 * SDRAM -  Bus Error Syndrome registers
 */

#define SDRAM_BESR0_M0ET_MASK 0xE0000000      /* Master 0 error type	     */
#define SDRAM_BESR0_M0ET_ECC  0x40000000      /*   ECC			     */
#define SDRAM_BESR0_M0RWS     0x10000000      /* Master 0 Read/Write Status  */
#define SDRAM_BESR0_M0FL      0x08000000      /* Master 0 BESR Field Lock    */
#define SDRAM_BESR0_M0AL      0x04000000      /* Master 0 BEAR Address Lock  */
#define SDRAM_BESR0_M1ET_MASK 0x03800000      /* Master 1 error type	     */
#define SDRAM_BESR0_M1ET_ECC  0x01000000      /*   ECC			     */
#define SDRAM_BESR0_M1RWS     0x00400000      /* Master 1 Read/Write Status  */
#define SDRAM_BESR0_M1FL      0x00200000      /* Master 1 BESR Field Lock    */
#define SDRAM_BESR0_M1AL      0x00100000      /* Master 1 BEAR Address Lock  */
#define SDRAM_BESR0_M2ET_MASK 0x000e0000      /* Master 2 error type	     */
#define SDRAM_BESR0_M2ET_ECC  0x00040000      /*   ECC			     */
#define SDRAM_BESR0_M2RWS     0x00010000      /* Master 2 Read/Write Status  */
#define SDRAM_BESR0_M2FL      0x00008000      /* Master 2 BESR Field Lock    */
#define SDRAM_BESR0_M2AL      0x00004000      /* Master 2 BEAR Address Lock  */
#define SDRAM_BESR0_M3ET_MASK 0x00003800      /* Master 3 error type	     */
#define SDRAM_BESR0_M3ET_ECC  0x00001000      /*   ECC			     */
#define SDRAM_BESR0_M3RWS     0x00000400      /* Master 3 Read/Write Status  */
#define SDRAM_BESR0_M3FL      0x00000200      /* Master 3 BESR Field Lock    */
#define SDRAM_BESR0_M3AL      0x00000100      /* Master 3 BEAR Address Lock  */

#define SDRAM_BESR1_M4ET_MASK 0xE0000000      /* Master 4 error type	     */
#define SDRAM_BESR1_M4ET_ECC  0x40000000      /*   ECC			     */
#define SDRAM_BESR1_M4RWS     0x10000000      /* Master 4 Read/Write Status  */
#define SDRAM_BESR1_M4FL      0x08000000      /* Master 4 BESR Field Lock    */
#define SDRAM_BESR1_M4AL      0x04000000      /* Master 4 BEAR Address Lock  */
#define SDRAM_BESR1_M5ET_MASK 0x03800000      /* Master 5 error type	     */
#define SDRAM_BESR1_M5ET_ECC  0x01000000      /*   ECC			     */
#define SDRAM_BESR1_M5RWS     0x00400000      /* Master 5 Read/Write Status  */
#define SDRAM_BESR1_M5FL      0x00200000      /* Master 5 BESR Field Lock    */
#define SDRAM_BESR1_M5AL      0x00100000      /* Master 5 BEAR Address Lock  */
#define SDRAM_BESR1_M6ET_MASK 0x000e0000      /* Master 6 error type	     */
#define SDRAM_BESR1_M6ET_ECC  0x00040000      /*   ECC			     */
#define SDRAM_BESR1_M6RWS     0x00010000      /* Master 6 Read/Write Status  */
#define SDRAM_BESR1_M6FL      0x00008000      /* Master 6 BESR Field Lock    */
#define SDRAM_BESR1_M6AL      0x00004000      /* Master 6 BEAR Address Lock  */
#define SDRAM_BESR1_M7ET_MASK 0x00003800      /* Master 7 error type	     */
#define SDRAM_BESR1_M7ET_ECC  0x00001000      /*   ECC			     */
#define SDRAM_BESR1_M7RWS     0x00000400      /* Master 7 Read/Write Status  */
#define SDRAM_BESR1_M7FL      0x00000200      /* Master 7 BESR Field Lock    */
#define SDRAM_BESR1_M7AL      0x00000100      /* Master 7 BEAR Address Lock  */

/*
 * SDRAM - Slave Interface Options Reg
 */

#define SDRAM_SLIO_RD_RE      0x80000000
#define SDRAM_SLIO_WR_RE      0x40000000
#define SDRAM_SLIO_RARW	      0x20000000

/*
 * SDRAM - Memory Controller Options 1
 */

#define SDRAM_CFG0_DC_EN      0x80000000      /* SDRAM Controller Enable        */
#define SDRAM_CFG0_MEMCHK     0x30000000      /* Memory data error checking mask*/
#define SDRAM_CFG0_MEMCHK_NON 0x00000000      /* No ECC generation            */
#define SDRAM_CFG0_MEMCHK_GEN 0x20000000      /* ECC generation               */
#define SDRAM_CFG0_MEMCHK_CHK 0x30000000      /* ECC generation and checking  */
#define SDRAM_CFG0_R_DIMM_EN  0x08000000      /* Registered DIMM enable       */
#define SDRAM_CFG0_PMU_DIS    0x04000000      /* Page management unit         */
#define SDRAM_CFG0_DRAMWDTH   0x02000000      /* DRAM width mask              */
#define SDRAM_CFG0_DRAMWDTH_32 0x00000000     /* 32 bits                      */
#define SDRAM_CFG0_DRAMWDTH_64 0x02000000     /* 64 bits                      */
#define SDRAM_CFG0_UUIOS      0x00C00000      /* Unused IO State              */
#define SDRAM_CFG0_PMU_DP     0x00200000      /* Page deallocation policy     */

/*
 * SDRAM - Memory Controller Options 2
 */

#define SDRAM_CFG1_SLFREFEN   0x80000000      /* Self-Refresh Entry          */
#define SDRAM_CFG1_PM_EN      0x40000000      /* Power Management Enable     */

/*
 * SDRAM - DEVOPT Options
 */

#define SDRAM_DEVOPT_DLL      0x80000000
#define SDRAM_DEVOPT_DS	      0x40000000

/*
 * SDRAM - MCSTS Options
 */

#define SDRAM_MCSTS_MRSC      0x80000000
#define SDRAM_MCSTS_SRMS      0x40000000
#define SDRAM_MCSTS_CIS       0x20000000

/*
 * SDRAM - Refresh Timer Register
 */

#define SDRAM_RTR_RINT            0xFFFF0000
#define SDRAM_RTR_RINT_ENCODE(n)  ((((unsigned long)(n))&0xFFFF)<<16)
#define SDRAM_RTR_RINT_DECODE(n)  ((((unsigned long)(n))>>16)&0xFFFF)

/*
 * SDRAM - UABus Base Address Reg
 */

#define SDRAM_UABBA_ADDR	      0x0000000F
 
/*
 * SDRAM - Memory Bank 0-7 configuration
 */

#define SDRAM_MBXCF_SDX_BA     0xffc00000      /* Base address                */
#define SDRAM_MBXCF_SDX_SZ     0x000e0000      /* Size                        */
#define SDRAM_MBXCF_SDX_SZ_4   0x00000000      /*   4M                        */
#define SDRAM_MBXCF_SDX_SZ_8   0x00020000      /*   8M                        */
#define SDRAM_MBXCF_SDX_SZ_16  0x00040000      /*  16M                        */
#define SDRAM_MBXCF_SDX_SZ_32  0x00060000      /*  32M                        */
#define SDRAM_MBXCF_SDX_SZ_64  0x00080000      /*  64M                        */
#define SDRAM_MBXCF_SDX_SZ_128 0x000a0000      /* 128M                        */
#define SDRAM_MBXCF_SDX_SZ_256 0x000c0000      /* 256M                        */
#define SDRAM_MBXCF_SDX_SZ_512 0x000e0000      /* 512M                        */
#define SDRAM_MBXCF_SDX_AM     0x0000e000      /* Addressing mode             */
#define SDRAM_MBXCF_SDX_AM_1   0x00000000      /*   Mode 1                    */
#define SDRAM_MBXCF_SDX_AM_2   0x00002000      /*   Mode 2                    */
#define SDRAM_MBXCF_SDX_AM_3   0x00004000      /*   Mode 3                    */
#define SDRAM_MBXCF_SDX_AM_4   0x00006000      /*   Mode 4                    */
#define SDRAM_MBXCF_SDX_AM_5   0x00008000      /*   Mode 5                    */
#define SDRAM_MBXCF_SDX_AM_6   0x0000a000      /*   Mode 6                    */
#define SDRAM_MBXCF_SDX_AM_7   0x0000c000      /*   Mode 7                    */
#define SDRAM_MBXCF_SDX_BE     0x00000001      /* Memory Bank Enable          */

/*
 * SDRAM - TR0 Options
 */

#define SDRAM_TR0_SDWR_MASK        0x80000000
#define   SDRAM_TR0_SDWR_2_CLK     0x00000000
#define   SDRAM_TR0_SDWR_3_CLK     0x80000000
#define SDRAM_TR0_SDWD_MASK        0x40000000
#define   SDRAM_TR0_SDWD_0_CLK     0x00000000
#define   SDRAM_TR0_SDWD_1_CLK     0x40000000
#define SDRAM_TR0_SDCL_MASK        0x01800000
#define   SDRAM_TR0_SDCL_2_0_CLK   0x00800000
#define   SDRAM_TR0_SDCL_2_5_CLK   0x01000000
#define   SDRAM_TR0_SDCL_3_0_CLK   0x01800000
#define SDRAM_TR0_SDPA_MASK        0x000C0000
#define   SDRAM_TR0_SDPA_2_CLK     0x00040000
#define   SDRAM_TR0_SDPA_3_CLK     0x00080000
#define   SDRAM_TR0_SDPA_4_CLK     0x000C0000
#define SDRAM_TR0_SDCP_MASK        0x00030000
#define   SDRAM_TR0_SDCP_2_CLK     0x00000000
#define   SDRAM_TR0_SDCP_3_CLK     0x00010000
#define   SDRAM_TR0_SDCP_4_CLK     0x00020000
#define   SDRAM_TR0_SDCP_5_CLK     0x00030000
#define SDRAM_TR0_SDLD_MASK        0x0000C000
#define   SDRAM_TR0_SDLD_1_CLK     0x00000000
#define   SDRAM_TR0_SDLD_2_CLK     0x00004000
#define SDRAM_TR0_SDRA_MASK        0x0000001C
#define   SDRAM_TR0_SDRA_6_CLK     0x00000000
#define   SDRAM_TR0_SDRA_7_CLK     0x00000004
#define   SDRAM_TR0_SDRA_8_CLK     0x00000008
#define   SDRAM_TR0_SDRA_9_CLK     0x0000000C
#define   SDRAM_TR0_SDRA_10_CLK    0x00000010
#define   SDRAM_TR0_SDRA_11_CLK    0x00000014
#define   SDRAM_TR0_SDRA_12_CLK    0x00000018
#define   SDRAM_TR0_SDRA_13_CLK    0x0000001C
#define SDRAM_TR0_SDRD_MASK        0x00000003
#define   SDRAM_TR0_SDRD_2_CLK     0x00000001
#define   SDRAM_TR0_SDRD_3_CLK     0x00000002
#define   SDRAM_TR0_SDRD_4_CLK     0x00000003


/*
 * SDRAM - TR1 Options
 */

#define SDRAM_TR1_RDSS_MASK         0xC0000000
#define   SDRAM_TR1_RDSS_TR0        0x00000000
#define   SDRAM_TR1_RDSS_TR1        0x40000000
#define   SDRAM_TR1_RDSS_TR2        0x80000000
#define   SDRAM_TR1_RDSS_TR3        0xC0000000
#define SDRAM_TR1_RDSL_MASK         0x00C00000
#define   SDRAM_TR1_RDSL_STAGE1     0x00000000
#define   SDRAM_TR1_RDSL_STAGE2     0x00400000
#define   SDRAM_TR1_RDSL_STAGE3     0x00800000
#define SDRAM_TR1_RDCD_MASK         0x00000800
#define   SDRAM_TR1_RDCD_RCD_0_0    0x00000000
#define   SDRAM_TR1_RDCD_RCD_1_2    0x00000800
#define SDRAM_TR1_RDCT_MASK         0x000001FF
#define   SDRAM_TR1_RDCT_ENCODE(x)  (((x) << 0) & SDRAM_TR1_RDCT_MASK)
#define   SDRAM_TR1_RDCT_DECODE(x)  (((x) & SDRAM_TR1_RDCT_MASK) >> 0)
#define   SDRAM_TR1_RDCT_MIN        0x00000000
#define   SDRAM_TR1_RDCT_MAX        0x000001FF

/*
 * SDRAM - WDDCTR Options
 */

#define SDRAM_WDDCTR_WRCP           0xC0000000
#define   SDRAM_WDDCTR_WRCP_0DEG    0x00000000
#define   SDRAM_WDDCTR_WRCP_90DEG   0x40000000
#define   SDRAM_WDDCTR_WRCP_180DEG  0x80000000
#define SDRAM_WDDCTR_DCD            0x000001FF
#define   SDRAM_WDDCTR_DCD_ENCODE(n) ((((unsigned long)(n))&0x1FF)<<0)
#define   SDRAM_WDDCTR_DCD_DECODE(n) ((((unsigned long)(n))>>0)&0x1FF)

/*
 * SDRAM - CLKTR Options
 */

#define SDRAM_CLKTR_CLKP	    0xC0000000
#define   SDRAM_CLKTR_CLKP_0DEG     0x00000000
#define   SDRAM_CLKTR_CLKP_90DEG    0x40000000
#define   SDRAM_CLKTR_CLKP_180DEG   0x80000000
#define SDRAM_CLKTR_DDRCLT	    0x000001FF
#define SDRAM_CLKTR_DCDT            0x000001FF
#define   SDRAM_CLKTR_DCDT_ENCODE(n) ((((unsigned long)(n))&0x1FF)<<0)
#define   SDRAM_CLKTR_DCDT_DECODE(n) ((((unsigned long)(n))>>0)&0x1FF)

/*
 * SDRAM - DLYCAL Options
 */

#define SDRAM_DLYCAL_DLCV_MASK   0x000003FC
#define   SDRAM_DLYCAL_DLCV_ENCODE(x) (((x)<<2) & SDRAM_DLYCAL_DLCV_MASK)
#define   SDRAM_DLYCAL_DLCV_DECODE(x) (((x) & SDRAM_DLYCAL_DLCV_MASK)>>2)

/*
 * SDRAM - ECCESR Options
 */

#define SDRAM_ECCESR_BnCE_MASK   0xffff0000
#define SDRAM_ECCESR_CKBE_MASK   0x0000c000
#define SDRAM_ECCESR_CE	         0x00002000
#define SDRAM_ECCESR_UE	         0x00001000
#define SDRAM_ECCESR_BKnE_MASK   0x00000f00

#ifdef __cplusplus
    }
#endif

#endif  /* INCsdramDcrh */
