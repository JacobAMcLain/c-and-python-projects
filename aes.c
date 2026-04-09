/**
* This class performs all the operations for binary data used in encrypt and decrypt
* @file aes.c
* @author Jacob McLain
*/
#include <string.h>
#include <stdio.h>
#include "aes.h"
/**
* This function fills in the subkey array
* with subkeys for each round of AES, computed from the given key.
* @param subkey the subkey that is generated from key
* @param key the key that is used to generate the subkey
*/
void generateSubkeys( byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] ) {
  for(int i = 0; i < BLOCK_SIZE; i++) {
    subkey[0][i] = key[i];
  }
  byte words1[BLOCK_SIZE / BLOCK_COLS];
  byte words2[BLOCK_SIZE / BLOCK_COLS];
  byte words3[BLOCK_SIZE / BLOCK_COLS];
  byte words4[BLOCK_SIZE / BLOCK_COLS];
  for(int i = 0; i < BLOCK_SIZE; i++) {
    if(i / BLOCK_COLS == 0) {
      words1[i % BLOCK_COLS] = key[i];
    }
    else if(i / BLOCK_COLS == 1) {
      words2[i % BLOCK_COLS] = key[i];
    }
    else if(i / BLOCK_COLS == 2) {
      words3[i % BLOCK_COLS] = key[i];
    }
    else {
      words4[i % BLOCK_COLS] = key[i];
    }
  }
  for(int j = 0; j < ROUNDS; j++) {
    byte temp[BLOCK_COLS];
 
    gFunction( temp, words4, j + 1);
    byte tWords1[BLOCK_COLS];
    byte tWords2[BLOCK_COLS];
    byte tWords3[BLOCK_COLS];
    byte tWords4[BLOCK_COLS];
  
    for(int i = 0; i < BLOCK_COLS; i++) {
      tWords1[i] = fieldAdd(temp[i], words1[i]);
      tWords2[i] = fieldAdd(tWords1[i], words2[i]);
      tWords3[i] = fieldAdd(tWords2[i], words3[i]);
      tWords4[i] = fieldAdd(tWords3[i], words4[i]);
    }

    for(int i = 0; i < BLOCK_COLS; i++) {
      subkey[j+1][i] = tWords1[i];
      subkey[j+1][i + BLOCK_COLS] = tWords2[i];
      subkey[j+1][i + (BLOCK_COLS * 2)] = tWords3[i];
      subkey[j+1][i + (BLOCK_COLS * 3)] = tWords4[i];
    }

    memcpy(words1, tWords1, BLOCK_COLS);
    memcpy(words2, tWords2, BLOCK_COLS);
    memcpy(words3, tWords3, BLOCK_COLS);
    memcpy(words4, tWords4, BLOCK_COLS);
  }
}
/**
* This function performs the addSubkey operation,
* adding the given subkey (key) to the given data array.
* @param data the 16-byte block of data
* @param key the key added to the given data array
*/
void addSubkey( byte data[ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] ) {
  for(int i = 0; i < BLOCK_SIZE; i++) {
    data[i] ^= key[i];
  }
}
/** 
* This function rearranges a block of 16 data values from
* the one-dimensional arrangement to the 4 × 4 arrangement.
* @param square the 16-byte block of data
* @param data the 16-byte block of data in a 2d form
*/
void blockToSquare( byte square[ BLOCK_ROWS ][ BLOCK_COLS ], byte const data[ BLOCK_SIZE ] ) {
  int count = 0;
  for(int i = 0; i < BLOCK_ROWS; i++) {
    for(int j = 0; j < BLOCK_COLS; j++) {
      square[j][i] = data[count];
      count++;
    }
  }
}
/**
* This function rearranges a 4 × 4 arrangement of data values,
* returning them as a one-dimensional array of 16 values.
* @param data the bits in a line of size 16
* @param square the data in a square form
*/
void squareToBlock( byte data[ BLOCK_SIZE ], byte const square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  int count = 0;
  for(int i = 0; i < BLOCK_ROWS; i++) {
    for(int j = 0; j < BLOCK_COLS; j++) {
      data[count] = square[j][i];
      count++;
    }
  }
}
/** 
* This function performs the shiftRows operation
* on the given 4 × 4 square of values.
* @param square the square of byte values
*/
void shiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  byte temp[BLOCK_ROWS][BLOCK_COLS];
  memcpy(temp, square, BLOCK_SIZE * sizeof(byte));
  for(int i = 0; i < BLOCK_ROWS; i++)
  {
    for(int j = 0; j < BLOCK_COLS; j++)
    {
      square[i][j] = temp[i][(j+i) % BLOCK_COLS];
    }
  }
}

/** 
* This function performs the inverse shiftRows
* operation on the given 4 × 4 square of values.
* @param square the square of byte values
*/
void unShiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  byte temp[BLOCK_ROWS][BLOCK_COLS];
  memcpy(temp, square, BLOCK_SIZE * sizeof(byte));
  for(int i = 0; i < BLOCK_ROWS; i++)
  {
    for(int j = 0; j < BLOCK_COLS; j++)
    {
      square[i][j] = temp[i][(j-i+BLOCK_COLS) % BLOCK_COLS];
    }
  }
}

/**
* This function performs the mixColumns operation on 
* the given 4 × 4 square of values, multiplying each 
* column by the matrix shown in the requirements section.
* @param square the square of byte values
*/
void mixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  for(int i = 0; i < BLOCK_ROWS; i++) {
    byte column[BLOCK_ROWS];
    for(int j = 0; j < BLOCK_COLS; j++) {
      column[j] = square[j][i];
    }

    for(int m = 0; m < BLOCK_ROWS; m++) {
      byte value = 0;
      for(int k = 0; k < BLOCK_ROWS; k++) {
        value = fieldAdd(value, fieldMul(forwardMixMatrix[m][k], column[k]));
      }
      square[m][i] = value;
    }
  }
}

/** 
* This function performs the inverse of the mixColumns operation on the given 4 × 4 square of values,
* multiplying each column by the inverse of the mixColumns matrix shown in the requirements section.
* @param square the square of byte values
*/
void unMixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  for(int i = 0; i < BLOCK_ROWS; i++) {
    byte column[BLOCK_ROWS];
    for(int j = 0; j < BLOCK_COLS; j++) {
      column[j] = square[j][i];
    }

    for(int m = 0; m < BLOCK_ROWS; m++) {
      byte value = 0;
      for(int k = 0; k < BLOCK_ROWS; k++) {
        value = fieldAdd(value, fieldMul(inverseMixMatrix[m][k], column[k]));
      }
      square[m][i] = value;
    }
  }
}

/**
* This function encrypts a 16-byte block of data using the given key.
* It generates the 11 subkeys from key, adds the first subkey,
* then performs the 10 rounds of operations needed to encrypt the block.
* @param data the 16-byte block of data
* @param key the key used to encrypt the data
*/
void encryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] ) {
  byte subkeys[ROUNDS + 1][BLOCK_SIZE];
  byte square[BLOCK_ROWS][BLOCK_COLS];
  generateSubkeys(subkeys, key);
  addSubkey(data, subkeys[0]);
  
  
  
  for(int i = 1; i < ROUNDS; i++) {
    
    for(int j = 0; j < BLOCK_SIZE; j++) {
      data[j] = substBox(data[j]);
    }
    blockToSquare(square, data);
    shiftRows(square);
    mixColumns(square);
    squareToBlock(data, square);
    addSubkey(data, subkeys[i]);
  }

  for(int i = 0; i < BLOCK_SIZE; i++) {
    data[i] = substBox(data[i]);
  }  
  blockToSquare(square, data);
  shiftRows(square);
  squareToBlock(data, square);
  addSubkey(data, subkeys[ROUNDS]);
 
}

/**
* This function decrypts a 16-byte block of data using the given key. It generates
* the 11 subkeys from key, then performs the 10 rounds of inverse operations 
* (and then an addSubkey) to decrypt the block.
* @param data the 16-byte block of data
* @param key the key used to decrypt the data
*/
void decryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] ) {
  byte subkeys[ROUNDS + 1][BLOCK_SIZE];
  byte square[BLOCK_ROWS][BLOCK_COLS];
  generateSubkeys(subkeys, key);
  addSubkey(data, subkeys[ROUNDS]);
  for(int i = ROUNDS - 1; i > 0; i--) {
    blockToSquare(square, data);
    unShiftRows(square);
    squareToBlock(data, square);
    for(int j = 0; j < BLOCK_SIZE; j++) {
      data[j] = invSubstBox(data[j]);
    }
    addSubkey(data, subkeys[i]);
    blockToSquare(square, data);
    unMixColumns(square);
    squareToBlock(data, square);
  }
  blockToSquare(square, data);
  unShiftRows(square);
  squareToBlock(data, square);
  for(int i = 0; i < BLOCK_SIZE; i++) {
    data[i] = invSubstBox(data[i]);
  }  
  addSubkey(data, subkeys[0]);
}
