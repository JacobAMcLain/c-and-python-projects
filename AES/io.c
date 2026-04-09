/**
* This file reads and writes bytes to and from files
* @file io.c
* @author Jacob McLain
*/
#include <stdio.h>
#include <stdlib.h>
#include "aes.h"
/** 
* This function reads the contents of the binary file with the given name. 
* It returns a pointer to a dynamically allocated array of bytes containing the
* entire file contents. The size parameter is an integer that’s passed by reference
* to the function. The function fills in this integer with the total size of the
* file (i.e., how many bytes are in the returned array). 
* @param filename the name of the file
* @param size the size of the file
* @return an array of bytes read from the file
*/
byte *readBinaryFile( char const *filename, int *size )
{
  FILE *fp;
  if ( ( fp = fopen( filename, "rb" ) ) == NULL ) {
    fprintf( stderr, "Can't open file: %s\n", filename );
    exit( EXIT_FAILURE );
  }
  fseek(fp, 0, SEEK_END);
  long fSize = ftell(fp);
  rewind(fp);
  byte *arr = (byte *)malloc(fSize * sizeof(byte));
  size_t reader = fread(arr, 1, fSize, fp);
  *size = (int) fSize;
  fclose(fp);
  return arr;
}
/** 
* This function writes the contents of the given data array (in binary) to the file with the 
* given name. The size parameter says how many bytes are contained in the data array.
* @param filename the name of the file
* @param data the data read from the file
* @param size the size of the file
*/
void writeBinaryFile( char const *filename, byte *data, int size ) 
{
  FILE *fp;
  if ( ( fp = fopen( filename, "wb" ) ) == NULL ) {
    fprintf( stderr, "Can't open file: %s\n", filename );
    exit( EXIT_FAILURE );
  }
  fwrite(data, 1, size, fp);
  fclose(fp);
}