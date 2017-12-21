# Makefile - makefile for Wind River wrSbc8548 BSP
#
# Copyright (c) 2006-2007 Wind River Systems, Inc.
#
# The right to copy, distribute, modify, or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
# 
# modification history
# --------------------
# 01d,19nov07,dtr  Change TOOL name to e500v2diab/gnu.
# 01c,08nov07,dtr  Add support for ev2diab for e500v2 hard float.
# 01b,28mar07,b_m  update CPU & CPU_VARIANT.
# 01a,30jan06,kds  Modify from cds8548/Makefile/01a
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Wind River SBC8548 reference board.
#
# INCLUDES
#     makeTarget
#*/

CPU             = PPC32
#TOOL           = sfdiab
TOOL		= e500v2diab
CPU_VARIANT	= _ppc85XX_e500v2

# Support for 36 bit physical addresses in the MMU can be added for
# vxWorks 6.2 and later with the following CPU_VARIANT.

#CPU_VARIANT 	= _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR	= sVPX6-EM512.1
VENDOR		= Wind River
BOARD		= VPX6-SSD-T

RELEASE		+= bootrom_uncmp.hex bootrom_uncmp.bin 

ifeq ($(MAKECMDGOALS),bootrom.bin)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

ifeq ($(MAKECMDGOALS),bootloader.bin)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

ifeq ($(MAKECMDGOALS),bootrom)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

ifeq ($(MAKECMDGOALS),bootloader)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

ifeq ($(MAKECMDGOALS),bootrom_uncmp.bin)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

ifeq ($(MAKECMDGOALS),bootrom_uncmp.hex)
EXTRA_DEFINE += -DCOMPILE_BOOTROM
endif

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000

RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 01000000 # RAM text/data address
#RAM_HIGH_ADRS    = 02000000 # FS_EXTENTED_FDT
#BOOTINIT        = bootInit.c
#USRCONFIG       = usrConfig.c
MACH_EXTRA       = usrApp.o
EXTRA_INCLUDE  += -I$(TGT_DIR)/h/ -I$(TGT_DIR)/src/ -I$(WIND_HOME)/components
BOOT_EXTRA       = 
#LIB_EXTRA        = lib/libVPX6-SSDPPC32e500v2diabvx.a

VMA_START        = 0x0
USR_ENTRY        = usrInit
## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

