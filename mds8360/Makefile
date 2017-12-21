# Makefile - makefile for Freescale MDS8360 BSP
#
# Copyright (c) 2005-2008 Wind River Systems, Inc.
# 
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01i,29jul08,z_l  Support MDS8360EA.
# 01h,12jul07,vik  removed cmdLine.c build dependency
# 01g,09jul07,dtr  Move sysAUtils.s from BOOT_EXTRA/MACH_EXTRA to LIB_EXTRA.
# 01f,07jun07,dtr  Link sysAUtils.s into boot image.
# 01e,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01e,01dec06,d_c  Add dependency on sysLib.c to create cmdLine.c
# 01d,21jun06,dtr  Change cpu tupe.
# 01c,03aug05,dtr  Remove romExtras.a and put cfiscs.c as include in sysTffs.c
# 01b,25mar05,dtr  Use CPU_VARIANT for 83xx.
# 01a,19jan05,dtr  created from ads827x Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Motorola Voyager Application Development Support Board with a PowerPC 8272
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
CPU_VARIANT      = _ppc603_83xx
TOOL             = diab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR  = mds8360
VENDOR      = Freescale
BOARD       = mds8360

USR_ENTRY   = usrInit

LIB_EXTRA   = romExtras.a

RELEASE_CMD += bootrom.bin

## The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h
## and Makefile. All definitions for these constants must be identical.

## ROM address definitions

ROM_TEXT_ADRS    = fe000100 # ROM entry address
ROM_WARM_ADRS    = fe000108 # ROM warm entry address
ROM_LINK_ADRS    = fe000000

ROM_SIZE         = 00100000 # number of bytes of ROM space
RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00e00000 # RAM text/data address

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g -O
EXTRA_DEFINE  += -DVXWORKS -DMPC83XX

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

EXTRA_OBJS = sysAUtils.o

romExtras.a: $(EXTRA_OBJS)
	$(AR) crus $@ $(EXTRA_OBJS)

