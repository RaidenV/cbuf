#include "gtest/gtest.h"

#include "cbuf.h"

typedef struct
{
    double hello;
    double world;
    float  ukcuf;
    char   array[23];
} TestElem;

typedef struct
{
    uint32_t raz;
    uint32_t dva;
    uint32_t tre;
} TestElem2;

TestElem2* mkelem2(void)
{
    TestElem2* elem = (TestElem2*) malloc(sizeof(TestElem2));
    elem->raz = 0xAAAAAAAA;
    elem->dva = 0x55555555;
    elem->tre = 0x99999999;

    return elem;
}

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

TEST( CBuf, AddElem )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), 3  );

    TestElem* elem = mkelem();

    ASSERT_TRUE( cbuf_size(cbuf)     == 0 );
    ASSERT_TRUE( cbuf_capacity(cbuf) == 3 );

    cbuf_add( cbuf, elem );

    ASSERT_TRUE( cbuf_size(cbuf) == 1 );

    cbuf_destroy( cbuf );
}

TEST( CBuf, GetElem )
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

TEST( CBuf, GetMultiElem )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem2), 4  );
    TestElem2* elem = mkelem2();

    cbuf_add( cbuf, elem );
    cbuf_add( cbuf, elem );
    cbuf_add( cbuf, elem );
    cbuf_add( cbuf, elem );

    // At this point we are expecting the buffer to have rolled over.
    // This means that the head pointer has locked the stale data that
    // the user failed to read before the buffer circled
    ASSERT_TRUE( cbuf->head == 0 ); // head has returned to beginning of buffer
    ASSERT_TRUE( cbuf->tail == 1 ); // tail has been pushed forward

    TestElem elem_cmp = {};
    // get the second element in the buffer
    int size = cbuf_get( cbuf, &elem_cmp );

    ASSERT_EQ( size, 1 );

    // get the third element in the buffer
    size = cbuf_get( cbuf, &elem_cmp );

    ASSERT_EQ( size, 1 );

    // get the fourth element in the buffer
    size = cbuf_get( cbuf, &elem_cmp );

    ASSERT_EQ( size , 1 );

    // we've rolled back to the locked head (stale data, head pushed
    // tail)
    size = cbuf_get( cbuf, &elem_cmp );

    // expect that we would not have read anything, even if valid data
    // is located in this index
    ASSERT_EQ( size, 0 );

    cbuf_destroy( cbuf );
}

TEST( CBuf, Head )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), 3  );
    TestElem* elem = mkelem();
    cbuf_add( cbuf, elem );

    ASSERT_EQ( cbuf->head, 1 );

    cbuf_add( cbuf, elem );

    ASSERT_EQ( cbuf->head, 2 );

    cbuf_add( cbuf, elem );

    ASSERT_EQ( cbuf->head, 0 );
    ASSERT_TRUE( cbuf_is_full(cbuf));
    ASSERT_EQ( cbuf->tail, 1 );

    cbuf_destroy( cbuf );
}

TEST( CBuf, FullEmpty )
{
    CBuf* cbuf = cbuf_make( sizeof(TestElem), 3  );
    TestElem* elem = mkelem();
    cbuf_add( cbuf, elem );

    ASSERT_EQ( cbuf->head, 1 );
    cbuf_add( cbuf, elem );

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

    cbuf_destroy( cbuf );
}
