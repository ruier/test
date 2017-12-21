/* bootInit_nand.c - ROM initialization module of nand boot */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,07may09,wap  Correct PAGE_PER_BLOCK macro
01a,18Sep07,l_g  initial creation for nand boot
*/

/*
DESCRIPTION

This file provides nand boot routines.

INCLUDE FILES:
*/

/* includes */

#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "errno.h"
#include "sioLib.h"

/*******************************************************************************
*
* romStart - copy bootrom image from nand flash to ram
*
* This routine copy bootrom image from nand flash to ram.
* It then jumps to the entry point of the vxWorks bootrom.
*
* RETURNS: N/A
*
*/
void romStart(int startType)
{
	int i,j;
	unsigned int *dst;
	unsigned char *dstc;
	unsigned char *srcc = 0;
	unsigned int *src;
	int page_num;
	int buf_num;
	unsigned int status;
	unsigned long bootrom_adrs;
	FUNCPTR bootrom;


    /* copy code to reserved area */	 
	dstc = (unsigned char *)RAM_IMAGE_ADRS;

	/* every time read a entire page */
	*M8313_FBCR(CCSBAR) = 0;	

    /* CW0,CA,PA,RBW */
	*M8313_FIR(CCSBAR) = 0xc12e0000;

	/* hold read command */
	*M8313_FCR(CCSBAR) = 0x00300000;

    /* block read */
    for(i=0;i<NAND_BLOCKCOPY_COUNT;i++)
    {

		srcc = 0;

		dst = (unsigned int *)dstc;

	
		/* read next block */
		*M8313_FBAR(CCSBAR)= *M8313_FBAR(CCSBAR) + 1;	

		/* page read of the block */
		for (page_num = 0; page_num < NAND_PAGE_PER_BLOCK; page_num++) 
		{

			/* small page device */ 
			*M8313_FPAR(CCSBAR) = ((page_num << 10) & 0x00007C00);
			
			 buf_num = page_num & 7;

		    /* 4194304 cycles of LCLK, normal functional mode, AL =0, execute FIR */
			*M8313_FMR(CCSBAR) = 0x0000e002;
		
			/* clear event registers */
			*M8313_LTESR(CCSBAR) = *M8313_LTESR(CCSBAR);
		
			*M8313_LTEATR(CCSBAR) = 0;
		
			/* execute special operation on bank 0 */
			*M8313_LSOR(CCSBAR) = 0;

		    /* delay */
			for(j=0;j<10000;j++)
			;
		
			/* copy previous page to RAM */
			if (srcc) 
			{
				src = (unsigned int *)srcc;
				for (j = NAND_PAGE_SIZE / sizeof (unsigned int);j; j--) 
					{
					   *(dst++) = *(src++);
					}
			}
		
			/* store the source address for the next page */
			srcc = (unsigned char *)(NAND_BUFFER_BASE + (buf_num * 1024));
		
			/* wait for FCM complete flag or timeout */
			status = 0;

			while(status == 0)
			{
				status = *M8313_LTESR(CCSBAR);
      		}

			/* check error */
            if(page_num <= 1)
            {
               if ((status != FCM_COMMAND_COMPLETE) && ((status & FCM_ECC_ERROR) == 0))
               	{
				   /* FCM command time out, 7 led on and return */
				   *(volatile unsigned long *)CS_LED = 0x1;
				   return;
               	}

			   if ((status == FCM_COMMAND_COMPLETE) && (srcc[512 + 5] != 0xFF))
			   	{
				   /* the block is marked as bad, 6 led on and return */
				   *(volatile unsigned long *)CS_LED = 0x3;
				   return;
			   	}
			   
			}
            else
            {
			   if (status != FCM_COMMAND_COMPLETE)
			    {
			       if(status & FCM_ECC_ERROR)
			       {
    			      /* pages had uncorrectable ECC errors, 5 led on and return */
    			      *(volatile unsigned long *)CS_LED = 0x7;
    			      return;
			       }
				   else
				   {
					  /* FCM command time out, 7 led on and return */
     				  *(volatile unsigned long *)CS_LED = 0x1;
					  return;
				   }
				}
 			}
		}

		/* copy last page to RAM */
		src = (unsigned int *)srcc;
		for (j = NAND_PAGE_SIZE / sizeof (unsigned int); j; j--) 
		{
			*(dst++) = *(src++);
		}
		
		/* to the next block */
		dstc += NAND_BLOCK_SIZE;

	}
	


	/* Jump to vxWorks bootrom image */

	bootrom_adrs = RAM_IMAGE_ADRS + 0x100;
	bootrom = (FUNCPTR) bootrom_adrs;	 

	(*bootrom) ();
}

extern void romStart();
extern void romInit();

/******************************************************************************
*
* copyBufferToRam - Copy first loader buffer to ram
*
* This routine copy first loader buffer to ram. After this, we can jump to ram and 
* use buffer to copy bootrom image.
*
* RETURNS: None.
*/

void copyBufferToRam()
{
   
   volatile unsigned char *dst;
   volatile unsigned char *src;
   int i;
   FUNCPTR run_in_ram;

   unsigned long code_ram_adrs,offset;

   offset=((unsigned long )romStart-(unsigned long )romInit);

   code_ram_adrs = NAND_LOADER_ADRS + 0x100 + offset;
   
   run_in_ram = (FUNCPTR)code_ram_adrs;   
   
   dst = (volatile unsigned char *)NAND_LOADER_ADRS;
   src = (volatile unsigned char *)NAND_BUFFER_BASE;
   
   for(i=0;i<NAND_BUFFER_SIZE;i++)
   {
	   *(dst++) = *(src++);
   }
   
   /* Jump to ram */
   (*run_in_ram) ();    
   
}

