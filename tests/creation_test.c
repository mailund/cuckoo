
#include "cuckoo_internal.h"
#include "testing.h"

static bool equality(void *key1, void *key2)
{
    return false;
}

static void destructor(void *key)
{
    return;
}

static void test_empty_set(struct cuckoo_table *set)
{
    for (size_t i = 0; i < set->table_size; ++i) {
        assertion(set->table1[i].tag == EMPTY);
    }
}


int main(int arvc, const char *argv[])
{
    size_t initial_size = 8;
    
    struct cuckoo_table *set = new_cuckoo_table(initial_size, 0, 0);
    assertion(set != 0);
    assertion(set->table1 != 0);
    assertion(set->table1 != 0);
    assertion(set->value_equality == 0);
    assertion(set->value_destructor == 0);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_table(set);
    
    set = new_cuckoo_table(initial_size, equality, 0);
    assertion(set != 0);
    assertion(set->table1 != 0);
    assertion(set->table2 != 0);
    assertion(set->value_equality == equality);
    assertion(set->value_destructor == 0);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_table(set);
    
    set = new_cuckoo_table(initial_size, 0, destructor);
    assertion(set != 0);
    assertion(set->table1 != 0);
    assertion(set->table2 != 0);
    assertion(set->value_equality == 0);
    assertion(set->value_destructor == destructor);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_table(set);
    
    set = new_cuckoo_table(initial_size, equality, destructor);
    assertion(set != 0);
    assertion(set->table1 != 0);
    assertion(set->table2 != 0);
    assertion(set->value_equality == equality);
    assertion(set->value_destructor == destructor);
    assertion(set->table_size == initial_size);
    assertion(set->table_used == 0);
    test_empty_set(set);
    delete_cuckoo_table(set);
    
    return EXIT_SUCCESS;
}
