/* sysMpc8540LbcError.h -  MPC8540 Error Handling for the LBC */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*

modification history
--------------------
01a,27jan05,efb  New file.
*/

/*
DESCRIPTION

Register bit definitions, masks, and legal field values for the 
Local Bus Controller (LBC) are defined in this file.

*/

#ifndef	INCsysMpc8540LbcErrorh
#define	INCsysMpc8540LbcErrorh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* includes */

#include "vxWorks.h"

/* defines */


/* LBC Transfer Error Status Register */

#define LBC_TRANS_ERROR_STATUS_BMTO_BIT   0 
#define LBC_TRANS_ERROR_STATUS_BMTO_MASK  (0x80000000UL >>  \
                                           LBC_TRANS_ERROR_STATUS_BMTO_BIT)

#define LBC_TRANS_ERROR_STATUS_PAR_BIT    2 
#define LBC_TRANS_ERROR_STATUS_PAR_MASK   (0x80000000UL >>  \
                                           LBC_TRANS_ERROR_STATUS_PAR_BIT)

#define LBC_TRANS_ERROR_STATUS_WP_BIT     5 
#define LBC_TRANS_ERROR_STATUS_WP_MASK    (0x80000000UL >>  \
                                           LBC_TRANS_ERROR_STATUS_WP_BIT)

#define LBC_TRANS_ERROR_STATUS_ATMW_BIT   8 
#define LBC_TRANS_ERROR_STATUS_ATMW_MASK  (0x80000000UL >>  \
                                           LBC_TRANS_ERROR_STATUS_ATMW_BIT)

#define LBC_TRANS_ERROR_STATUS_ATMR_BIT   9 
#define LBC_TRANS_ERROR_STATUS_ATMR_MASK  (0x80000000UL >>  \
                                           LBC_TRANS_ERROR_STATUS_ATMR_BIT)

#define LBC_TRANS_ERROR_STATUS_CS_BIT     12 
#define LBC_TRANS_ERROR_STATUS_CS_MASK    (0x80000000UL >>  \
                                           LBC_TRANS_ERROR_STATUS_CS_BIT)

/* LBC Transfer Error Check Disable Register */

#define LBC_TRANS_ERROR_DISAB_BMTO_BIT   0 
#define LBC_TRANS_ERROR_DISAB_BMTO_MASK  (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_DISAB_BMTO_BIT)

#define LBC_TRANS_ERROR_DISAB_PAR_BIT    2 
#define LBC_TRANS_ERROR_DISAB_PAR_MASK   (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_DISAB_PAR_BIT)

#define LBC_TRANS_ERROR_DISAB_WP_BIT     5 
#define LBC_TRANS_ERROR_DISAB_WP_MASK    (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_DISAB_WP_BIT)

#define LBC_TRANS_ERROR_DISAB_ATMW_BIT   8 
#define LBC_TRANS_ERROR_DISAB_ATMW_MASK  (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_DISAB_ATMW_BIT)

#define LBC_TRANS_ERROR_DISAB_ATMR_BIT   9 
#define LBC_TRANS_ERROR_DISAB_ATMR_MASK  (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_DISAB_ATMR_BIT)

#define LBC_TRANS_ERROR_DISAB_CS_BIT     12 
#define LBC_TRANS_ERROR_DISAB_CS_MASK    (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_DISAB_CS_BIT)

/* LBC Transfer Error Interrupt Enable Register */

#define LBC_TRANS_ERROR_INT_ENB_BMTO_BIT   0 
#define LBC_TRANS_ERROR_INT_ENB_BMTO_MASK  (0x80000000UL >>  \
                                            LBC_TRANS_ERROR_INT_ENB_BMTO_BIT)

#define LBC_TRANS_ERROR_INT_ENB_PAR_BIT    2 
#define LBC_TRANS_ERROR_INT_ENB_PAR_MASK   (0x80000000UL >>  \
                                            LBC_TRANS_ERROR_INT_ENB_PAR_BIT)

#define LBC_TRANS_ERROR_INT_ENB_WP_BIT     5 
#define LBC_TRANS_ERROR_INT_ENB_WP_MASK    (0x80000000UL >>  \
                                            LBC_TRANS_ERROR_INT_ENB_WP_BIT)

#define LBC_TRANS_ERROR_INT_ENB_ATMW_BIT   8 
#define LBC_TRANS_ERROR_INT_ENB_ATMW_MASK  (0x80000000UL >>  \
                                            LBC_TRANS_ERROR_INT_ENB_ATMW_BIT)

#define LBC_TRANS_ERROR_INT_ENB_ATMR_BIT   9 
#define LBC_TRANS_ERROR_INT_ENB_ATMR_MASK  (0x80000000UL >>  \
                                            LBC_TRANS_ERROR_INT_ENB_ATMR_BIT)

#define LBC_TRANS_ERROR_INT_ENB_CS_BIT     12 
#define LBC_TRANS_ERROR_INT_ENB_CS_MASK    (0x80000000UL >>  \
                                            LBC_TRANS_ERROR_INT_ENB_CS_BIT)

/* LBC Transfer Error Attributes Register */

#define LBC_TRANS_ERROR_ATTR_RWB_BIT     3
#define LBC_TRANS_ERROR_ATTR_RWB_MASK    (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_ATTR_RWB_BIT)

#define LBC_TRANS_ERROR_ATTR_SRCID_BIT   11
#define LBC_TRANS_ERROR_ATTR_SRCID_MASK  (0xF8000000UL >>  \
                                          LBC_TRANS_ERROR_ATTR_SRCID_BIT)

#define LBC_TRANS_ERROR_ATTR_PB_BIT      16
#define LBC_TRANS_ERROR_ATTR_PB_MASK     (0xF0000000UL >>  \
                                          LBC_TRANS_ERROR_ATTR_PB_BIT)

#define LBC_TRANS_ERROR_ATTR_BNK_BIT     20
#define LBC_TRANS_ERROR_ATTR_BNK_MASK    (0xFF000000UL >>  \
                                          LBC_TRANS_ERROR_ATTR_BNK_BIT)

#define LBC_TRANS_ERROR_ATTR_XA_BIT      28
#define LBC_TRANS_ERROR_ATTR_XA_MASK     (0xC0000000UL >>  \
                                          LBC_TRANS_ERROR_ATTR_XA_BIT)

#define LBC_TRANS_ERROR_ATTR_V_BIT       31
#define LBC_TRANS_ERROR_ATTR_V_MASK      (0x80000000UL >>  \
                                          LBC_TRANS_ERROR_ATTR_V_BIT)

STATUS sysMpc8540LbcErrorIntEnable (void);

#ifdef __cplusplus

    }
#endif /* __cplusplus */

#endif /* sysMpc8540LbcErrorh */
