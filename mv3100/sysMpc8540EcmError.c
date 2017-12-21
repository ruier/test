/* sysMpc8540EcmError.c -  Error Handling for the MPC8540 ECM */

/* Copyright 2005 Motorola Inc. All Rights Reserved */


/*
modification history
--------------------
01a,19jul05,efb  Created.
*/

/*
DESCRIPTION
This file provides handling for the ECM error interrupt. If error
handling has been included the error is fully logged, otherwise the
interrupt is simply cleared.

*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "sysMpc8540EcmError.h"
#include "sysMpc8540Error.h"

/* defines */

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

/******************************************************************************
*
* sysMpc8540EcmErrorInt - Interrupt handling routine ECM errors
*
* This function is invoked when an ECM error occurs. It will
* call the error scanning function to do a scan of the ECM error detect
* register to verify that an associated error actually occurred.  If the
* error indeed occurred, the scanning function will log the error.
*
* RETURNS: NA
*/

LOCAL void sysMpc8540EcmErrorInt
    (
    int param ARG_UNUSED
    )
    {
    UINT32 id = 0x0;
    UINT32 ecmErrDetReg = 0x0;
#ifdef INCLUDE_ERROR_HANDLING
    ERROR_MSG ecmErrorMsg;
#endif /* INCLUDE_ERROR_HANDLING */

    ecmErrDetReg = CCSR_READ32 (CCSBAR, CCSR_EEDR);

    id = ECM_ERR_LAE;

    if ((ecmErrDetReg & ECM_ERROR_DETECT_MULT_ERR_MASK) ==
	ECM_ERROR_DETECT_MULT_ERR_MASK)
        {
        id = ECM_ERR_MULTI_LAE;
        }

#ifdef INCLUDE_ERROR_HANDLING

    ecmErrorMsg.id = id;

    ecmErrorMsg.params[0] = CCSR_READ32 (CCSBAR, CCSR_EEDR);
    ecmErrorMsg.params[1] = CCSR_READ32 (CCSBAR, CCSR_EEATR);

    if ((ecmErrorMsg.params[1] & ECM_ERROR_ATTR_DATA_VAL_MASK)
        == ECM_ERROR_ATTR_DATA_VAL_MASK)
        {
        ecmErrorMsg.params[2] = CCSR_READ32 (CCSBAR, CCSR_EEADR);
        }

    /* log the error */

    sysMpc8540ErrorLog(&ecmErrorMsg);

#endif /* INCLUDE_ERROR_HANDLING */

    /* clear the error */

    CCSR_WRITE32 (CCSBAR, CCSR_EEDR, ECM_ERROR_DETECT_MULT_ERR_MASK |
                  ECM_ERROR_DETECT_LAE_MASK);
    }

/******************************************************************************
*
* sysMpc8540EcmErrorIntEnable - ECM error interrupts
*
* This routine enables e500 cohernecy module error interrupts.
*
* RETURNS: ERROR if intConnect or intEnable returned ERROR, OK otherwise.
*/

STATUS sysMpc8540EcmErrorIntEnable (void)
    {
    STATUS status = OK;
    static BOOL ecmErrorIntInitDone = FALSE;

    /*
     * Ensure HID1[RFXE] is cleared so that assertion of
     * core_fault_in cannot cause a machine check for
     * an ECM error, instead an interrupt will be
     * generated.
     */

    vxHid1Set (vxHid1Get() & ~0x00020000);

    /* clear any reported errors */

    CCSR_WRITE32 (CCSBAR, CCSR_EEDR, ECM_ERROR_DETECT_MULT_ERR_MASK |
                  ECM_ERROR_DETECT_LAE_MASK);

    /* generate interrupt upon error */

    CCSR_WRITE32 (CCSBAR, CCSR_EEER, ECM_ERROR_ENB_LAE_MASK);

    /* Connect an interrupt handler if none has already been connected */

    if (!ecmErrorIntInitDone)
        {
	if (intConnect ((VOIDFUNCPTR *)((int)EPIC_ECM_INT_VEC),
                        (VOIDFUNCPTR)sysMpc8540EcmErrorInt,0) == OK)
	    {
            ecmErrorIntInitDone = TRUE;

            /* Enable the interrupt */

            status = intEnable (EPIC_ECM_INT_VEC);
	    }
        else
            {
 
            /* Interrupt Connect failed */

            status = ERROR;
            }
        }

    return (status);
    }
