/* sysMpc8540SmcError.c -  Error Handling for the MPC8540 SMC */

/* Copyright 2005 Motorola Inc. All Rights Reserved */


/*
modification history
--------------------
01a,18aug05,efb  Created.
*/

/*
DESCRIPTION
This file provides handling for the SMC error interrupt. If error
handling has been included the error is fully logged, otherwise the
interrupt is simply cleared.

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysMpc8540SmcError.h"
#include "sysMpc8540Error.h"

/* defines */

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

/******************************************************************************
*
* sysMpc8540SmcErrorInt - Interrupt handling routine SMC errors
*
* This function is invoked when an SMC error occurs. It will
* call the error scanning function to do a scan of the SMC error detect
* register to verify that an associated error actually occurred.  If the
* error indeed occurred, the scanning function will log the error.
*
* RETURNS: NA
*/

LOCAL void sysMpc8540SmcErrorInt
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

    smcErrDetReg = CCSR_READ32 (CCSBAR, CCSR_ERR_DETECT);

    if ((smcErrDetReg & SMC_ERROR_DETECT_MME_MASK) ==
	SMC_ERROR_DETECT_MME_MASK)
        {
        id = SMC_ERR_MULTI_MEMERR;
        mask = SMC_ERROR_DETECT_MME_MASK;
        }

   if ((smcErrDetReg & SMC_ERROR_DETECT_MBE_MASK) ==
	SMC_ERROR_DETECT_MBE_MASK)
        {
        id = SMC_ERR_MULTI_BITERR;
        mask |= SMC_ERROR_DETECT_MBE_MASK;
        } 
    else if ((smcErrDetReg & SMC_ERROR_DETECT_MSE_MASK) ==
	SMC_ERROR_DETECT_MSE_MASK)
        {
        id = SMC_ERR_MEM_SELERR;
        mask |= SMC_ERROR_DETECT_MSE_MASK;
        } 
    else
        {
        id = SMC_ERR_SINGLE_BITERR;
        mask |= SMC_ERROR_DETECT_SBE_MASK;
        }

#ifdef INCLUDE_ERROR_HANDLING

    smcErrorMsg.id = id;

    smcErrorMsg.params[0] = CCSR_READ32 (CCSBAR, CCSR_ERR_DETECT);
    smcErrorMsg.params[1] = CCSR_READ32 (CCSBAR, CCSR_CAPTURE_ATTRIBUTES);

    if ((smcErrorMsg.params[1] & SMC_ERROR_ATTR_VLD_MASK) == 
        SMC_ERROR_ATTR_VLD_MASK)
        {
        smcErrorMsg.params[2] = CCSR_READ32 (CCSBAR, CCSR_CAPTURE_ADDRESS);
        }
    else
        {
        smcErrorMsg.params[2] = 0;
        }
        
    smcErrorMsg.params[3] = CCSR_READ32 (CCSBAR, CCSR_ERR_SBE);

    /* log the error */

    sysMpc8540ErrorLog(&smcErrorMsg);

#endif /* INCLUDE_ERROR_HANDLING */

    /* clear the error */

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DETECT, mask);
    }

/******************************************************************************
*
* sysMpc8540SmcErrorIntEnable - SMC error interrupts
*
* This routine enables SMC error interrupts.
*
* RETURNS: ERROR if intConnect or intEnable returned ERROR, OK otherwise.
*/

STATUS sysMpc8540SmcErrorIntEnable (void)
    {
    STATUS status = OK;
    static BOOL smcErrorIntInitDone = FALSE;

    /*
     * Ensure HID1[RFXE] is cleared so that assertion of
     * core_fault_in cannot cause a machine check for
     * an SMC error, instead an interrupt will be
     * generated.
     */

    vxHid1Set (vxHid1Get() & ~0x00020000);

    /* clear any reported errors */

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DETECT, SMC_ERROR_DETECT_MME_MASK |
                  SMC_ERROR_INT_MBEE_MASK | SMC_ERROR_DETECT_SBE_MASK | 
                  SMC_ERROR_DETECT_MSE_MASK);

    /* generate interrupt upon error */

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_DISABLE, 0x00);

    CCSR_WRITE32 (CCSBAR, CCSR_ERR_INT_EN, SMC_ERROR_INT_MBEE_MASK |
                  SMC_ERROR_INT_SBEE_MASK | SMC_ERROR_INT_MSEE_MASK);

    /* Connect an interrupt handler if none has already been connected */

    if (!smcErrorIntInitDone)
        {
	if (intConnect ((VOIDFUNCPTR *)((int)EPIC_DDR_INT_VEC),
                        (VOIDFUNCPTR)sysMpc8540SmcErrorInt, 0) == OK)
	    {
            smcErrorIntInitDone = TRUE;

            /* Enable the interrupt */

            status = intEnable (EPIC_DDR_INT_VEC);
	    }
        else
            {
 
            /* Interrupt Connect failed */

            status = ERROR;
            }
        }

    return (status);
    }
