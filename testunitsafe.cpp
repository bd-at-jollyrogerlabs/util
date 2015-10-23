/** -*- mode: c++ -*-
 *
 * testunitsafe.cpp 
 *
 * Copyright (C) 2015 Brian Davis
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
#include <cstring>
#include <ostream>
#include "unitsafe"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

static const double TEST_EPSILON = 0.00001;

static bool
isApproxEq(const double lhs, const double rhs)
{
  return fabs(lhs - rhs) < TEST_EPSILON;
}

TEST_CASE("Basic operations on distances", "[meters]")
{
  Meters m(10.0);
  Millimeters mm(50.0);
  Kilometers km(2.0);
  Inches in(15.0);
  Feet ft(20.0);

  SECTION("construction + output") {
    REQUIRE(10.0 == double(m));
    REQUIRE(!strcmp("meters", Meters::getName()));
    REQUIRE(!strcmp("m", Meters::getSymbol()));
    // cout << m << endl;

    REQUIRE(50.0 == double(mm));
    REQUIRE(!strcmp("millimeters", Millimeters::getName()));
    REQUIRE(!strcmp("mm", Millimeters::getSymbol()));
    // cout << mm << endl;

    REQUIRE(2.0 == double(km));
    REQUIRE(!strcmp("kilometers", Kilometers::getName()));
    REQUIRE(!strcmp("km", Kilometers::getSymbol()));
    // cout << km << endl;

    REQUIRE(15.0 == double(in));
    REQUIRE(!strcmp("inches", Inches::getName()));
    REQUIRE(!strcmp("\"", Inches::getSymbol()));
    // cout << in << endl;

    REQUIRE(20.0 == double(ft));
    REQUIRE(!strcmp("feet", Feet::getName()));
    REQUIRE(!strcmp("\'", Feet::getSymbol()));
    // cout << ft << endl;
  }

  SECTION("conversion") {
    Meters m1 = mm.get<Meters>();
    REQUIRE(isApproxEq(0.05, double(m1)));
    // cout << mm << " --> " << m1 << endl;
    Inches i1 = mm.get<Inches>();
    REQUIRE(isApproxEq(1.9685, double(i1)));
    // cout << mm << " --> " << i1 << endl;
    REQUIRE(isApproxEq(double((Inches(1)).get<Millimeters>()), 25.4));
  }
}
