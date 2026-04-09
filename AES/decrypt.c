/**
* The decrypt.c class takes an input of key-file, input-file, and output-file and decripts the binary and writes it to the output-file
* @file decrypt.c
* @author Jacob McLain
*/
#include <stdio.h>
#include <stdlib.h>
#include "aes.h"
#include "io.h"
#include "aesUtil.h"
#include "field.h"
/**
* takes an input of key-file, input-file, and output-file and decripts the binary and writes it to the output-file
* @param argc the amount of arguments in the command line
* @param argv the arguments in the command line as an array pointer
* @return whether or not the program exited successfully
*/
int main(int argc, char *argv[])
{
  if(argc != 4) {
    fprintf(stderr, "usage: decrypt <key-file> <input-file> <output-file>\n");
    exit(EXIT_FAILURE);
  }
  char *keyFileName = argv[1];
  char *inputFileName = argv[2];
  char *outputFileName = argv[3];
  int keySize;
  byte *key = readBinaryFile(keyFileName, &keySize);
  if(keySize != 16) {
    fprintf(stderr, "Bad key file: %s\n", keyFileName);
    exit(EXIT_FAILURE);
  }
  int cipherSize;
  byte *data = readBinaryFile(inputFileName, &cipherSize);
  if(cipherSize % 16 != 0) {
    fprintf(stderr, "Bad ciphertext file length: %s\n", inputFileName);
    exit(EXIT_FAILURE);
  }
  int num = cipherSize / BLOCK_SIZE;
  for(int i = 0; i < num; i++) {
    decryptBlock(data + (i * BLOCK_SIZE), key);
  }
  writeBinaryFile(outputFileName, data, cipherSize);
  free(key);
  free(data);
  return EXIT_SUCCESS;
}