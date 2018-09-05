
#include "cuckoo_internal.h"
#include "testing.h"

static const int N = 8;
static const int TABLE_SIZE = 2 * N;

// App values are letters 'A' to 'E'. App keys are
application_key_type app_keys[] = { 0, 1, 2, 3, 4 };

// FIXME: mock up a hash function id:18
//   
// ----
// <https://github.com/mailund/cuckoo/issues/16>
// Thomas Mailund
// mailund@birc.au.dk
hash_key_type keys1[] = { 5, 1, 5, 1, 5 };
hash_key_type keys2[] = { 2, 2, 4, 6, 6 };
static int hack = 0;
static hash_index_type mock_new_hash_index(hash_index_type old)
{
    hack++;
    if (hack == 1)      return keys1;
    else if (hack == 2) return keys2;
    else assertion("Unexpected call to reset hash index!");
    return old;
}
static void mock_free_hash_index(hash_index_type index)
{
    // do nothing
}
static hash_key_type mock_hash_func(application_key_type appkey, hash_index_type index)
{
    hash_key_type *keys = (hash_key_type*)index;
    return keys[appkey];
}


static void test_empty_set(struct cuckoo_table *set)
{
    for (size_t i = 0; i < set->table_size; ++i) {
        assertion(set->table1[i].tag == EMPTY);
    }
}

static void check_config_A(struct cuckoo_table *tbl)
{
    struct table_bin *bin;
    for (int i = 0; i < N; ++i) {
        bin = tbl->table1 + i;
        if (i == 5) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 0);
            assertion(((char)bin->application_value) == 'A');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
    for (int i = 0; i < N; ++i) {
        bin = tbl->table2 + i;
        assertion(bin->tag == EMPTY);
    }
}

static void check_config_B(struct cuckoo_table *tbl)
{
    struct table_bin *bin;
    for (int i = 0; i < N; ++i) {
        bin = tbl->table1 + i;
        if (i == 1) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 1);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 1);
            assertion(((char)bin->application_value) == 'B');
        } else if (i == 5) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 0);
            assertion(((char)bin->application_value) == 'A');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
    for (int i = 0; i < N; ++i) {
        bin = tbl->table2 + i;
        assertion(bin->tag == EMPTY);
    }
}

static void check_config_C(struct cuckoo_table *tbl)
{
    struct table_bin *bin;
    for (int i = 0; i < N; ++i) {
        bin = tbl->table1 + i;
        if (i == 1) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 1);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 1);
            assertion(((char)bin->application_value) == 'B');
        } else if (i == 5) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 4);
            assertion(bin->application_key == 2);
            assertion(((char)bin->application_value) == 'C');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
    for (int i = 0; i < N; ++i) {
        bin = tbl->table2 + i;
        if (i == 2) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 0);
            assertion(((char)bin->application_value) == 'A');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
}

static void check_config_D(struct cuckoo_table *tbl)
{
    struct table_bin *bin;
    for (int i = 0; i < N; ++i) {
        bin = tbl->table1 + i;
        if (i == 1) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 1);
            assertion(bin->hash_key2 == 6);
            assertion(bin->application_key == 3);
            assertion(((char)bin->application_value) == 'D');
        } else if (i == 5) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 0);
            assertion(((char)bin->application_value) == 'A');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
    for (int i = 0; i < N; ++i) {
        bin = tbl->table2 + i;
        if (i == 2) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 1);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 1);
            assertion(((char)bin->application_value) == 'B');
        } else if (i == 4) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 4);
            assertion(bin->application_key == 2);
            assertion(((char)bin->application_value) == 'C');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
}

static void check_config_E(struct cuckoo_table *tbl)
{
    struct table_bin *bin;
    for (int i = 0; i < N; ++i) {
        bin = tbl->table1 + i;
        if (i == 1) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 1);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 1);
            assertion(((char)bin->application_value) == 'B');
        } else if (i == 5) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 6);
            assertion(bin->application_key == 4);
            assertion(((char)bin->application_value) == 'E');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
    for (int i = 0; i < N; ++i) {
        bin = tbl->table2 + i;
        if (i == 2) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 2);
            assertion(bin->application_key == 0);
            assertion(((char)bin->application_value) == 'A');
        } else if (i == 4) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 5);
            assertion(bin->hash_key2 == 4);
            assertion(bin->application_key == 2);
            assertion(((char)bin->application_value) == 'C');
        } else if (i == 6) {
            assertion(bin->tag == OCCUPIED);
            assertion(bin->hash_key1 == 1);
            assertion(bin->hash_key2 == 6);
            assertion(bin->application_key == 3);
            assertion(((char)bin->application_value) == 'D');
        } else {
            assertion(bin->tag == EMPTY);
        }
    }
}

static void test_set_insertions()
{
    struct cuckoo_table *set = new_cuckoo_table(TABLE_SIZE, 0, 0);
    test_empty_set(set);
    
    // hack the mock functions to make the tests work...
    cuckoo_configure_hash_family(set, mock_new_hash_index, mock_free_hash_index, mock_hash_func);
    
    
    assertion(!cuckoo_table_contains(set, 0, 'A'));
    assertion(!cuckoo_table_contains(set, 1, 'B'));
    assertion(!cuckoo_table_contains(set, 2, 'C'));
    assertion(!cuckoo_table_contains(set, 3, 'D'));
    assertion(!cuckoo_table_contains(set, 4, 'E'));
    
    cuckoo_table_insert(set, 0, 'A');
    assertion(cuckoo_table_contains(set, 0, 'A'));
    assertion(!cuckoo_table_contains(set, 1, 'B'));
    assertion(!cuckoo_table_contains(set, 2, 'C'));
    assertion(!cuckoo_table_contains(set, 3, 'D'));
    assertion(!cuckoo_table_contains(set, 4, 'E'));
    
    assertion(!cuckoo_table_contains(set, 1, 'A')); // not there with a different key
    assertion(!cuckoo_table_contains(set, 0, 'B')); // not there with different value
    
    check_config_A(set);
    
    cuckoo_table_insert(set, 1, 'B');
    assertion(cuckoo_table_contains(set, 0, 'A'));
    assertion(cuckoo_table_contains(set, 1, 'B'));
    assertion(!cuckoo_table_contains(set, 2, 'C'));
    assertion(!cuckoo_table_contains(set, 3, 'D'));
    assertion(!cuckoo_table_contains(set, 4, 'E'));
    check_config_B(set);
    
    cuckoo_table_insert(set, 2, 'C');
    assertion(cuckoo_table_contains(set, 0, 'A'));
    assertion(cuckoo_table_contains(set, 1, 'B'));
    assertion(cuckoo_table_contains(set, 2, 'C'));
    assertion(!cuckoo_table_contains(set, 3, 'D'));
    assertion(!cuckoo_table_contains(set, 4, 'E'));
    check_config_C(set);
    
    cuckoo_table_insert(set, 3, 'D');
    assertion(cuckoo_table_contains(set, 0, 'A'));
    assertion(cuckoo_table_contains(set, 1, 'B'));
    assertion(cuckoo_table_contains(set, 2, 'C'));
    assertion(cuckoo_table_contains(set, 3, 'D'));
    assertion(!cuckoo_table_contains(set, 4, 'E'));
    check_config_D(set);
    
    cuckoo_table_insert(set, 4, 'E');
    assertion(cuckoo_table_contains(set, 0, 'A'));
    assertion(cuckoo_table_contains(set, 1, 'B'));
    assertion(cuckoo_table_contains(set, 2, 'C'));
    assertion(cuckoo_table_contains(set, 3, 'D'));
    assertion(cuckoo_table_contains(set, 4, 'E'));
    check_config_E(set);
    
    delete_cuckoo_table(set);
    
}

int main(int argc, const char *argv[])
{
    test_set_insertions();
    return EXIT_SUCCESS;
}
