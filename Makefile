CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS = 

dine: dine.o dine.h 
	$(LD) $(LDFLAGS) -o dine dine.o -lpthread -lrt

dine.o: dine.c util.c
	$(CC) $(CFLAGS) -c -o dine.o dine.c -lpthread -lrt

run: dine
	./dine