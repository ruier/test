/* bootInit_nand.c - ROM initialization module of nand boot */

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
01b,12oct11,e_d  add isync instruction to NAND read. (WIND00306790)
01a,30jun11,e_d  initial creation for nand boot
*/

/*
DESCRIPTION

This file provides nand boot copy routines.

INCLUDE FILES:
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include "sysNandFlash.h"

IMPORT void romInit();
IMPORT void romStart();

/*******************************************************************************
*
* PageRead - page read.
*
* This routine get one page data of the nand flash.
*
* RETURNS: N/A
*
*/

void PageRead
    (
    ULONG ulReadAddr,
    ULONG * ulDst
    )
    {
    int ix = 0;
    volatile ULONG pageNum;
    volatile ULONG ulTemp;
    volatile ULONG *pSrc;
    int timeOut = 0;

    pageNum = MICRO_NAND_PAGECAL(ulReadAddr);

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, (FIR_CM0_OPX(FIR_OP0_SHIFT)|
                                                 FIR_CA_OPX(FIR_OP1_SHIFT)|
                                                 FIR_PA_OPX(FIR_OP2_SHIFT)|
                                                 FIR_CM1_OPX(FIR_OP3_SHIFT)|
                                                 FIR_RBW_OPX(FIR_OP4_SHIFT)));

    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD0(MICRO_NAND_READFIRST) |
                                           FCR_CMD1(MICRO_NAND_READSEC));

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, pageNum >> 6);

    FCM_REG_WRITE_ULONG(FLASH_PAGEADDR_REG, MICRO_NAND_PAGESET(pageNum));

    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);

    ulTemp |= FMR_OP_EXENOPOT | FMR_AL_4BYTE;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG, 0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG, 0);

    while((((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
             && ((timeOut++ ) < TIMEOUT));

    pSrc  = (volatile ULONG *)(NAND_BUFFER_BASE + ((pageNum & FCM_LARBUF_MASK)
                                                    * FCM_LARBUF_SIZE));

    for (ix = NAND_PAGE_SIZE / sizeof (ULONG); ix; ix--)
        {
        *(ulDst++) = *(pSrc++);
        }

    return;
    }

/*******************************************************************************
*
* nandCopyRam - copy bootrom image from nand flash to ram
*
* This routine copy bootrom image from nand flash to ram.
* It then jumps to the entry point of the vxWorks bootrom.
*
* RETURNS: N/A
*
*/
void nandCopyRam
    (
    FAST int startType
    )
    {
    int i = 0;
    volatile UINT8 * dst;
    UINT32 copyAdrs = 0;
    FUNCPTR run_in_ram;

    unsigned long code_ram_adrs,offset;

    offset=((unsigned long )romStart -(unsigned long )romInit);

    code_ram_adrs = ROM_BASE_ADRS + 0x100 + offset;

    run_in_ram = (FUNCPTR)code_ram_adrs;

    dst = (volatile UINT8 *)ROM_BASE_ADRS;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, FMR_1M_LCLK);

    /*
     * By default, bootrom image size is less than 1M.
     * So we copy 1M size code to reserved area.
     * (8 block * 64 page * 2K page size = 1MB size)
     * If os image is more than 1M, we should increase
     * ROM_SIZE.
     */

    do
        {
        PageRead(copyAdrs,(ULONG *)dst);
        WRS_ASM("isync");
        copyAdrs += MICRO_NAND_PAGESIZE;
        ++i;
        WRS_ASM("isync");
        dst = (volatile ULONG *)((ROM_BASE_ADRS) + MICRO_NAND_PAGESIZE * i);
        }while(copyAdrs < ROM_SIZE);

       (*run_in_ram) (startType);

    }

/*******************************************************************************
*
* copyBufferToRam - Copy first loader buffer to ram
*
* This routine copy first loader buffer to ram. After this, we can jump to ram and
* use buffer to copy bootrom image.
*
* RETURNS: None.
*/

void copyBufferToRam
    (
    FAST int startType
    )
    {

    volatile unsigned char *dst;
    volatile unsigned char *src;
    int i;
    FUNCPTR run_in_ram;

    unsigned long code_ram_adrs,offset;

    offset=((unsigned long )nandCopyRam -(unsigned long )romInit);

    code_ram_adrs = ROM_BASE_ADRS + 0x100 + offset;

    run_in_ram = (FUNCPTR)code_ram_adrs;

    dst = (volatile unsigned char *)ROM_BASE_ADRS;
    src = (volatile unsigned char *)NAND_BUFFER_BASE;

    for(i = 0; i < 0x1000; i++)
        {
        *(dst++) = *(src++);
        }

    /* Jump to ram */

    (*run_in_ram) (startType);

    }

