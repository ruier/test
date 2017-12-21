/* xxx8540B.h - Motorola xxx8540 board dependency header file */

/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01d,31jan05,cak VPD Support.
01c,10dec04,cak Added error messages for VME and PCI spaces. 
01b,18nov04,cak Added error message for DDR SDRAM memory controller.
01a,25aug04,scb Initial writing
*/

/*
This file contains board header file dependencies (#ifdef's) as well as
config.h dependencies for the Motorola MVME6100 board.
*/

#ifndef __INCxxx8540Bh
#define __INCxxx8540Bh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#if (DDR_SDRAM_BSTOPRE_VAL == 0)
#   error DDR SDRAM BSTOPRE should not be set to 0x0 
#endif

/*
 * The following compile time checks deal with PCI spaces.
 * The checks are self-explanatory.
 */

#if (VME_MEM_SIZE < (VME_MEM_LOCAL_END - VME_MEM_LOCAL_START))
#   error VME_MEM_SIZE must be > (VME_MEM_LOCAL_END - VME_MEM_LOCAL_START)
#endif

#if (VME_MEM_SIZE & 0x000fffff)
#   error VME_MEM_SIZE must be multiple of 0x00100000
#endif

#if (VME_MEM_SIZE & (VME_MEM_SIZE - 1))
#   error VME_MEM_SIZE must be a power of 2
#endif

#if (VME_A32_MSTR_SIZE < (VME_CRG_MSTR_SIZE + VME_DRAM_ACCESS_SIZE))
#   error VME_A32_MSTR_SIZE must be >= (VME_CRG_MSTR_SIZE+VME_DRAM_ACCESS_SIZE)
#endif

#if (PCI_MSTR_MEMIO_LOCAL & 0x00000fff)
#   error PCI_MSTR_MEMIO_LOCAL must be multiple of 0x1000
#endif

#if (PCI_MSTR_MEMIO_SIZE & (PCI_MSTR_MEMIO_SIZE - 1))
#   error PCI_MSTR_MEMIO_SIZE must be a power of 2
#endif

#if (PCI_MSTR_MEM_LOCAL & 0x00000fff)
#   error PCI_MSTR_MEM_LOCAL must be multiple of 0x1000
#endif

#if (PCI_MSTR_MEM_SIZE & (PCI_MSTR_MEM_SIZE - 1))
#   error PCI_MSTR_MEM_SIZE must be a power of 2
#endif

#if (PCI_IO_SIZE & (PCI_IO_SIZE - 1))
#   error ISA_MSTR_IO_SIZE plus PCI_MSTR_IO_SIZE must be power of 2
#endif

#if (PCI_IO_LOCAL_END > (PCI_MEM_LOCAL_MAX + 1))
#    error PCI space too large, check PCI...SIZE parameters
#endif

/* Vital Product Data */

#ifdef VPD_ERRORS_NONFATAL
#   define  DEFAULT_BSP_ERROR_BEHAVIOR  CONTINUE_EXECUTION
#else /* VPD_ERRORS_NONFATAL */
#   define  DEFAULT_BSP_ERROR_BEHAVIOR  EXIT_TO_SYSTEM_MONITOR
#endif /* VPD_ERRORS_NONFATAL */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCxxx8540Bh */
