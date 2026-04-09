/**
* This program is for the buffer. It can make, free, and append and extract bytes to a buffer
* @file buffer.c
* @author Jacob McLain
*/

#include "buffer.h"
#include <stdlib.h>
#include <string.h>

/** Initial capacity of the buffer. */
#define INITIAL_CAP 5

/**
* This function dynamically allocates space for an 
* instance of Buffer, initializes its fields to represent 
* an empty buffer and returns a pointer to the new buffer.
* @return b the buffer
*/
Buffer *makeBuffer()
{
    Buffer *b = malloc(sizeof(Buffer));
    if(!b) {
        return NULL;
    }
    b->len = 0;
    b->cap = INITIAL_CAP;
    b->pos = 0;
    b->data = malloc(b->cap * sizeof(byte));
    if(!b->data) { 
        free(b); 
        return NULL; 
    }
    return b;

}
/**
* This function frees all the memory 
* associated with the given buffer.
* @param buf pointer to a buffer
*/
void freeBuffer( Buffer *buf )
{
    if(!buf) {
        return;
    }
    free(buf->data);
    free(buf);
}
/**
* This function adds the given byte to 
* the end of the Buffer’s data array, 
* automatically reallocating a larger array if needed.
* @param buf the buffer
* @param val the value
*/
void appendByte( Buffer *buf, byte val )
{
    if(buf->len >= buf->cap) {
        buf->cap *= 2;
        byte *newList = realloc(buf->data, buf->cap * sizeof(byte));
        buf->data = newList;
    }
    buf->data[buf->len++] = val;
}
/**
* This function adds the given sequence of bytes to
* the end of the Buffer’s data array, automatically
* reallocating a larger array if needed. If the given
* sequence is large, growing the data array may
* require doubling its size more than once.
* @param buf the buffer
* @param seq the sequence of bytes
* @param n the number of bytes to be appended
*/
void appendBytes( Buffer *buf, void *seq, int n )
{
    byte *bytes = (byte *)seq;
    for(int i = 0; i < n; i++) {
        if (buf->len >= buf->cap) {
            buf->cap *= 2;
            byte *newList = realloc(buf->data, buf->cap * sizeof(byte));
            buf->data = newList;

        }
        buf->data[buf->len++] = bytes[i]; 
    }
}
/**
* This function is for accessing the buffer’s contents
* sequentially. It copies the byte at index pos to the
* given val pointer and moves pos ahead by one. The return
* value is for handling buffer overflow. The there is no
* byte at position pos in the buffer (because you have already
* extracted everything in the buffer), this function returns
* false and leaves the byte pointed to by val unmodified.
* @param buf the buffer
* @param val the value
* @return T or F based on if the byte was successfully extracted
*/
bool extractByte( Buffer *buf, byte *val )
{
    if(buf == NULL || val == NULL || buf->pos >= buf->len) {
        return false;
    }
    *val = buf->data[buf->pos++];
    return true;
}
/**
* This function is for accessing the buffer’s contents
* sequentially. It copies the next n bytes starting
* from index pos to the given seq array and moves pos
* ahead by n. The return value is for handling buffer
* overflow. If the next n bytes extend past the end of
* the Buffer, this function returns false and leaves the seq array unmodified.
* @param buf the buffer
* @param seq the sequence of bytes
* @param n the number of bytes to be extracted
* @return T or F based on if the bytes were successfully extracted
*/
bool extractBytes( Buffer *buf, void *seq, int n )
{
    if(!buf || !seq) {
        return false;
    }
    if(n < 0) {
        return false;
    }
    if(n > buf->len - buf->pos) {
        return false;
    }
    memcpy(seq, buf->data + buf->pos, n);
    buf->pos += n;
    return true;
}