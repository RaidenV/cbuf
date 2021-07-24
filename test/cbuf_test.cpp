#include "gtest/gtest.h"

#include "cbuf.h"

typedef struct
{
    double hello;
    double world;
    float  ukcuf;
    char   array[23];
} TestElem;

TestElem* mkelem( void )
{
    TestElem* elem = (TestElem*) malloc(sizeof(TestElem));
    elem->hello = 2;
    elem->world = 3;
    elem->ukcuf = 123.456;

    const char* test_str = "I am working!\0";

    strcpy(elem->array, test_str);

    return elem;
}

TEST( CRing, AddElem )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), 3  );

    TestElem* elem = mkelem();

    ASSERT_TRUE( cbuf_size(cbuf)     == 0 );
    ASSERT_TRUE( cbuf_capacity(cbuf) == 3 );

    cbuf_add( cbuf, elem );

    ASSERT_TRUE( cbuf_size(cbuf) == 1 );

    cbuf_destroy( cbuf );
}

TEST( CRing, GetElem )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), 3  );
    TestElem* elem = mkelem();

    cbuf_add( cbuf, elem );

    TestElem elem_cmp = {};
    int size = cbuf_get( cbuf, &elem_cmp );

    ASSERT_TRUE( size == 1 );
    ASSERT_TRUE( elem->hello == elem_cmp.hello );
    ASSERT_TRUE( elem->world == elem_cmp.world );
    ASSERT_TRUE( elem->ukcuf == elem_cmp.ukcuf );
    ASSERT_EQ  ( strcasecmp( elem->array, elem_cmp.array ), 0 );

    cbuf_destroy( cbuf );
}

TEST( CRing, Head )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), 3  );
    TestElem* elem1 = mkelem();
    TestElem* elem2 = mkelem();
    TestElem* elem3 = mkelem();
    cbuf_add( cbuf, elem1 );

    ASSERT_EQ( cbuf->head, 1 );

    cbuf_add( cbuf, elem2 );

    ASSERT_EQ( cbuf->head, 2 );

    cbuf_add( cbuf, elem3 );

    ASSERT_EQ( cbuf->head, 0 );
    ASSERT_TRUE( cbuf_is_full(cbuf));
    ASSERT_EQ( cbuf->tail, 1 );
}

TEST( CRing, FullEmpty )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), 3  );
    TestElem* elem1 = mkelem();
    TestElem* elem2 = mkelem();
    cbuf_add( cbuf, elem1 );

    ASSERT_EQ( cbuf->head, 1 );
    cbuf_add( cbuf, elem2 );

    ASSERT_EQ  ( cbuf->head, 2 );
    ASSERT_TRUE( cbuf_is_full(cbuf));

    TestElem* elem_get = mkelem();
    int size = cbuf_get( cbuf, elem_get );

    ASSERT_FALSE( cbuf_is_full(cbuf) );
    ASSERT_EQ   ( size, 1 );

    size = cbuf_get( cbuf, elem_get );

    ASSERT_FALSE( cbuf_is_full(cbuf) );
    ASSERT_EQ   ( size, 1 );

    size = cbuf_get( cbuf, elem_get );

    ASSERT_FALSE( cbuf_is_full(cbuf) );
    ASSERT_EQ   ( size, 0 );

    ASSERT_TRUE( cbuf_is_empty(cbuf) );
    ASSERT_EQ  ( cbuf_size(cbuf), 0 );
}
