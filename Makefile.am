AUTOMAKE_OPTIONS = foreign
SUBDIRS = src man themes

desktopdir = $(datadir)/applications
dist_desktop_DATA = xjump.desktop

icondir = $(datadir)/icons
dist_icon_DATA = xjump.png

install-exec-hook:
	chgrp games @bindir@/xjump  || true
	chmod +s    @bindir@/xjump  || true
	
	mkdir -p    @localstatedir@/xjump
	touch       @localstatedir@/xjump/highscores
	chmod 660   @localstatedir@/xjump/highscores  || true
	chgrp games @localstatedir@/xjump/highscores  || true

uninstall-hook:
	rm -rf @localstatedir@/xjump
	rm -rf @datadir@/xjump