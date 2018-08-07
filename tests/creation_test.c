
#include "cuckoo_internal.h"

#include <stdlib.h>
#include <assert.h>

void comparison(void *key1, void *key2)
{
    return;
}

void destructor(void *key)
{
    return;
}

void comparison2(void *key1, void *key2)
{
    return;
}

void destructor2(void *key)
{
    return;
}

#warning I haven't tested that the bin arrays are allocated yet.
int main(int arvc, const char *argv[])
{
    size_t initial_size = 5;
    
    struct cuckoo_set *set = new_cuckoo_set(initial_size, 0, 0);
    assert(set != 0);
    assert(set->table1 != 0);
    assert(set->table2 != 0);
    assert(set->key_comparison == 0);
    assert(set->key_destructor == 0);
    assert(set->table_size == initial_size);
    assert(set->table_used == 0);
    delete_cuckoo_set(set);
    
    set = new_cuckoo_set(initial_size, comparison, 0);
    assert(set != 0);
    assert(set->table1 != 0);
    assert(set->table2 != 0);
    assert(set->key_comparison == comparison);
    assert(set->key_destructor == 0);
    assert(set->table_size == initial_size);
    assert(set->table_used == 0);
    delete_cuckoo_set(set);
    
    set = new_cuckoo_set(initial_size, 0, destructor);
    assert(set != 0);
    assert(set->table1 != 0);
    assert(set->table2 != 0);
    assert(set->key_comparison == 0);
    assert(set->key_destructor == destructor);
    assert(set->table_size == initial_size);
    assert(set->table_used == 0);
    delete_cuckoo_set(set);
    
    set = new_cuckoo_set(initial_size, comparison, destructor);
    assert(set != 0);
    assert(set->table1 != 0);
    assert(set->table2 != 0);
    assert(set->key_comparison == comparison);
    assert(set->key_destructor == destructor);
    assert(set->table_size == initial_size);
    assert(set->table_used == 0);
    delete_cuckoo_set(set);
    
    struct cuckoo_map *map = new_cuckoo_map(initial_size, 0, 0, 0, 0);
    assert(map != 0);
    assert(map->table1 != 0);
    assert(map->table2 != 0);
    assert(map->key_comparison == 0);
    assert(map->key_destructor == 0);
    assert(map->value_comparison == 0);
    assert(map->value_destructor == 0);
    assert(map->table_size == initial_size);
    assert(map->table_used == 0);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, 0, 0, 0);
    assert(map != 0);
    assert(map->table1 != 0);
    assert(map->table2 != 0);
    assert(map->key_comparison == comparison);
    assert(map->key_destructor == 0);
    assert(map->value_comparison == 0);
    assert(map->value_destructor == 0);
    assert(map->table_size == initial_size);
    assert(map->table_used == 0);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, 0, 0, comparison, 0);
    assert(map != 0);
    assert(map->table1 != 0);
    assert(map->table2 != 0);
    assert(map->key_comparison == 0);
    assert(map->key_destructor == 0);
    assert(map->value_comparison == comparison);
    assert(map->value_destructor == 0);
    assert(map->table_size == initial_size);
    assert(map->table_used == 0);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, 0, comparison2, 0);
    assert(map != 0);
    assert(map->table1 != 0);
    assert(map->table2 != 0);
    assert(map->key_comparison == comparison);
    assert(map->key_comparison != comparison2);
    assert(map->key_destructor == 0);
    assert(map->value_comparison == comparison2);
    assert(map->value_comparison != comparison);
    assert(map->value_destructor == 0);
    assert(map->table_size == initial_size);
    assert(map->table_used == 0);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, destructor, 0, 0);
    assert(map != 0);
    assert(map->table1 != 0);
    assert(map->table2 != 0);
    assert(map->key_comparison == comparison);
    assert(map->key_destructor == destructor);
    assert(map->value_comparison == 0);
    assert(map->value_destructor == 0);
    assert(map->table_size == initial_size);
    assert(map->table_used == 0);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, 0, 0, comparison, destructor);
    assert(map != 0);
    assert(map->table1 != 0);
    assert(map->table2 != 0);
    assert(map->key_comparison == 0);
    assert(map->key_destructor == 0);
    assert(map->value_comparison == comparison);
    assert(map->value_destructor == destructor);
    assert(map->table_size == initial_size);
    assert(map->table_used == 0);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, destructor, comparison2, destructor2);
    assert(map != 0);
    assert(map->table1 != 0);
    assert(map->table2 != 0);
    assert(map->key_comparison == comparison);
    assert(map->key_destructor == destructor);
    assert(map->value_comparison == comparison2);
    assert(map->value_destructor == destructor2);
    assert(map->table_size == initial_size);
    assert(map->table_used == 0);
    delete_cuckoo_map(map);

    return EXIT_SUCCESS;
}
