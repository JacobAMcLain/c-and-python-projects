/**
*  Header for utility functions used by AES.
*  @file aesUtil.h
*  @author Jacob McLain
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

/** Matrix of values used in the Mix Columns operation. */
extern const byte forwardMixMatrix[ BLOCK_ROWS ][ BLOCK_COLS ];

/** Matrix of values used to invert the Mix Columns operation. */
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
   @param src Input for the g Function computation.
   @param r Number of the current round being computed.
*/
void gFunction( byte dest[ WORD_SIZE ], byte const src[ WORD_SIZE ], int r );

#endif
