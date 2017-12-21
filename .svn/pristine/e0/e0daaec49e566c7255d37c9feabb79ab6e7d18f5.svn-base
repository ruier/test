/* sysMpc8540EcmError.h -  MPC8540 Error Handling for the ECM */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*

modification history
--------------------
01a,27jan05,efb  New file.
*/

/*
DESCRIPTION

Register bit definintions, masks, and legal field values for the 
e500 coherency module (ECM) are defined in this file.

*/

#ifndef	INCsysMpc8540EcmErrorh
#define	INCsysMpc8540EcmErrorh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* includes */

#include "vxWorks.h"

/* defines */


/* ECM Error Detect Register */

#define ECM_ERROR_DETECT_MULT_ERR_BIT  0 
#define ECM_ERROR_DETECT_MULT_ERR_MASK (0x80000000UL >>  \
                                        ECM_ERROR_DETECT_MULT_ERR_BIT)

#define ECM_ERROR_DETECT_LAE_BIT       31 
#define ECM_ERROR_DETECT_LAE_MASK      (0x80000000UL >>  \
                                        ECM_ERROR_DETECT_LAE_BIT) 

/* ECM Error Enable Register */

#define ECM_ERROR_ENB_LAE_BIT          31 
#define ECM_ERROR_ENB_LAE_MASK         (0x80000000UL >> ECM_ERROR_ENB_LAE_BIT) 

/* ECM Error Attributes Capture Register */

#define ECM_ERROR_ATTR_BYTE_CNT_BIT    3
#define ECM_ERROR_ATTR_BYTE_CNT_MASK   (0xF8000000 >>  \
                                        ECM_ERROR_ATTR_BYTE_CNT_BIT)

#define ECM_ERROR_ATTR_SCR_ID_BIT      11
#define ECM_ERROR_ATTR_SCR_ID_MASK     (0xF8000000 >>  \
                                        ECM_ERROR_ATTR_SCR_ID_BIT)

#define ECM_ERROR_ATTR_TRANS_TYPE_BIT  17
#define ECM_ERROR_ATTR_TRANS_TYPE_MASK (0xF0000000 >> \
                                        ECM_ERROR_ATTR_TRANS_TYPE_BIT)

#define ECM_ERROR_ATTR_DATA_VAL_BIT    13
#define ECM_ERROR_ATTR_DATA_VAL_MASK   (0xF0000000 >> \
                                        ECM_ERROR_ATTR_DATA_VAL_BIT)

#define ECM_ERROR_BYTE_CNT_32               0x00
#define ECM_ERROR_BYTE_CNT_1                0x01
#define ECM_ERROR_BYTE_CNT_2                0x02
#define ECM_ERROR_BYTE_CNT_3                0x03
#define ECM_ERROR_BYTE_CNT_4                0x04
#define ECM_ERROR_BYTE_CNT_8                0x08
#define ECM_ERROR_BYTE_CNT_16               0x10

#define ECM_ERROR_SRC_PCI_INTRFC            0x00
#define ECM_ERROR_SRC_BOOT_SEQCR            0x0A
#define ECM_ERROR_SRC_RAPID_IO              0x0C
#define ECM_ERROR_SRC_PROC_INSTR            0x10
#define ECM_ERROR_SRC_PROC_DATA             0x11
#define ECM_ERROR_SRC_DMA                   0x15
#define ECM_ERROR_SRC_SAP                   0x17
#define ECM_ERROR_SRC_TSEC1                 0x18
#define ECM_ERROR_SRC_TSEC2                 0x19
#define ECM_ERROR_SRC_FEC                   0x1A
#define ECM_ERROR_SRC_RAPIDIO_MSG           0x1C
#define ECM_ERROR_SRC_RAPIDIO_DBELL         0x1D
#define ECM_ERROR_SRC_RAPIDIO_WRITE         0x1E

#define ECM_ERROR_TTYPE_WRITE               0x00
#define ECM_ERROR_TTYPE_WRITE_ALLOCATE      0x02
#define ECM_ERROR_TTYPE_WRITE_ALLOCATE_LOCK 0x03
#define ECM_ERROR_TTYPE_ADDRS_ONLY          0x04
#define ECM_ERROR_TTYPE_READ                0x08
#define ECM_ERROR_TTYPE_READ_UNLOCK         0x09
#define ECM_ERROR_TTYPE_READ_CLEAR_ATOMIC   0x0C
#define ECM_ERROR_TTYPE_READ_SET_ATOMIC     0x0D
#define ECM_ERROR_TTYPE_READ_DECR_ATOMIC    0x0E
#define ECM_ERROR_TTYPE_READ_INCR_ATOMIC    0x0F

STATUS sysMpc8540EcmErrorIntEnable (void);

#ifdef __cplusplus

    }
#endif /* __cplusplus */

#endif /* sysMpc8540EcmErrorh */
