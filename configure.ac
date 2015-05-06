#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([xjump], [2.9.2], [hgualandi@gmail.com])
AC_DEFINE([MAJOR_VERSION], ["2.9"], [Major version number])  # To display inside game. Dunno how to automate this

AC_CONFIG_SRCDIR([src/game.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h stddef.h sys/file.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UID_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([ftruncate])
AC_CHECK_FUNCS([strchr])

# Check for dynamic libraries

AC_CHECK_LIB(X11, XCreateBitmapFromData, [], [
  echo "Error: You need to install libx11-dev"
  exit -1
])

AC_CHECK_LIB(Xt, XtAddEventHandler, [], [
  echo "Error: You need to install libxt-dev"
  exit -1
])

AC_CHECK_LIB(Xpm, XpmReadFileToPixmap, [], [
  echo "Error: You need to install libxpm-dev"
  exit -1
])

AC_CHECK_LIB(Xaw, formWidgetClass, [], [
  echo "Error: You need to install libxaw7-dev"
  exit -1
])

# Done!
AC_OUTPUT