
#include "cuckoo_internal.h"
#include "testing.h"

#include <stdlib.h>
#include <stdbool.h>

#define TABLE_SIZE 8
static long int keys[TABLE_SIZE / 2];

static void key_destructor(void *application_key)
{
    int key = (int)application_key;
    keys[key - 1] = 0;
}

static void mockup_set_populate(struct cuckoo_table *set)
{
    for (int i = 0; i < TABLE_SIZE / 2; ++i) {
        keys[i] = i + 1;
        set->table1[i].tag = OCCUPIED;
        set->table1[i].application_key = (void *)keys[i];
    }
}

static bool check_set_keys(struct cuckoo_table *set)
{
    for (int i = 0; i < TABLE_SIZE / 2; ++i) {
        if (keys[i] != (int)set->table1[i].application_key)
            return false;
    }
    return true;
}

static bool check_keys_deleted()
{
    for (int i = 0; i < TABLE_SIZE / 2; ++i) {
        if (keys[i] != 0) return false;
    }
    return true;
}

int main(int argc, const char *argv[])
{
    struct cuckoo_table *set = new_cuckoo_table(TABLE_SIZE, 0, key_destructor);
    mockup_set_populate(set);
    assertion(check_set_keys(set));
    delete_cuckoo_table(set);
    assertion(check_keys_deleted());
    
    return EXIT_SUCCESS;
}
