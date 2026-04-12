/**
* This is the error class. It sets and gets an error message
* @file error.h
* @author Jacob McLain
*/
#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

/**
* This copies the given string to dynamically allocated memory and sets the
* current error message. If the message parameter is NULL, this sets the current
* error message to NULL and frees any dynamically allocated memory for
* the previous error message (if there was one).
* @param message the message that is made the error message
* @return boolean T or F based on if the error message was set successfully
*/
bool setErrorMessage( char const *message );

/**
* This returns a pointer to the current error message string,
* or NULL if there is no current error message.
* @return errMessage the error message
*/
char const *getErrorMessage();

#endif
