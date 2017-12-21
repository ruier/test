/* sysNandFlash.c - Nand Flash Driver for MPC830x mixt board */

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
01a,30jun11,e_d initial creation
*/

/*
DESCRIPTION

This file provides Nand Flash Driver routines for MPC830x board. There is a 512Mb
NAND flash on this board, Before using read and write routines, fcmReset()
and fcmInit() should be run to initialize NAND controller.
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <iosLib.h>
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

STATUS fcmReadID
    (
    char * NandID
    )
    {
    int timeOut = 0;
    ULONG ulTemp = 0;

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_CW0_OPX(FIR_OP0_SHIFT) |
                                                 FIR_UA_OPX(FIR_OP1_SHIFT) |
                                                 FIR_RBW_OPX(FIR_OP2_SHIFT)));

    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, (UINT32)FCR_CMD0(MICRO_NAND_READID));

    /* 5 bytes for manuf, device and exts */

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 5);

    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);

    ulTemp |= FMR_OP_EXENOPOT;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(FCM_DATA_REG, MDR_AS0(0));

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG, 0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG,NAND_BANK);

    while((((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
             && ((timeOut++ ) < TIMEOUT))
        ;

    /* clear sts bit for FCM mode */

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    if(timeOut >= TIMEOUT)
        return (ERROR);

    bcopyBytes ((char *) NAND_BUFFER_BASE, NandID, 5);

    return (OK);

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

STATUS fcmPageRead
    (
    ULONG ulReadAddr,
    ULONG * ulDst
    )
    {
    int i;
    volatile ULONG ulTemp;
    volatile ULONG *pSrc;
    int timeOut = 0;

    if(ulDst == NULL)
        return ERROR;

    ulTemp = MICRO_NAND_PAGECAL(ulReadAddr);

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_CM0_OPX(FIR_OP0_SHIFT)|
                                                 FIR_CA_OPX(FIR_OP1_SHIFT)|
                                                 FIR_PA_OPX(FIR_OP2_SHIFT)|
                                                 FIR_CM1_OPX(FIR_OP3_SHIFT)|
                                                 FIR_RBW_OPX(FIR_OP4_SHIFT)));

    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD0(MICRO_NAND_READFIRST) |
                                           FCR_CMD1(MICRO_NAND_READSEC));

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, ulTemp >> 6);

    FCM_REG_WRITE_ULONG(FLASH_PAGEADDR_REG, MICRO_NAND_PAGESET
                                            (MICRO_NAND_PAGECAL(ulReadAddr)));
    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);

    ulTemp |= FMR_OP_EXENOPOT | FMR_AL_4BYTE;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG, 0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG, NAND_BANK);

    while((((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
             && ((timeOut++ ) < TIMEOUT))
        ;

    if(timeOut >= TIMEOUT)
        return (ERROR);

    pSrc  = (volatile ULONG *)(NAND_BUFFER_BASE + ((MICRO_NAND_PAGECAL(ulReadAddr)
                                                     & FCM_LARBUF_MASK)
                                                     * FCM_LARBUF_SIZE));

    for (i = NAND_PAGE_SIZE / sizeof (ULONG); i; i--)
        {
        *(ulDst++) = *(pSrc++);
        }

    return OK;
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

STATUS fcmBlockErase
    (
    ULONG ulEraseAddr
    )
    {
    int timeOut = 0;
    volatile ULONG ulTemp;

    ulTemp = MICRO_NAND_PAGECAL(ulEraseAddr);

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_CW0_OPX(FIR_OP0_SHIFT)|
                                                 FIR_PA_OPX(FIR_OP1_SHIFT)|
                                                 FIR_CM1_OPX(FIR_OP2_SHIFT)));

    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD0(MICRO_NAND_ERASEFIRST) |
                                           FCR_CMD1(MICRO_NAND_ERASESEC));

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, ulTemp >> 6);
    FCM_REG_WRITE_ULONG(FLASH_PAGEADDR_REG, MICRO_NAND_PAGESET
                                            (MICRO_NAND_PAGECAL(ulEraseAddr)));

    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);

    ulTemp |= FMR_OP_EXENOPOT | FMR_AL_4BYTE;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG,0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,(LTESR_FCT |
                       LTESR_PAR | LTESR_CC | LTESR_CS));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG,NAND_BANK);

    while((((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
             && ((timeOut++ ) < TIMEOUT))
        ;

    if(timeOut >= TIMEOUT)
        return (ERROR);
    return OK;

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

STATUS fcmReset()
    {
    int timeOut;
    volatile ULONG ulTemp;

    timeOut = 0x0;

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_CW0_OPX(FIR_OP0_SHIFT) |
                                                 FIR_RSW_OPX(FIR_OP1_SHIFT)));

    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, (UINT32)FCR_CMD0(MICRO_NAND_RESET));

    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);

    ulTemp |= FMR_OP_EXENOPOT;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG, 0);

    /* clear sts bit for FCM mode */

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG,NAND_BANK);

    while((((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
             && ((timeOut++ ) < TIMEOUT))
        ;

    /* clear sts bit for FCM mode */

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,
                        (LTESR_FCT | LTESR_PAR | LTESR_CC));

    if(timeOut >= TIMEOUT)
        return (ERROR);

    return (OK);
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
    ULONG * ulSrc
    )
    {
    int i;
    volatile ULONG * pSrc ;
    volatile ULONG   ulTemp;
    int timeOut = 0;

    ulTemp = MICRO_NAND_PAGECAL(ulWriteAddr);

    pSrc  = (volatile ULONG *)(NAND_BUFFER_BASE + ((MICRO_NAND_PAGECAL(ulWriteAddr)
                                                    & FCM_LARBUF_MASK)
                                                    * FCM_LARBUF_SIZE));

    for (i = NAND_PAGE_SIZE / sizeof (ULONG); i; i--)
        {
        *(pSrc++) = *(ulSrc++);
        }

    FCM_REG_WRITE_ULONG(FLASH_BYTECOUNT_REG, 0);

    FCM_REG_WRITE_ULONG(FLASH_INSTRUCTION_REG, ( FIR_CW0_OPX(FIR_OP0_SHIFT)|
                                                 FIR_CA_OPX(FIR_OP1_SHIFT)|
                                                 FIR_PA_OPX(FIR_OP2_SHIFT)|
                                                 FIR_WB_OPX(FIR_OP3_SHIFT)|
                                                 FIR_CW1_OPX(FIR_OP4_SHIFT)));

    FCM_REG_WRITE_ULONG(FLASH_COMMAND_REG, FCR_CMD0(MICRO_NAND_WRITEFIRST) |
                                           FCR_CMD1(MICRO_NAND_WRITESEC));

    FCM_REG_WRITE_ULONG(FLASH_BLOCKADDR_REG, ulTemp >> 6);

    FCM_REG_WRITE_ULONG(FLASH_PAGEADDR_REG, MICRO_NAND_PAGESET
                                            (MICRO_NAND_PAGECAL(ulWriteAddr)));
    ulTemp = FCM_REG_READ_ULONG(FLASH_MODE_REG);

    ulTemp |= FMR_OP_EXENOPOT | FMR_AL_4BYTE;

    FCM_REG_WRITE_ULONG(FLASH_MODE_REG, ulTemp);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_ATT_REG,0);

    FCM_REG_WRITE_ULONG(TRANSFER_ERROR_STU_REG,(LTESR_FCT |
                       LTESR_PAR | LTESR_CC | LTESR_CS));

    FCM_REG_WRITE_ULONG(SPECIAL_OP_INIT_REG,NAND_BANK);

    while((((FCM_REG_READ_ULONG(TRANSFER_ERROR_STU_REG)) & LTESR_CC) == 0 )
             && ((timeOut++ ) < TIMEOUT))
        ;

    if(timeOut >= TIMEOUT)
        return (ERROR);

    return OK;

     }

