/** -*- mode: c++-mode -*-
 *
 * testlog2int.cpp 
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

#include <cassert>
#include <cmath>
#include "log2int"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace jrl::util;

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
