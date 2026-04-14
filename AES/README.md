## AES (`encrypt.c`, `decrypt.c`)

A command-line file encryption and decryption tool written in C that implements the AES (Advanced Encryption Standard) block cipher.
This project includes a full AES implementation along with CLI utilities for encrypting and decrypting binary files using a 128-bit key.

### Features

- Full AES-128 block cipher implementation
- Command-line encryption and decryption tools
- Processes binary files in 16-byte blocks
- Supports file-based key input
- Validates input file sizes for correctness
- Encrypts/Decrypts by generating subkeys, then performing numerous matrix operations

### Compile 

```bash
make
```

### Usage

```bash
./encrypt [key-file] [input-file] [output-file]
./decrypt [key-file] [input-file] [output-file]
```
