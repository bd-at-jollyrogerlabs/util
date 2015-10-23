/** -*- mode: c++-mode -*-
 *
 * testjrlutil.cpp 
 *
 * Copyright (C) 2014 Brian Davis
 * All Rights Reserved
 * 
 * Limited copying permission is given solely
 * for self-educational purpose.
 * 
 * Author: Brian Davis <bd@jollyrogerlabs.com>
 * 
 */

#include <cstring>
#include <ctime>
#include <bitset>
#include "jrl_util"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace jrl::util;


TEST_CASE("test of BitMaskStatic", "BitMaskStatic")
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
