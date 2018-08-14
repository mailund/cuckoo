
#include "cuckoo_internal.h"

#include <stdio.h>
#include <stdbool.h>

#pragma mark auxilary functions
// A macro so we can swap arbitrary types.
#define SWAP(a,b,c) c = a; a = b; b = c;

// These are macros rather than inline functions so they
// work both with sets and maps.

// TODO: replace modulus with bit operations
#define HASH0(table, key) \
    (size_t)(((table->a0 * key + table->b0) % LARGE_PRIME) & (table->table_size - 1))
#define HASH1(table, key) \
    (size_t)(((table->a1 * key + table->b1) % LARGE_PRIME) & (table->table_size - 1))

#define HASH(tableno, table, key) \
    (tableno ? HASH1(table, key) : HASH0(table, key))
#define HASH_TABLE(tableno, table) \
    (tableno ? (table)->table0 : (table)->table1)


#pragma mark error handling functions
static inline bool is_power_of_two(size_t x) { return (x & (x - 1)) == 0; }
static inline void assertion(bool test, const char *error_string)
{
    if (test) return;
    fprintf(stderr, "ERROR: %s\n", error_string);
    exit(EXIT_FAILURE);
}
#if 0 // not used right now
static inline void error(const char *error_string)
{
    fprintf(stderr, "ERROR: %s\n", error_string);
    exit(EXIT_FAILURE);
}
#endif

#pragma mark cuckoo sets

struct cuckoo_set *new_cuckoo_set(size_t initial_size,
                                 comparison_type key_comparison,
                                 destructor_type key_destructor)
{
    assertion(initial_size > 0, "Initial size cannot be zero.");
    assertion(is_power_of_two(initial_size), "Table size must be a power of two.");
    
    struct cuckoo_set *table = malloc(sizeof(struct cuckoo_set));
    
    table->table0 = malloc(initial_size * sizeof(struct set_bin));
    table->table1 = table->table0 + initial_size / 2;
    for (size_t i = 0; i < initial_size; ++i) {
        table->table1[i].tag = EMPTY;
    }
    
    table->key_comparison = key_comparison;
    table->key_destructor = key_destructor;
    table->table_size = initial_size;
    table->table_used = 0;
    
    return table;
}

void delete_cuckoo_set(struct cuckoo_set *set)
{
    if (set->key_destructor) {
        for (size_t i = 0; i < set->table_size; ++i) {
            struct set_bin *bin = &(set->table0[i]);
            if (bin->tag == OCCUPIED)
                set->key_destructor(bin->application_key);
        }
    }
    free(set->table0); // table1 is part of table1 when freeing.
    free(set);
}



void cuckoo_set_insert(struct cuckoo_set *set,
                       hash_key_type hash_key,
                       void *application_key)
{
    // for swapping
    void *application_tmp;
    hash_key_type hash_tmp;
    
    bool done = false;
    struct set_bin *bin;
    // I'm using the last bit of this to pick the table.
    // This is so I can simply increment it to pick the other table.
    int table_counter = 0;
    
    // TODO: rehash if we iterate too long.
    do {
        // Low bit of table_counter picks the table.
        int tableno = table_counter & 1;
        struct set_bin *table = HASH_TABLE(tableno, set);
        bin = &(table[HASH0(set, hash_key)]);
        SWAP(hash_key, bin->hash_key, hash_tmp)
        SWAP(application_key, bin->application_key, application_tmp);
        table_counter++;
        if (bin->tag == EMPTY) done = true; // no elements to insert
    } while (!done);
}

#pragma mark cuckoo maps

struct cuckoo_map *new_cuckoo_map(size_t initial_size,
                                 comparison_type key_comparison,
                                 destructor_type key_destructor,
                                 comparison_type value_comparison,
                                 destructor_type value_destructor)
{
    assertion(initial_size > 0, "Initial size cannot be zero.");
    assertion(is_power_of_two(initial_size), "Table size must be a power of two.");
    
    struct cuckoo_map *table = malloc(sizeof(struct cuckoo_map));
    
    table->table0 = malloc(initial_size * sizeof(struct map_bin));
    table->table1 = table->table0 + initial_size / 2;
    for (size_t i = 0; i < initial_size; ++i) {
        table->table0[i].tag = EMPTY;
    }
    
    table->key_comparison = key_comparison;
    table->key_destructor = key_destructor;
    table->value_comparison = value_comparison;
    table->value_destructor = value_destructor;
    table->table_size = initial_size;
    table->table_used = 0;
    
    return table;
}

void delete_cuckoo_map(struct cuckoo_map *map)
{
    if (map->key_destructor) {
        for (size_t i = 0; i < map->table_size; ++i) {
            struct map_bin *bin = &(map->table0[i]);
            if (bin->tag == OCCUPIED)
                map->key_destructor(bin->application_key);
        }
    }
    if (map->value_destructor) {
        for (size_t i = 0; i < map->table_size; ++i) {
            struct map_bin *bin = &(map->table0[i]);
            if (bin->tag == OCCUPIED)
                map->value_destructor(bin->application_value);
        }
    }

    free(map->table0); // table1 is part of table1 when freeing.
    free(map);
}

void cuckoo_map_insert(struct cuckoo_set *map,
                       hash_key_type hash_key,
                       void *application_key,
                       void *application_value)
{
#warning Not implemented yet
}


