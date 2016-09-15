/** -*- mode: c++-mode -*-
 *
 * test_jrl_util.cpp
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

#include <cstring>
#include <ctime>
#include <bitset>
#include <sstream>
#include "jrl_util"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

#define DO_TEST(Type, construction, result)	\
  do {						\
    Type testValue = (construction);		\
    ostringstream ss;				\
    ss << as_binary(testValue);			\
    REQUIRE(result == ss.str());		\
  } while (0)

TEST_CASE("test of as_binary", "[as_binary]")
{
  // 8 bit value
  DO_TEST(uint8_t, (1 << 3) | (1 << 1), "00001010");
  // 32 bit value
  DO_TEST(uint32_t, (1 << 3) | (1 << 1), "00000000000000000000000000001010");
}

#undef DO_TEST

TEST_CASE("test of BitMaskStatic", "[BitMaskStatic]")
{
#define BITS(arg) ((1 << (arg)) - 1)

  SECTION("all 32 bit cases") {
    REQUIRE(BitMaskStatic<1>::value == 1);
    REQUIRE(BitMaskStatic<2>::value == BITS(2));
    REQUIRE(BitMaskStatic<3>::value == BITS(3));
    REQUIRE(BitMaskStatic<4>::value == BITS(4));
    REQUIRE(BitMaskStatic<5>::value == BITS(5));
    REQUIRE(BitMaskStatic<6>::value == BITS(6));
    REQUIRE(BitMaskStatic<7>::value == BITS(7));
    REQUIRE(BitMaskStatic<8>::value == BITS(8));
    REQUIRE(BitMaskStatic<9>::value == BITS(9));
    REQUIRE(BitMaskStatic<10>::value == BITS(10));
    REQUIRE(BitMaskStatic<11>::value == BITS(11));
    REQUIRE(BitMaskStatic<12>::value == BITS(12));
    REQUIRE(BitMaskStatic<13>::value == BITS(13));
    REQUIRE(BitMaskStatic<14>::value == BITS(14));
    REQUIRE(BitMaskStatic<15>::value == BITS(15));
    REQUIRE(BitMaskStatic<16>::value == BITS(16));
    REQUIRE(BitMaskStatic<17>::value == BITS(17));
    REQUIRE(BitMaskStatic<18>::value == BITS(18));
    REQUIRE(BitMaskStatic<19>::value == BITS(19));
    REQUIRE(BitMaskStatic<20>::value == BITS(20));
    REQUIRE(BitMaskStatic<21>::value == BITS(21));
    REQUIRE(BitMaskStatic<22>::value == BITS(22));
    REQUIRE(BitMaskStatic<23>::value == BITS(23));
    REQUIRE(BitMaskStatic<24>::value == BITS(24));
    REQUIRE(BitMaskStatic<25>::value == BITS(25));
    REQUIRE(BitMaskStatic<26>::value == BITS(26));
    REQUIRE(BitMaskStatic<27>::value == BITS(27));
    REQUIRE(BitMaskStatic<28>::value == BITS(28));
    REQUIRE(BitMaskStatic<29>::value == BITS(29));
    REQUIRE(BitMaskStatic<30>::value == BITS(30));
    REQUIRE(BitMaskStatic<31>::value ==
	    static_cast<unsigned>(static_cast<uint64_t>(1 << 31) - 1));
  }

#undef BITS

#define BITS(arg1, arg2) (((1 << (arg1)) - 1) << arg2)

  SECTION("all 32 bit cases") {
    REQUIRE((BitMaskStatic<1, 1>::value == 2));
    REQUIRE((BitMaskStatic<2, 1>::value == BITS(2, 1)));
    REQUIRE((BitMaskStatic<3, 1>::value == BITS(3, 1)));
    REQUIRE((BitMaskStatic<4, 1>::value == BITS(4, 1)));
    REQUIRE((BitMaskStatic<5, 1>::value == BITS(5, 1)));
    REQUIRE((BitMaskStatic<6, 1>::value == BITS(6, 1)));
    REQUIRE((BitMaskStatic<7, 1>::value == BITS(7, 1)));
    REQUIRE((BitMaskStatic<8, 1>::value == BITS(8, 1)));
    REQUIRE((BitMaskStatic<9, 1>::value == BITS(9, 1)));
    REQUIRE((BitMaskStatic<10, 1>::value == BITS(10, 1)));
    REQUIRE((BitMaskStatic<11, 1>::value == BITS(11, 1)));
    REQUIRE((BitMaskStatic<12, 1>::value == BITS(12, 1)));
    REQUIRE((BitMaskStatic<13, 1>::value == BITS(13, 1)));
    REQUIRE((BitMaskStatic<14, 1>::value == BITS(14, 1)));
    REQUIRE((BitMaskStatic<15, 1>::value == BITS(15, 1)));
    REQUIRE((BitMaskStatic<16, 1>::value == BITS(16, 1)));
    REQUIRE((BitMaskStatic<17, 1>::value == BITS(17, 1)));
    REQUIRE((BitMaskStatic<18, 1>::value == BITS(18, 1)));
    REQUIRE((BitMaskStatic<19, 1>::value == BITS(19, 1)));
    REQUIRE((BitMaskStatic<20, 1>::value == BITS(20, 1)));
    REQUIRE((BitMaskStatic<21, 1>::value == BITS(21, 1)));
    REQUIRE((BitMaskStatic<22, 1>::value == BITS(22, 1)));
    REQUIRE((BitMaskStatic<23, 1>::value == BITS(23, 1)));
    REQUIRE((BitMaskStatic<24, 1>::value == BITS(24, 1)));
    REQUIRE((BitMaskStatic<25, 1>::value == BITS(25, 1)));
    REQUIRE((BitMaskStatic<26, 1>::value == BITS(26, 1)));
    REQUIRE((BitMaskStatic<27, 1>::value == BITS(27, 1)));
    REQUIRE((BitMaskStatic<28, 1>::value == BITS(28, 1)));
    REQUIRE((BitMaskStatic<29, 1>::value == BITS(29, 1)));
    REQUIRE((BitMaskStatic<30, 1>::value == BITS(30, 1)));
  }

#undef BITS
}

TEST_CASE("test of is_power_of_2", "[is_power_of_2]")
{
  // all 16 bit cases seems reasonable
  using ArgType = uint16_t;
  // start matcher at 2^1
  ArgType matcher = 1 << 1;

  ArgType counter = 0;
  const ArgType max = ~counter;

  // handle 0 separately
  REQUIRE(is_power_of_2(counter));
  ++counter;

  for (; counter < max; ++counter) {
    if (is_power_of_2(counter)) {
      REQUIRE(matcher == counter);
      // move to the next power of 2 value
      matcher <<= 1;
    }
    else {
      REQUIRE(matcher != counter);
    }
  }
}
