# Makefile - Makefile for Freescale ADS5121e BSP
#
# Copyright (c) 2007-2009 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01c,22apr09,x_f  modified RAM_LOW_ADRS from 0x100000 to 0x200000
# 01b,14aug08,x_f  replace macro RELEASE with RELEASE_CMD
# 01a,06sep07,b_m  created.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale ADS5121e reference board.
#

CPU             = PPC32
TOOL            = diab
CPU_VARIANT     = _ppc603_83xx

TGT_DIR         = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_ads5121e
VENDOR          = Freescale
BOARD           = ADS5121e

RELEASE_CMD    += bootrom.bin

EXTRA_DEFINE    =

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in Makefile. All definitions for these constants must be identical.

# boot from vxWorks bootrom
ROM_TEXT_ADRS   = fff00100 # ROM entry address
ROM_WARM_ADRS   = fff00108 # ROM warm entry address
ROM_LINK_ADRS   = fff00000

# boot from U-Boot
#ROM_TEXT_ADRS   = ffe00100 # ROM entry address
#ROM_WARM_ADRS   = ffe00108 # ROM warm entry address
#ROM_LINK_ADRS   = ffe00000

ROM_SIZE        = 00100000 # number of bytes of ROM space
RAM_LOW_ADRS    = 00200000 # RAM text/data address
RAM_HIGH_ADRS   = 01D00000 # RAM text/data address

MACH_EXTRA      =

BOOT_EXTRA      =
LIB_EXTRA       =

VMA_START       = 0x0
USR_ENTRY       = usrInit

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
