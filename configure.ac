# -*- Autoconf -*-
#
# Copyright (C) 2014 Brian Davis
# All Rights Reserved
#
# Configure generating script for Jolly Roger Labs utility library.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Author: Brian Davis <bd@jollyrogerlabs.com>
#

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
