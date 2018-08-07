
#import "cuckoo_internal.h"

#pragma mark cuckoo sets
struct cuckoo_set *new_cuckoo_set(size_t initial_size)
{
    struct cuckoo_set *table = malloc(sizeof(struct cuckoo_set));
    return table;
}

#pragma mark cuckoo maps

