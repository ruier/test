# Makefile - make rules for amcc_taishan BSP (AMCC 440GX evaluation board)
#
# Copyright (c) 2007-2008,2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01e,09sep10,kab  Disable rom builds for default release
#		   rom images work, but must remove components to fit.
# 01d,15oct08,x_s  Added ROM_EXTRA definition 
# 01c,02sep08,x_f  Remove sysDcr.s in EXTRA_OBJS 
# 01b,31jul08,x_s  clean up
# 01a,17apr07,x_s  initial creation, based on the bsp of ocotea
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# AMCC 440GX evaluation board. This Makefile generates a 1024 KB
# bootrom image, which will fit in a single S29GL256N flash part.
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC440
CPU_VARIANT      = _x5

TOOL             = sfdiab

TGT_DIR=$(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR       = amcc_taishan
VENDOR           = AMCC
BOARD            = EVB440GX

USR_ENTRY        = usrInit

LIB_EXTRA        = romExtras.a

MACH_EXTRA       =

RELEASE_CMD      += bootrom.bin
RELEASE_PRJ	 = prj_diab_def prj_gnu_def

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h, MakeSkel, Makefile, and Makefile.*
# All definitions for these constants must be identical.
#

ROM_TEXT_ADRS    = fff00000 # ROM base address
ROM_SIZE         = 00100000 # number of bytes of ROM space

DATA_SEG_ALIGN   = 00080000 #Align data section for TLB locking

RAM_LOW_ADRS     = 00100000
RAM_HIGH_ADRS    = 00e00000

## Uncomment the following line to enable debug info (for gnu only)
# ADDED_CFLAGS   = -gdwarf-2 -O0

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

# Additional objects used by romInit

EXTRA_OBJS       = romSdramInit.o \
                   sysCpcr.o sysClkALib.o

ROM_EXTRA        = romSdramInit.o 

romExtras.a: $(EXTRA_OBJS)
	$(AR) crus $@ $(EXTRA_OBJS)
