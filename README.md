Jolly Roger Labs Utility library
========

###Latest Stuff
Currently working a new method for compile time policies that uses a
novel mechanism for searching variadic template parameter packs.  This
has the following features:
- Eliminates the position dependency of arguments to the template instantiation.
- All policies have defaults.
- Instantiation with extraneous type arguments produces a simple error message.
- Instantiation with conflicting policies for the same behavior produces a simple error message.

There is a proof of concept demonstration of this in test/test_hash_table.cpp

For example:
```c
// no policies
hash_table<string, string> simple;

// trivial hash of unsigned key which replaces mod with bit mask
hash_table<size_t, string, trivial_hash_policy, power_of_two_length_table_policy> fast;

// hash table that never rehashes (thus avoiding the expense of the calculation)
hash_table<size_t, string, no_rehash_policy> no_rehash;
```

An early version of this work was presented at the New York C++ Meetup on September 6, 2016 in a talk entitled "C++ Dark Arts: Configurable Policy-Based Class Design with Variadic Templates"
https://www.meetup.com/nyccpp/events/233772015/

###Other Stuff
- unitsafe: eliminate implicit conversion for better safety.
- stl/algorithm/boost usability: make certain library functions easier to use.
- iterable_buffer_set: wrapper around a structure similar to iovec that allows iteration and insertion.

###Under Development/Coming Soon
Use of variadic templates with unitsafe to implement optional function arguments in a position-independent style similar to "named" arguments in other languages.

Application of variadic templates to create new interfaces for
interacting with databases.

###License
MIT License
