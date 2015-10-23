# Makefile.am
#
# Copyright (C) 2014 Brian Davis
# All Rights Reserved
# 
# Limited copying permission is given solely
# for self-educational purpose.
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
	testjrlutil

#	testunitsusability

testlog2int_SOURCES = \
	testlog2int.cpp

# testunitsusability_SOURCES =
# 	testunitsusability.cpp

testboostusability_SOURCES = \
	testboostusability.cpp

testunitsafe_SOURCES = \
	testunitsafe.cpp

testjrlutil_SOURCES = \
	testjrlutil.cpp

rebuild: clean all
