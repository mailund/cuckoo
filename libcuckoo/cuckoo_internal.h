
#ifndef CUCKOO_INTERNAL_H
#define CUCKOO_INTERNAL_H

#include "cuckoo.h"

// Largest prime that fits into 32 bits which
// we are guaranteed can be held in size_t
#define LARGE_PRIME 2147483647

#pragma mark auxilary functions
// These are macros rather than inline functions so they
// work both with sets and maps.

// TODO: replace modulus with bit operations
#define HASH0(table, key) \
    (size_t)(((table->a0 * key + table->b0) % LARGE_PRIME) & (table->table_size - 1))
#define HASH1(table, key) \
    (size_t)(((table->a1 * key + table->b1) % LARGE_PRIME) & (table->table_size - 1))


// I use tags so I don't have to make assumptions
// on what application keys might be. Otherwise, I would
// have to assume that an application key is not null.
enum tags {
    EMPTY, OCCUPIED
};

#pragma mark cuckoo sets
struct set_bin {
    enum tags tag;
    hash_key_type hash_key;
    void * application_key;
};

struct cuckoo_set {
    struct set_bin *table0;
    struct set_bin *table1;
    size_t table_size;
    size_t table_used;
    size_t a0, a1, b0, b1; // Defining hash functions

    destructor_type key_destructor;
    comparison_type key_comparison;
};

#pragma mark cuckoo maps
struct map_bin {
    enum tags tag;
    hash_key_type hash_key;
    void * application_key;
    void * application_value;
};

struct cuckoo_map {
    struct map_bin *table0;
    struct map_bin *table1;
    size_t table_size;
    size_t table_used;

    size_t a0, a1, b0, b1; // Defining hash functions
    
    destructor_type key_destructor;
    comparison_type key_comparison;
    destructor_type value_destructor;
    comparison_type value_comparison;
};

#endif

