
CC=gcc

all: admin

debug: 
	$(CC) -DDEBUG -o admin admin.c

clean:
	rm -f *.o
	rm -f admin



admin: admin.o 
	$(CC) -o admin admin.o 

admin.o: admin.c
	$(CC) -Wall -c  admin.c 
