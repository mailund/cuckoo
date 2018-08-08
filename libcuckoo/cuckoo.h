
#ifndef CUCKOO_H
#define CUCKOO_H

#include <stdlib.h>

typedef size_t hash_key_type;
struct cuckoo_set;
struct cuckoo_map;

typedef void (*destructor_type)(void *);
typedef void (*comparison_type)(void *, void *);

#pragma mark cukoo sets
struct cuckoo_set *new_cuckoo_set(size_t initial_size,
                                 comparison_type key_comparison,
                                 destructor_type key_destructor);
void delete_cuckoo_set           (struct cuckoo_set *set);

#pragma mark cuckoo maps
struct cuckoo_map *new_cuckoo_map(size_t initial_size,
                                 comparison_type key_comparison,
                                 destructor_type key_destructor,
                                 comparison_type value_comparison,
                                 destructor_type value_destructor);
void delete_cuckoo_map           (struct cuckoo_map *map);

#endif


