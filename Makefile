#
# Makefile for the Video Disk Recorder
#
# See the main source file 'vdr.c' for copyright information and
# how to reach the author.
#
# $Id: Makefile 1.13 2000/10/07 16:24:08 kls Exp $

DVBDIR   = ../DVB

INCLUDES = -I$(DVBDIR)/driver
OBJS = config.o dvbapi.o dvbosd.o eit.o font.o interface.o menu.o osd.o\
       recording.o remote.o svdrp.o thread.o tools.o vdr.o videodir.o

OSDFONT = -adobe-helvetica-medium-r-normal--23-*-100-100-p-*-iso8859-1

ifndef REMOTE
REMOTE = KBD
endif

DEFINES += -DREMOTE_$(REMOTE)

ifdef DEBUG_OSD
DEFINES += -DDEBUG_OSD
endif

all: vdr
font: genfontfile fontosd.c
	@echo "font file created."

# Implicit rules:

%.o: %.c
	g++ -g -O2 -Wall -m486 -c $(DEFINES) $(INCLUDES) $<

# Dependencies:

config.o   : config.c config.h dvbapi.h dvbosd.h eit.h font.h interface.h svdrp.h tools.h
dvbapi.o   : dvbapi.c config.h dvbapi.h dvbosd.h font.h interface.h svdrp.h tools.h videodir.h
dvbosd.o   : dvbosd.c dvbosd.h font.h tools.h
eit.o      : eit.c eit.h tools.h
font.o     : font.c font.h fontosd.c tools.h
interface.o: interface.c config.h dvbapi.h dvbosd.h eit.h font.h interface.h remote.h svdrp.h thread.h tools.h
menu.o     : menu.c config.h dvbapi.h dvbosd.h font.h interface.h menu.h osd.h recording.h svdrp.h tools.h
osd.o      : osd.c config.h dvbapi.h dvbosd.h font.h interface.h osd.h svdrp.h tools.h
recording.o: recording.c config.h dvbapi.h dvbosd.h font.h interface.h recording.h svdrp.h tools.h videodir.h
remote.o   : remote.c config.h dvbapi.h dvbosd.h font.h remote.h thread.h tools.h
svdrp.o    : svdrp.c config.h dvbapi.h dvbosd.h font.h interface.h svdrp.h tools.h
thread.o   : thread.c thread.h
tools.o    : tools.c tools.h
vdr.o      : vdr.c config.h dvbapi.h dvbosd.h font.h interface.h menu.h osd.h recording.h svdrp.h tools.h videodir.h
videodir.o : videodir.c tools.h videodir.h

# The main program:

vdr: $(OBJS)
	g++ -g -O2 $(OBJS) -lncurses -ljpeg -lpthread -o vdr

# The font file:

fontosd.c:
	genfontfile "cFont::tPixelData FontOsd" $(OSDFONT) > $@

# The font file generator:

genfontfile.o: genfontfile.c
	gcc -O2 -c $<
genfontfile: genfontfile.o
	gcc -o $@ -L/usr/X11R6/lib $< -lX11

# Housekeeping:

clean:
	-rm -f $(OBJS) vdr genfontfile genfontfile.o
CLEAN: clean
	-rm -f fontosd.c
