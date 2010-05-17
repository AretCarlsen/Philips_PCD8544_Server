# Aret N Carlsen (aret@ncarlsen.net)
#
# Makefile for server
#
# Last mod: 2010-03

DIR = ./
export DIR := $(DIR)/..

#DEBUG = -DDEBUG

PROGRAM = server
SRC     = server.cpp

OBJS += virtual.o MemoryPool.o rprintf.o
OBJS += MEPEncoder.o MEPDecoder.o PosixCRC32Checksum.o SimpleServer.o MAP.o AddressGraph.o
OBJS += pcd8544.o sbFont.o images.o
OBJS += templateInstantiations.o

VPATH = ../common/avrlib:../common/virtual:../common/MemoryPool
VPATH += ../common/Packet/MEP:../common/Packet/MAP:../common/Packet/Servers:../common/Packet/PosixCRC32ChecksumEngine:../common/Packet/Routing
VPATH += ../common/ADCServer; ../drivers/Philips_PCD8544

all: server.hex server.eep

include $(DIR)/Makefile.inc

