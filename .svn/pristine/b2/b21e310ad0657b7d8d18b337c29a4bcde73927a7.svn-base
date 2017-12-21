/* sysMpc8540PciError.c -  Error Handling for the MPC8540 PCI */

/* Copyright 2005 Motorola Inc. All Rights Reserved */


/*
modification history
--------------------
01a,18aug05,efb  Created.
*/

/*
DESCRIPTION
This file provides handling for the PCI/X error interrupt. If error
handling has been included the error is fully logged, otherwise the
interrupt is simply cleared.

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysMpc8540PciError.h"
#include "sysMpc8540Error.h"

/* defines */

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

/******************************************************************************
*
* sysMpc8540PciErrorInt - Interrupt handling routine PCI/X errors
*
* This function is invoked when an PCI/X error occurs. It will
* call the error scanning function to do a scan of the PCI/X error detect
* register to verify that an associated error actually occurred.  If the
* error indeed occurred, the scanning function will log the error.
*
* RETURNS: NA
*/

LOCAL void sysMpc8540PciErrorInt
    (
    int param ARG_UNUSED
    )
    {
    UINT32 smcErrDetReg = 0x0;
    UINT32 id = 0x0;
    UINT32 mask = 0x00;
#ifdef INCLUDE_ERROR_HANDLING
    ERROR_MSG smcErrorMsg;
#endif /* INCLUDE_ERROR_HANDLING */

    smcErrDetReg = CCSR_READ32 (CCSBAR, CCSR_ERR_DR);

    if ((smcErrDetReg & PCI_ERROR_DETECT_MULT_ERR_MASK) ==
	PCI_ERROR_DETECT_MULT_ERR_MASK)
        {
        id = PCI_ERR_MULTI_ERR;
        mask = PCI_ERROR_DETECT_MULT_ERR_MASK;
        }

    if ((smcErrDetReg & PCI_ERROR_DETECT_ADDR_PARITY_MASK) ==
	PCI_ERROR_DETECT_ADDR_PARITY_MASK)
        {
        id = PCI_ERR_ADDR_PARITYERR;
        mask |= PCI_ERROR_DETECT_ADDR_PARITY_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_RCVD_SERR_MASK) ==
	PCI_ERROR_DETECT_RCVD_SERR_MASK)
        {
        id = PCI_ERR_RCVD_SERR;
        mask |= PCI_ERROR_DETECT_RCVD_SERR_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_MSTR_PERR_MASK) ==
	PCI_ERROR_DETECT_MSTR_PERR_MASK)
        {
        id = PCI_ERR_MSTR_PERR;
        mask |= PCI_ERROR_DETECT_MSTR_PERR_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_TRGT_PERR_MASK) ==
	PCI_ERROR_DETECT_TRGT_PERR_MASK)
        {
        id = PCI_ERR_TRGT_PERR;
        mask |= PCI_ERROR_DETECT_TRGT_PERR_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_MSTR_ABORT_MASK) ==
	PCI_ERROR_DETECT_MSTR_ABORT_MASK)
        {
        id = PCI_ERR_MSTR_ABORTERR;
        mask |= PCI_ERROR_DETECT_MSTR_ABORT_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_TRGT_ABORT_MASK) ==
	PCI_ERROR_DETECT_TRGT_ABORT_MASK)
        {
        id = PCI_ERR_TRGT_ABORTERR;
        mask |= PCI_ERROR_DETECT_TRGT_ABORT_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_OWMSV_MASK) ==
	PCI_ERROR_DETECT_OWMSV_MASK)
        {
        id = PCI_ERR_OWMSVERR;
        mask |= PCI_ERROR_DETECT_OWMSV_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_ORMSV_MASK) ==
	PCI_ERROR_DETECT_ORMSV_MASK)
        {
        id = PCI_ERR_ORMSVERR;
        mask |= PCI_ERROR_DETECT_ORMSV_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_IRMSV_MASK) ==
	PCI_ERROR_DETECT_IRMSV_MASK)
        {
        id = PCI_ERR_IRMSVERR;
        mask |= PCI_ERROR_DETECT_IRMSV_MASK;
        } 
    else if ((smcErrDetReg & PCI_ERROR_DETECT_SCM_MASK) ==
	PCI_ERROR_DETECT_SCM_MASK)
        {
        id = PCI_ERR_SCMERR;
        mask |= PCI_ERROR_DETECT_SCM_MASK;
        } 
    else 
        {
        id = PCI_ERR_TOEERR;
        mask |= PCI_ERROR_DETECT_TOE_MASK;
        } 

#ifdef INCLUDE_ERROR_HANDLING

    smcErrorMsg.id = id;

    smcErrorMsg.params[0] = CCSR_READ32 (CCSBAR, CCSR_ERR_DR);
    smcErrorMsg.params[1] = CCSR_READ32 (CCSBAR, CCSR_ERR_ATTRIB);

    if ((smcErrorMsg.params[1] & PCI_ERROR_ATTR_VLD_MASK) == 
        PCI_ERROR_ATTR_VLD_MASK)
        {
        smcErrorMsg.params[2] = CCSR_READ32 (CCSBAR, CCSR_ERR_ADDR);
        smcErrorMsg.params[3] = CCSR_READ32 (CCSBAR, CCSR_ERR_EXT_ADDR);
        smcErrorMsg.params[4] = CCSR_READ32 (CCSBAR, CCSR_ERR_DL);
        smcErrorMsg.params[5] = CCSR_READ32 (CCSBAR, CCSR_ERR_DH);
        }
    else
        {
        smcErrorMsg.params[2] = 0;
        smcErrorMsg.params[3] = 0;
        smcErrorMsg.params[4] = 0;
        smcErrorMsg.params[5] = 0;
        }

    /* log the error */

    sysMpc8540ErrorLog(&smcErrorMsg);

#endif /* INCLUDE_ERROR_HANDLING */

    /* clear the error */

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DR, mask);
    }

/******************************************************************************
*
* sysMpc8540PciErrorIntEnable - PCI/X error interrupts
*
* This routine enables PCI/X error interrupts.
*
* RETURNS: ERROR if intConnect or intEnable returned ERROR, OK otherwise.
*/

STATUS sysMpc8540PciErrorIntEnable (void)
    {
    STATUS status = OK;
    static BOOL smcErrorIntInitDone = FALSE;

    /*
     * Ensure HID1[RFXE] is cleared so that assertion of
     * core_fault_in cannot cause a machine check for
     * an PCI error, instead an interrupt will be
     * generated.
     */

    vxHid1Set (vxHid1Get() & ~0x00020000);

    /* clear any reported errors */

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DR, 
                  PCI_ERROR_DETECT_MULT_ERR_MASK |
                  PCI_ERROR_DETECT_ADDR_PARITY_MASK | 
                  PCI_ERROR_DETECT_RCVD_SERR_MASK |
                  PCI_ERROR_DETECT_MSTR_PERR_MASK |
                  PCI_ERROR_DETECT_TRGT_PERR_MASK |
                  PCI_ERROR_DETECT_MSTR_ABORT_MASK |
                  PCI_ERROR_DETECT_TRGT_ABORT_MASK |
                  PCI_ERROR_DETECT_OWMSV_MASK |
                  PCI_ERROR_DETECT_ORMSV_MASK |
                  PCI_ERROR_DETECT_IRMSV_MASK |
                  PCI_ERROR_DETECT_SCM_MASK |
                  PCI_ERROR_DETECT_TOE_MASK);

    /* generate interrupt upon error */

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_CAP_DR, 0x00);

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_EN, 
                  PCI_ERROR_INT_ADDR_PARITY_MASK | 
                  PCI_ERROR_INT_RCVD_SERR_MASK |
                  PCI_ERROR_INT_MSTR_PERR_MASK |
                  PCI_ERROR_INT_TRGT_PERR_MASK |
                  PCI_ERROR_INT_MSTR_ABORT_MASK |
                  PCI_ERROR_INT_TRGT_ABORT_MASK |
                  PCI_ERROR_INT_OWMSV_MASK |
                  PCI_ERROR_INT_ORMSV_MASK |
                  PCI_ERROR_INT_IRMSV_MASK |
                  PCI_ERROR_INT_SCM_MASK |
                  PCI_ERROR_INT_TOE_MASK);

    /* Connect an interrupt handler if none has already been connected */

    if (!smcErrorIntInitDone)
        {
	if (intConnect ((VOIDFUNCPTR *)((int)EPIC_PCI_INT_VEC),
                        (VOIDFUNCPTR)sysMpc8540PciErrorInt, 0) == OK)
	    {
            smcErrorIntInitDone = TRUE;

            /* Enable the interrupt */

            status = intEnable (EPIC_PCI_INT_VEC);
	    }
        else
            {
 
            /* Interrupt Connect failed */

            status = ERROR;
            }
        }

    return (status);
    }
