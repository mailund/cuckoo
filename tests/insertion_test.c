
#include "cuckoo_internal.h"
#include "testing.h"

int main(int argc, const char *argv[])
{
    const size_t initial_size = 8;
    struct cuckoo_set *set = new_cuckoo_set(initial_size, 0, 0);
    assertion_msg(set != 0, "Failed to create set.");
    
    warning("Test not implemented yet.");
    
    return EXIT_SUCCESS;
}
