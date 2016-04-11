/** -*- mode: c++ -*-
 *
 * test_stl_usability.cpp
 *
 * Test cases for stl_usability code.
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

#include <cstring>

#include <vector>
#include <functional>
#include <sstream>

#include "stl_usability/algorithm_usability"
#include "stl_usability/ostream_innerator"
#include "jrl_algorithm"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl::util::stl_usability;

using IntVector = vector<int>;
using Iterator = IntVector::iterator;
using ConstIterator = IntVector::const_iterator;
using IntStringMap = map<int, const char *>;

TEST_CASE("sort", "[sort]")
{
  IntVector ints{ 0, 2, 1, 3 };
  sort(ints);
  for (size_t idx = 0; idx < ints.size(); ++idx) {
    REQUIRE(idx == ints[idx]);
  }
}

TEST_CASE("find", "[find]")
{
  IntVector ints{ 0, 1, 2, 3 };
  {
    Iterator entry = find(ints, 2);
    REQUIRE(ints.end() != entry);
    REQUIRE(2 == *entry);
  }
  {
    Iterator entry = find(ints, 0);
    REQUIRE(ints.end() != entry);
    REQUIRE(0 == *entry);
    REQUIRE(ints.begin() == entry);
  }
  {
    Iterator entry = find(ints, 3);
    REQUIRE(ints.end() != entry);
    REQUIRE(3 == *entry);
  }
  {
    Iterator entry = find(ints, 4);
    REQUIRE(ints.end() == entry);
  }
}

TEST_CASE("find on const", "[find]")
{
  const IntVector ints{ 4, 5, 6, 7 };
  {
    ConstIterator entry = find(ints, 6);
    REQUIRE(ints.end() != entry);
    REQUIRE(6 == *entry);
  }
  {
    ConstIterator entry = find(ints, 4);
    REQUIRE(ints.end() != entry);
    REQUIRE(4 == *entry);
    REQUIRE(ints.begin() == entry);
  }
  {
    ConstIterator entry = find(ints, 7);
    REQUIRE(ints.end() != entry);
    REQUIRE(7 == *entry);
  }
  {
    ConstIterator entry = find(ints, 8);
    REQUIRE(ints.end() == entry);
  }
}

TEST_CASE("remove", "[remove]")
{
  IntVector ints{ 0, 1, 2 };
  const ConstIterator end = remove(ints, 1);
  // NOTE: result is a shorter range, but size is unchanged
  REQUIRE(3 == ints.size());
  ConstIterator itr = ints.begin();
  REQUIRE(0 == *(itr++));
  REQUIRE(2 == *(itr++));
  REQUIRE(end == itr);
}

TEST_CASE("remove_if", "[remove_if]")
{
  IntVector ints{ 0, 1, 2, 3, 4 };
  const ConstIterator end = remove_if(ints, [](auto val) {
      return 0 == (val % 2);
    });
  // NOTE: result is a shorter range, but size is unchanged
  REQUIRE(5 == ints.size());
  ConstIterator itr = ints.begin();
  REQUIRE(1 == *(itr++));
  REQUIRE(3 == *(itr++));
  REQUIRE(end == itr);
}

TEST_CASE("is_present", "[is_present]")
{
  IntStringMap intsMap{
    IntStringMap::value_type(0, "zero"),
    IntStringMap::value_type(1, "one"),
    IntStringMap::value_type(2, "two"),
    IntStringMap::value_type(3, "three"),
    IntStringMap::value_type(4, "four")
  };
  REQUIRE(is_present(intsMap, 0));
  REQUIRE(is_present(intsMap, 4));
  REQUIRE(!is_present(intsMap, 5));
}

/**
 * Simple struct whose instances record whether they have been moved
 * via either move constructor or move assignment operator.
 */
struct Moveable
{
  Moveable() = default;

  Moveable(Moveable &&src)
    : constructorMoved_(false)
  {
    src.constructorMoved_ = true;
  }

  Moveable&
  operator=(Moveable &&rhs)
  {
    if (&rhs != this) {
      assignmentMoved_ = false;
      rhs.assignmentMoved_ = true;
    }
    return *this;
  }

  bool constructorMoved_ = false;
  bool assignmentMoved_ = false;
};

TEST_CASE("move_all", "[move_all]")
{
  using jrl::util::stl_usability::back_emplacer;
  using jrl::util::algorithm::move_all;
  {
    vector<Moveable> src(2);
    REQUIRE(2 == src.size());
    for_each(src, [](const auto &entry) {
	REQUIRE(!entry.constructorMoved_);
	REQUIRE(!entry.assignmentMoved_);
      });
    vector<Moveable> tgt;
    REQUIRE(tgt.empty());
    move_all(src, back_emplacer(tgt));
    REQUIRE(2 == src.size());
    REQUIRE(2 == tgt.size());
    for_each(src, [](const auto &entry) {
	REQUIRE(entry.constructorMoved_);
	REQUIRE(!entry.assignmentMoved_);
      });
    for_each(tgt, [](const auto &entry) {
	REQUIRE(!entry.constructorMoved_);
	REQUIRE(!entry.assignmentMoved_);
      });
  }
  {
    vector<Moveable> src(2);
    REQUIRE(2 == src.size());
    for_each(src, [](const auto &entry) {
	REQUIRE(!entry.constructorMoved_);
	REQUIRE(!entry.assignmentMoved_);
      });
    vector<Moveable> tgt(2);
    for_each(tgt, [](const auto &entry) {
	REQUIRE(!entry.constructorMoved_);
	REQUIRE(!entry.assignmentMoved_);
      });
    REQUIRE(2 == tgt.size());
    move_all(src, begin(tgt));
    REQUIRE(2 == src.size());
    REQUIRE(2 == tgt.size());
    for_each(src, [](const auto &entry) {
	REQUIRE(!entry.constructorMoved_);
	REQUIRE(entry.assignmentMoved_);
      });
    for_each(tgt, [](const auto &entry) {
	REQUIRE(!entry.constructorMoved_);
	REQUIRE(!entry.assignmentMoved_);
      });
  }
}

TEST_CASE("ostream_innerator in 'innerate' mode", "[ostream_innerator]")
{
  ostringstream ss;
  ostream_innerator<int> output(ss, ",");

  // basic functionality
  vector<int> iVec = { 0, 1, 2, 3 };
  copy(iVec, output);
  REQUIRE("0,1,2,3" == ss.str());

  // automatic reset
  copy(iVec, output);
  REQUIRE("0,1,2,30,1,2,3" == ss.str());
  // manual reset
  for (auto entry : iVec) {
    output = entry;
    // NOTE: no need to increment output object here since the
    // increment operators are noops
  }
  REQUIRE("0,1,2,30,1,2,30,1,2,3" == ss.str());
}


TEST_CASE("ostream_innerator *NOT* in 'innerate' mode", "[ostream_innerator]")
{
  ostringstream ss;
  ostream_innerator<int> output(ss, ",", false /* turn off 'innerate' mode */);
  // basic functionality
  vector<int> iVec = { 0, 1, 2, 3 };
  copy(iVec, output);
  REQUIRE("0,1,2,3," == ss.str());
  copy(iVec, output);
  REQUIRE("0,1,2,3,0,1,2,3," == ss.str());
}
