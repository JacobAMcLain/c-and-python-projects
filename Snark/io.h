/**
* This is the io class. It reads lines, reads from a file, and writes to a file
* @file io.h
* @author Jacob McLain
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
* @param fp the file
* @return ch a string that is read from the line
*/
char *readLine( FILE *fp );

/**
* This function reads the entire contents of the file
* with the given name and returns it in a dynamically
* allocated Buffer. The function returns NULL if the
* file can’t be read successfully. The file could contain
* text or binary data, so the I/O should be done in binary mode.
* @param filename
* @return buf the buffer read from the file
*/
Buffer *readFile( char const *filename );

/**
* This function creates a file with the given name
* and writes the contents of the given buffer to the
* file. The Buffer could contain an arbitrary sequence
* of bytes, so I/O should be done in binary mode.
* @param filename the name of the file
* @param buf the buffer
* @return boolean T or F depending on if the file was wrote to successfully
*/
bool writeFile( char const *filename, Buffer *buf );


#endif
