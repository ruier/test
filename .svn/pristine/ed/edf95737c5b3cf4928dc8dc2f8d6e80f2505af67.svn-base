# Makefile - makefile for Freescale ads8572 (Whitefin) board
#
# Copyright (c) 2007, 2008, 2012 Wind River Systems, Inc.
# 
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01i,11jul12,l_z  Merge AMP support
# 01h,24feb12,d_l  change RAM_HIGH_ADRS. (WIND00332559)
# 01g,14apr08,dtr  Make hard float default.
# 01f,17jan08,dtr  Remove e500v2 option .
# 01e,19dec07,dtr  Update for AMP support.
# 01d,14nov07,dtr  change tool to e500v2diab.
# 01c,08nov07,dtr  Use ev2diab tool for hard-float.
# 01b,12jul07,vik  removed cmdLine.c dependency for sysLib.c build
# 01a,07feb07,dtr  Modify from cds8548/Makefile/01a
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale MPC8572 ADS reference board
#
# INCLUDES
#     makeTarget
#

CPU              = PPC32
#optional soft-float kernel 
#TOOL             = sfdiab
TOOL             = e500v2diab

CPU_VARIANT      = _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR	= ads8572
VENDOR		= Freescale
BOARD		= ads8572

RELEASE		+= bootrom_uncmp.hex bootrom_uncmp.bin 

EXTRA_DEFINE    =

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00104 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000
RAM_LOW_ADRS     = 00100000 # RAM text/data address

# change RAM_HIGH_ADRS to 0x01800000, corresonding to 20bsp.cdf BOOTAPP RAM_LOW_ADRS.
# Thus, both bootrom.bin and bootapp/vxWorks_romCompress.bin run at this address.
# BOOTAPP RAM image(PROFILE_BOOTAPP VIP including INCLUDE_BOOT_RAM_IMAGE) runs
# at 0x01000000. They can load BOOTAPP RAM image successfully.

RAM_HIGH_ADRS    = 01800000 # RAM text/data address

MACH_EXTRA       = 

BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

