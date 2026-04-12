/**
 * This file reads input from a file and condenses it into a line so that the other source code can use it
 * @file input.h
 * @author Jacob McLain
 */
/**
 * This function reads a single line of input from the given 
 * input stream and returns it as a string inside a block of
 * dynamically allocated memory. Since
 * this function returns a pointer to dynamically allocated memory,
 * some other code will be responsible for eventually freeing that
 * memory (to avoid a memory leak).
 * @param fp the file that is being read from
 * @return ch returns a pointer to the line
 */
char *readLine( FILE *fp );
