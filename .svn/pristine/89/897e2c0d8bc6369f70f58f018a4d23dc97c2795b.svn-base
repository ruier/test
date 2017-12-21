/* sysMpc8540LbcError.c -  Error Handling for the MPC8540 LBC */

/* Copyright 2005 Motorola Inc. All Rights Reserved */


/*
modification history
--------------------
01a,22jul05,efb  Created.
*/

/*
DESCRIPTION
This file provides handling for the LBC error interrupt. If error
handling has been included the error is fully logged, otherwise the
interrupt is simply cleared.

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysMpc8540LbcError.h"
#include "sysMpc8540Error.h"

/* defines */

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

/******************************************************************************
*
* sysMpc8540LbcErrorInt - Interrupt handling routine LBC errors
*
* This function is invoked when an LBC error occurs. It scans
* the LBC error status register to verify that an associated error actually 
* occurred.  If the error indeed occurred, the error is logged.
*
* RETURNS: NA
*/

LOCAL void sysMpc8540LbcErrorInt
    (
    int param ARG_UNUSED
    )
    {
    UINT32 id = 0x0;
    UINT32 mask = 0x0;
    UINT32 lbcErrStatReg = 0x0;
#ifdef INCLUDE_ERROR_HANDLING
    ERROR_MSG lbcErrorMsg;
#endif /* INCLUDE_ERROR_HANDLING */

    lbcErrStatReg = CCSR_READ32 (CCSBAR, CCSR_LTESR);

    if ((lbcErrStatReg & LBC_TRANS_ERROR_STATUS_BMTO_MASK) ==
	LBC_TRANS_ERROR_STATUS_BMTO_MASK)
        {
        id = LBC_ERR_BMTOERR;
        mask = LBC_TRANS_ERROR_STATUS_BMTO_MASK;
        }

    else if ((lbcErrStatReg & LBC_TRANS_ERROR_STATUS_PAR_MASK) ==
	LBC_TRANS_ERROR_STATUS_PAR_MASK)
        {
        id = LBC_ERR_PARERR;
        mask = LBC_TRANS_ERROR_STATUS_PAR_MASK;
        }

    else if ((lbcErrStatReg & LBC_TRANS_ERROR_STATUS_WP_MASK) ==
	LBC_TRANS_ERROR_STATUS_WP_MASK)
        {
        id = LBC_ERR_WPERR;
        mask = LBC_TRANS_ERROR_STATUS_WP_MASK;
        }

    else if ((lbcErrStatReg & LBC_TRANS_ERROR_STATUS_ATMW_MASK) ==
	LBC_TRANS_ERROR_STATUS_ATMW_MASK)
        {
        id = LBC_ERR_ATMWERR;
        mask = LBC_TRANS_ERROR_STATUS_ATMW_MASK;
        }

    else if ((lbcErrStatReg & LBC_TRANS_ERROR_STATUS_ATMR_MASK) ==
	LBC_TRANS_ERROR_STATUS_ATMR_MASK)
        {
        id = LBC_ERR_ATMRERR;
        mask = LBC_TRANS_ERROR_STATUS_ATMR_MASK;
        }

    else if ((lbcErrStatReg & LBC_TRANS_ERROR_STATUS_CS_MASK) ==
	LBC_TRANS_ERROR_STATUS_CS_MASK)
        {
        id = LBC_ERR_CSERR;
        mask = LBC_TRANS_ERROR_STATUS_CS_MASK;
        }

#ifdef INCLUDE_ERROR_HANDLING

    lbcErrorMsg.id = id;

    lbcErrorMsg.params[0] = CCSR_READ32 (CCSBAR, CCSR_LTESR);
    lbcErrorMsg.params[1] = CCSR_READ32 (CCSBAR, CCSR_LTEATR);

    if ((lbcErrorMsg.params[1] & LBC_TRANS_ERROR_ATTR_V_MASK)
        == LBC_TRANS_ERROR_ATTR_V_MASK)
        {
        lbcErrorMsg.params[2] = CCSR_READ32 (CCSBAR, CCSR_LTEAR);
        }
     else 
        {
         
        /* Attributes and address are not valid so clear params */

        lbcErrorMsg.params[1] = 0x00;
        lbcErrorMsg.params[2] = 0x00;
        }

    /* log the error */

    sysMpc8540ErrorLog(&lbcErrorMsg);

#endif /* INCLUDE_ERROR_HANDLING */

    /* clear the error */

    CCSR_WRITE32 (CCSBAR, CCSR_LTESR, mask);
    }

/******************************************************************************
*
* sysMpc8540LbcErrorIntEnable - LBC error interrupts
*
* This routine enables LBC transfer error interrupts.
*
* RETURNS: ERROR if intConnect or intEnable returned ERROR, OK otherwise.
*/

STATUS sysMpc8540LbcErrorIntEnable (void)
    {
    STATUS status = OK;
    static BOOL lbcErrorIntInitDone = FALSE;

    /*
     * Ensure HID1[RFXE] is cleared so that assertion of
     * core_fault_in cannot cause a machine check for
     * an LBC error, instead an interrupt will be
     * generated.
     */

    vxHid1Set (vxHid1Get() & ~0x00020000);

    /* clear any reported errors */

    CCSR_WRITE32 (CCSBAR, CCSR_LTESR, LBC_TRANS_ERROR_STATUS_BMTO_MASK |
                  LBC_TRANS_ERROR_STATUS_PAR_MASK |
                  LBC_TRANS_ERROR_STATUS_WP_MASK |
                  LBC_TRANS_ERROR_STATUS_ATMW_MASK |
                  LBC_TRANS_ERROR_STATUS_ATMR_MASK |
                  LBC_TRANS_ERROR_STATUS_CS_MASK);

    /* Enable error reporting */

    CCSR_WRITE32 (CCSBAR, CCSR_LTEDR, 0x00); 

    /* Generate interrupt upon error */

    CCSR_WRITE32 (CCSBAR, CCSR_LTEIR, LBC_TRANS_ERROR_STATUS_BMTO_MASK |
                  LBC_TRANS_ERROR_STATUS_PAR_MASK |
                  LBC_TRANS_ERROR_STATUS_WP_MASK |
                  LBC_TRANS_ERROR_STATUS_ATMW_MASK |
                  LBC_TRANS_ERROR_STATUS_ATMR_MASK |
                  LBC_TRANS_ERROR_STATUS_CS_MASK);

    /* Connect an interrupt handler if none has already been connected */

    if (!lbcErrorIntInitDone)
        {
	if (intConnect ((VOIDFUNCPTR *)((int)EPIC_LBC_INT_VEC),
                        (VOIDFUNCPTR)sysMpc8540LbcErrorInt, 0) == OK)
	    {
            lbcErrorIntInitDone = TRUE;

            /* Enable the interrupt */

            status = intEnable (EPIC_LBC_INT_VEC);
	    }
        else
            {
 
            /* Interrupt Connect failed */

            status = ERROR;
            }
        }

    return (status);
    }
