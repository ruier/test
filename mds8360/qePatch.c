/* qePatch.c - Freescale mds8360 board enet15 errata patch */

/* Copyright (c) 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,10jan06,dtr  created.
*/

/*
DESCRIPTION

This library provides errata patch for quicc engine microcode.

*/

/* Supplied by Freescale */

/* This patch will tell the QUICC Engine that there is a ucode patch in the IRAM (cercr commands).  
You may need to change the base address to match your configuration.  The base address used is
0xE0xx_xxxx.
*/
/* Load ucode patches */
void loadEnet15Patch()
    {
    uint32_t *ptra;
    uint32_t *ptrd;
    uint16_t *cercr;	
    uint32_t *ptra_risc1;
    uint32_t *ptra_risc2;

    ptra = (uint32_t *) (0x100000 | CCSBAR);
    ptrd = (uint32_t *) (0x100004 | CCSBAR);
    cercr = (uint16_t *) (0x100138 | CCSBAR);

    *cercr = 0x0800;	/* Common Instruction Ram		*/


    *ptra = 0x00080100;
    *ptrd = 0xb3ff0008;
    *ptra = 0x00080104;
    *ptrd = 0xc280740e;
    *ptra = 0x00080108;
    *ptrd = 0xb3ff0008;
    *ptra = 0x0008010c;
    *ptrd = 0xc2807400;
    *ptra = 0x00080110;
    *ptrd = 0xb3ff0007;
    *ptra = 0x00080114;
    *ptrd = 0xc2807408;
    *ptra = 0x00080118;
    *ptrd = 0xb3ff0006;
    *ptra = 0x0008011c;
    *ptrd = 0x0616d058;
    *ptra = 0x00080120;
    *ptrd = 0x9bfff400;
    *ptra = 0x00080124;
    *ptrd = 0x0616d058;
    *ptra = 0x00080128;
    *ptrd = 0x9ffff400;
    *ptra = 0x0008012c;
    *ptrd = 0x9ffff400;
    *ptra = 0x00080130;
    *ptrd = 0x0618d000;
    *ptra = 0x00080134;
    *ptrd = 0xc280742f;
    *ptra = 0x00080138;
    *ptrd = 0x7717b801;
    *ptra = 0x0008013c;
    *ptrd = 0x1417c028;
    *ptra = 0x00080140;
    *ptrd = 0x203ff800;
    *ptra = 0x00080144;
    *ptrd = 0x7716b001;
    *ptra = 0x00080148;
    *ptrd = 0x9bfff400;
    *ptra = 0x0008014c;
    *ptrd = 0x0402d060;

    ptra_risc1 = (uint32_t *) (0x104100 | CCSBAR);
    ptra_risc2 = (uint32_t *) (0x104200 | CCSBAR);

    *ptra_risc1 = 0xC000744e;  /* set traps */
    *ptra_risc2 = 0xC000744e;  /* set traps */
    ptra_risc1++;
    ptra_risc2++;
    *ptra_risc1 = 0xC000743f;  /* set traps */
    *ptra_risc2 = 0xC000743f;  /* set traps */
    ptra_risc1++;
    ptra_risc2++;
    *ptra_risc1 = 0xC0007448;  /* set traps */
    *ptra_risc2 = 0xC0007448;  /* set traps */
    ptra_risc1++;
    ptra_risc2++;
    *ptra_risc1 = 0xC000747a;  /* set traps */
    *ptra_risc2 = 0xC000747a;  /* set traps */

    ptra = (uint32_t *) (0x1041f0 | CCSBAR);	/* Enable traps */
    *ptra = 0x20800000;
    ptra = (uint32_t *) (0x1042f0 | CCSBAR);	/* Enable traps */
    *ptra = 0x20800000;

    }
