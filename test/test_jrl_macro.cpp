/** -*- mode: c++ -*-
 *
 * testmacro.cpp
 *
 * Test cases for stl_macro code.
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

#include <cstring>
#include <ctime>
#include <cerrno>

#include <system_error>

#include "jrl_macro"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("test CALL_SYSTEM_FUNC throwing exception on error", "[CALL_SYSTEM_FUNC]")
{
  REQUIRE_THROWS_AS(CALL_SYSTEM_FUNC(localtime_r(nullptr, nullptr), nullptr,
				     "calling localtime_r"),
		    std::system_error);
}

TEST_CASE("test CALL_SYSTEM_FUNC not throwing exception on success", "[CALL_SYSTEM_FUNC]")
{
  const time_t ts = time(nullptr);
  struct tm unpacked;
  // ensure that the unpacked data is cleared properly according to
  // the expected format
  memset(&unpacked, 0, sizeof(unpacked));
  REQUIRE(0 == unpacked.tm_year);
  REQUIRE_NOTHROW(CALL_SYSTEM_FUNC(localtime_r(&ts, &unpacked), nullptr,
				   "calling localtime_r"));
  // ensure that the unpacked data is populated
  REQUIRE(0 != unpacked.tm_year);
}
