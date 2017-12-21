/* ebcDcr.h - IBM external bus controller DCR definitions */

/*
*******************************************************************************
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

   COPYRIGHT   I B M   CORPORATION 2003
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

****************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01f,31aug04,mdo  Documentation fixes for apigen
01e,10jul03,jtp  Share with other ppc4xx BSPs; add encode/decode macros
01d,12jun02,pch  SPR 74987: C++
01c,18dec01,mcg  changed register names, added bit definitions
01b,19nov01,pch  cleanup
01a,30may00,mcg  created
*/

/*
This file contains the definitions of the external bus controller DCR register
access functions for EBC2PLB3 core.
*/

#ifndef INCebcDcrh
#define INCebcDcrh

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * External Bus Controller (EBC0) DCR Registers.
 */

#define EBC0_CFGADDR  (EBC0_DCR_BASE+0x0)  /* EBCO Controller Addr Reg     */
#define EBC0_CFGDATA  (EBC0_DCR_BASE+0x1)  /* EBCO Controller Data Reg     */

/* Indirect access registers for EBC0 -- use EBC0_CFGADDR to access */

#define EBC0_B0CR     0x00  /* EBCO Bank 0 Config Register */
#define EBC0_B1CR     0x01  /* EBCO Bank 1 Config Register */
#define EBC0_B2CR     0x02  /* EBCO Bank 2 Config Register */
#define EBC0_B3CR     0x03  /* EBCO Bank 3 Config Register */
#define EBC0_B4CR     0x04  /* EBCO Bank 4 Config Register */
#define EBC0_B5CR     0x05  /* EBCO Bank 5 Config Register */
#define EBC0_B6CR     0x06  /* EBCO Bank 6 Config Register */
#define EBC0_B7CR     0x07  /* EBCO Bank 7 Config Register */
#define EBC0_B0AP     0x10  /* EBCO Bank 0 Access Parameters */
#define EBC0_B1AP     0x11  /* EBCO Bank 1 Access Parameters */
#define EBC0_B2AP     0x12  /* EBCO Bank 2 Access Parameters */
#define EBC0_B3AP     0x13  /* EBCO Bank 3 Access Parameters */
#define EBC0_B4AP     0x14  /* EBCO Bank 4 Access Parameters */
#define EBC0_B5AP     0x15  /* EBCO Bank 5 Access Parameters */
#define EBC0_B6AP     0x16  /* EBCO Bank 6 Access Parameters */
#define EBC0_B7AP     0x17  /* EBCO Bank 7 Access Parameters */
#define EBC0_BEAR     0x20  /* EBCO Bus Error Address Register */
#ifdef PPC440EP
#define EBC0_BESR0    0x21  /* EBCO Bus Error Status Register 0 */
#define EBC0_BESR1    0x22  /* EBCO Bus Error Status Register 1 */
#else /* PPC440EP */
#define EBC_BESR      0x21  /* EBC0 Bus Error Status Register */
#endif /* PPC440EP */
#define EBC0_CFG      0x23  /* EBCO Peripheral Control Register */
#define EBC0_CID      0x24  /* EBCO Peripheral Core ID Register */

/* Bits for BxCR registers */
#define EBC_BXCR_BAS      0xFFF00000      /* Base address select              */
#define   BAS_BITSTOSHIFT   20
#define   EBC_BXCR_BAS_ENCODE(n)	((((unsigned long)(n))&0xFFF00000)<<0)
#define EBC_BXCR_BS       0x000E0000      /* Bank size mask                   */
#define   EBC_BXCR_BS_1MB    0x00000000   /* Bank size 1MB                    */
#define   EBC_BXCR_BS_2MB    0x00020000   /* Bank size 2MB                    */
#define   EBC_BXCR_BS_4MB    0x00040000   /* Bank size 4MB                    */
#define   EBC_BXCR_BS_8MB    0x00060000   /* Bank size 8MB                    */
#define   EBC_BXCR_BS_16MB   0x00080000   /* Bank size 16MB                   */
#define   EBC_BXCR_BS_32MB   0x000A0000   /* Bank size 32MB                   */
#define   EBC_BXCR_BS_64MB   0x000C0000   /* Bank size 64MB                   */
#define   EBC_BXCR_BS_128MB  0x000E0000   /* Bank size 128MB                  */
#define EBC_BXCR_BU       0x00018000      /* Bank usage mask                  */
#define   EBC_BXCR_BU_DIS    0x00000000   /* Bank usage disabled              */
#define   EBC_BXCR_BU_RO     0x00008000   /* Bank usage read-only             */
#define   EBC_BXCR_BU_WO     0x00010000   /* Bank usage write-only            */
#define   EBC_BXCR_BU_RW     0x00018000   /* Bank usage read-write            */
#define EBC_BXCR_BW       0x00006000      /* Bus Width mask                   */
#define   EBC_BXCR_BW_8      0x00000000   /* Bus Width 8-bit                  */
#define   EBC_BXCR_BW_16     0x00002000   /* Bus Width 16-bit                 */
#define   EBC_BXCR_BW_32     0x00006000   /* Bus Width 32-bit                 */

/* Bits for BxAP registers */
#define EBC_BXAP_BME      0x80000000      /* Burst Mode Enable                */
#define   EBC_BXAP_BME_EN  0x80000000
#define   EBC_BXAP_BME_DIS 0x00000000
#define EBC_BXAP_TWT      0x7F800000      /* Non-burst transfer wait           */
#define   TWT_BITSTOSHIFT   23
#define   EBC_BXAP_TWT_ENCODE(n)  	((((unsigned long)(n))&0xFF)<<23)
#define EBC_BXAP_FWT      0x7C000000      /* Burst first wait                 */
#define   FWT_BITSTOSHIFT   26
#define EBC_BXAP_BWT      0x03800000      /* Burst wait                       */
#define   BWT_BITSTOSHIFT   23
#define EBC_BXAP_BCE      0x00400000      /* Burst fixed length reads         */
#define   EBC_BXAP_BCE_DISABLE 0x00000000
#define   EBC_BXAP_BCE_ENABLE  0x00400000
#define EBC_BXAP_BCT      0x00300000      /* Burst fixed length count         */
#define   EBC_BXAP_BCT_2     0x00000000   /* Burst fixed length count 2       */
#define   EBC_BXAP_BCT_4     0x00100000   /* Burst fixed length count 4       */
#define   EBC_BXAP_BCT_8     0x00200000   /* Burst fixed length count 8       */
#define   EBC_BXAP_BCT_16    0x00300000   /* Burst fixed length count 16      */
#define EBC_BXAP_CSN      0x000C0000      /* Chip Select On Timing            */
#define   EBC_BXAP_CSN_0     0x00000000   /* Chip Select On Timing 0 cycles   */
#define   EBC_BXAP_CSN_1     0x00040000   /* Chip Select On Timing 1 cycles   */
#define   EBC_BXAP_CSN_2     0x00080000   /* Chip Select On Timing 2 cycles   */
#define   EBC_BXAP_CSN_3     0x000C0000   /* Chip Select On Timing 3 cycles   */
#define   EBC_BXAP_CSN_ENCODE(n)  	((((unsigned long)(n))&0x3)<<18)
#define EBC_BXAP_OEN      0x00030000      /* Output Enable On Timing          */
#define   EBC_BXAP_OEN_0     0x00000000   /* Output Enable On Timing 0 cycles */
#define   EBC_BXAP_OEN_1     0x00010000   /* Output Enable On Timing 1 cycles */
#define   EBC_BXAP_OEN_2     0x00020000   /* Output Enable On Timing 2 cycles */
#define   EBC_BXAP_OEN_3     0x00030000   /* Output Enable On Timing 3 cycles */
#define   EBC_BXAP_OEN_ENCODE(n)  	((((unsigned long)(n))&0x3)<<16)
#define EBC_BXAP_WBN      0x0000C000      /* Write Byte Enable On Timing      */
#define   EBC_BXAP_WBN_0     0x00000000   /* Write Byte Enable On Timing 0    */
#define   EBC_BXAP_WBN_1     0x00004000   /* Write Byte Enable On Timing 1    */
#define   EBC_BXAP_WBN_2     0x00008000   /* Write Byte Enable On Timing 2    */
#define   EBC_BXAP_WBN_3     0x0000C000   /* Write Byte Enable On Timing 3    */
#define   EBC_BXAP_WBN_ENCODE(n)  	((((unsigned long)(n))&0x3)<<14)
#define EBC_BXAP_WBF      0x00003000      /* Write Byte Enable Off Timing     */
#define   EBC_BXAP_WBF_0     0x00000000   /* Write Byte Enable Off Timing 0   */
#define   EBC_BXAP_WBF_1     0x00001000   /* Write Byte Enable Off Timing 1   */
#define   EBC_BXAP_WBF_2     0x00002000   /* Write Byte Enable Off Timing 2   */
#define   EBC_BXAP_WBF_3     0x00003000   /* Write Byte Enable Off Timing 3   */
#define   EBC_BXAP_WBF_ENCODE(n)  	((((unsigned long)(n))&0x3)<<12)
#define EBC_BXAP_TH       0x00000E00      /* Transfer Hold                    */
#define   EBC_BXAP_TH_0      0x00000000   /* Transfer Hold 0 cycles           */
#define   EBC_BXAP_TH_1      0x00000200   /* Transfer Hold 1 cycles           */
#define   EBC_BXAP_TH_2      0x00000400   /* Transfer Hold 2 cycles           */
#define   EBC_BXAP_TH_3      0x00000600   /* Transfer Hold 3 cycles           */
#define   EBC_BXAP_TH_4      0x00000800   /* Transfer Hold 4 cycles           */
#define   EBC_BXAP_TH_5      0x00000A00   /* Transfer Hold 5 cycles           */
#define   EBC_BXAP_TH_6      0x00000C00   /* Transfer Hold 6 cycles           */
#define   EBC_BXAP_TH_7      0x00000E00   /* Transfer Hold 7 cycles           */
#define   EBC_BXAP_TH_ENCODE(n)   	((((unsigned long)(n))&0x7)<<9)
#define EBC_BXAP_RE       0x00000100      /* Ready Enable                     */
#define   EBC_BXAP_RE_DIS 0x00000000      
#define   EBC_BXAP_RE_EN  0x00000100
#define EBC_BXAP_SOR      0x00000080      /* Sample On Ready device paced     */
#define   EBC_BXAP_SOR_DELAYED    0x00000000
#define   EBC_BXAP_SOR_NONDELAYED 0x00000080
#define EBC_BXAP_BEM      0x00000040      /* Byte Enable Mode                 */
#define   EBC_BXAP_BEM_W     0x00000000   /* Byte Enable Mode Write only      */
#define   EBC_BXAP_BEM_RW    0x00000040   /* Byte Enable Mode Read and Write  */
#define EBC_BXAP_PAR      0x00000020      /* Parity Enable                    */
#define EBC_BXAP_NO_PAR   0x00000000      /* Parity Disabled                  */

#ifdef PPC440EP
/* Bits for BESR0 register */
#define EBC_BESR0_EET0       0xE0000000   /* Error type for master 0          */
#define   EBC_BESR0_EET0_NO  0x00000000   /* No error                         */
#define   EBC_BESR0_EET0_PTE 0x20000000   /* Parity error                     */
#define   EBC_BESR0_EET0_PRE 0x80000000   /* Protection error                 */
#define   EBC_BESR0_EET0_IE  0xC0000000   /* External Bus Input error         */
#define   EBC_BESR0_EET0_TOE 0xE0000000   /* External Bus Time out error      */
#define EBC_BESR0_RWS0       0x10000000   /* Read/write status for master 0   */
#define EBC_BESR0_FL0        0x08000000   /* Field lock for master 0          */
#define EBC_BESR0_AL0        0x04000000   /* BEAR address lock for master 0   */
#define EBC_BESR0_EET1       0x03800000   /* Error type for master 1          */
#define   EBC_BESR0_EET1_NO  0x00000000   /* No error                         */
#define   EBC_BESR0_EET1_PTE 0x00800000   /* Parity error                     */
#define   EBC_BESR0_EET1_PRE 0x02000000   /* Protection error                 */
#define   EBC_BESR0_EET1_IE  0x03000000   /* External Bus Input error         */
#define   EBC_BESR0_EET1_TOE 0x03800000   /* External Bus Time out error      */
#define EBC_BESR0_RWS1       0x00400000   /* Read/write status for master 1   */
#define EBC_BESR0_FL1        0x00200000   /* Field lock for master 1          */
#define EBC_BESR0_AL1        0x00100000   /* BEAR address lock for master 1   */
#define EBC_BESR0_EET2       0x000E0000   /* Error type for master 2          */
#define   EBC_BESR0_EET2_NO  0x00000000   /* No error                         */
#define   EBC_BESR0_EET2_PTE 0x00020000   /* Parity error                     */
#define   EBC_BESR0_EET2_PRE 0x00080000   /* Protection error                 */
#define   EBC_BESR0_EET2_IE  0x000C0000   /* External Bus Input error         */
#define   EBC_BESR0_EET2_TOE 0x000E0000   /* External Bus Time out error      */
#define EBC_BESR0_RWS2       0x00010000   /* Read/write status for master 2   */
#define EBC_BESR0_FL2        0x00008000   /* Field lock for master 2          */
#define EBC_BESR0_AL2        0x00004000   /* BEAR address lock for master 2   */
#define EBC_BESR0_EET3       0x00003800   /* Error type for master 3          */
#define   EBC_BESR0_EET3_NO  0x00000000   /* No error                         */
#define   EBC_BESR0_EET3_PTE 0x00000800   /* Parity error                     */
#define   EBC_BESR0_EET3_PRE 0x00002000   /* Protection error                 */
#define   EBC_BESR0_EET3_IE  0x00003000   /* External Bus Input error         */
#define   EBC_BESR0_EET3_TOE 0x00003800   /* External Bus Time out error      */
#define EBC_BESR0_RWS3       0x00000400   /* Read/write status for master 3   */
#define EBC_BESR0_FL3        0x00000200   /* Field lock for master 3          */
#define EBC_BESR0_AL3        0x00000100   /* BEAR address lock for master 3   */

/* Bits for BESR1 register */
#define EBC_BESR1_EET4       0xE0000000   /* Error type for master 4          */
#define   EBC_BESR1_EET4_NO  0x00000000   /* No error                         */
#define   EBC_BESR1_EET4_PTE 0x20000000   /* Parity error                     */
#define   EBC_BESR1_EET4_PRE 0x80000000   /* Protection error                 */
#define   EBC_BESR1_EET4_IE  0xC0000000   /* External Bus Input error         */
#define   EBC_BESR1_EET4_TOE 0xE0000000   /* External Bus Time out error      */
#define EBC_BESR1_RWS4       0x10000000   /* Read/write status for master 4   */
#define EBC_BESR1_FL4        0x08000000   /* Field lock for master 4          */
#define EBC_BESR1_AL4        0x04000000   /* BEAR address lock for master 4   */
#define EBC_BESR1_EET5       0x03800000   /* Error type for master 5          */
#define   EBC_BESR1_EET5_NO  0x00000000   /* No error                         */
#define   EBC_BESR1_EET5_PTE 0x00800000   /* Parity error                     */
#define   EBC_BESR1_EET5_PRE 0x02000000   /* Protection error                 */
#define   EBC_BESR1_EET5_IE  0x03000000   /* External Bus Input error         */
#define   EBC_BESR1_EET5_TOE 0x03800000   /* External Bus Time out error      */
#define EBC_BESR1_RWS5       0x00400000   /* Read/write status for master 5   */
#define EBC_BESR1_FL5        0x00200000   /* Field lock for master 5          */
#define EBC_BESR1_AL5        0x00100000   /* BEAR address lock for master 5   */
#define EBC_BESR1_EET6       0x000E0000   /* Error type for master 6          */
#define   EBC_BESR1_EET6_NO  0x00000000   /* No error                         */
#define   EBC_BESR1_EET6_PTE 0x00020000   /* Parity error                     */
#define   EBC_BESR1_EET6_PRE 0x00080000   /* Protection error                 */
#define   EBC_BESR1_EET6_IE  0x000C0000   /* External Bus Input error         */
#define   EBC_BESR1_EET6_TOE 0x000E0000   /* External Bus Time out error      */
#define EBC_BESR1_RWS6       0x00010000   /* Read/write status for master 6   */
#define EBC_BESR1_FL6        0x00008000   /* Field lock for master 6          */
#define EBC_BESR1_AL6        0x00004000   /* BEAR address lock for master 6   */
#define EBC_BESR1_EET7       0x00003800   /* Error type for master 7          */
#define   EBC_BESR1_EET7_NO  0x00000000   /* No error                         */
#define   EBC_BESR1_EET7_PTE 0x00000800   /* Parity error                     */
#define   EBC_BESR1_EET7_PRE 0x00002000   /* Protection error                 */
#define   EBC_BESR1_EET7_IE  0x00003000   /* External Bus Input error         */
#define   EBC_BESR1_EET7_TOE 0x00003800   /* External Bus Time out error      */
#define EBC_BESR1_RWS7       0x00000400   /* Read/write status for master 7   */
#define EBC_BESR1_FL7        0x00000200   /* Field lock for master 7          */
#define EBC_BESR1_AL7        0x00000100   /* BEAR address lock for master 7   */
#else /* PPC440EP */
/* Bits for BESR register */
#define EBC_BESR_PYE      0x80000000      /* Parity Error Detected on Read    */
#define EBC_BESR_PRE      0x40000000      /* Bank Protection Error            */
#define EBC_BESR_EBE      0x20000000      /* PerErr asserted                  */
#define EBC_BESR_ETE      0x10000000      /* External Bus Timeout             */
#define EBC_BESR_RWS      0x08000000      /* Read/Write Error Status          */
#define   EBC_BESR_RWS_W     0x00000000   /* Write Error Status               */
#define   EBC_BESR_RWS_R     0x08000000   /* Read  Error Status               */
#define EBC_BESR_SAS      0x04000000      /* Sequential Address Status        */
#define EBC_BESR_EINT     0x02000000      /* Error Input                      */
#define EBC_BESR_DMAEP    0x01000000      /* DMA External Peripheral Error    */
#endif /* PPC440EP */

/* Bits for CFG register */
#ifdef PPC440EP
#define EBC_CFG_ETC       0x80000000      /* External Bus Three-state control */
#else /* PPC440EP */
#define EBC_CFG_LE        0x80000000      /* Lock Error                       */
#define   EBC_CFG_LE_UNLOCK 0x00000000
#define   EBC_CFG_LE_LOCK   0x80000000
#endif /* PPC440EP */
#define EBC_CFG_PTD       0x40000000      /* Device-Paced Time-out Disable    */
#define   EBC_CFG_PTD_EN  0x00000000      /* Enable timeouts                  */
#define   EBC_CFG_PTD_DIS 0x40000000      /* Disable timeouts                 */
#define EBC_CFG_RTC       0x38000000      /* Ready Timeout count              */
#define   EBC_CFG_RTC_16     0x00000000   /* Ready Timeout count 16 cycles    */
#define   EBC_CFG_RTC_32     0x08000000   /* Ready Timeout count 32 cycles    */
#define   EBC_CFG_RTC_64     0x10000000   /* Ready Timeout count 64 cycles    */
#define   EBC_CFG_RTC_128    0x18000000   /* Ready Timeout count 128 cycles   */
#define   EBC_CFG_RTC_256    0x20000000   /* Ready Timeout count 256 cycles   */
#define   EBC_CFG_RTC_512    0x28000000   /* Ready Timeout count 512 cycles   */
#define   EBC_CFG_RTC_1024   0x30000000   /* Ready Timeout count 1024 cycles  */
#define   EBC_CFG_RTC_2048   0x38000000   /* Ready Timeout count 2048 cycles  */
#ifdef PPC440EP
#define EBC_CFG_EMPH         0x06000000   /* External Master Priority (sig High)  */
#define   EBC_CFG_EMPH_P0    0x00000000   /* Priority 0                       */
#define   EBC_CFG_EMPH_P1    0x02000000   /* Priority 1                       */
#define   EBC_CFG_EMPH_P2    0x04000000   /* Priority 2                       */
#define   EBC_CFG_EMPH_P3    0x06000000   /* Priority 3                       */
#define EBC_CFG_EMPL         0x01800000   /* External Master Priority (sig Low)   */
#define   EBC_CFG_EMPL_P0    0x00000000   /* Priority 0                       */
#define   EBC_CFG_EMPL_P1    0x00800000   /* Priority 1                       */
#define   EBC_CFG_EMPL_P2    0x01000000   /* Priority 2                       */
#define   EBC_CFG_EMPL_P3    0x01800000   /* Priority 3                       */
#define EBC_CFG_CSTC         0x00400000   /* Chip Select Three State Control  */
#define EBC_CFG_BPR          0x00300000   /* Burst Prefetch                   */
#define   EBC_CFG_BPR_8      0x00000000   /* 8bytes                           */
#define   EBC_CFG_BPR_16     0x00100000   /* 16bytes                          */
#define   EBC_CFG_BPR_32     0x00200000   /* 32bytes                          */
#define EBC_CFG_EMS          0x000C0000   /* Ext Master Size                  */
#define   EBC_CFG_EMS_NO     0x000C0000   /* No external Master               */
#define   EBC_CFG_EMS_32     0x00080000   /* 32bits external Master           */
#define   EBC_CFG_EMS_16     0x00040000   /* 16bits external Master           */
#define   EBC_CFG_EMS_8      0x00000000   /* 8bits external Master            */
#define EBC_CFG_PME          0x00020000   /* Power Management Enable          */
#define EBC_CFG_PMT          0x0001F000   /* Power Management Timer           */
#else /* PPC440EP */

#define EBC_CFG_ATC       0x04000000      /* Adrs Bus High Impedance Control  */
#define   EBC_CFG_ATC_HI       0x00000000
#define   EBC_CFG_ATC_PREVIOUS 0x04000000
#define EBC_CFG_DTC       0x02000000      /* Data Bus High Impedance Control  */
#define   EBC_CFG_DTC_HI       0x00000000
#define   EBC_CFG_DTC_PREVIOUS 0x02000000
#define EBC_CFG_CTC       0x01000000      /* Cntl Sig High Impedance Control  */
#define   EBC_CFG_CTC_HI       0x00000000
#define   EBC_CFG_CTC_PREVIOUS 0x01000000
#define EBC_CFG_OEO       0x00800000      /* Ext Bus Override High Impedance  */
#define   EBC_CFG_OEO_HI       0x00000000
#define   EBC_CFG_OEO_PREVIOUS 0x00800000
#define EBC_CFG_EMC       0x00400000      /* Ext Master High Impedance Cntrl  */
#define   EBC_CFG_EMC_NONDEFAULT 0x00000000
#define   EBC_CFG_EMC_DEFAULT    0x00400000
#define EBC_CFG_PME       0x00020000      /* Power Management Enable          */
#define   EBC_CFG_PME_DISABLE    0x00000000
#define   EBC_CFG_PME_ENABLE     0x00200000
#define EBC_CFG_PMT       0x0001F000      /* Power Management Timer           */
#define   EBC_CFG_PMT_ENCODE(n)  		((((unsigned long)(n))&0x1F)<<12)
#define EBC_CFG_PR        0x00000C00      /* Pending Request Timer            */
#define   EBC_CFG_PR_16      0x00000000   /* Pending Request Timer 16 cycles  */
#define   EBC_CFG_PR_32      0x00000400   /* Pending Request Timer 32 cycles  */
#define   EBC_CFG_PR_64      0x00000800   /* Pending Request Timer 64 cycles  */
#define   EBC_CFG_PR_128     0x00000C00   /* Pending Request Timer 128 cycles */
#endif  /* PPC440EP */

#ifdef __cplusplus
    }
#endif

#endif  /* INCebcDcrh */
