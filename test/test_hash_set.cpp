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

#include <iostream> // @todo remove this eventually
#include <string>

#include "hash_set"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

using IntHashSet = hash_set<int>;

using StrHashSet = hash_set<string>;


// ********** tests of basic functionality **********

/**
 * check that an empty set object works correctly
 */
template<typename SetContainerType>
void
empty_set_tests()
{
  SetContainerType st;
  REQUIRE(st.empty());
  REQUIRE(0 == st.size());
  REQUIRE(end(st) == st.begin());
  REQUIRE(st.cend() == st.cbegin());
}

TEST_CASE("empty integer set test", "[hash_set]")
{
  empty_set_tests<IntHashSet>();
}

TEST_CASE("empty string set test", "[hash_set]")
{
  empty_set_tests<IntHashSet>();
}

// @todo break these apart into separate test cases
template<typename SetContainerType, typename InsertOrEmplaceType>
void
basic_tests(const typename SetContainerType::value_type &val1,
	    const typename SetContainerType::value_type &val2,
	    InsertOrEmplaceType insOrEmp)
{
  assert(val1 != val2);
  using ValType = typename SetContainerType::value_type;
  SetContainerType st;
  // test insert()/emplace()
  {
    // make a copy of val1 to avoid having it be replaced in a move
    ValType tmp(val1);
    REQUIRE(0 == st.size());
    const auto result = insOrEmp(st, move(tmp));
    // NOTE: val1 should not be used after move
    REQUIRE(result.second);
    REQUIRE(1 == st.size());
  }
  {
    // make a copy of val1 to avoid having it be replaced in a move
    ValType tmp(val1);
    const auto result = insOrEmp(st, move(tmp));
    REQUIRE(!result.second);
    REQUIRE(val1 == *(result.first));
    REQUIRE(1 == st.size());
  }
  // test find() on previously added value
  {
    const auto entry = st.find(val1);
    REQUIRE(end(st) != entry);
    REQUIRE(!st.empty());
    REQUIRE(1 == st.size());
    REQUIRE(st.count(val1));
  }
  // test find() on a value known not to be in the set
  {
    const auto entry = st.find(val2);
    REQUIRE(end(st) == entry);
    REQUIRE(0 == st.count(val2));
  }
  // test erase() on previously added value
  {
    const auto count = st.erase(val1);
    REQUIRE(1 == count);
    REQUIRE(st.empty());
    REQUIRE(0 == st.size());
    REQUIRE(0 == st.count(val1));
  }
  // test erase() on a value known not to be in the set
  {
    const auto count = st.erase(val2);
    REQUIRE(0 == count);
    REQUIRE(0 == st.count(val2));
  }
  // test erase() called on an iterator
  {
    // make a copy to move from
    ValType tmp(val1);
    const auto result = insOrEmp(st, move(tmp));
    // NOTE: tmp should not be used after move
    REQUIRE(result.second);
    const auto entry(result.first);
    REQUIRE(end(st) != entry);
    REQUIRE(!st.empty());
    REQUIRE(1 == st.size());
    REQUIRE(st.count(val1));
    const auto next_entry = st.erase(entry);
    REQUIRE(end(st) == next_entry);
    REQUIRE(st.empty());
    REQUIRE(0 == st.size());
    REQUIRE(0 == st.count(val1));
  }
  // test clear()
  {
    ValType tmp(val1);
    const auto result = insOrEmp(st, move(tmp));
    // NOTE: tmp should not be used after move
    REQUIRE(result.second);
    const auto entry(result.first);
    REQUIRE(end(st) != entry);
    REQUIRE(!st.empty());
    REQUIRE(1 == st.size());
    REQUIRE(st.count(val1));
    st.clear();
    REQUIRE(st.empty());
    REQUIRE(0 == st.size());
    REQUIRE(0 == st.count(val1));
  }
}

template<typename HashSetType>
class Inserter
{
public:
  auto
  operator()(HashSetType &st, const typename HashSetType::value_type &val)
  {
    return st.insert(val);
  }
};

template<typename HashSetType>
class Emplacer
{
public:
  auto
  operator()(HashSetType &st, typename HashSetType::value_type &&val)
  {
    return st.emplace(move(val));
  }
};

TEST_CASE("basic integer insertion tests", "[hash_set]")
{
  basic_tests<IntHashSet>(0, 1, Inserter<IntHashSet>());
}

TEST_CASE("basic string insertion tests", "[hash_set]")
{
  basic_tests<StrHashSet>("zero", "one", Inserter<StrHashSet>());
}

TEST_CASE("basic integer emplacement tests", "[hash_set]")
{
  basic_tests<IntHashSet>(0, 1, Emplacer<IntHashSet>());
}

TEST_CASE("basic string emplacement tests", "[hash_set]")
{
  basic_tests<StrHashSet>("zero", "one", Emplacer<StrHashSet>());
}

/**
 * check that basic iteration works correctly
 */
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

TEST_CASE("integer iteration tests", "[hash_set]")
{
  iteration_tests(IntVec{0, 1, 2, 3, 4});
}

TEST_CASE("string iteration tests", "[hash_set]")
{
  iteration_tests(StrVec{"zero", "one", "two", "three", "four"});
}

namespace key_extraction_unit_tests
{

// ********** support code for testing key extraction and predicate **********

/**
 * Simple class for testing key extraction, key can be either an int
 * or a string.
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

  // static variables used to verify that appropriate constructors are
  // being called
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

inline bool
operator!=(const TestType &lhs, const TestType &rhs)
{
  return (lhs.i_val != rhs.i_val) || (lhs.s_val != rhs.s_val);
}

inline bool
operator==(const TestType &lhs, const TestType &rhs)
{
  return !(lhs != rhs);
}

/**
 * Custom policy for treating the int value of TestType as the key.
 */
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

/**
 * Custom policy for treating the string value of TestType as the key.
 */
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

// @todo consolidate key_retrieval_tests and forwarding_emplace_tests,
// probably need to consolidate forwarding_emplace_tests arguments
// arrays and throw in some functor types to grease the wheels.

/**
 * check that key retrieval works correctly
 */
template<typename KeyRetrievalType, typename... ParamPack>
void
key_retrieval_tests(const vector<TestType> &vec)
{
  using HashSet = hash_set<TestType, KeyRetrievalType, ParamPack...>;
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
    const auto entry = st.find(ReboundExtractorType::get_key(value));
    REQUIRE(st.end() != entry);
    OtherType other1, other2;
    entry->get(other1);
    value.get(other2);
    REQUIRE(other1 == other2);
  }
}

template<typename HashSetType>
void
emplace_hint_tests(typename HashSetType::value_type &&val)
{
  HashSetType st;
  typename HashSetType::value_type tmp(val);
  st.emplace_hint(st.cbegin(), move(tmp));
  // NOTE: tmp is now invalid
  REQUIRE(end(st) != st.find(val));
  const auto entry = st.find(val);
  REQUIRE(*entry == val);
}

TEST_CASE("integer emplace_hint tests", "[hash_set]")
{
  int val1 = 0;
  emplace_hint_tests<IntHashSet>(move(val1));
}

TEST_CASE("string emplace_hint test", "[hash_set]")
{
  string val2 = "zero";
  emplace_hint_tests<StrHashSet>(move(val2));
}

TEST_CASE("emplace_hint tests for partially ordered types", "[hash_set]")
{
# define VAL1_ARGS 0, "zero"
# define VAL2_ARGS 0, "ZERO"
  using HashSet = hash_set<TestType, IntKey>;
  using Extractor = typename IntKey::template rebind<TestType>;
  HashSet st;
  const TestType val1(VAL1_ARGS);
  const TestType val2(VAL2_ARGS);
  REQUIRE(val1 != val2);
  auto entry = st.emplace_hint(st.cend(), VAL1_ARGS);
  REQUIRE(end(st) != entry);
  REQUIRE(*entry == val1);
  REQUIRE(1 == st.size());
  REQUIRE(1 == st.count(Extractor::get_key(val1)));
  entry = st.emplace_hint(st.cend(), VAL2_ARGS);
  REQUIRE(end(st) != entry);
  REQUIRE(*entry == val2);
  REQUIRE(1 == st.size());
  REQUIRE(1 == st.count(Extractor::get_key(val2)));
# undef VAL1_ARGS
# undef VAL2_ARGS
}

/**
 * check that emplace with perfect forwarding works correctly
 */
template<typename KeyRetrievalType, typename... ParamPack>
void
forwarding_emplace_tests(const vector<int> &iVec, const vector<string> &sVec)
{
  using HashSet = hash_set<TestType, KeyRetrievalType, ParamPack...>;
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
    const auto entry = st.find(ReboundExtractorType::get_key(value));
    REQUIRE(st.end() != entry);
    OtherType other1, other2;
    entry->get(other1);
    value.get(other2);
    REQUIRE(other1 == other2);
  }
}

// ********** key retrieval unit tests **********

TEST_CASE("internal integer key retrieval tests", "[hash_set]")
{
  vector<TestType> vec{ {0, "zero"}, {1, "one"}, {2, "two"}, {3, "three"} };
  TestType::clear();
  key_retrieval_tests<IntKey>(vec);
}

TEST_CASE("internal string key retrieval tests", "[hash_set]")
{
  vector<TestType> vec{ {0, "zero"}, {1, "one"}, {2, "two"}, {3, "three"} };
  TestType::clear();
  key_retrieval_tests<StrKey>(vec);
}

TEST_CASE("internal integer key emplacement with perfect forwarding", "[hash_set]")
{
  vector<int> iVec{ 0, 1, 2, 3 };
  vector<string>sVec{ "zero", "one", "two", "three" };
  TestType::clear();
  forwarding_emplace_tests<IntKey>(iVec, sVec);
}

TEST_CASE("internal string key emplacement with perfect forwarding", "[hash_set]")
{
  vector<int> iVec{ 0, 1, 2, 3 };
  vector<string>sVec{ "zero", "one", "two", "three" };
  TestType::clear();
  forwarding_emplace_tests<StrKey>(iVec, sVec);
}

} // namespace key_extraction_unit_tests

namespace predicate_and_custom_hash_function_unit_tests
{

// ********** support code for testing custom predicate + hash function **********

// NOTE: if it is reasonable for a key type to have a non-trivial
// custom predicate, then it is unlikely for std::hash to work
// correctly for that type; as a result, custom predicate will
// generally imply custom hash function and the combination is tested
// here.

using key_extraction_unit_tests::TestType;

/**
 * Simple hash calculation function for TestType used to test
 * free_function_hash_policy.
 */
size_t
test_type_hash_function(const TestType &key) noexcept
{
  return (hash<int>()(key.i_val) << 1) ^ hash<string>()(key.s_val);
}

/**
 * Hash function policy class for test_type_hash_function.
 */
MAKE_FREE_FUNCTION_HASH_POLICY(test_type_hash_function_policy, size_t,
			       test_type_hash_function, TestType);

/**
 * Custom policy for predicate
 */
struct both_equals_predicate : public predicate_tag
{
  template<typename DummyType>
  struct rebind
  {
    static_assert(is_same<DummyType, TestType>::value,
		  "rebind class template should only be passed TestType for "
		  "both_equals_predicate" /*" but received " + typeid(DummyType).name()*/);
    bool
    operator()(const TestType &lhs, const TestType &rhs) const
    {
      int lhsInt;
      int rhsInt;
      lhs.get(lhsInt);
      rhs.get(rhsInt);
      string lhsStr;
      string rhsStr;
      lhs.get(lhsStr);
      rhs.get(rhsStr);
      return ((lhsInt == rhsInt) && (lhsStr == rhsStr));
    }
  };
};

// ********** custom predicate + custom hash function unit tests **********

TEST_CASE("predicate and custom hash function insert tests", "[hash_set]")
{
  // specialized type under test is TestType where the entire object
  // is the key and both_equals_predicate is the predicate.
  using TestTypeSet = hash_set<TestType, both_equals_predicate,
			       test_type_hash_function_policy>;

  TestType v0(0, "zero");
  TestType v1(1, "one");
  basic_tests<TestTypeSet>(move(v0), move(v1), Inserter<TestTypeSet>());
}

TEST_CASE("predicate and custom hash function emplace tests", "[hash_set]")
{
  // specialized type under test is TestType where the entire object
  // is the key and both_equals_predicate is the predicate.
  using TestTypeSet = hash_set<TestType, both_equals_predicate,
			       test_type_hash_function_policy>;

  TestType v0(0, "zero");
  TestType v1(1, "one");
  basic_tests<TestTypeSet>(move(v0), move(v1), Emplacer<TestTypeSet>());
}

} // namespace predicate_and_custom_hash_function_unit_tests

namespace custom_hash_function_unit_tests
{

// ********** support code for testing custom hash functions **********

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
MAKE_FREE_FUNCTION_HASH_POLICY(test_string_hash_function_policy, size_t,
			       test_string_hash_function, string);

// ********** custom hash function unit tests **********

TEST_CASE("insert and find tests for custom hash function", "[hash_set]")
{
  using DemoHashSet = hash_set<string, test_string_hash_function_policy>;
  basic_tests<DemoHashSet>("zero", "one", Inserter<DemoHashSet>());
}

// @todo verify that std::hash<std::string> produces results that are
// radically different from test_string_hash_function and then add a
// test case to verify that at least some of the assigned buckets are
// different when inserting a list of string values into both an
// instance of hash_set<string> and an instance of
// hash_set<string, test_string_hash_function_policy>

} // namespace custom_hash_function_unit_tests

namespace rehash_unit_tests
{

// ********** constants and support code for testing rehash **********

const size_t TERMINATION_MULT = 10;
const bool DO_MASK_CHECK = true;

// the following two convert() functions support the generalization of
// rehash_tests to work for size_t and string

void
convert(const size_t arg, size_t &result)
{
  result = arg;
}

void
convert(const size_t arg, string &result)
{
  result = to_string(arg);
}

// the following two doMask() functions support the generalization of
// rehash_tests to work for size_t and string

size_t
doMask(const size_t value, const size_t mask)
{
  return value & mask;
}

size_t
doMask(const string value, const size_t mask)
{
  THROW_EXCEPTION(logic_error, "called doMask() for string argument");
}

/**
 * check that rehashing works correctly
 *
 * NOTE: bool template parameter IsCheckMask should be set to 'true'
 * when testing trivial hash with power of 2 table size.
 */
template<typename HashSetType, bool IsCheckMask = false>
void
rehash_tests(const float maxLoadFactor,
	     function<void(HashSetType &, typename HashSetType::value_type)> adder)
{
  using ValueType = typename HashSetType::value_type;
  HashSetType h1;
  using CheckVec = vector<pair<ValueType, size_t>>;
  CheckVec c1;
  auto currentBuckets = h1.buckets();
  const auto initialBuckets = currentBuckets;
  for (size_t val = 0; val < (initialBuckets * TERMINATION_MULT); ++val) {
    ValueType value;
    convert(val, value);
    adder(h1, value);
    REQUIRE(maxLoadFactor >= h1.load_factor());
    if (h1.buckets() != currentBuckets) {
      // check if any entries have moved to different buckets
      bool isBucketChanged = false;
      for (auto &entry : c1) {
	const auto bucket = h1.bucket(entry.first);
	if (bucket != entry.second) {
	  isBucketChanged = true;
	  entry.second = bucket;
	}
      }
      REQUIRE(isBucketChanged);
      currentBuckets = h1.buckets();
    }
    c1.push_back(make_pair(value, h1.bucket(value)));
  }
  if (!IsCheckMask) {
    return;
  }
  const auto mask = h1.buckets() - 1;
  for (size_t val = 0; val < (initialBuckets * TERMINATION_MULT); ++val) {
    ValueType value;
    convert(val, value);
    REQUIRE(doMask(value, mask) == h1.bucket(value));
  }
}

/**
 * Custom rehash threshold policy with threshold value of 1.0.
 */
struct ThresholdOneRehashPolicy
  : public custom_threshold_rehash_policy<ThresholdOneRehashPolicy>
{
  constexpr static float
  get_threshold() noexcept
  {
    return 1.0;
  }
};

// ********** rehash unit tests **********

TEST_CASE("test of default rehash policy for insert operation", "[hash_set]")
{
  using HashSet = hash_set<size_t>;
  rehash_tests<HashSet>(default_rehash_policy::get_threshold(),
			[](HashSet &h, size_t val) {
			  h.insert(val);
			});
}

TEST_CASE("test of default rehash policy for emplace operation", "[hash_set]")
{
  using HashSet = hash_set<size_t>;
  rehash_tests<HashSet>(default_rehash_policy::get_threshold(),
			[](HashSet &h, size_t val) {
			  h.emplace(move(val));
			});
}

TEST_CASE("test of default rehash policy for emplace_hint operation", "[hash_set]")
{
  using HashSet = hash_set<size_t>;
  rehash_tests<HashSet>(default_rehash_policy::get_threshold(),
			[](HashSet &h, size_t val) {
			  h.emplace_hint(h.cend(), move(val));
			});
}

TEST_CASE("test combination of 'power of two' table size and trivial hash "
	  "function", "[hash_set]")
{
  using HashSet = hash_set<size_t, power_of_two_length_table_policy,
			   trivial_hash_policy>;
  rehash_tests<HashSet,
	       DO_MASK_CHECK>(default_rehash_policy::get_threshold(),
			      [](HashSet &h, size_t val) {
				h.insert(val);
			      });
}

TEST_CASE("test of custom rehash threshold value 1.0", "[hash_set]")
{
  using HashSet = hash_set<string, ThresholdOneRehashPolicy>;
  rehash_tests<HashSet>(1.0,
			[](HashSet &h, string val) {
			  h.insert(move(val));
			});
}

TEST_CASE("test of no_rehash_policy", "[hash_set]")
{
  using HashSet = hash_set<size_t, no_rehash_policy>;
  HashSet h1;
  const auto initialBuckets = h1.buckets();
  for (size_t value = 0; value < (initialBuckets * TERMINATION_MULT);
       ++value) {
    h1.insert(value);
    REQUIRE(initialBuckets == h1.buckets());
  }
}

} // namespace rehash_unit_tests

namespace bucket_container_type_unit_tests
{

using BucketListHashSet = hash_set<int, std_list_bucket_container_policy>;

TEST_CASE("basic tests for list bucket container policy", "[hash_set]")
{
  empty_set_tests<BucketListHashSet>();
  basic_tests<BucketListHashSet>(0, 1, Inserter<BucketListHashSet>());
  basic_tests<BucketListHashSet>(0, 1, Emplacer<BucketListHashSet>());
}

// @todo add tests of custom bucket container policy

} // namespace bucket_container_type_unit_tests

// @todo test that swap produces the correct results for stateful data
// members

// @todo test predicate policy

// @todo erase() test case: erase the last entry in the map where it
// is known that this entry is also the only entry in the last bucket

// @todo test the case where a key cannot be extracted from
// constructor arguments

// @todo add test of reserve(): start with an empty set, add elements
// until a rehash is detected, create a new empty set, reserve rehash
// count + 2 elements, add rehash count + 1 elements, require that no
// rehash has occurred

// @todo test the case where a value is not move constructable or move
// assignable
