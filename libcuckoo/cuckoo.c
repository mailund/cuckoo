
#include "cuckoo_internal.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#pragma mark auxilary functions
// A macro so we can swap arbitrary types.
#define SWAP(a,b,c) c = a; a = b; b = c;

#define HASH(tableno, table, key)  (tableno ? HASH1(table, key) : HASH0(table, key))
#define HASH_TABLE(tableno, table) (tableno ? (table)->table1 : (table)->table0)



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
    
    // for easy access to bin
    struct set_bin *bin;
    
    int attempt_threshold = set->table_size; // FIXME , not this much id:9
                                             //   
                                             // ----
                                             // <https://github.com/mailund/cuckoo/issues/8>

    for (int i = 0; i < attempt_threshold; ++i) {
        
        // I'm using the last bit of i to pick the table.
        // This is so I can simply increment i to pick the other table.
        int tableno = i & 1;
        
        struct set_bin *table = HASH_TABLE(tableno, set);
        size_t bin_index = HASH(tableno, set, hash_key);
        bin = &(table[bin_index]);
        
        SWAP(hash_key, bin->hash_key, hash_tmp)
        SWAP(application_key, bin->application_key, application_tmp);
        
        
        if (bin->tag == EMPTY) {
            bin->tag = OCCUPIED;
            return; // done with insertion
        }
    }
    
    //error("At this point we should rehash. I haven't implemented that yet.");
    assert(false);
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

void cuckoo_map_insert(struct cuckoo_map *map,
                       hash_key_type hash_key,
                       void *application_key,
                       void *application_value)
{
    // for swapping
    void *application_key_tmp, *application_value_tmp;
    hash_key_type hash_tmp;
    
    // for easy access to bin
    struct map_bin *bin;
    
    int attempt_threshold = map->table_size; // FIXME , not this much id:15
                                             //   
                                             // ----
                                             // <https://github.com/mailund/cuckoo/issues/12>
    
    for (int i = 0; i < attempt_threshold; ++i) {
        
        // I'm using the last bit of i to pick the table.
        // This is so I can simply increment i to pick the other table.
        int tableno = i & 1;
        
        struct map_bin *table = HASH_TABLE(tableno, map);
        size_t bin_index = HASH(tableno, map, hash_key);
        bin = &(table[bin_index]);
        
        SWAP(hash_key, bin->hash_key, hash_tmp)
        SWAP(application_key, bin->application_key, application_key_tmp);
        SWAP(application_value, bin->application_value, application_value_tmp);
        
        if (bin->tag == EMPTY) {
            bin->tag = OCCUPIED;
            return; // done with insertion
        }
    }
    
    //error("At this point we should rehash. I haven't implemented that yet.");
    assert(false);

}


