/* m8560CpmIntrCtl.c - Motorola ads 85xx board system-dependent library */

/*
 * Copyright (c) 2004, 2006, 2007, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,8nov12, ylu  Correct SIMR_BIT_L description according to 
                 the MPC8560 reference manual. (WIND00071941)  
01c,26sep07,pdg  Fixed doc errors
01b,29aug06,wap  Add m85xxCpmIntDisconnect() function
01a,04oct04,mdo  Documentation fixes for apigen
*/

/*
DESCRIPTION
This library provides the routines to manage the CPM interrupt 
controllers.

All of the functions in this library are global.  This allows them to
be used by the BSP if it is necessary to create wrapper routines or to
incorporate several drivers together as one.

INCLUDE FILES: N/A
*/

/* Pre-initialized CPM interrupt vector table */
M8560VEC_TBL m8560VectorTbl[] =
    {
     {0, NULL, 0 },
     {1, NULL, 0 },
     {2, NULL, 0 },
     {3, NULL, 0 },
     {4, NULL, 0 },
     {5, NULL, 0 },
     {6, NULL, 0 },
     {7, NULL, 0 },
     {8, NULL, 0 },
     {9, NULL, 0 },
     {10, NULL, 0 },
     {11, NULL, 0 },
     {12, NULL, 0 },
     {13, NULL, 0 },
     {14, NULL, 0 },
     {15, NULL, 0 },
     {16, NULL, 0 },
     {17, NULL, 0 },
     {18, NULL, 0 },
     {19, NULL, 0 },
     {20, NULL, 0 },
     {21, NULL, 0 },
     {22, NULL, 0 },
     {23, NULL, 0 },
     {24, NULL, 0 },
     {25, NULL, 0 },
     {26, NULL, 0 },
     {27, NULL, 0 },
     {28, NULL, 0 },
     {29, NULL, 0 },
     {30, NULL, 0 },
     {31, NULL, 0 },
     {32, NULL, 0 },
     {33, NULL, 0 },
     {34, NULL, 0 },
     {35, NULL, 0 },
     {36, NULL, 0 },
     {37, NULL, 0 },
     {38, NULL, 0 },
     {39, NULL, 0 },
     {40, NULL, 0 },
     {41, NULL, 0 },
     {42, NULL, 0 },
     {43, NULL, 0 },
     {44, NULL, 0 },
     {45, NULL, 0 },
     {46, NULL, 0 },
     {47, NULL, 0 },
     {48, NULL, 0 },
     {49, NULL, 0 },
     {50, NULL, 0 },
     {51, NULL, 0 },
     {52, NULL, 0 },
     {53, NULL, 0 },
     {54, NULL, 0 },
     {55, NULL, 0 },
     {56, NULL, 0 },
     {57, NULL, 0 },
     {58, NULL, 0 },
     {59, NULL, 0 },
     {60, NULL, 0 },
     {61, NULL, 0 },
     {62, NULL, 0 },
     {63, NULL, 0 },
    };

/* Low Interrupt Mask */
UINT32 SIMR_BIT_L[64] = 
{
0,
0x00008000, /* 1 I2C */
0x00004000, /* 2 SPI */
0x00002000, /* 3 RTT */
0,0,0,0,    /* Reserved */
0,0,0,0,    /* Reserved */
0x00000010, /* 12 TIMER1 */
0x00000008, /* 13 TIMER2 */
0x00000004, /* 14 TIMER3 */
0x00000002, /* 15 TIMER4 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /*16 - 31 See High mask  */
0x80000000, /* 32 FCC1 */
0x40000000, /* 33 FCC2 */
0x20000000, /* 34 FCC3 */
0,          /* Reserved */
0x08000000, /* 36 MCC1 */
0x04000000, /* 37 MCC2 */
0,0,        /* Reserved */
0x00800000, /* 40 SCC1 */
0x00400000, /* 41 SCC2 */
0x00200000, /* 42 SCC3 */
0x00100000, /* 43 SCC4 */
0x00080000, /* 44 TC */
0x00040000, /* 45 Core I/F */  
0x00020000, /* 46 SDMA SYS */
0x00010000, /* 47 SDMA LCL */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  /* 48 - 63 See high mask */
};

/* High Interrupt Mask */
UINT32 SIMR_BIT_H[64] = 
{
0,0,0,0,
0,0,0,0,
0,0,0,0,
0,0,0,0, /* 0 - 15 See Low mask */
0,0,0,0,
0,0,0,0,
0,0,0,0,
0,0,0,0, /* 16 - 31 Reserved */ 
0,0,0,0,   
0,0,0,0,  
0,0,0,0,
0,0,0,0, /* 32 - 47 See Low Mask */
0x00010000, /* 48 PC15 */
0x00020000, /* 49 PC14 */
0x00040000, /* 50 PC13 */
0x00080000, /* 51 PC12 */
0x00100000, /* 52 PC11 */
0x00200000, /* 53 PC10 */
0x00400000, /* 54 PC9 */
0x00800000, /* 55 PC8 */
0x01000000, /* 56 PC7 */
0x02000000, /* 57 PC6 */
0x04000000, /* 50 PC5 */
0x08000000, /* 50 PC4 */
0x10000000, /* 60 PC3 */
0x20000000, /* 61 PC2 */
0x40000000, /* 62 PC1 */
0x80000000  /* 63 PC0 */
};

/*****************************************************************************
*
* m85xxCpmInt - CPM interrupt Handler. 
*
* This routine handles all CPM interrupts after being called by the 
* Epic interrupt handler. Reading SIVEC returns the vector with which the 
* CPM is prioritizing to be handled next. This device handler is then 
* called using the interrupt vector table
* 
* RETURNS: N/A
*
* ERRNO: N/A
*/ 
void m85xxCpmInt (void)
    {
      int dontCare,vector;

      /* Read vector to be processed */
      vector = (UINT32)((*M85XX_CPM_SIVEC(CCSBAR))>>2);
      
      /* !!!!Need to unlock interrupts to allow higher priority ISRs to run */
      /* !!!!NOT Done Yet!!! Interrupt mask to allow relevant interrupts */
      /* set ee bit */
      /* Allow external interrupts to the CPU. */

      CPU_INT_UNLOCK (_PPC_MSR_EE);
      
      if ((int)m8560VectorTbl[vector].excHandler!=0)
         m8560VectorTbl[vector].excHandler(m8560VectorTbl[vector].param);
      else
	{
        logMsg ("No handler for this CPM interrupt!! %d",vector,0,0,0,0,0);
	}
    
      CPU_INT_LOCK (&dontCare);
    }  

/*****************************************************************************
*
* sysCpmHwInit2 - connects the CPM interrupt handler
*
* This routine clears CPM interrupts pending and connects 
* the CPM interrupt handler to the Epic Interrupt. Should be called after 
* Epic is initialized but before CPM devices are enabled (sysHwInit2).
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void sysCpmHwInit2 (void)
    {
    *M85XX_CPM_SIPNR_L(CCSBAR) = 0xffffffff;
    *M85XX_CPM_SIPNR_H(CCSBAR) = 0xffffffff;
  
    /* connect EPIC interrupt of CPM */
    intConnect ( (VOIDFUNCPTR *)EPIC_CPM_INT_VEC , (VOIDFUNCPTR)m85xxCpmInt, (int)0);
    intEnable ( EPIC_CPM_INT_VEC);
    }


/*****************************************************************************
*
* m85xxCpmIntConnect - add device to CPM vector table
*
* This routines adds the devices to the CPM interrupt 
* vector table.
*
* RETURNS : N/A
*
* ERRNO: N/A
*/
void m85xxCpmIntConnect 
    (
    int inum,
    VOIDFUNCPTR handler,
    int *param
    )
    {
    /* Populate Cpm Device Int Table */
    m8560VectorTbl[inum].intVec = inum;
    m8560VectorTbl[inum].excHandler = handler;
    m8560VectorTbl[inum].param = (UINT32) param;
    /* set VPR of CPM intr ctrl */
    }

/*****************************************************************************
*
* m85xxCpmIntDisconnect - remove device from CPM vector table
*
* This routines removes an ISR from the CPM interrupt 
* vector table.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void m85xxCpmIntDisconnect 
    (
    int inum,
    VOIDFUNCPTR handler,
    int *param
    )
    {
    /* Populate Cpm Device Int Table */
    m8560VectorTbl[inum].intVec = 0;
    m8560VectorTbl[inum].excHandler = NULL;
    m8560VectorTbl[inum].param = 0;
    /* set VPR of CPM intr ctrl */
    }

/*****************************************************************************
*
* m85xxCpmIntEnable - enable CPM interrupt for a particular vector
*
* This routine enables the CPM interrupt for a particular vector.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/ 
void m85xxCpmIntEnable 
    (
    int vector
    )
    {
/* Un mask interrupt */
  *M85XX_CPM_SIMR_L(CCSBAR) |= SIMR_BIT_L[vector];
  *M85XX_CPM_SIMR_H(CCSBAR) |= SIMR_BIT_H[vector];
    }

/*****************************************************************************
*
* m85xxCpmIntDisable - disable the CPM interrupt for a particular vector
*
* This routine disables the CPM interrupt for a particular vector.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/ 
void m85xxCpmIntDisable 
    (
    int vector
    )
    {
/* Mask interrupt */
  *M85XX_CPM_SIMR_L(CCSBAR) &= ~SIMR_BIT_L[vector];
  *M85XX_CPM_SIMR_H(CCSBAR) &= ~SIMR_BIT_H[vector];
    }

#ifdef INCLUDE_SHOW_ROUTINES

/*****************************************************************************
*
* m85xxCpmRegDump - dump the CPM interrupt controller registers
*
* This routine dumps the CPM interrupt controller registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void m85xxCpmRegDump(void)
    {
    UINT32 tmp;

    tmp = *M85XX_CPM_SICR(CCSBAR);
    printf ("CPM_SICR      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SIVEC(CCSBAR);
    printf ("CPM_SIVEC      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SIPNR_H(CCSBAR);
    printf ("CPM_SIPNR_H      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SIPNR_L(CCSBAR);
    printf ("CPM_SIPNR_L      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SCPRR_H(CCSBAR);
    printf ("CPM_SCPRR_H      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SCPRR_L(CCSBAR);
    printf ("CPM_SCPRR_L      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SIMR_H(CCSBAR);
    printf ("CPM_SIMR_H      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SIMR_L(CCSBAR);
    printf ("CPM_SIMR_L      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SIEXR(CCSBAR);
    printf ("CPM_SIEXR      = 0x%x\n", tmp);

    tmp = *M85XX_CPM_SCCR(CCSBAR);
    printf ("CPM_SCCR      = 0x%x\n", tmp);

    }
#endif




