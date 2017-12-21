/* bootInit_nand.c - ROM initialization module of nand boot */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,18oct10,y_y  initial creation, based on fsl_rdb8313
*/

/*
DESCRIPTION
This module provides a generic NAND boot facility. The target-specific
romInit_nand.s module performs the minimal preliminary board initialization and
then jumps to the C routine copyBufferToRam(). This routine, copies the  
4kB startup code which locate in 4-Kbyte boot block buffer to USER_RESERVED_MEM 
address and jumps to it. The next stage copy the image
(eg:bootrom/vxWorks/bootapp) from NAND Flash to the other unused RAM address, 
which still in USER_RESERVED_MEM and jumps to romInit.s module.

Note: Because the boot block buffer only have 4Kb capacity,so we must 
guarantee the sizeiof startup code(romInit_nand.s and bootInit_nand.c) 
size are less than 4Kb.

ROM AND RAM MEMORY LAYOUT
Example memory layout for a 512-megabyte board:
\cs
    --------------  0x20000000 = LOCAL_MEM_SIZE = sysMemTop()
    |    4Kb     |  
    |startup code|
    |------------|  0x1ffff000 = NAND_LOADER_ADRS
    |            |
    |            |
    |------------|
    |   IMAGE    |  
    |eg: bootrom |
    |vxWorks_rom |  copy code(bootrom) in NADN Flash to here 
    |   bootapp  |
    |------------|  0x1fc00000 = RAM_IMAGE_ADRS = (LOCAL_MEM_LOCAL_ADRS + \
    |     RAM    |                       LOCAL_MEM_SIZE - USER_RESERVED_MEM)
    |  0 filled  |
    |            |
    |------------| = (romInit+ROM_COPY_SIZE) or binArrayStart
    | ROM image  |
    |----------- |  0x01000000  = RAM_HIGH_ADRS
    | STACK_SAVE |
    |------------|
    |            |
    |            |
    |            |
    | 0 filled   |
    |            |
    |            |  0x00100000  = RAM_ADRS & RAM_LOW_ADRS
    |            |
    |            |  exc vectors, bp anchor, exc msg, bootline
    |            |
    |            |
    --------------  0x00000000  = LOCAL_MEM_LOCAL_ADRS
\ce

*/

/* includes */

#include <vxWorks.h>
#include <sysLib.h>
#include "config.h"
#include <errno.h>
#include <sioLib.h>

IMPORT void romInit();

/*******************************************************************************
*
* copyNandToRam - copy bootrom image from nand flash to ram
*
* This routine copy bootrom image from nand flash to ram.
* It then jumps to the entry point of the vxWorks bootrom.
*
* RETURNS: N/A
*
*/

void copyNandToRam
    (
    int startType
    )
    {
    int i,j;
    unsigned int *dst;
    unsigned char *dstc;
    unsigned int *src;
    unsigned int page_num, buf_num;
    unsigned int status,temp;
    unsigned long bootrom_adrs;

    FUNCPTR bootrom;

    unsigned char *srcc = 0;
    
    /* delay a few time by write register */

    for (i=2; i>0; i--)
        {
        *ELBC_LTEDR(CCSBAR) = 0;
        _WRS_ASM("sync");
        }
    
    /* Set ABSWP to implement conversion of addresses in the LBC */

    temp =*M85XX_LBCR(CCSBAR);
    *M85XX_LBCR(CCSBAR)= temp | CONFIG_SYS_LBC_LBCR;
    
    /* copy code to reserved area */
    
    dstc = (unsigned char *)RAM_IMAGE_ADRS;

    /* hold read command */
    *ELBC_FCR(CCSBAR) = 0;
    *ELBC_FIR(CCSBAR) = ELBC_FIR_INIT_VAL;

    /* every time read an entire page */
    *ELBC_FBCR(CCSBAR) = 0;	
   
    /* block read */
    
    for(i = 0; i < NAND_BLOCKCOPY_COUNT; i++) 
        {
        srcc = 0;
        dst = (unsigned int *)dstc;

        /* read next block */
        *ELBC_FBAR(CCSBAR)= *ELBC_FBAR(CCSBAR) + 1; 

        /* page read of the block */
        for (page_num = 0; page_num < 32; page_num++) 
            {   
            /* clear event registers */
          
            *ELBC_LTESR(CCSBAR) = ~0;   
          
            *ELBC_LTEATR(CCSBAR) = 0;

            /* small page device */
 
            *ELBC_FPAR(CCSBAR) = ((page_num << 10) & ELBC_FPAR_MASK);
      
            buf_num = page_num & 7;
 
            *ELBC_FMR(CCSBAR) = ELBC_FMR_VAL;

            /* execute special operation on bank 0 */

            *ELBC_LSOR(CCSBAR) = 0;

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

            while(status==0)
                {
                status = *ELBC_LTESR(CCSBAR);
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

    (FUNCPTR)bootrom ();
    }

/******************************************************************************
*
* copyBufferToRam - Copy first loader buffer to ram
*
* This routine copy first 4Kb loader buffer to ram. After this,
* we can jump to ram and use buffer to copy bootrom image.
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

    offset=((unsigned long )copyNandToRam-(unsigned long )romInit);

    code_ram_adrs = NAND_LOADER_ADRS + 0x100 + offset;
   
    run_in_ram = (FUNCPTR)code_ram_adrs;   
   
    dst = (volatile unsigned char *)NAND_LOADER_ADRS;
    src = (volatile unsigned char *)NAND_BUFFER_BASE;
    
    for(i=0;i<0x1000;i++)
        {
        *(dst++) = *(src++);
        }

    /* Jump to ram */
    (FUNCPTR)run_in_ram ();    
}
