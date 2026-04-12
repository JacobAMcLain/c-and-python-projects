/**
* This is the io class. It reads lines, reads from a file, and writes to a file
* @file io.c
* @author Jacob McLain
*/

#include "IO.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
char *readLine( FILE *fp ) {
    if(!fp) {
        return NULL;
    }
    int capacity = 62;
    int length = 0;
    char *ch = (char *)malloc(capacity * sizeof(char));
    int c;
    while((c = fgetc(fp)) != EOF && c != '\n') {
        if(length >= capacity) {
            capacity *= 2;
            ch = (char *)realloc(ch, capacity * sizeof(char));
        }
        ch[length++] = c;
    }
    if(length == 0 && c == EOF) {
        free(ch);
        return NULL;
    }
    ch[length] = '\0';
    return ch;
}

/**
* This function reads the entire contents of the file
* with the given name and returns it in a dynamically
* allocated Buffer. The function returns NULL if the
* file can’t be read successfully. The file could contain
* text or binary data, so the I/O should be done in binary mode.
* @param filename
* @return buf the buffer read from the file
*/
Buffer *readFile(char const  *filename) {

    if(!filename) return NULL;

    FILE *fp = fopen(filename, "rb");
    
    if(!fp) {
        return NULL;
    }

    if(fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }

    int fsize = ftell(fp);

    if(fsize < 0) {
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    Buffer *buf = malloc(sizeof(Buffer));
    if(!buf) { 
        fclose(fp);
        return NULL;
    }

    buf->len = fsize;
    buf->cap = buf->len > 0 ? buf->len : 1;
    buf->pos = 0;
    buf->data = malloc(buf->cap);

    if(!buf->data) {
        free(buf);
        fclose(fp);
        return NULL;
    }

    if(fread(buf->data, 1, buf->len, fp) != buf->len) {
        free(buf->data);
        free(buf);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return buf;

}

/**
* This function creates a file with the given name
* and writes the contents of the given buffer to the
* file. The Buffer could contain an arbitrary sequence
* of bytes, so I/O should be done in binary mode.
* @param filename the name of the file
* @param buf the buffer
* @return boolean T or F depending on if the file was wrote to successfully
*/
bool writeFile( char const *filename, Buffer *buf ) {

    if(!buf || !buf->data) {
        return false;
    }

    FILE *fp = fopen(filename, "wb");
    if(!fp) {
        return false;
    }

    size_t written = fwrite(buf->data, 1, buf->len, fp);
    fclose(fp);
    
    return written == buf->len;
}
