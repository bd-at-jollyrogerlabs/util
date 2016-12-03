/** -*- mode: c++ -*-
 *
 * hash_test_support.h
 *
 * Support code for probabalistic testing of hash data structures.
 *
 * NOTE: non-template functions are implemented in this header, which
 * is should only be included in one translation unit per
 * test/benchmark program to avoid multiply-defined symbols
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

#if !defined(JRL_UTIL_HASH_TEST_SUPPORT_GUARD)
#define JRL_UTIL_HASH_TEST_SUPPORT_GUARD 1

#include <cstring>

#include <random>
#include <queue>
#include <tuple>

#include "unitsafe"

// enums for simple order states and events

enum class OrderDirection
{
  SELL = false,
  BUY = true
};

enum class OrderEvent
{
  ARRIVE = 0,
  DEPART = 1
};

// unitsafe types for simple order state

MAKE_UNITSAFE(SecurityIdType, uint32_t);

MAKE_UNITSAFE(OrderIdType, uint32_t);

MAKE_UNITSAFE(Quantity, uint32_t);

MAKE_UNITSAFE(Price, uint16_t);

MAKE_UNITSAFE(TimeUnit, uint32_t);

// @todo should this type be standard layout?
/**
 * Simple order state.
 */
struct OrderState
{
  Quantity qty;
  Price prc;
  OrderDirection direction;
  SecurityIdType secId;
  OrderIdType ordId;
  OrderIdType clOrdId;
};

/**
 * Trivial hash on OrderIdType.
 */
struct Hasher
{
  size_t
  operator()(const OrderIdType val) const
  {
    return static_cast<size_t>(base_cast(val));
  }
};

/**
 * Generic trivial hasher for unitsafe types
 *
 * @todo use SFINAE to avoid instantiation for types that are not
 * unsigned?
 */
template<typename UnitSafeType>
struct UnitSafeTrivialHasher
{
  static size_t
  hash(const UnitSafeType val)
  {
    return static_cast<size_t>(base_cast(val));
  }
};

// priority queue for order events
using PrioritizedOrderEvent = std::tuple<TimeUnit, OrderEvent, OrderIdType>;

bool operator<(const PrioritizedOrderEvent &lhs, const PrioritizedOrderEvent &rhs)
{
  return std::get<0>(lhs) < std::get<0>(rhs);
}

using OrderEventSequencer = std::priority_queue<PrioritizedOrderEvent>;

// container for storing order events
using SequencedOrderEvents = std::vector<std::pair<OrderEvent, OrderIdType>>;

// distribution to use when generating events
using Distribution = std::exponential_distribution<float>;

/**
 * Generate a vector of order events.
 */
// @todo pass the starting order ID?
template<typename SeedType, typename EventCountType>
SequencedOrderEvents
generateEvents(const SeedType seed, const EventCountType eventSz,
	       const double expectedEventInterArrivalTimeUnits,
	       const double expectedOrderLifetimeUnits)
{
  Distribution orderArrivalProcess(expectedEventInterArrivalTimeUnits);
  Distribution orderLifetimeProcess(expectedOrderLifetimeUnits);
  std::default_random_engine generator(seed);
  auto nextOrderArrival = [&]() {
    return orderArrivalProcess(generator);
  };
  auto nextOrderDeparture = [&]() {
    return orderLifetimeProcess(generator);
  };
  OrderEventSequencer sequencer;
  TimeUnit crntTime = 0;

  // sequence the order events
  if (eventSz < 0) {
    throw std::runtime_error("negative value for event count");
  }
  // NOTE: range(0) gives the number of events to process
  const unsigned maxGenerated = static_cast<unsigned>(eventSz);
  for (OrderIdType generated = 0; base_cast(generated) < maxGenerated;
       ++generated) {
    // next order arrives a random number of time units after the
    // current time
    const TimeUnit arrvOfst = static_cast<TimeUnit>(nextOrderArrival());
    sequencer.emplace(crntTime + arrvOfst, OrderEvent::ARRIVE, generated);
    crntTime += arrvOfst;
    // order departs a random number of time units after it arrives
    const TimeUnit dprtOfst = static_cast<TimeUnit>(nextOrderDeparture());
    sequencer.emplace(crntTime + dprtOfst, OrderEvent::DEPART, generated);
  }

  // store the sequenced order events in a data structure that
  // guarantees constant time retrieval

  // @todo the algo for iterating over the contents of a
  // priority_queue while removing them should be wrapped in an
  // iterator type
  SequencedOrderEvents result;
  result.reserve(sequencer.size());
  for (; !sequencer.empty(); sequencer.pop()) {
    const auto &event = sequencer.top();
    result.emplace_back(std::get<1>(event), std::get<2>(event));
  }
  return result;
}

/**
 * Process a vector of order events and execute the implied map
 * operations.
 */
template<typename MapType>
void
processSequencedOrderEvents(MapType &liveOrders, const SequencedOrderEvents &events)
{
  for (auto event : events) {
    OrderEvent eventType = std::get<0>(event);
    OrderIdType ordId = std::get<1>(event);
    switch (eventType) {
    case OrderEvent::ARRIVE:
      {
	OrderState ordState;
	memset(&ordState, 0, sizeof(ordState));
	ordState.ordId = ordId;
	liveOrders.emplace(std::move(ordId), std::move(ordState));
      }
      break;

    case OrderEvent::DEPART:
      liveOrders.erase(ordId);
      break;

    default:
      throw std::runtime_error("unknown order event");
      break;
    }
  }
}

#endif // JRL_UTIL_HASH_TEST_SUPPORT_GUARD
