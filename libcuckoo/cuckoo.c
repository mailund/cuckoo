
#import "cuckoo_internal.h"

#include <stdio.h>
#include <stdbool.h>

static bool is_even(size_t val) {
    return !(val & 1);
}
static void assertion(bool test, const char *error_string)
{
    if (test) return;
        
    fprintf(stderr, "ERROR: %s\n", error_string);
    exit(EXIT_FAILURE);
}

#pragma mark cuckoo sets

struct cuckoo_set *new_cuckoo_set(size_t initial_size,
                                 comparison_type key_comparison,
                                 destructor_type key_destructor)
{
    assertion(is_even(initial_size), "Table size must be even.");
    
    struct cuckoo_set *table = malloc(sizeof(struct cuckoo_set));
    
    table->table1 = malloc(initial_size * sizeof(struct set_bin));
    table->table2 = table->table1 + initial_size / 2;
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
            struct set_bin *bin = &(set->table1[i]);
            if (bin->tag == USED)
                set->key_destructor(bin->application_key);
        }
    }
    free(set->table1); // table2 is part of table1 when freeing.
    free(set);
}


#pragma mark cuckoo maps

struct cuckoo_map *new_cuckoo_map(size_t initial_size,
                                 comparison_type key_comparison,
                                 destructor_type key_destructor,
                                 comparison_type value_comparison,
                                 destructor_type value_destructor)
{
    assertion(is_even(initial_size), "Table size must be even.");

    struct cuckoo_map *table = malloc(sizeof(struct cuckoo_map));
    
    table->table1 = malloc(initial_size * sizeof(struct map_bin));
    table->table2 = table->table1 + initial_size / 2;
    for (size_t i = 0; i < initial_size; ++i) {
        table->table1[i].tag = EMPTY;
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
            struct map_bin *bin = &(map->table1[i]);
            if (bin->tag == USED)
                map->key_destructor(bin->application_key);
        }
    }
    if (map->value_destructor) {
        for (size_t i = 0; i < map->table_size; ++i) {
            struct map_bin *bin = &(map->table1[i]);
            if (bin->tag == USED)
                map->value_destructor(bin->application_value);
        }
    }

    free(map->table1); // table2 is part of table1 when freeing.
    free(map);
}
