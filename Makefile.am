AUTOMAKE_OPTIONS = foreign
SUBDIRS = src man themes

install-exec-hook:
	chgrp games @bindir@/xjump  || true
	chmod +s    @bindir@/xjump  || true
	
	mkdir -p    @localstatedir@/xjump
	touch       @localstatedir@/xjump/highscores
	chmod 660   @localstatedir@/xjump/highscores  || true
	chgrp games @localstatedir@/xjump/highscores  || true