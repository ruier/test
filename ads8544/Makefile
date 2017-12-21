# Makefile - makefile for Freescale ads8544 (Intrepid) board
#
# Copyright (c) 2007-2008,2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01e,08sep10,kab  Remove rommable images from default release target.
#		   rom images work, but must remove components to fit.
# 01d,25aug08,dtr  Add support for hardware float build.
# 01c,08aug08,x_f  replace macro RELEASE with RELEASE_CMD
# 01b,12jul07,vik  removed cmdLine.c dependency for sysLib.c build
# 01a,07feb07,dtr  Modify from cds8548/Makefile/01a
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# FreeScale MPC8544 ADS reference board
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
TOOL             = e500v2diab
#soft float build is also an option
#TOOL             = sfdiab

#Support for 36 bit physical addresses in the MMU can be added for 
#vxWorks 6.2 and later with the following CPU_VARIANT.
CPU_VARIANT      = _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR   = ads8544
VENDOR       = Freescale
BOARD        = ads8544

RELEASE_CMD 	+= bootrom.bin 
RELEASE_PRJ	= prj_diab_def prj_gnu_def

EXTRA_DEFINE    =

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000

RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 01000000 # RAM text/data address

MACH_EXTRA       = 

BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
USR_ENTRY        = usrInit

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

