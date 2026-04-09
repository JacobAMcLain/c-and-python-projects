/**
 */

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

/**
* The readLine() function reads a line of text from the given
* stream and returns it as a null-terminated, dynamically 
* allocated string. If there are no more lines of input in
* the given file, then this function should return NULL to
* indicate that the end-of-file has been reached.
*/
char *readLine( FILE *fp );

/**
* This function reads the entire contents of the file
* with the given name and returns it in a dynamically
* allocated Buffer. The function returns NULL if the
* file can’t be read successfully. The file could contain
* text or binary data, so the I/O should be done in binary mode.
*/
Buffer *readFile( char const *filename );

/**
* This function creates a file with the given name
* and writes the contents of the given buffer to the
* file. The Buffer could contain an arbitrary sequence
* of bytes, so I/O should be done in binary mode.
*/
bool writeFile( char const *filename, Buffer *buf );


#endif
