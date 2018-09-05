
#ifndef CUCKOO_H
#define CUCKOO_H

#include <stdlib.h>
#include <stdbool.h>

typedef size_t application_key_type;
typedef void * application_value_type;

/*
 These two functions are used to configure how application values are handled.
 
 Application keys are always assumed to be computer words that fit into size_t,
 and the user must make sure to map any application data to a key.
 The corresponding application values can be any type, and these two functions deal
 with that.
 
 The destructor deletes application values; if you do not provide it,
 they will not be freed (so they should either not need to be freed or be
 freed elsewhere).
 
 The comparison function can be used to test equality. If it is not provided,
 then actual equallity (==) is used to compare the application values. 
 */
typedef void (*destructor_func_type)(application_value_type);
typedef bool (*equality_func_type)  (application_value_type,
                                     application_value_type);

struct cuckoo_table;

struct cuckoo_table *
new_cuckoo_table            (size_t                  initial_size,
                             equality_func_type      value_equality,
                             destructor_func_type    value_destructor);
void
delete_cuckoo_table         (struct cuckoo_table    *set);

bool
cuckoo_table_contains       (struct cuckoo_table    *set,
                             application_key_type    application_key,
                             application_value_type  application_value);
void
cuckoo_table_insert         (struct cuckoo_table    *set,
                             application_key_type    application_key,
                             application_value_type  application_value);

#endif


