/* sysCacheErrorLib.c - P4080 cache error handler install code */

/*
 * Copyright (c) 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */
 
/*
modification history
--------------------
01b,05sep12,h_k  added sysL3CacheErrorMsg3 for T4240.
01a,22apr10,pmr  Created. 
*/
 
/*
DESCRIPTION

INCLUDE FILES:
*/

IMPORT void sysCacheErrorHandler (void);
IMPORT void sysCacheErrorHandlerEnd (void);
STATUS sysCacheErrorHandlerInstall (void);
#ifdef INCLUDE_SHOW_ROUTINES
UINT32 sysL1CacheErrorCount = 0;
#endif /* INCLUDE_SHOW_ROUTINES */

const char * sysL2CacheErrorMsg = "Unrecoverable L2 cache error detected.\n\tL2ERRDET\t= 0x%08x\n\tL2ERRADDR\t= 0x%08x\n\tL2ERRATTR\t= 0x%08x\n\tL2CAPTDATAHI\t= 0x%08x\n\tL2CAPTDATALO\t= 0x%08x\n\tL2CAPTECC\t= 0x%08x";

const char * sysL3CacheErrorMsg1 = "Unrecoverable L3 cache error detected on CPC1.\n\tCPCERRDET\t= 0x%08x\n\tCPCERRADDR\t= 0x%08x\n\tCPCERRATTR\t= 0x%08x\n\tCPCCAPTDATAHI\t= 0x%08x\n\tCPCCAPTDATALO\t= 0x%08x\n\tCPCCAPTECC\t= 0x%08x";

const char * sysL3CacheErrorMsg2 = "Unrecoverable L3 cache error detected on CPC2.\n\tCPCERRDET\t= 0x%08x\n\tCPCERRADDR\t= 0x%08x\n\tCPCERRATTR\t= 0x%08x\n\tCPCCAPTDATAHI\t= 0x%08x\n\tCPCCAPTDATALO\t= 0x%08x\n\tCPCCAPTECC\t= 0x%08x";

#ifdef	PPC_e6500
const char * sysL3CacheErrorMsg3 = "Unrecoverable L3 cache error detected on CPC3.\n\tCPCERRDET\t= 0x%08x\n\tCPCERRADDR\t= 0x%08x\n\tCPCERRATTR\t= 0x%08x\n\tCPCCAPTDATAHI\t= 0x%08x\n\tCPCCAPTDATALO\t= 0x%08x\n\tCPCCAPTECC\t= 0x%08x";
#endif	/* PPC_e6500 */

/*************************************************************************
*
* sysCacheErrorHandlerInstall - install cache error handler
*
* This routine installs the cache error handler.
* Handler not used until IVOR1 set. 
*
* RETURNS: OK or ERROR if handler could not be installed
*
* ERRNO
*
* \NOMANUAL
*/
STATUS sysCacheErrorHandlerInstall (void)
    {
    int key;
    STATUS status;
    GENERIC_LAYERED_EXC_INSTALL_DESC mchkCacheErrorDesc;
    static char installErrMsg[] = "Cache error exception handler install has failed";
    mchkCacheErrorDesc.funcStart = &sysCacheErrorHandler;
    mchkCacheErrorDesc.funcEnd = &sysCacheErrorHandlerEnd;
    mchkCacheErrorDesc.excOffset = _EXC_OFF_L1_PARITY;
    mchkCacheErrorDesc.errExcOffset = _EXC_OFF_MACH;
    mchkCacheErrorDesc.hdlrBase = NULL;
    mchkCacheErrorDesc.hdlrCodeBase = NULL;
    mchkCacheErrorDesc.forceBase = TRUE;
    mchkCacheErrorDesc.excMsg = &installErrMsg[0];
    /* Cautious should only be started from single thread */
    key = intCpuLock();
    status = genericLayeredExcHdlrInstall(&mchkCacheErrorDesc);
    intCpuUnlock(key);

    return status;
    }
