
#include "cuckoo_internal.h"
#include "testing.h"

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

void test_empty_set(struct cuckoo_set *set)
{
    for (size_t i = 0; i < set->table_size; ++i) {
        assertion(set->table0[i].tag == EMPTY);
    }
}

void test_empty_map(struct cuckoo_map *map)
{
    for (size_t i = 0; i < map->table_size; ++i) {
        assertion(map->table0[i].tag == EMPTY);
    }
}

int main(int arvc, const char *argv[])
{
    size_t initial_size = 8;
    
    struct cuckoo_set *set = new_cuckoo_set(initial_size, 0, 0);
    assertion(set != 0);
    assertion(set->table0 != 0);
    assertion(set->table1 != 0);
    assertion(set->key_comparison == 0);
    assertion(set->key_destructor == 0);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_set(set);
    
    set = new_cuckoo_set(initial_size, comparison, 0);
    assertion(set != 0);
    assertion(set->table0 != 0);
    assertion(set->table1 != 0);
    assertion(set->key_comparison == comparison);
    assertion(set->key_destructor == 0);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_set(set);
    
    set = new_cuckoo_set(initial_size, 0, destructor);
    assertion(set != 0);
    assertion(set->table0 != 0);
    assertion(set->table1 != 0);
    assertion(set->key_comparison == 0);
    assertion(set->key_destructor == destructor);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_set(set);
    
    set = new_cuckoo_set(initial_size, comparison, destructor);
    assertion(set != 0);
    assertion(set->table0 != 0);
    assertion(set->table1 != 0);
    assertion(set->key_comparison == comparison);
    assertion(set->key_destructor == destructor);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_set(set);
    
    struct cuckoo_map *map = new_cuckoo_map(initial_size, 0, 0, 0, 0);
    assertion(map != 0);
    assertion(map->table0 != 0);
    assertion(map->table1 != 0);
    assertion(map->key_comparison == 0);
    assertion(map->key_destructor == 0);
    assertion(map->value_comparison == 0);
    assertion(map->value_destructor == 0);
    assertion(map->table_size == initial_size);
    assertion(map->table_used == 0);
    test_empty_map(map);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, 0, 0, 0);
    assertion(map != 0);
    assertion(map->table0 != 0);
    assertion(map->table1 != 0);
    assertion(map->key_comparison == comparison);
    assertion(map->key_destructor == 0);
    assertion(map->value_comparison == 0);
    assertion(map->value_destructor == 0);
    assertion(map->table_size == initial_size);
    assertion(map->table_used == 0);
    test_empty_map(map);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, 0, 0, comparison, 0);
    assertion(map != 0);
    assertion(map->table0 != 0);
    assertion(map->table1 != 0);
    assertion(map->key_comparison == 0);
    assertion(map->key_destructor == 0);
    assertion(map->value_comparison == comparison);
    assertion(map->value_destructor == 0);
    assertion(map->table_size == initial_size);
    assertion(map->table_used == 0);
    test_empty_map(map);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, 0, comparison2, 0);
    assertion(map != 0);
    assertion(map->table0 != 0);
    assertion(map->table1 != 0);
    assertion(map->key_comparison == comparison);
    assertion(map->key_comparison != comparison2);
    assertion(map->key_destructor == 0);
    assertion(map->value_comparison == comparison2);
    assertion(map->value_comparison != comparison);
    assertion(map->value_destructor == 0);
    assertion(map->table_size == initial_size);
    assertion(map->table_used == 0);
    test_empty_map(map);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, destructor, 0, 0);
    assertion(map != 0);
    assertion(map->table0 != 0);
    assertion(map->table1 != 0);
    assertion(map->key_comparison == comparison);
    assertion(map->key_destructor == destructor);
    assertion(map->value_comparison == 0);
    assertion(map->value_destructor == 0);
    assertion(map->table_size == initial_size);
    assertion(map->table_used == 0);
    test_empty_map(map);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, 0, 0, comparison, destructor);
    assertion(map != 0);
    assertion(map->table0 != 0);
    assertion(map->table1 != 0);
    assertion(map->key_comparison == 0);
    assertion(map->key_destructor == 0);
    assertion(map->value_comparison == comparison);
    assertion(map->value_destructor == destructor);
    assertion(map->table_size == initial_size);
    assertion(map->table_used == 0);
    test_empty_map(map);
    delete_cuckoo_map(map);

    map = new_cuckoo_map(initial_size, comparison, destructor, comparison2, destructor2);
    assertion(map != 0);
    assertion(map->table0 != 0);
    assertion(map->table1 != 0);
    assertion(map->key_comparison == comparison);
    assertion(map->key_destructor == destructor);
    assertion(map->value_comparison == comparison2);
    assertion(map->value_destructor == destructor2);
    assertion(map->table_size == initial_size);
    assertion(map->table_used == 0);
    test_empty_map(map);
    delete_cuckoo_map(map);

    return EXIT_SUCCESS;
}
