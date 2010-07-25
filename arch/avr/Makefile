# Aret N Carlsen (aret@ncarlsen.net)
#
# Makefile for lcdServer AVR port
#
# Last mod: 2010-05

DEBUG = -DDEBUG

PROGRAM = server
SRC     = server.cpp
all: server.hex server.eep

OBJS = virtual.o MemoryPool.o rprintf.o
OBJS += MEPEncoder.o MEPDecoder.o PosixCRC32Checksum.o SimpleServer.o MAP.o AddressGraph.o
OBJS += sbFont.o images.o
OBJS += templateInstantiations.o

PROJROOT = /root/Projects
IROOT = -I $(PROJROOT)
IPATH = $(IROOT) -I .


VPATH = .
VPATH += $(PROJROOT)/avrlib:$(PROJROOT)/ATcommon/arch/avr:$(PROJROOT)/ATcommon/MemoryPool/arch/avr:$(PROJROOT)/ATcommon/arch/avr/virtual
VPATH += $(PROJROOT)/Upacket/MEP/arch/avr:$(PROJROOT)/Upacket/MAP/arch/avr:$(PROJROOT)/Upacket/PosixCRC32ChecksumEngine/arch/avr
VPATH += $(PROJROOT)/Upacket/Servers/arch/avr:$(PROJROOT)/Upacket/Routing/arch/avr
VPATH += $(PROJROOT)/Philips_PCD8544_driver/arch/avr

include $(PROJROOT)/ATcommon/arch/avr/Makefile.inc

