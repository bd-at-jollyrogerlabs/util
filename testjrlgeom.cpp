/** -*- mode: c++-mode -*-
 *
 * testjrlgeom.cpp 
 *
 * Copyright (C) 2014 Brian Davis
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

#include "jrl_geom"

using namespace std;
using namespace jrl::geom;

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("test of parameterized_form", "[parameterized_form")
{
  using ParameterizedLine = parameterized_form<double>;
  using DistanceType = ParameterizedLine::distance_type;
  using PointType = ParameterizedLine::cartesian_point_type;
  using SegmentType = ParameterizedLine::cartesian_segment_type;

  const PointType origin(0.0, 0.0);
  const PointType unitX(1.0, 0.0);
  const PointType unitY(0.0, 1.0);
  const PointType unitDiag(1.0, 1.0);
  {
    const ParameterizedLine pLine(SegmentType(origin, unitX));
    const PointType halfway(pLine(0.5));
    REQUIRE(0.5 == halfway.x());
    REQUIRE(0.0 == halfway.y());
    DistanceType distance = 1.0 / 3.0;
    const PointType oneThird(pLine(distance));
    REQUIRE(distance == oneThird.x());
    REQUIRE(0.0 == oneThird.y());
    distance = 2.0 / 3.0;
    const PointType twoThirds(pLine(distance));
    REQUIRE(distance == twoThirds.x());
    REQUIRE(0.0 == twoThirds.y());
  }
  {
    const ParameterizedLine pLine(SegmentType(origin, unitY));
    const PointType halfway(pLine(0.5));
    REQUIRE(0.0 == halfway.x());
    REQUIRE(0.5 == halfway.y());
  }
  {
    const ParameterizedLine pLine(SegmentType(origin, unitDiag));
    // NOTE: length of the line between (0,0) and (1,1) is sqrt(2)
    const PointType halfway(pLine(0.5 * sqrt(2.0)));
    REQUIRE(0.5 == halfway.x());
    REQUIRE(0.5 == halfway.y());
  }
}

#define REQUIRE_MATCH_CARTESIAN(pt1, pt2)	\
  do {						\
    REQUIRE(pt1.x() == Approx(pt2.x()));	\
    REQUIRE(pt1.y() == Approx(pt2.y()));	\
  } while (0)

TEST_CASE("test of polar_form", "[polar_form]")
{
  using AngleType = double;
  using PolarCoords = polar_form<double, AngleType>;
  using CartesianCoords = PolarCoords::cartesian_coordinates;
  
  const CartesianCoords origin(0.0, 0.0);
  const CartesianCoords unitX(1.0, 0.0);
  const CartesianCoords unitY(0.0, 1.0);
  const CartesianCoords negUnitX(-1.0, 0.0);
  const CartesianCoords negUnitY(0.0, -1.0);
  const CartesianCoords unitDiag(1.0, 1.0);

  PolarCoords p1(unitX);
  AngleType angle = M_PI_2l; // 90 degrees
  p1.setAngle(angle);
  REQUIRE_MATCH_CARTESIAN(static_cast<CartesianCoords>(p1), unitY);
  angle += M_PI_2l; // 180 degrees
  p1.setAngle(angle);
  REQUIRE_MATCH_CARTESIAN(static_cast<CartesianCoords>(p1), negUnitX);
  angle += M_PI_2l; // 270 degrees
  p1.setAngle(angle);
  REQUIRE_MATCH_CARTESIAN(static_cast<CartesianCoords>(p1), negUnitY);
  angle += M_PI_2l; // 360 degrees
  p1.setAngle(angle);
  REQUIRE_MATCH_CARTESIAN(static_cast<CartesianCoords>(p1), unitX);

  PolarCoords p2(sqrt(2), M_PI_4l);
  REQUIRE_MATCH_CARTESIAN(static_cast<CartesianCoords>(p2), unitDiag);
}
