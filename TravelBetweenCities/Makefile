travel: travel.o parklist.o citylist.o input.o
	gcc -g -Wall -std=c99 -D_GNU_SOURCE travel.o parklist.o citylist.o input.o -o travel -lm
travel.o: travel.c parklist.h citylist.h input.h
parklist.o: parklist.c parklist.h input.h
citylist.o: citylist.c citylist.h input.h
input.o: input.c input.h
clean:
	rm -f travel.o parklist.o citylist.o input.o travel