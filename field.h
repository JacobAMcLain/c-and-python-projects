/**
* This class uses Galois Field to Add/Subtract/Multiply Bytes
* @file field.h
* @author Jacob McLain
*/
#ifndef FIELD_H
#define FIELD_H

/** Type used for our field, an unsigned byte. */
typedef unsigned char byte;

/** Number of bits in a byte. */
#define BBITS 8

/**
* This function performs the addition operation in the 8-bit Galois field used by AES. It adds a and b and returns the result.
* @param a byte being added to b
* @param b byte begin added to a
* @return a added to b
*/
byte fieldAdd( byte a, byte b );

/**
* This function performs the subtraction operation in the 8-bit Galois field used by AES. It subtracts b from a and returns the result.
* @param a byte begin subtracted from b
* @param b byte begin subtracted from a
* @return a subtracted by b
*/
byte fieldSub( byte a, byte b );

/**
* This function performs the multiplication operation in the 8-bit Galois field used by AES. It multiplies a and b and returns the result.
* @param a byte being multiplied by b
* @param b byte being multiplied by a
* @return the product of a and b
*/
byte fieldMul( byte a, byte b );

/**
* returns the first occurance of one in a short (binary)
* @param num the num being accessed
*/
int firstOneOccurance(int num);
#endif
