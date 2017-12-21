/* dmaDcr.h - IBM DMA controller DCR access assembly routines */

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

/* Copyright 2001-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01f,04nov04,dr   Implementing 440EP registers.
01e,31aug04,mdo  Documentation fixes for apigen
01d,10jul03,jtp  Share with other PPC440 BSPs
01c,12jun02,pch  SPR 74987: C++
01b,18dec01,mcg  changed names of DMAx_CT to DMAx_CTC to match User Manual
01a,24oct01,jtp  ported to 440, based on walnut dmaDcr.h v. 01a
*/

/*
This file contains the definitions of the Device Control Register (DCR)
access functions of the IBM DMA controller core, as implemented on the
processor.
*/

#ifndef INCdmaDcrh
#define INCdmaDcrh

#ifdef __cplusplus
    extern "C" {
#endif

/*
 * DMA Controller register definitions. Each is a separate DCR register.
 */

#ifdef PPC440EP

/*
 * PLB3 DMA controller
 */
#define DMA0_CR0   (DMA0_DCR_BASE+0x00)  /* channel control register 0        */
#define DMA0_CT0   (DMA0_DCR_BASE+0x01)  /* count register 0                  */
#define DMA0_DA0   (DMA0_DCR_BASE+0x02)  /* destination address register 0    */
#define DMA0_SA0   (DMA0_DCR_BASE+0x03)  /* source address register 0         */
#define DMA0_SG0   (DMA0_DCR_BASE+0x04)  /* scatter/gather descriptor addr 0  */
#define DMA0_CR1   (DMA0_DCR_BASE+0x08)  /* channel control register 1        */
#define DMA0_CT1   (DMA0_DCR_BASE+0x09)  /* count register 1                  */
#define DMA0_DA1   (DMA0_DCR_BASE+0x0a)  /* destination address register 1    */
#define DMA0_SA1   (DMA0_DCR_BASE+0x0b)  /* source address register 1         */
#define DMA0_SG1   (DMA0_DCR_BASE+0x0c)  /* scatter/gather descriptor addr 1  */
#define DMA0_CR2   (DMA0_DCR_BASE+0x10)  /* channel control register 2        */
#define DMA0_CT2   (DMA0_DCR_BASE+0x11)  /* count register 2                  */
#define DMA0_DA2   (DMA0_DCR_BASE+0x12)  /* destination address register 2    */
#define DMA0_SA2   (DMA0_DCR_BASE+0x13)  /* source address register 2         */
#define DMA0_SG2   (DMA0_DCR_BASE+0x14)  /* scatter/gather descriptor addr 2  */
#define DMA0_CR3   (DMA0_DCR_BASE+0x18)  /* channel control register 3        */
#define DMA0_CT3   (DMA0_DCR_BASE+0x19)  /* count register 3                  */
#define DMA0_DA3   (DMA0_DCR_BASE+0x1a)  /* destination address register 3    */
#define DMA0_SA3   (DMA0_DCR_BASE+0x1b)  /* source address register 3         */
#define DMA0_SG3   (DMA0_DCR_BASE+0x1c)  /* scatter/gather descriptor addr 3  */
#define DMA0_SR    (DMA0_DCR_BASE+0x20)  /* status register                   */
#define DMA0_SGC   (DMA0_DCR_BASE+0x23)  /* scatter/gather command register   */
#define DMA0_SLP   (DMA0_DCR_BASE+0x25)  /* sleep mode                        */
#define DMA0_POL   (DMA0_DCR_BASE+0x26)  /* polarity                          */
/* DMA channel control register bits (same for all channels) */
#define DMA0_CR_CE         0x80000000    /* channel enable                     */
#define DMA0_CR_CIE        0x40000000    /* channel interrupt enable           */
#define DMA0_CR_TD         0x20000000    /* transfer direction                 */
#define DMA0_CR_PL         0x10000000    /* peripheral location                */
#define DMA0_CR_PW         0x0C000000    /* peripheral width mask              */
#define DMA0_CR_PW_BYTE    0x00000000    /* peripheral width byte              */
#define DMA0_CR_PW_HW      0x04000000    /* peripheral width halfword          */
#define DMA0_CR_PW_WORD    0x08000000    /* peripheral width word              */
#define DMA0_CR_PW_DW      0x0C000000    /* peripheral width doubleword        */
#define DMA0_CR_DAI        0x02000000    /* destination address increment      */
#define DMA0_CR_SAI        0x01000000    /* source address increment           */
#define DMA0_CR_BEN        0x00800000    /* buffer enable                      */
#define DMA0_CR_TM         0x00600000    /* transfer mode mask                 */
#define DMA0_CR_TM_PER     0x00000000    /* transfer mode peripheral           */
#define DMA0_CR_TM_SWMM    0x00400000    /* transfer mode SW memory-to-memory  */
#define DMA0_CR_TM_HWDP    0x00600000    /* transfer mode HW deviced paced     */
#define DMA0_CR_PSC        0x00180000    /* peripheral setup cycles mask       */
#define DMA0_CR_PSC_NONE   0x00000000    /* peripheral setup cycles 0          */
#define DMA0_CR_PSC_1      0x00080000    /* peripheral setup cycles 1          */
#define DMA0_CR_PSC_2      0x00100000    /* peripheral setup cycles 2          */
#define DMA0_CR_PSC_3      0x00180000    /* peripheral setup cycles 3          */
#define DMA0_CR_PWC        0x0007E000    /* peripheral wait cycles mask        */
#define DMA0_CR_PHC        0x00001C00    /* peripheral hold cycles mask        */
#define DMA0_CR_ETD        0x00000200    /* end-of-transfer / terminal count   */
#define DMA0_CR_TCE        0x00000100    /* terminal count enable              */
#define DMA0_CR_CP         0x000000C0    /* channel priority mask              */
#define DMA0_CR_CP_LOW     0x00000000    /* channel priority low               */
#define DMA0_CR_CP_MEDL    0x000000C0    /* channel priority medium-low        */
#define DMA0_CR_CP_MEDH    0x000000C0    /* channel priority medium-high       */
#define DMA0_CR_CP_HIGH    0x000000C0    /* channel priority high              */
#define DMA0_CR_PF         0x00000030    /* memory read prefetch mask          */
#define DMA0_CR_PF_1       0x00000000    /* memory read prefetch 1 doubleword  */
#define DMA0_CR_PF_2       0x00000010    /* memory read prefetch 2 doubleword  */
#define DMA0_CR_PF_4       0x00000020    /* memory read prefetch 4 doubleword  */
#define DMA0_CR_PCE        0x00000008    /* parity check enable                */
#define DMA0_CR_DEC        0x00000004    /* address decrement                  */

/* DMA status register bits */
#define DMA0_SR_CS         0xF0000000    /* channel terminal count mask        */
#define DMA0_SR_CS_0       0x80000000    /* channel 0 terminal count           */
#define DMA0_SR_CS_1       0x40000000    /* channel 1 terminal count           */
#define DMA0_SR_CS_2       0x20000000    /* channel 2 terminal count           */
#define DMA0_SR_CS_3       0x10000000    /* channel 3 terminal count           */
#define DMA0_SR_TS         0x0F000000    /* channel end-of-transfer status mask*/
#define DMA0_SR_TS_0       0x08000000    /* channel 0 end-of-transfer status   */
#define DMA0_SR_TS_1       0x04000000    /* channel 1 end-of-transfer status   */
#define DMA0_SR_TS_2       0x02000000    /* channel 2 end-of-transfer status   */
#define DMA0_SR_TS_3       0x01000000    /* channel 3 end-of-transfer status   */
#define DMA0_SR_RI         0x00F00000    /* channel error status mask          */
#define DMA0_SR_RI_0       0x00800000    /* channel 0 error status             */
#define DMA0_SR_RI_1       0x00400000    /* channel 1 error status             */
#define DMA0_SR_RI_2       0x00200000    /* channel 2 error status             */
#define DMA0_SR_RI_3       0x00100000    /* channel 3 error status             */
#define DMA0_SR_IR         0x000F0000    /* channel internal request mask      */
#define DMA0_SR_IR_0       0x00080000    /* channel 0 internal request         */
#define DMA0_SR_IR_1       0x00040000    /* channel 1 internal request         */
#define DMA0_SR_IR_2       0x00020000    /* channel 2 internal request         */
#define DMA0_SR_IR_3       0x00010000    /* channel 3 internal request         */
#define DMA0_SR_ER         0x0000F000    /* channel external request mask      */
#define DMA0_SR_ER_0       0x00008000    /* channel 0 external request         */
#define DMA0_SR_ER_1       0x00004000    /* channel 1 external request         */
#define DMA0_SR_ER_2       0x00002000    /* channel 2 external request         */
#define DMA0_SR_ER_3       0x00001000    /* channel 3 external request         */
#define DMA0_SR_CB         0x00000F00    /* channel busy mask                  */
#define DMA0_SR_CB_0       0x00000800    /* channel 0 busy                     */
#define DMA0_SR_CB_1       0x00000400    /* channel 1 busy                     */
#define DMA0_SR_CB_2       0x00000200    /* channel 2 busy                     */
#define DMA0_SR_CB_3       0x00000100    /* channel 3 busy                     */
#define DMA0_SR_SG         0x000000F0    /* channel scatter/gather status mask */
#define DMA0_SR_SG_0       0x00000080    /* channel 0 scatter/gather status    */
#define DMA0_SR_SG_1       0x00000040    /* channel 1 scatter/gather status    */
#define DMA0_SR_SG_2       0x00000020    /* channel 2 scatter/gather status    */
#define DMA0_SR_SG_3       0x00000010    /* channel 3 scatter/gather status    */

/* DMA scatter/gather command register bits */
#define DMA0_SGC_SSG0      0x80000000    /* channel 0 scatter/gather enable    */
#define DMA0_SGC_SSG1      0x40000000    /* channel 1 scatter/gather enable    */
#define DMA0_SGC_SSG2      0x20000000    /* channel 2 scatter/gather enable    */
#define DMA0_SGC_SSG3      0x10000000    /* channel 3 scatter/gather enable    */
#define DMA0_SGC_EM0       0x00008000    /* channel 0 scatter/gather mask      */
#define DMA0_SGC_EM1       0x00004000    /* channel 1 scatter/gather mask      */
#define DMA0_SGC_EM2       0x00002000    /* channel 2 scatter/gather mask      */
#define DMA0_SGC_EM3       0x00001000    /* channel 3 scatter/gather mask      */

/* DMA Polarity Config Reg bits */
#define DMA0_POL_R0P       0x80000000    /* DMAReq0 Polarity                   */
#define DMA0_POL_A0P       0x40000000    /* DMAAck0 Polarity                   */
#define DMA0_POL_E0P       0x20000000    /* EOT0[TC0] Polarity                 */
#define DMA0_POL_R1P       0x10000000    /* DMAReq1 Polarity                   */
#define DMA0_POL_A1P       0x08000000    /* DMAAck1 Polarity                   */
#define DMA0_POL_E1P       0x04000000    /* EOT1[TC1] Polarity                 */
#define DMA0_POL_R2P       0x02000000    /* DMAReq2 Polarity                   */
#define DMA0_POL_A2P       0x01000000    /* DMAAck2 Polarity                   */
#define DMA0_POL_E2P       0x00800000    /* EOT2[TC2] Polarity                 */
#define DMA0_POL_R3P       0x00400000    /* DMAReq3 Polarity                   */
#define DMA0_POL_A3P       0x00200000    /* DMAAck3 Polarity                   */
#define DMA0_POL_E3P       0x00100000    /* EOT3[TC3] Polarity                 */


/*
 * PLB4 DMA controller
 */
#define DMA1_CR0  (DMA1_DCR_BASE+0x00)  /* DMA channel control register 0     */
#define DMA1_CTC0 (DMA1_DCR_BASE+0x01)  /* DMA count register 0         */
#define DMA1_SAH0 (DMA1_DCR_BASE+0x02)  /* DMA source address high reg 0      */
#define DMA1_SAL0 (DMA1_DCR_BASE+0x03)  /* DMA source address low reg 0       */
#define DMA1_DAH0 (DMA1_DCR_BASE+0x04)  /* DMA destination address high reg 0 */
#define DMA1_DAL0 (DMA1_DCR_BASE+0x05)  /* DMA destination address low reg 0  */
#define DMA1_SGH0 (DMA1_DCR_BASE+0x06)  /* DMA scatter/gather desc high addr 0*/
#define DMA1_SGL0 (DMA1_DCR_BASE+0x07)  /* DMA scatter/gather desc low addr 0 */
#define DMA1_CR1  (DMA1_DCR_BASE+0x08)  /* DMA channel control register 1     */
#define DMA1_CTC1 (DMA1_DCR_BASE+0x09)  /* DMA count register 1         */
#define DMA1_SAH1 (DMA1_DCR_BASE+0x0A)  /* DMA source address high reg 1      */
#define DMA1_SAL1 (DMA1_DCR_BASE+0x0B)  /* DMA source address low reg 1       */
#define DMA1_DAH1 (DMA1_DCR_BASE+0x0C)  /* DMA destination address high reg 1 */
#define DMA1_DAL1 (DMA1_DCR_BASE+0x0D)  /* DMA destination address low reg 1  */
#define DMA1_SGH1 (DMA1_DCR_BASE+0x0E)  /* DMA scatter/gather desc high addr 1*/
#define DMA1_SGL1 (DMA1_DCR_BASE+0x0F)  /* DMA scatter/gather desc low addr 1 */
#define DMA1_CR2  (DMA1_DCR_BASE+0x10)  /* DMA channel control register 2     */
#define DMA1_CTC2 (DMA1_DCR_BASE+0x11)  /* DMA count register 2         */
#define DMA1_SAH2 (DMA1_DCR_BASE+0x12)  /* DMA source address high reg 2      */
#define DMA1_SAL2 (DMA1_DCR_BASE+0x13)  /* DMA source address low reg 2       */
#define DMA1_DAH2 (DMA1_DCR_BASE+0x14)  /* DMA destination address high reg 2 */
#define DMA1_DAL2 (DMA1_DCR_BASE+0x15)  /* DMA destination address low reg 2  */
#define DMA1_SGH2 (DMA1_DCR_BASE+0x16)  /* DMA scatter/gather desc high addr 2*/
#define DMA1_SGL2 (DMA1_DCR_BASE+0x17)  /* DMA scatter/gather desc low addr 2 */
#define DMA1_CR3  (DMA1_DCR_BASE+0x18)  /* DMA channel control register 3     */
#define DMA1_CTC3 (DMA1_DCR_BASE+0x19)  /* DMA count register 3         */
#define DMA1_SAH3 (DMA1_DCR_BASE+0x1A)  /* DMA source address high reg 3      */
#define DMA1_SAL3 (DMA1_DCR_BASE+0x1B)  /* DMA source address low reg 3       */
#define DMA1_DAH3 (DMA1_DCR_BASE+0x1C)  /* DMA destination address high reg 3 */
#define DMA1_DAL3 (DMA1_DCR_BASE+0x1D)  /* DMA destination address low reg 3  */
#define DMA1_SGH3 (DMA1_DCR_BASE+0x1E)  /* DMA scatter/gather desc high addr 3*/
#define DMA1_SGL3 (DMA1_DCR_BASE+0x1F)  /* DMA scatter/gather desc low addr 3 */
#define DMA1_SR   (DMA1_DCR_BASE+0x20)  /* DMA status register          */
#define DMA1_SGC  (DMA1_DCR_BASE+0x23)  /* DMA scatter/gather command register*/
#define DMA1_SLP  (DMA1_DCR_BASE+0x25)  /* DMA sleep mode         */
#define DMA1_POL  (DMA1_DCR_BASE+0x26)  /* DMA polarity           */


/* DMA channel control register bits (same for all channels) */
#define DMA1_CR_CE     0x80000000   /* Channel Enable       */
#define DMA1_CR_CIE      0x40000000   /* Channel Interrupt Enable   */
#define DMA1_CR_TD     0x20000000   /* Transfer Direction       */
#define DMA1_CR_PL     0x10000000   /* Peripheral Location      */
#define DMA1_CR_PW_8     0x00000000   /* 8 bit peripheral width     */
#define DMA1_CR_PW_16      0x02000000   /* 16 bit peripheral width    */
#define DMA1_CR_PW_32      0x04000000   /* 32 bit peripheral width    */
#define DMA1_CR_PW_64      0x06000000   /* 64 bit peripheral width    */
#define DMA1_CR_PW_128     0x08000000
#define DMA1_CR_DAI      0x01000000   /* Destination Addr Increment */
#define DMA1_CR_SAI      0x00800000   /* Source Address Increment   */
#define DMA1_CR_BEN      0x00400000   /* Buffer Enable        */
#define DMA1_CR_TM     0x00300000
#define DMA1_CR_TM_BUFFERED    0x00000000   /* Peripheral         */
#define DMA1_CR_TM_SW_MEM_TO_MEM   0x00200000   /* Software started mem to mem*/
#define DMA1_CR_TM_HW_MEM_TO_MEM   0x00300000   /* Hardware paced mem to mem  */
#define DMA1_CR_PSC_0      0x00000000   /* 0 Peripheral Setup Cycles  */
#define DMA1_CR_PSC_1      0x00040000   /* 1 Peripheral Setup Cycles  */
#define DMA1_CR_PSC_2      0x00080000   /* 2 Peripheral Setup Cycles  */
#define DMA1_CR_PSC_3      0x000C0000   /* 3 Peripheral Setup Cycles  */
#define DMA1_CR_PSC(n) (((n)&0x3)<<18)        /* n Peripheral setup cycles  */
#define DMA1_CR_PWC(n) (((n)&0x3f)<<12)         /* n peripheral wait cycles   */
#define DMA1_CR_PHC(n) (((n)&0x7)<<9)         /* n peripheral hold cycles   */
#define DMA1_CR_ETD      0x00000100   /* EOT/TC Pin Direction       */
#define DMA1_CR_TCE      0x00000080   /* Terminal Count Enable      */
#define DMA1_CR_CP     0x00000060   /* Channel Priority       */
#define DMA1_CR_CP_0     0x00000000   /*   Low          */
#define DMA1_CR_CP_1     0x00000020   /*   Medium Low         */
#define DMA1_CR_CP_2     0x00000040   /*   Medium High        */
#define DMA1_CR_CP_3     0x00000060   /*   High         */
#define DMA1_CR_PF     0x00000018   /* Memory read prefetch trans */
#define DMA1_CR_PF_1     0x00000000   /*   Prefetch 1 dword       */
#define DMA1_CR_PF_2     0x00000008   /*   Prefetch 2 dword       */
#define DMA1_CR_PF_4     0x00000010   /*   Prefetch 4 dword       */
#define DMA1_CR_DEC      0x00000004   /* Address decrement      */
#define DMA1_CR_SL     0x00000002   /* Src Addr Location/Mem Loc  */

/* DMA status register bits */
#define DMA1_SR_CS0      0x80000000   /* Ch 0: terminal count status*/
#define DMA1_SR_CS1      0x40000000   /* Ch 1: terminal count status*/
#define DMA1_SR_CS2      0x20000000   /* Ch 2: terminal count status*/
#define DMA1_SR_CS3      0x10000000   /* Ch 3: terminal count status*/
#define DMA1_SR_TS0      0x08000000   /* Ch 0: End of Transfr status*/
#define DMA1_SR_TS1      0x04000000   /* Ch 1: End of Transfr status*/
#define DMA1_SR_TS2      0x02000000   /* Ch 2: End of Transfr status*/
#define DMA1_SR_TS3      0x01000000   /* Ch 3: End of Transfr status*/
#define DMA1_SR_RI0      0x00800000   /* Ch 0: Error Status       */
#define DMA1_SR_RI1      0x00400000   /* Ch 1: Error Status       */
#define DMA1_SR_RI2      0x00200000   /* Ch 2: Error Status       */
#define DMA1_SR_RI3      0x00100000   /* Ch 3: Error Status       */
#define DMA1_SR_IR0      0x00080000   /* Ch 0: Internal DMA request */
#define DMA1_SR_IR1      0x00040000   /* Ch 1: Internal DMA request */
#define DMA1_SR_IR2      0x00020000   /* Ch 2: Internal DMA request */
#define DMA1_SR_IR3      0x00010000   /* Ch 3: Internal DMA request */
#define DMA1_SR_ER0      0x00008000   /* Ch 0: External DMA request */
#define DMA1_SR_ER1      0x00004000   /* Ch 1: External DMA request */
#define DMA1_SR_ER2      0x00002000   /* Ch 2: External DMA request */
#define DMA1_SR_ER3      0x00001000   /* Ch 3: External DMA request */
#define DMA1_SR_CB0      0x00000800   /* Ch 0: Channel Busy       */
#define DMA1_SR_CB1      0x00000400   /* Ch 1: Channel Busy       */
#define DMA1_SR_CB2      0x00000200   /* Ch 2: Channel Busy       */
#define DMA1_SR_CB3      0x00000100   /* Ch 3: Channel Busy       */
#define DMA1_SR_SG0      0x00000080   /* Ch 0: Scatter/gather status*/
#define DMA1_SR_SG1      0x00000040   /* Ch 1: Scatter/gather status*/
#define DMA1_SR_SG2      0x00000020   /* Ch 2: Scatter/gather status*/
#define DMA1_SR_SG3      0x00000010   /* Ch 3: Scatter/gather status*/
#define DMA1_SR_ALL0 (DMA1_SR_CS0|DMA1_SR_TS0|DMA1_SR_RI0|DMA1_SR_IR0|DMA1_SR_ER0|DMA1_SR_CB0|DMA1_SR_SG0)
#define DMA1_SR_ALL1 (DMA1_SR_CS1|DMA1_SR_TS1|DMA1_SR_RI1|DMA1_SR_IR1|DMA1_SR_ER1|DMA1_SR_CB1|DMA1_SR_SG1)
#define DMA1_SR_ALL2 (DMA1_SR_CS2|DMA1_SR_TS2|DMA1_SR_RI2|DMA1_SR_IR2|DMA1_SR_ER2|DMA1_SR_CB2|DMA1_SR_SG2)
#define DMA1_SR_ALL3 (DMA1_SR_CS3|DMA1_SR_TS3|DMA1_SR_RI3|DMA1_SR_IR3|DMA1_SR_ER3|DMA1_SR_CB3|DMA1_SR_SG3)


/* DMA scatter/gather command register bits */
#define DMA1_SGC_SSG0 0x80000000
#define DMA1_SGC_SSG1 0x40000000
#define DMA1_SGC_SSG2 0x20000000
#define DMA1_SGC_SSG3 0x10000000

/* DMA Polarity Config Reg bits */
#define DMA1_POL_R0P   0x80000000  /* DMAReq0 Polarity         */
#define DMA1_POL_A0P   0x40000000  /* DMAAck0 Polarity         */
#define DMA1_POL_E0P   0x20000000  /* EOT0[TC0] Polarity         */
#define DMA1_POL_R1P   0x10000000  /* DMAReq1 Polarity         */
#define DMA1_POL_A1P   0x08000000  /* DMAAck1 Polarity         */
#define DMA1_POL_E1P   0x04000000  /* EOT1[TC1] Polarity         */
#define DMA1_POL_R2P   0x02000000  /* DMAReq2 Polarity         */
#define DMA1_POL_A2P   0x01000000  /* DMAAck2 Polarity         */
#define DMA1_POL_E2P   0x00800000  /* EOT2[TC2] Polarity         */
#define DMA1_POL_R3P   0x00400000  /* DMAReq3 Polarity         */
#define DMA1_POL_A3P   0x00200000  /* DMAAck3 Polarity         */
#define DMA1_POL_E3P   0x00100000  /* EOT3[TC3] Polarity         */

#else /* PPC440EP */


#define DMA0_CR0  (DMA0_DCR_BASE+0x00)	/* DMA channel control register 0     */
#define DMA0_CTC0 (DMA0_DCR_BASE+0x01)	/* DMA count register 0 	      */
#define DMA0_SAH0 (DMA0_DCR_BASE+0x02)	/* DMA source address high reg 0      */
#define DMA0_SAL0 (DMA0_DCR_BASE+0x03)	/* DMA source address low reg 0       */
#define DMA0_DAH0 (DMA0_DCR_BASE+0x04)	/* DMA destination address high reg 0 */
#define DMA0_DAL0 (DMA0_DCR_BASE+0x05)	/* DMA destination address low reg 0  */
#define DMA0_SGH0 (DMA0_DCR_BASE+0x06)	/* DMA scatter/gather desc high addr 0*/
#define DMA0_SGL0 (DMA0_DCR_BASE+0x07)	/* DMA scatter/gather desc low addr 0 */
#define DMA0_CR1  (DMA0_DCR_BASE+0x08)	/* DMA channel control register 1     */
#define DMA0_CTC1 (DMA0_DCR_BASE+0x09)	/* DMA count register 1 	      */
#define DMA0_SAH1 (DMA0_DCR_BASE+0x0A)	/* DMA source address high reg 1      */
#define DMA0_SAL1 (DMA0_DCR_BASE+0x0B)	/* DMA source address low reg 1       */
#define DMA0_DAH1 (DMA0_DCR_BASE+0x0C)	/* DMA destination address high reg 1 */
#define DMA0_DAL1 (DMA0_DCR_BASE+0x0D)	/* DMA destination address low reg 1  */
#define DMA0_SGH1 (DMA0_DCR_BASE+0x0E)	/* DMA scatter/gather desc high addr 1*/
#define DMA0_SGL1 (DMA0_DCR_BASE+0x0F)	/* DMA scatter/gather desc low addr 1 */
#define DMA0_CR2  (DMA0_DCR_BASE+0x10)	/* DMA channel control register 2     */
#define DMA0_CTC2 (DMA0_DCR_BASE+0x11)	/* DMA count register 2 	      */
#define DMA0_SAH2 (DMA0_DCR_BASE+0x12)	/* DMA source address high reg 2      */
#define DMA0_SAL2 (DMA0_DCR_BASE+0x13)	/* DMA source address low reg 2       */
#define DMA0_DAH2 (DMA0_DCR_BASE+0x14)	/* DMA destination address high reg 2 */
#define DMA0_DAL2 (DMA0_DCR_BASE+0x15)	/* DMA destination address low reg 2  */
#define DMA0_SGH2 (DMA0_DCR_BASE+0x16)	/* DMA scatter/gather desc high addr 2*/
#define DMA0_SGL2 (DMA0_DCR_BASE+0x17)	/* DMA scatter/gather desc low addr 2 */
#define DMA0_CR3  (DMA0_DCR_BASE+0x18)	/* DMA channel control register 3     */
#define DMA0_CTC3 (DMA0_DCR_BASE+0x19)	/* DMA count register 3 	      */
#define DMA0_SAH3 (DMA0_DCR_BASE+0x1A)	/* DMA source address high reg 3      */
#define DMA0_SAL3 (DMA0_DCR_BASE+0x1B)	/* DMA source address low reg 3       */
#define DMA0_DAH3 (DMA0_DCR_BASE+0x1C)	/* DMA destination address high reg 3 */
#define DMA0_DAL3 (DMA0_DCR_BASE+0x1D)	/* DMA destination address low reg 3  */
#define DMA0_SGH3 (DMA0_DCR_BASE+0x1E)	/* DMA scatter/gather desc high addr 3*/
#define DMA0_SGL3 (DMA0_DCR_BASE+0x1F)	/* DMA scatter/gather desc low addr 3 */
#define DMA0_SR   (DMA0_DCR_BASE+0x20)	/* DMA status register		      */
#define DMA0_SGC  (DMA0_DCR_BASE+0x23)	/* DMA scatter/gather command register*/
#define DMA0_SLP  (DMA0_DCR_BASE+0x25)	/* DMA sleep mode		      */
#define DMA0_POL  (DMA0_DCR_BASE+0x26)	/* DMA polarity 		      */


/* DMA channel control register bits (same for all channels) */
#define DMA_CR_CE		  0x80000000   /* Channel Enable	     */
#define DMA_CR_CIE		  0x40000000   /* Channel Interrupt Enable   */
#define DMA_CR_TD		  0x20000000   /* Transfer Direction	     */
#define DMA_CR_PL		  0x10000000   /* Peripheral Location	     */
#define DMA_CR_PW_8		  0x00000000   /* 8 bit peripheral width     */
#define DMA_CR_PW_16		  0x02000000   /* 16 bit peripheral width    */
#define DMA_CR_PW_32		  0x04000000   /* 32 bit peripheral width    */
#define DMA_CR_PW_64		  0x06000000   /* 64 bit peripheral width    */
#define DMA_CR_PW_128		  0x08000000
#define DMA_CR_DAI		  0x01000000   /* Destination Addr Increment */
#define DMA_CR_SAI		  0x00800000   /* Source Address Increment   */
#define DMA_CR_BEN		  0x00400000   /* Buffer Enable 	     */
#define DMA_CR_TM		  0x00300000
#define DMA_CR_TM_BUFFERED	  0x00000000   /* Peripheral		     */
#define DMA_CR_TM_SW_MEM_TO_MEM   0x00200000   /* Software started mem to mem*/
#define DMA_CR_TM_HW_MEM_TO_MEM   0x00300000   /* Hardware paced mem to mem  */
#define DMA_CR_PSC_0		  0x00000000   /* 0 Peripheral Setup Cycles  */
#define DMA_CR_PSC_1		  0x00040000   /* 1 Peripheral Setup Cycles  */
#define DMA_CR_PSC_2		  0x00080000   /* 2 Peripheral Setup Cycles  */
#define DMA_CR_PSC_3		  0x000C0000   /* 3 Peripheral Setup Cycles  */
#define DMA_CR_PSC(n) (((n)&0x3)<<18)	       /* n Peripheral setup cycles  */
#define DMA_CR_PWC(n) (((n)&0x3f)<<12)	       /* n peripheral wait cycles   */
#define DMA_CR_PHC(n) (((n)&0x7)<<9)	       /* n peripheral hold cycles   */
#define DMA_CR_ETD		  0x00000100   /* EOT/TC Pin Direction	     */
#define DMA_CR_TCE		  0x00000080   /* Terminal Count Enable      */
#define DMA_CR_CP		  0x00000060   /* Channel Priority	     */
#define DMA_CR_CP_0		  0x00000000   /*   Low 		     */
#define DMA_CR_CP_1		  0x00000020   /*   Medium Low		     */
#define DMA_CR_CP_2		  0x00000040   /*   Medium High 	     */
#define DMA_CR_CP_3		  0x00000060   /*   High		     */
#define DMA_CR_PF		  0x00000018   /* Memory read prefetch trans */
#define DMA_CR_PF_1		  0x00000000   /*   Prefetch 1 dword	     */
#define DMA_CR_PF_2		  0x00000008   /*   Prefetch 2 dword	     */
#define DMA_CR_PF_4		  0x00000010   /*   Prefetch 4 dword	     */
#define DMA_CR_DEC		  0x00000004   /* Address decrement	     */
#define DMA_CR_SL		  0x00000002   /* Src Addr Location/Mem Loc  */

/* DMA status register bits */
#define DMA_SR_CS0		  0x80000000   /* Ch 0: terminal count status*/
#define DMA_SR_CS1		  0x40000000   /* Ch 1: terminal count status*/
#define DMA_SR_CS2		  0x20000000   /* Ch 2: terminal count status*/
#define DMA_SR_CS3		  0x10000000   /* Ch 3: terminal count status*/
#define DMA_SR_TS0		  0x08000000   /* Ch 0: End of Transfr status*/
#define DMA_SR_TS1		  0x04000000   /* Ch 1: End of Transfr status*/
#define DMA_SR_TS2		  0x02000000   /* Ch 2: End of Transfr status*/
#define DMA_SR_TS3		  0x01000000   /* Ch 3: End of Transfr status*/
#define DMA_SR_RI0		  0x00800000   /* Ch 0: Error Status	     */
#define DMA_SR_RI1		  0x00400000   /* Ch 1: Error Status	     */
#define DMA_SR_RI2		  0x00200000   /* Ch 2: Error Status	     */
#define DMA_SR_RI3		  0x00100000   /* Ch 3: Error Status	     */
#define DMA_SR_IR0		  0x00080000   /* Ch 0: Internal DMA request */
#define DMA_SR_IR1		  0x00040000   /* Ch 1: Internal DMA request */
#define DMA_SR_IR2		  0x00020000   /* Ch 2: Internal DMA request */
#define DMA_SR_IR3		  0x00010000   /* Ch 3: Internal DMA request */
#define DMA_SR_ER0		  0x00008000   /* Ch 0: External DMA request */
#define DMA_SR_ER1		  0x00004000   /* Ch 1: External DMA request */
#define DMA_SR_ER2		  0x00002000   /* Ch 2: External DMA request */
#define DMA_SR_ER3		  0x00001000   /* Ch 3: External DMA request */
#define DMA_SR_CB0		  0x00000800   /* Ch 0: Channel Busy	     */
#define DMA_SR_CB1		  0x00000400   /* Ch 1: Channel Busy	     */
#define DMA_SR_CB2		  0x00000200   /* Ch 2: Channel Busy	     */
#define DMA_SR_CB3		  0x00000100   /* Ch 3: Channel Busy	     */
#define DMA_SR_SG0		  0x00000080   /* Ch 0: Scatter/gather status*/
#define DMA_SR_SG1		  0x00000040   /* Ch 1: Scatter/gather status*/
#define DMA_SR_SG2		  0x00000020   /* Ch 2: Scatter/gather status*/
#define DMA_SR_SG3		  0x00000010   /* Ch 3: Scatter/gather status*/
#define DMA_SR_ALL0 (DMA0_SR_CS0|DMA0_SR_TS0|DMA0_SR_RI0|DMA0_SR_IR0|DMA0_SR_ER0|DMA0_SR_CB0|DMA0_SR_SG0)
#define DMA_SR_ALL1 (DMA0_SR_CS1|DMA0_SR_TS1|DMA0_SR_RI1|DMA0_SR_IR1|DMA0_SR_ER1|DMA0_SR_CB1|DMA0_SR_SG1)
#define DMA_SR_ALL2 (DMA0_SR_CS2|DMA0_SR_TS2|DMA0_SR_RI2|DMA0_SR_IR2|DMA0_SR_ER2|DMA0_SR_CB2|DMA0_SR_SG2)
#define DMA_SR_ALL3 (DMA0_SR_CS3|DMA0_SR_TS3|DMA0_SR_RI3|DMA0_SR_IR3|DMA0_SR_ER3|DMA0_SR_CB3|DMA0_SR_SG3)

/* DMA scatter/gather command register bits */
#define DMA_SGC_SSG0 0x80000000
#define DMA_SGC_SSG1 0x40000000
#define DMA_SGC_SSG2 0x20000000
#define DMA_SGC_SSG3 0x10000000

/* DMA Polarity Config Reg bits */
#define DMA_POL_R0P	  0x80000000	/* DMAReq0 Polarity		      */
#define DMA_POL_A0P	  0x40000000	/* DMAAck0 Polarity		      */
#define DMA_POL_E0P	  0x20000000	/* EOT0[TC0] Polarity		      */
#define DMA_POL_R1P	  0x10000000	/* DMAReq1 Polarity		      */
#define DMA_POL_A1P	  0x08000000	/* DMAAck1 Polarity		      */
#define DMA_POL_E1P	  0x04000000	/* EOT1[TC1] Polarity		      */
#define DMA_POL_R2P	  0x02000000	/* DMAReq2 Polarity		      */
#define DMA_POL_A2P	  0x01000000	/* DMAAck2 Polarity		      */
#define DMA_POL_E2P	  0x00800000	/* EOT2[TC2] Polarity		      */
#define DMA_POL_R3P	  0x00400000	/* DMAReq3 Polarity		      */
#define DMA_POL_A3P	  0x00200000	/* DMAAck3 Polarity		      */
#define DMA_POL_E3P	  0x00100000	/* EOT3[TC3] Polarity		      */

#endif /* PPC440EP */

/*
 * DMA Register definitions
 */

#ifdef __cplusplus
    }
#endif

#endif	/* INCdmaDcrh */
