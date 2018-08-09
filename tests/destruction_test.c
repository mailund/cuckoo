
#include "cuckoo_internal.h"
#include "testing.h"

#include <stdlib.h>
#include <stdbool.h>

static const int table_size = 8;
static int keys[table_size / 2];
static int values[table_size / 2];

static void key_destructor(void *application_key)
{
    int key = (int)application_key;
    keys[key - 1] = 0;
}

static void value_destructor(void *application_value)
{
    int value = (int)application_value;
    values[-value - 1] = 0;
}

static void mockup_set_populate(struct cuckoo_set *set)
{
    for (int i = 0; i < table_size / 2; ++i) {
        keys[i] = i + 1;
        set->table0[i].tag = OCCUPIED;
        set->table0[i].application_key = keys[i];
    }
}

static bool check_set_keys(struct cuckoo_set *set)
{
    for (int i = 0; i < table_size / 2; ++i) {
        if (keys[i] != (int)set->table0[i].application_key)
            return false;
    }
    return true;
}

static void mockup_map_populate(struct cuckoo_map *map)
{
    for (int i = 0; i < table_size / 2; ++i) {
        keys[i] = i + 1;
        values[i] = -(i + 1);
        map->table0[i].tag = OCCUPIED;
        map->table0[i].application_key = keys[i];
        map->table0[i].application_value = values[i];
    }
}

static bool check_map_keys(struct cuckoo_map *map)
{
    for (int i = 0; i < table_size / 2; ++i) {
        if (keys[i] != (int)map->table0[i].application_key)
            return false;
    }
    return true;
}

static bool check_map_values(struct cuckoo_map *map)
{
    for (int i = 0; i < table_size / 2; ++i) {
        if (values[i] != (int)map->table0[i].application_value)
            return false;
    }
    return true;
}

static bool check_keys_deleted()
{
    for (int i = 0; i < table_size / 2; ++i) {
        if (keys[i] != 0) return false;
    }
    return true;
}

static bool check_values_deleted()
{
    for (int i = 0; i < table_size / 2; ++i) {
        if (values[i] != 0) return false;
    }
    return true;
}

int main(int argc, const char *argv[])
{
    struct cuckoo_set *set = new_cuckoo_set(table_size, 0, key_destructor);
    mockup_set_populate(set);
    assertion(check_set_keys(set));
    delete_cuckoo_set(set);
    assertion(check_keys_deleted());
    
    struct cuckoo_map *map = new_cuckoo_map(table_size, 0, key_destructor, 0, value_destructor);
    mockup_map_populate(map);
    assertion(check_map_keys(map));
    assertion(check_map_values(map));
    delete_cuckoo_map(map);
    assertion(check_keys_deleted());
    assertion(check_values_deleted());
    
    return EXIT_SUCCESS;
}
