all: encrypt decrypt
encrypt: encrypt.o io.o aes.o aesUtil.o field.o
	gcc -g -Wall -std=c99 encrypt.o io.o aes.o field.o aesUtil.o -o encrypt
decrypt: decrypt.o io.o aes.o aesUtil.o field.o
	gcc -g -Wall -std=c99 decrypt.o io.o aes.o field.o aesUtil.o -o decrypt 
encrypt.o: encrypt.c io.h aes.h field.h aesUtil.h
decrypt.o: decrypt.c io.h aes.h field.h aesUtil.h
io.o: io.c io.h field.h
aes.o: aes.c aes.h aesUtil.h
aesUtil.o: aesUtil.c aesUtil.h field.h
field.o: field.c field.h
clean:
	rm -f encrypt.o decrypt.o io.o aes.o field.o aesUtil.o encrypt decrypt
