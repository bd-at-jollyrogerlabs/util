/** -*- mode: c++ -*-
 *
 * test_hash_table.cpp
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

// System includes
#include <cassert>

// STL includes
#include <experimental/optional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

// Boost includes

// Other library includes
#include <jrl_macro>
#include <jrl_variadic>

// local includes

#define CATCH_CONFIG_MAIN
#include <catch.hpp>



// hash map policies

namespace hash_table_policies
{


// hash function policies

struct custom_hash_function_policy_tag {};

struct rehash_threshold_policy_tag {};

template<typename KeyType, typename FunctionPointerType>
struct free_function_hash : public custom_hash_function_policy_tag
{
  size_t
  operator()(KeyType key) const
  {
    return function_(key);
  }

protected:
  FunctionPointerType function_;
};

/**
 * policy: no hash calculation, the key value is passed through
 * unchanged.
 */
struct trivial_hash_policy
{
  template<typename KeyType>
  struct rebind
  {
    using type = rebind<KeyType>;

    size_t
    operator()(KeyType key) const noexcept
    {
      return static_cast<size_t>(key);
    }
  };
};


// table length style policies

/**
 * policy: table length is a prime number and hash values are mapped
 * to buckets using modulus (AKA operator%)
 */
struct prime_length_table_style
{
  prime_length_table_style()
    : idx_(INITIAL_BUCKET_COUNT_IDX)
  {
  }

  static size_t
  initial_bucket_count()
  {
    return primes_[INITIAL_BUCKET_COUNT_IDX];
  }

  size_t
  hash_to_bucket(const size_t hash) const noexcept
  {
    return hash % primes_[idx_];
  }

  size_t
  grow()
  {
    ++idx_;
    return primes_[idx_];
  }

  static size_t INITIAL_BUCKET_COUNT_IDX;

private:
  static const std::vector<size_t> primes_;

  size_t idx_;
};

size_t prime_length_table_style::INITIAL_BUCKET_COUNT_IDX = 7;

const std::vector<size_t> prime_length_table_style::primes_{
    17, 29, 37, 53, 67, 79, 97, 131, 193, 257, 389, 521, 769,
    1031, 1543, 2053, 3079, 6151, 12289, 24593, 49157, 98317,
    196613, 393241, 786433, 1572869, 3145739, 6291469,
    12582917, 25165843, 50331653, 100663319, 201326611,
    402653189, 805306457, 1610612741, 3221225473, 4294967291
  };

/**
 * policy: table length is a power of two and hash values are mapped
 * to buckets by masking away higher order bits
 */
struct power_of_two_length_table_style
{
  power_of_two_length_table_style()
    : count_(INITIAL_BUCKET_COUNT),
      mask_(INITIAL_BUCKET_COUNT - 1)
  {
  }

  static size_t
  initial_bucket_count()
  {
    return INITIAL_BUCKET_COUNT;
  }

  size_t
  hash_to_bucket(const size_t hash) const noexcept
  {
    return hash & mask_;
  }

  size_t
  grow()
  {
    count_ <<= 1;
    mask_ <<= 1;
    mask_ |= 1;
    return count_;
    assert(mask_ == (count_ - 1));
  }

  static size_t INITIAL_BUCKET_COUNT;

private:
  size_t count_;
  size_t mask_;
};

size_t power_of_two_length_table_style::INITIAL_BUCKET_COUNT = 128;


// rehash policies

template<typename Derived>
struct load_factor_custom_threshold : public rehash_threshold_policy_tag
{
  static bool
  needs_rehash(size_t buckets, size_t entries)
  {
    return (static_cast<float>(entries) / static_cast<float>(buckets)) > Derived::get_threshold();
  }
};

/**
 * policy: rehash the table when the ratio of the number of entries to
 * the number of buckets exceeds a threshold
 */
struct default_load_factor_threshold_policy
  : public load_factor_custom_threshold<default_load_factor_threshold_policy>
{
  constexpr static float get_threshold() { return 1.5; }
};

/**
 * policy: rehash the table when the ratio of the number of entries to
 * the number of buckets exceeds a threshold
 */
template<uint32_t LoadFactor>
struct rehash_default_load_factor_threshold_policy_new
{
  static_assert((sizeof(uint32_t) == sizeof(float)),
		"sizeof(uint32_t) did not match sizeof(float) as expected");

  static bool
  needs_rehash(size_t buckets, size_t entries)
  {
    return (static_cast<float>(entries) / static_cast<float>(buckets)) > threshold_;
  }

private:
  static const float threshold_;
};

template<uint32_t LoadFactor>
const float rehash_default_load_factor_threshold_policy_new<LoadFactor>::threshold_ = 1.5;

/**
 * policy: never rehash the table
 */
struct no_rehash_policy
{
  static bool
  needs_rehash(size_t buckets, size_t entries)
  {
    return false;
  }
};

}


namespace detail
{
using namespace hash_table_policies;

// hash function policy

TAGGED_POLICY_BINDER_TYPE_1_ARG(hash_function_policy_binder, std::hash<KeyType>,
				KeyType, custom_hash_function_policy_tag);

ADD_POLICY_CHOICE_1_ARG(hash_function_policy_binder,
			hash_table_policies::trivial_hash_policy,
			KeyType);

// table length style policy

POLICY_BINDER_TYPE(table_style_policy_binder,
		   prime_length_table_style);

ADD_POLICY_CHOICE(table_style_policy_binder,
		  power_of_two_length_table_style);

// rehash policy

TAGGED_POLICY_BINDER_TYPE(rehash_policy_binder,
			  default_load_factor_threshold_policy,
			  rehash_threshold_policy_tag);

ADD_POLICY_CHOICE(rehash_policy_binder,
		  no_rehash_policy);

}

template<typename KeyType, typename ValueType, typename... Policies>
class hash_table
{
public:
  using key_type = KeyType;
  using value_type = ValueType;
  using mapped_type = std::pair<key_type, value_type>;

private:
  // policy configurations
  using hash_function_policy =
    typename detail::hash_function_policy_binder<KeyType, Policies...>::type;

  using table_style_policy =
    typename detail::table_style_policy_binder<Policies...>::type;

  using rehash_policy =
    typename detail::rehash_policy_binder<Policies...>::type;

  // policy configuration check: the length of the policy template
  // pack must equal the number of matched policies.
  static_assert((jrl::param_pack_counter<Policies...>::value ==
		 jrl::bound_policy_counter<detail::hash_function_policy_binder<KeyType, Policies...>,
		                           detail::table_style_policy_binder<Policies...>,
		                           detail::rehash_policy_binder<Policies...>
		                          >::value),
		"extraneous type specified in hash_table policies");

public:
  using hasher = hash_function_policy;

  hash_table()
    : table_(table_style_policy::initial_bucket_count(), bucket_type()), entry_count_(0)
  {
  }

  size_t
  size() const
  {
    return entry_count_;
  }

  size_t
  buckets() const
  {
    return table_.size();
  }

  /**
   * insert a (key, value) pair into the map, overwriting any previous
   * value associated with the same key
   */
  void
  insert(const KeyType &key, const ValueType &value)
  {
    auto search_result = find_entry(key);
    auto entry = search_result.first;
    if (entry) {
      bucket_iterator &i = *entry;
      assert(key == i->first);
      i->second = value;
    }
    else {
      const size_t found_idx = search_result.second;
      bucket_type &bucket(table_.at(found_idx));
      bucket.emplace_back(std::move(std::make_pair(key, value)));
      ++entry_count_;
      while (rehash_policy::needs_rehash(buckets(), size())) {
	rehash();
      }
    }
  }

  /**
   * get a copy of the value associated with a key, if any
   */
  std::experimental::optional<ValueType>
  get(const KeyType &key)
  {
    auto search_result = find_entry(key);
    auto entry = search_result.first;
    if (entry) {
      return (*entry)->second;
    }
    return std::experimental::nullopt;
  }

private:

  using bucket_allocator_type = std::allocator<ValueType>;
  using bucket_type = std::vector<mapped_type, bucket_allocator_type>;
  using bucket_iterator = typename bucket_type::iterator;
  using table_allocator_type = std::allocator<bucket_type>;
  using table_type = std::vector<bucket_type, table_allocator_type>;
  using table_iterator = typename table_type::iterator;
  using table_search_result =
    std::pair<std::experimental::optional<bucket_iterator>, size_t>;

  /**
   * shared code for finding an entry in a table
   */
  table_search_result
  find_entry(const KeyType &key)
  {
    const size_t idx = table_style_.hash_to_bucket(hasher_(key));
    bucket_type &bucket(table_.at(idx));
    auto entry = find_if(std::begin(bucket), std::end(bucket),
			 [key](const auto &entry) {
			   return entry.first == key;
			 });
    if (std::end(bucket) == entry) {
      return table_search_result(std::experimental::nullopt, idx);
    }
    return table_search_result(entry, idx);
  }

  /**
   * increase the number of table entries to the next size and then
   * rehash all current elements.
   */
  void
  rehash()
  {
    table_type temp_copy(table_);
    const size_t new_bucket_count = table_style_.grow();
    table_ = table_type(new_bucket_count, bucket_type());
    const size_t old_entry_count = entry_count_;
    // NOTE: reset the entry count to 0 since it will be updated
    // automatically by calls to insert()
    entry_count_ = 0;
    for (auto bucket : temp_copy) {
      for (auto entry : bucket) {
	insert(entry.first, entry.second);
      }
    }
    THROW_ON_FAIL(old_entry_count == entry_count_, std::logic_error,
    		  "mismatched entry count after rehash, old value was "
    		  << old_entry_count << " new value is " << entry_count_);
  }

  table_type table_;
  size_t entry_count_;
  hash_function_policy hasher_;
  table_style_policy table_style_;
};


// test-specific code

using namespace std;

// support code

string
make_string(const string &base, const size_t suffix)
{
  return base + to_string(suffix);
}

// custom policies

// weird function for calculating a hash value from a string key
size_t
weird_string_hash_function(const string &key)
{
  THROW_ON_FAIL(key.size() <= sizeof(size_t), runtime_error,
		"weird_string_hash_function received a key [" << key
		<< "] that was larger than the limit of " << sizeof(size_t));
  size_t result = 0;
  for (size_t idx = 0; (idx < key.size()) && (idx <= sizeof(result)); ++idx) {
    // convert the character to size_t
    size_t update = static_cast<size_t>(key[idx]);
    // shift the character to the appropriate position
    update <<= ((key.size() - idx - 1) * 8);
    // incorporate the new bits into the result
    result |= update;
  }
  return result;
}

typedef size_t (*StringHashFreeFunction)(const string &);

// wrapper class for weird_string_hash_function
struct weird_hash_function_wrap
  : public hash_table_policies::free_function_hash<const string &,
						   StringHashFreeFunction>
{
public:
  weird_hash_function_wrap()
  {
    function_ = &weird_string_hash_function;
  }
};

// custom rehash load factor threshold policy with load factor of 1.0
// (i.e. rehash whenever the number of entries exceeds the number of
// buckets.
struct load_factor_threshold_1_policy
  : public hash_table_policies::load_factor_custom_threshold<load_factor_threshold_1_policy>
{
  constexpr static float get_threshold() { return 1.0; }
};


// test cases

TEST_CASE("simple hash table mapping string to string", "[str2str]")
{
  using namespace hash_table_policies;

  using StrStrHashTable = hash_table<string, string>;
  StrStrHashTable strStrHashTable;

  {
    strStrHashTable.insert("foo", "bar");
    const auto result = strStrHashTable.get("foo");
    REQUIRE(result);
    REQUIRE(*result == "bar");
    REQUIRE(strStrHashTable.size() == 1);
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const string key = make_string("foo", counter);
    const string value = make_string("bar", counter);
    strStrHashTable.insert(key, value);
    REQUIRE(strStrHashTable.size() == (counter + 2));
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const string key = make_string("foo", counter);
    const auto result = strStrHashTable.get(key);
    REQUIRE(result);
    const string value = make_string("bar", counter);
    REQUIRE(*result == value);
  }
}

TEST_CASE("trivial + power of 2 hash table test", "[trivial]")
{
  using namespace hash_table_policies;

  using TrivalUnsignedStrTable =
    hash_table<size_t, string,
	       trivial_hash_policy,
	       power_of_two_length_table_style>;
  TrivalUnsignedStrTable trivialUnsignedStrTable;

  {
    trivialUnsignedStrTable.insert(20010911, "foo");
    const auto result = trivialUnsignedStrTable.get(20010911);
    REQUIRE(result);
    REQUIRE(*result == "foo");
    REQUIRE(trivialUnsignedStrTable.size() == 1);
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const string value = make_string("foo", counter);
    trivialUnsignedStrTable.insert(counter, value);
    REQUIRE(trivialUnsignedStrTable.size() == (counter + 2));
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const auto result = trivialUnsignedStrTable.get(counter);
    REQUIRE(result);
    const string value = make_string("foo", counter);
    REQUIRE(*result == value);
  }
}

TEST_CASE("free subprogram hash function test", "[free_function_hash]")
{
  using namespace hash_table_policies;

  using WeirdStrStrHashTable =
    hash_table<string, string,
	       weird_hash_function_wrap,
	       power_of_two_length_table_style>;
  WeirdStrStrHashTable weirdStrStrHashTable;
  {
    weirdStrStrHashTable.insert("foo", "bar");
    const auto result = weirdStrStrHashTable.get("foo");
    REQUIRE(result);
    REQUIRE(*result == "bar");
    REQUIRE(weirdStrStrHashTable.size() == 1);
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const string key = make_string("foo", counter);
    const string value = make_string("bar", counter);
    weirdStrStrHashTable.insert(key, value);
    REQUIRE(weirdStrStrHashTable.size() == (counter + 2));
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const string key = make_string("foo", counter);
    const auto result = weirdStrStrHashTable.get(key);
    REQUIRE(result);
    const string value = make_string("bar", counter);
    REQUIRE(*result == value);
  }
}

TEST_CASE("custom load factor test", "[load_factor_1]")
{
  using namespace hash_table_policies;

  using StrStrHashTable = hash_table<string, string, load_factor_threshold_1_policy>;
  StrStrHashTable strStrHashTable;

  {
    strStrHashTable.insert("foo", "bar");
    const auto result = strStrHashTable.get("foo");
    REQUIRE(result);
    REQUIRE(*result == "bar");
    REQUIRE(strStrHashTable.size() == 1);
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const string key = make_string("foo", counter);
    const string value = make_string("bar", counter);
    strStrHashTable.insert(key, value);
    REQUIRE(strStrHashTable.size() == (counter + 2));
  }
  for (size_t counter = 0; counter < 1000; ++counter) {
    const string key = make_string("foo", counter);
    const auto result = strStrHashTable.get(key);
    REQUIRE(result);
    const string value = make_string("bar", counter);
    REQUIRE(*result == value);
  }
}
