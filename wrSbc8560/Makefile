# Makefile - makefile for wrSbc8560 BSP
#
# Copyright 2002-2007 Wind River Systems, Inc. */
# 
# modification history
# --------------------
# 01n,27jul07,vik  Fix for WIND00099581, removed cmdLine.c dependency
# 01m,26apr07,rlg  fix for defect 93411
# 01l,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01l,01dec06,d_c  Add dependency on sysLib.c to create cmdLine.c
# 01k,30aug06,wap  Switch to VxBus FCC driver
# 01k,30nov06,rec  vxWorks 6.5 changes
# 01j,29aug06,slk  add IP_PORT_VXWORKS check
# 01j,28Jul06,tor  change from TSEC END to TSEC HEND
# 01i,22mar05,dtr  Remove TSEC. SPR 107045.
# 01h,12nov04,mil  Added cfiscs.o to MACH_EXTRA.
# 01g,28oct04,dtr  Compile Fcc2 wrapper.
# 01f,30sep04,dtr  Increase alignment on RAM_LOW_ADRS and data segment.
# 01e,10aug04,kab  SPR#100219: support for sfdiab, sfgnu.
# 01d,13apr04,mil  Removed workaround for TSEC GNU build.
# 01c,12sep03,mil  Updated MACH_EXTRA for FCC in 8560 mode.
# 01b,20aug03,dtr  Adding TSEC driver to MACH_EXTRA.
# 01a,03oct02,dtr  Created.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Wind River SBC 85xx
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC85XX

## Note: TOOL value change: sfgnu or sfdiab is preferred for soft-float CPU.
## PPC85XX use of TOOL=diab or gnu is preserved for backwards compatability,
## but deprecated:  PPC85XX is a soft-float CPU, and that should be
## correctly reflected in the TOOL name, but was not in prior releases.
## PPC85XX RTPs and user-mode libraries must be built with sfdiab or sfgnu.
## TOOL            = sfdiab

TOOL             = sfdiab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= wrSbc8560
VENDOR		= Wind River
BOARD		= SBC85XX

RELEASE		+= bootrom.bin bootrom_uncmp.hex bootrom_uncmp.bin

## Add -DINCLUDE_CPM for MPC8560 which uses on board DUART (vs on chip)

EXTRA_DEFINE    = -DINCLUDE_CPM

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000

RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 00c00000 # RAM text/data address

DATA_SEG_ALIGN   = 00080000 #Align data section for TLB locking

## Add object files to MACH_EXTRA if defined in config.h the following:
##   INCLUDE_TFFS        : cfiscs.o

MACH_EXTRA       = cfiscs.o
BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
USR_ENTRY        = usrInit

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp



