
#import "cuckoo_internal.h"

#pragma mark cuckoo sets

struct cuckoo_set *new_cuckoo_set(size_t initial_size,
                                  comparison_type key_comparison,
                                  destructor_type key_destructor)
{
    struct cuckoo_set *table = malloc(sizeof(struct cuckoo_set));
    
    table->table1 = malloc(initial_size * sizeof(struct set_bin));
    table->table2 = malloc(initial_size * sizeof(struct set_bin));
    for (size_t i = 0; i < initial_size; ++i) {
        table->table1[i].tag = EMPTY;
        table->table2[i].tag = EMPTY;
    }
    
    table->key_comparison = key_comparison;
    table->key_destructor = key_destructor;
    table->table_size = initial_size;
    table->table_used = 0;
    
    return table;
}

void delete_cuckoo_set(struct cuckoo_set *set)
{
    free(set->table1);
    free(set->table2);
    free(set);
}


#pragma mark cuckoo maps

struct cuckoo_map *new_cuckoo_map(size_t initial_size,
                                  comparison_type key_comparison,
                                  destructor_type key_destructor,
                                  comparison_type value_comparison,
                                  destructor_type value_destructor)
{
    struct cuckoo_map *table = malloc(sizeof(struct cuckoo_map));
    
    table->table1 = malloc(initial_size * sizeof(struct map_bin));
    table->table2 = malloc(initial_size * sizeof(struct map_bin));
    for (size_t i = 0; i < initial_size; ++i) {
        table->table1[i].tag = EMPTY;
        table->table2[i].tag = EMPTY;
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
    free(map->table1);
    free(map->table2);
    free(map);
}
