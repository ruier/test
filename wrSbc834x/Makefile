# Makefile - makefile for bsp/config/wrSbc834x
#
# Copyright (c) 2005-2008 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01g,03dec08,syt  modified ROM_LINK_ADRS to 0xfff00100. (WIND00128752)
# 01f,10oct07,dtr  Fix ROM_LINK_ADRS - defect WIND00098418.
# 01e,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01d,08dec06,rec  vxWorks 6.5 update
# 01c,29aug06,slk  add IP_PORT_VXWORKS check
# 01b,28jul05,j_b  remove romExtras.a, add sysAUtils.s to MACH_EXTRA
# 01a,24jun05,j_b  adapted from ads834x (rev 01b)
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Wind River SBC8349E/47E board with a PowerPC 8349E/47E CPU.
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC603
CPU_VARIANT      = _83xx
TOOL             = diab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= wrSbc834x
VENDOR		= Wind River
BOARD		= wrSbc834x

USR_ENTRY	= usrInit

RELEASE		+= bootrom_uncmp.hex

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address
ROM_LINK_ADRS	 = fff00100

RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00c00000 # RAM text/data address

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g -O
EXTRA_DEFINE   = -DVXWORKS -DMPC83XX

MACH_EXTRA      = sysAUtils.o

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
