# Makefile - make rules for vw/config/walnut IBM Walnut 405GP board
#
# Copyright 1984-2007 Wind River Systems, Inc.
#
# modification history
# --------------------
# 01o,31may07,pch  Don't add bootrom_uncmp.hex to RELEASE
# 01n,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01m,09nov06,rec  vxWorks 6.5 update
# 01l,04oct04,dtr  text/data alignment for SPR 99491.
# 01k,10aug04,kab  SPR#100219: support for sfdiab, sfgnu.
# 01j,18jul02,pch  SPR 79751:  fix compressed images
# 01i,30apr02,pch  SPR 75652: change ROM_SIZE from 0007ff00 to 00080000 to
#		   correspond with new build strategy in romInit.s
# 01h,30nov01,kab  Update for T2.2
# 01h,06dec01,tpw  Set ROM_SIZE to reach exactly the end of the ROM area.
# 01g,14nov01,dat  RELEASE update
# 01f,30oct01,tpw  Standardize kernel links and bootrom conversions.
# 01e,25sep01,dat  Update for T2.2
# 01d,21jun01,pch  Set HEX_FLAGS to empty for new GNU objcopyppc
# 01c,14nov00,s_m  commented out debug flags
# 01b,25oct00,s_m  renamed CPU type
# 01a,08sep99,mcg  created from evb403 version 01l.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# IBM Walnut 405GP board
#
# INCLUDES
#     makeTarget
#*/

CPU		= PPC405

## Note: TOOL value change: sfgnu or sfdiab is preferred for soft-float CPU.
## PPC4XX use of TOOL=diab, or gnu, is preserved for backwards compatability,
## but deprecated:  PPC4XX is a soft-float CPU, and that should be
## correctly reflected in the TOOL name, but was not in prior releases.
## PPC4xx RTPs and user-mode libraries must be built with sfdiab or sfgnu.
TOOL		= sfdiab

## Uncomment for Tornado 2.x
## TOOL		= diab

TGT_DIR=$(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR	= walnut
VENDOR		= IBM
BOARD		= WALNUT

USR_ENTRY	= usrInit

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h, MakeSkel, Makefile, and Makefile.*
# Normally, all definitions for these constants must be identical;
# but ROM_SIZE is different here for reasons related to the note
# below regarding ROM_LINK_ADRS.
#

ROM_TEXT_ADRS	= fff80100 # ROM entry address
ROM_SIZE	= 00080000 # number of bytes of ROM space

RAM_LOW_ADRS	= 00100000
RAM_HIGH_ADRS	= 00D00000

DATA_SEG_ALIGN	= 00080000 #Align data section for TLB locking

MACH_EXTRA	=

# ROM_LINK_ADRS defaults to ROM_TEXT_ADRS, but needs to be different
# here due to complications related to the .space 0x100 at the start
# of romInit.s.  Don't ask :)

ROM_LINK_ADRS	= fff80000 # ROM base address

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)
