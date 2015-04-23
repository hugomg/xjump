AUTOMAKE_OPTIONS = foreign

# Settings
xjump_themes_dir = @datadir@/xjump/themes
xjump_highscore_dir = @localstatedir@/xjump
xjump_settings_dir = @sysconfdir@

xjump_default_theme = "default"
xjump_highscore_filename = "highscores"
xjump_highscore_number_of_entries = 20
xjump_config_filename = "xjump.conf"


# Sources
AM_CFLAGS  = --std=c99 --pedantic -Wall -O2
AM_LDFLAGS = -lXaw -lXt -lXpm -lX11
AM_CPPFLAGS = \
  -DXJUMP_THEMES_DIR=\"$(xjump_themes_dir)\" \
  -DXJUMP_SPRITES_FILEPATH=\"$(xjump_themes_dir)/$(xjump_default_theme).xpm\" \
  -DXJUMP_HIGHSCORE_FILEPATH=\"$(xjump_highscore_dir)/$(xjump_highscore_filename)\" \
  -DXJUMP_HIGHSCORE_ENTRIES=$(xjump_highscore_number_of_entries) \
  -DXJUMP_GLOBAL_SETTINGS_DIR=\"$(xjump_settings_dir)\" \
  -DXJUMP_CONFIG_FILENAME=\"$(xjump_config_filename)\"

bin_PROGRAMS = xjump

xjump_SOURCES = \
  src/game.c \
  src/main.c \
  src/misc.c \
  src/record.c \
  src/record.h \
  src/resource.c \
  src/safestr.c \
  src/safestr.h \
  src/xjump.h \
  src/icon.xbm \
  src/icon_msk.xbm

# Sprites

themesdir = $(xjump_themes_dir)
dist_themes_DATA = \
  themes/default.xpm \
  themes/ion.xpm \
  themes/jumpnbump.xpm

# Documentation
dist_man_MANS = xjump.6
dist_doc_DATA = README.md

# Desktop stuff
desktopdir = @datadir@/applications
dist_desktop_DATA = xjump.desktop

icondir = @datadir@/icons
dist_icon_DATA = xjump.png

install-exec-hook:
	chgrp games @bindir@/xjump  || true
	chmod +s    @bindir@/xjump  || true
	
	mkdir -p    $(xjump_highscore_dir)
	touch       $(xjump_highscore_dir)/$(xjump_highscore_filename)
	chmod 660   $(xjump_highscore_dir)/$(xjump_highscore_filename)  || true
	chgrp games $(xjump_highscore_dir)/$(xjump_highscore_filename)  || true

uninstall-hook:
	rm -rf $(xjump_highscore_dir)
	rm -rf @datadir@/xjump