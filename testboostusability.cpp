/** -*- mode: c++ -*-
 *
 * testboostusability.cpp 
 *
 * Copyright (C) 2015 Brian Davis
 * All Rights Reserved
 * 
 * \TODO: new license
 *
 * Limited copying permission is given solely
 * for self-educational purpose.
 * 
 * Author: Brian Davis <bd@jollyrogerlabs.com>
 * 
 */

#include <iostream>
#include <functional>

#include <boost/range.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include "boost_usability"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

using IVec = vector<int>;

TEST_CASE("iterate over 2 streams", "[for_each_streams]")
{
  IVec v1{ 0, 1, 2 };
  IVec v2{ 0, 1, 2 };
  int counter = 0;
  for_each_streams(v1, v2,
		   [&](const int arg1, const int arg2)
		   {
		     REQUIRE(arg1 == arg2);
		     REQUIRE(arg1 == counter);
		     ++counter;
		   });
}

TEST_CASE("transform 2 streams", "[transform_streams]")
{
  IVec v1{ 1, 2, 3 };
  IVec v2{ 4, 5, 6 };
  IVec result;
  transform_streams(v1, v2, back_inserter(result),
		    [](const int arg1, const int arg2)
		    { return arg1 + arg2; });
  REQUIRE(5 == result[0]);
  REQUIRE(7 == result[1]);
  REQUIRE(9 == result[2]);
}

TEST_CASE("transform 3 streams", "[transform_streams]")
{
  IVec v1{ 1, 2, 3 };
  IVec v2{ 4, 5, 6 };
  IVec v3{ 7, 8, 9 };
  IVec result;
  transform_streams(v1, v2, v3, back_inserter(result),
		    [](int arg1, int arg2, int arg3)
		    { return arg1 + arg2 + arg3; });
  REQUIRE(12 == result[0]);
  REQUIRE(15 == result[1]);
  REQUIRE(18 == result[2]);
}

// \TODO see commented out find_first_match in boost_usability
// TEST_CASE("find_first_match", "[find_first_match]")
// {
//   IVec v1{ 1, 2, 3 };
//   IVec v2{ 1, 2, 3 };
//   IVec v3{ 0, 1, 2 };
//   IVec v4{ 0, 2, 3 };
//   auto noMatch =
//     find_first_match(v1, v2,
// 		     [](const int arg1, const int arg2)
// 		     { return arg1 != arg2; });
//   REQUIRE(v1.cend() == noMatch);
// }
