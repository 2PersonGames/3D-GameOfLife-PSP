#
# Author:	Sam Albon
# Created:	22/02/2011
# Defines:	Makefile for GameOfLife 3D for PSP
# 			Based on fountain makefile
#

SDK_TOP		= C:/cygwin/usr/local/psp/devkit
INCDIR		= $(SDK_TOP)/include
LIBDIR		= $(SDK_TOP)/lib

DBGFLAG		= -g
OPTFLAG		= -Os
#SECTFLAG	= 
CFLAGS		= -W -Wall $(DBGFLAG) $(OPTFLAG) $(SECTFLAG) -I$(INCDIR)
ASFLAGS		= $(DBGFLAG)
LDFLAGS		= $(DBGFLAG) -L$(LIBDIR)
LDLIBS		= -lgum -lgu -lm
LOADLIBES	=  $(LIBDIR)/ctrl_stub.a
LOADLIBES	+= $(LIBDIR)/display_stub.a
LOADLIBES	+= $(LIBDIR)/ge_user_stub.a

TARGET	= GameOfLife_100068439
OBJS	= CubeData.o FrontEnd.o main.o

all:	$(TARGET).prx #$(TARGET).elf

clean:
	rm -f $(TARGET).prx $(TARGET).elf *.sym *.o *~ *.bak

$(TARGET).prx: $(OBJS)
	$(LINK.c) $^ $(LDLIBS) $(LOADLIBES) -startfiles -zgc-sections -zv -o $@.sym
	$(STRIP) --strip-section-header -o $@ $@.sym

$(TARGET).elf: $(OBJS)
	$(LINK.c) $^ $(LDLIBS) $(LOADLIBES) -zgenelf -Wl,-gc-sections -o $@.sym
	$(STRIP) --strip-section-header -o $@ $@.sym

#----------- rules --------------
-include PathDefs
PathDefs:
	psp-path-setup > PathDefs || (rm -f PathDefs ; exit 1)
