/** -*- mode: c++ -*-
 *
 * hash_benchmark.cpp
 *
 * Benchmarking code for hash_set/hash_map
 * vs. unordered_map/unordered_set.
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

#include <cstring>

#include <chrono>
#include <map>
#include <unordered_map>

#include <benchmark/benchmark.h>
#include <benchmark/reporter.h>

#include "hash_map"

#include "../hash_test_support.h"

using namespace std;

namespace
{

const double EXPECTED_TIME_BETWEEN_ORDER_ARRIVAL_EVENTS(40.0);
const double EXPECTED_ORDER_LIFETIME(40.0);
const int MIN_ORDERS(1 << 5);
const int MAX_ORDERS(1 << 14);

const auto SEED = chrono::system_clock::now().time_since_epoch().count();

} // unnamed namespace

/**
 * Function template parameterized on map type that processes the set
 * of events for a benchmark framework state.
 */
template<typename MapType>
void
processSequencedOrderEvents(::benchmark::State &state)
{
  while (state.KeepRunning()) {
    state.PauseTiming();
    auto events = generateEvents(SEED, state.range(0),
				 EXPECTED_TIME_BETWEEN_ORDER_ARRIVAL_EVENTS,
				 EXPECTED_ORDER_LIFETIME);
    state.ResumeTiming();
    MapType liveOrders;
    processSequencedOrderEvents<MapType>(liveOrders, events);
  }
}

// specialization of std::map
using MapOrders = map<OrderIdType, OrderState>;

// specialization of std::unordered_map
using StdOrders = unordered_map<OrderIdType, OrderState, Hasher>;

// specialization of jrl::hash_map using trivial hash function and
// power-of-two table size
using HashMapOrders =
  jrl::hash_map<OrderIdType, OrderState,
		jrl::free_function_hash_policy<UnitSafeTrivialHasher<OrderIdType>>,
		jrl::power_of_two_length_table_policy>;

// specialization of jrl::hash_map using trivial hash function,
// power-of-two table size and std::list for the bucket container
using HashMapListBucketOrders =
  jrl::hash_map<OrderIdType, OrderState,
		jrl::free_function_hash_policy<UnitSafeTrivialHasher<OrderIdType>>,
		jrl::power_of_two_length_table_policy,
		jrl::std_list_bucket_container_policy>;

static void
BM_Map(::benchmark::State &state)
{
  processSequencedOrderEvents<MapOrders>(state);
}

static void
BM_StdUnorderedMap(::benchmark::State &state)
{
  processSequencedOrderEvents<StdOrders>(state);
}

static void
BM_JrlHashMap(::benchmark::State &state)
{
  processSequencedOrderEvents<HashMapOrders>(state);
}

static void
BM_JrlHashMapListBucket(::benchmark::State &state)
{
  processSequencedOrderEvents<HashMapListBucketOrders>(state);
}

// @todo: OLD CODE BENCHMARK(BM_Map)->RangeMultiplier(2)->Range(MIN_ORDERS, MAX_ORDERS);

// @todo: OLD CODE BENCHMARK(BM_StdUnorderedMap)->RangeMultiplier(2)->Range(MIN_ORDERS, MAX_ORDERS);

BENCHMARK(BM_JrlHashMap)->RangeMultiplier(2)->Range(MIN_ORDERS, MAX_ORDERS);

// @todo: OLD CODE BENCHMARK(BM_JrlHashMapListBucket)->RangeMultiplier(2)->Range(MIN_ORDERS, MAX_ORDERS);

// @todo: OLD CODE BENCHMARK_MAIN();
int
main(int argc, char **argv)
{
  using namespace benchmark;
  Initialize(&argc, argv);
  CSVReporter reporter;
  RunSpecifiedBenchmarks(&reporter);
}
