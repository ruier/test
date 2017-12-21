###############################################################################
#
# IBM RISCWatch command file for updating the Walnut board flash with a VxWorks
# bootrom image (bootrom.hex or bootrom_uncmp.hex).  JTAG RISCWatch is required.
#
#
# Usage:  From a RISCWatch command line type one the following commands:
#
#            exec vx_rw_flash.cmd {"bootrom_uncmp.hex"}
#
#            exec vx_rw_flash.cmd {"bootrom_.hex"}
#
#
# This utility makes the following assumptions:
#      - The 32MB SDRAM DIMM shipped with the Walnut board is being used.
#        Modifications to the SDRAM controller configuration may be necessary
#        if a different DIMM is being used (see note below).
#      - An AMD Am29F040 (512KB) flash part is being used.
#      - ROM_TEXT_ADRS in Makefile and config.h equals 0xfff80100.
#
# modification history
# --------------------
# 01b,10jul00,mcg  fixed polling, changed from chip to system reset
# 01a,27apr00,mcg  created
#
#

#
# Reset the chip
#
parms {input_file}                  # filename parameter being passed in
unload all
window output
print "Initializing 405GP."
halt on
reset sys
stop
halt off
stop
jtag pass cf fastload=no            # not required if using RISCWatch v4.6

#
# Initialize EBC Bank 0 for FLASH
#
write EBC0_B0AP    0x9B015480
write EBC0_B0CR    0xFFF18000

#
# Initialize SDRAM controller
# NOTE: These values may need to be modified if the SDRAM DIMM is different
# than the one shipped with the Walnut board.
#
write SDRAM0_B0CR  0x00046001       # Bank 0
write SDRAM0_B2CR  0x01046001       # Bank 2
write SDRAM0_TR    0x00854009
write SDRAM0_CFG   0x80800000

#
# Set up some machine state registers.
#
write EVPR         0x00000000
write SGR          0x00000000
write DCWR         0x00000000
write ESR          0x00000000
write TCR          0x00000000
write MSR          0x00001000       # machine check enabled
write DBSR         0xffffffff
write XER          0x00000000
write ICCR         0x80000000       # use I-cache for faster execution

#
# Setup and test the serial port
#
#print "Testing the Serial Port."

#
# Initialize Chip CONTROL0 reg
# - use external serial clock for UART0
#
#set CPC0_CR0.U0EC = 1

#
# Initialize Serial Port 0
#
#write UART0_LCR 0x80            # Access DLAB reg
#write UART0_DLL 0x48            # Set DLAB reg.  Baud = 9600
#write UART0_DLM 0x00            # DLL=0x48, DLM=0x00
#write UART0_LCR 0x03            # Turn off DLAB bit; 8 bits; no parity
#write UART0_FCR 0x00            # Disable fifos
#write UART0_MCR 0x03
#read UART0_LSR
#read UART0_RBR
#write UART0_SCR 0x00            # Reset scratch reg
#write UART0_IER 0x00            # Disable ints

#
# Output "ABCDEFG"
#
#write UART0_THR 0x41
#read  UART0_LSR
#write UART0_THR 0x42
#read  UART0_LSR
#write UART0_THR 0x43
#read  UART0_LSR
#write UART0_THR 0x44
#read  UART0_LSR
#write UART0_THR 0x45
#read  UART0_LSR
#write UART0_THR 0x46
#read  UART0_LSR
#write UART0_THR 0x47
#read  UART0_LSR
#write UART0_THR 0x0d
#read  UART0_LSR
#write UART0_THR 0x0a
#read  UART0_LSR


#
# Initialize SDRAM between 0x00000000 and 0x00080000 to look like a blank
# flash (all 0xff).
#
print "Initializing SDRAM."
memfill 0x00000000 0x00080000 0xff

#
# Load the new bootrom image into SDRAM on top of the memory just initialized.
#
print "Loading new VxWorks bootrom image ", input_file, " into SDRAM."
create before
set before = $ERRORS
load mot input_file
if ($ERRORS > before)
   uncreate input_file
   end                          # exit command file
endif

#
# Place a branch instruction at the end of the bootrom image that will get
# the processor from its reset vector to the entry point of the bootrom when
# it is executed from flash (0xFFFFFFFC to ROM_TEXT_ADRS).  Update this
# instruction if ROM_TEXT_ADRS is modified in the BSP.
#
assm "ba   0xfff80100"  0x0007fffc

print "Loading flash update code into SDRAM."

#
# Put assembly code in SDRAM that will program the bootrom image into an
# AMD 29F040 flash device
#

# progflash:
write r3 0xfff80000         # flash base address
write r4 0x00000000         # base address of new ROM image
write r5 0xfff80000         # flash base address for increment

# Set r3 to base flash address
assm "addi    r0, 0, 0xF0"                      0x80000
assm "stb     r0, 0x0000(r3)"                   0x80004
assm "addi    r0, 0, 0xAA"                      0x80008
assm "stb     r0, 0x5555(r3)"                   0x8000c
assm "addi    r0, 0, 0x55"                      0x80010
assm "stb     r0, 0x2AAA(r3)"                   0x80014
assm "addi    r0, 0, 0x90"                      0x80018
assm "stb     r0, 0x5555(r3)"                   0x8001c

# r10 = MFG code
# r11 = Device code
assm "lbz     r10, 0x0000(r3)"                  0x80020
assm "lbz     r11, 0x0001(r3)"                  0x80024

# Reset flash for read access
assm "addi    r0, 0, 0xf0"                      0x80028
assm "stb     r0, 0x0000(r3)"                   0x8002c

# Does MFG code = 0x01 (AMD)?
assm "cmpwi   r10, 0x01"                        0x80030
assm "bne     $+0x0c"                           0x80034  # to device_err:

# Does Device code = 0xa4 (29F040)?
assm "cmpwi   r11, 0xa4"                        0x80038
assm "beq     $+0x0c"                           0x8003c  # to device_ok:

#device_err:
assm "addi    r0, 0, -1"                        0x80040
assm "bl      $+0xd8"                           0x80044  # to done:

#device_ok:
# erase whole chip
assm "addi    r0, 0, 0xaa"                      0x80048
assm "stb     r0, 0x5555(r3)"                   0x8004c
assm "addi    r0, 0, 0x55"                      0x80050
assm "stb     r0, 0x2aaa(r3)"                   0x80054
assm "addi    r0, 0, 0x80"                      0x80058
assm "stb     r0, 0x5555(r3)"                   0x8005c
assm "addi    r0, 0, 0xaa"                      0x80060
assm "stb     r0, 0x5555(r3)"                   0x80064
assm "addi    r0, 0, 0x55"                      0x80068
assm "stb     r0, 0x2aaa(r3)"                   0x8006c
assm "addi    r0, 0, 0x10"                      0x80070
assm "stb     r0, 0x5555(r3)"                   0x80074

#erase_loop:
assm "lbz     r0, 0x0000(r3)"                   0x80078
assm "rlwinm  r0,r0,0x0,0x18,0x18"              0x8007c
assm "cmpwi   cr0, r0, 0"                       0x80080
assm "bne     $+0x28"                           0x80084  # to program_flash:
assm "lbz     r0, 0x0000(r3)"                   0x80088
assm "rlwinm  r0, r0, 0x0, 0x1a, 0x1a"          0x8008c
assm "cmpwi   cr0, r0, 0"                       0x80090
assm "beq     $-0x1c"                           0x80094  # to erase_loop:
assm "lbz     r0, 0x0000(r3)"                   0x80098
assm "rlwinm  r0, r0, 0x0, 0x18, 0x18"          0x8009c
assm "cmpwi   cr0, r0, 0"                       0x800a0
assm "bne     $+0x08"                           0x800a4  # to program_flash:
# err condition so set return code and leave
assm "bl      $-0x68"                           0x800a8  # to device_err:

#program_flash:
# Set (size) counter to 0x80000 bytes (512KB)
assm "addis   r0, 0, 0x0008"                    0x800ac
assm "ori     r0, r0, 0x0000"                   0x800b0
assm "mtctr   r0"                               0x800b4

#top_of_loop:
assm "addi    r0, 0, 0xaa"                      0x800b8
assm "stb     r0, 0x5555(r3)"                   0x800bc
assm "addi    r0, 0, 0x55"                      0x800c0
assm "stb     r0, 0x2aaa(r3)"                   0x800c4
assm "addi    r0, 0, 0xa0"                      0x800c8
assm "stb     r0, 0x5555(r3)"                   0x800cc
assm "lbz     r2, 0x0000(r4)"                   0x800d0
assm "stb     r2, 0x0(r5)"                      0x800d4

#poll:
assm "lbz     r0, 0x0000(r5)"                   0x800d8
assm "rlwinm  r0, r0, 0x0, 0x18, 0x18"          0x800dc
assm "rlwinm  r1, r2, 0x0, 0x18, 0x18"          0x800e0
assm "cmpw    cr0, r0, r1"                      0x800e4
assm "beq     $+0x28"                           0x800e8  # to increment counters

assm "lbz     r0, 0x0(r5)"                      0x800ec
assm "rlwinm  r0, r0, 0x0, 0x1a, 0x1a"          0x800f0
assm "cmpwi   cr0, r0, 0"                       0x800f4
assm "beq     $-0x20"                           0x800f8  # to poll:
assm "lbz     r0, 0x0(r5)"                      0x800fc
assm "rlwinm  r0, r0, 0x0, 0x18, 0x18"          0x80100
assm "rlwinm  r1, r2, 0x0, 0x18, 0x18"          0x80104
assm "cmpw    cr0, r0, r1"                      0x80108
assm "bne     $-0xcc"                           0x8010c  # to device_err
# increment counters
assm "addi    r4, r4, 1"                        0x80110
assm "addi    r5, r5, 1"                        0x80114
assm "bdnz    $-0x60"                           0x80118  # to top_of_loop:
#done:
assm "b       $+0"                              0x8011c

write iar 0x80000
bp set 0x8011c            # set a breakpoint at the end
stuff "iccci r0,r0"       # invalidate I-cache

print "Starting flash update."
run

if (r0 == -1)
  print "Flash update failed."
else
  print "Flash update successful."
endif

uncreate input_file
bp clear all
