/** -*- mode: c++ -*-
 *
 * test_stack_allocator.cpp
 *
 * Test cases for stl_allocator code.
 *
 * Copyright (C) 2016 Brian Davis
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Brian Davis <bd@jollyrogerlabs.com>
 *
 */

// @todo
#include <iostream>

#include <cstring>

#include <vector>

#include "stack_allocator"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl::util;

using IntVector = vector<int>;
using IntVectorStack = vector<int, stack_allocator<int>>;
using Iterator = IntVector::iterator;
using ConstIterator = IntVector::const_iterator;
using IntStringMap = map<int, const char *>;

/**
 * Test case: reserve memory, build a vector in it and compare with a
 * vector defined using std::allocator.
 */
TEST_CASE("compare", "[compare]")
{
  IntVector ints;
  IntVectorStack stackInts;
  ints.reserve(4);
  stackInts.reserve(4);
  REQUIRE(ints.size() == stackInts.size());
  for (size_t idx = 0; idx < ints.size(); ++idx) {
    ints.push_back(idx);
    stackInts.push_back(idx);
  }
  REQUIRE(ints.size() == stackInts.size());
  for (size_t idx = 0; idx < ints.size(); ++idx) {
    REQUIRE(idx == ints[idx]);
    REQUIRE(idx == stackInts[idx]);
    REQUIRE(ints[idx] == stackInts[idx]);
  }
}

/**
 * Test case: as previous, but memory is not reserved.
 */
TEST_CASE("build", "[build]")
{
  IntVector ints;
  IntVectorStack stackInts;
  REQUIRE(ints.size() == stackInts.size());
  for (size_t idx = 0; idx < ints.size(); ++idx) {
    ints.push_back(idx);
    stackInts.push_back(idx);
    // ints[idx] = idx;
    // stackInts[idx] = idx;
  }
  REQUIRE(ints.size() == stackInts.size());
  for (size_t idx = 0; idx < ints.size(); ++idx) {
    REQUIRE(idx == ints[idx]);
    REQUIRE(idx == stackInts[idx]);
    REQUIRE(ints[idx] == stackInts[idx]);
  }
}
