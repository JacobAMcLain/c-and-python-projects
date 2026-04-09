/**
* This class uses Galois Field to Add/Subtract/Multiply Bytes
* @file field.c
* @author Jacob McLain
*/
#include "field.h"
#include <stddef.h>

/**
* This function performs the addition operation in the 8-bit Galois field used by AES. It adds a and b and returns the result.
* @param a byte being added to b
* @param b byte begin added to a
* @return a added to b
*/
byte fieldAdd( byte a, byte b ) {
  return a ^ b;
}
/**
* This function performs the subtraction operation in the 8-bit Galois field used by AES. It subtracts b from a and returns the result.
* @param a byte begin subtracted from b
* @param b byte begin subtracted from a
* @return a subtracted by b
*/
byte fieldSub( byte a, byte b ) {
  return a ^ b;
}
/**
* This function performs the multiplication operation in the 8-bit Galois field used by AES. It multiplies a and b and returns the result.
* @param a byte being multiplied by b
* @param b byte being multiplied by a
* @return the product of a and b
*/
byte fieldMul( byte a, byte b ) {
  int bigNum = 0;
  for(int i = 0; i < 8; i++) {
    if((b & 1) == 1) {
      bigNum ^= a << i;
    }
    b = b >> 1;
  }
  int count = 0;
  while(bigNum >= 0x100) {
    count = firstOneOccurance(bigNum);
    bigNum = bigNum ^ (0x11B << (count - 8));
  }
  return (byte) bigNum;
}
/**
* returns the first occurance of one in a int (binary)
* @param num the number being accessed
* @return a number representing the first occurrance of 1
*/
int firstOneOccurance(int num) {
  for(int i = 31; i >= 0; i--) {
    if(((num >> i) & 1) == 1) {
      return i;
    }
  }
  return -1; 
}