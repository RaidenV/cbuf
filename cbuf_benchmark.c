/*
 * Simple benchmark for adding and retrieving elements from the
 * circular buffer.
 */

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "cbuf.h"

// it's all my VM memory can hold!
#define TEST_SZ (uint64_t)1e6

typedef struct
{
    uint32_t header;
    uint32_t elem[128];
}TestElem;

TestElem* make_test_elem( void )
{
    TestElem* elem = (TestElem*)malloc(sizeof(TestElem));
    elem->header = 1;

    for ( uint8_t i = 0; i < 128; ++i )
    {
        elem->elem[i] = i;
    }

    return elem;
}

double get_delta( struct timespec* start, struct timespec* end )
{
    return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec)/1e9;
}

int main( void )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), TEST_SZ);
    struct timespec start, end;

    TestElem* elems[TEST_SZ];

    for ( size_t i = 0; i < TEST_SZ; ++i )
    {
        elems[i] = make_test_elem();
    }

    clock_gettime(CLOCK_MONOTONIC, &start );
    for ( size_t i = 0; i < TEST_SZ; ++i )
    {
        cbuf_add( cbuf, elems[i] );
    }
    clock_gettime(CLOCK_MONOTONIC, &end );

    printf( "Total seconds, adding=%.6f\r\n", get_delta(&start, &end) );

    TestElem* output = (TestElem*)malloc(sizeof(TestElem));

    clock_gettime(CLOCK_MONOTONIC, &start );
    for ( size_t i = 0; i < TEST_SZ; ++i )
    {
        cbuf_get( cbuf, output );
    }
    clock_gettime(CLOCK_MONOTONIC, &end );

    printf( "Total seconds, retreiving=%.6f\r\n", get_delta(&start, &end) );
}
