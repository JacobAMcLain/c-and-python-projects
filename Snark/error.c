/**
* This is the error class. It sets and gets an error message
* @file error.c 
* @author Jacob McLain
*/

#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** The error message */
static char *errMessage = NULL;
/**
* This copies the given string to dynamically allocated memory and sets the
* current error message. If the message parameter is NULL, this sets the current
* error message to NULL and frees any dynamically allocated memory for
* the previous error message (if there was one).
* @param message the message that is made the error message
* @return boolean T or F based on if the error message was set successfully
*/
bool setErrorMessage( char const *message ) {
    if(message == NULL) {
      free(errMessage);
      errMessage = NULL;
      return false;
    }
    char *copy = malloc(strlen(message) + 1);
    if(!copy) {
      return false;
    }
    strcpy(copy, message);
    free(errMessage);
    errMessage = copy; 
    return true;
}

/**
* This returns a pointer to the current error message string,
* or NULL if there is no current error message.
* @return errMessage the error message
*/
char const *getErrorMessage() {
    return errMessage;
}