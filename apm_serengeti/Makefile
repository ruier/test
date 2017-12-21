# Makefile - makefile for APM86290 Serengeti board
#
#
# Copyright (c) 2011 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,16jun11,syt  initial creation
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# APM86290 Serengeti reference board
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
CPU_VARIANT      = _ppc465
TOOL             = diab

TGT_DIR=$(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR       = apm_serengeti
VENDOR           = APM
BOARD            = Serengeti

RELEASE          += bootrom.bin

LIB_EXTRA        = romExtras.a

# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h, MakeSkel, Makefile, and Makefile.*
# All definitions for these constants must be identical.

ROM_TEXT_ADRS   = FFF00000 # ROM entry address

ROM_SIZE        = 00100000 # number of bytes of ROM space
RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address

DATA_SEG_ALIGN  = 00080000 #Align data section for TLB locking

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

ROM_EXTRA       = romBoardInit.o

romExtras.a: $(ROM_EXTRA)
	$(AR) crus $@ $(ROM_EXTRA)
