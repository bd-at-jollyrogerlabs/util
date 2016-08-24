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

template<typename SetType>
void
insert_find_count_tests(const typename SetType::value_type &val1,
			const typename SetType::value_type &val2)
{
  SetType st;
  const auto result = st.insert(val1);
  REQUIRE(result.second);
  {
    const auto entry = st.find(val1);
    REQUIRE(st.end() != entry);
    REQUIRE(!st.empty());
    REQUIRE(1 == st.size());
    REQUIRE(st.count(val1));
  }
  {
    const auto entry = st.find(val2);
    REQUIRE(st.end() == entry);
    REQUIRE(0 == st.count(val2));
  }
}

TEST_CASE("insert and find tests", "[hash_set]")
{
  insert_find_count_tests<IntHashSet>(0, 1);
  insert_find_count_tests<StrHashSet>("zero", "one");
}

template<typename SetType>
void
emplace_find_count_tests(const typename SetType::value_type &val1,
			 const typename SetType::value_type &val2)
{
  typename SetType::value_type lVal1 = val1;
  SetType st;
  const auto result = st.emplace(std::move(lVal1));
  REQUIRE(result.second);
  {
    const auto entry = st.find(val1);
    REQUIRE(st.end() != entry);
    REQUIRE(!st.empty());
    REQUIRE(1 == st.size());
    REQUIRE(st.count(val1));
  }
  {
    const auto entry = st.find(val2);
    REQUIRE(st.end() == entry);
    REQUIRE(0 == st.count(val2));
  }
}

TEST_CASE("emplace and find tests", "[hash_set]")
{
  emplace_find_count_tests<IntHashSet>(0, 1);
  emplace_find_count_tests<StrHashSet>("zero", "one");
}

template<typename ContainedType>
void
iteration_tests(const vector<ContainedType> &vec)
{
  hash_set<ContainedType> st;
  for (const auto &entry : vec) {
    const auto result = st.insert(entry);
    REQUIRE(result.second);
  }
  REQUIRE(vec.size() == st.size());
  for (const auto &entry : st) {
    REQUIRE(vec.end() != find(begin(vec), end(vec), entry));
  }
}

using IntVec = vector<int>;
using StrVec = vector<string>;

TEST_CASE("iteration tests", "[hash_set]")
{
  iteration_tests(IntVec{0, 1, 2, 3, 4});
  iteration_tests(StrVec{"zero", "one", "two", "three", "four"});
}

/**
 * Simple hash calculation function for strings used to test
 * free_function_hash_policy.
 */
size_t
test_string_hash_function(const string &key) noexcept
{
  THROW_ON_FAIL(key.size() <= sizeof(size_t), runtime_error,
		"weird_string_hash_function received a key [" << key
		<< "] that was larger than the limit of " << sizeof(size_t));
  size_t result = 0;
  for (size_t idx = 0; (idx < key.size()) && (idx <= sizeof(result)); ++idx) {
    result += static_cast<size_t>(key[idx]);
    result <<= 1;
  }
  return result;
}

/**
 * Hash function policy class for test_string_hash_function.
 */
struct test_string_hash_function_policy
  : public free_function_hash_policy<test_string_hash_function_policy>
{
  static inline size_t
  hash(const string &key) noexcept
  {
    return test_string_hash_function(key);
  }
};

TEST_CASE("insert and find tests for custom hash function", "[hash_set]")
{
  using DemoHashSet = hash_set<string, test_string_hash_function_policy>;
  insert_find_count_tests<DemoHashSet>("zero", "one");
}

/**
 * Simple class for testing key extraction.
 */
struct TestType
{
  TestType(const int i, const string &s)
    : i_val(i), s_val(s)
  {
    ++arg_construct_count;
  }

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

  void
  get(int &i) const noexcept
  {
    i = i_val;
  }

  void
  get(string &s) const noexcept
  {
    s = s_val;
  }

  static void
  clear()
  {
    arg_construct_count = 0;
    copy_construct_count = 0;
    copy_assign_count = 0;
    move_construct_count = 0;
    move_assign_count = 0;
  }

  static unsigned arg_construct_count;
  static unsigned copy_construct_count;
  static unsigned copy_assign_count;
  static unsigned move_construct_count;
  static unsigned move_assign_count;

  int i_val;
  string s_val;
};

unsigned TestType::arg_construct_count = 0;
unsigned TestType::copy_construct_count = 0;
unsigned TestType::copy_assign_count = 0;
unsigned TestType::move_construct_count = 0;
unsigned TestType::move_assign_count = 0;

struct IntKey : public key_extractor_tag
{
  template<typename ValueType>
  struct rebind
  {
    static_assert(is_same<ValueType, TestType>::value,
		  "IntKey policy selected for hash_set that did not contain "
		  "entries of type 'TestType'");

    using key_type = int;

    static const key_type &
    get_key(const TestType &arg) noexcept
    {
      return arg.i_val;
    }

    static const bool can_get_key_from_constructor = true;

    static int
    key_from_ctor_args(int i, const string &s)
    {
      return i;
    }
  };
};

struct StrKey : public key_extractor_tag
{
  template<typename ValueType>
  struct rebind
  {
    static_assert(is_same<ValueType, TestType>::value,
		  "StrKey policy selected for hash_set that did not contain "
		  "entries of type 'TestType'");

    using key_type = string;

    static const key_type &
    get_key(const TestType &arg) noexcept
    {
      return arg.s_val;
    }

    static const bool can_get_key_from_constructor = true;

    static const key_type &
    key_from_ctor_args(const int i, const string &s)
    {
      return s;
    }
  };
};

template<typename KeyRetrievalType>
void
key_retrieval_tests(const vector<TestType> &vec)
{
  using HashSet = hash_set<TestType, KeyRetrievalType>;
  using ReboundExtractorType = typename KeyRetrievalType::template rebind<TestType>;
  using KeyType = typename ReboundExtractorType::key_type;
  using OtherType = typename conditional<is_same<KeyType, string>::value, string, int>::type;
  HashSet st;
  for (const auto &entry : vec) {
    const auto result = st.insert(entry);
    REQUIRE(result.second);
  }
  // only copy constructors should be called for insert()
  REQUIRE(0 == TestType::arg_construct_count);
  REQUIRE(vec.size() == TestType::copy_construct_count);
  REQUIRE(0 == TestType::copy_assign_count);
  REQUIRE(0 == TestType::move_construct_count);
  REQUIRE(0 == TestType::move_assign_count);
  for (const auto &value : vec) {
    const auto entry = st.find(value);
    REQUIRE(st.end() != entry);
    OtherType other1, other2;
    entry->get(other1);
    value.get(other2);
    REQUIRE(other1 == other2);
  }
}

TEST_CASE("key retrieval", "[hash_set]")
{
  vector<TestType> vec{ {0, "zero"}, {1, "one"}, {2, "two"}, {3, "three"} };
  TestType::clear();
  key_retrieval_tests<IntKey>(vec);
  TestType::clear();
  key_retrieval_tests<StrKey>(vec);
}

template<typename KeyRetrievalType>
void
forwarding_emplace_tests(const vector<int> &iVec, const vector<string> &sVec)
{
  using HashSet = hash_set<TestType, KeyRetrievalType>;
  using ReboundExtractorType = typename KeyRetrievalType::template rebind<TestType>;
  using KeyType = typename ReboundExtractorType::key_type;
  using OtherType = typename conditional<is_same<KeyType, string>::value, string, int>::type;
  HashSet st;
  for (size_t idx = 0; idx < iVec.size(); ++idx) {
    const auto result = st.emplace(iVec[idx], sVec[idx]);
    REQUIRE(result.second);
  }
  // only argument constructors should be called for insert()
  // (i.e. showing that emplace properly forwards its arguments to the
  // correct constructor)
  REQUIRE(iVec.size() == TestType::arg_construct_count);
  REQUIRE(0 == TestType::copy_construct_count);
  REQUIRE(0 == TestType::copy_assign_count);
  REQUIRE(0 == TestType::move_construct_count);
  REQUIRE(0 == TestType::move_assign_count);
  for (size_t idx = 0; idx < iVec.size(); ++idx) {
    const TestType value(iVec[idx], sVec[idx]);
    const auto entry = st.find(value);
    REQUIRE(st.end() != entry);
    OtherType other1, other2;
    entry->get(other1);
    value.get(other2);
    REQUIRE(other1 == other2);
  }
}

TEST_CASE("emplacement with perfect forwarding", "[hash_set]")
{
  vector<int> iVec{ 0, 1, 2, 3 };
  vector<string>sVec{ "zero", "one", "two", "three" };
  TestType::clear();
  forwarding_emplace_tests<IntKey>(iVec, sVec);
  TestType::clear();
  forwarding_emplace_tests<StrKey>(iVec, sVec);
}

// @todo test that swap produces the correct results for hasher_ and table_style_

// @todo test various loops that check bucket_size and load_factor
// while adding elements

// @todo build and test custom load factor policy with a threshold of 1.0
