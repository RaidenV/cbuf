# Scope

Simple circular buffer developed for embedded systems.  The difference between this and other libraries/utilities is that this library does not rely on passing pointers to the circular buffer's elements back to the user, but rather copies them for the user.  This is meant to ensure that adding to this circular buffer in an ISR does not void or otherwise alter elements pointed to within the vector, as the user now has ownership over the retrieved item.  There is, however, still the case where you may be reading from the buffer while retrieving an element.  To ensure this doesn't happen, make the buffer an adequate size.

# Usage

The intended usage of this utility is as follows:

Define your type:

```
typedef struct
{
    double hello;
    double world;
} MyAwesomeStruct;
```

Create your buffer with the desired number of elements:

```
CBuf* cbuf = cbuf_make( sizeof(MyAwesomeStruct), 100 ); 
```

Add elements:

```
cbuf_add( cbuf, my_awesome_struct_pointer );
cbuf_add( cbuf, &my_awesome_struct );
```

Get elements:

```
MyAwesomeStruct  mas = {};
MyAwesomeStruct* p_mas = (MyAwesomeStruct*) malloc(sizeof(MyAwesomeStruct));

cbuf_get( cbuf, &mas );
cbuf_get( cbuf, p_mas );
```

Destroy your buffer:

```
cbuf_destroy( cbuf );
```
