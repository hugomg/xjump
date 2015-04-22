AM_CFLAGS   = --std=c99 --pedantic -Wall -O2
AM_CPPFLAGS = -DRECORD_ENTRY=20 \
              -DRECORD_FILE=\"@localstatedir@/xjump/highscores\" \
              -DGRAPH_FILE=\"@datadir@/xjump/themes/default.xpm\"

AM_LDFLAGS = -lXaw -lXt -lXpm -lX11

bin_PROGRAMS = xjump

xjump_SOURCES = game.c main.c misc.c record.c record.h resource.c xjump.h icon.xbm icon_msk.xbm