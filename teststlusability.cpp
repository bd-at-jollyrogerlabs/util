/** -*- mode: c++ -*-
 *
 * teststlusability.cpp
 *
 * Test cases for stl_usability code.
 *
 * Copyright (C) 2015 Brian Davis
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

#include <vector>
#include <functional>

#include "stl_usability"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

using IntVector = vector<int>;
using Iterator = IntVector::const_iterator;

TEST_CASE("sort", "[sort]")
{
  IntVector ints{ 0, 2, 1, 3 };
  sort(ints);
  for (size_t idx = 0; idx < ints.size(); ++idx) {
    REQUIRE(idx == ints[idx]);
  }
}

TEST_CASE("remove", "[remove]")
{
  IntVector ints{ 0, 1, 2 };
  const Iterator end = remove(ints, 1);
  // NOTE: result is a shorter range, but size is unchanged
  REQUIRE(3 == ints.size());
  Iterator itr = ints.begin();
  REQUIRE(0 == *(itr++));
  REQUIRE(2 == *(itr++));
  REQUIRE(end == itr);
}

TEST_CASE("remove_if", "[remove_if]")
{
  IntVector ints{ 0, 1, 2, 3, 4 };
  const Iterator end = remove_if(ints, [](auto val) {
      return 0 == (val % 2);
    });
  // NOTE: result is a shorter range, but size is unchanged
  REQUIRE(5 == ints.size());
  Iterator itr = ints.begin();
  REQUIRE(1 == *(itr++));
  REQUIRE(3 == *(itr++));
  REQUIRE(end == itr);
}
