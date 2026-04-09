/**
   @file aesUtil.h
   Header for utility functions used by AES.  Putting some of these
   functions in a separate component shortens aes.c and it lets us
   keep a lot of magic numbers out of the main AES implementation.
   @author dbsturgi
*/

#ifndef AESUTIL_H
#define AESUTIL_H

#include "field.h"


/** Number of bytes in an AES key or an AES block. */
#define BLOCK_SIZE 16

/** Numer of rows when a data block is arranged in a square. */
#define BLOCK_ROWS 4

/** Numer of columns when a data block is arranged in a square. */
#define BLOCK_COLS 4

/** Number of bytes in a word. */
#define WORD_SIZE 4

/** Number of rounds for 128-bit AES. */
#define ROUNDS 10

/** Mattrix of values used in the Mix Columns operation. */
extern const byte forwardMixMatrix[ BLOCK_ROWS ][ BLOCK_COLS ];

/** Mattrix of values used to invert the Mix Columns operation. */
extern const byte inverseMixMatrix[ BLOCK_ROWS ][ BLOCK_COLS ];
  
/**
   Return the sBox substitution value for a given byte value.
   @param v byte input value.
   @return substitution for the given byte.
*/
byte substBox( byte v );

/**
   Return the inverse sBox substitution value for a given byte value.
   @param v byte input value.
   @return inverse substitution for the given byte.
*/
byte invSubstBox( byte v );

/**
   Compute the g function, rearranging bytes from src and combining
   with a round constant for round r.
   @param dest Result of the g Function computation.
   @param src Input for the g Function computtation.
   @param r Number of the current round being computed.
*/
void gFunction( byte dest[ WORD_SIZE ], byte const src[ WORD_SIZE ], int r );

#endif
