/** -*- mode: c++ -*-
 *
 * test_jrl_variadic.cpp
 *
 * Test cases for variadic template support code.
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

// STL includes

// Boost includes

// Other library includes

// local includes
#include "jrl_variadic"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace std;
using namespace jrl;

template<typename... ParameterPack>
struct variadic_test
{
  static unsigned
  count()
  {
    return counter_type::value;
  }

private:
  using counter_type = param_pack_counter<ParameterPack...>;
};

TEST_CASE("Test parameter pack counting", "[param_pack_counter]")
{
  {
    using Count0 = variadic_test<>;
    REQUIRE(0 == Count0::count());
  }

  {
    using Count1 = variadic_test<int>;
    REQUIRE(1 == Count1::count());
  }

  {
    using Count2 = variadic_test<int, double>;
    REQUIRE(2 == Count2::count());
  }
}
