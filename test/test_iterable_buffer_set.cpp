/** -*- mode: c++ -*-
 *
 * test_iterable_buffer_set.cpp
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

#include <cstdint>
#include <cassert>
#include <cstring>

#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <limits>
#include <iostream>
#include <type_traits>

#include "jrl_util"
#include "jrl_macro"
#include "jrl_metaprogram"
#include "stl_usability/algorithm_usability"
#include "iterable_buffer_set"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;


// Helper definitions.
using IbsIterator = iterable_buffer_set<>::iterator;
using ConstIbsIterator = iterable_buffer_set<>::const_iterator;

// @todo
// iterable_buffer_set<>
// makeBuffSet(const string &str)
// {
//   return iterable_buffer_set<>(str);
// }

iterable_buffer_set<>
makeBuffSet(const char * const str)
{
  return iterable_buffer_set<>(str, strlen(str));
}

// @todo
// void
// insertString(iterable_buffer_set<> &buffSet,
// 	     IbsIterator insertPos,
// 	     const string &insStr)
// {
//   buffSet.insert(insertPos, insStr);
// }

void
insertString(iterable_buffer_set<> &buffSet,
	     IbsIterator insertPos,
	     const char * const insStr)
{
  buffSet.insert(insertPos, insStr, strlen(insStr));
}

// @todo
// void
// pushBackString(iterable_buffer_set<> &buffSet,
// 	       const string &insStr)
// {
//   buffSet.push_back(insStr);
// }

void
pushBackString(iterable_buffer_set<> &buffSet,
	       const char * const insStr)
{
  buffSet.push_back(insStr, strlen(insStr));
}

template<typename Inserter>
string
insertTest(const char *testStr, const char *insertStr, Inserter inserter)
{
  // \TODO
  // static_assert(is_function<decltype(inserter)>::value, "type Inserter must be a function");
  iterable_buffer_set<> buffSet(makeBuffSet(testStr));
  inserter(buffSet, insertStr);
  string check;
  // \TODO
  // copy(buffSet, back_inserter(check));
  copy(buffSet.begin(), buffSet.end(), back_inserter(check));
  return check;
}


// Test code

#define BASIC_INSERT_TEST(StringType)					\
  do {									\
    /* Original string. */						\
    const StringType testStr = "Hello World!\n";			\
    /* String to insert. */						\
    const StringType insertStr = "Cruel ";				\
    /* Buffer set. */							\
    iterable_buffer_set<> buffSet(makeBuffSet(testStr));		\
    /* Find delimiter. */						\
    IbsIterator delimiterPos = find(buffSet.begin(), buffSet.end(), ' '); \
    /* NOTE: incrementing here because the new string should be inserted */ \
    /* after the space character. */					\
    ++delimiterPos;							\
    /* Insert new string at delimiter point and compare against expected */ \
    /* value. */							\
    insertString(buffSet, delimiterPos, insertStr);			\
    const string expected = "Hello Cruel World!\n";			\
    string check;							\
    copy(buffSet.begin(), buffSet.end(), back_inserter(check));		\
    REQUIRE(expected == check);						\
  } while (0)

TEST_CASE("Basic C string insertion test", "[insert]")
{
  // @todo
  // BASIC_INSERT_TEST(char * const);
  const string result =
    insertTest("Hello World!\n", "Cruel ",
  	       [](auto &buffSet, const char *insertStr)
  	       {
  		 // find delimiter
  		 IbsIterator delimiterPos =
  		 find(buffSet.begin(), buffSet.end(), ' ');
  		 // NOTE: incrementing here because the new string
  		 // should be inserted after the space character
  		 ++delimiterPos;
  		 insertString(buffSet, delimiterPos, insertStr);
  	       });
  REQUIRE("Hello Cruel World!\n" == result);
}

// @todo
// TEST_CASE("Basic STL string insertion test", "[insert]")
// {
//   BASIC_INSERT_TEST(string);
// }

#undef BASIC_INSERT_TEST

#define FRONT_INSERT_TEST(StringType)					\
  do {									\
    /* Original string. */						\
    const StringType testStr = "Cruel World!\n";			\
    /* String to insert */						\
    const char * const insStr = "Goodbye, ";				\
    /* Buffer set. */							\
    iterable_buffer_set<> buffSet(makeBuffSet(testStr));		\
    IbsIterator frontPos = buffSet.begin();				\
    insertString(buffSet, frontPos, insStr);				\
    const string expected = "Goodbye, Cruel World!\n";			\
    string check;							\
    copy(buffSet.begin(), buffSet.end(), back_insert_iterator<string>(check)); \
    REQUIRE(expected == check);						\
  } while (0)

TEST_CASE("C string front insert test", "[insert]")
{
  FRONT_INSERT_TEST(char * const);
}

// @todo
// TEST_CASE("STL string front insert test", "[insert]")
// {
//   FRONT_INSERT_TEST(string);
// }

#undef FRONT_INSERT_TEST

#define BACK_INSERT_TEST(StringType)					\
  do {									\
    /* Original string. */						\
    const char * const testStr = "Goodbye, ";				\
    /* String to insert */						\
    const StringType insStr = "Cruel World!\n";				\
    /* Buffer set. */							\
    iterable_buffer_set<> buffSet(makeBuffSet(testStr));		\
    pushBackString(buffSet, insStr);					\
    const string expected = "Goodbye, Cruel World!\n";			\
    string check;							\
    copy(buffSet.begin(), buffSet.end(), back_insert_iterator<string>(check)); \
    REQUIRE(expected == check);						\
  } while (0)

TEST_CASE("C string back insert test", "[insert]")
{
  BACK_INSERT_TEST(char * const);
}

// @todo
// TEST_CASE("STL string back insert test", "[insert]")
// {
//   BACK_INSERT_TEST(string);
// }

TEST_CASE("const_iterator test", "[const_iterator]")
{
  const  char *strings[2] = {
    "Hello ",
    "World!"
  };
  iterable_buffer_set<> buffSet(makeBuffSet(strings[0]));
  pushBackString(buffSet, strings[1]);
  const size_t sz = strlen(strings[0]);
  const char *ptr = strings[0];
  size_t idx = 0;
  const ConstIbsIterator end = buffSet.cend();
  for (ConstIbsIterator entry = buffSet.cbegin(); entry != end; ++entry) {
    REQUIRE(*entry == ptr[idx]);
    ++idx;
    if ((strings[0] == ptr) && (sz == idx)) {
      ptr = strings[1];
      idx = 0;
    }
  }
}

// @todo test copy construction

// @todo test copy assignment

// @todo test move construction

// @todo test move assignment

#undef BACK_INSERT_TEST
