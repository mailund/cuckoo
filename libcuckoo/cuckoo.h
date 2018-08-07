
#ifndef CUCKOO_H
#define CUCKOO_H

#include <stdlib.h>

typedef size_t hash_key_type;
struct cuckoo_set;
struct cuckoo_map;

typedef void (*destructor_type)(void *);
typedef void (*comparison_type)(void *);

struct cuckoo_set *new_cuckoo_set(size_t initial_size);

#endif


