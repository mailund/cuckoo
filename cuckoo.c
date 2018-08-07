
#import "cuckoo.h"

#pragma mark cuckoo sets
struct set_bin {
    hash_key_type hash_key;
    void * application_key;
};

struct cuckoo_set {
    destructor_type key_destructor;
    comparison_type key_comparison;
};

struct cuckoo_set *new_cuckoo_set(size_t initial_size)
{
    struct cuckoo_set *table = malloc(sizeof(struct cuckoo_set));
    return table;
}

#pragma mark cuckoo maps
struct map_bin {
    hash_key_type hash_key;
    void * application_key;
    void * application_value;
};

struct cuckoo_map {
    destructor_type key_destructor;
    comparison_type key_comparison;
    destructor_type value_destructor;
    comparison_type value_comparison;
};

