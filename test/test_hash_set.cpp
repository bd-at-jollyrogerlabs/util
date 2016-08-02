/** -*- mode: c++ -*-
 *
 * test_hash_set.cpp
 *
 * Test cases for hash set with variadic policies.
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

#include <iostream> // @todo
#include <string>

#include "hash_set"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

using IntHashSet = hash_set<int>;

using StrHashSet = hash_set<string>;

template<typename SetType>
void
empty_set_tests()
{
  SetType st;
  REQUIRE(st.empty());
  REQUIRE(0 == st.size());
  REQUIRE(st.end() == st.begin());
  REQUIRE(st.cend() == st.cbegin());
}

TEST_CASE("empty set tests", "[hash_set]")
{
  empty_set_tests<IntHashSet>();
  empty_set_tests<StrHashSet>();
}

template <typename SetType>
void
insert_find_count_tests(const typename SetType::value_type &val)
{
  SetType st;
  const auto result = st.insert(val);
  REQUIRE(result.second);
  const auto entry = st.find(val);
  REQUIRE(st.end() != entry);
  REQUIRE(!st.empty());
  REQUIRE(1 == st.size());
  REQUIRE(st.count(val));
}

TEST_CASE("insert and find tests", "[hash_set]")
{
  insert_find_count_tests<IntHashSet>(0);
  insert_find_count_tests<StrHashSet>("zero");
}

/**
 * Simple class for testing key extraction.
 */
struct TestType
{
  TestType(const TestType &src)
  {
    ++copy_construct_count;
    i_val = src.i_val;
    s_val = src.s_val;
  }

  TestType &
  operator=(const TestType &src)
  {
    ++copy_assign_count;
    if (&src != this) {
      i_val = src.i_val;
      s_val = src.s_val;
    }
    return *this;
  }

  TestType(TestType &&src)
  {
    ++move_construct_count;
    i_val = src.i_val;
    s_val = src.s_val;
  }

  TestType &
  operator=(TestType &&src)
  {
    ++move_assign_count;
    if (&src != this) {
      i_val = src.i_val;
      s_val = src.s_val;
    }
    return *this;
  }

  static void
  clear()
  {
    copy_construct_count = 0;
    copy_assign_count = 0;
    move_construct_count = 0;
    move_assign_count = 0;
  }

  static unsigned copy_construct_count;
  static unsigned copy_assign_count;
  static unsigned move_construct_count;
  static unsigned move_assign_count;

  int i_val;
  string s_val;
};

unsigned TestType::copy_construct_count = 0;
unsigned TestType::copy_assign_count = 0;
unsigned TestType::move_construct_count = 0;
unsigned TestType::move_assign_count = 0;

struct IntKey : public custom_key_extractor_policy_tag
{
  using key_type = int;
  static const key_type &
  get_key(const TestType &arg) noexcept
  {
    return arg.i_val;
  }
};

struct StrKey : public custom_key_extractor_policy_tag
{
  using key_type = string;
  static const key_type &
  get_key(const TestType &arg) noexcept
  {
    return arg.s_val;
  }
};

// @todo: OLD CODE TEST_CASE("hash set insert and find", "[hash_set]")
// @todo: OLD CODE {
// @todo: OLD CODE   using nsmespace jrl;
// @todo: OLD CODE   TestType::clear();
// @todo: OLD CODE }

// @todo test that swap produces the correct results for hasher_ and table_style_

// @todo test various loops that check bucket_size and load_factor
// while adding elements

// @todo build and test custom load factor policy with a threshold of 1.0
