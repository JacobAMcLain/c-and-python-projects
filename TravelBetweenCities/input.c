/**
 * This file reads input from a file and condenses it into a line so that the other source code can use it
 * @file input.c
 * @author Jacob McLain
 */
#include <stdlib.h>
#include <stdio.h>
#include "input.h"
/**
 * This function reads a single line of input from the given 
 * input stream and returns it as a string inside a block of
 * dynamically allocated memory. Since this function returns a pointer 
 * to dynamically allocated memory,
 * some other code will be responsible for eventually freeing that
 * memory (to avoid a memory leak).
 * @param fp the file that is being read from
 * @return ch returns a pointer to the line
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
