/**
 */

#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdbool.h>
#include "buffer.h"

/** Number of different codes a byte can represent. */
#define BYTE_CODES 256

/** Maximum size of a block. */
#define BLOCK_SIZE_LIMIT 16384

// Normally, I'd hide these two structs in the implementation file,
// rather than exposing them in the header.  I put them here so some
// of the internal functions could be more easily testable.

/** Rule for replacing a pair of bytes with a single-byte code. */
typedef struct {
  /** One-byte code we're replacing the two-byte sequence with. */
  byte code;
  
  /** Two-byte sequence we're replacing. */
  byte first, second;
} Replacement;

/** Representation for a block being compressed or decompressed. */
typedef struct {
  /** Sequence of bytes in this block. */
  byte data[ BLOCK_SIZE_LIMIT ];

  /** Length of the sequence of bytes. */
  unsigned short len;

  /** Sequence of replacement rules. */
  Replacement rlist[ BYTE_CODES - 1 ];

  /** Number of replaement rules for this block. */
  unsigned char rcount;
} Block;

/**
* This function writes out the contents of the given
* block by adding it to the end of the given buffer.
* You should write the block in the format described
* at the end of the “Block Compression” section above.
* Compressing a file could require multiple blocks,
* so this function should add the compressed representation
* of block to the end of the buffer, after anything that was previously in the buffer.
*/
void serializeBlock( Block *block, Buffer *buf );

/**
* This function reads the contents of a block from
* the given buffer. If successful, it should return
* true. If the given buffer doesn’t contain a valid 
* block (e.g., if the end of the buffer is reached 
* before an entire block can be read), it should return
* false. This function should use the extractByte() and
* extractBytes() functions to access the contents of the
* buffer consecutively. A file might be represented as
* multiple compressed blocks, so this function should
* continue processing the contents of the buffer, picking
* up from where a previous call to deserializeBlock() may have left off.
*/
bool deserializeBlock( Block *block, Buffer *buf );

/**
* This function attempts to compress the contents
* of the given block using the technique described 
* under “Block Compression” above. It should repeatedly
* select the most common byte pair in the block and 
* replace it with the lowest-numbered available byte,
* until there are no more available bytes or the most 
* common byte pair has fewer than three occurrences.
*/
void compressBlock( Block *block );

/**
* This function reverses the compressBlock operation,
* returning true if successful or false if there’s an
* error in how the block is compressed (e.g., if the 
* block would be too large after decompression). This
* function should undo each byte pair replacement in 
* the block’s rule list, working from the end of the 
* list to the start.
*/
bool uncompressBlock( Block *block );

/**
* This function compresses a file stored in the given
* src buffer, returning a new buffer containing a 
* compressed representation of the file. The file
* may need to be compressed as more than one buffer.
* If so, then the returned buffer will contain 
* encodings of each block, stored consecutively.
*/
Buffer *compressData( Buffer *src );

/**
* This function uncompresses the contents of the given
* source buffer and returns it in a new destination 
* buffer. If there is an error decompressing the
* given buffer, it returns NULL.
*/
Buffer *uncompressData( Buffer *src );

#endif
