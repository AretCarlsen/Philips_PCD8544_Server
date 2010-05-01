# Aret N Carlsen (aret@ncarlsen.net)
#
# Makefile for server
#
# Last mod: 2010-03

DIR = ./

export DIR := $(DIR)/..
include $(DIR)/Makefile.inc

#DEBUG = -DDEBUG

PROGRAM = server
SRC     = server.cpp

export AVR_FREQ   = 20000000L
ISPTOOL    = avrisp
ISPPORT    = /dev/ttyUSB0
ISPSPEED   = -b 38400
# 19200

export MCU_TARGET = atmega328p
ISP_TARGET = m328
# atmega328p
# LDSECTION  = --section-start=.text=0x3800

ISPFLASH    = avrdude -c $(ISPTOOL) -p $(ISP_TARGET) -P $(ISPPORT) $(ISPSPEED) -U flash:w:server.hex -U eeprom:w:server.eep
#ISPFLASH    = avrdude -c $(ISPTOOL) -p $(ISP_TARGET) -P $(ISPPORT) $(ISPSPEED) -U flash:w:$(PROGRAM)_$(TARGET).hex
# -U lock:w:0x0f:m

FORMAT = ihex
MSG_EEPROM = Creating load file for EEPROM:

OBJ     = $(PROGRAM).o
OPTIMIZE   = -Os

DEFS       = $(DEBUG)
LIBS       =

export CXXFLAGS   = -g -Wall -std=c++0x $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(AVR_FREQ) $(DEFS) -fno-implicit-templates
# -x c++
LDFLAGS       =
# -Wl ,$(LDSECTION)

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

OBJS = $(OBJ)
OBJS += virtual.o MemoryPool.o rprintf.o
OBJS += MEPEncoder.o MEPDecoder.o PosixCRC32Checksum.o SimpleServer.o MAP.o AddressGraph.o
OBJS += pcd8544.o sbFont.o images.o
OBJS += templateInstantiations.o

VPATH = ../common/avrlib:../common/virtual:../common/MemoryPool
VPATH += ../common/Packet/MEP:../common/Packet/MAP:../common/Packet/Servers:../common/Packet/PosixCRC32ChecksumEngine:../common/Packet/Routing
VPATH += ../common/ADCServer; ../drivers/Philips_PCD8544

$(PROGRAM) : $(SRC) $(OBJS)
	@echo "*** Building $@ ..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -Wl,-Map,server.map

all: server.hex server.eep

#atmega328: TARGET = atmega328
#atmega328: $(PROGRAM)_atmega328.hex

program: force
	$(ISPFLASH)

clean:
	$(ECHO) cleaning up in .
	-$(RM) -f *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex core $(OBJS)
#	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

fresh:
	make clean
	make all

%.elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) -Wl,-Map,$*.map

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

.PHONY: all clean fresh force

