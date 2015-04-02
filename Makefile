########################################################

# Compiler options

CC      = gcc
CFLAGS	= -O2
IDIR    = -I/usr/X11R6/include
LDIR    = -L/usr/X11R6/lib

#########################################################

# Instalation options

TARGET = /usr/games/

RECORD_DIR  = /var/games/xjump
RECORD_FILE  = $(RECORD_DIR)/record

#########################################################

# Maximum size of the highscores table? (This gets embedded in the binary)
RECORD_ENTRY  = 20

##########################################################

xjump : main.o game.o misc.o record.o resource.o
	$(CC) $(CFLAGS) -o xjump \
		main.o game.o misc.o record.o resource.o\
		-lXaw -lXt -lXpm -lX11 $(LDIR)

main.o : main.c xjump.h record.h \
		themes/default.xpm icon.xbm icon_msk.xbm Makefile
	$(CC) $(CFLAGS) -c -DRECORD_ENTRY=$(RECORD_ENTRY) main.c $(IDIR)

resource.o : resource.c
	$(CC) $(CFLAGS) -c resource.c

game.o : game.c xjump.h
	$(CC) $(CFLAGS) -c game.c $(IDIR)

misc.o : misc.c
	$(CC) $(CFLAGS) -c misc.c

record.o : record.c record.h Makefile
	$(CC) $(CFLAGS) -c \
		-DRECORD_ENTRY=$(RECORD_ENTRY) \
		-DRECORD_FILE=\"$(RECORD_FILE)\" \
		record.c


install	: xjump
	install -g games -c -s -m 2755 xjump $(DESTDIR)$(TARGET)

clean:
	rm -f *.o xjump *~
