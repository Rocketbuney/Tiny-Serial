CC = gcc
CFLAGS = -std=c89 -Wall -pedantic

default: main.o

main.o: tiny-serial.c
	$(CC) $(CFLAGS) tiny-serial.c -o tiny-serial

clean:
	rm tiny-serial
