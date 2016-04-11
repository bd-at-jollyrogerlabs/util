/** -*- mode: c++-mode -*-
 *
 * test_jrl_algorithm.cpp 
 *
 * Copyright (C) 2014 Brian Davis
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

#include <algorithm>
#include <vector>
#include <functional>
#include <bitset> // @todo remove when jrl_util no longer requires bitset

#include "jrl_util"
#include "jrl_algorithm"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl::util;
using namespace jrl::util::algorithm;

TEST_CASE("test of nondestructive_insert", "[nondestructive_insert]")
{
  using IntVector = vector<int>;
  using ReturnType = bool_return<IntVector::size_type>;

  // unsuccessful insert using default null_extender
  IntVector vec{ 1, 2, 3 };
  ReturnType result = nondestructive_insert(vec, 1);
  REQUIRE(!get<ReturnType::RESULT>(result));

  // successful insert using default null_extender
  result = nondestructive_insert(vec, 4);
  REQUIRE(get<ReturnType::RESULT>(result));
  REQUIRE(3 == get<ReturnType::VALUE>(result));

  bool extended = false;

  // unsuccessful insert, extender not called
  result = nondestructive_insert(vec, 4, [&](auto& c, const auto sz) {
      extended = true;
    });
  REQUIRE(!get<ReturnType::RESULT>(result));
  REQUIRE(!extended);

  // successful insert, extender called
  result = nondestructive_insert(vec, 5, [&](auto& c, const auto sz) {
      extended = true;
    });
  REQUIRE(get<ReturnType::RESULT>(result));
  REQUIRE(4 == get<ReturnType::VALUE>(result));
  REQUIRE(extended);
}
