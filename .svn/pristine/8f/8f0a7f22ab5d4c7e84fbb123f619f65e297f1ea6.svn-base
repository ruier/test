# Makefile - makefile for bsp/config/ads827x
#
# Copyright 2003,2004,2006,2007 Wind River Systems, Inc.
#
# modification history
# --------------------
# 01d,16feb07,rec  vxWorks 6.5 update
# 01c,29mar05,dtr  Add CPU variant.
# 01b,05nov04,dtr  Add VXWORKS define for security engine drivers.
# 01a,19dec03,dtr  created from ads826x Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Motorola Voyager Application Development Support Board with a PowerPC 8272
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC603
TOOL             = diab
CPU_VARIANT      = _g2le

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= ads827x
VENDOR		= Motorola
BOARD		= ads827x

USR_ENTRY   = usrInit

LIB_EXTRA   = romExtras.a

RELEASE		+= bootrom_uncmp.hex

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000

RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00d00000 # RAM text/data address

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g -O
EXTRA_DEFINE   = -DVXWORKS
MACH_EXTRA      = 
EXTRA_MODULES   =


## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

# Additional objects needed by romInit

EXTRA_OBJS       = sysAUtils.o sysUtils.o

romExtras.a: $(EXTRA_OBJS)
	$(AR) crus $@ $(EXTRA_OBJS)





