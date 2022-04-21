#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "cbuf.h"

CBuf* cbuf_make( size_t elem_sz, size_t len )
{
    assert( len > 1 );

    CBuf* buf = (CBuf*)malloc(sizeof(CBuf));

    buf->data = (char*)malloc( elem_sz * len );
    memset( buf->data, 0, elem_sz * len );
    buf->head    = 0;
    buf->tail    = 0;
    buf->elem_sz = elem_sz;
    buf->len     = len;

    return buf;
}

void cbuf_destroy( CBuf* buf )
{
    if ( buf->data )
    {
        free( buf->data );
        free( buf );
    }
}

size_t cbuf_capacity( CBuf* buf  )
{
    return buf->len;
}

size_t cbuf_size( CBuf* buf )
{
    if ( buf->head < buf->tail )
    {
        return buf->head + (buf->len - buf->tail);
    }

    return buf->head - buf->tail;
}

void cbuf_add( CBuf* buf, void* data )
{
    memcpy( (buf->data + buf->head * buf->elem_sz), data, buf->elem_sz );
    buf->head = (buf->head + 1) % buf->len;

    // if we're going to overwrite the oldest element,
    // simply increase the location of that oldest element
    // so that we can maintain a circular buffer where
    // full  = ( head - tail ) == 1||len-1 (depending on location of head/tail)
    // empty = ( head - tail ) == 0
    if ( buf->head == buf->tail )
    {
        buf->tail = (++buf->tail) % buf->len;
    }

#if CBUF_DEBUG
    fprintf(stderr, "Adding...Head=%zu Tail=%zu\r\n", buf->head, buf->tail );
    for ( int i = 0; i < buf->elem_sz * buf->len; ++i )
    {
        fprintf( stderr, "%02X ", (unsigned char)buf->data[i] );
        if ( !((i+1) % 12))
            fprintf(stderr, "\r\n" );
    }

    fprintf( stderr, "\r\n\r\n");
#endif
}

size_t cbuf_get( CBuf* buf, void* data )
{
    if ( buf->head == buf->tail ) return 0;

    memcpy( data, (buf->data + buf->tail * buf->elem_sz), buf->elem_sz );
    memset( buf->data + (buf->tail * buf->elem_sz), 0, buf->elem_sz );

    // if the buffer is not empty
    if ( buf->head != buf->tail )
        // shift tail to the next value
        buf->tail = (buf->tail + 1) % buf->len;

#ifdef CBUF_DEBUG
    fprintf( stderr, "Removing...Head=%zu Tail=%zu Empty=%d\r\n",
             buf->head, buf->tail, cbuf_is_empty(buf) );
    for ( int i = 0; i < buf->elem_sz * buf->len; ++i )
    {
        fprintf( stderr, "%02X ", (unsigned char)buf->data[i] );
        if ( !((i+1) % 12))
            fprintf(stderr, "\r\n" );
    }

    fprintf( stderr, "\r\n\r\n");
#endif

    return 1;
}

uint8_t cbuf_is_empty( CBuf* buf )
{
    return buf->head == buf->tail;
}

uint8_t cbuf_is_full( CBuf* buf )
{
    // if the head is less than the tail, then the only
    // indication that it is full is if the difference between
    // the two is equal to 1.  This is because we continue to push
    // the tail of the buffer if the buffer is added to beyond its
    // capacity
    if ( buf->head < buf->tail )
    {
        return ( (buf->tail - buf->head) == 1 );
    }

    // The only case wherein the head can be larger than the tail
    // and the buffer can be full is if tail is pointed at element
    // 0 and the difference between the two is the length of the buffer
    // less 1.
    return ( (buf->head - buf->tail) == buf->len - 1);
}
