CC=gcc
# visualizzo tutti i warning, con '-W' li disabilito
CFLAGS=-Wall -std=c11


all: main

main.o: main.c stampa.h
	$(CC) $(CFLAGS) -c main.c

stampa.o: stampa.c stampa.h
	$(CC) $(CFLAGS) -c stampa.c


main: main.o stampa.o
	$(CC) $(CFLAGS) -o main main.o stampa.o

clean:
	rm -rf *.o


