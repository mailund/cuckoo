
#include "cuckoo_internal.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


#pragma mark error handling functions
static inline bool is_power_of_two(size_t x) { return (x & (x - 1)) == 0; }
static inline void assertion(bool test, const char *error_string)
{
    if (test) return;
    fprintf(stderr, "ERROR: %s\n", error_string);
    exit(EXIT_FAILURE);
}
static inline void error(const char *error_string)
{
    fprintf(stderr, "ERROR: %s\n", error_string);
    exit(EXIT_FAILURE);
}


#pragma mark auxilary functions
static inline void bin_swap(struct table_bin *x, struct table_bin *y)
{
    struct table_bin tmp;
    tmp = *x; *x = *y; *y = tmp;
}


#pragma mark hash functions
// FIXME: this is a mock hash function id:17
//   
// ----
// <https://github.com/mailund/cuckoo/issues/15>
// Thomas Mailund
// mailund@birc.au.dk
static hash_index_type mock_new_hash_index(hash_index_type old)
{
    return old;
}
static void mock_free_hash_index(hash_index_type index)
{
    // do nothing
}
static hash_key_type mock_hash_func(application_key_type akey, hash_index_type index)
{
    return akey;
}



#pragma mark cuckoo table



void cuckoo_configure_hash_family(struct cuckoo_table *table,
                                  new_hash_index_func_type new_hash_index_func,
                                  free_hash_index_func_type free_hash_index_func,
                                  hash_func_type hash_func)
{
    table->new_hash_index_func = new_hash_index_func;
    table->free_hash_index_func = free_hash_index_func;
    table->hash_func = hash_func;
    
    // make new seeds by calling new index with null
    table->i1 = table->new_hash_index_func(0);
    table->i2 = table->new_hash_index_func(0);
}

struct cuckoo_table *
new_cuckoo_table(size_t initial_size,
                 equality_func_type value_equality,
                 destructor_func_type value_destructor)
{
    assertion(initial_size > 0,
              "Initial size cannot be zero.");
    assertion(is_power_of_two(initial_size),
              "Table size must be a power of two.");
    
    struct cuckoo_table *table = malloc(sizeof(struct cuckoo_table));
    
    table->table1 = malloc(initial_size * sizeof(struct table_bin));
    table->table2 = table->table1 + initial_size / 2;
    for (size_t i = 0; i < initial_size; ++i) {
        table->table1[i].tag = EMPTY;
    }
    
    table->value_equality = value_equality;
    table->value_destructor = value_destructor;
    table->table_size = initial_size;
    table->table_used = 0;
    
    // Using mock hash family right now...
    cuckoo_configure_hash_family(table,
                                 mock_new_hash_index,
                                 mock_free_hash_index,
                                 mock_hash_func);
    
    return table;
}

void
delete_cuckoo_table(struct cuckoo_table *set)
{
    if (set->value_destructor) {
        for (size_t i = 0; i < set->table_size; ++i) {
            struct table_bin *bin = &(set->table1[i]);
            if (bin->tag == OCCUPIED)
                set->value_destructor(bin->application_key);
        }
    }
    free(set->table1); // table2 is part of table1 when freeing.
    free(set);
}

inline static bool match_bin(struct table_bin *x, struct table_bin *y,
                             equality_func_type cmp)
{
    if (x->application_key != y->application_key) return false;
    if (cmp) return cmp(x->application_value, y->application_value);
    else return x->application_value == y->application_value;
}

static bool
cuckoo_table_contains_internal(struct cuckoo_table *set,
                               struct table_bin x)
{
    struct table_bin *tbl1_bin = &(set->table1[x.hash_key1]);
    struct table_bin *tbl2_bin = &(set->table2[x.hash_key2]);
    equality_func_type cmp = set->value_equality;
    
    return match_bin(&x, tbl1_bin, cmp) || match_bin(&x, tbl2_bin, cmp);
}

bool
cuckoo_table_contains(struct cuckoo_table *set,
                      application_key_type application_key,
                      application_value_type application_value)
{
    assertion(set->hash_func, "hash function not set!");
    assertion(set->i1, "hash function index not set for function 1");
    assertion(set->i2, "hash function index not set for function 1");
    
    struct table_bin x;
    x.application_key = application_key;
    x.application_value = application_value;
    x.hash_key1 = set->hash_func(application_key, set->i1);
    x.hash_key2 = set->hash_func(application_key, set->i2);
    
    assertion(x.hash_key1 < set->table_size, "Hash index out of range.");
    assertion(x.hash_key2 < set->table_size, "Hash index out of range.");
    
    return cuckoo_table_contains_internal(set, x);
}


static void
cuckoo_table_insert_internal(struct cuckoo_table *set,
                             struct table_bin x)
{
    // Get out if there is nothing to insert.
    if (cuckoo_table_contains_internal(set, x)) return;
    
    // FIXME: what should the threshold be? id:16
    //   
    // ----
    // <https://github.com/mailund/cuckoo/issues/14>
    // Thomas Mailund
    // mailund@birc.au.dk
    int attempt_threshold = set->table_size;

    struct table_bin *bin;
    for (int i = 0; i < attempt_threshold; ++i) {
        bin = &(set->table1[x.hash_key1]);
        if (bin->tag == EMPTY) { *bin = x; return; }
        bin_swap(&x, bin);
        
        bin = &(set->table2[x.hash_key2]);
        if (bin->tag == EMPTY) { *bin = x; return; }
        bin_swap(&x, bin);
    }
    
    // Dropping out here means we didn't manage to insert.
    cuckoo_table_rehash(set);
    cuckoo_table_insert_internal(set, x);
    
}

void
cuckoo_table_insert         (struct cuckoo_table *set,
                             application_key_type application_key,
                             application_value_type application_value)
{
    struct table_bin x;
    x.application_key = application_key;
    x.application_value = application_value;
    x.hash_key1 = set->hash_func(application_key, set->i1);
    x.hash_key2 = set->hash_func(application_key, set->i2);
    x.tag = OCCUPIED; // it will be when we are done.
    cuckoo_table_insert_internal(set, x);
    
}

void cuckoo_table_rehash(struct cuckoo_table *tbl)
{
    error("Rehashing is not implemented yet!");
}

