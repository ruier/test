/* usrvxTestV2Comps.c - VxTestV2 Component Testing */

/*
 * Copyright (c) 2009-2010 Wind River Systems, Inc.
 *
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,01apr10,pch  mips adaptation
01d,02jul09,jc0  to update a routine because the HLD is changed
01c,24jun09,jc0  to add some new test cases for the new components based on
                 VSB options
01b,15jun09,jc0  add some components verifications based on the least small
                 footprint HLD
01a,14may09,jc0  written
*/

/*
DESCRIPTION
*/

/* included components portion */
/***************************************************************************
* includeKernelDefined - test whether INCLUDE_KERNEL is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeKernelDefined(void)
    {
#if defined(INCLUDE_KERNEL)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeMemAllotDefined - test whether INCLUDE_MEM_ALLOT is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeMemAllotDefined(void)
    {
#if defined(INCLUDE_MEM_ALLOT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeVxBusDefined - test whether INCLUDE_VXBUS is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeVxBusDefined(void)
    {
#if defined(INCLUDE_VXBUS)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDrvTimerDavinciDefined - test whether DRV_TIMER_DAVINCI is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDrvTimerDavinciDefined(void)
    {
#if defined(DRV_TIMER_DAVINCI)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDrvIntctlrOmapl137Defined - test whether DRV_INTCTLR_OMAPL137 is 
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDrvIntctlrOmapl137Defined(void)
    {
#if defined(DRV_INTCTLR_OMAPL137)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeFormattedOutBasicDefined - test whether INCLUDE_FORMATTED_OUT_BASIC is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeFormattedOutBasicDefined(void)
    {
#if defined(INCLUDE_FORMATTED_OUT_BASIC)
      return TRUE;
#else
      return FALSE;
#endif
    }

/*
 * interdependent components portion
 *
 * Components tested here may be either present or
 * absent, but only certain combinations are valid.
 */
/***************************************************************************
* includeVxBusSysClkDefined - test whether INCLUDE_VXB_SYS_CLK is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeVxBusSysClkDefined(void)
    {
#if defined(INCLUDE_VXB_SYS_CLK)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeTimerSysDefined - test whether INCLUDE_TIMER_SYS is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeTimerSysDefined(void)
    {
#if defined(INCLUDE_TIMER_SYS)
      return TRUE;
#else
      return FALSE;
#endif
    }

/* excluded components portion */
/***************************************************************************
* includeAnsiStdioDefined - test whether INCLUDE_ANSI_STDIO is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeAnsiStdioDefined(void)
    {
#if defined(INCLUDE_ANSI_STDIO)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeCplusDefined - test whether INCLUDE_CPLUS is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeCplusDefined(void)
    {
#if defined(INCLUDE_CPLUS)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeMemMgrFullDefined - test whether INCLUDE_MEM_MGR_FULL is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeMemMgrFullDefined(void)
    {
#if defined(INCLUDE_MEM_MGR_FULL)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeMsgqDefined - test whether INCLUDE_MSG_Q is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeMsgqDefined(void)
    {
#if defined(INCLUDE_MSG_Q)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeMsgqInfoDefined - test whether INCLUDE_MSG_Q_INFO is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeMsgqInfoDefined(void)
    {
#if defined(INCLUDE_MSG_Q_INFO)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeEdrErrlogDefined - test whether INCLUDE_EDR_ERRLOG is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeEdrErrlogDefined(void)
    {
#if defined(INCLUDE_EDR_ERRLOG)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeEdrPmDefined - test whether INCLUDE_EDR_PM is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeEdrPmDefined(void)
    {
#if defined(INCLUDE_EDR_PM)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeFormattedIoDefined - test whether INCLUDE_FORMATTED_IO is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeFormattedIoDefined(void)
    {
#if defined(INCLUDE_FORMATTED_IO)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNetworkDefined - test whether INCLUDE_NETWORK is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNetworkDefined(void)
    {
#if defined(INCLUDE_NETWORK)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeIoBasicDefined - test whether INCLUDE_IO_BASIC is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeIoBasicDefined(void)
    {
#if defined(INCLUDE_IO_BASIC)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeLoggingDefined - test whether INCLUDE_LOGGING is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeLoggingDefined(void)
    {
#if defined(INCLUDE_LOGGING)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeStdioDefined - test whether INCLUDE_STDIO is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeStdioDefined(void)
    {
#if defined(INCLUDE_STDIO)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeLstlibDefined - test whether INCLUDE_LSTLIB is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeLstlibDefined(void)
    {
#if defined(INCLUDE_LSTLIB)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeTaskCreateHooksDefined - test whether INCLUDE_TASK_CREATE_HOOKS
* is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeTaskCreateHooksDefined(void)
    {
#if defined(INCLUDE_TASK_CREATE_HOOKS)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeHooksDefined - test whether INCLUDE_HOOKS is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeHooksDefined(void)
    {
#if defined(INCLUDE_HOOKS)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeSemReadWriteDefined - test whether INCLUDE_SEM_READ_WRITE is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeSemReadWriteDefined(void)
    {
#if defined(INCLUDE_SEM_READ_WRITE)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeSemReadWriteCreateDefined - test whether 
* INCLUDE_SEM_READ_WRITE_CREATE is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeSemReadWriteCreateDefined(void)
    {
#if defined(INCLUDE_SEM_READ_WRITE_CREATE)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeCacheSupportDefined - test whether INCLUDE_CACHE_SUPPORT is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeCacheSupportDefined(void)
    {
#if defined(INCLUDE_CACHE_SUPPORT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeMmuBasicDefined - test whether INCLUDE_MMU_BASIC is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeMmuBasicDefined(void)
    {
#if defined(INCLUDE_MMU_BASIC)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeRtpDefined - test whether INCLUDE_RTP is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeRtpDefined(void)
    {
#if defined(INCLUDE_RTP)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsMainDefined - test whether INCLUDE_DOSFS_MAIN is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsMainDefined(void)
    {
#if defined(INCLUDE_DOSFS_MAIN)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsFatDefined - test whether INCLUDE_DOSFS_FAT is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsFatDefined(void)
    {
#if defined(INCLUDE_DOSFS_FAT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsDirVfatDefined - test whether INCLUDE_DOSFS_DIR_VFAT is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsDirVfatDefined(void)
    {
#if defined(INCLUDE_DOSFS_DIR_VFAT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsDirFixedDefined - test whether INCLUDE_DOSFS_DIR_FIXED is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsDirFixedDefined(void)
    {
#if defined(INCLUDE_DOSFS_DIR_FIXED)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsFmtDefined - test whether INCLUDE_DOSFS_FMT is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsFmtDefined(void)
    {
#if defined(INCLUDE_DOSFS_FMT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsChkdskDefined - test whether INCLUDE_DOSFS_CHKDSK is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsChkdskDefined(void)
    {
#if defined(INCLUDE_DOSFS_CHKDSK)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeXbdTransDefined - test whether INCLUDE_XBD_TRANS is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeXbdTransDefined(void)
    {
#if defined(INCLUDE_XBD_TRANS)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsCacheDefined - test whether INCLUDE_DOSFS_CACHE is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsCacheDefined(void)
    {
#if defined(INCLUDE_DOSFS_CACHE)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeDosfsShowDefined - test whether INCLUDE_DOSFS_SHOW is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeDosfsShowDefined(void)
    {
#if defined(INCLUDE_DOSFS_SHOW)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeTarDefined - test whether INCLUDE_TAR is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeTarDefined(void)
    {
#if defined(INCLUDE_TAR)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeCoreNfsClientDefined - test whether INCLUDE_CORE_NFS_CLIENT is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeCoreNfsClientDefined(void)
    {
#if defined(INCLUDE_CORE_NFS_CLIENT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeCoreNfsServerDefined - test whether INCLUDE_CORE_NFS_SERVER is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeCoreNfsServerDefined(void)
    {
#if defined(INCLUDE_CORE_NFS_SERVER)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNfsMountAllDefined - test whether INCLUDE_NFS_MOUNT_ALL is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNfsMountAllDefined(void)
    {
#if defined(INCLUDE_NFS_MOUNT_ALL)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNfsClientAllDefined - test whether INCLUDE_NFS_CLIENT_ALL is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNfsClientAllDefined(void)
    {
#if defined(INCLUDE_NFS_CLIENT_ALL)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNfs3ClientDefined - test whether INCLUDE_NFS3_CLIENT is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNfs3ClientDefined(void)
    {
#if defined(INCLUDE_NFS3_CLIENT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNfs2ClientDefined - test whether INCLUDE_NFS2_CLIENT is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNfs2ClientDefined(void)
    {
#if defined(INCLUDE_NFS2_CLIENT)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNfsServerAllDefined - test whether INCLUDE_NFS_SERVER_ALL is
* defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNfsServerAllDefined(void)
    {
#if defined(INCLUDE_NFS_SERVER_ALL)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNfs2ServerDefined - test whether INCLUDE_NFS2_SERVER is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNfs2ServerDefined(void)
    {
#if defined(INCLUDE_NFS2_SERVER)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeNfs3ServerDefined - test whether INCLUDE_NFS3_SERVER is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeNfs3ServerDefined(void)
    {
#if defined(INCLUDE_NFS3_SERVER)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeClassListDefined - test whether INCLUDE_CLASS_LIST is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeClassListDefined(void)
    {
#if defined(INCLUDE_CLASS_LIST)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeTaskSwitchHooksDefined - test whether INCLUDE_TASK_SWITCH_HOOKS
* is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeTaskSwitchHooksDefined(void)
    {
#if defined(INCLUDE_TASK_SWITCH_HOOKS)
      return TRUE;
#else
      return FALSE;
#endif
    }

/***************************************************************************
* includeCpuPwrMgmtDefined - test whether INCLUDE_CPU_PWR_MGMT is defined
* 
* RETURNS: TRUE or FALSE
*
* SEE ALSO: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/
BOOL includeCpuPwrMgmtDefined(void)
    {
#if defined(INCLUDE_CPU_PWR_MGMT)
      return TRUE;
#else
      return FALSE;
#endif
    }
