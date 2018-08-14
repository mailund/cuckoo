
#include "cuckoo_internal.h"
#include "testing.h"

#define NO_KEYS 7
#define TABLE_SIZE 16
static hash_key_type hash_keys[NO_KEYS];
static hash_key_type hash_keys0[NO_KEYS];
static hash_key_type hash_keys1[NO_KEYS];

static int a0 = 4, b0 = 8;
static int a1 = 10, b1 = 1;
// the hash bins will be:
/*
    HASH0(0l) == 8l -- HASH1(0l) == 1l.
    HASH0(1l) == 12l -- HASH1(1l) == 11l.
    HASH0(2l) == 0l -- HASH1(2l) == 5l.
    HASH0(3l) == 4l -- HASH1(3l) == 15l.
    HASH0(4l) == 8l -- HASH1(4l) == 9l.
    HASH0(5l) == 12l -- HASH1(5l) == 3l.
    HASH0(6l) == 0l -- HASH1(6l) == 13l.
*/
// FIXME: I don't actually test how it deals with
// collisions on the second hash function...

static void initialise_hash_keys()
{
    for (int i = 0; i < NO_KEYS; ++i) {
        hash_keys[i] = i;
        hash_keys0[i] = (a0*i + b0) % TABLE_SIZE;
        hash_keys1[i] = (a1*i + b1) % TABLE_SIZE;
    }
}

static void test_empty_set(struct cuckoo_set *set)
{
    for (size_t i = 0; i < set->table_size; ++i) {
        assertion(set->table0[i].tag == EMPTY);
    }
}

static void test_empty_map(struct cuckoo_map *map)
{
    for (size_t i = 0; i < map->table_size; ++i) {
        assertion(map->table0[i].tag == EMPTY);
    }
}

int main(int argc, const char *argv[])
{
    int dummy_key = 0;
    initialise_hash_keys();
    
    struct cuckoo_set *set = new_cuckoo_set(TABLE_SIZE, 0, 0);
    assertion_msg(set != 0, "Failed to create set.");

    // Make sure we start with a clean slate
    test_empty_set(set);
    
    // With a0 zero, all keys are mapped to b0, i.e. zero as well
    set->a0 = a0; set->b0 = b0;
    // With a1 one, all keys are mapped to their initial values
    set->a1 = a1; set->b1 = b1;
    
    // With this reasoning, we should have:
    log("Checking initial state.");
    for (int i = 0; i < NO_KEYS; ++i) {
        assertion(hash_keys0[i] == HASH0(set, hash_keys[i]));
        assertion(hash_keys1[i] == HASH1(set, hash_keys[i]));
        printf("HASH0(%zul) == %zul -- HASH1(%zul) == %zul.\n",
               hash_keys[i], hash_keys0[i], hash_keys[i], hash_keys1[i]);
    }
    
    // insert first -- gets into table0[0]
    log("Inserting first key.");
    /*
     HASH0(0l) == 8l -- HASH1(0l) == 1l.
     => table0[8] = 0
     */
    cuckoo_set_insert(set, hash_keys[0], &dummy_key);
    assertion(set->table0[8].tag == OCCUPIED);
    assertion(set->table0[8].hash_key ==0l);
    
    // insert second -- replaces table0[0] and moves first to table1[1]
    log("Inserting second key.");
    /*
     HASH0(0l) == 8l -- HASH1(0l) == 1l.
     HASH0(1l) == 12l -- HASH1(1l) == 11l.
     => table0[ 8] == 0
        table0[12] == 1
     */
    cuckoo_set_insert(set, hash_keys[1], &dummy_key);
    assertion(set->table0[ 8].tag == OCCUPIED);
    assertion(set->table0[ 8].hash_key == 0l);
    assertion(set->table0[12].tag == OCCUPIED);
    assertion(set->table0[12].hash_key == 1l);
    
    // insert third -- replaces table0[0] and moves second to table1[1]
    log("Inserting third key.");
    /*
     HASH0(0l) == 8l -- HASH1(0l) == 1l.
     HASH0(1l) == 12l -- HASH1(1l) == 11l.
     HASH0(2l) == 0l -- HASH1(2l) == 5l.
     => table0[ 0] == 2
        table0[ 8] == 0
        table0[12] == 1
     */
    cuckoo_set_insert(set, hash_keys[2], &dummy_key);
    assertion(set->table0[ 0].tag == OCCUPIED);
    assertion(set->table0[ 0].hash_key == 2l);
    assertion(set->table0[ 8].tag == OCCUPIED);
    assertion(set->table0[ 8].hash_key == 0l);
    assertion(set->table0[12].tag == OCCUPIED);
    assertion(set->table0[12].hash_key == 1l);
    
    log("Inserting fourth key.");
    /*
     HASH0(0l) == 8l -- HASH1(0l) == 1l.
     HASH0(1l) == 12l -- HASH1(1l) == 11l.
     HASH0(2l) == 0l -- HASH1(2l) == 5l.
     HASH0(3l) == 4l -- HASH1(3l) == 15l.
     => table0[ 0] == 2
        table0[ 4] == 3
        table0[ 8] == 0
        table0[12] == 1
     */
    cuckoo_set_insert(set, hash_keys[3], &dummy_key);
    assertion(set->table0[ 0].tag == OCCUPIED);
    assertion(set->table0[ 0].hash_key == 2l);
    assertion(set->table0[ 4].tag == OCCUPIED);
    assertion(set->table0[ 4].hash_key == 3l);
    assertion(set->table0[ 8].tag == OCCUPIED);
    assertion(set->table0[ 8].hash_key == 0l);
    assertion(set->table0[12].tag == OCCUPIED);
    assertion(set->table0[12].hash_key == 1l);
    
    log("Inserting fifth key.");
    /*
     HASH0(0l) == 8l -- HASH1(0l) == 1l.
     HASH0(1l) == 12l -- HASH1(1l) == 11l.
     HASH0(2l) == 0l -- HASH1(2l) == 5l.
     HASH0(3l) == 4l -- HASH1(3l) == 15l.
     HASH0(4l) == 8l -- HASH1(4l) == 9l.
     => table0[ 0] == 2
        table0[ 4] == 3
        table0[ 8] == 4
        table0[12] == 1
        table1[ 1] == 0
     */
    cuckoo_set_insert(set, hash_keys[4], &dummy_key);
    assertion(set->table0[ 0].tag == OCCUPIED);
    assertion(set->table0[ 0].hash_key == 2l);
    assertion(set->table0[ 4].tag == OCCUPIED);
    assertion(set->table0[ 4].hash_key == 3l);
    assertion(set->table0[ 8].tag == OCCUPIED);
    assertion(set->table0[ 8].hash_key == 4l);
    assertion(set->table0[12].tag == OCCUPIED);
    assertion(set->table0[12].hash_key == 1l);
    assertion(set->table1[ 1].tag == OCCUPIED);
    assertion(set->table1[ 1].hash_key == 0l);
    
    log("Inserting sixth key.");
    cuckoo_set_insert(set, hash_keys[5], &dummy_key);
    /*
     HASH0(0l) == 8l -- HASH1(0l) == 1l.
     HASH0(1l) == 12l -- HASH1(1l) == 11l.
     HASH0(2l) == 0l -- HASH1(2l) == 5l.
     HASH0(3l) == 4l -- HASH1(3l) == 15l.
     HASH0(4l) == 8l -- HASH1(4l) == 9l.
     HASH0(5l) == 12l -- HASH1(5l) == 3l.
     => table0[ 0] == 2
        table0[ 4] == 3
        table0[ 8] == 4
        table0[12] == 5
        table1[ 1] == 0
        table1[11] == 1
     */
    assertion(set->table0[ 0].tag == OCCUPIED);
    assertion(set->table0[ 0].hash_key == 2l);
    assertion(set->table0[ 4].tag == OCCUPIED);
    assertion(set->table0[ 4].hash_key == 3l);
    assertion(set->table0[ 8].tag == OCCUPIED);
    assertion(set->table0[ 8].hash_key == 4l);
    assertion(set->table0[12].tag == OCCUPIED);
    assertion(set->table0[12].hash_key == 5l);
    assertion(set->table1[ 1].tag == OCCUPIED);
    assertion(set->table1[ 1].hash_key == 0l);
    assertion(set->table1[11].tag == OCCUPIED);
    assertion(set->table1[11].hash_key == 1l);
    
    log("Inserting seventh key.");
    cuckoo_set_insert(set, hash_keys[6], &dummy_key);
    /*
     HASH0(0l) == 8l -- HASH1(0l) == 1l.
     HASH0(1l) == 12l -- HASH1(1l) == 11l.
     HASH0(2l) == 0l -- HASH1(2l) == 5l.
     HASH0(3l) == 4l -- HASH1(3l) == 15l.
     HASH0(4l) == 8l -- HASH1(4l) == 9l.
     HASH0(5l) == 12l -- HASH1(5l) == 3l.
     HASH0(6l) == 0l -- HASH1(6l) == 13l.
     => table0[ 0] == 6
        table0[ 4] == 3
        table0[ 8] == 4
        table0[12] == 5
        table1[ 1] == 0
        table1[ 5] == 2
        table1[11] == 1
     */
    assertion(set->table0[ 0].tag == OCCUPIED);
    assertion(set->table0[ 0].hash_key == 6l);
    assertion(set->table0[ 4].tag == OCCUPIED);
    assertion(set->table0[ 4].hash_key == 3l);
    assertion(set->table0[ 8].tag == OCCUPIED);
    assertion(set->table0[ 8].hash_key == 4l);
    assertion(set->table0[12].tag == OCCUPIED);
    assertion(set->table0[12].hash_key == 5l);
    assertion(set->table1[ 1].tag == OCCUPIED);
    assertion(set->table1[ 1].hash_key == 0l);
    assertion(set->table1[ 5].tag == OCCUPIED);
    assertion(set->table1[ 5].hash_key == 2l);
    assertion(set->table1[11].tag == OCCUPIED);
    assertion(set->table1[11].hash_key == 1l);
    
    return EXIT_SUCCESS;
}
