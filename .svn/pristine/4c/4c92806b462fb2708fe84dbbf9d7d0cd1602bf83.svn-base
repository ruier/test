/* sysMpc8540Error.c  - catch and log MPC8540 asserted errors */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01e,18aug05,efb  Added support for handling SMC and PCI/X errors.
01d,22jul05,efb  Added support for LBC errors.
01c,19jul05,efb  Added support for EMC errors.
01b,24mar05,cak  Added support for DMA errors.
01a,09feb05,cak  Created.
*/

/*
DESCRIPTION

This module implements error handling for the interfaces located 
within the MPC8540.

*/

/* includes */

#include "vxWorks.h"
#include "errorHandler.h"
#include "sysMpc8540Error.h"
#include "sysMpc8540EcmError.h"
#include "sysMpc8540LbcError.h"
#include "sysMpc8540SmcError.h"
#include "sysMpc8540PciError.h"
#include "mpc8540.h"

/* defines */

/* typedefs */

/* globals */

ERROR_UTILS mpc8540ErrorUtils = { NULL };

/* locals */

/* forward declarations */

void sysMpc8540ErrorInit (void);
STATUS sysMpc8540ErrIntEnable (void);
void sysMpc8540ErrorLog (ERROR_MSG *);
LOCAL void sysMpc8540ErrorRegister (void);
LOCAL char * mpc8540ErrorFormat (ERROR_ID id);

/* externals */

/******************************************************************************
*
* sysMpc8540ErrorLog - log an MPC8540 error
*
* This routine will log an MPC8540 error.
*
* RETURNS: NA
*/

void sysMpc8540ErrorLog 
    (
    ERROR_MSG * errorMsg	/* error message to be logged */
    )
    {
    if (mpc8540ErrorUtils.errorLog != NULL)
	(* mpc8540ErrorUtils.errorLog) (errorMsg);
    }

/******************************************************************************
*
* sysMpc8540ErrorInit - Initialize error monitoring for MPC8540 interfaces 
*
* This routine will initialize the error domain for the MPC8540. 
*
* RETURNS: NA
*/

void sysMpc8540ErrorInit (void)
    {
    ERROR_USR_FUNCS myErrorFuncs = { mpc8540ErrorFormat,
                                     NULL,
                                     NULL 
                                   };

    if (mpc8540ErrorUtils.errorLog != NULL)
         return;                /* Already been through here */

    /* Initialize MPC8540 domain */

    if ( _func_errorHandlerInit == NULL)
        return;                 /* No error handler module is linked */

    mpc8540ErrorUtils = (* _func_errorHandlerInit)
                                (ERROR_CPU_TYPE,
                                 "MPC8540",
                                 &myErrorFuncs);

    if (mpc8540ErrorUtils.errorLog == NULL)
         return;                /* Error handler module is dead */

    /*
     *  MPC8540 error domain is now initialized.  We can now
     *  register the errors that we will be monitoring.
     */

    sysMpc8540ErrorRegister ();

    }

/******************************************************************************
*
* sysMpc8540ErrIntEnable - Initialize and enable MPC8540 interface interrupts
*
* This routine initializes and enables error handling/reporting
* interrupts for the MPC8540 interfaces.
*
* RETURNS: ERROR if any interrupt enabling routine returned ERROR, 
* OK otherwise.
*/

STATUS sysMpc8540ErrIntEnable (void)
    {
    STATUS status = OK;

#ifdef INCLUDE_L2_CACHE_SRAM
    status = sysL2CacheErrIntEnable(); 
#endif

    if (status == OK)
        {
        status = sysMpc8540EcmErrorIntEnable(); 

        if (status == OK)
            {
            status = sysMpc8540LbcErrorIntEnable(); 
            }

        if (status == OK)
            {
            status = sysMpc8540SmcErrorIntEnable(); 
            }

        if (status == OK)
            {
            status = sysMpc8540PciErrorIntEnable(); 
            }
        }

    return (status);
    }

/******************************************************************************
*
* sysMpc8540ErrorRegister - Register errors for MPC8540 
*
* This routine will register errors associated with the MPC8540. 
*
* RETURNS: NA
*/

LOCAL void sysMpc8540ErrorRegister (void)
    {
#ifdef INCLUDE_L2_CACHE_SRAM

    /* Register Tag Parity Error */

    (* mpc8540ErrorUtils.errorRegister) (L2CACHE_ERR_TPARERR, 
        ERROR_DISP_SAVE, 0, 0);

    /* Register Multiple-Bit ECC Error */

    (* mpc8540ErrorUtils.errorRegister) (L2CACHE_ERR_MBECCERR, 
        ERROR_DISP_SAVE, 0, 0);

    /* Register Single-Bit ECC Error */

    (* mpc8540ErrorUtils.errorRegister) (L2CACHE_ERR_SBECCERR, 
        ERROR_DISP_SAVE, 0, 0);

    /* Register L2 Configuration Error */

    (* mpc8540ErrorUtils.errorRegister) (L2CACHE_ERR_L2CFGERR, 
        ERROR_DISP_SAVE, 0, 0);
#endif /* INCLUDE_L2_CACHE_SRAM */

#ifdef INCLUDE_MPC8540_DMA

    /* Register DMA errors (transfer and programming) */

    (* mpc8540ErrorUtils.errorRegister) (DMA_ERR,
        ERROR_DISP_SAVE, 0, 0);
#endif /* INCLUDE_MPC8540_DMA */

    /* Register ECM local access errors  */

    (* mpc8540ErrorUtils.errorRegister) (ECM_ERR_LAE,
        ERROR_DISP_SAVE, 0, 0);

    (* mpc8540ErrorUtils.errorRegister) (ECM_ERR_MULTI_LAE,
        ERROR_DISP_SAVE, 0, 0);

    /* Register LBC bus monitor time-out transfer errors */

    (* mpc8540ErrorUtils.errorRegister) (LBC_ERR_BMTOERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register LBC parity transfer errors */

    (* mpc8540ErrorUtils.errorRegister) (LBC_ERR_PARERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register LBC write protect transfer errors */

    (* mpc8540ErrorUtils.errorRegister) (LBC_ERR_WPERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register LBC atomic error write transfer errors */

    (* mpc8540ErrorUtils.errorRegister) (LBC_ERR_ATMWERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register LBC atomic error read transfer errors */

    (* mpc8540ErrorUtils.errorRegister) (LBC_ERR_ATMRERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register LBC chip select transfer errors */

    (* mpc8540ErrorUtils.errorRegister) (LBC_ERR_CSERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register SMC Multiple Memory errors */

    (* mpc8540ErrorUtils.errorRegister) (SMC_ERR_MULTI_MEMERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register SMC Multiple-Bit Memory errors */

    (* mpc8540ErrorUtils.errorRegister) (SMC_ERR_MULTI_BITERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register SMC Single-Bit Memory errors */

    (* mpc8540ErrorUtils.errorRegister) (SMC_ERR_SINGLE_BITERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register SMC Memory Select errors */

    (* mpc8540ErrorUtils.errorRegister) (SMC_ERR_MEM_SELERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI Mulitple Same-type errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_MULTI_ERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI Address Parity errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_ADDR_PARITYERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI Rcvd ~SERR errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_RCVD_SERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI Mstr ~PERR errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_MSTR_PERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI Trgt ~PERR errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_TRGT_PERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI Mstr Abort errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_MSTR_ABORTERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI Trgt Abort errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_TRGT_ABORTERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI OWMSV errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_OWMSVERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI ORMSV errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_ORMSVERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI IRMSV errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_IRMSVERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI SCM errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_SCMERR,
        ERROR_DISP_SAVE, 0, 0);

    /* Register PCI TOE errors */

    (* mpc8540ErrorUtils.errorRegister) (PCI_ERR_TOEERR,
        ERROR_DISP_SAVE, 0, 0);

    }

/******************************************************************************
*
* mpc8540ErrorFormat - Format a message for error handling module
*
* This is the standard "error formatting" helper function for the error
* handling module.  When called from the error handling module, it will
* return an error format string (or NULL if one does not exist) associated
* with the requested error ID.
*
* RETURNS: Error formatting string or NULL if one does not exit.
*/

LOCAL char * mpc8540ErrorFormat
    (
    ERROR_ID id
    )
    {
    switch (id)
        {
#ifdef INCLUDE_L2_CACHE_SRAM
	case L2CACHE_ERR_TPARERR:
	    return ("Tag Parity Error:" L2ERR_STR);

	case L2CACHE_ERR_MBECCERR:
	    return ("Multiple-bit ECC Error:" L2ERR_STR);

	case L2CACHE_ERR_SBECCERR:
	    return ("Single-bit ECC Error:" L2ERR_STR);

	case L2CACHE_ERR_L2CFGERR:
	    return ("L2 Configuration Error:" L2ERR_STR); 
#endif /* INCLUDE_L2_CACHE_SRAM */

#ifdef INCLUDE_MPC8540_DMA
	case DMA_ERR:
	    return ("DMA Error:" DMAERR_STR);
#endif /* INCLUDE_MPC8540_DMA */

        case ECM_ERR_LAE:
            return ("ECM Local Access Error:" ECMERR_STR);

        case ECM_ERR_MULTI_LAE:
            return ("ECM Multiple Same Type Local Access Errors:" ECMERR_STR);

        case LBC_ERR_BMTOERR:
            return ("LBC Bus Monitor Time-out Error:" LBCERR_STR);

        case LBC_ERR_PARERR:
            return ("LBC Parity Error:" LBCERR_STR);

        case LBC_ERR_WPERR:
            return ("LBC Write Protect Error:" LBCERR_STR);

        case LBC_ERR_ATMWERR:
            return ("LBC Atomic Write Error:" LBCERR_STR);

        case LBC_ERR_ATMRERR:
            return ("LBC Atomic Read Error:" LBCERR_STR);

        case LBC_ERR_CSERR:
            return ("LBC Chip Select Error:" LBCERR_STR);

        case SMC_ERR_MULTI_MEMERR:
            return ("DDR SMC Multiple Same Type Mem Errors:" SMCERR_STR);

        case SMC_ERR_MULTI_BITERR:
            return ("DDR SMC Multiple-bit Error:" SMCERR_STR);

        case SMC_ERR_MEM_SELERR:
            return ("DDR SMC Memory Select Error:" SMCERR_STR);

        case SMC_ERR_SINGLE_BITERR:
            return ("DDR SMC Single-bit Errors:" SMCERR_STR);

        case PCI_ERR_MULTI_ERR:
            return ("PIC/X Multiple Same Type Errors:" PCIERR_STR);

        case PCI_ERR_ADDR_PARITYERR:
            return ("PIC/X Address Parity Error:" PCIERR_STR);

        case PCI_ERR_RCVD_SERR:
            return ("PIC/X Received ~SERR Error:" PCIERR_STR);

        case PCI_ERR_MSTR_PERR:
            return ("PIC/X Master ~PERR Error:" PCIERR_STR);

        case PCI_ERR_TRGT_PERR:
            return ("PIC/X Target ~PERR Error:" PCIERR_STR);

        case PCI_ERR_MSTR_ABORTERR:
            return ("PIC/X Master Abort Error:" PCIERR_STR);

        case PCI_ERR_TRGT_ABORTERR:
            return ("PIC/X Target Abort Error:" PCIERR_STR);

        case PCI_ERR_OWMSVERR:
            return ("PIC/X OWMSV Error:" PCIERR_STR);

        case PCI_ERR_ORMSVERR:
            return ("PIC/X ORMSV Error:" PCIERR_STR);

        case PCI_ERR_IRMSVERR:
            return ("PIC/X IRMSV Error:" PCIERR_STR);

        case PCI_ERR_SCMERR:
            return ("PIC/X SCM Error:" PCIERR_STR);

        case PCI_ERR_TOEERR:
            return ("PIC/X TOE Error:" PCIERR_STR);

        default:
            return (NULL);
        }
    }
