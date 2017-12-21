# Makefile - makefile for Freescale P1010RDB board
#
# Copyright (c) 2011 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,22oct11,fao  derived from version 01a of fsl_p2020_rdb/Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale P1010RDB reference board
#
# INCLUDES
#     makeTarget
#

CPU           = PPC32
TOOL          = e500v2diab
CPU_VARIANT   = _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR    = fsl_p1010_rdb
VENDOR        = Freescale
BOARD         = P1010RDB

RELEASE_CMD   += bootrom.bin bootrom_uncmp.bin

ROM_TEXT_ADRS = EFE00000 # ROM entry address
ROM_WARM_ADRS = EFE00008 # ROM warm entry address
ROM_LINK_ADRS = EFE00000

ROM_SIZE      = 00200000 # number of bytes of ROM space

RAM_LOW_ADRS  = 00100000 # RAM text/data address
RAM_HIGH_ADRS = 01000000 # RAM text/data address

VMA_START     = 0x0

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
