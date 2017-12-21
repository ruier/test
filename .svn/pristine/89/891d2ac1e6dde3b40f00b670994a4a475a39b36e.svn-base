# Makefile - Makefile for Freescale MPC8536DS BSP
#
# Copyright (c) 2008, 2011 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01b,02apr11,y_c  Support VxWorks native bootrom. (WIND00240155)
# 01a,20oct08,b_m  created.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale MPC8536DS reference board.
#

CPU             = PPC32
TOOL            = e500v2diab
CPU_VARIANT     = _ppc85XX_e500v2

TGT_DIR         = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_mpc8536_ds
VENDOR          = Freescale
BOARD           = MPC8536DS

RELEASE_CMD    += bootrom.bin

EXTRA_DEFINE    =

## The constants ROM_TEXT_ADRS & ROM_SIZE are defined in config.h and Makefile.
## All definitions for these constants must be identical.

ROM_TEXT_ADRS   = fff00100 # ROM entry address
ROM_WARM_ADRS   = fff00108 # ROM warm entry address
ROM_LINK_ADRS   = fff00000

RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address
ROM_SIZE        = 00100000 # number of bytes of ROM space

USR_ENTRY       = usrInit

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp