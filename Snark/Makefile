
snark: snark.o archive.o error.o io.o compress.o buffer.o
	gcc -g -Wall -std=c99 snark.o archive.o io.o compress.o buffer.o error.o -o snark
snark.o: snark.c archive.h error.h
archive.o: archive.c io.h compress.h error.h
io.o: io.c io.h buffer.h
compress.o: compress.c buffer.h
error.o: error.c
buffer.o: buffer.c
clean:
	rm -f snark.o archive.o io.o compress.o error.o buffer.o snark
