
#ifndef TESTING_H
#define TESTING_H

#include <stdlib.h>
#include <stdio.h>

#define assertion_msg(test, msg) \
    if (!(test)) { \
        fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, msg); \
        exit(EXIT_FAILURE); \
    }

#define assertion(test) \
    assertion_msg(test, #test)

#define warning(msg) \
    fprintf(stderr, "%s:%d: warning: %s\n", __FILE__, __LINE__, msg)

#define error(msg) \
    { \
        fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, msg); \
        exit(EXIT_FAILURE); \
    }

#endif
