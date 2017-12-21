/* 01profile_bootapp.cdf - VxWorks bootApp profile */

/*
 * Copyright (c) 2005-2007, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01m,03apr12,mze  restrict BOOTAPP to non WRHV_GUEST
01l,18may11,mze  these profiles are not valid for SMP based projects
01k,05jan11,mze  PROFILE_BOOTROM_COMPRESSED_BASIC required for MIPSI64R2
01j,24nov10,mze  add MIPSI64R2
01i,22nov10,mze  changing ifdefs
01h,11nov10,mze  set profile up for only LP64.
01g,21apr10,mze  more changes
01f,20apr10,mze  new profile options
01e,22mar07,jmt  Defect: Remove cache components from bootapp profile.
01d,22feb07,jmt  Defect 88750 - Fix problem with network boot device CDF
                 defines
01c,17oct06,jmt  Update Synopsis for use restrictions
01b,11jul06,jmt  Update profile to no longer use default profile.
01a,17aug05,jmt  written
*/

#if (!defined _WRS_CONFIG_SMP) && (!defined _WRS_CONFIG_WRHV_GUEST) && (!defined SIMLINUX) && (!defined  SIMNT) && (!defined SIMSPARCSOLARIS)
Profile PROFILE_BOOTAPP {
    NAME        bootApp configuration
    SYNOPSIS    This profile enumerates all the components comprising the \
BOOTAPP profile.  This profile adds RAM-based bootloader functionality \
to a standard VxWorks image. The build spec is set to default. \
The full networking libraries are used for the image.
    COMPONENTS        \
                INCLUDE_ANSI_ABS                \
                INCLUDE_ANSI_BSEARCH            \
                INCLUDE_ANSI_CTYPE              \
                INCLUDE_ANSI_MEMCHR             \
                INCLUDE_ANSI_MEMCMP             \
                INCLUDE_ANSI_MEMCPY             \
                INCLUDE_ANSI_MEMMOVE            \
                INCLUDE_ANSI_MEMSET             \
                INCLUDE_ANSI_STDIO              \
                INCLUDE_ANSI_STDLIB             \
                INCLUDE_ANSI_STRDUP             \
                INCLUDE_ANSI_STRERROR           \
                INCLUDE_ANSI_STRING             \
                INCLUDE_ANSI_TIME               \
                INCLUDE_APPL_LOG_UTIL           \
                INCLUDE_BASE_KERNEL             \
                INCLUDE_BOOT_APP                \
                INCLUDE_BOOT_ELF                \
                INCLUDE_BOOT_END                \
                INCLUDE_BOOT_EXC_HANDLER        \
                INCLUDE_BOOT_FTP_LOADER         \
                INCLUDE_BOOT_INIT               \
                INCLUDE_BOOT_LINE_INIT          \
                INCLUDE_BOOT_MEM_CMDS           \
                INCLUDE_BOOT_NETWORK            \
                INCLUDE_BOOT_SHELL              \
                INCLUDE_BOOT_RSH_LOADER         \
                INCLUDE_BOOT_TFTP_LOADER        \
                INCLUDE_COPROCESSOR             \
                INCLUDE_DLL                     \
                INCLUDE_EXC_TASK                \
                INCLUDE_FLOATING_POINT          \
                INCLUDE_FORMATTED_IO            \
                INCLUDE_IO_SYSTEM               \
                INCLUDE_IPCOM                   \
                INCLUDE_IPNET                   \
                INCLUDE_IPNET_IFCONFIG_1        \
                INCLUDE_JOB_QUEUE               \
                INCLUDE_JOB_TASK                \
                INCLUDE_LOGGING                 \
                INCLUDE_KERNEL                  \
                INCLUDE_MEM_MGR_BASIC           \
                INCLUDE_MEM_MGR_FULL            \
                INCLUDE_MSG_Q                   \
                INCLUDE_MSG_Q_CREATE_DELETE     \
                INCLUDE_MSG_Q_INFO              \
                INCLUDE_NETWORK                 \
                INCLUDE_NET_DRV                 \
                INCLUDE_NET_HOST_SETUP          \
                INCLUDE_OBJ_LIB                 \
                INCLUDE_REBOOT_HOOKS            \
                INCLUDE_SELECT                  \
                INCLUDE_SEM_BINARY              \
                INCLUDE_SEM_BINARY_CREATE       \
                INCLUDE_SEM_COUNTING            \
                INCLUDE_SEM_COUNTING_CREATE     \
                INCLUDE_SEM_DELETE              \
                INCLUDE_SEM_INFO                \
                INCLUDE_SEM_LIB                 \
                INCLUDE_SEM_MUTEX               \
                INCLUDE_SEM_MUTEX_CREATE        \
                INCLUDE_SIGNALS                 \
                INCLUDE_SIO                     \
                INCLUDE_SLL                     \
                INCLUDE_SYSCLK_INIT             \
                INCLUDE_TASK_CREATE_DELETE      \
                INCLUDE_TASK_HOOKS              \
                INCLUDE_TASK_INFO               \
                INCLUDE_TASK_RESTART            \
                INCLUDE_TCPV4                   \
                INCLUDE_TTY_DEV                 \
                INCLUDE_UDPV4                   \
                INCLUDE_USER_APPL               \
                INCLUDE_WATCHDOGS               \
                INCLUDE_WATCHDOGS_CREATE_DELETE
    PROFILES BSP_PARAMETER_VALUES
}
#if (defined _WRS_CONFIG_LP64) || (defined PENTIUM) || (defined PENTIUM4) || (defined ATOM) || (defined CORE) || (defined NEHALEM) || (defined MIPSI64R2)
Profile PROFILE_BOOTAPP_BASIC {
    NAME        bootApp-basic configuration
    SYNOPSIS    This profile enumerates all the components comprising the \
BOOTAPP profile.  This profile adds RAM-based bootloader functionality \
to a standard VxWorks image. The build spec is set to default. \
The -basic networking libraries are used for a smaller footprint.
    COMPONENTS        \
                INCLUDE_ANSI_ABS                \
                INCLUDE_ANSI_BSEARCH            \
                INCLUDE_ANSI_CTYPE              \
                INCLUDE_ANSI_MEMCHR             \
                INCLUDE_ANSI_MEMCMP             \
                INCLUDE_ANSI_MEMCPY             \
                INCLUDE_ANSI_MEMMOVE            \
                INCLUDE_ANSI_MEMSET             \
                INCLUDE_ANSI_STDIO              \
                INCLUDE_ANSI_STDLIB             \
                INCLUDE_ANSI_STRDUP             \
                INCLUDE_ANSI_STRERROR           \
                INCLUDE_ANSI_STRING             \
                INCLUDE_ANSI_TIME               \
                INCLUDE_APPL_LOG_UTIL           \
                INCLUDE_BASE_KERNEL             \
                INCLUDE_BOOT_APP                \
                INCLUDE_BOOT_ELF                \
                INCLUDE_BOOT_END                \
                INCLUDE_BOOT_EXC_HANDLER        \
                INCLUDE_BOOT_FTP_LOADER         \
                INCLUDE_BOOT_INIT               \
                INCLUDE_BOOT_LINE_INIT          \
                INCLUDE_BOOT_MEM_CMDS           \
                INCLUDE_BOOT_NETWORK            \
                INCLUDE_BOOT_SHELL              \
                INCLUDE_BOOT_RSH_LOADER         \
                INCLUDE_BOOT_TFTP_LOADER        \
                INCLUDE_COPROCESSOR             \
                INCLUDE_DLL                     \
                INCLUDE_EXC_TASK                \
                INCLUDE_FLOATING_POINT          \
                INCLUDE_FORMATTED_IO            \
                INCLUDE_IO_SYSTEM               \
                INCLUDE_IPCOM                   \
                INCLUDE_IPNET                   \
                INCLUDE_IPNET_IFCONFIG_1        \
                INCLUDE_JOB_QUEUE               \
                INCLUDE_JOB_TASK                \
                INCLUDE_LOGGING                 \
                INCLUDE_KERNEL                  \
                INCLUDE_MEM_MGR_BASIC           \
                INCLUDE_MEM_MGR_FULL            \
                INCLUDE_MSG_Q                   \
                INCLUDE_MSG_Q_CREATE_DELETE     \
                INCLUDE_MSG_Q_INFO              \
                INCLUDE_NETWORK                 \
                INCLUDE_NET_DRV                 \
                INCLUDE_NET_HOST_SETUP          \
                INCLUDE_OBJ_LIB                 \
                INCLUDE_REBOOT_HOOKS            \
                INCLUDE_SELECT                  \
                INCLUDE_SEM_BINARY              \
                INCLUDE_SEM_BINARY_CREATE       \
                INCLUDE_SEM_COUNTING            \
                INCLUDE_SEM_COUNTING_CREATE     \
                INCLUDE_SEM_DELETE              \
                INCLUDE_SEM_INFO                \
                INCLUDE_SEM_LIB                 \
                INCLUDE_SEM_MUTEX               \
                INCLUDE_SEM_MUTEX_CREATE        \
                INCLUDE_SIGNALS                 \
                INCLUDE_SIO                     \
                INCLUDE_SLL                     \
                INCLUDE_SYSCLK_INIT             \
                INCLUDE_TASK_CREATE_DELETE      \
                INCLUDE_TASK_HOOKS              \
                INCLUDE_TASK_INFO               \
                INCLUDE_TASK_RESTART            \
                INCLUDE_TCPV4                   \
                INCLUDE_TTY_DEV                 \
                INCLUDE_UDPV4                   \
                INCLUDE_USER_APPL               \
                INCLUDE_WATCHDOGS               \
                INCLUDE_WATCHDOGS_CREATE_DELETE 
    IPNET_OPT -basic
    PROFILES BSP_PARAMETER_VALUES
}
#endif

#ifdef _WRS_CONFIG_LP64
Profile PROFILE_BOOTROM {
    NAME        bootRom configuration
    SYNOPSIS    This profile enumerates all the components comprising the \
BOOTROM profile.  This profile adds ROM-based bootloader functionality \
to a standard VxWorks image. The build spec is set to default_rom. \
The full networking libraries are used for the image.
    COMPONENTS        \
                INCLUDE_ANSI_ABS                \
                INCLUDE_ANSI_BSEARCH            \
                INCLUDE_ANSI_CTYPE              \
                INCLUDE_ANSI_MEMCHR             \
                INCLUDE_ANSI_MEMCMP             \
                INCLUDE_ANSI_MEMCPY             \
                INCLUDE_ANSI_MEMMOVE            \
                INCLUDE_ANSI_MEMSET             \
                INCLUDE_ANSI_STDIO              \
                INCLUDE_ANSI_STDLIB             \
                INCLUDE_ANSI_STRDUP             \
                INCLUDE_ANSI_STRERROR           \
                INCLUDE_ANSI_STRING             \
                INCLUDE_ANSI_TIME               \
                INCLUDE_APPL_LOG_UTIL           \
                INCLUDE_BASE_KERNEL             \
                INCLUDE_BOOT_APP                \
                INCLUDE_BOOT_ELF                \
                INCLUDE_BOOT_END                \
                INCLUDE_BOOT_EXC_HANDLER        \
                INCLUDE_BOOT_FTP_LOADER         \
                INCLUDE_BOOT_INIT               \
                INCLUDE_BOOT_LINE_INIT          \
                INCLUDE_BOOT_MEM_CMDS           \
                INCLUDE_BOOT_NETWORK            \
                INCLUDE_BOOT_SHELL              \
                INCLUDE_BOOT_RSH_LOADER         \
                INCLUDE_BOOT_TFTP_LOADER        \
                INCLUDE_COPROCESSOR             \
                INCLUDE_DLL                     \
                INCLUDE_EXC_TASK                \
                INCLUDE_FLOATING_POINT          \
                INCLUDE_FORMATTED_IO            \
                INCLUDE_IO_SYSTEM               \
                INCLUDE_IPCOM                   \
                INCLUDE_IPNET                   \
                INCLUDE_IPNET_IFCONFIG_1        \
                INCLUDE_JOB_QUEUE               \
                INCLUDE_JOB_TASK                \
                INCLUDE_LOGGING                 \
                INCLUDE_KERNEL                  \
                INCLUDE_MEM_MGR_BASIC           \
                INCLUDE_MEM_MGR_FULL            \
                INCLUDE_MSG_Q                   \
                INCLUDE_MSG_Q_CREATE_DELETE     \
                INCLUDE_MSG_Q_INFO              \
                INCLUDE_NETWORK                 \
                INCLUDE_NET_DRV                 \
                INCLUDE_NET_HOST_SETUP          \
                INCLUDE_OBJ_LIB                 \
                INCLUDE_REBOOT_HOOKS            \
                INCLUDE_SELECT                  \
                INCLUDE_SEM_BINARY              \
                INCLUDE_SEM_BINARY_CREATE       \
                INCLUDE_SEM_COUNTING            \
                INCLUDE_SEM_COUNTING_CREATE     \
                INCLUDE_SEM_DELETE              \
                INCLUDE_SEM_INFO                \
                INCLUDE_SEM_LIB                 \
                INCLUDE_SEM_MUTEX               \
                INCLUDE_SEM_MUTEX_CREATE        \
                INCLUDE_SIGNALS                 \
                INCLUDE_SIO                     \
                INCLUDE_SLL                     \
                INCLUDE_SYSCLK_INIT             \
                INCLUDE_TASK_CREATE_DELETE      \
                INCLUDE_TASK_HOOKS              \
                INCLUDE_TASK_INFO               \
                INCLUDE_TASK_RESTART            \
                INCLUDE_TCPV4                   \
                INCLUDE_TTY_DEV                 \
                INCLUDE_UDPV4                   \
                INCLUDE_USER_APPL               \
                INCLUDE_WATCHDOGS               \
                INCLUDE_WATCHDOGS_CREATE_DELETE 
    IMAGE_LOCATION ROM
    PROFILES BSP_PARAMETER_VALUES
}

Profile PROFILE_BOOTROM_BASIC {
    NAME        bootRom-basic configuration
    SYNOPSIS    This profile enumerates all the components comprising the \
BOOTROM profile.  This profile adds ROM-based bootloader functionality \
to a standard VxWorks image. The build spec is set to default_rom. \
The -basic networking libraries are used for a smaller footprint.
    COMPONENTS        \
                INCLUDE_ANSI_ABS                \
                INCLUDE_ANSI_BSEARCH            \
                INCLUDE_ANSI_CTYPE              \
                INCLUDE_ANSI_MEMCHR             \
                INCLUDE_ANSI_MEMCMP             \
                INCLUDE_ANSI_MEMCPY             \
                INCLUDE_ANSI_MEMMOVE            \
                INCLUDE_ANSI_MEMSET             \
                INCLUDE_ANSI_STDIO              \
                INCLUDE_ANSI_STDLIB             \
                INCLUDE_ANSI_STRDUP             \
                INCLUDE_ANSI_STRERROR           \
                INCLUDE_ANSI_STRING             \
                INCLUDE_ANSI_TIME               \
                INCLUDE_APPL_LOG_UTIL           \
                INCLUDE_BASE_KERNEL             \
                INCLUDE_BOOT_APP                \
                INCLUDE_BOOT_ELF                \
                INCLUDE_BOOT_END                \
                INCLUDE_BOOT_EXC_HANDLER        \
                INCLUDE_BOOT_FTP_LOADER         \
                INCLUDE_BOOT_INIT               \
                INCLUDE_BOOT_LINE_INIT          \
                INCLUDE_BOOT_MEM_CMDS           \
                INCLUDE_BOOT_NETWORK            \
                INCLUDE_BOOT_SHELL              \
                INCLUDE_BOOT_RSH_LOADER         \
                INCLUDE_BOOT_TFTP_LOADER        \
                INCLUDE_COPROCESSOR             \
                INCLUDE_DLL                     \
                INCLUDE_EXC_TASK                \
                INCLUDE_FLOATING_POINT          \
                INCLUDE_FORMATTED_IO            \
                INCLUDE_IO_SYSTEM               \
                INCLUDE_IPCOM                   \
                INCLUDE_IPNET                   \
                INCLUDE_IPNET_IFCONFIG_1        \
                INCLUDE_JOB_QUEUE               \
                INCLUDE_JOB_TASK                \
                INCLUDE_LOGGING                 \
                INCLUDE_KERNEL                  \
                INCLUDE_MEM_MGR_BASIC           \
                INCLUDE_MEM_MGR_FULL            \
                INCLUDE_MSG_Q                   \
                INCLUDE_MSG_Q_CREATE_DELETE     \
                INCLUDE_MSG_Q_INFO              \
                INCLUDE_NETWORK                 \
                INCLUDE_NET_DRV                 \
                INCLUDE_NET_HOST_SETUP          \
                INCLUDE_OBJ_LIB                 \
                INCLUDE_REBOOT_HOOKS            \
                INCLUDE_SELECT                  \
                INCLUDE_SEM_BINARY              \
                INCLUDE_SEM_BINARY_CREATE       \
                INCLUDE_SEM_COUNTING            \
                INCLUDE_SEM_COUNTING_CREATE     \
                INCLUDE_SEM_DELETE              \
                INCLUDE_SEM_INFO                \
                INCLUDE_SEM_LIB                 \
                INCLUDE_SEM_MUTEX               \
                INCLUDE_SEM_MUTEX_CREATE        \
                INCLUDE_SIGNALS                 \
                INCLUDE_SIO                     \
                INCLUDE_SLL                     \
                INCLUDE_SYSCLK_INIT             \
                INCLUDE_TASK_CREATE_DELETE      \
                INCLUDE_TASK_HOOKS              \
                INCLUDE_TASK_INFO               \
                INCLUDE_TASK_RESTART            \
                INCLUDE_TCPV4                   \
                INCLUDE_TTY_DEV                 \
                INCLUDE_UDPV4                   \
                INCLUDE_USER_APPL               \
                INCLUDE_WATCHDOGS               \
                INCLUDE_WATCHDOGS_CREATE_DELETE 
    IPNET_OPT   -basic
    IMAGE_LOCATION ROM
    PROFILES BSP_PARAMETER_VALUES
}

Profile PROFILE_BOOTROM_COMPRESSED {
    NAME        compressed bootRom configuration
    SYNOPSIS    This profile enumerates all the components comprising the \
BOOTAPP profile.  This profile adds Compressed ROM-based bootloader functionality \
to a standard VxWorks image. The build spec is set to default_romCompress. \
The full networking libraries are used for the image.
    COMPONENTS        \
                INCLUDE_ANSI_ABS                \
                INCLUDE_ANSI_BSEARCH            \
                INCLUDE_ANSI_CTYPE              \
                INCLUDE_ANSI_MEMCHR             \
                INCLUDE_ANSI_MEMCMP             \
                INCLUDE_ANSI_MEMCPY             \
                INCLUDE_ANSI_MEMMOVE            \
                INCLUDE_ANSI_MEMSET             \
                INCLUDE_ANSI_STDIO              \
                INCLUDE_ANSI_STDLIB             \
                INCLUDE_ANSI_STRDUP             \
                INCLUDE_ANSI_STRERROR           \
                INCLUDE_ANSI_STRING             \
                INCLUDE_ANSI_TIME               \
                INCLUDE_APPL_LOG_UTIL           \
                INCLUDE_BASE_KERNEL             \
                INCLUDE_BOOT_APP                \
                INCLUDE_BOOT_ELF                \
                INCLUDE_BOOT_END                \
                INCLUDE_BOOT_EXC_HANDLER        \
                INCLUDE_BOOT_FTP_LOADER         \
                INCLUDE_BOOT_INIT               \
                INCLUDE_BOOT_LINE_INIT          \
                INCLUDE_BOOT_MEM_CMDS           \
                INCLUDE_BOOT_NETWORK            \
                INCLUDE_BOOT_SHELL              \
                INCLUDE_BOOT_RSH_LOADER         \
                INCLUDE_BOOT_TFTP_LOADER        \
                INCLUDE_COPROCESSOR             \
                INCLUDE_DLL                     \
                INCLUDE_EXC_TASK                \
                INCLUDE_FLOATING_POINT          \
                INCLUDE_FORMATTED_IO            \
                INCLUDE_IO_SYSTEM               \
                INCLUDE_IPCOM                   \
                INCLUDE_IPNET                   \
                INCLUDE_IPNET_IFCONFIG_1        \
                INCLUDE_JOB_QUEUE               \
                INCLUDE_JOB_TASK                \
                INCLUDE_LOGGING                 \
                INCLUDE_KERNEL                  \
                INCLUDE_MEM_MGR_BASIC           \
                INCLUDE_MEM_MGR_FULL            \
                INCLUDE_MSG_Q                   \
                INCLUDE_MSG_Q_CREATE_DELETE     \
                INCLUDE_MSG_Q_INFO              \
                INCLUDE_NETWORK                 \
                INCLUDE_NET_DRV                 \
                INCLUDE_NET_HOST_SETUP          \
                INCLUDE_OBJ_LIB                 \
                INCLUDE_REBOOT_HOOKS            \
                INCLUDE_SELECT                  \
                INCLUDE_SEM_BINARY              \
                INCLUDE_SEM_BINARY_CREATE       \
                INCLUDE_SEM_COUNTING            \
                INCLUDE_SEM_COUNTING_CREATE     \
                INCLUDE_SEM_DELETE              \
                INCLUDE_SEM_INFO                \
                INCLUDE_SEM_LIB                 \
                INCLUDE_SEM_MUTEX               \
                INCLUDE_SEM_MUTEX_CREATE        \
                INCLUDE_SIGNALS                 \
                INCLUDE_SIO                     \
                INCLUDE_SLL                     \
                INCLUDE_SYSCLK_INIT             \
                INCLUDE_TASK_CREATE_DELETE      \
                INCLUDE_TASK_HOOKS              \
                INCLUDE_TASK_INFO               \
                INCLUDE_TASK_RESTART            \
                INCLUDE_TCPV4                   \
                INCLUDE_TTY_DEV                 \
                INCLUDE_UDPV4                   \
                INCLUDE_USER_APPL               \
                INCLUDE_WATCHDOGS               \
                INCLUDE_WATCHDOGS_CREATE_DELETE 
    IMAGE_LOCATION ROM_COMPRESSED
    PROFILES BSP_PARAMETER_VALUES
}
#endif

#if (defined _WRS_CONFIG_LP64) || (defined MIPSI64R2) 
Profile PROFILE_BOOTROM_COMPRESSED_BASIC {
    NAME        compressed bootRom-basic configuration
    SYNOPSIS    This profile enumerates all the components comprising the \
BOOTAPP profile.  This profile adds Compressed ROM-based bootloader functionality \
to a standard VxWorks image. The build spec is set to default_romCompress. \
The -basic networking libraries are used for a smaller footprint.
    COMPONENTS        \
                INCLUDE_ANSI_ABS                \
                INCLUDE_ANSI_BSEARCH            \
                INCLUDE_ANSI_CTYPE              \
                INCLUDE_ANSI_MEMCHR             \
                INCLUDE_ANSI_MEMCMP             \
                INCLUDE_ANSI_MEMCPY             \
                INCLUDE_ANSI_MEMMOVE            \
                INCLUDE_ANSI_MEMSET             \
                INCLUDE_ANSI_STDIO              \
                INCLUDE_ANSI_STDLIB             \
                INCLUDE_ANSI_STRDUP             \
                INCLUDE_ANSI_STRERROR           \
                INCLUDE_ANSI_STRING             \
                INCLUDE_ANSI_TIME               \
                INCLUDE_APPL_LOG_UTIL           \
                INCLUDE_BASE_KERNEL             \
                INCLUDE_BOOT_APP                \
                INCLUDE_BOOT_ELF                \
                INCLUDE_BOOT_END                \
                INCLUDE_BOOT_EXC_HANDLER        \
                INCLUDE_BOOT_FTP_LOADER         \
                INCLUDE_BOOT_INIT               \
                INCLUDE_BOOT_LINE_INIT          \
                INCLUDE_BOOT_MEM_CMDS           \
                INCLUDE_BOOT_NETWORK            \
                INCLUDE_BOOT_SHELL              \
                INCLUDE_BOOT_RSH_LOADER         \
                INCLUDE_BOOT_TFTP_LOADER        \
                INCLUDE_COPROCESSOR             \
                INCLUDE_DLL                     \
                INCLUDE_EXC_TASK                \
                INCLUDE_FLOATING_POINT          \
                INCLUDE_FORMATTED_IO            \
                INCLUDE_IO_SYSTEM               \
                INCLUDE_IPCOM                   \
                INCLUDE_IPNET                   \
                INCLUDE_IPNET_IFCONFIG_1        \
                INCLUDE_JOB_QUEUE               \
                INCLUDE_JOB_TASK                \
                INCLUDE_LOGGING                 \
                INCLUDE_KERNEL                  \
                INCLUDE_MEM_MGR_BASIC           \
                INCLUDE_MEM_MGR_FULL            \
                INCLUDE_MSG_Q                   \
                INCLUDE_MSG_Q_CREATE_DELETE     \
                INCLUDE_MSG_Q_INFO              \
                INCLUDE_NETWORK                 \
                INCLUDE_NET_DRV                 \
                INCLUDE_NET_HOST_SETUP          \
                INCLUDE_OBJ_LIB                 \
                INCLUDE_REBOOT_HOOKS            \
                INCLUDE_SELECT                  \
                INCLUDE_SEM_BINARY              \
                INCLUDE_SEM_BINARY_CREATE       \
                INCLUDE_SEM_COUNTING            \
                INCLUDE_SEM_COUNTING_CREATE     \
                INCLUDE_SEM_DELETE              \
                INCLUDE_SEM_INFO                \
                INCLUDE_SEM_LIB                 \
                INCLUDE_SEM_MUTEX               \
                INCLUDE_SEM_MUTEX_CREATE        \
                INCLUDE_SIGNALS                 \
                INCLUDE_SIO                     \
                INCLUDE_SLL                     \
                INCLUDE_SYSCLK_INIT             \
                INCLUDE_TASK_CREATE_DELETE      \
                INCLUDE_TASK_HOOKS              \
                INCLUDE_TASK_INFO               \
                INCLUDE_TASK_RESTART            \
                INCLUDE_TCPV4                   \
                INCLUDE_TTY_DEV                 \
                INCLUDE_UDPV4                   \
                INCLUDE_USER_APPL               \
                INCLUDE_WATCHDOGS               \
                INCLUDE_WATCHDOGS_CREATE_DELETE 
    IPNET_OPT   -basic
    IMAGE_LOCATION ROM_COMPRESSED
    PROFILES BSP_PARAMETER_VALUES
}
#endif
#endif  /* not a Simulator */
