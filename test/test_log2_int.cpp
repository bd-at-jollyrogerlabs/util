/** -*- mode: c++-mode -*-
 *
 * test_log2_int.cpp
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

#include <cassert>
#include <cmath>
#include "log2int"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace jrl;

TEST_CASE("results of log2int", "[log2int]")
{
  SECTION("a few easy cases") {
    for (unsigned result = 1; result <= 10; ++result) {
      INFO("checking arguments that should evaluate to " << result);
      for (unsigned arg = (1 << result);
	   arg < static_cast<unsigned>((1 << (result + 1)));
	   ++arg) {
	INFO("checking argument " << arg);
	REQUIRE(log2int(arg) == result);
      }
    }
  }

  SECTION("comparison to floor(log2())") {
    for (unsigned arg = 1; arg <= 1024; ++arg) {
      INFO("checking argument " << arg << " (" << static_cast<unsigned>(floor(log2(arg))) << ")");
      REQUIRE(log2int(arg) == static_cast<unsigned>(floor(log2(arg))));
    }
  }
}

#define STATIC_TEST(arg, result)			\
  do {							\
    REQUIRE(Log2IntStatic<arg>::value == result);	\
    REQUIRE(Log2IntStatic<arg>::value ==		\
	    static_cast<unsigned>(floor(log2(arg))));	\
  } while(0)

TEST_CASE("results of Log2IntStatic", "[Log2IntStatic]")
{
  SECTION("a few easy cases") {
    STATIC_TEST(1, 0);

    STATIC_TEST(2, 1);
    STATIC_TEST(3, 1);

    STATIC_TEST(4, 2);
    STATIC_TEST(5, 2);
    STATIC_TEST(6, 2);
    STATIC_TEST(7, 2);

    STATIC_TEST(8, 3);
    STATIC_TEST(9, 3);
    STATIC_TEST(10, 3);
    STATIC_TEST(11, 3);
    STATIC_TEST(12, 3);
    STATIC_TEST(13, 3);
    STATIC_TEST(14, 3);
    STATIC_TEST(15, 3);

    STATIC_TEST(16, 4);
  }
}
