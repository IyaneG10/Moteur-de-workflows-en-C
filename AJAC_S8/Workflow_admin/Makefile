
CC=gcc

all: admin



clean:
	rm -f *.o



admin: admin.o 
	$(CC) -o admin admin.o 

admin.o: admin.c
	$(CC) -Wall -c  admin.c 
