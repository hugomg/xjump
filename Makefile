########################################################

# 使用するコンパイラとオプションを指定
CC      = gcc -O2 

# 標準以外のインクルードファイルのディレクトリを指定
IDIR    = -I/usr/X11R6/include

# 標準以外のライブラリのディレクトリを指定
LDIR    = -L/usr/X11R6/lib

#########################################################

# インストール先を指定
TARGET = /usr/games/

# レコードファイルのディレクトリを指定
RECORD_DIR  = /var/games/xjump

#レコードファイルのファイル名を指定(注：これはバイナリに埋め込まれます)
RECORD_FILE  = $(RECORD_DIR)/record

#########################################################

# レコードのエントリ数を指定(注：これはバイナリに埋め込まれます)
RECORD_ENTRY  = 20

##########################################################

xjump : main.o game.o key.o misc.o record.o resource.o
	$(CC) -o xjump \
		main.o game.o key.o misc.o record.o resource.o\
		-lXaw -lXmu -lXt -lXpm -lXext -lX11 $(LDIR)

main.o : main.c xjump.h record.h \
		picture.xpm icon.xbm icon_msk.xbm Makefile
	$(CC) -c -DRECORD_ENTRY=$(RECORD_ENTRY) main.c $(IDIR)

resource.o : resource.c
	$(CC) -c resource.c

game.o : game.c xjump.h
	$(CC) -c game.c $(IDIR)

key.o : key.c xjump.h
	$(CC) -c key.c $(IDIR)

misc.o : misc.c
	$(CC) -c misc.c

record.o : record.c record.h Makefile
	$(CC) -c \
		-DRECORD_ENTRY=$(RECORD_ENTRY) \
		-DRECORD_FILE=\"$(RECORD_FILE)\" \
		record.c


install	: xjump
	install -g games -c -s -m 2755 xjump $(DESTDIR)$(TARGET)

clean:
	rm -f *.o xjump *~
