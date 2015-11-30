/** -*- mode: c++ -*-
 *
 * testunitsafe.cpp
 *
 * Test cases for unitsafe code.
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

#include "unitsafe"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

MAKE_UNITSAFE(Foo, unsigned);
MAKE_UNITSAFE(Bar, unsigned);

TEST_CASE("Basic operations", "[basic]")
{
  const Foo foo0(0);
  const Foo foo1(1);

  REQUIRE(foo0 == foo0);
  REQUIRE(foo0 <= foo0);
  REQUIRE(foo0 <= foo1);
  REQUIRE(foo0 < foo1);
  REQUIRE(foo1 > foo0);
  REQUIRE(foo1 >= foo0);

  const unsigned two = 2;
  REQUIRE(two > static_cast<unsigned>(foo1));
  REQUIRE(two >= static_cast<unsigned>(foo1));

  const Foo foo1a = foo0 + foo1;
  const Foo foo1b = foo1 + foo0;
  const Foo foo2 = foo1 + foo1;

  REQUIRE(foo1a >= foo1);
  REQUIRE(foo1b >= foo1);
  REQUIRE(foo1a <= foo1);
  REQUIRE(foo1b <= foo1);
  REQUIRE(foo1a == foo1);
  REQUIRE(foo1b == foo1);
  REQUIRE(foo2 >= foo1);
  REQUIRE(foo2 > foo1);

  const Bar bar0(0);
  const Bar bar1(1);
  Bar bar = bar0;
  REQUIRE(bar0 == bar);
  REQUIRE(bar == bar0);
  REQUIRE(bar0 <= bar);
  REQUIRE(bar <= bar0);
  bar = bar1;
  REQUIRE(bar1 == bar);
  REQUIRE(bar == bar1);
  REQUIRE(bar1 >= bar);
  REQUIRE(bar >= bar1);
  REQUIRE(bar > bar0);
  REQUIRE(bar0 < bar);
}
