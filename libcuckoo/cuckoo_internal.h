
#ifndef CUCKOO_INTERNAL_H
#define CUCKOO_INTERNAL_H

#import "cuckoo.h"

// Largest prime that fits into 32 bits which
// we are guaranteed can be held in size_t
#define LARGE_PRIME 2147483647

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

