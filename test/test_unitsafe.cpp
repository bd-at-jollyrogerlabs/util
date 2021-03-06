/** -*- mode: c++ -*-
 *
 * test_unitsafe.cpp
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

#include "unitsafe_math"

#include <string>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;


// ******************** simple unitsafe tests ********************

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

TEST_CASE("Streaming output from value", "[ostream]")
{
  const Foo foo0(0);
  ostringstream ss;
  ss << foo0;
  REQUIRE(ss.str() == "0");
}

TEST_CASE("Streaming output from reference", "[ostream]")
{
  const Foo foo0(0);
  const Foo &foo0Ref(foo0);
  ostringstream ss;
  ss << foo0Ref;
  REQUIRE(ss.str() == "0");
}

TEST_CASE("square root, as a proxy for all math functions", "[sqrt]")
{
  // NOTE: this test stands in for all math functions as a means of
  // testing the mechanism in unitsafe_math
  const Foo foo4(4);
  const Foo foo2(sqrt(foo4));
  REQUIRE(2.0 == base_cast(foo2));
}


// ******************** contained unitsafe tests ********************

class DerivedFromEnclosing;

class Enclosing
{
  MAKE_PUBLIC_UNITSAFE(Public, int);

  MAKE_CONTAINED_UNITSAFE(Protected, int, protected);

public:
  Enclosing() = default;

  Enclosing(const int val) : val_(val) {}

protected:
  friend class DerivedFromEnclosing;

  Protected val_;
};

class DerivedFromEnclosing : public Enclosing
{
public:
  using Exported = Enclosing::Protected;

  void setVal(const Exported val)
  {
    Enclosing::val_ = val;
  }

  Exported getVal() const
  {
    return Enclosing::val_;
  }

  Exported processEnclosing(const Enclosing &val)
  {
    return Enclosing::val_ + val.val_;
  }
};

TEST_CASE("basic contained operations", "[contained]")
{
  Enclosing encl(1);
  DerivedFromEnclosing deriv;
  deriv.setVal(20010910);

  DerivedFromEnclosing mixed;
  mixed.setVal(deriv.processEnclosing(encl));
  REQUIRE(20010911 == base_cast(mixed.getVal()));
}

TEST_CASE("publicly accessible type", "[contained]")
{
  using Public = Enclosing::Public;
  const Public val1(20010910);
  const Public val2 = 1;
  const auto result = val1 + val2;
  REQUIRE(20010911 == base_cast(result));
}


// ******************** complex unitsafe tests ********************

MAKE_COMPLEX_UNITSAFE(Str, std::string);

TEST_CASE("complex unitsafe type for std::string", "[complex]")
{
  const Str str1(string("foo"));
  {
    ostringstream ss;
    ss << str1;
    REQUIRE(ss.str() == "foo");
  }
  const Str str2(string("bar"));
  REQUIRE(str1 != str2);
  const auto str3 = str1 + str2;
  {
    ostringstream ss;
    ss << str3;
    REQUIRE(ss.str() == "foobar");
  }
}
