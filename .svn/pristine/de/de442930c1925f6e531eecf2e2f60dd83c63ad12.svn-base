# Makefile - makefile for Freescale T4240QDS board
#
# Copyright (c) 2012, 2013 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
# 
# modification history
# --------------------
# 01i,28mar13,h_k  extracted fslDdrCtlr.o from libdrv.a to fix the build error
#                  in install env.
# 01h,11mar13,wyt  Fix symbol error for vxprj build. (WIND00407534)
# 01g,26feb13,syt  Rename sysDdrConfig.o to sysDdrConf.o.
# 01f,08feb13,syt  Added ROM_EXTRA for DDR SPD code.
# 01e,23jan13,wyt  Update command for creating microcode patch
# 01d,19jan13,x_s  Corrected the wrong ROM_WARM_ADRS.
# 01c,16jan13,wap  Update to new Fman microcode
# 01b,18sep12,h_k  take gnu out of make release.
# 01a,07may12,h_k  created from fsl_p5020_ds 01a and ads8572 01h.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale T4240
#
# INCLUDES
#     makeTarget
#*/

CPU		= PPC32
TOOL		= diab

## Support for 36 bit physical addresses in the MMU can be added for 
## VxWorks 6.8.2 and later with the following CPU_VARIANT.

CPU_VARIANT	= _ppc85XX_e6500

TGT_DIR		= $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= fsl_t4240_qds
VENDOR		= Freescale
BOARD		= fsl_t4240_qds

RELEASE		+= bootrom.bin

LIB_EXTRA        = romExtras.a

## Until GCC supports e6500
RELEASE_PRJ	= prj_diab

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS	= fff00100	# ROM entry address
ROM_SIZE	= 00100000	# number of bytes of ROM space
ROM_WARM_ADRS	= fff00104	# ROM warm entry address 
ROM_LINK_ADRS	= fff00000	# ROM base address
RAM_LOW_ADRS	= 00100000	# RAM text/data address

# Set RAM_HIGH_ADRS to 0x01800000, corresponding to 20bsp.cdf bootApp
# RAM_LOW_ADRS.  Thus, bootrom.bin, bootApp/vxWorks_romCompress,
# vxWorks_romCompress run at this address.  (And bootApp/vxWorks_romResident,
# if supported, uses this address for .data, .bss, system heap and so on.)
# The bootApp RAM image(PROFILE_BOOTAPP VIP including INCLUDE_BOOT_RAM_IMAGE)
# runs at 0x01000000 and the bootApp ROMs can load bootApp RAM image
# successfully.

RAM_HIGH_ADRS	= 01800000	# RAM text/data address

## The following microcode patch is required for the
## frame manager in the T4240.

subststring =$(join $(subst x,$(subst -,_,$(subst .,_,$(subst /,_,$(1)))),_binary_x_),$(2))
fsl_fman_ucode_%.obj: fsl_fman_ucode_%.bin
	objcopyppc -I binary -O elf32-powerpc-vxworks \
	--redefine-sym $(call subststring,$<,start)=$(call subststring,$(notdir $<),start) \
	--redefine-sym $(call subststring,$<,end)=$(call subststring,$(notdir $<),end) \
	-N $(call subststring,$<,size) \
	-B powerpc:603 $< $@
UCODE_BINLIST = $(wildcard fsl_fman_ucode_*.bin)
UCODE_OBJLIST = $(UCODE_BINLIST:.bin=.obj)

MACH_EXTRA	= $(UCODE_OBJLIST)
VMA_START	= 0x0
USR_ENTRY	= usrInit

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

ROM_EXTRA	= fslDdrCtlr.o sysDdrConf.o

romExtras.a: $(ROM_EXTRA)
	$(AR) crus $@ $(ROM_EXTRA)

fslDdrCtlr.o:
	$(AR) -x $(TGT_DIR)/lib/ppc/PPC32/common/libdrv.a fslDdrCtlr.o
