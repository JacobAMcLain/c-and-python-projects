/**
* This class performs all the operations for binary data used in encrypt and decrypt
* @file aes.h
* @author Jacob McLain
*/
#ifndef AES_H
#define AES_H
#include "aesUtil.h"
/**
* This function fills in the subkey array 
* with subkeys for each round of AES, computed from the given key.
* @param subkey the subkey that is generated from key
* @param key the key that is used to generate the subkey
*/
void generateSubkeys( byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );
/**
* This function performs the addSubkey operation, 
* adding the given subkey (key) to the given data array. 
* @param data the 16-byte block of data
* @param key the key added to the given data array
*/
void addSubkey( byte data[ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );
/** 
* This function rearranges a block of 16 data values from
* the one-dimensional arrangement to the 4 × 4 arrangement.
* @param square the 16-byte block of data
* @param data the 16-byte block of data in a 2d form
*/
void blockToSquare( byte square[ BLOCK_ROWS ][ BLOCK_COLS ], byte const data[ BLOCK_SIZE ] );
/**
* This function rearranges a 4 × 4 arrangement of data values,
* returning them as a one-dimensional array of 16 values.
* @param data the bits in a line of size 16
* @param square the data in a square form
*/
void squareToBlock( byte data[ BLOCK_SIZE ], byte const square[ BLOCK_ROWS ][ BLOCK_COLS ] );
/** 
* This function performs the shiftRows operation
* on the given 4 × 4 square of values.
* @param square the square of byte values
*/
void shiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );
/** 
* This function performs the inverse shiftRows
* operation on the given 4 × 4 square of values. 
* @param square the square of byte values
*/
void unShiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );
/**
* This function performs the mixColumns operation on
* the given 4 × 4 square of values, multiplying each
* column by the matrix shown in the requirements section.
* @param square the square of byte values
*/
void mixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );
/** 
* This function performs the inverse of the mixColumns operation on the given 4 × 4 square of values,
* multiplying each column by the inverse of the mixColumns matrix shown in the requirements section.
* @param square the square of byte values
*/
void unMixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );
/**
* This function encrypts a 16-byte block of data using the given key.
* It generates the 11 subkeys from key, adds the first subkey,
* then performs the 10 rounds of operations needed to encrypt the block.
* @param data the 16-byte block of data
* @param key the key used to encrypt the data
*/
void encryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );
/**
* This function decrypts a 16-byte block of data using the given key. It generates
* the 11 subkeys from key, then performs the 10 rounds of inverse operations
* (and then an addSubkey) to decrypt the block.
* @param data the 16-byte block of data
* @param key the key used to decrypt the data
*/
void decryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );
#endif