# -*- Autoconf -*-
#
# Copyright (C) 2014 Brian Davis
# All Rights Reserved
#
# Limited copying permission is given solely
# for self-educational purpose.
#
# Author: Brian Davis <bd@jollyrogerlabs.com>
#
# Configure generating script for Jolly Roger Labs utility library.

AC_PREREQ([2.68])
AC_INIT([libjrlutil], [0.0.0], [bd@jollyrogerlabs.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([jrl_util])
AC_CONFIG_HEADERS([config.h])

# Initialize automake support.
AM_INIT_AUTOMAKE([-Wall foreign])
AM_MAINTAINER_MODE

# Initialize libtool support.
LT_PREREQ([2.2])
LT_INIT([disable-shared])

# Various build types:

# Debug build
AC_ARG_ENABLE(debug,
	AS_HELP_STRING([--enable-debug],
		       [Build with debugging @<:@default=no@:>@]),
        [ case "${enableval}" in
          yes)
                CXXFLAGS="$CXXFLAGS -O0 -g";
          ;;
          no) ;;
          *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
          esac ],
        [ debug=false ])

# Default values of CXXFLAGS if not previously set.
if test "$CXXFLAGS" == ""; then
    CXXFLAGS="-O3 -DNDEBUG"
fi

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T

# Checks for library functions.

# Create Makefile
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
