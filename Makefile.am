# Makefile.am
#
# Copyright (C) 2014 Brian Davis
# All Rights Reserved
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

LIBRARY_CURRENT_INTERFACE_VERSION = 0
LIBRARY_CURRENT_INTERFACE_REVISION = 0
LIBRARY_SUPPORTED_INTERFACE_AGE = 0

LIBRARY_VERSION = $(LIBRARY_CURRENT_INTERFACE_VERSION):$(LIBRARY_CURRENT_INTERFACE_REVISION):$(LIBRARY_SUPPORTED_INTERFACE_AGE)

WARNING_FLAGS=\
	-Wall \
	-Wempty-body \
	-Wignored-qualifiers \
	-Wmissing-field-initializers \
	-Wsign-compare \
	-Wtype-limits \
	-Wuninitialized
	-Wunused-parameter \
	-Wunused-but-set-parameter

# Use of -Wextra turns on some warnings which cannot be controlled by
# #pragma in the body of the code and which are occurring in some of
# the template meta-programs; currently replaced by a set of relevant
# warnings (which appear after -Wall in the WARNING_FLAGS variable)
# -Wextra

AM_CXXFLAGS = $(WARNING_FLAGS) -std=c++14 $(BOOST_CPPFLAGS)
ACLOCAL_AMFLAGS = -I m4

# lib_LTLIBRARIES = \
# 	libjrlutil.la

include_HEADERS = \
	unitsafe \
	units_usability \
	log2int \
	jrl_util

bin_PROGRAMS = \
	testlog2int \
	testboostusability \
	testunitsafe \
	testjrlutil \
	testjrlgeom

testlog2int_SOURCES = \
	testlog2int.cpp

testboostusability_SOURCES = \
	testboostusability.cpp

testunitsafe_SOURCES = \
	testunitsafe.cpp

testjrlutil_SOURCES = \
	testjrlutil.cpp

testjrlgeom_SOURCES = \
	testjrlgeom.cpp

rebuild: clean all
