
#include "cuckoo_internal.h"

#include <stdlib.h>
#include <assert.h>

int main(int arvc, const char *argv[])
{
    struct cuckoo_set *set = new_cuckoo_set(5);
    assert(set == 0);
    return EXIT_SUCCESS;
}
