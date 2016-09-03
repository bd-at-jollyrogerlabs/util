/** -*- mode: c++ -*-
 *
 * test_jrl_metaprogram.cpp
 *
 * Test cases for jrl_metaprogram code.
 *
 * Copyright (C) 2016 Brian Davis
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

#include <vector>
#include <type_traits>
#include <typeinfo>

#include "jrl_metaprogram"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;


// test cases for SELECTIVE_CONST

struct Inner {};

using IntVec = vector<int>;

template<bool IsConst>
struct Outer
{
  Outer() : inner() {}
  using EffectiveInner = SELECTIVE_CONST(IsConst, Inner);
  using ArrayType = SELECTIVE_CONST(IsConst, IntVec);

  EffectiveInner inner;
  ArrayType array;
};

using ConstInner = Outer<true>;
using NonConstInner = Outer<false>;

TEST_CASE("SELECTIVE_CONST", "[SELECTIVE_CONST]")
{
  ConstInner ci;
  NonConstInner nci;
  REQUIRE(is_const<decltype(ci.inner)>::value);
  REQUIRE(!is_const<decltype(nci.inner)>::value);
  REQUIRE(is_const<decltype(ci.array)>::value);
  REQUIRE(!is_const<decltype(nci.array)>::value);
}


// test cases for variadic template parameter pack counting

template<typename... ParameterPack>
struct variadic_test0
{
  static unsigned
  count()
  {
    return counter_type::value;
  }

private:
  using counter_type = param_pack_size<ParameterPack...>;
};

template<typename Dummy, typename... ParameterPack>
struct variadic_test1
{
  static unsigned
  count()
  {
    return counter_type::value;
  }

private:
  using counter_type = param_pack_size<ParameterPack...>;
};

TEST_CASE("Test parameter pack counting", "[param_pack_counter]")
{
  {
    using Count0 = variadic_test0<>;
    REQUIRE(0 == Count0::count());
  }

  {
    using Count1 = variadic_test0<int>;
    REQUIRE(1 == Count1::count());
  }

  {
    using Count2 = variadic_test0<int, double>;
    REQUIRE(2 == Count2::count());
  }
  {
    using Count0 = variadic_test1<int>;
    REQUIRE(0 == Count0::count());
  }

  {
    using Count1 = variadic_test1<int, int>;
    REQUIRE(1 == Count1::count());
  }

  {
    using Count2 = variadic_test1<int, int, double>;
    REQUIRE(2 == Count2::count());
  }
}


// test cases for variadic template policy matching

struct name_policy_tag {};

struct default_name_policy
{
  template<typename StringType>
  struct rebind
  {
    static constexpr StringType
    name()
    {
      return "Foo";
    }
  };
};

struct bar_name_policy : private name_policy_tag
{
  template<typename StringType>
  struct rebind
  {
    static constexpr StringType
    name()
    {
      return "Bar";
    }
  };
};

DEFINE_POLICY_BINDER(name_policy_binder,
		     default_name_policy,
		     name_policy_tag);

template<typename StringType, typename... Policies>
struct NamePolicyTest
{
  using NamePolicyBinding = name_policy_binder<Policies...>;
  using NamePolicy = typename NamePolicyBinding::type;
  using ParameterCount = param_pack_size<Policies...>;
  using PolicyCount = policy_bind_counter<NamePolicyBinding>;

  static_assert((ParameterCount::value == PolicyCount::value),
  		"extraneous type specified in NamePolicyTest policies");

  using Name =
    typename NamePolicy::template rebind<StringType>;

  static constexpr StringType
  name()
  {
    return Name::name();
  }

  static constexpr unsigned
  parameters()
  {
    return ParameterCount::value;
  }

  static constexpr unsigned
  policies()
  {
    return PolicyCount::value;
  }
};

namespace
{
const string FOO_NAME_STRING = "Foo";
const string BAR_NAME_STRING = "Bar";
const char *FOO_NAME_CSTRING = "Foo";
const char *BAR_NAME_CSTRING = "Bar";
}

TEST_CASE("policy binding test", "[binding]")
{
  using StringFooName = NamePolicyTest<string>;
  using CStringFooName = NamePolicyTest<const char *>;
  using StringBarName = NamePolicyTest<string, bar_name_policy>;
  using CStringBarName = NamePolicyTest<const char *, bar_name_policy>;

  REQUIRE(StringFooName::parameters() == 0);
  REQUIRE(CStringFooName::parameters() == 0);
  REQUIRE(StringBarName::parameters() == 1);
  REQUIRE(CStringBarName::parameters() == 1);

  REQUIRE(StringFooName::policies() == 0);
  REQUIRE(CStringFooName::policies() == 0);
  REQUIRE(StringBarName::policies() == 1);
  REQUIRE(CStringBarName::policies() == 1);

  REQUIRE(StringFooName::name() == FOO_NAME_STRING);
  REQUIRE(CStringFooName::name() == FOO_NAME_CSTRING);
  REQUIRE(StringBarName::name() == BAR_NAME_STRING);
  REQUIRE(CStringBarName::name() == BAR_NAME_CSTRING);
}

#if 0
TEST_CASE("will not compile due to extraneous parameter", "[broken Foo]")
{
  using BrokenName = NamePolicyTest<string, int>;
  REQUIRE(BrokenName::parameters() == 1);
  REQUIRE(BrokenName::policies() == 0);
}
#endif

#if 0
TEST_CASE("will not compile due to extraneous parameter before policy", "[broken Bar]")
{
  using BrokenName = NamePolicyTest<string, int, bar_name_policy>;
  REQUIRE(BrokenName::parameters() == 2);
  REQUIRE(BrokenName::policies() == 1);
}
#endif

#if 0
TEST_CASE("will not compile due to extraneous parameter after policy", "[broken Bar]")
{
  using BrokenName = NamePolicyTest<string, bar_name_policy, int>;
  REQUIRE(BrokenName::parameters() == 2);
  REQUIRE(BrokenName::policies() == 1);
}
#endif
