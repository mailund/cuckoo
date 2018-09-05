
#ifndef CUCKOO_INTERNAL_H
#define CUCKOO_INTERNAL_H

#include "cuckoo.h"

typedef size_t hash_key_type;
typedef void *hash_index_type; // Index here means into the family of functions

typedef hash_index_type (*new_hash_index_func_type) (hash_index_type old_index);
typedef void            (*free_hash_index_func_type)(hash_index_type old_index);
typedef hash_key_type   (*hash_func_type)(application_key_type app_key,
                                          hash_index_type hash_index);

struct table_bin {
    // I use tags so I don't have to make assumptions
    // on what application keys might be. Otherwise, I would
    // have to assume that an application key is not null.
    enum { EMPTY, OCCUPIED } tag;
    
    hash_key_type hash_key1;
    hash_key_type hash_key2;
    
    application_key_type application_key;
    application_value_type application_value;
};

struct cuckoo_table {
    struct table_bin *table1;
    struct table_bin *table2;
    size_t table_size;
    size_t table_used;
    
    hash_index_type i1;
    hash_index_type i2;
    
    // These are pointers kept in the table just in case
    // you want to change them. Otherwise, they could just
    // as easily be static in the source file.
    new_hash_index_func_type new_hash_index_func;
    free_hash_index_func_type free_hash_index_func;
    hash_func_type hash_func;
    
    destructor_func_type value_destructor;
    equality_func_type value_equality;
};

void
cuckoo_configure_hash_family(struct cuckoo_table *table,
                             new_hash_index_func_type new_hash_index_func,
                             free_hash_index_func_type free_hash_index_func,
                             hash_func_type hash_func);

void
cuckoo_table_rehash          (struct cuckoo_table *tbl);

#endif

