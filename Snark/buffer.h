/**
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <stdbool.h>

/** Type used to represent a byte. */
typedef unsigned char byte;

typedef struct {
  /** Dynamically allocated sequence of bytes stored in this buffer. */
  byte *data;

  /** Number of bytes stored in this buffer. */
  int len;

  /** Capacity of the data array. */
  int cap;
  
  /** Current position in the buffer, to simplify processing the
      contents of the buffer from front to back. */
  int pos;
} Buffer;

/**
* This function dynamically allocates space for an instance of
* Buffer, initializes its fields to represent an empty
* buffer and returns a pointer to the new buffer.
*/
Buffer *makeBuffer();

/**
* This function frees all the memory
* associated with the given buffer.
*/
void freeBuffer( Buffer *buf );

/**
* This function adds the given byte to the end 
* of the Buffer’s data array, automatically
* reallocating a larger array if needed.
*/
void appendByte( Buffer *buf, byte val );

/**
* This function adds the given sequence of
* bytes to the end of the Buffer’s data array,
* automatically reallocating a larger array if
* needed. If the given sequence is large, growing
* the data array may require doubling its size more than once.
*/
void appendBytes( Buffer *buf, void *seq, int n );

/**
* This function is for accessing the buffer’s 
* contents sequentially. It copies the byte at
* index pos to the given val pointer and moves
* pos ahead by one. The return value is for handling
* buffer overflow. The there is no byte at position
* pos in the buffer (because you have already extracted
* everything in the buffer), this function returns
* false and leaves the byte pointed to by val unmodified.
*/
bool extractByte( Buffer *buf, byte *val );

/**
* This function is for accessing the buffer’s contents
* sequentially. It copies the next n bytes starting 
* from index pos to the given seq array and moves pos
* ahead by n. The return value is for handling buffer
* overflow. If the next n bytes extend past the end of
* the Buffer, this function returns false and leaves
* the seq array unmodified.
*/
bool extractBytes( Buffer *buf, void *seq, int n );


#endif
