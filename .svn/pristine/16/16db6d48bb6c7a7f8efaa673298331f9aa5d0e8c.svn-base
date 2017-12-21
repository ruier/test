/* sysNandFlash.c - Nand Flash Driver for MPC8313ERDB */

/*
 * Copyright (c) 2008,2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,15jan10,syt add workaround for write flash hanged on P2020E
01b,05mar08,z_l fix block read/write error, modify for code convention.
01a,18jan08,l_g initial creation
*/

/*
DESCRIPTION

This file provides Nand Flash Driver routines for MPC8313ERDB. There is a 32Mb
NAND flash on this board, so address bits a0-a21 are valid, and address bits 
upper a21 are ignored. For instance, if fcmBlockWrite(...) are invoked, only 
a0-a21 are used. For this reason, we don't define specific base address and
size for NAND flash in MMU. Before using read and write routines, fcmReset() 
and fcmInit() should be run to initialize NAND controller.
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include "sysNandFlash.h"

/*******************************************************************************
*
* fcmInit - intializes the nand controller
*
* This routine initializes the nand controller.
*
* RETURNS: N/A
*
*/

VOID fcmInit(VOID)
    {
    ULONG ulTemp;
    
    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);
    ulTemp |= FMR_1M_LCLK;
    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);
    }

/*******************************************************************************
*
* fcmReadID - read nand flash ID.
*
* This routine get the ID of nand flash.
*
* RETURNS: ID of flash
*
*/

VOID fcmReadID
    (
    char * NandID
    )
    {
    ULONG ulTemp = 0;
    
    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_OP0_CW0 |
                                                 FIR_OP1_UA  |
                                                 FIR_OP2_RBW ));
    
    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD0_WS_NOP);

    /* 5 bytes for manuf, device and exts */
    
    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 5);

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, 0);
    
    FCM_REG_WRITE_ULONG(FLASH_PAGEADDR_REG, 0);

    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);

    ulTemp |= 3;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);
   
    FCM_REG_WRITE_ULONG(FCM_DATA_REG, 0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG, 0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG,NAND_BANK);

    while(((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
        ;

    bcopyBytes ((char *) NAND_BUFFER_BASE, NandID, 5);
    }

/*******************************************************************************
*
* fcmPageRead - page read.
*
* This routine get one page data of the nand flash.
*
* RETURNS: page data
*
*/

VOID fcmPageRead
    (
    ULONG ulReadAddr,
    ULONG ulDst
    )
    {
    ULONG ulBufNum;
    int i;
    volatile ULONG ulTemp;
    volatile ULONG *pDest;
    volatile ULONG *pSrc;
        
    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_OP0_CW0 |
                                                 FIR_OP1_CA  |
                                                 FIR_OP2_PA  |
                                                 FIR_OP3_RBW ));
    
    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD1_UA_NOP);
    
    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);
    
    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, (ulReadAddr >> 14));

    ulTemp = ulReadAddr >> 9;
    ulTemp = ulTemp % 32;

    FCM_REG_WRITE_ULONG(FLASH_PAGEADDR_REG, ((ulTemp << 10) & 0x00007C00));

    ulBufNum = ulTemp & 7;

    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);
        
    ulTemp |= 3;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG, 0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG, 
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG, NAND_BANK);

    while(((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
        ;

    pDest = (volatile ULONG *)ulDst;
    pSrc  = (volatile ULONG *)(NAND_BUFFER_BASE + (ulBufNum * 1024));
    
    for (i = NAND_PAGE_SIZE / sizeof (ULONG); i; i--) 
        {
        *(pDest++) = *(pSrc++);
        }    
    }

/*******************************************************************************
*
* fcmBlockErase - block erase.
*
* This routine erase one block of the nand flash.
*
* RETURNS: N/A
*
*/

VOID fcmBlockErase
    (
    ULONG ulEraseAddr
    )
    {
    volatile ULONG ulTemp;

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, (ulEraseAddr >> 14));

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_OP0_CW0 |
                                                 FIR_OP1_PA  |
                                                 FIR_OP2_CM1 ));
    
    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD0_CM2_CW1);

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);
    
    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);
        
    ulTemp |= 3;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG,ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG,0);
    
    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));
    
    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG, NAND_BANK);
    
    while(((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
     ;

    }

/*******************************************************************************
*
* fcmReset - nand controller reset.
*
* This routine reset the nand controller.
*
* RETURNS: N/A
*
*/

VOID fcmReset()
    {
    volatile ULONG ulTemp;

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, (0 >> 14));

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_OP0_CW0 |
                                                 FIR_OP1_PA  |
                                                 FIR_OP2_CM1 ));
    
    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD0_RSW_RSW);

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);
    
    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);
        
    ulTemp |= 3;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG, 0);
    
    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));
    
    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG,NAND_BANK);
    
    while(((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
        ;

    }

/*******************************************************************************
*
* fcmPageWrite - page write.
*
* This routine write data into one page of the nand flash.
*
* RETURNS: OK or ERROR
*
*/

STATUS fcmPageWrite
    (
    ULONG ulWriteAddr,
    ULONG ulSrc
    )
    {   
    ULONG ulBufNum;
    int i;
    volatile ULONG * pDest ;
    volatile ULONG * pSrc ;
    volatile ULONG   ulTemp;
    volatile ULONG   status;
    register ULONG   cont;
    volatile UCHAR   temp;

    ulTemp = ulWriteAddr >> 9;
    ulTemp = ulTemp % 32;
    ulBufNum = ulTemp & 7;
    pSrc = (volatile ULONG *)ulSrc;
    pDest  = (volatile ULONG *)(NAND_BUFFER_BASE + (ulBufNum * 1024));
   
    for (i = NAND_PAGE_SIZE / sizeof (ULONG); i; i--) 
        {
        *(pDest++) = *(pSrc++);
        }

    /*
     * Workaround for P2020E
     *
     * Read the last byte writen into buffer, otherwise NAND Flash page 
     * write operation would be hanged. The reason I don't know, but it 
     * indeed in effect.
     */

    if ((vxPvrGet() & PPC_PVR_MASK) == PPC_PVR_E500V2)
        {
        if ((_PPC_VX_SVR_GET & PPC_SVR_MASK) == PPC_SVR_P2020)
            temp = *(pDest - 1);
        }

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, (FIR_OP0_CW0 |
                                                FIR_OP1_CA  |
                                                FIR_OP2_PA  |
                                                FIR_OP3_WB  |
                                                FIR_OP4_CW1 ));
    
    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, (FCR_CMD0_WB_NOP | FCR_CMD1_CA_NOP));

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG,(ulWriteAddr >> 14));
   
    ulTemp = ulWriteAddr >> 9;
    ulTemp = ulTemp % 32;

    FCM_REG_WRITE_ULONG(FLASH_PAGEADDR_REG,((ulTemp << 10) & 0x00007C00));

    ulBufNum = ulTemp & 7;

    pSrc = (volatile ULONG *)ulSrc;
    pDest  = (volatile ULONG *)(NAND_BUFFER_BASE + (ulBufNum * 1024));

    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);
       
    ulTemp |= 3;
   
    FCM_REG_WRITE_ULONG(FLASH_MODE_REG,ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG,0);
   
    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,(LTESR_FCT | 
                       LTESR_PAR | LTESR_CC | LTESR_CS));
   
    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG,NAND_BANK);
   
    cont = 0;
   
    while(cont++ < 0x200000)
        {
        status = (FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG));
        if(status & LTESR_CC)
            return (OK);
        } 
    if(cont >= 0x200000)
        return (ERROR);
    else
        return (OK); 
     }

/*******************************************************************************
*
* fcmBlockRead - block read.
*
* This routine get one block data of the nand flash.
*
* RETURNS: N/A
*
*/

VOID fcmBlockRead
    (
    ULONG ulReadAddr,
    ULONG ulDst 
    )
    {
    int i;
    
    ulReadAddr &= 0xffffc000;
    
    for(i = 0; i < NAND_PAGE_PER_BLOCK; i++)
        {
        fcmPageRead(ulReadAddr, ulDst);
        ulReadAddr += NAND_PAGE_SIZE;
        ulDst += NAND_PAGE_SIZE;
        }
    }

/******************************************************************************
*
* fcmBlockWrite - block write.
*
* This routine write data into one block of the nand flash.
*
* RETURNS: OK or ERROR
*
*/

STATUS fcmBlockWrite
    (
    ULONG ulWriteAddr, 
    volatile ULONG ulSrc
    )
    {
    int i;

    ulWriteAddr &= 0xffffc000; /* block address align */
   
    for(i = 0; i < NAND_PAGE_PER_BLOCK; i++)
        {
        if(fcmPageWrite(ulWriteAddr, ulSrc) == ERROR)
        {
            printf("\r\nnand write fail!");
            return (ERROR);
        }
        ulWriteAddr += NAND_PAGE_SIZE;
        ulSrc += NAND_PAGE_SIZE;
        }

    return (OK);
    }

