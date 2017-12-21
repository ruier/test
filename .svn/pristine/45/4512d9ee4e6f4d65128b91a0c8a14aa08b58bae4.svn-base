
CPU             = PENTIUM2
TOOL            = gnu
TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

BOARD           = PM8001

TARGET_DIR      = testD510
VENDOR          = prophet
BSP_DIR = $(TGT_DIR)/config/$(TARGET_DIR)
SRC_DIR = $(TGT_DIR)/config/$(TARGET_DIR)/driver
XJB8640DOBJDIR   = AHCIvx
XJB8640DLIB = AHCIvx.a

ADDED_CFLAGS    += -g2

#include $(TGT_DIR)/h/make/rules.bsp
EXTRA_INCLUDE  += -I$(BSP_DIR) 

SPC_LIB:
#DRIVER
	$(MAKE) $(BSP_DIR)/driver/spcdrv.o 
	$(AR) rv $(XJB8640DLIB) spcdrv.o
	$(MV) spcdrv.o $(XJB8640DOBJDIR)
#HARDWARE INTERFACE
	$(MAKE) $(BSP_DIR)/driver/spchwi.o 
	$(AR) rv $(XJB8640DLIB) spchwi.o
	$(MV) spchwi.o $(XJB8640DOBJDIR)   
#OSINFT
	$(MAKE) $(BSP_DIR)/driver/osintf.o 
	$(AR) rv $(XJB8640DLIB) osintf.o
	$(MV) osintf.o $(XJB8640DOBJDIR)  
#SPCBLK
	$(MAKE) $(BSP_DIR)/driver/spcblk.o 
	$(AR) rv $(XJB8640DLIB) spcblk.o
	$(MV) spcblk.o $(XJB8640DOBJDIR)	