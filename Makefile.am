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
	log2int \
	logging \
	jrl_algorithm \
	jrl_macro \
	jrl_metaprogram \
	jrl_util \
	stl_usability/algorithm_usability \
	stl_usability/misc_usability \
	stl_usability/ostream_innerator \
	unitsafe

bin_PROGRAMS = \
	test_log2_int \
	test_boost_usability \
	test_hash_set \
	test_jrl_algorithm \
	test_jrl_geom \
	test_jrl_macro \
	test_jrl_metaprogram \
	test_jrl_util \
	test_logging \
	test_iterable_buffer_set \
	test_stl_usability \
	test_unitsafe

TESTS = \
	test_log2_int \
	test_boost_usability \
	test_hash_set \
	test_jrl_algorithm \
	test_jrl_geom \
	test_jrl_macro \
	test_jrl_metaprogram \
	test_jrl_util \
	test_logging \
	test_iterable_buffer_set \
	test_stl_usability \
	test_unitsafe

test_log2_int_SOURCES = \
	test/test_log2_int.cpp

test_boost_usability_SOURCES = \
	test/test_boost_usability.cpp

test_hash_set_SOURCES = \
	test/test_hash_set.cpp

test_jrl_algorithm_SOURCES = \
	test/test_jrl_algorithm.cpp

test_jrl_geom_SOURCES = \
	test/test_jrl_geom.cpp

test_jrl_macro_SOURCES = \
	test/test_jrl_macro.cpp

test_jrl_metaprogram_SOURCES = \
	test/test_jrl_metaprogram.cpp

test_jrl_util_SOURCES = \
	test/test_jrl_util.cpp

test_logging_SOURCES = \
	test/test_logging.cpp

test_iterable_buffer_set_SOURCES = \
	test/test_iterable_buffer_set.cpp

test_stl_usability_SOURCES = \
	test/test_stl_usability.cpp

test_unitsafe_SOURCES = \
	test/test_unitsafe.cpp

rebuild: clean all
