/*!
 * \file cbuf.h
 *
 * \brief Basic circular buffer, with the twist that objects/structures
 * are copied in and out of the buffer into the preexisting memory, thus
 * there is no concern of buffer overwrites voiding a retrieved buffer
 * pointer.
 *
 * What separates this, possibly, from other buffers is that it's based
 * around generic elements that you, the user, define.  Then it operates
 * on void pointers to take said elements and manage them.
 *
 * \legalese If you decide to use this code the following must be
 * observed:
 *
 * In the event that we are in physical proximity of one another
 * and you witness an event which may cause my demise, you must save me.
 * Likewise, if I am in a position in which I am dying, it is now your
 * responsibility to seek the appropriate treatment immediately. Lastly,
 * if you are capable of resuscitating me, you are likewise obligated
 * to perform the necessary actions required.
 *
 * \author Alex Baum
 * \date 2021-07-24
 */

#ifndef CBUF_H
#define CBUF_H

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/*!
 * \struct CBuf defines the circular buffer structure
 */
typedef struct
{
    char*  data;    //!< circular ring buffer data
    size_t len;     //!< the intended size of the buffer, in elements
    size_t elem_sz; //!< the size of an element, in bytes

    size_t head;    //!< current location of buffer head
    size_t tail;    //!< current location of buffer tail
} CBuf;


/*!
 * \brief cbuf_make initializes the circular buffer
 *
 * \param buf       instance of buffer to initialize
 * \param elem_sz   size of single buffer element
 * \param len       length of buffer to initialize
 *
 * \note Due to the design and being able to determine whether
 * or not the buffer is full, the largest numbers of items that
 * can be contained in the buffer are len - 1.  That is, for
 * a buffer size of 2, only 1 item will be able to be removed
 * before the buffer is reported as empty.
 */
extern CBuf* cbuf_make( size_t elem_sz, size_t len );

/*!
 * \brief cbuf_destroy takes down an instance of the circular buffer
 * including any freeing that needs to be done internally or externally.
 */
extern void cbuf_destroy( CBuf* buf );


/*!
 * \brief cbuf_capacity returns the capacity of the circular buffer in
 * elements
 * \return maximum number of data elements for buffer
 */
extern size_t cbuf_capacity( CBuf* buf );


/*!
 * \brief cbuf_size returns the current size of the buffer in elements
 * \return current size of buffer
 */
extern size_t cbuf_size( CBuf* buf );


/*!
 * \brief cbuf_add adds a new value to the circular buffer
 * \param data      data to add to the buffer
 */
extern void cbuf_add( CBuf* buf, void* data );


/*!
 * \brief cbuf_get gets the next piece of data from the buffer
 * \param data      data returned from the buffer
 * \return number of elements read
 */
extern size_t cbuf_get( CBuf* buf, void* data );


/*!
 * \brief cbuf_is_empty returns whether or not the circular buffer is empty
 * \return 0 if not empty, else 1
 */
extern uint8_t cbuf_is_empty( CBuf* buf );

/*!
 * \brief cbuf_is_full returns whether or not the circular buffer is full
 * \return 0 if not full, else 1
 */
extern uint8_t cbuf_is_full( CBuf* buf );

#ifdef __cplusplus
}
#endif

#endif // CBUF_H
