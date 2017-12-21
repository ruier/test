# Makefile - makefile for  ads8560 BSP
#
# Copyright 1984-2002 Wind River Systems, Inc. */
# 
# modification history
# --------------------
# 01j,01aug07,wap  Remove old FCC driver (WIND00100585)
# 01i,31jan07,wap  Undo temporary INCLUDE_CPM change
# 01h,25jan07,d_c  Change INCLUDE_CPM to INCLUDE_CPM_NOVXBUS so not to
#                  conflict with INCLUDE_CPM component
# 01g,22mar05,dtr  Remove TSEC.
# 01f,28oct04,dtr  Compile FCC2 local wrapper.
# 01e,20sep04,dtr  Add increased alignment requirements to allow PageLock to
#                  work on text section.
# 01d,10aug04,kab  SPR#100219: support for sfdiab, sfgnu.
# 01c,21oct03,dtr  Removing motFccEnd.o.
# 01b,20aug03,dtr  Adding TSEC driver to MACH_EXTRA.
# 01a,03oct02,dtr  Created.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Motorola MPC8500 ADS
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
TOOL            = sfdiab

## Uncomment for Tornado 2.x
## TOOL             = diab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR	= ads8560
VENDOR		= Motorola
BOARD		= MPC85XX ADS

RELEASE		+= bootrom_uncmp.hex

EXTRA_DEFINE    =-DINCLUDE_CPM

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000

RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 01000000 # RAM text/data address

DATA_SEG_ALIGN   = 00080000 #Align data section for TLB locking

MACH_EXTRA       =
BOOT_EXTRA       = 
LIB_EXTRA        = 
DATA_SEG_ALIGN	 = 00080000

VMA_START        = 0x0
USR_ENTRY        = usrInit
## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)


