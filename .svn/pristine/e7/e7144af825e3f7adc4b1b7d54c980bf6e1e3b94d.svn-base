/* sysMpc8540SmcError.h -  MPC8540 Error Handling for the SMC */

/* Copyright 2005 Motorola Inc. All Rights Reserved */

/*

modification history
--------------------
01a,18aug05,efb  New file.
*/

/*
DESCRIPTION

Register bit definitions, masks, and legal field values for the 
system memory controller (SMC) are defined in this file.

*/

#ifndef	INCsysMpc8540SmcErrorh
#define	INCsysMpc8540SmcErrorh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* includes */

#include "vxWorks.h"

/* defines */

/* SMC Error Detect Register */

#define SMC_ERROR_DETECT_MME_BIT  0  
#define SMC_ERROR_DETECT_MME_MASK (0x80000000UL >> SMC_ERROR_DETECT_MME_BIT)

#define SMC_ERROR_DETECT_MBE_BIT  28 
#define SMC_ERROR_DETECT_MBE_MASK (0x80000000UL >> SMC_ERROR_DETECT_MBE_BIT)

#define SMC_ERROR_DETECT_SBE_BIT  29 
#define SMC_ERROR_DETECT_SBE_MASK (0x80000000UL >> SMC_ERROR_DETECT_SBE_BIT)

#define SMC_ERROR_DETECT_MSE_BIT  31 
#define SMC_ERROR_DETECT_MSE_MASK (0x80000000UL >> SMC_ERROR_DETECT_MSE_BIT)

/* SMC Error Disable Register */

#define SMC_ERROR_DISABLE_MBED_BIT  28 
#define SMC_ERROR_DISABLE_MBED_MASK (0x80000000UL >> SMC_ERROR_DISABLE_MBED_BIT)

#define SMC_ERROR_DISABLE_SBED_BIT  29 
#define SMC_ERROR_DISABLE_SBED_MASK (0x80000000UL >> SMC_ERROR_DISABLE_SBED_BIT)

#define SMC_ERROR_DISABLE_MSED_BIT  31 
#define SMC_ERROR_DISABLE_MSED_MASK (0x80000000UL >> SMC_ERROR_DISABLE_MSED_BIT)

/* SMC Error Interrupt Enable Register */

#define SMC_ERROR_INT_MBEE_BIT  28 
#define SMC_ERROR_INT_MBEE_MASK (0x80000000UL >> SMC_ERROR_INT_MBEE_BIT)

#define SMC_ERROR_INT_SBEE_BIT  29 
#define SMC_ERROR_INT_SBEE_MASK (0x80000000UL >> SMC_ERROR_INT_SBEE_BIT)

#define SMC_ERROR_INT_MSEE_BIT  31 
#define SMC_ERROR_INT_MSEE_MASK (0x80000000UL >> SMC_ERROR_INT_MSEE_BIT)

/* SMC Error Attributes Capture Register */

#define SMC_ERROR_ATTR_BNUM_BIT    1
#define SMC_ERROR_ATTR_BNUM_MASK   (0xE0000000UL >> SMC_ERROR_ATTR_BNUM_BIT)

#define SMC_ERROR_ATTR_TSIZ_BIT    5
#define SMC_ERROR_ATTR_TSIZ_MASK   (0xE0000000UL >> SMC_ERROR_ATTR_TSIZ_BIT)

#define SMC_ERROR_ATTR_TSRC_BIT    11
#define SMC_ERROR_ATTR_TSRC_MASK   (0xF8000000UL >> SMC_ERROR_ATTR_TSRC_BIT)

#define SMC_ERROR_ATTR_TTYP_BIT    18
#define SMC_ERROR_ATTR_TTYP_MASK   (0xC0000000UL >> SMC_ERROR_ATTR_TTYP_BIT)

#define SMC_ERROR_ATTR_VLD_BIT     31
#define SMC_ERROR_ATTR_VLD_MASK    (0x80000000UL >> SMC_ERROR_ATTR_VLD_BIT)

#define SMC_ERROR_TSRC_PCI                  0x00
#define SMC_ERROR_TSRC_LOCAL_BUS            0x04
#define SMC_ERROR_TSRC_CONFIG_SPACE         0x08
#define SMC_ERROR_TSRC_BOOT_SEQNCR          0x0A
#define SMC_ERROR_TSRC_RAPIDIO              0x0C
#define SMC_ERROR_TSRC_PROC_INSTR           0x10
#define SMC_ERROR_TSRC_PROC_DATA            0x11
#define SMC_ERROR_TSRC_DMA                  0x15
#define SMC_ERROR_TSRC_SAP                  0x17
#define SMC_ERROR_TSRC_TSEC1                0x18
#define SMC_ERROR_TSRC_TSEC2                0x19
#define SMC_ERROR_TSRC_FEC                  0x1A
#define SMC_ERROR_TSRC_RAPIDIO_MSG          0x1C
#define SMC_ERROR_TSRC_RAPIDIO_DBELL        0x1D
#define SMC_ERROR_TSRC_RAPIDIO_WRITE        0x1E

/* SMC Single-bit ECC Memory Error Management */

#define SMC_SINGLE_BIT_ECC_ERROR_SBET_BIT   8  
#define SMC_SINGLE_BIT_ECC_ERROR_SBET_MASK  \
        (0xFF000000UL >> SMC_SINGLE_BIT_ECC_ERROR_SBET_MASK)

#define SMC_SINGLE_BIT_ECC_ERROR_SBEC_BIT   24  
#define SMC_SINGLE_BIT_ECC_ERROR_SBEC_MASK  \
        (0xFF000000UL >> SMC_SINGLE_BIT_ECC_ERROR_SCET_MASK)

STATUS sysMpc8540SmcErrorIntEnable (void);

#ifdef __cplusplus

    }
#endif /* __cplusplus */

#endif /* sysMpc8540SmcErrorh */
