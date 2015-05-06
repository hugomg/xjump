AUTOMAKE_OPTIONS = foreign

# Settings
xjump_themes_dir = @datadir@/xjump/themes
xjump_highscore_dir = @localstatedir@/xjump
xjump_settings_dir = @sysconfdir@

xjump_default_theme = "default"
xjump_highscore_filename = "highscores"
xjump_highscore_number_of_entries = 20
xjump_config_filename = "xjump.conf"

####

xjump_highscore_filepath = $(xjump_highscore_dir)/$(xjump_highscore_filename)

# This gets appended later...
EXTRA_DIST =

# Sources
AM_CFLAGS = --std=c99 --pedantic -Wall -O2 -fPIE
AM_LDFLAGS = -pie
AM_CPPFLAGS = \
  -D_FORTIFY_SOURCE=2 \
  -DXJUMP_THEMES_DIR=\"$(xjump_themes_dir)\" \
  -DXJUMP_SPRITES_FILEPATH=\"$(xjump_themes_dir)/$(xjump_default_theme).xpm\" \
  -DXJUMP_HIGHSCORE_FILEPATH=\"$(xjump_highscore_filepath)\" \
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
EXTRA_DIST += README.md

# Desktop menu integration
desktopdir = @datadir@/applications
dist_desktop_DATA = xjump.desktop

icondir = @datadir@
nobase_dist_icon_DATA = \
  icons/hicolor/32x32/apps/xjump.png


# Scripts:

dist_TESTS = \
  scripts/check-desktop-files.sh

install-exec-hook:
	if [ ! -e  $(DESTDIR)$(xjump_highscore_dir)/$(xjump_highscore_filename) ]; then \
	  mkdir -p $(DESTDIR)$(xjump_highscore_dir); \
	  echo   > $(DESTDIR)$(xjump_highscore_dir)/$(xjump_highscore_filename); \
	fi # Make sure the highscore file has non-zero length to make rpmlint happy...
	
	if [ $$(id -u) = 0 ] && \
	   getent group  games >/dev/null && \
	   getent passwd games >/dev/null; \
	then \
	  chgrp games $(DESTDIR)@bindir@/xjump; \
	  chmod g+s   $(DESTDIR)@bindir@/xjump; \
	  chgrp games $(DESTDIR)$(xjump_highscore_dir); \
	  chmod 755   $(DESTDIR)$(xjump_highscore_dir); \
	  chgrp games $(DESTDIR)$(xjump_highscore_dir)/$(xjump_highscore_filename); \
	  chmod 660   $(DESTDIR)$(xjump_highscore_dir)/$(xjump_highscore_filename); \
	fi

uninstall-hook:
	rm -rf $(DESTDIR)$(xjump_highscore_dir)
	rm -rf $(DESTDIR)@datadir@/xjump