/** -*- mode: c++ -*-
 *
 * test_hash_map.cpp
 *
 * Test cases for hash map with variadic policies.
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

#include <chrono>
#include <map>
#include <string>
#include <unordered_map>

#include "hash_map"
#include "hash_test_support.h"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

using IntIntHashMap = hash_map<int, int>;
using StringStringHashMap = hash_map<string, string>;
using IntStringHashMap = hash_map<int, string>;
using StringIntHashMap = hash_map<string, int>;


// ********** tests of basic functionality **********

TEST_CASE("at() throws exception on missing entry", "[hash_map]")
{
  StringStringHashMap mp;
  try {
    auto entry = mp.at("nowhere");
  }
  catch (out_of_range &exc) {
    REQUIRE(true);
  }
  catch (exception &exc) {
    REQUIRE(false);
  }
  catch (...) {
    REQUIRE(false);
  }
}

/**
 * check that an empty map object works correctly
 */
template<typename MapContainerType>
void
empty_map_tests()
{
  MapContainerType mp;
  REQUIRE(mp.empty());
  REQUIRE(0 == mp.size());
  REQUIRE(end(mp) == mp.begin());
  REQUIRE(mp.cend() == mp.cbegin());
}

TEST_CASE("empty (integer, integer) map test", "[hash_map]")
{
  empty_map_tests<IntIntHashMap>();
}

TEST_CASE("empty (string, string) map test", "[hash_map]")
{
  empty_map_tests<StringStringHashMap>();
}

TEST_CASE("empty (integer, string) map test", "[hash_map]")
{
  empty_map_tests<IntStringHashMap>();
}

TEST_CASE("empty (string, integer) map test", "[hash_map]")
{
  empty_map_tests<StringIntHashMap>();
}

TEST_CASE("insert/lookup/erase", "[hash_map]")
{
  const int key = 20010911;
  const int value = 19700101;
  IntIntHashMap mp;
  auto result = mp.insert(make_pair(key, value));
  REQUIRE(result.second);
  REQUIRE(end(mp) != result.first);
  REQUIRE(key == result.first->first);
  REQUIRE(value == result.first->second);
  REQUIRE(1 == mp.count(key));
  {
    auto ref = mp[key];
    REQUIRE(value == ref);
  }
  {
    auto ref = mp.at(key);
    REQUIRE(value == ref);
  }
  {
    const IntIntHashMap &c_mp(mp);
    IntIntHashMap::const_iterator entry = c_mp.find(key);
    REQUIRE(end(c_mp) != entry);
    REQUIRE(key == entry->first);
    REQUIRE(value == entry->second);
  }
  REQUIRE(1 == mp.erase(key));
  REQUIRE(0 == mp.count(key));
}

// support constants for probabalistic test
namespace
{

const double EXPECTED_TIME_BETWEEN_ORDER_ARRIVAL_EVENTS(40.0);
const double EXPECTED_ORDER_LIFETIME(40.0);

const auto SEED = chrono::system_clock::now().time_since_epoch().count();

} // unnamed namespace

TEST_CASE("probabalistic test", "[hash_map]")
{

  auto events = generateEvents(SEED, 1 << 14,
			       EXPECTED_TIME_BETWEEN_ORDER_ARRIVAL_EVENTS,
			       EXPECTED_ORDER_LIFETIME);

  using StdOrders = unordered_map<OrderIdType, OrderState, Hasher>;
  using HashMapOrders =
    hash_map<OrderIdType, OrderState,
		  free_function_hash_policy<UnitSafeTrivialHasher<OrderIdType>>,
		  power_of_two_length_table_policy>;

  // perform identical operations on both map types
  StdOrders stdOrders;
  processSequencedOrderEvents(stdOrders, events);
  HashMapOrders hashMapOrders;
  processSequencedOrderEvents(hashMapOrders, events);

  // resulting maps should be the same size
  REQUIRE(stdOrders.size() == hashMapOrders.size());

  // all keys in hashMapOrders should also be in stdOrders
  for (auto entry : hashMapOrders) {
    const auto key = entry.first;
    REQUIRE(1 == stdOrders.count(key));
    stdOrders.erase(key);
  }

  // stdOrders should contain no other keys
  REQUIRE(0 == stdOrders.size());
}
