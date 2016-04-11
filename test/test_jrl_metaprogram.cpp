/** -*- mode: c++ -*-
 *
 * test_jrl_metaprogram.cpp
 *
 * Test cases for jrl_metaprogram code.
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

#include <iostream>
#include <vector>
#include <type_traits>
#include <typeinfo>

#include "jrl_metaprogram"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;

struct Inner {};

using IntVec = vector<int>;

template<bool IsConst>
struct Outer
{
  Outer() : inner() {}
  using EffectiveInner = SELECTIVE_CONST(IsConst, Inner);
  using ArrayType = SELECTIVE_CONST(IsConst, IntVec);

  EffectiveInner inner;
  ArrayType array;
};

using ConstInner = Outer<true>;
using NonConstInner = Outer<false>;

TEST_CASE("SELECTIVE_CONST", "[SELECTIVE_CONST]")
{
  ConstInner ci;
  NonConstInner nci;
  REQUIRE(is_const<decltype(ci.inner)>::value);
  REQUIRE(!is_const<decltype(nci.inner)>::value);
  REQUIRE(is_const<decltype(ci.array)>::value);
  REQUIRE(!is_const<decltype(nci.array)>::value);
}
