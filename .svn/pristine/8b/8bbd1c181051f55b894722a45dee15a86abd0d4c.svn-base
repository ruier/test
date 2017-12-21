# Makefile - makefile for Freescale MDS8568 BSP
#
# Copyright (c) 2007-2008 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01c,24jul08,b_m  switch toolchain to e500v2diab.
# 01b,16may07,b_m  add BOOT_FROM_UBOOT address definitions.
# 01a,10may07,b_m  derived from version 01b of wrSbc8548/Makefile.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale MDS8568 reference board.
#

CPU             = PPC32
TOOL            = e500v2diab
CPU_VARIANT     = _ppc85XX_e500v2

TGT_DIR         = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_mds8568
VENDOR          = Freescale
BOARD           = MDS8568

RELEASE_CMD    += bootrom.bin

EXTRA_DEFINE    =

## The constants ROM_TEXT_ADRS & ROM_SIZE are defined in config.h and Makefile.
## All definitions for these constants must be identical.

# boot from vxWorks bootrom
ROM_TEXT_ADRS   = fff00100 # ROM entry address
ROM_WARM_ADRS   = fff00108 # ROM warm entry address
ROM_LINK_ADRS   = fff00000

# boot from U-Boot
#ROM_TEXT_ADRS  = ffe00100 # ROM entry address
#ROM_WARM_ADRS  = ffe00108 # ROM warm entry address
#ROM_LINK_ADRS  = ffe00000

RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address
ROM_SIZE        = 00100000 # number of bytes of ROM space

MACH_EXTRA      =

BOOT_EXTRA      =
LIB_EXTRA       =

VMA_START       = 0x0
USR_ENTRY       = usrInit

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
