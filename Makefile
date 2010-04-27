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

export CXXFLAGS   = -g -Wall -std=c++0x $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(AVR_FREQ) $(DEFS) -Wl,-Map,server.map
# -fno-implicit-templates
# -x c++
LDFLAGS       =
# -Wl ,$(LDSECTION)

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

PacketOBJS = common/Packet/MEP/MEPEncoder.o common/Packet/MEP/MEPDecoder.o common/Packet/PosixCRC32ChecksumEngine/PosixCRC32Checksum.o common/Packet/Servers/SimpleServer.o common/Packet/MAP/MAP.o common/Packet/Routing/AddressGraph.o
OBJS = $(OBJ) $(PacketOBJS) common/virtual/virtual.o common/MemoryPool/MemoryPool.o common/avrlib/rprintf.o common/PCD8544/pcd8544.o common/PCD8544/sbFont.o images.o

$(PROGRAM) : $(SRC) $(OBJS)
	@echo "*** Building $@ ..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -Wl,-Map,server.map

all: server.hex server.eep

#atmega328: TARGET = atmega328
#atmega328: $(PROGRAM)_atmega328.hex

common/Packet/MEP/%.o: force
	{ cd ../common/Packet/MEP; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/Packet/MEP/$*.o common/Packet/MEP/$*.o

common/Packet/MAP/%.o: force
	{ cd ../common/Packet/MAP; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/Packet/MAP/$*.o common/Packet/MAP/$*.o

common/Packet/Servers/%.o: force
	{ cd ../common/Packet/Servers; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/Packet/Servers/$*.o common/Packet/Servers/$*.o

common/Packet/PosixCRC32ChecksumEngine/%.o: force
	{ cd ../common/Packet/PosixCRC32ChecksumEngine; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/Packet/PosixCRC32ChecksumEngine/$*.o common/Packet/PosixCRC32ChecksumEngine/$*.o

common/virtual/%.o: force
	{ cd ../common/virtual; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/virtual/$*.o common/virtual/$*.o

common/MemoryPool/%.o: force
	{ cd ../common/MemoryPool; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/MemoryPool/$*.o common/MemoryPool/$*.o

common/MotorDriver/%.o: force
	{ cd ../common/MotorDriver; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/MotorDriver/$*.o common/MotorDriver/$*.o

common/AnalogEncoder/%.o: force
	{ cd ../common/AnalogEncoder; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/AnalogEncoder/$*.o common/AnalogEncoder/$*.o

common/ADCServer/%.o: force
	{ cd ../common/ADCServer; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/ADCServer/$*.o common/ADCServer/$*.o

common/Packet/Routing/%.o: force
	{ cd ../common/Packet/Routing; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/Packet/Routing/$*.o common/Packet/Routing/$*.o

common/avrlib/%.o: force
	{ cd ../common/avrlib; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/avrlib/$*.o common/avrlib/$*.o

common/PCD8544/%.o: force
	{ cd ../common/PCD8544; $(MAKE) $(MFLAGS) $*.o; }
	mv ../common/PCD8544/$*.o common/PCD8544/$*.o

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
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

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

