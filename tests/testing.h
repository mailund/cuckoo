
#ifndef TESTING_H
#define TESTING_H

#include <stdlib.h>
#include <stdio.h>

#define BASENAME_FILE (__FILE__ + SOURCE_PATH_SIZE)
#define assertion_msg(test, msg) \
    if (!(test)) { \
        fprintf(stderr, "Error %s:%d: %s\n", BASENAME_FILE, __LINE__, msg); \
        exit(EXIT_FAILURE); \
    }
#define assertion(test) \
    assertion_msg(test, #test)

#define warning(msg) \
    fprintf(stderr, "Warning %s:%d: %s\n", BASENAME_FILE, __LINE__, msg);

#define error(msg) \
    { \
        fprintf(stderr, "Error %s:%d: %s\n", BASENAME_FILE, __LINE__, msg); \
        exit(EXIT_FAILURE); \
    }

#endif
